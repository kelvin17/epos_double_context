/**
 * @file
 *
 * @ingroup libcsupport
 *
 * @brief ftruncate() - Truncate a File to the Specified Length
 *
 *  COPYRIGHT (c) 1989-1999.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: ftruncate.c,v 1.12.8.1 2011/07/31 14:12:29 joel Exp $
 */

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <unistd.h>
#include <errno.h>


#include <rtems/libio_.h>
#include <rtems/seterr.h>

int ftruncate(
  int     fd,
  off_t   length
)
{
  epos_libio_t                    *iop;
  epos_filesystem_location_info_t  loc;

  epos_libio_check_fd( fd );
  iop = epos_libio_iop( fd );
  epos_libio_check_is_open(iop);
  epos_libio_check_permissions( iop, LIBIO_FLAGS_WRITE );

  /*
   *  Now process the ftruncate() request.
   */

  /*
   *  Make sure we are not working on a directory
   */

  loc = iop->pathinfo;
  if ( !loc.ops->node_type_h )
    epos_set_errno_and_return_minus_one( ENOTSUP );

  if ( (*loc.ops->node_type_h)( &loc ) == RTEMS_FILESYSTEM_DIRECTORY )
    epos_set_errno_and_return_minus_one( EISDIR );

  epos_libio_check_permissions( iop, LIBIO_FLAGS_WRITE );

  if ( !iop->handlers->ftruncate_h )
    epos_set_errno_and_return_minus_one( ENOTSUP );

  return (*iop->handlers->ftruncate_h)( iop, length );
}
