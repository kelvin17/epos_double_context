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

epos_task Init(
  epos_task_argument argument
)
{
  epos_status_code   status;
  epos_task_priority previous_priority;

  printk( "\n\n*** TEST 6 ***" );

  Task_name[ 1 ] =  epos_build_name( 'T', 'A', '1', ' ' );
  Task_name[ 2 ] =  epos_build_name( 'T', 'A', '2', ' ' );
  Task_name[ 3 ] =  epos_build_name( 'T', 'A', '3', ' ' );

  Argument = 0;
  Restart_argument = 1;

  status = epos_task_create(
     Task_name[ 1 ],
     1,
     RTEMS_MINIMUM_STACK_SIZE,
     RTEMS_DEFAULT_MODES,
     RTEMS_DEFAULT_ATTRIBUTES,
     &Task_id[ 1 ]
  );
  directive_failed( status, "epos_task_create of TA1" );

  status = epos_task_create(
     Task_name[ 2 ],
     1,
     RTEMS_MINIMUM_STACK_SIZE,
     RTEMS_DEFAULT_MODES,
     RTEMS_DEFAULT_ATTRIBUTES,
     &Task_id[ 2 ]
  );
  directive_failed( status, "epos_task_create of TA2" );

  status = epos_task_create(
     Task_name[ 3 ],
     10,
     RTEMS_MINIMUM_STACK_SIZE,
     RTEMS_DEFAULT_MODES,
     RTEMS_DEFAULT_ATTRIBUTES,
     &Task_id[ 3 ]
  );
  directive_failed( status, "epos_task_create of TA3" );

  status = epos_task_start( Task_id[ 1 ], Task_1, 0 );
  directive_failed( status, "epos_task_start of TA1" );

  status = epos_task_start( Task_id[ 2 ], Task_2, Argument );//Argument = 0;
  directive_failed( status, "epos_task_start of TA2" );

  status = epos_task_start( Task_id[ 3 ], Task_3, Argument );//Argument = 0;
  directive_failed( status, "epos_task_start of TA3" );

  status = epos_task_set_priority( Task_id[ 3 ], 5, &previous_priority );

  status = epos_task_delete( RTEMS_SELF );
  directive_failed( status, "epos_task_delete of RTEMS_SELF" );
}
