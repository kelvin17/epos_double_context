/*  Task_1
 *
 *  This routine serves as a test task.  It verifies the message manager.
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
#include <string.h>             /* for memcmp */

unsigned char big_send_buffer[2048];
unsigned char big_receive_buffer[2048];

long buffer[ MESSAGE_SIZE / sizeof(long) ];

void dope_buffer(unsigned char *buff,
                 int   buff_size,
                 uint32_t   v)
{
    int i;
    unsigned char ch;

    ch = (' ' + (v % (0x7f - ' ')));

    for (i=0; i<buff_size; i++)
    {
        *buff++ = ch++;
        if (ch >= 0x7f)
            ch = ' ';
    }
}

epos_task Task_1(
  epos_task_argument argument
)
{
  epos_id          qid;
  uint32_t          index;
  uint32_t          count;
  epos_status_code status;
  size_t            size;
  size_t            queue_size;
  unsigned char    *cp;

  status = epos_message_queue_ident(
    Queue_name[ 1 ],
    RTEMS_SEARCH_ALL_NODES,
    &qid
  );
  printk( "TA1 - epos_message_queue_ident - qid => %08x\n", qid );
  directive_failed( status, "epos_message_queue_ident" );

  Fill_buffer( "B1Q1", buffer );
  printk( "TA1 - epos_message_queue_send - BUFFER 1 TO Q 1" );
  status = epos_message_queue_send( Queue_id[ 1 ], buffer, MESSAGE_SIZE );
  directive_failed( status, "epos_message_queue_send" );

  Fill_buffer( "B2Q1", buffer );
  printk( "TA1 - epos_message_queue_send - BUFFER 2 TO Q 1" );
  status = epos_message_queue_send( Queue_id[ 1 ], buffer, MESSAGE_SIZE );
  directive_failed( status, "epos_message_queue_send" );

  printk( "TA1 - epos_task_wake_after - sleep 5 seconds" );
  status = epos_task_wake_after( 5*TICKS_PER_SECOND );
  directive_failed( status, "epos_task_wake_after" );

  Fill_buffer( "B3Q1", buffer );
  printk( "TA1 - epos_message_queue_send - BUFFER 3 TO Q 1" );
  status = epos_message_queue_send( Queue_id[ 1 ], buffer, MESSAGE_SIZE );
  directive_failed( status, "epos_message_queue_send" );

  printk( "TA1 - epos_task_wake_after - sleep 5 seconds" );
  status = epos_task_wake_after( 5*TICKS_PER_SECOND );
  directive_failed( status, "epos_task_wake_after" );

epos_test_pause();

  Fill_buffer( "B1Q2", buffer );
  printk( "TA1 - epos_message_queue_send - BUFFER 1 TO Q 2" );
  status = epos_message_queue_send( Queue_id[ 2 ], buffer, MESSAGE_SIZE );
  directive_failed( status, "epos_message_queue_send" );

  puts_nocr( "TA1 - epos_message_queue_receive - receive from queue 1 - " );
  printk     ( "10 second timeout" );
  status = epos_message_queue_receive(
    Queue_id[ 1 ],
    buffer,
    &size,
    RTEMS_DEFAULT_OPTIONS,
    10 * TICKS_PER_SECOND
  );
  directive_failed( status, "epos_message_queue_receive" );
  puts_nocr( "TA1 - buffer received: " );
  Put_buffer( buffer );
  new_line;

  printk( "TA1 - epos_task_delete - delete TA2" );
  status = epos_task_delete( Task_id[ 2 ] );
  directive_failed( status, "epos_task_delete" );

  Fill_buffer( "B1Q3", buffer );
  printk( "TA1 - epos_message_queue_send - BUFFER 1 TO Q 3" );
  status = epos_message_queue_send( Queue_id[ 3 ], buffer, MESSAGE_SIZE );
  directive_failed( status, "epos_message_queue_send" );

  printk( "TA1 - epos_task_wake_after - sleep 5 seconds" );
  status = epos_task_wake_after( 5*TICKS_PER_SECOND );
  directive_failed( status, "epos_task_wake_after" );

epos_test_pause();

  Fill_buffer( "B2Q3", buffer );
  printk( "TA1 - epos_message_queue_send - BUFFER 2 TO Q 3" );
  status = epos_message_queue_send( Queue_id[ 3 ], buffer, MESSAGE_SIZE );
  directive_failed( status, "epos_message_queue_send" );

  Fill_buffer( "B3Q3", buffer );
  printk( "TA1 - epos_message_queue_send - BUFFER 3 TO Q 3" );
  status = epos_message_queue_send( Queue_id[ 3 ], buffer, MESSAGE_SIZE );
  directive_failed( status, "epos_message_queue_send" );

  Fill_buffer( "B4Q3", buffer );
  printk( "TA1 - epos_message_queue_send - BUFFER 4 TO Q 3" );
  status = epos_message_queue_send( Queue_id[ 3 ], buffer, MESSAGE_SIZE );
  directive_failed( status, "epos_message_queue_send" );

  Fill_buffer( "B5Q3", buffer );
  printk( "TA1 - epos_message_queue_urgent - BUFFER 5 TO Q 3" );
  status = epos_message_queue_urgent( Queue_id[ 3 ], buffer, MESSAGE_SIZE );
  directive_failed( status, "epos_message_queue_urgent" );

  for ( index = 1 ; index <= 4 ; index++ ) {
    printk(
      "TA1 - epos_message_queue_receive - receive from queue 3 - "
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
    puts_nocr( "TA1 - buffer received: " );
    Put_buffer( buffer );
    new_line;
  }

  Fill_buffer( "B3Q2", buffer );
  printk( "TA1 - epos_message_queue_urgent - BUFFER 3 TO Q 2" );
  status = epos_message_queue_urgent( Queue_id[ 2 ], buffer, MESSAGE_SIZE );
  directive_failed( status, "epos_message_queue_urgent" );

  printk(
    "TA1 - epos_message_queue_receive - receive from queue 2 - "
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
  puts_nocr( "TA1 - buffer received: " );
  Put_buffer( buffer );
  new_line;

epos_test_pause();

  printk( "TA1 - epos_message_queue_delete - delete queue 1" );
  status = epos_message_queue_delete( Queue_id[ 1 ] );
  directive_failed( status, "epos_message_queue_delete" );

  Fill_buffer( "B3Q2", buffer );
  printk( "TA1 - epos_message_queue_urgent - BUFFER 3 TO Q 2" );
  status = epos_message_queue_urgent( Queue_id[ 2 ], buffer, MESSAGE_SIZE );
  directive_failed( status, "epos_message_queue_urgent" );

  printk( "TA1 - epos_message_queue_delete - delete queue 2" );
  status = epos_message_queue_delete( Queue_id[ 2 ] );
  directive_failed( status, "epos_message_queue_delete" );

  printk( "TA1 - epos_message_queue_get_number_pending - check Q 3" );
  status = epos_message_queue_get_number_pending( Queue_id[ 3 ], &count );
  directive_failed( status, "epos_message_queue_get_number_pending" );
  printk( "TA1 - %d messages are pending on Q 3\n", count );

  printk( "TA1 - epos_message_queue_flush - empty Q 3" );
  status = epos_message_queue_flush( Queue_id[ 3 ], &count );
  directive_failed( status, "epos_message_queue_flush" );
  printk( "TA1 - %d messages were flushed from Q 3\n", count );

  Fill_buffer( "B1Q3", buffer );
  printk( "TA1 - epos_message_queue_send - BUFFER 1 TO Q 3" );
  status = epos_message_queue_send( Queue_id[ 3 ], buffer, MESSAGE_SIZE );
  directive_failed( status, "epos_message_queue_send" );

  Fill_buffer( "B2Q3", buffer );
  printk( "TA1 - epos_message_queue_send - BUFFER 2 TO Q 3" );
  status = epos_message_queue_send( Queue_id[ 3 ], buffer, MESSAGE_SIZE );
  directive_failed( status, "epos_message_queue_send" );

  /* this broadcast should have no effect on the queue */
  Fill_buffer( "NBQ1", (long *)buffer );
  printk( "TA1 - epos_message_queue_broadcast - NO BUFFER TO Q1" );
  status = epos_message_queue_broadcast(
    Queue_id[ 1 ],
    (long (*)[4])buffer,
    16,
    &count
  );
  printk( "TA1 - number of tasks awakened = %d\n", count );

  printk( "TA1 - epos_message_queue_get_number_pending - check Q 3" );
  status = epos_message_queue_get_number_pending( Queue_id[ 3 ], &count );
  directive_failed( status, "epos_message_queue_get_number_pending" );
  printk( "TA1 - %d messages are pending on Q 3\n", count );

  Fill_buffer( "B3Q3", buffer );
  printk( "TA1 - epos_message_queue_send - BUFFER 3 TO Q 3" );
  status = epos_message_queue_send( Queue_id[ 3 ], buffer, MESSAGE_SIZE );
  directive_failed( status, "epos_message_queue_send" );

  printk( "TA1 - epos_message_queue_flush - Q 3" );
  status = epos_message_queue_flush( Queue_id[ 3 ], &count );
  printk( "TA1 - %d messages were flushed from Q 3\n", count );

  printk( "TA1 - epos_message_queue_send until all message buffers consumed" );
  while ( FOREVER ) {
    status = epos_message_queue_send( Queue_id[ 3 ], buffer, MESSAGE_SIZE );
    if ( status == RTEMS_TOO_MANY ) break;
    directive_failed( status, "epos_message_queue_send loop" );
  }

  printk( "TA1 - all message buffers consumed" );
  printk( "TA1 - epos_message_queue_flush - Q 3" );
  status = epos_message_queue_flush( Queue_id[ 3 ], &count );
  printk( "TA1 - %d messages were flushed from Q 3\n", count );

epos_test_pause();

  printk( "TA1 - create message queue of 20 bytes on queue 1" );
  status = epos_message_queue_create(
    Queue_name[ 1 ],
    100,
    20,
    RTEMS_DEFAULT_ATTRIBUTES,
    &Queue_id[ 1 ]
  );
  directive_failed( status, "epos_message_queue_create of Q1; 20 bytes each" );
  status = epos_message_queue_send( Queue_id[ 1 ], big_send_buffer, 40 );
  fatal_directive_status(status, RTEMS_INVALID_SIZE, "expected RTEMS_INVALID_SIZE");

  printk( "TA1 - epos_message_queue_delete - delete queue 1" );
  status = epos_message_queue_delete( Queue_id[ 1 ] );
  directive_failed( status, "epos_message_queue_delete" );

epos_test_pause();

  printk( "TA1 - epos_message_queue_create - variable sizes " );
  for (queue_size = 1; queue_size < 1030; queue_size++)
  {
      status = epos_message_queue_create(
          Queue_name[ 1 ],
          2,            /* just 2 msgs each */
          queue_size,
          RTEMS_DEFAULT_ATTRIBUTES,
          &Queue_id[ 1 ]
          );
      if (status != RTEMS_SUCCESSFUL)
      {
          printk("TA1 - msq que size: %d\n", queue_size);
          directive_failed( status, "epos_message_queue_create of Q1" );
      }

      status = epos_message_queue_delete( Queue_id[ 1 ] );
      directive_failed( status, "epos_message_queue_delete" );
  }

  printk( "TA1 - epos_message_queue_create and send - variable sizes " );
  for (queue_size = 1; queue_size < 1030; queue_size++)
  {
      /* printk("TA1 - message queue size: %d\n", queue_size); */

      status = epos_message_queue_create(
          Queue_name[ 1 ],
          2,            /* just 2 msgs each */
          queue_size,
          RTEMS_DEFAULT_ATTRIBUTES,
          &Queue_id[ 1 ]
          );

      directive_failed( status, "epos_message_queue_create of Q1" );

      dope_buffer(big_send_buffer, sizeof(big_send_buffer), queue_size);
      memset(big_receive_buffer, 'Z', sizeof(big_receive_buffer));

      /* send a msg too big */
      status = epos_message_queue_send( Queue_id[ 1 ],
                                         big_send_buffer,
                                         queue_size + 1 );

      fatal_directive_status(status,
                             RTEMS_INVALID_SIZE,
                             "epos_message_queue_send too large");

      /* send a msg that is just right */
      status = epos_message_queue_send(Queue_id[ 1 ],
                                        big_send_buffer,
                                        queue_size);
      directive_failed(status, "epos_message_queue_send exact size");

      /* now read and verify the message just sent */
      status = epos_message_queue_receive(Queue_id[ 1 ],
                                           big_receive_buffer,
                                           &size,
                                           RTEMS_DEFAULT_OPTIONS,
                                           1 * TICKS_PER_SECOND);
      directive_failed(status, "epos_message_queue_receive exact size");
      if (size != queue_size)
      {
          printk("TA1 - exact size size match failed");
          epos_test_exit(1);
      }

      if (memcmp(big_send_buffer, big_receive_buffer, size) != 0)
      {
          printk("TA1 - exact size data match failed");
          epos_test_exit(1);
      }

      for (cp = (big_receive_buffer + size);
           cp < (big_receive_buffer + sizeof(big_receive_buffer));
           cp++)
          if (*cp != 'Z')
          {
              printk("TA1 - exact size overrun match failed");
              epos_test_exit(1);
          }

      /* all done with this one; delete it */
      status = epos_message_queue_delete( Queue_id[ 1 ] );
      directive_failed( status, "epos_message_queue_delete" );
  }

  printk( "*** END OF TEST 13 ***" );
  epos_test_exit( 0 );
}
