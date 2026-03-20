#include "rocketasm/lexer.h"
#include "rocketasm/parser.h"
#include "rocketasm/program.h"
#include "rocketasm/encoder.h"
#include "rocketasm/emitter.h"

#include <rocketvm/common/utility/logger.h>
#include <rocketvm/common/utility/io.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char **argv)
{
	if (argc <= 1) {
		fprintf(stdout, "Usage: %s <input.rsm> [-o output.rvm]\n", argv[0]);
		return 0;
	}

	const char *input_path = NULL;
	const char *output_path = "out.rvm";

	for (int i = 1; i < argc; i++) {
		if (!strcmp(argv[i], "-o")) {
			if (i + 1 >= argc) {
				fprintf(stderr, "-o requires out file path\n");
				return -1;
			}
			output_path = argv[++i];
		} else {
			input_path = argv[i];
		}
	}

	if (!input_path) {
		fprintf(stderr, "No input file provided\n");
		return -1;
	}

	if (access(input_path, R_OK) == -1) {
		perror(input_path);
		return -1;
	}

	size_t rsm_size;
	char *rsm = rvm_io_read_file(input_path, &rsm_size);
	if (!rsm) {
		rvm_error("rvm_io_read_file(%s) failed", input_path);
		return -1;
	}

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

	if (rsm_emit_binary(program, output_path) == -1) {
		rvm_error("rsm_emit_binary failed");
		return -1;
	}

	rsm_program_destroy(program);
	rsm_parser_destroy(parser);
	rsm_lexer_destroy(lexer);
	free(rsm);

	return 0;
}
