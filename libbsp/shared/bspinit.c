/*
 *  COPYRIGHT (c) 1989-2009.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: bspinit.c,v 1.3 2009/11/30 04:54:28 ralf Exp $
 */

#include <stdlib.h>
#include <string.h>

#include <bsp.h>
#include <bsp/bootcard.h>
#ifdef RTEMS_NETWORKING
#include "rtems/epos_bsdnet.h"
#endif

/*
 * This routine calls main from a confdefs.h default Init task
 * set up. The bootcard will provide the task argument as
 * command line string (ASCIIZ).
 */



//void Init (epos_task_argument arg)
//{}



