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
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif


#define CONFIGURE_INIT
#include "sp01.h"

 

/* comment to use no isr_installc
void init_timer0_int();
extern void (*isr_installc(int sig, void *func(void)))(int);

void kong_counter()
{
}
     
  */  
  
SCORE_EXTERN uint32_t i_1;/*by zwj*/


epos_task Init(
  epos_task_argument argument
)  
{   
  put("TEST Partition");
  epos_status_code status;
  
  epos_time_of_day time;
  
  Task_name[ 1 ] = epos_build_name( 'T', 'A', '1', ' ' );
 
  Partition_name[ 1 ] =  epos_build_name( 'P', 'T', '1', ' ' );
  Partition_name[ 2 ] =  epos_build_name( 'P', 'T', '2', ' ' );
  status = epos_task_create(
     Task_name[ 1 ],
     4u,
     RTEMS_MINIMUM_STACK_SIZE,
     RTEMS_DEFAULT_MODES,
     RTEMS_DEFAULT_ATTRIBUTES,
     &Task_id[ 1 ]
  );
 directive_suc(status);


  status = epos_task_start( Task_id[ 1 ], Task_1, 0 );
  directive_suc(status);
  
  
  put("INIT - rtems_partition_create - partition 1");
  status = epos_partition_create(
    Partition_name[ 1 ],
    Area_1,
    4096,
    512,
    RTEMS_DEFAULT_ATTRIBUTES,
    &Partition_id[ 1 ]
  );
  directive_suc(status);


  put("INIT - rtems_partition_create - partition 2");
  status = epos_partition_create(
    Partition_name[ 2 ],
    Area_2,
    256,
    128,
    RTEMS_DEFAULT_ATTRIBUTES,
    &Partition_id[ 2 ]
  );
  directive_suc( status);

  status = epos_task_delete( RTEMS_SELF );
  directive_suc( status);


}


int Intr_Count_Start = 0;
int Intr_Count_End = 0;
  


void Intr_Debug()
{
	printf("The Intrrupt Starts at %d, Ends at %d\n", Intr_Count_Start, Intr_Count_End);
}


int main()
{
	_ISR_Nest_level=0;
	bootcard("zwj");
	return 0;
}
