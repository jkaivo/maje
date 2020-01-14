#define _XOPEN_SOURCE 700
#include <ftw.h>
#include <string.h>
#include <stdlib.h>

#include "maje.h"

static char **filelist = NULL;
static size_t nfiles = 0;

static int addfile(const char *path, const struct stat *st, int flags, struct FTW *ft)
{
	(void)st; (void)flags; (void)ft;

	if (path[0] == '.' && path[1] == '/') {
		path += 2;
	}

	if (strcmp(path + strlen(path) - 2, ".c") == 0) {
		char **tmp = realloc(filelist, sizeof(*filelist) * (nfiles + 2));
		if (tmp == NULL) {
			return 1;
		}
		filelist = tmp;

		filelist[nfiles] = strdup(path);
		filelist[nfiles + 1] = NULL;
		nfiles++;
	}

	return 0;
}

char ** find_source_files(const char *dir)
{
	nftw(dir, addfile, -1, 0);
	return filelist;
}
