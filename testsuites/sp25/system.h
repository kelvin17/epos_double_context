/*  system.h
 *
 *  This include file contains information that is included in every
 *  function in the test set.
 *
 *  COPYRIGHT (c) 1989-1999.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id$
 */

#include <tmacros.h>

/* functions */

epos_task Init(
  epos_task_argument argument
);

epos_task Task_1(
  epos_task_argument argument
);

/* configuration information */

//#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER

#define CONFIGURE_MAXIMUM_TASKS               2
#define CONFIGURE_MAXIMUM_REGIONS             1

#define CONFIGURE_RTEMS_INIT_TASKS_TABLE

#include <rtems/confdefs.h>

/* global variables */

TEST_EXTERN epos_id   Task_id[ 6 ];      /* array of task ids */
TEST_EXTERN epos_name Task_name[ 6 ];    /* array of task names */
TEST_EXTERN epos_id   Region_id[ 2 ];    /* array of region ids */
TEST_EXTERN epos_name Region_name[ 2 ];  /* array of region names */

TEST_EXTERN uint8_t   Area_1[64000]  CPU_STRUCTURE_ALIGNMENT;

#define BASE_PRIORITY 140

#define Put_address_from_area_1( _to_be_printed ) \
   printk( "0x%08lx", \
     (unsigned long) ((uint8_t   *)(_to_be_printed) - Area_1 ) )

/* end of include file */
