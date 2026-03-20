#ifndef RVM_VM_H
#define RVM_VM_H

#include "rocketvm/cpu/cpu.h"
#include "rocketvm/memory.h"

typedef struct {
	rvm_cpu_t cpu;
	rvm_memory_t mem;
} rvm_vm_t;

rvm_vm_t *rvm_vm_create(void);
void rvm_vm_destroy(rvm_vm_t *vm);
int rvm_vm_load(rvm_vm_t *vm, const char *rvm_path);
int rvm_vm_run(rvm_vm_t *vm);

#endif
