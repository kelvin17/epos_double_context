
#include <bsp/bootcard.h>

#include <rtems.h>
#include <rtems/bspIo.h>
#include <rtems/malloc.h>
#include <rtems/score/interrupts.h>

#ifdef CONFIGURE_MALLOC_BSP_SUPPORTS_SBRK
#include <unistd.h> /* for sbrk() */
#endif

//uint8_t epos_maximum_priority=255; 

//uint32_t epos_minimum_stack_size=4096;

const char *bsp_boot_cmdline;
extern void init_swi(int);
 
void _CPU_Context_save_fp(
  Context_Control_fp **fp_context_ptr
){}
void _CPU_Context_restore_fp(
  Context_Control_fp **fp_context_ptr
){}


/*
 * Are we using a single heap for the RTEMS Workspace and C Program Heap?
 */
extern bool epos_unified_work_area;

//uint32_t epos_minimum_stack_size=4096;
//System_state_Codes _System_state_Current=SYSTEM_STATE_BEFORE_INITIALIZATION;

 
//void printk(const char *fmt, ...){}
//int getchark(void){}

// void vprintk(const char *fmt, va_list ap){}//libc.a realized this function

 //void putk(const char *s){}
/* */
 //void epos_libio_init(void){}
// void	 open_dev_console(void){}
 //void epos_filesystem_initialize(void){}
/* note by zwj
 bool newlib_create_hook(
   epos_tcb *current_task,
   epos_tcb *creating_task
 ){}
 
#define __RTEMS_NEWLIB_BEGIN 0
 
 void newlib_delete_hook(
   epos_tcb *current_task,
   epos_tcb *deleted_task
 ){}
*/




 static void bootcard_bsp_libc_helper(
   void 	 *work_area_start,
   uintptr_t  work_area_size,
   void 	 *heap_start,
   uintptr_t  heap_size,
   uintptr_t  sbrk_amount
 )
 {
 /*
   if ( heap_start == BSP_BOOTCARD_HEAP_USES_WORK_AREA ) {
	 if ( ! epos_unified_work_area ) {
	   uintptr_t work_space_size = epos_configuration_get_work_space_size();
 
	   heap_start = (char *) work_area_start + work_space_size;
 
	   if (heap_size == BSP_BOOTCARD_HEAP_SIZE_DEFAULT) {
		 uintptr_t heap_size_default = work_area_size - work_space_size;
 
		 heap_size = heap_size_default;
	   }
	 } else {
	   heap_start = work_area_start;
	   if (heap_size == BSP_BOOTCARD_HEAP_SIZE_DEFAULT) {
		 heap_size = work_area_size;
	   }
	 }
   }
   */
   //added by zwj
     uintptr_t work_space_size = Configuration.work_space_size;
 
	   heap_start = (char *) work_area_start + work_space_size;

	uintptr_t heap_size_default = work_area_size - work_space_size;
 
		 heap_size = heap_size_default;
 
   bsp_libc_init(heap_start, heap_size, sbrk_amount);
 }
 


int bootcard(
  const char *cmdline
)
{
  // epos_interrupt_level  bsp_isr_level;
  void                  *work_area_start = NULL;
  uintptr_t              work_area_size = 0;
  void                  *heap_start = NULL;
  uintptr_t              heap_size = 0;
  uintptr_t              sbrk_amount = 0;

	int level;
	_ISR_Disable(level);
	//printk("EPOS is running...\n");
	_ISR_Enable(level);

	
  /*
   *  Make sure interrupts are disabled.
   */
//  epos_interrupt_disable( bsp_isr_level );

  bsp_boot_cmdline = cmdline;

  /*
   * Invoke Board Support Package initialization routine written in C.
   */
  //bsp_start();

  /*
   *  Find out where the block of memory the BSP will use for
   *  the RTEMS Workspace and the C Program Heap is.
   */
  //bsp_get_work_area(&work_area_start, &work_area_size,
   //                 &heap_start, &heap_size);


 // if ( work_area_size <= Configuration.work_space_size ) {
   /* printk(
      "bootcard: work space too big for work area: %p > %p\n",
      (void *) Configuration.work_space_size,
      (void *) work_area_size
    );*/
 //   bsp_cleanup();
 //   return -1;
 // }

// if ( epos_unified_work_area ) {
//    Configuration.work_space_start = work_area_start;
//    Configuration.work_space_size  = work_area_size;
//  } else {
    Configuration.work_space_start = (void*)0x800000;
    Configuration.work_space_B_start = (void*)0xC00000;
//  }
    Configuration.work_space_size=0x36000;
    Configuration.work_space_B_size=0x36000;

  #if (BSP_DIRTY_MEMORY == 1)
	printk("bsp_dirty_memory==1\n");
    memset( work_area_start, 0xCF,  work_area_size );
  #endif

  /*
   *  Initialize RTEMS data structures
   */
	//printk("initializing data structures...\n");
	epos_initialize_data_structures();
	

  /*
   *  Initialize the C library for those BSPs using the shared
   *  framework.
   */
 // bsp_libc_init(heap_start, heap_size, sbrk_amount);
//added by zwj
  work_area_start=Configuration.work_space_start;
 work_area_size = 0x40000;
 bootcard_bsp_libc_helper(
 work_area_start,
 work_area_size,
 heap_start,
 heap_size,
 sbrk_amount
 );

  /*
   *  All BSP to do any required initialization now that RTEMS
   *  data structures are initialized.  In older BSPs or those
   *  which do not use the shared framework, this is the typical
   *  time when the C Library is initialized so malloc()
   *  can be called by device drivers.  For BSPs using the shared
   *  framework, this routine can be empty.
   */
 // bsp_pretasking_hook();

  /*
   *  If debug is enabled, then enable all dynamic RTEMS debug
   *  capabilities.
   *
   *  NOTE: Most debug features are conditionally compiled in
   *        or enabled via configure time plugins.
   */
  #ifdef RTEMS_DEBUG
    epos_debug_enable( RTEMS_DEBUG_ALL_MASK );
  #endif

  /*
   *  Let RTEMS perform initialization it requires before drivers
   *  are allowed to be initialized.
   */
epos_initialize_before_drivers();
 
  /*
   *  Execute BSP specific pre-driver hook. Drivers haven't gotten
   *  to initialize yet so this is a good chance to initialize
   *  buses, spurious interrupt handlers, etc..
   *
   *  NOTE: Many BSPs do not require this handler and use the
   *        shared stub.
   */
//  bsp_predriver_hook();

  /*
   *  Initialize all device drivers.
   */
  
  epos_initialize_device_drivers();

  /*
   *  Invoke the postdriver hook.  This normally opens /dev/console
   *  for use as stdin, stdout, and stderr.
   */
  bsp_postdriver_hook();
  isr_installc(SIGSWI, NULL);
  init_swi(SIGSWI);
  /*
   *  Complete initialization of RTEMS and switch to the first task.
   *  Global C++ constructors will be executed in the context of that task.
   */
  epos_initialize_start_multitasking();

  /***************************************************************
   ***************************************************************
   *  APPLICATION RUNS HERE!!!  When it shuts down, we return!!! *
   ***************************************************************
   ***************************************************************
   */

  /*
   *  Perform any BSP specific shutdown actions which are written in C.
   */
  bsp_cleanup();

  /*
   *  Now return to the start code.
   */
  return 0;
}
