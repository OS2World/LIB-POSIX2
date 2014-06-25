#include <stdlib.h>
#include <stdio.h>
#include <sys/shm.h>

int main(int argc, char *argv[])
{
  int shmid;
  int pid;
  char *shmptr;

  if ((shmid = shmget(IPC_PRIVATE, 1000, SHM_R | SHM_W)) < 0)
    fprintf(stderr, "shmget() failed\n");

  if ((shmptr = shmat(shmid, 0, 0)) == NULL)
    fprintf(stderr, "shmat() failed\n");

  sprintf(shmptr, "process %d: shmid %d address %lx\n", getpid(), shmid, shmptr);

  pid = fork();
  if (pid > 0) {
      sleep(10);
  }
  else if (pid == 0) {
      shmptr = shmat(shmid, 0, 0);
      printf("child: %s", shmptr);
      exit(1);
  }
  else fprintf(stderr, "fork() failed\n");

  if (shmctl(shmid, IPC_RMID, 0) < 0)
    fprintf(stderr, "shmctl() failed\n");

  return 0;
}
