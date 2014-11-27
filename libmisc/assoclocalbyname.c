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
 *  $Id: assoclocalbyname.c,v 1.1 2007/03/26 22:31:31 joel Exp $
 */

#if HAVE_CONFIG_H
#include "config.h"
#endif

#define INSIDE_ASSOC

#include <rtems.h>
#include <rtems/assoc.h>

#include <string.h>             /* strcat, strcmp */

uint32_t epos_assoc_local_by_name(
  const epos_assoc_t *ap,
  const char          *name
)
{
  const epos_assoc_t *nap;

  nap = epos_assoc_ptr_by_name(ap, name);
  if (nap)
    return nap->local_value;

  return 0;
}

