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
 *  COPYRIGHT (c) 1989-2009.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: threaddispatch.c,v 1.20.2.1 2011/05/25 14:17:52 ralf Exp $
 */

#if HAVE_CONFIG_H
#include "config.h"
#endif
#include <stdio.h>
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


#ifndef __RTEMS_USE_TICKS_FOR_STATISTICS__
  #include <rtems/score/timestamp.h>
#endif

/*PAGE
 *
 *  _Thread_Enable_dispatch
 *
 *  This kernel routine exits a context switch disable critical section.
 *  This is the NOT INLINED version.
 *
 *  Input parameters:  NONE
 *
 *  Output parameters:  NONE
 *
 *  INTERRUPT LATENCY:
 *    dispatch thread
 *    no dispatch thread
 */

#if ( (defined(CPU_INLINE_ENABLE_DISPATCH) &&  \
       (CPU_INLINE_ENABLE_DISPATCH == FALSE)) || \
      (__RTEMS_DO_NOT_INLINE_THREAD_ENABLE_DISPATCH__ == 1) )
void _Thread_Enable_dispatch( void )
{
  if ( --_Thread_Dispatch_disable_level )
    return;
  _Thread_Dispatch();
}
#endif

/*PAGE
 *
 *  _Thread_Dispatch
 *
 *  This kernel routine determines if a dispatch is needed, and if so
 *  dispatches to the heir thread.  Once the heir is running an attempt
 *  is made to dispatch any ASRs.
 *
 *  ALTERNATE ENTRY POINTS:
 *    void _Thread_Enable_dispatch();
 *
 *  Input parameters:  NONE
 *
 *  Output parameters:  NONE
 *
 *  INTERRUPT LATENCY:
 *    dispatch thread
 *    no dispatch thread
 */

void _Thread_Dispatch( void )
{
  Thread_Control   *executing;
  Thread_Control   *heir;
  ISR_Level         level;

  executing   = _Thread_Executing;
  _ISR_Disable( level );
  while ( _Context_Switch_necessary == true ) {
    heir = _Thread_Heir;
    _Thread_Dispatch_disable_level = 1;
    _Context_Switch_necessary = false;
    _Thread_Executing = heir;

    if ( heir->budget_algorithm == THREAD_CPU_BUDGET_ALGORITHM_RESET_TIMESLICE )
      heir->cpu_time_budget = _Thread_Ticks_per_timeslice;
    _ISR_Enable( level );

    #ifndef __RTEMS_USE_TICKS_FOR_STATISTICS__
      {
        Timestamp_Control uptime, ran;
        _TOD_Get_uptime( &uptime );
        _Timestamp_Subtract(
          &_Thread_Time_of_last_context_switch,
          &uptime,
          &ran
        );  
		//printf("threaddispatch.c\n");
        _Timestamp_Add_to( &executing->cpu_time_used, &ran );
        _Thread_Time_of_last_context_switch = uptime;
      } 
    #else 
      heir->cpu_time_used++;
    #endif

    /*
     * Switch libc's task specific data.
     */
    if ( _Thread_libc_reent ) {
      executing->libc_reent = *_Thread_libc_reent;
      *_Thread_libc_reent = heir->libc_reent;
    }

    _User_extensions_Thread_switch( executing, heir );

    /*
     *  If the CPU has hardware floating point, then we must address saving
     *  and restoring it as part of the context switch.
     *
     *  The second conditional compilation section selects the algorithm used
     *  to context switch between floating point tasks.  The deferred algorithm
     *  can be significantly better in a system with few floating point tasks
     *  because it reduces the total number of save and restore FP context
     *  operations.  However, this algorithm can not be used on all CPUs due
     *  to unpredictable use of FP registers by some compilers for integer
     *  operations.
     */

#if ( CPU_HARDWARE_FP == TRUE ) || ( CPU_SOFTWARE_FP == TRUE )
#if ( CPU_USE_DEFERRED_FP_SWITCH != TRUE )
    if ( executing->fp_context != NULL )
      _Context_Save_fp( &executing->fp_context );
#endif
#endif
	_Thread_Dispatch_disable_level = 0;
    _Context_Switch( &executing->Registers, heir->Registers );
	_Thread_Dispatch_disable_level = 1;
#if ( CPU_HARDWARE_FP == TRUE ) || ( CPU_SOFTWARE_FP == TRUE )
#if ( CPU_USE_DEFERRED_FP_SWITCH == TRUE )
    if ( (executing->fp_context != NULL) &&
         !_Thread_Is_allocated_fp( executing ) ) {
      if ( _Thread_Allocated_fp != NULL )
        _Context_Save_fp( &_Thread_Allocated_fp->fp_context );
      _Context_Restore_fp( &executing->fp_context );
      _Thread_Allocated_fp = executing;
    }
#else
    if ( executing->fp_context != NULL )
      _Context_Restore_fp( &executing->fp_context );
#endif
#endif

    executing = _Thread_Executing;

    _ISR_Disable( level );
  }

  _Thread_Dispatch_disable_level = 0;

  _ISR_Enable( level );

  if ( _Thread_Do_post_task_switch_extension ||
       executing->do_post_task_switch_extension ) {
    executing->do_post_task_switch_extension = false;
    _API_extensions_Run_postswitch();
  }

}

Context_Control* _Int_Thread_Dispatch(Context_Control* this_context)
{
	Thread_Control   *executing;
	Thread_Control   *heir;
	ISR_Level         level=0;
	
	_Thread_Dispatch_disable_level = 1;
	executing   = _Thread_Executing;
	_ISR_Disable( level );
	heir = _Thread_Heir;
	
	_User_extensions_Thread_switch( executing, heir );
	
	_Context_Switch_necessary = false;
	_Thread_Executing = heir;

//	_ISR_Enable(level);
	executing->Registers = this_context;
	this_context = heir->Registers;
	return this_context;		
}
