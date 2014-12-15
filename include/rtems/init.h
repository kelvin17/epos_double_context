/**
 * @file rtems/init.h
 *
 *
 *  This include file contains all the constants and structures associated
 *  with the Initialization Manager.  This manager is responsible for
 *  initializing RTEMS, creating and starting all configured initialization
 *  tasks, invoking the initialization routine for each user-supplied device
 *  driver, and initializing the optional multiprocessor layer.
 *
 *  This manager provides directives to:
 *
 *     + initialize the RTEMS executive
 *     + shutdown the RTEMS executive
 */

/*
 *  COPYRIGHT (c) 1989-2008.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: init.h,v 1.19 2009/11/29 13:51:52 ralf Exp $
 */

#ifndef _RTEMS_INIT_H
#define _RTEMS_INIT_H

#ifdef __cplusplus
extern "C" {
#endif

#include <rtems/config.h>
#include <rtems/rtems/types.h>
#include <rtems/rtems/intr.h>


/**
 *  @brief epos_initialize_data_structures
 *
 *  This routine implements the portion of the RTEMS initializatin process
 *  that involves initializing data structures to a state that scheduling
 *  can occur in a consistent manner.
 */
void epos_initialize_data_structures(void);

/**
 *  @brief epos_initialize_before_drivers
 *
 *  This routine implements the portion of RTEMS initialization that
 *  is done immediately before device drivers are initialized.
 */
void epos_initialize_before_drivers(void);

/**
 *  @brief epos_initialize_device_drivers
 *
 *  This routine implements the portion of RTEMS initialization that
 *  initializes all device drivers.
 */
void epos_initialize_device_drivers(void);

/**
 *  @brief epos_initialize_start_multitasking
 *
 *  This routine implements the early portion of epos_initialize_executive
 *  directive up to the pretasking hook. This directive is invoked at system
 *  startup to initialize the RTEMS multitasking environment.
 */
void epos_initialize_start_multitasking(void);

/**
 *  @brief epos_shutdown_executive
 *
 *  This routine implements the epos_shutdown_executive directive.  The
 *  invocation of this directive results in the RTEMS environment being
 *  shutdown and multitasking halted.  From the application's perspective,
 *  invocation of this directive results in the epos_initialize_executive
 *  directive exitting to the startup code which invoked it.
 */
void epos_shutdown_executive(
  uint32_t   result
);

#ifdef __cplusplus
}
#endif

#endif
/* end of include file */
