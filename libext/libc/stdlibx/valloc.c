#include <sys/param.h>
#include <stdlib.h>
#include <unistd.h>

#define val ((long)PAGE_SIZE - 1L)

	extern void *
valloc(size_t i)
{
	long j;
	void *cp = malloc(i + val);
	
	if (!cp) 
		return(NULL);

	j = ((long)cp + val) &~ (val);
	return ((void*)j);
}

