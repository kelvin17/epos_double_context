/*  Task_3
 *
 *  This routine serves as a test task.  Makes sure a task can suspend
 *  itself.
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

epos_task Task_3(
  epos_task_argument argument
)
{
  epos_status_code status;

  printk( "TA3 - epos_task_suspend - suspend self" );
  status = epos_task_suspend( RTEMS_SELF );
  directive_failed( status, "epos_task_suspend of TA3" );

  while( FOREVER ) {
    printk( "TA3 - epos_task_wake_after - sleep 1 second" );
    status = epos_task_wake_after( 1*TICKS_PER_SECOND );
    directive_failed( status, "epos_task_wake_after of TA3" );
  }
}
