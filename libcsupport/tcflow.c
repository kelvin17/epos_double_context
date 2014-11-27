/**
 * @file
 *
 * @ingroup libcsupport
 *
 * @brief  tcflow() - POSIX 1003.1b 7.2.2 - Line Control Functions
 */

/*
 *  tcflow() - POSIX 1003.1b 7.2.2 - Line Control Functions
 *
 *  COPYRIGHT (c) 1989-1999.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: tcflow.c,v 1.8 2009/11/29 13:35:32 ralf Exp $
 */
#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <rtems.h>
#if defined(RTEMS_NEWLIB)
#include <termios.h>
#include <unistd.h>

int tcflow (
  int fd ,
  int action )
{
  return 0;
}

#endif
