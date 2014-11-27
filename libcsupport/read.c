/**
 * @file
 *
 * @ingroup libcsupport
 *
 * @brief read() - POSIX 1003.1b 6.4.1 - Read From a File
 */

/*
 *  read() - POSIX 1003.1b 6.4.1 - Read From a File
 *
 *  COPYRIGHT (c) 1989-1999.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: read.c,v 1.15.2.1 2011/07/31 14:12:29 joel Exp $
 */

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <rtems.h>
#include <rtems/libio_.h>
#include <rtems/seterr.h>
#include <errno.h>

ssize_t read(
  int         fd,
  void       *buffer,
  size_t      count
)
{
  ssize_t      rc;
  epos_libio_t *iop;

  epos_libio_check_fd( fd );
  iop = epos_libio_iop( fd );
  epos_libio_check_is_open( iop );
  epos_libio_check_buffer( buffer );
  epos_libio_check_count( count );
  epos_libio_check_permissions_with_error( iop, LIBIO_FLAGS_READ, EBADF );

  /*
   *  Now process the read().
   */

  if ( !iop->handlers->read_h )
    epos_set_errno_and_return_minus_one( ENOTSUP );

  rc = (*iop->handlers->read_h)( iop, buffer, count );

  if ( rc > 0 )
    iop->offset += rc;

  return rc;
}

/*
 *  _read_r
 *
 *  This is the Newlib dependent reentrant version of read().
 */

#if defined(RTEMS_NEWLIB) && !defined(HAVE__READ_R)

#include <reent.h>

ssize_t _read_r(
  struct _reent *ptr __attribute__((unused)),
  int            fd,
  void          *buf,
  size_t         nbytes
)
{
  return read( fd, buf, nbytes );
}
#endif
