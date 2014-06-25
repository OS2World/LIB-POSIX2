/* ulsitem.h - definitions for use with the UniQueryLocaleItem
   function
    */
/* These have been guessed from output of the en_EN locale.  
   Corrections and additions welcome.  This file is part of
   ulsemx.zip; problems to 
	<john_fairhurst@iname.com>
  	*/
#ifndef _ULSITEM_H__
#define _ULSITEM_H__

#ifdef __cplusplus
extern "C" {
#endif

#ifndef LOCALEITEM_TYPE_DEFINED
 #define LOCALEITEM_TYPE_DEFINED
 typedef int *LocaleItem;
#endif

#define D_T_FMT ((LocaleItem)1)
#define D_FMT   ((LocaleItem)2)
#define T_FMT   ((LocaleItem)3)
#define T_FMT   ((LocaleItem)3)

#define AM_STR ((LocaleItem)4)
#define PM_STR ((LocaleItem)5)

#define ABDAY_1 ((LocaleItem)6)
#define ABDAY_2 ((LocaleItem)7)
#define ABDAY_3 ((LocaleItem)8)
#define ABDAY_4 ((LocaleItem)9)
#define ABDAY_5 ((LocaleItem)10)
#define ABDAY_6 ((LocaleItem)11)
#define ABDAY_7 ((LocaleItem)12)

#define DAY_1 ((LocaleItem)13)
#define DAY_2 ((LocaleItem)14)
#define DAY_3 ((LocaleItem)15)
#define DAY_4 ((LocaleItem)16)
#define DAY_5 ((LocaleItem)17)
#define DAY_6 ((LocaleItem)18)
#define DAY_7 ((LocaleItem)19)
                 
#define ABMON_1  ((LocaleItem)20)
#define ABMON_2  ((LocaleItem)21)
#define ABMON_3  ((LocaleItem)22)
#define ABMON_4  ((LocaleItem)23)
#define ABMON_5  ((LocaleItem)24)
#define ABMON_6  ((LocaleItem)25)
#define ABMON_7  ((LocaleItem)26)
#define ABMON_8  ((LocaleItem)27)
#define ABMON_9  ((LocaleItem)28)
#define ABMON_10 ((LocaleItem)29)
#define ABMON_11 ((LocaleItem)30)
#define ABMON_12 ((LocaleItem)31)
#define ABMON_13 ((LocaleItem) DONT_KNOW)

#define MON_1  ((LocaleItem)32)
#define MON_2  ((LocaleItem)33)
#define MON_3  ((LocaleItem)34)
#define MON_4  ((LocaleItem)35)
#define MON_5  ((LocaleItem)36)
#define MON_6  ((LocaleItem)37)
#define MON_7  ((LocaleItem)38)
#define MON_8  ((LocaleItem)39)
#define MON_9  ((LocaleItem)40)
#define MON_10 ((LocaleItem)41)
#define MON_11 ((LocaleItem)42)
#define MON_12 ((LocaleItem)43)
#define MON_13 ((LocaleItem) DONT_KNOW)

#define NOEXPR  ((LocaleItem)61) /* regexp */
#define NOSTR   ((LocaleItem)47)
#define YESEXPR ((LocaleItem)62) /* regexp */
#define YESSTR  ((LocaleItem)46)

#define DATESEP   ((LocaleItem)63)
#define TIMESEP   ((LocaleItem)64)
#define THOUSEP   ((LocaleItem)65)
#define RADIXCHAR ((LocaleItem)66)
#define LISTSEP   ((LocaleItem)67)

#define CRNCYSTR  ((LocaleItem)93)

#define CODESET   ((LocaleItem)109)

#define ERA         ((LocaleItem) DONT_KNOW)
#define ERA_T_FMT   ((LocaleItem) DONT_KNOW)
#define ERA_D_FMT   ((LocaleItem) DONT_KNOW)
#define ERA_D_T_FMT ((LocaleItem) DONT_KNOW)

#define ALT_DIGITS ((LocaleItem) DONT_KNOW)

#ifdef __cplusplus
}
#endif

#endif /* _ULSITEM_H__*/
