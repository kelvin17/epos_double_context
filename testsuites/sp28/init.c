/*
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id$
 */

#define CONFIGURE_INIT
#include <tmacros.h>

epos_task Init(epos_task_argument argument);

//#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER
#define CONFIGURE_MAXIMUM_USER_EXTENSIONS    2

#define CONFIGURE_MAXIMUM_TASKS              4
#define CONFIGURE_MAXIMUM_TASK_VARIABLES     (4)
#define CONFIGURE_RTEMS_INIT_TASKS_TABLE
//#define CONFIGURE_MICROSECONDS_PER_TICK      10000

#define CONFIGURE_MICROSECONDS_INIT

#include <rtems/confdefs.h>

#include <bsp.h>
#include <rtems/error.h>
#include <stdio.h>

volatile void *taskvar;
volatile int nRunning;
volatile int nDeleted;

epos_task
subtask (epos_task_argument arg)
{
  int localvar = arg;
  int i;
  epos_status_code sc;

  nRunning++;
  while (nRunning != 3)
    epos_task_wake_after (0);
  sc = epos_task_variable_add(RTEMS_SELF, (void **)&taskvar, NULL);
  directive_failed( sc, "task variable add" );

  taskvar = (void *)localvar;
  while (localvar < 1000) {
    localvar++;
    epos_task_wake_after (0);
    taskvar = (void *)((int)taskvar + 1);
    epos_task_wake_after (0);
    if ((int)taskvar != localvar) {
      printk ("Task:%d taskvar:%d localvar:%d\n", arg, (int)taskvar, localvar);
      epos_task_suspend (RTEMS_SELF);
    }
  }
  sc = epos_task_variable_delete(RTEMS_SELF, (void **)&taskvar);
  nDeleted++;
  directive_failed( sc, "task variable delete" );

  if ((int)taskvar == localvar) {
    printk("Task:%d deleted taskvar:%d localvar:%d\n", arg, (int)taskvar, localvar);
    nRunning--;
    epos_task_suspend (RTEMS_SELF);
  }
  while (nDeleted != 3)
    epos_task_wake_after (0);
  for (i = 0 ; i < 1000 ; i++) {
    taskvar = (void *)(localvar = 100 * arg);
    epos_task_wake_after(0);
    if (nRunning <= 1)
      break;
    if ((int)taskvar == localvar) {
      printk("Task:%d taskvar:%d localvar:%d\n", arg, (int)taskvar, localvar);
      nRunning--;
      epos_task_suspend(RTEMS_SELF);
    }
  }
  nRunning--;
  while (nRunning)
    epos_task_wake_after(0);

  printk("*** END OF TEST 28 ***" );
  epos_test_exit(0);
}

void
starttask (int arg)
{
  epos_id tid;
  epos_status_code sc;

  sc = epos_task_create(epos_build_name ('S', 'R', 'V', arg + 'A'),
    100,
    RTEMS_MINIMUM_STACK_SIZE,
    RTEMS_PREEMPT|RTEMS_NO_TIMESLICE|RTEMS_NO_ASR|RTEMS_INTERRUPT_LEVEL(0),
    RTEMS_NO_FLOATING_POINT|RTEMS_LOCAL,
    &tid
  );
  directive_failed( sc, "task create" );

  sc = epos_task_start(tid, subtask, arg);
  directive_failed( sc, "task start" );
}

volatile void *taskvar1;
volatile void *taskvar2;
volatile void *taskvar3;

void test_errors(void)
{
  epos_status_code  sc;
  void              *value;

  /*
   *  task variable get error status codes
   */
  printk( "task variable get - NULL pointer - RTEMS_INVALID_ADDRESS" );
  sc = epos_task_variable_get(RTEMS_SELF, NULL, &value );
  fatal_directive_status( sc, RTEMS_INVALID_ADDRESS, "get NULL pointer" );

  printk( "task variable get - bad result - RTEMS_INVALID_ADDRESS" );
  sc = epos_task_variable_get(RTEMS_SELF, (void **)&taskvar1, NULL);
  fatal_directive_status( sc, RTEMS_INVALID_ADDRESS, "get bad result" );

  printk( "task variable get - bad pointer - RTEMS_INVALID_ADDRESS" );
  sc = epos_task_variable_get(RTEMS_SELF, (void **)&taskvar1, &value);
  fatal_directive_status( sc, RTEMS_INVALID_ADDRESS, "get bad pointer" );

  /*
   *  task variable delete error status codes
   */
  printk( "task variable delete - NULL pointer - RTEMS_INVALID_ADDRESS" );
  sc = epos_task_variable_delete(RTEMS_SELF, NULL);
  fatal_directive_status( sc, RTEMS_INVALID_ADDRESS, "delete NULL pointer" );

  printk( "task variable delete - bad pointer - RTEMS_INVALID_ADDRESS" );
  sc = epos_task_variable_delete(RTEMS_SELF, (void **)&taskvar1);
  fatal_directive_status( sc, RTEMS_INVALID_ADDRESS, "delete bad pointer" );

}

volatile uint32_t test_dtor_ran;

void test_dtor(void *pointer)
{
  test_dtor_ran++;
}


void test_multiple_taskvars(void)
{
  epos_status_code  sc;
  void              *value;

  test_dtor_ran = 0;

  /*
   *  Add multiple task variables and add each twice to
   *  verify that behavior is OK
   */
  printk( "Adding multiple task variables" );
  sc = epos_task_variable_add(RTEMS_SELF, (void **)&taskvar1, NULL);
  directive_failed( sc, "add multiple #1" );

  sc = epos_task_variable_add(RTEMS_SELF, (void **)&taskvar1, test_dtor);
  directive_failed( sc, "add multiple #2" );

  sc = epos_task_variable_add(RTEMS_SELF, (void **)&taskvar2, test_dtor);
  directive_failed( sc, "add multiple #3" );

  sc = epos_task_variable_add(RTEMS_SELF, (void **)&taskvar2, NULL);
  directive_failed( sc, "add multiple #4" );

  sc = epos_task_variable_add(RTEMS_SELF, (void **)&taskvar3, NULL);
  directive_failed( sc, "add multiple #5" );

  sc = epos_task_variable_add(RTEMS_SELF, (void **)&taskvar3, test_dtor);
  directive_failed( sc, "add multiple #6" );

  /*
   *  Obtain task variables in various spots on the chain
   */
  printk( "Obtaining multiple task variables" );
  sc = epos_task_variable_get( RTEMS_SELF, (void **)&taskvar3, &value );
  directive_failed( sc, "get multiple #1" );
  sc = epos_task_variable_get( RTEMS_SELF, (void **)&taskvar2, &value );
  directive_failed( sc, "get multiple #2" );
  sc = epos_task_variable_get( RTEMS_SELF, (void **)&taskvar1, &value );
  directive_failed( sc, "get multiple #2" );
  
  /*
   *  Delete task variables in various spots on the chain
   */

  /* to trip the destructors */
  taskvar1 = (void *)1;
  taskvar2 = (void *)2;
  taskvar3 = (void *)3;

  printk( "Deleting multiple task variables" );
  sc = epos_task_variable_delete(RTEMS_SELF, (void **)&taskvar2);
  directive_failed( sc, "delete multiple #1" );
  sc = epos_task_variable_delete(RTEMS_SELF, (void **)&taskvar3);
  directive_failed( sc, "delete multiple #2" );
  sc = epos_task_variable_delete(RTEMS_SELF, (void **)&taskvar1);
  directive_failed( sc, "delete multiple #3" );

  if ( test_dtor_ran != 2 ) {
    printk( "Test dtor ran %d times not 2 times as expected\n", test_dtor_ran );
    epos_test_exit(0);
  }
}

#define MAX_VARS 4096

void *Pointers[MAX_VARS];

void test_out_of_memory(void)
{
  int                 i;
  int                 max;
  epos_status_code   sc;
  int                 ran_out = 0;
  void              **base;

  base = Pointers;

  for (i=0 ; i<MAX_VARS ; i++ ) {
    sc = epos_task_variable_add(RTEMS_SELF, &base[i], NULL);
    if ( sc == RTEMS_NO_MEMORY ) {
      printk( "task_variable_add - returns NO_MEMORY" );
      max = i;
      ran_out = 1;
      break;
    }
    directive_failed( sc, "add loop until out of memory" );
  }

  if ( !ran_out ) {
    printk( "ERROR!!! did not run out of memory adding task variables!" );
    epos_test_exit(0);
  }

  for (i=0 ; i<max ; i++ ) {
    sc = epos_task_variable_delete(RTEMS_SELF, &base[i]);
    directive_failed( sc, "delete loop until out of memory" );
  }
}

epos_id main_task;
epos_id other_task;

epos_task Other_Task(epos_task_argument ignored)
{
  epos_status_code sc;

  printk( "Deleting task variables in another task" );
  sc = epos_task_variable_delete(main_task, (void **)&taskvar1);
  directive_failed( sc, "delete loop for other task" );

  (void) epos_task_delete( RTEMS_SELF );
}

void test_delete_from_other_task(void)
{
  epos_status_code sc;

  test_dtor_ran = 0;

  sc = epos_task_ident( RTEMS_SELF, 0, &main_task );
  directive_failed( sc, "task ident" );

  sc = epos_task_variable_add(RTEMS_SELF, (void **)&taskvar1, test_dtor);
  directive_failed( sc, "add for other task case" );

  sc = epos_task_create(epos_build_name ('O', 'T', 'H', 'R'),
    100,
    RTEMS_MINIMUM_STACK_SIZE,
    RTEMS_PREEMPT|RTEMS_NO_TIMESLICE|RTEMS_NO_ASR|RTEMS_INTERRUPT_LEVEL(0),
    RTEMS_NO_FLOATING_POINT|RTEMS_LOCAL,
    &other_task
  );
  directive_failed( sc, "task create other" );

  sc = epos_task_start(other_task, Other_Task, 0);
  directive_failed( sc, "task start other" );

  epos_task_wake_after( 100 );
  
  if ( test_dtor_ran != 1 ) {
    printk( "Test dtor ran %d times not 1 times as expected\n", test_dtor_ran );
    epos_test_exit(0);
  }
}

/*
 *  Task which adds task variables just to delete them
 */
epos_task Task_variable_deleter(epos_task_argument ignored)
{
  epos_status_code sc;

  printk( "Adding multiple task variables to delete implicitly" );
  sc = epos_task_variable_add(RTEMS_SELF, (void **)&taskvar1, test_dtor);
  directive_failed( sc, "add multiple for delete #1" );

  sc = epos_task_variable_add(RTEMS_SELF, (void **)&taskvar2, NULL);
  directive_failed( sc, "add multiple for delete #2" );

  sc = epos_task_variable_add(RTEMS_SELF, (void **)&taskvar3, test_dtor);
  directive_failed( sc, "add multiple for delete #3" );

  (void) epos_task_delete( RTEMS_SELF );
}

void test_delete_as_side_effect(void)
{
  epos_status_code sc;
  epos_id          deleter_task;

  test_dtor_ran = 0;

  sc = epos_task_create(epos_build_name ('O', 'T', 'H', 'R'),
    100,
    RTEMS_MINIMUM_STACK_SIZE,
    RTEMS_PREEMPT|RTEMS_NO_TIMESLICE|RTEMS_NO_ASR|RTEMS_INTERRUPT_LEVEL(0),
    RTEMS_NO_FLOATING_POINT|RTEMS_LOCAL,
    &deleter_task
  );
  directive_failed( sc, "task create deleter" );

  sc = epos_task_start(deleter_task, Task_variable_deleter, 0);
  directive_failed( sc, "task start deleter" );

  epos_task_wake_after( 100 );
  
  if ( test_dtor_ran != 2 ) {
    printk( "Test dtor ran %d times not 2 times as expected\n", test_dtor_ran );
    epos_test_exit(0);
  }
}

epos_task Init (epos_task_argument ignored)
{
  printk("*** START OF TEST 28 ***" );

  test_errors();

  test_multiple_taskvars();

  test_delete_as_side_effect();

  test_delete_from_other_task();

  starttask (1);
  starttask (2);
  starttask (3);

  test_out_of_memory();

  epos_task_suspend (RTEMS_SELF);
}
