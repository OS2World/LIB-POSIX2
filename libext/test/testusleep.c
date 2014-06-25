/* * * testusleep.c/.h --- emx/gcc
 * using readmode of HRTimer 1.1 to emulate usleep() api of BSD Unix
 * parts from HRTest.C by Rick Fishman and parts 3/99 by Heinz Repp
 *

This package is distributed on an as-is basis solely for the benefit of other
developers either in need of a high-resolution hfTimer or some sample device
driver code. The author makes no promises to provide support for the contents.

The core of HRTimer has been nearly completely rewritten.  The
differences to the original version are listed below:

>>> improved accuracy <<<
The hfTimer chip in all PCs up to pentium III systems is driven by the
frequency of 1/4 of the first IBM PC's clock, 4.772727 MHz, which in
turn is derived from 4/3 of the NTSC pixel frequency of 3.579545 MHz.
This frequency (1.193182 MHz) corresponds with a tick time of 838.0953
ns. The first version used a tick time of 840 ns, which differs from the
correct value by +0.23 %. This was partly corrected by the value taken
for the tick count of a complete counter cycle. The 16 bit counter
loaded with 0 runs a cycle of 2^16 = 65536 ticks, whereas the
calculations used a value of 65535, which reduced the result by
-0.0015 %, by far not enough to compensate for the first error.

The new version uses a ratio of integers for the tick time, 17600 / 21.
This agrees with 6 decimal places of the correct value, compared to 2
decimal places before. Btw: who *really* missed this accuracy? And whose
quartz is so exact?

>>> reduced system load <<<
The original version already used a very small amount of processor time.
Nevertheless this small amount is markedly reduced. In the first version
the calculation of the milliseconds/nanoseconds-structure was done
during interrupt time every time slice (typically 31 ms), involving
multiple multiplications and additions. The new version does all
calculations in kernel time, i.e. when a readis issued. So it
has to do it only once for every readrequest, not continuously; there
are only 19 machine instructions during interrupt time, at most one
comparison and the hfTimer I/O readto keep one 48 bit memory counter up
to date. Furthermore disabling of interrupts has been reduced to the
absolute minimum, leaving only two short code sequences to be protected.

>>> increased compatibility <<<
Version 1.0 initialized the hfTimer channel 0 to mode 2 and starting value
65536 (= 0). Version 1.1 leaves off this step for two reasons:

1. Timer 0 is exclusively owned and used by the OS/2 clock device.
   HRTimer is kind of a parasite: it should not be allowed to change
   settings crucial to the functioning of the system for principal
   reasons. Btw this is also the reason HRTimer must not report to the
   Resource Manager; it would be rejected. It only may work as a
   'Non-Resource manager aware device driver' in the secret.

2. Fortunately all clock devices of the current versions (CLOCK01.SYS
   resp. CLOCK02.SYS) initialize hfTimer 0 with exactly the same settings
   as HRTimer would want to. This is the reason why HRTimer did no harm
   until now with initializing the hfTimer: it only repeated what CLOCK$
   did before.

Conclusion: As long as OS/2 initializes hfTimer 0 to the settings above
   there is no need for HRTimer to do so. If a future version of OS/2
   decides to change the way hfTimer 0 works we should let it do so: it,
   and not HRTimer, is the owner, and the clock function is critical.
   Disturbing it may corrupt the system.

Rick Fishman told about a bug in HRTimer when running together with full
screen Windows. IMHO this is not a bug, it is the fact that both the
Windows TIMER.DRV as well as MMSOUND.DRV use hfTimer 0  with *different*
settings (different mode, different starting value). When running
HRTimer concurrently with one of these drivers, the output of HRTimer
is absolutely not reliable. It can both be (sometimes) much too high or
(mostly) much too low. In my experience the values turn out to be
reliable again when opening a seamless Windows session *after* closing
the full screen Windows session (it seems that the seamless driver
resets the hfTimer to the standard OS/2 settings). Otherwise the intervals
were reported about 75 % too short even after full screen Windows has
finished. Interaction has especially been observed when the Windows
Media Player was running (probably because of MMSOUND being involved).
The conclusion is *not having running any Windows session at all* while
using HRTimer!

One final topic covering compatibility: HRTimer (all versions) requires
the time slice be less than 55 milliseconds. This should be the case in
all unaltered environments. If you choose to increase the TIMESLICE
setting in the CONFIG.SYS you may not be able to use HRTimer (readout
too low).

>>> second Read API <<<
The TIMESTAMP structure introduced with the first version was fine, but
it also had some disadvantages. It always involved two long integers in
all calculations, with the need of special routines for addition,
subtraction, multiplication and so on. One could encapsulate all that in
a C++ class, but ... Furthermore this structure suggested extreme
precision by showing single nanoseconds, but, as said above, the maximum
precision achievable is 838 ns, about 1 microsecond. Furthermore one
seldom needs the 2^32 milliseconds range provided, resp. 49.7 days.

This is why I introduced a second readformat, that returns a single
unsigned 32 bit integer count of microseconds. [This is equivalent to 
the BSD Unix interface 

	void usleep(unsigned long int usec);

(A.H.)] This allows easy
manipulation of time values with simple long integer operations,
preserving the physical precision. It provides for measuring intervals
up to 1 h 11 min 35 s. A wraparound of the returned value from 2^32 - 1
(4294967295) to 0 needs not to be accounted for, as long as one is
interested in differences, for long integer subtraction works modulo
2^32 (the difference between two return values 200 microseconds apart
is always 200, whether the values are e.g. 123456 and 123656 or
4294967196 and 100).

How to use this second format? You only have to specify the size of long
integer (4) as the buffer length for DosRead. The driver decides by the
buffer length, whether a TIMESTAMP structure (12 bytes) or an unsigned
long integer (4 bytes) is requested. A working example is provided with
a clone of the original HRTEST program, HRTEST2.C/EXE. It has the same
functionality, but note the simplicity! And note one further difference:
To determine the overhead time for an empty operation you should always
issue a dummy readin advance to avoid including the processor cache
priming time in the Overhead. The following reads are always faster, in
my system the difference is almost 100 %!

If one uses a second time source, less exact but stable for a long time
like maybe the system clock, that provides information about how many
times 2^32 microseconds (approx. 1.19305 h) must be added to the
start-stop difference, the interval to be measured can easily be
expanded to many months. Same is true for the TIMESTAMP value exceeding
49.7 days. Then the only limit is the period the 48 bit tick counter
covers: 7.475 (tropical) years! N.B.: After this time *after opening the
device* the returned values of both readAPIs (TIMESTAMP and unsigned
long) will suddenly drop to zero (from 3974603640:168000 [ms:ns] resp.
1758891368 microseconds), but then you could continue to use the hfTimer
as usual for another 7.475 years a.s.o. ;-)

>>> use of 32 bit operations of 386 processor and more <<<
The use of 32 bit instructions of the 386 family clearly facilitated the
arithmetic operations. Furthermore one function no longer required with
OS/2 2.1 and up was removed (UnPhysToVirt). The driver therefore needs
at least OS/2 2.1. Additionally a build level entry was added (for the
buildlevel utility). One 'bug' which probably never occurred was
removed: you may have crashed version 1.0 by issuing a REMOVABLE_MEDIA
strategy command to the driver, but I don't know if this is really
possible, this command should be restricted to block devices only
(HRTIMER is a char device).

>>> despite the enhancements: disk and resident size reduced <<<
The code size (resident and transient) was reduced partly because of
using the 386 commands, mainly due to the different approach. The
reduced resident data size is also due to a harmless bug in the first
version: the data segment size was mixed with the size of transient
portion. Luckily this made it longer, not shorter ;-)

Finally: have fun, take precise measurements and: USE AT YOUR OWN RISK!

February 1999 - this driver is Y2K-ready!  Heinz Repp

Comments or bug reports are always welcome. Please mail them to:
		Heinz.Repp@online.de
 */

#ifndef __MT__
#warning You must compile with '-Zmt' !
#define __MT__
#endif

#define INCL_BASE
#define OS2_API32
#define INCL_DOSFILEMGR
#include <os2.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <sys/time.h>

#define DEFAULT_TIME_PERIOD 500
#define MAX_LOOP 20
   
static int Timer; 		/* Timer handle repository */

	int
main (int argc, char *argv[])
{
 ULONG ulTP, ulTP2, ulStart, ulStop;
 unsigned int i = 0;
 
	if(_initHrTimer()) {
		perror("timer init failed! Error code");
		return(2);
	}
	Timer = open("TIMER$", O_RDONLY | O_BINARY);
	if (Timer == (-1)) {
		fputs("Opening hfTimer device 'TIMER$' failed!\n", stderr);
		return (1);
	}
	/* check for commandline args */
	if (argc != 2)
		ulTP = DEFAULT_TIME_PERIOD;
	else
		ulTP = atoi(argv[1]);

	ulTP2 = ulTP * 1000; /* usecs */

	_dPrintf("HRTest2 - A short skeleton to demonstrate the use of the second\n\
simplified readmode of HRTimer 1.1 ((C) 3/99 by Heinz Repp, parts from \n\
HRTest.C by Rick Fishman)\n\
	Usage: hrhfTimer2 [TIMEPERIOD (msecs)]\n\n\
* Measuring a DosSleep of %lu milliseconds\n", ulTP);

	/* Main loop: running until user aborts */
	while (i++ < MAX_LOOP) {
	 ULONG 	_ul2HRTimerReadsOverhead;
   
		_dPrintf("%2u : ", i);
		_dPrintf("Sleeping %lu msecs ...", ulTP);
		if ((1 > read(Timer, &ulStop, sizeof(ULONG)))) /* dummy read */
			printf("Reading from timer failed");

		/* Calculate internal timing overhead */
		read(Timer, &ulStart, sizeof(ULONG));
		read(Timer, &ulStop, sizeof(ULONG));
		_ul2HRTimerReadsOverhead = ulStop - ulStart;
		
		read(Timer, &ulStart, sizeof(ULONG));
		DosSleep(ulTP);
		read(Timer, &ulStop, sizeof(ULONG));
		_dPrintf(" DosSleep = %10lu us - ", ulStop - ulStart -
			_ul2HRTimerReadsOverhead);
		read(Timer, &ulStart, sizeof(ULONG));
		usleep(ulTP2);
		read(Timer, &ulStop, sizeof(ULONG));
		_dPrintf(" usleep = %10lu us\n", ulStop - ulStart -
			_ul2HRTimerReadsOverhead);
	} /* Main loop ends */
	close(Timer);
	return(0);
}



