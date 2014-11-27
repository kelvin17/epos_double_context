/* spmonotonic -- sanity check the rate monotonic manager
 *
 * license and distribution terms for this file may be found in the file
 * LICENSE in this distribution or at
 * http://www.rtems.com/license/LICENSE .
 *
 * $Id$
 */

#define CONFIGURE_INIT
#include <tmacros.h>  /* includes bsp.h, stdio, etc... */

/* prototype */
epos_task Init (epos_task_argument ignored);

#define CONFIGURE_RTEMS_INIT_TASKS_TABLE

//#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER
#define CONFIGURE_MAXIMUM_TASKS     5
#define CONFIGURE_MAXIMUM_BARRIERS  1

#define CONFIGURE_INIT

epos_id    Barrier;

int SuccessfulCase;
int DeletedCase;

epos_task Waiter(
  epos_task_argument number
)
{
  epos_status_code status;

  printk( "Waiter %d waiting on barrier\n", number );
  status = epos_barrier_wait( Barrier, RTEMS_NO_TIMEOUT );

  printk( "Waiter %d back from barrier\n", number );

  if ( SuccessfulCase == TRUE ) {
    directive_failed(status, "epos_barrier_wait");
  } else if ( DeletedCase == TRUE ) {
    fatal_directive_status(
      status,
      RTEMS_OBJECT_WAS_DELETED,
      "epos_barrier_wait did not get deleted"
    );
  }

  epos_task_delete( RTEMS_SELF );
}

#include <rtems/confdefs.h>

epos_task Init(
  epos_task_argument ignored
)
{
  epos_status_code status;
  epos_name        name = epos_build_name('B','A','R','1');
  uint32_t          released;
  epos_id          testId;
  epos_id          Tasks[CONFIGURE_MAXIMUM_TASKS-1];
  uint32_t          i;

  printk("\n\n*** TEST 33 ***");

  /* Check bad argument cases */
  printk( "Delete barrier with bad id" );
  status = epos_barrier_delete( 100 );
  fatal_directive_status(
    status,
    RTEMS_INVALID_ID,
    "epos_barrier_delete did not return RTEMS_INVALID_ID"
  );

  printk( "Release barrier with bad id" );
  status = epos_barrier_release( 100, &released );
  fatal_directive_status(
    status,
    RTEMS_INVALID_ID,
    "epos_barrier_release did not return RTEMS_INVALID_ID"
  );

  printk( "Wait on barrier with bad id" );
  status = epos_barrier_wait( 100, 10 );
  fatal_directive_status(
    status,
    RTEMS_INVALID_ID,
    "epos_barrier_wait did not return RTEMS_INVALID_ID"
  );

  /* Create barrier with automatic release and 0 maximum waiters */
  printk( "Create barrier with automatic release and 0 max waiters" );
  status = epos_barrier_create(
    name, RTEMS_BARRIER_AUTOMATIC_RELEASE, 0, &Barrier
  );
  fatal_directive_status(
    status,
    RTEMS_INVALID_NUMBER,
    "epos_barrier_create did not return RTEMS_INVALID_NUMBER"
  );

  /* Create barrier */
  printk( "Create barrier" );
  status = epos_barrier_create( name, RTEMS_DEFAULT_ATTRIBUTES, 0, &Barrier );
  directive_failed(status, "epos_barrier_create");

  /* Check for creating too many */
  printk( "Create too many barriers" );
  status = epos_barrier_create( name, RTEMS_DEFAULT_ATTRIBUTES, 0, &Barrier );
  fatal_directive_status(
    status,
    RTEMS_TOO_MANY,
    "epos_barrier_create did not return RTEMS_TOO_MANY"
  );

  printk( "Check barrier ident" );
  status = epos_barrier_ident( name, &testId );
  directive_failed(status, "epos_barrier_ident");
  if ( testId != Barrier ) {
    printk( "ERROR -- epos_barrier_create -- did not get Id expected\n" );
    exit( 0 );
  }

  printk( "Wait on barrier w/timeout and TIMEOUT" );
  status = epos_barrier_wait( Barrier, 25 );
  fatal_directive_status(
    status,
    RTEMS_TIMEOUT,
    "epos_barrier_wait did not timeout"
  );

  /* Release no tasks */
  status = epos_barrier_release( Barrier, &released );
  directive_failed(status, "epos_barrier_release");
  if ( released != 0 ) {
    printk( "ERROR -- epos_barrier_release -- released != 0, = %d", released);
    exit(0);
  }

  /*  Create some tasks to wait for the barrier */
  SuccessfulCase = TRUE;
  DeletedCase    = FALSE;
  printk( "\n*** Testing manual release of barrier ***" );
  for (i=0 ; i<(CONFIGURE_MAXIMUM_TASKS-1) ; i++) {
    status = epos_task_create(
      epos_build_name('W','A','I','T'),
      1,
      RTEMS_MINIMUM_STACK_SIZE,
      RTEMS_DEFAULT_MODES,
      RTEMS_DEFAULT_ATTRIBUTES,
      &Tasks[ i ]
    );
    directive_failed( status, "epos_task_create of Waiter" );

    status = epos_task_start( Tasks[ i ], Waiter, i );
    directive_failed( status, "epos_task_start of Waiter" );
  }

  printk( "Delay to let Waiters block" ); 
  status = epos_task_wake_after( TICKS_PER_SECOND );
  directive_failed(status, "epos_task_wake_after");

  /* Release tasks which were waiting */
  printk( "Releasing tasks" );
  status = epos_barrier_release( Barrier, &released );
  directive_failed(status, "epos_barrier_release");
  if ( released != (CONFIGURE_MAXIMUM_TASKS-1) ) {
    printk( "ERROR -- epos_barrier_release -- released != %d, = %d",
         (CONFIGURE_MAXIMUM_TASKS-1), released);
    exit(0);
  }

  printk( "Delay to let Waiters print a message" ); 
  status = epos_task_wake_after( TICKS_PER_SECOND );
  directive_failed(status, "epos_task_wake_after");

  /*  Create some tasks to wait for the barrier */
  SuccessfulCase = FALSE;
  DeletedCase    = TRUE;
  printk( "\n*** Testing Deletion of barrier ***" );
  for (i=0 ; i<(CONFIGURE_MAXIMUM_TASKS-1) ; i++) {
    status = epos_task_create(
      epos_build_name('W','A','I','T'),
      1,
      RTEMS_MINIMUM_STACK_SIZE,
      RTEMS_DEFAULT_MODES,
      RTEMS_DEFAULT_ATTRIBUTES,
      &Tasks[ i ]
    );
    directive_failed( status, "epos_task_create of Waiter" );

    status = epos_task_start( Tasks[ i ], Waiter, i );
    directive_failed( status, "epos_task_start of Waiter" );
  }

  printk( "Delay to let Waiters block" ); 
  status = epos_task_wake_after( TICKS_PER_SECOND );
  directive_failed(status, "epos_task_wake_after");

  printk( "Delete barrier" );
  status = epos_barrier_delete( Barrier );
  directive_failed(status, "epos_barrier_delete");

  printk( "Delay to let Waiters print a message" ); 
  status = epos_task_wake_after( TICKS_PER_SECOND );
  directive_failed(status, "epos_task_wake_after");

  /* Create barrier with automatic release */
  printk( "Create barrier" );
  status = epos_barrier_create(
    name, RTEMS_BARRIER_AUTOMATIC_RELEASE, CONFIGURE_MAXIMUM_TASKS-1, &Barrier
  );
  directive_failed(status, "epos_barrier_create");

  /*  Create some tasks to wait for the barrier */
  SuccessfulCase = TRUE;
  DeletedCase    = FALSE;
  printk( "\n*** Testing automatic release of barrier ***" );
  for (i=0 ; i<(CONFIGURE_MAXIMUM_TASKS-1) ; i++) {
    status = epos_task_create(
      epos_build_name('W','A','I','T'),
      1,
      RTEMS_MINIMUM_STACK_SIZE,
      RTEMS_DEFAULT_MODES,
      RTEMS_DEFAULT_ATTRIBUTES,
      &Tasks[ i ]
    );
    directive_failed( status, "epos_task_create of Waiter" );

    status = epos_task_start( Tasks[ i ], Waiter, i );
    directive_failed( status, "epos_task_start of Waiter" );
  }

  printk( "Delay to let task wait on barrier" ); 
  status = epos_task_wake_after( TICKS_PER_SECOND );
  directive_failed(status, "epos_task_wake_after");

  /* the end */
  printk("*** END OF TEST 33 ***");
  epos_test_exit(0);
}
