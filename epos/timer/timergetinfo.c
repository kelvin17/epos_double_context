/*
 *  Timer Manager - epos_timer_get_information directive
 *
 *
 *  COPYRIGHT (c) 1989-2007.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: timergetinfo.c,v 1.8 2009/12/15 18:26:42 humph Exp $
 */

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <rtems/system.h>
#include <rtems/rtems/status.h>
#include <rtems/rtems/support.h>
#include <rtems/score/object.h>
#include <rtems/score/thread.h>
#include <rtems/rtems/timer.h>
#include <rtems/score/tod.h>
#include <rtems/score/watchdog.h>

/*PAGE
 *
 *  epos_timer_get_information
 *
 *  This directive allows a thread to obtain information about a timer.
 *
 *  Input parameters:
 *    id       - timer id
 *    the_info - pointer to timer information block
 *
 *  Output parameters:
 *    *the_info        - region information block filled in
 *    RTEMS_SUCCESSFUL - if successful
 *    error code       - if unsuccessful
 *
 */

epos_status_code epos_timer_get_information(
  epos_id                 id,
  epos_timer_information *the_info
)
{
  Timer_Control     *the_timer;
  Objects_Locations  location;

  if ( !the_info )
    return RTEMS_INVALID_ADDRESS;

  the_timer = _Timer_Get( id, &location );
  switch ( location ) {

    case OBJECTS_LOCAL:
      the_info->the_class  = the_timer->the_class;
      the_info->initial    = the_timer->Ticker.initial;
      the_info->start_time = the_timer->Ticker.start_time;
      the_info->stop_time  = the_timer->Ticker.stop_time;
      _Thread_Enable_dispatch();
      return RTEMS_SUCCESSFUL;

    case OBJECTS_ERROR:
      break;
  }

  return RTEMS_INVALID_ID;
}

