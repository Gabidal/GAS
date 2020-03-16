#ifndef _PARSER_H_
#define _PARSER_H_
#include <vector>
#include <string>
#include "../Lexer/Word.h"
#include "../Back/Token.h"
#include "../Lexer/Lexer.h"
#include <algorithm>
using namespace std;

class Parser
{
public:

    int Space_Reservation = 0;
	string Working_Dir = "";
    vector<Word*> Input;
    vector<Token*> Output;
    vector<Token*> Defined_Keywords;
    void Include_Files(int i);
    void Connect_Array(int i);
    void Init_Definition(int i);
    int Get_Size(int i);
    void Init_Operator(int i);
    void Reserve_Operator_Tokens(int i);
    void Reserve_Function_Parameters(int i);
    void Patternize_Operations(int& i, string f);
    void Do_In_Order();
    void Init_Parenthesis(int i);
    void Init_Conditions(int i);
    void Type_Definition(int i);
    void Set_Right_Stack_Offset(Token* t);
    void Set_Right_Flag_Info(Token* t);
    string Defined(string name);
    int Count_Familiar_Tokens(int F, int i);
    void Init_Variable(int i);
    void Check_For_Correlation(int i);
    void Set_Special_Feature(int i);
    void Factory();
    void Append(vector<Token*>* Dest, vector<Token*> Source);
    Parser &operator=(const Parser& other)
    {
        Working_Dir = other.Working_Dir;
        //Input = other.Input;
        //Output = other.Output;
        Defined_Keywords = other.Defined_Keywords;
        return *this;
    }
    ~Parser()
    {
    }
};

#endif