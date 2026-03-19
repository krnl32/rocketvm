#include "rocketvm/utility/io.h"
#include "rocketvm/utility/logger.h"

#include <stdio.h>
#include <stdlib.h>

char *rvm_io_read_file(const char *path, size_t *out_size)
{
	FILE *fp = fopen(path, "rb");
	if (!fp) {
		perror("fopen");
		return NULL;
	}

	if (fseek(fp, 0, SEEK_END) == -1) {
		perror("fseek");
		fclose(fp);
		return NULL;
	}

	long size = ftell(fp);
	if (size == -1) {
		perror("ftell");
		fclose(fp);
		return NULL;
	}

	if (size == 0) {
		rvm_error("dsm_fileio_read: empty file: %s", path);
		fclose(fp);
		return NULL;
	}

	if (fseek(fp, 0, SEEK_SET) == -1) {
		perror("fseek");
		fclose(fp);
		return NULL;
	}

	char *buf = malloc((size_t)size + 1);
	if (!buf) {
		perror("malloc");
		fclose(fp);
		return NULL;
	}

	if (fread(buf, 1, (size_t)size, fp) != (size_t)size) {
		perror("fread");
		free(buf);
		fclose(fp);
		return NULL;
	}

	buf[size] = 0;
	fclose(fp);

	if (out_size) {
		*out_size = (size_t)size;
	}

	return buf;
}
