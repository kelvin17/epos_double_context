/*
 *  Classic API Signal to Task from ISR
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

#define CONFIGURE_INIT
#define __RTEMS_VIOLATE_KERNEL_VISIBILITY__ 1
#include "system.h"

volatile bool case_hit;

epos_id main_task;
epos_id Semaphore;

Thread_blocking_operation_States getState(void)
{
  Objects_Locations  location;
  Semaphore_Control *sem;

  sem = (Semaphore_Control *)_Objects_Get(
    &_Semaphore_Information, Semaphore, &location ); 
  if ( location != OBJECTS_LOCAL ) {
    printk( "Bad object lookup" );
    epos_test_exit(0);
  }
  _Thread_Unnest_dispatch();

  return sem->Core_control.semaphore.Wait_queue.sync_state;
}

epos_timer_service_routine test_release_from_isr(
  epos_id  timer,
  void     *arg
)
{
  epos_status_code     status;

  if ( getState() == THREAD_BLOCKING_OPERATION_NOTHING_HAPPENED ) {
    case_hit = TRUE;
  }
  status = epos_semaphore_release( Semaphore );
}

epos_timer_service_routine test_release_with_timeout_from_isr(
  epos_id  timer,
  void     *arg
)
{
  epos_status_code     status;

  if ( getState() == THREAD_BLOCKING_OPERATION_NOTHING_HAPPENED ) {
    /*
     *  We want to catch the task while it is blocking.  Otherwise
     *  just send and make it happy.
     */
    case_hit = TRUE;
  }
  status = epos_semaphore_release( Semaphore );
}

epos_task Init(
  epos_task_argument argument
)
{
  epos_status_code     status;
  epos_id              timer;
  int                   i;
  int                   max;
  int                   iterations = 0;

  printk( "\n\n*** TEST 41 ***" );

  main_task = epos_task_self();

  /*
   *  Timer used in multiple ways
   */
  status = epos_timer_create( 1, &timer );
  directive_failed( status, "epos_timer_create" );

  status = epos_semaphore_create(
    epos_build_name( 'S', 'M', '1', ' ' ),
    1,
    RTEMS_DEFAULT_ATTRIBUTES,
    RTEMS_NO_PRIORITY,
    &Semaphore
  );
  directive_failed( status, "epos_semaphore_create of SM1" );

  /*
   * Test semaphore release successful from ISR -- obtain is forever
   */
  case_hit = FALSE;
  iterations = 0;
  max = 1;

  while (1) {
    if ( case_hit )
      break;
    status = epos_timer_fire_after( timer, 1, test_release_from_isr, NULL );
    directive_failed( status, "timer_fire_after failed" );

    for (i=0 ; i<max ; i++ )
      if ( getState() == THREAD_BLOCKING_OPERATION_SATISFIED )
        break;

    status = epos_semaphore_obtain( Semaphore, RTEMS_DEFAULT_OPTIONS, 0 );
    directive_failed( status, "epos_semaphore_obtain" );
    if ( case_hit == TRUE )
      break;
    max += 2;

    /* with our clock tick, this is about 30 seconds */
    if ( ++iterations >= 4 * 1000 * 30)
      break;
  }

  status = epos_semaphore_release( Semaphore );
  directive_failed( status, "epos_semaphore_release" );
  printk(
    "Release from ISR hitting synchronization point has %soccurred\n",
    (( case_hit == TRUE ) ? "" : "NOT ")
  ); 

  /*
   * Test semaphore release successful from ISR -- obtain has timeout
   */
  case_hit = FALSE;
  iterations = 0;
  max = 1;

  while (1) {
    if ( case_hit )
      break;
    status = epos_timer_fire_after(
      timer, 1, test_release_with_timeout_from_isr, NULL );
    directive_failed( status, "timer_fire_after failed" );

    for (i=0 ; i<max ; i++ )
      if ( getState() == THREAD_BLOCKING_OPERATION_SATISFIED )
        break;

    status = epos_semaphore_obtain( Semaphore, RTEMS_DEFAULT_OPTIONS, 10 );
    directive_failed( status, "epos_semaphore_obtain" );
    if ( case_hit == TRUE )
      break;
    max += 2;

    /* with our clock tick, this is about 30 seconds */
    if ( ++iterations >= 4 * 1000 * 30)
      break;
  }

  printk(
    "Release from ISR (with timeout) hitting synchronization "
      "point has %soccurred\n",
    (( case_hit == TRUE ) ? "" : "NOT ")
  ); 

  /*
   *  Now try for a timeout case -- semaphore must not be available
   */
  iterations = 0;
  case_hit = FALSE;
  max = 1;

  printk(
    "Run multiple times in attempt to hit threadq timeout synchronization point"
  ); 
  while (1) {

    for (i=0 ; i<max ; i++ )
      if ( getState() == THREAD_BLOCKING_OPERATION_SATISFIED )
        break;

    status = epos_semaphore_obtain( Semaphore, RTEMS_DEFAULT_OPTIONS, 1 );
    fatal_directive_status( status, RTEMS_TIMEOUT, "epos_semaphore_obtain" );

    if ( ++max > 10240 )
      max = 0;

    /* with our clock tick, this is about 30 seconds */
    if ( ++iterations >= 4 * 1000 * 30)
      break;
  }

  printk( "*** END OF TEST 41 ***" );
  epos_test_exit( 0 );
}
