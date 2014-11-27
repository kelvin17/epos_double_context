/*
 *  Test for epos_semaphore_flush
 *
 *  $Id$
 */

#include <bsp.h>

epos_task Init (epos_task_argument argument);

#define CONFIGURE_EXTRA_TASK_STACKS RTEMS_MINIMUM_STACK_SIZE

#define CONFIGURE_RTEMS_INIT_TASKS_TABLE

//#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER
#define CONFIGURE_MAXIMUM_TASKS             2
#define CONFIGURE_MAXIMUM_SEMAPHORES        2

#define CONFIGURE_INIT

#include <rtems/confdefs.h>

#include <rtems/error.h>
#include <stdio.h>
#include <stdlib.h>

epos_interval ticksPerSecond;

epos_task
subtask (epos_task_argument arg)
{
	epos_status_code sc;
	epos_id sem = (epos_id)arg;

	for (;;) {
		epos_task_wake_after (ticksPerSecond * 2);

		sc = epos_semaphore_release (sem);
		if (sc != RTEMS_SUCCESSFUL)
			printk ("%d: Can't release semaphore: %s\n", __LINE__, epos_status_text (sc));
	}
}

void
startTask (epos_id arg)
{
	epos_id tid;
	epos_status_code sc;

	sc = epos_task_create (epos_build_name ('S', 'R', 'V', 'A'),
		100,
		RTEMS_MINIMUM_STACK_SIZE * 2,
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

epos_task Init (epos_task_argument ignored)
{
	int i;
	epos_id semrec, semnorec;
	epos_status_code sc;
	epos_interval then, now;

	printk( "*** SP29 - SIMPLE SEMAPHORE TEST ***" );
	printk( "This test only prints on errors." );

	sc = epos_clock_get (RTEMS_CLOCK_GET_TICKS_PER_SECOND, &ticksPerSecond);
	if (sc != RTEMS_SUCCESSFUL) {
		printk ("Can't get ticks per second: %s\n", epos_status_text (sc));
		exit (1);
	}
	sc = epos_semaphore_create (epos_build_name ('S', 'M', 'r', 'c'),
		1,
		RTEMS_PRIORITY|RTEMS_BINARY_SEMAPHORE|RTEMS_INHERIT_PRIORITY |RTEMS_NO_PRIORITY_CEILING|RTEMS_LOCAL,
		0,
		&semrec);
	if (sc != RTEMS_SUCCESSFUL) {
		printk ("%d: Can't create recursive-lock semaphore: %s\n", __LINE__, epos_status_text (sc));
		exit (1);
	}
	sc = epos_semaphore_create (epos_build_name ('S', 'M', 'n', 'c'),
		1,
		RTEMS_PRIORITY|RTEMS_SIMPLE_BINARY_SEMAPHORE|RTEMS_INHERIT_PRIORITY |RTEMS_NO_PRIORITY_CEILING|RTEMS_LOCAL,
		0,
		&semnorec);
	if (sc != RTEMS_SUCCESSFUL) {
		printk ("%d: Can't create non-recursive-lock semaphore: %s\n", __LINE__, epos_status_text (sc));
		exit (1);
	}

	sc = epos_semaphore_obtain (semrec, RTEMS_NO_WAIT, 0);
	if (sc != RTEMS_SUCCESSFUL) {
		printk ("%d: Can't obtain recursive-lock semaphore: %s\n", __LINE__, epos_status_text (sc));
	}
	sc = epos_semaphore_obtain (semrec, RTEMS_NO_WAIT, 0);
	if (sc != RTEMS_SUCCESSFUL) {
		printk ("%d: Can't reobtain recursive-lock semaphore: %s\n", __LINE__, epos_status_text (sc));
	}

	sc = epos_semaphore_obtain (semnorec, RTEMS_NO_WAIT, 0);
	if (sc != RTEMS_SUCCESSFUL) {
		printk ("%d: Can't obtain non-recursive-lock semaphore: %s\n", __LINE__, epos_status_text (sc));
	}
	sc = epos_semaphore_obtain (semnorec, RTEMS_NO_WAIT, 0);
	if (sc == RTEMS_SUCCESSFUL) {
		printk ("%d: Reobtain non-recursive-lock semaphore -- and should not have.\n", __LINE__);
	}

	sc = epos_semaphore_release (semnorec);
	if (sc != RTEMS_SUCCESSFUL) {
		printk ("%d: Can't release non-recursive-lock semaphore: %s\n", __LINE__, epos_status_text (sc));
	}
	sc = epos_semaphore_release (semnorec);
	if (sc != RTEMS_SUCCESSFUL) {
		printk ("%d: Can't rerelease non-recursive-lock semaphore: %s\n", __LINE__, epos_status_text (sc));
	}
	sc = epos_semaphore_obtain (semnorec, RTEMS_NO_WAIT, 0);
	if (sc != RTEMS_SUCCESSFUL) {
		printk ("%d: Can't obtain non-recursive-lock semaphore: %s\n", __LINE__, epos_status_text (sc));
	}
	sc = epos_semaphore_obtain (semnorec, RTEMS_NO_WAIT, 0);
	if (sc == RTEMS_SUCCESSFUL) {
		printk ("%d: Reobtain non-recursive-lock semaphore -- and should not have.\n", __LINE__);
	}
	else if (sc != RTEMS_UNSATISFIED) {
		printk ("%d: Reobtain non-recursive-lock semaphore failed, but error is %d (%s), not RTEMS_UNSATISFIED.\n", __LINE__, sc, epos_status_text (sc));
	}

	sc = epos_semaphore_release (semnorec);
	if (sc != RTEMS_SUCCESSFUL) {
		printk ("%d: Can't release non-recursive-lock semaphore: %s\n", __LINE__, epos_status_text (sc));
	}
	sc = epos_semaphore_release (semnorec);
	if (sc != RTEMS_SUCCESSFUL) {
		printk ("%d: Can't rerelease non-recursive-lock semaphore: %s\n", __LINE__, epos_status_text (sc));
	}
	sc = epos_semaphore_obtain (semnorec, RTEMS_NO_WAIT, 0);
	if (sc != RTEMS_SUCCESSFUL) {
		printk ("%d: Can't obtain non-recursive-lock semaphore: %s\n", __LINE__, epos_status_text (sc));
	}
        /*
         *  Since this task is holding this, this task will block and timeout.
         *  Then the timeout error will be returned.
         */
	epos_clock_get (RTEMS_CLOCK_GET_TICKS_SINCE_BOOT, &then);
	sc = epos_semaphore_obtain (semnorec, RTEMS_WAIT, 5);
	epos_clock_get (RTEMS_CLOCK_GET_TICKS_SINCE_BOOT, &now);
	if (sc == RTEMS_SUCCESSFUL) {
		printk ("%d: Reobtain non-recursive-lock semaphore -- and should not have.\n", __LINE__);
	}
	else if (sc != RTEMS_TIMEOUT) {
		printk ("%d: Reobtain non-recursive-lock semaphore failed, but error is %d (%s), not RTEMS_TIMEOUT.\n", __LINE__, sc, epos_status_text (sc));
	}
	if ((then - now) < 4)
		printk ("%d: Reobtain non-recursive-lock semaphore failed without timeout.\n", __LINE__);

	startTask (semnorec);
	epos_clock_get (RTEMS_CLOCK_GET_TICKS_SINCE_BOOT, &then);
	for (i = 0 ; i < 5 ; i++) {
		int diff;

		sc = epos_semaphore_obtain (semnorec, RTEMS_WAIT, RTEMS_NO_TIMEOUT);
		epos_clock_get (RTEMS_CLOCK_GET_TICKS_SINCE_BOOT, &now);
		diff = now - then;
		then = now;
		if (sc != RTEMS_SUCCESSFUL)
			printk ("%d: Failed to obtain non-recursive-lock semaphore: %s\n", __LINE__, epos_status_text (sc));
		else if (diff < (2 * ticksPerSecond))
			printk ("%d: Obtained obtain non-recursive-lock semaphore too quickly -- %d ticks not %d ticks\n", __LINE__, diff, (2 * ticksPerSecond) );
	}

	printk( "*** END OF TEST 29 ***" );
	exit (0);
}
