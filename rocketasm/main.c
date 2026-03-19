#include "rocketasm/lexer.h"
#include "rocketasm/parser.h"
#include "rocketasm/program.h"

#include <rocketvm/common/utility/logger.h>
#include <rocketvm/common/utility/io.h>
#include <stdio.h>

int main(void)
{
	const char *path = "/home//Workspace/rocketvm/examples/simple.rsm";

	size_t src_size;
	char *src = rvm_io_read_file(path, &src_size);

	rsm_lexer_t *lexer = rsm_lexer_create(src, src_size);
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

	rsm_program_dump_instrs(program);
	printf("\n");
	rsm_program_dump_labels(program);

	return 0;
}
