#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

int main(void)
{
  _SHMx_showActiveSeg();
  exit(0);
}

