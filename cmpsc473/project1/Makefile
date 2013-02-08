# Makefile for cmpsc 473 tests

OPT = -W -Wall -Wextra -Werror -std=c89 -ggdb

threadtest: threadtest.c
	gcc ${OPT} -pthread -o threadtest threadtest.c 

exercise1: exercise1.c
	gcc ${OPT} -pthread -o exercise1 exercise1.c

clean:
	rm -f threadtest exercise1 a.out *.o
