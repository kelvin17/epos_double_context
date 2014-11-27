#ifndef __SP01_H__
#define __SP01_H__

#include <tmacros.h>
#include <stdio.h>
#include <stdlib.h>
#include <rtems.h>
/* functions */
void counter_asm( uint32_t * f);
//void init_counter();


  
epos_task Init(
  epos_task_argument argument
);

epos_task Task_1_through_3 (
  epos_task_argument argument
);

epos_task Task_0 (
  epos_task_argument argument
);
 
/* configuration information */
 
//#define CONFIGURE_IDLE_TASK_BODY Init  
//#define CONFIGURE_IDLE_TASK_INITIALIZES_APPLICATION 
#define CONFIGURE_RTEMS_INIT_TASKS_TABLE
#define CONFIGURE_APPLICATION_DISABLE_FILESYSTEM
#define CONFIGURE_DISABLE_NEWLIB_REENTRANCY
//#define CONFIGURE_TERMIOS_DISABLED

//#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER
//#define CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER
#define CONFIGURE_MICROSECONDS_PER_TICK 1000000

#define CONFIGURE_APPLICATION_DOES_NOT_NEED_CLOCK_DRIVER

#define CONFIGURE_MAXIMUM_TASKS               4
//#define CONFIGURE_TICKS_PER_TIMESLICE       100

//#define CONFIGURE_RTEMS_INIT_TASKS_TABLE

#define CONFIGURE_EXTRA_TASK_STACKS         (4 * RTEMS_MINIMUM_STACK_SIZE)
#define CONFIGURE_MAXIMUM_TASKS             4

#define CONFIGURE_APPLICATION_DISABLE_FILESYSTEM
#define CONFIGURE_TERMIOS_DISABLED


//#define CONFIGURE_MALLOC_BSP_SUPPORTS_SBRK// added by zwj

//#define CONFIGURE_MALLOC_STATISTICS  // added by zwj

#include <rtems/confdefs.h>

/* global variables */
TEST_EXTERN epos_task_argument Argument;
TEST_EXTERN epos_id   Task_id[ 4 ];         /* array of task ids */
TEST_EXTERN epos_name Task_name[ 4 ];       /* array of task names */
int bootcard(const char *cmdline);
//TEST_EXTERN  epos_time_of_day timeOS;
#endif   /* end of include file */
