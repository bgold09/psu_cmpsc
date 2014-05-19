# CMPSC 311, Spring 2012, Project 2
#
# Author:   Brian Golden
# Email:    bwg5079@psu.edu

SRC = pr2.3.c

# use Sun's C compiler, GCC, and Lint
all: pr2-sun pr2-gcc pr2-lint

# The following compile commands all use C89, with warnings enabled.
# For C99, make these changes:
#    [Sun]  replace cc with c99, replace lint with lint -Xc99
#    [GCC]  replace gcc with gcc -std=c99

pr2-sun: $(SRC)
	c99 -v -o pr2-sun $(SRC)

pr2-gcc: $(SRC)
	gcc -std=c99 -Wall -Wextra -o pr2-gcc $(SRC)

pr2-lint: $(SRC)
	lint -Xc99 $(SRC)

# This will create four files, for testing purposes.
# To see the difference between them, use the "ls -l" and "od -c" commands.
asdf-dos: dos-style.c unix-style.c oldmacos-style.c empty-style.c
	gcc -o asdf dos-style.c
	./asdf > asdf-dos
	gcc -o asdf unix-style.c
	./asdf > asdf-unix
	gcc -o asdf oldmacos-style.c
	./asdf > asdf-oldmacos
	gcc -o asdf empty-style.c
	./asdf > asdf-empty
	rm asdf

compare-asdf: asdf-dos
	ls -l asdf-dos asdf-unix asdf-oldmacos asdf-empty
	od -c asdf-dos
	od -c asdf-unix
	od -c asdf-oldmacos
	od -c asdf-empty

test-sun-1: pr2-sun asdf-dos
	./pr2-sun < asdf-dos
	./pr2-sun < asdf-unix
	./pr2-sun < asdf-oldmacos
	./pr2-sun < asdf-empty

test-gcc-1: pr2-gcc asdf-dos
	./pr2-gcc < asdf-dos
	./pr2-gcc < asdf-unix
	./pr2-gcc < asdf-oldmacos
	./pr2-gcc < asdf-empty

# This will use files in the directory testdata
test-sun-2: pr2-sun
	./pr2-sun < testdata/vi123
	./pr2-sun < testdata/word123.txt
	./pr2-sun < testdata/wordpad123.txt
	./pr2-sun < testdata/mixed123
	./pr2-sun < testdata/Workbook1dos.csv
	./pr2-sun < testdata/Workbook1win.csv

# This will use files in the directory testdata
test-gcc-2: pr2-gcc
	./pr2-gcc < testdata/vi123
	./pr2-gcc < testdata/word123.txt
	./pr2-gcc < testdata/wordpad123.txt
	./pr2-gcc < testdata/mixed123
	./pr2-gcc < testdata/Workbook1dos.csv
	./pr2-gcc < testdata/Workbook1win.csv

clean:
	rm -f pr2-sun pr2-gcc pr2 a.out *.o
	rm -f asdf-dos asdf-unix asdf-oldmacos asdf-empty

