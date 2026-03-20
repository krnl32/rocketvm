#ifndef RVM_CPU_H
#define RVM_CPU_H

#include "rocketvm/common/isa/instruction.h"
#include "rocketvm/common/isa/register.h"
#include "rocketvm/memory.h"

#include <stdbool.h>

#define RVM_CPU_GP_REGISTER_COUNT 8
#define RVM_CPU_INSTRUCTION_SIZE 2
#define RVM_CPU_OP_COUNT 16

#define RVM_CPU_FLAG_RZ (1u << 0)
#define RVM_CPU_FLAG_RN (1u << 1)
#define RVM_CPU_FLAG_RC (1u << 2)
#define RVM_CPU_FLAG_RO (1u << 3)

#define RVM_CPU_FLAG_SET(cpu, flag) ((cpu)->regs.flags |= (flag))
#define RVM_CPU_FLAG_CLEAR(cpu, flag) ((cpu)->regs.flags &= (uint16_t)(~(uint16_t)(flag)))
#define RVM_CPU_FLAG_CHECK(cpu, flag) (((cpu)->regs.flags & (flag)) != 0)

typedef struct {
	uint16_t rv[RVM_CPU_GP_REGISTER_COUNT];
	uint16_t rip;
	uint16_t rsp;
	uint16_t rsb;
	uint16_t flags;
} rvm_registers_t;

typedef struct {
	rvm_registers_t regs;
	bool halt;
} rvm_cpu_t;

int rvm_cpu_cycle(rvm_cpu_t *cpu, rvm_memory_t *mem);

#endif
