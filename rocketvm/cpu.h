#ifndef _RVM_CPU_H
#define _RVM_CPU_H

#include "rocketvm/memory.h"

#include <stdbool.h>

#define RVM_CPU_GP_REGISTER_COUNT 8

#define RVM_CPU_FLAG_RZ (1u << 0)
#define RVM_CPU_FLAG_RN (1u << 1)
#define RVM_CPU_FLAG_RC (1u << 2)

#define RVM_CPU_FLAG_SET(cpu, flag) ((cpu)->regs.flags |= (flag))
#define RVM_CPU_FLAG_CLEAR(cpu, flag) ((cpu)->regs.flags &= ~(flag))
#define RVM_CPU_FLAG_CHECK(cpu, flag) (((cpu)->regs.flags & (flag)) != 0)

typedef enum {
	RVM_REG_RV0,
	RVM_REG_RV1,
	RVM_REG_RV2,
	RVM_REG_RV3,
	RVM_REG_RV4,
	RVM_REG_RV5,
	RVM_REG_RV6,
	RVM_REG_RV7,
} rvm_reg_t;

typedef struct {
	uint16_t rv[RVM_CPU_GP_REGISTER_COUNT];
	uint16_t rip;
	uint16_t rsp;
	uint16_t rsb;
	uint16_t flags;
} rvm_registers_t;

typedef struct {
	rvm_registers_t regs;
	rvm_memory_t mem;
	bool halt;
} rvm_cpu_t;

int rvm_cpu_cycle(rvm_cpu_t *cpu);

#endif
