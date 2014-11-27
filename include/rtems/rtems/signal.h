/**
 * @file signal.h
 *
 * @ingroup ClassicSignal
 *
 *  This include file contains all the constants and structures associated
 *  with the Signal Manager.   This manager provides capabilities required
 *  for asynchronous communication between tasks via signal sets.
 *
 *  Directives provided are:
 *
 *     + establish an asynchronous signal routine
 *     + send a signal set to a task
 */

/*  COPYRIGHT (c) 1989-2008.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: signal.h,v 1.18 2009/12/15 18:26:41 humph Exp $
 */

#ifndef _RTEMS_RTEMS_SIGNAL_H
#define _RTEMS_RTEMS_SIGNAL_H

/**
 *  @defgroup ClassicSignal Signals
 *
 *  @ingroup ClassicRTEMS
 *
 *  @brief RTEMS信号服务模块。
 *
 *  This encapsulates functionality which XXX
 */
/**@{*/

#include <rtems/rtems/asr.h>
#include <rtems/rtems/modes.h>
#include <rtems/score/object.h>
#include <rtems/rtems/status.h>
#include <rtems/rtems/types.h>

#ifdef __cplusplus
extern "C" {
#endif
/**
 *  @brief _Signal_Manager_initialization
 *
 *  This routine performs the initialization necessary for this manager.
 */
void _Signal_Manager_initialization( void );

/**
 *  @brief epos_signal_catch
 *
 *  This routine implements the epos_signal_catch directive.  This directive
 *  is used to establish asr_handler as the Asynchronous Signal Routine
 *  (RTEMS_ASR) for the calling task.  The asr_handler will execute with a
 *  mode of mode_set.
 */
epos_status_code epos_signal_catch(
  epos_asr_entry   asr_handler,
  epos_mode        mode_set
);

/**
 *  @brief epos_signal_send
 *
 *  This routine implements the epos_signal_send directive.  This directive
 *  sends the signal_set to the task specified by ID.
 */
epos_status_code epos_signal_send(
  epos_id         id,
  epos_signal_set signal_set
);

#if defined(RTEMS_MULTIPROCESSING)
#include <rtems/rtems/signalmp.h>
#endif

#ifdef __cplusplus
}
#endif

/**@}*/

#endif
/* end of include file */
