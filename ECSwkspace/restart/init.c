/*  Init
 *
 *  This routine is the initialization task for this test program.
 *  It is a user initialization task and has the responsibility for creating
 *  and starting the tasks that make up the test.  If the time of day
 *  clock is required for the test, it should also be set to a known
 *  value by this function.
 *
 *  Input parameters:
 *    argument - task argument
 *
 *  Output parameters:  NONE
 *
 *  COPYRIGHT (c) 1989-1999.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: init.c,v 1.11 2008/02/01 00:45:10 joel Exp $
 */

 
#define CONFIGURE_INIT
#include "sp01.h"
   
int *ptr;
epos_task Init(
  epos_task_argument argument
)   
{      
	epos_time_of_day time;
	epos_status_code status;
	int i=4;  

	printk( "\n\n*** TEST Restart ***\n" );
	Argument = 0;

	Task_name[ 1 ] = epos_build_name( 'T', 'A', '1', ' ' );
	status = epos_task_create(
		Task_name[ 1 ],
		2,
		RTEMS_MINIMUM_STACK_SIZE * 4,
		RTEMS_DEFAULT_MODES,
		RTEMS_DEFAULT_ATTRIBUTES,
		&Task_id[ 1 ]
	);
	status = epos_task_start( Task_id[ 1 ], Task_1_through_3, 0 );
		Task_name[ 2 ] = epos_build_name( 'T', 'A', '2', ' ' );
	status = epos_task_create(
		Task_name[ 2 ],
		2,
		RTEMS_MINIMUM_STACK_SIZE * 4,
		RTEMS_DEFAULT_MODES,
		RTEMS_DEFAULT_ATTRIBUTES,
		&Task_id[ 2 ]
	);
	status = epos_task_start( Task_id[ 2 ], Task_0, Argument );
	
	epos_task_delete(RTEMS_SELF);
}

//restart是为了用不同的参数启动相同的任务，所以，给出如下例子，每次给一个不同的参数进行重启
epos_task Task_1_through_3 (
  epos_task_argument argument
) 
{
	 epos_status_code   status;
	uint32_t pass = argument + 1;
	printk("In task1:%d\n",pass);
	if(pass == 10)
	{
		epos_task_delete(Task_id[2]);
		epos_task_delete(RTEMS_SELF);
	}
	if(pass % 2 ==1)
	{	
		epos_task_restart(Task_id[2],pass);
	}

	epos_task_restart(Task_id[1],pass);
}

epos_task Task_0 (
  epos_task_argument argument
) 
{
	 epos_status_code   status;
	if(argument == Argument)
	{
		printk( "TA2 - epos_task_wake_after - sleep 2 second-start" );
    		status = epos_task_wake_after( 2 * TICKS_PER_SECOND );
    		directive_failed( status, "epos_task_wake_after" );
  	}

  	while( FOREVER ) 
	{
    		printk( "TA2 - epos_task_wake_after - sleep 4 second-restart-argument:%d\n",argument );
    		status = epos_task_wake_after( 4*TICKS_PER_SECOND  );
    		directive_failed( status, "epos_task_wake_after" );
  	}
}

int main()
{
	_ISR_Nest_level=0;
	bootcard("zwj");
	return 0;
}
