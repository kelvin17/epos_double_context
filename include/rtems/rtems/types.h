/**
 * @file
 *
 * @ingroup ClassicRTEMS
 *
 * @brief Types used by the Classic API.
 */

/*  COPYRIGHT (c) 1989-2009.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: types.h,v 1.33 2009/12/02 18:22:18 humph Exp $
 */

#ifndef _RTEMS_RTEMS_TYPES_H
#define _RTEMS_RTEMS_TYPES_H

/*
 *  RTEMS basic type definitions
 */

#include <stdint.h>
#include <rtems/score/heap.h>
#include <rtems/score/object.h>
#include <rtems/score/priority.h>
#include <rtems/score/tod.h>
#include <rtems/score/watchdog.h>
#include <rtems/rtems/modes.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup ClassicRTEMS
 *
 * @{
 */

#ifdef RTEMS_DEPRECATED_TYPES
/**
 * @brief Single precision float type.
 *
 * @deprecated Use @c float instead.
 */
typedef single_precision epos_single;

/**
 * @brief Double precision float type.
 *
 * @deprecated Use @c double instead.
 */
typedef double_precision epos_double;

/**
 * @brief RTEMS boolean type.
 *
 * @deprecated Use @c bool instead
 */
typedef boolean          epos_boolean;
#endif

/**
 * @brief Classic API object name Type.
 *
 * Contains the name of a Classic API object. It is an unsigned 32 bit integer
 * which can be treated as a numeric value or initialized using
 * epos_build_name() to contain four ASCII characters.
 */
typedef uint32_t         epos_name;

/**
 * @brief Used to manage and manipulate RTEMS object identifier.
 */
typedef Objects_Id       epos_id;

/**
 * @brief Invalid object identifier value.
 *
 * No object can have this identifier value.
 */
#define RTEMS_ID_NONE OBJECTS_ID_NONE

/**
 * @brief Public name for task context area.
 */
typedef Context_Control            epos_context;

#if (CPU_HARDWARE_FP == TRUE) || (CPU_SOFTWARE_FP == TRUE)
/**
 * @brief Public name for task floating point context area.
 */
typedef Context_Control_fp         epos_context_fp;
#endif

/**
 * @brief Defines the format of the interrupt stack frame as it appears to a
 * user ISR.
 *
 * This data structure may not be defined on all ports
 */

typedef CPU_Interrupt_frame        epos_interrupt_frame;

/**
 * @brief Information structure returned by the Heap Handler via the Region
 * Manager.
 */
typedef Heap_Information_block region_information_block;

/**
 * @brief Used to manage and manipulate intervals specified by clock ticks.
 */
typedef Watchdog_Interval epos_interval;

/**
 * @brief Represents the CPU usage per thread.
 *
 * When using nano seconds granularity timing, RTEMS may internally use a
 * variety of representations.
 */
#ifndef RTEMS_USE_TICKS_FOR_STATISTICS
 typedef struct timespec epos_thread_cpu_usage_t;
#else
  typedef uint32_t epos_thread_cpu_usage_t;
#endif

/**
 * @brief Data structure to manage and manipulate calendar time.
 */
typedef struct {
  /**
   * @brief Year, A.D.
   */
  uint32_t   year;
  /**
   * @brief Month, 1 .. 12.
   */
  uint32_t   month;
  /**
   * @brief Day, 1 .. 31.
   */
  uint32_t   day;
  /**
   * @brief Hour, 0 .. 23.
   */
  uint32_t   hour;
  /**
   * @brief Minute, 0 .. 59.
   */
  uint32_t   minute;
  /**
   * @brief Second, 0 .. 59.
   */
  uint32_t   second;
  /**
   * @brief Elapsed ticks between seconds.
   */
  uint32_t   ticks;
}   epos_time_of_day;

/**
 * @brief Task mode type.
 */
typedef Modes_Control epos_mode;

/** @} */

#ifdef __cplusplus
}
#endif

#endif
/* end of include file */
