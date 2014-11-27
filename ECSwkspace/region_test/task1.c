/*  Task_1
 *
 *  This routine serves as a test task.  It tests the region manager.
 *
 *  Input parameters:
 *    argument - task argument
 *
 *  Output parameters:  NONE
 *
 *  COPYRIGHT (c) 1989-2009.
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

epos_task Task_1(
  epos_task_argument argument
)
{
  epos_id           rnid;
  void              *segment_address_1;
  void              *segment_address_2;
  void              *segment_address_3;
  void              *segment_address_4;
  epos_status_code  status;

  status = epos_region_ident( Region_name[ 1 ], &rnid );
  printk( "TA1 - epos_region_ident - rnid => %08" PRIxepos_id "\n", rnid );
  directive_suc( status);
 

  put(
    "TA1 - epos_region_get_segment - wait on 1000 byte segment from region 2"
  );
  status = epos_region_get_segment(
    Region_id[ 2 ],
    1000,
    RTEMS_DEFAULT_OPTIONS,
    RTEMS_NO_TIMEOUT,
    &segment_address_1
  );
  directive_suc( status);
  put( "TA1 - got segment from region 2 - " );
  Put_address_from_area_2( segment_address_1 );
  new_line;

  put( "TA1 - epos_region_get_segment - get 3080 byte segment " );
  put( "from region 1 - NO_WAIT" );
  status = epos_region_get_segment(
    Region_id[ 1 ],
    3080,
    RTEMS_NO_WAIT,
    RTEMS_NO_TIMEOUT,
    &segment_address_3
  );
  directive_suc( status );
  put( "TA1 - got segment from region 1 - " );
  Put_address_from_area_1( segment_address_3 );
 // new_line;

  put( "TA1 - epos_task_wake_after - yield processor" );
  status = epos_task_wake_after( RTEMS_YIELD_PROCESSOR );
  directive_suc( status);

//epos_test_pause();

  put(
    "TA1 - epos_region_return_segment - return segment to region 1 - "
  );
  Put_address_from_area_1( segment_address_3 );
  status = epos_region_return_segment( Region_id[ 1 ], segment_address_3 );
  directive_suc( status);
 // new_line;

  put(
    "TA1 - epos_region_get_segment - wait 10 seconds for 3K "
      "segment from region 1"
  );
  status = epos_region_get_segment(
    Region_id[ 1 ],
    3072,
    RTEMS_DEFAULT_OPTIONS,
    10 * epos_clock_get_ticks_per_second(),
    &segment_address_4
  );
  directive_suc( status );
  put( "TA1 - got segment from region 1 - " );
  Put_address_from_area_1( segment_address_4 );
 // new_line;

  puts(
    "TA1 - epos_region_return_segment - return segment to region 2 - "
  );
  Put_address_from_area_2( segment_address_1 );
 // new_line;
  status = epos_region_return_segment( Region_id[ 2 ], segment_address_1 );
  directive_suc( status );

  put( "TA1 - epos_task_wake_after - yield processor" );
  status = epos_task_wake_after( RTEMS_YIELD_PROCESSOR );
  directive_suc( status );

  put( "TA1 - epos_task_delete - delete TA3 false" );
//  status = epos_task_delete( Task_id[ 3 ] );
//  directive_suc( status );

/*
  status = epos_task_create(
    Task_name[ 4 ],
    BASE_PRIORITY,
    RTEMS_MINIMUM_STACK_SIZE,
    RTEMS_DEFAULT_MODES,
    RTEMS_DEFAULT_ATTRIBUTES,
    &Task_id[ 4 ]
  );
  directive_suc( status);

  status = epos_task_create(
    Task_name[ 5 ],
    BASE_PRIORITY,
    RTEMS_MINIMUM_STACK_SIZE,
    RTEMS_DEFAULT_MODES,
    RTEMS_DEFAULT_ATTRIBUTES,
    &Task_id[ 5 ]
  );
  directive_suc( status );

  status = epos_task_start( Task_id[ 4 ], Task_4, 0 );
  directive_suc( status );

  status = epos_task_start( Task_id[ 5 ], Task5, 0 );
  directive_suc( status );

  puts( "TA1 - epos_task_wake_after - yield processor" );
  status = epos_task_wake_after( 1 * epos_clock_get_ticks_per_second() );
 directive_suc( status );

  puts(
    "TA1 - epos_region_return_segment - return segment to region 1 - "
  );
  Put_address_from_area_1( segment_address_4 );
  status = epos_region_return_segment( Region_id[ 1 ], segment_address_4 );
  directive_suc( status );
  new_line;

  puts( "TA1 - epos_task_wake_after - yield processor" );
  status = epos_task_wake_after( 1 * epos_clock_get_ticks_per_second() );
  directive_suc( status );

  puts( "TA1 - epos_region_get_segment - wait 10 seconds for 3K " );
  puts     ( "segment from region 1");
  status = epos_region_get_segment(
    Region_id[ 1 ],
    3072,
    RTEMS_DEFAULT_OPTIONS,
    10 * epos_clock_get_ticks_per_second(),
    &segment_address_4
  );
  directive_suc( status );
  puts( "TA1 - got segment from region 1 - " );
  Put_address_from_area_1( segment_address_4 );
  new_line;

  puts( "TA1 - epos_task_wake_after - yield processor" );
  status = epos_task_wake_after( 1 * epos_clock_get_ticks_per_second() );
 directive_suc( status );

  puts( "TA1 - epos_task_delete - delete TA4" );
  status = epos_task_delete( Task_id[ 4 ] );
  directive_suc( status );

  puts(
    "TA1 - epos_region_return_segment - return segment to region 1 - "
  );
  Put_address_from_area_1( segment_address_4 );
  status = epos_region_return_segment( Region_id[ 1 ], segment_address_4 );
  directive_suc( status );
  new_line;

  puts( "TA1 - epos_task_wake_after - yield processor" );
  status = epos_task_wake_after( RTEMS_YIELD_PROCESSOR );

  puts( "TA1 - epos_region_delete - delete region 1" );
  status = epos_region_delete( Region_id[ 1 ] );
  directive_suc( status );
*/
  put( "TA1 - epos_region_get_segment - get 3K segment from region 4" );
  status = epos_region_get_segment(
    Region_id[ 4 ],
    3072,
    RTEMS_DEFAULT_OPTIONS,
    RTEMS_NO_TIMEOUT,
    &segment_address_1
  );
  directive_suc( status );
  put( "TA1 - got segment from region 4 - " );
  Put_address_from_area_4( segment_address_1 );
 // new_line;

  put(
   "TA1 - epos_region_get_segment - attempt to get 3K segment from region 4"
  );
  status =  epos_region_get_segment(
    Region_id[ 4 ],
    3072,
    RTEMS_NO_WAIT,
    RTEMS_NO_TIMEOUT,
    &segment_address_2
  );
 /* fatal_directive_status(
    status,
    RTEMS_UNSATISFIED,
    "epos_region_get_segment with no memory left"
  );*/
  if(status!=RTEMS_UNSATISFIED)printf("system down\n");
  
  printk( "TA1 - epos_region_get_segment - RTEMS_UNSATISFIED:%d\n",status);

  put( "TA1 - epos_region_extend - extend region 4 by 4K - 1" );
  status = epos_region_extend(
    Region_id[ 4 ],
    (char *) &Area_4[4096] ,//(char *) &Area_4[4096] + 1,
    4096 - 1
  );
  directive_suc( status );

  put(
   "TA1 - epos_region_get_segment - attempt to get 3K segment from region 4"
  );
  status = epos_region_get_segment(
    Region_id[ 4 ],
    3072,
    RTEMS_NO_WAIT,
    RTEMS_NO_TIMEOUT,
    &segment_address_3
  );
  directive_suc( status );
  put( "TA1 - got 3K segment from region 4 - " );
  Put_address_from_area_4( segment_address_3 );
 // new_line;

  put(
    "TA1 - epos_region_return_segment - return segment to region 4 - "
  );
  Put_address_from_area_4( segment_address_1 );
  status = epos_region_return_segment( Region_id[ 4 ], segment_address_1 );
 directive_suc( status );
//  new_line;

  put(
    "TA1 - epos_region_return_segment - return segment to region 4 - "
  );
  Put_address_from_area_4( segment_address_3 );
  status = epos_region_return_segment( Region_id[ 4 ], segment_address_3 );
  directive_suc( status );
//  new_line;

  put( "TA1 - epos_region_delete - delete region 4" );
  status = epos_region_delete( Region_id[ 4 ] );
  directive_suc( status );

  put( "*** END OF TEST 16 ***" );
}
