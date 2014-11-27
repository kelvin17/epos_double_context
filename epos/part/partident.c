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
 *  $Id: partident.c,v 1.10 2009/12/15 18:26:41 humph Exp $
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
 *  epos_partition_ident
 *
 *  This directive returns the system ID associated with
 *  the partition name.
 *
 *  Input parameters:
 *    name - user defined partition name
 *    node - node(s) to be searched
 *    id   - pointer to partition id
 *
 *  Output parameters:
 *    *id               - partition id
 *    RTEMS_SUCCESSFUL - if successful
 *    error code        - if unsuccessful
 */

epos_status_code epos_partition_ident(
  epos_name  name,
  uint32_t    node,
  epos_id   *id
)
{
  Objects_Name_or_id_lookup_errors  status;

  status = _Objects_Name_to_id_u32( &_Partition_Information, name, node, id );

  return _Status_Object_name_errors_to_status[ status ];
}

