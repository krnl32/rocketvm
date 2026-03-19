#ifndef RVM_COMMON_ISA_OPCODE_H
#define RVM_COMMON_ISA_OPCODE_H

typedef enum {
	RVM_OP_MOV = 0x0,

	RVM_OP_LOAD = 0x1,
	RVM_OP_STORE = 0x2,

	RVM_OP_ADD = 0x3,
	RVM_OP_SUB = 0x4,
	RVM_OP_MUL = 0x5,
	RVM_OP_DIV = 0x6,
	RVM_OP_MOD = 0x7,
	RVM_OP_SHL = 0x8,
	RVM_OP_SHR = 0x9,

	RVM_OP_CMP = 0xA,
	RVM_OP_JMP = 0xB,
	RVM_OP_JZ = 0xC,
	RVM_OP_JNZ = 0xD,

	RVM_OP_CALL = 0xE,
	RVM_OP_RET = 0xF
} rvm_opcode_t;

#endif
