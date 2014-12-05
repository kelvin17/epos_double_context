/*
 *  RTEMS Object Helper -- Obtain Name of API
 *
 *  COPYRIGHT (c) 1989-2008.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: rtemsobjectgetapiname.c,v 1.4 2009/11/30 15:59:55 ralf Exp $
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <rtems/system.h>
#include <rtems/score/object.h>
#include <rtems/rtems/object.h>

#include <rtems/assoc.h>

epos_assoc_t epos_objects_api_assoc[] = {
  { "Internal", OBJECTS_INTERNAL_API, 0},
  { "Classic",  OBJECTS_CLASSIC_API, 0},
  { "POSIX",    OBJECTS_POSIX_API, 0},
  { 0, 0, 0 }
};
const char *epos_object_get_api_name(
  int api
)
{
  const epos_assoc_t *api_assoc;

  api_assoc = epos_assoc_ptr_by_local( epos_objects_api_assoc, api );
  if ( api_assoc )
    return api_assoc->name;
  return "BAD CLASS";
}

