/* CMPSC 311 Project 7 toolkit 
 *
 * Authors:  Aaron Goodermuth, Brian Golden
 * Emails:   ajg5353@psu.edu,  bwg5079@psu.edu 
 *
 * 10 April 2012
 */

/*----------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

#include "table.h"

#define STATE_NONE       0
#define STATE_RUNNING    1
#define STATE_TERMINATED 2

static char *state[] = { "none", "running", "terminated"};

/*----------------------------------------------------------------------------*/

void init_process_table(struct process_table *table)
{
  verify(table != NULL, "null arg table");

  table->head = table->tail = NULL;
}

struct process_table *process_table_allocate()
{
  struct process_table *table = Malloc(sizeof(struct process_table));

  table->head = table->tail = NULL;

  return table;
}

void process_table_deallocate(struct process_table *table)
{
  verify(table != NULL, "null arg table");

  struct process *prev = NULL;
  struct process *p;
  for (p = table->head; p != NULL; p = p->next)
  {
    free(prev->prog_name);
    free(prev);
    prev = p;
  }

  free(prev);
  free(table);
}

void print_process_table(struct process_table *table, const char * const caller)
{
  printf("  process table, printed by %s\n", caller);
  printf("       pid         state        status       program\n");

  struct process *p;
  for (p = table->head; p != NULL; p = p->next)
  {
      printf("    %6d    %10s    0x%08x       %s\n",
        (int)p->pid,
	state[p->state],
	p->exit_status,
	p->prog_name);
  }
}

void insert_process_table(struct process_table *table, pid_t pid, char *prog_name)
{
  verify(table != NULL, "null arg table");
  verify(pid > 0, "invalid process ID");
  verify(prog_name != NULL, "null arg prog_name");
  verify(prog_name[0] != '\0', "empty arg prog_name");

  struct process *proc = Malloc(sizeof(struct process));

  proc->pid = pid;
  proc->state = STATE_RUNNING;
  proc->exit_status = 0;
  proc->prog_name = strdup(prog_name);
  proc->next = NULL;

  if (table->head == NULL)
  {
    table->head = table->tail = proc;
  }
  else
  {
    table->tail->next = proc;
    table->tail = proc;
  }

  if (pr7_debug) print_process_table(table, __func__);
}

void update_process_table(struct process_table *table, pid_t pid, int status)
  /* exit status from completed process */
{
  
  struct process *p;
  for (p = table->head; p != NULL; p = p->next)
  {
    if (p->pid == pid)
    {
      p->state = STATE_TERMINATED;
      p->exit_status = status;
      break;
    }
  }

  if (pr7_debug) print_process_table(table, __func__);
}

void remove_process_table(struct process_table *table, pid_t pid)
{
  struct process *currP, *prevP;

  prevP = NULL;

  for (currP = table->head; currP != NULL; prevP = currP, currP = currP->next) 
  {

    if (currP->pid == pid)  // found it
    {
      if (prevP == NULL) 
        { table->head = currP->next; } 
      else 
        { prevP->next = currP->next; }

      free(currP->prog_name);
      free(currP);
    }
  }

  if (pr7_debug) print_process_table(table, __func__);
}

int has_background_processes(struct process_table *table)
{

  struct process *p;
  for (p = table->head; p != NULL; p = p->next)
  {
    if (p->state == STATE_RUNNING)
      return 1;
  }

  return 0;
}

/*----------------------------------------------------------------------------*/

