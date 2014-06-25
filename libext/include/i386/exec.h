/* $Header: /cvsroot/posix2/posix2/libext/include/i386/exec.h,v 1.1 2000/11/17 19:14:58 arnd Exp $ */

/*
 * Copyright (c) 1993 Christopher G. Demetriou
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _I386_EXEC_H_
#define _I386_EXEC_H_

#ifndef EMX
#define EMX
#endif


#define SEGMENT_SIZE 		0x10000    	/* emx */
#define HOST_SEGMENT_SIZE 	SEGMENT_SIZE

#define PAGE_SIZE		0x1000
#define __LDPGSZ		PAGE_SIZE
#define HOST_PAGE_SIZE 		PAGE_SIZE

#define MACHINE_ARCH		arch_emx
#define HOST_MACHINE_ARCH	bfd_arch_emx

#define TEXT_START_ADDR		0x10000
#define HOST_TEXT_START_ADDR	TEXT_START_ADDR

#define STACK_END_ADDR		0x80000000
#define HOST_STACK_END_ADDR	STACK_END_ADDR

#define EXEC_BYTES_SIZE 	(8 * 4)

/* EXACT TYPES */
typedef char int8e_type;
typedef unsigned char uint8e_type;
typedef short int16e_type;
typedef unsigned short uint16e_type;
typedef int int32e_type;
typedef unsigned int uint32e_type;
 
 /* CORRECT SIZE OR GREATER */
typedef char int8_type;
typedef unsigned char uint8_type;
typedef short int16_type;
typedef unsigned short uint16_type;
typedef int int32_type;
typedef unsigned int uint32_type;

/* Relocation format. */

struct relocation_info_i386 {
	int r_address;			/* offset in text or data segment */
	unsigned int r_symbolnum : 24,	/* ordinal number of add symbol */
			 r_pcrel :  1,	/* 1 if value should be pc-relative */
			r_length :  2,	/* log base 2 of value's width */
			r_extern :  1,	/* 1 if need to add symbol to value */
		       r_baserel :  1,	/* linkage table relative */
		      r_jmptable :  1,	/* relocate to jump table */
		      r_relative :  1,	/* load address relative */
			  r_copy :  1;	/* run time copy */
};
#define relocation_info	relocation_info_i386

#define _NLIST_DO_AOUT
#define _KERN_DO_AOUT

#if 0 /* not yet implemented */
#define _NLIST_DO_ELF

#if defined(COMPAT_LINUX) || defined(COMPAT_SVR4) || defined(COMPAT_FREEBSD) || \
    !defined(_KERNEL)
#define _KERN_DO_ELF
#endif

#define ELF_TARG_CLASS		ELFCLASS32
#define ELF_TARG_DATA		ELFDATA2LSB
#define ELF_TARG_MACH		EM_386 /* XXX - EM_486 is currently unused
                                          by all OSs/compilers/linkers */
#endif /* 0 */
#endif  /* _I386_EXEC_H_ */
