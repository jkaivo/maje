#define _XOPEN_SOURCE 700
#include <string.h>
#include <stdlib.h>

#include "maje.h"

struct majefile *insert_file(struct majefile *list, const char *path, const struct stat *st)
{
	struct majefile *tmp = malloc(sizeof(*tmp) + strlen(path) + 1);
	if (tmp == NULL) {
		return NULL;
	}

	tmp->next = NULL;
	tmp->prev = NULL;

	if (st) {
		tmp->st = *st;
	} else {
		tmp->st = (struct stat) { 0 };
	}

	strcpy(tmp->path, path);

	if (list == NULL) {
		return tmp;
	}

	tmp->next = list->next;
	tmp->prev = list;
	list->next = tmp;
	if (tmp->next != NULL) {
		tmp->next->prev = tmp;
	}
	return tmp;
}
