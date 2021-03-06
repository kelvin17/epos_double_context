/**
 * @file
 *
 * @ingroup libcsupport
 *
 * @brief  __assert - small RTEMS Specific Implementation
 */

 /*  __assert - small RTEMS Specific Implementation
 *
 *  COPYRIGHT (c) 2007.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: __assert.c,v 1.7 2009/11/29 13:35:32 ralf Exp $
 */
#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <rtems/bspIo.h>
#include <rtems.h>

#if defined(RTEMS_NEWLIB) && !defined(HAVE___ASSERT_FUNC)
/*
 * Newlib 1.16.0 added this method.  Together these provide an
 * RTEMS safe, low memory implementation.
 */
void __assert_func(
  const char *file,
  int         line,
  const char *func,
  const char *failedexpr
)
{
/*temporary ,by zwj
  printk("assertion \"%s\" failed: file \"%s\", line %d%s%s\n",
    failedexpr,
    file,
    line,
    (func) ? ", function: " : "",
    (func) ? func : ""
  );
 */
  epos_fatal_error_occurred(0);
}
#endif

#if defined(RTEMS_NEWLIB) && !defined(HAVE___ASSERT)
void __assert(
  const char *file,
  int         line,
  const char *failedexpr
)
{
  __assert_func (file, line, NULL, failedexpr);
}
#endif
