#include "rocketvm/cpu/cpu_mem.h"
#include "rocketvm/utility/logger.h"

static inline void rvm_cpu_fault(rvm_cpu_t *cpu, const char *msg, uint16_t addr)
{
	rvm_error("%s: 0x%04X", msg, addr);
	cpu->halt = true;
}

void rvm_cpu_exec_load(rvm_cpu_t *cpu, rvm_instr_t instr)
{
	uint16_t addr = 0;

	if (instr.mode == RVM_MODE_IMM_OR_ADDR) {
		addr = instr.operand;
	} else if (instr.mode == RVM_MODE_REG) {
		addr = cpu->regs.rv[instr.operand & 0x7];
	}

	if (addr >= RVM_MEMORY_SIZE - 1) {
		rvm_cpu_fault(cpu, "LOAD out of bounds", addr);
		return;
	}

	if (addr & 1) {
		rvm_cpu_fault(cpu, "LOAD unaligned access", addr);
		return;
	}

	cpu->regs.rv[instr.reg] = rvm_memory_read_uint16(&cpu->mem, addr);
}

void rvm_cpu_exec_store(rvm_cpu_t *cpu, rvm_instr_t instr)
{
	uint16_t addr = 0;

	if (instr.mode == RVM_MODE_IMM_OR_ADDR) {
		addr = instr.operand;
	} else if (instr.mode == RVM_MODE_REG) {
		addr = cpu->regs.rv[instr.operand & 0x7];
	}

	if (addr >= RVM_MEMORY_SIZE - 1) {
		rvm_cpu_fault(cpu, "STORE out of bounds", addr);
		return;
	}

	if (addr & 1) {
		rvm_cpu_fault(cpu, "STORE unaligned access", addr);
		return;
	}

	// if (addr <= RVM_MEMORY_PROGRAM_END) {
	// 	rvm_cpu_fault(cpu, "STORE to program memory", addr);
	// 	return;
	// }

	rvm_memory_write_uint16(&cpu->mem, addr, cpu->regs.rv[instr.reg]);
}
