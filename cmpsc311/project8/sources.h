/* CMPSC 311, Spring 2012, Project 8
 *
 * Author:   Brian Golden,    Aaron Goodermuth
 * Email:    bwg5079@psu.edu, ajg5353@psu.edu
 *
 * makefile source list header
 * version 1, 24 April 2012
 */

#ifndef CMPSC311_SOURCES_H
#define CMPSC311_SOURCES_H

//------------------------------------------------------------------------------

// singly-linked list of sources

struct source
{
  struct source *next;	     // NULL indicates end-of-list
  char *name;		         // from strdup()
};

struct list_sources
{
  struct source *head;	     // NULL indicates empty list
  struct source *tail;
};

//------------------------------------------------------------------------------

void list_sources_init(struct list_sources *list);
struct list_sources *list_sources_allocate(void);
void list_sources_deallocate(struct list_sources *list);
void list_sources_append(struct list_sources *list, const char *name);
void list_sources_iterate(struct list_sources *list, void *func(const char *));
void list_sources_print(struct list_sources *list);

//------------------------------------------------------------------------------

/* Notes
 *
 * If list_names_allocate() or one of the list_names_append() functions
 *   can't allocate enough memory, the program ends with an error message.
 *
 * Do not apply list_names_deallocate() to something that did not come from
 *   list_names_allocate().
 *
 * list_names_append_if_new() works as follows:
 *   if (name is on the list already) { return 1 }
 *   else { put name on the list and return 0 }
 *
 */

//------------------------------------------------------------------------------

#endif

