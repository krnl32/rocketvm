#ifndef _RVM_CPU_DEBUG_H
#define _RVM_CPU_DEBUG_H

#include "rocketvm/cpu/cpu.h"

void rvm_cpu_dump(rvm_cpu_t *cpu);
void rvm_instr_dump(rvm_instr_t instr);

const char *rvm_opcode_to_string(rvm_opcode_t opcode);
const char *rvm_instr_mode_to_string(rvm_instr_mode_t mode);

#endif
