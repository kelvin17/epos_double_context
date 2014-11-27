/**
 *  @file  stack.h
 *
 *  @ingroup ScoreStack
 *
 *  @brief Header file of the thread stack handler.
 *
 *  This include file contains all information about the thread
 *  Stack Handler.  This Handler provides mechanisms which can be used to
 *  initialize and utilize stacks.
 */

/*
 *  COPYRIGHT (c) 1989-2006.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: stack.h,v 1.19 2009/11/28 05:58:54 ralf Exp $
 */

#ifndef _RTEMS_SCORE_STACK_H
#define _RTEMS_SCORE_STACK_H

/**
 *  @defgroup ScoreStack Stack Handler
 *
 *  @ingroup Score
 *
 *  @brief 核心抽象层之stack抽象管理模块
 *
 *  This handler encapsulates functionality which is used in the management
 *  of thread stacks.
 */
/**@{*/

#ifdef __cplusplus
extern "C" {
#endif

/**
 *  The following constant defines the minimum stack size which every
 *  thread must exceed.
 */
#define CPU_STACK_MINIMUM_SIZE 4096
#define STACK_MINIMUM_SIZE  CPU_STACK_MINIMUM_SIZE

/**
 *  The following defines the control block used to manage each stack.
 */
typedef struct {
  /** This is the stack size. */
  size_t      size;
  /** This is the low memory address of stack. */
  void       *area;
  /** This is the low memory address of old stack, when restarting a task, it's useful. */
  void       *old_area;
}   Stack_Control;

/**
 *  This variable contains the the minimum stack size;
 *
 *  @note It is instantiated and set by User Configuration via confdefs.h.
 */
extern uint32_t epos_minimum_stack_size;

#ifndef __RTEMS_APPLICATION__
#ifndef _OCC_NO_INLINE_
#include <rtems/score/stack.inl>
#endif

#ifdef _OCC_NO_INLINE_
RTEMS_INLINE_ROUTINE void _Stack_Initialize (
  Stack_Control *the_stack,
  void          *starting_address,
  size_t         size
);
RTEMS_INLINE_ROUTINE uint32_t _Stack_Minimum (void);
RTEMS_INLINE_ROUTINE bool _Stack_Is_enough (
  size_t size
);
RTEMS_INLINE_ROUTINE size_t _Stack_Ensure_minimum (
  size_t size
);
RTEMS_INLINE_ROUTINE uint32_t   _Stack_Adjust_size (
  size_t size
);


#endif

#endif

#ifdef __cplusplus
}
#endif

/**@}*/

#endif
/* end of include file */
