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
		.raw = instr,
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

static inline uint16_t rvm_cmp_imm(rvm_reg_t rvd, uint8_t imm)
{
	return rvm_encode(RVM_OP_CMP, rvd, RVM_MODE_IMM_OR_ADDR, imm);
}

static inline uint16_t rvm_cmp_reg(rvm_reg_t rvd, rvm_reg_t rvs)
{
	return rvm_encode(RVM_OP_CMP, rvd, RVM_MODE_REG, (uint8_t)rvs);
}

static inline uint16_t rvm_jmp_imm(int16_t simm)
{
	assert((simm >= -1024 && simm <= 1023) && "rvm_jmp_imm simm out of bounds");
	return ((uint16_t)(RVM_OP_JMP & 0xF) << 12) | ((uint16_t)RVM_MODE_IMM_OR_ADDR << 11) | (uint16_t)(simm & 0x7FF);
}

static inline uint16_t rvm_jmp_reg(rvm_reg_t rvd)
{
	return ((uint16_t)(RVM_OP_JMP & 0xF) << 12) | ((uint16_t)RVM_MODE_REG << 11) | (rvd & 0x7);
}

static inline uint16_t rvm_jz_imm(int16_t simm)
{
	assert((simm >= -1024 && simm <= 1023) && "rvm_jz_imm simm out of bounds");
	return ((uint16_t)(RVM_OP_JZ & 0xF) << 12) | ((uint16_t)RVM_MODE_IMM_OR_ADDR << 11) | (uint16_t)(simm & 0x7FF);
}

static inline uint16_t rvm_jz_reg(rvm_reg_t rvd)
{
	return ((uint16_t)(RVM_OP_JZ & 0xF) << 12) | ((uint16_t)RVM_MODE_REG << 11) | (rvd & 0x7);
}

static inline uint16_t rvm_jnz_imm(int16_t simm)
{
	assert((simm >= -1024 && simm <= 1023) && "rvm_jnz_imm simm out of bounds");
	return ((uint16_t)(RVM_OP_JNZ & 0xF) << 12) | ((uint16_t)RVM_MODE_IMM_OR_ADDR << 11) | (uint16_t)(simm & 0x7FF);
}

static inline uint16_t rvm_jnz_reg(rvm_reg_t rvd)
{
	return ((uint16_t)(RVM_OP_JNZ & 0xF) << 12) | ((uint16_t)RVM_MODE_REG << 11) | (rvd & 0x7);
}

static inline uint16_t rvm_call_imm(int16_t simm)
{
	assert((simm >= -1024 && simm <= 1023) && "rvm_call_imm simm out of bounds");
	return ((uint16_t)(RVM_OP_CALL & 0xF) << 12) | ((uint16_t)RVM_MODE_IMM_OR_ADDR << 11) | (uint16_t)(simm & 0x7FF);
}

static inline uint16_t rvm_call_reg(rvm_reg_t rvd)
{
	return ((uint16_t)(RVM_OP_CALL & 0xF) << 12) | ((uint16_t)RVM_MODE_REG << 11) | (rvd & 0x7);
}

static inline uint16_t rvm_ret(void)
{
	return (uint16_t)(RVM_OP_RET & 0xF) << 12;
}

#endif
