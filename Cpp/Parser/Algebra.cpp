#include "../../H/Parser/Algebra.h"
#include "../../H/UI/Safe.h"
#include "../../H/Parser/PostProsessor.h"

#include <cmath>

bool Optimized = false;
long long Inlined_Function_Count = 0;

void Algebra::Factory() {
	for (int i = 0; i < Input->size(); i++) {
		Function_Inliner(Input->at(i), i);
	}
	for (int i = 0; i < Input->size(); i++) {
		Inline_Variables(i);
		Set_Coefficient_Value(i);
		Prosess_Return(Input->at(i), i);
		//Prosess_Paranthesis(Input->at(i));
		Combine_Scattered(Input->at(i));
		Set_Defining_Value(i);
		Reset_Defining_Value(i);
		Un_Wrap_Parenthesis(Input->at(i));
	}
	for (int i = 0; i < Input->size(); i++)
		Clean_Inlined(i); 
	for (auto& i : *Input)
		Operate_Numbers_As_Constants(i);
	for (auto& i : *Input) {
		Operate_Coefficient_Constants(i);
		Operate_Distant_Coefficients(i);
		Reduce_Operator_Operations(i);
	}
	for (auto& i : *Input) {
		//Fix_Order_Into_Real_Operator(i);
		Fix_Coefficient_Into_Real_Operator(i);
	}
	Clean_Unused();
}

vector<Node*> Linearise(Node* ast, bool Include_Operator = false)
{
		vector<Node*> Result;
		if (ast->is(OPERATOR_NODE) || ast->is(CONDITION_OPERATOR_NODE) || ast->is(BIT_OPERATOR_NODE) || ast->is(ASSIGN_OPERATOR_NODE) || ast->is(ARRAY_NODE)) {
			vector<Node*> left = Linearise(ast->Left);
			Result.insert(Result.end(), left.begin(), left.end());

			Result.push_back(ast);

			vector<Node*> right = Linearise(ast->Right);
			Result.insert(Result.end(), right.begin(), right.end());
		}
		else if (ast->is(PREFIX_NODE)) {
			vector<Node*> right = Linearise(ast->Right);
			Result.insert(Result.end(), right.begin(), right.end());
		}
		else if (ast->is(POSTFIX_NODE) || ast->is(NODE_CASTER)) {
			vector<Node*> left = Linearise(ast->Left);
			Result.insert(Result.end(), left.begin(), left.end());
		}
		else if (ast->is(CONTENT_NODE)) {
			vector<Node*> childs;
			for (auto c : ast->Childs) {
				vector<Node*> tmp = Linearise(c);
				childs.insert(childs.end(), tmp.begin(), tmp.end());
			}
			Result.insert(Result.end(), childs.begin(), childs.end());
		}
		else if (ast->Has({ IF_NODE, ELSE_IF_NODE, ELSE_NODE, WHILE_NODE })) {
			vector<Node*> childs;
			for (auto c : ast->Parameters) {
				vector<Node*> tmp = Linearise(c);
				childs.insert(childs.end(), tmp.begin(), tmp.end());
			}
			for (auto c : ast->Childs) {
				vector<Node*> tmp = Linearise(c);
				childs.insert(childs.end(), tmp.begin(), tmp.end());
			}
			Result.insert(Result.end(), childs.begin(), childs.end());
		}
		else if (ast->is(CALL_NODE)) {
			vector<Node*> childs;
			for (auto c : ast->Parameters) {
				vector<Node*> tmp = Linearise(c);
				childs.insert(childs.end(), tmp.begin(), tmp.end());
			}
			Result.insert(Result.end(), childs.begin(), childs.end());

			Result.push_back(ast);
		}
		else if (ast->Name == "return" && ast->Right != nullptr) {
			for (auto c : Linearise(ast->Right))
				Result.push_back(c);
		}
		else
			Result.push_back(ast);

	for (int i = 0; i < Result.size(); i++) {
		if (Include_Operator && Result[i]->Has({ ASSIGN_OPERATOR_NODE, CONDITION_OPERATOR_NODE, BIT_OPERATOR_NODE, LOGICAL_OPERATOR_NODE, ARRAY_NODE }))
			Result.erase(Result.begin() + i);
	}

	return Result;
}

void Algebra::Set_Return_To_Jump(Node* n, Node* Return_Value, Node* end, Node* Context)
{
	if (n->Name == "return") {
		if (Context != nullptr && Context->Get_Context_As("return", Context) != nullptr) {
			return;
		}
		else {
			Node* Return_Paranthesis = new Node(CONTENT_NODE, n->Location);
			Return_Paranthesis->Paranthesis_Type = '{';
			Return_Paranthesis->Name = "Paranthesis";

			if (Return_Value) {
				Node* Assign = new Node(OPERATOR_NODE, n->Location);
				Assign->Name = "=";

				Node* Left = Return_Value->Copy_Node(Return_Value, n->Scope);
				Left->Type = OBJECT_NODE;
				Left->Context = Assign;

				Node* Right = n->Copy_Node(n->Right, n->Scope);
				Right->Context = Assign;

				Assign->Left = Left;
				Assign->Right = Right;

				Return_Paranthesis->Childs.push_back(Assign);
			}
			Node* Return = new Node(FLOW_NODE, n->Location);
			Return->Name = "jump";
			Return->Right = end;

			Return_Paranthesis->Childs.push_back(Return);

			*n = *Return_Paranthesis;
		}
	}
}

void Algebra::Function_Inliner(Node* c, int i)
{
	if (!c->is(CALL_NODE) || c->Function_Ptr || c->is("import") != -1)
		return;

	//copy the nodes to a safe heaven
	vector<Node*> Parameters;
	vector<Node*> Defined;
	vector<Node*> Childs;

	for (auto i : c->Function_Implementation->Childs) {
		Childs.push_back(c->Copy_Node(i, c->Scope));
	}

	//give the parameters a new name;
	for (auto i : c->Function_Implementation->Parameters) {
		Node* tmp = c->Copy_Node(i, c->Scope);
		tmp->Name += "_" + to_string(Inlined_Function_Count);
		Parameters.push_back(tmp);
	}

	//generate the end_of_function_label
	Node* End_of_Function_Label = new Node(LABEL_NODE, c->Location);
	End_of_Function_Label->Name = ".Return_Here_" + to_string(Inlined_Function_Count);

	Childs.push_back(End_of_Function_Label);

	Node* Return_Value = nullptr;
	if (c->Context) {
		Return_Value = new Node(OBJECT_DEFINTION_NODE, c->Location);
		Return_Value->Name = ".Return_Value" + to_string(Inlined_Function_Count);
	}

	//go thr�ugh all the children and update the names
	//the defined also have the parameters so only here we need to go through all th childrens.
	for (auto i : c->Function_Implementation->Defined) {
		Node* tmp = c->Copy_Node(i, c->Scope);
		string New_Name = tmp->Name + "_" + to_string(Inlined_Function_Count);

		for (auto j : Childs) {
			for (auto k : j->Get_all({PARAMETER_NODE, OBJECT_DEFINTION_NODE, OBJECT_NODE, FLOW_NODE})) {
				if (k->Name == i->Name)
					k->Name = New_Name;	

				//replace all the return statement with a jump to a end label command
				Set_Return_To_Jump(k, Return_Value, End_of_Function_Label, c->Context);
			}
		}

		tmp->Name = New_Name;
		Defined.push_back(tmp);
	}

	//first anchor the parameters to setted with the value corresponding at the callers parameters.
	for (int i = 0; i < Parameters.size(); i++) {
		Node* Left_Side = Parameters[i]->Copy_Node(Parameters[i], Parameters[i]->Scope);
		
		Node* Right_Side = c->Copy_Node(c->Parameters[i], c->Scope);

		Node* Operator = new Node(ASSIGN_OPERATOR_NODE, c->Parameters[i]->Location);
		Operator->Name = "=";
		Operator->Left = Left_Side;
		Operator->Right = Right_Side;
		Operator->Scope = c->Scope;

		Left_Side->Context = Operator;
		Right_Side->Context = Operator;

		Childs.insert(Childs.begin(), Operator);
	}

	Node* Scope = c->Scope;

	c->Function_Implementation->Calling_Count--;
	Input->insert(Input->begin() + i, Childs.begin(), Childs.end());
	c->Scope->Defined.insert(c->Scope->Defined.end(), Defined.begin(), Defined.end());

	if (Return_Value)
		*c = *Return_Value;
	else
		Input->erase(Input->begin() + Childs.size() + i);

	Inlined_Function_Count++;
	Optimized = true;

	Scope->Update_Defined_Stack_Offsets();
	Scope->Update_Stack_Space_Size(Scope);
}



vector<Node*> Algebra::Get_all(Node* n, int f)
{
	vector<Node*> Result;
	if (n->Left != nullptr) {
		vector<Node*> left = Get_all(n->Left, f);
		Result.insert(Result.end(), left.begin(), left.end());
	}
	if (n->Right != nullptr) {
		vector<Node*> right = Get_all(n->Right, f);
		Result.insert(Result.end(), right.begin(), right.end());
	}
	if (n->Succsessor != nullptr) {
		vector<Node*> Succsessors = Get_all(n->Succsessor, f);
		Result.insert(Result.end(), Succsessors.begin(), Succsessors.end());
	}
	if (n->Predecessor != nullptr) {
		vector<Node*> Predecessors = Get_all(n->Predecessor, f);
		Result.insert(Result.end(), Predecessors.begin(), Predecessors.end());
	}
	if (n->Fetcher != nullptr) {
		vector<Node*> Fetchers = Get_all(n->Fetcher, f);
		Result.insert(Result.end(), Fetchers.begin(), Fetchers.end());
	}
	for (Node* i : n->Header) {
		vector<Node*> Headers = Get_all(i, f);
		Result.insert(Result.end(), Headers.begin(), Headers.end());
	}
	for (Node* i : n->Childs) {
		vector<Node*> childs = Get_all(i, f);
		Result.insert(Result.end(), childs.begin(), childs.end());
	}
	for (Node* i : n->Parameters) {
		vector<Node*> childs = Get_all(i, f);
		Result.insert(Result.end(), childs.begin(), childs.end());
	}
	for (Node* i : n->Defined) {
		vector<Node*> childs = Get_all(i, f);
		Result.insert(Result.end(), childs.begin(), childs.end());
	}

	if (n->is(f))
		Result.push_back(n);

	return Result;
}

void Algebra::Prosess_Return(Node* n, int i)
{
	if (n->Name != "return")
		return;
	if (n->Right == nullptr)
		return;
	vector<Node*> tmp = { n->Right };
	Algebra a(n, &tmp);

	if (tmp.size() == 1)
		n->Right = tmp.back();
	else {
		Input->erase(Input->begin() + i);
		Input->insert(Input->begin() + i, tmp.begin(), tmp.end());
	}
}

void Algebra::Prosess_Call_Parameters(Node* n)
{
}

void Algebra::Prosess_Paranthesis(Node* n)
{
	if (!n->is(CONTENT_NODE))
		return;
	Algebra a(n, &n->Childs);
}

void Algebra::Inline_Variables(int i)
{
	//<summary>
	//finds a math equatin and tryes to inline the used variables set Values.
	//</summary>

	vector<Node*> Linear_Ast;
	if (Input->at(i)->is(OPERATOR_NODE) || Input->at(i)->is(ASSIGN_OPERATOR_NODE) || Input->at(i)->is(BIT_OPERATOR_NODE)) {
		if (Input->at(i)->Name != "=")
			Linear_Ast = Linearise(Input->at(i));
		else {
			if (Input->at(i)->is("ptr") != -1)
				return;
			Linear_Ast = Linearise(Input->at(i)->Right);
		}
	}
	else {
		return;
	}

	for (int j = 0; j < Linear_Ast.size(); j++) {
		if (Linear_Ast[j]->is(CALL_NODE)) {
			for (auto k : Linear_Ast[j]->Parameters) {
				vector<Node*> tmp = Linearise(k);
				Linear_Ast.insert(Linear_Ast.end(), tmp.begin(), tmp.end());
			}		
		}
	}

	for (Node* n : Linear_Ast) {
		if (n->is(CALL_NODE))
			continue;
		Node* d = Parent->Find(n->Name, Parent, OBJECT_DEFINTION_NODE);
		//if this is nullptr is means it is defined outside this scope.
		if (d != nullptr)
			if (d->Current_Value != nullptr) {
				//set right current coefficient value
				//n = -1
				//a = -n
				//Node* represents the -1 and n on this example
				//Node* n is same as the -n variable on example
				if (d->Current_Value->Expiring_Index >= i) {
					//do unwrap manuever if content is optimized into singlurar content: (1 + 2) -> (3)
					if (d->Current_Value->Var->is(CONTENT_NODE))
						if (!d->Current_Value->Var->Childs[0]->is(OPERATOR_NODE) && !d->Current_Value->Var->Childs[0]->is(ASSIGN_OPERATOR_NODE) && !d->Current_Value->Var->Childs[0]->is(CONDITION_OPERATOR_NODE) && !d->Current_Value->Var->Childs[0]->is(BIT_OPERATOR_NODE))
							*d->Current_Value->Var = *d->Current_Value->Var->Childs[0];
					//d->Current_Value->Var->Coefficient *= n->Coefficient;
					d->Current_Value->Var->Context = n->Context;
					d->Current_Value->Var->Scope = n->Scope;
					*n = *d->Current_Value->Var;
					d->Inlined = true;
					//maybe this is useless:
					n->Inlined = false;
					Optimized = true;
				}
			}
	}

}

void Algebra::Reduce_Operator_Operations(Node* n)
{
	if (!n->is(OPERATOR_NODE) && !n->is(CONDITION_OPERATOR_NODE) && !n->is(ASSIGN_OPERATOR_NODE))
		return;
	
	vector<Node*> Variables = Linearise(n);			
	// 2b^2 + b = b^2 + b
	// 2b^2 + b - b = b^2
	// 2b^2 - b^2 = 0
	// b^2 = 0

	// 2b^2 + b < -b^2 - b
	// 2b^2 + b^2 + b + b < 0
	// 3b^2 + 2b < 0

	//2b^2 / b < -b^2 * b
	//2b^2 / b < -b^3 
	//now go through one side and try to delete one coefficient amount of that variable on both sides.
	for (Node* v : Variables) {
		if (v->is(OBJECT_NODE) || v->is(OBJECT_DEFINTION_NODE)) {
			if (v->Coefficient == 0)
				continue;
			//TODO: make a fucking flag for those Karens that want to crash theyre exe, by dividing by 0.
			//if (v->Holder->Name == "/")	//stuff can go wrong if the v value is 0
			//	continue;
			for (Node* other : Variables) {
				if (v == other)
					continue;
				if (v->Name != other->Name)
					continue;
				if (other->Coefficient == 0)
					continue;
				if ((v->Fetcher != nullptr && other->Fetcher != nullptr) && v->Fetcher->Name != other->Fetcher->Name)
					continue;
				if (v->Context->is(ASSIGN_OPERATOR_NODE) || other->Context->is(ASSIGN_OPERATOR_NODE))
					continue;
				//decide wich one is in wich side
				Node* l = v;
				Node* r = other;
				if (v->Context->Right == v && other->Context->Left == other) {
					r = v;
					l = other;
				}
				if (v->Context->Name == "+" || v->Context->Name == "-") {
					if (other->Order != v->Order)
						continue;
					//here it affects the coefficient of other
					l->Coefficient -= r->Coefficient;
					r->Coefficient = 0;
					goto Next;
				}
				/*else if (v->Holder->Name == "*" && l == v->Holder->Left && r == v->Holder->Right) {
					//here it affects the order of other
					l->Order += r->Order;
					l->Coefficient *= r->Coefficient;
					r->Coefficient = 0;
					goto Next;
				}
				else if (v->Holder->Name == "/" && l == v->Holder->Left && r == v->Holder->Right) {
					//here it affects the order of other
					r->Order -= l->Order;
					r->Coefficient /= l->Coefficient;
					l->Coefficient = 0;
					goto Next;
				}*/
			}
		}
	Next:;
	}

}

void Algebra::Clean_Inlined(int i)
{
	if (!Input->at(i)->is(OPERATOR_NODE) && !Input->at(i)->is(ASSIGN_OPERATOR_NODE))
		return;
	if (Input->at(i)->Name != "=")
		return;
	Node* l = Input->at(i)->Get_Most_Left();
	if (Parent->Find(l->Name, l->Get_Most_Left()->Get_Right_Parent())->Inlined == false)
		return;

	//!!! MUST COMBINE THE CALLATION AND THE CLASS FETCHER!!!
	vector<Node*> Calls = Input->at(i)->Has(CALL_NODE);
	Optimized = true;

	Input->erase(Input->begin() + i);

	if (Calls.size() > 0) {
		//put the function callations into the deleted operations location else.
		Input->insert(Input->begin() + i, Calls.begin(), Calls.end());
	}

	if (i < Input->size())
		Clean_Inlined(i);
		
	return;
}

void Algebra::Set_Defining_Value(int i)
{
	//x = 12 + a
	//y = x * 2
	//x = 12
	//z = x * 4
	if (!Input->at(i)->is(ASSIGN_OPERATOR_NODE))
		return;
	//ignore arrays
	if (Input->at(i)->Left->is(ARRAY_NODE))
		return;

	if (Input->at(i)->Right->Get_All("ptr") != Input->at(i)->Left->Get_All("ptr"))
		return;

	//callations sould not be inlined because theyre return value may vary.
	for (auto j : Linearise(Input->at(i)->Right))
		if (j->is(CALL_NODE))
			return;
	

	//if the right side is a operator wrap it in a parenthesis just because the '-' prefix!!
	Node* right = Input->at(i)->Right;

	if (Input->at(i)->Right->is(OPERATOR_NODE) || Input->at(i)->Right->is(CONDITION_OPERATOR_NODE) || Input->at(i)->Right->is(BIT_OPERATOR_NODE)) {
		//a = 1+2
		//b = a * 3 --> b = (1+2) *3;		maintain the math order
		right = new Node(CONTENT_NODE, right->Location);
		right->Paranthesis_Type = '(';
		right->Childs.push_back(Input->at(i)->Right);
		right->Scope = Input->at(i);
		right->Context = Input->at(i)->Right;
	}
	//give the defining node the current set-val.
	//this wont work with array offsets, because this doesnt save the current offsetter value to check later on.
	Variable_Descriptor* description = new Variable_Descriptor(right, i, *Input);
	Parent->Find(Input->at(i)->Left->Name, Input->at(i)->Left)->Current_Value = description;

	return;
}

void Algebra::Set_Coefficient_Value(int i)
{
	//a = x * 2
	//b = x * 2 + a
	if (!Input->at(i)->is(OPERATOR_NODE) && !Input->at(i)->is(CONDITION_OPERATOR_NODE) && !Input->at(i)->is(ASSIGN_OPERATOR_NODE))
		return;
	//return;	//depricated
	vector<Node*> linear_ast = Linearise(Input->at(i), true);

	Node* Coefficient = nullptr;
	Node* Variable = nullptr;
	Node* Operator = nullptr;

	//now try to pair these
	for (int j = 0; j < linear_ast.size(); j++) {
		if (linear_ast[j]->is(NUMBER_NODE))
			Coefficient = linear_ast[j];
		else if (linear_ast[j]->is(OBJECT_NODE) || linear_ast[j]->is(PARAMETER_NODE) || linear_ast[j]->is(OBJECT_DEFINTION_NODE))
			Variable = linear_ast[j];
		else if (linear_ast[j]->Name == "*")
			Operator = linear_ast[j];
		else {
			//if it is not either one of those the conditions are not met and then just zero all.
			Coefficient = nullptr;
			Variable = nullptr;
			Operator = nullptr;
		}
		if ((Coefficient != nullptr) && (Variable != nullptr) && (Operator != nullptr)) {

			linear_ast.erase(linear_ast.begin() + j - 2, linear_ast.begin() + j);

			//now apply the coefficient to the variable
			Variable->Coefficient = atoi(Coefficient->Name.c_str()) * Coefficient->Coefficient;
			Variable->Context = Operator->Context;

			/*if (Variable->Holder->Name == "-") {
				Variable->Coefficient *= -1;
				Variable->Holder->Name = "+";
			}*/


			*Operator = *Variable;
			//for next iteration
			linear_ast[j - 2] = Variable;

			j -= 2;
		}
	}
}

void Algebra::Reset_Defining_Value(int i)
{
	if (!Input->at(i)->is(POSTFIX_NODE) && !Input->at(i)->is(PREFIX_NODE))
		return;

	Node* Modified = Input->at(i)->Left;
	if (Input->at(i)->is(PREFIX_NODE))
		Modified = Input->at(i)->Right;

	//i++ | ++i -> defining val = 0

	if (Modified->Has({ OPERATOR_NODE, ARRAY_NODE }))
		Modified = Modified->Get_Most_Right();

	Parent->Find(Modified, Modified->Scope)->Current_Value = nullptr;
}

void Algebra::Clean_Unused()
{
	/*for (int d = 0; d < Parent->Defined.size(); d++) {
		if (Parent->Defined[d]->is("ptr") != -1)
			if (Parent->Defined[d]->is(PARAMETER_NODE))
				continue;
		if (Parent->Defined[d]->is("const") != -1)
			continue;
		//update the calling count.
		Parent->Defined[d]->Calling_Count = 0;

		for (int i = 0; i < Input->size(); i++) {
			if (!Input->at(i)->is(OPERATOR_NODE) && !Input->at(i)->is(ASSIGN_OPERATOR_NODE) && !Input->at(i)->is(CONDITION_OPERATOR_NODE) && !Input->at(i)->is(BIT_OPERATOR_NODE)) {
				if (Input->at(i)->Name == "return") {
					if (Input->at(i)->Right == nullptr)	//void return
						continue;
					vector<Node*> tmp = Linearise({ Input->at(i)->Right });
					for (int j = 0; j < tmp.size(); j++)
						if (Parent->Defined[d]->Name == tmp[j]->Name) {
							if ((size_t)j + 1 < tmp.size()) {
								if (!tmp[(size_t)j + 1]->is(ASSIGN_OPERATOR_NODE))
									Parent->Defined[d]->Calling_Count++;
							}
							else
								Parent->Defined[d]->Calling_Count++;
						}
				}
				else if (Input->at(i)->Childs.size() > 0) {
					for (auto c : Input->at(i)->Childs) {
						vector<Node*> tmp = Linearise(c);
						for (int j = 0; j < tmp.size(); j++)
							if (Parent->Defined[d]->Name == tmp[j]->Name) {
								if ((size_t)j + 1 < tmp.size()) {
									if (!tmp[(size_t)j + 1]->is(ASSIGN_OPERATOR_NODE))
										Parent->Defined[d]->Calling_Count++;
								}
								else
									Parent->Defined[d]->Calling_Count++;
							}
					}
				}
				else
					continue;
			}
			vector<Node*> linear = Linearise(Input->at(i), true);
			for (int l = 0; l < linear.size(); l++) {
				if (Parent->Defined[d]->Name == linear[l]->Name) {
					if ((size_t)l + 1 < linear.size()) {
						if (!linear[(size_t)l + 1]->is(ASSIGN_OPERATOR_NODE))
							Parent->Defined[d]->Calling_Count++;
					}
					else
						Parent->Defined[d]->Calling_Count++;
				}
			}
		}
	}
	for (int d = 0; d < Parent->Defined.size(); d++) {
		if (Parent->Defined[d]->is("ptr") != -1)
			if (Parent->Defined[d]->is(PARAMETER_NODE))
				continue;
		if (Parent->Defined[d]->is("const") != -1)
			continue;
		for (int i = 0; i < Input->size(); i++) {
			if (!Input->at(i)->is(OPERATOR_NODE) && !Input->at(i)->is(ASSIGN_OPERATOR_NODE) && !Input->at(i)->is(CONDITION_OPERATOR_NODE) && !Input->at(i)->is(BIT_OPERATOR_NODE))
				continue;
			vector<Node*> linear = Linearise(Input->at(i), false);
			for (int l = 0; l < linear.size(); l++) {
				if (Parent->Defined[d]->Name == linear[l]->Name) {
					if (Parent->Defined[d]->Calling_Count < 1) {
						Input->erase(Input->begin() + i--);
						Parent->Defined.erase(Parent->Defined.begin() + d);
						Optimized = true;
						break;
					}
				}
			}
		}
	}*/
	for (int i = 0; i < Parent->Defined.size(); i++) {
		if (Parent->Defined[i]->is(PARAMETER_NODE))
			continue;
		//check if the expiring index is same as the definition index
		if (Parent->Defined[i]->Current_Value){
			if (Parent->Defined[i]->Current_Value->Define_Index == Parent->Defined[i]->Current_Value->Expiring_Index)
				Parent->Defined.erase(Parent->Defined.begin() + i);
			}
		else {
			//the inlining value could not be made.
			for (auto line : *Input) {
				for (auto element : Linearise(line)) {
					if (element->Name == Parent->Defined[i]->Name)
						Parent->Defined[i]->Calling_Count++;
				}
			}
			if (Parent->Defined[i]->Calling_Count < 1)
				Parent->Defined.erase(Parent->Defined.begin() + i);
		}

	}
}

void Algebra::Operate_Coefficient_Constants(Node* op)
{
	//b = 2x * -2x
	//return; //depricated
	if (!op->is(OPERATOR_NODE) && !op->is(ASSIGN_OPERATOR_NODE))
		return;
	if (op->Left->is(OPERATOR_NODE) || op->Left->is(ASSIGN_OPERATOR_NODE))
		Operate_Coefficient_Constants(op->Left);
	if (op->Right->is(OPERATOR_NODE))
		Operate_Coefficient_Constants(op->Right);

	if (op->Left->Name != op->Right->Name)
		return;
	if (op->Left->Coefficient == 0 || op->Right->Coefficient == 0)
		return;

	if (op->Name != "*" && op->Name != "/")
		return;

	Node* New_Num;

	long left = op->Left->Coefficient;
	long right = op->Right->Coefficient;

	
	if (op->Name == "/") {
		//3b / b == 3
		if (op->Left->Order == op->Right->Order) {
			long double remainder = left / right;
			New_Num = new Node(NUMBER_NODE, op->Location);

			if (remainder - (long long)remainder == 0)
				New_Num->Name = to_string((long long)remainder);
			else
				New_Num->Name = to_string(remainder);
		}
		else {
			//3b / b^2 == 3b^-1
			long double coefficient = left / right;
			string Name = op->Left->Name;
			long Order = op->Left->Order - op->Right->Order;
			New_Num = new Node(OBJECT_NODE, op->Location);
			New_Num->Name = Name;
			New_Num->Coefficient = coefficient;
			New_Num->Order = Order;
		}
		New_Num->Context = op->Context;
		New_Num->Scope = op->Scope;
	}
	else if (op->Name == "*") {
		//3b * b == 3b^2
		//3b * b^2 == 3b^3
		long double Coefficient = left * right;
		string Name = op->Left->Name;
		long Order = op->Left->Order + op->Right->Order;
		New_Num = new Node(OBJECT_NODE, op->Location);
		New_Num->Name = Name;
		New_Num->Coefficient = Coefficient;
		New_Num->Order = Order;
		New_Num->Context = op->Context;
		New_Num->Scope = op->Scope;
	}


	*op = *New_Num;

	if (op->Coefficient == 0) {
		Node* num = new Node(NUMBER_NODE, op->Location);
		num->Name = "0";
		*op = *num;
	}

	Optimized = true;

}

void Algebra::Operate_Distant_Coefficients(Node* op)
{
	if (!op->is(OPERATOR_NODE) && !op->is(ASSIGN_OPERATOR_NODE) && !op->is(CONDITION_OPERATOR_NODE) && !op->is(BIT_OPERATOR_NODE))
		return;

	//((3 / b) - b) -> (3 / (b - b)) -> 3
	//this function tryes to find next to eachother variables and computes the result


	vector<Node*> Linear = Linearise(op, true);
	vector<Node*> Coefficients;
	Node* Operator = nullptr;
	for (auto i : Linear) {
		if (i->is(OPERATOR_NODE))
			Operator = i;
		else if (i->is(OBJECT_NODE)) {
			Coefficients.push_back(i);
		}
		else {
			Operator = nullptr;
			Coefficients.clear();
			continue;
		}

		if (Operator != nullptr && Coefficients.size() == 2) {

			Node* New_Num;

			if (Coefficients[0]->Name != Coefficients[1]->Name) {
				Operator = nullptr;
				Coefficients.clear();
				continue;
			}
			//find the other operators that may stuck under the operator override.
			//(6 + b) - (b * 3)
			//(6 + b) - 3b
			//b - 3b --> the 6 has been forgotten
			Node* Forgotten_Operator = nullptr;
			bool Forgotten_Operator_is_Left_Side = true;
			if (Operator->Left == Coefficients[0] && Operator->Right != Coefficients[1])
				Forgotten_Operator = Operator->Right;
			else if (Operator->Left != Coefficients[0] && Operator->Right == Coefficients[1]) {
				Forgotten_Operator_is_Left_Side = false;
				Forgotten_Operator = Operator->Left;
			}
				
			long left = Coefficients[0]->Coefficient;
			long right = Coefficients[1]->Coefficient;

			if (Operator->Name == "/") {
				//3b / b == 3
				if (Coefficients[0]->Order == Coefficients[1]->Order) {
					long double remainder = left / right;
					New_Num = new Node(NUMBER_NODE, Operator->Location);

					if (remainder - (long long)remainder == 0)
						New_Num->Name = to_string((long long)remainder);
					else
						New_Num->Name = to_string(remainder);
				}
				else {
					//3b / b^2 == 3b^-1
					long double coefficient = left / right;
					string Name = Coefficients[0]->Name;
					long Order = Coefficients[0]->Order - Coefficients[1]->Order;
					New_Num = new Node(OBJECT_NODE, Operator->Location);
					New_Num->Name = Name;
					New_Num->Coefficient = coefficient;
					New_Num->Order = Order;
				}
				New_Num->Context = Operator->Context;
				New_Num->Scope = Operator->Scope;
			}
			else if (Operator->Name == "*") {
				//3b * b == 3b^2
				//3b * b^2 == 3b^3
				long double Coefficient = left * right;
				string Name = Coefficients[0]->Name;
				long Order = Coefficients[0]->Order + Coefficients[1]->Order;
				New_Num = new Node(OBJECT_NODE, Operator->Location);
				New_Num->Name = Name;
				New_Num->Coefficient = Coefficient;
				New_Num->Order = Order;
				New_Num->Context = Operator->Context;
				New_Num->Scope = Operator->Scope;
			}
			else if (Operator->Name == "+") {
				if (Coefficients[0]->Order != Coefficients[1]->Order)
					continue;
				//here it affects the coefficient of other
				long long Coefficient = Coefficients[0]->Coefficient + Coefficients[1]->Coefficient;
				New_Num = new Node(OBJECT_NODE, Operator->Location);
				New_Num->Name = Coefficients[0]->Name;
				New_Num->Coefficient = Coefficient;
				New_Num->Order = Coefficients[0]->Order;
				New_Num->Context = Operator->Context;
				New_Num->Scope = Operator->Scope;
			}
			else if (Operator->Name == "-") {
				if (Coefficients[0]->Order != Coefficients[1]->Order)
					continue;
				//here it affects the coefficient of other				
				long long Coefficient = Coefficients[0]->Coefficient - Coefficients[1]->Coefficient;
				New_Num = new Node(OBJECT_NODE, Operator->Location);
				New_Num->Name = Coefficients[0]->Name;
				New_Num->Coefficient = Coefficient;
				New_Num->Order = Coefficients[0]->Order;
				New_Num->Context = Operator->Context;
				New_Num->Scope = Operator->Scope;
			}

			if (Forgotten_Operator != nullptr) {
				Operator->Context = Forgotten_Operator;
				if (Forgotten_Operator_is_Left_Side) {
					//put the operator into forgotten operators Left side
					Forgotten_Operator->Left = New_Num;
					New_Num->Context = Forgotten_Operator;
				}
				else {
					Forgotten_Operator->Right = New_Num;
					New_Num->Context = Forgotten_Operator;
				}
				*Operator = *Forgotten_Operator;
			}
			else
				*Operator = *New_Num;
		}
	}

}

void Algebra::Operate_Numbers_As_Constants(Node* op)
{
	if (!op->is(OPERATOR_NODE) && !op->is(ASSIGN_OPERATOR_NODE) && !op->is(BIT_OPERATOR_NODE)) // && !op->is(CONDITION_OPERATOR_NODE))
			return;
	if (op->Left->is(OPERATOR_NODE) || op->Left->is(BIT_OPERATOR_NODE)) // || op->Left->is(CONDITION_OPERATOR_NODE))
		Operate_Numbers_As_Constants(op->Left);
	else if (op->Left->is(CONTENT_NODE)) {
		for (Node* i : op->Left->Childs)
			Operate_Numbers_As_Constants(i);
		if (op->Left->Childs.size() == 1 && op->Left->Childs[0]->is(NUMBER_NODE)) {
			op->Left->Childs[0]->Coefficient *= op->Left->Coefficient;
			op->Left->Childs[0]->Context = op->Left->Context;
			*op->Left = *op->Left->Childs[0];
		}
	}
	if (op->Right->is(OPERATOR_NODE) || op->Right->is(BIT_OPERATOR_NODE)) // || op->Right->is(CONDITION_OPERATOR_NODE))
		Operate_Numbers_As_Constants(op->Right);
	else if (op->Right->is(CONTENT_NODE)) {
		for (Node* i : op->Right->Childs)
			Operate_Numbers_As_Constants(i);
		if (op->Right->Childs.size() == 1 && op->Right->Childs[0]->is(NUMBER_NODE)) {
			op->Right->Childs[0]->Coefficient *= op->Right->Coefficient;
			op->Right->Childs[0]->Context = op->Right->Context;
			*op->Right = *op->Right->Childs[0];
		}
	}

	if (!op->Left->is(NUMBER_NODE))
		return;
	if (!op->Right->is(NUMBER_NODE))
		return;

	Node* New_Num = new Node(NUMBER_NODE, op->Location);

	//set sizes
	if (op->Left->Size < op->Right->Size)
		New_Num->Size = op->Right->Size;
	else
		New_Num->Size = op->Left->Size;

	if (op->Left->Format == "decimal" || op->Right->Format == "decimal") {
		long double left = atof(op->Left->Name.c_str());
		long double right = atof(op->Right->Name.c_str());

		New_Num->Format = "decimal";

		if (op->Name == "+")
			New_Num->Name = to_string(left + right);
		else if (op->Name == "-")
			New_Num->Name = to_string(left - right);
		else if (op->Name == "*")
			New_Num->Name = to_string(left * right);
		else if (op->Name == "/")
			New_Num->Name = to_string(left / right);
		else if (op->Name == "<<")
			New_Num->Name = to_string((long long)left << (long long)right);
		else if (op->Name == ">>")
			New_Num->Name = to_string((long long)left >> (long long)right);
		else if (op->Name == "&")
			New_Num->Name = to_string((long long)left & (long long)right);
		else if (op->Name == "|")
			New_Num->Name = to_string((long long)left | (long long)right);
		else if (op->Name == "�")
			New_Num->Name = to_string((long long)left ^ (long long)right);
		else if (op->Name == "%")
			New_Num->Name = to_string((long long)left % (long long)right);
		else if (op->Name == "^")
			New_Num->Name = to_string(pow(left, right));
	}
	else {
		long left = atol(op->Left->Name.c_str());
		long right = atol(op->Right->Name.c_str());

		if (op->Name == "+")
			New_Num->Name = to_string(left + right);
		else if (op->Name == "-")
			New_Num->Name = to_string(left - right);
		else if (op->Name == "*")
			New_Num->Name = to_string(left * right);
		else if (op->Name == "/")
			New_Num->Name = to_string(left / right);
		else if (op->Name == "<<")
			New_Num->Name = to_string(left << right);
		else if (op->Name == ">>")
			New_Num->Name = to_string(left >> right);
		else if (op->Name == "&")
			New_Num->Name = to_string(left & right);
		else if (op->Name == "|")
			New_Num->Name = to_string(left | right);
		else if (op->Name == "�")
			New_Num->Name = to_string(left ^ right);
		else if (op->Name == "%")
			New_Num->Name = to_string(left % right);
		else if (op->Name == "^")
			New_Num->Name = to_string(pow(left, right));
	}

	New_Num->Context = op->Context;
	New_Num->Scope = op->Scope;
	*op = *New_Num;
	Optimized = true;
}

Node* Algebra::Operate_Constants(Node* l, Node* r) {

	Node* tmp;
	if (r->Context->Left == r) {
		tmp = l;
		l = r;
		r = tmp;
	}

	Node* New_Num = new Node(NUMBER_NODE, l->Location);
	New_Num->Context = l->Context;
	New_Num->Scope = l->Scope;
	//set sizes
	if (l->Size < r->Size)
		New_Num->Size = r->Size;
	else
		New_Num->Size = l->Size;

	if (l->Format == "decimal") {
		long double left = atof(l->Name.c_str());
		long double right = atof(r->Name.c_str());

		New_Num->Format = "decimal";

		if (r->Coefficient > 0)
			New_Num->Name = to_string(left + right);
		else
			New_Num->Name = to_string(left - right);
	}
	else {
		long left = atoi(l->Name.c_str());
		long right = atoi(r->Name.c_str());

		if (r->Coefficient > 0)
			New_Num->Name = to_string(left + right);
		else
			New_Num->Name = to_string(left - right);
	}

	return New_Num;
}

void Algebra::Combine_Scattered(Node* op) {
	if (!op->is(OPERATOR_NODE) && !op->is(ASSIGN_OPERATOR_NODE) && !op->is(CONDITION_OPERATOR_NODE) && !op->is(BIT_OPERATOR_NODE))
		return;

	vector<Node*> list = Linearise(op, true);

	vector<int> Number_Indices;

	//find and save the indices of constants.
	for (int i = 0; i < list.size(); i++)
		if (list[i]->is(NUMBER_NODE))
			Number_Indices.push_back(i);

	//now check if you can operate those two constants together.
	for (int i = 0; i < Number_Indices.size(); i++) {
		if (Number_Indices[i] - 1 > 0 && (list[(size_t)Number_Indices[i] - 1]->Name != "+" && list[(size_t)Number_Indices[i] - 1]->Name != "-")) {
			if ((size_t)Number_Indices[i] + 1 < list.size() && (list[(size_t)Number_Indices[i] + 1]->Name != "+" && list[(size_t)Number_Indices[i] + 1]->Name != "-"))
				Number_Indices.erase(Number_Indices.begin() + i--);
			else if ((size_t)Number_Indices[i] + 1 >= list.size())
				Number_Indices.erase(Number_Indices.begin() + i--);
		}
		else if ((size_t)Number_Indices[i] + 1 < list.size() && (list[(size_t)Number_Indices[i] + 1]->Name != "+" && list[(size_t)Number_Indices[i] + 1]->Name != "-"))
			Number_Indices.erase(Number_Indices.begin() + i--);
	}

	for (int i = 0; (size_t)i+1 < Number_Indices.size(); i += 2) {
		*list[Number_Indices[i]] = *Operate_Constants(list[Number_Indices[i]], list[Number_Indices[(size_t)i + 1]]);
		//now remove the other constant
		Node* other = Get_Other_Pair(op, list[Number_Indices[(size_t)i + 1]]);
		*other->Context = *list[Number_Indices[i]];
	}
}

Node* Algebra::Get_Other_Pair(Node* ast, Node* other) {
	if (ast->Left == other)
		return ast->Right;
	else if (ast->Right == other)
		return ast->Left;
	else if (ast->Left != nullptr && Get_Other_Pair(ast->Left, other) != nullptr)
		return Get_Other_Pair(ast->Left, other);
	else if (ast->Right != nullptr && Get_Other_Pair(ast->Right, other) != nullptr)
		return Get_Other_Pair(ast->Right, other);
	else if (ast->is(CONTENT_NODE)) {
		if (ast->Childs[0]->is(OPERATOR_NODE) || ast->Childs[0]->is(ASSIGN_OPERATOR_NODE) || ast->Childs[0]->is(CONDITION_OPERATOR_NODE) || ast->Childs[0]->is(BIT_OPERATOR_NODE)) {
			if (Get_Other_Pair(ast->Childs[0], other) != nullptr)
				return Get_Other_Pair(ast->Childs[0], other);
		}
		else if (ast->Childs[0] == other){
			if (ast->Context->Left == ast)
				return ast->Context->Right;
			else if (ast->Context->Right == ast)
				return ast->Context->Left;
		}
	}
	return nullptr;
}

void Algebra::Un_Wrap_Parenthesis(Node* p)
{
	if (p->is(OPERATOR_NODE) || p->is(ASSIGN_OPERATOR_NODE) || p->is(CONDITION_OPERATOR_NODE) || p->is(BIT_OPERATOR_NODE)) {
		Un_Wrap_Parenthesis(p->Left);
		Un_Wrap_Parenthesis(p->Right);
	}
	else if (p->is(CONTENT_NODE))
		Un_Wrap_Parenthesis(p->Childs[0]);
	if (!p->is(CONTENT_NODE))
		return;
	if (p->Childs.size() > 1)
		return;
	//(b * 3)
	//(3b)
	//3b
	if (!p->is(OPERATOR_NODE) && !p->is(ASSIGN_OPERATOR_NODE) && !p->is(CONDITION_OPERATOR_NODE) && !p->is(BIT_OPERATOR_NODE)) {
		p->Childs[0]->Context = p->Context;
		p->Childs[0]->Scope = p->Scope;
		p->Childs[0]->Coefficient *= p->Coefficient;
		p->Childs[0]->Order *= p->Order;
		p->Childs[0]->Cast_Type = p->Cast_Type;
		*p = *p->Childs[0];

		//Optimized = true;
	}
}

void Algebra::Fix_Coefficient_Into_Real_Operator(Node* n)
{
	//here we will fix the coefficient into a real operator as the name yells.
	if (n->is(OPERATOR_NODE) || n->is(CONDITION_OPERATOR_NODE) || n->is(ASSIGN_OPERATOR_NODE)) {
		if (n->Left->Coefficient != 0)
			Fix_Coefficient_Into_Real_Operator(n->Left);
		else {
			//this is needed to be cleaned!!
			//and remember the ((a <-- this is no more) - 1) <-- so this is -1 after the clean!!
			n->Right->Context = n->Context;
			*n = *n->Right;
			return;
		}
		if (n->Right->Coefficient != 0)
			Fix_Coefficient_Into_Real_Operator(n->Right);
		else {
			//this is needed to be cleaned!!
			//and remember the ((a <-- this is no more) - 1) <-- so this is -1 after the clean!!
			n->Left->Context = n->Context;
			*n = *n->Left;
			return;
		}
	}
	else if (n->is(CONTENT_NODE)) {
		for (auto j : n->Childs)
			Fix_Coefficient_Into_Real_Operator(j);
	}
	//only variables are accepted
	if (!n->is(OBJECT_NODE) && !n->is(PARAMETER_NODE))
		return;
	if (n->Coefficient == 1 || n->Coefficient == -1)
		return;

	//make operator that is going to hold the new coefficient and the variable
	Node* New_Operator = new Node(OPERATOR_NODE, n->Location);
	New_Operator->Name = "*";
	New_Operator->Scope = n->Scope;
	New_Operator->Context = n->Context;

	//making the coefficient into a real number token
	Node* Coefficient = new Node(NUMBER_NODE, n->Location);
	Coefficient->Name = to_string(n->Coefficient);
	Coefficient->Scope = n->Scope;
	Coefficient->Context = n->Context;

	//now clean the coefficient
	n->Coefficient = 1;

	//transform the negative holder into positive operator
	if (n->Context->Name == "-")
		n->Context->Name = "+";

	//combine
	//this is because the override later
	New_Operator->Left = new Node(OBJECT_NODE, n->Location);

	*New_Operator->Left = *n;
	New_Operator->Right = Coefficient;

	Node* Parenthesis = new Node(CONTENT_NODE, n->Location);
	Parenthesis->Childs.push_back(New_Operator);
	Parenthesis->Paranthesis_Type = '(';

	Parenthesis->Context = New_Operator->Context;
	Parenthesis->Scope = New_Operator->Scope;

	New_Operator->Context = Parenthesis;
	New_Operator->Scope = Parenthesis;

	*n = *Parenthesis;

	return;
}

void Algebra::Fix_Order_Into_Real_Operator(Node* n)
{
	//here we will fix the coefficient into a real operator as the name yells.
	if (n->is(OPERATOR_NODE) || n->is(CONDITION_OPERATOR_NODE) || n->is(ASSIGN_OPERATOR_NODE)) {
		if (n->Left->Order != 0)
			Fix_Order_Into_Real_Operator(n->Left);
		else {
			//this is needed to be cleaned!!
			//and remember the ((a <-- this is no more) - 1) <-- so this is -1 after the clean!!
			*n = *n->Right;
			return;
		}
		if (n->Right->Order != 0)
			Fix_Order_Into_Real_Operator(n->Right);
		else {
			//this is needed to be cleaned!!
			//and remember the ((a <-- this is no more) - 1) <-- so this is -1 after the clean!!
			*n = *n->Left;
			return;
		}
	}
	//only variables are accepted
	if (!n->is(OBJECT_NODE) && !n->is(PARAMETER_NODE))
		return;
	if (n->Order == 1)
		return;

	int Order = n->Order;
	n->Order = 1;
	Node* L = n->Copy_Node(n, n->Scope);
	for (int i = 0; i < Order; i++) {
		//(((a * a) * a)...)
		Node* mul = new Node(OPERATOR_NODE, "*", n->Location);
		mul->Left = L;
		mul->Left->Context = mul;	//give the previus mul this as a holder.
		mul->Right = n->Copy_Node(n, n->Scope);	//multiply self by self
		mul->Scope = n->Scope;

		L = mul;
	}

	//give the last mul the n->holder
	L->Context = n->Context;
	
	*n = *L;
	return;
}