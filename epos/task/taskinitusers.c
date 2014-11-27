/*
 *  RTEMS Task Manager
 *
 *
 *  COPYRIGHT (c) 1989-2008.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: taskinitusers.c,v 1.10 2009/07/04 19:45:18 joel Exp $
 */

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <rtems/system.h>
#include <rtems/config.h>
#include <rtems/rtems/status.h>
#include <rtems/rtems/support.h>
#include <rtems/rtems/modes.h>
#include <rtems/score/object.h>
#include <rtems/score/stack.h>
#include <rtems/score/states.h>
#include <rtems/rtems/tasks.h>
#include <rtems/score/thread.h>
#include <rtems/score/threadq.h>
#include <rtems/score/tod.h>
#include <rtems/score/userext.h>
#include <rtems/score/wkspace.h>
#include <rtems/score/apiext.h>
#include <rtems/score/sysstate.h>
#include <rtems/score/apimutex.h>


/*PAGE
 *
 *  _RTEMS_tasks_Initialize_user_tasks_body
 *
 *  This routine creates and starts all configured user
 *  initialzation threads.
 *
 *  Input parameters: NONE
 *
 *  Output parameters:  NONE
 */

void _RTEMS_tasks_Initialize_user_tasks_body( void )
{
  uint32_t                          index;
  uint32_t                          maximum;
  epos_id                          id;
  epos_status_code                 return_value;
  epos_initialization_tasks_table *user_tasks;

  /*
   *  Move information into local variables
   */
  user_tasks = Configuration_RTEMS_API.User_initialization_tasks_table;
  maximum    = Configuration_RTEMS_API.number_of_initialization_tasks;

  /*
   *  Verify that we have a set of user tasks to iterate
   */
  if ( !user_tasks )
    return;

  /*
   *  Now iterate over the initialization tasks and create/start them.
   */
  for ( index=0 ; index < maximum ; index++ ) {
    return_value = epos_task_create(
      user_tasks[ index ].name,
      user_tasks[ index ].initial_priority,
      user_tasks[ index ].stack_size,
      user_tasks[ index ].mode_set,
      user_tasks[ index ].attribute_set,
      &id
    );
    if ( !epos_is_status_successful( return_value ) )
      _Internal_error_Occurred( INTERNAL_ERROR_RTEMS_API, true, return_value );

    return_value = epos_task_start(
      id,
      user_tasks[ index ].entry_point,
      user_tasks[ index ].argument
    );
    if ( !epos_is_status_successful( return_value ) )
      _Internal_error_Occurred( INTERNAL_ERROR_RTEMS_API, true, return_value );
  }
}

