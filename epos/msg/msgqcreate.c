/*
 *  Message Queue Manager
 *
 *
 *  COPYRIGHT (c) 1989-1999.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: msgqcreate.c,v 1.15 2009/12/15 18:26:41 humph Exp $
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
 *  epos_message_queue_create
 *
 *  This directive creates a message queue by allocating and initializing
 *  a message queue data structure.
 *
 *  Input parameters:
 *    name             - user defined queue name
 *    count            - maximum message and reserved buffer count
 *    max_message_size - maximum size of each message
 *    attribute_set    - process method
 *    id               - pointer to queue
 *
 *  Output parameters:
 *    id                - queue id
 *    RTEMS_SUCCESSFUL  - if successful
 *    error code        - if unsuccessful
 */

epos_status_code epos_message_queue_create(
  epos_name       name,
  uint32_t         count,
  size_t           max_message_size,
  epos_attribute  attribute_set,
  epos_id        *id
)
{
  register Message_queue_Control *the_message_queue;
  CORE_message_queue_Attributes   the_msgq_attributes;

  if ( !epos_is_name_valid( name ) )
    return RTEMS_INVALID_NAME;

  if ( !id )
    return RTEMS_INVALID_ADDRESS;


  if ( count == 0 )
      return RTEMS_INVALID_NUMBER;

  if ( max_message_size == 0 )
      return RTEMS_INVALID_SIZE;

  _Thread_Disable_dispatch();              /* protects object pointer */

  the_message_queue = _Message_queue_Allocate();

  if ( !the_message_queue ) {
    _Thread_Enable_dispatch();
    return RTEMS_TOO_MANY;
  }

  the_message_queue->attribute_set = attribute_set;

  if (_Attributes_Is_priority( attribute_set ) )
    the_msgq_attributes.discipline = CORE_MESSAGE_QUEUE_DISCIPLINES_PRIORITY;
  else
    the_msgq_attributes.discipline = CORE_MESSAGE_QUEUE_DISCIPLINES_FIFO;

  if ( ! _CORE_message_queue_Initialize(
           &the_message_queue->message_queue,
           &the_msgq_attributes,
           count,
           max_message_size
         ) ) {

    _Message_queue_Free( the_message_queue );
    _Thread_Enable_dispatch();
    return RTEMS_UNSATISFIED;
  }

  _Objects_Open(
    &_Message_queue_Information,
    &the_message_queue->Object,
    (Objects_Name) name
  );

  *id = the_message_queue->Object.id;
  _Thread_Enable_dispatch();
  return RTEMS_SUCCESSFUL;
}

