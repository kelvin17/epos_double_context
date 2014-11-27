/*  Task_3
 *
 *  This routine serves as a test task.  Its major function is to
 *  broadcast a messge to all the other tasks.
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

epos_task Task_3(
  epos_task_argument argument
)
{
  long              buffer[ 4 ];
  size_t            size;
  uint32_t          count;
  epos_status_code status;

  printk(
    "TA3 - epos_message_queue_receive - receive from queue 2 - "
      "RTEMS_WAIT FOREVER"
  );
  status = epos_message_queue_receive(
    Queue_id[ 2 ],
    (long (*)[4])buffer,
    &size,
    RTEMS_DEFAULT_OPTIONS,
    RTEMS_NO_TIMEOUT
  );
  directive_failed( status, "epos_message_queue_receive" );
  puts_nocr( "TA3 - buffer received: " );
  Put_buffer( buffer );
  new_line;

  Fill_buffer( "B3Q1", (long *)buffer );
  printk( "TA3 - epos_message_queue_broadcast - BUFFER 3 TO Q 1" );
  status = epos_message_queue_broadcast(
    Queue_id[ 1 ],
    (long (*)[4])buffer,
    16,
    &count
  );
  printk( "TA3 - number of tasks awakened = %02d\n", count );
  printk(
    "TA3 - epos_message_queue_receive - receive from queue 3 - "
      "RTEMS_WAIT FOREVER"
  );

  status = epos_message_queue_receive(
    Queue_id[ 3 ],
    (long (*)[4])buffer,
    &size,
    RTEMS_DEFAULT_OPTIONS,
    RTEMS_NO_TIMEOUT
  );
  directive_failed( status, "epos_message_queue_receive" );
  puts_nocr( "TA3 - buffer received: " );
  Put_buffer( buffer );
  new_line;

  printk( "TA3 - epos_task_delete - delete self" );
  status = epos_task_delete( RTEMS_SELF );
  directive_failed( status, "epos_task_delete of RTEMS_SELF" );
}
