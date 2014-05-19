# CMPSC 311 Project 7, makefile for use on Solaris
#
# Authors:    Aaron Goodermuth, Brian Golden
# Emails:     ajg5353@psu.edu,  bwg5079@psu.edu
#
# 10 April 2012

SRC = pr7.3.c

LIB = table.c pr7_lib.c pr7_signal.c
INC = $(LIB:.c=.h) 

ALL = $(SRC) $(LIB) $(INC)

OPT = -D_POSIX_C_SOURCE=200112L -D_XOPEN_SOURCE=600

pr7: $(ALL)
	c99 -v -o pr7 $(OPT) $(SRC) $(LIB)

check: $(ALL)
	c99 -v -o pr7 $(OPT) $(SRC) $(LIB)
	gcc -std=c99 -Wall -Wextra -o pr7 $(OPT) $(SRC) $(LIB)
	lint $(SRC) $(LIB)
	rm pr7

clean: 
	rm -f pr7 *.o
