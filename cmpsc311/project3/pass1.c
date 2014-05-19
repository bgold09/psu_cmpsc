/* CMPSC 311, Spring 2012, Project 3
 * pass1.c 
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
  int c0, c1;

  c0 = getc(stdin);
  c1 = getc(stdin);

  while (c0 != EOF)
  {
    int shift = 1;

    switch(c0)
    {
      case '\f':
      case '\v':
      case '\t':
        putc(' ', stdout);
        break;
      case '\r':
        if (c1 == '\n')
        { 
          putc('\n', stdout); 
          shift = 2; 
        }
        else 
        { 
          putc('\n', stdout); 
        }
        break;
      default:
        putc(c0, stdout);
        break;
    }

    if (shift == 1)
    {
      c0 = c1;
      c1 = getc(stdin);
    }
    else
    {
      c0 = getc(stdin);
      c1 = getc(stdin);
    }

  }
}
