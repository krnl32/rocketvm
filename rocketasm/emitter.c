#include "rocketasm/emitter.h"
#include "rocketasm/encoder.h"

#include <rocketvm/common/utility/logger.h>
#include <stdio.h>

int rsm_emit_binary(const rsm_program_t *program, const char *path)
{
	FILE *fp = fopen(path, "wb");
	if (!fp) {
		perror("fopen");
		return -1;
	}

	for (size_t i = 0; i < program->instr_count; i++) {
		int encoded = rsm_encode_instr(program, &program->instrs[i], i);
		if (encoded == -1) {
			rvm_error("rsm_encoder_encode_instr failed");
			return -1;
		}

		uint16_t word = (uint16_t)encoded;
		rvm_debug("[%04zu] 0x%04X", i, (uint16_t)word);

		// little-endian
		uint8_t bytes[2];
		bytes[0] = (uint8_t)(word & 0xFF);
		bytes[1] = (uint8_t)((word >> 8) & 0xFF);

		fwrite(bytes, 1, 2, fp);
	}

	rvm_info("rsm_emit_binary written to %s", path);
	fclose(fp);
	return 0;
}
