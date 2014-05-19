/* CMPSC 311, Spring 2012, Project 5
 *
 * Author:   Brian Golden
 * Email:    bwg5079@psu.edu
 *
... <additional comment text>
 */

//--------------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cmpsc311.h"
#include "macro.h"

#define MAXLINE 4096
//--------------------------------------------------------------------------------

// list of known macros, initially empty

struct macro {
  struct macro *next;
  char *name;
  char *body;
};

static struct macro macro_list = {
  .next = NULL,
  .name = NULL,
  .body = NULL
};

//--------------------------------------------------------------------------------

void macro_list_print(void)
{
  printf("Macro list:\n");

  if (macro_list.name != NULL)
  {
    for (struct macro *p = &macro_list; p != NULL; p = p->next)
    {
      if (p->name) printf("  %s = %s (%d)\n", p->name, p->body, macro_length(p->name));
    }
  }

  printf("  -- end of list\n");
  
}

static struct macro *macro_lookup(char *name)
{
  verify(name != NULL, "null arg name");

  if (macro_list.name == NULL)
  {
    return NULL;
  }
  else
  {
    for (struct macro *p = &macro_list; p != NULL; p = p->next)
    {
      if (strcmp(p->name, name) == 0)
        { return p; }
    }
  }

  return NULL;
}

char *macro_body(char *name)
{
  verify(name != NULL, "null arg name");

  struct macro *temp = macro_lookup(name);

  if (temp == NULL)
    { return ""; }

  return temp->body;
}

int macro_length(char *name)
// POST: returns the length of the macro name, 
//       -1 if macro name was not found
{
  verify(name != NULL, "null arg name");

  struct macro *temp = macro_lookup(name);

  if (temp == NULL)
    { return 0; }

  return strlen(temp->body);
}

void macro_set(char *name, char *body)
{
  verify(name != NULL, "null arg name");
  verify(body != NULL, "null arg body");
  verify(name[0] != '\0', "empty arg name");

  int macroset = 0;
  struct macro *fn = Malloc(sizeof(struct macro));

  fn->next = NULL;
  fn->name = Strdup(name);
  fn->body = Strdup(body);

  if (macro_list.name == NULL)
  {
    macro_list = *fn;
  }
  else
  {
    struct macro *p;

    for (p = &macro_list; p != NULL && !macroset; p = p->next)
    {
      if (strcmp(p->name, name) == 0)
      {
        p->body = fn->body;
        macroset = 1;
      }
      else if (p->next == NULL)
      {
        p->next = fn;
        macroset = 1;
      }
    }
  }
}

// assume in[] is constructed properly
// assume out[] is large enough
// assume macro definition is valid
void macro_expand(char *in, char *out)
{
  int ending_brace_index = -1;
  char *name;

  for (int i = 0; i < (int) strlen(in); i++)
  {
    if(in[i] == '$' && (in[i+1] == '{' || in[i+1] == '('))  // possible beginning of macro
    {
      ending_brace_index = -1;

      for (int j = i; j < (int) strlen(in) && ending_brace_index == -1; j++)
      {
        if (in[j] == '}')
        {
          ending_brace_index = j;
        }
      } 

      if (ending_brace_index != -1)    // a closing brace and therefore a macro was found
      {
        if (i+1 == ending_brace_index-1)
        {
          //fprintf(stderr, "%s: %s: line %d: empty macro name\n", prog, filename, line_number);
          continue;
        }
        else                            // not an empty macro name
        {
          name = strdup(in);
          name[ending_brace_index] = '\0';
          name += (i + 2);
           
          int body_length = macro_expand_length(name);
 
          char *body = macro_body(name);
          char temp_buf[MAXLINE+2];
          
          int q;
          int index = 0;

          for (q = 0; in[q] != '\0'; q++)
          {
            if (q < i || q > ending_brace_index)       // if before or after macro
            {
              temp_buf[index] = in[q];
              index++;
            }
            else                                       // reached macro
            {
              for (int r = 0; r < (int) strlen(body); r++)
              {
                temp_buf[index] = body[r];
                index++;
              }

              q = ending_brace_index;                  // skip past rest of macro name  
            }

          }
          temp_buf[index] = '\0';
             
          strcpy(out, temp_buf);
        } 
      }
    }
  }
}

// assume in[] is constructed properly
// returns length of the expansion
int macro_expand_length(char *in)
// POST: returns length of macro body, returns -1 if 
//       macro definition could not be found

{
  struct macro *temp = macro_lookup(in);

  if (temp == NULL)
    { return -1; }

  return strlen(temp->body);
}

//--------------------------------------------------------------------------------









