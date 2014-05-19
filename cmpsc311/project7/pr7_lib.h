/* base library for CMPSC 311 projects
 *
 * Authors:   Aaron Goodermuth, Brian Golden
 * Emails:    ajg5353@psu.edu,  bwg5079@psu.edu
 *
 */

#ifndef CMPSC311_H
#define CMPSC311_H

//------------------------------------------------------------------------------

// global variables set from the command line

extern char *prog;	// program name
extern int verbose;	// -v option, extra output

//------------------------------------------------------------------------------

// utility functions

// check function arguments

#define verify(tf, msg) cmpsc311_verify(tf, msg, __func__, __LINE__)

void cmpsc311_verify(const int tf, const char *msg				, const char *func, const int line);

//------------------------------------------------------------------------------

// check function return values
//   malloc
//   strdup
//   fopen

#define Malloc(size) cmpsc311_malloc(size, __func__, __LINE__)

void *cmpsc311_malloc(const size_t size						, const char *func, const int line);

//------------------------------------------------------------------------------

#endif
