/*************************************/
/* events.c */
/*************************************/
#include "sys.h"
#include "rtos_api.h"
#include <iostream>
#include <bitset>

TEventMask InitEvent( const char* name )
{
     int event = System->FreeEvent;
     System->FreeEvent++;
     System->EventQueue[ event ].name = name;

     return ( 1 << event );
}

void SetEvent( TTaskID id, TEventMask event )
{
     TTask* taskQueue = System->TaskQueue;
     int runningTask = System->RunningTask;

     TTask* task = &taskQueue[ id ];
     TEventMask eventsToSet = event & ~task->eventMask;

     std::cout << "Events " << std::bitset<MAX_EVENTS>( eventsToSet ) << " are set for " << task->name << "\n";
     task->eventMask |= eventsToSet;

     if ( eventsToSet )
     {
          task->state = TASK_READY;
     }

     swapcontext( &taskQueue[ runningTask ].context, &System->DispatchContext );
}

void WaitEvent( TEventMask event )
{
     TTask* taskQueue = System->TaskQueue;
     int runningTask = System->RunningTask;

     TTask* task = &taskQueue[ runningTask ];
     std::cout << task->name << " is waiting events " << std::bitset<MAX_EVENTS>( event ) << "\n";

     while ( !( event & task->eventMask ) )
     {
          task->state = TASK_WAITING;
          swapcontext( &taskQueue[ runningTask ].context, &System->DispatchContext );
     }

     TEventMask triggeredEvents = event & task->eventMask;
     std::cout << task->name << " received events " << std::bitset<MAX_EVENTS>( triggeredEvents ) << "\n";
     task->eventMask |= triggeredEvents;
     std::cout << task->name << " events " << std::bitset<MAX_EVENTS>( task->eventMask ) << "\n";
}

void ClearEvent( TEventMask mask )
{
     System->TaskQueue[ System->RunningTask ].eventMask &= ~mask;
}

void GetEvent( TTaskID id, TEventMask* event )
{
     *event = System->TaskQueue[ id ].eventMask;
}
