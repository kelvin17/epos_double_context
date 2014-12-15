/*
 *  Semaphore Manager
 *
 *  DESCRIPTION:
 *
 *  This package is the implementation of the Semaphore Manager.
 *  This manager utilizes standard Dijkstra counting semaphores to provide
 *  synchronization and mutual exclusion capabilities.
 *
 *  Directives provided are:
 *
 *     + create a semaphore
 *     + get an ID of a semaphore
 *     + delete a semaphore
 *     + acquire a semaphore
 *     + release a semaphore
 *
 *  COPYRIGHT (c) 1989-2007.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: semdelete.c,v 1.11 2007/11/30 21:49:41 joel Exp $
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
 *  epos_semaphore_delete
 *
 *  This directive allows a thread to delete a semaphore specified by
 *  the semaphore id.  The semaphore is freed back to the inactive
 *  semaphore chain.
 *
 *  Input parameters:
 *    id - semaphore id
 *
 *  Output parameters:
 *    RTEMS_SUCCESSFUL - if successful
 *    error code       - if unsuccessful
 */

#define SEMAPHORE_MP_OBJECT_WAS_DELETED NULL


epos_status_code epos_semaphore_delete(
  epos_id   id
)
{
  register Semaphore_Control *the_semaphore;
  Objects_Locations           location;

  the_semaphore = _Semaphore_Get( id, &location );
  switch ( location ) {

    case OBJECTS_LOCAL:
      if ( !_Attributes_Is_counting_semaphore(the_semaphore->attribute_set) ) {
        if ( _CORE_mutex_Is_locked( &the_semaphore->Core_control.mutex ) &&
             !_Attributes_Is_simple_binary_semaphore(
                 the_semaphore->attribute_set ) ) {
          _Thread_Enable_dispatch();
          return RTEMS_RESOURCE_IN_USE;
        }
        _CORE_mutex_Flush(
          &the_semaphore->Core_control.mutex,
          SEMAPHORE_MP_OBJECT_WAS_DELETED,
          CORE_MUTEX_WAS_DELETED
        );
      } else {
        _CORE_semaphore_Flush(
          &the_semaphore->Core_control.semaphore,
          SEMAPHORE_MP_OBJECT_WAS_DELETED,
          CORE_SEMAPHORE_WAS_DELETED
        );
     }

      _Objects_Close( &_Semaphore_Information, &the_semaphore->Object );

      _Semaphore_Free( the_semaphore );

      _Thread_Enable_dispatch();
      return RTEMS_SUCCESSFUL;

    case OBJECTS_ERROR:
      break;
  }

  return RTEMS_INVALID_ID;
}

