#include "rocketvm/utility/logger.h"
#include "rocketvm/cpu/arch.h"
#include "rocketvm/cpu/cpu.h"

#include <stdio.h>
#include <string.h>

int main(void)
{
	rvm_cpu_t cpu = { 0 };

	// rvm_memory_write_uint16(&cpu.mem, 0, rvm_mov_imm(RVM_REG_RV0, 10));
	// rvm_memory_write_uint16(&cpu.mem, 2, rvm_mov_reg(RVM_REG_RV2, RVM_REG_RV0));

	rvm_memory_write_uint16(&cpu.mem, 0, rvm_mov_imm(RVM_REG_RV0, 0xFF));
	rvm_memory_write_uint16(&cpu.mem, 2, rvm_mov_imm(RVM_REG_RV1, 1));

	// ADD → should overflow to 0x0100 (no carry yet)
	rvm_memory_write_uint16(&cpu.mem, 4, rvm_add_reg(RVM_REG_RV0, RVM_REG_RV1));

	// ADD again → 0x0100 + 1 = 0x0101
	rvm_memory_write_uint16(&cpu.mem, 6, rvm_add_imm(RVM_REG_RV0, 1));

	// SUB → test borrow
	rvm_memory_write_uint16(&cpu.mem, 8, rvm_sub_imm(RVM_REG_RV0, 0xFF));

	// SHL → test carry
	rvm_memory_write_uint16(&cpu.mem, 10, rvm_mov_imm(RVM_REG_RV2, 0x80));
	rvm_memory_write_uint16(&cpu.mem, 12, rvm_shl_imm(RVM_REG_RV2, 1));

	// SHR → test carry
	rvm_memory_write_uint16(&cpu.mem, 14, rvm_mov_imm(RVM_REG_RV3, 1));
	rvm_memory_write_uint16(&cpu.mem, 16, rvm_shr_imm(RVM_REG_RV3, 1));

	rvm_memory_write_uint16(&cpu.mem, 18, rvm_hlt());

	while (!cpu.halt) {
		rvm_cpu_dump(&cpu);
		printf("\n");

		if (rvm_cpu_cycle(&cpu) == -1) {
			rvm_error("rvm_cpu_cycle failed");
			return -1;
		}
	}

	return 0;
}
