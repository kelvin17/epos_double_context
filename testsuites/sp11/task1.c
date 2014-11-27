/*  Task_1
 *
 *  This routine serves as a test task.  It verifies the event maager.
 *
 *  Input parameters:
 *    argument - task argument
 *
 *  Output parameters:  NONE
 *
 *  COPYRIGHT (c) 1989-1999.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id$
 */

#include "system.h"

epos_task Task_1(
  epos_task_argument argument
)
{
  epos_event_set   eventout;
  epos_time_of_day time;
  epos_status_code status;
  uint32_t    index;

  printk( "TA1 - epos_event_send - send RTEMS_EVENT_16 to TA2" );
  status = epos_event_send( Task_id[ 2 ], RTEMS_EVENT_16 );
  directive_failed( status, "epos_event_send" );

  printk(
    "TA1 - epos_event_receive - waiting forever on "
      "RTEMS_EVENT_14 and RTEMS_EVENT_15"
  );
  status = epos_event_receive(
    RTEMS_EVENT_14 | RTEMS_EVENT_15,
    RTEMS_DEFAULT_OPTIONS,
    RTEMS_NO_TIMEOUT,
    &eventout
  );
  directive_failed( status, "epos_event_receive" );
  printk(
    "TA1 - RTEMS_EVENT_14 and RTEMS_EVENT_15 received - eventout => %08x\n",
    eventout
  );

  printk( "TA1 - epos_event_send - send RTEMS_EVENT_18 to TA2" );
  status = epos_event_send( Task_id[ 2 ], RTEMS_EVENT_18 );
  directive_failed( status, "epos_event_send" );

  printk(
  "TA1 - epos_event_receive - waiting with 10 second timeout on RTEMS_EVENT_14"
  );
  status = epos_event_receive(
    RTEMS_EVENT_14,
    RTEMS_DEFAULT_OPTIONS,
    10 * TICKS_PER_SECOND,
    &eventout
  );
  directive_failed( status, "epos_event_receive" );
  printk( "TA1 - RTEMS_EVENT_14 received - eventout => %08x\n", eventout );

  printk( "TA1 - epos_event_send - send RTEMS_EVENT_19 to TA2" );
  status = epos_event_send( Task_id[ 2 ], RTEMS_EVENT_19 );
  directive_failed( status, "epos_event_send" );

  status = epos_clock_get( RTEMS_CLOCK_GET_TOD, &time );
  directive_failed( status, "epos_clock_get" );
  print_time( "TA1 - epos_clock_get - ", &time, "\n" );

epos_test_pause();

  printk( "TA1 - epos_event_send - send RTEMS_EVENT_18 to self after 5 seconds");
  status = epos_timer_fire_after(
    Timer_id[ 1 ],
    5 * TICKS_PER_SECOND,
    TA1_send_18_to_self_5_seconds,
    NULL
  );
  directive_failed( status, "epos_timer_fire_after 5 seconds" );

  printk( "TA1 - epos_event_receive - waiting forever on RTEMS_EVENT_18"  );
  status = epos_event_receive(
    RTEMS_EVENT_18,
    RTEMS_DEFAULT_OPTIONS,
    RTEMS_NO_TIMEOUT,
    &eventout
  );
  directive_failed( status, "epos_event_receive of 18" );
  printk( "TA1 - RTEMS_EVENT_18 received - eventout => %08x\n", eventout );

  status = epos_clock_get( RTEMS_CLOCK_GET_TOD, &time );
  directive_failed( status, "TA1 epos_clock_get" );
  print_time( "TA1 - epos_clock_get - ", &time, "\n" );

  printk( "TA1 - epos_event_send - send RTEMS_EVENT_3 to self" );
  status = epos_event_send( RTEMS_SELF, RTEMS_EVENT_3 );
  directive_failed( status, "epos_event_send" );

  printk(
    "TA1 - epos_event_receive - RTEMS_EVENT_3 or "
      "RTEMS_EVENT_22 - NO_WAIT and ANY"
  );
  status = epos_event_receive(
    RTEMS_EVENT_3 | RTEMS_EVENT_22,
    RTEMS_NO_WAIT | RTEMS_EVENT_ANY,
    RTEMS_NO_TIMEOUT,
    &eventout
  );
  directive_failed( status, "epos_event_receive of 3 and 22" );
  printk( "TA1 - RTEMS_EVENT_3 received - eventout => %08x\n", eventout );

  printk( "TA1 - epos_event_send - send RTEMS_EVENT_4 to self" );
  status = epos_event_send( RTEMS_SELF, RTEMS_EVENT_4 );
  directive_failed( status, "epos_event_send" );

  printk (
  "TA1 - epos_event_receive - RTEMS_EVENT_4 or "
      "RTEMS_EVENT_5 - forever and ANY"
  );
  status = epos_event_receive(
    RTEMS_EVENT_4 | RTEMS_EVENT_5,
    RTEMS_EVENT_ANY,
    RTEMS_NO_TIMEOUT,
    &eventout
  );
  directive_failed( status, "epos_event_receive" );
  printk( "TA1 - RTEMS_EVENT_4 received - eventout => %08x\n", eventout );

epos_test_pause();

  printk( "TA1 - epos_event_send - send RTEMS_EVENT_18 to self after 5 seconds");
  status = epos_timer_fire_after(
    Timer_id[ 1 ],
    5 * TICKS_PER_SECOND,
    TA1_send_18_to_self_5_seconds,
    NULL
  );
  directive_failed( status, "epos_timer_fire_after 5 seconds" );

  printk( "TA1 - epos_timer_cancel - cancelling timer for event RTEMS_EVENT_18");
  status = epos_timer_cancel( Timer_id[ 1 ] );
  directive_failed( status, "epos_timer_cancel" );

  printk( "TA1 - epos_event_send - send RTEMS_EVENT_8 to self after 60 seconds");
  status = epos_timer_fire_after(
    Timer_id[ 1 ],
    60 * TICKS_PER_SECOND,
    TA1_send_8_to_self_60_seconds,
    NULL
  );
  directive_failed( status, "epos_timer_fire_after 60 seconds" );

  printk( "TA1 - epos_event_send - send RTEMS_EVENT_9 to self after 60 seconds");
  status = epos_timer_fire_after(
    Timer_id[ 2 ],
    60 * TICKS_PER_SECOND,
    TA1_send_9_to_self_60_seconds,
    NULL
  );
  directive_failed( status, "epos_timer_fire_after 60 seconds" );

  printk(
    "TA1 - epos_event_send - send RTEMS_EVENT_10 to self after 60 seconds"
  );
  status = epos_timer_fire_after(
    Timer_id[ 3 ],
    60 * TICKS_PER_SECOND,
    TA1_send_10_to_self,
    NULL
  );
  directive_failed( status, "epos_timer_fire_after 60 seconds" );

  printk( "TA1 - epos_timer_cancel - cancelling timer for event RTEMS_EVENT_8" );
  status = epos_timer_cancel( Timer_id[ 1 ] );
  directive_failed( status, "epos_timer_cancel" );

  build_time( &time, 2, 12, 1988, 8, 15, 0, 0 );

  print_time( "TA1 - epos_clock_set - ", &time, "\n" );
  status = epos_clock_set( &time );
  directive_failed( status, "epos_clock_set" );

  printk( "TA1 - epos_event_send - send RTEMS_EVENT_1 every second" );
  status = epos_timer_fire_after(
    Timer_id[ 1 ],
    TICKS_PER_SECOND,
    TA1_send_1_to_self_every_second,
    NULL
  );
  directive_failed( status, "epos_timer_fire_after 1 second" );

  for ( index = 0; index < 3; index++ ) {

    status = epos_event_receive(
      RTEMS_EVENT_1,
      RTEMS_EVENT_ANY,
      RTEMS_NO_TIMEOUT,
      &eventout
    );
    directive_failed( status, "epos_event_receive" );

    status = epos_clock_get( RTEMS_CLOCK_GET_TOD, &time );
    directive_failed( status, "epos_clock_get" );

    printk( "TA1 - RTEMS_EVENT_1 received - eventout => %08x - ", eventout );
    print_time( "at ", &time, "\n" );

    if ( index < 2 ) {
      status = epos_timer_reset( Timer_id[ 1 ] );
      directive_failed( status, "epos_timer_reset" );
    };

  }

  printk( "TA1 - epos_timer_cancel - cancelling timer for event RTEMS_EVENT_1" );
  status = epos_timer_cancel( Timer_id[ 1 ] );
  directive_failed( status, "epos_timer_cancel" );

epos_test_pause();

  time.day = 13;
  printk( "TA1 - epos_event_send - sending RTEMS_EVENT_11 to self in 1 day" );
  status = epos_timer_fire_when(
    Timer_id[ 1 ],
    &time,
    TA1_send_11_to_self,
    NULL
  );
  directive_failed( status, "epos_timer_fire_when 1 day" );

  time.hour = 7;
  printk( "TA1 - epos_event_send - sending RTEMS_EVENT_11 to self in 1 day" );
  status = epos_timer_fire_when(
    Timer_id[ 2 ],
    &time,
    TA1_send_11_to_self,
    NULL
  );
  directive_failed( status, "epos_timer_fire_when 1 day" );

  time.hour = 8;   /* so code below has correct time/date */
  time.day = 14;
  printk( "TA1 - epos_event_send - sending RTEMS_EVENT_11 to self in 2 days" );
  status = epos_timer_fire_when(
    Timer_id[ 3 ],
    &time,
    TA1_send_11_to_self,
    NULL
  );
  directive_failed( status, "epos_timer_fire_when 2 days" );

  printk("TA1 - epos_timer_cancel - cancelling RTEMS_EVENT_11 to self in 1 day");
  status = epos_timer_cancel( Timer_id[ 1 ] );
  directive_failed( status, "epos_timer_cancel" );

  printk(
    "TA1 - epos_timer_cancel - cancelling RTEMS_EVENT_11 to self in 2 days"
  );
  status = epos_timer_cancel( Timer_id[ 3 ] );
  directive_failed( status, "epos_timer_cancel" );

  printk(
    "TA1 - epos_event_send - resending RTEMS_EVENT_11 to self in 2 days"
  );
  status = epos_timer_fire_when(
    Timer_id[ 3 ],
    &time,
    TA1_send_11_to_self,
    NULL
  );
  directive_failed( status, "epos_timer_fire_when 2 days" );

  time.day = 15;
  print_time( "TA1 - epos_clock_set - ", &time, "\n" );
  status = epos_clock_set( &time );
  directive_failed( status, "TA1 epos_clock_set" );

  printk( "TA1 - epos_event_receive - waiting forever on RTEMS_EVENT_11" );
  status = epos_event_receive(
    RTEMS_EVENT_11,
    RTEMS_DEFAULT_OPTIONS,
    RTEMS_NO_TIMEOUT,
    &eventout
  );
  directive_failed( status, "epos_event_receive" );
  printk( "TA1 - RTEMS_EVENT_11 received - eventout => %08x\n", eventout );

epos_test_pause();

  printk( "TA1 - epos_event_send/epos_event_receive combination" );
  status = epos_timer_fire_after(
    Timer_id[ 1 ],
    10,
    TA1_send_11_to_self,
    NULL
  );
  directive_failed( status, "epos_timer_fire_after 10 ticks" );

  status = epos_event_receive(
    RTEMS_EVENT_11,
    RTEMS_DEFAULT_OPTIONS,
    RTEMS_NO_TIMEOUT,
    &eventout
  );
  directive_failed( status, "epos_event_receive" );

  build_time( &time, 2, 12, 1988, 8, 15, 0, 0 );

  print_time( "TA1 - epos_clock_set - ", &time, "\n" );
  status = epos_clock_set( &time );
  directive_failed( status, "epos_clock_set" );

  time.day = 13;
  printk( "TA1 - epos_event_receive all outstanding events" );
  status  = epos_event_receive(
    RTEMS_ALL_EVENTS,
    RTEMS_NO_WAIT | RTEMS_EVENT_ANY,
    0,
    &eventout
  );
  fatal_directive_status(
    status,
    RTEMS_UNSATISFIED,
    "epos_event_receive all events"
  );

  printk( "TA1 - epos_event_send - sending RTEMS_EVENT_10 to self in 1 day" );
  status = epos_timer_fire_when(
    Timer_id[ 1 ],
    &time,
    TA1_send_10_to_self,
    NULL
  );
  directive_failed( status, "epos_timer_fire_when 1 day" );

  time.day = 14;
  printk( "TA1 - epos_event_send - sending RTEMS_EVENT_11 to self in 2 days" );
  status = epos_timer_fire_when(
    Timer_id[ 2 ],
    &time,
    TA1_send_11_to_self,
    NULL
  );
  directive_failed( status, "epos_timer_fire_when 2 days" );

  build_time( &time, 2, 12, 1988, 7, 15, 0, 0 );

  print_time( "TA1 - epos_clock_set - ", &time, "\n" );
  printk( "TA1 - set time backwards" );
  status = epos_clock_set( &time );
  directive_failed( status, "epos_clock_set" );

  status  = epos_event_receive(
    RTEMS_ALL_EVENTS,
    RTEMS_NO_WAIT | RTEMS_EVENT_ANY,
    RTEMS_NO_TIMEOUT,
    &eventout
  );
  if ( eventout ) printk( "ERROR -0x%08x events received\n", eventout );
  else            printk( "TA1 - no events received" );
  fatal_directive_status(
    status,
    RTEMS_UNSATISFIED,
    "epos_event_receive all events"
  );

  build_time( &time, 2, 14, 1988, 7, 15, 0, 0 );

  print_time( "TA1 - epos_clock_set - ", &time, "\n" );
  printk( "TA1 - set time forwards (leave a timer)" );
  status = epos_clock_set( &time );
  directive_failed( status, "epos_clock_set" );

  status  = epos_event_receive(
    RTEMS_ALL_EVENTS,
    RTEMS_NO_WAIT | RTEMS_EVENT_ANY,
    RTEMS_NO_TIMEOUT,
    &eventout
  );
  if ( eventout == RTEMS_EVENT_10 ) printk( "TA1 - RTEMS_EVENT_10 received" );
  else      printk( "ERROR -0x%08x events received\n", eventout );
  directive_failed( status, "epos_event_receive all events" );

  printk( "TA1 - epos_event_send - sending RTEMS_EVENT_11 to self in 100 ticks");
  status = epos_timer_fire_after(
    Timer_id[ 1 ],
    100,
    TA1_send_11_to_self,
    NULL
  );
  directive_failed( status, "epos_timer_fire_after 100 ticks" );

  printk( "TA1 - epos_event_send - sending RTEMS_EVENT_11 to self in 200 ticks");
  status = epos_timer_fire_after(
    Timer_id[ 1 ],
    200,
    TA1_send_11_to_self,
    NULL
  );
  directive_failed( status, "epos_timer_fire_after 200 ticks" );

  printk( "*** END OF TEST 11 ***" );
  epos_test_exit( 0 );
}
