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
 */

#include <tmacros.h>
#include <stdio.h>
/* functions */

void counter_asm( uint32_t * f);
void put(char* p);

epos_task Init(
  epos_task_argument argument
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

void directive_suc(int status);

/* configuration information */

//#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER

#define CONFIGURE_MAXIMUM_TASKS               4
#define CONFIGURE_MAXIMUM_REGIONS             4
#define CONFIGURE_TICKS_PER_TIMESLICE       100

#define CONFIGURE_RTEMS_INIT_TASKS_TABLE

#define CONFIGURE_EXTRA_TASK_STACKS         (1 * RTEMS_MINIMUM_STACK_SIZE)

#include <rtems/confdefs.h>

/* global variables */

TEST_EXTERN epos_id   Task_id[ 6 ];      /* array of task ids */
TEST_EXTERN epos_name Task_name[ 6 ];    /* array of task names */
TEST_EXTERN epos_id   Region_id[ 5 ];    /* array of region ids */
TEST_EXTERN epos_name Region_name[ 5 ];  /* array of region names */

TEST_EXTERN uint8_t   Area_1[4096] CPU_STRUCTURE_ALIGNMENT;
TEST_EXTERN uint8_t   Area_2[4096] CPU_STRUCTURE_ALIGNMENT;
TEST_EXTERN uint8_t   Area_3[4096] CPU_STRUCTURE_ALIGNMENT;
TEST_EXTERN uint8_t   Area_4[8192] CPU_STRUCTURE_ALIGNMENT;



#define BASE_PRIORITY ((RTEMS_MAXIMUM_PRIORITY / 2u) + 1u)

#define Put_address_from_area_1( _to_be_printed ) \
   printk( "0x%08x", \
     (unsigned long) ((uint8_t   *)(_to_be_printed) - Area_1 ) )

#define Put_address_from_area_2( _to_be_printed ) \
   printk( "0x%08x", \
     (unsigned long) ((uint8_t   *)(_to_be_printed) - Area_2 ) )

#define Put_address_from_area_3( _to_be_printed ) \
   printk( "0x%08x", \
     (unsigned long) ((uint8_t   *)(_to_be_printed) - Area_3 ) )

#define Put_address_from_area_4( _to_be_printed ) \
   printk( "0x%08x", \
     (unsigned long) ((uint8_t   *)(_to_be_printed) - Area_4 ) )

int bootcard(const char *cmdline);

/* end of include file */
