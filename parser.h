#ifndef _PARSER_H_
#define _PARSER_H_
#include <string>
#include <vector>
#include <map>
using namespace std;
string parameters;
string varbuffer;
string codbuffer;
string texbuffer;
string includes;
int usedregister = 0;
string regbuffer;
int inLayer = 0;
int layerId = 0;
string elsedEndString[256];
vector<string> variables;
map <string, string> varPointers;
vector<string> functions;

extern int getWord(char, string&, string, int);
extern int getReversedIndex(char, string, int);
extern int getReversedWord(char, string&, string, int);
extern string readFile(string name);

string sx()
{
    string spaces = "";
    for (int i = 0; i < inLayer; i++)
    {
        spaces += " ";
    }
    return spaces;
}

void getFreeReg()
{
    if (usedregister == 3)
    {
        regbuffer = "edx ";
        usedregister = 0;
    }
    else if (usedregister == 2)
    {
        regbuffer = "ecx ";
        usedregister = 3;
    }
    else if (usedregister == 1)
    {
        regbuffer = "ebx ";
        usedregister = 2;
    }
    else if (usedregister == 0)
    {
        regbuffer = "eax ";
        usedregister = 1;
    }
}

void doAddition(int &index)
{
    int offset;
    string para1;  //b
    string para2;  //c
    string para3;  //a
    string trash;
    string trash1;
    string trash2;
    // a       = b + c
    offset = getReversedIndex(' ', parameters, index-1); //trash offset
    offset = getReversedWord(' ', trash, parameters, offset); //trash

    offset = getReversedIndex(' ', parameters, offset); //b offset
    offset = getReversedWord(' ', para1, parameters, offset); //b 

    offset = getReversedIndex(' ', parameters, offset); //trash offset
    offset = getReversedWord(' ', trash, parameters, offset); //trash

    offset = getReversedIndex(' ', parameters, offset); //a offset
    offset = getReversedWord(' ', para3, parameters, offset); //a
    
    index = getWord(' ', para2, parameters, index);  // c
    auto node = varPointers.find(para1);
    if (node != varPointers.end())
    {
        auto node1 = varPointers.find(para2);
        if (node1 != varPointers.end())
        {
            codbuffer += sx() + "add " + node->second + ", " + node1->second + "\n";
            codbuffer += sx() + "mov [" + para3 + "], " + node->second + "\n";
        }
        else
        {
            getFreeReg();
            string reg1 = regbuffer;
            codbuffer += sx() + "mov " + reg1 + ", dword [" + para2 + "]\n";
            codbuffer += sx() +  "add " + node->second + ", " + reg1 + "\n";
            codbuffer += sx() +  "mov [" + para3 + "], " + node->second + "\n";
            varPointers.insert(make_pair(para2, reg1));
        }
    }
    else
    {
        getFreeReg();
        trash1 = regbuffer;
        codbuffer += sx() +  "mov " + trash1 + ", dword [" + para1 + "]\n";
        getFreeReg();
        trash2 = regbuffer;
        codbuffer += sx() +  "mov " + trash2 + ", dword [" + para2 + "]\n";
        codbuffer += sx() +  "add " + trash1 + ", " + trash2 + "\n";
        codbuffer += sx() +  "mov [" + para3 + "], " + trash1 + "\n";
            varPointers.insert(make_pair(para1, trash1));
            varPointers.insert(make_pair(para2, trash2));
    }
}

void doSubstraction(int &index)
{
    int offset;
    string para1;  //b
    string para2;  //c
    string para3;  //a
    string trash;
    string trash1;
    string trash2;
    // a       = b + c
    offset = getReversedIndex(' ', parameters, index-1); //trash offset
    offset = getReversedWord(' ', trash, parameters, offset); //trash

    offset = getReversedIndex(' ', parameters, offset); //b offset
    offset = getReversedWord(' ', para1, parameters, offset); //b 

    offset = getReversedIndex(' ', parameters, offset); //trash offset
    offset = getReversedWord(' ', trash, parameters, offset); //trash

    offset = getReversedIndex(' ', parameters, offset); //a offset
    offset = getReversedWord(' ', para3, parameters, offset); //a
    
    index = getWord(' ', para2, parameters, index);  // c
    auto node = varPointers.find(para1);
    if (node != varPointers.end())
    {
        auto node1 = varPointers.find(para2);
        if (node1 != varPointers.end())
        {
            codbuffer += sx() +  "sub " + node->second + ", " + node1->second + "\n";
            codbuffer += sx() +  "mov [" + para3 + "], " + node1->second + "\n";
        }
        else
        {
            getFreeReg();
            string reg1 = regbuffer;
            codbuffer += sx() +  "mov " + reg1 + ", dword [" + para2 + "]\n";
            codbuffer += sx() +  "sub " + node->second + ", " + reg1 + "\n";
            codbuffer += sx() +  "mov [" + para3 + "], " + node->second + "\n";
            varPointers.insert(make_pair(para2, reg1));
        }
    }
    else
    {
        getFreeReg();
        trash1 = regbuffer;
        codbuffer += sx() +  "mov " + trash1 + ", dword [" + para1 + "]\n";
        getFreeReg();
        trash2 = regbuffer;
        codbuffer += sx() +  "mov " + trash2 + ", dword [" + para2 + "]\n";
        codbuffer += sx() +  "sub " + trash1 + ", " + trash2 + "\n";
        codbuffer += sx() +  "mov [" + para3 + "], " + trash1 + "\n";
            varPointers.insert(make_pair(para1, trash1));
            varPointers.insert(make_pair(para2, trash2));
    }
}

void doMultiply(int &index)
{
    int offset;
    string para1;  //b
    string para2;  //c
    string para3;  //a
    string trash;
    string trash1;
    string trash2;
    // a       = b + c
    offset = getReversedIndex(' ', parameters, index-1); //trash offset
    offset = getReversedWord(' ', trash, parameters, offset); //trash

    offset = getReversedIndex(' ', parameters, offset); //b offset
    offset = getReversedWord(' ', para1, parameters, offset); //b 

    offset = getReversedIndex(' ', parameters, offset); //trash offset
    offset = getReversedWord(' ', trash, parameters, offset); //trash

    offset = getReversedIndex(' ', parameters, offset); //a offset
    offset = getReversedWord(' ', para3, parameters, offset); //a
    
    index = getWord(' ', para2, parameters, index);  // c
    auto node = varPointers.find(para1);
    if (node != varPointers.end())
    {
        auto node1 = varPointers.find(para2);
        if (node1 != varPointers.end())
        {
            codbuffer += sx() +  "mov eax, " + node->second + "\n";
            codbuffer += sx() +  "mul " + node->second + "\n";
            codbuffer += sx() +  "mov [" + para3 + "], eax\n";
            codbuffer += sx() +  "mov " + node1->second + ", eax\n";
        }
        else
        {
            getFreeReg();
            string reg1 = regbuffer;
            codbuffer += sx() +  "mov eax, " + node->second + "\n";
            codbuffer += sx() +  "mov " + reg1 + ", dword [" + para2 + "]\n";
            codbuffer += sx() +  "mul " + reg1 + "\n";
            codbuffer += sx() +  "mov [" + para3 + "], eax\n";
            codbuffer += sx() +  "mov " + reg1 + ", eax\n";
            varPointers.insert(make_pair(para2, reg1));

        }
    }
    else
    {
        getFreeReg();
        trash1 = regbuffer;
        codbuffer += sx() +  "mov " + trash1 + ", dword [" + para1 + "]\n";
        getFreeReg();
        trash2 = regbuffer;
        codbuffer += sx() +  "mov " + trash2 + ", dword [" + para2 + "]\n";
            codbuffer += sx() +  "mov eax, " + trash1 + "\n";
            codbuffer += sx() +  "mul " + trash2 + "\n";
            codbuffer += sx() +  "mov [" + para3 + "], eax\n";
            codbuffer += sx() +  "mov " + trash2 + ", eax\n";
            varPointers.insert(make_pair(para1, trash1));
            varPointers.insert(make_pair(para2, trash2));
    }
}

void makeVar(int &index)
{
    string para1;
    string para2;
    string para3;
    index = getWord(' ', para1, parameters, index);
    index = getWord(' ', para2, parameters, index);
    index = getWord(' ', para3, parameters, index);
    varbuffer += para1 + " dd " + para3 + "\n";
    variables.push_back(para1);
}

void useVar(int &index, string destination)
{//mov reg, dword [b]
 //mov [a], reg
    string para1;  //register
    string para2;  //[b]
    index = getWord(' ', para2, parameters, index); //skip the '='
    para2 = "";
    index = getWord(' ', para2, parameters, index); //get the [b]
    getFreeReg();
    para1 = regbuffer;
    auto varOnReg = varPointers.find(para2);
    if (varOnReg != varPointers.end())
    {
        string para3;
        para3 = varOnReg->second;
        codbuffer += sx() +  "mov [" + destination + "] , " + para3 + "\n";
    }
    else
    {
    codbuffer += sx() +  "mov " + para1 + ", dword [" + para2 + "]\n";
    codbuffer += sx() +  "mov [" + destination + "] , " + para1 + "\n";
    varPointers.insert(make_pair(para2, para1));
    varPointers.insert(make_pair(destination, para1));

    }
}

void doReturn()
{
    codbuffer += sx() +  "ret\n\n";
}
void makeFunc(int &index)
{
    string para1;
    index = getWord(' ', para1, parameters, index);
    codbuffer += sx() +  para1 + ":\n";
    functions.push_back(para1);
}

void callFunction(string function)
{
    codbuffer += sx() +   "call " + function + "\n";
}

void setVar(int &continu)
{
    
}

string getJump(string condition)
{
    if (condition == "==")
    {
        return "jne ";
    }
    if (condition == "!=")
    {
        return "je ";
    }
    if (condition == ">")
    {
        return "jle ";
    }
    if (condition == "<")
    {
        return "jge ";
    }
    if (condition == "!>")
    {
        return "jg ";
    }
    if (condition == "!<")
    {
        return "jl ";
    }
    if (condition == "=>")
    {
        return "jl ";
    }
    if (condition == "=<")
    {
        return "jg ";
    }
    else
    {
        return "jmp ";
    }
}

void doComparing(int &i)
{
    string para1;  //a
    string para2;  // ==
    string para3;  //b
    i = getWord(' ', para1, parameters, i);
    i = getWord(' ', para2, parameters, i);
    i = getWord(' ', para3, parameters, i);
    para2 = getJump(para2);
    auto reg1 = varPointers.find(para1);
    auto reg2 = varPointers.find(para3);
    if (reg1 != varPointers.end())
    {
        if (reg2 != varPointers.end())
        {
            codbuffer += sx() +   "cmp " + reg1->second + ", " + reg2->second + "\n";
        }
        else
        {
            getFreeReg();
            para1 = regbuffer;
            codbuffer += sx() +   "mov " + para1 + ", dword [" + para3 + "]\n";
            codbuffer += sx() +   "cmp " + reg1->second + ", " + para1 + "\n";
            varPointers.insert(make_pair(para3, para1));
        }
    }
    else
    {
        getFreeReg();
        string reg3 = regbuffer;
        getFreeReg();
        string reg4 = regbuffer;
        codbuffer += sx() +   "mov " + reg3 + ", dword [" + para1 + "]\n";
        codbuffer += sx() +   "mov " + reg4 + ", dword [" + para3 + "]\n";
        codbuffer += sx() +   "cmp " + reg3 + ", " + reg4 + "\n";
        varPointers.insert(make_pair(para1, reg3));
        varPointers.insert(make_pair(para3, reg4));
    }
    inLayer++;
    layerId++;
    codbuffer += sx() +   para2 + "else" + to_string(inLayer) + to_string(layerId) + "\n";
}


void doElse()
{
    codbuffer += sx() +   "jmp end" + to_string(inLayer) + to_string(layerId) + "\n";
    codbuffer += sx() +   "else" + to_string(inLayer) + to_string(layerId) + ": \n";
    elsedEndString[inLayer] = "end" + to_string(inLayer) + to_string(layerId) + ": \n";
}

void doEnd()
{
    codbuffer += sx() + elsedEndString[inLayer];
    inLayer--;
}

bool replace(string& str, const string& from, const string& to) {
    size_t start_pos = str.find(from);
    if(start_pos == string::npos)
        return false;
    str.replace(start_pos, from.length(), to);
    return true;
}

void doInclude(int &i)
{
    int offset;
    string including;
    //string name;
    i -= 4;
    i = getWord('\n', including, parameters, i);
    replace(parameters, including, "");
    replace(including, "use ", "");
    includes = readFile(including);
    i = 0;
}

void parser(string destination, string &file, int &continu, string &varbuffer1, string &codbuffer1, string &texbuffer1, string &includes1)
{
    codbuffer = "";
    texbuffer = "";
    varbuffer = "";
    parameters = file;
    if (destination == "var")
    {
        makeVar(continu);
    }
    if (destination == "func")
    {
        makeFunc(continu);
    }
    if (destination == "+")
    {
        doAddition(continu);
    }
    if (destination == "-")
    {
        doSubstraction(continu);
    }
    if (destination == "*")
    {
        for (auto it = varPointers.rbegin(); it != varPointers.rend(); it++) {
            if (it->second == "eax ")
            {
                varPointers.erase(it->second);
            }
        }

        for (auto node : varPointers) 
        {
            
        }
        doMultiply(continu);
    }
    if (destination == "ret")
    {
        doReturn();
    }
    if (destination == "if")
    {
        doComparing(continu);
    }
    if (destination == "else")
    {
        doElse();
    }
    if (destination == "end")
    {
        doEnd();
    }
    if (destination == "use")
    {
        doInclude(continu);
    }
    

    //...

    for (int i = 0; i < variables.size();i++)
    {
        if (variables[i] == destination)
        {
            useVar(continu, destination);
        }
    }
    for (int i = 0; i < functions.size();i++)
    {
        if (functions[i] == destination)
        {
            callFunction(destination);
        }
    }
    file = ::parameters;
    varbuffer1 += ::varbuffer;
    codbuffer1 += ::codbuffer;
    texbuffer1 += ::texbuffer;
    includes1 += ::includes;
    includes = "";
}

#endif