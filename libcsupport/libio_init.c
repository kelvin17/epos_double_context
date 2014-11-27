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
 *  $Id: libio_init.c,v 1.5 2010/05/14 04:04:25 ccj Exp $
 */
#if HAVE_CONFIG_H
#include "config.h"
#endif


#include <rtems/libio_.h>               /* libio_.h pulls in rtems */
#include <rtems.h>
#include <rtems/assoc.h>                /* assoc.h not included by rtems.h */

#include <stdio.h>                      /* O_RDONLY, et.al. */
#include <fcntl.h>						/* O_RDONLY, et.al. */  //modbycxl: original: "fcntl.h"
#include <assert.h>

#include <errno.h>
#include <string.h>                     /* strcmp */
#include <unistd.h>
#include <stdlib.h>                     /* calloc() */

#include <rtems/libio.h>                /* libio.h not pulled in by rtems */

/*
 *  File descriptor Table Information
 */

epos_id           epos_libio_semaphore;
epos_libio_t     *epos_libio_iops;
epos_libio_t     *epos_libio_iop_freelist;

/*
 *  epos_libio_init
 *
 *  Called by BSP startup code to initialize the libio subsystem.
 */

void epos_libio_init( void )
{
    epos_status_code rc;
    uint32_t i;
    epos_libio_t *iop;

    if (epos_libio_number_iops > 0)
    {
        epos_libio_iops = (epos_libio_t *) calloc(epos_libio_number_iops,
                                                    sizeof(epos_libio_t));
        if (epos_libio_iops == NULL)
            epos_fatal_error_occurred(RTEMS_NO_MEMORY);

        iop = epos_libio_iop_freelist = epos_libio_iops;
        for (i = 0 ; (i + 1) < epos_libio_number_iops ; i++, iop++)
          iop->data1 = iop + 1;
        iop->data1 = NULL;
    }

  /*
   *  Create the binary semaphore used to provide mutual exclusion
   *  on the IOP Table.
   */

  rc = epos_semaphore_create(
    RTEMS_LIBIO_SEM,
    1,
    RTEMS_BINARY_SEMAPHORE | RTEMS_INHERIT_PRIORITY | RTEMS_PRIORITY,
    RTEMS_NO_PRIORITY,
    &epos_libio_semaphore
  );
  if ( rc != RTEMS_SUCCESSFUL )
    epos_fatal_error_occurred( rc );

  /*
   *  Initialize the base file system infrastructure.
   */

  if (epos_fs_init_helper)
     (* epos_fs_init_helper)();
}
