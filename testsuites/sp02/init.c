/*  Init
 *
 *   This routine is the initialization task for this test program.
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
  epos_status_code status;

  printk( "\n\n*** TEST 2 ***" );

  Preempt_task_name =  epos_build_name( 'P', 'R', 'M', 'T' );

  status = epos_task_create(
     Preempt_task_name,
     1,
     RTEMS_MINIMUM_STACK_SIZE,
     RTEMS_DEFAULT_MODES,
     RTEMS_DEFAULT_ATTRIBUTES,
     &Preempt_task_id
  );
  directive_failed( status, "epos_task_create of RTEMS_PREEMPT" );
  
  status = epos_task_start( Preempt_task_id, Preempt_task, 0 );
  directive_failed( status, "epos_task_start of RTEMS_PREEMPT" );

  printk( "INIT - epos_task_wake_after - yielding processor" );
  status = epos_task_wake_after( RTEMS_YIELD_PROCESSOR );
  directive_failed( status, "epos_task_wake_after" );

  Task_name[ 1 ] =  epos_build_name( 'T', 'A', '1', ' ' );
  Task_name[ 2 ] =  epos_build_name( 'T', 'A', '2', ' ' );
  Task_name[ 3 ] =  epos_build_name( 'T', 'A', '3', ' ' );

  status = epos_task_create(
     Task_name[ 1 ],
     3,
     RTEMS_MINIMUM_STACK_SIZE,
     RTEMS_DEFAULT_MODES,
     RTEMS_DEFAULT_ATTRIBUTES,
     &Task_id[ 1 ]
  );
  directive_failed( status, "epos_task_create of TA1" );

  status = epos_task_create(
     Task_name[ 2 ],
     3,
     RTEMS_MINIMUM_STACK_SIZE,
     RTEMS_DEFAULT_MODES,
     RTEMS_DEFAULT_ATTRIBUTES,
     &Task_id[ 2 ]
  );
  directive_failed( status, "epos_task_create of TA2" );

  status = epos_task_create(
     Task_name[ 3 ],
     3,
     RTEMS_MINIMUM_STACK_SIZE,
     RTEMS_DEFAULT_MODES,
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

  printk( "INIT - suspending TA2 while middle task on a ready chain" );
  status = epos_task_suspend( Task_id[ 2 ] );
  directive_failed( status, "epos_task_suspend of TA2" );

  status = epos_task_delete( Task_id[ 1 ] );
  directive_failed( status, "epos_task_delete of TA1" );

  status = epos_task_delete( Task_id[ 2 ] );
  directive_failed( status, "epos_task_delete of TA2" );

  status = epos_task_delete( Task_id[ 3 ] );
  directive_failed( status, "epos_task_delete of TA3" );

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
     3,
     RTEMS_MINIMUM_STACK_SIZE,
     RTEMS_DEFAULT_MODES,
     RTEMS_DEFAULT_ATTRIBUTES,
     &Task_id[ 2 ]
  );
  directive_failed( status, "epos_task_create of TA2" );

  status = epos_task_create(
     Task_name[ 3 ],
     3,
     RTEMS_MINIMUM_STACK_SIZE,
     RTEMS_DEFAULT_MODES,
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
