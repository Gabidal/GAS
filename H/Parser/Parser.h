#ifndef _PARSER_H_
#define _PARSER_H_
#include <vector>
#include <string>
#include "../Lexer/Word.h"
#include "../Back/Token.h"
using namespace std;

class Parser
{
public:
    Token *ParentType = 0;
    Token *ParentFunc = 0;
    bool InsideOfType = false;
    bool InsideOfFunction = false;
    string Assembly = "";
    vector<Word> Input;
    vector<Token> Output;
    void Pattern_Variable(int i);
    void Pattern_Equal(int i);
    void Pattern_Array(int i);
    void Pattern_Locator(int i);
    void Pattern_Function(int i);
    void Factory();
    Parser()
    {

    }
    
    ~Parser()
    {
    }
};

#endif