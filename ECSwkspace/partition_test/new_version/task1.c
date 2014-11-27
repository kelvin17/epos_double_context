/*  Task_1
 *
 *  This routine serves as a test task.  It tests the partition manager.
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

epos_task Task_1(
  epos_task_argument argument
)
{
  epos_id           ptid_1;
  epos_id           ptid_2;
  void              *buffer_address_1;
  void              *buffer_address_2;
  void              *buffer_address_3;
  void              *buffer_address_4;
  void              *buffer_address_5;
  epos_status_code  status;

  puts( "TA1 - rtems_partition_ident - partition 1 id = " );
  status = epos_partition_ident(
    Partition_name[ 1 ],
    RTEMS_SEARCH_ALL_NODES,
    &ptid_1
  );
  //directive_failed( status, "rtems_partition_ident of PT1" );
  directive_suc(status);
  printf( "%08" PRIxepos_id "\n", ptid_1 );

  puts( "TA1 - rtems_partition_ident - partition 2 id = " );
  status = epos_partition_ident(
    Partition_name[ 2 ],
    RTEMS_SEARCH_ALL_NODES,
    &ptid_2
  );
  directive_suc(status);
  printf( "%08" PRIxepos_id "\n", ptid_2 );


  puts(
    "TA1 - rtems_partition_get_buffer - buffer 1 from partition 1  - "  );
  status = epos_partition_get_buffer( ptid_1, &buffer_address_1 );
  //directive_failed( status, "rtems_partition_get_buffer" );
  directive_suc(status);
  Put_address_from_area_1( buffer_address_1 );
  new_line;
  

  puts(
    "TA1 - rtems_partition_get_buffer - buffer 2 from partition 1  - "  );
  status = epos_partition_get_buffer( ptid_1, &buffer_address_2 );
  //directive_failed( status, "rtems_partition_get_buffer" );
  directive_suc(status);
  Put_address_from_area_1( buffer_address_2 );
  new_line;

  puts(
    "TA1 - rtems_partition_get_buffer - buffer 3 from partition 2  - "  );
  status = epos_partition_get_buffer( ptid_2, &buffer_address_3 );
  //directive_failed( status, "rtems_partition_get_buffer" );
  directive_suc(status);
  Put_address_from_area_2( buffer_address_3 );
  new_line;

  puts(
    "TA1 - rtems_partition_get_buffer - buffer 4 from partition 2  - "  );
  status = epos_partition_get_buffer( ptid_2, &buffer_address_4 );
  //directive_failed( status, "rtems_partition_get_buffer" );
  directive_suc(status);
  Put_address_from_area_2( buffer_address_4 );
  new_line;

  puts(
    "TA1 - rtems_partition_get_buffer - attempt to get buffer 5 from partition 2"  );
  status = epos_partition_get_buffer( ptid_2, &buffer_address_5 );
  //directive_failed( status, "rtems_partition_get_buffer" );
  if(status == RTEMS_UNSATISFIED)printf("Error code :RTEMS_UNSATISFIED.For all buffers are allocated!\n");

  puts(
    "TA1 - rtems_partition_return_buffer - buffer 1 to partition 1 - "  );
  Put_address_from_area_1( buffer_address_1 );
  new_line;
  status = epos_partition_return_buffer( ptid_1, buffer_address_1 );
  //directive_failed( status, "rtems_partition_return_buffer" );
  directive_suc(status);

  puts(
    "TA1 - rtems_partition_return_buffer - buffer 2 to partition 1 - "  );
  Put_address_from_area_1( buffer_address_2 );
  new_line;
  status = epos_partition_return_buffer( ptid_1, buffer_address_2 );
  //directive_failed( status, "rtems_partition_return_buffer" );
  directive_suc(status);

  puts(
    "TA1 - rtems_partition_return_buffer - buffer 3 to partition 2 - "  );
  Put_address_from_area_2( buffer_address_3 );
  new_line;
  status = epos_partition_return_buffer( ptid_2, buffer_address_3 );
  //directive_failed( status, "rtems_partition_return_buffer" );
  directive_suc(status);

  puts(
    "TA1 - rtems_partition_return_buffer - buffer 4 to partition 2 - "  );
   Put_address_from_area_2( buffer_address_4 );
  new_line;
  status = epos_partition_return_buffer( ptid_2, buffer_address_4 );
  //directive_failed( status, "rtems_partition_return_buffer" );
  directive_suc(status);

  puts( "TA1 - rtems_partition_delete - delete partition 1"  );
  status = epos_partition_delete( ptid_1 );
  //directive_failed( status, "rtems_partition_delete" );
  directive_suc(status);

  puts( "TA1 - rtems_partition_delete - delete partition 2"  );
  status = epos_partition_delete( ptid_2 );
  //directive_failed( status, "rtems_partition_delete" );
  directive_suc(status);
  puts( "*** END OF TEST Partition ***" );
 // epos_test_exit(0);
}
