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
#include "inttest.h"

epos_task Task_1(
  epos_task_argument argument
)
{
  epos_status_code status;
  epos_id          tid;
  epos_time_of_day time;
  uint32_t    task_index;
  INTEGER_DECLARE;

  status = epos_task_ident( RTEMS_SELF, RTEMS_SEARCH_ALL_NODES, &tid );
  directive_failed( status, "epos_task_ident" );

  task_index = task_number( tid );

  INTEGER_LOAD( INTEGER_factors[ task_index ] );

  put_name( Task_name[ task_index ], FALSE );
  printk( " - integer base = (0x%x)\n", INTEGER_factors[ task_index ] );

  while( FOREVER ) {
    status = epos_clock_get( RTEMS_CLOCK_GET_TOD, &time );
    directive_failed( status, "epos_clock_get" );

    put_name( Task_name[ task_number( tid ) ], FALSE );
    print_time( " - epos_clock_get - ", &time, "\n" );

    INTEGER_CHECK( INTEGER_factors[ task_index ] );

    status = epos_task_wake_after(
      ( task_number( tid ) ) * 5 * TICKS_PER_SECOND
    );
    directive_failed( status, "epos_task_wake_after" );
  }
}
