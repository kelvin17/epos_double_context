/**
 * @file
 *
 * @ingroup libcsupport
 *
 * @brief   Implementation of hooks for the CYGNUS newlib libc
 *  These hooks set things up so that:
 *       + '_REENT' is switched at task switch time.
 *
 *  COPYRIGHT (c) 1994 by Division Incorporated
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: newlibc_init.c,v 1.3 2009/11/29 13:35:32 ralf Exp $
 *
 */

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <rtems.h>
#if defined(RTEMS_NEWLIB)

/*
 *  Init libc for CYGNUS newlib
 *
 *  Set up _REENT to use our global libc_global_reent.
 *  (newlib provides a global of its own, but we prefer our own name for it)
 *
 *  If reentrancy is desired (which it should be), then
 *  we install the task extension hooks to maintain the
 *  newlib reentrancy global variable _REENT on task
 *  create, delete, switch, exit, etc.
 *
 */


void
libc_init(void)
{
}

#endif
