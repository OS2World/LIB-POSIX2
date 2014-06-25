/* uniapi.h - Top-level unicode API functions.  Includes everything else.  */
/* For best results, #include this file AFTER <os2.h> 
   Link with libuni.lib, uconv.lib, unikbd.lib. This file is part of 
   ulsemx.zip; problems to <john_fairhurst@iname.com>
   */

/* ensure we have toolkit headers */
#ifdef __EMX__
 #ifndef _OS2_H
  #warning Include <os2.h> fist !
  #include <os2.h>
 #endif
 #ifndef _SIZE_T
  #include <stddef.h>
 #endif
#else
 #ifndef OS2DEF_INCLUDED
  #include <os2def.h>
 #endif
 #ifndef __size_t
  #include <stddef.h>
 #endif
#endif

#include "unidef.h"
#include "uconv.h"
#include "unikbd.h"
#include "ulsitem.h"
