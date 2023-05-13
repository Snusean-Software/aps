#include <stdio.h>
#include "sys.h"
#include "rtos_api.h"

int StartOS( TTaskID task )
{
     printf( "[StartOS]\n" );

     for ( int i = 0; i < MAX_TASK; i++ )
     {
          System->TaskQueue[ i ].ref = i + 1;
          System->TaskQueue[ i ].state = TASK_SUSPENDED;
     }
     System->TaskQueue[ MAX_TASK - 1 ].ref = _NULL;

     // Dispatch context
     getcontext( &System->DispatchContext );
     System->DispatchContext.uc_link = &System->MainContext;
     System->DispatchContext.uc_stack.ss_sp = System->DispatchStack;
     System->DispatchContext.uc_stack.ss_size = sizeof( System->DispatchStack );
     makecontext( &System->DispatchContext, Dispatch, 0 );

     getcontext( &System->MainContext );
     if ( System->Finished )
     {
          return 0;
     }

     ActivateTask( task );
     return 0;
}

void ShutdownOS()
{
     delete System;
     printf( "[ShutdownOS]\n" );
}
