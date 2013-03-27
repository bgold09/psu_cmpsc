# CMPSC 473 Project 2, makefile for use on Linux
#
# Authors:    Mike Fitz, Brian Golden
# Emails:     mrf5200@psu.edu, bwg5079@psu.edu
#
# 1 April 2013

PNAME=out$(NUM)
OPT=-W -Wall -Wextra -lm -std=c89 -ggdb
SRC=tests/inputs/test$(NUM).c
LIB = my_memory.c
INC = $(LIB:.c=.h) 
ALL = $(SRC) $(LIB) $(INC)
NUM=1

$(PNAME): $(ALL) 
	gcc $(OPT) -o $(PNAME) $(LIB) $(SRC)

clean:
	rm -f out* a.out *.o temp test*_output.txt
