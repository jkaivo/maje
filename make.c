#define _XOPEN_SOURCE 700
#include <stdio.h>

#include "maje.h"

static void make_header(FILE *makefile, const char *target)
{
	fprintf(makefile, ".POSIX:\n");
	fprintf(makefile, "default: all\n");

	fprintf(makefile, "CC=c99\n");
	fprintf(makefile, "CFLAGS=-Wall -Wextra -Wpedantic -Werror -g\n");

	fprintf(makefile, "all: %s\n", target);

	fprintf(makefile, "clean:\n");
	fprintf(makefile, "\trm -f %s *.o\n", target);
}

static void addfile(FILE *makefile, const char *src, const char *target)
{
	fprintf(makefile, "%s: %s\n", src, src);
	fprintf(makefile, "%s: %s\n", target, src);
}

void make_makefile(const char *makepath, char **sources, const char *target)
{
	FILE *makefile = fopen(makepath, "w");
	if (makefile == NULL) {
		perror("fopen: Makefile");
		return;
	}

	make_header(makefile, target);
	for (char **src = sources; *src != NULL; src++) {
		addfile(makefile, *src, target);
	}

	fprintf(makefile, "%s:\n", target);
	fprintf(makefile, "\t$(CC) -o $@");
	for (char **src = sources; *src != NULL; src++) {
		fprintf(makefile, " %s", *src);
	}
	fprintf(makefile, "\n");

	fclose(makefile);
}
