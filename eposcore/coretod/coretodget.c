/**
 * @file
 *
 * @ingroup ScoreTOD
 *
 * @brief 核心抽象层TOD管理器实现。(SuperCore Time of Day (TOD) Handler implementation.)
 */

/*
 *  Time of Day (TOD) Handler - get TOD
 *
 *  COPYRIGHT (c) 1989-2008.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: coretodget.c,v 1.4 2008/12/08 19:41:31 joel Exp $
 */

#if HAVE_CONFIG_H
#include "config.h"
#endif
#include <stdio.h>
#include <rtems/system.h>
#include <rtems/score/isr.h>
#include <rtems/score/timespec.h>
#include <rtems/score/timestamp.h>
#include <rtems/score/tod.h>
#include <rtems/score/watchdog.h>

/*
 *  _TOD_Get
 *
 *  This routine is used to obtain the current date and time.
 *
 *  Input parameters:
 *    time  - pointer to the time and date structure
 *
 *  Output parameters: NONE
 */

void _TOD_Get(
  struct timespec *time
)
{
  ISR_Level         level;
  Timestamp_Control offset;
  Timestamp_Control now;
  long              nanoseconds;
 
  /* assume time checked for NULL by caller */

  /* _TOD_Now is the native current time */
  nanoseconds = 0;
  _ISR_Disable( level );
    now = _TOD_Now;
    if ( _Watchdog_Nanoseconds_since_tick_handler )
    	{
      nanoseconds = (*_Watchdog_Nanoseconds_since_tick_handler)();
    	} 
  _ISR_Enable( level );
//printf("nanoseconds is %d\n",nanoseconds);
  _Timestamp_Set( &offset, 0, nanoseconds );
//printf("offset is %d,%d\n",offset.tv_sec,offset.tv_nsec);
  _Timestamp_Add_to( &now, &offset );
 // printf("now.second= %d\n",now.tv_sec);

  _Timestamp_To_timespec( &now, time );
 // printf("time.second= %d\n",time->tv_sec);
}

