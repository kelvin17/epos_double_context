/**
 * @file
 *
 * @ingroup libcsupport
 *
 * @brief 
 */

/*
 *  COPYRIGHT (c) 1989-2008.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: getrusage.c,v 1.1 2008/11/12 15:36:49 joel Exp $
 */

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <sys/resource.h>
#include <errno.h>


#include <rtems.h>
#include <rtems/seterr.h>

int getrusage(int who, struct rusage *usage)
{
  struct timespec uptime;
  struct timeval  rtime;

  if ( !usage )
    epos_set_errno_and_return_minus_one( EFAULT );

  /*
   *  RTEMS only has a single process so there are no children.
   *  The single process has been running since the system
   *  was booted and since there is no distinction between system
   *  and user time, we will just report the uptime.
   */
  if (who == RUSAGE_SELF) {
    epos_clock_get_uptime( &uptime );

    rtime.tv_sec  = uptime.tv_sec;
    rtime.tv_usec = uptime.tv_nsec / 1000;

    usage->ru_utime = rtime;
    usage->ru_stime = rtime;

    return 0;
  } else if (who == RUSAGE_CHILDREN) {
    epos_set_errno_and_return_minus_one( ENOSYS );
  }
  epos_set_errno_and_return_minus_one( EINVAL );
}

