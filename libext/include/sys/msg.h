/* libExt+gcc: sys/msg.h - $Id: msg.h,v 1.6 2000/10/11 14:48:55 arnd Exp $ */

/* Portions:
 * Copyright 1993 Daniel Boulet and RTMX Inc.
 * SVID compatible msg.h file
 * This system call was implemented by Daniel Boulet under contract from RTMX.
 * Author:  Daniel Boulet
 *
 * Redistribution and use in source forms, with and without modification,
 * are permitted provided that this entire comment appears intact.
 *
 * Redistribution in binary form may occur without any restrictions.
 * Obviously, it would be nice if you gave credit where credit is due
 * but requiring it would be too onerous.
 *
 * This software is provided ``AS IS'' without any warranties of any kind.
 */
#ifndef _SYS_MSG_H_
#define _SYS_MSG_H_

#include <sys/ipc.h>

#if defined(__cplusplus)
extern "C" {
#endif

/*
 * The MSG_NOERROR identifier value, the msqid_ds struct and the msg struct
 * are as defined by the SV API Intel 386 Processor Supplement.
 */
#define MSG_NOERROR	010000	/* don't complain about too long msgs */
#define MSG_EXCEPT	020000	/* recv any msg except of specified type */

/* Structure of record for one message inside the kernel. */
struct msqid_ds {
	struct ipc_perm	msg_perm;	/* msg queue permission bits */
	struct msg	*msg_first;	/* first message in the queue */
	struct msg	*msg_last;	/* last message in the queue */
	unsigned long	msg_cbytes;	/* number of bytes in use on the queue */
	unsigned long	msg_qnum;	/* number of msgs in the queue */
	unsigned long	msg_qbytes;	/* max # of bytes on the queue */
	pid_t		msg_lspid;	/* pid of last msgsnd() */
	pid_t		msg_lrpid;	/* pid of last msgrcv() */
	time_t		msg_stime;	/* time of last msgsnd() */
	long		msg_pad1;
	time_t		msg_rtime;	/* time of last msgrcv() */
	long		msg_pad2;
	time_t		msg_ctime;	/* time of last msgctl() */
	long		msg_pad3;
	long		msg_pad4[4];
};

/* ipcs ctl commands */
#define MSG_STAT 11
#define MSG_INFO 12

#ifdef _KERNEL
struct omsqid_ds {
	struct ipc_perm msg_perm;	/* msg queue permission bits */
	struct msg	*msg_first;	/* first message in the queue */
	struct msg	*msg_last;	/* last message in the queue */
	unsigned long	msg_cbytes;	/* number of bytes in use on the queue */
	unsigned long	msg_qnum;	/* number of msgs in the queue */
	unsigned long	msg_qbytes;	/* max # of bytes on the queue */
	pid_t		msg_lspid;	/* pid of last msgsnd() */
	pid_t		msg_lrpid;	/* pid of last msgrcv() */
	time_t		msg_stime;	/* time of last msgsnd() */
	long		msg_pad1;
	time_t		msg_rtime;	/* time of last msgrcv() */
	long		msg_pad2;
	time_t		msg_ctime;	/* time of last msgctl() */
	long		msg_pad3;
	long		msg_pad4[4];
};
#endif

struct msg {
	struct msg	*msg_next;	/* next msg in the chain */
	long		msg_type;	/* type of this message */
    					/* >0 -> type of this message */
	    				/* 0 -> free header */
	unsigned short	msg_ts;		/* size of this message */
	short		msg_spot;	/* location of start of msg in buffer */
};

#ifdef _KERNEL
/*
 * Based on the configuration parameters described in an SVR2(yes, two)
 * config(1m) man page.
 *
 * Each message is broken up and stored in segments that are msgssz bytes
 * long.  For efficiency reasons, this should be a power of two.  Also,
 * it doesn't make sense if it is less than 8 or greater than about 256.
 * Consequently, msginit in kern/sysv_msg.c checks that msgssz is a power of
 * two between 8 and 1024 inclusive(and panic's if it isn't).
 */
struct _msginfo {
	int	msgmax,		/* max chars in a message */
		msgmni,		/* max message queue identifiers */
		msgmnb,		/* max chars in a queue */
		msgtql,		/* max messages in system */
		msgssz,		/* size of a message segment(see notes above) */
		msgseg;		/* number of message segments */
};
#define msginfo	 __Param_msginfo  /* see param.c */
extern struct _msginfo	msginfo;

#ifndef MSGSSZ
#define MSGSSZ	8		/* Each segment must be 2^N long */
#endif
#ifndef MSGSEG
#define MSGSEG	2048		/* must be less than 32767 */
#endif
#undef MSGMAX			/* ALWAYS compute MGSMAX! */
#define MSGMAX	(MSGSSZ*MSGSEG)
#ifndef MSGMNB
#define MSGMNB	2048		/* max # of bytes in a queue */
#endif
#ifndef MSGMNI
#define MSGMNI	40
#endif
#ifndef MSGTQL
#define MSGTQL	40
#endif

/*
 * macros to convert between msqid_ds's and msqid's.
 *(specific to this implementation)
 */
#define MSQID(ix,ds)	((ix) & 0xffff |(((ds).msg_perm.seq << 16) & 0xffff0000))
#define MSQID_IX(id)	((id) & 0xffff)
#define MSQID_SEQ(id)	(((id) >> 16) & 0xffff)
#endif

/* The following System V style IPC functions implement a message queue
   system.  The definition is found in XPG2.
   */
/*
 * Structure describing a message.  The SVID doesn't suggest any
 * particular name for this structure.  There is a reference in the
 * msgop man page that reads "The structure mymsg is an example of what
 * this user defined buffer might look like, and includes the following
 * members:".  This sentence is followed by two lines equivalent
 * to the mtype and mtext field declarations below.  It isn't clear
 * if "mymsg" refers to the name of the structure type or the name of an
 * instance of the structure...
 */

#if 0
/* Template for struct to be used as argument for `msgsnd' and `msgrcv' */
struct msgbuf {
  long mtype;		/* type of received/sent message */
  char mtext[1];	/* text of the message */
};
#endif

extern int msgctl(int,int,struct msqid_ds*); 	   /* Message queue control */
extern int msgget(key_t,int); 			   /* Get messages queue */
extern int msgsnd(int,__const__ void*,size_t,int); /* Send msg to queue */
extern int msgrcv(int,void*,size_t,long,int); 	   /* Receive msg from queue */

#if defined(__cplusplus)
}
#endif

#endif /* !_SYS_MSG_H_ */
