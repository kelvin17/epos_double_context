/**
 * @file
 *
 * @ingroup ScoreRWLock
 *
 * @brief 核心抽象层读写锁管理器实现。(SuperCore RWLock Handler implementation.)
 */

/*
 *  Thread Queue Handler
 *
 *
 *  COPYRIGHT (c) 1989-2007.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: corerwlocktimeout.c,v 1.2 2007/10/26 20:19:02 joel Exp $
 */

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <rtems/system.h>
#include <rtems/score/corerwlock.h>

/*
 *  _CORE_RWLock_Timeout
 *
 *  This routine processes a thread which timeouts while waiting on
 *  a thread queue. It is called by the watchdog handler.
 *
 *  Input parameters:
 *    id - thread id
 *
 *  Output parameters: NONE
 */

void _CORE_RWLock_Timeout(
  Objects_Id  id,
  void       *ignored
)
{
  Thread_Control       *the_thread;
  Objects_Locations     location;

  the_thread = _Thread_Get( id, &location );
  switch ( location ) {
    case OBJECTS_ERROR:
      break;
    case OBJECTS_LOCAL:
      _Thread_queue_Process_timeout( the_thread );
      _Thread_Unnest_dispatch();
      break;
  }
}

