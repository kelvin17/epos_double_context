/*  Task_2
 *
 *  This routine serves as a test task.  It simply runs forever so
 *  another task can restart it.
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

epos_task Task_2( argument )
epos_task_argument argument;
{
  epos_status_code status;

  printk( "TA2 - is beginning to run" );
  if ( argument == Argument ) {
  printk( "TA2 - epos_task_wake_after - sleep 2 second-start" );
    status = epos_task_wake_after( 2 * TICKS_PER_SECOND );
    directive_failed( status, "epos_task_wake_after" );
  }

  while( FOREVER ) {
    printk( "TA2 - epos_task_wake_after - sleep 1/2 second-restart" );
    status = epos_task_wake_after( TICKS_PER_SECOND / 2 );
    directive_failed( status, "epos_task_wake_after" );
  }
}
