/** 
 *  @file  threadmp.inl
 *
 * @ingroup ScoreThreadMP
 *
 *  @brief The include file of the multiprocessing portion of the thread package.
 *
 *  This include file contains the bodies of all inlined routines
 *  for the multiprocessing part of thread package.
 */

/*
 *  COPYRIGHT (c) 1989-2008.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: threadmp.inl,v 1.19.4.1 2011/11/09 18:44:01 joel Exp $
 */

#ifdef _OCC_NO_INLINE_
#include <rtems/system.h>
#include <rtems/score/thread.h>
#include <rtems/score/threadmp.h>
#endif

#ifndef _RTEMS_SCORE_THREADMP_H
# error "Never use "threadmp.inl" directly; include "threadmp.h" instead."
#endif

#ifndef _OCC_NO_INLINE_
#include "mpci.h"
#endif

#ifndef _RTEMS_SCORE_THREADMP_INL
#define _RTEMS_SCORE_THREADMP_INL

/**
 *  @addtogroup ScoreThreadMP 
 *  @{
 */

/**
 *  This function returns true if the thread in question is the
 *  multiprocessing receive thread.
 *
 *  @note This is a macro to avoid needing a prototype for 
 *        _MPCI_Receive_server_tcb until it is used.
 */
#define _Thread_MP_Is_receive(_the_thread) \
  ((_the_thread) == _MPCI_Receive_server_tcb)

/**
 *  This routine frees a proxy control block to the
 *  inactive chain of free proxy control blocks.
 */

RTEMS_INLINE_ROUTINE void _Thread_MP_Free_proxy (
  Thread_Control *the_thread
)
{
  Thread_Proxy_control *the_proxy;

  the_proxy = (Thread_Proxy_control *) the_thread;

  _Chain_Extract( &the_proxy->Active );

  _Chain_Append( &_Thread_MP_Inactive_proxies, &the_thread->Object.Node );
}

/**@}*/

#endif
/* end of include file */
