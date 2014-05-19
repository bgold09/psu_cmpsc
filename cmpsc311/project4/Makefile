# CMPSC 311, Spring 2012, Project 4
#
# Author:   Brian Golden 
# Email:    bwg5079@psu.edu

SRC = hake.c
LIB = cmpsc311.c names.c macro.c
INC = cmpsc311.h names.h macro.h

hake-sun: $(SRC) $(LIB) $(INC)
	c99 -D_XOPEN_SOURCE=600 -v -o hake $(SRC) $(LIB)

hake-gcc: $(SRC) $(LIB) $(INC)
	gcc -std=c99 -D_XOPEN_SOURCE=600 -Wall -Wextra -o hake $(SRC) $(LIB)

hake-mac: $(SRC) $(LIB) $(INC)
	gcc -std=c99 -Wall -Wextra -o hake $(SRC) $(LIB)

hake-lint: $(SRC) $(LIB) $(INC)
	lint -Xc99 $(SRC) $(LIB)

clean:
	rm -f hake a.out *.o
