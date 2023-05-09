#include <cstdio>
#include "rtos_api.h"

DeclareTask( Task1, 5, "Task1" );

DeclareTask( Task2, 1, "Task2" );

DeclareTask( Task3, 10, "Task3" );

DeclareTask( Task4, 10, "Task4" );

DeclareTask( Task5, 5, "Task5" );

DeclareTask( Task6, 5, "Task6" );

DeclareTask( Task7, 10, "Task7" );

DeclareTask( Task8, 1, "Task8" );

DeclareTask( Task9, 10, "Task9" );

DeclareTask( Task10, 5, "Task10" );

DeclareEvent( Event1, "Event1" );

TResourceID Res1 = "Res1";

int main( void )
{
     StartOS( Task8 );
     ShutdownOS();

     delete System;
     return 0;
}
// Test 1 Preemptive algorithm
TASK( Task1 )
{
     printf( "Task 1 started!\n" );
     ActivateTask( Task2 );
     ActivateTask( Task3 );
     printf( "Task 1 finished!\n" );
     TerminateTask();
}

TASK( Task2 )
{
     printf( "Task 2 started!\n" );
     printf( "Task 2 finished!\n" );
     TerminateTask();
}

TASK( Task3 )
{
     printf( "Task 3 started!\n" );
     printf( "Task 3 finished!\n" );
     TerminateTask();
}

// Test 2 PIP resource managment algorithm
TASK( Task4 )
{
     printf( "Task 4 started!\n" );
     InitRes( Res1 );
     PIP_GetRes( Res1 );
     ActivateTask( Task5 );
     PIP_ReleaseRes( Res1 );
     printf( "Task 4 finished!\n" );
     TerminateTask();
}

TASK( Task5 )
{
     printf( "Task 5 started!\n" );
     InitRes( Res1 );
     PIP_GetRes( Res1 );
     PIP_ReleaseRes( Res1 );
     TerminateTask();
     printf( "Task 5 finished!\n" );
}
// Test 3 Tasks events
TASK( Task6 )
{
     printf( "Task 6 started!\n" );
     ActivateTask( Task7 );
     WaitEvent( Event1 );
     printf( "Task6 finished\n" );
     TerminateTask();
}

TASK( Task7 )
{
     printf( "Task 7 started!\n" );
     SetEvent( Task6, Event1 );
     printf( "Task7 finished\n" );
     TerminateTask();
}
// Test 4 All functionality
TASK( Task8 )
{
     printf( "Task 8 started!\n" );
     ActivateTask( Task9 );
     WaitEvent( Event1 );
     printf( "Task 8 finished\n" );
     TerminateTask();
}

TASK( Task9 )
{
     printf( "Task 9 started!\n" );
     InitRes( Res1 );
     PIP_GetRes( Res1 );
     ActivateTask( Task10 );
     PIP_ReleaseRes( Res1 );
     printf( "Task 9 finished\n" );
     TerminateTask();
}

TASK( Task10 )
{
     printf( "Task 10 started!\n" );
     InitRes( Res1 );
     PIP_GetRes( Res1 );
     SetEvent( Task8, Event1 );
     PIP_ReleaseRes( Res1 );
     printf( "Task 10 finished\n" );
     TerminateTask();
}
