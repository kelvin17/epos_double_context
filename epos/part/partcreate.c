/*
 *  Partition Manager
 *
 *
 *  COPYRIGHT (c) 1989-1999.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: partcreate.c,v 1.11 2009/12/15 18:26:41 humph Exp $
 */

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <rtems/system.h>
#include <rtems/rtems/status.h>
#include <rtems/rtems/support.h>
#include <rtems/score/address.h>
#include <rtems/score/object.h>
#include <rtems/rtems/part.h>
#include <rtems/score/thread.h>
#include <rtems/score/sysstate.h>


/*PAGE
 *
 *  epos_partition_create
 *
 *  This directive creates a partiton of fixed sized buffers from the
 *  given contiguous memory area.
 *
 *  Input parameters:
 *    name             - user defined partition name
 *    starting_address - physical start address of partition
 *    length           - physical length in bytes
 *    buffer_size      - size of buffers in bytes
 *    attribute_set    - partition attributes
 *    id               - pointer to partition id
 *
 *  Output parameters:
 *    id               - partition id
 *    RTEMS_SUCCESSFUL - if successful
 *    error code       - if unsuccessful
 */

epos_status_code epos_partition_create(
  epos_name       name,
  void            *starting_address,
  uint32_t         length,
  uint32_t         buffer_size,
  epos_attribute  attribute_set,
  epos_id        *id
)
{
  register Partition_Control *the_partition;

  if ( !epos_is_name_valid( name ) )
    return RTEMS_INVALID_NAME;

  if ( !starting_address )
    return RTEMS_INVALID_ADDRESS;

  if ( !id )
    return RTEMS_INVALID_ADDRESS;

  if ( length == 0 || buffer_size == 0 || length < buffer_size ||
         !_Partition_Is_buffer_size_aligned( buffer_size ) )
    return RTEMS_INVALID_SIZE;

  if ( !_Addresses_Is_aligned( starting_address ) )
     return RTEMS_INVALID_ADDRESS;



  _Thread_Disable_dispatch();               /* prevents deletion */

  the_partition = _Partition_Allocate();

  if ( !the_partition ) {
    _Thread_Enable_dispatch();
    return RTEMS_TOO_MANY;
  }


  the_partition->starting_address      = starting_address;
  the_partition->length                = length;
  the_partition->buffer_size           = buffer_size;
  the_partition->attribute_set         = attribute_set;
  the_partition->number_of_used_blocks = 0;

  _Chain_Initialize( &the_partition->Memory, starting_address,
                        length / buffer_size, buffer_size );

  _Objects_Open(
    &_Partition_Information,
    &the_partition->Object,
    (Objects_Name) name
  );

  *id = the_partition->Object.id;

  _Thread_Enable_dispatch();
  return RTEMS_SUCCESSFUL;
}

