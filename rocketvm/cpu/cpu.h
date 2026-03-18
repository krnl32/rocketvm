#ifndef _RVM_CPU_H
#define _RVM_CPU_H

#include "rocketvm/memory.h"

#include <stdbool.h>

#define RVM_CPU_GP_REGISTER_COUNT 8
#define RVM_CPU_INSTRUCTION_SIZE 2

#define RVM_CPU_FLAG_RZ (1u << 0)
#define RVM_CPU_FLAG_RN (1u << 1)
#define RVM_CPU_FLAG_RC (1u << 2)
#define RVM_CPU_FLAG_RO (1u << 3)

#define RVM_CPU_FLAG_SET(cpu, flag) ((cpu)->regs.flags |= (flag))
#define RVM_CPU_FLAG_CLEAR(cpu, flag) ((cpu)->regs.flags &= (uint16_t)(~(uint16_t)(flag)))
#define RVM_CPU_FLAG_CHECK(cpu, flag) (((cpu)->regs.flags & (flag)) != 0)

typedef enum {
	RVM_REG_RV0 = 0x0,
	RVM_REG_RV1 = 0x1,
	RVM_REG_RV2 = 0x2,
	RVM_REG_RV3 = 0x3,
	RVM_REG_RV4 = 0x4,
	RVM_REG_RV5 = 0x5,
	RVM_REG_RV6 = 0x6,
	RVM_REG_RV7 = 0x7,
} rvm_reg_t;

typedef enum {
	RVM_OP_MOV = 0x0,

	RVM_OP_LOAD = 0x1,
	RVM_OP_STORE = 0x2,

	RVM_OP_ADD = 0x3,
	RVM_OP_SUB = 0x4,
	RVM_OP_MUL = 0x5,
	RVM_OP_DIV = 0x6,
	RVM_OP_MOD = 0x7,
	RVM_OP_SHL = 0x8,
	RVM_OP_SHR = 0x9,

	RVM_OP_CMP = 0xA,
	RVM_OP_JMP = 0xB,
	RVM_OP_JZ = 0xC,
	RVM_OP_JNZ = 0xD,

	RVM_OP_HLT = 0xF,
} rvm_opcode_t;

typedef enum {
	RVM_MODE_IMM_OR_ADDR = 0x0,
	RVM_MODE_REG = 0x1,
} rvm_instr_mode_t;

typedef struct {
	rvm_opcode_t opcode;
	rvm_reg_t reg;
	rvm_instr_mode_t mode;
	uint8_t operand;
} rvm_instr_t;

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
