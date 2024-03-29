/* CMPSC 311, Spring 2012, Project 4
 *
 * Author:   Brian Golden
 * Email:    bwg5079@psu.edu
 *
... <additional comment text>
 */

/* base library for CMPSC 311 projects
 * version of 14 Feb. 2012
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <errno.h>
#include <string.h>

#include "cmpsc311.h"

//------------------------------------------------------------------------------

// global variables set from the command line

char *prog = "[no name]";	// program name
int verbose = 0;		// -v option, extra output
				// -v can be repeated for even more output

//------------------------------------------------------------------------------

// utility functions

// guard against null pointer for character string output
const char *safe_string(const char *str)
{
  if (str == NULL) return "(null)";
  else return str;
}

/* The Solaris C library doesn't like printf("%s", p) when p is a null pointer.
 * It will generate a segmentation fault, and end the program.  This is
 * an acceptable implementation according to the C and Posix Standards.
 * The GNU C library (on Linux and Mac OS X) is more generous; it will print
 * the string "(null)" and keep going.
 * If you compile with GCC with all the warnings turned on, there are complaints
 * about printf("%s", NULL); but none about printf("%s", p) when p happens to be
 * a null pointer.
 * With the safe_string() function, we can act like the GNU C library version of
 * printf(), even on Solaris.
 * Of course, we could also use assert() to catch NULL pointers, which is
 * often a better idea, unless you really want the program to keep going.
 */

//------------------------------------------------------------------------------

// check function arguments

// #define verify(tf, msg) cmpsc311_verify(tf, msg, __func__, __LINE__)

void cmpsc311_verify(const bool tf, const char *msg, const char *func, const int line)
{
  if (tf == false)
    {
      fprintf(stderr, "%s: %s() at line %d failed: %s\n", prog, func, line, msg);
      exit(EXIT_FAILURE);
    }
}

//------------------------------------------------------------------------------

// check function return values
//   malloc
//   strdup
//   fopen

// #define Malloc(size) cmpsc311_malloc(size, __func__, __LINE__)

void *cmpsc311_malloc(const size_t size, const char *func, const int line)
{
  void *p = malloc(size);
  if (p == NULL)
    {
      fprintf(stderr, "%s: %s() at line %d failed: malloc(): %s\n", prog, func, line, strerror(errno));
      exit(EXIT_FAILURE);
    }

  if (verbose > 2)
    { fprintf(stderr, "%s: malloc(%zd) at %p from %s line %d\n", prog, size, p, func, line); }

  return p;
}

// #define Strdup(s) cmpsc311_strdup(s, __func__, __LINE__)

char *cmpsc311_strdup(const char *s, const char *func, const int line)
{
  char *p = strdup(s);
  if (p == NULL)
    {
      fprintf(stderr, "%s: %s() at line %d failed: strdup(): %s\n", prog, func, line, strerror(errno));
      exit(EXIT_FAILURE);
    }

  if (verbose > 2)
    { fprintf(stderr, "%s: strdup(%zd) at %p from %s line %d\n", prog, strlen(s)+1, p, func, line); }

  return p;
}

// #define Fopen(filename,mode) cmpsc311_fopen(filename, mode, __func__, __LINE__)

FILE *cmpsc311_fopen(const char *restrict filename, const char *restrict mode, const char *func, const int line)
{
  FILE *f = fopen(filename, mode);
  if (f == NULL)
    {
      fprintf(stderr, "%s: %s() at line %d failed: fopen(): %s\n", prog, func, line, strerror(errno));
      exit(EXIT_FAILURE);
    }

  return f;
}

//------------------------------------------------------------------------------

