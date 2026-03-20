#include <rocketvm/vm.h>
#include <rocketvm/common/utility/logger.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char **argv)
{
	if (argc <= 1) {
		fprintf(stdout, "Usage: %s <bin.rvm>\n", argv[0]);
		return 0;
	}

	const char *rvm_path = argv[1];
	if (access(rvm_path, R_OK) == -1) {
		perror(rvm_path);
		return -1;
	}

	rvm_vm_t *vm = rvm_vm_create();
	if (!vm) {
		rvm_error("rvm_vm_create failed");
		return -1;
	}

	if (rvm_vm_load(vm, rvm_path) == -1) {
		rvm_error("rvm_vm_create failed");
		rvm_vm_destroy(vm);
		return -1;
	}

	if (rvm_vm_run(vm) == -1) {
		rvm_error("rvm_vm_run failed");
		rvm_vm_destroy(vm);
		return -1;
	}

	rvm_vm_destroy(vm);
	return 0;
}
