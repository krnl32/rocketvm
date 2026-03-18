#ifndef _RVM_CPU_ALU_H
#define _RVM_CPU_ALU_H

#include "rocketvm/cpu/cpu.h"

void rvm_cpu_exec_add(rvm_cpu_t *cpu, rvm_instr_t instr);
void rvm_cpu_exec_sub(rvm_cpu_t *cpu, rvm_instr_t instr);
void rvm_cpu_exec_mul(rvm_cpu_t *cpu, rvm_instr_t instr);
void rvm_cpu_exec_div(rvm_cpu_t *cpu, rvm_instr_t instr);
void rvm_cpu_exec_mod(rvm_cpu_t *cpu, rvm_instr_t instr);
void rvm_cpu_exec_shl(rvm_cpu_t *cpu, rvm_instr_t instr);
void rvm_cpu_exec_shr(rvm_cpu_t *cpu, rvm_instr_t instr);

#endif
