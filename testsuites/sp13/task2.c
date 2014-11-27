/*  Task_2
 *
 *  This routine serves as a test task.  Multiple tasks are required to
 *  verify all message manager capabilities.
 *
 *  Input parameters:
 *    argument - task argument
 *
 *  Output parameters:  NONE
 *
 *  COPYRIGHT (c) 1989-2007.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id$
 */

#include "system.h"

epos_task Task_2(
  epos_task_argument argument
)
{
  long                buffer[ 4 ];
  size_t              size;
  epos_task_priority previous_priority;
  epos_status_code   status;

  printk(
    "TA2 - epos_message_queue_receive - receive from queue 1 - RTEMS_NO_WAIT"
  );
  status = epos_message_queue_receive(
    Queue_id[ 1 ],
    buffer,
    &size,
    RTEMS_NO_WAIT,
    RTEMS_NO_TIMEOUT
  );
  directive_failed( status, "epos_message_queue_receive" );
  puts_nocr( "TA2 - buffer received: " );
  Put_buffer( buffer );
  new_line;

  printk(
    "TA2 - epos_message_queue_receive - receive from queue 1 - "
      "RTEMS_WAIT FOREVER"
  );
  status = epos_message_queue_receive(
    Queue_id[ 1 ],
    buffer,
    &size,
    RTEMS_DEFAULT_OPTIONS,
    RTEMS_NO_TIMEOUT
  );
  directive_failed( status, "epos_message_queue_receive" );
  puts_nocr( "TA2 - buffer received: " );
  Put_buffer( buffer );
  new_line;

  printk(
    "TA2 - epos_message_queue_receive - receive from queue 1 - "
      "RTEMS_WAIT FOREVER"
 );
  status = epos_message_queue_receive(
    Queue_id[ 1 ],
    buffer,
    &size,
    RTEMS_DEFAULT_OPTIONS,
    RTEMS_NO_TIMEOUT
  );
  directive_failed( status, "epos_message_queue_receive" );
  puts_nocr( "TA2 - buffer received: " );
  Put_buffer( buffer );
  new_line;

  printk( "TA2 - epos_task_set_priority - make self highest priority task" );
  status = epos_task_set_priority( RTEMS_SELF, 3, &previous_priority );
  directive_failed( status, "epos_task_set_priority" );

  printk(
    "TA2 - epos_message_queue_receive - receive from queue 2 - "
      "RTEMS_WAIT FOREVER"
  );
  status = epos_message_queue_receive(
    Queue_id[ 2 ],
    buffer,
    &size,
    RTEMS_DEFAULT_OPTIONS,
    RTEMS_NO_TIMEOUT
  );
  directive_failed( status, "epos_message_queue_receive" );
  puts_nocr( "TA2 - buffer received: " );
  Put_buffer( buffer );
  new_line;

  Fill_buffer( "B2Q2", (long *)buffer );
  printk( "TA2 - epos_message_queue_send - BUFFER 2 TO Q 2" );
  status = epos_message_queue_send( Queue_id[ 2 ], buffer, MESSAGE_SIZE );
  directive_failed( status, "epos_message_queue_send" );

  printk(
    "TA2 - epos_message_queue_receive - receive from queue 1 - "
      "10 second timeout"
  );
  status = epos_message_queue_receive(
    Queue_id[ 1 ],
    buffer,
    &size,
    RTEMS_DEFAULT_OPTIONS,
    10 * TICKS_PER_SECOND
  );
  directive_failed( status, "epos_message_queue_receive" );
  puts_nocr( "TA2 - buffer received: " );
  Put_buffer( buffer );
  new_line;

  printk(
    "TA2 - epos_message_queue_receive - receive from queue 3 - "
      "RTEMS_WAIT FOREVER"
  );
  status = epos_message_queue_receive(
    Queue_id[ 3 ],
    buffer,
    &size,
    RTEMS_DEFAULT_OPTIONS,
    RTEMS_NO_TIMEOUT
  );
  directive_failed( status, "epos_message_queue_receive" );
  puts_nocr( "TA2 - buffer received: " );
  Put_buffer( buffer );
  new_line;

}
