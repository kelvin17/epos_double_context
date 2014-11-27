/**
 * @file
 *
 * @ingroup libcsupport
 *
 * @brief 
 */

/*
 *  This file contains the support infrastructure used to manage the
 *  table of integer style file descriptors used by the socket calls.
 *
 *  COPYRIGHT (c) 1989-1999.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: libio_sockets.c,v 1.12 2009/03/27 13:45:31 joel Exp $
 */

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <rtems/libio_.h>             /* libio_.h pulls in rtems */
#include <rtems.h>

#include <errno.h>

/*
 * Convert an RTEMS file descriptor to a BSD socket pointer.
 */

struct socket *epos_bsdnet_fdToSocket(
  int fd
)
{
  epos_libio_t *iop;

  /* same as epos_libio_check_fd(_fd) but different return */
  if ((uint32_t)fd >= epos_libio_number_iops) {
    errno = EBADF;
    return NULL;
  }
  iop = &epos_libio_iops[fd];

  /* same as epos_libio_check_is_open(iop) but different return */
  if ((iop->flags & LIBIO_FLAGS_OPEN) == 0) {
    errno = EBADF;
    return NULL;
  } 

  if (iop->data1 == NULL)
    errno = EBADF;
  return iop->data1;
}

/*
 * Create an RTEMS file descriptor for a socket
 */

int epos_bsdnet_makeFdForSocket(
  void *so,
  const epos_filesystem_file_handlers_r *h
)
{
  epos_libio_t *iop;
  int fd;

  iop = epos_libio_allocate();
  if (iop == 0) {
      errno = ENFILE;
      return -1;
  }
  fd = iop - epos_libio_iops;
  iop->flags |= LIBIO_FLAGS_WRITE | LIBIO_FLAGS_READ;
  iop->data0 = fd;
  iop->data1 = so;
  iop->handlers = h;
  return fd;
}
