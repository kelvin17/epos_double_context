/*  Task_1
 *
 *  This routine serves as a test task.  It verifies the basic task
 *  switching capabilities of the executive.
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
  epos_id          tmid;
  epos_time_of_day time;
  epos_status_code status;

/* Get id */

  printk( "TA1 - epos_timer_ident - identing timer 1" );
  status = epos_timer_ident( Timer_name[ 1 ], &tmid );
  directive_failed( status, "epos_timer_ident" );
  printk( "TA1 - timer 1 has id (0x%x)\n", tmid );

/* after which is allowed to fire */

  Print_time();

  printk( "TA1 - epos_timer_fire_after - timer 1 in 3 seconds" );
  status = epos_timer_fire_after(
    tmid,
    3 * TICKS_PER_SECOND,
    Delayed_resume,
    NULL
  );
  directive_failed( status, "epos_timer_fire_after" );

  printk( "TA1 - epos_task_suspend( RTEMS_SELF )" );
  status = epos_task_suspend( RTEMS_SELF );
  directive_failed( status, "epos_task_suspend" );

  Print_time();

/* after which is reset and allowed to fire */

  printk( "TA1 - epos_timer_fire_after - timer 1 in 3 seconds" );
  status = epos_timer_fire_after(
    tmid,
    3 * TICKS_PER_SECOND,
    Delayed_resume,
    NULL
  );
  directive_failed( status, "epos_timer_fire_after" );

  printk( "TA1 - epos_task_wake_after - 1 second" );
  status = epos_task_wake_after( 1 * TICKS_PER_SECOND );
  directive_failed( status, "epos_task_wake_after" );

  Print_time();

  printk( "TA1 - epos_timer_reset - timer 1" );
  status = epos_timer_reset( tmid );
  directive_failed( status, "epos_timer_reset" );

  printk( "TA1 - epos_task_suspend( RTEMS_SELF )" );
  status = epos_task_suspend( RTEMS_SELF );
  directive_failed( status, "epos_task_suspend" );

  Print_time();

  epos_test_pause();

  /*
   *  Reset the time since we do not know how long the user waited
   *  before pressing <cr> at the pause.  This insures that the
   *  actual output matches the screen.
   */

  build_time( &time, 12, 31, 1988, 9, 0, 7, 0 );

  status = epos_clock_set( &time );
  directive_failed( status, "epos_clock_set" );

/* after which is canceled */

  printk( "TA1 - epos_timer_fire_after - timer 1 in 3 seconds" );
  status = epos_timer_fire_after(
    tmid,
    3 * TICKS_PER_SECOND,
    Delayed_resume,
    NULL
  );
  directive_failed( status, "epos_timer_fire_after" );

  printk( "TA1 - epos_timer_cancel - timer 1" );
  status = epos_timer_cancel( tmid );
  directive_failed( status, "epos_timer_cancel" );

/* when which is allowed to fire */

  Print_time();

  status = epos_clock_get( RTEMS_CLOCK_GET_TOD, &time );
  directive_failed( status, "epos_clock_get" );

  time.second += 3;

  printk( "TA1 - epos_timer_fire_when - timer 1 in 3 seconds" );
  status = epos_timer_fire_when( tmid, &time, Delayed_resume, NULL );
  directive_failed( status, "epos_timer_fire_when" );

  printk( "TA1 - epos_task_suspend( RTEMS_SELF )" );
  status = epos_task_suspend( RTEMS_SELF );
  directive_failed( status, "epos_task_suspend" );

  Print_time();

/* when which is canceled */

  status = epos_clock_get( RTEMS_CLOCK_GET_TOD, &time );
  directive_failed( status, "epos_clock_get" );

  time.second += 3;

  printk( "TA1 - epos_timer_fire_when - timer 1 in 3 seconds" );
  status = epos_timer_fire_when( tmid, &time, Delayed_resume, NULL );
  directive_failed( status, "epos_timer_fire_when" );

  printk( "TA1 - epos_task_wake_after - 1 second" );
  status = epos_task_wake_after( 1 * TICKS_PER_SECOND );
  directive_failed( status, "epos_task_wake_after" );

  Print_time();

  printk( "TA1 - epos_timer_cancel - timer 1" );
  status = epos_timer_cancel( tmid );
  directive_failed( status, "epos_timer_cancel" );

/* delete */
  printk( "TA1 - epos_task_wake_after - YIELD (only task at priority)" );
  status = epos_task_wake_after( RTEMS_YIELD_PROCESSOR );
  directive_failed( status, "epos_task_wake_after" );

  printk( "TA1 - timer_deleting - timer 1" );
  status = epos_timer_delete( tmid );
  directive_failed( status, "epos_timer_delete" );

  printk( "*** END OF TEST 22 *** " );
  epos_test_exit( 0 );
}
