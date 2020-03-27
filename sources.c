#define _XOPEN_SOURCE 700
#include <ftw.h>
#include <string.h>
#include <stdlib.h>

#include "maje.h"

static struct majefile *filelist = NULL;

static int add_source(const char *path, const struct stat *st, int flags, struct FTW *ft)
{
	(void)flags; (void)ft;

	if (path[0] == '.' && path[1] == '/') {
		path += 2;
	}

	if (strcmp(path + strlen(path) - 2, ".c") == 0) {
		filelist = insert_file(filelist, path, st);
		if (filelist == NULL) {
			return 1;
		}
	}

	return 0;
}

struct majefile * find_source_files(const char *dir)
{
	nftw(dir, add_source, -1, 0);

	if (filelist == NULL) {
		return NULL;
	}

	while (filelist->prev != NULL) {
		filelist = filelist->prev;
	}

	return filelist;
}
