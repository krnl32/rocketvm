#ifndef RVM_CPU_CTRLF_H
#define RVM_CPU_CTRLF_H

#include "rocketvm/cpu/cpu.h"

void rvm_cpu_exec_cmp(rvm_cpu_t *cpu, rvm_instr_t instr);
void rvm_cpu_exec_jmp(rvm_cpu_t *cpu, rvm_instr_t instr);
void rvm_cpu_exec_jz(rvm_cpu_t *cpu, rvm_instr_t instr);
void rvm_cpu_exec_jnz(rvm_cpu_t *cpu, rvm_instr_t instr);
void rvm_cpu_exec_call(rvm_cpu_t *cpu, rvm_instr_t instr);
void rvm_cpu_exec_ret(rvm_cpu_t *cpu, rvm_instr_t instr);

#endif
