#ifndef MESSAGE_H
#define MESSAGE_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <memory.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>

enum { MSGKEY=176, MAX_LEN=256, ILLEGAL_CMD=4 };
enum { LOCAL_TIME=1, UTC_TIME=2, QUIT_CMD=3};

typedef struct mgbuf
{
	long mtype;
	char mtext[MAX_LEN];
} MSGBUF;	

class message
{
private:
 struct mgbuf mObj;
 int msgId; 

 public:

message(int key)
 { 
 if ((msgId=msgget(key,IPC_CREAT|0666))==-1)
    perror("msgget");
 };     
~message() {}; 
int good() { return (msgId >=0) ? 1:0;};
int rmQ()
{
 int rc=msgctl(msgId,IPC_RMID,0);
 if (rc==-1) perror("msgctl");
 return rc;
} 

int send(const void*msgBuf,int size,int type)
{
 mObj.mtype=type;
 memcpy(mObj.mtext,msgBuf,size);
 if (msgsnd(msgId,&mObj,size,0)) {
 	perror("msgsnd"); return -1;
 };
 return 0;
};

int rcv (void*msgBuf,int,int type,int *rtype)
{
 int len = msgrcv(msgId,&mObj,MAX_LEN,type,MSG_NOERROR);
  if (len==-1) {
     perror("msgrcv"); return -1;
  }   
  memcpy(msgBuf,mObj.mtext,len);  
 if (rtype) *rtype= mObj.mtype; 
 return len;
 };
 
}; 
#endif

     
  