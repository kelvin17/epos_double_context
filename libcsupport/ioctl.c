/**
 * @file
 *
 * @ingroup libcsupport
 *
 * @brief   ioctl() system call
 *
 *  This routine is not defined in the POSIX 1003.1b standard but is
 *  commonly supported on most UNIX and POSIX systems.
 *
 *  COPYRIGHT (c) 1989-1999.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: ioctl.c,v 1.15 2006/08/31 01:10:00 joel Exp $
 */

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdarg.h>

#include <rtems/libio_.h>
#include <rtems/seterr.h>

#include <unistd.h>
#include <sys/errno.h>

int ioctl(
  int  fd,
  ioctl_command_t  command,
  ...
)
{
  va_list            ap;
  epos_status_code  rc;
  epos_libio_t     *iop;
  void              *buffer;

  epos_libio_check_fd( fd );
  iop = epos_libio_iop( fd );
  epos_libio_check_is_open(iop);

  va_start(ap, command);

  buffer = va_arg(ap, void *);

  /*
   *  Now process the ioctl().
   */

  if ( !iop->handlers )
    epos_set_errno_and_return_minus_one( EBADF );

  if ( !iop->handlers->ioctl_h )
    epos_set_errno_and_return_minus_one( ENOTSUP );

  rc = (*iop->handlers->ioctl_h)( iop, command, buffer );

  return rc;
}
