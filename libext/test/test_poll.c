/*
   $Id: test_poll.c,v 1.3 2000/08/02 21:16:07 amai Exp $ 

   A very poor testing facility for poll(2) ...
   This thing needs to be enhanced to include some more
   interesting test examples!!

   This is Public Domain
   written in 2000 by
   Alexander Mai
   st002279@hrzpub.tu-darmstadt.de 
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <limits.h>

#include <poll.h>

/* prototypes, don't ask why ... */
static void do_poll (struct pollfd ufds[], nfds_t nfds, int timeout);
static void test1 (void);
static void test2 (void);
static char *idev (int ev);


	static void 
do_poll (struct pollfd ufds[], nfds_t nfds, int timeout)
{
  /* do a poll() for a single struct pollfd and 
     be very verbose ;-) */

  unsigned int i = 0;
  /* struct pollfd *ufd; */
  const int pollrc = poll (ufds, nfds, timeout);

  if (pollrc < 0)
    perror ("poll");
  fprintf (stdout, "poll([");
  for (; i < nfds; i++)
    {
      fprintf (stdout, "{%i,%i=%s,%i=%s}",
	       ufds[i].fd,
	       ufds[i].events, idev (ufds[i].events),
	       ufds[i].revents, idev (ufds[i].revents));

    }
  fprintf (stdout, "], %u, %i) = %i\n",
	   nfds, timeout, pollrc);
}


	static void 
test1 (void)
{
  /* test with a single descriptor */

  struct pollfd ufds[1];
  unsigned int nfds = 1;
  int timeout = 5000;
  int filehd;


  ufds[0].fd = 0;
  ufds[0].events = POLLIN;
  do_poll (ufds, nfds, timeout);

  ufds[0].fd = 0;
  ufds[0].events = POLLOUT;
  do_poll (ufds, nfds, timeout);

  ufds[0].fd = 1;
  timeout=0;
  ufds[0].events = POLLIN;
  do_poll (ufds, nfds, timeout);

  ufds[0].fd = 1;
  timeout=20;
  ufds[0].events = POLLOUT;
  do_poll (ufds, nfds, timeout);

  ufds[0].fd = 42;
  ufds[0].events = POLLIN;
  do_poll (ufds, nfds, timeout);

  ufds[0].fd = 42;
  ufds[0].events = POLLOUT;
  do_poll (ufds, nfds, timeout);

  filehd = open ("dummy", O_WRONLY | O_CREAT, S_IREAD | S_IWRITE);
  if (-1 == filehd)
    {
      perror ("open()");
      exit (1);
    }

  ufds[0].fd = filehd;
  ufds[0].events = POLLIN;
  do_poll (ufds, nfds, timeout);

  ufds[0].fd = filehd;
  ufds[0].events = POLLOUT;
  do_poll (ufds, nfds, timeout);

  close (filehd);

  ufds[0].fd = filehd;
  ufds[0].events = POLLIN;
  do_poll (ufds, nfds, timeout);

  ufds[0].fd = filehd;
  ufds[0].events = POLLOUT;
  do_poll (ufds, nfds, timeout);

  return;

}


	static void 
test2 (void)
{

  /* test with two descriptors */

  struct pollfd ufds[2];
  unsigned int nfds = 2;
  int timeout = 3000;
  /* int filehd; */

  ufds[0].fd = 0;
  ufds[0].events = POLLIN;
  ufds[1].fd = 1;
  ufds[1].events = POLLIN;
  do_poll (ufds, nfds, timeout);

  ufds[0].fd = 0;
  ufds[0].events = POLLIN;
  ufds[1].fd = 42;
  ufds[1].events = POLLIN;
  do_poll (ufds, nfds, timeout);

}


	static char *
idev (int ev)
{

  char *str;
  char *ptr;

#define pre ""
#define suf ","

  str = (char *) malloc (1024);	/* a "well-defined" memory leak; no a static
				   string doesn't help either */
  strcpy (str, ",");
  if (ev & POLLIN)
    {
      strcat (str, pre);
      strcat (str, "POLLIN");
      strcat (str, suf);
    }
  if (ev & POLLPRI)
    {
      strcat (str, pre);
      strcat (str, "POLLPRI");
      strcat (str, suf);
    }
  if (ev & POLLOUT)
    {
      strcat (str, pre);
      strcat (str, "POLLOUT");
      strcat (str, suf);
    }
  if (ev & POLLERR)
    {
      strcat (str, pre);
      strcat (str, "POLLIN");
      strcat (str, suf);
    }
  if (ev & POLLHUP)
    {
      strcat (str, pre);
      strcat (str, "POLLIN");
      strcat (str, suf);
    }
  if (ev & POLLNVAL)
    {
      strcat (str, pre);
      strcat (str, "POLLNVAL");
      strcat (str, suf);
    }
  if (ev & POLLRDNORM)
    {
      strcat (str, pre);
      strcat (str, "POLLRDNORM");
      strcat (str, suf);
    }
  if (ev & POLLRDBAND)
    {
      strcat (str, pre);
      strcat (str, "POLLRDBAND");
      strcat (str, suf);
    }
#if 0
  if (ev & POLLWRNORM)
    {				/* same as POLLIN */
      strcat (str, pre);
      strcat (str, "POLLWRNORM");
      strcat (str, suf);
    }
#endif
  if (ev & POLLWRBAND)
    {
      strcat (str, pre);
      strcat (str, "POLLWRBAND");
      strcat (str, suf);
    }

  ptr = &str[strlen (str) - 1];
  if (',' == *ptr)
    *ptr = '\0';
  return (str + 1);
}


int main (void)
{

/* the main program, I suggest to keep it clean and simple
   and just call different functions for each test procedure */

#if 1
  test1 ();
#endif
#if 1
  test2 ();
#endif

  exit (0);

}
