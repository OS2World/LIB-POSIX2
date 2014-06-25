/* libExt+gcc: $Id: search.c,v 1.1 2000/08/15 08:06:02 amai Exp $ */

#include <sys/types.h>
#include <string.h>
#include <search.h>

/*
 * Copyright (c) 1989, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * This code is derived from software contributed to Berkeley by
 * Roger L. Snyder.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by the University of
 *	California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */
typedef int(*cmp_fn_t)(const void*,const void*);
static void*linear_base(const void*,const void*,size_t*,size_t,cmp_fn_t,int);


	static void *
linear_base(const void *key, const void *base, size_t *nelp, 
			size_t width, cmp_fn_t compar, int add_flag)
{
	register const char
 *element = base, 
 *end = (const char*)base + *nelp * width;
	
	for (; element < end; element += width)
		if (!compar(key, element))		/* key found */
			return((void*)element);

	if (!add_flag)					/* key not found */
		return(NULL);

	/*
	 * The UNIX System User's Manual, 1986 edition claims that
	 * a NULL pointer is returned by lsearch with errno set
	 * appropriately, if there is not enough room in the table
	 * to add a new item.  This can't be done as none of these
	 * routines have any method of determining the size of the
	 * table.  This comment isn't in the 1986-87 System V
	 * manual.
	 */
	++*nelp;
	memcpy((void *)end, key, width);
	return((void *)end);
}

void *
lsearch(key, base, nelp, width, compar)
	const void *key, *base;
	size_t *nelp, width;
	cmp_fn_t compar;
{
	return(linear_base(key, base, nelp, width, compar, 1));
}

	extern void *
lfind(const void *key, const void *base, size_t *nelp, 
				size_t width, cmp_fn_t compar)
{
	return(linear_base(key, base, nelp, width, compar, 0));
}

/*
 *  Copyright (c) 1993 John Brezak
 *  All rights reserved.
 * 
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *  1. Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *  3. The name of the author may be used to endorse or promote products
 *     derived from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR `AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

struct __qelement {
        struct __qelement *q_forw;
        struct __qelement *q_back;
};

	extern void
insque(void *entry, void *pred)
{
	struct __qelement *e = (struct __qelement *) entry;
	struct __qelement *p = (struct __qelement *) pred;

	e->q_forw = p->q_forw;
	e->q_back = p;
	p->q_forw->q_back = e;
	p->q_forw = e;
}

	extern void
remque(void *element)
{
	struct __qelement *e = (struct __qelement *) element;
	e->q_forw->q_back = e->q_back;
	e->q_back->q_forw = e->q_forw;
}

/*
 * Tree search generalized from Knuth (6.2.2) Algorithm T just like
 * the AT&T man page says.
 *
 * Written by reading the System V Interface Definition, not the code.
 *
 * Totally public domain.
 */

/*LINTLIBRARY*/
/* The __node_t structure is for internal use only, lint doesn't grok it.
 */
typedef struct __node_t { 
    char	  *key;
    struct __node_t *leftlink, *rightlink;
} node;

/* find or insert datum into search tree */
	extern void *
tsearch(const void*vkey, void**vrootp, int(*compar)(const void*,const void*))
	/* const void 	*vkey;		key to be located 
	   void		**vrootp;	address of tree root */
{
    register node *q;
    char *key = (char *)vkey;
    node **rootp = (node **)vrootp;

    if (rootp == (struct __node_t **)0)
	return ((void *)0);
    while (*rootp != (struct __node_t *)0) {	/* Knuth's T1: */
	int r;

	if ((r = (*compar)(key, (*rootp)->key)) == 0)	/* T2: */
	    return ((void *)*rootp);		/* we found it! */
	rootp = (r < 0) ?
	    &(*rootp)->leftlink :		/* T3: follow left branch */
	    &(*rootp)->rightlink;		/* T4: follow right branch */
    }
    q = (node *) malloc(sizeof(node));	/* T5: key not found */
    if (q != (struct __node_t *)0) {	/* make new node */
	*rootp = q;			/* link new node to old */
	q->key = key;			/* initialize new node */
	q->leftlink = q->rightlink = (struct __node_t *)0;
    }
    return ((void *)q);
}

/* delete node with given key */
	extern void *
tdelete(const void*vkey, void**vrootp, int(*compar)(const void*,const void*))
	/* const void	*vkey;		key to be deleted 
	   void		**vrootp;	address of the root of tree */
{
    node **rootp = (node **)vrootp;
    char *key = (char *)vkey;
    node *p;
    register node *q;
    register node *r;
    int cmp;

    if (rootp == (struct __node_t **)0 || (p = *rootp) == (struct __node_t *)0)
	return ((struct __node_t *)0);
    while ((cmp = (*compar)(key, (*rootp)->key)) != 0) {
	p = *rootp;
	rootp = (cmp < 0) ?
	    &(*rootp)->leftlink :		/* follow left branch */
	    &(*rootp)->rightlink;		/* follow right branch */
	if (*rootp == (struct __node_t *)0)
	    return ((void *)0);		/* key not found */
    }
    r = (*rootp)->rightlink;			/* D1: */
    if ((q = (*rootp)->leftlink) == (struct __node_t *)0)	/* Left (struct __node_t *)0? */
	q = r;
    else if (r != (struct __node_t *)0) {		/* Right link is null? */
	if (r->leftlink == (struct __node_t *)0) {	/* D2: Find successor */
	    r->leftlink = q;
	    q = r;
	} else {			/* D3: Find (struct __node_t *)0 link */
	    for (q = r->leftlink; q->leftlink != (struct __node_t *)0; q = r->leftlink)
		r = q;
	    r->leftlink = q->rightlink;
	    q->leftlink = (*rootp)->leftlink;
	    q->rightlink = (*rootp)->rightlink;
	}
    }
    free((struct __node_t *) *rootp);	/* D4: Free node */
    *rootp = q;				/* link parent to new node */
    return(p);
}

/* Walk the nodes of a tree */
	/* root: Root of the tree to be walked */
	/* action: Function to be called at each node */
	static void
trecurse(node*root, void(*action)(const void*,VISIT, int), int level)
{
    if (root->leftlink == (struct __node_t *)0 && root->rightlink == (struct __node_t *)0)
	(*action)(root, leaf, level);
    else {
	(*action)(root, preorder, level);
	if (root->leftlink != (struct __node_t *)0)
	    trecurse(root->leftlink, action, level + 1);
	(*action)(root, postorder, level);
	if (root->rightlink != (struct __node_t *)0)
	    trecurse(root->rightlink, action, level + 1);
	(*action)(root, endorder, level);
    }
}

/* Walk the nodes of a tree */
	extern void
twalk(const void*vroot, void(*action)(const void*,VISIT, int))
	 /* vroot: Root of the tree to be walked */
{
 node *root = (node *)vroot;

	if (root != (node *)0 && action != (void(*)(const void*,VISIT, int)) NULL)
		trecurse(root, action, 0);
}


/* find a node, or return 0 */
	extern void *
tfind(const void*vkey, void*const*vrootp, int(*compar)(const void*,const void*))
{
    char *key = (char *)vkey;
    node **rootp = (node **)vrootp;

    if (rootp == (struct __node_t **)0)
	return ((struct __node_t *)0);
    while (*rootp != (struct __node_t *)0) {	/* T1: */
	int r;
	if ((r = (*compar)(key, (*rootp)->key)) == 0)	/* T2: */
	    return (*rootp);		/* key found */
	rootp = (r < 0) ?
	    &(*rootp)->leftlink :		/* T3: follow left branch */
	    &(*rootp)->rightlink;		/* T4: follow right branch */
    }
    return (node *)0;
}

