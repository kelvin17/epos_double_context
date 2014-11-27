/*  FP_task
 *
 *  This routine serves as a floating point test task.  It verifies the
 *  basic task switching capabilities of the executive when floating
 *  point is configured.
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
#include "fptest.h"
#include "inttest.h"

epos_task FP_task(
  epos_task_argument argument
)
{
  epos_status_code status;
  epos_id          tid;
  epos_time_of_day time;
  uint32_t    task_index;
  uint32_t    previous_seconds;
  INTEGER_DECLARE;
  FP_DECLARE;

  status = epos_task_ident( RTEMS_SELF, RTEMS_SEARCH_ALL_NODES, &tid );
  directive_failed( status, "epos_task_ident of self" );

  task_index = task_number( tid );

  INTEGER_LOAD( INTEGER_factors[ task_index ] );
  FP_LOAD( FP_factors[ task_index ] );

  put_name( Task_name[ task_index ], FALSE );
  printk( " - integer base = (0x%x)\n", INTEGER_factors[ task_index ] );
  put_name( Task_name[ task_index ], FALSE );
#if ( RTEMS_HAS_HARDWARE_FP == 1 )
  printk( " - float base = (%g)\n", FP_factors[ task_index ] );
#else
  printk( " - float base = (NA)\n" );
#endif

  previous_seconds = -1;

  while( FOREVER ) {

    status = epos_clock_get( RTEMS_CLOCK_GET_TOD, &time );
    directive_failed( status, "epos_clock_get" );

    if ( time.second >= 16 ) {

      if ( task_number( tid ) == 4 ) {
        printk( "TA4 - epos_task_delete - self" );
        status = epos_task_delete( RTEMS_SELF );
        directive_failed( status, "epos_task_delete of TA4" );
      }
      printk( "TA5 - epos_task_delete - TA3" );
      status = epos_task_delete( Task_id[ 3 ] );
      directive_failed( status, "epos_task_delete of TA3" );

      printk( "*** END OF TEST 19 *** " );
      epos_test_exit( 0 );
    }

    if (previous_seconds != time.second)
    {
      put_name( Task_name[ task_index ], FALSE );
      print_time( " - epos_clock_get - ", &time, "\n" );
      previous_seconds = time.second;
    }

    INTEGER_CHECK( INTEGER_factors[ task_index ] );
    FP_CHECK( FP_factors[ task_index ] );

    /* for the first 4 seconds we spin as fast as possible
     * so that we likely are interrupted
     * After that, we go to sleep for a second at a time
     */
    if (time.second >= 4)
    {
      status = epos_task_wake_after( TICKS_PER_SECOND );
      directive_failed( status, "epos_task_wake_after" );
    }
  }
}
