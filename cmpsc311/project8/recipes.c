/* CMPSC 311, Spring 2012, Project 8
 *
 * Author:   Brian Golden,    Aaron Goodermuth
 * Email:    bwg5079@psu.edu, ajg5353@psu.edu
 *
 * makefile recipe list implementation
 * version 1, 24 April 2012
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <errno.h>
#include <string.h>

#include "cmpsc311.h"
#include "recipes.h"

//------------------------------------------------------------------------------

void list_recipes_init(struct list_recipes *list)
{
  verify(list != NULL, "null arg list");

  list->head = list->tail = NULL;
}

//------------------------------------------------------------------------------

struct list_recipes *list_recipes_allocate()
{

  struct list_recipes *list = Malloc(sizeof(struct list_recipes));

  list->head = list->tail = NULL;

  return list;
}

//------------------------------------------------------------------------------

void list_recipes_deallocate(struct list_recipes *list)
{
  verify(list != NULL, "null arg list");

  struct recipe *prev = NULL;
  for (struct recipe *p = list->head; p != NULL; p = p->next)
    {
      free(prev);	// free(NULL) is harmless
      prev = p;
    }
  free(prev);		// prev == list->tail
  free(list);
}

//------------------------------------------------------------------------------

void list_recipes_print(struct list_recipes *list)
{
  verify(list != NULL, "null arg list");


  if (list->head == NULL)
    {
      printf("  <empty>\n");
    }
  else
    {
      for (struct recipe *p = list->head; p != NULL; p = p->next)
	{
	  printf("  %s\n", p->name);
	}
    }
}

//------------------------------------------------------------------------------

void list_recipes_append(struct list_recipes *list, const char *name)
{
  verify(list != NULL, "null arg list");
  verify(name != NULL, "null arg name");
  verify(name[0] != '\0', "empty arg name");

  struct recipe *fn = Malloc(sizeof(struct recipe));

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

void list_recipes_iterate(struct list_recipes *list, void *func(const char *))
{
  verify(list != NULL, "null arg list");
  verify(func != NULL, "null arg func");

  for (struct recipe *p = list->head; p != NULL; p = p->next)
    {
      printf("calling func on %s\n", p->name);
    }
}

//------------------------------------------------------------------------------

