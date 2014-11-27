/*  Task_2
 *
 *  This routine serves as a test task.  It simply obtains semaphores
 *  1 and 2, the later when it is a high priority task.
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
 *  $Id: task2.c,v 1.7 2003/09/04 18:53:48 joel Exp $
 */


#include "sp12.h"

epos_task Task_2(
  epos_task_argument argument
)
{
 epos_id          smid;
  epos_status_code status;
  put( "TA2 - epos_semaphore_obtain - wait forever on SM1" );
  status = epos_semaphore_obtain(
    Semaphore_id[ 1 ],
    RTEMS_NO_WAIT,
    RTEMS_NO_TIMEOUT
    
  );
  //directive_failed( status, "epos_semaphore_obtain of SM1" );RTEMS_DEFAULT_OPTIONS
  printk("epos_semaphore_obtain of SM1,status=%d\n",status);
  put( "TA2 - NOT get SM1" );
  
  printk("****end of test samaphore manager***\n");
}
