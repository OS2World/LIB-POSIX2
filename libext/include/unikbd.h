/* unikbd.h - unicode keyboard access */
/* Programs wishing to use these functions should link with unikbd.lib      */
/* For best results, #include this file AFTER <os2.h>. This file is part of 
   ulsemx.zip; problems to <john_fairhurst@iname.com>
   */
#ifndef _UNIKBD_H__
#define _UNIKBD_H__

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

/* Unichar type defined in all uniapi headers */
#ifndef UNICHAR_TYPE_DEFINED
 #define UNICHAR_TYPE_DEFINED
 typedef USHORT UniChar;
#endif

/* uni error codes */
#include <ulserr.h>

/* translation table manipulation functions */

typedef unsigned int KHAND;
typedef UniChar      KBDNAME;

int APIENTRY UniCreateKeyboard( KHAND   *pkhand,
                                KBDNAME *pkName,
                                ULONG    ulMode);

int APIENTRY UniDestroyKeyboard( KHAND khand);

#define KBDF_DEFAULTVKEY    0x0001
#define KBDF_NOCTRLSHIFT    0x0002
#define KBDF_NOALTGR        0x0004
#define KBDF_SHIFTALTGR     0x0010
#define KBDF_DEADGOOD       0x0020
#define KBDF_DEADPRIVATE    0x0040
#define KBDF_SYSTEM         0x8000
#define KBDF_INTERNATIONAL  0x4000
#define KBDF_DVORAK         0x2000
#define KBDF_NATIONAL       0x1000
#define KBDF_LETTERKEYS     0x3000
#define KBDF_ISOKEYS        0x0800

typedef struct _KEYBOARDINFO
{
   ULONG   len;              /*  Length of structure. */
   USHORT  kbid;             /*  Keyboard architecture identifier. */
   USHORT  version;          /*  Version number. */
   BYTE    language[2];      /*  Normal language. */
   BYTE    country[2];       /*  Normal country. */
   USHORT  flags;            /*  Flags from KBDF_ */
   USHORT  resv;             /*  Reserved. */
   UniChar description[32];  /*  Description of keyboard. */
} KEYBOARDINFO;

typedef KEYBOARDINFO *PKEYBOARDINFO;

int APIENTRY UniQueryKeyboard( KHAND         khand,
                               PKEYBOARDINFO pKeyboardInfo);

/* Translation and state maintenance functions */

#define KBD_SHIFT       0x0001
#define KBD_CTRL        0x0002
#define KBD_ALT         0x0004
#define KBD_ALTGR       0x0008

#define KBD_SCROLLLOCK  0x0100
#define KBD_NUMLOCK     0x0200
#define KBD_CAPSLOCK    0x0400

typedef struct _USHIFTSTATE
{
   ULONG shift;      /*  Actual shift and lock state. */
   ULONG Effective;  /*  Effective shift and lock state. */
   ULONG Led;        /*  Keyboard indicators. */
} USHIFTSTATE;

typedef USHIFTSTATE *PUSHIFTSTATE;

typedef UCHAR VSCAN;

int APIENTRY UniUpdateShiftState( KHAND        khand,
                                  PUSHIFTSTATE pUShiftState,
                                  VSCAN        scScancode,
                                  BYTE         bMakeBreak);

int APIENTRY UniResetShiftState( KHAND        khand,
                                 PUSHIFTSTATE pUShiftState,
                                 ULONG        ulType);

typedef USHORT VDKEY;

int APIENTRY UniTranslateKey( KHAND    khand,
                              ULONG    ulEffectiveShift,
                              VSCAN    scScancode,
                              UniChar *pucsChar,
                              VDKEY   *pvdKey,
                              BYTE    *pbBiosScancode);

int APIENTRY UniTranslateDeadkey( KHAND    khand,
                                  VDKEY   *pvdDeadKey,
                                  UniChar  ucCharIn,
                                  UniChar *ucCharOut,
                                  VDKEY   *pvdNewDead);

int APIENTRY UniUntranslateKey( KHAND   khand,
                                UniChar ucChar,
                                VDKEY   vkKey,
                                VSCAN  *pscScancode,
                                ULONG  *pulEffectiveShift);

#ifdef __cplusplus
}
#endif

#endif /* _UNIKBD_H__*/
