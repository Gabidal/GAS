#include "../../H/Parser/Parser.h"

void Parser::Pattern_Init_Variable(int i)
{
    //((var) (a) (:) (b) (= 123))
    if (Input.at(i).is(_KEYWORD) && Input.at(i+1).is(_TEXT) && Input.at(i).WORD == "var" && Input.at(i).UsedToken == false)
    {
        //create new variable;
        Token Var(Assembly);
        Var.Flags |= Variable;
        Var.Flags |= Real;
        Var.Name = Input.at(i+1).WORD;
        if (InsideOfType && InsideOfFunction != true)
        {
            Var.Flags |= Member;
            ParentType->addChild(Var);
        }
        else if (InsideOfFunction)
        {
            Var.Flags |= Private;
            ParentFunc->addChild(Var);
        }
        else
        {
            Output.push_back(Var);
        }
        Input.at(i).UsedToken = true;
    }
}

void Parser::Pattern_Variable(int i)
{
    vector<Token> *T;
    if ((Input.at(i).is(_TEXT) == false) && (Input.at(i).is(_NUMBER) == false))
    {
        return;
    }

    Give_Output(T);
    
    if (Input.at(i).is(_TEXT) && Find(Input.at(i).WORD, Variable, *T) != -1)
    {
        int j = Find(Input.at(i).WORD, Variable, *T);
        if ((i > 0 && Input.at(i-1).WORD != "var") || Started != 0)
        {
            T->at(j).Flags |= Used;
        }
        Token t = T->at(j);
        if (Input.at(i).Offsetter != 0)
        {
            //if it is an array
            //if the offsetter is a Variable
            int k = Find(Input.at(i).Offsetter->WORD, Variable, *T);
            if (k == -1)
            {
                //if the offsetter is a number
                k = Find(Input.at(i).Offsetter->WORD, Number, *T);
            }
            if (k == -1)
            {
                //if the offsetter is a function
                k = Find(Input.at(i).Offsetter->WORD, Call, *T);
            }
            
            Token *ofsetter = &T->at(k);
            t.Offsetter = ofsetter;
            t.Flags |= Array;
        }
        if (Priority)
        {
            T->push_back(t);
        }
        
    }
    if (Input.at(i).is(_NUMBER))
    {
        Token n(Assembly);
        n.Flags |= Number & Real & Used;
        n.Name = Input.at(i).WORD;
        if (Priority)
        {
            T->push_back(n);
        }
    }
}

void Parser::Pattern_Operators(int i)
{
    //set variable value;
    //a <: a> = b
    //c = a<()> + b
    if (Input.at(i).is(_OPERATOR) && Input.at(i).Tokens.size() > 0 && Input.at(i).UsedToken == true)
    {
        Token OP(Assembly);
        OP.Flags |= OPERATOR;
        OP.Name = Input.at(i).WORD;

        Parser p = *this;
        p.Input = Input.at(i).Tokens;
        p.Started = Input.at(i).Tokens.size();
        p.Priority = true;
        
        p.Factory();
        //check this also in debugging!!!
        Token A(Assembly);
        Token B(Assembly);
        if (InsideOfFunction)
        {
            A = ParentFunc->Childs.at(ParentFunc->Childs.size() - 2);
            B = ParentFunc->Childs.at(ParentFunc->Childs.size() - 1);
            ParentFunc->Childs.erase(ParentFunc->Childs.begin() + ParentFunc->Childs.size() - 1);
            ParentFunc->Childs.erase(ParentFunc->Childs.begin() + ParentFunc->Childs.size() - 1);
        }
        else if (InsideOfType)
        {
            A = ParentType->Childs.at(ParentType->Childs.size() - 2);
            B = ParentType->Childs.at(ParentType->Childs.size() - 1);
            ParentType->Childs.erase(ParentType->Childs.begin() + ParentType->Childs.size() - 1);
            ParentType->Childs.erase(ParentType->Childs.begin() + ParentType->Childs.size() - 1);
        }
        else
        {
            A = p.Output.at(p.Started + 0);
            B = p.Output.at(p.Started + 1);
            p.Output.erase(p.Output.begin() + p.Started - 1);
            p.Output.erase(p.Output.begin() + p.Started - 1);
        }
        
        OP.Parameters.push_back(A);
        OP.Childs.push_back(B);
        
        vector<Token> *T;
        Give_Output(T);
        T->push_back(OP);
    }
}

void Parser::Pattern_Init_Array(int i)
{
    // a : a
    int Mark;
    int Offset;
    int _Owner;
    if (i == Input.size() - 1)
    {
        return;
    }
    else if (i < 1)
    {
        return;
    }
    
    if (Input.at(i+1).WORD == ":")
    {
        _Owner = i;
        Mark = i+1;
        Offset = i+2;
    }
    else if (Input.at(i-1).WORD == ":")
    {
        _Owner = i-2;
        Mark = i-1;
        Offset = i;
    }
    else if (Input.at(i).WORD == ":")
    {
        _Owner = i-1;
        Mark = i;
        Offset = i+1;
    }
    else
    {
        return;
    }
    if (Input.at(Mark).UsedToken == false)
    {
        Input.at(_Owner).Offsetter = &Input.at(Offset);
        
        Input.erase(Input.begin() + Mark);
        Input.erase(Input.begin() + Offset);
    }
}

/*void Parser::Pattern_Init_Locator(int i)
{
    if (Input.at(i).WORD == "&")
    {
        Token ptr(Assembly);
        ptr.Flags |= Ptr;
        ptr.Name = Input.at(i+1).WORD;
        Output.push_back(ptr);
    }
}*/

void Parser::Pattern_Function(int i)
{
    //func banana(a, &b, c, &d)
    if (Input.at(i).is(_KEYWORD) && Input.at(i).WORD == "func")
    {
        //make new Function;
        Token Name(Assembly);
        Name.Flags |= Function;
        Name.Flags |= Real;
        Name.Name = Input.at(i+1).WORD;
        if (InsideOfType)
        {
            Name.ParentType = ParentType;
            Name.Flags |= Member;
        }
        
        //make the parameters;
        for (int j = 0; j < Input.at(i+2).Tokens.size(); j++)
        {
            if (Input.at(i+2).Tokens.at(j).WORD == "&")
            {
                Token ptr(Assembly);
                ptr.Flags |= Ptr;
                ptr.Flags |= Parameter;
                ptr.Size = 4;
                ptr.Name = Input.at(i+2).Tokens.at(j+1).WORD;
                Name.addParameter(ptr);
                j++;
            }
            else if (Input.at(i+2).Tokens.at(j).is(_TEXT))
            {
                Token var(Assembly);
                var.Flags |= Parameter;
                var.Flags |= Variable;
                var.Size = 4;
                var.Name = Input.at(i+2).Tokens.at(j).WORD;
                
                Name.addParameter(var);
            }
        }
        Input.erase(Input.begin() + i + 2);
        vector<Token> *T;
        Give_Output(T);
        T->push_back(Name);
    }
    
}

void Parser::Pattern_Type(int i)
{
    //type banana
    if (Input.at(i).WORD == "type")
    {
        Token Type(Assembly);
        Type.Flags |= TypE;
        Type.Flags |= Real;
        Type.Name = Input.at(i+1).WORD;
        vector<Token> *T;
        T->push_back(Type);
    }
}

void Parser::Pattern_Parenthesis(int i)
{
    /*func banana <deleted () > \n
    (
        var c = a + a
        b = d + c
    )*/
    if (Input.at(i).is(_PAREHTHESIS) && Input.at(i-3).WORD == "func")
    {
        vector<Token> *T;
        Give_Output(T);
        int j = Find(Input.at(i-2).WORD, Function, *T);
        ParentFunc = &Output.at(j);

        Layer++;
        Parser parser = *this;
        parser.Input = Input.at(i).Tokens;
        parser.InsideOfFunction = true;
        parser.Started = Output.size();
        parser.Factory();
        ParentFunc->Flags |= PARENT;
        Layer--;
    }
    /*type banana
    (
        var 1
    )*/
    else if (Input.at(i).is(_PAREHTHESIS) && Input.at(i-3).WORD == "type")
    {
        vector<Token> *T;
        Give_Output(T);

        int j = Find(Input.at(i-2).WORD, TypE, *T);
        ParentType = &Output.at(j);

        Layer++;
        Parser parser = *this;
        parser.Input = Input.at(i).Tokens;
        parser.InsideOfType = true;
        parser.Started = Output.size();
        parser.Factory();
        ParentType->Flags |= PARENT;
        Layer--;
    }
    /*if ( a == b & a == c)
    (
        var 1
    )*/
    else if (Input.at(i).is(_PAREHTHESIS) && Input.at(i-1).WORD == "if")
    {
        Token IF(Assembly);
        IF.Flags |= PARENT & If & Used;
        IF.Name = to_string(ID) + "_if";

        Parser parser1(Input.at(i).Tokens, Output);

        for (int i = parser1.Started; i < parser1.Output.size(); i++)
        IF.addParameter(parser1.Output.at(i));

        Parser parser2(Input.at(i+2).Tokens);

        for (int i = parser2.Started; i < parser2.Output.size(); i++)
        IF.addChild(parser2.Output.at(i));

        vector<Token> *T;
        T->push_back(IF);
    }
    /*while (a < b)
    (
        var 1
    )*/
    else if (Input.at(i).is(_PAREHTHESIS) && Input.at(i-1).WORD == "while")
    {
        Token WHILE(Assembly);
        WHILE.Flags |= PARENT & If & Used;
        WHILE.Name = to_string(ID) + "_while";

        Parser parser1(Input.at(i).Tokens);

        for (int i = 0; i < parser1.Output.size(); i++)
        WHILE.addParameter(parser1.Output.at(i));

        Parser parser2(Input.at(i+2).Tokens);

        for (int i = 0; i < parser2.Output.size(); i++)
        WHILE.addChild(parser2.Output.at(i));

        vector<Token> *T;
        T->push_back(WHILE);
    }
    
}

void Parser::Pattern_Init_Operators(int &i)
{
    //<a = b>
    if (Input.at(i).is(_OPERATOR) && Input.at(i).UsedToken != true)
    {
        Input.at(i).Tokens.push_back(Input.at(i-1)); //a
        Input.at(i).Tokens.push_back(Input.at(i+1)); //b
        Input.erase(Input.begin() + i-1);
        Input.erase(Input.begin() + i);
        Input.at(i-1).UsedToken = true;
        i--;
    }
}

void Parser::Pattern_Init_Call_Func(int i)
{
    //banana(1, 2)
    if (Input.at(i).is(_PAREHTHESIS) && Input.at(i-1).is(_TEXT) && Input.at(i-2).WORD != "func")
    {
        Input.at(i).WORD = Input.at(i-1).WORD;
        Input.at(i)._func = true;
        Input.at(i)._type = Input.at(i-1)._type;
        Input.erase(Input.begin() + i-1);
    }
}

void Parser::Pattern_Call_Func(int i)
{
    if (Input.at(i).is(_PAREHTHESIS) && Input.at(i)._func)
    {
        Token func(Assembly);
        func.Flags |= Call && Function;
        func.Name = Input.at(i).WORD;
        vector<Token> *t;
        if (ParentType != 0)
        {
            t = &ParentType->Childs;
        }
        else if (ParentType == 0)
        {
            t = &Output;
        }
        if (Input.at(i)._type)
        {
            func.Flags |= This;
            int j = Find(Input.at(i).Fetcher, TypE, Output);
            func.ParentType = &Output.at(j);
        }
        

        if (Find(func.Name, Function, *t) != -1)
        {
            int j = Find(func.Name, Function, *t);
            t->at(j).Flags |= Used;
        }

        for (int j = 0; j < Input.at(i).Tokens.size(); j++)
        {
            if (Input.at(i+2).Tokens.at(j).WORD == "&")
            {
                Token ptr(Assembly);
                ptr.Flags |= Ptr;
                ptr.Flags |= Parameter;
                ptr.Size = 4;
                ptr.Name = Input.at(i+2).Tokens.at(j+1).WORD;
                func.addParameter(ptr);
                j++;
            }
            else if (Input.at(i+2).Tokens.at(j).is(_TEXT))
            {
                Token var(Assembly);
                var.Flags |= Parameter;
                var.Flags |= Variable;
                var.Size = 4;
                var.Name = Input.at(i+2).Tokens.at(j).WORD;
                
                func.addParameter(var);
            }
        }
        
        t->push_back(func);
    }
    
}

void Parser::Pattern_Init_Type(int i)
{
    /*type banana
    (
        var a = 1
        apple(a, b, c)
    )
    new banana b
    <b.apple>(a, b, c)
    <b.a> : <b.a> = <b.a> : <b.a>
    */
    if (Input.at(i).is(_OPERATOR) && Input.at(i).WORD == ".")
    {
        Input.at(i+1)._type = true;
        Input.at(i+1).Fetcher = Input.at(i-1).WORD;
        Input.erase(Input.begin() + i);
        Input.erase(Input.begin() + i-1);
    }
}

void Parser::Pattern_New(int i)
{
    /*
    new banana b
    */
    if (Input.at(i).is(_KEYWORD) && Input.at(i).WORD == "new")
    {
        vector<Token> *T;
        Give_Output(T);
        int j = Find(Input.at(i+1).WORD, TypE, *T);
        Token t = T->at(j);
        t.Name = Input.at(i+2).WORD;
        t.Origin = &T->at(j);
        t.Flags |= __NEW & NotOriginal;
        T->at(j).Flags |= Used;
        Input.erase(Input.begin() + i);
        Input.erase(Input.begin() + i + 1);
        Input.erase(Input.begin() + i + 2);
    }
}

int Parser::Find(string name, int flag, vector<Token> list)
{
    for (int i = 0; i < list.size(); i++)
    {
        if (list.at(i).Name == name && list.at(i).is(flag))
        {
            return i;
        }
    }
    return -1;
}

void Parser::Give_Output(vector<Token> *&T)
{
    if (InsideOfFunction)
    {
        if (ParentFunc->Parameters.size() > 0)
        {
            T = new vector<Token>;
            T->reserve(ParentFunc->Parameters.size() + ParentFunc->Childs.size());
            T->insert(T->end(), ParentFunc->Parameters.begin(), ParentFunc->Parameters.end());
            T->insert(T->end(), ParentFunc->Childs.begin(), ParentFunc->Childs.end());
        }
        else
        {
            T = &ParentFunc->Childs;
        }
        
    }
    else if (InsideOfType)
    {
        T = &ParentType->Childs;
    }
    else
    {
        T = &Output;
    }
}

void Parser::Factory()
{
    for (int i = 0; i < Input.size(); i++)
    {
        //for Inits;
        Pattern_Init_Type(i);
        Pattern_Init_Call_Func(i);
        Pattern_Init_Array(i);
        Pattern_Init_Variable(i);
        Pattern_Init_Operators(i);
    }
    for (int i = 0; i < Input.size(); i++)
    {
        //for Makers;
        Pattern_New(i);
        Pattern_Function(i);
        Pattern_Type(i);
        Pattern_Parenthesis(i);
        Pattern_Call_Func(i);
        Pattern_Operators(i);
        Pattern_Variable(i);
    }
}