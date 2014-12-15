/*
 *  Message Queue Manager
 *
 *
 *  COPYRIGHT (c) 1989-2007.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: msgqbroadcast.c,v 1.12 2009/12/15 18:26:41 humph Exp $
 */

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <rtems/system.h>
#include <rtems/score/sysstate.h>
#include <rtems/score/chain.h>
#include <rtems/score/isr.h>
#include <rtems/score/coremsg.h>
#include <rtems/score/object.h>
#include <rtems/score/states.h>
#include <rtems/score/thread.h>
#include <rtems/score/wkspace.h>
#include <rtems/rtems/status.h>
#include <rtems/rtems/attr.h>
#include <rtems/rtems/message.h>
#include <rtems/rtems/options.h>
#include <rtems/rtems/support.h>

/*PAGE
 *
 *  epos_message_queue_broadcast
 *
 *  This directive sends a message for every thread waiting on the queue
 *  designated by id.
 *
 *  Input parameters:
 *    id     - pointer to message queue
 *    buffer - pointer to message buffer
 *    size   - size of message to broadcast
 *    count  - pointer to area to store number of threads made ready
 *
 *  Output parameters:
 *    count             - number of threads made ready
 *    RTEMS_SUCCESSFUL  - if successful
 *    error code        - if unsuccessful
 */

epos_status_code epos_message_queue_broadcast(
  epos_id    id,
  const void *buffer,
  size_t      size,
  uint32_t   *count
)
{
  register Message_queue_Control *the_message_queue;
  Objects_Locations               location;
  CORE_message_queue_Status       core_status;

  if ( !buffer )
    return RTEMS_INVALID_ADDRESS;

  if ( !count )
    return RTEMS_INVALID_ADDRESS;

  the_message_queue = _Message_queue_Get( id, &location );
  switch ( location ) {

    case OBJECTS_LOCAL:
      core_status = _CORE_message_queue_Broadcast(
                      &the_message_queue->message_queue,
                      buffer,
                      size,
                      id,
                      NULL,
                      count
                    );

      _Thread_Enable_dispatch();
      return
        _Message_queue_Translate_core_message_queue_return_code( core_status );
    case OBJECTS_ERROR:
      break;
  }
  return RTEMS_INVALID_ID;
}

