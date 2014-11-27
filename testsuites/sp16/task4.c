/*  Task_4
 *
 *  This routine serves as a test task.  It competes with the other tasks
 *  for region resources.
 *
 *  Input parameters:
 *    argument - task argument
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

epos_task Task_4(
  epos_task_argument argument
)
{
  epos_status_code  status;
  void              *segment_address_1;
  void              *segment_address_2;

  printk( "TA4 - epos_region_get_segment - wait on 1.5K segment from region 1" );
  status = epos_region_get_segment(
    Region_id[ 1 ],
    1536,
    RTEMS_DEFAULT_OPTIONS,
    RTEMS_NO_TIMEOUT,
    &segment_address_1
  );
  directive_failed( status, "epos_region_get_segment" );

  status = epos_region_return_segment( Region_id[ 1 ], segment_address_1 );
  directive_failed( status, "epos_region_return_segment" );
  puts_nocr( "TA4 - got and returned " );
  Put_address_from_area_1( segment_address_1 );
  new_line;

  status = epos_task_wake_after( TICKS_PER_SECOND );
  directive_failed( status, "epos_task_wake_after" );

  printk( "TA4 - epos_region_get_segment - wait on 3K segment from region 1" );
  status = epos_region_get_segment(
    Region_id[ 1 ],
    3072,
    RTEMS_DEFAULT_OPTIONS,
    RTEMS_NO_TIMEOUT,
    &segment_address_2
  );
  directive_failed( status, "epos_region_get_segment" );
}
