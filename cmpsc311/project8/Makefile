# CMPSC 311, Spring 2012, Project 8 
#
# Author:   Brian Golden,    Aaron Goodermuth
# Email:    bwg5079@psu.edu, ajg5353@psu.edu

SRC = pr8.4.c
LIB = cmpsc311.c names.c macro.c sources.c recipes.c targets.c
INC = cmpsc311.h names.h macro.h sources.h recipes.h targets.h

# select the version of mtime() appropriate for your OS

# Solaris, mtime() uses time_t st_mtime
OPT = -D_XOPEN_SOURCE=600

# Linux, mtime() uses struct timespec st_mtim
#OPT = -D_XOPEN_SOURCE=700 -DMTIME=1

# Mac OS X, mtime() uses struct timespec st_mtimespec
# OPT = -DMTIME=2

pr8: $(SRC) $(LIB) $(INC)
	c99 $(OPT) -v -o pr8 $(SRC) $(LIB)

check: $(SRC) $(LIB) $(INC)
	c99 -$(OPT) -v -o pr8  $(SRC) $(LIB)
	gcc -std=c99 $(OPT) -Wall -Wextra -o pr8 $(SRC) $(LIB)
	lint -Xc99 $(SRC) $(LIB)
	rm pr8

pr8-lint: $(SRC) $(LIB) $(INC)
	lint -Xc99 $(SRC) $(LIB)

clean: 
	rm -f pr8 a.out *.o a b c d e f g h 
	
test:
	sh test-script
