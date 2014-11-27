/*  Task_1
 *
 *  This routine serves as a test task.  It tests the I/O manager.
 *
 *  Input parameters:
 *    argument - task argument
 *
 *  Output parameters:  NONE
 *
 *  COPYRIGHT (c) 1989-2002.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id$
 */

#include "system.h"

#define STUB_DRIVER_MAJOR     0x2
#define NO_DRIVER_MAJOR       0x3
#define INVALID_DRIVER_MAJOR  \
     (epos_configuration_get_number_of_device_drivers() + 1)

/* driver entries to use with io_register */
epos_driver_address_table GoodDriver = DEVNULL_DRIVER_TABLE_ENTRY;
epos_driver_address_table BadDriver_Nothing = {
  NULL, NULL, NULL, NULL, NULL, NULL
};

epos_task Task_1(
  epos_task_argument argument
)
{
  epos_status_code         status;
  epos_device_major_number registered;
  epos_device_major_number registered_worked;

  printk( "-----  TESTING THE NULL DRIVER CHECKS  -----" );

  status = epos_io_initialize( NO_DRIVER_MAJOR, 0, NULL );
  directive_failed( status, "epos_io_initialize" );
  printk( "TA1 - epos_io_initialize  - NULL DRIVER RTEMS_SUCCESSFUL" );

  status = epos_io_open( NO_DRIVER_MAJOR, 0, NULL );
  directive_failed( status, "epos_io_open" );
  printk( "TA1 - epos_io_open        - NULL DRIVER RTEMS_SUCCESSFUL" );

  status = epos_io_close( NO_DRIVER_MAJOR, 0, NULL );
  directive_failed( status, "epos_io_close" );
  printk( "TA1 - epos_io_close       - NULL DRIVER RTEMS_SUCCESSFUL" );

  status = epos_io_read( NO_DRIVER_MAJOR, 0, NULL );
  directive_failed( status, "epos_io_read" );
  printk( "TA1 - epos_io_read        - NULL DRIVER RTEMS_SUCCESSFUL" );

  status = epos_io_write( NO_DRIVER_MAJOR, 0, NULL );
  directive_failed( status, "epos_io_write" );
  printk( "TA1 - epos_io_write       - NULL DRIVER RTEMS_SUCCESSFUL" );

  status = epos_io_control( NO_DRIVER_MAJOR, 0, NULL );
  directive_failed( status, "epos_io_control" );
  printk( "TA1 - epos_io_control     - NULL DRIVER RTEMS_SUCCESSFUL" );

  printk( "-----  TESTING THE I/O MANAGER DIRECTIVES  -----" );

  status = epos_io_initialize( STUB_DRIVER_MAJOR, 0, NULL );
  directive_failed( status, "epos_io_initialize" );
  printk( "TA1 - epos_io_initialize  - STUB DRIVER RTEMS_SUCCESSFUL" );

  status = epos_io_open( STUB_DRIVER_MAJOR, 0, NULL );
  directive_failed( status, "epos_io_open" );
  printk( "TA1 - epos_io_open        - STUB DRIVER RTEMS_SUCCESSFUL" );

  status = epos_io_close( STUB_DRIVER_MAJOR, 0, NULL );
  directive_failed( status, "epos_io_close" );
  printk( "TA1 - epos_io_close       - STUB DRIVER RTEMS_SUCCESSFUL" );

  status = epos_io_read( STUB_DRIVER_MAJOR, 0, NULL );
  directive_failed( status, "epos_io_read" );
  printk( "TA1 - epos_io_read        - STUB DRIVER RTEMS_SUCCESSFUL" );

  status = epos_io_write( STUB_DRIVER_MAJOR, 0, NULL );
  directive_failed( status, "epos_io_write" );
  printk( "TA1 - epos_io_write       - STUB DRIVER RTEMS_SUCCESSFUL" );

  status = epos_io_control( STUB_DRIVER_MAJOR, 0, NULL );
  directive_failed( status, "epos_io_control" );
  printk( "TA1 - epos_io_control     - STUB DRIVER RTEMS_SUCCESSFUL" );


  /*
   *  Invalid major number
   */
  printk( "-----  RETURNING INVALID MAJOR NUMBER -----" );

  status = epos_io_initialize( INVALID_DRIVER_MAJOR, 0, NULL );
  fatal_directive_status(
    status,
    RTEMS_INVALID_NUMBER,
    "epos_io_initialize"
  );
  printk( "TA1 - epos_io_initialize  - RTEMS_INVALID_NUMBER" );

  status = epos_io_open( INVALID_DRIVER_MAJOR, 0, NULL );
  fatal_directive_status(
    status,
    RTEMS_INVALID_NUMBER,
    "epos_io_open"
  );
  printk( "TA1 - epos_io_open        - RTEMS_INVALID_NUMBER" );

  /*
   *  Exercise the io register driver directive
   */
  printk( "-----  EXERCISE DRIVER REGISTER -----" );

  status = epos_io_register_driver( 0, &GoodDriver, NULL );
  fatal_directive_status(
    status,
    RTEMS_INVALID_ADDRESS,
    "epos_io_register_driver"
  );
  printk(
   "TA1 - epos_io_register_driver - RTEMS_INVALID_ADDRESS - NULL registered"
  );

  status = epos_io_register_driver( 0, NULL, &registered );
  fatal_directive_status(
    status,
    RTEMS_INVALID_ADDRESS,
    "epos_io_register_driver"
  );
  printk(
    "TA1 - epos_io_register_driver - RTEMS_INVALID_ADDRESS - NULL device"
  );

  status = epos_io_register_driver( 0, &BadDriver_Nothing, &registered );
  fatal_directive_status(
    status,
    RTEMS_INVALID_ADDRESS,
    "epos_io_register_driver"
  );
  printk(
    "TA1 - epos_io_register_driver - RTEMS_INVALID_ADDRESS - no callouts"
  );

  status = epos_io_register_driver(
    INVALID_DRIVER_MAJOR, &GoodDriver, &registered );
  fatal_directive_status(
    status,
    RTEMS_INVALID_NUMBER,
    "epos_io_register_driver"
  );
  printk( "TA1 - epos_io_register_driver - RTEMS_INVALID_NUMBER - bad major" );

  /* this has to succeed to overwrite NULL driver entry required above. */
  status = epos_io_register_driver( 0, &GoodDriver, &registered_worked );
  directive_failed( status, "epos_io_register_driver" );
  printk( "TA1 - epos_io_register_driver - RTEMS_SUCCESSFUL - overwrite NULL" );

  status = epos_io_register_driver( 0, &GoodDriver, &registered );
  if ( status == RTEMS_SUCCESSFUL )
    printk( "registered major = %d\n", registered );
  fatal_directive_status(
    status,
    RTEMS_TOO_MANY,
    "epos_io_register_driver"
  );
  printk( "TA1 - epos_io_register_driver - RTEMS_TOO_MANY - no slots" );

  /* there should be a driver at major 1 -- clock, console, or stub */
  status = epos_io_register_driver( 1, &GoodDriver, &registered );
  fatal_directive_status(
    status,
    RTEMS_RESOURCE_IN_USE,
    "epos_io_register_driver"
  );
  printk(
    "TA1 - epos_io_register_driver - RTEMS_RESOURCE_IN_USE - major in use"
  );

  /* this should unregister the device driver we registered above */
  status = epos_io_unregister_driver( registered_worked );
  directive_failed( status, "epos_io_unregister_driver" );
  printk( "TA1 - epos_io_unregister_driver - RTEMS_SUCCESSFUL" );

  printk( "*** END OF TEST 21 ***" );
  epos_test_exit( 0 );
}
