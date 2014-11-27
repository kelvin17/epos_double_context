/*  Get_all_counters
 *
 *  This routine allows TA5 to atomically obtain the iteration counters.
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

void Get_all_counters()
{
  epos_mode        previous_mode;
  epos_status_code status;

  status = epos_task_mode(
    RTEMS_NO_PREEMPT,
    RTEMS_PREEMPT_MASK,
    &previous_mode
  );
  directive_failed( status, "epos_task_mode to RTEMS_NO_PREEMPT" );

  Temporary_count = Count;
  Count.count[ 1 ] = 0;
  Count.count[ 2 ] = 0;
  Count.count[ 3 ] = 0;
  Count.count[ 4 ] = 0;
  Count.count[ 5 ] = 0;
  Count.count[ 6 ] = 0;

  status = epos_task_mode( RTEMS_PREEMPT, RTEMS_PREEMPT_MASK, &previous_mode );
  directive_failed( status, "epos_task_mode to RTEMS_PREEMPT" );
}
