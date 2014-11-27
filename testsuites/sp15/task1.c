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
 *
 *  $Id$
 */

#include "system.h"

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
  epos_status_code  status;

  puts_nocr( "TA1 - epos_partition_ident - partition 1 id = " );
  status = epos_partition_ident(
    Partition_name[ 1 ],
    RTEMS_SEARCH_ALL_NODES,
    &ptid_1
  );
  directive_failed( status, "epos_partition_ident of PT1" );
  printk( "%08x\n", ptid_1 );

  puts_nocr( "TA1 - epos_partition_ident - partition 2 id = " );
  status = epos_partition_ident(
    Partition_name[ 2 ],
    RTEMS_SEARCH_ALL_NODES,
    &ptid_2
  );
  directive_failed( status, "epos_partition_ident of PT2" );
  printk( "%08x\n", ptid_2 );

  puts_nocr(
    "TA1 - epos_partition_get_buffer - buffer 1 from partition 1  - "
  );
  status = epos_partition_get_buffer( ptid_1, &buffer_address_1 );
  directive_failed( status, "epos_partition_get_buffer" );
  Put_address_from_area_1( buffer_address_1 );
  new_line;

  puts_nocr(
    "TA1 - epos_partition_get_buffer - buffer 2 from partition 1  - "
  );
  status = epos_partition_get_buffer( ptid_1, &buffer_address_2 );
  directive_failed( status, "epos_partition_get_buffer" );
  Put_address_from_area_1( buffer_address_2 );
  new_line;

  puts_nocr(
    "TA1 - epos_partition_get_buffer - buffer 1 from partition 2  - "
  );
  status = epos_partition_get_buffer( ptid_2, &buffer_address_3 );
  directive_failed( status, "epos_partition_get_buffer" );
  Put_address_from_area_2( buffer_address_3 );
  new_line;

  puts_nocr(
    "TA1 - epos_partition_get_buffer - buffer 2 from partition 2  - "
  );
  status = epos_partition_get_buffer( ptid_2, &buffer_address_4 );
  directive_failed( status, "epos_partition_get_buffer" );
  Put_address_from_area_2( buffer_address_4 );
  new_line;

  puts_nocr(
    "TA1 - epos_partition_return_buffer - buffer 1 to partition 1 - "
  );
  Put_address_from_area_1( buffer_address_1 );
  new_line;
  status = epos_partition_return_buffer( ptid_1, buffer_address_1 );
  directive_failed( status, "epos_partition_return_buffer" );

  puts_nocr(
    "TA1 - epos_partition_return_buffer - buffer 2 to partition 1 - "
  );
  Put_address_from_area_1( buffer_address_2 );
  new_line;
  status = epos_partition_return_buffer( ptid_1, buffer_address_2 );
  directive_failed( status, "epos_partition_return_buffer" );

  puts_nocr(
    "TA1 - epos_partition_return_buffer - buffer 1 to partition 2 - "
  );
  Put_address_from_area_2( buffer_address_3 );
  new_line;
  status = epos_partition_return_buffer( ptid_2, buffer_address_3 );
  directive_failed( status, "epos_partition_return_buffer" );

  puts_nocr(
    "TA1 - epos_partition_return_buffer - buffer 2 to partition 2 - "
  );
  Put_address_from_area_2( buffer_address_4 );
  new_line;
  status = epos_partition_return_buffer( ptid_2, buffer_address_4 );
  directive_failed( status, "epos_partition_return_buffer" );

  printk( "TA1 - epos_partition_delete - delete partition 1"
  );
  status = epos_partition_delete( ptid_1 );
  directive_failed( status, "epos_partition_delete" );

  printk( "TA1 - epos_partition_delete - delete partition 2"
  );
  status = epos_partition_delete( ptid_2 );
  directive_failed( status, "epos_partition_delete" );

  printk( "*** END OF TEST 15 ***" );
  epos_test_exit( 0 );
}
