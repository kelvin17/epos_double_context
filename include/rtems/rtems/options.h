/**
 * @file options.h
 *
 *  @ingroup ClassicOptions
 *
 *  This include file contains information which defines the
 *  options available on many directives.
 */

/*  COPYRIGHT (c) 1989-2008.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: options.h,v 1.14 2008/04/18 20:08:08 joel Exp $
 */

#ifndef _RTEMS_RTEMS_OPTIONS_H
#define _RTEMS_RTEMS_OPTIONS_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 *  @defgroup ClassicOptions Classic API Options
 *
 *  @ingroup ClassicRTEMS
 *
 *  @brief RTEMS接口option码定义。
 *
 *  This encapsulates functionality which XXX
 */
/**@{*/

/**
 *  The following type defines the control block used to manage
 *  option sets.
 */
typedef uint32_t   epos_option;

/**
 *  The following constants define the individual options which may
 *  be used to compose an option set.
 */
#define RTEMS_DEFAULT_OPTIONS   0x00000000

/**
 *  This option constants indicates that the task is to wait on resource.
 */
#define RTEMS_WAIT      0x00000000
/**
 *  This option constants indicates that the task is to not wait on
 *  the resource.  If it is not available, return immediately with
 *  a status to indicate unsatisfied.
 */
#define RTEMS_NO_WAIT   0x00000001

/**
 *  This option constants indicates that the task wishes to wait until
 *  all events of interest are available.
 */
#define RTEMS_EVENT_ALL 0x00000000

/**
 *  This option constants indicates that the task wishes to wait until
 *  ANY events of interest are available.
 */
#define RTEMS_EVENT_ANY 0x00000002

#ifndef __RTEMS_APPLICATION__
#ifndef _OCC_NO_INLINE_
#include <rtems/rtems/options.inl>
#endif

#ifdef _OCC_NO_INLINE_
RTEMS_INLINE_ROUTINE bool _Options_Is_no_wait (
  epos_option option_set
);

RTEMS_INLINE_ROUTINE bool _Options_Is_any (
  epos_option option_set
);
#endif
#endif

#ifdef __cplusplus
}
#endif

/**@}*/

#endif
/* end of include file */
