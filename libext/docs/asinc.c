#define INCL_BASE
#define OS2_API32
#define INCL_DOSDATETIME   /* Date and time values */
#include <os2.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
 
int main()
 {
     APIRET rc;                     /* Return code */
     ULONG  TimeInterval = 10000L;  /* Time delay */
     HEV   SemHandle = 0;          /* Event-semaphore handle repository */
     HTIMER Handle = 0;             /* Timer handle */
     ULONG ulTimeout = 20000L;      /* Time-out value */
 
     /* Create an event semaphore to be posted by the timer. */
     rc = DosCreateEventSem(NULL, &SemHandle, DC_SEM_SHARED, FALSE);

     if (rc != 0)                          /* Error received */
       {
         printf("DosCreateEventSem failed -- rc = %ld", rc);
         DosExit(0,1);
       }
 
     /* Start the timer. */
 
     rc = DosAsyncTimer(TimeInterval, (HSEM)SemHandle, &Handle);
 
     if (rc != 0)                          /* Error received */
       {
         printf("DosAsyncTimer failed -- rc = %ld", rc);
         DosExit(0,1);
       }
 
     /* Wait for the semaphore to be posted.  */
 
     rc = DosWaitEventSem((ULONG) SemHandle, ulTimeout);
 
     if (rc != 0)                          /* Error received */
       {
         printf("DosWaitEventSem failed -- rc = %ld", rc);
         DosExit(0,1);
       }
 
     /* Indicate that the timer has posted the event semaphore. */
 
     printf("Timer has posted semaphore.");
     DosExit(0,0);
 }
