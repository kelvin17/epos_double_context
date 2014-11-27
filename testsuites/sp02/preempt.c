/*  Preempt_task
 *
 *  This routine serves as a test task.  It verifies the task manager.
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

epos_task Preempt_task(
  epos_task_argument argument
)
{
  epos_status_code status;

  printk( "PREEMPT - epos_task_delete - deleting self" );
  status = epos_task_delete( RTEMS_SELF );
  directive_failed( status, "epos_task_delete of RTEMS_PREEMPT" );
}
