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

	//  puts( "\n\n*** TEST 1 ***" );

	build_time( &time, 4, 10, 2013, 9, 0, 0, 0 );
	status = epos_clock_set( &time );

	Task_name[ 1 ] = epos_build_name( 'T', 'A', '1', ' ' );
	status = epos_task_create(
		Task_name[ 1 ],
		3,
		RTEMS_MINIMUM_STACK_SIZE * 4,
		RTEMS_INTERRUPT_LEVEL(31),
		RTEMS_DEFAULT_ATTRIBUTES,
		&Task_id[ 1 ]
	);

	status = epos_task_start( Task_id[ 1 ], Task_1_through_3, 0 );

	Task_name[ 0 ] = epos_build_name( 'T', 'A', '0', ' ' );
	status = epos_task_create(
		Task_name[ 0 ],
		10,
		RTEMS_MINIMUM_STACK_SIZE * 4,
		RTEMS_INTERRUPT_LEVEL(31),
		RTEMS_DEFAULT_ATTRIBUTES,
		&Task_id[ 0 ]
	);

	status = epos_task_start( Task_id[ 0 ], Task_0, 0 );
	epos_task_delete(RTEMS_SELF);
}

int main()
{
	_ISR_Nest_level=0;
	bootcard("zwj");
	return 0;
}
