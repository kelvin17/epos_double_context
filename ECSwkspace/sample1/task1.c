/*  Task_1_through_3
 *
 *  This routine serves as a test task.  It verifies the basic task
 *  switching capabilities of the executive.
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
 *  $Id: task1.c,v 1.12 2009/11/30 03:33:23 ralf Exp $
 */


#include "sp01.h"

epos_task Task_1_through_3 (
  epos_task_argument argument
) 
{
  epos_id          tid=1;
  epos_time_of_day time;
  epos_status_code status;
  epos_interval    ticks=1;

	printk("in task1\n");
  status = epos_task_ident( RTEMS_SELF, RTEMS_SEARCH_ALL_NODES, &tid );
  //directive_failed( status, "epos_task_ident" );

  /*
   * Use TOD_MILLISECONDS_TO_TICKS not RTEMS_MILLISECONDS_TO_TICKS to
   * test C implementation in SuperCore -- not macro version used
   * everywhere else.
   */
  ticks = TOD_MILLISECONDS_TO_TICKS( task_number( tid ) * 5 * 1000 );

  //while( 1 ) { 
//  	printf("i_1 is %d\n",i_1);
    status = epos_clock_get_tod( &time );
// printf("epos_clock_get_tod status is %d\n",status);
    //if ( time.second >= 6 ) {
    //  puts( "*** END OF TEST 1 ***" );
	
     // epos_test_exit( 0 );
    //}

   // put_name( Task_name[ task_number( tid ) ], FALSE );
    //print_time( " tod - ", &time, "\n" );
//    printf("%d,%d,%d,%d,%d,%d\n",time.year,time.month,time.day,time.hour,time.minute,time.second);
	

 // 	}
 int i = 0, sum = 0;
for (i=0; i<100; i++)
{
	sum += i;
	if (i == 50)
	{
		epos_task_suspend(RTEMS_SELF);
	}
}
printk("the sum is %d\n", sum);

printk("task1 resume\n");
   epos_task_delete(RTEMS_SELF);

}


epos_task Task_0 (
  epos_task_argument argument
) 
{
	printk("in task0\n");
    epos_task_resume(Task_id[1]);
	printk("task0 delete\n");
	epos_task_delete(RTEMS_SELF);
}