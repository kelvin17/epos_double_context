/*  Priority_task
 *
 *  This routine serves as a test task.  It verifies the semaphore manager.
 *
 *  Input parameters:
 *    its_index - priority index
 *
 *  Output parameters:  NONE
 *
 *  COPYRIGHT (c) 1989-2007.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id$
 */

#include "system.h"

epos_task Priority_task(
  epos_task_argument its_index
)
{
  epos_interval      timeout;
  epos_task_priority its_priority;
  epos_task_priority current_priority;
  epos_status_code   status;
  uint32_t      index;

  its_priority = Task_priority[ its_index ];

  if ( its_priority < 3 )
    timeout = 5 * TICKS_PER_SECOND;
  else
    timeout = RTEMS_NO_TIMEOUT;

  put_name( Priority_task_name[ its_index ], FALSE );
  printk( " - epos_semaphore_obtain - wait forever on SM2" );

  status = epos_semaphore_obtain(
    Semaphore_id[ 2 ],
    RTEMS_DEFAULT_OPTIONS,
    timeout
  );
  directive_failed( status, "epos_semaphore_obtain of SM2" );
  if(status == RTEMS_SUCCESSFUL)
  	printk("%s get SM\n ", Priority_task_name[ its_index ]);

  if ( its_priority < 64 ) {
    printk( "PRI%d - WHY AM I HERE? (pri=%d)", its_index, its_priority );
    epos_test_exit( 0 );
  }

  /* special case of setting priority while holding a resource */
  { 
    epos_task_priority priority;
    epos_task_priority old_priority;

    printk( "Set priority of self while holding resource" );
    status =
      epos_task_set_priority( RTEMS_SELF, RTEMS_CURRENT_PRIORITY, &priority );
    directive_failed( status, "epos_task_set_priority get current" );
    status = epos_task_set_priority( RTEMS_SELF, priority, &old_priority );
    directive_failed( status, "epos_task_set_priority with resource" );
    if ( priority != old_priority ) {
      printk( "priority != old_priority (%d != %d)\n", priority, old_priority );
      epos_test_exit(0);
    }
  }

  if ( its_index == 5 )
    printk( "PRI5 - epos_task_suspend - until all priority tasks blocked" );
  status = epos_task_suspend( RTEMS_SELF );
  directive_failed( status, "epos_task_suspend" );

  printk( "PRI5 - epos_task_delete - all tasks waiting on SM2" );
  for ( index = 1 ; index < 5 ; index++ ) {
    status = epos_task_delete( Priority_task_id[ index ] );
    directive_failed( status, "epos_task_delete loop" );
  }

  printk( "PRI5 - epos_semaphore_obtain - nested" );
  status = epos_semaphore_obtain(
    Semaphore_id[ 2 ],
    RTEMS_DEFAULT_OPTIONS,
    timeout
  );
  directive_failed( status, "epos_semaphore_obtain nested" );

  printk( "PRI5 - epos_semaphore_release - nested" );
  status = epos_semaphore_release( Semaphore_id[ 2 ] );
  directive_failed( status, "epos_semaphore_release nested " );

  printk( "PRI5 - epos_semaphore_release - restore priority" );
  status = epos_semaphore_release( Semaphore_id[ 2 ] );
  directive_failed( status, "epos_semaphore_release" );

  status = epos_task_set_priority(
    RTEMS_SELF,
    RTEMS_CURRENT_PRIORITY,
    &current_priority
  );
  directive_failed( status, "PRI5 epos_task_set_priority CURRENT" );
  printk( "PRI5 - priority of PRI5 is %d\n", current_priority );

  (void) epos_task_suspend( RTEMS_SELF );
}
