/* CMPSC 311, Spring 2012, Project 8
 *
 * Author:   Brian Golden,    Aaron Goodermuth
 * Email:    bwg5079@psu.edu, ajg5353@psu.edu
 *
 *
 * emulate "make -n" 
 * Adapted from Don Heller's project 5 solution
 *
 * version 1, April 23, 2012
 *   
 * Command-line Options
 *    -h           print help
 *    -v           verbose mode; enable extra printing; can be repeated
 *    -f file      input filename; default is hakefile or Hakefile
 *
 */

//------------------------------------------------------------------------------

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>

#include "cmpsc311.h"
#include "names.h"
#include "macro.h"
#include "targets.h"
#include "recipes.h"
#include "sources.h"

//------------------------------------------------------------------------------

static int read_file(char *filename, int quiet, struct list_targets *targets);
  // return 1 if successful, 0 if not
  // "success" means the file could be opened for reading, or that we had seen
  //    the file before and don't need to read it again
  // quiet == 0 enables error messages if the file can't be opened
  // quiet == 1 suppresses error messages if the file can't be opened

static void read_lines(char *filename, FILE *fp, struct list_targets *targets);
static void parse_sources(char *buf, struct list_sources *list);
static int eval_target(char* target_name, struct list_targets *targets);
static struct timespec mtime(const char *file);
static char *remove_whitespace(char *input);

//static char *prog;
static char *master_target;
static char *default_goal;

// maximum line length in an input file (buffer size in read_lines)
#define MAXLINE 4096

//------------------------------------------------------------------------------

static void usage(int status)
{
  if (status == EXIT_SUCCESS)
    {
      printf("usage: %s [-h] [-v] [-n] [-f file]\n", prog);
      printf("  -h           print help\n");
      printf("  -v           verbose mode; enable extra printing; can be repeated\n");
      printf("  -n           print commands but do not execute them\n");
      printf("  -f file      input filename; default is hakefile or Hakefile\n");
    }
  else
    {
      fprintf(stderr, "%s: Try '%s -h' for usage information.\n", prog, prog);
    }

  exit(status);
}

//------------------------------------------------------------------------------

int main(int argc, char *argv[])
{
  // for use with getopt(3)
  int ch;
  extern char *optarg;
  extern int optind;
  extern int optopt;
  extern int opterr;
  
  struct list_targets targets;

  list_targets_init(&targets, "targets");
    
  // program name as actually used
  prog = argv[0];
  /* In extremely strange situations, argv[0] could be NULL, or point to an
   * empty string.  Let's just ignore that for now.
   */

  // exit status
  int status = EXIT_SUCCESS;

  // option flags and option-arguments set from the command line
  int f_flag = 0;  // number of -f options supplied

  // first, see if the -v option is given
  // we'll catch all the other cases on the next pass over argv
  while ((ch = getopt(argc, argv, ":hvf:")) != -1)
  {
    if (ch == 'v') verbose++;
  }

  // scan the argv array again, from the beginning
  optind = 1;
  while ((ch = getopt(argc, argv, ":hvnf:")) != -1)
  {
    switch (ch) 
    {
      case 'h':
        usage(EXIT_SUCCESS);
        break;
      case 'v':
        // verbose++;
        break;
      case 'n':
        break;
      case 'f':
        f_flag++;    // number of -f options supplied
        (void) read_file(optarg, 0, &targets);
        break;
      case '?':
        fprintf(stderr, "%s: invalid option '%c'\n", prog, optopt);
        usage(EXIT_FAILURE);
        break;
      case ':':
        fprintf(stderr, "%s: invalid option '%c' (missing argument)\n", prog, optopt);
        usage(EXIT_FAILURE);
        break;
      default:
        usage(EXIT_FAILURE);
        break;
    }
  }
    
  if (f_flag == 0 && !read_file("hakefile", 1, &targets) && !read_file("Hakefile", 1, &targets))
  {
    fprintf(stderr, "%s: no input\n", prog);
    usage(EXIT_FAILURE);
  }

  int flag = 0;
  
  if(optind == argc && default_goal != NULL)
  {
    master_target = default_goal;
    flag = eval_target(default_goal, &targets);
    if(flag == 0)
    {
      printf("`%s' is up to date\n", default_goal);
    }
  }
  else if(default_goal != NULL)
  {
    for (int i = optind; i < argc; i++)
    {
      master_target = argv[i];
      flag = eval_target(argv[i], &targets);
      if(flag == 0)
      {
        printf("`%s' is up to date\n", argv[i]);
      }
    }
    
  }
  else
  {
    printf("%s: Error: There were no goals to make\n", prog);
    exit(0);
  }

  list_targets_deallocate(&targets);    

  return status;
}

//------------------------------------------------------------------------------

// return 1 if successful, 0 if not
// "success" means the file could be opened for reading, or that we had seen
//    the file before and don't need to read it again
// quiet == 0 enables error messages if the file can't be opened
// quiet == 1 suppresses error messages if the file can't be opened

static int read_file(char *filename, int quiet, struct list_targets *targets)
{
  verify(filename != NULL, "null arg filename");
  verify(filename[0] != '\0', "empty arg filename");
  verify(targets != NULL, "targets is null");

  if (verbose > 0)
    { fprintf(stderr, "%s: read_file(%s)\n", prog, filename); }

  static struct list_names filenames;                      // file names come from -f and include
  static int init = 0;
  if (init == 0)
  { 
    list_names_init(&filenames, "filenames");
    init++; 
  }

  if (verbose > 1)
    { list_names_print(&filenames); }

  // if (filename is on the list already) { return 1 }
  // else { put filename on the list and continue }
  if (list_names_append_if_new(&filenames, filename) == 1)
    { return 1; }

  if (verbose > 0)
    { list_names_print(&filenames); }

  if (strcmp(filename, "-") == 0)
  { 
    read_lines("[stdin]", stdin, targets); 
    return 1; 
  }

  FILE *fp = fopen(filename, "r");
  if (fp == NULL)
  {
    if (quiet == 0)
      { fprintf(stderr, "%s: could not open input file %s: %s\n", prog, filename, strerror(errno)); }
    return 0;
  }

  read_lines(filename, fp, targets);
  default_goal = targets->head->filename;

  if (fclose(fp) != 0)
  {
    fprintf(stderr, "%s: could not close input file %s: %s\n", prog, filename, strerror(errno));
  }
    
  return 1;
}

//------------------------------------------------------------------------------

static void read_lines(char *filename, FILE *fp, struct list_targets *targets)
{
  verify(filename != NULL, "null arg filename");
  verify(filename[0] != '\0', "empty arg filename");
  verify(fp != NULL, "null arg fp");

  if (verbose > 0)
    { fprintf(stderr, "%s: read_lines(%s)\n", prog, filename); }

  char original[MAXLINE+2];                                // from fgets()
  char expanded[MAXLINE+2];                                // after macro expansion
  char buffer[MAXLINE+2];                                  // working copy, safe to modify

  char whsp[] = " \t\n\v\f\r";                             // whitespace characters
  int line_number = 0;
  int recipe_line_number = 0;

  struct stat s;
  if (stat(filename, &s) == 0)
  {
    if(0 == s.st_size)                                     //file has size 0
    {
      printf("%s: Error: file '%s' has size zero bytes\n", prog, filename);
      exit(0);
    }
  }
  else
  {
    printf("%s: stat() error: %s\n", prog, strerror(errno));
  }
  while (fgets(original, MAXLINE, fp) != NULL) 
  {
    // it is possible that the input line was too long, so terminate the string cleanly
    original[MAXLINE] = '\n';
    original[MAXLINE+1] = '\0';

    line_number++;
    if (verbose > 0) printf("%s: %s: line %d: %s", prog, filename, line_number, original);

    // assume original[] is constructed properly
    // assume expanded[] is large enough
    macro_expand(original, expanded);
    if (verbose > 0) printf("%s: %s: line %d: %s", prog, filename, line_number, expanded);

    strcpy(buffer, expanded);                              // copy, safe to modify

    char *buf = buffer;

    while (*buf == ' ') buf++;                             // skip past leading spaces (not tabs!)

    char *p_hash = strchr(buf, '#');                       // a comment starts with #
    if (p_hash != NULL)
      { *p_hash = '\0'; }                                  // remove the comment

    int n = 0;                                             // remove trailing whitespace
    while (buf[n] != '\0')
      {
        int n1 = strspn(&buf[n], whsp);                    // buf[n .. n+n1-1] is whitespace
        int n2 = strcspn(&buf[n + n1], whsp);              // buf[n+n1 .. n+n1+n2-1] is not
        if (n2 == 0) { buf[n] = '\0'; break; }             // remove trailing whitespace
        n += n1 + n2;
      }

    if (buf[0] == '\0')                                    // nothing left?
      { continue; }

    char *p_colon = strchr(buf, ':');                      // : indicates a target-prerequisite line
    char *p_equal = strchr(buf, '=');                      // = indicates a macro definition
    if (buffer[0] == '\t')
    {
      recipe_line_number++;
      if (verbose > 0) printf("  >>> recipe line %d\n", recipe_line_number);
      
      list_recipes_append(targets->tail->recipes, buffer+1);    // add to receipe list
    }
    else if (p_colon != NULL)
    {
      char target_name[(int)(p_colon - buffer) + 1];
      struct list_sources *sources;
      struct list_recipes *recipes;
      char *temp;
      sources = list_sources_allocate();
      recipes = list_recipes_allocate();
      parse_sources(p_colon+1, sources);
      char *start = buf;
      char *end = buf;
      while(end <= p_colon)                                // get target names from line,
      {                                                    // add target to list
        if (isspace(end[0]) || end[0] == ':')
        {
          if (!(isspace(end[0]) && end[1] == ':'))
          {
            strncpy(target_name, start, (size_t)(end-start+1)); 
            target_name[(int)(end - start)] = '\0'; 
            start = end+1;

            temp = remove_whitespace(target_name);

            list_targets_append(targets, buffer, temp, line_number, sources, recipes);
            free(temp);
          }

        }
        end++;
      }
    
      recipe_line_number = 0;
      
      if (verbose > 0) printf("  >>> target-prerequisite\n");
    }
    else if (p_equal != NULL)
    {
      if (verbose > 0) printf("  >>> macro definition\n");
        // name = body
        // *p_equal is '='
      char *name_start = buf;
      while (*name_start == ' ' || *name_start == '\t')    // skip past spaces and tabs
        { name_start++; }
      char *name_end = p_equal-1;
      while (*name_end == ' ' || *name_end == '\t')
        { name_end--; }
      name_end++;
      *name_end = '\0';
      char *body_start = p_equal+1;
      while (*body_start == ' ' || *body_start == '\t')
        { body_start++; }
      char *body_end = body_start;
      while (*body_end != '\0')                            // end of string
        { body_end++; }
      while (*body_end == ' ' || *body_end == '\t')
        { body_end--; }
      body_end++;
      *body_end = '\0';
      if (verbose > 1) macro_list_print();
      macro_set(name_start, body_start);
      if (verbose > 1) macro_list_print();
    }
    else if (strncmp("include", buf, 7) == 0)
    {
      if (verbose > 0) printf("  >>> include\n");
      char *name_start = buf + 7;                          // skip past "include"
      while (*name_start == ' ' || *name_start == '\t')    // skip past spaces and tabs
        { name_start++; }
      if (*name_start == '\0')
      {
        // following GNU Make, this is not an error
        if (verbose > 0) fprintf(stderr, "%s: %s: line %d: include but no filename\n", prog, filename, line_number);
        continue;
      }
      else if (*name_start == '\'' || *name_start == '"')  // quoted filename
      {
        // find matching quote, remove it
        char *q = name_start + 1;                          // skip past ' or "
        while (*q != *name_start && *q != '\0') q++;       // find end of string or line
        if (*q == '\0')
        {
          fprintf(stderr, "%s: %s: line %d: file name error: >>>%s<<<\n", prog, filename, line_number, name_start);
          continue;
        }
        name_start++;                                      // skip past opening quote
        *q = '\0';                                         // remove closing quote
      }
      read_file(name_start, 0, targets);
    }
    else
    {
      if (verbose > 0) printf("  >>> something else\n");
      fprintf(stderr, "%s: %s: line %d: not recognized: %s", prog, filename, line_number, original);
    }
  }

  if (ferror(fp))                                          // error when reading the file
    { fprintf(stderr, "%s: %s: read error: %s\n", prog, filename, strerror(errno)); }

  return;
}

static void parse_sources(char *buf, struct list_sources *list)
{
  int i = 0;

  while(buf[i] != '\0')
  {
    if(!isspace(buf[i]))
    {
      char* end = buf + i;
      while(!isspace(end[0]) && end[0] != '\0') end++;
      //add to list
      char source_name[(int)(end - (buf+i)+1)];
      strncpy(source_name, buf+i, (int)(end - (buf+i)));
      source_name[(int)(end - (buf+i))] = '\0';
      list_sources_append(list, source_name);
      i = end - buf;
    }
    i++;
  }
}

//------------------------------------------------------------------------------

static int eval_target(char* target_name, struct list_targets *targets)
{
  int found = 0;                                           //status of whether things were made
  int flag = 0;
  struct target *cur;
  struct timespec t1 = mtime(target_name);                 // target
  struct timespec t2;                                      // source

  for(cur = targets->head; cur != NULL; cur = cur->next)
  {
    if(0 == strcmp(cur->filename, target_name) )
    {
      struct source *s;

      s = cur->sources->head;

      if (s == NULL)
      {
        list_recipes_print(cur->recipes);
        return 1;
      }

      for(s = cur->sources->head; s != NULL; s = s->next)
      {
        FILE *fp;
        if((fp = fopen(s->name, "r")) != NULL)             //file exists
        {
          fclose(fp);
          t2 = mtime(s->name);
          if (t1.tv_sec == 0                               // target does not exist
              || t1.tv_sec < t2.tv_sec                     // target is older than source
              || ((t1.tv_sec == t2.tv_sec) && (t1.tv_nsec < t2.tv_nsec))
             )
          {
            if (strcmp(master_target, cur->filename) != 0)
              { list_recipes_print(cur->recipes); }
            found++;
            
          }
        }
        else 
        {
          flag = 0;
          struct target *t;
          for(t = targets->head; t != NULL; t = t->next)
          {
            if(0 == strcmp(t->filename, s->name))
            {
              found += eval_target(s->name, targets);
              flag = 1;
            }
          }
          
          if(0 == flag)
          {
            printf("%s: Fatal error: Don't know how to make target `%s'\n", prog, s->name);
            exit(0);
          }
        }

      }
      
      if(strcmp(master_target, cur->filename) == 0 && found > 0)
      {
        list_recipes_print(cur->recipes);
      }
      break; 
    }
  }

  return found;
}


static struct timespec mtime(const char *file)
{
  struct stat s;
  struct timespec t = { 0, 0 };

  if (stat(file, &s) == 0)
#if   defined(MTIME) && MTIME == 1                         // Linux
    { t = s.st_mtim; }
#elif defined(MTIME) && MTIME == 2                         // Mac OS X
    { t = s.st_mtimespec; }
#elif defined(MTIME) && MTIME == 3                         // Mac OS X, with some additional settings
    { t.tv_sec = s.st_mtime; t.tv_nsec = s.st_mtimensec; }
#else                                                      // Solaris
    { t.tv_sec = s.st_mtime; }
#endif 

  return t;
}

static char *remove_whitespace(char *input)
{
  char *temp = strdup(input);
  int i;  

  for (i = 0; temp[i] != '\0'; i++)  
  {
    if (isspace(temp[i]))
    { 
      temp[i] = '\0'; 
      break;
    }
  }

  return temp;
}
