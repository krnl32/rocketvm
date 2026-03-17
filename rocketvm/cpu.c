#include "rocketvm/cpu.h"
#include "rocketvm/arch.h"
#include "utility/logger.h"

static int rvm_cpu_execute(rvm_cpu_t *cpu, rvm_instr_t instr);

int rvm_cpu_cycle(rvm_cpu_t *cpu)
{
	if (cpu->halt) {
		return 0;
	}

	uint16_t rip = cpu->regs.rip;
	if (rip > RVM_MEMORY_PROGRAM_END - 1) {
		rvm_error("rvm_cpu_cycle RIP out of bounds: 0x%X", rip);
		return -1;
	}

	uint16_t instr = rvm_memory_read_uint16(&cpu->mem, rip);
	rvm_debug("rvm_cpu_cycle executing instruction: 0x%04X at RIP: 0x%03X", instr, cpu->regs.rip);

	rvm_instr_t instr_decoded = rvm_decode(instr);

	if (rvm_cpu_execute(cpu, instr_decoded) == -1) {
		rvm_error("rvm_cpu_execute failed");
		return -1;
	}

	if (cpu->regs.rip == rip) {
		cpu->regs.rip += RVM_INSTRUCTION_SIZE;
	}

	return 0;
}

static int rvm_cpu_execute(rvm_cpu_t *cpu, rvm_instr_t instr)
{
	switch (instr.opcode) {
		case RVM_OP_MOV: {
			if (instr.mode == RVM_MODE_IMM_OR_ADDR) {
				cpu->regs.rv[instr.reg] = instr.operand;
			} else if (instr.mode == RVM_MODE_REG) {
				cpu->regs.rv[instr.reg] = cpu->regs.rv[instr.operand & 0x7];
			}
			break;
		}

		case RVM_OP_HLT: {
			rvm_info("rvm_cpu_cycle HALTING....");
			cpu->halt = true;
			break;
		}

		default:
			break;
	}

	return 0;
}
