/*
 *  Test for epos_semaphore_flush
 *
 *  $Id$
 */

#include <bsp.h>

epos_task Init (epos_task_argument argument);

#define CONFIGURE_RTEMS_INIT_TASKS_TABLE

//#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER

#define CONFIGURE_MAXIMUM_TASKS               6
#define CONFIGURE_MAXIMUM_SEMAPHORES          1

//#define CONFIGURE_MICROSECONDS_PER_TICK       52429

#define CONFIGURE_INIT

#include <rtems/confdefs.h>

#include <rtems/error.h>
#include <stdio.h>
#include <stdlib.h>

#define NTASK 4

epos_id semaphore;
volatile int flags[NTASK];

epos_task
subtask (epos_task_argument arg)
{
  epos_status_code sc;

  for (;;) {
    flags[arg]++;
    sc = epos_semaphore_obtain (semaphore, RTEMS_WAIT, RTEMS_NO_TIMEOUT);
    if (sc == RTEMS_SUCCESSFUL)
      printk ("Obtained semaphore -- and should not have done so!");
    else if (sc != RTEMS_UNSATISFIED)
      printk ("Can't get semaphore: %s\n", epos_status_text (sc));
  }
}

void
starttask (int arg)
{
  epos_id tid;
  epos_status_code sc;
  epos_task_priority priority;

  epos_task_set_priority (RTEMS_SELF, RTEMS_CURRENT_PRIORITY, &priority);
  sc = epos_task_create (epos_build_name ('S', 'R', 'V', arg + 'A'),
    priority,
    RTEMS_MINIMUM_STACK_SIZE,
    RTEMS_PREEMPT|RTEMS_NO_TIMESLICE|RTEMS_NO_ASR|RTEMS_INTERRUPT_LEVEL(0),
    RTEMS_NO_FLOATING_POINT|RTEMS_LOCAL,
    &tid);
  if (sc != RTEMS_SUCCESSFUL) {
    printk ("Can't create task: %s\n", epos_status_text (sc));
    epos_task_suspend (RTEMS_SELF);
  }
  sc = epos_task_start (tid, subtask, arg);
  if (sc != RTEMS_SUCCESSFUL) {
  	printk ("Can't start task: %s\n", epos_status_text (sc));
    epos_task_suspend (RTEMS_SELF);
  }
}

epos_task
Init (epos_task_argument ignored)
{
  int pass, i;
  epos_status_code sc;

  printk( "\n\n*** TEST 27 ***" );
  printk("Testing semaphore flush"); 
  sc = epos_semaphore_create (
    epos_build_name ('S', 'E', 'M', 'F'),
    0,
    RTEMS_LOCAL|
      RTEMS_BINARY_SEMAPHORE|RTEMS_NO_INHERIT_PRIORITY|
      RTEMS_NO_PRIORITY_CEILING|RTEMS_FIFO,
    0,
    &semaphore);
  if (sc != RTEMS_SUCCESSFUL) {
    printk ("Can't flush semaphore: %s\n", epos_status_text (sc));
    exit (1);
  }
  for (i = 0 ; i < NTASK ; i++)
    starttask (i);
  for (pass = 1 ; pass < 10 ; pass++) {
    epos_task_wake_after (1);
    for (i = 0 ; i < NTASK ; i++) {
      if (flags[i] != pass)
        printk ("flags[%d] = %d -- expected %d\n", i, flags[i], pass);
    }
    sc = epos_semaphore_flush (semaphore);
    if (sc != RTEMS_SUCCESSFUL) {
      printk ("Can't flush semaphore: %s\n", epos_status_text (sc));
      exit (1);
    }
  }
  printk ("Flushed all waiting tasks\n", NTASK );
  printk( "*** END OF TEST 27 ***" );

  exit (1);
}
