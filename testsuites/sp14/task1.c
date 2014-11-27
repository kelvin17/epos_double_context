/*  Task_1
 *
 *  This routine serves as a test task.  It establishes an RTEMS_ASR and
 *  sends signal to itself to determine if the RTEMS_ASR gets to execute.
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

epos_timer_service_routine Signal_3_to_task_1(
  epos_id  id,
  void     *pointer
)
{
  epos_status_code status;

  status = epos_signal_send( Task_id[ 1 ], RTEMS_SIGNAL_3 );
  directive_failed_with_level( status, "epos_signal_send of 3", 1 );

  Timer_got_this_id  = id;
  Timer_got_this_pointer = pointer;

  Signals_sent = TRUE;
}

epos_task Task_1(
  epos_task_argument argument
)
{
  epos_mode        previous_mode;
  epos_status_code status;

  printk( "TA1 - epos_signal_catch - RTEMS_INTERRUPT_LEVEL( 3 )" );
  status = epos_signal_catch( Process_asr, RTEMS_INTERRUPT_LEVEL(3) );
  directive_failed( status, "epos_signal_catch" );

  printk( "TA1 - epos_signal_send - RTEMS_SIGNAL_16 to self" );
  status = epos_signal_send( RTEMS_SELF, RTEMS_SIGNAL_16 );
  directive_failed( status, "epos_signal_send" );

  printk( "TA1 - epos_signal_send - RTEMS_SIGNAL_0 to self" );
  status = epos_signal_send( RTEMS_SELF, RTEMS_SIGNAL_0 );
  directive_failed( status, "epos_signal_send" );

  printk( "TA1 - epos_signal_catch - RTEMS_NO_ASR" );
  status = epos_signal_catch( Process_asr, RTEMS_NO_ASR );
  directive_failed( status, "epos_signal_catch" );

epos_test_pause();

  printk( "TA1 - epos_signal_send - RTEMS_SIGNAL_1 to self" );
  status = epos_signal_send( RTEMS_SELF, RTEMS_SIGNAL_1 );
  directive_failed( status, "epos_signal_send" );

  printk( "TA1 - epos_task_mode - disable ASRs" );
  status = epos_task_mode( RTEMS_NO_ASR, RTEMS_ASR_MASK, &previous_mode );
  directive_failed( status, "epos_task_mode" );

  Timer_got_this_id = 0;
  Timer_got_this_pointer = NULL;

  printk( "TA1 - sending signal to RTEMS_SELF from timer" );
  status = epos_timer_fire_after(
    Timer_id[ 1 ],
    TICKS_PER_SECOND / 2,
    Signal_3_to_task_1,
    (void *) Task_1
  );
  directive_failed( status, "epos_timer_fire_after" );

  printk( "TA1 - waiting for signal to arrive" );

  Signals_sent = FALSE;
  Asr_fired    = FALSE;

  while ( Signals_sent == FALSE )
    ;

  if ( Timer_got_this_id == Timer_id[ 1 ] &&
       Timer_got_this_pointer == Task_1 )
    printk( "TA1 - timer routine got the correct arguments" );
  else
    printk(
      "TA1 - timer got (0x%x, %p) instead of (0x%x, %p)!!!!\n",
      Timer_got_this_id,
      Timer_got_this_pointer,
      Timer_id[ 1 ],
      Task_1
    );

  printk( "TA1 - epos_task_mode - enable ASRs" );
  status = epos_task_mode( RTEMS_ASR, RTEMS_ASR_MASK, &previous_mode );
  directive_failed( status, "epos_task_mode" );

  printk( "TA1 - epos_signal_catch - asraddr of NULL" );
  status = epos_signal_catch( NULL, RTEMS_DEFAULT_MODES );
  directive_failed( status, "epos_signal_catch" );

  printk( "TA1 - epos_task_delete - delete self" );
  status = epos_task_delete( RTEMS_SELF );
  directive_failed( status, "epos_task_delete of RTEMS_SELF" );
}
