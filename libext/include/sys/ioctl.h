/* ioctl.h (emx/libext+gcc): $Id: ioctl.h,v 1.2 2000/04/18 12:01:52 veit Exp $ */

#ifndef _SYS_IOCTL_H
#define _SYS_IOCTL_H

#if defined (__cplusplus)
extern "C" {
#endif

#if defined (_EMX_TCPIP)
#include <sys/so_ioctl.h>
#endif

#if !defined (TCGETA)
#define TCGETA      1
#define TCSETA      2
#define TCSETAW     3
#define TCSETAF     4
#define TCFLSH      5
#define TCSBRK      6
#define TCXONC      7
#endif

#define _TCGA       8           /* Used internally for tcgetattr() */
#define _TCSANOW    9           /* Used internally for tcsetattr() */
#define _TCSADRAIN  10          /* Used internally for tcsetattr() */
#define _TCSAFLUSH  11          /* Used internally for tcsetattr() */

#if !defined (FIONREAD)
#define FIONREAD   16
#endif

#if !defined (FGETHTYPE)
#define FGETHTYPE  32
#endif

#if !defined (HT_FILE)
#define HT_FILE         0
#define HT_UPIPE        1
#define HT_NPIPE        2
#define HT_DEV_OTHER    3
#define HT_DEV_NUL      4
#define HT_DEV_CON      5
#define HT_DEV_CLK      7
#define HT_SOCKET       8
#define HT_ISDEV(n)     ((n) >= HT_DEV_OTHER && (n) <= HT_DEV_CLK)
#endif

/* Window/terminal size structure. */
struct winsize {
	unsigned short	ws_row;		/* rows, in characters */
	unsigned short	ws_col;		/* columns, in characters */
	unsigned short	ws_xpixel;	/* horizontal size, pixels */
	unsigned short	ws_ypixel;	/* vertical size, pixels */
};
#define TIOCGWINSZ	0x66	/* (struct pt_winsize) get winsize */

int ioctl (int, int, ...);

int _ioctl (int, int, ...);

#if defined (__cplusplus)
}
#endif

#endif /* not _SYS_IOCTL_H */
