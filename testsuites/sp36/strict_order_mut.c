/*
 *  Simple test program to demonstrate strict order mutex
 *
 */

#define CONFIGURE_INIT

#include <bsp.h>
#include <stdio.h>
#include "tmacros.h"
#include <rtems/score/coremutex.h>

#define BACK_TYPE(_type_in_ptr,_type_out,_type_in_name)		\
  ((_type_out *)((unsigned int)_type_in_ptr - (unsigned int)(&((_type_out *)0)->_type_in_name)))


/* configuration information */

//#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER

#define CONFIGURE_RTEMS_INIT_TASKS_TABLE

#define CONFIGURE_EXTRA_TASK_STACKS (RTEMS_MINIMUM_STACK_SIZE * 3)

#define CONFIGURE_MAXIMUM_TASKS 10
#define CONFIGURE_MAXIMUM_SEMAPHORES 10


void p_mutex_chain(Thread_Control *the_thread);
epos_task Task0(epos_task_argument ignored);
epos_task Task1(epos_task_argument ignored);
epos_task Init(epos_task_argument ignored);
epos_task_priority Get_current_pri(void);


#include <rtems/confdefs.h>



epos_id Task_id[4];
epos_name Task_name[4];

epos_id Mutex_id[4];
epos_name Mutex_name[4];

epos_task Init(epos_task_argument ignored)
{
  epos_status_code status;
  printk("\n----------------TEST 36 ------------\n");

  Mutex_name[0] = epos_build_name( 'S','0',' ',' ');
  status = epos_semaphore_create(
				  Mutex_name[0],
				  1,
				  RTEMS_LOCAL|
				  RTEMS_SIMPLE_BINARY_SEMAPHORE|
				  RTEMS_INHERIT_PRIORITY|
				  RTEMS_PRIORITY,
				  0,
				  &Mutex_id[0]);
  directive_failed( status, "epos_semaphore_create of S0");
  printk("Create S0, Inherit_priority\n");

  Mutex_name[1] = epos_build_name( 'S','1',' ',' ');
  status = epos_semaphore_create(
				  Mutex_name[1],
				  1,
				  RTEMS_LOCAL|
				  RTEMS_SIMPLE_BINARY_SEMAPHORE|
				  RTEMS_PRIORITY_CEILING|
				  RTEMS_PRIORITY,
				  1,
				  &Mutex_id[1]);
  directive_failed( status, "epos_semaphore_create of S1");
  printk("Create S1, Priority_celling  is 1\n");

  Mutex_name[2] = epos_build_name( 'S','Y','N','C');

				 
  Task_name[0] = epos_build_name( 'T','0',' ',' ');
  status = epos_task_create(
			     Task_name[0],
			     4,
			     RTEMS_MINIMUM_STACK_SIZE *2,
			     RTEMS_DEFAULT_MODES,
			     RTEMS_DEFAULT_ATTRIBUTES,
			     &Task_id[0]
			     );
  directive_failed( status,"epos_task_create of T0");
  printk("Create T0,priority is 4\n");

  

  status = epos_task_start( Task_id[0],Task0, 0);
  directive_failed( status,"epos_task_start of T0");

  status = epos_task_delete( RTEMS_SELF);
  directive_failed( status,"epos_task_delete of INIT");
}


epos_task Task0(epos_task_argument ignored)
{
  epos_status_code status;
  
  
  status = epos_semaphore_obtain(
				  Mutex_id[0],
				  RTEMS_WAIT,
				  0);
  printk("T0 epos_semaphore_obtain - S0\n");
  directive_failed( status,"epos_semaphore_obtain of S0\n");
  printk("The current priority of T0 is %d\n",Get_current_pri());
 
  status = epos_semaphore_obtain(
				  Mutex_id[1],
				  RTEMS_WAIT,
				  0);
  printk("T0 epos_semaphore_obtain - S1\n");
  directive_failed( status,"epos_semaphore_obtain of S1");
  printk("The current priority of T0 is %d\n",Get_current_pri());

#ifdef __RTEMS_STRICT_ORDER_MUTEX__
  status = epos_semaphore_release( Mutex_id[0] );
  printk("T0 - epos_semaphore_release - S0\n");
  if(status == CORE_MUTEX_RELEASE_NOT_ORDER)
    printk("T0 releasing S0 not in order\n");
#endif

  status = epos_semaphore_release(Mutex_id[1]);
  printk("T0 - epos_semaphore_release - S1\n");
  directive_failed( status,"epos_semaphore_release of S1\n");
  printk("The current priority of T0 is %d\n",Get_current_pri());
  

  Task_name[1] = epos_build_name( 'T','1',' ',' ');
  status = epos_task_create(
			     Task_name[1],
			     1,
			     RTEMS_MINIMUM_STACK_SIZE *2,
			     RTEMS_DEFAULT_MODES,
			     RTEMS_DEFAULT_ATTRIBUTES,
			     &Task_id[1]
			     );
  directive_failed( status , "epos_task_create of T1\n");
  printk("Create S1,priority is 1\n");
  

  status = epos_task_start( Task_id[1],Task1, 0);
  directive_failed( status, "epos_task_start of T1\n");

  printk("The current priority of T0 is %d\n",Get_current_pri());

  status = epos_semaphore_release(Mutex_id[0]);
  printk("T0 - epos_semaphore_release - S0\n");
  directive_failed( status, "epos_semaphore_release of S0\n");
  
}
  

epos_task Task1(epos_task_argument ignored)
{

  epos_status_code status;
  status =  epos_semaphore_obtain(
			 Mutex_id[0],
			 RTEMS_WAIT,
			 0);
  printk("T1 - epos_semaphore_obtain - S0");
  directive_failed( status," epos_semaphore_obtain S0");
}


epos_task_priority Get_current_pri(void)
{
  epos_status_code status;
  epos_task_priority pri;
  status = epos_task_set_priority(RTEMS_SELF,
				   RTEMS_CURRENT_PRIORITY,
				   &pri);
  directive_failed( status, " epos_task_set_priority ");
  return pri;
}

/*void p_mutex_chain(Thread_Control *the_thread)
{
  Chain_Control *con = &the_thread->lock_mutex;
  Chain_Node  * node = con->first;
  CORE_mutex_Control * p_mutex ;

  if(!_Chain_Is_empty(&the_thread->lock_mutex)){
    while(node != _Chain_Tail(con)){
      p_mutex = BACK_TYPE(node,CORE_mutex_Control,queue);
      printk("node:Id=%p,priority_before=%d,"
	     "holder_id=%d,holder's current priority=%d\n",node,
	     ((CORE_mutex_order_list *)node)->priority_before,p_mutex->holder_id,p_mutex->holder->current_priority);
      node = node->next;    
    }
  }
  else
    printk("the Chain is empty\n");
}
*/  
