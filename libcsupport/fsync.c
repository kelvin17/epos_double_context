/**
 * @file
 *
 * @ingroup libcsupport
 *
 * @brief  fsync() - POSIX 1003.1b 6.6.1 - Synchronize the State of a File
 *
 *  COPYRIGHT (c) 1989-2011.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: fsync.c,v 1.11.8.1 2011/07/31 14:12:29 joel Exp $
 */

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <unistd.h>

#include <rtems/libio_.h>
#include <rtems/seterr.h>
#include <errno.h>

int fsync(
  int     fd
)
{
  epos_libio_t *iop;

  epos_libio_check_fd( fd );
  iop = epos_libio_iop( fd );
  epos_libio_check_is_open(iop);

  /*
   *  Now process the fsync().
   */

  if ( !iop->handlers )
    epos_set_errno_and_return_minus_one( EBADF );

  if ( !iop->handlers->fsync_h )
    epos_set_errno_and_return_minus_one( ENOTSUP );

  return (*iop->handlers->fsync_h)( iop );
}

