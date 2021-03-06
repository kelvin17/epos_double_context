/*
 *  epos_task_variable_add - Add a per-task variable
 *
 *
 *  COPYRIGHT (c) 1989-2007.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: taskvariableadd.c,v 1.16 2007/12/12 23:19:57 joel Exp $
 */

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <rtems/system.h>
#include <rtems/rtems/tasks.h>
#include <rtems/score/wkspace.h>

/*
 *  epos_task_variable_add
 *
 *  This directive registers a task variable.
 */

epos_status_code epos_task_variable_add(
  epos_id tid,
  void **ptr,
  void (*dtor)(void *)
)
{
  Thread_Control        *the_thread;
  Objects_Locations      location;
  epos_task_variable_t *tvp, *new;

  if ( !ptr )
    return RTEMS_INVALID_ADDRESS;

  the_thread = _Thread_Get (tid, &location);
  switch (location) {

    case OBJECTS_LOCAL:
      /*
       *  Figure out if the variable is already in this task's list.
       */
      tvp = the_thread->task_variables;
      while (tvp) {
        if (tvp->ptr == ptr) {
          tvp->dtor = dtor;
          _Thread_Enable_dispatch();
          return RTEMS_SUCCESSFUL;
        }
        tvp = (epos_task_variable_t *)tvp->next;
      }

      /*
       *  Now allocate memory for this task variable.
       */
      new = (epos_task_variable_t *)
         _Workspace_Allocate(sizeof(epos_task_variable_t));
      if (new == NULL) {
        _Thread_Enable_dispatch();
        return RTEMS_NO_MEMORY;
      }
      new->gval = *ptr;
      new->ptr = ptr;
      new->dtor = dtor;

      new->next = (struct epos_task_variable_tt *)the_thread->task_variables;
      the_thread->task_variables = new;
      _Thread_Enable_dispatch();
      return RTEMS_SUCCESSFUL;

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

