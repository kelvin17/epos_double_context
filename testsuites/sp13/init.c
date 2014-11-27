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
  epos_status_code status;

  printk( "\n\n*** TEST 13 ***" );

  Task_name[ 1 ] =  epos_build_name( 'T', 'A', '1', ' ' );
  Task_name[ 2 ] =  epos_build_name( 'T', 'A', '2', ' ' );
  Task_name[ 3 ] =  epos_build_name( 'T', 'A', '3', ' ' );

  status = epos_task_create(
    Task_name[ 1 ],
    4,
    RTEMS_MINIMUM_STACK_SIZE * 2,
    RTEMS_DEFAULT_MODES,
    RTEMS_DEFAULT_ATTRIBUTES,
    &Task_id[ 1 ]
  );
  directive_failed( status, "epos_task_create of TA1" );

  status = epos_task_create(
    Task_name[ 2 ],
    4,
    RTEMS_MINIMUM_STACK_SIZE * 2,
    RTEMS_DEFAULT_MODES,
    RTEMS_DEFAULT_ATTRIBUTES,
    &Task_id[ 2 ]
  );
  directive_failed( status, "epos_task_create of TA2" );

  status = epos_task_create(
    Task_name[ 3 ],
    4,
    RTEMS_MINIMUM_STACK_SIZE * 2,
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

  Queue_name[ 1 ] = epos_build_name( 'Q', '1', ' ', ' ' );
  Queue_name[ 2 ] = epos_build_name( 'Q', '2', ' ', ' ' );
  Queue_name[ 3 ] = epos_build_name( 'Q', '3', ' ', ' ' );

  status = epos_message_queue_create(
    Queue_name[ 1 ],
    100,
    MESSAGE_SIZE,
    RTEMS_DEFAULT_ATTRIBUTES,
    &Queue_id[ 1 ]
  );
  directive_failed( status, "epos_message_queue_create of Q1" );

  status = epos_message_queue_create(
    Queue_name[ 2 ],
    10,
    MESSAGE_SIZE,
    RTEMS_PRIORITY,
    &Queue_id[ 2 ]
  );
  directive_failed( status, "epos_message_queue_create of Q2" );

  status = epos_message_queue_create(
    Queue_name[ 3 ],
    100,
    MESSAGE_SIZE,
    RTEMS_DEFAULT_ATTRIBUTES,
    &Queue_id[ 3 ]
  );
  directive_failed( status, "epos_message_queue_create of Q3" );

  status = epos_task_delete( RTEMS_SELF );
  directive_failed( status, "epos_task_delete of RTEMS_SELF" );
}
