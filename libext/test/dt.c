/*
   Example code to demonstrate the use of dlopen() interface.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "dlfcn.h"

static void test1(void);
static void test2(void);
static void test3(void);
static void test4(void);
static int (*mathfcn) (int);
static int (*dh)(int);
static const char *dlerrortext;
static char pfad[256];

#define intARG   3
#define FLOATARG 3.1415F

static int res;

	static void 
test1 (void)
{
  _abspath (pfad, "sam.dll", 256);
  fprintf(stdout, "pfad=%s\n", pfad);
  dh = dlopen (pfad, 0);
  if (!dh)
    if ((dlerrortext = dlerror ()) != NULL)
      {
	fprintf(stdout, "dlopen(): %s\n", dlerrortext);
	return;
      }
  mathfcn = dlsym (dh, "my2");	/* get handle for external function */
  if ((dlerrortext = dlerror ()) != NULL)
    {
      printf ("dlsym()=%s\n", dlerrortext);
      return;
    }
  res = (*mathfcn) (intARG);
  fprintf (stdout, "my2(%i) = %i\n", intARG, res);
  dlclose (dh);

}

	static void 
test2 (void)
{
 float(*mathfcn2)(float);
 float(*dh2)(float);
 float res2;
 const float fARG = FLOATARG;

  dh2 = dlopen ("sam", 0);	/* Try referencing dll w/o absolute path.
				   Now LIBPATH is being searched */
  if (!dh2)
    if ((dlerrortext = dlerror ()) != NULL)
      {
	fprintf(stdout, "dlopen(): %s\n", dlerrortext);
	return;
      }
	/* get handle for external function */
  mathfcn2 = dlsym(dh2, "mysin");
  if ((dlerrortext = dlerror ()) != NULL)
    {
      fprintf (stdout, "dlsym(): %s\n", dlerrortext);
      return;
    }
  res2 = (*mathfcn2)(fARG);
  fprintf(stdout, "mysin(%f) = %f\n", fARG, res2);
  dlclose(dh2);
}


	static void 
test3 (void)
{
 int(*dh2)(float);
  	/* Try referencing dll with relative path. */
	dh2 = dlopen ("./sam.dll", 0);
  if (!dh2) if ((dlerrortext = dlerror ()) != NULL) {
		fprintf (stdout, "dlopen(): %s\n", dlerrortext);
		return;
  }
	fprintf (stdout, "dlopen(\"./sam.dll\", 0): Success!\n");
  dlclose (dh2);
}

	static void 
test4 (void)
{
 int(*dh2)(float);
  dh2 = dlopen ("sam2", 0);	/* Try referencing dll w/o absolute path.
				   Now LIBPATH is being searched */
  if (!dh2) if ((dlerrortext = dlerror ()) != NULL) {
		fprintf (stdout, "dlopen(): %s\n", dlerrortext);
		return;
  }
  dlclose (dh2);
}


int main (void)
{

  test1 ();
  test2 ();
  test3 ();
  test4 ();

  exit (0);
}
