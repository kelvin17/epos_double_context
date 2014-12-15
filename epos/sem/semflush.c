/*
 *  epos_semaphore_flush
 *
 *  DESCRIPTION:
 *
 *  This package is the implementation of the flush directive
 *  of the Semaphore Manager.
 *
 *  COPYRIGHT (c) 1989-2007.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: semflush.c,v 1.12 2007/11/30 21:49:41 joel Exp $
 */

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <rtems/system.h>
#include <rtems/rtems/status.h>
#include <rtems/rtems/support.h>
#include <rtems/rtems/attr.h>
#include <rtems/score/isr.h>
#include <rtems/score/object.h>
#include <rtems/rtems/options.h>
#include <rtems/rtems/sem.h>
#include <rtems/score/coremutex.h>
#include <rtems/score/coresem.h>
#include <rtems/score/states.h>
#include <rtems/score/thread.h>
#include <rtems/score/threadq.h>
#include <rtems/score/sysstate.h>

#include <rtems/score/interr.h>

/*PAGE
 *
 *  epos_semaphore_flush
 *
 *  This directive allows a thread to flush the threads
 *  pending on the semaphore.
 *
 *  Input parameters:
 *    id         - semaphore id
 *
 *  Output parameters:
 *    RTEMS_SUCCESSFUL - if successful
 *    error code       - if unsuccessful
 */


#define SEND_OBJECT_WAS_DELETED NULL


epos_status_code epos_semaphore_flush(
  epos_id        id
)
{
  register Semaphore_Control *the_semaphore;
  Objects_Locations           location;

  the_semaphore = _Semaphore_Get( id, &location );
  switch ( location ) {

    case OBJECTS_LOCAL:
      if ( !_Attributes_Is_counting_semaphore(the_semaphore->attribute_set) ) {
        _CORE_mutex_Flush(
          &the_semaphore->Core_control.mutex,
          SEND_OBJECT_WAS_DELETED,
          CORE_MUTEX_STATUS_UNSATISFIED_NOWAIT
        );
      } else {
        _CORE_semaphore_Flush(
          &the_semaphore->Core_control.semaphore,
          SEND_OBJECT_WAS_DELETED,
          CORE_SEMAPHORE_STATUS_UNSATISFIED_NOWAIT
        );
      }
      _Thread_Enable_dispatch();
      return RTEMS_SUCCESSFUL;

    case OBJECTS_ERROR:
      break;
  }

  return RTEMS_INVALID_ID;
}

