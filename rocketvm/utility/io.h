#ifndef _RVM_IO_H
#define _RVM_IO_H

#include <stddef.h>

char *rvm_io_read_file(const char *path, size_t *out_size);

#endif
