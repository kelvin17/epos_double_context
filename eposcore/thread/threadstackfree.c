/**
 * @file
 *
 * @ingroup ScoreThread
 *
 * @brief 核心抽象层线程管理器实现。(SuperCore Thread Handler implementation.)
 */

/*
 *  Thread Handler
 *
 *
 *  COPYRIGHT (c) 1989-2008.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: threadstackfree.c,v 1.7.2.1 2011/05/25 14:17:53 ralf Exp $
 */

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <rtems/system.h>
#include <rtems/config.h>
#include <rtems/score/apiext.h>
#include <rtems/score/context.h>
#include <rtems/score/interr.h>
#include <rtems/score/isr.h>
#include <rtems/score/object.h>
#include <rtems/score/priority.h>
#include <rtems/score/states.h>
#include <rtems/score/sysstate.h>
#include <rtems/score/thread.h>
#include <rtems/score/threadq.h>
#include <rtems/score/userext.h>
#include <rtems/score/wkspace.h>

/*
 *  _Thread_Stack_Free
 *
 *  Deallocate the Thread's stack.
 */

void _Thread_Stack_Free(
  Thread_Control *the_thread, bool is_new_stack
)
{
  #if defined(RTEMS_SCORE_THREAD_ENABLE_USER_PROVIDED_STACK_VIA_API)
    /*
     *  If the API provided the stack space, then don't free it.
     */
    if ( !the_thread->Start.core_allocated_stack )
      return;
  #endif

  void *stack = NULL;
  if (is_new_stack == true)
  	stack = the_thread->Start.Initial_stack.area;
  else 
  	stack = the_thread->Start.Initial_stack.old_area;
  
  /*
   * Call ONLY the CPU table stack free hook, or the
   * the RTEMS workspace free.  This is so the free
   * routine properly matches the allocation of the stack.
   */

  if ( Configuration.stack_free_hook )
    (*Configuration.stack_free_hook)( stack );
  else
    _Workspace_Free( stack );
}

