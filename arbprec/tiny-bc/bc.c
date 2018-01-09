#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <getopt.h>

#include "lex.h"

uint32_t bc_ibase = 10;
uint32_t bc_obase = 10;

int bc_mathlib = 0;
int bc_quiet = 0;
int bc_std = 0;
int bc_warn = 0;

static const struct option bc_opts[] = {
    { "help", no_argument, NULL, 'h' },
    { "mathlib", no_argument, &bc_mathlib, 'l' },
    { "quiet", no_argument, &bc_quiet, 'q' },
    { "standard", no_argument, &bc_std, 's' },
    { "version", no_argument, NULL, 'v' },
    { "warn", no_argument, &bc_warn, 'w' },
    { 0, 0, 0, 0},
};

static const char* const bc_short_opts = "hlqsvw";

static const char* const bc_stdin_name = "-";

int main(int argc, char* argv[]) {

	// Create a one-name array.
	const char* stdin_array[] = { bc_stdin_name };

	// Getopt needs this.
	int opt_idx = 0;

	int c = getopt_long(argc, argv, bc_short_opts, bc_opts, &opt_idx);

	while (c != -1) {

		switch (c) {

			case 0:
				// This is the case when a long option is
				// found, so we don't need to do anything.
				break;

			case 'h':
				// TODO: Print help.
				break;

			case 'l':
				bc_mathlib = 'l';
				break;

			case 'q':
				bc_quiet = 'q';
				break;

			case 's':
				bc_std = 's';
				break;

			case 'v':
				// TODO: Print version.
				break;

			case 'w':
				bc_warn = 'w';
				break;

			case '?':
				// Getopt printed an error message, but we should exit.
			default:
				exit(BC_STATUS_INVALID_OPTION);
				break;
		}

		// Get the next option.
		c = getopt_long(argc, argv, bc_short_opts, bc_opts, &opt_idx);
	}

	// Get the number of files.
	uint32_t num_files = argc - optind;

	// Get the list of file names.
	const char** file_names = num_files == 0 ? stdin_array : (const char**) argv + optind;

	// Make sure the number of files is correct.
	num_files = num_files == 0 ? num_files : 1;

	return bc_main(num_files, file_names);
}

BcStatus bc_main(int filec, const char* filev[]) {

	BcStatus status = BC_STATUS_SUCCESS;

	// Print out the options.
	printf("mathlib: %d, quiet: %d, standard: %d, warn: %d\n",
	       bc_mathlib, bc_quiet, bc_std, bc_warn);

	// Print out the file names.
	for (int i = 0; i < filec; ++i) {
		printf("File[%d]: %s\n", i, filev[i]);
	}

	return status;
}
