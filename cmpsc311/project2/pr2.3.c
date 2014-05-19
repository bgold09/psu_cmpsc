/* CMPSC 311, Spring 2012, Project 2
 *
 * Author:   Brian Golden
 * Email:    bwg5079@psu.edu
 *
 * version 3, 30 Jan. 2012
 *   check all character types
 *   determine line terminator types
 *   determine text file type
 *   check for oddities in input data 
 *   print results
 * version 2, 20 Jan. 2012
 *   replace apue.h with
 *     stdio.h   for getc(), putc(), fprintf()
 *     stdlib.h  for exit()
 *   replace err_sys() with a simpler error handler, report_failure()
 *   replace main(void)
 *   reformat to personal taste
 * version 1, 20 Jan. 2012
 *   APUE, Fig. 1.5
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

void report_failure(char *msg)
{
  fprintf(stderr, "failure: %s\n", msg);
  exit(1);
}

int main(int argc, char *argv[])
{
  int c;		// current read character
  int characters, alnums, alphas, blanks, cntrls, digits, graphs, lowers, prints, puncts, spaces, uppers, xdigits; 
  int return_chars, newline_chars, ret_new_chars; 
  char last_char;
  char * file_type;
  bool oddity_found;

  characters = alnums = alphas = blanks = cntrls = digits = graphs = lowers = prints = puncts = spaces = uppers = xdigits = 0;
  return_chars = newline_chars = ret_new_chars = 0;
  oddity_found = false;

  while ((c = getc(stdin)) != EOF)	// while end-of-file has not been reached
    {      
      if (isalnum(c))
	{ alnums++; } 

      if (isalpha(c))
        { alphas++; }

      if (isdigit(c))
        { digits++; }

      if (isblank(c))
        { blanks++; }

      if (iscntrl(c))
        { cntrls++; }

      if (isgraph(c))
        { graphs++; }

      if (islower(c))
        { lowers++; }

      if (isprint(c))
        { prints++; }

      if (ispunct(c))
        { puncts++; }

      if (isspace(c))
        { spaces++; }

      if (isupper(c))
        { uppers++; }

      if (isxdigit(c))
        { xdigits++; }

      if (c == '\n' && last_char != '\r')	// if the current character is a newline character
        { newline_chars++; }			//  and the last character was not a return character

      if (c == '\n' && last_char == '\r') { 	// if the current character is a newline character 
        ret_new_chars++;			//  and the last character was a return character
        return_chars--;				//  then a return-newline character is present
      }

      if (c == '\r')
        { return_chars++; }

      last_char = c;
      characters++;
    }

  if (ferror(stdin))					// if there was an error with the input
    { report_failure("input terminated on error"); }

  if (characters == 0)
    { report_failure("file is empty"); }

  printf("characters  %8d\n", characters);		// print character counts
  printf("  isalnum   %8d\n", alnums);
  printf("  isalpha   %8d\n", alphas);
  printf("  isdigit   %8d\n", digits);
  printf("  isgraph   %8d\n", graphs);
  printf("  isprint   %8d\n", prints);
  printf("  iscntrl   %8d\n", cntrls);
  printf("  islower   %8d\n", lowers);
  printf("  isupper   %8d\n", uppers);
  printf("  ispunct   %8d\n", puncts);
  printf("  isblank   %8d\n", blanks);
  printf("  isspace   %8d\n", spaces);
  printf("  isxdigit  %8d\n", xdigits);
  printf("end of line?\n");
  printf("  return          %8d\n", return_chars);
  printf("  newline         %8d\n", newline_chars);
  printf("  return-newline  %8d\n", ret_new_chars);

  printf("oddities?\n");

  if (last_char != '\n') {					// if last char was not a newline
     printf("  file does not end with a newline character\n"); 
     oddity_found = true;
  }
  
  if (alnums != alphas + digits) {
     printf("  alphanumerics don't add up\n"); 
     oddity_found = true;
  }

  if (!oddity_found)
   { printf("  none detected\n");  }

  printf("file type?\n");

  if (characters != prints + cntrls) {
     file_type = "not a text file";
  } else if (return_chars == 0 && newline_chars == 0 && ret_new_chars == 0) { 
     file_type = "strange text file, no CR or LF at all";
  } else if (return_chars == 0 && newline_chars == 0 && ret_new_chars > 0) {
     file_type = "DOS-style text file, only CRLF line terminators";
  } else if (return_chars == 0 && newline_chars > 0 && ret_new_chars == 0) { 
     file_type = "UNIX-style text file, only LF line terminators";
  } else if (return_chars == 0 && newline_chars > 0 && ret_new_chars > 0) { 
     file_type = "mixed UNIX/DOS-style text file, both CRLF and LF line terminators";
  } else if (return_chars > 0 && newline_chars == 0 && ret_new_chars == 0) { 
     file_type = "MacOS-style text file, only CR line terminators";
  } else if (return_chars > 0 && newline_chars == 0 && ret_new_chars > 0) { 
     file_type = "MacOS/DOS-style text file, both CRLF and CR line terminators";
  } else if (return_chars > 0 && newline_chars > 0 && ret_new_chars == 0) { 
     file_type = "MacOS/UNIX-style text file, both CR and LF line terminators";
  } else if (return_chars > 0 && newline_chars > 0 && ret_new_chars > 0) { 
     file_type = "MacOS/UNIX/DOS-style text file, CRLF, CR and LF liner terminators";
  }

  printf("  %s\n", file_type);

  return 0;
}

