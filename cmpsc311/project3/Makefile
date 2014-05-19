# CMPSC 311, Spring 2012, Project 3
#
# Author:   Brian Golden
# Email:    bwg5079@psu.edu

CC = gcc -std=c99 -Wall -Wextra

SRC = pass1.c pass2.c pass3.c pass4.c
# LIB = pr3lib.c
# INC = pr3lib.h
# If you did not use the files pr3lib.c and pr3lib.h,
# then remove or comment-out the lines that define LIB and INC

all: pass1 pass2 pass3 pass4

pass1: pass1.c $(LIB) $(INC)
	$(CC) -o pass1 pass1.c $(LIB)

pass2: pass2.c $(LIB) $(INC)
	$(CC) -o pass2 pass2.c $(LIB)

pass3: pass3.c $(LIB) $(INC)
	$(CC) -o pass3 pass3.c $(LIB)

pass4: pass4.c $(LIB) $(INC)
	$(CC) -o pass4 pass4.c $(LIB)

test1: pass1
	./pass1 < /dev/null
	./pass1 < testfiles/test0
	cat     < testfiles/test1.c | od -c
	./pass1 < testfiles/test1.c | od -c

test3: pass3
	./pass3 < testfiles/test3a
	./pass3 < testfiles/test3b
	./pass3 < testfiles/test3c

create-tests:
	cd testfiles ; make all

clean:
	rm -f pass[1234] a.out *.o
