/**
 * @file
 *
 * @ingroup ClassicIO
 *
 * @brief Classic Input/Output Manager implementation.
 */

/*
 *  COPYRIGHT (c) 1989-2009.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  Copyright (c) 2009 embedded brains GmbH.
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: ioregisterdriver.c,v 1.9 2009/11/29 13:51:52 ralf Exp $
 */

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <rtems/system.h>
#include <rtems/io.h>
#include <rtems/rtems/intr.h>
#include <rtems/score/thread.h>

static inline bool epos_io_is_empty_table(
  const epos_driver_address_table *table
)
{
  return table->initialization_entry == NULL && table->open_entry == NULL;
}

static epos_status_code epos_io_obtain_major_number(
  epos_device_major_number *major
)
{
  epos_device_major_number n = _IO_Number_of_drivers;
  epos_device_major_number m = 0;

  /* major is error checked by caller */

  for ( m = 0; m < n; ++m ) {
    epos_driver_address_table *const table = _IO_Driver_address_table + m;

    if ( epos_io_is_empty_table( table ) )
      break;
  }

  /* Assigns invalid value in case of failure */
  *major = m;

  if ( m != n )
    return RTEMS_SUCCESSFUL;

  return RTEMS_TOO_MANY;
}

epos_status_code epos_io_register_driver(
  epos_device_major_number         major,
  const epos_driver_address_table *driver_table,
  epos_device_major_number        *registered_major
)
{
  epos_device_major_number major_limit = _IO_Number_of_drivers;

  if ( epos_interrupt_is_in_progress() )
    return RTEMS_CALLED_FROM_ISR;

  if ( registered_major == NULL )
    return RTEMS_INVALID_ADDRESS;

  /* Set it to an invalid value */
  *registered_major = major_limit;

  if ( driver_table == NULL )
    return RTEMS_INVALID_ADDRESS;

  if ( epos_io_is_empty_table( driver_table ) )
    return RTEMS_INVALID_ADDRESS;

  if ( major >= major_limit )
    return RTEMS_INVALID_NUMBER;

  _Thread_Disable_dispatch();

  if ( major == 0 ) {
    epos_status_code sc = epos_io_obtain_major_number( registered_major );

    if ( sc != RTEMS_SUCCESSFUL ) {
      _Thread_Enable_dispatch();
      return sc;
    }
    major = *registered_major;
  } else {
    epos_driver_address_table *const table = _IO_Driver_address_table + major;

    if ( !epos_io_is_empty_table( table ) ) {
      _Thread_Enable_dispatch();
      return RTEMS_RESOURCE_IN_USE;
    }

    *registered_major = major;
  }

  _IO_Driver_address_table [major] = *driver_table;

  _Thread_Enable_dispatch();

  return epos_io_initialize( major, 0, NULL );
}

