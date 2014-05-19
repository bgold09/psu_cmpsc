#ifndef PR6_TABLE_H
#define PR6_TABLE_H

/* CMPSC 311 Project 7 toolkit 
 *
 * Authors:  Aaron Goodermuth, Brian Golden
 * Emails:   ajg5353@psu.edu,  bwg5079@psu.edu
 *
 * 10 April 2012
 */

/*----------------------------------------------------------------------------*/


#include <string.h>
#include <sys/types.h>
#include "pr7_lib.h"

extern int pr7_debug;


struct process
{
  pid_t pid;                              // process ID, supplied from fork() 
  int state;                              // process state
  int exit_status;                        // supplied from wait() if process finished
  char* prog_name;                        // process name

  struct process *next;
};

struct process_table
{
  struct process *head;
  struct process *tail;
};

void init_process_table(struct process_table *table);
struct process_table *process_table_allocate();
void process_table_deallocate(struct process_table *table);

void print_process_table(struct process_table *table, const char * const caller);

void insert_process_table(struct process_table *table, pid_t pid, char* prog_name);   /* new process     */
void update_process_table(struct process_table *table, pid_t pid, int status);        /* exit status from completed process */
void remove_process_table(struct process_table *table, pid_t pid);                    /* defunct process */
int has_background_processes(struct process_table *table);

/*----------------------------------------------------------------------------*/

#endif
