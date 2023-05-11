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

// Осуществляет региcтрацию ресурса в системе. 
// Должен вызываться ДО использования ресурса в коде
// пользовательского приложения
void InitRes( TResourceID name );

// Захват ресурса. Вызов этой функции отмечает
// вход в критическую секцию. Выход из этой
// критической секции всегда должен 
// осуществляться вызовом PIP_ReleaseRes()
void PIP_GetRes( TResourceID res );

// Освобождение ресурса. Вызов этого сервиса
// отмечает выход из критической секции и
// должен сопровождаться предварительным 
// вызовом PIP_GetRes()
void PIP_ReleaseRes( TResourceID res );

TEventMask InitEvent( const char* name );

void SetEvent( TTaskID id, TEventMask event );

void WaitEvent( TEventMask event );

void ClearEvent( TEventMask event );

void GetEvent( TTaskID id, TEventMask* event );

bool AddPeriod( int period );

#endif // RTOS_API_H
