/* CMPSC 311, Spring 2012, Project 3
 * pass4.c
 *
 * Author:   Brian Golden
 * Email:    bwg5079@psu.edu
 *
 * version 1, 10 Feb. 2012
 */


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int main (int argc, char *argv[])
{
  int c0, c1, c2, newline_ct;
  bool look_doub_quote, look_sing_quote, look_sing_comment, look_multi_comment;
  
  c0 = getc(stdin);
  c1 = getc(stdin);
  c2 = getc(stdin);
  newline_ct = 0;

  look_doub_quote = look_sing_quote = look_sing_comment = look_multi_comment = false;

  while (c0 != EOF)
  {
    int shift = 1;

    if (look_doub_quote)                  // if looking for end of a string literal
    {
      putc(c0, stdout);
      if (c0 == '\\' && c1 == '\\' && c2 == '"')  // 
      {
        putc('\\', stdout);
        putc('"', stdout);
        look_doub_quote = false;
        shift = 3;        
      }
      else if (c0 == '\\' && c1 == '"')
        { putc('"', stdout); shift = 2; }

      if (c0 == '"')                      // if end of string literal was found
        { look_doub_quote = false; }
    }
    else if (look_sing_quote)             // if looking for the end of a character
    {
      putc(c0, stdout);
      
      if (c0 == '\\' && c1 == '\\' && c2 == '\'')
      { 
        putc('\\', stdout); 
        putc('\'', stdout); 
        look_sing_quote = false; 
        shift = 3; 
      }
      else if (c0 == '\\' && c1 == '\'')
        { putc('\'', stdout); shift = 2; }

      if (c0 == '\'')                     // if end of character constant was found
        { look_sing_quote = false; }
    }
    else if (look_sing_comment)           // if looking for the end of a single-line comment '\n'
    {
      if (c0 == '\n')                     // if the end of a single-line comment has been found
      {
        fprintf(stdout, " \n");           // output a single space character where the comment was
        look_sing_comment = false;
      }
    }
    else if (look_multi_comment)          // while looking for the end of a multi-line comment "*/"
    {
      if (c0 == '\n')
      { 
        putc(c0, stdout); 
        newline_ct++;
      }
 
      if (c0 == '*' && c1 == '/')         // if end of multi-line comment was found
      { 
        look_multi_comment = false; 
        
        if (newline_ct == 0)
         { putc(' ', stdout); }
      }
    }
    else                                  // if not looking for any char, string or comment ending
    {
      switch(c0)
      {
        case '\'':
          putc(c0, stdout);
          look_sing_quote = true;         // start looking for next '\'
          break;
        case '"':
          putc(c0, stdout);
          look_doub_quote = true;         // start looking for next '"'
          break;
        case '/':
          if (c1 == '/')                  // beginning of single-line comment found
          {
            look_sing_comment = true;     // start looking for newline to end comment
            shift = 2;  
          }
          else if (c1 == '*')             // beginning of multi-line comment found         
          { 
            look_multi_comment = true;    // start looking for end of multi-line comment
            newline_ct = 0;
            shift = 2; 
          }
          break;
        default:
          putc(c0, stdout);
          break;
      }
    }

    if (shift == 1)
    {
      c0 = c1;
      c1 = c2; 
      c2 = getc(stdin);
    }
    else if (shift == 2)
    {
      c0 = c2;
      c1 = getc(stdin);
      c2 = getc(stdin);
    } 
    else
    {
      c0 = getc(stdin);
      c1 = getc(stdin);
      c2 = getc(stdin);
    }     
  }

  if (look_sing_comment)
    { fprintf(stderr, "warning: unterminated // comment, fixed, Pass 4\n"); }
  if (look_multi_comment)
    { fprintf(stderr, "warning: unterminated /* */ comment, fixed, Pass 4\n"); }

  return 0;
}
