/* $Id: stat.c,v 1.2 2000/08/15 19:04:12 amai Exp $ */
/* test facility for stat() call */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/param.h>


int main(int argc, char *argv[]) {

  struct stat buf;
  char filename[MAXPATHLEN];
  FILE *outfile=stdout;

  if (argc != 2) {
     fprintf(outfile,"stat: <infile>\n");
     exit(1);
  }
  strncpy(filename, argv[1], sizeof(filename));


  if (stat(argv[1], &buf)) {
     perror("failure upon stat'ing()");
     exit(2);
}

/* Now check for available file types */

#ifdef S_ISREG
  if (S_ISREG(buf.st_mode))
	    fprintf(outfile,"%s is regular file \n",filename);
  else
#endif
#ifdef S_ISLNK
  if (S_ISLNK(buf.st_mode))
	    fprintf(outfile,"%s is link \n",filename);
  else
#endif
#ifdef S_ISDIR
  if(S_ISDIR(buf.st_mode))
	    fprintf(outfile,"%s is directory \n",filename);
  else
#endif
#ifdef S_ISCHR
  if (S_ISCHR(buf.st_mode))
	    fprintf(outfile,"%s is character device \n",filename);
  else
#endif
#ifdef S_ISBLK
  if (S_ISBLK(buf.st_mode))
	    fprintf(outfile,"%s is block device \n",filename);
  else
#endif
#ifdef S_ISFIFO
  if (S_ISFIFO(buf.st_mode))
	    fprintf(outfile,"%s is FIFO \n",filename);
  else
#endif
#ifdef S_ISSOCK
  if (S_ISSOCK(buf.st_mode))
      fprintf(outfile,"%s is socket \n",filename);
  else
#endif

      fprintf(outfile,"%s is unknown filetype: st_mode=%li \n",filename, (long)buf.st_mode);

 
 exit(0);
}
