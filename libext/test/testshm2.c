#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>

/* test segment leakage */
int main(void)
{
  int shmid, i;
  void *addr;
  struct shmid_ds buf;

	__SHMx_shmInit();
  shmid = shmget(IPC_PRIVATE, 1024, 0777);
  addr = shmat(shmid, 0, 0);

  if (shmid < 0 || !addr) {
	perror("testshm2: Error attaching segment");
	_SHMx_showActiveSeg();
	return(1);
  }
 for (i = 1; i < 1500; i++) {
      shmid = shmget(IPC_PRIVATE, 1024, 0777);
      if (shmid >= 0) {
	  addr = shmat(shmid, 0, 0);
		if (!addr)
			{perror("testshm2: testshm2: Error attaching segment");exit(3);}
		if (shmdt(addr))
			{perror("testshm2: testshm2: Error detaching segment");exit(4);}
		if (shmctl(shmid, IPC_RMID, &buf))
			{perror("testshm2: testshm2: Error removing segment");exit(5);}
	}
			else {
	  printf("testshm2: shmget() Error...(i = %d) wait for 10sec\n", i);
			_SHMx_showActiveSeg();
	  perror("testshm2: testshm2: Error");
	  sleep(5);
	  exit(2);
    }
 }
 _SHMx_showActiveSeg();
 __SHMx_shmDone();
  exit(0);
}

