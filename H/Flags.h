#ifndef _FLAGS_H_
#define _FLAGS_H_

namespace TOKEN {
	constexpr long REGISTER = 1 << 0;	//this is for that the selector can select an register if it sees this
	constexpr long VOLATILE = 1 << 1;
	constexpr long NONVOLATILE = 1 << 2;
	constexpr long RETURNING = 1 << 3;
	constexpr long PARAMETER = 1 << 4;	//fast call
	constexpr long REMAINDER = 1 << 5;
	constexpr long QUOTIENT = 1 << 6;

	constexpr long NUM = 1 << 7;
	constexpr long DECIMAL = 1 << 8;

	constexpr long MEMORY = 1 << 9;

	constexpr long OPERATOR = 1 << 10;	//any operator
	constexpr long SCALER = 1 << 11;		//*
	constexpr long OFFSETTER = 1 << 12;	//+
	constexpr long DEOFFSETTER = 1 << 13;	//+

	constexpr long CONTENT = 1 << 14;	//any childs owner
	constexpr long LABEL = 1 << 15;

	constexpr long FLOW = 1 << 16; //ret, jmp
	constexpr long CALL = 1 << 17; // calls

	//RULES
	constexpr long ALL_ARGS_SAME_SIZE = 1 << 18;	//this means that all args on the OPCODE must be same size
	constexpr long OPTIONAL = 1 << 19;
	constexpr long INF = 1 << 20;
	constexpr long UN_ORDERED = 1 << 21;
}

namespace Flags{
	//the main flags that LEXER uses
	constexpr long KEYWORD_COMPONENT	 = 1 << 0;
	constexpr long UNDEFINED_COMPONENT	 = 1 << 1;
	constexpr long PAREHTHESIS_COMPONENT = 1 << 2;
	constexpr long END_COMPONENT		 = 1 << 3;
	constexpr long STRING_COMPONENT		 = 1 << 4;
	constexpr long NUMBER_COMPONENT		 = 1 << 5;
	constexpr long COMMENT_COMPONENT	 = 1 << 6;
	constexpr long OPERATOR_COMPONENT	 = 1 << 7;
	constexpr long TEXT_COMPONENT		 = 1 << 8;
}

enum Node_Type {
	CONTENT_NODE,
	ELSE_IF_NODE,
	ELSE_NODE,
	IF_NODE,
	WHILE_NODE,
	FUNCTION_NODE,
	CALL_NODE,
	PROTOTYPE,
	FLOW_NODE,
	PTR_NODE,
	NUMBER_NODE,

	OPERATOR_NODE,				//classical operators like +-/*
	ASSIGN_OPERATOR_NODE,
	CONDITION_OPERATOR_NODE,	//==, !=, <, >
	BIT_OPERATOR_NODE,			// &, |, �, <<, >>
	ARRAY_NODE,					//a[1]

	STRING_NODE,
	CLASS_NODE,
	OBJECT_NODE,
	ANY,
	TEMPLATE_NODE,
	KEYWORD_NODE,
	PREFIX_NODE,
	POSTFIX_NODE,
	OBJECT_DEFINTION_NODE,
	LABEL_NODE
};

#endif