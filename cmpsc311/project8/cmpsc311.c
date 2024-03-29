macro.c                                                                                             0000600 0046321 0000152 00000005731 11745344451 011154  0                                                                                                    ustar   bwg5079                         ucse                                                                                                                                                                                                                   /* CMPSC 311, Spring 2012, Project 8
 *
 * Author:   Brian Golden,    Aaron Goodermuth
 * Email:    bwg5079@psu.edu, ajg5353@psu.edu
 *
 * makefile macro-expansion implementation
 * version 1, 24 April 2012
 */

//--------------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cmpsc311.h"
#include "macro.h"

//--------------------------------------------------------------------------------

// list of known macros, initially empty

struct macro {
  struct macro *next;
  char *name;
  char *body;
  int body_length;
};

static struct macro macro_list = {
  .next = NULL,
  .name = NULL,
  .body = NULL,
  .body_length = 0
};

//--------------------------------------------------------------------------------

void macro_list_print(void)
{
  printf("Macro list\n");

  for (struct macro *p = macro_list.next; p != NULL; p = p->next)
    {
      printf("  %s = %s (%d)\n", p->name, p->body, p->body_length);
    }

  printf("  -- end of list\n");
}

static struct macro *macro_lookup(char *name)
{
  for (struct macro *p = macro_list.next; p != NULL; p = p->next)
    {
      if (strcmp(p->name, name) == 0)
        { return p; }
    }

  return NULL;
}

char *macro_body(char *name)
{
  struct macro *p = macro_lookup(name);

  return (p == NULL) ? "" : p->body;
}

int macro_length(char *name)
{
  struct macro *p = macro_lookup(name);

  return (p == NULL) ? 0 : p->body_length;
}

void macro_set(char *name, char *body)
{
  struct macro *p = macro_lookup(name);

  if (p == NULL)
    {
      p = Malloc(sizeof(struct macro));
      p->next = macro_list.next;
      macro_list.next = p;
      p->name = strdup(name);
      p->body = strdup(body);
      p->body_length = strlen(body);
    }
  else
    {
      free(p->body);
      p->body = strdup(body);
      p->body_length = strlen(body);
    }
}

// assume in[] is constructed properly
// assume out[] is large enough
void macro_expand(char *in, char *out)
{
  while (*in != '\0')
    {
      if (*in == '$')	// macro, to be expanded
        {
	  char *name = in+2;	// skip past ${
	  char *end = name;
	  while (*end != '}') end++;
	  *end = '\0';	// temporary
	  char *body = macro_body(name);
	  *end = '}';	// restore original }
	  strcpy(out, body);
	  out += strlen(body);
	  in = end+1;	// skip past }
	}
      else
        {
	  *out++ = *in++;
	}
    }

  *out = '\0';
}

// assume in[] is constructed properly
// returns length of the expansion
int macro_expand_length(char *in)
{
  int n = 0;

  while (*in != '\0')
    {
      if (*in == '$')	// macro, to be expanded
        {
	  char *name = in+2;	// skip past ${
	  char *end = name;
	  while (*end != '}') end++;
	  *end = '\0';	// temporary
	  int body_length = macro_length(name);
	  *end = '}';	// restore original }
	  n += body_length;
	  in = end+1;	// skip past }
	}
      else
        {
	  in++;
	  n++;
	}
    }

  return n;
}

//--------------------------------------------------------------------------------

                                       names.c                                                                                             0000600 0046321 0000152 00000012763 11745344451 011161  0                                                                                                    ustar   bwg5079                         ucse                                                                                                                                                                                                                   /* CMPSC 311, Spring 2012, Project 8
 *
 * Author:   Brian Golden,    Aaron Goodermuth
 * Email:    bwg5079@psu.edu, ajg5353@psu.edu
 *
 * makefile file list implementation
 * version 1, 24 April 2012
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <errno.h>
#include <string.h>

#include "cmpsc311.h"
#include "names.h"

//------------------------------------------------------------------------------

void list_names_init(struct list_names *list, const char *listname)
{
  verify(list != NULL, "null arg list");
  verify(listname != NULL, "null arg listname");
  verify(listname[0] != '\0', "empty arg listname");

  list->name = Strdup(listname);
  list->head = list->tail = NULL;
}

//------------------------------------------------------------------------------

struct list_names *list_names_allocate(const char *listname)
{
  verify(listname != NULL, "null arg listname");
  verify(listname[0] != '\0', "empty arg listname");

  struct list_names *list = Malloc(sizeof(struct list_names));

  list->name = Strdup(listname);
  list->head = list->tail = NULL;

  return list;
}

//------------------------------------------------------------------------------

void list_names_deallocate(struct list_names *list)
{
  verify(list != NULL, "null arg list");

  free(list->name);
  struct name *prev = NULL;
  for (struct name *p = list->head; p != NULL; p = p->next)
    {
      free(prev);	// free(NULL) is harmless
      free(p->name);
      prev = p;
    }
  free(prev);		// prev == list->tail
  free(list);
}

//------------------------------------------------------------------------------

void list_names_print(struct list_names *list)
{
  verify(list != NULL, "null arg list");

  printf("list of names: %s\n", safe_string(list->name));

  if (list->head == NULL)
    {
      printf("  <empty>\n");
    }
  else
    {
      for (struct name *p = list->head; p != NULL; p = p->next)
	{
	  printf("  %s\n", p->name);
	}
    }
}

//------------------------------------------------------------------------------

void list_names_append(struct list_names *list, const char *name)
{
  verify(list != NULL, "null arg list");
  verify(name != NULL, "null arg name");
  verify(name[0] != '\0', "empty arg name");

  struct name *fn = Malloc(sizeof(struct name));

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

// if (name is on the list already) { return 1 }
// else { put name on the list and return 0 }

int list_names_append_if_new(struct list_names *list, const char *name)
{
  verify(list != NULL, "null arg list");
  verify(name != NULL, "null arg name");
  verify(name[0] != '\0', "empty arg name");

  for (struct name *p = list->head; p != NULL; p = p->next)
    {
      if (strcmp(p->name, name) == 0)
        return 1;	// name is on the list already
    }

  list_names_append(list, name);

  return 0;
}

//------------------------------------------------------------------------------

void list_names_append_from_file(struct list_names *list, const char *filename)
{
  verify(list != NULL, "null arg list");
  verify(filename != NULL, "null arg filename");
  verify(filename[0] != '\0', "empty arg filename");

  FILE *infile;

  if (strcmp(filename, "-") == 0)
    { infile = stdin; }
  else
    {
      infile = fopen(filename, "r");
      if (infile == NULL)
        {
          fprintf(stderr, "%s: failed: could not open file %s: %s\n", prog, filename, strerror(errno));
          exit(EXIT_FAILURE);
        }
    }

  char buffer[256];
  char whsp[] = " \t\n\v\f\r";		// whitespace characters
  char *p;

  while (fgets(buffer, sizeof(buffer), infile) != NULL)
    {
      /* Note that buffer has a newline at the end if the input line
       *   was short enough to fit.  Line-too-long is probably an error,
       *   to be caught later.
       * NULL from fgets() indicates end-of-file or error, so we just quit.
       */
      p = strchr(buffer, '\n');		// points to the newline character, if there is one
      if (p != NULL)
        { *p = '\0'; }	// remove the newline
  
      p = strchr(buffer, '#');		// a comment starts with #
      if (p != NULL)
        { *p = '\0'; }	// remove the comment

      int m = strspn(buffer, whsp);	// index of first non-whitespace character

      // remove trailing whitespace
      int n = 0;
      while (buffer[n] != '\0')
        {
	  int n1 = strspn(&buffer[n], whsp);		// buffer[n .. n+n1-1] is whitespace
	  int n2 = strcspn(&buffer[n + n1], whsp);	// buffer[n+n1 .. n+n1+n2-1] is not
	  if (n2 == 0) { buffer[n] = '\0'; break; }	// remove trailing whitespace
	  n += n1 + n2;
	}

      if (buffer[m] == '\0') continue;	// empty line

      list_names_append(list, &buffer[m]);
    } 

  if (infile != stdin && fclose(infile) != 0)
    {
      fprintf(stderr, "%s: failed: could not close input file %s: %s\n", prog, filename, strerror(errno));
      exit(EXIT_FAILURE);
    }
}

//------------------------------------------------------------------------------

void list_names_iterate(struct list_names *list, void *func(const char *))
{
  verify(list != NULL, "null arg list");
  verify(func != NULL, "null arg func");

  for (struct name *p = list->head; p != NULL; p = p->next)
    {
      printf("calling func on %s\n", p->name);
    }
}

//------------------------------------------------------------------------------

             pr8.4.c                                                                                             0000600 0046321 0000152 00000040315 11745344451 010723  0                                                                                                    ustar   bwg5079                         ucse                                                                                                                                                                                                                   /* CMPSC 311, Spring 2012, Project 8
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
                                                                                                                                                                                                                                                                                                                   recipes.c                                                                                           0000600 0046321 0000152 00000005103 11745344452 011477  0                                                                                                    ustar   bwg5079                         ucse                                                                                                                                                                                                                   /* CMPSC 311, Spring 2012, Project 8
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

                                                                                                                                                                                                                                                                                                                                                                                                                                                             sources.c                                                                                           0000600 0046321 0000152 00000004737 11745344452 011544  0                                                                                                    ustar   bwg5079                         ucse                                                                                                                                                                                                                   /* CMPSC 311, Spring 2012, Project 8
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
                                 targets.c                                                                                           0000600 0046321 0000152 00000007114 11745344452 011522  0                                                                                                    ustar   bwg5079                         ucse                                                                                                                                                                                                                   /* CMPSC 311, Spring 2012, Project 8
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
                                                                                                                                                                                                                                                                                                                                                                                                                                                    cmpsc311.h                                                                                          0000600 0046321 0000152 00000003300 11745344451 011400  0                                                                                                    ustar   bwg5079                         ucse                                                                                                                                                                                                                   /* CMPSC 311, Spring 2012, Project 8
 *
 * Author:   Brian Golden,    Aaron Goodermuth
 * Email:    bwg5079@psu.edu, ajg5353@psu.edu
 *
 * base library for CMPSC 311 projects
 * version 1, 24 April 2012
 */

#ifndef CMPSC311_H
#define CMPSC311_H

#include <stdbool.h>

//------------------------------------------------------------------------------

// global variables set from the command line

extern char *prog;	// program name
extern int verbose;	// -v option, extra output
			// -v can be repeated for even more output

//------------------------------------------------------------------------------

// utility functions

// guard against null pointer for character string output
const char *safe_string(const char *str);

//------------------------------------------------------------------------------

// check function arguments

#define verify(tf, msg) cmpsc311_verify(tf, msg, __func__, __LINE__)

void cmpsc311_verify(const bool tf, const char *msg				, const char *func, const int line);

//------------------------------------------------------------------------------

// check function return values
//   malloc
//   strdup
//   fopen

#define Malloc(size) cmpsc311_malloc(size, __func__, __LINE__)
#define Strdup(s1) cmpsc311_strdup(s1, __func__, __LINE__)
#define Fopen(filename,mode) cmpsc311_fopen(filename, mode, __func__, __LINE__)

void *cmpsc311_malloc(const size_t size						, const char *func, const int line);
char *cmpsc311_strdup(const char *s1						, const char *func, const int line);
FILE *cmpsc311_fopen(const char * restrict filename, const char * restrict mode	, const char *func, const int line);

//------------------------------------------------------------------------------

#endif

                                                                                                                                                                                                                                                                                                                                macro.h                                                                                             0000600 0046321 0000152 00000001156 11745344451 011156  0                                                                                                    ustar   bwg5079                         ucse                                                                                                                                                                                                                   /* CMPSC 311, Spring 2012, Project 8
 *
 * Author:   Brian Golden,    Aaron Goodermuth
 * Email:    bwg5079@psu.edu, ajg5353@psu.edu
 *
 * makefile macro-expansion header
 * version 1, 24 April 2012
 */

#ifndef CMPSC311_MACRO_H
#define CMPSC311_MACRO_H

void  macro_list_print(void);
char *macro_body(char *name);
int   macro_length(char *name);
void  macro_set(char *name, char *body);

// assume in[] is constructed properly
// assume out[] is large enough
void  macro_expand(char *in, char *out);

// assume in[] is constructed properly
// returns length of the expansion
int   macro_expand_length(char *in);

#endif

                                                                                                                                                                                                                                                                                                                                                                                                                  names.h                                                                                             0000600 0046321 0000152 00000003410 11745344451 011153  0                                                                                                    ustar   bwg5079                         ucse                                                                                                                                                                                                                   /* CMPSC 311, Spring 2012, Project 8
 *
 * Author:   Brian Golden,    Aaron Goodermuth
 * Email:    bwg5079@psu.edu, ajg5353@psu.edu
 *
 * makefile file list header
 * version 1, 24 April 2012
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

                                                                                                                                                                                                                                                        recipes.h                                                                                           0000600 0046321 0000152 00000003152 11745344452 011506  0                                                                                                    ustar   bwg5079                         ucse                                                                                                                                                                                                                   /* CMPSC 311, Spring 2012, Project 8
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

                                                                                                                                                                                                                                                                                                                                                                                                                      sources.h                                                                                           0000600 0046321 0000152 00000003144 11745344452 011540  0                                                                                                    ustar   bwg5079                         ucse                                                                                                                                                                                                                   /* CMPSC 311, Spring 2012, Project 8
 *
 * Author:   Brian Golden,    Aaron Goodermuth
 * Email:    bwg5079@psu.edu, ajg5353@psu.edu
 *
 * makefile source list header
 * version 1, 24 April 2012
 */

#ifndef CMPSC311_SOURCES_H
#define CMPSC311_SOURCES_H

//------------------------------------------------------------------------------

// singly-linked list of sources

struct source
{
  struct source *next;	     // NULL indicates end-of-list
  char *name;		         // from strdup()
};

struct list_sources
{
  struct source *head;	     // NULL indicates empty list
  struct source *tail;
};

//------------------------------------------------------------------------------

void list_sources_init(struct list_sources *list);
struct list_sources *list_sources_allocate(void);
void list_sources_deallocate(struct list_sources *list);
void list_sources_append(struct list_sources *list, const char *name);
void list_sources_iterate(struct list_sources *list, void *func(const char *));
void list_sources_print(struct list_sources *list);

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

                                                                                                                                                                                                                                                                                                                                                                                                                            targets.h                                                                                           0000600 0046321 0000152 00000003750 11745344452 011531  0                                                                                                    ustar   bwg5079                         ucse                                                                                                                                                                                                                   /* CMPSC 311, Spring 2012, Project 8
 *
 * Author:   Brian Golden,    Aaron Goodermuth
 * Email:    bwg5079@psu.edu, ajg5353@psu.edu
 *
 * makefile target list header
 * version 1, 24 April 2012
 */

#ifndef CMPSC311_TARGETS_H
#define CMPSC311_TARGETS_H

#include "sources.h"
#include "recipes.h"

//------------------------------------------------------------------------------

// singly-linked list of targets

struct target
{
  struct target *next;	               // NULL indicates end-of-list
  char *line;		                   // from strdup()
  char *filename;
  int linenum;
  struct list_sources *sources;
  struct list_recipes *recipes;
};

struct list_targets
{
  struct target *head;	               // NULL indicates empty list
  struct target *tail;
  char *name;		                   // from strdup()
};

//------------------------------------------------------------------------------

void list_targets_init(struct list_targets *list, const char *listname);
struct list_targets *list_targets_allocate(const char *listname);
void list_targets_deallocate(struct list_targets *list);
void list_targets_print(struct list_targets *list);
void list_targets_append(struct list_targets *list, const char *line, const char *filename,
                         int linenum, struct list_sources *sources, struct list_recipes *recipes);
void list_targets_iterate(struct list_targets *list, void *func(const char *));

//------------------------------------------------------------------------------

/* Notes
 *
 * If list_targets_allocate() or one of the list_targets_append() functions
 *   can't allocate enough memory, the program ends with an error message.
 *
 * Do not apply list_targets_deallocate() to something that did not come from
 *   list_targets_allocate().
 *
 * list_targets_append_if_new() works as follows:
 *   if (name is on the list already) { return 1 }
 *   else { put name on the list and return 0 }
 *
 */

//------------------------------------------------------------------------------

#endif
                        Makefile                                                                                            0000600 0046321 0000152 00000001605 11745344451 011343  0                                                                                                    ustar   bwg5079                         ucse                                                                                                                                                                                                                   # CMPSC 311, Spring 2012, Project 8 
#
# Author:   Brian Golden,    Aaron Goodermuth
# Email:    bwg5079@psu.edu, ajg5353@psu.edu

SRC = pr8.4.c
LIB = cmpsc311.c names.c macro.c sources.c recipes.c targets.c
INC = cmpsc311.h names.h macro.h sources.h recipes.h targets.h

# select the version of mtime() appropriate for your OS

# Solaris, mtime() uses time_t st_mtime
OPT = -D_XOPEN_SOURCE=600

# Linux, mtime() uses struct timespec st_mtim
#OPT = -D_XOPEN_SOURCE=700 -DMTIME=1

# Mac OS X, mtime() uses struct timespec st_mtimespec
# OPT = -DMTIME=2

pr8: $(SRC) $(LIB) $(INC)
	c99 $(OPT) -v -o pr8 $(SRC) $(LIB)

check: $(SRC) $(LIB) $(INC)
	c99 -$(OPT) -v -o pr8  $(SRC) $(LIB)
	gcc -std=c99 $(OPT) -Wall -Wextra -o pr8 $(SRC) $(LIB)
	lint -Xc99 $(SRC) $(LIB)
	rm pr8

pr8-lint: $(SRC) $(LIB) $(INC)
	lint -Xc99 $(SRC) $(LIB)

clean: 
	rm -f pr8 a.out *.o a b c d e f g h 
	
test:
	sh test-script
                                                                                                                           README                                                                                              0000700 0046321 0000152 00000002125 11745627757 010577  0                                                                                                    ustar   bwg5079                         ucse                                                                                                                                                                                                                   The program works. pr8 mimics the behavior of the make program with the '-n' option enabled. 

Total Time spent on the project: approximately 10 hrs. 
The project was mostly straightforward and easy to complete. 

Known Bugs:
  # There is a discovered issue with the way the program deals with macro expansion. 
    We feel this is a bug in the code provided to us, and as such, did not attempt to fix it 
	(as it was outside the scope of this project, having been told that we are to build this program 
	under the assumption that the macro expansion was successful).
	
  # There was a discovered issue with the code provided to us, related to trying to read empty files as makefiles. Attempting to run an empty makefile would cause a Segmentation Fault. This bug, although present in the code provided, was fixed in pr8. pr8 provides an error message and quits gracefully. 

From here it would be trivial to create a rudimentary make program. Now that we can determine when 
it is appropriate to execute a recipe, all we would need to do is provide a call to an exec function 
with the recipe we are given.
                                                                                                                                                                                                                                                                                                                                                                                                                                           test-script                                                                                         0000600 0046321 0000152 00000000652 11745345101 012101  0                                                                                                    ustar   bwg5079                         ucse                                                                                                                                                                                                                   # test script for CMPSC 311 Project 8

# Important - edit this file!
#    change pr8 to the name of your program

while read in
do
  echo 'pr8 test:' $in
  $in
  echo
done <<EOF
  rm -f a b c d e f g h 
  pr8
  sleep 1
  touch d
  pr8
  sleep 1
  touch e
  pr8
  sleep 1
  touch c
  pr8
  sleep 1
  touch b
  pr8
  sleep 1
  touch a
  pr8
  sleep 1
  touch c
  pr8
  pr8 b c
  pr8 f
  touch h
  pr8 f
  pr8 g
  pr8 empty
EOF

                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      