/*  Resume_task
 *
 *  This subprogram is scheduled as a timer service routine.  When
 *  it fires it resumes the task which is mapped to this timer.
 *
 *  Input parameters:  NONE
 *
 *  Output parameters:  NONE
 *
 *  COPYRIGHT (c) 1989-2008.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id$
 */

#include "system.h"

epos_timer_service_routine Resume_task(
  epos_id  timer_id,
  void     *ignored_address
)
{
  epos_id          task_to_resume;
  epos_status_code status;

  task_to_resume = Task_id[ epos_object_id_get_index( timer_id ) ];
  status = epos_task_resume( task_to_resume );
  directive_failed_with_level( status, "epos_task_resume", 1 );
}
