/**
 * @file
 *
 * @ingroup libcsupport
 *
 * @brief 
 */

/*
 *  COPYRIGHT (c) 1989-2007.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 * $Id: printk_plugin.c,v 1.5 2009/11/29 13:35:32 ralf Exp $
 */

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdarg.h>
#include <rtems/bspIo.h>

int printk_plugin(
  void *ignored ,
  const char *format,
  ...
)
{
  va_list arg_pointer;

  va_start (arg_pointer, format);

  vprintk( format, arg_pointer );

  va_end(arg_pointer); /* clean up when done */

  return 0;
}

