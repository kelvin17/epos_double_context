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
 *  $Id: assocnamebad.c,v 1.8 2009/09/15 09:29:55 ralf Exp $
 */

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <rtems.h>
#include <rtems/assoc.h>

#include <string.h>             /* strcat, strcmp */

#include <inttypes.h>
#include <stdio.h>		          /* sprintf */

/*
 * what to return if a value is not found
 * this is not reentrant, but it really shouldn't be invoked anyway
 */

const char *
epos_assoc_name_bad(
#ifdef RTEMS_DEBUG
    uint32_t   bad_value
#else
    uint32_t   bad_value 
#endif
)
{
#ifdef RTEMS_DEBUG
    static char bad_buffer[32];

    sprintf(bad_buffer, "< %" PRId32 "[0x%" PRIx32 " ] >", bad_value, bad_value);
#else
    static char bad_buffer[40] = "<assocnamebad.c: : BAD NAME>";
#endif
    return bad_buffer;
}
