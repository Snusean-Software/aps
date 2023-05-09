/****************************************/
/* sys.h /
/****************************************/
#ifndef SYS_H
#define SYS_H

#include "defs.h"
#include <csetjmp>
#include <csignal>
#include <cstring>
#include <ucontext.h>

#define INSERT_TO_TAIL 1
#define INSERT_TO_HEAD 0
#define _NULL -1
enum TTaskState
{
     TASK_INACTIVE,
     TASK_RUNNING,
     TASK_READY,
     TASK_SUSPENDED,
     TASK_WAITING,
     TASK_DONE
};
typedef unsigned int TEventMask;
typedef int TTaskID;
typedef struct Type_Task
{
     int ref;
     int period;
     int initial_priority;
     int priority;
     TTaskState state;
     TEventMask eventMask = 0;

     void ( *entry )( void );

     char stack[ SIGSTKSZ ];
     ucontext_t context;
     const char* name;
} TTask;

typedef const char* TResourceID;
typedef struct Type_resource
{
     int task;
     const char* name;
} TResource;

typedef struct Event
{
     bool set = false;
     const char* name;
} TEvent;

typedef struct Type_System
{
     TTask TaskQueue[ MAX_TASK ];
     TResource ResourceQueue[ MAX_RES ];
     TEvent EventQueue[ MAX_EVENTS ];
     int Priorities[ MAX_TASK ];
     int RunningTask = _NULL;
     int FreeTask = 0;
     int HeadTask = _NULL;
     int FreeResource = 0;
     int FreeEvent = 0;
     int FreePriority = MAX_TASK - 1;
     ucontext_t MainContext;
     ucontext_t DispatchContext;
     char DispatchStack[ SIGSTKSZ ];
     bool Finished = false;
} TSystem;

extern TSystem* System;

void Schedule( int task, int mode = INSERT_TO_TAIL );

void Dispatch();

#endif // SYS_H
