#include "rocketvm/cpu/cpu.h"
#include "rocketvm/cpu/cpu_debug.h"
#include "rocketvm/cpu/cpu_encoder.h"
#include "rocketvm/cpu/cpu_data.h"
#include "rocketvm/cpu/cpu_mem.h"
#include "rocketvm/cpu/cpu_alu.h"
#include "rocketvm/cpu/cpu_ctrlf.h"
#include "rocketvm/utility/logger.h"

#include <string.h>

static int rvm_cpu_execute(rvm_cpu_t *cpu, rvm_instr_t instr);

int rvm_cpu_init(rvm_cpu_t *cpu)
{
	memset(cpu, 0, sizeof(*cpu));
	cpu->regs.rsp = RVM_MEMORY_STACK_END;
	return 0;
}

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
	rvm_debug("EXECUTING: 0x%04X at RIP: 0x%03X", instr_bin, cpu->regs.rip);

	rvm_instr_t instr = rvm_decode(instr_bin);
	rvm_instr_dump(instr);
	rvm_debug("\n");

	cpu->regs.rip += RVM_CPU_INSTRUCTION_SIZE;

	if (rvm_cpu_execute(cpu, instr) == -1) {
		rvm_error("rvm_cpu_execute failed");
		return -1;
	}

	return 0;
}

static int rvm_cpu_execute(rvm_cpu_t *cpu, rvm_instr_t instr)
{
	switch (instr.opcode) {
		case RVM_OP_MOV: {
			rvm_cpu_exec_mov(cpu, instr);
			break;
		}

		case RVM_OP_LOAD: {
			rvm_cpu_exec_load(cpu, instr);
			break;
		}
		case RVM_OP_STORE: {
			rvm_cpu_exec_store(cpu, instr);
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

		case RVM_OP_CMP: {
			rvm_cpu_exec_cmp(cpu, instr);
			break;
		}
		case RVM_OP_JMP: {
			rvm_cpu_exec_jmp(cpu, instr);
			break;
		}
		case RVM_OP_JNZ: {
			rvm_cpu_exec_jnz(cpu, instr);
			break;
		}
		case RVM_OP_JZ: {
			rvm_cpu_exec_jz(cpu, instr);
			break;
		}

		case RVM_OP_CALL: {
			rvm_cpu_exec_call(cpu, instr);
			break;
		}
		case RVM_OP_RET: {
			rvm_cpu_exec_ret(cpu, instr);
			break;
		}

		default:
			break;
	}

	return 0;
}
