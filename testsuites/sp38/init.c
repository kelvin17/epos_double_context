/*
 *  Classic API Signal to Task from ISR
 *
 *  COPYRIGHT (c) 1989-2007.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id$
 */

#define CONFIGURE_INIT
#include "system.h"

volatile bool signal_sent;
volatile bool signal_processed;

epos_id main_task;

void signal_handler(
  epos_signal_set signals
)
{
  signal_processed = TRUE;
}

epos_timer_service_routine test_signal_from_isr(
  epos_id  timer,
  void     *arg
)
{
  epos_status_code     status;

  status = epos_signal_send( main_task, 0x0a0b0c0d );
  directive_failed_with_level( status, "epos_signal_send", 1 );

  signal_sent = TRUE;
}

epos_task Init(
  epos_task_argument argument
)
{
  epos_status_code     status;
  epos_id              timer;
  epos_interval        start;
  epos_interval        now;

  printk( "\n\n*** TEST 38 ***" );

  main_task = epos_task_self();

  /*
   *  Timer used in multiple ways
   */
  status = epos_timer_create( 1, &timer );
  directive_failed( status, "epos_timer_create" );

  /*
   *  Get starting time
   */
  status = epos_clock_get( RTEMS_CLOCK_GET_TICKS_SINCE_BOOT, &start );
  directive_failed( status, "epos_clock_get start" );

  status = epos_signal_catch( signal_handler, RTEMS_DEFAULT_MODES );
  directive_failed( status, "epos_signal_catch" );
  printk( "epos_signal_catch - handler installed" );

  /*
   * Test Signal from ISR
   */
  signal_sent = FALSE;

  status = epos_timer_fire_after( timer, 10, test_signal_from_isr, NULL );
  directive_failed( status, "timer_fire_after failed" );

  while (1) {
    status = epos_clock_get( RTEMS_CLOCK_GET_TICKS_SINCE_BOOT, &now );
    directive_failed( status, "epos_clock_get now" );
    if ( (now-start) > 100 ) {
      printk( "Signal from ISR did not get processed\n" );
      epos_test_exit( 0 );
    }
    if ( signal_processed )
      break;
  }

  printk( "Signal sent from ISR has been processed" ); 
  printk( "*** END OF TEST 38 ***" );
  epos_test_exit( 0 );
}
