/*  Task_3
 *
 *  This routine serves as a test task.  It simply obtains semaphore
 *  2 and waits forever attempting to obtain semaphore 3.
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

epos_task Task_3(
  epos_task_argument argument
)
{
  epos_status_code status;

  printk( "TA3 - epos_semaphore_obtain - wait forever on SM2" );
  status = epos_semaphore_obtain(
    Semaphore_id[ 2 ],
    RTEMS_DEFAULT_OPTIONS,
    RTEMS_NO_TIMEOUT
  );
  directive_failed( status, "epos_semaphore_obtain on SM2" );
  printk( "TA3 - got SM2" );

  printk( "TA3 - epos_semaphore_release - release SM2" );
  status = epos_semaphore_release( Semaphore_id[ 2 ] );
  directive_failed( status, "epos_semaphore_release on SM2" );

  printk( "TA3 - epos_semaphore_obtain - wait forever on SM3" );
  status = epos_semaphore_obtain(
    Semaphore_id[ 3 ],
    RTEMS_DEFAULT_OPTIONS,
    RTEMS_NO_TIMEOUT
  );
  directive_failed( status, "epos_semaphore_obtain on SM3" );
}
