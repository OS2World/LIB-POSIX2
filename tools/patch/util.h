/*	$Id: util.h,v 1.2 2000/08/21 13:37:23 amai Exp $ */

/* and for those machine that can't handle a variable argument list */

#ifdef CANVARARG

#define say1 say
#define say2 say
#define say3 say
#define say4 say
#define ask1 ask
#define ask2 ask
#define ask3 ask
#define ask4 ask
#define fatal1 fatal
#define fatal2 fatal
#define fatal3 fatal
#define fatal4 fatal
#define pfatal1 pfatal
#define pfatal2 pfatal
#define pfatal3 pfatal
#define pfatal4 pfatal

#else /* hope they allow multi-line macro actual arguments */

    /* if this doesn't work, try defining CANVARARG above */
#define say1(a) say(a, Nullch, Nullch, Nullch)
#define say2(a,b) say(a, b, Nullch, Nullch)
#define say3(a,b,c) say(a, b, c, Nullch)
#define say4 say
#define ask1(a) ask(a, Nullch, Nullch, Nullch)
#define ask2(a,b) ask(a, b, Nullch, Nullch)
#define ask3(a,b,c) ask(a, b, c, Nullch)
#define ask4 ask
#define fatal1(a) fatal(a, Nullch, Nullch, Nullch)
#define fatal2(a,b) fatal(a, b, Nullch, Nullch)
#define fatal3(a,b,c) fatal(a, b, c, Nullch)
#define fatal4 fatal
#define pfatal1(a) pfatal(a, Nullch, Nullch, Nullch)
#define pfatal2(a,b) pfatal(a, b, Nullch, Nullch)
#define pfatal3(a,b,c) pfatal(a, b, c, Nullch)
#define pfatal4 pfatal

/* if neither of the above work, join all multi-line macro calls. */
#endif

EXT char serrbuf[BUFSIZ];		/* buffer for stderr */

char *fetchname(const char *,int,int);
int move_file(const char *,const char *);
void copy_file(const char *,const char *);
void say(const char*,...);
void fatal(const char*,...);
void pfatal(const char*,...);
void ask(const char*,...);
char *savestr(const char *);
void set_signals(int);
void ignore_signals(void);
void makedirs(const char *,bool);

#ifdef __GNUC__
void my_exit(int) __attribute__((noreturn));
#else
void my_exit();
#endif
