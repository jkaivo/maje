#define _XOPEN_SOURCE 700
#include <assert.h>
#include <fcntl.h>
#include <regex.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <unistd.h>

#include "maje.h"

static bool matches(const char * restrict path, const regex_t * restrict re)
{
	bool ret = false;

	int fd = open(path, O_RDONLY);
	if (fd == -1) {
		return false;
	}

	struct stat st = { 0 };
	if (fstat(fd, &st) != 0) {
		close(fd);
		return false;
	}

	void *map = mmap(NULL, (size_t)st.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
	if (map == MAP_FAILED) {
		close(fd);
		return false;
	}

	
	if (regexec(re, map, (size_t)st.st_size, NULL, 0) == 0) {
		ret = true;
	}

	munmap(map, st.st_size);
	close(fd);

	return ret;
}

char *find_main(char **sources)
{
	regex_t re;
	int rc = regcomp(&re, "int[[:space:]]+main[[:space:]\(]", REG_EXTENDED | REG_NEWLINE | REG_NOSUB);
	assert(rc == 0);
	char *ret = NULL;

	for (char **src = sources; *src != NULL; src++) {
		if (matches(*src, &re)) {
			ret = *src;
			break;
		}
	}

	regfree(&re);

	return ret;
}
