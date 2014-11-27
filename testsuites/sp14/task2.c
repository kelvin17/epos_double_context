/*  Task_2
 *
 *  This routine serves as a test task.  It verifies that one task can
 *  send signals to another task ( invoking the other task's RTEMS_ASR ).
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

epos_task Task_2(
  epos_task_argument argument
)
{
  epos_status_code status;

  printk( "TA2 - epos_signal_send - RTEMS_SIGNAL_17 to TA1" );
  status = epos_signal_send( Task_id[ 1 ], RTEMS_SIGNAL_17 );
  directive_failed( status, "epos_signal_send" );

  printk( "TA2 - epos_task_wake_after - yield processor" );
  status = epos_task_wake_after( RTEMS_YIELD_PROCESSOR );
  directive_failed( status, "epos_task_wake_after" );

  printk("TA2 - epos_signal_send - RTEMS_SIGNAL_18 and RTEMS_SIGNAL_19 to TA1");
  status = epos_signal_send( Task_id[ 1 ], RTEMS_SIGNAL_18 | RTEMS_SIGNAL_19 );
  directive_failed( status, "epos_signal_send" );

  printk( "TA2 - epos_task_wake_after - yield processor" );
  status = epos_task_wake_after( RTEMS_YIELD_PROCESSOR );
  directive_failed( status, "epos_task_wake_after" );

  printk( "*** END OF TEST 14 ***" );
  epos_test_exit( 0 );
}
