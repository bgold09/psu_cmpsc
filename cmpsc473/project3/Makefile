# CMPSC 473 Project 3, makefile for use on Linux
#
# Authors:    Mike Fitz, Brian Golden
# Emails:     mrf5200@psu.edu, bwg5079@psu.edu
#
# 26 April 2013

PNAME=out$(NUM)
OPT=-W -Wall -Wextra -lm -ggdb
SRC=tests/test-code$(NUM).c
LIB = 473_mm.c
INC = $(LIB:.c=.h) 
ALL = $(SRC) $(LIB) $(INC)
NUM=1

$(PNAME): $(ALL) 
	gcc $(OPT) -o $(PNAME) $(LIB) $(SRC)

clean:
	rm -f out* a.out *.o temp results.txt
