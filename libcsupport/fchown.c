/**
 * @file
 *
 * @ingroup libcsupport
 *
 * @brief fchown() - POSIX 1003.1b 5.6.5 - Change Owner and Group of a File
 *
 *  COPYRIGHT (c) 1989-1999.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: fchown.c,v 1.1 2008/04/11 22:57:54 ccj Exp $
 */
#if HAVE_CONFIG_H
#include "config.h"
#endif
		  
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>
#include <rtems.h>
#include <rtems/libio.h>
#include <rtems/libio_.h>
#include <rtems/seterr.h>

int fchown(
  int   fd,
  uid_t owner,
  gid_t group
)
{
	errno = ENOSYS;
	return -1;
}

