/*  Task_1
 *
 *  This routine serves as a test task.  It verifies the semaphore manager.
 *
 *  Input parameters:
 *    argument - task argument
 *
 *  Output parameters:  NONE
 *
 *  COPYRIGHT (c) 1989-2009.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: task1.c,v 1.11 2009/10/26 11:29:24 ralf Exp $
 */

#include "sp12.h"

epos_task Task_1(
  epos_task_argument argument
)
{
  epos_id          smid;
  epos_status_code status;

 
  put( "TA1 - epos_semaphore_obtain - wait forever on SM1" );
  status = epos_semaphore_obtain(
    Semaphore_id[ 1 ],
    RTEMS_DEFAULT_OPTIONS,
    RTEMS_NO_TIMEOUT
  );
  //directive_failed( status, "epos_semaphore_obtain of SM1" );
  printk("epos_semaphore_obtain of SM1,status=%d\n",status);
  put( "TA1 - got SM1" );
  epos_task_wake_after(10);
  

}
