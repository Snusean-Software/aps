/*************************************/
/*             resource.c            */
/*************************************/

#include "sys.h"
#include "rtos_api.h"
#include <cstdio>
#include <stdexcept>

TResource* findResource( TResourceID name )
{
     for ( int i = 0; i < System->FreeResource; ++i )
     {
          if ( strcmp( name, System->ResourceQueue[ i ].name ) == 0 )
          {
               return &System->ResourceQueue[ i ];
          }
     }
     return nullptr;
}

void InitRes( const char* name )
{
     TResource* res = findResource( name );
     if ( res != nullptr )
     {
          return;
     }

     int resource = System->FreeResource;
     System->FreeResource++;
     System->ResourceQueue[ resource ].name = name;
     System->ResourceQueue[ resource ].task = _NULL;
     printf( "%s is initialized\n", name );
}

void PIP_GetRes( TResourceID name )
{
     TTask* taskQueue = System->TaskQueue;
     int runningTask = System->RunningTask;

     TResource* res = findResource( name );
     if ( res == nullptr )
     {
          printf( "%s not registered\n", name );
          throw std::exception();
     }

     printf( "[GetResource] %s\n", res->name );

     while ( res->task != _NULL )
     {
          taskQueue[ res->task ].priority = taskQueue[ runningTask ].priority;
          printf( "%s inherited %s priority\n", taskQueue[ res->task ].name, taskQueue[ runningTask ].name );

          Schedule( res->task, INSERT_TO_HEAD );
          swapcontext( &taskQueue[ runningTask ].context, &System->DispatchContext );
     }

     res->task = runningTask;
     printf( "Resource %s captured by %s\n", res->name, taskQueue[ runningTask ].name );
}

void PIP_ReleaseRes( TResourceID name )
{
     TTask* taskQueue = System->TaskQueue;
     int runningTask = System->RunningTask;

     TResource* res = findResource( name );
     if ( res == nullptr )
     {
          printf( "%s not registered\n", name );
          throw std::exception();
     }

     printf( "[ReleaseRes] %s\n", res->name );

     if ( runningTask != res->task )
     {
          printf(
               "%s is trying to release the resource %s, which is not captured by it",
               taskQueue[ runningTask ].name,
               res->name
          );
          throw std::exception();
     }

     taskQueue[ runningTask ].priority = taskQueue[ runningTask ].initial_priority;
     res->task = _NULL;
     printf( "Resource %s released by %s\n", res->name, taskQueue[ runningTask ].name );

     Schedule( runningTask );
     swapcontext( &taskQueue[ runningTask ].context, &System->DispatchContext );
}
