/**
 * @file
 *
 * @ingroup libcsupport
 *
 * @brief fdatasync() - POSIX 1003.1b 6.6.2 - Synchronize the Data of a File
 *
 *  COPYRIGHT (c) 1989-2011.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: fdatasync.c,v 1.10.8.1 2011/07/31 14:12:29 joel Exp $
 */

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <unistd.h>

#include <rtems/libio_.h>
#include <rtems/seterr.h>
#include <errno.h>

int fdatasync(
  int     fd
)
{
  epos_libio_t *iop;

  epos_libio_check_fd( fd );
  iop = epos_libio_iop( fd );
  epos_libio_check_is_open(iop);
  epos_libio_check_permissions_with_error( iop, LIBIO_FLAGS_WRITE, EBADF );

  /*
   *  Now process the fdatasync().
   */

  if ( !iop->handlers->fdatasync_h )
    epos_set_errno_and_return_minus_one( ENOTSUP );

  return (*iop->handlers->fdatasync_h)( iop );
}

