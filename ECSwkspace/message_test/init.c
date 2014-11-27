#define CONFIGURE_INIT
#include <bsp/bootcard.h>
#include <stdio.h>
#include "sp13.h"

void put(char* p){
  bwdsp_disable_interrupt(0);
  printk("%s \n",p);
  bwdsp_enable_interrupt(0);
}

epos_task Init(
  epos_task_argument argument
)
{
  epos_status_code status;

  printk( "*** TEST MESSAGE ***" );

  Task_name[ 1 ] =  epos_build_name( 'T', 'A', '1', ' ' );
  Task_name[ 2 ] =  epos_build_name( 'T', 'A', '2', ' ' );

  
   	 Queue_name[ 1 ] = epos_build_name( 'Q', '1', ' ', ' ' );


	
  	status = epos_message_queue_create(
    Queue_name[ 1 ],
    100,
    MESSAGE_SIZE,
    RTEMS_DEFAULT_ATTRIBUTES,
    &Queue_id[ 1 ]
  );
  status = epos_task_create(
    Task_name[ 1 ],
    4,
    RTEMS_MINIMUM_STACK_SIZE * 2,
    RTEMS_DEFAULT_MODES,
    RTEMS_DEFAULT_ATTRIBUTES,
    &Task_id[ 1 ]
  );
  //directive_failed( status, "epos_task_create of TA1" );

  status = epos_task_create(
    Task_name[ 2 ],
    4,
    RTEMS_MINIMUM_STACK_SIZE * 2,
    RTEMS_DEFAULT_MODES,
    RTEMS_DEFAULT_ATTRIBUTES,
    &Task_id[ 2 ]
  );
  //directive_failed( status, "epos_task_create of TA2" );

 // directive_failed( status, "epos_task_create of TA3" );

 	 status = epos_task_start( Task_id[ 1 ], Task_1, 0 );
  //directive_failed( status, "epos_task_start of TA1" );

 	 status = epos_task_start( Task_id[ 2 ], Task_2, 0 );
  //directive_failed( status, "epos_task_start of TA2" );


 // directive_failed( status, "epos_message_queue_create of Q1" );


 // directive_failed( status, "epos_message_queue_create of Q3" );

  //status = epos_task_delete( RTEMS_SELF );
 // directive_failed( status, "epos_task_delete of RTEMS_SELF" );
 printk("\n&&&end of message end&&&\n");
}
	
int main()
{
	 _ISR_Nest_level = 0;
	 bootcard("asd");
	 return 0;
}
