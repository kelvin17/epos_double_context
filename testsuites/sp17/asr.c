/*  Process_asr
 *
 *  This routine performs the processing for task 1's RTEMS_ASR.  It is called
 *  by an assembly routine which saves the necessary registers.
 *
 *  Input parameters:
 *    signal - signal set
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

epos_asr Process_asr(
  epos_signal_set signal_set
)
{
  epos_status_code status;

  status = epos_task_resume( Task_id[ 2 ] );
  directive_failed( status, "RTEMS_ASR - epos_task_resume of TA2" );
}
