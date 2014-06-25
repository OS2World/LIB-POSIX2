/* $Id: mmaptest.c,v 1.7 2000/10/11 14:48:56 arnd Exp $
 * Written by Holger Veit, released into the public domain

 * Note: For testing purposes we pass a constant address instead of a buffer;
 * this is not correct, because the prototype reads:
 *
 * void *
 * mmap(void *addr, size_t len, int prot, int flags, int fd, off_t offset)
 *
 * not: 'const void *addr'.
 ***/

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/errno.h>
#include <signal.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <limits.h>	/* for PAGESIZE */

#define ROUNDS 	3	/* number of maps/unmaps */

static const char* FN = "mmaptest.tst";
static const char TXT[11]= {0xD,0xE,0xA,0xD,0xB,0xE,0xE,0xF,'\r','\n', '\0'};

static int more_tests(void);
static int SigsegvAfterMprotect(void);

static void print_signal(int sig)
{
	if (sig)
		fprintf(stderr, "\n *** !! Caught signal #%d <%s>!! Exiting ...\n", sig,
			strsignal(sig));
	exit(99);
}

static void print_error(void)
{
	if (errno)
		perror("  Check errorlevel! Test ended with errno content ");
}

int
main(void)
{
	int fd, i, rc;
	char buf[10001], c;
	char *p[ROUNDS];
	FILE *fp;

	atexit(print_error);
	signal(SIGSEGV, print_signal);
	/* create a file */
	if ((fd=open(FN,O_RDWR|O_CREAT,0666)) < 0) {
		perror("Create file");
		exit(1);
	}
	/* fill it with lines of 0xDEADBEEF*/
	buf[0] = 0;
	for (i=0; i<10000; i+=10)
		strcat(buf+i, TXT);

	if (write(fd,buf,10001) != 10001) {
		perror("Write file");
		exit(2);
	}
	close(fd); 	/* close the file */
#if 0
	fp = fopen(FN, "rb");
	while (EOF != (c = getc(fp)))
		putchar(c);
	fclose(fp); 	/* close the file */
#endif
	/* now open the file for reading and try a shared map write */
	if ((fd=open(FN,O_RDONLY)) < 0) {
		perror("Open test1");
		exit(3);
	}
	for (i = 0; i < ROUNDS; ++i) {
		fprintf(stderr,
"MAP_SHARED-test round #<%d> on fd <%d> *****\n", i, fd);

		p[i] = mmap(0, 5000, PROT_READ, MAP_SHARED, fd, 0);
		if (p[i] != MAP_FAILED) {
			fprintf(stderr, "Test 0 ok: Addr: <%p>\n", p[i]);
	} else {
			perror("Test 0 FAILED -- MAP_SHARED");
		exit(4);
	}
		p[i] = mmap(0, 5000, PROT_WRITE, MAP_SHARED, fd, 0);
		if (p[i] != MAP_FAILED) {
			fprintf(stderr,"Test 0 ok: Addr: <%p>\n", p[i]);
		} else {
			perror("Test 0 FAILED -- MAP_SHARED");
			exit(4);
		}
		/* unmap the region */
		rc = munmap(p[i], 5000);
		if (rc != -1) {
			fprintf(stderr,
			"Test 4 (Unmap the file from memory) at <%p> ok\n", p[i]);
		} else {
			fprintf(stderr,
			"Test 4 FAILED: cannot unmap the region at <%p>!! \n", p[i]);
			perror("munmap(): Error message");
			exit(8);
		}
		/* try second unmap to the shared region */
		rc = munmap(p[i], 5000);
		if (rc) {
			perror("munmap()(cannot unmap twice): Error message");
			fprintf(stderr,"Test 4 at <%p> ok\n", p[i]);
		} else {
			fprintf(stderr,
			"Test 4 FAILED: Allowed double unmap at <%p>!! \n", p[i]);
			exit(8);
		}
	}
	sleep(5);

	for (i=0; i<ROUNDS; ++i, fd = dup(fd)) {
		fprintf(stderr,
"******* mmap()-test round #<%d> on fd <%d> *****\n", i, fd);
	/* try to use MAP_FIXED (unsupported) */
		p[i] = mmap((char*)(440*1024*1024), 5000, PROT_READ, MAP_FIXED|MAP_SHARED,
							fd, 0);
		if (p[i] == MAP_FAILED) 
			perror(
			"Test 1 ok (unsupported MAP_FIXED flag): Return ENOTSUP\n");
		 else {
			fprintf(stderr,
			"Test 1 FAILED: allowed MAP_FIXED flag, returns <%p>\n",p[i]);
		exit(5);
	}

	/* ok, then try to read */
		p[i] = mmap(0, 5000, PROT_READ, MAP_PRIVATE, fd, 0);
		if (p[i] != MAP_FAILED) {
		fprintf(stderr,
		 "Test 2 (map for reading) ok, at addr=<%p>\n",p[i]);
	} else {
			perror("Test 2 FAILED: didn't allow to map R/O file, errno");
		exit(6);
	}
	/* check some data */
		/* at Offset 2021, there should be a "0xDEADBEEF" */
		if (strncmp(p[i] + 1010, TXT, 10) == 0) {
			fprintf(stderr,"Test 3 (Data check) ok\n");
	} else {
			fprintf(stderr,
			"Test 3 FAILED: expected was \"0xDEADBEEF\", found was \"");
			for (i=0; i<8; i++)
				putchar(*(p[i] + 1011 + i));
			fprintf(stderr,"\"\n");
		perror("strncmp(): Error message");
		exit(7);
	}}
	fprintf(stderr,
"******* mmap()-test round #<%d> completed, sleeping...\n\n", i-1);
	sleep(5);
	for (i=0; i<ROUNDS; ++i) {
		fprintf(stderr,
"******* unmmap()-test round #<%d> *****\n", i);

	/* unmap the region */
	if (!munmap(p[i], 5000)) {
		fprintf(stderr,"Test 4 (Unmap the file from memory) at <%p> ok\n", p[i]);
	} else {
		fprintf(stderr,
		"Test 4 FAILED: cannot unmap the region at <%p>!! \n", p[i]);
		perror("munmap(): Error message");
		exit(8);
	}
	/* look whether you can unmap a region which is already unmapped */
		rc = munmap(p[i],5000);
	if (rc == -1) {
			fprintf(stderr,"Test 5 (Double unmap not allowed) ok\n");
		perror("munmap(): Returns error message");
	} else {
			fprintf(stderr,
			"Test 5 FAILED: apparently can unmap an already unmapped region\n");
		perror("munmap(): Error message");
		return(1);
	}
	}
	/* close the file */
	close(fd);
	return more_tests();
	}

/* Modified mmap test -- only tests if MAP_PRIVATE works 
 */

/* Thanks to Mike Haertel and Jim Avera for this test.
   Here is a matrix of mmap possibilities:
	mmap private not fixed
	mmap private fixed at somewhere currently unmapped
	mmap private fixed at somewhere already mapped
	mmap shared not fixed
	mmap shared fixed at somewhere currently unmapped
	mmap shared fixed at somewhere already mapped
   For private mappings, we should verify that changes cannot be read()
   back from the file, nor mmap's back from the file at a different
   address.  (There have been systems where private was not correctly
   implemented like the infamous i386 svr4.0, and systems where the
   VM page cache was not coherent with the filesystem buffer cache
   like early versions of FreeBSD and possibly contemporary NetBSD.)
   For shared mappings, we should conversely verify that changes get
   propogated back to all the places they're supposed to be.

   Grep wants private fixed already mapped.
   The main things grep needs to know about mmap are:
   * does it exist and is it safe to write into the mmap'd area
   * how to use it (BSD variants)  */


static int more_tests(void)
{
char *data, *data2, *data3;
int pagesize, i;
int fd;

	pagesize = getpagesize();
	/*
	 * First, make a file with some known garbage in it.
	 */
	data = malloc((size_t)pagesize);
	if (!data)
		exit(99);
	for (i = 0; i < pagesize; ++i)
		*(data + i) = rand();
	umask(0);
	fd = creat(FN, 0600|O_BINARY);
	if (fd < 0)
		exit(8);
	if (write(fd, data, (size_t)pagesize) != pagesize)
		exit(9);
	close(fd);

	/*
	 * Next, try to mmap the file at a fixed address which
	 * already has something else allocated at it.  If we can,
	 * also make sure that we see the same garbage.
	 */
	fd = open(FN, O_RDWR|O_BINARY);
	if (fd < 0)
		exit(10);
	if ((data2 = mmap(0, (size_t)pagesize, PROT_READ | PROT_WRITE,
		     MAP_PRIVATE, fd, 0L)) == (char*)-1) {
		perror("mmap(): Error message");
		exit(11);
	}

	for (i = 0; i < pagesize; ++i)
		if (*(data + i) != *(data2 + i)) {
			fprintf(stderr, "Test for %d elements failed at position %d:\n\
Original: <%c>    --     Mapped: <%c>\n", pagesize, i, *(data + i), 
			  *(data2 + i));
			perror("mmap(): Error message");
			exit(12);
		}
	fprintf(stderr, "Test 6 ok\n");
		perror("mmap(): Error message");
	/*
	 * Finally, make sure that changes to the mapped area
	 * do not percolate back to the file as seen by read().
	 * (This is a bug on some variants of i386 svr4.0.)
	 */

	for (i = 0; i < pagesize; ++i)
		*(data2 + i) = *(data2 + i) + 1;
	data3 = (char*)malloc((size_t)pagesize);
	if (!data3)
		exit(99);
	i = read(fd, data3, (size_t)pagesize);
	if ( i != pagesize ) {
		fprintf(stderr, 
		"Test if file was influenced by changes of mapped areas \
has failed!\n Original size: <%d>    --     File size: <%d>\n", 
				pagesize, i);
		perror("read(): Error message");
		exit(13);
	}

	for (i = 0; i < pagesize; ++i)
		if (*(data + i) != *(data3 + i))
			exit(14);

 	if (close(fd))
 		perror("close()");
	if (unlink(FN))
		perror("unlink(): Errno content is");
	fprintf(stderr,"Test 7 ok: \n");

	return SigsegvAfterMprotect();
}

	static int 
SigsegvAfterMprotect(void)
{
 char c;
    /* Allocate a buffer; it will have the default
       protection of PROT_READ|PROT_WRITE. */
 char *p = 
    malloc(1024+PAGESIZE-1);
    if (!p) {
        perror("Couldn't malloc(1024)");
        exit(errno);
    }
    fprintf(stderr, 
    "Test 8:\nPerforming test #8: mprotect() a memory area as readonly.\n\
Beware! Should crash (SIGSEGV) upon attempt to write to protected area...\n");

    /* Align to a multiple of PAGESIZE, assumed to be a power of two */
    p = (char *)(((int) p + PAGESIZE-1) & ~(PAGESIZE-1));

    c = p[666];         /* Read; ok */
    p[666] = 42;        /* Write; ok */

    /* Mark the buffer read-only. */
    if (mprotect(p, 1024, PROT_READ)) {
        perror("Couldn't mprotect");
        exit(errno);
    }
    fprintf(stderr, "Protected successfully as readonly.\n");

    c = p[666];             fprintf(stderr, "Read; ok!\n");
    fprintf(stderr, 
    "Write; If working correctly, program must die on SIGSEGV now! ...\n");
    p[666] = 42;            
    return(0);
}

