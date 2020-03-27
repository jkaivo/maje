#ifndef MAJE_H
#define MAJE_H

#include <sys/stat.h>

struct majefile {
	struct majefile *next;
	struct majefile *prev;
	struct stat st;
	char path[];
};

struct majefile *find_source_files(const char *path);
char *find_main(struct majefile *sources);
struct majefile *find_includes(const struct majefile *file);
void make_makefile(const char *makefile, struct majefile *sources, const char *target);
struct majefile *insert_file(struct majefile *list, const char *path, const struct stat *st);

#endif
