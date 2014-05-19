/* CMPSC 311, Spring 2012, Project 8
 *
 * Author:   Brian Golden,    Aaron Goodermuth
 * Email:    bwg5079@psu.edu, ajg5353@psu.edu
 *
 * makefile target list implementation
 * version 1, 24 April 2012
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <errno.h>
#include <string.h>

#include "cmpsc311.h"
#include "targets.h"
#include "recipes.h"
#include "sources.h"

//------------------------------------------------------------------------------

void list_targets_init(struct list_targets *list, const char *listname)
{
  verify(list != NULL, "null arg list");
  verify(listname != NULL, "null arg listname");
  verify(listname[0] != '\0', "empty arg listname");

  list->name = Strdup(listname);
  list->head = list->tail = NULL;
}

//------------------------------------------------------------------------------

struct list_targets *list_targets_allocate(const char *listname)
{
  verify(listname != NULL, "null arg listname");
  verify(listname[0] != '\0', "empty arg listname");

  struct list_targets *list = Malloc(sizeof(struct list_targets));

  list->name = Strdup(listname);
  list->head = list->tail = NULL;

  return list;
}

//------------------------------------------------------------------------------

void list_targets_deallocate(struct list_targets *list)
{
  verify(list != NULL, "null arg list");

  free(list->name);
  struct target *prev = NULL;
  for (struct target *p = list->head; p != NULL; p = p->next)
    {
      free(prev);	// free(NULL) is harmless
      free(p->line);
      free(p->filename);
      list_sources_deallocate(p->sources);
      list_recipes_deallocate(p->recipes);
      prev = p;
    }
  free(prev);		// prev == list->tail
  free(list);
}

//------------------------------------------------------------------------------

void list_targets_print(struct list_targets *list)
{
  verify(list != NULL, "null arg list");

  printf("list of targets: %s\n", safe_string(list->name));

  if (list->head == NULL)
    {
      printf("  <empty>\n");
    }
  else
    {
      for (struct target *p = list->head; p != NULL; p = p->next)
	{
	  printf("  %s\n", p->line);
          list_recipes_print(p->recipes);
	}
    }
}

//------------------------------------------------------------------------------

void list_targets_append(struct list_targets *list, const char *line, const char *filename, 
                         int linenum, struct list_sources *sources, struct list_recipes *recipes)
{
  verify(list != NULL, "null arg list");
  verify(line != NULL, "null arg line");
  verify(line[0] != '\0', "empty arg line");
  verify(filename != NULL, "null arg filename");
  verify(filename[0] != '\0', "empty arg filename");
  verify(linenum > 0, "invalid arg linenum");
  verify(sources != NULL, "null arg sources");

  struct target *fn = Malloc(sizeof(struct target));

  fn->next = NULL;
  fn->line = Strdup(line);
  fn->filename = Strdup(filename);
  fn->linenum = linenum;
  fn->sources = sources;
  fn->recipes = recipes;

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

void list_targets_iterate(struct list_targets *list, void *func(const char *))
{
  verify(list != NULL, "null arg list");
  verify(func != NULL, "null arg func");

  for (struct target *p = list->head; p != NULL; p = p->next)
    {
      printf("calling func on %s\n", p->line);
    }
}

//------------------------------------------------------------------------------
