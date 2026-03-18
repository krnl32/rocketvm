#ifndef _RVM_CPU_ENCODER_H
#define _RVM_CPU_ENCODER_H

#include "rocketvm/cpu/cpu.h"

static inline uint16_t rvm_encode(rvm_opcode_t opcode, rvm_reg_t reg, rvm_instr_mode_t mode, uint8_t operand)
{
	return ((uint16_t)(opcode & 0xF) << 12) | ((uint16_t)(reg & 0x7) << 9) | ((uint16_t)(mode & 0x1) << 8) | ((uint16_t)(operand & 0xFF));
}

static inline rvm_instr_t rvm_decode(uint16_t instr)
{
	return (rvm_instr_t){
		.opcode = (rvm_opcode_t)((instr >> 12) & 0xF),
		.reg = (rvm_reg_t)(instr >> 9) & 0x7,
		.mode = (rvm_instr_mode_t)(instr >> 8) & 0x1,
		.operand = (uint8_t)(instr & 0xFF),
	};
}

static inline uint16_t rvm_mov_imm(rvm_reg_t rvd, uint8_t imm)
{
	return rvm_encode(RVM_OP_MOV, rvd, RVM_MODE_IMM_OR_ADDR, imm);
}

static inline uint16_t rvm_mov_reg(rvm_reg_t rvd, rvm_reg_t rvs)
{
	return rvm_encode(RVM_OP_MOV, rvd, RVM_MODE_REG, (uint8_t)rvs);
}

static inline uint16_t rvm_load_addr(rvm_reg_t rvd, uint8_t addr)
{
	return rvm_encode(RVM_OP_LOAD, rvd, RVM_MODE_IMM_OR_ADDR, addr);
}

static inline uint16_t rvm_load_reg(rvm_reg_t rvd, rvm_reg_t rvs)
{
	return rvm_encode(RVM_OP_LOAD, rvd, RVM_MODE_REG, (uint8_t)rvs);
}

static inline uint16_t rvm_store_addr(rvm_reg_t rvs, uint8_t addr)
{
	return rvm_encode(RVM_OP_STORE, rvs, RVM_MODE_IMM_OR_ADDR, addr);
}

static inline uint16_t rvm_store_reg(rvm_reg_t rvs, rvm_reg_t raddr)
{
	return rvm_encode(RVM_OP_STORE, rvs, RVM_MODE_REG, (uint8_t)raddr);
}

static inline uint16_t rvm_add_imm(rvm_reg_t rvd, uint8_t imm)
{
	return rvm_encode(RVM_OP_ADD, rvd, RVM_MODE_IMM_OR_ADDR, imm);
}

static inline uint16_t rvm_add_reg(rvm_reg_t rvd, rvm_reg_t rvs)
{
	return rvm_encode(RVM_OP_ADD, rvd, RVM_MODE_REG, (uint8_t)rvs);
}

static inline uint16_t rvm_sub_imm(rvm_reg_t rvd, uint8_t imm)
{
	return rvm_encode(RVM_OP_SUB, rvd, RVM_MODE_IMM_OR_ADDR, imm);
}

static inline uint16_t rvm_sub_reg(rvm_reg_t rvd, rvm_reg_t rvs)
{
	return rvm_encode(RVM_OP_SUB, rvd, RVM_MODE_REG, (uint8_t)rvs);
}

static inline uint16_t rvm_mul_imm(rvm_reg_t rvd, uint8_t imm)
{
	return rvm_encode(RVM_OP_MUL, rvd, RVM_MODE_IMM_OR_ADDR, imm);
}

static inline uint16_t rvm_mul_reg(rvm_reg_t rvd, rvm_reg_t rvs)
{
	return rvm_encode(RVM_OP_MUL, rvd, RVM_MODE_REG, (uint8_t)rvs);
}

static inline uint16_t rvm_div_imm(rvm_reg_t rvd, uint8_t imm)
{
	return rvm_encode(RVM_OP_DIV, rvd, RVM_MODE_IMM_OR_ADDR, imm);
}

static inline uint16_t rvm_div_reg(rvm_reg_t rvd, rvm_reg_t rvs)
{
	return rvm_encode(RVM_OP_DIV, rvd, RVM_MODE_REG, (uint8_t)rvs);
}

static inline uint16_t rvm_mod_imm(rvm_reg_t rvd, uint8_t imm)
{
	return rvm_encode(RVM_OP_MOD, rvd, RVM_MODE_IMM_OR_ADDR, imm);
}

static inline uint16_t rvm_mod_reg(rvm_reg_t rvd, rvm_reg_t rvs)
{
	return rvm_encode(RVM_OP_MOD, rvd, RVM_MODE_REG, (uint8_t)rvs);
}

static inline uint16_t rvm_shl_imm(rvm_reg_t rvd, uint8_t imm)
{
	return rvm_encode(RVM_OP_SHL, rvd, RVM_MODE_IMM_OR_ADDR, imm);
}

static inline uint16_t rvm_shl_reg(rvm_reg_t rvd, rvm_reg_t rvs)
{
	return rvm_encode(RVM_OP_SHL, rvd, RVM_MODE_REG, (uint8_t)rvs);
}

static inline uint16_t rvm_shr_imm(rvm_reg_t rvd, uint8_t imm)
{
	return rvm_encode(RVM_OP_SHR, rvd, RVM_MODE_IMM_OR_ADDR, imm);
}

static inline uint16_t rvm_shr_reg(rvm_reg_t rvd, rvm_reg_t rvs)
{
	return rvm_encode(RVM_OP_SHR, rvd, RVM_MODE_REG, (uint8_t)rvs);
}

static inline uint16_t rvm_hlt(void)
{
	return rvm_encode(RVM_OP_HLT, 0, 0, 0);
}

#endif
