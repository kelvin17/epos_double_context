/*  Init
 *
 *  This routine is the initialization task for this test program.
 *  It is a user initialization task and has the responsibility for creating
 *  and starting the tasks that make up the test.  If the time of day
 *  clock is required for the test, it should also be set to a known
 *  value by this function.
 *
 *  Input parameters:
 *    argument - task argument
 *
 *  Output parameters:  NONE
 *
 *  COPYRIGHT (c) 1989-2002.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id$
 */

#define CONFIGURE_INIT
#include "system.h"

epos_task Init(
  epos_task_argument argument
)
{
  epos_time_of_day  time;
  epos_status_code  status;

  printk( "\n\n*** TEST 31 ***" );

  build_time( &time, 12, 31, 1988, 9, 0, 0, 0 );

  status = epos_clock_set( &time );
  directive_failed( status, "epos_clock_set" );

  status = epos_timer_initiate_server(
    RTEMS_TIMER_SERVER_DEFAULT_PRIORITY,
    RTEMS_MINIMUM_STACK_SIZE,
    RTEMS_DEFAULT_ATTRIBUTES
  );
  directive_failed( status, "epos_timer_initiate_server" );

  Task_name[ 1 ]  = epos_build_name( 'T', 'A', '1', ' ' );
  Timer_name[ 1 ] = epos_build_name( 'T', 'M', '1', ' ' );
  Timer_name[ 2 ] = epos_build_name( 'T', 'M', '2', ' ' );

  status = epos_task_create(
    Task_name[ 1 ],
    1,
    RTEMS_MINIMUM_STACK_SIZE * 2,
    RTEMS_DEFAULT_MODES,
    RTEMS_DEFAULT_ATTRIBUTES,
    &Task_id[ 1 ]
  );
  directive_failed( status, "epos_task_create of TA1" );

  status = epos_task_start( Task_id[ 1 ], Task_1, 0 );
  directive_failed( status, "epos_task_start of TA1" );

  printk( "INIT - epos_timer_create - creating timer 1" );
  status = epos_timer_create( Timer_name[ 1 ], &Timer_id[ 1 ] );
  directive_failed( status, "epos_timer_create" );
  printk( "INIT - timer 1 has id (0x%x)\n", Timer_id[ 1 ] );

  printk( "INIT - epos_timer_create - creating timer 2" );
  status = epos_timer_create( Timer_name[ 2 ], &Timer_id[ 2 ] );
  directive_failed( status, "epos_timer_create" );
  printk( "INIT - timer 2 has id (0x%x)\n", Timer_id[ 2 ] );

  status = epos_task_delete( RTEMS_SELF );
  directive_failed( status, "epos_task_delete of RTEMS_SELF" );
}
