/*  Task_1_through_3
 *
 *  This routine serves as a test task.  It verifies the basic task
 *  switching capabilities of the executive.
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
 *  $Id: task1.c,v 1.12 2009/11/30 03:33:23 ralf Exp $
 */

epos_task_argument Restart_argument_2;
#include "sp01.h"
epos_task Task_1_through_3 (
  epos_task_argument argument
) 
{
	printk("in task0\n");
       epos_task_restart(Task_id[1],Restart_argument_2);
}