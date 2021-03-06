/**
 * @file tasks.inl
 *
 * @ingroup ClassicTasks
 *
 *  This file contains the static inline implementation of all inlined
 *  routines in the with RTEMS Tasks Manager.
 */

/*  COPYRIGHT (c) 1989-2008.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: tasks.inl,v 1.19 2008/09/04 17:43:18 ralf Exp $
 */

#ifdef _OCC_NO_INLINE_
#include <rtems/system.h>
#include <rtems/rtems/tasks.h>
#endif

#ifndef _RTEMS_RTEMS_TASKS_H
# error "Never use "tasks.inl" directly; include "tasks.h" instead."
#endif

#ifndef _RTEMS_RTEMS_TASKS_INL
#define _RTEMS_RTEMS_TASKS_INL

/**
 *  @addtogroup ClassicTasks
 *  @{
 */

/**
 *  @brief RTEMS_tasks_Allocate
 *
 *  This function allocates a task control block from
 *  the inactive chain of free task control blocks.
 */
RTEMS_INLINE_ROUTINE Thread_Control *_RTEMS_tasks_Allocate( void )
{
  return (Thread_Control *) _Objects_Allocate( &_RTEMS_tasks_Information );
}

/**
 *  @brief RTEMS_tasks_Free
 *
 *  This routine frees a task control block to the
 *  inactive chain of free task control blocks.
 */
RTEMS_INLINE_ROUTINE void _RTEMS_tasks_Free (
  Thread_Control *the_task
)
{
  _Objects_Free( 
    _Objects_Get_information_id( the_task->Object.id ),
    &the_task->Object
  );
}

/**
 *  @brief RTEMS_tasks_Priority_to_Core
 *
 *  This function converts an RTEMS API priority into a core priority.
 */
RTEMS_INLINE_ROUTINE Priority_Control _RTEMS_tasks_Priority_to_Core(
  epos_task_priority   priority
)
{
  return (Priority_Control) priority;
}

/**
 *  @brief RTEMS_tasks_Priority_is_valid
 *
 *  This function returns TRUE if the_priority is a valid user task priority
 *  and FALSE otherwise.
 */
RTEMS_INLINE_ROUTINE bool _RTEMS_tasks_Priority_is_valid (
  epos_task_priority the_priority
)
{
  return (  ( the_priority >= RTEMS_MINIMUM_PRIORITY ) &&
            ( the_priority <= RTEMS_MAXIMUM_PRIORITY ) );
}

/**@}*/

#endif
/* end of include file */
