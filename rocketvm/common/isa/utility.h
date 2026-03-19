#ifndef RVM_COMMON_ISA_UTILITY_H
#define RVM_COMMON_ISA_UTILITY_H

#include "rocketvm/common/isa/opcode.h"
#include "rocketvm/common/isa/register.h"

const char *rvm_opcode_to_string(rvm_opcode_t opcode);
const char *rvm_reg_to_string(rvm_reg_t reg);

#endif
