/*
 *  Interrupt Disable/Enable Tests
 *  Clock Tick from task level
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

#define __RTEMS_VIOLATE_KERNEL_VISIBILITY__
#define CONFIGURE_INIT
#include "system.h"

void test_interrupt_inline(void)
{
  epos_interrupt_level level;
  bool                  in_isr;

  printk( "interrupt is in progress (use body)" );
  in_isr = epos_interrupt_is_in_progress();
  if ( in_isr ) {
    printk( "interrupt reported to be is in progress (body)" );
    epos_test_exit( 0 );
  }

  printk( "interrupt disable (use inline)" );
  epos_interrupt_disable( level );

  printk( "interrupt flash (use inline)" );
  epos_interrupt_flash( level );

  printk( "interrupt enable (use inline)" );
  epos_interrupt_enable( level );
}

volatile int isr_in_progress_body;
volatile int isr_in_progress_inline;

void check_isr_in_progress_inline(void)
{
  bool in_isr;

  in_isr = epos_interrupt_is_in_progress();
  isr_in_progress_inline = ( in_isr ) ? 1 : 2;
}

/*
#undef epos_interrupt_disable
#undef epos_interrupt_enable
#undef epos_interrupt_flash
#undef epos_interrupt_is_in_progress
*/

epos_timer_service_routine test_isr_in_progress(
  epos_id  timer,
  void     *arg
)
{
  bool in_isr;

  check_isr_in_progress_inline();

  in_isr = epos_interrupt_is_in_progress();
  isr_in_progress_body = ( in_isr ) ? 1 : 2;
}

void check_isr_worked(
  char *s,
  int   result
)
{
  switch (result) {
    case -1:
      printk( "isr_in_progress(%s) timer did not fire\n", s );
      break;
    case 1:
      printk( "isr_in_progress(%s) from ISR -- OK\n", s );
      break;
    case 2:
      printk( "isr_in_progress(%s) from ISR -- returned bad value\n");
      epos_test_exit(0);
      break;
  }
}

volatile int blocked_task_status;
epos_id     blocked_task_id;

epos_task blocked_task(
  epos_task_argument argument
)
{
  epos_status_code     status;

  printk( "Blocking task... suspending self" );
  blocked_task_status = 1;
  status = epos_task_suspend( RTEMS_SELF );
  directive_failed( status, "epos_task_suspend" );

  blocked_task_status = 3;
  status = epos_task_delete( RTEMS_SELF );
  directive_failed( status, "epos_task_delete" );
}

/*
 *  Timer Service Routine
 *
 *  If we are in an ISR, then this is a normal clock tick.
 *  If we are not, then it is the test case.
 */
epos_timer_service_routine test_unblock_task(
  epos_id  timer,
  void     *arg
)
{
  bool              in_isr;
  epos_status_code status;

  in_isr = epos_interrupt_is_in_progress();
  status = epos_task_is_suspended( blocked_task_id );
  if ( in_isr ) {
    status = epos_timer_fire_after( timer, 1, test_unblock_task, NULL );
    directive_failed( status, "timer_fire_after failed" );
    return;
  }

  if ( (status != RTEMS_ALREADY_SUSPENDED) ) {
    status = epos_timer_fire_after( timer, 1, test_unblock_task, NULL );
    directive_failed( status, "timer_fire_after failed" );
    return;
  } 

  blocked_task_status = 2;
  _Thread_Disable_dispatch();
  status = epos_task_resume( blocked_task_id );
  _Thread_Unnest_dispatch();
  directive_failed( status, "epos_task_resume" );
}

epos_task Init(
  epos_task_argument argument
)
{
  epos_time_of_day     time;
  epos_status_code     status;
  epos_interrupt_level level;
  bool                  in_isr;
  epos_id              timer;
  int                   i;

  printk( "\n\n*** TEST 37 ***" );

  build_time( &time, 12, 31, 1988, 9, 0, 0, 0 );
  status = epos_clock_set( &time );
  directive_failed( status, "epos_clock_set" );

  /*
   *  Timer used in multiple ways
   */
  status = epos_timer_create( 1, &timer );
  directive_failed( status, "epos_timer_create" );

  /*
   *  Test clock tick from outside ISR
   */
  status = epos_clock_tick();
  directive_failed( status, "epos_clock_tick" );
  printk( "clock_tick from task level" );

  /*
   *  Now do a dispatch directly out of a clock tick that is
   *  called from a task.  We need to create a task that will
   *  block so we have one to unblock.  Then we schedule a TSR
   *  to run in the clock tick but it has to be careful to
   *  make sure it is not called from an ISR and that the
   *  dispatching critical section is managed properly to
   *  make the dispatch happen.
   */

  blocked_task_status = -1;

  status = epos_task_create(
    epos_build_name( 'T', 'A', '1', ' ' ),
    1,
    RTEMS_MINIMUM_STACK_SIZE,
    RTEMS_DEFAULT_MODES,
    RTEMS_DEFAULT_ATTRIBUTES,
    &blocked_task_id
  );
  directive_failed( status, "epos_task_create" );

  status = epos_task_start( blocked_task_id, blocked_task, 0 );
  directive_failed( status, "epos_task_start" );

  status = epos_task_wake_after( 10 );
  directive_failed( status, "epos_task_wake_after" );

  status = epos_timer_fire_after( timer, 1, test_unblock_task, NULL );
  directive_failed( status, "timer_fire_after failed" );

  /* we expect to be preempted from this call */
  for ( i=0 ; i<100 && blocked_task_status != 3 ; i++ ) {
    status = epos_clock_tick();
    directive_failed( status, "epos_clock_tick" );
  }
  switch ( blocked_task_status ) {
     case -1:
       printk( "clock_tick with task preempt -- task blocked, timer did not fire" );
       epos_test_exit(0);
       break;
     case 1:
       printk( "clock_tick with task preempt -- timer fired case 1" );
       epos_test_exit(0);
       break;
     case 2:
       printk( "clock_tick with task preempt -- timer fired case 2" );
       epos_test_exit(0);
       break;
     case 3:
       printk( "clock_tick from task level with preempt -- OK" );
       break;
  }

  /*
   *  Test interrupt inline versions
   */
  test_interrupt_inline();

  /*
   *  Test interrupt bodies
   */
  printk( "interrupt is in progress (use body)" );
  in_isr = epos_interrupt_is_in_progress();
  if ( in_isr ) {
    printk( "interrupt reported to be is in progress (body)" );
    epos_test_exit( 0 );
  }

  printk( "interrupt disable (use body)" );
  level = epos_interrupt_disable();

  printk( "interrupt disable (use body)" );
  level = epos_interrupt_disable();

  printk( "interrupt flash (use body)" );
  epos_interrupt_flash( level );

  printk( "interrupt enable (use body)" );
  epos_interrupt_enable( level );

  /*
   * Test ISR in progress from actual ISR
   */
  isr_in_progress_body   = -1;
  isr_in_progress_inline = -1;

  status = epos_timer_fire_after( timer, 10, test_isr_in_progress, NULL );
  directive_failed( status, "timer_fire_after failed" );

  status = epos_task_wake_after( 100 );
  directive_failed( status, "wake_after failed" );
  
  check_isr_worked( "inline", isr_in_progress_body );

  check_isr_worked( "body", isr_in_progress_body );


  printk( "*** END OF TEST 37 ***" );
  epos_test_exit( 0 );
}
