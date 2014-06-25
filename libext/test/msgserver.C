#include <strstream>
#include "msge.h"
#include <string.h>
#include <sys/time.h>
#include <signal.h>

int main()
{
  int len,pid,cmdId;
  char txtbuf[256];
  time_t tim;
  
  cout<<"server: start executing...\n"<<flush;
  message mqueue(MSGKEY);
  if (!mqueue.good()) 
    { 
    cout<<"server: msg API failed\n"<<flush;
    exit(1);
    } 
  
  while((len=mqueue.rcv(txtbuf,sizeof(txtbuf),-99,&cmdId))>0)   
  {
   istrstream(txtbuf,sizeof(txtbuf) ) >> pid; 
   cerr<<"server: receive cmd #"<<cmdId
        <<", from client:"<<pid<<endl;
   switch(cmdId)     
   {
     case LOCAL_TIME:
      tim= time(0);
      strcpy(txtbuf,ctime(&tim));
      break;
     case UTC_TIME:  
      tim= time(0);
      strcpy(txtbuf,asctime(gmtime(&tim)));
      break;
     case QUIT_CMD:
      cerr<< "server: deleting msg queue...\n";
      return mqueue.rmQ();
     default: 
      ostrstream(txtbuf,sizeof(txtbuf))<<"Illegal cmd:"<<cmdId<<'\0';
   }   
  if (mqueue.send(txtbuf,strlen(txtbuf),pid)==-1) 
  cerr<<"Server: "<<getpid()<<"send response fails\n";
 }; 
 return 0; 
} 
