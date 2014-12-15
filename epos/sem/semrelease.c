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
 *  $Id: semrelease.c,v 1.9 2007/11/30 21:49:41 joel Exp $
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
 *  epos_semaphore_release
 *
 *  This directive allows a thread to release a semaphore.
 *
 *  Input parameters:
 *    id - semaphore id
 *
 *  Output parameters:
 *    RTEMS_SUCCESSFUL - if successful
 *    error code        - if unsuccessful
 */

#define MUTEX_MP_SUPPORT NULL

epos_status_code epos_semaphore_release(
  epos_id   id
)
{
  register Semaphore_Control *the_semaphore;
  Objects_Locations           location;
  CORE_mutex_Status           mutex_status;
  CORE_semaphore_Status       semaphore_status;

  the_semaphore = _Semaphore_Get( id, &location );
  switch ( location ) {

    case OBJECTS_LOCAL:
      if ( !_Attributes_Is_counting_semaphore(the_semaphore->attribute_set) ) {
        mutex_status = _CORE_mutex_Surrender(
          &the_semaphore->Core_control.mutex,
          id,
          MUTEX_MP_SUPPORT
        );
        _Thread_Enable_dispatch();
        return _Semaphore_Translate_core_mutex_return_code( mutex_status );
      } else {
        semaphore_status = _CORE_semaphore_Surrender(
          &the_semaphore->Core_control.semaphore,
          id,
          MUTEX_MP_SUPPORT
        );
        _Thread_Enable_dispatch();
        return
          _Semaphore_Translate_core_semaphore_return_code( semaphore_status );
      }


    case OBJECTS_ERROR:
      break;
  }

  return RTEMS_INVALID_ID;
}

