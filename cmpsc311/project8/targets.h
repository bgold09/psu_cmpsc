/* CMPSC 311, Spring 2012, Project 8
 *
 * Author:   Brian Golden,    Aaron Goodermuth
 * Email:    bwg5079@psu.edu, ajg5353@psu.edu
 *
 * makefile target list header
 * version 1, 24 April 2012
 */

#ifndef CMPSC311_TARGETS_H
#define CMPSC311_TARGETS_H

#include "sources.h"
#include "recipes.h"

//------------------------------------------------------------------------------

// singly-linked list of targets

struct target
{
  struct target *next;	               // NULL indicates end-of-list
  char *line;		                   // from strdup()
  char *filename;
  int linenum;
  struct list_sources *sources;
  struct list_recipes *recipes;
};

struct list_targets
{
  struct target *head;	               // NULL indicates empty list
  struct target *tail;
  char *name;		                   // from strdup()
};

//------------------------------------------------------------------------------

void list_targets_init(struct list_targets *list, const char *listname);
struct list_targets *list_targets_allocate(const char *listname);
void list_targets_deallocate(struct list_targets *list);
void list_targets_print(struct list_targets *list);
void list_targets_append(struct list_targets *list, const char *line, const char *filename,
                         int linenum, struct list_sources *sources, struct list_recipes *recipes);
void list_targets_iterate(struct list_targets *list, void *func(const char *));

//------------------------------------------------------------------------------

/* Notes
 *
 * If list_targets_allocate() or one of the list_targets_append() functions
 *   can't allocate enough memory, the program ends with an error message.
 *
 * Do not apply list_targets_deallocate() to something that did not come from
 *   list_targets_allocate().
 *
 * list_targets_append_if_new() works as follows:
 *   if (name is on the list already) { return 1 }
 *   else { put name on the list and return 0 }
 *
 */

//------------------------------------------------------------------------------

#endif
