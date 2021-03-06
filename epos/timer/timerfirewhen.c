/*
 *  Timer Manager - epos_timer_fire_when directive
 *
 *
 *  COPYRIGHT (c) 1989-2007.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: timerfirewhen.c,v 1.10 2009/12/15 18:26:41 humph Exp $
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
 *  epos_timer_fire_when
 *
 *  This directive allows a thread to start a timer.
 *
 *  Input parameters:
 *    id        - timer id
 *    wall_time - time of day to fire timer
 *    routine   - routine to schedule
 *    user_data - passed as argument to routine when it is fired
 *
 *  Output parameters:
 *    RTEMS_SUCCESSFUL - if successful
 *    error code       - if unsuccessful
 */

epos_status_code epos_timer_fire_when(
  epos_id                            id,
  epos_time_of_day                  *wall_time,
  epos_timer_service_routine_entry   routine,
  void                               *user_data
)
{
  Timer_Control       *the_timer;
  Objects_Locations    location;
  epos_interval       seconds;

  if ( !_TOD_Is_set )
    return RTEMS_NOT_DEFINED;

  if ( !_TOD_Validate( wall_time ) )
    return RTEMS_INVALID_CLOCK;

  if ( !routine )
    return RTEMS_INVALID_ADDRESS;

  seconds = _TOD_To_seconds( wall_time );
  if ( seconds <= _TOD_Seconds_since_epoch() )
    return RTEMS_INVALID_CLOCK;

  the_timer = _Timer_Get( id, &location );
  switch ( location ) {

    case OBJECTS_LOCAL:
      (void) _Watchdog_Remove( &the_timer->Ticker );
      the_timer->the_class = TIMER_TIME_OF_DAY;
      _Watchdog_Initialize( &the_timer->Ticker, routine, id, user_data );
      _Watchdog_Insert_seconds(
         &the_timer->Ticker,
         seconds - _TOD_Seconds_since_epoch()
       );
      _Thread_Enable_dispatch();
      return RTEMS_SUCCESSFUL;

#if defined(RTEMS_MULTIPROCESSING)
    case OBJECTS_REMOTE:            /* should never return this */
#endif
    case OBJECTS_ERROR:
      break;
  }

  return RTEMS_INVALID_ID;
}

