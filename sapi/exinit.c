/*
 *  Initialization Manager
 *
 *  COPYRIGHT (c) 1989-2008.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: exinit.c,v 1.53 2009/08/28 18:26:05 joel Exp $
 */
#include <stdio.h>
#if HAVE_CONFIG_H
#include "config.h"
#endif
 
/*
 *  SCORE_INIT and SAPI_INIT are defined so all of the super core and
 *  super API data will be included in this object file.
 */

#define SAPI_INIT
#define SCORE_INIT

#include <rtems/system.h>
#include <rtems/config.h>
#ifdef RTEMS_DEBUG
#include <rtems/debug.h>
#endif
#include <rtems/extension.h>
#include <rtems/fatal.h>
#include <rtems/init.h>
#include <rtems/io.h>
#include <rtems/rtemsapi.h>
#include <rtems/sptables.h>

#include <rtems/score/sysstate.h>
#include <rtems/score/apiext.h>
#include <rtems/score/apimutex.h>
#include <rtems/score/copyrt.h>
#include <rtems/score/heap.h>
#include <rtems/score/interr.h>
#include <rtems/score/isr.h>
#include <rtems/score/interrupts.h>
#include <rtems/score/priority.h>
#include <rtems/score/thread.h>
#include <rtems/score/tod.h>
#include <rtems/score/userext.h>
#include <rtems/score/watchdog.h>
#include <rtems/score/wkspace.h>

#ifdef RTEMS_POSIX_API
  #include <rtems/posix/posixapi.h>
#endif

Objects_Information *_Internal_Objects[ OBJECTS_INTERNAL_CLASSES_LAST + 1 ];

void epos_initialize_data_structures(void)
{
  /*
   *  Dispatching and interrupts are disabled until the end of the
   *  initialization sequence.  This prevents an inadvertent context
   *  switch before the executive is initialized.
   *
   *  WARNING: Interrupts should have been disabled by the BSP and
   *           are disabled by boot_card().
   */


    _System_state_Handler_initialization( FALSE );


  /*
   * Initialize any target architecture specific support as early as possible
   */
//  _CPU_Initialize();


  /*
   *  Do this as early as possible to ensure no debugging output
   *  is even attempted to be printed.
   */
 // _Debug_Manager_initialization();

  _API_extensions_Initialization();
	
  _Thread_Dispatch_initialization();
   

  /*
   *  Before this is called, we are not allowed to allocate memory
   *  from the Workspace because it is not initialized.
   */
  _Workspace_Handler_initialization();

  _User_extensions_Handler_initialization();
  _ISR_Handler_initialization();

  /*
   * Initialize the internal support API and allocator Mutex
   */
  _Objects_Information_table[OBJECTS_INTERNAL_API] = _Internal_Objects;

  _API_Mutex_Initialization( 1 );
  _API_Mutex_Allocate( &_RTEMS_Allocator_Mutex );

  _Priority_Handler_initialization();
  _Watchdog_Handler_initialization();
  _TOD_Handler_initialization();

  _Thread_Handler_initialization();

/* MANAGERS */

  _RTEMS_API_Initialize();

  _Extension_Manager_initialization();

  _IO_Manager_initialization();

  #ifdef RTEMS_POSIX_API
    _POSIX_API_Initialize();
  #endif

  _System_state_Set( SYSTEM_STATE_BEFORE_MULTITASKING );

  /*
   *  No threads should be created before this point!!!
   *  _Thread_Executing and _Thread_Heir are not set.
   *
   *  At this point all API extensions are in place.  After the call to
   *  _Thread_Create_idle() _Thread_Executing and _Thread_Heir will be set.
   */
   
  _Thread_Create_idle();
  

  /*
   *  Scheduling can properly occur now as long as we avoid dispatching.
   */
}

void epos_initialize_before_drivers(void)
{
  #if defined(FUNCTIONALITY_NOT_CURRENTLY_USED_BY_ANY_API)
    /*
     *  Run the API and BSPs predriver hook.
     */
    _API_extensions_Run_predriver();
  #endif
}

void epos_initialize_device_drivers(void)
{
  /*
   *  Initialize all the device drivers and initialize the MPCI layer.
   *
   *  NOTE:  The MPCI may be build upon a device driver.
   */

  _IO_Initialize_all_drivers();

  /*
   *  Run the APIs and BSPs postdriver hooks.
   *
   *  The API extensions are supposed to create user initialization tasks.
   */
 _API_extensions_Run_postdriver();
}

void epos_initialize_start_multitasking(void)
{
  _System_state_Set( SYSTEM_STATE_BEGIN_MULTITASKING );

  isr_installc(SIGSWI, NULL);
  init_swi(SIGSWI);

  _Thread_Start_multitasking();

  /*******************************************************************
   *******************************************************************
   *******************************************************************
   ******                 APPLICATION RUNS HERE                 ******
   ******            RETURNS WHEN SYSTEM IS SHUT DOWN           ******
   *******************************************************************
   *******************************************************************
   *******************************************************************/
}


/*PAGE
 *
 *  rtems_shutdown_executive
 *
 *  This kernel routine shutdowns the executive.  It halts multitasking
 *  and returns control to the application execution "thread" which
 *  initialially invoked the rtems_initialize_executive directive.
 *
 *  Input parameters:   NONE
 *
 *  Output parameters:  NONE
 */

void epos_shutdown_executive(
   uint32_t result
)
{
  if ( _System_state_Get() != SYSTEM_STATE_SHUTDOWN ) {
    _System_state_Set( SYSTEM_STATE_SHUTDOWN );
    _Thread_Stop_multitasking();
  }
}

