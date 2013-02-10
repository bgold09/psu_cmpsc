# Makefile for cmpsc 473 tests

PNAME=out
OPT=-W -Wall -Wextra -Werror -std=c89 -lpthreadi -lrt -lc -lm -ggdb
SOURCE=project1.c scheduler.c

${PNAME}: scheduler.c project1.c
	gcc ${OPT} -o ${PNAME} ${SOURCE}

clean:
	rm -f ${PNAME} a.out *.o
