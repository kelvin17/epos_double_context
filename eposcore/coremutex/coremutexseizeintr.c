/**
 * @file
 *
 * @ingroup ScoreMutex
 *
 * @brief 核心抽象层互斥锁管理器实现。(SuperCore Mutex Handler implementation.)
 */

/*
 *  Mutex Handler -- Seize interrupt disable version
 *
 *  COPYRIGHT (c) 1989-2007.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: coremutexseizeintr.c,v 1.2 2008/01/09 21:11:04 joel Exp $
 */

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <rtems/system.h>
#include <rtems/score/isr.h>
#include <rtems/score/coremutex.h>
#include <rtems/score/states.h>
#include <rtems/score/thread.h>
#include <rtems/score/threadq.h>

#if defined(__RTEMS_DO_NOT_INLINE_CORE_MUTEX_SEIZE__)
int _CORE_mutex_Seize_interrupt_trylock(
  CORE_mutex_Control  *the_mutex,
  ISR_Level           *level_p
)
{
  return _CORE_mutex_Seize_interrupt_trylock_body( the_mutex, level_p );
}
#endif
