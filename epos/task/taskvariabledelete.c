/*
 *  epos_task_variable_delete - Delete a per-task variable
 *
 *
 *  COPYRIGHT (c) 1989-2007.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: taskvariabledelete.c,v 1.17 2009/11/30 15:59:56 ralf Exp $
 */

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <rtems/system.h>
#include <rtems/rtems/tasks.h>
#include <rtems/score/wkspace.h>

/*
 *  epos_task_variable_delete
 *
 *  This directive removes a task variable.
 */

epos_status_code epos_task_variable_delete(
  epos_id  tid,
  void    **ptr
)
{
  Thread_Control        *the_thread;
  Objects_Locations      location;
  epos_task_variable_t *tvp, *prev;

  if ( !ptr )
    return RTEMS_INVALID_ADDRESS;

  prev = NULL;

  the_thread = _Thread_Get (tid, &location);
  switch (location) {

    case OBJECTS_LOCAL:
      tvp = the_thread->task_variables;
      while (tvp) {
        if (tvp->ptr == ptr) {
          if (prev)
            prev->next = tvp->next;
          else
            the_thread->task_variables = (epos_task_variable_t *)tvp->next;

          _RTEMS_Tasks_Invoke_task_variable_dtor( the_thread, tvp );
          _Thread_Enable_dispatch();
          return RTEMS_SUCCESSFUL;
        }
        prev = tvp;
        tvp = (epos_task_variable_t *)tvp->next;
      }
      _Thread_Enable_dispatch();
      return RTEMS_INVALID_ADDRESS;

    case OBJECTS_ERROR:
        break;
  }

  return RTEMS_INVALID_ID;
}
