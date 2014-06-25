/* $Id: dlfcn.h,v 1.2 2000/03/24 08:52:07 veit Exp $    dlfcn.h -- emx/gcc -- 
    Implementation of dlopen() for OS/2 -- Released into public domain
     */
#ifndef _DLFCN_H
#define _DLFCN_H

/*  We do not actually (i.e. not yet) use the definitions below but have
     to keep them for compatibility ... 
      */
#define RTLD_LAZY       0x001
#define RTLD_NOW        0x002
#define RTLD_GLOBAL     0x004
#define RTLD_LOCAL      0x008

#if defined (__cplusplus)
# ifndef __inline__ 
#  define __inline__ inline
# endif
extern "C" {
#endif

extern void*_dlopen (__const__ char*,int);
extern __const__ char*_dlerror(void);
extern void*_dlsym(void *, __const__ char*);
extern int _dlclose(void *);

/* Machine-dependent inline versions */
#if (!__GNUC__ || __GNUC__ < 2 || (__GNUC__ == 2 && __GNUC_MINOR__ <= 7))

extern void*dlopen (__const__ char*,int);
extern char*dlerror(void);
extern void*dlsym(void *, __const__ char*);
extern int dlclose(void *);

#else

extern __inline__ void*dlopen (__const__ char*,int);
extern __inline__ __const__ char*dlerror(void);
extern __inline__ void*dlsym(void *, __const__ char*);
extern __inline__ int dlclose(void *);

	extern __inline__ void*
dlopen(const char *p,int d){return _dlopen(p,d);}
	extern __inline__ int 
dlclose(void *handle){return _dlclose(handle);}
	extern __inline__ void*
dlsym(void*h,const char*s){return _dlsym(h,s);}
	extern __inline__ __const__ char *
dlerror(void){return _dlerror();}

#endif

#if defined (__cplusplus)
}
#endif

#endif /* not _DLFCN_H */
