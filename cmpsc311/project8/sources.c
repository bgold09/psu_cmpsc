/* CMPSC 311, Spring 2012, Project 8
 *
 * Author:   Brian Golden,    Aaron Goodermuth
 * Email:    bwg5079@psu.edu, ajg5353@psu.edu
 *
 * makefile source list implementation
 * version 1, 24 April 2012
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <errno.h>
#include <string.h>

#include "cmpsc311.h"
#include "sources.h"

//------------------------------------------------------------------------------

void list_sources_init(struct list_sources *list)
{
  verify(list != NULL, "null arg list");

  list->head = list->tail = NULL;
}

//------------------------------------------------------------------------------

struct list_sources *list_sources_allocate(void)
{

  struct list_sources *list = Malloc(sizeof(struct list_sources));

  list->head = list->tail = NULL;

  return list;
}

//------------------------------------------------------------------------------

void list_sources_deallocate(struct list_sources *list)
{
  verify(list != NULL, "null arg list");

  struct source *prev = NULL;
  for (struct source *p = list->head; p != NULL; p = p->next)
    {
      free(prev);	// free(NULL) is harmless
      prev = p;
    }
  free(prev);		// prev == list->tail
  free(list);
}


//------------------------------------------------------------------------------

void list_sources_append(struct list_sources *list, const char *name)
{
  verify(list != NULL, "null arg list");
  verify(name != NULL, "null arg name");

  struct source *fn = Malloc(sizeof(struct source));

  fn->next = NULL;
  fn->name = Strdup(name);

  if (list->head == NULL)	// empty list, list->tail is also NULL
    {
      list->head = list->tail = fn;
    }
  else
    {
      list->tail->next = fn;
      list->tail = fn;
    }
}

//------------------------------------------------------------------------------

void list_sources_iterate(struct list_sources *list, void *func(const char *))
{
  verify(list != NULL, "null arg list");
  verify(func != NULL, "null arg func");

  for (struct source *p = list->head; p != NULL; p = p->next)
    {
      printf("calling func on %s\n", p->name);
    }
}

//------------------------------------------------------------------------------

void list_sources_print(struct list_sources *list)
{
  verify(list != NULL, "null arg list");

  if (list->head == NULL)
    {
       printf("  <empty>\n");
    }
  else
    {
      for (struct source *p = list->head; p != NULL; p = p->next)
        {
          printf("  '%s'\n", p->name);
        }
    }
}
