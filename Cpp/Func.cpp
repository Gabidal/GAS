#include "H/Func.h"

void Father::addChild(Token *local) 
{
    Childs.push_back(local);
    local->ParentFunc = this;
    local->StackOffset = this->StackOffset;
    this->StackOffset += local->Size;
	local->Flags |= Private;
}

void Func::addParameter(Token *Param)
{
    Parameters.push_back(Param);
    Param->ParameterOffset = this->ParameterOffset;
    this->ParameterOffset += Param->Size;
}

Func::Func(bool getsThisAsParameter)
{
    if (getsThisAsParameter)
    {
        this->Flags |= This;
    }
}