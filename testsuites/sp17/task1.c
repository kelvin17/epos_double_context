/*  Task_1
 *
 *  This task initializes the signal catcher, sends the first signal
 *  if running on the first node, and loops while waiting for signals.
 *
 *  NOTE: The signal catcher is not reentrant and hence RTEMS_NO_ASR must
 *        be a part of its execution mode.
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
  epos_status_code status;

  printk( "TA1 - epos_signal_catch: initializing signal catcher" );
  status = epos_signal_catch( Process_asr, RTEMS_NO_ASR | RTEMS_NO_PREEMPT );
  directive_failed( status, "epos_signal_catch" );

  printk( "TA1 - Sending signal to self" );
  status = epos_signal_send( Task_id[ 1 ], RTEMS_SIGNAL_16 );
  directive_failed( status, "epos_signal_send" );

  if ( Task_2_preempted == TRUE )
    printk( "TA1 - TA2 correctly preempted me" );

  printk("TA1 - Got Back!!!");

  printk( "*** END OF TEST 17 ***" );
  epos_test_exit( 0 );
}
