#define _XOPEN_SOURCE 700
#include <ftw.h>
#include <string.h>
#include <stdlib.h>

#include "maje.h"

static struct majefile **filelist = NULL;
static size_t nfiles = 0;

static int addfile(const char *path, const struct stat *st, int flags, struct FTW *ft)
{
	(void)st; (void)flags; (void)ft;

	if (path[0] == '.' && path[1] == '/') {
		path += 2;
	}

	if (strcmp(path + strlen(path) - 2, ".c") == 0) {
		struct majefile **tmp = realloc(filelist, sizeof(*filelist) * (nfiles + 2));
		if (tmp == NULL) {
			return 1;
		}
		filelist = tmp;

		filelist[nfiles] = calloc(1, sizeof(*filelist[nfiles]) + strlen(path) + 1);

		filelist[nfiles]->st = *st;
		strcpy(filelist[nfiles]->path, strdup(path));

		filelist[nfiles + 1] = NULL;

		nfiles++;
	}

	return 0;
}

struct majefile ** find_source_files(const char *dir)
{
	nftw(dir, addfile, -1, 0);
	return filelist;
}
