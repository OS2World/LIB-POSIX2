#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/extensions/XShm.h>

int main(int argc, char *argv[])
{
  Display *display;

  int major;
  int minor;
  Bool status;
  Bool pixmaps;
  char *progname;
  char *display_name;

  XShmSegmentInfo shminfo;

  progname = argv[0];

  if ((display_name = getenv("DISPLAY")) == NULL)
    display_name = "localhost:0";

  if ((display = XOpenDisplay(display_name)) == NULL) {
    fprintf(stderr, "%s: Cannot connect to X server %s\n", progname, XDisplayName(display_name));
    exit(-1);
  }

  status = XShmQueryVersion(display, &major, &minor, &pixmaps);
  if (status == True) {
    printf("MIT shared memory extentions (version %d.%d) available,\n");
    if (pixmaps == True)
      printf("shared memory pixmaps supported\n");
    else
      printf("shared memory pixmaps not supported\n");
  } else
    printf("MIT shared memory extentions not available\n");


  shmdt(shminfo.shmaddr);
  shmctl(shminfo.shmid, IPC_RMID, 0);

  XCloseDisplay(display);

  return 0;
}

