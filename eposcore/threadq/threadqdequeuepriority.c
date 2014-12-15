/**
 * @file
 *
 * @ingroup ScoreThreadQ
 *
 * @brief 核心抽象层线程队列管理器实现。(SuperCore Thread Queue Handler implementation.)
 */

/*
 *  Thread Queue Handler
 *
 *
 *  COPYRIGHT (c) 1989-2008.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: threadqdequeuepriority.c,v 1.9 2008/01/22 21:19:18 joel Exp $
 */

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <rtems/system.h>
#include <rtems/score/chain.h>
#include <rtems/score/isr.h>
#include <rtems/score/object.h>
#include <rtems/score/states.h>
#include <rtems/score/thread.h>
#include <rtems/score/threadq.h>
#include <rtems/score/tqdata.h>

/*PAGE
 *
 *  _Thread_queue_Dequeue_priority
 *
 *  This routine removes a thread from the specified PRIORITY based
 *  threadq, unblocks it, and cancels its timeout timer.
 *
 *  Input parameters:
 *    the_thread_queue - pointer to thread queue
 *
 *  Output parameters:
 *    returns - thread dequeued or NULL
 *
 *  INTERRUPT LATENCY:
 *    only case
 */

Thread_Control *_Thread_queue_Dequeue_priority(
  Thread_queue_Control *the_thread_queue
)
{
  uint32_t        index;
  ISR_Level       level;
  Thread_Control *the_thread = NULL;  /* just to remove warnings */
  Thread_Control *new_first_thread;
  Chain_Node     *new_first_node;
  Chain_Node     *new_second_node;
  Chain_Node     *last_node;
  Chain_Node     *next_node;
  Chain_Node     *previous_node;

  _ISR_Disable( level );
  for( index=0 ;
       index < TASK_QUEUE_DATA_NUMBER_OF_PRIORITY_HEADERS ;
       index++ ) {
    if ( !_Chain_Is_empty( &the_thread_queue->Queues.Priority[ index ] ) ) {
      the_thread = (Thread_Control *)
                    the_thread_queue->Queues.Priority[ index ].first;
      goto dequeue;
    }
  }

  /*
   * We did not find a thread to unblock.
   */
  _ISR_Enable( level );
  return NULL;

dequeue:
  the_thread->Wait.queue = NULL;
  new_first_node   = the_thread->Wait.Block2n.first;
  new_first_thread = (Thread_Control *) new_first_node;
  next_node        = the_thread->Object.Node.next;
  previous_node    = the_thread->Object.Node.previous;

  if ( !_Chain_Is_empty( &the_thread->Wait.Block2n ) ) {
    last_node       = the_thread->Wait.Block2n.last;
    new_second_node = new_first_node->next;

    previous_node->next      = new_first_node;
    next_node->previous      = new_first_node;
    new_first_node->next     = next_node;
    new_first_node->previous = previous_node;

    if ( !_Chain_Has_only_one_node( &the_thread->Wait.Block2n ) ) {
                                                /* > two threads on 2-n */
      new_second_node->previous =
                _Chain_Head( &new_first_thread->Wait.Block2n );

      new_first_thread->Wait.Block2n.first = new_second_node;
      new_first_thread->Wait.Block2n.last  = last_node;

      last_node->next = _Chain_Tail( &new_first_thread->Wait.Block2n );
    }
  } else {
    previous_node->next = next_node;
    next_node->previous = previous_node;
  }

  if ( !_Watchdog_Is_active( &the_thread->Timer ) ) {
    _ISR_Enable( level );
    _Thread_Unblock( the_thread );
  } else {
    _Watchdog_Deactivate( &the_thread->Timer );
    _ISR_Enable( level );
    (void) _Watchdog_Remove( &the_thread->Timer );
    _Thread_Unblock( the_thread );
  }
  return( the_thread );
}

