#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <assert.h>
#include <ieeefp.h>
#include <float.h>

void sigfpe(int);
volatile sig_atomic_t signal_cought;

static volatile const double one  = 1.0;
static volatile const double zero = 0.0;
static volatile const double huge = DBL_MAX;
static volatile const double tiny = DBL_MIN;

int
main(int argc, const char *argv[])
{
	volatile double x;

	if (argc != 2)
		exit(1);

	/*
	 * check to make sure that all exceptions are masked and 
	 * that the accumulated exception status is clear.
 	 */
	assert(fpgetmask() == 0);
	assert(fpgetsticky() == 0);

	/* set up signal handler */
	signal(SIGFPE, sigfpe);
	signal_cought = 0;

	/* trip divide by zero */
	x = one / zero;
	assert (fpgetsticky() & FP_X_DZ);
	assert (signal_cought == 0);
	fpsetsticky(0);

	/* trip invalid operation */
	x = zero / zero;
	assert (fpgetsticky() & FP_X_INV);
	assert (signal_cought == 0);
	fpsetsticky(0);

	/* trip overflow */
	x = huge * huge;
	assert (fpgetsticky() & FP_X_OFL);
	assert (signal_cought == 0);
	fpsetsticky(0);

	/* trip underflow */
	x = tiny * tiny;
	assert (fpgetsticky() & FP_X_UFL);
	assert (signal_cought == 0);
	fpsetsticky(0);

	/* unmask and then trip divide by zero */
if (argv[1][0] == 'o'||argv[1][0] == 'a') {
	fpsetmask(FP_X_DZ);
	x = one / zero;
	assert (signal_cought == 1);
	signal_cought = 0;
}

	/* unmask and then trip invalid operation */
if (argv[1][0] == 'z'||argv[1][0] == 'a') {
	fpsetmask(FP_X_INV);
	x = zero / zero;
	assert (signal_cought == 1);
	signal_cought = 0;
}

	/* unmask and then trip overflow */
if (argv[1][0] == 'h'||argv[1][0] == 'a') {
	fpsetmask(FP_X_OFL);
	x = huge * huge;
	assert (signal_cought == 1);
	signal_cought = 0;
}
	/* unmask and then trip underflow */
if (argv[1][0] == 't'||argv[1][0] == 'a') {
	fpsetmask(FP_X_UFL);
	x = tiny * tiny;
	assert (signal_cought == 1);
	signal_cought = 0;
}
	exit(0);
}

void
sigfpe(int dummy)
{
  if (dummy)
  	++dummy;
  
	signal_cought = 1;
}
