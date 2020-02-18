#ifndef _BACK_H_
#define _BACK_H_
#include <string>
#include "Token.h"
#include "Assembly_Definitions.h"
#include "../OpC/IR.h"
using namespace std;

class Back
{
public:
	Back(string& s) : Output(s)
	{
	}
	string& Output;
	string Get_Agent(bool Storing, Token* t);
	//agent prepensitives
	string Get_Mem_Address(Token *t);
	string Get_Size_Translator(int Size);
	void Make(IR* ir, bool Storing_To_Mem);
	string Get_PreFix(IR* ir);

	~Back();

private:

};


#endif