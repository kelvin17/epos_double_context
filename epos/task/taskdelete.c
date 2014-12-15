/*
 *  RTEMS Task Manager
 *
 *
 *  COPYRIGHT (c) 1989-2008.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: taskdelete.c,v 1.11 2009/12/15 18:26:41 humph Exp $
 */

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <rtems/system.h>
#include <rtems/rtems/status.h>
#include <rtems/rtems/support.h>
#include <rtems/rtems/modes.h>
#include <rtems/score/object.h>
#include <rtems/score/stack.h>
#include <rtems/score/states.h>
#include <rtems/rtems/tasks.h>
#include <rtems/score/thread.h>
#include <rtems/score/threadq.h>
#include <rtems/score/tod.h>
#include <rtems/score/userext.h>
#include <rtems/score/wkspace.h>
#include <rtems/score/apiext.h>
#include <rtems/score/sysstate.h>
#include <rtems/score/apimutex.h>

/*PAGE
 *
 *  epos_task_delete
 *
 *  This directive allows a thread to delete itself or the thread
 *  identified in the id field.  The executive halts execution
 *  of the thread and frees the thread control block.
 *
 *  Input parameters:
 *    id - thread id
 *
 *  Output parameters:
 *    nothing           - if id is the requesting thread (always succeeds)
 *    RTEMS_SUCCESSFUL - if successful and id is
 *                           not the requesting thread
 *    error code        - if unsuccessful
 */

epos_status_code epos_task_delete(
  epos_id id
)
{ 
  register Thread_Control *the_thread;
  Objects_Locations        location;
  Objects_Information     *the_information;

  _RTEMS_Lock_allocator();

  the_thread = _Thread_Get( id, &location );
  switch ( location ) {

    case OBJECTS_LOCAL:
      the_information = _Objects_Get_information_id( the_thread->Object.id );

      #if defined(RTEMS_DEBUG)
	if ( !the_information ) {
	  _Thread_Enable_dispatch();
	  return RTEMS_INVALID_ID;
	  /* This should never happen if _Thread_Get() works right */
	}
      #endif


      _Thread_Close( the_information, the_thread );

      _RTEMS_tasks_Free( the_thread );

      _RTEMS_Unlock_allocator();
      _Thread_Enable_dispatch();
      return RTEMS_SUCCESSFUL;

    case OBJECTS_ERROR:
      break;
  }

  _RTEMS_Unlock_allocator();
  return RTEMS_INVALID_ID;
}

