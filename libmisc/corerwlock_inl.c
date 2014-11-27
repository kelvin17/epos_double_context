/** 
 *  @file  corerwlock.inl
 *
 *  @ingroup ScoreRWLock
 *
 *  @brief The include file of the RWLock Handler.
 *
 *  This include file contains all of the inlined routines associated
 *  with the SuperCore RWLock.
 */

/*
 *  COPYRIGHT (c) 1989-2008.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: corerwlock.inl,v 1.3 2008/08/19 08:32:59 ralf Exp $
 */

#ifdef _OCC_NO_INLINE_
#include <rtems/system.h>
#include <rtems/score/corerwlock.h>
#endif

#ifndef _RTEMS_SCORE_CORERWLOCK_H
# error "Never use "corerwlock.inl" directly; include "corerwlock.h" instead."
#endif

#ifndef _RTEMS_SCORE_CORERWLOCK_INL
#define _RTEMS_SCORE_CORERWLOCK_INL

/**
 *  @addtogroup ScoreRWLock 
 *  @{
 */

#include <rtems/score/thread.h>
#include <rtems/score/threadq.h>


/**@}*/

#endif
/* end of include file */
