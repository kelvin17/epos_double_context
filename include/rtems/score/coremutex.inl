/** 
 *  @file  coremutex.inl
 *
 *  @ingroup ScoreMutex
 *
 *  @brief The include file of the Mutex Handler.
 *
 *  This include file contains all of the inlined routines associated
 *  with the CORE mutexes.
 */

/*
 *  COPYRIGHT (c) 1989-2009.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: coremutex.inl,v 1.28 2009/11/09 14:52:28 joel Exp $
 */

#ifdef _OCC_NO_INLINE_
#include <rtems/system.h>
#include <rtems/score/coremutex.h>
#endif

#ifndef _RTEMS_SCORE_COREMUTEX_H
# error "Never use "coremutex.inl" directly; include "coremutex.h" instead."
#endif

#ifndef _RTEMS_SCORE_COREMUTEX_INL
#define _RTEMS_SCORE_COREMUTEX_INL

/**
 *  @addtogroup ScoreMutex 
 *  @{
 */

/**
 *  @brief Seize Mutex with Quick Success Path
 *
 *  This routine attempts to receive a unit from the_mutex.
 *  If a unit is available or if the wait flag is false, then the routine
 *  returns.  Otherwise, the calling task is blocked until a unit becomes
 *  available.
 *
 *  @param[in] the_mutex is the mutex to attempt to lock
 *  @param[in] level_p is the interrupt level holder
 *
 *  @return This routine returns 0 if "trylock" can resolve whether or not
 *  the mutex is immediately obtained or there was an error attempting to
 *  get it.  It returns 1 to indicate that the caller cannot obtain
 *  the mutex and will have to block to do so.
 *
 *  @note  For performance reasons, this routine is implemented as
 *         a macro that uses two support routines.
 */

RTEMS_INLINE_ROUTINE int _CORE_mutex_Seize_interrupt_trylock_body(
  CORE_mutex_Control  *the_mutex,
  ISR_Level           *level_p
);

#if defined(__RTEMS_DO_NOT_INLINE_CORE_MUTEX_SEIZE__)
  /**
   *  When doing test coverage analysis or trying to minimize the code
   *  space for RTEMS, it is often helpful to not inline this method
   *  multiple times.  It is fairly large and has a high branch complexity
   *  which makes it harder to get full binary test coverage.
   *
   *  @param[in] the_mutex will attempt to lock
   *  @param[in] level_p is the interrupt level holder
   */
  int _CORE_mutex_Seize_interrupt_trylock(
    CORE_mutex_Control  *the_mutex,
    ISR_Level           *level_p
  );
#else
  /**
   *  The default is to favor speed and inlining this definitely saves
   *  a few instructions.  This is very important for mutex performance.
   *
   *  @param[in] _mutex will attempt to lock
   *  @param[in] _level_p is the interrupt level holder
   */
  #define _CORE_mutex_Seize_interrupt_trylock( _mutex, _level_p ) \
     _CORE_mutex_Seize_interrupt_trylock_body( _mutex, _level_p )
#endif


/**
 *  @brief Is Mutex Locked
 *
 *  This routine returns true if the mutex specified is locked and false
 *  otherwise.
 *
 *  @param[in] the_mutex is the mutex to check
 *
 *  @return This method returns true if the mutex is locked.
 */
RTEMS_INLINE_ROUTINE bool _CORE_mutex_Is_locked(
  CORE_mutex_Control  *the_mutex
)
{
  return the_mutex->lock == CORE_MUTEX_LOCKED;
}
 
/**
 *  @brief Does Core Mutex Use FIFO Blocking
 *
 *  This routine returns true if the mutex's wait discipline is FIFO and false
 *  otherwise.
 *
 *  @param[in] the_attribute is the attribute set of the mutex
 *
 *  @return This method returns true if the mutex is using FIFO blocking order.
 */
RTEMS_INLINE_ROUTINE bool _CORE_mutex_Is_fifo(
  CORE_mutex_Attributes *the_attribute
)
{
  return the_attribute->discipline == CORE_MUTEX_DISCIPLINES_FIFO;
}
 
/**
 *  @brief Doex Core Mutex Use Priority Blocking
 *
 *  This routine returns true if the mutex's wait discipline is PRIORITY and
 *  false otherwise.
 *
 *  @param[in] the_attribute is the attribute set of the mutex
 *
 *  @return This method returns true if the mutex is using
 *          priority blocking order.
 */
RTEMS_INLINE_ROUTINE bool _CORE_mutex_Is_priority(
  CORE_mutex_Attributes *the_attribute
)
{
  return the_attribute->discipline == CORE_MUTEX_DISCIPLINES_PRIORITY;
}
 
/**
 *  @brief Does Mutex Use Priority Inheritance
 *
 *  This routine returns true if the mutex's wait discipline is
 *  INHERIT_PRIORITY and false otherwise.
 *
 *  @param[in] the_attribute is the attribute set of the mutex
 *
 *  @return This method returns true if the mutex is using priority
 *          inheritance.
 */
RTEMS_INLINE_ROUTINE bool _CORE_mutex_Is_inherit_priority(
  CORE_mutex_Attributes *the_attribute
)
{
  return the_attribute->discipline == CORE_MUTEX_DISCIPLINES_PRIORITY_INHERIT;
}
 
/**
 *  @brief Does Mutex Use Priority Ceiling
 *
 *  This routine returns true if the mutex's wait discipline is
 *  PRIORITY_CEILING and false otherwise.
 *
 *  @param[in] the_attribute is the attribute set of the mutex
 *  @return This method returns true if the mutex is using priority
 *          ceiling.
 */
RTEMS_INLINE_ROUTINE bool _CORE_mutex_Is_priority_ceiling(
  CORE_mutex_Attributes *the_attribute
)
{
  return the_attribute->discipline == CORE_MUTEX_DISCIPLINES_PRIORITY_CEILING;
}
 
/*
 *  Seize Mutex with Quick Success Path
 *
 *  NOTE: There is no MACRO version of this routine.  A body is in
 *  coremutexseize.c that is duplicated from the .inl by hand.
 *
 *  NOTE: The Doxygen for this routine is in the .h file.
 */

RTEMS_INLINE_ROUTINE int _CORE_mutex_Seize_interrupt_trylock_body(
  CORE_mutex_Control  *the_mutex,
  ISR_Level           *level_p
)
{
  Thread_Control   *executing;

  /* disabled when you get here */

  executing = _Thread_Executing;
  executing->Wait.return_code = CORE_MUTEX_STATUS_SUCCESSFUL;
  if ( !_CORE_mutex_Is_locked( the_mutex ) ) {
    the_mutex->lock       = CORE_MUTEX_LOCKED;
    the_mutex->holder     = executing;
    the_mutex->holder_id  = executing->Object.id;
    the_mutex->nest_count = 1;
    if ( _CORE_mutex_Is_inherit_priority( &the_mutex->Attributes ) ||
         _CORE_mutex_Is_priority_ceiling( &the_mutex->Attributes ) ){

#ifdef __RTEMS_STRICT_ORDER_MUTEX__
       _Chain_Prepend_unprotected( &executing->lock_mutex,
                                   &the_mutex->queue.lock_queue );
       the_mutex->queue.priority_before = executing->current_priority;
#endif

      executing->resource_count++;
    }

    if ( !_CORE_mutex_Is_priority_ceiling( &the_mutex->Attributes ) ) {
      _ISR_Enable( *level_p );
      return 0;
    } /* else must be CORE_MUTEX_DISCIPLINES_PRIORITY_CEILING
       *
       * we possibly bump the priority of the current holder -- which
       * happens to be _Thread_Executing.
       */
    {
      Priority_Control  ceiling;
      Priority_Control  current;

      ceiling = the_mutex->Attributes.priority_ceiling;
      current = executing->current_priority;
      if ( current == ceiling ) {
        _ISR_Enable( *level_p );
        return 0;
      }

      if ( current > ceiling ) {
        _Thread_Disable_dispatch();
        _ISR_Enable( *level_p );
        _Thread_Change_priority(
          the_mutex->holder,
          the_mutex->Attributes.priority_ceiling,
         false
        );
        _Thread_Enable_dispatch();
        return 0;
      }
      /* if ( current < ceiling ) */ {
        executing->Wait.return_code = CORE_MUTEX_STATUS_CEILING_VIOLATED;
        the_mutex->lock       = CORE_MUTEX_UNLOCKED;
        the_mutex->nest_count = 0;     /* undo locking above */
        executing->resource_count--;   /* undo locking above */
        _ISR_Enable( *level_p );
        return 0;
      }
    }
    return 0;
  }

  /*
   *  At this point, we know the mutex was not available.  If this thread
   *  is the thread that has locked the mutex, let's see if we are allowed
   *  to nest access.
   */
  if ( _Thread_Is_executing( the_mutex->holder ) ) {
    switch ( the_mutex->Attributes.lock_nesting_behavior ) {
      case CORE_MUTEX_NESTING_ACQUIRES:
        the_mutex->nest_count++;
        _ISR_Enable( *level_p );
        return 0;
      case CORE_MUTEX_NESTING_IS_ERROR:
        executing->Wait.return_code = CORE_MUTEX_STATUS_NESTING_NOT_ALLOWED;
        _ISR_Enable( *level_p );
        return 0;
      case CORE_MUTEX_NESTING_BLOCKS:
        break;
    }
  }

  /*
   *  The mutex is not available and the caller must deal with the possibility
   *  of blocking.
   */
  return 1;
}

/**@}*/

#endif
/* end of include file */
