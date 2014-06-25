/* uniext.h - Extra non-standard bits and pieces  */
/* Use at your own risk. For best results, #include this file AFTER <os2.h>.
   This file is part of ulsemx.zip; problems to <john_fairhurst@iname.com>
   */
#ifndef _UNIEXT_H__
#define _UNIEXT_H__

#ifdef __cplusplus
extern "C" {
#endif

/* ensure we have toolkit headers */
#ifdef __EMX__
 #ifndef _OS2_H
  #include <os2.h>
 #endif
#else
 #ifndef OS2DEF_INCLUDED
  #include <os2def.h>
 #endif
#endif

#ifndef LOCALEITEM_TYPE_DEFINED
 #define LOCALEITEM_TYPE_DEFINED
 typedef int *LocaleItem;
#endif

#define COUNTRYCODE  ((LocaleItem)108)  /* os/2 countrycode */
#define ULS_CODEPAGE ((LocaleItem)109)  /* eg. iso8859-1 */
#define OS2_CODEPAGE ((LocaleItem)111)  /* eg. 850 */
#define ALTCODEPAGE  ((LocaleItem)112)  /* alternative os/2 cp ? */
#define WINCODEPAGE  ((LocaleItem)110)  /* win32-compatible cp */
#define MACCODEPAGE  ((LocaleItem)113)  /* applemac-compat. cp */

#define SHORTLANGNAME    ((LocaleItem)101) /* eg. de */
#define SHORTCTRYNAME    ((LocaleItem)102) /* eg. DE */
#define EN_LANGUAGENAME  ((LocaleItem)103) /* eg. German */
#define LANGUAGENAME     ((LocaleItem)104) /* eg. Deutsch */
#define EN_COUNTRYNAME   ((LocaleItem)105) /* eg. Germany */
#define COUNTRYNAME      ((LocaleItem)106) /* eg. Deutschland */

#ifdef __cplusplus
}
#endif

#endif /* _UNIEXT_H__*/
