/*
 *  Workspace Handler
 *
 *  COPYRIGHT (c) 1989-2009.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: workspace.c,v 1.5 2009/05/13 16:48:08 joel Exp $
 */

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <rtems/system.h>
#include <rtems/score/wkspace.h>
#include <rtems/score/protectedheap.h>
#include <rtems/score/interr.h>
#include <rtems/config.h>

#include <string.h>  /* for memset */

bool epos_workspace_get_information(
  Heap_Information_block  *the_info
)
{
  if ( !the_info )
    return false;

  return _Protected_heap_Get_information( &_Workspace_Area, the_info );
}

/*
 *  _Workspace_Allocate
 */
bool epos_workspace_allocate(
  uintptr_t   bytes,
  void      **pointer
)
{
  void *ptr;

  /*
   * check the arguments
   */
  if ( !pointer )
    return false;

  if ( !bytes )
    return false;

  /*
   * Allocate the memory
   */
  ptr =  _Protected_heap_Allocate( &_Workspace_Area, (intptr_t) bytes );
  if (!ptr)
    return false;

  *pointer = ptr;
  return true;
}

/*
 *  _Workspace_Allocate
 */
bool epos_workspace_free(
  void *pointer
)
{
   return _Protected_heap_Free( &_Workspace_Area, pointer );
}

