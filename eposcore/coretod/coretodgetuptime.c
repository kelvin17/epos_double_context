/**
 * @file
 *
 * @ingroup ScoreTOD
 *
 * @brief 核心抽象层TOD管理器实现。(SuperCore Time of Day (TOD) Handler implementation.)
 */

/*
 *  Time of Day (TOD) Handler - get uptime
 */

/*  COPYRIGHT (c) 1989-2008.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: coretodgetuptime.c,v 1.5 2008/12/10 22:13:28 joel Exp $
 */

#if HAVE_CONFIG_H
#include "config.h"
#endif
#include <stdio.h>
#include <rtems/system.h>
#include <rtems/score/isr.h>
#include <rtems/score/timestamp.h>
#include <rtems/score/tod.h>
#include <rtems/score/watchdog.h>

/*
 *  _TOD_Get_uptime
 *
 *  This routine is used to obtain the system uptime
 *
 *  Input parameters:
 *    time  - pointer to the timestamp structure
 *
 *  Output parameters: NONE
 */

void _TOD_Get_uptime(
  Timestamp_Control *uptime
)
{
  ISR_Level         level;
  Timestamp_Control offset;
  Timestamp_Control up;
  long              nanoseconds;

  /* assume time checked for NULL by caller */

  /* _TOD_Uptime is in native timestamp format */
  nanoseconds = 0;
  _ISR_Disable( level );
    up = _TOD_Uptime;
    if ( _Watchdog_Nanoseconds_since_tick_handler )
      nanoseconds = (*_Watchdog_Nanoseconds_since_tick_handler)();
  _ISR_Enable( level );

  _Timestamp_Set( &offset, 0, nanoseconds );
//  printf("coretodgetuptime.c\n");
  _Timestamp_Add_to( &up, &offset );
  *uptime = up;
}

