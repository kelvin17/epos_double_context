/**
 * @file
 *
 * @ingroup libcsupport
 *
 * @brief Process free requests deferred because they were from ISR
 *  or other critical section.
 *
 *  COPYRIGHT (c) 1989-2007.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: malloc_deferred.c,v 1.4 2008/09/01 11:42:19 ralf Exp $
 */

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <rtems.h>
#ifdef RTEMS_NEWLIB
#include <stdlib.h>
#include <errno.h>

#include <malloc_p.h>

epos_chain_control RTEMS_Malloc_GC_list;

bool malloc_is_system_state_OK(void)
{
  if ( _Thread_Dispatch_disable_level > 0 )
    return false;

  if ( _ISR_Nest_level > 0 )
    return false;

  return true;
}

void malloc_deferred_frees_initialize(void)
{
  epos_chain_initialize_empty(&RTEMS_Malloc_GC_list);
}

void malloc_deferred_frees_process(void)
{
  epos_chain_node  *to_be_freed;

  /*
   *  If some free's have been deferred, then do them now.
   */
  while ((to_be_freed = epos_chain_get(&RTEMS_Malloc_GC_list)) != NULL)
    free(to_be_freed);
}

void malloc_deferred_free(
  void *pointer
)
{
  epos_chain_append(&RTEMS_Malloc_GC_list, (epos_chain_node *)pointer);
}
#endif