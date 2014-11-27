/*
 *  Clock Manager - epos_clock_get_tod
 *
 *  COPYRIGHT (c) 1989-2007.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: clockgettod.c,v 1.3 2009/11/30 15:59:55 ralf Exp $
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
#include <time.h>		
#include <sys/time.h>

epos_status_code epos_clock_get_tod(
  epos_time_of_day  *time_buffer
)
{
  epos_time_of_day *tmbuf = time_buffer;
  struct tm *time;
  struct timeval now;
 
  if ( !time_buffer )
    return RTEMS_INVALID_ADDRESS;
 
  if ( !_TOD_Is_set )
    return RTEMS_NOT_DEFINED;

  /* Obtain the current time */
  _TOD_Get_timeval( &now);
 
  /* Split it into a closer format ,in time.h*/
//  gmtime_r( &now.tv_sec, &time );
//now.tv_sec=1337472000;
//printf("task1,now.tv_sec=%d\n",now.tv_sec);
	time=gmtime(&now.tv_sec);
//time->tm_year=now.tv_sec/TOD_SECONDS_PER_NON_LEAP_YEAR;
//time->tm_mon=(now.tv_sec-time->tm_year * TOD_SECONDS_PER_NON_LEAP_YEAR)/(TOD_SECONDS_PER_DAY*30);
/* Now adjust it to the RTEMS format */
//now.tv_sec-=time->tm_year*TOD_SECONDS_PER_NON_LEAP_YEAR;
	tmbuf->year	 = time->tm_year + 1900;
	tmbuf->month  = time->tm_mon + 1;
	tmbuf->day	  = time->tm_mday;
	tmbuf->hour   = time->tm_hour;
	tmbuf->minute = time->tm_min;
	tmbuf->second = time->tm_sec;
	tmbuf->ticks  = now.tv_usec / 
	epos_configuration_get_microseconds_per_tick();

 //  printf("%d,%d,%d,%d,%d\n",tmbuf->year,tmbuf->day,tmbuf->hour,tmbuf->minute,tmbuf->second);

  return RTEMS_SUCCESSFUL;
}
