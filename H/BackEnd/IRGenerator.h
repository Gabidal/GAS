#ifndef _IRGENERATOR_H_
#define _IRGENERATOR_H_
#include "../Nodes/IR.h"
#include "../Nodes/Token.h"
#include "../Nodes/Node.h"
#include "../Flags.h"

#include <vector>
#include <string>

using namespace std;

class IRGenerator {
public:
	Node* Parent = nullptr;
	Token* Handle = nullptr;

	vector<IR*>* Output;
	vector<Node*> Input;

	IRGenerator(Node* p) : Parent(p) { Output = new vector<IR*>; }
	IRGenerator(Node* p, vector<Node*> in) : Parent(p), Input(in) { Output = new vector<IR*>; }
	IRGenerator(Node* p, vector<Node*> in, vector<IR*>* Out) : Parent(p), Input(in), Output(Out) { Factory(); }
	IRGenerator(Node* p, vector<Node*> in, vector<IR*>* Out, bool assign) : Parent(p), Input(in), Output(Out), Is_In_Left_Side_Of_Operator(assign) { Factory(); }
	IRGenerator(Node* p, vector<IR*>* Out) : Parent(p), Output(Out){}
	void Generate(vector<Node*> in, bool set_insert_mode = false) { Handle = nullptr; Input = in; Is_In_Left_Side_Of_Operator = set_insert_mode; Factory(); }

	void Factory();
	void Parse_Function(int i);
	void Parse_Calls(int i);
	void Parse_If(int i);
	void Loop_Elses(Node* e);
	void Un_Wrap_Inline(int i);
	void Parse_Loops(int i);
	void Parse_Return(int i);

	void Parse_Operators(int i);
	void Parse_Jumps(int i);
	void Parse_Pointers(int i);
	void Parse_Arrays(int i);
	void Parse_PreFixes(int i);
	void Parse_PostFixes(int i);

	void Parse_Jump(int i);
	void Parse_Parenthesis(int i);
	void Update_Operator(Node*n);
	//TODO: hey gab! use log(inlining count) base 10 for the global inlining label adding rendom char adder.
	//TODO: make a repeating optimizer thet makes 
	//a new variable and saves the result once and then uses it those other times
	string Get_Inverted_Condition(string c);
	vector<Token*> Get_All_Extern_Variables(int end_index, int start_index, Node* scope);
	vector<Token*> Find(string n, Token* t);
	vector<Token*> Find(long n, Token* t);

	//p is the pointter ,			, -x unwrap | +x wrap
	Token* Operate_Pointter(Token* p, int Difference, bool Needed_At_Addressing = false);

	IR* Make_Label(Node* n, bool Mangle);
	IR* Make_Jump(string condition, string l);
	int Get_Amount(string t, Node* n);
private:
	bool Is_In_Left_Side_Of_Operator = false;
};

#endif