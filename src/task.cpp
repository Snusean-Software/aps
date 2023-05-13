/*********************************/
/*          task.c               */
/*********************************/
#include <cstdio>
#include "sys.h"
#include "rtos_api.h"

int findPriority( int period )
{
     for ( int i = MAX_TASK - 1; i >= 0; --i )
     {
          if ( System->Priorities[ i ] == period )
          {
               return i;
          }

          if ( System->Priorities[ i ] == System->FreePriority )
          {
               return 0;
          }
     }

     return 0;
}

void ActivateTask( TTaskID id )
{
     TTask* task = &System->TaskQueue[ id ];
     printf( "[ActivateTask] %s\n", task->name );

     int priority = findPriority( task->period );
     task->initial_priority = priority;
     task->priority = priority;
     task->state = TASK_READY;
     Schedule( id );

     printf( "End of [ActivateTask] %s\n", task->name );

     if ( System->RunningTask != _NULL )
     {
          swapcontext( &System->TaskQueue[ System->RunningTask ].context, &System->DispatchContext );
     }
     else
     {
          Dispatch();
     }
}

void TerminateTask( void )
{
     int task = System->RunningTask;
     printf( "[TerminateTask] %s\n", System->TaskQueue[ task ].name );
     System->TaskQueue[ task ].state = TASK_DONE;
     System->RunningTask = _NULL;
     printf( "End of [TerminateTask] %s\n", System->TaskQueue[ task ].name );
     swapcontext( &System->TaskQueue[ task ].context, &System->DispatchContext );
}

void Schedule( int task, int mode )
{
     TTask* queue = System->TaskQueue;

     printf( "[Schedule] %s\n", queue[ task ].name );
     int cur = System->HeadTask;
     while ( cur != _NULL && queue[ cur ].ref != task )
     {
          cur = queue[ cur ].ref;
     }
     if ( cur != _NULL )
     {
          queue[ cur ].ref = queue[ task ].ref;
     }
     if ( task == System->HeadTask )
     {
          System->HeadTask = queue[ task ].ref;
     }

     cur = System->HeadTask;
     int prev = _NULL;
     int priority = queue[ task ].priority;
     while ( cur != _NULL && queue[ cur ].priority > priority && queue[ cur ].state != TASK_INACTIVE )
     {
          prev = cur;
          cur = queue[ cur ].ref;
     }
     if ( mode == INSERT_TO_TAIL )
     {
          while ( cur != _NULL && queue[ cur ].priority == priority && queue[ cur ].state != TASK_INACTIVE )
          {
               prev = cur;
               cur = queue[ cur ].ref;
          }
     }
     queue[ task ].ref = cur;
     if ( prev == _NULL )
     {
          System->HeadTask = task;
     }
     else
          queue[ prev ].ref = task;
     printf( "End of [Schedule] %s\n", queue[ task ].name );
}

void Dispatch()
{
     printf( "[Dispatch]\n" );
     int cur;
     TTask* queue = System->TaskQueue;
     int task = System->HeadTask;
     while ( task != _NULL && queue[ task ].state != TASK_INACTIVE )
     {
          switch ( queue[ task ].state )
          {
               case TASK_RUNNING:
                    printf( "%s is running\n", queue[ task ].name );
                    setcontext( &queue[ task ].context );
               case TASK_READY:
                    cur = queue[ task ].ref;
                    while ( cur != _NULL )
                    {
                         if ( queue[ cur ].state == TASK_RUNNING )
                         {
                              queue[ cur ].state = TASK_READY;
                              printf( "%s is switched to ready\n", queue[ cur ].name );
                              break;
                         }
                         cur = queue[ cur ].ref;
                    }

                    printf( "%s is switched to running\n", queue[ task ].name );
                    System->RunningTask = task;
                    queue[ task ].state = TASK_RUNNING;
                    setcontext( &queue[ task ].context );
               case TASK_WAITING:
                    printf( "%s is waiting\n", queue[ task ].name );
               case TASK_DONE:
                    task = queue[ task ].ref;
          }
     }
     System->Finished = true;
}

bool AddPeriod( int period )
{
     int* priorities = System->Priorities;
     int cur = System->FreePriority;
     if ( cur == -1 )
     {
          return false;
     }
     ( System->FreePriority )--;

     priorities[ cur ] = period;
     while ( cur != MAX_TASK - 1 && priorities[ cur ] < priorities[ cur + 1 ] )
     {
          int tmp = priorities[ cur ];
          priorities[ cur ] = priorities[ cur + 1 ];
          priorities[ cur + 1 ] = tmp;
          cur++;
     }

     return true;
}

TTaskID InitTask( TTaskCall entry, const char* name, int period )
{
     AddPeriod( period );
     int occupy = System->FreeTask;
     TTask& newTask = System->TaskQueue[ occupy ];
     System->FreeTask++;

     newTask.period = period;
     newTask.name = name;
     newTask.entry = entry;
     newTask.state = TASK_INACTIVE;

     getcontext( &newTask.context );
     newTask.context.uc_link = &System->DispatchContext;
     newTask.context.uc_stack.ss_sp = newTask.stack;
     newTask.context.uc_stack.ss_size = sizeof( newTask.stack );
     makecontext( &newTask.context, newTask.entry, 0 );

     return occupy;
}
