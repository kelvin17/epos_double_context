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
 *  $Id: putk.c,v 1.2 2009/11/29 13:35:32 ralf Exp $
 */

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <rtems/bspIo.h>

/*
 * putk
 *
 * Kernel putk (e.g. puts) function requiring minimal infrastrure.
 */
void putk(const char *s)
{
  const char *p;


  for (p = s ; *p ; p++ )
    BSP_output_char(p);
  BSP_output_char("\n");
}
