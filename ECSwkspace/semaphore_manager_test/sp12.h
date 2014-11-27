/*  system.h
 *
 *  This include file contains information that is included in every
 *  function in the test set.
 *
 *  COPYRIGHT (c) 1989-2008.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: system.h,v 1.20 2009/10/26 13:58:56 ralf Exp $
 */
#ifndef SP12_H_
#define SP12_H_

#include <tmacros.h>
#include <stdio.h>
#define PRIORITY_INHERIT_BASE_PRIORITY 5

/* functions */
void put(char* p);

epos_task Init(
  epos_task_argument argument
);

void Priority_test_driver(
  epos_task_priority priority_base
);

epos_task Priority_task(
  epos_task_argument its_index
);

epos_task Task_1(
  epos_task_argument argument
);

epos_task Task_2(
  epos_task_argument argument
);

epos_task Task_3(
  epos_task_argument argument
);

epos_task Task_4(
  epos_task_argument argument
);

epos_task Task5(
  epos_task_argument argument
);

/* configuration information */

#define CONFIGURE_IDLE_TASK_BODY Init  
#define CONFIGURE_IDLE_TASK_INITIALIZES_APPLICATION 


#define CONFIGURE_APPLICATION_DISABLE_FILESYSTEM
#define CONFIGURE_DISABLE_NEWLIB_REENTRANCY
#define CONFIGURE_TERMIOS_DISABLED

//#define CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER
#define CONFIGURE_APPLICATION_DOES_NOT_NEED_CLOCK_DRIVER

#define CONFIGURE_INIT_TASK_STACK_SIZE      (RTEMS_MINIMUM_STACK_SIZE * 2)
#define CONFIGURE_MAXIMUM_TASKS               8
#define CONFIGURE_MAXIMUM_SEMAPHORES         10
#define CONFIGURE_TICKS_PER_TIMESLICE       100

//#define CONFIGURE_RTEMS_INIT_TASKS_TABLE

#define CONFIGURE_EXTRA_TASK_STACKS         (6 * RTEMS_MINIMUM_STACK_SIZE)

#include <rtems/confdefs.h>

/* global variables */


TEST_EXTERN epos_id   Task_id[ 6 ];             /* array of task ids */
TEST_EXTERN epos_name Task_name[ 6 ];           /* array of task names */

TEST_EXTERN epos_id   Priority_task_id[ 6 ];    /* array of task ids */
TEST_EXTERN epos_name Priority_task_name[ 6 ];  /* array of task names */

TEST_EXTERN epos_task_priority Task_priority[ 6 ];

TEST_EXTERN epos_id   Semaphore_id[ 4 ];        /* array of semaphore ids */
TEST_EXTERN epos_name Semaphore_name[ 4 ];      /* array of semaphore names */

/* end of include file */
#endif