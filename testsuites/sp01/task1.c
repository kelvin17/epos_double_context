/*  Task_1_through_3
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
 
epos_task Task_1_through_3(
  epos_task_argument argument
)
{
  epos_id          tid;
  epos_time_of_day time;
  epos_status_code status;
 
  status = epos_task_ident( RTEMS_SELF, RTEMS_SEARCH_ALL_NODES, &tid );
  directive_failed( status, "epos_task_ident" );

  while( FOREVER ) {
    status = epos_clock_get( RTEMS_CLOCK_GET_TOD, &time );
    directive_failed( status, "epos_clock_get" );
 
    if ( time.second >= 35 ) {
      printk( "*** END OF TEST 1 ***" );
      epos_test_exit( 0 );
    }
 
    put_name( Task_name[ task_number( tid ) ], FALSE );
    print_time( " - epos_clock_get - ", &time, "\n" );

   // status = epos_task_wake_after( task_number( tid ) * 5 * TICKS_PER_SECOND );
    status = epos_task_wake_after( task_number( tid ) * 5);
    directive_failed( status, "epos_task_wake_after" );
  }
}
