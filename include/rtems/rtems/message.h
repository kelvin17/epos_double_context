/**
 * @file message.h
 *
 * @ingroup ClassicMessageQueue
 *
 *  This include file contains all the constants and structures associated
 *  with the Message Queue Manager.  This manager provides a mechanism for
 *  communication and synchronization between tasks using messages.
 *
 *  Directives provided are:
 *
 *     - create a queue
 *     - get ID of a queue
 *     - delete a queue
 *     - put a message at the rear of a queue
 *     - put a message at the front of a queue
 *     - broadcast N messages to a queue
 *     - receive message from a queue
 *     - flush all messages on a queue
 */

/*  COPYRIGHT (c) 1989-2008.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: message.h,v 1.33 2009/12/15 18:26:41 humph Exp $
 */

#ifndef _RTEMS_RTEMS_MESSAGE_H
#define _RTEMS_RTEMS_MESSAGE_H

/**
 *  This constant is defined to extern most of the time when using
 *  this header file.  However by defining it to nothing, the data
 *  declared in this header file can be instantiated.  This is done
 *  in a single per manager file.
 */
#ifndef RTEMS_MESSAGE_EXTERN
#define RTEMS_MESSAGE_EXTERN extern
#endif

#ifdef __cplusplus
extern "C" {
#endif

#include <rtems/rtems/types.h>
#include <rtems/rtems/status.h>
#include <rtems/rtems/options.h>
#include <rtems/score/chain.h>
#include <rtems/score/object.h>
#include <rtems/rtems/attr.h>
#include <rtems/score/coremsg.h>

/**
 *  @defgroup ClassicMessageQueue Message Queues
 *
 *  @ingroup ClassicRTEMS
 *
 *  @brief RTEMS消息队列管理服务模块。
 *
 *  This encapsulates functionality which XXX
 */
/**@{*/

/**
 *  The following enumerated type details the modes in which a message
 *  may be submitted to a message queue.  The message may be posted
 *  in a send or urgent fashion.
 */
typedef enum {
  /**
   *  This value indicates the user wants to send the message using the
   *  normal message insertion protocol (FIFO or priority).
   */
  MESSAGE_QUEUE_SEND_REQUEST   = 0,
  /**
   *  This value indicates the user considers the message to be urgent
   *  and wants it inserted at the head of the pending message queue.
   */
  MESSAGE_QUEUE_URGENT_REQUEST = 1
}  Message_queue_Submit_types;

/**
 *  The following records define the control block used to manage
 *  each message queue.
 */
typedef struct {
  /** This field is the inherited object characteristics. */
  Objects_Control             Object;
  /** This field is the attribute set as defined by the API. */
  epos_attribute             attribute_set;
  /** This field is the instance of the SuperCore Message Queue. */
  CORE_message_queue_Control  message_queue;
}   Message_queue_Control;

/**
 *  The following defines the information control block used to
 *  manage this class of objects.
 */
RTEMS_MESSAGE_EXTERN Objects_Information  _Message_queue_Information;

/**
 *  @brief Message_queue_Manager_initialization
 *
 *  This routine performs the initialization necessary for this manager.
 */
void _Message_queue_Manager_initialization(void);

/**
 *  @brief epos_message_queue_create
 *
 *  This routine implements the epos_message_queue_create directive.  The
 *  message queue will have the name name.  If the attribute_set indicates
 *  that the message queue is to be limited in the number of messages
 *  that can be outstanding, then count indicates the maximum number of
 *  messages that will be held.  It returns the id of the created
 *  message queue in ID.
 */
epos_status_code epos_message_queue_create(
  epos_name       name,
  uint32_t         count,
  size_t           max_message_size,
  epos_attribute  attribute_set,
  epos_id        *id
);

/**
 *  @brief epos_message_queue_ident
 *
 *  This routine implements the epos_message_queue_ident directive.
 *  This directive returns the message queue ID associated with NAME.
 *  If more than one message queue is named name, then the message
 *  queue to which the ID belongs is arbitrary.  node indicates the
 *  extent of the search for the ID of the message queue named name.
 *  The search can be limited to a particular node or allowed to
 *  encompass all nodes.
 */
epos_status_code epos_message_queue_ident(
  epos_name  name,
  uint32_t    node,
  epos_id   *id
);

/**
 *  @brief epos_message_queue_delete
 *
 *  This routine implements the epos_message_queue_delete directive.  The
 *  message queue indicated by ID is deleted.
 */
epos_status_code epos_message_queue_delete(
  epos_id id
);

/**
 *  @brief epos_message_queue_send
 *
 *  This routine implements the epos_message_queue_send directive.
 *  This directive sends the message buffer to the message queue
 *  indicated by ID.  If one or more tasks is blocked waiting
 *  to receive a message from this message queue, then one will
 *  receive the message.  The task selected to receive the
 *  message is based on the task queue discipline algorithm in
 *  use by this particular message queue.  If no tasks are waiting,
 *  then the message buffer will be placed at the REAR of the
 *  chain of pending messages for this message queue.
 */
epos_status_code epos_message_queue_send(
  epos_id    id,
  const void *buffer,
  size_t      size
);

/**
 *  @brief epos_message_queue_urgent
 *
 *  This routine implements the epos_message_queue_urgent directive.
 *  This directive has the same behavior as epos_message_queue_send
 *  except that if no tasks are waiting, the message buffer will
 *  be placed at the FRONT of the chain of pending messages rather
 *  than at the REAR.
 */
epos_status_code epos_message_queue_urgent(
  epos_id    id,
  const void *buffer,
  size_t      size
);

/**
 *  @brief epos_message_queue_broadcast
 *
 *  This routine implements the epos_message_queue_broadcast directive.
 *  This directive sends the message buffer to all of the tasks blocked
 *  waiting for a message on the message queue indicated by ID.
 *  If no tasks are waiting, then the message buffer will not be queued.
 */
epos_status_code epos_message_queue_broadcast(
  epos_id    id,
  const void *buffer,
  size_t      size,
  uint32_t   *count
);

/**
 *  @brief epos_message_queue_receive
 *
 *  This routine implements the epos_message_queue_receive directive.
 *  This directive is invoked when the calling task wishes to receive
 *  a message from the message queue indicated by ID.  The received
 *  message is to be placed in buffer.  If no messages are outstanding
 *  and the option_set indicates that the task is willing to block,
 *  then the task will be blocked until a message arrives or until,
 *  optionally, timeout clock ticks have passed.
 */
epos_status_code epos_message_queue_receive(
  epos_id        id,
  void           *buffer,
  size_t         *size,
  epos_option    option_set,
  epos_interval  timeout
);

/**
 *  @brief epos_message_queue_flush
 *
 *  This routine implements the epos_message_queue_flush directive.
 *  This directive takes all outstanding messages for the message
 *  queue indicated by ID and returns them to the inactive message
 *  chain.  The number of messages flushed is returned in COUNT.
 */
epos_status_code epos_message_queue_flush(
  epos_id  id,
  uint32_t *count
);

/**
 *  @brief epos_message_queue_get_number_pending
 *
 *  This routine implements the epos_message_queue_get_number_pending
 *  directive.  This directive returns the number of pending
 *  messages for the message queue indicated by ID
 *  chain.  The number of messages pending is returned in COUNT.
 */
epos_status_code epos_message_queue_get_number_pending(
  epos_id  id,
  uint32_t *count
);


/**
 *  @brief Message_queue_Submit
 *
 *  This routine implements the directives epos_message_queue_send
 *  and epos_message_queue_urgent.  It processes a message that is
 *  to be submitted to the designated message queue.  The message will
 *  either be processed as a send send message which it will be inserted
 *  at the rear of the queue or it will be processed as an urgent message
 *  which will be inserted at the front of the queue.
 */
epos_status_code _Message_queue_Submit(
  epos_id                    id,
  const void                 *buffer,
  size_t                      size,
  Message_queue_Submit_types  submit_type
);

/**
 *  @brief Message_queue_Allocate
 *
 *  This function allocates a message queue control block from
 *  the inactive chain of free message queue control blocks.
 */
Message_queue_Control *_Message_queue_Allocate (void);

/**
 *  @brief Message_queue_Translate_core_message_queue_return_code
 *
 *  This function returns a RTEMS status code based on the core message queue
 *  status code specified.
 */
epos_status_code _Message_queue_Translate_core_message_queue_return_code (
  uint32_t   the_message_queue_status
);

#ifndef __RTEMS_APPLICATION__
#ifndef _OCC_NO_INLINE_
#include <rtems/rtems/message.inl>
#endif

#ifdef _OCC_NO_INLINE_
RTEMS_INLINE_ROUTINE bool _Message_queue_Is_null (
  Message_queue_Control *the_message_queue
);
RTEMS_INLINE_ROUTINE void _Message_queue_Free (
  Message_queue_Control *the_message_queue
);
RTEMS_INLINE_ROUTINE Message_queue_Control *_Message_queue_Get (
  Objects_Id         id,
  Objects_Locations *location
);

#endif
#endif

#ifdef __cplusplus
}
#endif

/**@}*/

#endif
/* end of include file */
