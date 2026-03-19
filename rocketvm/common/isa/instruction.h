#ifndef RVM_COMMON_ISA_INSTRUCTION_H
#define RVM_COMMON_ISA_INSTRUCTION_H

#include "rocketvm/common/isa/opcode.h"
#include "rocketvm/common/isa/register.h"

#include <stdint.h>

typedef enum {
	RVM_MODE_IMM_OR_ADDR = 0x0,
	RVM_MODE_REG = 0x1
} rvm_instr_mode_t;

typedef struct {
	uint16_t raw;
	rvm_opcode_t opcode;
	rvm_reg_t reg;
	rvm_instr_mode_t mode;
	uint8_t operand;
} rvm_instr_t;

#endif
