/*  Task_1_through_3
 *
 *  This task is a cyclic version of test1 to asssure that the times
 *  displayed are not skewed as in test1.  "TA1" is printed once every
 *  5 seconds, "TA2" is printed once every 10 seconds, and "TA3" is
 *  printed once every 15 seconds.  The times displayed should be
 *  in multiples of 5, 10, and 15 for TA1, TA2, and TA3 respectively.
 *  If the times are skewed from these values, then the calendar time
 *  does not correspond correctly with the number of ticks.
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

epos_task Task_1_through_3(
  epos_task_argument argument
)
{
  epos_id          tid;
  epos_time_of_day time;
  epos_status_code status;

  status = epos_task_ident( RTEMS_SELF, RTEMS_SEARCH_ALL_NODES, &tid );
  directive_failed( status, "epos_task_ident of self" );

  while ( FOREVER )  {
    status = epos_timer_fire_after(
      Timer_id[ argument ],
      task_number( tid ) * 5 * TICKS_PER_SECOND,
      Resume_task,
      NULL
    );
    directive_failed( status, "tm_fire_after failed" );

    status = epos_clock_get( RTEMS_CLOCK_GET_TOD, &time );
    directive_failed( status, "epos_clock_get failed" );

    if ( time.second >= 35 ) {
      printk( "*** END OF TEST 24 ***" );
      epos_test_exit( 0 );
    }

    put_name( Task_name[ task_number( tid ) ], FALSE );
    print_time( " - epos_clock_get - ", &time, "\n" );

    status = epos_task_suspend( RTEMS_SELF );
    directive_failed( status, "epos_task_suspend" );
  }
}
