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
 *  COPYRIGHT (c) 1989-2009.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: init.c,v 1.15 2009/05/10 14:39:46 joel Exp $
 */

#define CONFIGURE_INIT
#include "sp12.h"

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

  put( "\n\n*** TEST SAMAPHORE MAMAGER ***" );

  Task_name[ 1 ]          = epos_build_name( 'T', 'A', '1', ' ' );
  Task_name[ 2 ]          = epos_build_name( 'T', 'A', '2', ' ' );
 

  Priority_task_name[ 1 ] = epos_build_name( 'P', 'R', 'I', '1' );
  Priority_task_name[ 2 ] = epos_build_name( 'P', 'R', 'I', '2' );
  

  Semaphore_name[ 1 ]     = epos_build_name( 'S', 'M', '1', ' ' );
  Semaphore_name[ 2 ]     = epos_build_name( 'S', 'M', '2', ' ' );
  Semaphore_name[ 3 ]     = epos_build_name( 'S', 'M', '3', ' ' );

  status = epos_semaphore_create(
    Semaphore_name[ 1 ],
    1,
    RTEMS_BINARY_SEMAPHORE,
    RTEMS_NO_PRIORITY,
    &Semaphore_id[ 1 ]
  );
  //directive_failed( status, "epos_semaphore_create of SM1" );
printk("epos_semaphore_create of SM1,status=%d\n",status);

/*
  status = epos_semaphore_create(
    Semaphore_name[ 2 ],
    0,
    RTEMS_PRIORITY,
    RTEMS_NO_PRIORITY,
    &Semaphore_id[ 2 ]
  );
  //directive_failed( status, "epos_semaphore_create of SM2" );

  printk("epos_semaphore_create of SM2,status=%d\n",status);

  put( "INIT - Binary Semaphore and Priority Inheritance Test" );

  status = epos_semaphore_delete( Semaphore_id[ 2 ] );
  //directive_failed( status, "epos_semaphore_delete of SM2 #1" );
  printk("epos_semaphore_delete of SM2,status=%d\n",status);

  put( "INIT - epos_semaphore_create - allocated binary semaphore" );
  status = epos_semaphore_create(
    Semaphore_name[ 2 ],
    0,
    RTEMS_BINARY_SEMAPHORE | RTEMS_PRIORITY | RTEMS_INHERIT_PRIORITY,
    RTEMS_NO_PRIORITY,
    &Semaphore_id[ 2 ]
  );
  //directive_failed( status, "epos_semaphore_create of priority inherit SM2" );
  printk("epos_semaphore_create of priority inherit SM2,status=%d\n",status);

  put( "INIT - epos_semaphore_release - allocated binary semaphore" );
  status = epos_semaphore_release( Semaphore_id[ 2 ] );
  //directive_failed( status, "epos_semaphore_release of SM2" );
  printk("epos_semaphore_release of SM2,status=%d\n",status);

  put( "INIT - epos_semaphore_delete - allocated binary semaphore" );
  status = epos_semaphore_delete( Semaphore_id[ 2 ] );
  //directive_failed( status, "epos_semaphore_delete of SM2 #2" );
  printk("epos_semaphore_delete of SM2 ,status=%d\n",status);

  status = epos_semaphore_create(
    Semaphore_name[ 2 ],
    1,
    RTEMS_BINARY_SEMAPHORE | RTEMS_PRIORITY | RTEMS_INHERIT_PRIORITY,
    RTEMS_NO_PRIORITY,
    &Semaphore_id[ 2 ]
  );
  //directive_failed( status, "epos_semaphore_create of priority inherit SM2" );

   printk("epos_semaphore_create of priority inherit SM2 ,status=%d\n",status);

  status = epos_semaphore_delete( Semaphore_id[ 2 ] );
 // directive_failed( status, "epos_semaphore_delete of SM2 #3" );

  status = epos_semaphore_create(
    Semaphore_name[ 2 ],
    0,
    RTEMS_PRIORITY,
    RTEMS_NO_PRIORITY,
    &Semaphore_id[ 2 ]
  );
  //directive_failed( status, "epos_semaphore_create of priority SM2" );
  printk("epos_semaphore_create of priority SM2 ,status=%d\n",status);

  status = epos_semaphore_release( Semaphore_id[ 2 ] );
  //directive_failed( status, "epos_semaphore_release of SM2" );
  printk("epos_semaphore_release of SM2 ,status=%d\n",status);
*/
  status = epos_task_create(
    Task_name[ 1 ],
    4,
    RTEMS_MINIMUM_STACK_SIZE,
    RTEMS_DEFAULT_MODES,
    RTEMS_DEFAULT_ATTRIBUTES,
    &Task_id[ 1 ]
  );
  //directive_failed( status, "epos_task_create of TA1" );
  printk("epos_task_create of TA1 ,status=%d\n",status);

  status = epos_task_create(
    Task_name[ 2 ],
    4,
    RTEMS_MINIMUM_STACK_SIZE,
    RTEMS_DEFAULT_MODES,
    RTEMS_DEFAULT_ATTRIBUTES,
    &Task_id[ 2 ]
  ); 
  //directive_failed( status, "epos_task_create of TA2" );

 printk("epos_task_create of TA2 ,status=%d\n",status);

  status = epos_task_start( Task_id[ 1 ], Task_1, 0 );
 // directive_failed( status, "epos_task_start of TA1" );

  status = epos_task_start( Task_id[ 2 ], Task_2, 0 );
  //directive_failed( status, "epos_task_start of TA2" );

 printk("end of init\n");

  status = epos_task_delete( RTEMS_SELF );
  //directive_failed( status, "epos_task_delete of RTEMS_SELF" );
}
int bootcard(const char *cmdline);


int main()
{
	 _ISR_Nest_level = 0;
	 bootcard("asd");
	 return 0;
}
