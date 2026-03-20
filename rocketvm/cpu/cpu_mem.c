#include "rocketvm/cpu/cpu_mem.h"
#include "rocketvm/cpu/cpu_internal.h"

void rvm_cpu_exec_load(rvm_cpu_t *cpu, rvm_memory_t *mem, rvm_instr_t instr)
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

	cpu->regs.rv[instr.reg] = rvm_memory_read_uint16(mem, addr);
}

void rvm_cpu_exec_store(rvm_cpu_t *cpu, rvm_memory_t *mem, rvm_instr_t instr)
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

	rvm_memory_write_uint16(mem, addr, cpu->regs.rv[instr.reg]);
}
