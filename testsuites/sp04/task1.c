/*  Task_1
 *
 *  This test serves as a test task.  It verifies timeslicing activities
 *  and tswitch extension processing.
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

static void
showTaskSwitches (void)
{
  int i;
  int switches = taskSwitchLogIndex;

  for (i = 0 ; i < switches ; i++) {
      put_name( Task_name[taskSwitchLog[i].taskIndex], FALSE );
      print_time( "- ", &taskSwitchLog[i].when, "\n" );
  }
}

epos_task Task_1(
  epos_task_argument argument
)
{
  uint32_t    seconds;
  uint32_t    old_seconds;
  epos_mode        previous_mode;
  epos_time_of_day time;
  epos_status_code status;
  uint32_t    start_time;
  uint32_t    end_time;

  printk( "TA1 - epos_task_suspend - on Task 2" );
  status = epos_task_suspend( Task_id[ 2 ] );
  directive_failed( status, "epos_task_suspend of TA2" );

  printk( "TA1 - epos_task_suspend - on Task 3" );
  status = epos_task_suspend( Task_id[ 3 ] );
  directive_failed( status, "epos_task_suspend of TA3" );

  status = epos_clock_get( RTEMS_CLOCK_GET_SECONDS_SINCE_EPOCH, &start_time );
  directive_failed( status, "epos_clock_get" );

  printk( "TA1 - killing time" );

  for ( ; ; ) {
    status = epos_clock_get( RTEMS_CLOCK_GET_SECONDS_SINCE_EPOCH, &end_time );
    directive_failed( status, "epos_clock_get" );

    if ( end_time > (start_time + 2) )
      break;
  }

  printk( "TA1 - epos_task_resume - on Task 2" );
  status = epos_task_resume( Task_id[ 2 ] );
  directive_failed( status, "epos_task_resume of TA2" );

  printk( "TA1 - epos_task_resume - on Task 3" );
  status = epos_task_resume( Task_id[ 3 ] );
  directive_failed( status, "epos_task_resume of TA3" );

  while ( FOREVER ) {
    if ( Run_count[ 1 ] >= 3 ) {
      printk( "TA1 - epos_task_mode - change mode to NO RTEMS_PREEMPT" );

      status = epos_task_mode(
        RTEMS_NO_PREEMPT,
        RTEMS_PREEMPT_MASK,
        &previous_mode
      );
      directive_failed( status, "epos_task_mode" );

      status = epos_clock_get( RTEMS_CLOCK_GET_TOD, &time );
      directive_failed( status, "epos_clock_get" );

      old_seconds = time.second;

      for ( seconds = 0 ; seconds < 6 ; ) {
        status = epos_clock_get( RTEMS_CLOCK_GET_TOD, &time );
        directive_failed( status, "epos_clock_get" );

        if ( time.second != old_seconds ) {
          old_seconds = time.second;
          seconds++;
          print_time( "TA1 - ", &time, "\n" );
        }
      }

      printk( "TA1 - epos_task_mode - change mode to RTEMS_PREEMPT" );
      status = epos_task_mode(
        RTEMS_PREEMPT,
        RTEMS_PREEMPT_MASK,
        &previous_mode
      );
      directive_failed( status, "epos_task_mode" );

      while ( !testsFinished );
      showTaskSwitches ();
      printk( "*** END OF TEST 4 ***" );
      epos_test_exit (0);
    }
  }
}
