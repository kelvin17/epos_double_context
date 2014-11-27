/**
 * @file
 *
 * @ingroup libcsupport
 *
 * @brief symlink() - POSIX 1003.1b - X.X.X - XXX
 */

/*
 *  symlink() - POSIX 1003.1b - X.X.X - XXX
 *
 *  COPYRIGHT (c) 1989-1999.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: symlink.c,v 1.12 2009/03/30 17:05:23 joel Exp $
 */

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <rtems/libio_.h>
#include <rtems/seterr.h>
#include <errno.h>

int symlink(
  const char *actualpath,
  const char *sympath
)
{
  epos_filesystem_location_info_t    loc;
  int                                 i;
  const char                         *name_start;
  int                                 result;

  epos_filesystem_get_start_loc( sympath, &i, &loc );

  if ( !loc.ops->evalformake_h ) {
    epos_set_errno_and_return_minus_one( ENOTSUP );
  }

  result = (*loc.ops->evalformake_h)( &sympath[i], &loc, &name_start );
  if ( result != 0 )
    return -1;

  if ( !loc.ops->symlink_h ) {
    epos_filesystem_freenode( &loc );
    epos_set_errno_and_return_minus_one( ENOTSUP );
  }

  result = (*loc.ops->symlink_h)( &loc, actualpath, name_start);

  epos_filesystem_freenode( &loc );

  return result;
}
