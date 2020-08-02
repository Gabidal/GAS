#include "../../H/PostProsessor/Algebra.h"

void Algebra::Factory() {
	for (int i = 0; i < Input->size(); i++) {
		Set_Coefficient_Value(i);
		Set_Defining_Value(i);
		Inline_Variables(i);
	}
	for (int i = 0; i < Input->size(); i++)
		Clean(i);
	for (auto i : *Input)
		Operate_Numbers_As_Constants(i);
	for (auto i : *Input)
		Operate_Coefficient_Constants(i);
	for (auto i : *Input)
		Fix_Coefficient_Into_Real_Operator(i);
}

vector<Node*> Algebra::Linearise(Node* ast, bool Include_Operator = false)
{
	vector<Node*> Result;
	if (ast->is(OPERATOR_NODE)) {
		vector<Node*> left = Linearise(ast->Left, Include_Operator);
		Result.insert(Result.end(), left.begin(), left.end());

		if (Include_Operator)
			Result.push_back(ast);

		vector<Node*> right = Linearise(ast->Right, Include_Operator);
		Result.insert(Result.end(), right.begin(), right.end());
	}
	else if (ast->is(CONTENT_NODE))
		Result.insert(Result.end(), ast->Childs.begin(), ast->Childs.end());
	else
		Result.push_back(ast);

	return Result;
}

void Algebra::Inline_Variables(int i)
{
	//<summary>
	//finds a math equatin and tryes to inline the used variables set Values.
	//</summary>

	if (!Input->at(i)->is(OPERATOR_NODE))
		return;

	vector<Node*> Linear_Ast;

	if (Input->at(i)->Name != "=")
		Linear_Ast = Linearise(Input->at(i));
	else
		Linear_Ast = Linearise(Input->at(i)->Right);

	for (Node* n : Linear_Ast) {
		Node* d = Parent->Find(n->Name, Parent);
		//if this is nullptr is means it is defined outside this scope.
		if (d != nullptr)
			if (d->Current_Value != nullptr) {
				*n = *d->Current_Value;
				d->Inlined = true;
				//maybe this is useless:
				n->Inlined = false;
			}
	}

}

void Algebra::Clean(int i)
{
	if (!Input->at(i)->is(OPERATOR_NODE))
		return;
	if (Input->at(i)->Name != "=")
		return;
	if (Parent->Find(Input->at(i)->Left->Name, Parent)->Inlined == false)
		return;

	//!!! MUST COMBINE THE CALLATION AND THE CLASS FETCHER!!!
	vector<Node*> Calls = Input->at(i)->Has(CALL_NODE);

	Input->erase(Input->begin() + i);

	if (Calls.size() > 0) {
		//put the function callations into the deleted operations location else.
		Input->insert(Input->begin() + i, Calls.begin(), Calls.end());
	}

	Clean(i);
		
	return;
}

void Algebra::Set_Defining_Value(int i)
{
	//x = 12 + a
	//y = x * 2
	//x = 12
	//z = x * 4
	if (!Input->at(i)->is(OPERATOR_NODE))
		return;
	if (Input->at(i)->Name != "=")
		return;
	//remeber if functoin call has do something global/pointter-
	//dont let this function run on that!

	//give the defining node the current set-val.
	Parent->Find(Input->at(i)->Left->Name, Parent)->Current_Value = Input->at(i)->Right;

	return;
}

void Algebra::Set_Coefficient_Value(int i)
{
	//a = x * 2
	//b = x * 2 + a
	if (!Input->at(i)->is(OPERATOR_NODE))
		return;

	vector<Node*> linear_ast = Linearise(Input->at(i), true);

	Node* Coefficient = nullptr;
	Node* Variable = nullptr;
	Node* Operator = nullptr;

	//now try to pair these
	for (int j = 0; j < linear_ast.size(); j++) {
		if (linear_ast[j]->is(NUMBER_NODE))
			Coefficient = linear_ast[j];
		else if (linear_ast[j]->is(OBJECT_NODE))
			Variable = linear_ast[j];
		else if (linear_ast[j]->Name == "*")
			Operator = linear_ast[j];
		else {
			//if it is not either one of those the conditions are not met and then just zero all.
			Coefficient = nullptr;
			Variable = nullptr;
			Operator = nullptr;
		}
		if ((Coefficient != nullptr) && (Variable != nullptr) && (Operator != nullptr))
			break;
	}

	//the required nodes where not found thus return. 
	if ((Coefficient == nullptr) || (Variable == nullptr) || (Operator == nullptr))
		return;

	//now apply the coefficient to the variable
	Variable->Coefficient = atoi(Coefficient->Name.c_str());

	*Operator = *Variable;
}

void Algebra::Operate_Coefficient_Constants(Node* op)
{
	//b = x2 + x2
	if (!op->is(OPERATOR_NODE))
		return;
	if (op->Left->is(OPERATOR_NODE))
		Operate_Coefficient_Constants(op->Left);
	if (op->Right->is(OPERATOR_NODE))
		Operate_Coefficient_Constants(op->Right);

	if (op->Left->Coefficient == 0)
		return;
	if (op->Right->Coefficient == 0)
		return;
	if (op->Left->Name != op->Right->Name)
		return;


	Node* New_Num = new Node(*op->Left);

	long left = op->Left->Coefficient;
	long right = op->Right->Coefficient;

	if (op->Name == "+")
		New_Num->Coefficient = left + right;
	else if (op->Name == "-")
		New_Num->Coefficient = left - right;
	else if (op->Name == "*")
		New_Num->Coefficient = left * right;
	else if (op->Name == "/")
		New_Num->Coefficient = left / right;
	else if (op->Name == "<<")
		New_Num->Coefficient = left << right;
	else if (op->Name == ">>")
		New_Num->Coefficient = left >> right;
	else if (op->Name == "&")
		New_Num->Coefficient = left & right;
	else if (op->Name == "|")
		New_Num->Coefficient = left | right;
	else if (op->Name == "�")
		New_Num->Coefficient = left ^ right;
	else if (op->Name == "%")
		New_Num->Coefficient = left % right;
	else if (op->Name == "^")
		New_Num->Coefficient = pow(left, right);

	*op = *New_Num;

}

void Algebra::Operate_Numbers_As_Constants(Node* op)
{
	if (!op->is(OPERATOR_NODE))
		return;
	if (op->Left->is(OPERATOR_NODE))
		Operate_Numbers_As_Constants(op->Left);
	if (op->Right->is(OPERATOR_NODE))
		Operate_Numbers_As_Constants(op->Right);

	if (!op->Left->is(NUMBER_NODE))
		return;
	if (!op->Right->is(NUMBER_NODE))
		return;

	Node* New_Num = new Node(NUMBER_NODE);

	long left = atoi(op->Left->Name.c_str());
	long right = atoi(op->Right->Name.c_str());

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

	*op = *New_Num;
}

void Algebra::Fix_Coefficient_Into_Real_Operator(Node* n)
{
	//here we will fix the coefficient into a real operator as the name yells.
	if (n->is(OPERATOR_NODE)) {
		Fix_Coefficient_Into_Real_Operator(n->Left);
		Fix_Coefficient_Into_Real_Operator(n->Right);
	}
	//only variables are accepted
	if (!n->is(OBJECT_NODE))
		return;
	//no point in fixing nothing :D
	if (n->Coefficient == 0)
		return;

	//make operator that is going to hold the new coefficient and the variable
	Node* New_Operator = new Node(OPERATOR_NODE);
	New_Operator->Name = "*";

	//making the coefficient into a real number token
	Node* Coefficient = new Node(NUMBER_NODE);
	Coefficient->Name = to_string(n->Coefficient);

	//now clean the coefficient
	n->Coefficient = 0;

	//combine
	//this is because the override later
	New_Operator->Left = new Node(OBJECT_NODE);

	*New_Operator->Left = *n;
	New_Operator->Right = Coefficient;

	*n = *New_Operator;

	return;
}
