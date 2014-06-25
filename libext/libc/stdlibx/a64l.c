/* a64l.c - libExt+gcc
 * Written by J.T. Conklin <jtc@netbsd.org>.
 * Public domain.
 */

#include <stdlib.h>
#include <errno.h>

	extern long
_a64l(const char *s)
{
	long value, digit, shift;
	int i;

	if (s == NULL) {
		errno = EINVAL;
		return(-1L);
	}

	value = 0;
	shift = 0;
	for (i = 0; *s && i < 6; i++, s++) {
		if (*s >= '.' && *s <= '/')
			digit = *s - '.';
		else if (*s >= '0' && *s <= '9')
			digit = *s - '0' + 2;
		else if (*s >= 'A' && *s <= 'Z')
			digit = *s - 'A' + 12;
		else if (*s >= 'a' && *s <= 'z')
			digit = *s - 'a' + 38;
		else {
			errno = EINVAL;
			return(-1L);
		}

		value |= digit << shift;
		shift += 6;
	}

	return(value);
}

	extern long
a64l(const char *s){return _a64l(s);}



	extern char *
_l64a(long value)
{
	if (value < 0) {
		errno = EINVAL; return(NULL);
	} {
	 static char buf[8];
	 char *s = buf;
	 int i = 0;

		while (value != 0 && i < 6) {

		 int digit = value & 0x3f;

			if (digit < 2) 
				*s = digit + '.';
			else if (digit < 12)
				*s = digit + '0' - 2;
			else if (digit < 38)
				*s = digit + 'A' - 12;
			else
				*s = digit + 'a' - 38;

			value >>= 6;
			++s;
			++i;
		}
		*s = '\0';
		return(buf);
	}
}

	extern char *
l64a(long value){return _l64a(value);}
