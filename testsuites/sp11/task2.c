/*  Task_2
 *
 *  This routine serves as a test task.  Multiple tasks are required to
 *  verify all capabilities of the event manager.
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

epos_task Task_2(
  epos_task_argument argument
)
{
  epos_event_set   eventout;
  epos_time_of_day time;
  epos_status_code status;

  status = epos_task_wake_after( 1*TICKS_PER_SECOND );
  directive_failed( status, "epos_task_wake_after" );

  printk( "TA2 - epos_event_receive - waiting forever on RTEMS_EVENT_16" );
  status = epos_event_receive(
    RTEMS_EVENT_16,
    RTEMS_DEFAULT_OPTIONS,
    RTEMS_NO_TIMEOUT,
    &eventout
  );
  directive_failed( status, "epos_event_receive" );
  printk( "TA2 - RTEMS_EVENT_16 received - eventout => %08x\n", eventout );

  printk(
    "TA2 - epos_event_send - send RTEMS_EVENT_14 and RTEMS_EVENT_15 to TA1"
  );
  status = epos_event_send( Task_id[ 1 ], RTEMS_EVENT_14 | RTEMS_EVENT_15 );
  directive_failed( status, "epos_event_send" );

  printk(
    "TA2 - epos_event_receive - RTEMS_EVENT_17 or "
      "RTEMS_EVENT_18 - forever and ANY"
  );
  status = epos_event_receive(
    RTEMS_EVENT_17 | RTEMS_EVENT_18,
    RTEMS_EVENT_ANY,
    RTEMS_NO_TIMEOUT,
    &eventout
  );
  directive_failed( status, "epos_event_receive" );
  printk(
    "TA2 - RTEMS_EVENT_17 or RTEMS_EVENT_18 received - eventout => %08x\n",
    eventout
  );

  printk( "TA2 - epos_event_send - send RTEMS_EVENT_14 to TA1" );
  status = epos_event_send( Task_id[ 1 ], RTEMS_EVENT_14 );
  directive_failed( status, "epos_event_send" );

  build_time( &time, 2, 12, 1988, 8, 15, 0, 0 );
  print_time( "TA2 - epos_clock_set - ", &time, "\n" );
  status = epos_clock_set( &time );
  directive_failed( status, "TA2 epos_clock_set" );

  time.second += 4;
  printk(
    "TA2 - epos_event_send - sending RTEMS_EVENT_10 to self after 4 seconds"
  );
  status = epos_timer_fire_when(
    Timer_id[ 5 ],
    &time,
    TA2_send_10_to_self,
    NULL
  );
  directive_failed( status, "epos_timer_fire_when after 4 seconds" );

  printk( "TA2 - epos_event_receive - waiting forever on RTEMS_EVENT_10" );
  status = epos_event_receive(
    RTEMS_EVENT_10,
    RTEMS_DEFAULT_OPTIONS,
    RTEMS_NO_TIMEOUT,
    &eventout
  );
  directive_failed( status, "epos_event_receive" );

  status = epos_clock_get( RTEMS_CLOCK_GET_TOD, &time );
  directive_failed( status, "epos_clock_get" );

  printk( "TA2 - RTEMS_EVENT_10 received - eventout => %08x\n", eventout );
  print_time( "TA2 - epos_clock_get - ", &time, "\n" );

  printk( "TA2 - epos_event_receive - RTEMS_PENDING_EVENTS" );
  status = epos_event_receive(
    RTEMS_PENDING_EVENTS,
    RTEMS_DEFAULT_OPTIONS,
    RTEMS_NO_TIMEOUT,
    &eventout
  );
  directive_failed( status, "epos_event_receive" );
  printk( "TA2 - eventout => %08x\n", eventout );

  printk( "TA2 - epos_event_receive - RTEMS_EVENT_19 - RTEMS_NO_WAIT" );
  status = epos_event_receive(
    RTEMS_EVENT_19,
    RTEMS_NO_WAIT,
    RTEMS_NO_TIMEOUT,
    &eventout
  );
  directive_failed( status, "epos_event_receive" );
  printk( "TA2 - RTEMS_EVENT_19 received - eventout => %08x\n", eventout );

  printk( "TA2 - epos_task_delete - deletes self" );
  status = epos_task_delete( Task_id[ 2 ] );
  directive_failed( status, "epos_task_delete of TA2" );
}
