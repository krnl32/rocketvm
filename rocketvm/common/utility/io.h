#ifndef RVM_COMMON_UTILITY_IO_H
#define RVM_COMMON_UTILITY_IO_H

#include <stddef.h>

char *rvm_io_read_file(const char *path, size_t *out_size);

#endif
