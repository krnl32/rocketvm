#ifndef RVM_CPU_MEM_H
#define RVM_CPU_MEM_H

#include "rocketvm/cpu/cpu.h"

void rvm_cpu_exec_load(rvm_cpu_t *cpu, rvm_memory_t *mem, rvm_instr_t instr);
void rvm_cpu_exec_store(rvm_cpu_t *cpu, rvm_memory_t *mem, rvm_instr_t instr);

#endif
