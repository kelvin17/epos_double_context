/*  Task_1
 *
 *  This task serves as a test task.  Verifies that a task can restart
 *  both itself and other tasks.
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
  epos_task_argument   argument
)
{
  uint32_t    pass;
  epos_status_code status;

  printk( "TA1 - is beginning to run" );

  pass = argument + 1;
  printk( "TA1 -pass:%d\n",pass);
  
  if ( pass == 5 ) {
    printk( "*** END OF TEST 6 ***" );
    epos_test_exit( 0 );
  }

  if ( pass == 1 ) {
    printk( "TA1 - restarting TA3" );
    status = epos_task_restart( Task_id[ 3 ], Restart_argument );
    directive_failed( status, "epos_task_restart of TA3" );

    status = epos_task_wake_after( 2 * TICKS_PER_SECOND );
    directive_failed( status, "epos_task_wake_after" );
  }

  status = epos_task_wake_after( TICKS_PER_SECOND );
  directive_failed( status, "epos_task_wake_after" );

  printk( "TA1 - epos_task_restart - restarting TA2" );
  status = epos_task_restart( Task_id[ 2 ], Restart_argument );
  directive_failed( status, "epos_task_restart TA2" );

  status = epos_task_wake_after( 1*TICKS_PER_SECOND );
  directive_failed( status, "epos_task_wake_after" );

  printk( "TA1 - epos_task_restart - restarting self" );
  status = epos_task_restart( Task_id[ 1 ], pass );
  directive_failed( status, "epos_task_restart of RTEMS_SELF" );
}
