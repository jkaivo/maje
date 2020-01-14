#define _XOPEN_SOURCE 700
#include <libgen.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "maje.h"

static void make_header(FILE *makefile, const char *target)
{
	fprintf(makefile, ".POSIX:\n\n");
	fprintf(makefile, "# This Makefile was generated by maje\n");
	fprintf(makefile, "# See https://gitlab.com/jkaivo/maje/ for more information\n");
	fprintf(makefile, "# Do not edit this Makefile by hand\n\n");

	fprintf(makefile, "default: all\n\n");

	fprintf(makefile, "CC=c99\n");
	fprintf(makefile, "LD=$(CC)\n");
	fprintf(makefile, "CFLAGS=-Wall -Wextra -Wpedantic -Werror -g\n");
	fprintf(makefile, "LDFLAGS=\n");
	fprintf(makefile, "\n");

	fprintf(makefile, "all: %s\n\n", target);

	fprintf(makefile, "clean:\n");
	fprintf(makefile, "\trm -f %s *.o\n\n", target);
}

static void add_object(FILE *makefile, const char *src, const char *target)
{
	char *fullobj = strdup(src);
	char *obj = basename(fullobj);
	obj[strlen(obj) - 1] = 'o';

	fprintf(makefile, "%s: %s\n", target, obj);
	fprintf(makefile, "%s: %s\n", obj, src);
	fprintf(makefile, "\t$(CC) $(CFLAGS) -c %s\n\n", src);

	free(fullobj);
}

void make_makefile(const char *makepath, struct majefile *sources, const char *target)
{
	FILE *makefile = fopen(makepath, "w");
	if (makefile == NULL) {
		perror("fopen: Makefile");
		return;
	}

	make_header(makefile, target);
	for (struct majefile *src = sources; src != NULL; src = src->next) {
		add_object(makefile, src->path, target);
	}

	fprintf(makefile, "%s:\n", target);
	fprintf(makefile, "\t$(LD) $(LDFLAGS) -o $@ *.o\n");

	fclose(makefile);
}
