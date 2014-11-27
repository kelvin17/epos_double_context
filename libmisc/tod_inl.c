/** 
 *  @file  tod.inl
 *
 * @ingroup ScoreTod
 *
 *  @brief The include file of the Time of Day Handler.
 *
 *  This file contains the static inline implementation of the inlined routines
 *  from the Time of Day Handler.
 */

/*
 *  COPYRIGHT (c) 1989-2007.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: tod.inl,v 1.25 2009/05/05 19:37:28 joel Exp $
 */

#ifdef _OCC_NO_INLINE_
#include <rtems/system.h>
#include <rtems/score/tod.h>
#endif

#ifndef _RTEMS_SCORE_TOD_H
# error "Never use "tod.inl" directly; include "tod.h" instead."
#endif

#ifndef _RTEMS_SCORE_TOD_INL
#define _RTEMS_SCORE_TOD_INL

#include <sys/time.h> /* struct timeval */

#include <rtems/score/isr.h>
#include <sys/types.h>

/**
 *  @addtogroup ScoreTOD 
 *  @{
 */

/**
 *  This routine deactivates updating of the current time of day.
 */

RTEMS_INLINE_ROUTINE void _TOD_Deactivate( void )
{
  /* XXX do we need something now that we are using timespec for TOD */
}

/**
 *  This routine activates updating of the current time of day.
 */

RTEMS_INLINE_ROUTINE void _TOD_Activate( void )
{
  /* XXX do we need something now that we are using timespec for TOD */
}

/**
 *  This routine returns a timeval based upon the internal timespec format TOD.
 */

RTEMS_INLINE_ROUTINE void _TOD_Get_timeval(
  struct timeval *time
)
{
  ISR_Level       level;
  struct timespec now;
  suseconds_t     useconds;

  _ISR_Disable(level);
    _TOD_Get( &now );
  _ISR_Enable(level);

  useconds = (suseconds_t)now.tv_nsec;
  useconds /= (suseconds_t)TOD_NANOSECONDS_PER_MICROSECOND;
 
  time->tv_sec  = now.tv_sec;
  time->tv_usec = useconds;
}

/**@}*/

#endif
/* end of include file */
