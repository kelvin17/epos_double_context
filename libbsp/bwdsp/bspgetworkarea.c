/*
 *  This routine is an implementation of the bsp_get_work_area()
 *  that can be used by all BSPs following linkcmds conventions
 *  regarding heap, stack, and workspace allocation.
 *
 *  COPYRIGHT (c) 1989-2008.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: bspgetworkarea.c,v 1.7 2009/11/30 04:55:42 ralf Exp $
 */

/* #define BSP_GET_WORK_AREA_DEBUG */

#include <bsp.h>
#include <bsp/bootcard.h>
#ifdef BSP_GET_WORK_AREA_DEBUG
  #include <rtems/bspIo.h>
#endif

/*
 *  These are provided by the linkcmds for ALL of the BSPs which use this file.
 */
extern uintptr_t rtemsWorkAreaStart;

//extern char HeapSize[];


/*
 *  This method returns the base address and size of the area which
 *  is to be allocated between the RTEMS Workspace and the C Program
 *  Heap.
 */
void bsp_get_work_area(
  void      **work_area_start,
  uintptr_t  *work_area_size,
  void      **heap_start,
  uintptr_t  *heap_size
)
{
   *work_area_start = (void *) rtemsWorkAreaStart;
 // *work_area_size  = 0;
  *heap_start      = (void *)0x610000;//modified by zwj
 // *heap_size       = (uintptr_t) HeapSize;

 /* 

  *work_area_start=(void*)0x400000;
  *work_area_size= (uintptr_t) 0x40000; 
	
  void		**heap_start;
  *heap_start=(void*) 0x600000;
   uintptr_t  *heap_size;
  *heap_size =(uintptr_t) 0x40000

  *work_area_start = WorkAreaBase;
  *work_area_size  = ram_end - (uintptr_t) WorkAreaBase;
  *heap_start      = BSP_BOOTCARD_HEAP_USES_WORK_AREA;
  *heap_size       = (uintptr_t) HeapSize;

  /*
   *  The following may be helpful in debugging what goes wrong when
   *  you are allocating the Work Area in a new BSP.
   */


  #ifdef BSP_GET_WORK_AREA_DEBUG
    {
      void *sp = __builtin_frame_address(0);
      void *end = *work_area_start + *work_area_size;
      printk(
        "work_area_start = 0x%p\n"
        "work_area_size = %d 0x%08x\n"
        "end = 0x%p\n"
        "heap_start = 0x%p\n"
        "heap_size = %d\n"
        "current stack pointer = 0x%p%s\n",
        *work_area_start,
        *work_area_size,  /* decimal */
        *work_area_size,  /* hexadecimal */
        end,
        *heap_start,
        *heap_size,
        sp,
        ((sp >= *work_area_start && sp <= end) ? " OVERLAPS!" : "")
     );
  }
 #endif
}
