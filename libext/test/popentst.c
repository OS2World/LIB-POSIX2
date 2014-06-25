/* popentst.c (libext + gcc) */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

	int
main(void)
{
  FILE *fp1, *fp2, *fp3;
  int c;

  __popen_LLprint();
  fp1 = popen("\"gcc --version 2>NUL\"", "r");
  __popen_LLprint();
  fp2 = popen("\" uname -a \" 2>NUL", "r");
  __popen_LLprint();
  fp3 = popen(" link386/? 2>NUL", "r");
  __popen_LLprint();
  puts("Now printing pipe output. Please wait...");
  sleep(5);

  while ((c = getc(fp1)) != EOF)
    printf("%c", c);

  while ((c = getc(fp2)) != EOF)
    printf("%c", c);

  while ((c = getc(fp3)) != EOF)
    printf("%c", c);

  __popen_LLprint();
  pclose(fp1);
  __popen_LLprint();
  pclose(fp2);
  __popen_LLprint();
  pclose(fp3);
  __popen_LLprint();


  return(0);
}
