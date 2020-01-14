char **find_source_files(const char *path);
char *find_main(char **sources);
void make_makefile(const char *makefile, char **sources, const char *target);
