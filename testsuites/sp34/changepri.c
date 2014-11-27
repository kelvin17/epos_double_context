/*
 *  Test program to demonstrate reordering of threads on thread queues
 *  when their priority changes.
 *
 *  $Id$
 */

#include <bsp.h>
#include <stdio.h>

/********************************************************************/
/* define this to use the RTEMS 4.5 scheme for object names */
#define TEST_ON_RTEMS_45

/* define this to print the Id of the calling task */
//#define TEST_ON_TASK_ID

/********************************************************************/

#define CONFIGURE_INIT

#include <bsp.h>
#include <stdio.h>
#include "tmacros.h"

epos_task BlockingTasks(epos_task_argument arg);

/*
 *  CallerName -- print the calling tasks name or id as configured
 */
const char *CallerName()
{
  static char buffer[32];
#if defined(TEST_PRINT_TASK_ID)
  sprintk( buffer, "0x%08x -- %d",
      epos_task_self(), _Thread_Executing->current_priority );
#else
  union {
    uint32_t u;
    unsigned char c[4];
  } TempName;

  #if defined(TEST_ON_RTEMS_45)
    TempName.u = *(uint32_t *)_Thread_Executing->Object.name;
  #else
    TempName.u = _Thread_Executing->Object.name.name_u32;
  #endif
  sprintk( buffer, "%c%c%c%c -- %d",
      TempName.c[0], TempName.c[1], TempName.c[2], TempName.c[3],
      _Thread_Executing->current_priority
  );
#endif
  return buffer;
}

#define NUMBER_OF_BLOCKING_TASKS 5

/* RTEMS ids of blocking threads */
epos_id  Blockers[NUMBER_OF_BLOCKING_TASKS];

/* Semaphore they are all blocked on */
epos_id  Semaphore;

epos_task BlockingTasks(epos_task_argument arg)
{
  epos_status_code   status;
  epos_task_priority opri;
  epos_task_priority npri;

  status = epos_task_set_priority(RTEMS_SELF, RTEMS_CURRENT_PRIORITY, &opri);
  directive_failed( status, "epos_task_set_priority" );

  printk("semaphore_obtain -- BlockingTask %d @ pri=%d) blocks\n", arg, opri);
  status = epos_semaphore_obtain(Semaphore, RTEMS_WAIT, RTEMS_NO_TIMEOUT);
  directive_failed( status, "epos_semaphore_obtain" );

  /* priority should have changed while blocked */
  status = epos_task_set_priority(RTEMS_SELF, RTEMS_CURRENT_PRIORITY, &npri);
  directive_failed( status, "epos_task_set_priority" );

  printk("semaphore_obtain -- BlockingTask %d @ pri=%d) returns\n", arg, npri);

  (void) epos_task_delete( RTEMS_SELF );
}

/*************************************************************************/
/**********************        INITIALIZATION        *********************/
/*************************************************************************/

epos_task Init(epos_task_argument ignored)
{
  epos_status_code   status;   
  int                 i;
  
  printk( "\n\n*** TEST 34 ***" );

  /* Create synchronisation semaphore for LocalHwIsr -> Test Tasks */
  status = epos_semaphore_create(
    epos_build_name ('S', 'E', 'M', '1'),           /* name */
    0,                                               /* initial count = 0 */
    RTEMS_LOCAL                   | 
    RTEMS_COUNTING_SEMAPHORE      | 
    RTEMS_PRIORITY,
    0,
    &Semaphore);                                    /* *id */
  directive_failed( status, "epos_semaphore_create" );
  
  /* Create and start all tasks in the test */

  for (i = 0; i < NUMBER_OF_BLOCKING_TASKS; i++) {
    status = epos_task_create(
      epos_build_name('B','L','K','0'+i),               /* Name */
      2+i,                                               /* Priority */
      RTEMS_MINIMUM_STACK_SIZE*2,                        /* Stack size (8KB) */
      RTEMS_DEFAULT_MODES | RTEMS_NO_ASR,                /* Mode */
      RTEMS_DEFAULT_ATTRIBUTES | RTEMS_FLOATING_POINT,   /* Attributes */
      &Blockers[i]);                                     /* Assigned ID */
    directive_failed( status, "epos_task_create (BLKn)" );
  
    printk( "Blockers[%d] Id = 0x%08x\n", i, Blockers[i] );
    status = epos_task_start(Blockers[i], BlockingTasks, i);
    directive_failed( status, "epos_task_start (BLKn)" );
  }

  status = epos_task_wake_after( 100 );
  directive_failed( status, "epos_task_wake_after" );

  printk( "epos_task_set_priority -- invert priorities of tasks" );
  for (i = 0; i < NUMBER_OF_BLOCKING_TASKS; i++) {
    epos_task_priority opri;
    epos_task_priority npri= 2 + NUMBER_OF_BLOCKING_TASKS - i - 1;

    status = epos_task_set_priority(Blockers[i], npri, &opri);
    directive_failed( status, "epos_task_set_priority" );
  }

  for (i = 0; i < NUMBER_OF_BLOCKING_TASKS; i++) {
    printk( "epos_semaphore_release -- OK" );
    status = epos_semaphore_release(Semaphore);
    directive_failed( status, "epos_semaphore_release" );

    status = epos_task_wake_after( 100 );
    directive_failed( status, "epos_task_wake_after" );
  }

  /* exit the test */
  printk( "*** END OF TEST 34 ***" );
  exit(0);
}

/* configuration information */

//#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER

#define CONFIGURE_RTEMS_INIT_TASKS_TABLE

#define CONFIGURE_EXTRA_TASK_STACKS \
          (RTEMS_MINIMUM_STACK_SIZE * NUMBER_OF_BLOCKING_TASKS)

#define CONFIGURE_MAXIMUM_TASKS 6
#define CONFIGURE_MAXIMUM_SEMAPHORES 1

#include <rtems/confdefs.h>

/* end of file */
