/* uconv.h - conversion between encodings */
/* Programs wishing to use these functions should link with uconv.lib 
   For best results, #include this file AFTER <os2.h>  This file is part
   of ulsemx.zip; problems to <john_fairhurst@iname.com>
   */
#ifndef _UCONV_H__
#define _UCONV_H__

#ifdef __cplusplus
extern "C" {
#endif

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

/* Unichar type defined in all uniapi headers */
#ifndef UNICHAR_TYPE_DEFINED
 #define UNICHAR_TYPE_DEFINED
 typedef USHORT UniChar;
#endif

/* uni error codes */
#include <ulserr.h>

typedef VOID *UconvObject;

/* main converter functions */

int APIENTRY UniMapCpToUcsCp( ULONG    ulCodepage,
                              UniChar *ucsCodepage,
                              size_t   cBufflen);

int APIENTRY UniCreateUconvObject( UniChar     *ucsCodepage,
                                   UconvObject *uconv_object);

int APIENTRY UniFreeUconvObject( UconvObject uconv_object);

int APIENTRY UniUconvToUcs( UconvObject   uconv_object,
                            VOID        **ppvInBuff,
                            size_t       *pcBuffsize,
                            UniChar     **pucsOutBuff,
                            size_t       *pcCharsLeft,
                            size_t       *pcNonidentical);

int APIENTRY UniUconvFromUcs( UconvObject   uconv_object,
                              UniChar     **pucsInBuff,
                              size_t       *pcBuffsize,
                              VOID        **pOutBuff,
                              size_t       *pcBytesLeft,
                              size_t       *pcNonidentical);

/* access to converter internals */

typedef struct _conv_endian_t
{
   USHORT     source;  /*  Source information. */
   USHORT     target;  /*  Target information. */
} conv_endian_t;

typedef conv_endian_t *pconv_endian_t;

typedef struct _uconv_attribute_t 
{
   ULONG         version;      /*  Version (must be zero). Settable. */
   CHAR          mb_min_len;   /*  Minimum character size. */
   CHAR          mb_max_len;   /*  Maximum character size. */
   CHAR          usc_min_len;  /*  UCS minimum character size. */
   CHAR          usc_max_len;  /*  UCS maximum character size. */
   USHORT        esid;         /*  Encoding scheme ID. */
   CHAR          options;      /*  Substitution options. Settable. */
   CHAR          state;        /*  Current state. Settable. */
   conv_endian_t endian;       /*  Source and target chain. Settable. */
   ULONG         displaymask;  /*  Display and data mask. Settable. */
   ULONG         converttype;  /*  Conversion type. Settable. */
   USHORT        subchar_len;  /*  MBCS sub-character length. Settable. */
   USHORT        subuni_len;   /*  Unicode sub-character length. Settable. */
   CHAR          subchar[16];  /*  MBCS subcharacters. Settable. */
   UniChar       subuni[8];    /*  Unicode subcharacters. Settable. */
} uconv_attribute_t;

typedef uconv_attribute_t *puconv_attribute_t;

typedef struct _udcrange_t
{
   USHORT     first;  /*  First code point. */
   USHORT     last;   /*  Last code point. */
} udcrange_t;

typedef udcrange_t *pudcrange_t;

int APIENTRY UniQueryUconvObject( UconvObject        uconv_object,
                                  uconv_attribute_t *pUconvAttr,
                                  size_t             cbAttrBuff,
                                  CHAR               achFirst[256],
                                  CHAR               achOther[256],
                                  udcrange_t         arange[32]);

int APIENTRY UniSetUconvObject( UconvObject        uconv_object,
                                uconv_attribute_t *pUconvAttr);

#ifdef __cplusplus
}
#endif

#endif /* _UCONV_H__*/
