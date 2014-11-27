/** 
 *  @file  isr.inl
 *
 *  @ingroup ScoreISR
 *
 *  @brief The include file of the management of processor interrupt levels.
 *
 *  This include file contains the static implementation of all
 *  inlined routines in the Interrupt Handler.
 */

/*
 *  COPYRIGHT (c) 1989-2004.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: isr.inl,v 1.22 2008/12/22 09:22:17 ralf Exp $
 */

#ifdef _OCC_NO_INLINE_
#include <rtems/system.h>
#include <rtems/score/isr.h>
#endif

#ifndef _RTEMS_SCORE_ISR_H
# error "Never use "isr.inl" directly; include "isr.h" instead."
#endif

#ifndef _RTEMS_SCORE_ISR_INL
#define _RTEMS_SCORE_ISR_INL

/**
 *  @addtogroup ScoreISR 
 *  @{
 */

/**
 *  This function returns true if the vector is a valid vector number
 *  for this processor and false otherwise.
 */

RTEMS_INLINE_ROUTINE bool _ISR_Is_vector_number_valid (
  uint32_t   vector
)
{
  return ( vector <= CPU_INTERRUPT_MAXIMUM_VECTOR_NUMBER );
}

/**
 *  This function returns true if handler is the entry point of a valid
 *  use interrupt service routine and false otherwise.
 */

RTEMS_INLINE_ROUTINE bool _ISR_Is_valid_user_handler (
  void *handler
)
{
  return (handler != NULL);
}

/**@}*/

#endif
/* end of include file */
