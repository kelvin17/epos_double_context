/*  Task_2
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
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "sp01.h"

epos_task Task_2(
  epos_task_argument argument
)
{
  epos_status_code    status;
  epos_task_priority  previous_priority;
  void                *segment_address_1;
  void                *segment_address_2;

  puts( "TA2 - epos_region_get_segment - wait on 2K segment from region 1" );
  status = epos_region_get_segment(
    Region_id[ 1 ],
    2048,
    RTEMS_DEFAULT_OPTIONS,
    RTEMS_NO_TIMEOUT,
    &segment_address_1
  );
  directive_suc( status);
  puts( "TA2 - got segment from region 1 - " );
  Put_address_from_area_1( segment_address_1 );
  new_line;

  puts(
    "TA2 - epos_region_return_segment - return segment to region 1 - "
  );
  Put_address_from_area_1( segment_address_1 );
  status = epos_region_return_segment( Region_id[ 1 ], segment_address_1 );
  directive_suc( status);
  new_line;

  puts( "TA2 - epos_task_set_priority - make self highest priority task" );
  status = epos_task_set_priority(
    RTEMS_SELF,
    BASE_PRIORITY-1,
    &previous_priority
  );
  directive_suc( status);

  puts("TA2 - epos_region_get_segment - wait on 3750 byte segment");
  status = epos_region_get_segment(
    Region_id[ 2 ],
    3750,
    RTEMS_DEFAULT_OPTIONS,
    RTEMS_NO_TIMEOUT,
    &segment_address_2
  );
  directive_suc( status);
  puts( "TA2 - got segment from region 2 - ");
  Put_address_from_area_2( segment_address_2 );
  new_line;

  puts(
    "TA2 - epos_region_return_segment - return segment to region 2 - "
  );
  Put_address_from_area_2( segment_address_2 );
  status = epos_region_return_segment( Region_id[ 2 ], segment_address_2 );
  directive_suc( status);
  new_line;

  puts( "TA2 - epos_task_delete - delete self" );
  status = epos_task_wake_after( RTEMS_MICROSECONDS_TO_TICKS( 1000000 ) );
//  status = epos_task_delete( RTEMS_SELF );
//  directive_suc( status);
}
