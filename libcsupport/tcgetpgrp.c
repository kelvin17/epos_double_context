/**
 * @file
 *
 * @ingroup libcsupport
 *
 * @brief tcgetprgrp() - POSIX 1003.1b 7.2.3 - Get Foreground Process Group ID
 */

/*
 *  tcgetprgrp() - POSIX 1003.1b 7.2.3 - Get Foreground Process Group ID
 *
 *  COPYRIGHT (c) 1989-1999.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: tcgetpgrp.c,v 1.1 2009/09/15 05:32:49 ralf Exp $
 */

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <rtems.h>
#if defined(RTEMS_NEWLIB) && !defined(HAVE_TCGETPGRP)

#include <sys/types.h>
#include <unistd.h>

pid_t tcgetpgrp(int fd )
{
  return getpid();
}

#endif
