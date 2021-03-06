Maje
====

Maje is a simple utility to generate basic Makefiles for simple utilities. Maje
scans a directory tree for C source code, and builds a Makefile with proper
dependency information based on that code. The Makefile in this source tree
is itself generated by Maje.

There are a few restrictions:

- Source files must be named ending with .c.

- Only executables are supported, not libraries (there must be a main()).

- Only one executable per source tree is supported (exactly one main()).

- Maje expects the resulting binary to have the same base name as the source
file containing main() (e.g. if main() is in maje.c, the binary will be called
maje).

Usage
=====

Run Maje with:

    maje [-n]

By default, Maje will execute `make` when it is done creating the Makefile. Use
the `-n` option to prevent this.

Maje creates Makefiles with prefixes $(SRCDIR) and $(OBJDIR) for the source
directory and object directory, respectively. By default these are set to
`.` (the current directory). An out-of-tree build can be accomplished by 
first generating the Makefile in the source directory and manually specifying
the object directory as an option to `make`:

    maje -n
    mkdir obj
    make OBDIR=obj

Or:

    maje -n
    mkdir ../build
    cd ../build
    make -f ${OLDPWD}/Makefile SRCDIR=${OLDPWD}
