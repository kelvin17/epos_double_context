/*
 *  Partition Manager
 *
 *
 *  COPYRIGHT (c) 1989-2007.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: partreturnbuffer.c,v 1.7 2009/12/15 18:26:41 humph Exp $
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
 *  epos_partition_return_buffer
 *
 *  This directive will return the given buffer to the specified
 *  buffer partition.
 *
 *  Input parameters:
 *    id     - partition id
 *    buffer - pointer to buffer address
 *
 *  Output parameters:
 *    RTEMS_SUCCESSFUL - if successful
 *    error code - if unsuccessful
 */

epos_status_code epos_partition_return_buffer(
  epos_id  id,
  void     *buffer
)
{
  register Partition_Control *the_partition;
  Objects_Locations           location;

  the_partition = _Partition_Get( id, &location );
  switch ( location ) {

    case OBJECTS_LOCAL:
      if ( _Partition_Is_buffer_valid( buffer, the_partition ) ) {
        _Partition_Free_buffer( the_partition, buffer );
        the_partition->number_of_used_blocks -= 1;
        _Thread_Enable_dispatch();
        return RTEMS_SUCCESSFUL;
      }
      _Thread_Enable_dispatch();
      return RTEMS_INVALID_ADDRESS;

    case OBJECTS_ERROR:
      break;
  }

  return RTEMS_INVALID_ID;
}

