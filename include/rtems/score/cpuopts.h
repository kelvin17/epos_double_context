/**
 * @file cpuopts.h
 *
 * @ingroup EposConfig
 *
 *  @brief Header file of Epos config??.
 */

/* target cpu dependent options file */
/* automatically generated -- DO NOT EDIT!! */
#ifndef _RTEMS_SCORE_CPUOPTS_H
#define _RTEMS_SCORE_CPUOPTS_H

/* if RTEMS_DEBUG is enabled */
/* #undef RTEMS_DEBUG */

/* if using newlib */
#define RTEMS_NEWLIB 1
//#undef RTEMS_NEWLIB

/* if posix api is supported */
/* #undef RTEMS_POSIX_API */

/* if networking is enabled */
/* #undef RTEMS_NETWORKING */

/* RTEMS version string */
#define RTEMS_VERSION "4.10.2"
#define inline 
/* disable nanosecond granularity for statistics */
/* #undef __RTEMS_USE_TICKS_FOR_STATISTICS__ */
//#define __RTEMS_USE_TICKS_FOR_STATISTICS__
/* disable nanosecond granularity for cpu usage statistics */
/* #undef __RTEMS_USE_TICKS_CPU_USAGE_STATISTICS__ */

/* disable nanosecond granularity for period statistics */
/* #undef __RTEMS_USE_TICKS_RATE_MONOTONIC_STATISTICS__ */

/* disable inlining _Thread_Enable_dispatch */
/* #undef __RTEMS_DO_NOT_INLINE_THREAD_ENABLE_DISPATCH__ */

/* disable inlining _Thread_Enable_dispatch */
/* #undef __RTEMS_DO_NOT_INLINE_CORE_MUTEX_SEIZE__ */

/* disable inlining _Thread_queue_Enqueue_priority */
/* #undef __RTEMS_DO_NOT_UNROLL_THREADQ_ENQUEUE_PRIORITY__ */

/* disable strict order mutex */
/* #undef __RTEMS_STRICT_ORDER_MUTEX__ */


/* major version portion of an RTEMS release */
#define __RTEMS_MAJOR__ 4

/* minor version portion of an RTEMS release */
#define __RTEMS_MINOR__ 10

/* revision version portion of an RTEMS release */
#define __RTEMS_REVISION__ 2

#endif /* _RTEMS_SCORE_CPUOPTS_H */
