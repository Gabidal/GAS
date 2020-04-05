#include "../../../H/Architecture/x86/x86.h"
#include "../../../H/Back/Token.h"


void x86::ARC_Factory()
{
	OpC* x86_ADD_ = new OpC(0, "+", "add ", { 1, 8, 1, 8 });
	OpCodes.push_back(x86_ADD_);
	OpC* x86_ADDF_ = new OpC(0, "+", "addss ", { 12, 12, 12, 12});
	OpCodes.push_back(x86_ADDF_);
	OpC* x86_OR_ = new OpC(0, "|", "or ", { 1, 8, 1, 8 });
	OpCodes.push_back(x86_OR_);
	OpC* x86_AND_ = new OpC(0, "&", "and ", { 1, 8, 1, 8 });
	OpCodes.push_back(x86_AND_);
	OpC* x86_XOR_ = new OpC(0, "^", "xor ", { 1, 8, 1, 8 });
	OpCodes.push_back(x86_XOR_);
	OpC* x86_CMP_ = new OpC(0, "cmp", "cmp ", { 1, 8, 1, 8 });
	OpCodes.push_back(x86_CMP_);
	OpC* x86_CMPF_ = new OpC(0, "cmp", "cmpss ", { 12, 12, 12, 12 });
	OpCodes.push_back(x86_CMPF_);
	OpC* x86_Question_ = new OpC(0, "?", "cmp ", { 1, 8, 1, 8 });
	OpCodes.push_back(x86_Question_);
	OpC* x86_POP_ = new OpC(0, "pop", "pop ", { 1, 8});
	OpCodes.push_back(x86_POP_);
	OpC* x86_PUSH_ = new OpC(0, "push", "push ", { 1, 8});
	OpCodes.push_back(x86_PUSH_);
	OpC* x86_XCHG_ = new OpC(0, "xchg", "xchg ", { 1, 8, 1, 8 });
	OpCodes.push_back(x86_XCHG_);
	OpC* x86_LOAD_ = new OpC(0, "ldr", "mov ", { 1, 8, 1, 8 });
	OpCodes.push_back(x86_LOAD_);
	OpC* x86_STORE_ = new OpC(0, "str", "mov ", { 1, 8, 1, 8 });
	OpCodes.push_back(x86_STORE_);
	OpC* x86_SET_TO_ = new OpC(0, "=", "mov ", { 1, 8, 1, 8 });
	OpCodes.push_back(x86_SET_TO_);
	OpC* x86_MOVF_ = new OpC(0, "=", "movss ", { 12, 12, 12, 12 });
	OpCodes.push_back(x86_MOVF_);
	OpC* x86_MOVFldr_ = new OpC(0, "ldr", "movss ", { 12, 12, 12, 12 });
	OpCodes.push_back(x86_MOVFldr_);
	OpC* x86_SCALE_ = new OpC(0, "convert", "movsx ", { 2, 8, 1, 4 });
	OpCodes.push_back(x86_SCALE_);
	OpC* x86_ConvertLargeToEnourmess_ = new OpC(0, "convert", "cvtsi2sd ", { 12, 12, 4, 4 });
	OpCodes.push_back(x86_ConvertLargeToEnourmess_);
	OpC* x86_ConvertEnourmessToLarge_ = new OpC(0, "convert", "cvttss2si ", { 4, 4, 12, 12 });
	OpCodes.push_back(x86_ConvertEnourmessToLarge_);
	OpC* x86_LEA_ = new OpC(0, ":", "lea ", { 2, 8, 2, 8 });
	OpCodes.push_back(x86_LEA_);
	OpC* x86_SHL_ = new OpC(0, "<<", "shl ", { 1, 4, 1, 1 });
	OpCodes.push_back(x86_SHL_);
	OpC* x86_SHR_ = new OpC(0, ">>", "shr ", { 1, 4, 1, 1 });
	OpCodes.push_back(x86_SHR_);
	OpC* x86_NEG_ = new OpC(0, "neg", "neg ", { 1, 8});
	OpCodes.push_back(x86_NEG_);
	OpC* x86_NOT_ = new OpC(0, "!", "not ", { 1, 8});
	OpCodes.push_back(x86_NOT_);
	OpC* x86_CALL_ = new OpC(0, "call", "call ", { 0, 8});
	OpCodes.push_back(x86_CALL_);
	OpC* x86_SUB_ = new OpC(0, "-", "sub ", { 1, 8, 1, 8 });
	OpCodes.push_back(x86_SUB_);
	OpC* x86_SUBF_ = new OpC(0, "-", "subss ", { 12, 12, 12, 12 });
	OpCodes.push_back(x86_SUBF_);
	OpC* x86_IMUL_ = new OpC(0, "*", "imul ", { 2, 8, 2, 8 });
	OpCodes.push_back(x86_IMUL_);
	OpC* x86_MULF_ = new OpC(0, "*", "mulss ", { 12, 12, 12, 12 });
	OpCodes.push_back(x86_MULF_);
	OpC* x86_IDIV_ = new OpC(0, "/", "idiv ", { 2, 8, 2, 8 });
	OpCodes.push_back(x86_IDIV_);
	OpC* x86_DIVF_ = new OpC(0, "/", "divss ", { 12, 12, 12, 12 });
	OpCodes.push_back(x86_DIVF_);
	OpC* x86_JNE_ = new OpC(0, "==", "jne ", {});
	OpCodes.push_back(x86_JNE_);
	OpC* x86_JE_ = new OpC(0, "!=", "je ", {});
	OpCodes.push_back(x86_JE_);
	OpC* x86_JNLE_ = new OpC(0, "<=", "jnle ", {});
	OpCodes.push_back(x86_JNLE_);
	OpC* x86_JNGE_ = new OpC(0, ">=", "jnge ", {});
	OpCodes.push_back(x86_JNGE_);
	OpC* x86_JLE_ = new OpC(0, "!<", "jle ", {});
	OpCodes.push_back(x86_JLE_);
	OpC* x86_JGE_ = new OpC(0, "!>", "jge ", {});
	OpCodes.push_back(x86_JGE_);
	OpC* x86_JNL_ = new OpC(0, "<", "jnl ", {});
	OpCodes.push_back(x86_JNL_);
	OpC* x86_JNG_ = new OpC(0, ">", "jng ", {});
	OpCodes.push_back(x86_JNG_);
	OpC* x86_JMP_ = new OpC(0, "jmp", "jmp ", {});
	OpCodes.push_back(x86_JMP_);
	OpC* x86_RET_ = new OpC(0, "return", "ret\n ", {});
	OpCodes.push_back(x86_RET_);
	OpC* x86_QDWORD_ = new OpC(0, "12", " ", { 12, 12});
	OpCodes.push_back(x86_QDWORD_);
	OpC* x86_QWORD_ = new OpC(0, "8", "qword ", { 8, 8});
	OpCodes.push_back(x86_QWORD_);
	OpC* x86_DWORD_ = new OpC(0, "4", "dword ", { 4, 4});
	OpCodes.push_back(x86_DWORD_);
	OpC* x86_WORD_ = new OpC(0, "2", "word ", { 2, 2});
	OpCodes.push_back(x86_WORD_);
	OpC* x86_BYTE_ = new OpC(0, "1", "byte ", { 1, 1});
	OpCodes.push_back(x86_BYTE_);
	OpC* x86_NULL_ = new OpC(0, "0", "", { 0, 0});
	OpCodes.push_back(x86_NULL_);
	OpC* x86_LABEL_ = new OpC(0, "label", ": ", {});
	OpCodes.push_back(x86_LABEL_);
	OpC* x86_RAW_LABEL_ = new OpC(0, "raw_label", ": ", {});
	OpCodes.push_back(x86_RAW_LABEL_);
	OpC* x86_COMMENT_ = new OpC(0, "comment", "; ", { 0, 0, 0, 0});
	OpCodes.push_back(x86_COMMENT_);
	OpC* x86_EXPORT_ = new OpC(0, "export", "export ", {});
	OpCodes.push_back(x86_EXPORT_);
	OpC* x86_GLOBAL_ = new OpC(0, "global", "global ", {});
	OpCodes.push_back(x86_GLOBAL_);
	OpC* x86_IMPORT_ = new OpC(0, "import", "extern ", {});
	OpCodes.push_back(x86_IMPORT_);
	OpC* x86_LEAVE_ = new OpC(0, "leave", "leave ", {});
	OpCodes.push_back(x86_LEAVE_);


	//8Bit
	AL = new Token("al ", 1);
	AL->add(Task_For_Returning);
	AL->add(Task_For_General_Purpose);
	Registers8.push_back(AL);
	AH = new Token("ah ", 1);
	AH->add(Task_For_Returning);
	AH->add(Task_For_General_Purpose);
	Registers8.push_back(AH);
	BL = new Token("bl ", 1);
	BL->add(Task_For_General_Purpose);
	Registers8.push_back(BL);
	BH = new Token("bh ", 1);
	BH->add(Task_For_General_Purpose);
	Registers8.push_back(BH);
	CL = new Token("cl ", 1);
	CL->add(Task_For_General_Purpose);
	Registers8.push_back(CL);
	CH = new Token("ch ", 1);
	CH->add(Task_For_General_Purpose);
	Registers8.push_back(CH);
	DL = new Token("dl ", 1);
	DL->add(Task_For_Remainder);
	DL->add(Task_For_General_Purpose);
	Registers8.push_back(DL);
	DH = new Token("�dh ", 1);
	DH->add(Task_For_Remainder);
	DH->add(Task_For_General_Purpose);
	Registers8.push_back(DH);

	//16Bit
	AX = new Token("ax ", 2, AL, AH);
	AX->add(Task_For_Returning);
	AX->add(Task_For_General_Purpose);
	Registers16.push_back(AX);
	BX = new Token("bx ", 2, BL, BH);
	BX->add(Task_For_General_Purpose);
	Registers16.push_back(BX);
	CX = new Token("cx ", 2, CL, CH);
	CX->add(Task_For_General_Purpose);
	Registers16.push_back(CX);
	DX = new Token("dx ", 2, DL, DH);
	DX->add(Task_For_Remainder);
	DX->add(Task_For_General_Purpose);
	Registers16.push_back(DX);
	DI = new Token("di ", 2);
	DI->add(Task_For_Offsetting);
	DI->add(Task_For_General_Purpose);
	Registers16.push_back(DI);
	SI = new Token("si ", 2);
	SI->add(Task_For_Offsetting);
	SI->add(Task_For_General_Purpose);
	Registers16.push_back(SI);
	SP = new Token("sp ", 2);
	SP->add(Task_For_Type_Address);
	Registers16.push_back(SP);
	BP = new Token("bp ", 2);
	BP->add(Task_For_Type_Address_Basing);
	Registers16.push_back(BP);

	//32 bit
	EAX = new Token("eax ", 4, AX);
	EAX->add(Task_For_Returning);
	EAX->add(Task_For_General_Purpose);
	Registers32.push_back(EAX);
	EBX = new Token("ebx ", 4, BX);
	EBX->add(Task_For_General_Purpose);
	Registers32.push_back(EBX);
	ECX = new Token("ecx ", 4, CX);
	ECX->add(Task_For_General_Purpose);
	Registers32.push_back(ECX);
	EDX = new Token("edx ", 4, DX);
	EDX->add(Task_For_Remainder);
	EDX->add(Task_For_General_Purpose);
	Registers32.push_back(EDX);
	EDI = new Token("edi ", 4, DI);
	EDI->add(Task_For_Offsetting);
	EDI->add(Task_For_General_Purpose);
	Registers32.push_back(EDI);
	ESI = new Token("esi ", 4, SI);
	ESI->add(Task_For_Offsetting);
	ESI->add(Task_For_General_Purpose);
	Registers32.push_back(ESI);
	ESP = new Token("esp ", 4, SP);
	ESP->add(Task_For_Type_Address);
	Registers32.push_back(ESP);
	EBP = new Token("ebp ", 4, BP);
	EBP->add(Task_For_Type_Address_Basing);
	Registers32.push_back(EBP);

	//64 bit
	RAX = new Token("rax ", 8, EAX);
	RAX->add(Task_For_Returning);
	RAX->add(Task_For_General_Purpose);
	Registers64.push_back(RAX);
	RBX = new Token("rbx ", 8, EBX);
	RBX->add(Task_For_General_Purpose);
	Registers64.push_back(RBX);
	RCX = new Token("rcx ", 8, ECX);
	RCX->add(Task_For_General_Purpose);
	Registers64.push_back(RCX);
	RDX = new Token("rdx ", 8, EDX);
	RDX->add(Task_For_Remainder);
	RDX->add(Task_For_General_Purpose);
	Registers64.push_back(RDX);
	RDI = new Token("rdi ", 8, EDI);
	RDI->add(Task_For_Offsetting);
	RDI->add(Task_For_General_Purpose);
	Registers64.push_back(RDI);
	RSI = new Token("rsi ", 8, ESI);
	RSI->add(Task_For_Offsetting);
	RSI->add(Task_For_General_Purpose);
	Registers64.push_back(RSI);
	RSP = new Token("rsp ", 8, ESP);
	RSP->add(Task_For_Type_Address);
	Registers64.push_back(RSP);
	RBP = new Token("rbp ", 8, EBP);
	RBP->add(Task_For_Type_Address_Basing);
	Registers64.push_back(RBP);
	//Rrrrr...
	R8 = new Token("r8 ", 8, R8);
	R8->add(Task_For_General_Purpose);
	Registers64.push_back(R8);
	R9 = new Token("r9 ", 8, R9);
	R8->add(Task_For_General_Purpose);
	Registers64.push_back(R9);
	R10 = new Token("r10 ", 8, R10);
	R10->add(Task_For_General_Purpose);
	Registers64.push_back(R10);
	R11 = new Token("r11 ", 8, R11);
	R11->add(Task_For_General_Purpose);
	Registers64.push_back(R11);
	R12 = new Token("r11 ", 8, R12);
	R12->add(Task_For_General_Purpose);
	Registers64.push_back(R12);
	R13 = new Token("r13 ", 8, R13);
	R12->add(Task_For_General_Purpose);
	Registers64.push_back(R13);
	R14 = new Token("r14 ", 8, R14);
	R12->add(Task_For_General_Purpose);
	Registers64.push_back(R14);
	R15 = new Token("r15 ", 8, R15);
	R12->add(Task_For_General_Purpose);
	Registers64.push_back(R15);

	//64 bit float
	MMX0 = new Token("xmm0 ", 12);
	MMX0->add(Task_For_Floating_Math);
	MMX0->add(Task_For_Returning);
	Registers128.push_back(MMX0);
	MMX1 = new Token("xmm1 ", 12);
	MMX1->add(Task_For_Floating_Math);
	Registers128.push_back(MMX1);
	MMX2 = new Token("xmm2 ", 12);
	MMX2->add(Task_For_Floating_Math);
	Registers128.push_back(MMX2);
	MMX3 = new Token("xmm3 ", 12);
	MMX3->add(Task_For_Floating_Math);
	Registers128.push_back(MMX3);
	MMX4 = new Token("xmm4 ", 12);
	MMX4->add(Task_For_Floating_Math);
	Registers128.push_back(MMX4);
	MMX5 = new Token("xmm5 ", 12);
	MMX5->add(Task_For_Floating_Math);
	Registers128.push_back(MMX5);
	MMX6 = new Token("xmm6 ", 12);
	MMX6->add(Task_For_Floating_Math);
	Registers128.push_back(MMX6);
	MMX7 = new Token("xmm7 ", 12);
	MMX7->add(Task_For_Floating_Math);
	Registers128.push_back(MMX7);
}
