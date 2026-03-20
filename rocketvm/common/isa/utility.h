#ifndef RVM_COMMON_ISA_UTILITY_H
#define RVM_COMMON_ISA_UTILITY_H

#include "rocketvm/common/isa/opcode.h"
#include "rocketvm/common/isa/register.h"
#include "rocketvm/common/isa/instruction.h"

const char *rvm_opcode_to_string(rvm_opcode_t opcode);
const char *rvm_reg_to_string(rvm_reg_t reg);
const char *rvm_instr_mode_to_string(rvm_instr_mode_t mode);

#endif
