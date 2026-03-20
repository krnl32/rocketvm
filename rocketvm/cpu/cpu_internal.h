#ifndef RVM_CPU_INTERNAL_H
#define RVM_CPU_INTERNAL_H

#include "rocketvm/cpu/cpu.h"
#include "rocketvm/common/utility/logger.h"

static inline void rvm_cpu_fault(rvm_cpu_t *cpu, const char *msg, uint16_t addr)
{
	rvm_error("%s: 0x%04X", msg, addr);
	cpu->halt = true;
}

#endif
