#include "../../H/Back/Definer.h"
#include <iostream>
using namespace std;
extern vector<string> Pre_Defined_Tokens;
extern vector<Token*> Generated_Undefined_Tokens;
extern int _SYSTEM_BIT_TYPE;

int Definer::Get_Location_Of_Type_Constructor(string type)
{
	int i = 0;
	for (Token* T : Input_Of_Tokens)
	{
		if (T->is(_Constructor_) && T->Name == type)
		{
			return i;
		}
		i++;
	}
	return -1;
}

int Definer::Get_Definition_Setting(Token* t, string f)
{
	for (Token* i : t->Right_Side_Token->Childs)
	{
		//go a double loop and make a vector for type settings like: Size, etc...
		//loop and find if that type constructor has correlating settings and set em.
		if (i->Type == f)
		{
			return atoi(i->Right_Side_Token->Name.c_str());
		}
	}
	return 0;
}

void Definer::Type_Collect(Token* t)
{
	if (t->is(_Inheritting_) != true)
		return;
	t->Size = FIND(t->Type)->Size + FIND(t->PreFix_Type)->Size;
	t->Static = FIND(t->Type)->Static + FIND(t->PreFix_Type)->Static;
}

Token* Definer::FIND(string name)
{
	for (int i = 0; i < Output.size(); i++)
	{
		if (Output.at(i)->Name == name)
		{
			return Output.at(i);
		}
	}
	for (int i = 0; i < Generated_Undefined_Tokens.size(); i++)
	{
		if (Generated_Undefined_Tokens.at(i)->Name == name)
		{
			return Generated_Undefined_Tokens.at(i);
		}
	}
	cout << "Error:: Invalid PreType: " + name + "." << endl;
	return nullptr;
}

bool Definer::Has(Token* t, string s)
{
	if (t->Right_Side_Token == nullptr)
	{
		return false;
	}
	for (Token* T : t->Right_Side_Token->Childs)
	{
		if (T->Name == s)
		{
			return true;
		}
	}
	return false;
}

void Definer::Factory()
{
	//type var()
	//(
	//  Size 4		#the size of var type is 4Bits.
	//	Static 0	#the value can change.
	//)
	//func main()
	//(
	//	return 0
	//)
	for (Token*s : Defined_Types)
	{
		Token* New_Defined_Class = new Token();
		//first lets find the defined types
		int i = Get_Location_Of_Type_Constructor(s->Name);
		if (i == -1)
		{
			continue;
		}
		New_Defined_Class->Size = Get_Definition_Setting(Input_Of_Tokens.at(i), "Size");
		New_Defined_Class->Static = Get_Definition_Setting(Input_Of_Tokens.at(i), "Static");
		New_Defined_Class->Name = s->Name;
		New_Defined_Class->Type = s->Type;
		if (Has(s, "return"))
		{
			New_Defined_Class->add(_Returning_);
		}
		Output.push_back(New_Defined_Class);
		/*for (Token* t : Generated_Undefined_Tokens)
		{
			if (t->Type == New_Defined_Class->Name)
			{
				t->Size = New_Defined_Class->Size;
				t->Static = New_Defined_Class->Static;
				//offset - Size -> all
			}
		}*/
	}
	for (Token* i : Generated_Undefined_Tokens)
	{
		//ADVANCED
		Type_Collect(i);
	}
}
