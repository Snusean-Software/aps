/****************************************/
/* rtos_api.h */
/****************************************/
#ifndef RTOS_API_H
#define RTOS_API_H

#include "sys.h"

/* MACROS */

// Осуществляет регистрацию задачи в системе
#define DeclareTask( TaskID, period, name )                                                                            \
     TASK( TaskID );                                                                                                   \
     TTaskID TaskID = InitTask( TaskID##entry, name, period );

#define TASK( TaskID ) void TaskID##entry( void )

// Осуществляет регистрацию события в системе
// Должен вызываться ДО использования идентификатора события в коде пользовательского приложения.
#define DeclareSysEvent( EventID, name ) TEventMask EventID = InitSysEvent( name );

/**********************************************************/

typedef void TTaskCall( void );

/* TASK */

// Инициализация задачи
TTaskID InitTask( TTaskCall entry, const char* name, int period );

// Переводит задачу из состояния suspended в состояние ready
void ActivateTask( TTaskID task );

// Завершает выполнение текущей задачи, переводя её из состояния running в состояние suspended. При этом освобождаются
// все системные ресурсы, занятые задачей.
void TerminateTask( void );

/**********************************************************/

/* OS */

// Осуществляет необходимую инициализацию и запускает ОС на выполнение.
int StartOS( TTaskID task );

// Используется для немедленного завершения работы системы.
void ShutdownOS();

/**********************************************************/

/* RESOURCE */

void InitRes( TResourceID name );

void PIP_GetRes( TResourceID res );

void PIP_ReleaseRes( TResourceID res );

/**********************************************************/

/* EVENT */

// Инициализация системного события
TEventMask InitSysEvent( const char* name );

// Устанавливает системные события, задаваемые маской mask. При вызове данного сервиса любая задача будет переведена в
// готовое состояние, если она ждала хотя бы одного из событий, заданных маской mask.
// note: Любые события, не заданные в маске, останутся неизменны.
void SetSysEvent( TEventMask mask );

// Возвращает текущее состояние системной маски установленных событий
void GetSysEvent( TEventMask* event );

// Если не установлено ни одного события, заданного маской, то задача переводится в состояние ожидания. При выходе из
// данного сервиса должны быть сброшены те события, которые вызвали этот выход (то есть те из наступивших событий,
// наступления которых задача ждала), должно быть произведено удаление признаков ожидания этих событий как описано в
// «Common Specification»
void WaitSysEvent( TEventMask event );

/**********************************************************/

bool AddPeriod( int period );

#endif // RTOS_API_H
