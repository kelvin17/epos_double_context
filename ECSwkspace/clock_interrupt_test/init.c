#include <bsp.h> /* for device driver prototypes */
#include <stdio.h>
#include <rtems/system.h>
#include <rtems/rtems/status.h>
#include <rtems/rtems/support.h>
#include <rtems/rtems/modes.h>
#include <rtems/score/object.h>
#include <rtems/score/stack.h>
#include <rtems/score/states.h>
#include <rtems/rtems/tasks.h>
#include <rtems/score/thread.h>
#include <rtems/score/threadq.h>
#include <rtems/score/tod.h>
#include <rtems/score/userext.h>
#include <rtems/score/wkspace.h>
#include <rtems/score/apiext.h>
#include <rtems/score/sysstate.h>
#include <rtems/score/interrupts.h>

//#include<userconfig.h> //Jo

void put(char* p){
  bwdsp_disable_interrupt(0);
  printk("%s \n",p);
  bwdsp_enable_interrupt(0);
}


void directive_fail(int status,const char *p){
  if(status) {
    bwdsp_disable_interrupt(0);
    printk("%s    , error status:%d\n",p,status);
    bwdsp_enable_interrupt(0);
  }
}


 
#include <tmacros.h>  //////////


#define TEST_EXTERN

epos_id   Task_id[ 4 ],Extension_id;  
epos_name Task_name[ 4 ],Extension_name;

void Task_switch(
  epos_tcb *unused,
  epos_tcb *heir
)
{ 
  char *p,p_name[6];
  p=p_name;
  bwdsp_disable_interrupt(0);
  if( heir->Object.id==Task_id[1]) 
      p="Task1";
  else if ( heir->Object.id==Task_id[2])
      p="Task2";
  else 	 
	  p="Task3";

  printk("%s is running\n",p);
  bwdsp_enable_interrupt(0);/////open interrupt safe??
}

epos_extensions_table Extensions = {
  NULL,                      /* task create user extension */
  NULL,                      /* task start user extension */
  NULL,                      /* task restart user extension */
  NULL,                      /* task delete user extension */
  Task_switch,               /* task switch user extension */
  NULL,                      /* task begin user extension */
  NULL,                      /* task exitted user extension */
  NULL                       /* fatal error user extension */
};

epos_task Task_1(
  epos_task_argument argument
)
{
   while( FOREVER );
}

epos_task Task_2(
  epos_task_argument argument
)
{ 
  while( FOREVER );
}

epos_task Task_3(
  epos_task_argument argument
)
{
  while( FOREVER );
}

epos_task Init(
  epos_task_argument argument
)
{
  epos_time_of_day time;
  epos_status_code status;
  put("*************** the Init task*****************\n");

  Extension_name = epos_build_name( 'E', 'X', 'T', ' ' );  
  status = epos_extension_create( Extension_name, &Extensions, &Extension_id   ); 
  directive_fail( status, "rtems_extension_create" );

 
  Task_name[ 1 ] =  epos_build_name( 'T', 'A', '1', ' ' );
  Task_name[ 2 ] =  epos_build_name( 'T', 'A', '2', ' ' );
  Task_name[ 3 ] =  epos_build_name( 'T', 'A', '3', ' ' );

  status = epos_task_create(
     Task_name[ 1 ],
     1,
     RTEMS_MINIMUM_STACK_SIZE * 2,
     RTEMS_PREEMPT|RTEMS_TIMESLICE, 
     RTEMS_DEFAULT_ATTRIBUTES,
     &Task_id[ 1 ]
  );
  directive_fail( status, "epos_task_create of TA1" );

  status = epos_task_create(
     Task_name[ 2 ],
     1,
     RTEMS_MINIMUM_STACK_SIZE * 2,
     RTEMS_PREEMPT|RTEMS_TIMESLICE,
     RTEMS_DEFAULT_ATTRIBUTES,
     &Task_id[ 2 ]
  );
  directive_fail( status, "epos_task_create of TA2" );
/*
  status = epos_task_create(
     Task_name[ 3 ],
     1,
     RTEMS_MINIMUM_STACK_SIZE * 2,
     RTEMS_PREEMPT|RTEMS_TIMESLICE,
     RTEMS_DEFAULT_ATTRIBUTES,
     &Task_id[ 3 ]
  );
  directive_fail( status, "epos_task_create of TA3" );
*/
  status = epos_task_start( Task_id[ 1 ], Task_1, 0 );
  directive_fail( status, "epos_task_start of TA1" );

  status = epos_task_start( Task_id[ 2 ], Task_2, 0 );
  directive_fail( status, "epos_task_start of TA2" );

//  status = epos_task_start( Task_id[ 3 ], Task_3, 0 );
//  directive_fail( status, "epos_task_start of TA3" );
 

  put("end of init\n");
  status = epos_task_delete( RTEMS_SELF );
  directive_fail( status, "epos_task_delete( RTEMS_SELF )" );

}
 


#define CONFIGURE_MAXIMUM_USER_EXTENSIONS     1 //user extension

#define CONFIGURE_TICKS_PER_TIMESLICE 	1

#define CONFIGURE_RTEMS_INIT_TASKS_TABLE   ///whether use init tasks

#define CONFIGURE_MAXIMUM_TASKS           4
 
#define CONFIGURE_INIT

#define CONFIGURE_APPLICATION_DISABLE_FILESYSTEM

#define CONFIGURE_DISABLE_NEWLIB_REENTRANCY

#define CONFIGURE_TERMIOS_DISABLED

#define CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER   //whether use clock tick

#define CONFIGURE_MICROSECONDS_PER_TICK 100000 //0.1second

#include <rtems/confdefs.h>
 
int bootcard( const char *cmdline);

int main(void) { 
  return bootcard("asd");
}


