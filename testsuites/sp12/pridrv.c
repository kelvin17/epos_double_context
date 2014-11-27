/*  Priority_test_driver
 *
 *  This routine is the initialization task for this test program.
 *  It is a user initialization task and has the responsibility for creating
 *  and starting the tasks that make up the test.  If the time of day
 *  clock is required for the test, it should also be set to a known
 *  value by this function.
 *
 *  Input parameters:
 *    priority_base - priority_base switch
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

void Priority_test_driver(
  uint32_t   priority_base
)
{
  epos_task_priority previous_priority;
  uint32_t      index;
  epos_status_code   status;

  for ( index = 1 ; index <= 5 ; index++ ) {
    switch ( index ) {
       case 1:
       case 2:
       case 3:
         Task_priority[ index ] = priority_base + index;
         break;
       default:
         Task_priority[ index ] = priority_base + 3;
         break;
    }

    status = epos_task_create(
      Priority_task_name[ index ],
      Task_priority[ index ],
      RTEMS_MINIMUM_STACK_SIZE * 2,
      RTEMS_DEFAULT_MODES,
      RTEMS_DEFAULT_ATTRIBUTES,
      &Priority_task_id[ index ]
    );
    directive_failed( status, "epos_task_create loop" );

  }

  if ( priority_base == 0 ) {
    for ( index = 1 ; index <= 5 ; index++ ) {
      status = epos_task_start(
        Priority_task_id[ index ],
        Priority_task,
        index
      );
      directive_failed( status, "epos_task_start loop" );
    }
  } else {
    for ( index = 5 ; index >= 1 ; index-- ) {
      status = epos_task_start(
        Priority_task_id[ index ],
        Priority_task,
        index
      );
      directive_failed( status, "epos_task_start loop" );

      status = epos_task_wake_after( TICKS_PER_SECOND );
      directive_failed( status, "epos_task_wake_after loop" );

      if ( priority_base == 64 ) {
        if ( index == 4 ) {
          status = epos_task_set_priority(
            Priority_task_id[ 5 ],
            priority_base + 4,
            &previous_priority
          );
          printk( "PDRV - change priority of PRI5 from %d to %d\n",
             previous_priority,
             priority_base + 4
          );
          directive_failed( status, "PDRV epos_task_set_priority" );
        }
        status = epos_task_set_priority(
          Priority_task_id[ 5 ],
          RTEMS_CURRENT_PRIORITY,
          &previous_priority
        );
        directive_failed( status, "PDRV epos_task_set_priority CURRENT" );
        printk( "PDRV - priority of PRI5 is %d\n", previous_priority );
      }
    }
  }

  status = epos_task_wake_after( TICKS_PER_SECOND );
  directive_failed( status, "epos_task_wake_after after loop" );

  if ( priority_base == 0 ) {
    for ( index = 1 ; index <= 5 ; index++ ) {
      status = epos_semaphore_release( Semaphore_id[ 2 ] );
      directive_failed( status, "epos_semaphore_release loop" );
    }
  }

  if ( priority_base == 64 ) {
    printk( "PDRV - epos_task_resume - PRI5" );
    status = epos_task_resume( Priority_task_id[ 5 ] );
    directive_failed( status, "epos_task_resume" );

    status = epos_task_wake_after( 1 * TICKS_PER_SECOND );
    directive_failed( status, "epos_task_wake_after so PRI5 can run" );

    status = epos_task_delete( Priority_task_id[ 5 ] );
    directive_failed( status, "epos_task_delete of PRI5" );
  }
  else {
    for ( index = 1 ; index <= 5 ; index++ ) {
      status = epos_task_delete( Priority_task_id[ index ] );
      directive_failed( status, "epos_task_delete loop" );
    }
  }
}
