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
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#define CONFIGURE_INIT
#include "sp01.h"

void put(char* p){
  bwdsp_disable_interrupt(0);
  printk("%s \n",p);
  bwdsp_enable_interrupt(0);
}

void directive_suc(int status)
{
	if(status != RTEMS_SUCCESSFUL)
	{
		printk("error code: %d\n",status);
	}
}

epos_task Init(
  epos_task_argument argument
)
{
  epos_status_code status;

  put( "\n\n*** TEST Region ***" );

  Task_name[ 1 ] =  epos_build_name( 'T', 'A', '1', ' ' );
  Task_name[ 2 ] =  epos_build_name( 'T', 'A', '2', ' ' );
  Task_name[ 3 ] =  epos_build_name( 'T', 'A', '3', ' ' );
  Task_name[ 4 ] =  epos_build_name( 'T', 'A', '4', ' ' );
  Task_name[ 5 ] =  epos_build_name( 'T', 'A', '5', ' ' );

  status = epos_task_create(
    Task_name[ 1 ],
    BASE_PRIORITY,
    RTEMS_MINIMUM_STACK_SIZE * 2,
    RTEMS_DEFAULT_MODES,
    RTEMS_DEFAULT_ATTRIBUTES,
    &Task_id[ 1 ]
  );
  directive_suc( status);

  status = epos_task_create(
    Task_name[ 2 ],
    BASE_PRIORITY,
    RTEMS_MINIMUM_STACK_SIZE,
    RTEMS_DEFAULT_MODES,
    RTEMS_DEFAULT_ATTRIBUTES,
    &Task_id[ 2 ]
  );
  directive_suc( status);

  status = epos_task_create(
    Task_name[ 3 ],
    BASE_PRIORITY,
    RTEMS_MINIMUM_STACK_SIZE,
    RTEMS_DEFAULT_MODES,
    RTEMS_DEFAULT_ATTRIBUTES,
    &Task_id[ 3 ]
  );
  directive_suc( status);

  status = epos_task_start( Task_id[ 1 ], Task_1, 0 );
  directive_suc( status);

  status = epos_task_start( Task_id[ 2 ], Task_2, 0 );
  directive_suc( status);

  status = epos_task_start( Task_id[ 3 ], Task_3, 0 );
  directive_suc( status);

  Region_name[ 1 ] = epos_build_name( 'R', 'N', '1', ' ' );
  Region_name[ 2 ] = epos_build_name( 'R', 'N', '2', ' ' );
  Region_name[ 3 ] = epos_build_name( 'R', 'N', '3', ' ' );
  Region_name[ 4 ] = epos_build_name( 'R', 'N', '4', ' ' );

  status = epos_region_create(
    Region_name[ 1 ],
    Area_1,
    4096,
    128,
    RTEMS_DEFAULT_ATTRIBUTES,
    &Region_id[ 1 ]
  );
  directive_suc( status);

  status = epos_region_create(
    Region_name[ 2 ],
    Area_2,
    4096,
    128,
    RTEMS_PRIORITY,
    &Region_id[ 2 ]
  );
  directive_suc( status);

  status = epos_region_create(
    Region_name[ 3 ],
    Area_3,
    4096,
    128,
    RTEMS_DEFAULT_ATTRIBUTES,
    &Region_id[ 3 ]
  );
  directive_suc( status);

  status = epos_region_create(
    Region_name[ 4 ],
    Area_4,
    4096,
    128,
    RTEMS_DEFAULT_ATTRIBUTES,
    &Region_id[ 4 ]
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

