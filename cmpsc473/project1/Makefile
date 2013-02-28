# Makefile for cmpsc 473 project 1

PNAME=out
OPT=-W -Wall -Wextra -std=c89 -lpthread -lrt -lc -lm -ggdb
#OPT=-W -Wall -Wextra -Werror -std=c89 -lpthreadi -lrt -lc -lm -ggdb
SOURCE=project1.c scheduler.c

${PNAME}: project1.c scheduler.c 
	gcc ${OPT} -o ${PNAME} ${SOURCE}

clean:
	rm -f ${PNAME} a.out *.o temp
