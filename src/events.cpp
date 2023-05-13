/*************************************/
/* events.c */
/*************************************/
#include "sys.h"
#include "rtos_api.h"
#include <cstdio>

TEventMask InitSysEvent()
{
     if ( System->FreeEvent >= MAX_EVENTS )
     {
          printf( "[CRIT] Maximum count of declared events reached.\n" );
          return 0;
     }
     int event = System->FreeEvent;
     System->FreeEvent++;
     return ( 1 << event );
}

void SetSysEvent( TEventMask event )
{
     printf( "[SetSysEvent] mask: %i\n", event );

     TTask* taskQueue = System->TaskQueue;

     System->WorkingEvents |= event;
     for ( int i = 0; i < MAX_TASK; i++ )
     {
          if ( taskQueue[ i ].state == TASK_WAITING && ( System->WorkingEvents & taskQueue[ i ].waitingEvents ) )
          {
               taskQueue[ i ].waitingEvents &= !event;
               taskQueue[ i ].state = TASK_READY;
               printf( "Task \"%s\" is ready\n", taskQueue[ i ].name );
          }
     }

     int runningTask = System->RunningTask;
     swapcontext( &taskQueue[ runningTask ].context, &System->DispatchContext );
}

void WaitSysEvent( TEventMask event )
{
     printf( "[WaitSysEvent] mask: %i\n", event );

     TTask* taskQueue = System->TaskQueue;
     int runningTask = System->RunningTask;
     TTask* task = &taskQueue[ runningTask ];

     if ( !( System->WorkingEvents & event ) )
     {
          task->waitingEvents |= event;
          task->state = TASK_WAITING;
          swapcontext( &taskQueue[ runningTask ].context, &System->DispatchContext );
     }
     printf( "End of [WaitSysEvent]\n" );
}

void GetSysEvent( TEventMask* event )
{
     *event = System->WorkingEvents;
}
