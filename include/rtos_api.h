/****************************************/
/* rtos_api.h */
/****************************************/
#ifndef RTOS_API_H
#define RTOS_API_H

#include "sys.h"

#define DeclareTask( TaskID, period, name )                                                                            \
     TASK( TaskID );                                                                                                   \
     TTaskID TaskID = InitTask( TaskID##entry, name, period );

#define TASK( TaskID ) void TaskID##entry( void )

#define DeclareEvent( EventID, name ) TEventMask EventID = InitEvent( name );

typedef void TTaskCall( void );

TTaskID InitTask( TTaskCall entry, const char* name, int period );

void ActivateTask( TTaskID task );

void TerminateTask( void );

int StartOS( TTaskID task );

void ShutdownOS();

void InitRes( TResourceID name );

void PIP_GetRes( TResourceID res );

void PIP_ReleaseRes( TResourceID res );

TEventMask InitEvent( const char* name );

void SetEvent( TTaskID id, TEventMask event );

void WaitEvent( TEventMask event );

void ClearEvent( TEventMask event );

void GetEvent( TTaskID id, TEventMask* event );

bool AddPeriod( int period );

#endif // RTOS_API_H
