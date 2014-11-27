/*  Task_3
 *
 *  This routine serves as a test task.  It simply sleeps for 5 seconds
 *  and then deletes itself.
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

  printk( "TA3 - epos_task_wake_after - sleep 5 seconds" );
  status = epos_task_wake_after( 5*TICKS_PER_SECOND );
  directive_failed( status, "epos_task_wake_after in TA3" );

  printk( "TA3 - epos_task_delete - delete self" );
  status = epos_task_delete( RTEMS_SELF );
  directive_failed( status, "epos_task_delete of TA3" );
}
