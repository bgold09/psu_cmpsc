/* CMPSC 311, Spring 2012, Project 4
 *
 * Author:   <your name>
 * Email:    <your preferred email address>
 *
... <additional comment text>
 */

/* base library for CMPSC 311 projects
 * version of 14 Feb. 2012
 */

#ifndef CMPSC311_NAMES_H
#define CMPSC311_NAMES_H

//------------------------------------------------------------------------------

// singly-linked list of names

struct name
{
  struct name *next;	// NULL indicates end-of-list
  char *name;		// from strdup()
};

struct list_names
{
  struct name *head;	// NULL indicates empty list
  struct name *tail;
  char *name;		// from strdup()
};

//------------------------------------------------------------------------------

void list_names_init(struct list_names *list, const char *listname);
struct list_names *list_names_allocate(const char *listname);
void list_names_deallocate(struct list_names *list);
void list_names_print(struct list_names *list);
void list_names_append(struct list_names *list, const char *name);
 int list_names_append_if_new(struct list_names *list, const char *name);
void list_names_append_from_file(struct list_names *list, const char *filename);
void list_names_iterate(struct list_names *list, void *func(const char *));

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

