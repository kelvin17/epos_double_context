/*
 *  Simple test program to demonstrate priority inversion when blocking
 *
   -Ulf Ivraeus ------------- --- -- -  -   -
 *  Saab Ericsson Space AB
 *  phone:  +46 (0)31 735 4542
 *  e-mail: ulf.ivraeus@space.se
 *  ----------------------------- --- -- -  -   -
 *
 *  $Id$
 */

/********************************************************************

  Semaphore Ids:
    Sync Mutex Id   = 0x18010009
    Local Mutex Id  = 0x1801000a
    Remote Mutex Id = 0x1801000b

  Task Ids:
    TaMedium[0] Id  = 0x08010002
    TaMedium[1] Id  = 0x08010003
    TaMedium[2] Id  = 0x08010004
    TaHigh Id       = 0x08010005
    TaLow Id        = 0x08010006
    TaHwSim Id      = 0x08010007

********************************************************************/

/********************************************************************/
/* define this to use the RTEMS 4.5 scheme for object names */
#define TEST_ON_RTEMS_45

/* define this to print the Id of the calling task */
//#define TEST_ON_TASK_ID

/* define this to print statistics when acquiring the mutex */
/* #define TEST_PRINT_STATISTICS */

/* define this if you are (1) on a ERC32 and (2) want a SW ISR trigger */
#if defined(__sparc__)
/* #define TEST_USE_ISR */
#endif

/* define this if you want the test to exit */
#define TEST_EXIT_AFTER_ITERATIONS 10
/********************************************************************/

#define CONFIGURE_INIT

#include <bsp.h>
#include <stdio.h>
#include "tmacros.h"

#if defined(TEST_EXIT_AFTER_ITERATIONS)
volatile uint32_t Iterations = 0;
#endif

/* Task entry point prototypes */
epos_task Medium_Exec(epos_task_argument TaskArg);
epos_task Low_Exec(epos_task_argument TaskArg);
epos_task High_Exec(epos_task_argument TaskArg);
epos_task LocalHwSim_Exec(epos_task_argument TaskArg);

/* ISR that signals that HW has completed */
epos_isr  LocalHwIsr(/*in*/ epos_vector_number   Vector);

/* Test driver functions */
void AccessLocalHw(void);
void AccessRemoteHw(void);

const char *CallerName()
{
  static char buffer[32];
#if defined(TEST_PRINT_TASK_ID)
  sprintk( buffer, "0x%08x -- %d",
      epos_task_self(), _Thread_Executing->current_priority );
#else
  union {
    uint32_t u;
    unsigned char c[4];
  } TempName;

  #if defined(TEST_ON_RTEMS_45)
    TempName.u = *(uint32_t *)_Thread_Executing->Object.name;
  #else
    TempName.u = (uint32_t) _Thread_Executing->Object.name.name_u32;
  #endif
  sprintk( buffer, "%c%c%c%c -- %d",
      TempName.c[0], TempName.c[1], TempName.c[2], TempName.c[3],
      _Thread_Executing->current_priority
  );
#endif
  return buffer;
}

#define NofMediumTask_C 3

/* RTEMS identifiers */                                               
epos_id  TaMedium[NofMediumTask_C]; /* Medium-prio tasks accessing */
                                     /*    the common local HW */
epos_id  TaHwSim;                   /* HW simulator */
epos_id  TaLow;                     /* Low-prio task accessing common */
                                     /*    remote HW */
epos_id  TaHigh;                    /* High-prio task accessing common */
                                     /*    remote HW */


epos_id  LocalHwSync_S;             /* Syncrhonize task to local HW */
epos_id  LocalHwAccess_R;           /* Execlusive access to the local HW */
epos_id  RemoteHwAccess_R;          /* Execlusive access to the remote HW */


/* The following variable triggers simulated HW activity */
volatile bool StartHw = false;

epos_task Medium_Exec(epos_task_argument TaskArg)
{
  printk("Medium_Exec (%d) begins...\n", TaskArg);

  epos_task_wake_after(50);

  while(1) {
    AccessLocalHw();
  }

  /* JRS - task does not get here */

  printk("Medium_Exec (%d) ends...\n", TaskArg);
  while(1) {
    epos_task_wake_after(10000);
  }
}

epos_task High_Exec(epos_task_argument TaskArg)
{
  printk("High_Exec (%d) begins...\n", TaskArg);

  /* Delay more than the Low-prio task so that Remote HW access resource is
   * taken before call to AccesRemoteHw.
   */
  epos_task_wake_after(250);

  while(1) {
    AccessRemoteHw();
  }

  /* JRS - task does not get here */

  printk("High_Exec (%d) ends...\n", TaskArg);
  while(1) {
    epos_task_wake_after(10000);
  }
  /* task does not get here */
}

epos_task Low_Exec(epos_task_argument TaskArg)
{
  printk("Low_Exec (%d) begins...\n", TaskArg);

  /* Delay less than High-prio task so that we take the remote HW access
   * resource before it does it. However, delay longer than the mid-prio
   * tasks so that we know for certain that they have taken control over
   * the local HW access.
   */
  epos_task_wake_after(100);

  while(1) {
    AccessRemoteHw();
  }

  /* JRS - task does not get here */

  printk("Low_Exec (%d) ends...\n", TaskArg);
  while(1) {
    epos_task_wake_after(10000);
  }
}


epos_task LocalHwSim_Exec(epos_task_argument TaskArg)
{
  int ISRCount = 0;
  printk("LocalHwSim_Exec begins...\n");
  
  while(1) {
    if (StartHw) {
      /* A test task has activated the HW, wait for a while and then
       * generate an interrupt
       */
      epos_task_wake_after(100);

      StartHw = FALSE;
#if 0
      fprintk( stderr, "StartHw -- fire ISR (%d)\n", ++ISRCount );
#endif
#if defined(TEST_USE_ISR)
      __asm__ volatile ("ta 5");
      __asm__ volatile ("nop");
      __asm__ volatile ("nop");
#else
      LocalHwIsr( 0x85 );
#endif
    }

  }

  printk("LocalHwSim_Exec ends...\n");
  while(1) {
    epos_task_wake_after(10000);
  }
}


epos_isr  LocalHwIsr(/*in*/ epos_vector_number   Vector)
{
  epos_status_code status;

  /* Signal synchroniztion semaphore to invoke waiting task */
  status = epos_semaphore_release(LocalHwSync_S);
  if (status != RTEMS_SUCCESSFUL) {
    fprintk( stderr, "LocalHwISR release %d\n", status );
    while(1); /* Error */
  }

  return;
}

void AccessLocalHw(void)
{
  epos_status_code     Sts;
    
  epos_task_priority   EnterPrio;   /* Statistics log */
  epos_task_priority   AccessPrio;  /*      :         */
  epos_task_priority   LeavePrio;   /*      :         */
  uint32_t              EnterCnt;    /*      :         */
  uint32_t              AccessCnt;   /*      :         */
  uint32_t              LeaveCnt;    /*      :         */

  /* Store information about the current situation */
  EnterPrio = _Thread_Executing->current_priority;
  EnterCnt  = _Thread_Executing->resource_count;


  printk("  AccessLocalHw called by %s\n", CallerName());

  /* Obtain exclusive access to local HW, Start HW, Wait for completion,
   * Release access
   */

  Sts = epos_semaphore_obtain(LocalHwAccess_R, RTEMS_WAIT, RTEMS_NO_TIMEOUT);
  directive_failed( Sts, "epos_semaphore_obtain(LocalHwAccess_R...)" );

  StartHw = TRUE;

  Sts = epos_semaphore_obtain(LocalHwSync_S, RTEMS_WAIT, RTEMS_NO_TIMEOUT);
  directive_failed( Sts, "epos_semaphore_obtain(LocalHwAccess_R...)" );

  /* Store information about the current situation */
  AccessPrio = _Thread_Executing->current_priority;
  AccessCnt  = _Thread_Executing->resource_count;

  Sts = epos_semaphore_release(LocalHwAccess_R);
  directive_failed( Sts, "epos_semaphore_release(LocalHwAccess_R)" );

  /* Store information about the current situation */
  LeavePrio = _Thread_Executing->current_priority;
  LeaveCnt  = _Thread_Executing->resource_count;

#if defined(TEST_PRINT_STATISTICS)
  printk(
    "  AccessLocalHw from %s statistics:\n"
    " - Prio: %d -> %d -> %d\n - Cnt: %d -> %d -> %d\n",
    CallerName(),
    EnterPrio, AccessPrio, LeavePrio,
    EnterCnt, AccessCnt, LeaveCnt
  );
#endif

  printk("  AccessLocalHw returns to %s\n", CallerName());
  #if defined(TEST_EXIT_AFTER_ITERATIONS)
    if ( ++Iterations == 10 ) {
      printk( "*** END OF TEST 35 ***" );
      exit(0);
    } 
  #endif
  return;
}

void AccessRemoteHw(void)
{
  epos_status_code     Sts;
    
  epos_task_priority   EnterPrio;   /* Statistics log */
  epos_task_priority   AccessPrio;  /*      :         */
  epos_task_priority   LeavePrio;   /*      :         */
  uint32_t              EnterCnt;    /*      :         */
  uint32_t              AccessCnt;   /*      :         */
  uint32_t              LeaveCnt;    /*      :         */

  /* Store information about the current situation */
  EnterPrio = _Thread_Executing->current_priority;
  EnterCnt  = _Thread_Executing->resource_count;


  printk("AccessRemoteHw called by %s\n", CallerName());

  /* Obtain exclusive access to remote HW, Start HW, Wait for completion,
   * Release access
   */

  Sts = epos_semaphore_obtain(RemoteHwAccess_R, RTEMS_WAIT, RTEMS_NO_TIMEOUT);
  directive_failed( Sts, "epos_semaphore_obtain(RemoteHwAccess_R...)" );

  /* Carry out the remote access via the Local HW interface */
  printk("AccessRemoteHw access local %s\n", CallerName());
  AccessLocalHw();

  /* Store information about the current situation */
  AccessPrio = _Thread_Executing->current_priority;
  AccessCnt  = _Thread_Executing->resource_count;

  Sts = epos_semaphore_release(RemoteHwAccess_R);
  directive_failed( Sts, "epos_semaphore_release(RemoreHwAccess_R" );

  /* Store information about the current situation */
  LeavePrio = _Thread_Executing->current_priority;
  LeaveCnt  = _Thread_Executing->resource_count;

#if defined(TEST_PRINT_STATISTICS)
  printk(
    "\nAccessRemoteHw from %s statistics:\n"
    " - Prio: %d -> %d -> %d\n - Cnt: %d -> %d -> %d\n",
    CallerName(),
    EnterPrio, AccessPrio, LeavePrio,
    EnterCnt, AccessCnt, LeaveCnt);
#endif

  printk("AccessRemoteHw returns to %s\n", CallerName());
  return;
}

/*************************************************************************/
/**********************        INITIALIZATION        *********************/
/*************************************************************************/

/* The Init operation (the Init-task) */
epos_task Init(epos_task_argument ignored)
{
  epos_status_code status;   
#if defined(TEST_USE_ISR)
  epos_isr_entry   DummyIsr; 
#endif
  int i;
  
  printk( "\n\n*** TEST 35 ***" );

  /* Create synchronisation semaphore for LocalHwIsr -> Test Tasks */
  status = epos_semaphore_create(
    epos_build_name ('S', 'Y', 'N', 'C'),           /* name */
    0,                                               /* initial count = 0 */
    RTEMS_LOCAL                   | 
    RTEMS_SIMPLE_BINARY_SEMAPHORE | 
    RTEMS_NO_INHERIT_PRIORITY     | 
    RTEMS_NO_PRIORITY_CEILING     | 
    RTEMS_FIFO,
    0,
    &LocalHwSync_S);                                 /* *id */
  directive_failed( status, "epos_semaphore_create (SYNC)" );
  
  printk( "Sync Mutex Id = 0x%08x\n", LocalHwSync_S );
  
  /* Create resource semaphore for exclusive access to the local HW */
  status = epos_semaphore_create(
    epos_build_name ('R', 'E', 'S', '1'), /* name             */
    1,                                     /* count            */
    RTEMS_PRIORITY         |
    RTEMS_BINARY_SEMAPHORE |
    RTEMS_INHERIT_PRIORITY |
    RTEMS_LOCAL,                           /* attribute_set    */
    1, /* insignificant */                 /* priority_ceiling */
    &LocalHwAccess_R);                     /* *id              */
  directive_failed( status, "epos_semaphore_create (RES1)" );

  printk( "Local Mutex Id = 0x%08x\n", LocalHwAccess_R );

  /* Create resource semaphore for exclusive access to the remote HW */
  status = epos_semaphore_create(
    epos_build_name ('R', 'E', 'S', '2'), /* name             */
    1,                                     /* count            */
    RTEMS_PRIORITY         |
    RTEMS_BINARY_SEMAPHORE |
    RTEMS_INHERIT_PRIORITY |
    RTEMS_LOCAL,                           /* attribute_set    */
    1, /* insignificant */                 /* priority_ceiling */
    &RemoteHwAccess_R);                    /* *id              */
  directive_failed( status, "epos_semaphore_create (RES2)" );

  printk( "Remote Mutex Id = 0x%08x\n", RemoteHwAccess_R );
  
#if defined(TEST_USE_ISR)
  /* Install ISR for HW/SW synchronization, use ta 0x85 which is synchronous */
  status = epos_interrupt_catch(LocalHwIsr, 0x85 + 0x100, &DummyIsr);
  directive_failed( status, "epos_interrupt_catch" );
#endif

  
  printk("Ending Init-task\n");
  /* Create and start all tasks in the test */

  /* -- Medium-prio Test Tasks --- */
  for (i = 0; i < NofMediumTask_C; i++) {
    status = epos_task_create(
      epos_build_name('M','E','D','0'+i),               /* Name */
      100,                                               /* Priority */
      RTEMS_MINIMUM_STACK_SIZE*2,                        /* Stack size (8KB) */
      RTEMS_DEFAULT_MODES | RTEMS_NO_ASR,                /* Mode */
      RTEMS_DEFAULT_ATTRIBUTES | RTEMS_FLOATING_POINT,   /* Attributes */
      &TaMedium[i]);                                     /* Assigned ID */
    directive_failed( status, "epos_task_create (MEDn)" );

    printk( "TaMedium[%d] Id = 0x%08x\n", i, TaMedium[i] );
    status = epos_task_start(TaMedium[i], Medium_Exec, i);
    directive_failed( status, "epos_task_start (MEDn)" );
  }

  /* -- High-prio Test Task --- */
  status = epos_task_create(
    epos_build_name('H','I','G','H'),                 /* Name */
    10,                                                /* Priority */
    RTEMS_MINIMUM_STACK_SIZE*2,                        /* Stack size (8KB) */
    RTEMS_DEFAULT_MODES | RTEMS_NO_ASR,                /* Mode */
    RTEMS_DEFAULT_ATTRIBUTES | RTEMS_FLOATING_POINT,   /* Attributes */
    &TaHigh);                                          /* Assigned ID */
  directive_failed( status, "epos_task_create (HIGH)" );

  printk( "TaHigh Id = 0x%08x\n", TaHigh );
  status = epos_task_start(TaHigh, High_Exec, 0);
  directive_failed( status, "epos_task_start (HIGH)" );

  /* -- Low-prio Test Task --- */
  status = epos_task_create(
    epos_build_name('L','O','W',' '),                 /* Name */
    200,                                               /* Priority */
    RTEMS_MINIMUM_STACK_SIZE*2,                        /* Stack size (8KB) */
    RTEMS_DEFAULT_MODES | RTEMS_NO_ASR,                /* Mode */
    RTEMS_DEFAULT_ATTRIBUTES | RTEMS_FLOATING_POINT,   /* Attributes */
    &TaLow);                                           /* Assigned ID */
  directive_failed( status, "epos_task_create (LOW)" );

  printk( "TaLow Id = 0x%08x\n", TaLow );
  status = epos_task_start(TaLow, Low_Exec, 0);
  directive_failed( status, "epos_task_start (LOW)" );

  /* -- HW Simulator Task --- */
  status = epos_task_create(
    epos_build_name('H','W','S','M'),                 /* Name */
    240,                                               /* Priority */
    RTEMS_MINIMUM_STACK_SIZE*2,                        /* Stack size (8KB) */
    RTEMS_DEFAULT_MODES | RTEMS_NO_ASR,                /* Mode */
    RTEMS_DEFAULT_ATTRIBUTES | RTEMS_FLOATING_POINT,   /* Attributes */
    &TaHwSim);                                         /* Assigned ID */
  directive_failed( status, "epos_task_create (HWSM)" );

  printk( "TaHwSim Id = 0x%08x\n", TaHwSim );

  status = epos_task_start(TaHwSim, LocalHwSim_Exec, 0);
  directive_failed( status, "epos_task_start (HWSM)" );

  /* Destroy the Init task (and let the ready tasks start running) */
  epos_task_delete(RTEMS_SELF);
}

/* configuration information */

//#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER

#define CONFIGURE_RTEMS_INIT_TASKS_TABLE

#define CONFIGURE_EXTRA_TASK_STACKS (RTEMS_MINIMUM_STACK_SIZE * 3)

#define CONFIGURE_MAXIMUM_TASKS 10
#define CONFIGURE_MAXIMUM_SEMAPHORES 10

#include <rtems/confdefs.h>

/* end of file */
