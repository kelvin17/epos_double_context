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



void put(char* p){
  bwdsp_disable_interrupt(0);
  printk("%s \n",p);
  bwdsp_enable_interrupt(0);
}


void directive_fail(int status,const char *p){
  if(status) {
    bwdsp_disable_interrupt(0);
    printk("%s    %d\n",p,status);
    bwdsp_enable_interrupt(0);
  }
}


 
#include <tmacros.h>  //////////

epos_task Delay_task(
  epos_task_argument ignored
)
{
  epos_status_code    status;

 
  put( "Delay - epos_task_wake_after - OK" );

  status = epos_task_wake_after(4);
  while(1);

  put( "ERROR - delay task woke up!!" );
}

epos_task Init(
  epos_task_argument ignored
)
{
  epos_status_code    status;
  epos_id             task_id;

  put( "\n*** task_wake_test ***" );

 // put( "Init - epos_task_create - delay task - OK" );
  status = epos_task_create(
     epos_build_name( 'T', 'A', '1', ' ' ),
     1,
     RTEMS_MINIMUM_STACK_SIZE,
     RTEMS_DEFAULT_OPTIONS,
     RTEMS_DEFAULT_ATTRIBUTES,
     &task_id
  );
  directive_fail( status, "epos_task_create" );

  //put( "Init - epos_task_start - delay task - OK" );
  status = epos_task_start( task_id, Delay_task, 0 );
  directive_fail( status, "epos_task_start" );

  put( "Init - epos_task_wake_after - let delay the init task 2 ticks - OK" );
  status = epos_task_wake_after( 2 );
  directive_fail( status, "epos_task_wake_after" );

  put( "Init - The init task returns - OK" );

//  put( "Init - epos_task_restart - delay task - OK" );
//  status = epos_task_restart( task_id, 0 );
//  directive_fail( status, "epos_task_restart" );

  put( "*** END OF TEST ***" );
  bwdsp_disable_interrupt(0);
}

//#define CONFIGURE_TICKS_PER_TIMESLICE 3


#define CONFIGURE_RTEMS_INIT_TASKS_TABLE   ///whether use init tasks

#define CONFIGURE_MAXIMUM_TASKS           2
 
#define CONFIGURE_INIT

#define CONFIGURE_APPLICATION_DISABLE_FILESYSTEM

#define CONFIGURE_DISABLE_NEWLIB_REENTRANCY

#define CONFIGURE_TERMIOS_DISABLED

//#define CONFIGURE_IDLE_TASK_BODY Idle_my               //无初始任务，idle任务为初始任务
//#define CONFIGURE_IDLE_TASK_INITIALIZES_APPLICATION  //

//#define CONFIGURE_APPLICATION_DOES_NOT_NEED_CLOCK_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER   //whether use clock tick

#include <rtems/confdefs.h>
 
int bootcard( const char *cmdline);

int main(void) {
 _ISR_Nest_level = 0;

 return bootcard("asd");
}


