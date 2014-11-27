/**
 * @file
 *
 * @ingroup ScoreThread
 *
 * @brief 核心抽象层线程管理器实现: _Thread_Load_environment。(SuperCore Thread Handler implementation.)
 */

/*
 *  Thread Handler
 *
 *
 *  COPYRIGHT (c) 1989-1999.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: threadloadenv.c,v 1.7.4.1 2011/05/25 14:17:52 ralf Exp $
 */

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <rtems/system.h>
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

/*PAGE
 *
 *  _Thread_Load_environment
 *
 *  Load starting environment for another thread from its start area in the
 *  thread.  Only called from t_restart and t_start.
 *
 *  Input parameters:
 *    the_thread - thread control block pointer
 *
 *  Output parameters:  NONE
 */
void _CPU_Context_Initialize(
  Context_Control  **the_context,
  uint32_t		   *stack_base,
  uint32_t			size,
  uint32_t			new_level,
  void			   *entry_point,
  bool				is_fp
);

bool _Thread_Load_environment(
  Thread_Control *the_thread, bool is_restart
)
{
  bool is_fp;

if (is_restart == true)
{
  size_t        actual_stack_size = 0;
  void          *stack = NULL;
  void 			*stack_area = NULL;
  size_t 		stack_size = the_thread->Start.Initial_stack.size;

  /*
   *  ReAllocate and ReInitialize the stack for this thread.
   */
  #if !defined(RTEMS_SCORE_THREAD_ENABLE_USER_PROVIDED_STACK_VIA_API)
    actual_stack_size = _Thread_Stack_Allocate( the_thread, stack_size );
    if ( !actual_stack_size || actual_stack_size < stack_size )
      return false;                     /* stack allocation failed */

    stack = the_thread->Start.stack;
  #else
    if ( !stack_area ) {
      actual_stack_size = _Thread_Stack_Allocate( the_thread, stack_size );
      if ( !actual_stack_size || actual_stack_size < stack_size )
        return false;                     /* stack allocation failed */

      stack = the_thread->Start.stack;
      the_thread->Start.core_allocated_stack = true;
    } else {
      stack = stack_area;
      actual_stack_size = stack_size;
      the_thread->Start.core_allocated_stack = false;
    }
  #endif

  _Stack_Initialize(
     &the_thread->Start.Initial_stack,
     stack,
     actual_stack_size
  );
}


#if ( CPU_HARDWARE_FP == TRUE ) || ( CPU_SOFTWARE_FP == TRUE )
  if ( the_thread->Start.fp_context ) {
    the_thread->fp_context = the_thread->Start.fp_context;
    _Context_Initialize_fp( &the_thread->fp_context );
    is_fp = true;
  } else
#endif
    is_fp = false;
	
  the_thread->do_post_task_switch_extension = false;
  the_thread->is_preemptible   = the_thread->Start.is_preemptible;
  the_thread->budget_algorithm = the_thread->Start.budget_algorithm;
  the_thread->budget_callout   = the_thread->Start.budget_callout;
  _CPU_Context_Initialize(
    &the_thread->Registers,
    the_thread->Start.Initial_stack.area,
    the_thread->Start.Initial_stack.size,
    the_thread->Start.isr_level,
    _Thread_Handler,
    is_fp
  );

  return true;
}

