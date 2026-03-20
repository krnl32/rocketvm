#include "rocketasm/lexer.h"
#include "rocketasm/parser.h"
#include "rocketasm/program.h"
#include "rocketasm/encoder.h"
#include "rocketasm/emitter.h"

#include <rocketvm/common/utility/logger.h>
#include <rocketvm/common/utility/io.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char **argv)
{
	if (argc <= 1) {
		fprintf(stdout, "Usage: %s <*.rsm>\n", argv[0]);
		return 0;
	}

	const char *rsm_path = argv[1];
	if (access(rsm_path, R_OK) == -1) {
		perror(rsm_path);
		return -1;
	}

	size_t rsm_size;
	char *rsm = rvm_io_read_file(rsm_path, &rsm_size);

	rsm_lexer_t *lexer = rsm_lexer_create(rsm, rsm_size);
	if (!lexer) {
		rvm_error("rsm_lexer_create failed");
		return -1;
	}

	rsm_parser_t *parser = rsm_parser_create();
	if (!parser) {
		rvm_error("rsm_parser_create failed");
		return -1;
	}

	rsm_program_t *program = rsm_program_create();
	if (!program) {
		rvm_error("rsm_program_create failed");
		return -1;
	}

	if (rsm_parser_parse(parser, lexer, program) == -1) {
		rvm_error("rsm_parser_parse failed");
		return -1;
	}

	if (rsm_resolve_labels(program) == -1) {
		rvm_error("rsm_resolver_resolve_labels failed");
		return -1;
	}

#ifdef DEBUG
	printf("\n");
	rsm_program_dump_instrs(program);
	printf("\n");
	rsm_program_dump_labels(program);
	printf("\n");
#endif

	if (rsm_emit_binary(program, "out.rvm") == -1) {
		rvm_error("rsm_emit_binary failed");
		return -1;
	}

	rsm_program_destroy(program);
	rsm_parser_destroy(parser);
	rsm_lexer_destroy(lexer);
	free(rsm);

	return 0;
}
