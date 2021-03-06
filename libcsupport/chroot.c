/**
 * @file
 *
 * @ingroup libcsupport
 *
 * @brief chroot() -  Change Root Directory
 */

/*
 *  chroot() -  Change Root Directory
 *  Author: fernando.ruiz@ctv.es
 *
 *  COPYRIGHT (c) 1989-2008.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: chroot.c,v 1.9 2009/06/12 01:53:32 ccj Exp $
 */

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <rtems.h>

#include <unistd.h>
#include <errno.h>

#include <rtems/libio_.h>
#include <rtems/seterr.h>

int chroot(
  const char *pathname
)
{
  int                               result;
  epos_filesystem_location_info_t  loc;

  /* an automatic call to new private env the first time */
  if (epos_current_user_env == &epos_global_user_env) {
   epos_libio_set_private_env(); /* try to set a new private env*/
   if (epos_current_user_env == &epos_global_user_env) /* not ok */
    epos_set_errno_and_return_minus_one( ENOTSUP );
  }

  result = chdir(pathname);
  if (result) {
    epos_set_errno_and_return_minus_one( errno );
  }
 
  /* clone the new root location */
  if (epos_filesystem_evaluate_path(".", 1, 0, &loc, 0)) {
    /* our cwd has changed, though - but there is no easy way of return :-( */
    epos_set_errno_and_return_minus_one( errno );
  }
  epos_filesystem_freenode(&epos_filesystem_root);
  epos_filesystem_root = loc;

  return 0;
}
