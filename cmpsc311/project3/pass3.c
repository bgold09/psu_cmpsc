/* CMPSC 311, Spring 2012, Project 3
 * pass3.c  
 *
 *  Author:   Brian Golden
 *  Email:    jms6520@psu.edu
 *  
 *  verison 1, 10 Feb. 2012    
 */
#include <stdio.h>
#include <ctype.h>

int main(int argc, char *argv[])
{
  int c0, c1, c2;

  c0 = getc(stdin); c1 = getc(stdin);c2 = getc(stdin);

  while (c0 != EOF) 
  {
    int shift = 1;

    if(c0 == '\\')
    {
      switch(c1) 
      { 
        case EOF:
          fprintf(stderr, "warning: backslash at end of file\n");
          shift = 2;
          break;
        case '\n':
          shift = 2;
          if(c2 == EOF)
          {
            fprintf(stderr, "warning: backslash-newline at end of file\n");
            shift = 3;
          }
            break;
         default:
          putc(c0, stdout); 
          break;   
       } 
     }
     else
     {
       putc(c0,stdout);
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
}
