/**
 * @file
 *
 * @ingroup libcsupport
 *
 * @brief rtems assoc routines
 */

/*
 * assoc.c
 *      rtems assoc routines
 *
 *  $Id: assocptrbyremote.c,v 1.1 2007/03/26 22:31:31 joel Exp $
 */

#if HAVE_CONFIG_H
#include "config.h"
#endif

#define INSIDE_ASSOC

#include <rtems.h>
#include <rtems/assoc.h>

#include <string.h>             /* strcat, strcmp */

const epos_assoc_t *epos_assoc_ptr_by_remote(
  const epos_assoc_t *ap,
  uint32_t       remote_value
)
{
  const epos_assoc_t *default_ap = 0;

  if (epos_assoc_is_default(ap))
    default_ap = ap++;

  for ( ; ap->name; ap++)
    if (ap->remote_value == remote_value)
      return ap;

  return default_ap;
}
