/**
 * @file rtems/sptables.h
 *
 *  This include file contains the executive's pre-initialized tables
 *  used when in a single processor configuration.
 */

/*
 *  COPYRIGHT (c) 1989-1999.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: sptables.h,v 1.11 2009/11/29 13:51:52 ralf Exp $
 */

#ifndef _RTEMS_SPTABLES_H
#define _RTEMS_SPTABLES_H

#ifdef __cplusplus
extern "C" {
#endif

#include <rtems/config.h>
#ifdef RTEMS_DEBUG
#include <rtems/debug.h>
#endif
#include <rtems/fatal.h>
#include <rtems/init.h>
#include <rtems/io.h>

#include <rtems/score/sysstate.h>

#include <rtems/rtems/intr.h>
#include <rtems/rtems/clock.h>
#include <rtems/rtems/tasks.h>
#include <rtems/rtems/event.h>
#include <rtems/rtems/message.h>
#include <rtems/rtems/part.h>
#include <rtems/rtems/ratemon.h>
#include <rtems/rtems/region.h>
#include <rtems/rtems/sem.h>
#include <rtems/rtems/signal.h>
#include <rtems/rtems/timer.h>


#ifdef __cplusplus
}
#endif

#endif
