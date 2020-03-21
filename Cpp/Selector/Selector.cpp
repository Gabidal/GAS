#include "..\..\H\Selector\Selector.h"
#include "../../H/Back/Token.h"
extern int _SYSTEM_BIT_TYPE;
extern map<string, Token*> Register_Lock;

Selector::Selector(string s)
{
	Board_type = s;
	if (s == "x86")
	{
		x86 x;
		x.ARC_Factory();
		Registers128 = x.Registers128;
		Registers64 = x.Registers64;
		Registers32 = x.Registers32;
		Registers16 = x.Registers16;
		Registers8 = x.Registers8;
		OpCodes = x.OpCodes;
	}
	else if (s == "arm")
	{
		ARM x;
		x.ARC_Factory();
		Registers128 = x.Registers128;
		Registers64 = x.Registers64;
		Registers32 = x.Registers32;
		Registers16 = x.Registers16;
		Registers8 = x.Registers8;
		OpCodes = x.OpCodes;
	}
}

Selector::~Selector()
{
}

OpC* Selector::OpCode_Selector()
{
	return nullptr;
}

Token* Selector::Get_Right_Reg(int F, int Size)
{
	Token* r;
	if (((F & Task_For_General_Purpose) == Task_For_General_Purpose) || ((F & Task_For_Floating_Math) == Task_For_Floating_Math))
	{
		if (Size == 12)
		{
			r = Get_Reg(Registers128, F, Reg_Turn128);
			if (r != nullptr)
				return r;
		}
		if (Size == 8)
		{
			r = Get_Reg(Registers64, F, Reg_Turn64);
			if (r != nullptr)
				return r;
		}
		else if (Size == 4)
		{
			r = Get_Reg(Registers32, F, Reg_Turn32);
			if (r != nullptr)
				return r;
		}
		else if (Size == 2)
		{
			r = Get_Reg(Registers16, F, Reg_Turn16);
			if (r != nullptr)
				return r;
		}
		else if (Size == 1)
		{
			r = Get_Reg(Registers8, F, Reg_Turn8);
			if (r != nullptr)
				return r;
		}
	}
	if (Size == 12)
	{
		for (Token* r : Registers128)
		{
			if (r->is(F))
			{
				return r;
			}
		}
	}
	else if (Size == 8)
	{
		for (Token* r : Registers64)
		{
			if (r->is(F))
			{
				return r;
			}
		}
	}
	else if (Size == 4)
	{
		for (Token* r : Registers32)
		{
			if (r->is(F))
			{
				return r;
			}
		}
	}
	else if (Size == 2)
	{
		for (Token* r : Registers16)
		{
			if (r->is(F))
			{
				return r;
			}
		}
	}
	else
	{
		for (Token* r : Registers8)
		{
			if (r->is(F))
			{
				return r;
			}
		}
	}
	cout << "Error:: Couldn't find suitable architehture register(" + Board_type + ": " + to_string(Size * 8) + " )." << endl;
	return nullptr;
}

Token* Selector::Get_Reg(vector<Token*> regs, int F, int &Previus)
{
	Token* r = new Token;
	if (Previus >= regs.size()) Previus = 0;
	int i = Previus;
	do// int i = Reg_Turn16; i < Registers16.size(); i++)
	{
		if (regs.at(i)->is(F))
		{
			r = regs.at(i);
			Previus = i + 1;
			return r;
		}
		if (++i >= regs.size())
		{
			i = 0;
		}
	} while (i != Previus);
	return nullptr;
}

string Selector::Get_ID(string id, string trust, vector<int> minmax)
{
	if (id == "")
		return "";

	vector<int> i_Sizes = minmax;
	for (OpC* o : OpCodes)
	{
		vector<int> o_Sizes = o->MinMax;
		if (o_Sizes.size() != (i_Sizes.size() * 2))
			continue;
		for (int i = 0; i < i_Sizes.size(); i++)
		{
			int in = i_Sizes.at(i);
			int in_min = o_Sizes.at(i * 2);
			int in_max = o_Sizes.at(i * 2 + 1);
			if ((in_min <= in) && (in_max >= in))
				continue;
			else
				goto CONTINUE;
		}
		if (o->ID == id)
			return o->OpCode;
		CONTINUE:;
	}
	if ((trust == "export") || (trust == "raw_label") || (trust == "global"))
		return id;
	if ((trust != "label") && (trust != "<") && (trust != ">") && (trust != "!<") && (trust != "!>") && (trust != "==") && (trust != "!=") && (trust != "<=") && (trust != ">=") && (trust != "jmp"))
	{
		cout << "Warning:: This is here because your'e using a illegal opcode: " << id << " ";
		for (int i : minmax)
		{
			cout << i << ", ";
		}
		cout << endl;
	}

	return "_" + id;
}
