#include <stdio.h>
#include <signal.h>
#include <string.h>

/* NSIG and strsignal() are non-standard. */

int main(void)
{
  int i;
  for (i = 0; i < NSIG; i++)
    {
      printf("%2d %s\n", i, strsignal(i));
    }
  return 0;
}
