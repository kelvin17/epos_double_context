/*
 *  COPYRIGHT (c) 1989-1999.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id$
 */

#include <stdio.h>
#include <rtems.h>
#include <rtems/error.h>

static epos_id taskId1;
static epos_id taskId2;
epos_interval ticksPerSecond;

#include "system.h"

static int
isSuspended (epos_id tid)
{
  epos_status_code sc;

  sc = epos_task_is_suspended (tid);
  if (sc == RTEMS_ALREADY_SUSPENDED)
    return 1;
  if (sc != RTEMS_SUCCESSFUL)
    printk ("epos_task_is_suspended: %s\n", epos_status_text (sc));
  return 0;
}

static void
subTask1 (epos_task_argument arg)
{
  epos_status_code sc;

  epos_task_wake_after (ticksPerSecond * 3);
  sc = epos_event_send (taskId2, 1);
  if (sc != RTEMS_SUCCESSFUL) {
    printk ("subTask1 - Can't send event (%d)\n", sc);
    epos_task_suspend (RTEMS_SELF);
  }
  epos_task_wake_after (ticksPerSecond * 3);
  printk ("subTask1 - Event sent\n");
  epos_task_suspend (RTEMS_SELF);
  printk ("subTask1 - Back to task 1\n");
  epos_task_wake_after (ticksPerSecond * 3);
  epos_task_suspend (RTEMS_SELF);
}

static void
subTask2 (epos_task_argument arg)
{
  epos_status_code sc;
  epos_event_set ev;

  epos_task_wake_after (ticksPerSecond * 1);
  sc = epos_event_receive (1, RTEMS_WAIT|RTEMS_EVENT_ANY, RTEMS_NO_TIMEOUT, &ev);
  if (sc != RTEMS_SUCCESSFUL) {
    printk ("subTask2 - Can't receive event (%d)\n", sc);
    epos_task_suspend (RTEMS_SELF);
  }
  printk ("subTask2 - Task 1 suspended? - should be 0: %d\n",
     isSuspended (taskId1));
  epos_task_wake_after (ticksPerSecond * 4);
  printk ("subTask2 - Task 1 suspended? - should be 1: %d\n",
     isSuspended (taskId1));
  epos_task_resume (taskId1);
  printk ("subTask2 - Task 1 suspended? - should be 0: %d\n",
     isSuspended (taskId1));
  epos_task_wake_after (ticksPerSecond * 4);
  printk ("subTask2 - Task 1 suspended? - should be 1: %d\n",
     isSuspended (taskId1));

  printk( "*** END OF TEST 26 ***" );
  epos_test_exit( 0 );
}

static void
createTask (char c, epos_id *tid)
{
  epos_status_code sc;

  sc = epos_task_create (epos_build_name('S','u','b',c),
    100,
    RTEMS_MINIMUM_STACK_SIZE * 4,
    RTEMS_PREEMPT|RTEMS_NO_TIMESLICE|RTEMS_NO_ASR|RTEMS_INTERRUPT_LEVEL(0),
    RTEMS_NO_FLOATING_POINT|RTEMS_LOCAL,
    tid);
  if (sc != RTEMS_SUCCESSFUL) {
    printk ("Can't create task (%d)\n", sc);
    epos_task_suspend (RTEMS_SELF);
  }
}

static void
startTask (epos_id tid, epos_task_entry entry_point)
{
  epos_status_code sc;

  sc = epos_task_start (tid, entry_point, 0);
  if (sc != RTEMS_SUCCESSFUL) {
    printk ("Can't start task (%d)\n", sc);
    epos_task_suspend (RTEMS_SELF);
  }
}

void
task1 (void)
{
  epos_clock_get (RTEMS_CLOCK_GET_TICKS_PER_SECOND, &ticksPerSecond);
  createTask ('1', &taskId1);
  createTask ('2', &taskId2);
  startTask (taskId1, subTask1);
  startTask (taskId2, subTask2);
  epos_task_suspend (RTEMS_SELF);
}
