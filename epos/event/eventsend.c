/*
 *  Event Manager
 *
 *  COPYRIGHT (c) 1989-2007.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: eventsend.c,v 1.8 2009/12/15 18:26:41 humph Exp $
 */

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <rtems/system.h>
#include <rtems/rtems/status.h>
#include <rtems/rtems/event.h>
#include <rtems/score/isr.h>
#include <rtems/score/object.h>
#include <rtems/rtems/options.h>
#include <rtems/score/states.h>
#include <rtems/score/thread.h>
#include <rtems/rtems/tasks.h>

/*PAGE
 *
 *  epos_event_send
 *
 *  This directive allows a thread send an event set to another thread.
 *
 *  Input parameters:
 *    id    - thread id
 *    event - event set
 *
 *  Output parameters:
 *    RTEMS_SUCCESSFUL - if successful
 *    error code - if unsuccessful
 */

epos_status_code epos_event_send(
  epos_id          id,
  epos_event_set   event_in
)
{
  register Thread_Control *the_thread;
  Objects_Locations        location;
  RTEMS_API_Control       *api;

  the_thread = _Thread_Get( id, &location );
  switch ( location ) {

    case OBJECTS_LOCAL:
      api = the_thread->API_Extensions[ THREAD_API_RTEMS ];
      _Event_sets_Post( event_in, &api->pending_events );
      _Event_Surrender( the_thread );
      _Thread_Enable_dispatch();
      return RTEMS_SUCCESSFUL;

#if defined(RTEMS_MULTIPROCESSING)
    case OBJECTS_REMOTE:
      return(
        _Event_MP_Send_request_packet(
          EVENT_MP_SEND_REQUEST,
          id,
          event_in
        )
      );
#endif

    case OBJECTS_ERROR:
      break;
  }

  return RTEMS_INVALID_ID;
}

