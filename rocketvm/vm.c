#include "rocketvm/vm.h"
#include "rocketvm/common/utility/logger.h"
#include "rocketvm/common/utility/io.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

rvm_vm_t *rvm_vm_create(void)
{
	rvm_vm_t *vm = malloc(sizeof(*vm));
	if (!vm) {
		perror("malloc");
		return NULL;
	}

	memset(&vm->cpu, 0, sizeof(vm->cpu));
	memset(&vm->mem, 0, sizeof(vm->mem));

	vm->cpu.regs.rsp = RVM_MEMORY_STACK_END;
	return vm;
}

void rvm_vm_destroy(rvm_vm_t *vm)
{
	if (vm) {
		free(vm);
	}
}

int rvm_vm_load(rvm_vm_t *vm, const char *rvm_path)
{
	size_t rvm_size;
	char *rvm = rvm_io_read_file(rvm_path, &rvm_size);

	if (!rvm) {
		rvm_error("rvm_io_read_file(%s) failed", rvm_path);
		return -1;
	}

	if (rvm_size > (RVM_MEMORY_SIZE - RVM_MEMORY_PROGRAM_START)) {
		rvm_error("rvm_vm_load %s too large (%zu bytes) for RVM", rvm_path, rvm_size);
		free(rvm);
		return -1;
	}

	memcpy(&vm->mem.data[RVM_MEMORY_PROGRAM_START], rvm, rvm_size);
	free(rvm);

	rvm_info("rvm_vm_load loaded RVM: %s (%zu bytes)", rvm_path, rvm_size);
	return 0;
}

int rvm_vm_run(rvm_vm_t *vm)
{
	while (!vm->cpu.halt) {
		// rvm_cpu_dump(&cpu);
		// printf("\n");

		if (rvm_cpu_cycle(&vm->cpu, &vm->mem) == -1) {
			rvm_error("rvm_cpu_cycle failed");
			return -1;
		}
	}

	return 0;
}
