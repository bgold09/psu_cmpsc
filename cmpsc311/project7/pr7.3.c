/* CMPSC 311, Spring 2012, Project 7
 *
 * Program:      Command Shell
 * Authors:      Aaron Goodermuth, Brian Golden
 * Emails:       ajg5353@psu.edu,  bwg5079@psu.edu
 * Description:    Emulates the behavior of a shell script
 *
 * Modified From: CMPSC 311 Project 7, version 2 
 * This version is derived from the shellex.c program in CS:APP Sec. 8.4.
 *
 * version 3, 10 April 2012
 *
 * Usage:
 *   c99 -v -o pr7 pr7.3.c table.c pr7_signal.c pr7_lib.c                            [Sun compiler]
 *   gcc -std=c99 -Wall -Wextra -o pr7 pr7.3.c table.c pr7_signal.c pr7_lib.c        [GNU compiler]
 *
 *   pr7
 *   pr7%      [type a command and then return]
 *   pr7% exit
 *
 *  See also: the accompanying Makefile and README
 */

/*----------------------------------------------------------------------------*/

/* Include Files */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/wait.h>

#include "pr7.h"
#include "pr7_signal.h"
#include "table.h"

/* Global Constants */
#define MAXLINE                    128
#define MAXARGS                    128
#define USE_EXECVE                 0
#define USE_EXECLP                 1
#define USE_EXECVP                 2
#define DEFAULT_EXEC               USE_EXECVP
#define NUM_MICROSECOND_SLEEP      10000
#define DEFAULT_STARTUP_FILENAME   "pr7.init"

/* Function Headers */
static void usage(char *prog, int status);      /* print the help information                         */
static int parse(char *buf, char *argv[]);      /* build the argv array                               */
static int eval_line(char *cmdline);            /* evaluate a command line                            */
static int eval_file(char *filename);           /* execute commands in a shell script                 */
static int builtin(char *argv[]);               /* if builtin command, run it                         */
static void SIGINT_handler(int sig);            /* handle an interrupt signal                         */
static int cleanup_terminated_children(void);   /* update process table when process end              */

/* Global Variables */
int verbose;                                    /* print extra information if true                    */
static int echo_option;                         /* echo commands entered if true                      */
static int exec_option = DEFAULT_EXEC;          /* option for which exec runs (execlp, execvp, execve)*/
static int command_number = 0;
static struct process_table *table;             /* the table of currently running process             */
int pr7_debug = 0;                              /* print debug information if true                    */
static int background;                          /* run job in foreground or background                */
extern char **environ;                          /* the array of environment variables                 */
char* prog;                                     /* the program name                                   */

/*----------------------------------------------------------------------------*/

int main(int argc, char *argv[])
{
  /* Local Variables */
  int ret = EXIT_SUCCESS;                     // return status of the program
  int interactive = 0;                        // get commands from stdin (and print prompt) 
  int ch;                                     // the temporary variable for getting command line options 
  char cmdline[MAXLINE];                      // command line character array 
  char startup_filename[MAXLINE] = DEFAULT_STARTUP_FILENAME;     // the startup filename

  /* Init globals */
  prog            = argv[0];  
  echo_option     = 0;
  table = process_table_allocate();
  
  if (install_signal_handler(SIGINT, SIGINT_handler) == -1)
    { printf("install_signal_handler() error: %s\n", strerror(errno)); }

  /* get command line options */
  while ((ch = getopt(argc, argv, ":hvieds:x:")) != -1)
  {
    switch (ch) 
    {
      case 'h':                                            // print help information  
        usage(argv[0], USAGE_STATUS_HELP);
        exit(EXIT_SUCCESS);
        break;
      case 'v':                                            // turn verbose mode on
        verbose = 1;
        break;
      case 'd':                                            // turn debug mode on
        pr7_debug = 1;
        verbose   = 1;
    break;
      case 'i':                                            // turn interactive mode on 
        interactive = 1;
        break;
      case 'e':                                            // turn echo mode on                  
        echo_option = 1;
        break;
      case 's':                                            // read from specified startup file 
        (void) strcpy(startup_filename, optarg);
        break;
      case 'x':
        if     (strcmp(optarg, "execlp") == 0)  { exec_option = USE_EXECLP; }
        else if (strcmp(optarg, "execvp") == 0) { exec_option = USE_EXECVP; }
        else if (strcmp(optarg, "execve") == 0) { exec_option = USE_EXECVE; }
        else 
        {
          fprintf(stderr, "%s: Invalid option: '%c' (missing option)\n", prog, optopt); 
          exit(EXIT_FAILURE);
        }
        break;
      case '?':                                            // invalid option; print help info
        printf("%s: invalid option '%c'\n", argv[0], optopt);
        usage(argv[0], USAGE_STATUS_HELP);
        exit(EXIT_FAILURE);
        break;
      case ':':                                            // missing argument to an option
        fprintf(stderr, "%s: Invalid option: '%c' (missing argument)\n", prog, optopt);
        exit(EXIT_FAILURE);
        break;
      default:                                             // default: print help info 
        printf("we got here\n");
        break;
    }
  }
  
  if(verbose || pr7_debug)                                 // Print options list
  {
    printf("\nOptions:\n");
    printf("Init file set to:        %s\n", startup_filename);
    printf("Interactive Mode:        %s\n",  interactive ? "on" : "off"); //interactive
    printf("Echo Mode:               %s\n",  echo_option ? "on" : "off"); // echo
    printf("Verbose Mode:            %s\n",  verbose     ? "on" : "off"); // echo
    printf("\n");
  }
  
  /* Run commands in the init file */
  (void) eval_file(startup_filename);
  command_number = 0;
  int arg_num = 1;                                         // iterator for command line options 
  while (1)
  {
    if(interactive)                                        // issue prompt and read command line
    {  
      (void) cleanup_terminated_children();
      background = 1;
      printf("%s %d%% ", argv[0], command_number);         // print prompt
      command_number++;
      fgets(cmdline, MAXLINE, stdin);                      // cmdline includes trailing newline
      if (feof(stdin))                                     // end of file
        { break; }
      ret = eval_line(cmdline);
    }
    else                                                   // look for file and then quit 
    {
      if(arg_num >= argc) {break;}
      if(argv[arg_num][0] != '-' || (argv[arg_num][1] == '\0' && argv[arg_num][0] == '-'))
      {
        ret = eval_file(argv[arg_num]);
        break;                     // only run first fi (in accordance with typical shell functionality)
      }
      else
      {
        arg_num++;        
      }
      arg_num++;
    }
  }
  
  process_table_deallocate(table);                         // free process table memory

  return ret;
}

/*----------------------------------------------------------------------------*/

/* 
 * Print help information
 */ 
static void usage(char *prog, int status)
{
  if (status == USAGE_STATUS_HELP)                      // print shell help
  {
    printf("Usage:  %s [-h] [-v] [-i] [-e] [-s f] [-x c] [file]\n", prog);
    printf("  -h     help\n");
    printf("  -v     verbose mode\n");
    printf("  -i     interactive mode\n");
    printf("  -e     echo commands before execution\n");
    printf("  -s f   use startup file f, default pr7.init\n");
    printf("  -x c   use exec function c, default execvp()\n");
  }
  else if (status == USAGE_STATUS_CMD)                  // print inner shell usage 
  {
    printf("Shell commands:\n");
    printf("  help\n");
    printf("  exit\n");
    printf("  echo\n");
    printf("  dir\n");
    printf("  cdir [directory]\n");
    printf("  penv\n");
    printf("  penv variable_name\n");
    printf("  senv variable_name value\n");
    printf("  unsenv variable_name\n");
    printf("  pjobs\n");
    printf("  limits\n");
    printf("  set debug    [on|off]\n");
    printf("  set exec     [lp|vp|ve]\n");
    printf("  set verbose  [on|off]\n");
  }
  else                                                  // unsure of request, direct to help information
  {
    fprintf(stderr, "%s: Try '%s -h' for usage information.\n", prog, prog);
  }
}

/* 
 * parse the command line and build the argv array
 *
 * Compare to parseline() in CS:APP Fig. 8.24.
 */
static int parse(char *buf, char *argv[])
{
  char *delim;                                             // points to first whitespace delimiter 
  int argc = 0;                                            // number of args 
  int bg;                                                  // background job? 

  char whsp[] = " \t\n\v\f\r";                             // whitespace characters                   

  /* Note - the trailing '\n' in buf is whitespace, and we need it as a delimiter. */

  while (1)                                                // build the argv list                     
  {
    buf += strspn(buf, whsp);                              // skip leading whitespace                 
    char *p_hash = strchr(buf, '#');
    if(p_hash != NULL) 
      {*p_hash = '\0';}
    delim = strpbrk(buf, whsp);                            // next whitespace char or NULL            
    if (delim == NULL)                                     // end of line                             
      { break; }
    if(strchr(buf, ';') != NULL)
    {  
      int len = 0;
      if ((len = strlen(buf)) != 0)
      {
        char first[len+2];
        char rest[len+2];
        int i = 0;
        while(buf[i] != ';')
        {
          first[i] = buf[i];
          i++;
        }
        first[i] = '\n';
        first[i+1] = '\0';
        i++;
        int j=0;
        while(buf[i+j] != '\n' && buf[i+j] != '\0')
        {
          rest[j] = buf[i+j];
          j++;
        }
        rest[j] = '\n';
        rest[j+1] = '\0';
        if (i != 1)
        {
          (void) eval_line(first);
          command_number++;
        }

        (void) eval_line(rest);
        break;
      }
      else
      {
        fprintf(stderr, "Buffer 'buf' is empty\n");
        exit(EXIT_FAILURE);
      }
      
    }
    else
    {
      argv[argc++] = buf;                                  // start argv[i]         
      *delim = '\0';                                       // terminate argv[i]     
      buf = delim + 1;                                     // start argv[i+1]?      
    }
  }
  argv[argc] = NULL;

  if (argc == 0)                                           // blank line 
    { return 0; }

  /* should the job run in the background? */
  if ((bg = (strcmp(argv[argc-1], "&") == 0)))
    { argv[--argc] = NULL; }

  return bg;
}

/* 
 * evaluate a command line
 * Compare to eval() in CS:APP Fig. 8.23.
 */
static int eval_line(char *cmdline)
{
  char *argv[MAXARGS];                                     // argv for execve()                                 
  char buf[MAXLINE];                                       // holds modified command line                       
  pid_t pid;                                               // process id                                        
  int ret = EXIT_SUCCESS;                                  // return value                                      

  (void) strcpy(buf, cmdline);                                    // buf[] will be modified by parse()                 
  background = parse(buf, argv);                           // build the argv array                              

  if (argv[0] == NULL)                                     // ignore empty lines                                
  { 
    command_number--;
    return ret; 
  }

  if(echo_option)
    { printf(cmdline);  }

  if (builtin(argv) == 1)                    // try running the command through the built in commands 
    { return ret; }                          // if successful, the work is done                        

  if ((pid = fork()) == 0)                   // child runs user job                                   
  {  
    if(exec_option == USE_EXECLP)
    {
      //TODO figure out how to pass bunch of arguments individually rather than in array
      //for now:
      printf("We have no idea how to use execlp right now, please excuse our mess\n");
      _exit(EXIT_FAILURE);
      /*if (execlp(argv[0], argv) == -1)                   // call execlp to run child job
      {  // serious error in here
        printf("%s: failed: %s\n", argv[0], strerror(errno));  
        _exit(EXIT_FAILURE);
      }*/
    }
    else if(exec_option == USE_EXECVP)
    {
      if (execvp(argv[0], argv) == -1)                     // call execvp to run child job
      {
        printf("%s: failed: %s\n", argv[0], strerror(errno));
        _exit(EXIT_FAILURE);
      }
    }
    else if(exec_option == USE_EXECVE)
    {
      if (execve(argv[0], argv, environ) == -1)            // call execve to run child job
      {
        printf("%s: failed: %s\n", argv[0], strerror(errno));  
        if (verbose)  { printf("Using execve: did you remember to type the full pathname?\n"); }
        _exit(EXIT_FAILURE);
      }
    }
    else
    {
      fprintf(stderr, "Uknown exec_option: %d\n", exec_option);
      exit(0);
    }

  }

  /* this is always the parent here */

  if (background)                                // parent does NOT wait for background job to terminate
  {
    if (setpgid(pid, 0) == -1)
    {
      printf("%s: setgpid() failed: %s\n", prog, strerror(errno));
      exit(EXIT_FAILURE);
    }    
    
    insert_process_table(table, pid, argv[0]);
    
    if(verbose || pr7_debug) { printf("background process %d: %s", (int) pid, cmdline); }
    
    /* In background processes, there is a 'race condition' (not the signal type) if the background 
       process has output. This code forces the shell to sleep for 1/100th of a second in order to try 
       and coax the output of the background process before continuing and outputing a new prompt. 
     */
    (void) usleep(NUM_MICROSECOND_SLEEP);

  }
  else                                                 // parent waits for foreground job to terminate            
  {  
    pid_t pid_number;                                  // the number of the returned pid              
    
    if ((pid_number = waitpid(pid, &ret, 0)) == -1)    // wait for foreground process                 
    {                                                  // error in here 
      printf("%s: failed: %s\n", argv[0], strerror(errno));
      exit(EXIT_FAILURE);
    }
    else 
    {                                                  // foreground process finished running         
      
      if(verbose || pr7_debug)                         // print exit status of process                
      {
        if(WIFEXITED(ret))
          printf("Process %d completed normally with status %d\n",(int)pid_number, ret);
        else
          printf("Process %d did not complete normally with status %d\n",(int)pid_number, ret);
      }
    }
  }

  return ret;
}

/* 
 * evaluate a shell script line by line
 */
static int eval_file(char *filename)
{
  char buffer[MAXLINE];                                    // line buffered contents of file          
  FILE *fp;                                                // file pointer                            
  int ret = 0;                                             // return value                            

  if(strcmp(filename, "-") == 0)
  {
    fp = stdin;
  }
  else
  {

    if((fp = fopen(filename, "r")) == NULL)                // attempt to open file                    
    {
      if(strcmp(filename, DEFAULT_STARTUP_FILENAME) != 0)  // if init file not found, do not complain 
      {
                                                           // unsuccessful in opening file 
        fprintf(stderr, " file '%s': %s \n", filename, strerror(errno)); 
        exit(EXIT_FAILURE);
      }
      
      return ret;
    }
    else
    {
      if(verbose || pr7_debug) {printf("%s: Reading file '%s'\n", prog, filename);}
    }
  }
    
  while(fgets(buffer, MAXLINE, fp) != NULL) // read file line by line until EOF or CTRL-D encountered
  {
    (void) cleanup_terminated_children();
    ret = eval_line(buffer);                               // execute the line
  }

  if(strcmp(filename, "-") != 0)
  {
    if(fclose(fp) == EOF) { printf("fclose() error: %s\n", strerror(errno)); };
  }
  
  return ret;
  
}

/* 
 * if first arg is a builtin command, run it and return true
 *
 * Compare to builtin_command() in CS:APP Fig. 8.23.
 */
static int builtin(char *argv[])
{
  if (strcmp(argv[0], "exit") == 0)                        // exit command     
  { 
    (void) cleanup_terminated_children();
    if (has_background_processes(table))
    {
      printf("pr7 has background processes\n");
      return 1;
    }
    else
    {
      exit(EXIT_SUCCESS);
    }
  }
  
  if (strcmp(argv[0], "echo") == 0)                        // echo command 
  { 
    int i = 1;
    while(argv[i] != NULL)                                 // while we have more command line arguments 
    {
      printf("%s ", argv[i]);                              // print all arguments 
      i++;
    }
    
    printf("\n");
    return 1;
  }
  
  if (strcmp(argv[0], "dir") == 0)                         // dir command 
  { 
    char* path = malloc(sizeof(char) * MAX_PATH);
    
                                                           // print current working directory 
    if(getcwd(path, MAX_PATH) != NULL)
      printf("%s\n", path);
    else
      fprintf(stderr, "%s: getcwd() failed, line:%d\n", argv[0],  __LINE__);
      
    free(path);
    return 1;      
  }

  if (strcmp(argv[0], "cdir") == 0)                        // cdir command 
  { 
    char* path = malloc(sizeof(char) * MAX_PATH);
    
                                                           // change environment variable and 
                                                           //  change working dir 
    if(argv[1] == NULL)                                    // no argument provided to cdir 
    {
      if(setenv("PWD", getenv("HOME"), 1) == -1)   
        {fprintf(stderr, "setenv() error: %s, line: %d\n", strerror(errno), __LINE__);}
      if(chdir(getenv("HOME")) == -1)      
        {fprintf(stderr, "chdir() error: %s, line: %d\n", strerror(errno), __LINE__);};
    }
    else if(realpath(argv[1], path) == NULL)               // argument to cdir does not exist 
    {
      printf("%s: failed: %s\n", argv[0], strerror(errno));
    }    
    else                                                   // cdir argument exists 
    {
      if(setenv("PWD", path, 1) == -1)    
        {fprintf(stderr, "setenv() error: %s, line: %d\n", strerror(errno), __LINE__);}
      if(chdir(path) == -1) 
        {fprintf(stderr, "chdir() error: %s, line: %d\n", strerror(errno), __LINE__);};      
    }
    
    free(path);
    return 1;
  }

  if (strcmp(argv[0], "penv") == 0)                        // penv command   
  {   
    if(argv[1] == NULL)                                    // print all envp vars 
    {  
      char** i = environ;                                  // copy environ to an iterator 
      while(*i!=NULL)                                      // print all environment variables 
      {
        printf("%s\n", *(i));
        i++;
      }
    }
    else                                                   // print only vars on command line 
    {  
      int i = 1;
      char* path = malloc(sizeof(char) * MAX_PATH);
      
      while(argv[i] != NULL)                               // while we have more command line arguments 
      { 
        if((path = getenv(argv[i])) == NULL)
          fprintf(stderr, "getenv() error: %s does not exist\n", argv[i]);
        else
          printf("%s = %s\n", argv[i], path);
        i++;
      }
      free(path);
    }
    return 1;    
  }
  
  if (strcmp(argv[0], "senv") == 0)                        // senv command   
  {   
    if(argv[1] == NULL)                                    // print all envp vars         
    {  
      char** i = environ;                                  // copy environ to an iterator 
      while(*i!=NULL)
      {
        printf("%s\n", *(i));
        i++;
      }
    }
    else                                                   // print only vars on command line   
    {  
      int i = 1;
      char* path = malloc(sizeof(char) * MAX_PATH);
      
      while(argv[i] != NULL)                               // while we have more command line arguments 
      { 
        if(argv[i+1] == NULL)                              // no value provided for environ variable 
        {
          if(setenv(argv[i], "test", 1) == -1)             // set to null 
            {fprintf(stderr, "setenv() error: %s\n", strerror(errno));}
          i++;
        }
        else                                               // value provided for env var
        {
          if(setenv(argv[i], argv[i+1], 1) == -1)          // set to val 
            {fprintf(stderr, "setenv() error: %s\n", strerror(errno));}
          i+=2;                                            // skip next command line argument, 
        }                                                  //  as it was used for the value of senv
      }
      free(path);
    }
    return 1;    
  }
  
  if (strcmp(argv[0], "unsenv") == 0)                      // unsenv command 
  {   
    if(argv[1] == NULL)                                    // no argument with unsenv command 
    {
      printf("%s: Too few arguments\n", argv[0]);
    }
    else                                                   // unset only vars on command line  
    {  
      int i = 1;
      while(argv[i] != NULL)
      {
        (void) unsetenv(argv[i]);
        i++;
      }
    }
    return 1;    
  }
  
  if (strcmp(argv[0], "help") == 0)                        // help command 
  { 
    usage(argv[0], USAGE_STATUS_CMD);                      // print usage info 
    return 1; 
  }
  
  if (strcmp(argv[0], "&") == 0)                           // ignore singleton & 
    { return 1; }
  
  if (strcmp(argv[0], "pjobs") == 0)                       // pjobs command 
  { 
    print_process_table(table, "pjobs");
    return 1; 
  }

  if(strcmp(argv[0], "limits") == 0)                       // limits command
  {
    printf("Limits of the shell:\n");
    printf("   Maximum Length of a System Line:          %d\n", MAX_LINE);
    printf("   Maximum Length of a Shell Line:           %d\n", MAXLINE);
    printf("   Maximum Length of a Path:                 %d\n", MAX_PATH);
    printf("   Maximum Number of Child Processes:        %d\n", MAX_CHILDREN);
    
    return 1;
  }
  
  if (strcmp(argv[0], "set") == 0)                         // set command
  {
    if (argv[1] == NULL || argv[2] == NULL)
    {
      printf("set: missing argument\n");
      return 1;
    }

    if (strcmp(argv[1], "debug") == 0)                     // set debug variable
    {
      if (strcmp(argv[2], "on") == 0)
        { pr7_debug = 1;}
      else if (strcmp(argv[2], "off") == 0)
        { pr7_debug = 0; }
      else
        { printf("set debug: invalid argument '%s'\n", argv[2]); }
    }
    else if (strcmp(argv[1], "verbose") == 0)              // set verbose variable
    {
      if (strcmp(argv[2], "on") == 0)
        { verbose = 1; }
      else if (strcmp(argv[2], "off") == 0)
        { verbose = 0; }
      else
        { printf("set verbose: invalid argument '%s'\n", argv[2]); }
    }
    else if (strcmp(argv[1], "exec") == 0)                 // set exec variable (exec function)
    {
      if (strcmp(argv[2], "vp") == 0)
        { exec_option = USE_EXECVP; }
      else if (strcmp(argv[2], "ve") == 0)
        { exec_option = USE_EXECVE; }
      else if (strcmp(argv[2], "lp") == 0)
        { exec_option = USE_EXECLP; }
      else
        { printf("set exec: invalid argument '%s'\n", argv[2]); } 
    } 
    else
    {
    printf("set: invalid argument '%s'\n", argv[1]);
    }

    return 1;
  }
  
  /* not a builtin command */
  return 0;                             
}

/* 
 * Handle what happens when an interrupt signal is recieved
 *
 *   -kill foreground process if the foreground process IS NOT the shell
 *   -do nothing if the foreground process IS the shell
 */ 
static void SIGINT_handler(int sig)
{
  if(SIGINT == sig)                                        // only handle interrupt signals  
  {  
    printf("\n");                                          // move to next output line 
    
    if(verbose || pr7_debug) 
      { printf("Ctrl-C SIGINT received and ignored in %s\n", prog); }

    if (background)
    {
      printf("%s %d%% ", prog, --command_number);
      command_number++;
    }
    fflush(stdout);
    return;
  }
  else                   // somehow the signal handler got passed a signal it is not supposed to handle;
  {                      //  this is a serious error, so print that fact and quit
    fprintf(stderr, "Serious error occurred. Unknown signal received\n");
    exit(EXIT_FAILURE);
  }
}

/* 
 * Find all the child processes that have terminated, without waiting.
 *
 * This code is adapted from the GNU info page on waitpid() and the Solaris
 * man page for waitpid(2).
 */ 
static int cleanup_terminated_children(void)
{
  pid_t pid;                                               // process id of child process      
  int status;                                              // return status of child process    
  int count = 0;                                           // number of child processes that quit?  

  while (1)
  {
    pid = waitpid(-1, &status, WNOHANG); 

    if (pid == 0)                                          // returns 0 if no child process to wait for 
    { break; } 

    if (pid == -1)                                         // returns -1 if there was an error 
    {
                                                           // errno will have been set by waitpid() 
      if (errno == ECHILD)                                 // no children 
        { break; }
      if (errno == EINTR)                                  // waitpid() was interrupted by a signal 
        { continue; }                                      // try again 
      else
      {
        printf("unexpected error in cleanup_terminated_children(): %s\n", strerror(errno));
        break;
      }
    } 

    //TODO: figure out what this is supposed to do 
    //print_wait_status(pid, status);       /* supply this yourself */
    /* we have no idea what the function above was intended to make us do. so we ignored it */
    update_process_table(table, pid, status);
    remove_process_table(table, pid);
    count++;
  } 

  return count;
}

/*----------------------------------------------------------------------------*/
