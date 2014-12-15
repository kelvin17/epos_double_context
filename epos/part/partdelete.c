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
 *  $Id: partdelete.c,v 1.8 2009/12/15 18:26:41 humph Exp $
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
 *  epos_partition_delete
 *
 *  This directive allows a thread to delete a partition specified by
 *  the partition identifier, provided that none of its buffers are
 *  still allocated.
 *
 *  Input parameters:
 *    id - partition id
 *
 *  Output parameters:
 *    RTEMS_SUCCESSFUL - if successful
 *    error code       - if unsuccessful
 */

epos_status_code epos_partition_delete(
  epos_id id
)
{
  register Partition_Control *the_partition;
  Objects_Locations           location;

  the_partition = _Partition_Get( id, &location );
  switch ( location ) {

    case OBJECTS_LOCAL:
      if ( the_partition->number_of_used_blocks == 0 ) {
        _Objects_Close( &_Partition_Information, &the_partition->Object );
        _Partition_Free( the_partition );

        _Thread_Enable_dispatch();
        return RTEMS_SUCCESSFUL;
      }
      _Thread_Enable_dispatch();
      return RTEMS_RESOURCE_IN_USE;

    case OBJECTS_ERROR:
      break;
  }

  return RTEMS_INVALID_ID;
}

