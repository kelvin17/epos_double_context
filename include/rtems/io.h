/**
 * @file
 *
 * @ingroup ClassicIO
 *
 * @brief Classic Input/Output Manager API.
 */

/*
 *  COPYRIGHT (c) 1989-2008.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: io.h,v 1.30 2009/11/29 13:51:52 ralf Exp $
 */

#ifndef _RTEMS_IO_H
#define _RTEMS_IO_H

#ifndef SAPI_IO_EXTERN
#define SAPI_IO_EXTERN extern
#endif

#include <rtems/rtems/status.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup ClassicIO Input/Output
 *
 * @ingroup ClassicRTEMS
 *
 *  @brief RTEMS I/O管理服务模块。
 *
 * @{
 */

typedef uint32_t epos_device_major_number;

typedef uint32_t epos_device_minor_number;

typedef epos_status_code epos_device_driver;

typedef epos_device_driver (*epos_device_driver_entry)(
  epos_device_major_number,
  epos_device_minor_number,
  void *
);

typedef struct {
  epos_device_driver_entry initialization_entry;
  epos_device_driver_entry open_entry;
  epos_device_driver_entry close_entry;
  epos_device_driver_entry read_entry;
  epos_device_driver_entry write_entry;
  epos_device_driver_entry control_entry;
} epos_driver_address_table;

/**
 * @name Device Driver Maintainance
 *
 * @{
 */

/**
 * @brief Returns @c RTEMS_IO_ERROR.
 *
 * @retval RTEMS_IO_ERROR Only this one.
 */
epos_status_code epos_io_driver_io_error(
  epos_device_major_number major,
  epos_device_minor_number minor,
  void *arg
);

/**
 * @brief Registers and initializes the device with the device driver table
 * @a driver_table and major number @a major.
 *
 * If the major number equals zero a major number will be obtained.  The major
 * number of the registered driver will be returned in @a registered_major.
 *
 * After a successful registration epos_io_initialize() will be called to
 * initialize the device.
 *
 * @retval RTEMS_SUCCESSFUL Device successfully registered and initialized.
 * @retval RTEMS_INVALID_ADDRESS Pointer to driver table or to registered
 * major number are invalid.  Device driver table is empty.
 * @retval RTEMS_INVALID_NUMBER Invalid major number.
 * @retval RTEMS_TOO_MANY No major number available.
 * @retval RTEMS_RESOURCE_IN_USE Major number in use.
 * @retval RTEMS_CALLED_FROM_ISR Called from interrupt context.
 * @retval * Status code depends on epos_io_initialize().
 */
epos_status_code epos_io_register_driver(
  epos_device_major_number major,
  const epos_driver_address_table *driver_table,
  epos_device_major_number *registered_major
);

/**
 * @brief Unregisters the device driver with number @a major.
 *
 * @retval RTEMS_SUCCESSFUL Device driver successfully unregistered.
 * @retval RTEMS_UNSATISFIED Invalid major number.
 * @retval RTEMS_CALLED_FROM_ISR Called from interrupt context.
 */
epos_status_code epos_io_unregister_driver(
  epos_device_major_number major
);

/**
 * @brief Registers the name @a device_name in the file system for the device
 * with number tuple @a major and @a minor.
 *
 * @retval RTEMS_SUCCESSFUL Name successfully registered.
 * @retval RTEMS_TOO_MANY Name already in use or other errors.
 */
epos_status_code epos_io_register_name(
  const char *device_name,
  epos_device_major_number major,
  epos_device_minor_number minor
);

/** @} */

/**
 * @name Device Driver Invocation
 *
 * @{
 */

epos_status_code epos_io_initialize(
  epos_device_major_number  major,
  epos_device_minor_number  minor,
  void                      *argument
);

epos_status_code epos_io_open(
  epos_device_major_number  major,
  epos_device_minor_number  minor,
  void                      *argument
);

epos_status_code epos_io_close(
  epos_device_major_number  major,
  epos_device_minor_number  minor,
  void                      *argument
);

epos_status_code epos_io_read(
  epos_device_major_number  major,
  epos_device_minor_number  minor,
  void                      *argument
);

epos_status_code epos_io_write(
  epos_device_major_number  major,
  epos_device_minor_number  minor,
  void                      *argument
);

epos_status_code epos_io_control(
  epos_device_major_number  major,
  epos_device_minor_number  minor,
  void                      *argument
);

/** @} */

/** @} */

typedef struct {
    char                     *device_name;
    size_t                    device_name_length;
    epos_device_major_number major;
    epos_device_minor_number minor;
} epos_driver_name_t;

/**
 * @deprecated Use stat() instead.
 */
epos_status_code epos_io_lookup_name(
    const char           *name,
    epos_driver_name_t  *device_info
) RTEMS_COMPILER_DEPRECATED_ATTRIBUTE;

SAPI_IO_EXTERN uint32_t _IO_Number_of_drivers;

SAPI_IO_EXTERN epos_driver_address_table *_IO_Driver_address_table;

void _IO_Manager_initialization( void );

void _IO_Initialize_all_drivers( void );

#ifdef __cplusplus
}
#endif

#endif
/* end of include file */
