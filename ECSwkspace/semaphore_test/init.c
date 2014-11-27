
#include <tmacros.h>  //////////
epos_task Task_1(epos_task_argument arg);

epos_task Init(epos_task_argument ignored) {
	int status, ceiling, old_ceiling;
	epos_id Mutex_id, Task_id;
	epos_interval ticks_per_second;
	printk("\n\n*** TEST samaphore obtain ***\n\n");
 
	/*
	 *  Create binary semaphore (a.k.a. Mutex) with Priority Ceiling
	 *  attribute.
	 */
  
	printk("Creating semaphore\n");
	status = epos_semaphore_create(epos_build_name('s', 'e', 'm', '1'), 1,
			RTEMS_BINARY_SEMAPHORE | RTEMS_PRIORITY | RTEMS_PRIORITY_CEILING, 1,
			&Mutex_id);
	// directive_failed( status, "epos_semaphore_create" );
	printk("status=%d,Mutex_id=%d\n",status,Mutex_id);
	printk("Calling epos_semaphore_obtain\n");
	status = epos_semaphore_obtain(Mutex_id, RTEMS_DEFAULT_OPTIONS, 0);
	// directive_failed( status, "epos_semaphore_obtain" );
	printk("status of obtain=%d\n",status);
	printk("Calling epos_task_create\n");
	status = epos_task_create(epos_build_name('T', 'A', 'S', '1'), 2,
			RTEMS_MINIMUM_STACK_SIZE*4, RTEMS_DEFAULT_MODES,
			RTEMS_DEFAULT_ATTRIBUTES, &Task_id);
	//directive_failed( status, "epos_task_create" );

	printk("Calling epos_task_start\n");
	status = epos_task_start(Task_id, Task_1, (epos_task_argument) &Mutex_id);
	//directive_failed( status, "epos_task_start" );
 
	// sleep(1);
 
	//ticks_per_second = epos_clock_get_ticks_per_second();

	//status = epos_task_wake_after(ticks_per_second);

	printk("Calling semaphore release\n");
	status = epos_semaphore_release(Mutex_id);
	//directive_failed( status, "epos_semaphore_release" );

	printk("\n*** END OF TEST SAMAPHORE ***\n");

//  epos_test_exit(0);
}

epos_task Task_1(epos_task_argument arg) {
	int status_in_task;
	epos_id *Mutex_id = (epos_id *) arg;

	printk("Init Task_1: Obtaining semaphore\n");
	status_in_task = epos_semaphore_obtain(*Mutex_id, RTEMS_DEFAULT_OPTIONS,
			0);
	printk("status_in_task=%d\n",status_in_task);
	//directive_failed( status_in_task, "Task_1 epos_semaphore_obtain" );
	//return;
	//epos_task_delete(RTEMS_SELF);
	epos_task_wake_after(10);
}

#define CONFIGURE_MAXIMUM_SEMAPHORES    1

//#define CONFIGURE_RTEMS_INIT_TASKS_TABLE   ///whether use init tasks

#define CONFIGURE_MAXIMUM_TASKS           3

#define CONFIGURE_INIT

#define CONFIGURE_APPLICATION_DOES_NOT_NEED_CLOCK_DRIVER

#define CONFIGURE_APPLICATION_DISABLE_FILESYSTEM

#define CONFIGURE_DISABLE_NEWLIB_REENTRANCY

#define CONFIGURE_TERMIOS_DISABLED

#define CONFIGURE_IDLE_TASK_BODY Init                //无初始任务，idle任务为初始任务
#define CONFIGURE_IDLE_TASK_INITIALIZES_APPLICATION  //

//#define CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER   //whether use clock tick
 

#include <rtems/confdefs.h>

int bootcard(const char *cmdline);

void test(void) { //printk("software int\n");
}

void test_asm(void);

int Show_add = 0x220000;

int main(void) {
	_ISR_Nest_level = 0;
	bootcard("asd");
	return 0;
}

