#include "rocketvm/cpu/cpu_ctrlf.h"
#include "rocketvm/cpu/cpu_internal.h"

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

void rvm_cpu_exec_jmp(rvm_cpu_t *cpu, rvm_instr_t instr)
{
	uint16_t addr;
	rvm_instr_mode_t mode = (instr.raw >> 11) & 0x1;

	if (mode == RVM_MODE_IMM_OR_ADDR) {
		uint16_t raw = instr.raw & 0x7FF;
		int16_t offset = (raw & 0x400) ? (int16_t)(raw | 0xF800) : (int16_t)raw;
		addr = (uint16_t)(cpu->regs.rip + offset);
	} else {
		uint8_t reg = instr.raw & 0x7;
		addr = cpu->regs.rv[reg];
	}

	if (addr > RVM_MEMORY_SIZE - RVM_CPU_INSTRUCTION_SIZE) {
		rvm_cpu_fault(cpu, "JMP out of bounds", addr);
		return;
	}

	if (addr & 1) {
		rvm_cpu_fault(cpu, "JMP unaligned access", addr);
		return;
	}

	cpu->regs.rip = addr;
}

void rvm_cpu_exec_jz(rvm_cpu_t *cpu, rvm_instr_t instr)
{
	if (RVM_CPU_FLAG_CHECK(cpu, RVM_CPU_FLAG_RZ)) {
		rvm_cpu_exec_jmp(cpu, instr);
	}
}

void rvm_cpu_exec_jnz(rvm_cpu_t *cpu, rvm_instr_t instr)
{
	if (!RVM_CPU_FLAG_CHECK(cpu, RVM_CPU_FLAG_RZ)) {
		rvm_cpu_exec_jmp(cpu, instr);
	}
}

void rvm_cpu_exec_call(rvm_cpu_t *cpu, rvm_instr_t instr)
{
	uint16_t addr;
	rvm_instr_mode_t mode = (instr.raw >> 11) & 0x1;

	if (mode == RVM_MODE_IMM_OR_ADDR) {
		uint16_t raw = instr.raw & 0x7FF;
		int16_t offset = (raw & 0x400) ? (int16_t)(raw | 0xF800) : (int16_t)raw;
		addr = (uint16_t)(cpu->regs.rip + offset);
	} else {
		uint8_t reg = instr.raw & 0x7;
		addr = cpu->regs.rv[reg];
	}

	// Validate
	if (addr > RVM_MEMORY_SIZE - RVM_CPU_INSTRUCTION_SIZE) {
		rvm_cpu_fault(cpu, "CALL out of bounds", addr);
		return;
	}

	if (addr & 1) {
		rvm_cpu_fault(cpu, "CALL unaligned access", addr);
		return;
	}

	if (cpu->regs.rsp < RVM_MEMORY_STACK_START + 2) {
		rvm_cpu_fault(cpu, "CALL stack overflow", cpu->regs.rsp);
		return;
	}

	// Save Return Address
	cpu->regs.rsp -= 2;
	rvm_memory_write_uint16(&cpu->mem, cpu->regs.rsp, cpu->regs.rip);

	cpu->regs.rip = addr;
}

void rvm_cpu_exec_ret(rvm_cpu_t *cpu, rvm_instr_t instr)
{
	(void)instr;

	if (cpu->regs.rsp >= RVM_MEMORY_STACK_END) {
		cpu->halt = true;
		return;
	}

	uint16_t addr = rvm_memory_read_uint16(&cpu->mem, cpu->regs.rsp);
	cpu->regs.rsp += 2;

	if (addr > RVM_MEMORY_SIZE - RVM_CPU_INSTRUCTION_SIZE) {
		rvm_cpu_fault(cpu, "RET out of bounds", addr);
		return;
	}

	if (addr & 1) {
		rvm_cpu_fault(cpu, "RET unaligned access", addr);
		return;
	}

	cpu->regs.rip = addr;
}
