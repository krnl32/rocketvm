#include "rocketvm/cpu/cpu_data.h"

void rvm_cpu_exec_mov(rvm_cpu_t *cpu, rvm_memory_t *mem, rvm_instr_t instr)
{
	(void)mem;

	if (instr.mode == RVM_MODE_IMM_OR_ADDR) {
		cpu->regs.rv[instr.reg] = instr.operand;
	} else if (instr.mode == RVM_MODE_REG) {
		cpu->regs.rv[instr.reg] = cpu->regs.rv[instr.operand & 0x7];
	}
}
