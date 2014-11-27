/*
 *  Clock Manager
 *
 *  COPYRIGHT (c) 1989-2009.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: clockget.c,v 1.11 2009/11/30 15:59:55 ralf Exp $
 */

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <rtems/system.h>
#include <rtems/rtems/status.h>
#include <rtems/rtems/clock.h>
#include <rtems/score/isr.h>
#include <rtems/score/thread.h>
#include <rtems/score/tod.h>
#include <rtems/score/watchdog.h>

/*
 *  epos_clock_get
 *
 *  This directive returns the current date and time.  If the time has
 *  not been set by a tm_set then an error is returned.
 *
 *  Input parameters:
 *    option      - which value to return
 *    time_buffer - pointer to output buffer (a time and date structure
 *                  or an interval)
 *
 *  Output parameters:
 *    time_buffer      - output filled in
 *    RTEMS_SUCCESSFUL - if successful
 *    error code       - if unsuccessful
 */

epos_status_code epos_clock_get(
  epos_clock_get_options  option,
  void                    *time_buffer
)
{
  if ( !time_buffer )
    return RTEMS_INVALID_ADDRESS;

  if ( option == RTEMS_CLOCK_GET_TOD )
    return epos_clock_get_tod( (epos_time_of_day *)time_buffer );

  if ( option == RTEMS_CLOCK_GET_SECONDS_SINCE_EPOCH )
      return epos_clock_get_seconds_since_epoch((epos_interval *)time_buffer);

  if ( option == RTEMS_CLOCK_GET_TICKS_SINCE_BOOT ) {
    epos_interval *interval = (epos_interval *)time_buffer;

    *interval = epos_clock_get_ticks_since_boot();
    return RTEMS_SUCCESSFUL;
  }

  if ( option == RTEMS_CLOCK_GET_TICKS_PER_SECOND ) {
    epos_interval *interval = (epos_interval *)time_buffer;

    *interval = epos_clock_get_ticks_per_second();
    return RTEMS_SUCCESSFUL;
  }

  if ( option == RTEMS_CLOCK_GET_TIME_VALUE )
      return epos_clock_get_tod_timeval( (struct timeval *)time_buffer );

  return RTEMS_INVALID_NUMBER;

}
