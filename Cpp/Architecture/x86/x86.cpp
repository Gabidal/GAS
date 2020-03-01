#include "..\..\..\H\Architecture\x86\x86.h"
#include "../../../H/Back/Token.h"


void x86::ARC_Factory()
{
	OpC* x86_ADD_ = new OpC(0, "+", "add ");
	OpCodes.push_back(x86_ADD_);
	OpC* x86_OR_ = new OpC(0, "|", "or ");
	OpCodes.push_back(x86_OR_);
	OpC* x86_AND_ = new OpC(0, "&", "and ");
	OpCodes.push_back(x86_AND_);
	OpC* x86_XOR_ = new OpC(0, "^", "xor ");
	OpCodes.push_back(x86_XOR_);
	OpC* x86_CMP_ = new OpC(0, "cmp", "cmp ");
	OpCodes.push_back(x86_CMP_);
	OpC* x86_Question_ = new OpC(0, "?", "cmp ");
	OpCodes.push_back(x86_Question_);
	OpC* x86_POP_ = new OpC(0, "pop", "pop ");
	OpCodes.push_back(x86_POP_);
	OpC* x86_PUSH_ = new OpC(0, "push", "push ");
	OpCodes.push_back(x86_PUSH_);
	OpC* x86_XCHG_ = new OpC(0, "xchg", "xchg ");
	OpCodes.push_back(x86_XCHG_);
	OpC* x86_LOAD_ = new OpC(0, "ldr", "mov ");
	OpCodes.push_back(x86_LOAD_);
	OpC* x86_STORE_ = new OpC(0, "str", "mov ");
	OpCodes.push_back(x86_STORE_);
	OpC* x86_SET_TO_ = new OpC(0, "=", "mov ");
	OpCodes.push_back(x86_SET_TO_);
	OpC* x86_LEA_ = new OpC(0, ":", "lea ");
	OpCodes.push_back(x86_LEA_);
	OpC* x86_SHL_ = new OpC(0, "<<", "shl ");
	OpCodes.push_back(x86_SHL_);
	OpC* x86_SHR_ = new OpC(0, ">>", "shr ");
	OpCodes.push_back(x86_SHR_);
	OpC* x86_NEG_ = new OpC(0, "neg", "neg ");
	OpCodes.push_back(x86_NEG_);
	OpC* x86_NOT_ = new OpC(0, "!", "not ");
	OpCodes.push_back(x86_NOT_);
	OpC* x86_CALL_ = new OpC(0, "call", "call ");
	OpCodes.push_back(x86_CALL_);
	OpC* x86_SUB_ = new OpC(0, "-", "sub ");
	OpCodes.push_back(x86_SUB_);
	OpC* x86_IMUL_ = new OpC(0, "*", "imul ");
	OpCodes.push_back(x86_IMUL_);
	OpC* x86_IDIV_ = new OpC(0, "*", "idiv ");
	OpCodes.push_back(x86_IDIV_);
	OpC* x86_JNE_ = new OpC(0, "==", "jne ");
	OpCodes.push_back(x86_JNE_);
	OpC* x86_JE_ = new OpC(0, "!=", "je ");
	OpCodes.push_back(x86_JE_);
	OpC* x86_JNLE_ = new OpC(0, "<=", "jnle ");
	OpCodes.push_back(x86_JNLE_);
	OpC* x86_JNGE_ = new OpC(0, ">=", "jnge ");
	OpCodes.push_back(x86_JNGE_);
	OpC* x86_JLE_ = new OpC(0, "!<", "jle ");
	OpCodes.push_back(x86_JLE_);
	OpC* x86_JGE_ = new OpC(0, "!>", "jge ");
	OpCodes.push_back(x86_JGE_);
	OpC* x86_JNL_ = new OpC(0, "<", "jnl ");
	OpCodes.push_back(x86_JNL_);
	OpC* x86_JNG_ = new OpC(0, ">", "jng ");
	OpCodes.push_back(x86_JNG_);
	OpC* x86_JMP_ = new OpC(0, "jmp", "jmp ");
	OpCodes.push_back(x86_JMP_);
	OpC* x86_RET_ = new OpC(0, "return", "ret ");
	OpCodes.push_back(x86_RET_);
	OpC* x86_DWORD_ = new OpC(0, "4", "dword ");
	OpCodes.push_back(x86_DWORD_);
	OpC* x86_QWORD_ = new OpC(0, "8", "qword ");
	OpCodes.push_back(x86_QWORD_);
	OpC* x86_WORD_ = new OpC(0, "2", "word ");
	OpCodes.push_back(x86_WORD_);
	OpC* x86_BYTE_ = new OpC(0, "1", "byte ");
	OpCodes.push_back(x86_BYTE_);
	OpC* x86_NULL_ = new OpC(0, "0", "");
	OpCodes.push_back(x86_NULL_);
	OpC* x86_LABEL_ = new OpC(0, "label", ": ");
	OpCodes.push_back(x86_LABEL_);
	OpC* x86_COMMENT_ = new OpC(0, "comment", "; ");
	OpCodes.push_back(x86_COMMENT_);
	OpC* x86_EXPORT_ = new OpC(0, "export", "global ");
	OpCodes.push_back(x86_EXPORT_);



	EAX = new Register("EAX ", 4, "r1");
	EAX->Flags |= Task_For_Returning;
	Registers32.push_back(EAX);
	EBX = new Register("EBX ", 4, "r2");
	Registers32.push_back(EBX);
	ECX = new Register("ECX ", 4, "r3");
	Registers32.push_back(ECX);
	EDX = new Register("EDX ", 4, "r4");
	EDX->Flags |= Task_For_Remainder;
	Registers32.push_back(EDX);
	EDI = new Register("EDI ", 4, "r5");
	EDI->Flags |= Task_For_Offsetting;
	Registers32.push_back(EDI);
	ESI = new Register("ESI ", 4, "r6");
	ESI->Flags |= Task_For_Offsetting;
	Registers32.push_back(ESI);
	ESP = new Register("ESP ", 4, "r7");
	ESP->Flags |= Task_For_Type_Address;
	Registers32.push_back(ESP);
	EBP = new Register("EBP ", 4, "r8");
	EBP->Flags |= Task_For_Type_Address_Basing;
	Registers32.push_back(EBP);
	//16Bit
	AX = new Register("AX ", 2, "r9");
	AX->Flags |= Task_For_Returning;
	Registers16.push_back(AX);
	BX = new Register("BX ", 2, "r10");
	Registers16.push_back(BX);
	CX = new Register("CX ", 2, "r11");
	Registers16.push_back(CX);
	DX = new Register("DX ", 2, "r12");
	DX->Flags |= Task_For_Remainder;
	Registers16.push_back(DX);
	DI = new Register("DI ", 2, "r13");
	DI->Flags |= Task_For_Offsetting;
	Registers16.push_back(DI);
	SI = new Register("SI ", 2, "r14");
	SI->Flags |= Task_For_Offsetting;
	Registers16.push_back(SI);
	SP = new Register("SP ", 2, "r15");
	SP->Flags |= Task_For_Type_Address;
	Registers16.push_back(SP);
	BP = new Register("BP ", 2, "r16");
	BP->Flags |= Task_For_Type_Address_Basing;
	Registers16.push_back(BP);
	//8Bit
	AL = new Register("AL ", 1, "r17");
	AL->Flags |= Task_For_Returning;
	Registers8.push_back(AL);
	AH = new Register("AH ", 1, "r18");
	AH->Flags |= Task_For_Returning;
	Registers8.push_back(AH);
	BL = new Register("BL ", 1, "r19");
	Registers8.push_back(BL);
	BH = new Register("BH ", 1, "r20");
	Registers8.push_back(BH);
	CL = new Register("CL ", 1, "r21");
	Registers8.push_back(CL);
	CH = new Register("CH ", 1, "r22");
	Registers8.push_back(CH);
	DL = new Register("DL ", 1, "r23");
	DL->Flags |= Task_For_Remainder;
	Registers8.push_back(DL);
	DH = new Register("DH ", 1, "r24");
	DH->Flags |= Task_For_Remainder;
	Registers8.push_back(DH);

}
