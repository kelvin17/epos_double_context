/*
 *  Exercise thread queue enqueue and dequeue priority
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

#include <stdio.h>
#include <stdlib.h>

#include <bsp.h>

#include "tmacros.h"

#define MAX_TASKS 20

/*
 * Carefully chosen to exercise threadq enqueue/dequeue priority logic.
 * Somewhat randomly sorted to ensure than if discipline is FIFO, run-time
 * behavior won't be the same when released.
 */
epos_task_priority Priorities[MAX_TASKS] = {
  37, 37, 37, 37,       /* backward - more 2-n */
  2, 2, 2, 2,           /* forward - multiple are on 2-n chain */
  4, 3,                 /* forward - search forward arbitrary */
  3, 3, 3, 3,           /* forward - more 2-n */
  38, 37,               /* backward - search backward arbitrary */
  34, 34, 34, 34,       /* backward - multple on 2-n chain */
};


epos_id   Semaphore;
epos_id   Task_id[ MAX_TASKS ];
epos_name Task_name[ MAX_TASKS ];

epos_task Locker_task(
  epos_task_argument unused
)
{
  epos_id          tid;
  uint32_t          task_index;
  epos_status_code status;

  status = epos_task_ident( RTEMS_SELF, RTEMS_SEARCH_ALL_NODES, &tid );
  directive_failed( status, "epos_task_ident" ); 

  task_index = task_number( tid ) - 1;

  status = epos_semaphore_obtain( Semaphore, RTEMS_DEFAULT_OPTIONS, 0 );
  directive_failed( status, "epos_semaphore_obtain" ); 

  put_name( Task_name[ task_index ], FALSE );
  printk( " - unblocked - OK" );

  (void) epos_task_delete( RTEMS_SELF );
}

void do_test(
  epos_attribute attr,
  bool            extract  /* TRUE if extract, not release */
)
{
  epos_status_code status;
  int               i;

  status = epos_semaphore_create(
    epos_build_name( 'S', 'E', 'M', '0' ),  /* name = SEM0 */
    0,                                       /* unlocked */
    RTEMS_BINARY_SEMAPHORE | attr,           /* mutex w/desired discipline */
    0,                                       /* IGNORED */
    &Semaphore
  );
  directive_failed( status, "epos_semaphore_create" ); 

  for (i=0 ; i< MAX_TASKS ; i++ ) {
    
    Task_name[ i ] = epos_build_name(
       'T',
       'A',
       '0' + (char)(i/10),
       '0' + (char)(i%10)
    );

    status = epos_task_create(
      Task_name[ i ],
      Priorities[ i ],
      RTEMS_MINIMUM_STACK_SIZE,
      RTEMS_DEFAULT_MODES,
      RTEMS_DEFAULT_ATTRIBUTES,
      &Task_id[ i ]
    );
    directive_failed( status, "epos_task_create" ); 

    status = epos_task_start( Task_id[ i ], Locker_task, i );
    directive_failed( status, "epos_task_start" ); 

    status = epos_task_wake_after( 10 );
    directive_failed( status, "epos_task_wake_after" ); 
  }

  for (i=0 ; i< MAX_TASKS ; i++ ) {
    if ( extract == FALSE ) {
      status = epos_semaphore_release( Semaphore );
      directive_failed( status, "epos_semaphore_release" ); 

      status = epos_task_wake_after( 100 );
      directive_failed( status, "epos_task_wake_after" ); 
    } else {
      status = epos_task_delete( Task_id[ i ]  );
      directive_failed( status, "epos_task_delete" ); 
    }
  }
  
  /* one extra release for the initial state */
  status = epos_semaphore_release( Semaphore );
  directive_failed( status, "epos_semaphore_release" ); 

  /* now delete the semaphore since no one is waiting and it is unlocked */
  status = epos_semaphore_delete( Semaphore );
  directive_failed( status, "epos_semaphore_delete" ); 
}

epos_task Init(
  epos_task_argument argument
)
{
  printk( "\n\n*** START OF TEST 40 ***" );

  if ( sizeof( Priorities ) / sizeof( epos_task_priority ) != MAX_TASKS ) {
    printk( "Priorities table does not have right number of entries" );
    exit( 0 );
  }

  printk( "Exercising blocking discipline w/extract in FIFO order " );
  do_test( RTEMS_FIFO, TRUE );

  printk( "Exercising blocking discipline w/unblock in FIFO order" );
  do_test( RTEMS_FIFO, FALSE );

  epos_test_pause_and_screen_number( 2 );

  printk( "Exercising blocking discipline w/extract in priority order " );
  do_test( RTEMS_PRIORITY, TRUE );

  printk( "Exercising blocking discipline w/unblock in priority order" );
  do_test( RTEMS_PRIORITY, FALSE );

  printk( "*** END OF TEST 42 ***" );
  exit(0);
}

/**************** START OF CONFIGURATION INFORMATION ****************/

#define CONFIGURE_INIT

//#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER

#define CONFIGURE_MAXIMUM_TASKS             MAX_TASKS+1
#define CONFIGURE_MAXIMUM_SEMAPHORES        1

#define CONFIGURE_RTEMS_INIT_TASKS_TABLE

#include <rtems/confdefs.h>

/****************  END OF CONFIGURATION INFORMATION  ****************/

