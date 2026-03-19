#include "rocketvm/common/isa/utility.h"

const char *rvm_opcode_to_string(rvm_opcode_t opcode)
{
	switch (opcode) {
		case RVM_OP_MOV:
			return "MOV";
		case RVM_OP_LOAD:
			return "LOAD";
		case RVM_OP_STORE:
			return "STORE";
		case RVM_OP_ADD:
			return "ADD";
		case RVM_OP_SUB:
			return "SUB";
		case RVM_OP_MUL:
			return "MUL";
		case RVM_OP_DIV:
			return "DIV";
		case RVM_OP_MOD:
			return "MOD";
		case RVM_OP_SHL:
			return "SHL";
		case RVM_OP_SHR:
			return "SHR";
		case RVM_OP_CMP:
			return "CMP";
		case RVM_OP_JMP:
			return "JMP";
		case RVM_OP_JZ:
			return "JZ";
		case RVM_OP_JNZ:
			return "JNZ";
		case RVM_OP_CALL:
			return "CALL";
		case RVM_OP_RET:
			return "RET";
		default:
			return "INVALID_OPCODE";
	}
}

const char *rvm_reg_to_string(rvm_reg_t reg)
{
	switch (reg) {
		case RVM_REG_RV0:
			return "RV0";
		case RVM_REG_RV1:
			return "RV1";
		case RVM_REG_RV2:
			return "RV2";
		case RVM_REG_RV3:
			return "RV3";
		case RVM_REG_RV4:
			return "RV4";
		case RVM_REG_RV5:
			return "RV5";
		case RVM_REG_RV6:
			return "RV6";
		case RVM_REG_RV7:
			return "RV7";
		default:
			return "INVALID_REG";
	}
}
