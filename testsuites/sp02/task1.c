/*  Task_1
 *
 *  This routine serves as a test task.  It verifies the task manager.
 *
 *  Input parameters:
 *    argument - task argument
 *
 *  Output parameters:  NONE
 *
 *  COPYRIGHT (c) 1989-2008.
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
  epos_id          tid2;
  epos_id          tid3;
  epos_status_code status;
  epos_name        tid2_name;
  uint32_t    previous_priority;

  printk( "TA1 - epos_task_wake_after - sleep 1 second" );
  status = epos_task_wake_after( 1*TICKS_PER_SECOND );
  directive_failed( status, "epos_task_wake_after" );

  status = epos_task_ident( Task_name[ 2 ], RTEMS_SEARCH_ALL_NODES, &tid2 );
  directive_failed( status, "epos_task_ident of TA2" );

  printk( "TA1 - epos_task_ident - tid of TA2 (0x%.8x)\n", tid2 );

  status = epos_object_get_classic_name( tid2, &tid2_name );
  directive_failed( status, "epos_object_get_classic_name of TA2" );

  printk( "TA1 - epos_get_classic_name - id -> name of TA2 %sOK\n",
    (tid2_name != Task_name[2]) ? "NOT " : "" );

  status = epos_task_ident( Task_name[ 3 ], RTEMS_SEARCH_ALL_NODES, &tid3 );
  directive_failed( status, "epos_task_ident of TA3" );

  printk( "TA1 - epos_task_ident - tid of TA3 (0x%.8x)\n", tid3 );

  status = epos_task_set_priority( tid3, 2, &previous_priority );
  directive_failed( status, "epos_task_set_priority" );

  printk( "TA1 - epos_task_set_priority - set TA3's priority to 2" );


  printk( "TA1 - epos_task_suspend - suspend TA2" );
  status = epos_task_suspend( tid2 );
  directive_failed( status, "epos_task_suspend of TA2" );

  printk( "TA1 - epos_task_delete - delete TA2" );
  status = epos_task_delete( tid2 );
  directive_failed( status, "epos_task_delete of TA2" );

  printk( "TA1 - epos_task_wake_after - sleep for 5 seconds" );
  status = epos_task_wake_after( 5*TICKS_PER_SECOND );
  directive_failed( status, "epos_task_wake_after" );

  printk( "*** END OF TEST 2 ***" );
  epos_test_exit( 0 );
}
