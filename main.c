#define _XOPEN_SOURCE 700
#include <fcntl.h>
#include <regex.h>
#include <sys/mman.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "maje.h"

static bool matches(const struct majefile * restrict file, const regex_t * restrict re)
{
	bool ret = false;

	int fd = open(file->path, O_RDONLY);
	if (fd == -1) {
		return false;
	}

	void *map = mmap(NULL, (size_t)file->st.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
	if (map == MAP_FAILED) {
		close(fd);
		return false;
	}

	
	if (regexec(re, map, (size_t)file->st.st_size, NULL, 0) == 0) {
		ret = true;
	}

	munmap(map, file->st.st_size);
	close(fd);

	return ret;
}

char *find_main(struct majefile *sources)
{
	regex_t re;
	int rc = regcomp(&re, "int[[:space:]]+main[[:space:]\\(]",
		REG_EXTENDED | REG_NEWLINE | REG_NOSUB);
	if (rc != 0) {
		fprintf(stderr, "maje: regcomp() failed\n");
		abort();
	}

	char *ret = NULL;

	for (struct majefile *src = sources; src != NULL; src = src->next) {
		if (matches(src, &re)) {
			ret = src->path;
			break;
		}
	}

	regfree(&re);

	return ret;
}
