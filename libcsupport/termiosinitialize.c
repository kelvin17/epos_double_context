/**
 * @file
 *
 * @ingroup libcsupport
 *
 * @brief Termios initialization routine
 */

/*
 *  Termios initialization routine
 *
 *  Author:
 *    W. Eric Norum
 *    Saskatchewan Accelerator Laboratory
 *    University of Saskatchewan
 *    Saskatoon, Saskatchewan, CANADA
 *    eric@skatter.usask.ca
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: termiosinitialize.c,v 1.3 2003/09/04 18:54:13 joel Exp $
 */

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <rtems.h>
#include <rtems/libio.h>
#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

struct epos_termios_tty *epos_termios_ttyHead;
struct epos_termios_tty *epos_termios_ttyTail;
epos_id epos_termios_ttyMutex;

void
epos_termios_initialize (void)
{
  epos_status_code sc;

  /*
   * Create the mutex semaphore for the tty list
   */
  if (!epos_termios_ttyMutex) {
    sc = epos_semaphore_create (
      epos_build_name ('T', 'R', 'm', 'i'),
      1,
      RTEMS_BINARY_SEMAPHORE | RTEMS_INHERIT_PRIORITY | RTEMS_PRIORITY,
      RTEMS_NO_PRIORITY,
      &epos_termios_ttyMutex);
    if (sc != RTEMS_SUCCESSFUL)
      epos_fatal_error_occurred (sc);
  }
}
