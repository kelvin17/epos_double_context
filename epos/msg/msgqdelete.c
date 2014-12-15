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
 *  $Id: msgqdelete.c,v 1.7 2009/12/15 18:26:41 humph Exp $
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
 *  epos_message_queue_delete
 *
 *  This directive allows a thread to delete the message queue specified
 *  by the given queue identifier.
 *
 *  Input parameters:
 *    id - queue id
 *
 *  Output parameters:
 *    RTEMS_SUCCESSFUL - if successful
 *    error code        - if unsuccessful
 */

epos_status_code epos_message_queue_delete(
  epos_id id
)
{
  register Message_queue_Control *the_message_queue;
  Objects_Locations               location;

  the_message_queue = _Message_queue_Get( id, &location );
  switch ( location ) {

    case OBJECTS_LOCAL:
      _Objects_Close( &_Message_queue_Information,
                      &the_message_queue->Object );

      _CORE_message_queue_Close(
        &the_message_queue->message_queue,
         NULL,
        CORE_MESSAGE_QUEUE_STATUS_WAS_DELETED
      );

      _Message_queue_Free( the_message_queue );

      _Thread_Enable_dispatch();
      return RTEMS_SUCCESSFUL;

    case OBJECTS_ERROR:
      break;
  }

  return RTEMS_INVALID_ID;
}

