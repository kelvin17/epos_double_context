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
 *  $Id: assocremotebylocal.c,v 1.1 2007/03/26 22:31:31 joel Exp $
 */

#if HAVE_CONFIG_H
#include "config.h"
#endif

#define INSIDE_ASSOC

#include <rtems.h>
#include <rtems/assoc.h>

#include <string.h>             /* strcat, strcmp */

uint32_t epos_assoc_remote_by_local(
  const epos_assoc_t *ap,
  uint32_t       local_value
)
{
  const epos_assoc_t *nap;

  nap = epos_assoc_ptr_by_local(ap, local_value);
  if (nap)
    return nap->remote_value;

  return 0;
}
