/*  system.h
 *
 *  This include file contains information that is included in every
 *  function in the test set.
 *
 *  COPYRIGHT (c) 1989-2007.
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

/* configuration information */

//#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER

#define CONFIGURE_RTEMS_INIT_TASKS_TABLE

//#define CONFIGURE_MICROSECONDS_PER_TICK   250

#define CONFIGURE_MAXIMUM_TASKS             2
#define CONFIGURE_MAXIMUM_TIMERS            1
#define CONFIGURE_MAXIMUM_SEMAPHORES        1

#include <rtems/confdefs.h>

/* end of include file */
