#include "..\..\H\Back\Generator.h"
extern vector<string> Pre_Defined_Tokens;

void Generator::Factory()
{
	for (Token*t : Input)
	{
		Detect_Condition(t);
		Detect_Function(t);
		Detect_Operator(t);
		Detect_Pre_Defined_Tokens(t);
	}
}

void Generator::Detect_Function(Token* t)
{
	if (t->is(_Constructor_))
	{
		//make a label OpC*
		IR* ir = new IR;
		ir->ID = "label";
		ir->PreFix = t->Name;
		//make the insights of this constructor
		Generator g;
		g.Input = t->Right_Side_Token->Childs;
		g.Types = this->Types;
		g.Factory();
		//get the output as the new IR's childs.
		ir->Childs = g.Output;
	}
	else if (t->is(_Call_))
	{
		//make a callation OpC*
		IR* ir = new IR;
		ir->ID = t->Name;
		ir->PreFix = "call";
	}
}

void Generator::Detect_Condition(Token* t)
{
}

void Generator::Detect_Operator(Token* t)
{
}

void Generator::Detect_Pre_Defined_Tokens(Token* t)
{
	for (string T : Pre_Defined_Tokens)
	{
		if (t->Name == T)
		{
			IR *ir = new IR;
			ir->ID = T;
			ir->Parameters.push_back(t->Right_Side_Token);
			Output.push_back(ir);
		}
	}
}
