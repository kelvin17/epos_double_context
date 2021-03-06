/*
 *  epos_task_variable_get - Get a per-task variable
 *
 *
 *  COPYRIGHT (c) 1989-2007.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: taskvariableget.c,v 1.9 2007/11/30 21:49:41 joel Exp $
 */

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <rtems/system.h>
#include <rtems/rtems/tasks.h>
#include <rtems/score/wkspace.h>

/*
 *  epos_task_variable_get
 *
 *  This directive gets the value of a task variable.
 */

epos_status_code epos_task_variable_get(
  epos_id tid,
  void **ptr,
  void **result
)
{
  Thread_Control        *the_thread;
  Objects_Locations      location;
  epos_task_variable_t *tvp;

  if ( !ptr )
    return RTEMS_INVALID_ADDRESS;

  if ( !result )
    return RTEMS_INVALID_ADDRESS;

  the_thread = _Thread_Get (tid, &location);
  switch (location) {

    case OBJECTS_LOCAL:
      /*
       *  Figure out if the variable is in this task's list.
       */
      tvp = the_thread->task_variables;
      while (tvp) {
        if (tvp->ptr == ptr) {
	  /*
	   * Should this return the current (i.e not the
	   * saved) value if `tid' is the current task?
	   */
          *result = tvp->tval;
          _Thread_Enable_dispatch();
          return RTEMS_SUCCESSFUL;
        }
        tvp = (epos_task_variable_t *)tvp->next;
      }
      _Thread_Enable_dispatch();
      return RTEMS_INVALID_ADDRESS;

#if defined(RTEMS_MULTIPROCESSING)
    case OBJECTS_REMOTE:
      _Thread_Dispatch();
      return RTEMS_ILLEGAL_ON_REMOTE_OBJECT;
#endif

    case OBJECTS_ERROR:
      break;
  }
  return RTEMS_INVALID_ID;
}

