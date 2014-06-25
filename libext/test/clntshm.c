#include <stdlib.h>
#include <stdio.h>
#include <sys/shm.h>

int main(int argc, char *argv[])
{
  int shmid;
  char *shmptr;
  int i;

  if (argc != 2) exit(1);


  while (1) {
  i = atoi(argv[1]);
  if ((shmptr = shmat(i, 0, 0)) == NULL)
    fprintf(stderr, "shmat() failed\n");

  printf("running...\n");

  for (i=0;i<10;i++) {
  printf("process %d: shmid %d address %lx\n", getpid(), atoi(argv[1]), shmptr);
  printf("%s", shmptr);
  sleep(3);
  }

  shmdt(shmptr);
  }

  return 0;
}
