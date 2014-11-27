/**
 * @file options.inl
 *
 * @ingroup ClassicOptions
 *
 *  This file contains the static inline implementation of the inlined
 *  routines from the Options Handler.
 */

/*  COPYRIGHT (c) 1989-2008.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: options.inl,v 1.15 2008/09/04 17:43:18 ralf Exp $
 */

#ifdef _OCC_NO_INLINE_
#include <rtems/system.h>
#include <rtems/rtems/options.h>
#endif

#ifndef _RTEMS_RTEMS_OPTIONS_H
# error "Never use "options.inl" directly; include "options.h" instead."
#endif

#ifndef _RTEMS_RTEMS_OPTIONS_INL
#define _RTEMS_RTEMS_OPTIONS_INL

/**
 *  @addtogroup ClassicOptions
 *  @{
 */

/**
 *  @brief Options_Is_no_wait
 *
 *  This function returns TRUE if the RTEMS_NO_WAIT option is enabled in
 *  option_set, and FALSE otherwise.
 */
RTEMS_INLINE_ROUTINE bool _Options_Is_no_wait (
  epos_option option_set
)
{
   return (option_set & RTEMS_NO_WAIT) ? true : false;
}

/**
 *  @brief Options_Is_any
 *
 *  This function returns TRUE if the RTEMS_EVENT_ANY option is enabled in
 *  OPTION_SET, and FALSE otherwise.
 */
RTEMS_INLINE_ROUTINE bool _Options_Is_any (
  epos_option option_set
)
{
   return (option_set & RTEMS_EVENT_ANY) ? true : false;
}

/**@}*/

#endif
/* end of include file */
