/*	$Id: util.c,v 1.2 2000/08/21 13:37:23 amai Exp $	*/

#ifndef lint
static char rcsid[] = "$OpenBSD: util.c,v 1.6 1999/01/11 00:16:32 marc Exp $";
#endif /* not lint */

#include "EXTERN.h"
#include "common.h"
#include <stdarg.h>
#include "INTERN.h"
#include "util.h"
#include "backupfile.h"

/* Rename a file, copying it if necessary. */

int
move_file(from,to)
const char *from, *to;
{
    char bakname[MAXPATHLEN];
    Reg1 char *s;
    Reg2 int i;
    Reg3 int fromfd;

    /* to stdout? */

    if (strEQ(to, "-")) {
#ifdef DEBUGGING
	if (debug & 4)
	    say2("Moving %s to stdout.\n", from);
#endif
	fromfd = open(from, O_RDONLY);
	if (fromfd < 0)
	    pfatal2("internal error, can't reopen %s", from);
	while ((i=read(fromfd, buf, sizeof buf)) > 0)
	    if (write(1, buf, (size_t)i) != 1)
		pfatal1("write failed");
	Close(fromfd);
	return 0;
    }

    if (origprae) {
	Strcpy(bakname, origprae);
	Strcat(bakname, to);
    } else {
#ifndef NODIR
	char *backupname = find_backup_file_name(to);
	if (backupname == (char *) 0)
	    fatal1("out of memory\n");
	Strcpy(bakname, backupname);
	free(backupname);
#else /* NODIR */
	Strcpy(bakname, to);
    	Strcat(bakname, simple_backup_suffix);
#endif /* NODIR */
    }

    if (stat(to, &filestat) == 0) {	/* output file exists */
	dev_t to_device = filestat.st_dev;
	ino_t to_inode  = filestat.st_ino;
	char *simplename = bakname;
	
	for (s=bakname; *s; s++) {
	    if (*s == '/')
		simplename = s+1;
	}
	/* Find a backup name that is not the same file.
	   Change the first lowercase char into uppercase;
	   if that isn't sufficient, chop off the first char and try again.  */
	while (stat(bakname, &filestat) == 0 &&
		to_device == filestat.st_dev && to_inode == filestat.st_ino) {
	    /* Skip initial non-lowercase chars.  */
	    for (s=simplename; *s && !islower(*s); s++) ;
	    if (*s)
		*s = toupper(*s);
	    else
		Strcpy(simplename, simplename+1);
	}
	while (unlink(bakname) >= 0) ;	/* while() is for benefit of Eunice */
#ifdef DEBUGGING
	if (debug & 4)
	    say3("Moving %s to %s.\n", to, bakname);
#endif
	if (link(to, bakname) < 0) {
	    /* Maybe `to' is a symlink into a different file system.
	       Copying replaces the symlink with a file; using rename
	       would be better.  */
	    Reg4 int tofd;
	    Reg5 int bakfd;

	    bakfd = creat(bakname, 0666);
	    if (bakfd < 0) {
		say4("Can't backup %s, output is in %s: %s\n", to, from,
		     strerror(errno));
		return -1;
	    }
	    tofd = open(to, O_RDONLY);
	    if (tofd < 0)
		pfatal2("internal error, can't open %s", to);
	    while ((i=read(tofd, buf, sizeof buf)) > 0)
		if (write(bakfd, buf, (size_t)i) != i)
		    pfatal1("write failed");
	    Close(tofd);
	    Close(bakfd);
	}
	while (unlink(to) >= 0) ;
    }
#ifdef DEBUGGING
    if (debug & 4)
	say3("Moving %s to %s.\n", from, to);
#endif
    if (link(from, to) < 0) {		/* different file system? */
	Reg4 int tofd;
	
	tofd = creat(to, 0666);
	if (tofd < 0) {
	    say4("Can't create %s, output is in %s: %s\n",
	      to, from, strerror(errno));
	    return -1;
	}
	fromfd = open(from, O_RDONLY);
	if (fromfd < 0)
	    pfatal2("internal error, can't reopen %s", from);
	while ((i=read(fromfd, buf, sizeof buf)) > 0)
	    if (write(tofd, buf, (size_t)i) != i)
		pfatal1("write failed");
	Close(fromfd);
	Close(tofd);
    }
    Unlink(from);
    return 0;
}

/* Copy a file. */

void
copy_file(const char *from,const char *to)
{
    Reg3 int tofd;
    Reg2 int fromfd;
    Reg1 int i;
    
    tofd = creat(to, 0666);
    if (tofd < 0)
	pfatal2("can't create %s", to);
    fromfd = open(from, O_RDONLY);
    if (fromfd < 0)
	pfatal2("internal error, can't reopen %s", from);
    while ((i=read(fromfd, buf, sizeof buf)) > 0)
	if (write(tofd, buf, (size_t)i) != i)
	    pfatal2("write to %s failed", to);
    Close(fromfd);
    Close(tofd);
}

/* Allocate a unique area for a string. */

char *
savestr(s)
Reg1 const char *s;
{
    Reg3 char *rv;
    Reg2 char *t;

    if (!s)
	s = "Oops";
    t = (char*)s;
    while (*t++);
    rv = malloc((MEM) (t - s));
    if (rv == Nullch) {
	if (using_plan_a)
	    out_of_mem = TRUE;
	else
	    fatal1("out of memory\n");
    }
    else {
	t = rv;
	while ((*t++ = *s++))
	    ;
    }
    return rv;
}

/* Vanilla terminal output (buffered). */

void
say(const char *pat, ...)
{
  va_list arg_ptr;

  va_start (arg_ptr, pat);
  vfprintf (stderr, pat, arg_ptr);
  va_end (arg_ptr);
  fputc ('\n', stderr);

    Fflush(stderr);
}

/* Terminal output, pun intended. */

void				/* very void */
fatal(const char*pat, ...)
{
  va_list arg_ptr;

  va_start (arg_ptr, pat);
    fprintf(stderr, "patch: **** ");
  vfprintf(stderr, pat, arg_ptr);
  va_end (arg_ptr);
  fputc ('\n', stderr);

    my_exit(1);
}

/* Say something from patch, something from the system, then silence . . . */

void				/* very void */
pfatal(const char* pat, ...)
{
    int errnum = errno;
  va_list arg_ptr;

  va_start (arg_ptr, pat);
    fprintf(stderr, "patch: **** ");
  vfprintf(stderr, pat, arg_ptr);
  va_end (arg_ptr);
    fprintf(stderr, ": %s\n", strerror(errnum));
    my_exit(1);
}

/* Get a response from the user, somehow or other. */

void
ask(const char* pat,...)
{
    int ttyfd;
    int r;
    bool tty2 = isatty(2);
  va_list arg_ptr;

  va_start (arg_ptr, pat);

  vsnprintf(buf, sizeof buf, pat, arg_ptr);
  va_end (arg_ptr);
  fputc ('\n', stderr);
    Fflush(stderr);
    write(2, buf, strlen(buf));
    if (tty2) {				/* might be redirected to a file */
	r = read(2, buf, sizeof buf);
    }
    else if (isatty(1)) {		/* this may be new file output */
	Fflush(stdout);
	write(1, buf, strlen(buf));
	r = read(1, buf, sizeof buf);
    }
    else if ((ttyfd = open(_PATH_TTY, O_RDWR)) >= 0 && isatty(ttyfd)) {
					/* might be deleted or unwriteable */
	write(ttyfd, buf, strlen(buf));
	r = read(ttyfd, buf, sizeof buf);
	Close(ttyfd);
    }
    else if (isatty(0)) {		/* this is probably patch input */
	Fflush(stdin);
	write(0, buf, strlen(buf));
	r = read(0, buf, sizeof buf);
    }
    else {				/* no terminal at all--default it */
	buf[0] = '\n';
	r = 1;
    }
    if (r <= 0)
	buf[0] = 0;
    else
	buf[r] = '\0';
    if (!tty2)
	say1(buf);
}

/* How to handle certain events when not in a critical region. */

void
set_signals(reset)
int reset;
{
    static sig_t hupval, intval;

    if (!reset) {
	hupval = signal(SIGHUP, SIG_IGN);
	if (hupval != SIG_IGN)
	    hupval = (sig_t)my_exit;
	intval = signal(SIGINT, SIG_IGN);
	if (intval != SIG_IGN)
	    intval = (sig_t)my_exit;
    }
    Signal(SIGHUP, hupval);
    Signal(SIGINT, intval);
}

/* How to handle certain events when in a critical region. */

void
ignore_signals()
{
    Signal(SIGHUP, SIG_IGN);
    Signal(SIGINT, SIG_IGN);
}

/* Make sure we'll have the directories to create a file.
   If `striplast' is TRUE, ignore the last element of `filename'.  */

void
makedirs(filename,striplast)
Reg1 const char *filename;
bool striplast;
{
    char tmpbuf[256];
    Reg2 char *s = tmpbuf;
    char *dirv[20];		/* Point to the NULs between elements.  */
    Reg3 int i;
    Reg4 int dirvp = 0;		/* Number of finished entries in dirv. */

    /* Copy `filename' into `tmpbuf' with a NUL instead of a slash
       between the directories.  */
    while (*filename) {
	if (*filename == '/') {
	    filename++;
	    dirv[dirvp++] = s;
	    *s++ = '\0';
	}
	else {
	    *s++ = *filename++;
	}
    }
    *s = '\0';
    dirv[dirvp] = s;
    if (striplast)
	dirvp--;
    if (dirvp < 0)
	return;

    strcpy(buf, "mkdir");
    s = buf;
    for (i=0; i<=dirvp; i++) {
	struct stat sbuf;

	if (stat(tmpbuf, &sbuf) && errno == ENOENT) {
	    while (*s) s++;
	    *s++ = ' ';
	    strcpy(s, tmpbuf);
	}
	*dirv[i] = '/';
    }
    if (s != buf)
	system(buf);
}

/* Make filenames more reasonable. */

char *
fetchname(at,strip_leading,assume_exists)
const char *at;
int strip_leading;
int assume_exists;
{
    char *fullname;
    char *name;
    Reg1 char *t;
    char tmpbuf[200];
    int sleading = strip_leading;

    if (!at || *at == '\0')
	return Nullch;
    while (isspace(*at))
	at++;
#ifdef DEBUGGING
    if (debug & 128)
	say4("fetchname %s %d %d\n",at,strip_leading,assume_exists);
#endif
    if (strnEQ(at, "/dev/null", 9))	/* so files can be created by diffing */
	return Nullch;			/*   against /dev/null. */
    name = fullname = t = savestr(at);

    /* Strip off up to `sleading' leading slashes and null terminate.  */
    for (; *t && !isspace(*t); t++)
	if (*t == '/')
	    if (--sleading >= 0)
		name = t+1;
    *t = '\0';

    /* If no -p option was given (957 is the default value!),
       we were given a relative pathname,
       and the leading directories that we just stripped off all exist,
       put them back on.  */
    if (strip_leading == 957 && name != fullname && *fullname != '/') {
	name[-1] = '\0';
	if (stat(fullname, &filestat) == 0 && S_ISDIR (filestat.st_mode)) {
	    name[-1] = '/';
	    name=fullname;
	}
    }

    name = savestr(name);
    free(fullname);

    if (stat(name, &filestat) && !assume_exists) {
	char *filebase = basename(name);
	char *filedir = dirname(name);

#define try(f, a1, a2, a3) (Snprintf(tmpbuf, sizeof tmpbuf, f, a1, a2, a3), stat(tmpbuf, &filestat) == 0)
	if (   try("%s/RCS/%s%s", filedir, filebase, RCSSUFFIX)
	    || try("%s/RCS/%s%s", filedir, filebase,        "")
	    || try(    "%s/%s%s", filedir, filebase, RCSSUFFIX)
	    || try("%s/SCCS/%s%s", filedir, SCCSPREFIX, filebase)
	    || try(     "%s/%s%s", filedir, SCCSPREFIX, filebase))
	  return name;
	free(name);
	name = Nullch;
    }

    return name;
}
