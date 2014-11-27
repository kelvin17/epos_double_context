/*  devnull.h
 *
 *  Null device driver, derived from rtems' stub driver.
 *
 *  Author: Ralf Corsepius (corsepiu@faw.uni-ulm.de)
 *
 *  COPYRIGHT (c) 1989-2000.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: devnull.h,v 1.2 2003/09/04 18:54:19 joel Exp $
 */

#ifndef __NULL_DRIVER_h
#define __NULL_DRIVER_h

#ifdef __cplusplus
extern "C" {
#endif

#define DEVNULL_DRIVER_TABLE_ENTRY \
  { null_initialize, null_open, null_close, null_read, \
    null_write, null_control }

#define NULL_SUCCESSFUL RTEMS_SUCCESSFUL

epos_device_driver null_initialize(
  epos_device_major_number,
  epos_device_minor_number,
  void *
);

epos_device_driver null_open(
  epos_device_major_number,
  epos_device_minor_number,
  void *
);

epos_device_driver null_close(
  epos_device_major_number,
  epos_device_minor_number,
  void *
);

epos_device_driver null_read(
  epos_device_major_number,
  epos_device_minor_number,
  void *
);

epos_device_driver null_write(
  epos_device_major_number,
  epos_device_minor_number,
  void *
);

epos_device_driver null_control(
  epos_device_major_number,
  epos_device_minor_number,
  void *
);

#ifdef __cplusplus
}
#endif

#endif
/* end of include file */
