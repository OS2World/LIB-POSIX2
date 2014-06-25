#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>


int
main(void)
{
 int err, fd = creat("fchmod.tst", O_RDWR);
	if (fd < 0) {
		perror("statxtest: creat() error");
		return(2);
	}
	err = fchmod(fd, S_IREAD);
	if (err < 0) {
		perror("statxtest: fchmod() error");
		return(3);
	}
	err = close(fd);
	if (err < 0) {
		perror("statxtest: close() error");
		return(4);
	} else {
	 struct stat buf;
		err = lstat("fchmod.tst", &buf);
		if (err < 0) {
			perror("statxtest: fchmod() error");
			return(5);
		}
	}
	return(0);
}
