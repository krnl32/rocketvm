#ifndef _RVM_ARCH_H
#define _RVM_ARCH_H

#include "rocketvm/cpu.h"

#define RVM_INSTRUCTION_SIZE 2

typedef enum {
	RVM_OP_MOV = 0x0,

	RVM_OP_HLT = 0xF,
} rvm_opcode_t;

typedef enum {
	RVM_MODE_IMM_OR_ADDR,
	RVM_MODE_REG,
} rvm_instr_mode_t;

typedef struct {
	rvm_opcode_t opcode;
	rvm_reg_t reg;
	rvm_instr_mode_t mode;
	uint8_t operand;
} rvm_instr_t;

static inline uint16_t rvm_encode(rvm_opcode_t opcode, rvm_reg_t reg, rvm_instr_mode_t mode, uint8_t operand)
{
	return ((uint16_t)(opcode & 0xF) << 12) | ((uint16_t)(reg & 0x7) << 9) | ((uint16_t)(mode & 0x1) << 8) | ((uint16_t)(operand & 0xFF));
}

static inline rvm_instr_t rvm_decode(uint16_t instr)
{
	return (rvm_instr_t){
		.opcode = (uint16_t)((instr >> 12) & 0xF),
		.reg = (uint16_t)(instr >> 9) & 0x7,
		.mode = (uint16_t)(instr >> 8) & 0x1,
		.operand = (uint16_t)(instr & 0xFF),
	};
}

static inline uint16_t rvm_mov_reg(rvm_reg_t rvd, rvm_reg_t rvs)
{
	return rvm_encode(RVM_OP_MOV, rvd, RVM_MODE_REG, (uint8_t)rvs);
}

static inline uint16_t rvm_mov_imm(rvm_reg_t rvd, uint8_t imm)
{
	return rvm_encode(RVM_OP_MOV, rvd, RVM_MODE_IMM_OR_ADDR, imm);
}

static inline uint16_t rvm_hlt(void)
{
	return rvm_encode(RVM_OP_HLT, 0, 0, 0);
}

#endif
