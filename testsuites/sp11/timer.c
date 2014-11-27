/*  Timer_functions
 *
 *  These routines are the timer service routines used by this test.
 *
 *  Input parameters:  NONE
 *
 *  Output parameters:  NONE
 *
 *  COPYRIGHT (c) 1989-1999.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id$
 */

#include "system.h"

epos_timer_service_routine TA1_send_18_to_self_5_seconds(
  epos_id  ignored_id,
  void     *ignored_address
)
{
  epos_status_code status;

  status = epos_event_send( Task_id[ 1 ], RTEMS_EVENT_18 );
  directive_failed_with_level( status, "epos_event_send of 18", 1 );
}

epos_timer_service_routine TA1_send_8_to_self_60_seconds(
  epos_id  ignored_id,
  void     *ignored_address
)
{
  epos_status_code status;

  status = epos_event_send( Task_id[ 1 ], RTEMS_EVENT_8 );
  directive_failed_with_level( status, "epos_event_send of 8", 1 );
}

epos_timer_service_routine TA1_send_9_to_self_60_seconds(
  epos_id  ignored_id,
  void     *ignored_address
)
{
  epos_status_code status;

  status = epos_event_send( Task_id[ 1 ], RTEMS_EVENT_9 );
  directive_failed_with_level( status, "epos_event_send of 9", 1 );
}

epos_timer_service_routine TA1_send_10_to_self(
  epos_id  ignored_id,
  void     *ignored_address
)
{
  epos_status_code status;

  status = epos_event_send( Task_id[ 1 ], RTEMS_EVENT_10 );
  directive_failed_with_level( status, "epos_event_send of 10", -1 );
}

epos_timer_service_routine TA1_send_1_to_self_every_second(
  epos_id  ignored_id,
  void     *ignored_address
)
{
  epos_status_code status;

  status = epos_event_send( Task_id[ 1 ], RTEMS_EVENT_1 );
  directive_failed_with_level( status, "epos_event_send of 1", 1 );
}

epos_timer_service_routine TA1_send_11_to_self(
  epos_id  ignored_id,
  void     *ignored_address
)
{
  epos_status_code status;

  status = epos_event_send( Task_id[ 1 ], RTEMS_EVENT_11 );
  directive_failed_with_level( status, "epos_event_send of 11", -1 );
}

epos_timer_service_routine TA2_send_10_to_self(
  epos_id  ignored_id,
  void     *ignored_address
)
{
  epos_status_code status;

  status = epos_event_send( Task_id[ 2 ], RTEMS_EVENT_10 );
  directive_failed_with_level( status, "epos_event_send of 10", 1 );
}
