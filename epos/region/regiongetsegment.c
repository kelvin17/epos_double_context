/*
 *  Region Manager
 *
 *
 *  COPYRIGHT (c) 1989-2007.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: regiongetsegment.c,v 1.17 2009/12/15 18:26:41 humph Exp $
 */

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <rtems/system.h>
#include <rtems/rtems/status.h>
#include <rtems/rtems/support.h>
#include <rtems/score/object.h>
#include <rtems/rtems/options.h>
#include <rtems/rtems/region.h>
#include <rtems/score/states.h>
#include <rtems/score/thread.h>
#include <rtems/score/apimutex.h>

/*PAGE
 *
 *  epos_region_get_segment
 *
 *  This directive will obtain a segment from the given region.
 *
 *  Input parameters:
 *    id         - region id
 *    size       - segment size in bytes
 *    option_set - wait option
 *    timeout    - number of ticks to wait (0 means wait forever)
 *    segment    - pointer to segment address
 *
 *  Output parameters:
 *    segment    - pointer to segment address filled in
 *    RTEMS_SUCCESSFUL - if successful
 *    error code - if unsuccessful
 */

epos_status_code epos_region_get_segment(
  epos_id           id,
  uintptr_t          size,
  epos_option       option_set,
  epos_interval     timeout,
  void              **segment
)
{
  Thread_Control     *executing;
  Objects_Locations   location;
  epos_status_code   return_status;
  Region_Control     *the_region;
  void               *the_segment;

  if ( !segment )
    return RTEMS_INVALID_ADDRESS;

  *segment = NULL;

  if ( size == 0 )
    return RTEMS_INVALID_SIZE;

  _RTEMS_Lock_allocator();

    executing  = _Thread_Executing;
    the_region = _Region_Get( id, &location );
    switch ( location ) {

      case OBJECTS_LOCAL:
        if ( size > the_region->maximum_segment_size )
          return_status = RTEMS_INVALID_SIZE;

        else {
          _Region_Debug_Walk( the_region, 1 );

          the_segment = _Region_Allocate_segment( the_region, size );

          _Region_Debug_Walk( the_region, 2 );

          if ( the_segment ) {
            the_region->number_of_used_blocks += 1;
            *segment = the_segment;
            return_status = RTEMS_SUCCESSFUL;
          } else if ( _Options_Is_no_wait( option_set ) ) {
            return_status = RTEMS_UNSATISFIED;
          } else {
            /*
             *  Switch from using the memory allocation mutex to using a
             *  dispatching disabled critical section.  We have to do this
             *  because this thread is going to block.
             */
            _Thread_Disable_dispatch();
            _RTEMS_Unlock_allocator();

            executing->Wait.queue           = &the_region->Wait_queue;
            executing->Wait.id              = id;
            executing->Wait.count           = size;
            executing->Wait.return_argument = segment;

            _Thread_queue_Enter_critical_section( &the_region->Wait_queue );

            _Thread_queue_Enqueue( &the_region->Wait_queue, timeout );

            _Thread_Enable_dispatch();

            return (epos_status_code) executing->Wait.return_code;
          }
        }
        break;

      case OBJECTS_ERROR:
      default:
        return_status = RTEMS_INVALID_ID;
        break;
    }

  _RTEMS_Unlock_allocator();
  return return_status;
}

