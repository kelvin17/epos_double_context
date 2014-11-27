/**
 * @file
 *
 * @ingroup ScoreTOD
 *
 * @brief 核心抽象层TOD管理器实现。(SuperCore Time of Day (TOD) Handler implementation.)
 */

/*  COPYRIGHT (c) 1989-2011.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: coretodusectoticks.c,v 1.1.2.1 2011/09/01 18:24:57 joel Exp $
 */


#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <rtems/system.h>
#include <rtems/config.h>
#include <rtems/score/tod.h>

uint32_t TOD_MICROSECONDS_TO_TICKS(
  uint32_t microseconds
)
{ 
  uint32_t ticks;
  uint32_t microseconds_per_tick;

  /**
   *  We should ensure the ticks not be truncated by integer division.  We
   *  need to have it be greater than or equal to the requested time.  It
   *  should not be shorter.
   */ 
  microseconds_per_tick = epos_configuration_get_microseconds_per_tick();
  ticks                 = microseconds / microseconds_per_tick;
  if ( (microseconds % microseconds_per_tick) != 0 )
    ticks += 1;

  return ticks;
}

