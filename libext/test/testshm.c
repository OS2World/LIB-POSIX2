/* testshm.c */
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <stdio.h>

int main(void)
{
  int shmid;
  void *shmptr;
  int i;

	__SHMx_shmInit();

	for (i=0;i<1000;++i) {
		if ((shmid = shmget(IPC_PRIVATE, 1000, SHM_R | SHM_W)) < 0) {
			_SHMx_showActiveSeg();
			fprintf(stderr, "Shmid: %d\n", shmid);
			perror("shmget() failed");
			return(1);
		}
		if ((shmptr = shmat(shmid, 0, 0)) == NULL) {
		_SHMx_showActiveSeg();
			fprintf(stderr, "shmid: %d - address: %p\n", shmid, shmptr);
			perror("shmat() failed");
			return(2);
		}
  shmdt(shmptr);
		if (shmctl(shmid, IPC_RMID, 0) < 0) {
			_SHMx_showActiveSeg();
			fprintf(stderr, "shmid %d address %p\n", shmid, shmptr);
			perror("shmctl() failed");
			return(3);
		}
		printf("shmid: %d address: %p\n", shmid, shmptr);
	}
	_SHMx_showActiveSeg();
	__SHMx_shmDone();
  return 0;
}

