#define _XOPEN_SOURCE 700
#include <fcntl.h>
#include <regex.h>
#include <sys/mman.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "maje.h"

#define MAX_MATCHES (2)

/* TODO: recurse through headers */
/* TODO: avoid loops */

static struct majefile * find_inc(const struct majefile *file, const regex_t * restrict re)
{
	int fd = open(file->path, O_RDONLY);
	if (fd == -1) {
		return NULL;
	}

	void *map = mmap(NULL, (size_t)file->st.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
	if (map == MAP_FAILED) {
		close(fd);
		return NULL;
	}

	struct majefile *inc = NULL;

	regmatch_t match[MAX_MATCHES];
	
	void *base = map;
	while (regexec(re, base, MAX_MATCHES, match, 0) == 0) {
		if (match[1].rm_so == -1) {
			break;
		}
		char header[FILENAME_MAX] = { 0 };
		char *start = (char*)base + match[1].rm_so;
		memcpy(header, start, match[1].rm_eo - match[1].rm_so);
		inc = insert_file(inc, header, NULL);
		base = start + match[1].rm_eo;
	}

	munmap(map, file->st.st_size);
	close(fd);

	return inc;
}

struct majefile *find_includes(const struct majefile *source)
{
	regex_t re;
	int rc = regcomp(&re,
		"#[[:space:]]*include[[:space:]]+.*\"(.*)\"",
		REG_EXTENDED | REG_NEWLINE);
	if (rc != 0) {
		fprintf(stderr, "maje: regcomp() failed\n");
		abort();
	}

	struct majefile *ret = find_inc(source, &re);

	regfree(&re);

	if (ret != NULL) {
		while (ret->prev != NULL) {
			ret = ret->prev;
		}
	}

	return ret;
}
