/*  Task5
 *
 *  This routine serves as a test task.  It obtains semaphore 1 correctly
 *  once, then waits for semaphore 1 again.  Task 1 should delete the
 *  semaphore, thus waking this task up.
 *
 *  Input parameters:
 *    argument - task argument
 *
 *  Output parameters:  NONE
 *
 *  COPYRIGHT (c) 1989-1999.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id$
 */

#include "system.h"

epos_task Task5(
  epos_task_argument argument
)
{
  epos_status_code status;

  printk( "TA5 - epos_semaphore_obtain - wait forever on SM1" );
  status = epos_semaphore_obtain(
    Semaphore_id[ 1 ],
    RTEMS_DEFAULT_OPTIONS,
    RTEMS_NO_TIMEOUT
  );
  directive_failed( status, "epos_semaphore_obtain on SM1" );
  printk( "TA5 - got SM1" );

  printk( "TA5 - epos_semaphore_obtain - wait forever on SM1" );
  status = epos_semaphore_obtain(
    Semaphore_id[ 1 ],
    RTEMS_DEFAULT_OPTIONS,
    RTEMS_NO_TIMEOUT
  );
  fatal_directive_status(
    status,
    RTEMS_OBJECT_WAS_DELETED,
    "epos_semaphore_obtain on SM1"
  );
  printk( "TA5 - SM1 deleted by TA1" );

  printk( "*** END OF TEST 12 ***" );
  epos_test_exit( 0 );
}
