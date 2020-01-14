#ifndef MAJE_H
#define MAJE_H

#include <sys/stat.h>

struct majefile {
	struct majefile *next;
	struct stat st;
	char path[];
};

struct majefile *find_source_files(const char *path);
char *find_main(struct majefile *sources);
void make_makefile(const char *makefile, struct majefile *sources, const char *target);

#endif
