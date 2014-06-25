#include <stdlib.h>
#include <stdio.h>


int main(int argc, char *argv[]) {

  int rc;
  char *oldn, *newn;

  if (argc!=3) {
     fprintf(stderr, "renametest: wrong args\n");
     fprintf(stderr, "%s: <oldname> <newname>\n", argv[0]);
     exit(1); 
  }
 
 oldn=argv[1];
 newn=argv[2];
 rc=rename(oldn, newn);

 fprintf(stderr, "rc(%s, %s) =%i\n", oldn, newn, rc);
 if (rc) {
   perror("rename() failed");
 }
  
 exit(rc);
}
