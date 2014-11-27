/*  Task_1
 *
 *  This routine serves as a test task.  It verifies that tasks can
 *  be suspended and resumed.
 *
 *  Input parameters:
 *    argument - task argument
 *
 *  Output parameters:  NONE
 *
 *  NOTE: The epos_task_suspend() directives fail on the first iteration.
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
  epos_id          tid2;
  epos_id          tid3;
  uint32_t    pass;
  epos_status_code status;

  status = epos_task_ident( Task_name[ 2 ], 1, &tid2 );
  directive_failed( status, "epos_task_ident of TA2" );

  status = epos_task_ident( Task_name[ 3 ], 1, &tid3 );
  directive_failed( status, "epos_task_ident of TA3" );

  for ( pass=1 ; pass <= 3 ; pass++ ) {

    printk( "TA1 - epos_task_wake_after - sleep 5 seconds" );
    status = epos_task_wake_after( 5*TICKS_PER_SECOND );
    directive_failed( status, "epos_task_wake_after of TA1" );

    printk( "TA1 - epos_task_suspend - suspend TA3" );
    status = epos_task_suspend( tid3 );
    if ( pass == 1 ) {
      fatal_directive_status(
         status,
         RTEMS_ALREADY_SUSPENDED,
         "epos_task_suspend of TA3"
      );
    } else {
      directive_failed( status, "epos_task_suspend of TA3" );
    }

    printk( "TA1 - epos_task_resume - resume TA2" );
    status = epos_task_resume( tid2 );
    directive_failed( status, "epos_task_resume of TA2" );

    printk( "TA1 - epos_task_wake_after - sleep 5 seconds" );
    status = epos_task_wake_after( 5*TICKS_PER_SECOND );
    directive_failed( status, "epos_task_wake_after" );

    printk( "TA1 - epos_task_suspend - suspend TA2" );
    status = epos_task_suspend( tid2 );
    directive_failed( status, "epos_task_suspend of TA2" );

    printk( "TA1 - epos_task_resume - resume TA3" );
    status = epos_task_resume( tid3 );
    directive_failed( status, "epos_task_resume" );
  }

  printk( "*** END OF TEST 5 ***" );
  epos_test_exit( 0 );
}
