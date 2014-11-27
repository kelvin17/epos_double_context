/*  Task_3
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

epos_task Task_3(
  epos_task_argument argument
)
{
  epos_status_code  status;
  void              *segment_address_1;
  void              *segment_address_2;

  printk(
    "TA3 - epos_region_get_segment - wait on 3950 byte segment from region 2"
  );
  status = epos_region_get_segment(
    Region_id[ 2 ],
    3950,
    RTEMS_DEFAULT_OPTIONS,
    RTEMS_NO_TIMEOUT,
    &segment_address_1
  );
  directive_failed( status, "epos_region_get_segment" );
  puts_nocr( "TA3 - got segment from region 2 - " );
  Put_address_from_area_2( segment_address_1 );
  new_line;
  directive_failed( status, "epos_region_return_segment" );

  printk( "TA3 - epos_region_get_segment - wait on 2K segment from region 3" );
  status = epos_region_get_segment(
    Region_id[ 3 ],
    2048,
    RTEMS_DEFAULT_OPTIONS,
    RTEMS_NO_TIMEOUT,
    &segment_address_2
  );
  directive_failed( status, "epos_region_get_segment" );
}
