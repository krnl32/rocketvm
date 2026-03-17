#include "rocketvm/utility/logger.h"
#include "rocketvm/arch.h"
#include "rocketvm/cpu.h"

#include <string.h>

int main(void)
{
	rvm_cpu_t cpu = { 0 };

	rvm_memory_write_uint16(&cpu.mem, 0, rvm_mov_imm(RVM_REG_RV0, 10));
	rvm_memory_write_uint16(&cpu.mem, 2, rvm_mov_reg(RVM_REG_RV2, RVM_REG_RV0));
	rvm_memory_write_uint16(&cpu.mem, 4, rvm_hlt());

	while (!cpu.halt) {
		if (rvm_cpu_cycle(&cpu) == -1) {
			rvm_error("rvm_cpu_cycle failed");
			return -1;
		}
	}

	return 0;
}
