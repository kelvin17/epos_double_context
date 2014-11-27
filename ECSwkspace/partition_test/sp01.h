#ifndef __SP01_H__
#define __SP01_H__

#include "tmacros.h"
//#include <stdio.h>


/* functions */
void counter_asm( uint32_t * f);
//void init_counter();

void directive_suc(int status);
void put(char* p);
  
epos_task Init(
  epos_task_argument argument
);

epos_task Task_1(
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
#define CONFIGURE_APPLICATION_DOES_NOT_NEED_CLOCK_DRIVER

#define CONFIGURE_MAXIMUM_PARTITIONS          5
#define CONFIGURE_TICKS_PER_TIMESLICE       100

//#define CONFIGURE_RTEMS_INIT_TASKS_TABLE

#define CONFIGURE_EXTRA_TASK_STACKS         (4 * RTEMS_MINIMUM_STACK_SIZE)
#define CONFIGURE_MAXIMUM_TASKS             4

#include <rtems/confdefs.h>

/* global variables */

TEST_EXTERN epos_id   Task_id[ 4 ];         /* array of task ids */
TEST_EXTERN epos_name Task_name[ 4 ];       /* array of task names */

TEST_EXTERN epos_id   Partition_id[ 4 ];   /* array of partition ids */
TEST_EXTERN epos_name Partition_name[ 4 ]; /* array of partition names */

TEST_EXTERN uint8_t   Area_1[4096] CPU_STRUCTURE_ALIGNMENT;
TEST_EXTERN uint8_t   Area_2[274] CPU_STRUCTURE_ALIGNMENT;

#define Put_address_from_area_1( _to_be_printed ) \
   printk( "Offset of the start:0x%08x", \
     (unsigned long)((uint8_t   *)(_to_be_printed) - Area_1 ) )

#define Put_address_from_area_2( _to_be_printed ) \
   printk( "Offset of the start:0x%08x", \
     (unsigned long)((uint8_t   *)(_to_be_printed) - Area_2 ) )


int bootcard(const char *cmdline);
//TEST_EXTERN  epos_time_of_day timeOS;

#endif/* end of include file */
