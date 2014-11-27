/*
 * sp13.h
 *
 *  Created on: 2012-11-21
 *      Author: Administrator
 */

#ifndef SP13_H_
#define SP13_H_
/*  system.h
 * 
 *  This include file contains information that is included in every
 *  function in the test set.
 *
 *  COPYRIGHT (c) 1989-2009.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: system.h,v 1.21 2009/05/10 14:39:46 joel Exp $
 */

#include <tmacros.h>

/* functions */
void put(char* p);

epos_task Init(
  epos_task_argument argument
);

void Fill_buffer(
  char  source[],
  long *buffer
);

void Put_buffer(
  void *buffer
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

/* configuration information */
#define CONFIGURE_IDLE_TASK_BODY Init  
#define CONFIGURE_IDLE_TASK_INITIALIZES_APPLICATION 


#define CONFIGURE_APPLICATION_DISABLE_FILESYSTEM
#define CONFIGURE_DISABLE_NEWLIB_REENTRANCY
#define CONFIGURE_TERMIOS_DISABLED

//#define CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER
#define CONFIGURE_APPLICATION_DOES_NOT_NEED_CLOCK_DRIVER
//#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER 


#define CONFIGURE_MAXIMUM_TASKS               4
#define CONFIGURE_MAXIMUM_MESSAGE_QUEUES     10
#define CONFIGURE_TICKS_PER_TIMESLICE       100

//#define CONFIGURE_RTEMS_INIT_TASKS_TABLE

#define CONFIGURE_MESSAGE_BUFFER_MEMORY     (32 * 1024)
#define CONFIGURE_EXTRA_TASK_STACKS         (3 * RTEMS_MINIMUM_STACK_SIZE)

#include <rtems/confdefs.h>

/* global variables */

TEST_EXTERN epos_id   Task_id[ 4 ];         /* array of task ids */
TEST_EXTERN epos_name Task_name[ 4 ];       /* array of task names */

TEST_EXTERN epos_id   Queue_id[ 4 ];        /* array of queue ids */
TEST_EXTERN epos_name Queue_name[ 4 ];      /* array of queue names */

/* test configuration */

#define MESSAGE_SIZE (sizeof(long) * 4)  /* must be multiple of sizeof(long) */

/* end of include file */



#endif /* SP13_H_ */
