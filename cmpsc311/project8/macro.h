/* CMPSC 311, Spring 2012, Project 8
 *
 * Author:   Brian Golden,    Aaron Goodermuth
 * Email:    bwg5079@psu.edu, ajg5353@psu.edu
 *
 * makefile macro-expansion header
 * version 1, 24 April 2012
 */

#ifndef CMPSC311_MACRO_H
#define CMPSC311_MACRO_H

void  macro_list_print(void);
char *macro_body(char *name);
int   macro_length(char *name);
void  macro_set(char *name, char *body);

// assume in[] is constructed properly
// assume out[] is large enough
void  macro_expand(char *in, char *out);

// assume in[] is constructed properly
// returns length of the expansion
int   macro_expand_length(char *in);

#endif

