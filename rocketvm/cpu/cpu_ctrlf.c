#include "rocketvm/cpu/cpu_ctrlf.h"

void rvm_cpu_exec_cmp(rvm_cpu_t *cpu, rvm_instr_t instr)
{
	uint16_t dst = cpu->regs.rv[instr.reg];
	uint16_t src = (instr.mode == RVM_MODE_IMM_OR_ADDR) ? instr.operand : (cpu->regs.rv[instr.operand & 0x7]);
	uint32_t result = dst - src;
	bool borrow = dst < src;

	RVM_CPU_FLAG_CLEAR(cpu, RVM_CPU_FLAG_RZ | RVM_CPU_FLAG_RN | RVM_CPU_FLAG_RC | RVM_CPU_FLAG_RO);

	if ((result & 0xFFFF) == 0) {
		RVM_CPU_FLAG_SET(cpu, RVM_CPU_FLAG_RZ);
	}

	if ((uint16_t)result & 0x8000) {
		RVM_CPU_FLAG_SET(cpu, RVM_CPU_FLAG_RN);
	}

	if (borrow) {
		RVM_CPU_FLAG_SET(cpu, RVM_CPU_FLAG_RC);
	}

	// Sub different-sign numbers but result sign is incorrect -> overflow
	bool overflow = ((dst ^ src) & (dst ^ (uint16_t)result) & 0x8000) != 0;
	if (overflow) {
		RVM_CPU_FLAG_SET(cpu, RVM_CPU_FLAG_RO);
	}
}
