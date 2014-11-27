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
 *  COPYRIGHT (c) 1989-1999.
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

epos_extensions_table Extensions = {
  NULL,                      /* task create user extension */
  NULL,                      /* task start user extension */
  NULL,                      /* task restart user extension */
  NULL,                      /* task delete user extension */
  Task_switch,               /* task switch user extension */
  NULL,                      /* task begin user extension */
  NULL,                      /* task exitted user extension */
  NULL                       /* fatal error user extension */
};

epos_task Init(
  epos_task_argument argument
)
{
  epos_status_code status;
  epos_time_of_day time;

  printk( "\n\n*** TEST 4 ***" );
  build_time( &time, 12, 31, 1988, 9, 15, 0, 0 );

  status = epos_clock_set( &time );
  directive_failed( status, "epos_clock_set" );

  Extension_name[ 1 ] =  epos_build_name( 'E', 'X', 'T', ' ' );

  status = epos_extension_create(
    Extension_name[ 1 ],
    &Extensions,
    &Extension_id[ 1 ]
  );
  directive_failed( status, "epos_extension_create" );

  Task_name[ 1 ] =  epos_build_name( 'T', 'A', '1', ' ' );
  Task_name[ 2 ] =  epos_build_name( 'T', 'A', '2', ' ' );
  Task_name[ 3 ] =  epos_build_name( 'T', 'A', '3', ' ' );

  Run_count[ 1 ] = 0;
  Run_count[ 2 ] = 0;
  Run_count[ 3 ] = 0;

  status = epos_task_create(
     Task_name[ 1 ],
     1,
     RTEMS_MINIMUM_STACK_SIZE * 2,
     RTEMS_TIMESLICE,
     RTEMS_DEFAULT_ATTRIBUTES,
     &Task_id[ 1 ]
  );
  directive_failed( status, "epos_task_create of TA1" );

  status = epos_task_create(
     Task_name[ 2 ],
     1,
     RTEMS_MINIMUM_STACK_SIZE * 2,
     RTEMS_TIMESLICE,
     RTEMS_DEFAULT_ATTRIBUTES,
     &Task_id[ 2 ]
  );
  directive_failed( status, "epos_task_create of TA2" );

  status = epos_task_create(
     Task_name[ 3 ],
     1,
     RTEMS_MINIMUM_STACK_SIZE * 2,
     RTEMS_TIMESLICE,
     RTEMS_DEFAULT_ATTRIBUTES,
     &Task_id[ 3 ]
  );
  directive_failed( status, "epos_task_create of TA3" );

  status = epos_task_start( Task_id[ 1 ], Task_1, 0 );
  directive_failed( status, "epos_task_start of TA1" );

  status = epos_task_start( Task_id[ 2 ], Task_2, 0 );
  directive_failed( status, "epos_task_start of TA2" );

  status = epos_task_start( Task_id[ 3 ], Task_3, 0 );
  directive_failed( status, "epos_task_start of TA3" );

  status = epos_task_delete( RTEMS_SELF );
  directive_failed( status, "epos_task_delete of RTEMS_SELF" );
}
