/*
 *  COPYRIGHT (c) 1989-2008.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  $Id: bsplibc.c,v 1.15 2009/09/08 13:35:07 joel Exp $
 */

#include <rtems.h>
#include <rtems/libio.h>
#include <rtems/libcsupport.h>

#include <bsp/bootcard.h>


void bsp_libc_init(
  void *heap_begin,
  uintptr_t heap_size,
  size_t sbrk_amount
)
{
    RTEMS_Malloc_Initialize( heap_begin, heap_size, sbrk_amount );

    /*
     *  Init the RTEMS libio facility to provide UNIX-like system
     *  calls for use by newlib (ie: provide open, close, etc)
     *  Uses malloc() to get area for the iops, so must be after malloc init
     */
    if (epos_libio_init_helper)
	(*epos_libio_init_helper)();
	
#if defined(RTEMS_NEWLIB)
    /*
     * Set up for the libc handling.
     */
    libc_init();
#endif
}
