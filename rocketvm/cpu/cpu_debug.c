#include "rocketvm/cpu/cpu_debug.h"
#include "rocketvm/utility/logger.h"

void rvm_cpu_dump(rvm_cpu_t *cpu)
{
	rvm_debug("==== CPU STATE ====");

	rvm_debug("RIP: 0x%04X", cpu->regs.rip);
	rvm_debug("RSP: 0x%04X", cpu->regs.rsp);
	rvm_debug("RSB: 0x%04X", cpu->regs.rsb);

	for (int i = 0; i < RVM_CPU_GP_REGISTER_COUNT; i++) {
		rvm_debug("RV%d: 0x%04X (%5u)", i, cpu->regs.rv[i], cpu->regs.rv[i]);
	}

	rvm_debug("FLAGS: 0x%X", cpu->regs.flags);
	rvm_debug("  RZ=%d RN=%d RC=%d RO=%d", RVM_CPU_FLAG_CHECK(cpu, RVM_CPU_FLAG_RZ), RVM_CPU_FLAG_CHECK(cpu, RVM_CPU_FLAG_RN), RVM_CPU_FLAG_CHECK(cpu, RVM_CPU_FLAG_RC), RVM_CPU_FLAG_CHECK(cpu, RVM_CPU_FLAG_RO));

	rvm_debug("===================");
}

void rvm_instr_dump(rvm_instr_t instr)
{
	// rvm_debug("INSTR: opcode=%u reg=%u mode=%u operand=0x%02X", (uint8_t)instr.opcode, instr.reg, instr.mode, instr.operand);
	rvm_debug("INSTR: opcode=%s reg=RV%u mode=%s operand=0x%02X", rvm_opcode_to_string(instr.opcode), instr.reg, rvm_instr_mode_to_string(instr.mode), instr.operand);
}

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
			return "INVALID";
	}
}

const char *rvm_instr_mode_to_string(rvm_instr_mode_t mode)
{
	switch (mode) {
		case RVM_MODE_IMM_OR_ADDR:
			return "IMM_OR_ADDR";
		case RVM_MODE_REG:
			return "REG";
		default:
			return "INVALID";
	}
}
