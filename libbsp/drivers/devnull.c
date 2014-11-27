/*  /dev/null
 *
 *  Derived from rtems' stub driver.
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
 *  devnull.c,v 1.3.4.2 2003/09/04 18:47:05 joel Exp
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <rtems.h>
#include <bsp/drivers/devnull.h>
#include <rtems/libio.h>

/*  null_initialize
 *
 *  This routine is the null device driver init routine.
 *
 *  Input parameters:
 *    major - device major number
 *    minor - device minor number
 *    pargp - pointer to parameter block
 *
 *  Output parameters:
 *    rval       - NULL_SUCCESSFUL
 */

uint32_t NULL_major;
static char initialized;

epos_device_driver null_initialize(
  epos_device_major_number major,
  epos_device_major_number minor,
  void *pargp
)
{
  epos_device_driver status;
  
  if ( !initialized ) {
    initialized = 1;

    status = epos_io_register_name(
      "/dev/null",
      major,
      (epos_device_minor_number) 0
    );

    if (status != RTEMS_SUCCESSFUL)
      epos_fatal_error_occurred(status);
      
    NULL_major = major;
  }
  
  return RTEMS_SUCCESSFUL;
}

/*  null_open
 *
 *  This routine is the null device driver open routine.
 *
 *  Input parameters:
 *    major - device major number
 *    minor - device minor number
 *    pargb - pointer to open parameter block
 *
 *  Output parameters:
 *    rval       - NULL_SUCCESSFUL
 */

epos_device_driver null_open(
  epos_device_major_number major,
  epos_device_major_number minor,
  void *pargp
)
{
  return NULL_SUCCESSFUL;
}


/*  null_close
 *
 *  This routine is the null device driver close routine.
 *
 *  Input parameters:
 *    major - device major number
 *    minor - device minor number
 *    pargb - pointer to close parameter block
 *
 *  Output parameters:
 *    rval       - NULL_SUCCESSFUL
 */

epos_device_driver null_close(
  epos_device_major_number major,
  epos_device_major_number minor,
  void *pargp
)
{
  return NULL_SUCCESSFUL;
}


/*  null_read
 *
 *  This routine is the null device driver read routine.
 *
 *  Input parameters:
 *    major - device major number
 *    minor - device minor number
 *    pargp - pointer to read parameter block
 *
 *  Output parameters:
 *    rval       - NULL_SUCCESSFUL
 */

epos_device_driver null_read(
  epos_device_major_number major,
  epos_device_major_number minor,
  void *pargp
)
{
  return NULL_SUCCESSFUL;
}


/*  null_write
 *
 *  This routine is the null device driver write routine.
 *
 *  Input parameters:
 *    major - device major number
 *    minor - device minor number
 *    pargp - pointer to write parameter block
 *
 *  Output parameters:
 *    rval       - NULL_SUCCESSFUL
 */

epos_device_driver null_write(
  epos_device_major_number major,
  epos_device_major_number minor,
  void *pargp
)
{
  epos_libio_rw_args_t *rw_args = (epos_libio_rw_args_t *) pargp;
  
  if ( rw_args )
    rw_args->bytes_moved = rw_args->count;
  
  return NULL_SUCCESSFUL;
}


/*  null_control
 *
 *  This routine is the null device driver control routine.
 *
 *  Input parameters:
 *    major - device major number
 *    minor - device minor number
 *    pargp - pointer to cntrl parameter block
 *
 *  Output parameters:
 *    rval       - NULL_SUCCESSFUL
 */

epos_device_driver null_control(
  epos_device_major_number major,
  epos_device_major_number minor,
  void *pargp
)
{
  return NULL_SUCCESSFUL;
}
