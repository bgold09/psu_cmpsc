/* base library for CMPSC 311 projects
 *
 * Authors:   Aaron Goodermuth, Brian Golden
 * Emails:    ajg5353@psu.edu,  bwg5079@psu.edu
 *
 * 10 April 2012
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#include "pr7_lib.h"

//------------------------------------------------------------------------------

// global variables set from the command line

char *prog = "[no name]";	// program name
int verbose = 0;		// -v option, extra output

//------------------------------------------------------------------------------

// utility functions

// check function arguments

// #define verify(tf, msg) cmpsc311_verify(tf, msg, __func__, __LINE__)

void cmpsc311_verify(const int tf, const char *msg, const char *func, const int line)
{
  if (tf == 0)
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

//------------------------------------------------------------------------------
