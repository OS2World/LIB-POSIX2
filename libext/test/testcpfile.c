/**** testcpfile.c: _cpFile(), symlink() **** */
#include <stdlib.h>
#include <stdio.h>
#include <errno.h> 
#include <unistd.h>
#include <sys/param.h>


int main(int argc, char *argv[])
{
	if (argc != 3) {
		puts("Usage: testcpfile INFILE OUTFILE");
		return(1);
	}
	if ((int)sizeof(argv[1]) > PATH_MAX || (int)sizeof(argv[2]) > PATH_MAX) {
		printf("%s: Pathname too long! Longer than <%d> byte.",
			argv[0], PATH_MAX);
		return(2);
	}
	errno = 0; 		/* not crucial here, but never a bad idea */
	
	if ( _cpFile(argv[1], argv[2]) ) {
		fprintf(stderr,
"%s: [%s] [%s]: Errorlevel: 2\n", argv[0], argv[1], argv[2]);
  		perror("symlink()"); return(2);
	}
	if ( symlink(argv[2], "symlnk.tst") ) {
		fprintf(stderr, 
"%s: <%s> (%ld byte long, PATH_MAX: %d) <symlnk.tst>: Errorlevel: 3\n", 
       	        argv[0], argv[2], sizeof(argv[2]), PATH_MAX);
		if (errno) {
	  		perror("symlink()");
			return(3);
		}
	}
	return(0);
}
