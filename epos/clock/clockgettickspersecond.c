/*
 *  Clock Manager - Get Ticks Per Second
 *
 *  COPYRIGHT (c) 1989-2008.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: clockgettickspersecond.c,v 1.2 2008/12/16 17:35:59 joel Exp $
 */

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <rtems/system.h>
#include <rtems/config.h>
#include <rtems/rtems/status.h>
#include <rtems/rtems/clock.h>
#include <rtems/score/isr.h>
#include <rtems/score/thread.h>
#include <rtems/score/tod.h>
#include <rtems/score/watchdog.h>

epos_interval epos_clock_get_ticks_per_second(void)
{
  return TOD_MICROSECONDS_PER_SECOND /
    epos_configuration_get_microseconds_per_tick(); 
}
