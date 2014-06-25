/* rev.c (libext+gcc) - statical values:  libextensions revision index
 * $Id: rev.c,v 1.6 2000/11/26 16:43:01 arnd Exp $
 */

#include <stdlib.h>

#include "../../rev.h"

const unsigned int _libExtensions_vcmp    = _LIBEXTNS_REV;
const unsigned int _libExtensions_rev     = _LIBEXTNS_REV;
const char         _libExtensions_vprt[]  = _LIBEXTNS_VPRT;

const char __Rev_ostype[] = "Posix Extensions for OS/2 WARP";
const char __Rev_osrelease[] = _LIBEXTNS_VPRT;
const char __Rev_osversion[] = "libext#" _LIBEXTNS_VPRT "xyz";
const char sccs[8] = { ' ', ' ', ' ', ' ', '@', '(', '#', ')' };
const char __Rev_version[] = "Posix for OS/2 WARP"
_LIBEXTNS_VPRT "(libExt) $Id: rev.c,v 1.6 2000/11/26 16:43:01 arnd Exp $\n\
    arnd@loopback:/OpenBSD/sys/conf\n";

