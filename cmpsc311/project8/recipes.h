/* CMPSC 311, Spring 2012, Project 8
 *
 * Author:   Brian Golden,    Aaron Goodermuth
 * Email:    bwg5079@psu.edu, ajg5353@psu.edu
 *
 * makefile recipe list header
 * version 1, 24 April 2012
 */

#ifndef CMPSC311_RECIPES_H
#define CMPSC311_RECIPES_H

//------------------------------------------------------------------------------

// singly-linked list of recipes

struct recipe
{
  struct recipe *next;	     // NULL indicates end-of-list
  char *name;		         // from strdup()
};

struct list_recipes
{
  struct recipe *head;	     // NULL indicates empty list
  struct recipe *tail;
};

//------------------------------------------------------------------------------

void list_recipes_init(struct list_recipes *list);
struct list_recipes *list_recipes_allocate();
void list_recipes_deallocate(struct list_recipes *list);
void list_recipes_print(struct list_recipes *list);
void list_recipes_append(struct list_recipes *list, const char *name);
void list_recipes_iterate(struct list_recipes *list, void *func(const char *));

//------------------------------------------------------------------------------

/* Notes
 *
 * If list_recipes_allocate() or one of the list_recipes_append() functions
 *   can't allocate enough memory, the program ends with an error message.
 *
 * Do not apply list_recipes_deallocate() to something that did not come from
 *   list_recipes_allocate().
 *
 * list_recipes_append_if_new() works as follows:
 *   if (name is on the list already) { return 1 }
 *   else { put name on the list and return 0 }
 *
 */

//------------------------------------------------------------------------------

#endif

