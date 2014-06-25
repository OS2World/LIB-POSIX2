# $Header: /cvsroot/posix2/posix2/libext/libc/libm/Makefile,v 1.1 2000/08/18 19:01:57 amai Exp $
#  @(#)Makefile 5.1beta 93/09/24 
# 
#  ====================================================
#  Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.
# 
#  Developed at SunPro, a Sun Microsystems, Inc. business.
#  Permission to use, copy, modify, and distribute this
#  software is freely granted, provided that this notice 
#  is preserved.
#  ====================================================
# 
# 
#
# There are two options in making libm at fdlibm compile time:
# 	_IEEE_LIBM 	--- IEEE libm; smaller, and somewhat faster
#	_MULTI_LIBM	--- Support multi-standard at runtime by 
#			    imposing wrapper functions defined in 
#			    fdlibm.h:
#				_IEEE_MODE 	-- IEEE
#				_XOPEN_MODE 	-- X/OPEN
#				_POSIX_MODE 	-- POSIX/ANSI
#				_SVID3_MODE 	-- SVID
#
# Here is how to set up CFLAGS to create the desired libm at 
# compile time:
#
# 	CFLAGS = -D_IEEE_LIBM	... IEEE libm (recommended)
#	CFLAGS = -D_SVID3_MODE	... Multi-standard supported
#					    libm with SVID as the 
#					    default standard
#	CFLAGS = -D_XOPEN_MODE	... Multi-standard supported
#					    libm with XOPEN as the 
#					    default standard
#	CFLAGS = -D_POSIX_MODE	... Multi-standard supported
#					    libm with POSIX as the 
#					    default standard
#	CFLAGS = 		... Multi-standard supported
#					    libm with IEEE as the 
#					    default standard
# 
#For easy debugging do not use gcc '__builtin_' and inline functions:
# add: '-fno-builtin -fno-inline'

LIB = m
EXP = m.exp

DEFADD  = -D__LIBM_PRIVATE -D_USE_WRITE -D_MULTI_LIBM -D_POSIX_MODE # -D_IEEE_LIBM 

HEADERS = $(INC)math.h $(INC)mathx.h $(INC)float.h $(INC)ieee.h 	\
	  $(INC)ieeefp.h $(INC)sys/types.h math_private.h

COMMON_SRCS = e_rem_pio2.c e_rem_pio2f.c \
	k_cos.c k_cosf.c k_rem_pio2.c k_rem_pio2f.c k_sin.c k_sinf.c 	\
	k_standard.c k_tan.c k_tanf.c s_asinh.c s_cbrt.c s_cbrtf.c 	\
	s_erf.c s_erff.c s_expm1.c s_expm1f.c s_fabsf.c s_frexp.c 	\
	s_frexpf.c s_isinf.c s_isinff.c s_ldexpf.c s_lib_version.c 	\
	s_modff.c s_nextafter.c s_nextafterf.c s_tanh.c s_tanhf.c	\
	w_acos.c w_atanhf.c w_exp.c w_expf.c w_fmod.c w_fmodf.c w_hypot.c \
	w_hypotf.c w_j0.c w_log.c w_logf.c w_sqrt.c w_sqrtf.c w_pow.c 	\
	w_powf.c w_remainder.c w_remainderf.c w_scalb.c w_scalbf.c 

SRCS    = $(COMMON_SRCS) mathx.c 
ASMOBJS = i387math.o

INC 	= ../../include/
MKINC 	= ../../../usr/share/mk/

include ../../Ext.mk

# Add override flags here:
WFLAGS = -Wall -Wmissing-prototypes -Wshadow -Wcast-align -Winline \
	 -Wpointer-arith -Wmissing-declarations -Wstrict-prototypes \
# 	 -Wconversion -W 
