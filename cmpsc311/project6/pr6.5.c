/* CMPSC 311 Project 6 starter kit, version 5 
 *
 * Author:   Brian Golden
 * Email:    bwg5079@psu.edu
 *
 * Project 6, version 5
 * 23 March 2012
 */

/*----------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#include "pr6_ctime.h"
#include "pr6_signal.h"
#include "pr6_wait.h"
#include "pr6_table.h"

/* for use with getopt(3) */
extern char *optarg;
extern int optind;
extern int optopt;
extern int opterr;

/* debug option, with -v */
int pr6_debug = 0;

/* upper limit on -c option (this is a temporary version) */
#define MAX_CHILDREN 8

static pid_t Fork(void);
/*----------------------------------------------------------------------------*/

static void usage(char *prog, int status)
{
  if (status == EXIT_SUCCESS)
    {
      printf("Usage: %s [-h] [-v] [-a n] [-b n] [-c n] [-f n] [-s n] [-t n] [-x n]\n",
        prog);
      printf("    -h      help\n");
      printf("    -v      verbose mode\n");
      printf("    -a n    child alarm time interval, default 0\n");
      printf("    -b n    parent alarm time interval, default 0\n");
      printf("    -c n    fork() n child processes, default 0, max %d\n", MAX_CHILDREN);
      printf("    -f n    fflush() before fork(), 0 = no, 1 = yes, default 1\n");
      printf("    -s n    child sleep time, default 0\n");
      printf("    -t n    parent sleep time, default 0\n");
      printf("    -x n    child exit status n, default 0\n");
    }
  else
    {
      fprintf(stderr, "%s: Try '%s -h' for usage information.\n", prog, prog);
    }

  exit(status);
}

/* conditional is expected to be true
 * return termination indicator (1) if there is a fatal problem, otherwise return 0
 */
static int check_arg(char *prog, char *option, int val, int fatal, int conditional)
{
  if (!conditional)
    {
      if (fatal)
        {
          fprintf(stderr, "%s: %s %d out of range\n", prog, option, val);
          return 1;
        }
      else
        {
          fprintf(stderr, "%s: %s %d out of range (ignored)\n", prog, option, val);
          return 0;
        }
    }
  return 0;
}

/*----------------------------------------------------------------------------*/

static void Sleep(int alarm_time_interval, int total_sleep_time)
{
  int remaining_sleep_time = total_sleep_time;

  while (remaining_sleep_time > 0)
    {
      if (alarm_time_interval > 0)
        { alarm(alarm_time_interval); }
      remaining_sleep_time = sleep(remaining_sleep_time);
    }
}

/*----------------------------------------------------------------------------*/

static void SIGALRM_handler(int sig)
{
  if (sig == SIGALRM)
    { print_msg("alarm signal received"); }
  else
    { print_msg_1("serious error: SIGALRM_handler, received signal", sig); }
}

static void SIGINT_handler(int sig)
{
  if (sig == SIGINT)
    { print_msg("interrupt signal received - ignored"); }
  else
    { print_msg_1("serious error: SIGINT_handler, received signal", sig); }
}

static void SIGCHLD_handler(int sig)
{
  if (sig == SIGCHLD)
    { print_msg("child signal received - ignored"); }
  else
    { print_msg_1("serious error: SIGCHLD_handler, received signal", sig); }
}

/* replace the signal handlers for SIGALRM, SIGINT and SIGCHLD */
static void signal_setup(void)
{
  install_signal_handler(SIGALRM, SIGALRM_handler);
  install_signal_handler(SIGINT, SIGINT_handler);
  install_signal_handler(SIGCHLD, SIGCHLD_handler);
}

/*----------------------------------------------------------------------------*/

int main(int argc, char *argv[])
{
  /* for use with getopt(3) */
  int ch;

  /* option flags and option-arguments set from the command line */
  int verbose = 0;
  int child_alarm_time = 0;
  int parent_alarm_time = 0;
  int child_processes = 0;
  int flush_before_fork = 1;
  int child_sleep_time = 0;
  int parent_sleep_time = 0;
  int child_exit_status = EXIT_SUCCESS;

  printf("CMPSC 311 Project 6, version 4\n");

  while ((ch = getopt(argc, argv, ":hva:b:c:f:s:t:x:")) != -1)
    {
      switch (ch) {
        case 'h':
          usage(argv[0], EXIT_SUCCESS);
          break;
        case 'v':
          verbose = 1;
          pr6_debug = 1;
          break;
        case 'a':
          child_alarm_time = atoi(optarg);
          break;
        case 'b':
          parent_alarm_time = atoi(optarg);
          break;
        case 'c':
          child_processes = atoi(optarg);
          break;
        case 'f':
          flush_before_fork = atoi(optarg);
          break;
        case 's':
          child_sleep_time = atoi(optarg);
          break;
        case 't':
          parent_sleep_time = atoi(optarg);
          break;
        case 'x':
          child_exit_status = atoi(optarg);
          break;
        case '?':
          printf("%s: invalid option '%c'\n", argv[0], optopt);
          usage(argv[0], EXIT_FAILURE);
          break;
        case ':':
          printf("%s: invalid option '%c' (missing argument)\n", argv[0], optopt);
          usage(argv[0], EXIT_FAILURE);
          break;
        default:
          usage(argv[0], EXIT_FAILURE);
          break;
      }
    }

  if (verbose == 1 || child_alarm_time != 0)
    { printf("  child_alarm_time = %d\n", child_alarm_time); }
  if (verbose == 1 || parent_alarm_time != 0)
    { printf("  parent_alarm_time = %d\n", parent_alarm_time); }
  if (verbose == 1 || child_processes != 0)
    { printf("  child_processes = %d\n", child_processes); }
  if (verbose == 1 || flush_before_fork != 1)
    { printf("  flush_before_fork = %d\n", flush_before_fork); }
  if (verbose == 1 || child_sleep_time != 0)
    { printf("  child_sleep_time = %d\n", child_sleep_time); }
  if (verbose == 1 || parent_sleep_time != 0)
    { printf("  parent_sleep_time = %d\n", parent_sleep_time); }
  if (verbose == 1 || child_exit_status != 0)
    { printf("  child_exit_status = %d\n", child_exit_status); }

  int fatal = 1;
  int terminate = 0;

  terminate += check_arg(argv[0], "-a", child_alarm_time,   fatal,
                         0 <= child_alarm_time);
  terminate += check_arg(argv[0], "-b", parent_alarm_time,  fatal,
                         0 <= parent_alarm_time);
  terminate += check_arg(argv[0], "-c", child_processes,    fatal,
                         0 <= child_processes && child_processes <= MAX_CHILDREN);
  terminate += check_arg(argv[0], "-s", child_sleep_time,   fatal,
                         0 <= child_sleep_time);
  terminate += check_arg(argv[0], "-t", parent_sleep_time,  fatal,
                         0 <= parent_sleep_time);
  terminate += check_arg(argv[0], "-x", child_exit_status, !fatal,
                         0 <= child_exit_status && child_exit_status < 256);

  if (terminate > 0)
    {
      fprintf(stderr, "%s: giving up\n", argv[0]);
      exit(EXIT_FAILURE);
    }

  signal_setup();

  init_process_table();

  int i;
  for (i = 0; i < child_processes; i++)
  {
    pid_t child_pid;

    if (flush_before_fork)
      { fflush(stdout); fflush(stderr); }

    child_pid = fork();

    if (child_pid == (pid_t) (-1))                    // this is the parent process, fork failed
    {
      print_msg_error("fork()", strerror(errno));

    }
    else if (child_pid == 0)                          // this is the child process, fork suceeded
    {
      print_msg_1("here is child", i);

      Sleep(child_alarm_time, child_sleep_time);

      exit(child_exit_status);
    } 
    else                                              // this is the parent process, fork succeeded
    {                                                 // child_pid is the pid of the new child process
      insert_process_table(child_pid);
    }
  }
  
  print_msg("here is the parent, all children created");

  Sleep(parent_alarm_time, parent_sleep_time);

  pid_t some_child_pid;
  int child_status;

  while(wait_any_child(&some_child_pid, &child_status) == 1)     // wait for any child process to finish
  {
    print_msg_2("child finished", (int) some_child_pid, child_status);   // print pid and exit status of exited child 
    update_process_table(some_child_pid, child_status);
    remove_process_table(some_child_pid);
  }

  return EXIT_SUCCESS;
}

/*----------------------------------------------------------------------------*/
