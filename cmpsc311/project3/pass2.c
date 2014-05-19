/* CMPSC 311, Spring 2012, Project 3
 * pass2.c 
 *
 * Author:   Brian Golden
 * Email:    bwg5079@psu.edu
 * 
 * version 1, 10 Feb. 2012
 *
 */

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
  int c0, c1, c2;

  c0 = getc(stdin);
  c1 = getc(stdin);
  c2 = getc(stdin);

  while (c0 != EOF)
  {
    int shift = 1;

    if (c0 == '\?' && c1 == '\?')
    {
      switch(c2)
      {
        case '=':
          putc('#', stdout);
          shift = 2;
          break;
        case '(':
          putc('[', stdout);
          shift = 2;
          break;
        case '/':
          putc('\\', stdout);
          shift = 2;
          break;
        case ')':
          putc(']', stdout);
          shift = 2;
          break;
        case '\'':
          putc('^', stdout);
          shift = 2;
          break;
        case '<':
          putc('{', stdout);
          shift = 2;
          break;
        case '!':
          putc('|', stdout); 
          shift = 2;
          break;
        case '>':
          putc('}', stdout);          
          shift = 2;
          break;
        case '-':
          putc('~', stdout);
          shift = 2;
          break;
        default:
          putc('\?', stdout);
          break;
      }
    }
    else
    {
      putc(c0, stdout);
    }

    if (shift == 1)
    {
      c0 = c1;
      c1 = c2;
      c2 = getc(stdin);
    }
    else
    {
      c0 = getc(stdin);
      c1 = getc(stdin);
      c2 = getc(stdin);
    }

  }
}
