/* libExt+gcc: - $Id: stdlib.h,v 1.11 2000/11/26 16:43:01 arnd Exp $ */

#ifndef _STDLIB_H
#define _STDLIB_H

#if defined(__cplusplus)
extern "C" {
#endif

#if !defined(_SIZE_T)
#define _SIZE_T
typedef unsigned long size_t;
#endif

#if !defined(_WCHAR_T)
#define _WCHAR_T
typedef unsigned short wchar_t;
#endif

#if !defined(NULL)
#if defined(__cplusplus)
#define NULL 0
#else
#define NULL ((void *)0)
#endif
#endif

#if !defined(_DIV_T)
#define _DIV_T
typedef struct _div_t {
  int quot;
  int rem;
} div_t;
typedef struct _ldiv_t {
  long quot;
  long rem;
} ldiv_t;
#endif

#if !defined(RAND_MAX)
#define RAND_MAX 0x7fff
#endif

#if !defined(EXIT_SUCCESS)
#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1
#endif

#if !defined(MB_CUR_MAX)
extern int _mb_cur_max;
#define MB_CUR_MAX _mb_cur_max
#endif

#if !defined(_MAX_PATH)
#define _MAX_PATH   260
#define _MAX_DRIVE    3
#define _MAX_DIR    256
#define _MAX_FNAME  256
#define _MAX_EXT    256
#endif


void abort(void) __attribute__((__noreturn__));
int atexit(void(*)(void));
double atof(__const__ char *);
int atoi(__const__ char *);
long atol(__const__ char *);
void *bsearch(__const__ void *, __const__ void *, size_t, size_t,
    int(*)(__const__ void *, __const__ void *));
div_t div(int, int);
void exit(int) __attribute__((__noreturn__));
char *getenv(__const__ char *);
ldiv_t ldiv(long, long);
int mblen(__const__ char *, size_t);
size_t mbstowcs(wchar_t *, __const__ char *, size_t);
int mbtowc(wchar_t *, __const__ char *, size_t);
void qsort(void *, size_t, size_t,
    int(*)(__const__ void *, __const__ void *));
int rand(void);
void srand(unsigned);
double strtod(__const__ char *, char **);
long strtol(__const__ char *, char **, int);
unsigned long strtoul(__const__ char *, char **, int);
int system(__const__ char *);
size_t wcstombs(char *, __const__ wchar_t *, size_t);
int wctomb(char *, wchar_t);

#if !defined(_ABS)                                      /* see also math.h */
#define _ABS
#if !defined(__GNUC__) || __GNUC__ >= 2
extern int abs(int);
extern long labs(long);
#else
extern __inline__ int abs(int _n) { return(_n < 0 ? -_n : _n); }
extern __inline__ long labs(long _n) { return(_n < 0 ? -_n : _n); }
#endif
#endif

#if !defined(__NO_C9X)

float strtof(__const__ char *, char **);
long double strtold(__const__ char *, char **);

#endif


#if(!defined(__STRICT_ANSI__)&&!defined(_POSIX_SOURCE))||defined(_WITH_UNDERSCORE)

#if !defined(OS2_MODE)
#define DOS_MODE 0
#define OS2_MODE 1
#endif

#if !defined(_ERRNO)
#define _ERRNO
#if defined(__MT__) || defined(__ST_MT_ERRNO__)
extern int *_errno(void);
#define errno (*_errno())
#else
extern int errno;
#endif
#endif

#if !defined(_ULDIV_T)
#define _ULDIV_T
typedef struct {
  unsigned long quot;
  unsigned long rem;
} _uldiv_t;
typedef struct {
  long long quot;
  long long rem;
} _lldiv_t;
typedef struct {
  unsigned long long quot;
  unsigned long long rem;
} _ulldiv_t;
#endif

extern char **environ;

extern __const__ char* __const__ *sys_errlist;
extern __const__ int sys_nerr;

extern __const__ unsigned int _emx_vcmp;
extern __const__ unsigned int _emx_env;
extern __const__ unsigned int _emx_rev;
extern __const__ unsigned char _osminor;
extern __const__ unsigned char _osmajor;
extern __const__ unsigned char _osmode;
extern __const__ char _emx_vprt[5];


unsigned alarm(unsigned);
void *brk(void *);
int chdir(__const__ char *);
char *gcvt(double, int, char *);
char *getcwd(char *, size_t);
#undef getpagesize
extern __const__ int getpagesize(void);
#define getpagesize()(4096)
char *getwd(char *);
int mkdir(__const__ char *, long);
void perror(__const__ char *);
int putenv(__const__ char *);
int rmdir(__const__ char *);
void *sbrk(int);
unsigned sleep(unsigned);
void swab(__const__ void *, void *, size_t);
long ulimit(int, ...);

/* 4.4 BSD */
int heapsort(void*,size_t,size_t,int(*)(__const__ void*,__const__ void*));
char*initstate(unsigned long,char*,long int);         /* BSD */
long random(void);                                     /* BSD */
char*setstate(char *);                                /* BSD */
void srandom(unsigned long);                            /* BSD */

#endif


#if (!defined(__STRICT_ANSI__) && !defined(_POSIX_SOURCE) || defined(_WITH_UNDERSCORE))

extern char **_environ;
extern __const__ char * __const__ _sys_errlist[];
extern __const__ int _sys_nerr;

unsigned _alarm(unsigned);
void *_brk(void *);
int _chdir(__const__ char *);
char *_gcvt(double, int, char *);
char *_getcwd(char *, size_t);
extern __const__ int _getpagesize(void);
char *_getwd(char *);
int _mkdir(__const__ char *, long);
int _putenv(__const__ char *);
int _rmdir(__const__ char *);
void *_sbrk(int);
unsigned _sleep(unsigned);
void _swab(__const__ void *, void *, size_t);
long _ulimit(int, ...);

int _abspath(char *, __const__ char *, int);
long long _atoll(__const__ char *);
long double _atofl(__const__ char *);
int _chdir2(__const__ char *);
int _chdrive(char);
int _core(int);
void _defext(char *, __const__ char *);
void _envargs(int *, char ***, __const__ char *);
int _execname(char *, size_t);
void _exit(int) __attribute__((__noreturn__));
int _filesys(__const__ char *, char *, size_t);
int _fncmp(__const__ unsigned char *, __const__ unsigned char *);
char **_fnexplode(__const__ char *);
void _fnexplodefree(char **);
char _fngetdrive(__const__ char *);
int _fnisabs(__const__ char *);
int _fnisrel(__const__ char *);
void _fnlwr(char *);
void _fnlwr2(char *, __const__ char *);
char *_fnslashify(char *);
int _fullpath(char *, __const__ char *, int);
int _getcwd1(char *, char);
char *_getcwd2(char *, int);
char _getdrive(void);
char *_getext(__const__ char *);
char *_getext2(__const__ char *);
char *_getname(__const__ char *);
int _getsockhandle(int);
int _gettid(void);
char *_getvol(char);
char *_itoa(int, char *, int);
_lldiv_t _lldiv(long long, long long);
char *_lltoa(long long, char *, int);
char *_ltoa(long, char *, int);
void _makepath(char *, __const__ char *, __const__ char *,
    __const__ char *, __const__ char *);
int _path(char *, __const__ char *);
int _read_kbd(int, int, int);
void _remext(char *);
void _rfnlwr(void);
void _response(int *, char ***);
void _scrsize(int *);
void _searchenv(__const__ char *, __const__ char *, char *);
int _setsyserrno(int);
void _sfnlwr(__const__ char *);
unsigned _sleep2(unsigned);
char ** _splitargs(char *, int *);
void _splitpath(__const__ char *, char *, char *, char *, char *);
float _strtof(__const__ char *, char **);
long double _strtold(__const__ char *, char **);
long long _strtoll(__const__ char *, char **, int);
unsigned long long _strtoull(__const__ char *, char **, int);
char _swchar(void);
int _syserrno(void);
_uldiv_t _uldiv(unsigned long, unsigned long);
_ulldiv_t _ulldiv(unsigned long long, unsigned long long);
char *_ulltoa(unsigned long long, char *, int);
char *_ultoa(unsigned long, char *, int);
void _wildcard(int *, char ***);

#if defined(__MT__)
int _beginthread(void(*)(void *), void *, unsigned, void *);
void _endthread(void);
void **_threadstore(void);
#endif

/* Machine-dependent inline versions */
#if (!__GNUC__ || __GNUC__ < 2 || (__GNUC__ == 2 && __GNUC_MINOR__ <= 7))
# define _STD_INL extern
#else
#define _STD_INL extern __inline__
#endif

extern __const__ unsigned int _libExtensions_vcmp; /* lib version */
extern __const__ unsigned int _libExtensions_env;
extern __const__ unsigned int _libExtensions_rev;
extern __const__ char         _libExtensions_vprt[];

/* bsd 4.4
 */
char *__getenv_findEnv(__const__ char*,size_t*);
char*_getenv(__const__ char *);
int _setenv(__const__ char*,__const__ char*,int);
extern void _srandom(unsigned long);
extern void _srandomdev(void);
extern char*_initstate(unsigned long,char*,long);
extern char*_setstate(char*);
extern long _random(void);
extern unsigned int _arc4random(void);
extern void _arc4random_stir(void);
extern void _arc4random_addrandom(unsigned char*,int);

extern int _heapsort(void*,size_t,size_t,int(*)(__const__ void*,__const__ void*));
extern int _radixsort(__const__ unsigned char**,int,__const__ unsigned char*,unsigned);
extern int _sradixsort(__const__ unsigned char**,int,__const__ unsigned char*,unsigned);

_STD_INL char*_getenv(__const__ char *n){return __getenv_findEnv(n, NULL);}

extern long _a64l(__const__ char*);
extern char*_l64a(long);
extern int _daemon(int,int); /* stubs: OS/2 uses detached processes instead! */

#if !defined(__STRICT_ANSI__) && !defined(_POSIX_SOURCE)

/* System V style 48-bit random number generator functions: 
    Seed random number generator.
  */
extern void srand48(long);
extern unsigned short*seed48(unsigned short[3]);
extern void lcong48(unsigned short[7]);
   /*  Return non-negative, double-precision floating-point value in [0.0,1.0).
    */
extern double drand48(void);
extern double erand48(unsigned short[3]);
  /* Return non-negative, long integer in [0,2^31).
   */
extern long lrand48(void);
extern long nrand48(unsigned short[3]);
  /* Return signed, long integers in [-2^31,2^31).
   */
extern long mrand48(void);
extern long jrand48(unsigned short[3]);

extern void*valloc(size_t);

	/* BSD */
extern __const__ char *__progname; /* crt0 stores name of app to this global */
extern long a64l(__const__ char*);
extern char *l64a(long);
extern char	*getbsize(int*,long*);
extern char	*cgetcap(char*,__const__ char*,int);
extern int	 cgetclose(void);
extern int	 cgetent(char**,char**,__const__ char*);
extern int	 cgetfirst(char**,char**);
extern int	 cgetmatch(char*,__const__ char*);
extern int	 cgetnext(char**,char**);
extern int	 cgetnum(char*,__const__ char*,long*);
extern int	 cgetset(__const__ char*);
extern int	 cgetstr(char*,__const__ char*,char**);
extern int	 cgetustr(char*,__const__ char*,char**);
extern int  daemon(int,int); /* OS/2 uses detached processes instead!*/
extern void unsetenv(const char *name);
extern int  setenv(__const__ char*,__const__ char*,int);
extern int  mergesort(void*,size_t,size_t,int(*)(__const__ void*,__const__ void*));
extern char*realpath(__const__ char*,char*);
extern int  getsubopt(char **, char * __const__ *, char **);
extern char*suboptarg;		/* getsubopt(3) external variable */
extern char*optarg;
extern int optind;
extern int opterr;
extern int optopt;

/* Note: The 2nd argument is not const as GETOPT_ANY reorders the
   array pointed to.*/
int getopt(int, char**, __const__ char*);
size_t confstr(int,char*,size_t);

_STD_INL unsigned int arc4random(void);
_STD_INL void arc4random_stir(void);
_STD_INL void arc4random_addrandom(unsigned char*,int);
_STD_INL void srandom(unsigned long);
_STD_INL void srandomdev(void);
_STD_INL char* initstate(unsigned long seed,char*arg_state,long n);
_STD_INL char* setstate(char*);
_STD_INL long random(void);
_STD_INL int heapsort(void*,size_t,size_t,int(*c)(__const__ void*,__const__ void*));
_STD_INL int radixsort(__const__ unsigned char**,int,__const__ unsigned char*,unsigned);
_STD_INL int sradixsort(__const__ unsigned char**,int,__const__ unsigned char*,unsigned);

typedef struct {
	long long quot;		/* quotient */
	long long rem;		/* remainder */
} qdiv_t;

extern qdiv_t qdiv(long long,long long);
extern long long qabs(long long);
extern long long strtoq(__const__ char*,char**,int);
extern unsigned long long strtouq(__const__ char*,char**,int);
extern long long strtoll(__const__ char*,char**,int);
extern unsigned long long strtoull(__const__ char*,char**,int);

#if !(!__GNUC__ || __GNUC__ < 2 || (__GNUC__ == 2 && __GNUC_MINOR__ <= 7))
_STD_INL char*getenv(__const__ char *__n){return _getenv(__n);}
_STD_INL int putenv(__const__ char *__s){return _putenv(__s);}
_STD_INL int setenv(__const__ char*__n,__const__ char*__v,int __r)
	{return _setenv(__n,__v,__r);}
_STD_INL void srandom(unsigned long __s) {_srandom(__s);}
_STD_INL void srandomdev(void) {_srandomdev();}
_STD_INL char* initstate(unsigned long __s,char*__a,long __n) 
	{return _initstate(__s,__a,__n);}
_STD_INL char* setstate(char*c) {return _setstate(c);}
_STD_INL void arc4random_stir(void) {_arc4random_stir();}
_STD_INL void arc4random_addrandom(unsigned char* __c,int __i) 
	{_arc4random_addrandom(__c,__i);}
_STD_INL unsigned int arc4random(void) {return _arc4random();}
_STD_INL long random(void) {return _random();}
_STD_INL int heapsort(void*__b,size_t __n,size_t __s,
  int(*__c)(__const__ void*,__const__ void*)){return _heapsort(__b,__n,__s,__c);}
_STD_INL int radixsort(__const__ unsigned char**__a,int __n,__const__ unsigned 
	char*__t, unsigned __e) {return _radixsort(__a,__n,__t,__e);}
_STD_INL int sradixsort(__const__ unsigned char**__a,int __n, __const__ unsigned
	 char*__t, unsigned __e) {return _sradixsort(__a,__n,__t,__e);}
#endif /* GNUC, etc. */

#endif /* !__STRICT_ANSI__,etc. */
#endif /* !UNDERSCORE,etc. */

#if defined(__cplusplus)
}
#endif

#include <malloc.h>

#ifdef _DEBUG
# include <sys/debugx.h>
#endif

#endif /* not _STDLIB_H */
