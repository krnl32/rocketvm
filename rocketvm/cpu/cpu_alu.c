#include "rocketvm/cpu/cpu_alu.h"
#include "rocketvm/utility/logger.h"

void rvm_cpu_exec_add(rvm_cpu_t *cpu, rvm_instr_t instr)
{
	uint32_t result = 0;

	if (instr.mode == RVM_MODE_IMM_OR_ADDR) {
		result = cpu->regs.rv[instr.reg] + instr.operand;
	} else if (instr.mode == RVM_MODE_REG) {
		result = cpu->regs.rv[instr.reg] + cpu->regs.rv[instr.operand & 0x7];
	}

	RVM_CPU_FLAG_CLEAR(cpu, RVM_CPU_FLAG_RZ | RVM_CPU_FLAG_RN | RVM_CPU_FLAG_RC);

	if ((result & 0xFFFF) == 0) {
		RVM_CPU_FLAG_SET(cpu, RVM_CPU_FLAG_RZ);
	}

	if ((uint16_t)result & 0x8000) {
		RVM_CPU_FLAG_SET(cpu, RVM_CPU_FLAG_RN);
	}

	if (result > 0xFFFF) {
		RVM_CPU_FLAG_SET(cpu, RVM_CPU_FLAG_RC);
	}

	cpu->regs.rv[instr.reg] = (uint16_t)result;
}

void rvm_cpu_exec_sub(rvm_cpu_t *cpu, rvm_instr_t instr)
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

	cpu->regs.rv[instr.reg] = (uint16_t)result;
}

void rvm_cpu_exec_mul(rvm_cpu_t *cpu, rvm_instr_t instr)
{
	uint32_t result = 0;

	if (instr.mode == RVM_MODE_IMM_OR_ADDR) {
		result = cpu->regs.rv[instr.reg] * instr.operand;
	} else if (instr.mode == RVM_MODE_REG) {
		result = cpu->regs.rv[instr.reg] * cpu->regs.rv[instr.operand & 0x7];
	}

	RVM_CPU_FLAG_CLEAR(cpu, RVM_CPU_FLAG_RZ | RVM_CPU_FLAG_RN | RVM_CPU_FLAG_RC);

	if ((result & 0xFFFF) == 0) {
		RVM_CPU_FLAG_SET(cpu, RVM_CPU_FLAG_RZ);
	}

	if ((uint16_t)result & 0x8000) {
		RVM_CPU_FLAG_SET(cpu, RVM_CPU_FLAG_RN);
	}

	if (result > 0xFFFF) {
		RVM_CPU_FLAG_SET(cpu, RVM_CPU_FLAG_RC);
	}

	cpu->regs.rv[instr.reg] = (uint16_t)result;
}

void rvm_cpu_exec_div(rvm_cpu_t *cpu, rvm_instr_t instr)
{
	uint32_t result = 0;

	if (instr.mode == RVM_MODE_IMM_OR_ADDR) {
		if (instr.operand == 0) {
			rvm_error("rvm_cpu_execute division by 0 error");
			cpu->halt = true;
			return;
		}

		result = cpu->regs.rv[instr.reg] / instr.operand;
	} else if (instr.mode == RVM_MODE_REG) {
		uint16_t value = cpu->regs.rv[instr.operand & 0x7];
		if (value == 0) {
			rvm_error("rvm_cpu_execute division by 0 error");
			cpu->halt = true;
			return;
		}

		result = cpu->regs.rv[instr.reg] / value;
	}

	RVM_CPU_FLAG_CLEAR(cpu, RVM_CPU_FLAG_RZ | RVM_CPU_FLAG_RN | RVM_CPU_FLAG_RC);

	if ((result & 0xFFFF) == 0) {
		RVM_CPU_FLAG_SET(cpu, RVM_CPU_FLAG_RZ);
	}

	if ((uint16_t)result & 0x8000) {
		RVM_CPU_FLAG_SET(cpu, RVM_CPU_FLAG_RN);
	}

	cpu->regs.rv[instr.reg] = (uint16_t)result;
}

void rvm_cpu_exec_mod(rvm_cpu_t *cpu, rvm_instr_t instr)
{
	uint32_t result = 0;

	if (instr.mode == RVM_MODE_IMM_OR_ADDR) {
		if (instr.operand == 0) {
			rvm_error("rvm_cpu_execute division by 0 error");
			cpu->halt = true;
			return;
		}

		result = cpu->regs.rv[instr.reg] % instr.operand;
	} else if (instr.mode == RVM_MODE_REG) {
		uint16_t value = cpu->regs.rv[instr.operand & 0x7];
		if (value == 0) {
			rvm_error("rvm_cpu_execute division by 0 error");
			cpu->halt = true;
			return;
		}

		result = cpu->regs.rv[instr.reg] % value;
	}

	RVM_CPU_FLAG_CLEAR(cpu, RVM_CPU_FLAG_RZ | RVM_CPU_FLAG_RN | RVM_CPU_FLAG_RC);

	if ((result & 0xFFFF) == 0) {
		RVM_CPU_FLAG_SET(cpu, RVM_CPU_FLAG_RZ);
	}

	if ((uint16_t)result & 0x8000) {
		RVM_CPU_FLAG_SET(cpu, RVM_CPU_FLAG_RN);
	}

	cpu->regs.rv[instr.reg] = (uint16_t)result;
}

void rvm_cpu_exec_shl(rvm_cpu_t *cpu, rvm_instr_t instr)
{
	uint16_t dst_value = cpu->regs.rv[instr.reg];
	uint8_t shift = 0;
	uint32_t result = 0;

	if (instr.mode == RVM_MODE_IMM_OR_ADDR) {
		shift = instr.operand & 0xF;
		result = (dst_value << shift) & 0xFFFF;
	} else if (instr.mode == RVM_MODE_REG) {
		shift = cpu->regs.rv[instr.operand & 0x7] & 0xF;
		result = (dst_value << shift) & 0xFFFF;
	}

	RVM_CPU_FLAG_CLEAR(cpu, RVM_CPU_FLAG_RZ | RVM_CPU_FLAG_RN | RVM_CPU_FLAG_RC);

	if ((result & 0xFFFF) == 0) {
		RVM_CPU_FLAG_SET(cpu, RVM_CPU_FLAG_RZ);
	}

	if ((uint16_t)result & 0x8000) {
		RVM_CPU_FLAG_SET(cpu, RVM_CPU_FLAG_RN);
	}

	if (shift != 0) {
		uint16_t bit = (dst_value >> (16 - shift)) & 1;
		if (bit) {
			RVM_CPU_FLAG_SET(cpu, RVM_CPU_FLAG_RC);
		}
	}

	cpu->regs.rv[instr.reg] = (uint16_t)result;
}

void rvm_cpu_exec_shr(rvm_cpu_t *cpu, rvm_instr_t instr)
{
	uint16_t dst_value = cpu->regs.rv[instr.reg];
	uint8_t shift = 0;
	uint32_t result = 0;

	if (instr.mode == RVM_MODE_IMM_OR_ADDR) {
		shift = instr.operand & 0xF;
		result = dst_value >> shift;
	} else if (instr.mode == RVM_MODE_REG) {
		shift = cpu->regs.rv[instr.operand & 0x7] & 0xF;
		result = dst_value >> shift;
	}

	RVM_CPU_FLAG_CLEAR(cpu, RVM_CPU_FLAG_RZ | RVM_CPU_FLAG_RN | RVM_CPU_FLAG_RC);

	if ((result & 0xFFFF) == 0) {
		RVM_CPU_FLAG_SET(cpu, RVM_CPU_FLAG_RZ);
	}

	if ((uint16_t)result & 0x8000) {
		RVM_CPU_FLAG_SET(cpu, RVM_CPU_FLAG_RN);
	}

	if (shift != 0) {
		uint16_t bit = (dst_value >> (shift - 1)) & 1;
		if (bit) {
			RVM_CPU_FLAG_SET(cpu, RVM_CPU_FLAG_RC);
		}
	}

	cpu->regs.rv[instr.reg] = (uint16_t)result;
}
