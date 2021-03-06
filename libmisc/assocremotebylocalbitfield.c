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
 *  $Id: assocremotebylocalbitfield.c,v 1.1 2007/03/26 22:31:31 joel Exp $
 */

#if HAVE_CONFIG_H
#include "config.h"
#endif

#define INSIDE_ASSOC

#include <rtems.h>
#include <rtems/assoc.h>

#include <string.h>             /* strcat, strcmp */

uint32_t   epos_assoc_remote_by_local_bitfield(
    const epos_assoc_t *ap,
    uint32_t             local_value
)
{
  uint32_t   b;
  uint32_t   remote_value = 0;

  for (b = 1; b; b <<= 1)
    if (b & local_value)
      remote_value |= epos_assoc_remote_by_local(ap, b);

  return remote_value;
}
