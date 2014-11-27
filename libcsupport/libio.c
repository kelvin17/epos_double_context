/**
 * @file
 *
 * @ingroup libcsupport
 *
 * @brief 
 */

/*
 *  This file contains the support infrastructure used to manage the
 *  table of integer style file descriptors used by the low level
 *  POSIX system calls like open(), read, fstat(), etc.
 *
 *  COPYRIGHT (c) 1989-1999.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: libio.c,v 1.49 2010/04/30 08:55:41 sh Exp $
 */
#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <rtems.h>
#include <rtems/libio_.h>
#include <rtems/assoc.h>

/* define this to alias O_NDELAY to  O_NONBLOCK, i.e.,
 * O_NDELAY is accepted on input but fcntl(F_GETFL) returns
 * O_NONBLOCK. This is because rtems has no distinction
 * between the two (but some systems have).
 * Note that accepting this alias creates a problem:
 * an application trying to clear the non-blocking flag
 * using a
 *
 *    fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) & ~O_NDELAY);
 *
 * does (silently) ignore the operation.
 */
#undef ACCEPT_O_NDELAY_ALIAS

/*
 *  epos_libio_fcntl_flags
 *
 *  Convert UNIX fnctl(2) flags to ones that RTEMS drivers understand
 */

const epos_assoc_t access_modes_assoc[] = {
  { "READ",       LIBIO_FLAGS_READ,  O_RDONLY },
  { "WRITE",      LIBIO_FLAGS_WRITE, O_WRONLY },
  { "READ/WRITE", LIBIO_FLAGS_READ_WRITE, O_RDWR },
  { 0, 0, 0 },
};

const epos_assoc_t status_flags_assoc[] = {
#ifdef ACCEPT_O_NDELAY_ALIAS
  { "NO DELAY",  LIBIO_FLAGS_NO_DELAY,  O_NDELAY },
#endif
  { "NONBLOCK",  LIBIO_FLAGS_NO_DELAY,  O_NONBLOCK },
  { "APPEND",    LIBIO_FLAGS_APPEND,    O_APPEND },
  { "CREATE",    LIBIO_FLAGS_CREATE,    O_CREAT },
  { 0, 0, 0 },
};

uint32_t   epos_libio_fcntl_flags(
  uint32_t   fcntl_flags
)
{
  uint32_t   flags = 0;
  uint32_t   access_modes;

  /*
   * Access mode is a small integer
   */

  access_modes = fcntl_flags & O_ACCMODE;
  fcntl_flags &= ~O_ACCMODE;
  flags = epos_assoc_local_by_remote( access_modes_assoc, access_modes );

  /*
   * Everything else is single bits
   */

  flags |=
     epos_assoc_local_by_remote_bitfield(status_flags_assoc, fcntl_flags);
  return flags;
}

/*
 *  epos_libio_to_fcntl_flags
 *
 *  Convert RTEMS internal flags to UNIX fnctl(2) flags
 */

uint32_t   epos_libio_to_fcntl_flags(
  uint32_t   flags
)
{
  uint32_t   fcntl_flags = 0;

  if ( (flags & LIBIO_FLAGS_READ_WRITE) == LIBIO_FLAGS_READ_WRITE ) {
    fcntl_flags |= O_RDWR;
  } else if ( (flags & LIBIO_FLAGS_READ) == LIBIO_FLAGS_READ) {
    fcntl_flags |= O_RDONLY;
  } else if ( (flags & LIBIO_FLAGS_WRITE) == LIBIO_FLAGS_WRITE) {
    fcntl_flags |= O_WRONLY;
  }

  if ( (flags & LIBIO_FLAGS_NO_DELAY) == LIBIO_FLAGS_NO_DELAY ) {
    fcntl_flags |= O_NONBLOCK;
  }

  if ( (flags & LIBIO_FLAGS_APPEND) == LIBIO_FLAGS_APPEND ) {
    fcntl_flags |= O_APPEND;
  }

  if ( (flags & LIBIO_FLAGS_CREATE) == LIBIO_FLAGS_CREATE ) {
    fcntl_flags |= O_CREAT;
  }

  return fcntl_flags;
}

/*
 *  epos_libio_allocate
 *
 *  This routine searches the IOP Table for an unused entry.  If it
 *  finds one, it returns it.  Otherwise, it returns NULL.
 */

epos_libio_t *epos_libio_allocate( void )
{
  epos_libio_t *iop, *next;
  epos_status_code rc;
  epos_id sema;

  epos_libio_lock();

  if (epos_libio_iop_freelist) {
    rc = epos_semaphore_create(
      RTEMS_LIBIO_IOP_SEM(epos_libio_iop_freelist - epos_libio_iops),
      1,
      RTEMS_BINARY_SEMAPHORE | RTEMS_INHERIT_PRIORITY | RTEMS_PRIORITY,
      0,
      &sema
    );
    if (rc != RTEMS_SUCCESSFUL)
      goto failed;
    iop = epos_libio_iop_freelist;
    next = iop->data1;
    (void) memset( iop, 0, sizeof(epos_libio_t) );
    iop->flags = LIBIO_FLAGS_OPEN;
    iop->sem = sema;
    epos_libio_iop_freelist = next;
    goto done;
  }

failed:
  iop = 0;

done:
  epos_libio_unlock();
  return iop;
}

/*
 *  epos_libio_free
 *
 *  This routine frees the resources associated with an IOP (file descriptor)
 *  and clears the slot in the IOP Table.
 */

void epos_libio_free(
  epos_libio_t *iop
)
{
  epos_libio_lock();

    if (iop->sem)
      epos_semaphore_delete(iop->sem);

    iop->flags &= ~LIBIO_FLAGS_OPEN;
    iop->data1 = epos_libio_iop_freelist;
    epos_libio_iop_freelist = iop;

  epos_libio_unlock();
}

/*
 *  epos_libio_is_open_files_in_fs
 *
 *  This routine scans the entire file descriptor table to determine if the
 *  are any active file descriptors that refer to the at least one node in the
 *  file system that we are trying to dismount.
 *
 *  If there is at least one node in the file system referenced by the mount
 *  table entry a 1 is returned, otherwise a 0 is returned.
 */

int epos_libio_is_open_files_in_fs(
  epos_filesystem_mount_table_entry_t * fs_mt_entry
)
{
  epos_libio_t     *iop;
  int                result = 0;
  uint32_t           i;

  epos_libio_lock();

  /*
   *  Look for any active file descriptor entry.
   */

  for (iop=epos_libio_iops,i=0; i < epos_libio_number_iops; iop++, i++){

    if ((iop->flags & LIBIO_FLAGS_OPEN) != 0) {

       /*
        *  Check if this node is under the file system that we
        *  are trying to dismount.
        */

       if ( iop->pathinfo.mt_entry == fs_mt_entry ) {
          result = 1;
          break;
       }
    }
  }

  epos_libio_unlock();

  return result;
}

/*
 *  epos_libio_is_file_open
 *
 *  This routine scans the entire file descriptor table to determine if the
 *  given file refers to an active file descriptor.
 *
 *  If the given file is open a 1 is returned, otherwise a 0 is returned.
 */

int epos_libio_is_file_open(
  void         *node_access
)
{
  epos_libio_t     *iop;
  int                result=0;
  uint32_t           i;

  epos_libio_lock();

  /*
   *  Look for any active file descriptor entry.
   */

 for (iop=epos_libio_iops,i=0; i < epos_libio_number_iops; iop++, i++){
    if ((iop->flags & LIBIO_FLAGS_OPEN) != 0) {

       /*
        *  Check if this node is under the file system that we
        *  are trying to dismount.
        */

       if ( iop->pathinfo.node_access == node_access ) {
          result = 1;
          break;
       }
    }
  }

  epos_libio_unlock();

  return result;
}
