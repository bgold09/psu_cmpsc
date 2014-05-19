# CMPSC 311, Project 1

SRC = pr1.2.c

# Solaris, Linux, Mac OS X
run-gcc: $(SRC)
	gcc -std=c99 -Wall -Wextra -o pr1 -m32 $(SRC)
	./pr1 > m32
	gcc -std=c99 -Wall -Wextra -o pr1 -m64 $(SRC)
	./pr1 > m64
	-diff m32 m64 > m32-m64-diff
	rm pr1

# Solaris only - c99 does not recognize __alignof__
run-c99: $(SRC)
	c99 -v -o pr1 -m32 $(SRC)
	./pr1 > m32
	c99 -v -o pr1 -m64 $(SRC)
	./pr1 > m64
	-diff m32 m64 > m32-m64-diff
	rm pr1

# use this on one of the workstations in 218 IST (Linux, Intel processors)
lab: run-gcc
	mv m32 m64 m32-m64-diff lab

# use this on eru.cse.psu.edu (Solaris, SPARC processors)
sun: run-gcc
	mv m32 m64 m32-m64-diff sun

# use this on ladon.cse.psu.edu (Linux, AMD processors)
amd: run-gcc
	mv m32 m64 m32-m64-diff amd

# use this on an iMac (Mac OS X, Intel processor)
imc: run-gcc
	mv m32 m64 m32-m64-diff imc

# use this on a PowerMac (Mac OS X, PowerPC processor)
pmc: run-gcc
	mv m32 m64 m32-m64-diff pmc

setup:
	-mkdir lab
	-mkdir sun
	-mkdir amd
	-mkdir imc
	-mkdir pmc

clean:
	rm -f m32 m64 m32-m64-diff
	rm -f pr1

Clean:
	rm -f -r lab sun amd imc pmc
