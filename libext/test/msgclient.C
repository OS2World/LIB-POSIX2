#include <strstream>
#include <string.h>
#include "msge.h"

int main()
{

 int cmdId;
 cout<<"client: start executing...\n";
 message mqueue(MSGKEY); 
 /* Second mq for addition test*/
 /* Third mq for GL walk test */
 
 if (!mqueue.good()) exit(1);
 
 char procId[256],buf2[256];
 ostrstream(procId,sizeof(procId))<< 101 << '\0';
 do  {
     cout << "cmd> "<<flush; 
     cin >> cmdId;
     cout <<endl;
     if (cin.eof()) break;
       
     if (!cin.good() || cmdId < 0 || cmdId > 99) {
      cerr<< "Invalid input: "<< cmdId<<endl; continue;
     } 
     if (mqueue.send(procId,strlen(procId),cmdId)) 
       cout<<"client: "<<getpid() << "msgsnd error\n";
     else if (cmdId==QUIT_CMD) break;  
     else if (mqueue.rcv(buf2,sizeof(buf2),101,0)==-1) 
	      cout<<"client: "<<getpid()<<" msgrcv error\n"; 
     else cout << "client: "<< getpid()<< "  "<<buf2<<endl;	      
 } while (1);
 
 cout<<"client: "<<getpid()<<" exiting...\n"<<flush;
 return 0;
} 