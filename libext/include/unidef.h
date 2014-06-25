/* unidef.h - Main unicode API functions */
/* Programs wishing to use these functions should link with libuni.lib
   For best results, #include this file AFTER <os2.h>. This file is part of 
   ulsemx.zip; problems to <john_fairhurst@iname.com>
   */
#ifndef _UNIDEF_H__
#define _UNIDEF_H__

#ifdef __cplusplus
extern "C" {
#endif

/* ensure we have toolkit headers */
  #include <os2.h>
 #ifndef _SIZE_T
  #include <stddef.h>
 #endif
 #define _LOCALE_H

/* Unichar type defined in all uniapi headers */
#ifndef UNICHAR_TYPE_DEFINED
 #define UNICHAR_TYPE_DEFINED
 typedef USHORT UniChar;
#endif

/* uni error codes */
#include <ulserr.h>

/* Locale categories - <locale.h> may already been included */
#ifdef LC_ALL
 #undef LC_ALL     
 #undef LC_COLLATE 
 #undef LC_CTYPE   
 #undef LC_MONETARY
 #undef LC_NUMERIC 
 #undef LC_TIME    
 #undef LC_MESSAGES
#endif

#define LC_ALL     (-1)
#define LC_COLLATE   0
#define LC_CTYPE     1
#define LC_MONETARY  2
#define LC_NUMERIC   3
#define LC_TIME      4
#define LC_MESSAGES  5

/* Locale creation and managment */

int APIENTRY UniMapCtryToLocale( ULONG    ulCountryCode,
                                 UniChar *ucsLocaleName, /*out*/
                                 size_t   cchBuffer);
/* LocaleSpec type IDs */
#define UNI_TOKEN_POINTER      1
#define UNI_MBS_STRING_POINTER 2
#define UNI_UCS_STRING_POINTER 3

typedef VOID *LocaleObject;

int APIENTRY UniCreateLocaleObject( int           iLocaleSpecType,
                                    const VOID   *pvLocaleSpec,
                                    LocaleObject *pLocaleObject); /*out*/

int APIENTRY UniFreeLocaleObject( LocaleObject localeObject);

int APIENTRY UniQueryLocaleObject( const LocaleObject localeObject,
                                   int                iCategory,
                                   int                iLocaleSpecType,
                                   VOID             **ppLocaleName); /*out*/

int APIENTRY UniFreeMem( VOID *pMemory);

typedef unsigned int LocaleToken;

int APIENTRY UniLocaleStrToToken( int          iLocaleStringType,
                                  const VOID  *pvLocaleString,
                                  LocaleToken *pLocaleToken); /*out*/

int APIENTRY UniLocaleTokenToStr( const LocaleToken localeToken,
                                  int               iLocaleStringType,
                                  VOID            **ppLocaleString); /*out*/
/* Locale info */
typedef struct _UniLconv
{
   UniChar  *decimal_point;     /* Non-monetary decimal point. */
   UniChar  *thousands_sep;     /* Non-monetary thousands' separator. */
   SHORT    *grouping;          /* Size of each group of digits in non-monetary quantities. */
   UniChar  *int_curr_symbol;   /* International currency symbol and separator. */
   UniChar  *currency_symbol;   /* Local currency symbol. */
   UniChar  *mon_decimal_point; /* Monetary decimal point. */
   UniChar  *mon_thousands_sep; /* Monetary thousands' separator. */
   SHORT    *mon_grouping;      /* Size of each group of digits in monetary quantities. */
   UniChar  *positive_sign;     /* Non-negative values sign. */
   UniChar  *negative_sign;     /* Negative values sign. */
   SHORT     int_frac_digits;   /* Number of fractional digits for international currency. */
   SHORT     frac_digits;       /* Number of fractional digits for local currency. */
   SHORT     p_cs_precedes;     /* Non-negative currency symbol: 1 - precedes; 0 - succeeds. */
   SHORT     p_sep_by_space;    /* Non-negative currency symbol: 1 - space; 0 - no space. */
   SHORT     n_cs_precedes;     /* Negative currency symbol: 1 - precedes; 0 - succeeds. */
   SHORT     n_sep_by_space;    /* Negative currency symbol: 1 - space; 0 - no space. */
   SHORT     p_sign_posn;       /* Positioning of non-negative monetary sign. */
   SHORT     os2_mondecpt;      /* OS/2 currency symbol positioning. */
   UniChar  *debit_sign;        /* Non-negative valued debit monetary symbol. */
   UniChar  *credit_sign;       /* Negative valued credit monetary symbol. */
   UniChar  *left_parenthesis;  /* Negative valued left parenthesis monetary symbol. */
   UniChar  *right_parenthesis; /* Negative valued right parenthesis monetary symbol. */
} UniLconv;

typedef UniLconv *pUniLconv;

int APIENTRY UniQueryLocaleInfo( const LocaleObject   localeObject,
                                 UniLconv           **ppUniLconv); /*out*/

int APIENTRY UniFreeLocaleInfo( UniLconv *pUniLconv);

/* locale entries (defined in ulsitem.h) */

#ifndef LOCALEITEM_TYPE_DEFINED
 #define LOCALEITEM_TYPE_DEFINED
 typedef int *LocaleItem;
#endif

int APIENTRY UniQueryLocaleItem( const LocaleObject localeObject,
                                 LocaleItem         localeItem,
                                 UniChar          **ppUcsString); /*out*/

/* unicode character attribute api */

typedef VOID *AttrObject;

#define ATTR_ALNUM     ((const UniChar*)L"alnum")
#define ATTR_ALPHA     ((const UniChar*)L"alpha")
#define ATTR_BLANK     ((const UniChar*)L"blank")
#define ATTR_CHARACTER ((const UniChar*)L"character")
#define ATTR_CNTRL     ((const UniChar*)L"cntrl")
#define ATTR_DIGIT     ((const UniChar*)L"digit")
#define ATTR_GRAPH     ((const UniChar*)L"graph")
#define ATTR_LOWER     ((const UniChar*)L"lower")
#define ATTR_PRINT     ((const UniChar*)L"print")
#define ATTR_PUNCT     ((const UniChar*)L"punct")
#define ATTR_SPACE     ((const UniChar*)L"space")
#define ATTR_UPPER     ((const UniChar*)L"upper")
#define ATTR_XDIGIT    ((const UniChar*)L"xdigit")

/* following only valid with UniScanForAttr */
#define ATTR_CSID      ((const UniChar*)L"csid")
#define ATTR_HYPHENLOC ((const UniChar*)L"hyphenloc")
#define ATTR_LINEBREAK ((const UniChar*)L"linebreak")
#define ATTR_PARAGRAPH ((const UniChar*)L"paragraph")
#define ATTR_SENTENCE  ((const UniChar*)L"sentence")
#define ATTR_WORD      ((const UniChar*)L"word")

int APIENTRY UniCreateAttrObject( const LocaleObject localeObject,
                                  const UniChar     *ucsAttrName,
                                  AttrObject        *pAttrObject); /*out*/

int APIENTRY UniFreeAttrObject( AttrObject attrObject);

int APIENTRY UniQueryCharAttr( AttrObject attrObject,
                               UniChar    ucCharacter);

#ifndef ULSBOOL_TYPE_DEFINED
 #define ULSBOOL_TYPE_DEFINED
 typedef int ulsBool;
#endif

int APIENTRY UniScanForAttr( AttrObject     attrObject,
                             const UniChar *ucsString,
                             size_t         cNumElems,
                             ulsBool        bInverseOp,
                             size_t        *pcOffset); /*out*/

/* transformation functions */

typedef VOID *XformObject;

#define XFORM_LOWER ((const UniChar*)L"lower")
#define XFORM_UPPER ((const UniChar*)L"upper")

int APIENTRY UniCreateTransformObject( const LocaleObject localeObject,
                                       const UniChar     *ucsXformType,
                                       XformObject       *pXformObject); /*out*/

int APIENTRY UniFreeTransformObject( XformObject xform_object);

int APIENTRY UniTransformStr( XformObject    xform_object,
                              const UniChar *ucsInputBuf,
                              int           *cInputLen,    /*out*/
                              UniChar       *ucsOutputBuf, /*out*/
                              int           *cOutputLen);  /*out*/

/* <ctype.h>-like simple attribute and transform shortcuts */

int APIENTRY UniQueryAlnum( const LocaleObject localeObject,
                            UniChar            ucCharacter);

int APIENTRY UniQueryAlpha( const LocaleObject localeObject,
                            UniChar            ucCharacter);

int APIENTRY UniQueryBlank( const LocaleObject localeObject,
                            UniChar            ucCharacter);

int APIENTRY UniQueryCntrl( const LocaleObject localeObject,
                            UniChar            ucCharacter);

int APIENTRY UniQueryDigit( const LocaleObject localeObject,
                            UniChar            ucCharacter);

int APIENTRY UniQueryGraph( const LocaleObject localeObject,
                            UniChar            ucCharacter);

int APIENTRY UniQueryLower( const LocaleObject localeObject,
                            UniChar            ucCharacter);

int APIENTRY UniQueryPrint( const LocaleObject localeObject,
                            UniChar            ucCharacter);

int APIENTRY UniQueryPunct( const LocaleObject localeObject,
                            UniChar            ucCharacter);

int APIENTRY UniQuerySpace( const LocaleObject localeObject,
                            UniChar            ucCharacter);

int APIENTRY UniQueryUpper( const LocaleObject localeObject,
                            UniChar            ucCharacter);

int APIENTRY UniQueryXDigit( const LocaleObject localeObject,
                             UniChar            ucCharacter);

UniChar APIENTRY UniTransLower( const LocaleObject localeObject,
                                UniChar            ucCharacter);

UniChar APIENTRY UniTransUpper( const LocaleObject localeObject,
                                UniChar            ucCharacter);

UniChar APIENTRY UniTolower( UniChar ucCharacter);

UniChar APIENTRY UniToupper( UniChar ucCharacter);

/* UniChar correct & locale-sensitive <string.h> replacements */

UniChar *APIENTRY UniStrcat( UniChar       *ucsString1, /*out*/
                             const UniChar *ucsString2);

UniChar *APIENTRY UniStrchr( const UniChar *ucsString1,
                             UniChar        ucCharacter);

int APIENTRY UniStrcmp( const UniChar *ucsString1,
                        const UniChar *ucsString2);

int APIENTRY UniStrcmpi( const LocaleObject  localeObject,
                         const UniChar      *ucsString1,
                         const UniChar      *ucsString2);

int APIENTRY UniStrcoll( const LocaleObject  localeObject,
                         const UniChar      *ucsString1,
                         const UniChar      *ucsString2);

UniChar *APIENTRY UniStrcpy( UniChar       *ucsString1, /*out*/
                             const UniChar *ucsString2);

size_t APIENTRY UniStrcspn( const UniChar *ucsString1,
                            const UniChar *ucsString2);

size_t APIENTRY UniStrlen( const UniChar *ucsString);

UniChar *APIENTRY UniStrlwr( UniChar *ucsString); /*out*/

UniChar *APIENTRY UniStrncat( UniChar       *ucsString1, /*out*/
                              const UniChar *ucsString2,
                              size_t         cbMaxCat);

int APIENTRY UniStrncmp( const UniChar *ucsString1,
                         const UniChar *ucsString2,
                         size_t         cbMaxCopy);

int APIENTRY UniStrncmpi( const LocaleObject  localeObject,
                          const UniChar      *ucsString1,
                          const UniChar      *ucsString2,
                          size_t              cbMaxCopy);

UniChar *APIENTRY UniStrncpy( UniChar       *ucsString1, /*out*/
                              const UniChar *ucsString2,
                              size_t         cbMaxCopy);

UniChar *APIENTRY UniStrpbrk( const UniChar *ucsString1,
                              const UniChar *ucsString2);

UniChar *APIENTRY UniStrrchr( const UniChar *ucsString1,
                              UniChar        ucCharacter);

size_t APIENTRY UniStrspn( const UniChar *ucsString1,
                           const UniChar *ucsString2);

UniChar *APIENTRY UniStrstr( const UniChar *ucsString1,
                             const UniChar *ucsString2);

UniChar *APIENTRY UniStrtok( UniChar *ucsString1, /*out (note cannot be NULL)*/
                             UniChar *ucsString2); /* I think this should be const, but docs say no. */

size_t APIENTRY UniStrxfrm( const LocaleObject  localeObject,
                            UniChar            *ucsString1, /*out*/
                            const UniChar      *ucsString2,
                            size_t              cBuffSize);

/* Other C-library replacement functions */

int APIENTRY UniStrfmon( const LocaleObject  localeObject,
                         UniChar            *ucsBuffer, /*out*/
                         size_t              cBuffSize,
                         const UniChar      *ucsFormat,
                         ...);

#include <time.h>

size_t APIENTRY UniStrftime( const LocaleObject  localeObject,
                             UniChar            *ucsBuffer, /*out*/
                             size_t              cBuffSize,
                             const UniChar      *ucsFormat,
                             const struct tm    *pTmTime);

UniChar *APIENTRY UniStrptime( const LocaleObject  localeObject,
                               const UniChar      *ucsBuffer,
                               const UniChar      *ucsFormat,
                               struct tm          *pTmTime); /*out*/

int APIENTRY UniStrtod( const LocaleObject  localeObject,
                        const UniChar      *ucsString,
                        UniChar           **pucsEnd, /*out*/
                        double             *pfValue); /*out*/

int APIENTRY UniStrtol( const LocaleObject  localeObject,
                        const UniChar      *ucsString,
                        UniChar           **pucsEnd, /*out*/
                        int                 iBase,
                        long int           *plValue); /*out*/

int APIENTRY UniStrtoul( const LocaleObject  localeObject,
                         const UniChar      *ucsString,
                         UniChar           **pucsEnd, /*out*/
                         int                 iBase,
                         unsigned long int  *pulValue); /*out*/

#ifdef __cplusplus
}
#endif

#endif /* _UNIDEF_H__*/
