#define _XOPEN_SOURCE 700
#include <ftw.h>
#include <string.h>
#include <stdlib.h>

#include "maje.h"

static struct majefile *filelist = NULL;
static struct majefile *tail = NULL;

static int addfile(const char *path, const struct stat *st, int flags, struct FTW *ft)
{
	(void)flags; (void)ft;

	if (path[0] == '.' && path[1] == '/') {
		path += 2;
	}

	if (strcmp(path + strlen(path) - 2, ".c") == 0) {
		struct majefile *tmp = malloc(sizeof(*filelist) + strlen(path) + 1);
		if (tmp == NULL) {
			return 1;
		}

		tmp->next = NULL;
		tmp->st = *st;
		strcpy(tmp->path, path);

		if (tail == NULL) {
			filelist = tmp;
			tail = filelist;
		} else {
			tail->next = tmp;
			tail = tail->next;
		}
	}

	return 0;
}

struct majefile * find_source_files(const char *dir)
{
	nftw(dir, addfile, -1, 0);
	return filelist;
}
