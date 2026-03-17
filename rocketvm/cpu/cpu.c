#include "rocketvm/cpu/cpu.h"
#include "rocketvm/cpu/arch.h"
#include "rocketvm/cpu/cpu_data.h"
#include "rocketvm/cpu/cpu_alu.h"
#include "rocketvm/utility/logger.h"

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

	uint16_t instr_bin = rvm_memory_read_uint16(&cpu->mem, rip);
	rvm_debug("rvm_cpu_cycle executing instruction: 0x%04X at RIP: 0x%03X", instr_bin, cpu->regs.rip);

	rvm_instr_t instr = rvm_decode(instr_bin);
	rvm_debug("INSTR: opcode=%u reg=%u mode=%u operand=0x%02X\n", (uint8_t)instr.opcode, instr.reg, instr.mode, instr.operand);

	if (rvm_cpu_execute(cpu, instr) == -1) {
		rvm_error("rvm_cpu_execute failed");
		return -1;
	}

	if (cpu->regs.rip == rip) {
		cpu->regs.rip += RVM_INSTRUCTION_SIZE;
	}

	return 0;
}

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
	rvm_debug("  RZ=%d RN=%d RC=%d", RVM_CPU_FLAG_CHECK(cpu, RVM_CPU_FLAG_RZ), RVM_CPU_FLAG_CHECK(cpu, RVM_CPU_FLAG_RN), RVM_CPU_FLAG_CHECK(cpu, RVM_CPU_FLAG_RC));

	rvm_debug("===================");
}

static int rvm_cpu_execute(rvm_cpu_t *cpu, rvm_instr_t instr)
{
	switch (instr.opcode) {
		case RVM_OP_MOV: {
			rvm_cpu_exec_mov(cpu, instr);
			break;
		}

		case RVM_OP_ADD: {
			rvm_cpu_exec_add(cpu, instr);
			break;
		}

		case RVM_OP_SUB: {
			rvm_cpu_exec_sub(cpu, instr);
			break;
		}

		case RVM_OP_MUL: {
			rvm_cpu_exec_mul(cpu, instr);
			break;
		}

		case RVM_OP_DIV: {
			rvm_cpu_exec_div(cpu, instr);
			break;
		}

		case RVM_OP_MOD: {
			rvm_cpu_exec_mod(cpu, instr);
			break;
		}

		case RVM_OP_SHL: {
			rvm_cpu_exec_shl(cpu, instr);
			break;
		}

		case RVM_OP_SHR: {
			rvm_cpu_exec_shr(cpu, instr);
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
