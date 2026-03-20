#include "rocketvm/cpu/cpu_debug.h"
#include "rocketvm/common/isa/utility.h"
#include "rocketvm/common/utility/logger.h"

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
	rvm_debug("  RZ=%d RN=%d RC=%d RO=%d", RVM_CPU_FLAG_CHECK(cpu, RVM_CPU_FLAG_RZ), RVM_CPU_FLAG_CHECK(cpu, RVM_CPU_FLAG_RN), RVM_CPU_FLAG_CHECK(cpu, RVM_CPU_FLAG_RC), RVM_CPU_FLAG_CHECK(cpu, RVM_CPU_FLAG_RO));

	rvm_debug("===================");
}

void rvm_instr_dump(rvm_instr_t instr)
{
	// rvm_debug("INSTR: opcode=%u reg=%u mode=%u operand=0x%02X", (uint8_t)instr.opcode, instr.reg, instr.mode, instr.operand);
	rvm_debug("INSTR: opcode=%s reg=RV%u mode=%s operand=0x%02X", rvm_opcode_to_string(instr.opcode), instr.reg, rvm_instr_mode_to_string(instr.mode), instr.operand);
}

