# CMPSC 473 Project 2, makefile for use on Linux
#
# Authors:    Mike Fitz, Brian Golden
# Emails:     mrf5200@psu.edu, bwg5079@psu.edu
#
# 1 April 2013

PNAME=out
OPT=-W -Wall -Wextra -Werror -std=c89 -ggdb
SRC=
LIB = my_memory.c
INC = $(LIB:.c=.h) 
ALL = $(SRC) $(LIB) $(INC)

${PNAME}: $(ALL) 
	gcc $(OPT) -o $(PNAME) $(SRC)

clean:
	rm -f ${PNAME} a.out *.o temp
