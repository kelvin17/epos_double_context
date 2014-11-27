/*
 *  COPYRIGHT (c) 1989-2008.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id$
 */

#include <tmacros.h>

epos_id   Timer_id[ 3 ];    /* array of timer ids */
epos_name Timer_name[ 3 ];  /* array of timer names */

volatile int TSR_fired;
volatile void *TSR_malloc_ptr;

epos_timer_service_routine Malloc_From_TSR(
  epos_id  ignored_id,
  void     *ignored_address
)
{
  epos_status_code  status;

  TSR_fired = 2;
  printk( "TSR: calling malloc" );
  TSR_malloc_ptr = malloc( 64 );

  printk( "TSR: calling free" );
  free( (void *) TSR_malloc_ptr );

  printk( "TSR: delaying with epos_task_wake_after" );
  status = epos_task_wake_after( TICKS_PER_SECOND / 2 );
  directive_failed( status, "epos_task_wake_after" );
}

epos_task Init(
  epos_task_argument argument
)
{
  epos_status_code  status;

  printk( "\n\n*** TEST 45 ***" );

  status = epos_timer_initiate_server(
    RTEMS_TIMER_SERVER_DEFAULT_PRIORITY,
    RTEMS_MINIMUM_STACK_SIZE,
    RTEMS_DEFAULT_ATTRIBUTES
  );
  directive_failed( status, "epos_timer_initiate_server" );

  /*
   * Initialize Timers
   */

  Timer_name[ 1 ] = epos_build_name( 'T', 'M', '1', ' ' );
  Timer_name[ 2 ] = epos_build_name( 'T', 'M', '2', ' ' );

  printk( "INIT - epos_timer_create - creating timer 1" );
  status = epos_timer_create( Timer_name[ 1 ], &Timer_id[ 1 ] );
  directive_failed( status, "epos_timer_create" );
  printk( "INIT - timer 1 has id (0x%x)\n", Timer_id[ 1 ] );

  printk( "INIT - epos_timer_create - creating timer 2" );
  status = epos_timer_create( Timer_name[ 2 ], &Timer_id[ 2 ] );
  directive_failed( status, "epos_timer_create" );
  printk( "INIT - timer 2 has id (0x%x)\n", Timer_id[ 2 ] );

  /*
   *  Schedule malloc TSR for 1 second from now
   */

  TSR_fired = 0;
  TSR_malloc_ptr = (void *) 0xa5a5a5;
  printk( "TA1 - epos_timer_server_fire_after - timer 1 in 1 seconds" );
  status = epos_timer_server_fire_after(
    Timer_id[ 1 ],
    1 * TICKS_PER_SECOND,
    Malloc_From_TSR,
    NULL
  );
  directive_failed( status, "epos_timer_server_fire_after" );

  printk( "TA1 - epos_task_wake_after - 2 second" );
  status = epos_task_wake_after( 2 * TICKS_PER_SECOND );
  directive_failed( status, "epos_task_wake_after" );

  if ( TSR_fired == 2 &&
       (TSR_malloc_ptr && TSR_malloc_ptr != (void *)0xa5a5a5) )
    printk( "TSR appears to have executed OK" );
  else {
    printk( "FAILURE ptr=%p TSR_fired=%d\n", TSR_malloc_ptr, TSR_fired );
    epos_test_exit( 0 );
  }

  /*
   *  Delete timer and exit test
   */
  printk( "TA1 - timer_deleting - timer 1" );
  status = epos_timer_delete( Timer_id[ 1 ] );
  directive_failed( status, "epos_timer_delete" );


  printk( "*** END OF TEST 45 *** " );
  epos_test_exit( 0 );
}

#define CONFIGURE_INIT
/* configuration information */

//#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER

/* Two Tasks: Init and Timer Server */
#define CONFIGURE_MAXIMUM_TASKS           2
#define CONFIGURE_MAXIMUM_TIMERS          2
#define CONFIGURE_INIT_TASK_STACK_SIZE    (RTEMS_MINIMUM_STACK_SIZE * 2)

#define CONFIGURE_RTEMS_INIT_TASKS_TABLE

#define CONFIGURE_EXTRA_TASK_STACKS       (1 * RTEMS_MINIMUM_STACK_SIZE)

#include <rtems/confdefs.h>

