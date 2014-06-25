/*	$Id: arc4random.c,v 1.1 2000/11/17 19:24:18 arnd Exp $	*/

/*
 * Arc4 random number generator for OpenBSD.
 * Copyright 1996 David Mazieres <dm@lcs.mit.edu>.
 *
 * Modification and redistribution in source and binary forms is
 * permitted provided that due credit is given to the author and the
 * OpenBSD project (for instance by leaving this copyright notice
 * intact).
 */

/*
 * This code is derived from section 17.1 of Applied Cryptography,
 * second edition, which describes a stream cipher allegedly
 * compatible with RSA Labs "RC4" cipher (the actual description of
 * which is a trade secret).  The same algorithm is used as a stream
 * cipher called "arcfour" in Tatu Ylonen's ssh package.
 *
 * Here the stream cipher has been modified always to include the time
 * when initializing the state.  That makes it impossible to
 * regenerate the same random sequence twice, so this can't be used
 * for encryption, but will generate good random numbers.
 *
 * RC4 is a registered trademark of RSA Laboratories.
 */

#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/param.h>
#include <sys/time.h>
#include <sys/sysctl.h>

#ifdef __GNUC__
#define inline __inline__
#else
#define inline
#endif				/* !__GNUC__ */

struct _arc4_stream {
	u_int8_t i;
	u_int8_t j;
	u_int8_t s[256];
};

int     rs_initialized;
static struct _arc4_stream rs;

	static inline void
_arc4_init(struct _arc4_stream *as)
{
	int     n;

	for (n = 0; n < 256; n++)
		as->s[n] = n;
	as->i = 0;
	as->j = 0;
}

	static inline void
_arc4_addrandom(struct _arc4_stream *as, u_char *dat, int datlen)
{
 int     n;

	as->i--;
	for (n = 0; n < 256; n++) {
	 u_int8_t si;

		as->i = (as->i + 1);
		si = as->s[as->i];
		as->j = (as->j + si + dat[n % datlen]);
		as->s[as->i] = as->s[as->j];
		as->s[as->j] = si;
	}
	as->j = as->i;
}

	static void
_arc4_stir(struct _arc4_stream *as)
{
	struct {
		struct timeval tv;
		u_int rnd[(128 - sizeof(struct timeval)) / sizeof(u_int)];
	}       rdat;

	gettimeofday(&rdat.tv, NULL);
#if 0 /* don't have (AH) */
	{ int fd = open("/dev/arandom", O_RDONLY);
	
	if (fd != -1) {
		read(fd, rdat.rnd, sizeof(rdat.rnd));
		close(fd);
	}} else 
#endif
	{
		int mib[2];
		size_t len, i = 0;

		/* Device could not be opened, we might be chrooted, take
		 * randomness from sysctl. */

		mib[0] = CTL_KERN;
		mib[1] = KERN_ARND;

		while (i < sizeof(rdat.rnd) / sizeof(u_int)) {
			len = sizeof(u_int);
			if (sysctl(mib, 2, &rdat.rnd[i], &len, NULL, 0) == -1)
				break;
			++i;
		}
	}
	/* fd < 0 or failed sysctl ?  Ah, what the heck. We'll just take
	 * whatever was on the stack... */

	_arc4_addrandom(as, (void *) &rdat, sizeof(rdat));
}

	static inline u_int8_t
_arc4_getbyte(struct _arc4_stream *as)
{
 u_int8_t si, sj;

	as->i = (as->i + 1);
	si = as->s[as->i];
	as->j = (as->j + si);
	sj = as->s[as->j];
	as->s[as->i] = sj;
	as->s[as->j] = si;
	return (as->s[(si + sj) & 0xff]);
}

	static inline u_int32_t
_arc4_getword(struct _arc4_stream *as)
{
 u_int32_t val;
	val =  _arc4_getbyte(as) << 24;
	val |= _arc4_getbyte(as) << 16;
	val |= _arc4_getbyte(as) << 8;
	val |= _arc4_getbyte(as);
	return val;
}

	void
_arc4random_stir(void)
{
	if (!rs_initialized) {
		_arc4_init(&rs);
		rs_initialized = 1;
	}
	_arc4_stir(&rs);
}

	void
_arc4random_addrandom(u_char *dat, int datlen)
{
	if (!rs_initialized)
		_arc4random_stir();
	_arc4_addrandom(&rs, dat, datlen);
}

	extern unsigned int
_arc4random(void)
{
	if (!rs_initialized)
		_arc4random_stir();
	return _arc4_getword(&rs);
}

	void
arc4random_stir(void)			{_arc4random_stir();}
	void
arc4random_addrandom(u_char *d, int l)	{_arc4random_addrandom(d,l);}
	extern unsigned int
arc4random(void)			{return _arc4random();}


#if 0
/*-------- Test code for i386 --------*/
#include <stdio.h>
#include <machine/pctr.h>
int
main(int argc, char **argv)
{
	const int iter = 1000000;
	int     i;
	pctrval v;

	v = rdtsc();
	for (i = 0; i < iter; i++)
		arc4random();
	v = rdtsc() - v;
	v /= iter;

	printf("%qd cycles\n", v);
}
#endif

