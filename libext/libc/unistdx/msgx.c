/* msgx.c (libExt+gcc) - $Id: msgx.c,v 1.5 2000/11/21 18:24:22 arnd Exp $ */

/* msgget,msgsnd,msgrcv by Stauff__  Ver 0.2A 

   Changes: Implemented partly UNIX msg* structs,msgctl(..)    
   TODO: Implement real msgget between 2 different apps,  
   */

#define INCL_DOSQUEUES   /* DOS Queue values */
#define INCL_DOSPROCESS  /* DOS thread and process values */
#define INCL_DOSERRORS   /* DOS error values */
#include <os2.h>

#define _KERNEL		 	/* sys/msg.h definitions */
#include <sys/types.h> 
#include <sys/ipc.h> 
#include <sys/msg.h> 
#include <sys/stat.h> 
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/errnox.h>

struct tmsgbuf {
  long msg;
  void *ptr;
};

struct _msg_libext {
   struct msqid_ds *msgid_ptr; /* Pointer to kernel msg record structure */
   int key_used;	       /* Key used */  	
   struct tmsgbuf *tmsgptr;    /* Pointer to array of shared memory objects */
   int asize;                  /* Size of array of shared memory objects */
   struct _msg_libext *pnext;  /* Next msgarray */
};

/* global start pointer to msg queue */
struct _msg_libext *__msgx_globMsgPtr;	

	extern int 			/* msgget - get message queue */
msgget(key_t key, int flags) 
{
 char szQueName[50] = "\\QUEUES\\EMX.";
 char buffer[10];
 HQUEUE hqSpecialQue = NULLHANDLE; /* Queue handle     */
 PID  pidOwner     = 0;
 APIRET rc    = NO_ERROR;   /* Return code */

	strcat(szQueName,(char*)_itoa(key,buffer,10));
	if (flags==0)	{
		_dPrintf("Checking %s\n",szQueName);	
		rc = DosOpenQueue(&pidOwner,   /* PID of queue owner*/
				   &hqSpecialQue,     /* Handle for created queue */
				     szQueName); /* Name of the queue to open*/
		if (rc!= NO_ERROR) {
			_dPrintf("nmsgget(): DosOpenQueue error: Return code = %lu\n",rc);
			errno = _rc2Errno(rc);  return(-1);
		}
	}
	else {
		_dPrintf("Creating %s\n",szQueName);	
		rc = DosCreateQueue(&hqSpecialQue,    /* Queue handle */
				QUE_FIFO,     /* Convert 16-bit addresses to 32  */
				szQueName);   /* Name of the queue to create     */
		if (rc!= NO_ERROR) {
			_dPrintf("nmsgget():DosCreateQueue error: return code = %lu\n",rc);
			errno = _rc2Errno(rc); return(-1); 
		} 
		
		if (!__msgx_globMsgPtr) {
			__msgx_globMsgPtr =
					(struct _msg_libext*) malloc(sizeof(struct _msg_libext)); 
			__msgx_globMsgPtr->key_used=key;
			__msgx_globMsgPtr->pnext=NULL;     
			__msgx_globMsgPtr->msgid_ptr =
						(struct msqid_ds*)malloc(sizeof(struct msqid_ds));
			__msgx_globMsgPtr->tmsgptr = NULL;
		}   
		else {
		 struct _msg_libext *temp_msg_ptr = __msgx_globMsgPtr;
			
			while (temp_msg_ptr->pnext) 
				temp_msg_ptr = temp_msg_ptr->pnext;

			temp_msg_ptr->pnext = (struct _msg_libext*)
								malloc(sizeof(struct _msg_libext));
			temp_msg_ptr->pnext->key_used = key;
			temp_msg_ptr->pnext->pnext = NULL;
			temp_msg_ptr->pnext->msgid_ptr =
					(struct msqid_ds*)malloc(sizeof(struct msqid_ds));
			temp_msg_ptr->pnext->tmsgptr = NULL;
		}
	}
	return(key);
}

	extern int 
msgsnd(int qkey,const void *messbuf, size_t msgsize, int flags)
{
 char szQueName[50]	=		"\\QUEUES\\EMX.";
 char szMemName[50]	=		"\\SHAREMEM\\EMX.";
 char	buffer[10];
 HQUEUE hqSpecialQue =	NULLHANDLE; /* Queue handle     */
 PID  pidOwner	= 0;
 APIRET rc			= NO_ERROR;   /* Return code */
 PVOID  pObject	= NULL;     /* Pointer to shared memory object */
 static long msg_calls = 20; /* Magic Number ;) */
 int z;
 struct _msg_libext *temp_msg_ptr = __msgx_globMsgPtr;  
 
	if (__msgx_globMsgPtr == NULL) {
		_dPuts("msgsnd(): PANIC! Main structure not initialized!\n");
		errno = _rc2Errno(rc); return(-1);
	}    
	while (temp_msg_ptr->key_used != qkey && temp_msg_ptr->pnext != NULL)
		temp_msg_ptr = temp_msg_ptr->pnext; 

	if (temp_msg_ptr->key_used==qkey) { 
			if (!temp_msg_ptr->tmsgptr) {
				temp_msg_ptr->tmsgptr=(struct tmsgbuf*)malloc(sizeof(struct tmsgbuf));
				temp_msg_ptr->asize=0;     
			}
			else  {
				++temp_msg_ptr->asize;
				temp_msg_ptr->tmsgptr = (struct tmsgbuf*)realloc(temp_msg_ptr->tmsgptr,
					 (temp_msg_ptr->asize + 1) * sizeof(struct tmsgbuf));
		 }
	}  
	else  {    
		_dPrintf("\nmsgsnd():PANIC! id structure %d not found!\n",qkey);
		errno = _rc2Errno(rc); return(-1); 
	} 
	strcat(szQueName,(char*)_itoa(qkey,buffer,10));
	strcat(szMemName,(char*)_itoa(qkey,buffer,10));
	strcat(szMemName,(char*)_itoa(msg_calls,buffer,10));   
		
	msgsize += sizeof(long);	/* struct tmsgbuf is (long+data) */
		
	rc = DosAllocSharedMem(&pObject,/* Pointer to object pointer     */
	szMemName, /* Name for shared memory */
	msgsize, /* Desired size of object */
	PAG_COMMIT |    /* Commit memory   */
	PAG_WRITE );    /* Allocate memory as read/write */
		
	if (rc != NO_ERROR) {
		_dPrintf("msgsnd():DosAllocSharedMem error:  return code = %lu\n",rc);
		errno = _rc2Errno(rc); return(-1);
	}
	memcpy(pObject,messbuf,msgsize);
	temp_msg_ptr->tmsgptr[temp_msg_ptr->asize].ptr=pObject;    
	temp_msg_ptr->tmsgptr[temp_msg_ptr->asize].msg=msg_calls; 

	_dPrintf("----msgsnd---Sending key %ld\n", msg_calls);	
	_dPrintf("size is %ld. ", msgsize); _dPrintf("Queue is %d.\n", qkey);
	
	rc = DosOpenQueue (&pidOwner,  /* PID of queue owner*/
				&hqSpecialQue,     /* Handle for created queue */
				szQueName); /* Name of the queue to open*/
	
	if (rc!= NO_ERROR) {
		_dPrintf ("\nmsgsnd():DosOpenQueue error: Return code = %lu\n",rc);
		errno = _rc2Errno(rc); return(-1);
	}
	rc = DosWriteQueue (hqSpecialQue, /* Queue to write to    */
		 (ULONG)msgsize, /* Request data  */
	 		sizeof(long), /* Length of data to write     */
	 		(PVOID)msg_calls++,  /* Pointer to data */
	 			0L);   /* Priority (not applicable here) */
	if (rc!= NO_ERROR) {
		_dPrintf ("msgsnd():DosWriteQueue error: return code = %lu\n", rc);
		errno = _rc2Errno(rc); return(-1);
	}
	for (z=0; z <= temp_msg_ptr->asize; z++) {
	 struct tmsgbuf *rtmpmsg; 
		if (strcmp((char*)temp_msg_ptr->tmsgptr[z].ptr,
					_itoa(temp_msg_ptr->tmsgptr[z].msg,buffer,10))==0) {
			_dPrintf("Deleting delting message %ld", temp_msg_ptr->tmsgptr[z].msg);
			_dPrintf("from queue %d\n", qkey);

			rtmpmsg = (struct tmsgbuf*)
				malloc(temp_msg_ptr->asize*sizeof(struct tmsgbuf));
			memcpy(rtmpmsg,temp_msg_ptr->tmsgptr,z*sizeof(struct tmsgbuf));
			memcpy(rtmpmsg+z,temp_msg_ptr->tmsgptr+1,
					(temp_msg_ptr->asize-z)*sizeof(struct tmsgbuf));
					
			rc = DosFreeMem(temp_msg_ptr->tmsgptr[z--].ptr);
			if (rc != NO_ERROR) {
				_dPrintf("\nmsgrcv():DosFreeMem error:  return code = %lu\n",rc);
				errno = _rc2Errno(rc); return(-1);
			}
			free(temp_msg_ptr->tmsgptr);  
			temp_msg_ptr->tmsgptr=rtmpmsg; 
			temp_msg_ptr->asize--;
		}
	}  
	return 0;     
}
  

	extern int 
msgrcv(int qkey, void *msgp, size_t msgsz, long int __msgtyp, int msgflg)
{ 
 char szQueName[50]  = "\\QUEUES\\EMX.";
 char szMemName[50]  = "\\SHAREMEM\\EMX.";    
 char	    buffer[10];
 HQUEUE hqSpecialQue = NULLHANDLE; /* Queue handle     */
 long DataBuffer     = 0;   /* Data buffer for queue data     */
 REQUESTDATA Request = {0}; /* Reques */
 PID  pidOwner       = 0;
 ULONG ulDataLen     = 0;   /* Length of data returned */
 BYTE ElemPrty       = 0;   /* Priority of element (returned) */
 APIRET rc           = NO_ERROR;   /* Return code */
 PVOID pObject       = NULL;     /* Pointer to shared memory object */    
 ULONG ulNumElems;
    
	strcat(szQueName,(char*)_itoa(qkey,buffer,10));
 
	rc = DosOpenQueue(&pidOwner,  /* PID of queue owner */
			  &hqSpecialQue,    /* Handle for created queue */
  				szQueName); 		/* Name of the queue to open*/
    
	if (rc!= NO_ERROR) {
		_dPrintf ("\nmsgrcv():DosOpenQueue error: return code = %lu\n", rc);
		errno = _rc2Errno(rc); return(-1);
	}
    
	rc = DosQueryQueue (hqSpecialQue, &ulNumElems);
	if (rc != NO_ERROR) {
		_dPrintf ("\nmsgrcv():DosQueryQueue error: return code = %lu\n", rc);
		errno = _rc2Errno(rc); return(-1);   
  }
	if ((ulNumElems==0) && (msgflg==IPC_NOWAIT))
		return(-1);   
	Request.pid = pidOwner;    /* process ID for the DosReadQueue */
	rc = DosReadQueue (hqSpecialQue,   /* Queue to read from   */
  					&Request,/* Request data from write     */
  					&ulDataLen,     /* Length of data returned     */
  					(PVOID)&DataBuffer,    /* The data */
  					0L, /* Remove first element */
  					DCWW_WAIT, /* Wait for available data     */
  					&ElemPrty, /* Priority of data (returned) */
  					0L);     /* Semaphore to use when not waiting */
	if (rc!= NO_ERROR) {
		_dPrintf("\nmsgrcv():DosReadQueue error: return code = %lu\n", rc);
		errno = _rc2Errno(rc); return(-1);
	}
	_dPrintf("\n----msgrcv---Getting key %ld\n", DataBuffer);	

	strcat(szMemName,(char*)_itoa(qkey,buffer,10));
	strcat(szMemName,(char*)_itoa(DataBuffer,buffer,10));   
    
	rc = DosGetNamedSharedMem(&pObject,     /* Pointer to pointer of object */
						  szMemName,    /* Name of shared memory */
  						PAG_WRITE);   /* Want read-only access */
	if (rc != NO_ERROR) {
		_dPrintf("\nmsgrcv():DosGetNamedSharedMem error:  return code = %lu\n",rc);
		errno = _rc2Errno(rc); return(-1);
	}
	memcpy(msgp,pObject,Request.ulData);
	memcpy(pObject,_itoa(DataBuffer,buffer,10),Request.ulData);
	rc = DosFreeMem(pObject);
	if (rc != NO_ERROR) {
		_dPrintf("\nmsgrcv():DosFreeMem error:  return code = %lu\n",rc);
		errno = _rc2Errno(rc); return(-1);
	}
	return(Request.ulData);
}

	extern int 
msgctl(int qkey, int cmd, struct msqid_ds *buf)
{
 struct _msg_libext *temp_msg_ptr = __msgx_globMsgPtr,
                    *del_msg_ptr;  
 int z = 0;
 APIRET rc    = NO_ERROR;   /* Return code */
    
	while (temp_msg_ptr->key_used!=qkey && temp_msg_ptr->pnext!=NULL)
		temp_msg_ptr=temp_msg_ptr->pnext; 
		
	if (temp_msg_ptr->key_used==qkey) { 
		if (cmd==IPC_RMID) {
			while (z <= temp_msg_ptr->asize) { 
			
				_dPrintf("\nDeleting %ld", temp_msg_ptr->tmsgptr[z].msg);
				_dPrintf(" from queue %d\n", qkey);

				rc = DosFreeMem(temp_msg_ptr->tmsgptr[z].ptr);
				if (rc != NO_ERROR) {
				  _dPrintf("\nmsgrcv():DosFreeMem error:  return code = %lu\n",rc);
					errno = _rc2Errno(rc); return -1;
				}
     		++z;
			}
			free(temp_msg_ptr->tmsgptr);  
			del_msg_ptr=temp_msg_ptr;
		if (del_msg_ptr==__msgx_globMsgPtr)
			__msgx_globMsgPtr=__msgx_globMsgPtr->pnext; 
		else {
			for(temp_msg_ptr=__msgx_globMsgPtr;
				temp_msg_ptr->pnext!=del_msg_ptr;temp_msg_ptr = 
								temp_msg_ptr->pnext); 
			temp_msg_ptr->pnext=del_msg_ptr->pnext;
		}
		free(del_msg_ptr);  
   } /* IPC_RMID */
  }
	return 0;
}

/*
 * Copyright (c) 1994 SigmaSoft, Th. Lockert <tholo@sigmasoft.com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

	key_t
ftok(const char *path, int id)
{
	struct stat st;

	if (stat(path, &st) < 0)
		return (key_t)-1;

	return (key_t)
	    ((id & 0xff) << 24 | (st.st_dev & 0xff) << 16 | (st.st_ino & 0xffff));
}
