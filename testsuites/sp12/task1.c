/*  Task_1
 *
 *  This routine serves as a test task.  It verifies the semaphore manager.
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

epos_task Task_1(
  epos_task_argument argument
)
{
  epos_id          smid;
  epos_status_code status;

  status = epos_semaphore_ident(
    Semaphore_name[ 1 ],
    RTEMS_SEARCH_ALL_NODES,
    &smid
  );
  printk( "TA1 - epos_semaphore_ident - smid => %08x\n", smid );
  directive_failed( status, "epos_semaphore_ident of SM1" );

  printk( "TA1 - epos_semaphore_obtain - wait forever on SM2" );
  status = epos_semaphore_obtain(
    Semaphore_id[ 2 ],
    RTEMS_DEFAULT_OPTIONS,
    RTEMS_NO_TIMEOUT
  );
  directive_failed( status, "epos_semaphore_obtain of SM2" );
  printk( "TA1 - got SM2" );

  printk( "TA1 - epos_semaphore_obtain - wait forever on SM3" );
  status = epos_semaphore_obtain(
    Semaphore_id[ 3 ],
    RTEMS_DEFAULT_OPTIONS,
    RTEMS_NO_TIMEOUT
  );
  directive_failed( status, "epos_semaphore_obtain of SM3" );
  printk( "TA1 - got SM3" );

  printk( "TA1 - epos_semaphore_obtain - get SM1 - RTEMS_NO_WAIT" );
  status = epos_semaphore_obtain(
    Semaphore_id[ 1 ],
    RTEMS_NO_WAIT,
    RTEMS_NO_TIMEOUT
  );
  directive_failed( status, "epos_semaphore_obtain of SM1" );
  printk( "TA1 - got SM1" );

  printk( "TA1 - epos_task_wake_after - sleep 5 seconds" );
  status = epos_task_wake_after( 5*TICKS_PER_SECOND );
  directive_failed( status, "epos_task_wake_after" );

epos_test_pause();

  printk( "TA1 - epos_semaphore_release - release SM1" );
  status = epos_semaphore_release( Semaphore_id[ 1 ] );
  directive_failed( status, "epos_semaphore_release of SM1" );

  printk(
    "TA1 - epos_semaphore_obtain - waiting for SM1 with 10 second timeout"
  );
  status = epos_semaphore_obtain(
    Semaphore_id[ 1 ],
    RTEMS_DEFAULT_OPTIONS,
    10 * TICKS_PER_SECOND
  );
  directive_failed( status, "epos_semaphore_obtain of SM1" );
  printk( "TA1 - got SM1" );

  printk( "TA1 - epos_semaphore_release - release SM2" );
  status = epos_semaphore_release( Semaphore_id[ 2 ] );
  directive_failed( status, "epos_semaphore_release of SM2" );

  printk( "TA1 - epos_task_wake_after - sleep 5 seconds" );
  status = epos_task_wake_after( 5*TICKS_PER_SECOND );
  directive_failed( status, "epos_task_wake_after" );

epos_test_pause();

  printk( "TA1 - epos_task_delete - delete TA3" );
  status = epos_task_delete( Task_id[ 3 ] );
  directive_failed( status, "epos_task_delete of TA3" );

  status = epos_task_create(
    Task_name[ 4 ],
    4,
    RTEMS_MINIMUM_STACK_SIZE,
    RTEMS_DEFAULT_MODES,
    RTEMS_DEFAULT_ATTRIBUTES,
    &Task_id[ 4 ]
  );
  directive_failed( status, "epos_task_create of TA4" );

  status = epos_task_create(
    Task_name[ 5 ],
    4,
    RTEMS_MINIMUM_STACK_SIZE,
    RTEMS_DEFAULT_MODES,
    RTEMS_DEFAULT_ATTRIBUTES,
    &Task_id[ 5 ]
   );
  directive_failed( status, "epos_task_create of TA5" );

  status = epos_task_start( Task_id[ 4 ], Task_4, 0 );
  directive_failed( status, "epos_task_start of TA4" );

  status = epos_task_start( Task_id[ 5 ], Task5, 0 );
  directive_failed( status, "epos_task_start of TA5" );

  printk( "TA1 - epos_task_wake_after - sleep 5 seconds" );
  status = epos_task_wake_after( 5*TICKS_PER_SECOND );
  directive_failed( status, "epos_task_wake_after" );

  printk( "TA1 - epos_task_delete - delete TA4" );
  status = epos_task_delete( Task_id[ 4 ] );
  directive_failed( status, "epos_task_delete of TA4" );

  printk( "TA1 - epos_semaphore_release - release SM1" );
  status = epos_semaphore_release( Semaphore_id[ 1 ] );
  directive_failed( status, "epos_semaphore_release on SM1" );

  printk( "TA1 - epos_task_wake_after - sleep 5 seconds" );
  status = epos_task_wake_after( 5*TICKS_PER_SECOND );
  directive_failed( status, "epos_task_wake_after" );

  printk( "TA1 - epos_semaphore_delete - delete SM1" );
  status = epos_semaphore_delete( Semaphore_id[ 1 ] );
  directive_failed( status, "epos_semaphore_delete of SM1" );

  printk( "TA1 - epos_semaphore_delete - delete SM3" );
  status = epos_semaphore_delete( Semaphore_id[ 3 ] );
  directive_failed( status, "epos_semaphore_delete of SM3" );

  printk( "TA1 - epos_task_delete - delete self" );
  status = epos_task_delete( RTEMS_SELF );
  directive_failed( status, "epos_task_delete of TA1" );
}
