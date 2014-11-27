/**
 * @file eventset.inl
 *
 * @ingroup ClassicEventSet
 *
 *  This include file contains the information pertaining to event sets.
 */

/*  COPYRIGHT (c) 1989-2008.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: eventset.inl,v 1.16 2008/09/04 17:43:18 ralf Exp $
 */

#ifdef _OCC_NO_INLINE_
#include <rtems/system.h>
#include <rtems/rtems/eventset.h>
#include <rtems/score/isr.h>
#endif

#ifndef _RTEMS_RTEMS_EVENTSET_H
# error "Never use "eventset.inl" directly; include "eventset.h" instead."
#endif

#ifndef _RTEMS_RTEMS_EVENTSET_INL
#define _RTEMS_RTEMS_EVENTSET_INL

/**
 *  @addtogroup ClassicEventSet
 *  @{
 */

/**
 *  @brief Event_sets_Is_empty
 *
 *  This function returns TRUE if on events are posted in the event_set,
 *  and FALSE otherwise.
 */
RTEMS_INLINE_ROUTINE bool _Event_sets_Is_empty(
  epos_event_set the_event_set
)
{
  return ( the_event_set == 0 );
}

/**
 *  @brief Event_sets_Post
 *
 *  This routine posts the given new_events into the event_set
 *  passed in.  The result is returned to the user in event_set.
 */
RTEMS_INLINE_ROUTINE void _Event_sets_Post(
  epos_event_set  the_new_events,
  epos_event_set *the_event_set
)
{
  ISR_Level level;

  _ISR_Disable( level );
    *the_event_set |= the_new_events;
  _ISR_Enable( level );
}

/**
 *  @brief Event_sets_Get
 *
 *  This function returns the events in event_condition which are
 *  set in event_set.
 */
RTEMS_INLINE_ROUTINE epos_event_set _Event_sets_Get(
  epos_event_set the_event_set,
  epos_event_set the_event_condition
)
{
   return ( the_event_set & the_event_condition );
}

/**
 *  @brief Event_sets_Clear
 *
 *  This function removes the events in mask from the event_set
 *  passed in.  The result is returned to the user in event_set.
 */
RTEMS_INLINE_ROUTINE epos_event_set _Event_sets_Clear(
 epos_event_set the_event_set,
 epos_event_set the_mask
)
{
   return ( the_event_set & ~(the_mask) );
}

/**@}*/

#endif
/* end of include file */
