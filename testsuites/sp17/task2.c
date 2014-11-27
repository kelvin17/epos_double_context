/*  Task_2
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

epos_task Task_2(
  epos_task_argument argument
)
{
  epos_status_code status;

  Task_2_preempted = FALSE;

  printk( "TA2 - Suspending self" );
  status = epos_task_suspend( RTEMS_SELF );
  directive_failed( status, "epos_task_suspend of TA2" );

  printk( "TA2 - signal_return preempted correctly" );

  Task_2_preempted = TRUE;

  status = epos_task_suspend( RTEMS_SELF );
  directive_failed( status, "epos_task_suspend of TA2" );
}
