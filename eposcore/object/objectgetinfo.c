/*
 *  COPYRIGHT (c) 1989-2008.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: objectgetinfo.c,v 1.7 2009/07/08 17:56:07 joel Exp $
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <rtems/system.h>
#include <rtems/score/object.h>
#include <rtems/score/thread.h>
#include <rtems/score/wkspace.h>

Objects_Information *_Objects_Get_information(
  Objects_APIs   the_api,
  uint32_t       the_class
)
{
  Objects_Information *info;
  int the_class_api_maximum;

  if ( !the_class )
    return NULL;

  /*
   *  This call implicitly validates the_api so we do not call
   *  _Objects_Is_api_valid above here.
   */
  the_class_api_maximum = _Objects_API_maximum_class( the_api );
  if ( the_class_api_maximum == 0 )
    return NULL;

  if ( the_class > (uint32_t) the_class_api_maximum )
    return NULL;

  if ( !_Objects_Information_table[ the_api ] )
    return NULL;

  info = _Objects_Information_table[ the_api ][ the_class ];
  if ( !info )
    return NULL;
  return info;
}

