/*  Task_2
 *
 *  This routine serves as a test task.  It repeatedly gets the system time
 *  and date, converts it to printable form, and print it.
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
  epos_status_code status;
  epos_time_of_day time;

  while( FOREVER ) {
    status = epos_task_wake_after( 1*TICKS_PER_SECOND );
    directive_failed( status, "epos_task_wake_after" );

    status = epos_clock_get( RTEMS_CLOCK_GET_TOD, &time );
    directive_failed( status, "epos_clock_get" );

    if ( time.second >= 17 ) {
      printk( "*** END OF TEST 3 *** " );
      epos_test_exit( 0 );
    }

    put_name( Task_name[ 2 ], FALSE );
    print_time( "  ", &time, "\n" );
  }
}
