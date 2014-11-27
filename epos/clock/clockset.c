/*
 *  Clock Manager
 *
 *  COPYRIGHT (c) 1989-1999.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: clockset.c,v 1.8 2009/11/30 15:59:55 ralf Exp $
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

/*PAGE
 *
 *  epos_clock_set
 *
 *  This directive sets the date and time for this node.
 *
 *  Input parameters:
 *    time_buffer - pointer to the time and date structure
 *
 *  Output parameters:
 *    RTEMS_SUCCESSFUL - if successful
 *    error code        - if unsuccessful
 */

epos_status_code epos_clock_set(
  epos_time_of_day *time_buffer
)
{
  struct timespec  newtime;
 /*
	printf("year=%d,month=%d,day=%d,hour=%d,minute=%d,second=%d\n",time_buffer->year,time_buffer->month,
		time_buffer->day,time_buffer->hour,time_buffer->minute,time_buffer->second);
		*/
  if ( !time_buffer )
    return RTEMS_INVALID_ADDRESS;

  if ( _TOD_Validate( time_buffer ) ) {
    newtime.tv_sec = _TOD_To_seconds( time_buffer );
    newtime.tv_nsec = time_buffer->ticks *
      epos_configuration_get_nanoseconds_per_tick();

    _Thread_Disable_dispatch();
      _TOD_Set( &newtime );
    _Thread_Enable_dispatch();
    return RTEMS_SUCCESSFUL;
  }
  return RTEMS_INVALID_CLOCK;
}
