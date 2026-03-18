#include "rocketvm/cpu/cpu_ctrlf.h"

void rvm_cpu_exec_cmp(rvm_cpu_t *cpu, rvm_instr_t instr)
{
	bool borrow = false;
	uint32_t result = 0;

	if (instr.mode == RVM_MODE_IMM_OR_ADDR) {
		borrow = cpu->regs.rv[instr.reg] < instr.operand;
		result = cpu->regs.rv[instr.reg] - instr.operand;
	} else if (instr.mode == RVM_MODE_REG) {
		borrow = cpu->regs.rv[instr.reg] < cpu->regs.rv[instr.operand & 0x7];
		result = cpu->regs.rv[instr.reg] - cpu->regs.rv[instr.operand & 0x7];
	}

	RVM_CPU_FLAG_CLEAR(cpu, RVM_CPU_FLAG_RZ | RVM_CPU_FLAG_RN | RVM_CPU_FLAG_RC);

	if ((result & 0xFFFF) == 0) {
		RVM_CPU_FLAG_SET(cpu, RVM_CPU_FLAG_RZ);
	}

	if ((uint16_t)result & 0x8000) {
		RVM_CPU_FLAG_SET(cpu, RVM_CPU_FLAG_RN);
	}

	if (borrow) {
		RVM_CPU_FLAG_SET(cpu, RVM_CPU_FLAG_RC);
	}
}
