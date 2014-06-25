/* ulserr.h - error codes for use with Uni* functions 
   */
/* This file is part of ulsemx.zip; problems to
	 <john_fairhurst@iname.com>
  */
#ifndef _ULSERR_H__
#define _ULSERR_H__

#ifdef __cplusplus
extern "C" {
#endif

#define ULS_SUCCESS                0x00000000

#define ULS_BADATTR                0x00020415
#define ULS_BADHANDLE              0x00020408
#define ULS_BADOBJECT              0x0002040F
#define ULS_BUFFERFULL             0x00020412
#define ULS_ILLEGALSEQUENCE        0x00020402
#define ULS_INVALID                0x0002040E
#define ULS_INVALIDSCAN            0x0002040B
#define ULS_KBDNOTFOUND            0x00020407
#define ULS_MAXFILES               0x00020404
#define ULS_MAXFILESPERPROC        0x00020403
#define ULS_NODEAD                 0x00020409
#define ULS_NOMATCH                0x00020411
#define ULS_NOMEMORY               0x0002040D
#define ULS_NOOP                   0x00020405
#define ULS_NOSCAN                 0x0002040A
#define ULS_NOTIMPLEMENTED         0x0002040C
#define ULS_NOTOKEN                0x00020410
#define ULS_OTHER                  0x00020401
#define ULS_RANGE                  0x00020413
#define ULS_TOOMANYKBD             0x00020406
#define ULS_UNSUPPORTED            0x00020414
#define ULS_VERSION                0x00020416

#define UCONV_BADATTR              0x00020415
#define UCONV_E2BIG                0x00020412
#define UCONV_EBADF                0x0002040F
#define UCONV_EILSEQ               0x00020402
#define UCONV_EINVAL               0x0002040E
#define UCONV_EMFILE               0x00020403
#define UCONV_ENFILE               0x00020404
#define UCONV_ENOMEM               0x0002040D
#define UCONV_EOTHER               0x00020401
#define UCONV_NOTIMPLEMENTED       0x0002040C

#ifdef __cplusplus
}
#endif

#endif /* _ULSERR_H__*/
