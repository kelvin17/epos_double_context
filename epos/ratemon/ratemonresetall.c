/*
 *  Rate Monotonic Manager -- Reset Statistics for All Periods
 *
 *  COPYRIGHT (c) 1989-2007.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: ratemonresetall.c,v 1.2 2007/05/17 22:46:45 joel Exp $
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <rtems/system.h>
#include <rtems/rtems/status.h>
#include <rtems/rtems/support.h>
#include <rtems/score/object.h>
#include <rtems/rtems/ratemon.h>
#include <rtems/score/thread.h>

/*
 *  epos_rate_monotonic_reset_all_statistics
 */
void epos_rate_monotonic_reset_all_statistics( void )
{
  Objects_Id        id;
  epos_status_code status;

   /*
    *  Prevent allocation or deallocation of any of the periods while
    *  we are cycling.  Also this is an optimization which ensures that
    *  we only disable/enable once.  The call to
    *  epos_rate_monotonic_reset_statistics will be in a nested dispatch
    *  disabled critical section.
    */
  _Thread_Disable_dispatch();

    /*
     * Cycle through all possible ids and try to reset each one.  If it
     * is a period that is inactive, we just get an error back.  No big deal.
     */
    for ( id=_Rate_monotonic_Information.minimum_id ;
          id <= _Rate_monotonic_Information.maximum_id ;
          id++ ) {
      status = epos_rate_monotonic_reset_statistics( id );
    }

  /*
   *  Done so exit thread dispatching disabled critical section.
   */
  _Thread_Enable_dispatch();
}

