/* $Id: direntx.c,v 1.2 2000/08/22 22:20:23 amai Exp $ */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>

#define INITIAL_SIZE	20

typedef int(*comp_T)(const void*,const void*); 

/* SCANDIR  Scan directory, collect all (selected) items into a an array. */
	extern int
_scandir(const char *Name, struct dirent ***List,
            int (*Selector)(const struct dirent *), 
            int (*Sorter)(const struct dirent **, const struct dirent **))
{
 DIR *Dp;
 struct dirent **names;

  /* Get initial list space and open directory. */
  size_t size = INITIAL_SIZE;

  if ( NULL == ( names = malloc(size * sizeof(names[0])) )
       || (Dp = opendir(Name)) == NULL )    return(-1);
  else {
   struct dirent *E;
   size_t i = 0;
    /* Read entries in the directory. */
    while ((E = readdir(Dp))) if (Selector == NULL||(*Selector)(E)) {
      /* User wants them all, or he wants this one. */
      if (++i >= size) {
        size <<= 1;
        names = realloc(names, size * sizeof(names[0]));

        if (!names) {closedir(Dp);  return(-1);}
      }
      /* Copy the struct dirent. */
      if ( !(names[i - 1] = malloc(sizeof(struct dirent))) ) {
        closedir(Dp); return(-1);
      }
      *names[i - 1] = *E;
    }
    /* Close things off. */
    names[i] = NULL;
    *List = names;
    closedir(Dp);

    /* Sort? */
    if (i && Sorter)
      qsort(names, i, sizeof names[0], (comp_T)Sorter);
    return (int)i;
  }
}

/* Alphabetic order comparison routine for those who want it. */
	extern int
_alphasort(const struct dirent **d1, const struct dirent **d2 )
{
	return strcmp((*d1)->d_name, (*d2)->d_name);
}

	extern int
scandir(const char *N, struct dirent ***L,int(*S)(const struct dirent*), 
            int(*C)(const struct dirent **, const struct dirent **))
{
	return _scandir(N,L,S,C);
}

	extern int
alphasort(const struct dirent**d1,const struct dirent**d2)
{
	return strcmp((*d1)->d_name, (*d2)->d_name);
}

