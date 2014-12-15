/*
 *  Signal Manager
 *
 *
 *  COPYRIGHT (c) 1989-1999.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: signal.c,v 1.16 2005/01/18 09:03:44 ralf Exp $
 */

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <rtems/system.h>
#include <rtems/rtems/status.h>
#include <rtems/rtems/asr.h>
#include <rtems/score/isr.h>
#include <rtems/rtems/modes.h>
#include <rtems/rtems/signal.h>
#include <rtems/score/thread.h>
#include <rtems/rtems/tasks.h>

/*PAGE
 *
 *  _Signal_Manager_initialization
 *
 *  This routine initializes all signal manager related data structures.
 *
 *  Input parameters:   NONE
 *
 *  Output parameters:  NONE
 */

void _Signal_Manager_initialization( void )
{
}
