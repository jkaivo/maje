#define _XOPEN_SOURCE 700
#include <libgen.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "maje.h"

static void usage(char *progname)
{
	printf("usage: %s [-n]\n", progname);
}

int main(int argc, char *argv[])
{
	int noexec = 0;
	char *srcdir = NULL;

	int c;
	while ((c = getopt(argc, argv, "n")) != -1) {
		switch (c) {
		case 'n':
			noexec = 1;
			break;

		default:
			usage(argv[0]);
			return 1;
		}
	}

	if (optind < argc) {
		fprintf(stderr, "maje: extra operands\n");
		return 1;
	}

	srcdir = argv[optind];

	if (srcdir == NULL) {
		srcdir = ".";
	}

	struct majefile *sources = find_source_files(srcdir);
	char *mainname = find_main(sources);
	if (!mainname) {
		printf("libraries not yet supported\n");
		return 0;
	}
	char *target = strdup(mainname);
	target[strlen(target) - 2] = '\0';
	target = basename(target);

	FILE *makefile = fopen("Makefile", "w");
	if (makefile == NULL) {
		perror("fopen");
		return 1;
	}

	make_makefile("Makefile", sources, target);

	if (noexec) {
		return 0;
	}

	char *args[] = { "make", NULL };
	execvp(args[0], args);
}
