/**
 * @file rtems/malloc.h
 */

/*
 *  RTEMS Malloc Extensions
 *
 *  COPYRIGHT (c) 1989-2007.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may in
 *  the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 * $Id: malloc.h,v 1.9.2.1 2010/06/21 21:13:30 joel Exp $
 */

#ifndef _RTEMS_MALLOC_H
#define _RTEMS_MALLOC_H


#include <rtems.h>
#include <rtems/bspIo.h>

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 *  Malloc Statistics Structure
 */
typedef struct {
    uint32_t    space_available;             /* current size of malloc area */
    uint32_t    malloc_calls;                /* # calls to malloc */
    uint32_t    memalign_calls;              /* # calls to memalign */
    uint32_t    free_calls;
    uint32_t    realloc_calls;
    uint32_t    calloc_calls;
    uint32_t    max_depth;		     /* most ever malloc'd at 1 time */
    uintmax_t   lifetime_allocated;
    uintmax_t   lifetime_freed;
} epos_malloc_statistics_t;

/*
 *  Malloc statistics plugin
 */
typedef struct {
  void (*initialize)(void);
  void (*at_malloc)(void *);
  void (*at_free)(void *);
} epos_malloc_statistics_functions_t;

extern epos_malloc_statistics_functions_t
  epos_malloc_statistics_helpers_table;
extern epos_malloc_statistics_functions_t *epos_malloc_statistics_helpers;

/*
 *  Malloc boundary support plugin
 */
typedef struct {
  void     (*initialize)(void);
  uint32_t (*overhead)(void);
  void     (*at_malloc)(void *, size_t);
  void     (*at_free)(void *);
  void     (*at_realloc)(void *, size_t);
} epos_malloc_boundary_functions_t;

extern epos_malloc_boundary_functions_t epos_malloc_boundary_helpers_table;
extern epos_malloc_boundary_functions_t *epos_malloc_boundary_helpers;

/*
 *  Malloc Heap Extension (sbrk) plugin
 */
typedef struct {
  void *(*initialize)(void *, size_t);
  void *(*extend)(size_t);
} epos_malloc_sbrk_functions_t;

extern epos_malloc_sbrk_functions_t epos_malloc_sbrk_helpers_table;
extern epos_malloc_sbrk_functions_t *epos_malloc_sbrk_helpers;

/*
 * Malloc Plugin to Dirty Memory at Allocation Time
 */
typedef void (*epos_malloc_dirtier_t)(void *, size_t);
extern epos_malloc_dirtier_t epos_malloc_dirty_helper;

/** @brief Dirty memory function
 *
 *  This method fills the specified area with a non-zero pattern
 *  to aid in debugging programs which do not initialize their
 *  memory allocated from the heap.
 */
void epos_malloc_dirty_memory(
  void   *start,
  size_t  size
);

/** @brief Print Malloc Statistic Usage Report
 *
 *  This method fills in the called provided malloc statistics area.
 *
 *  @return This method returns 0 if successful and -1 on error.
 */
int malloc_get_statistics(
  epos_malloc_statistics_t *stats
);

/** @brief Print Malloc Statistic Usage Report
 *
 *  This method prints a malloc statistics report.
 *
 *  @note It uses printk to print the report.
 */
void malloc_report_statistics(void);

/** @brief Print Malloc Statistic Usage Report
 *
 *  This method prints a malloc statistics report.
 *
 *  @param[in] context is the context to pass to the print handler
 *  @param[in] print is the print handler
 *
 *  @note It uses the CALLER's routine to print the report.
 */
void malloc_report_statistics_with_plugin(
  void                  *context,
  epos_printk_plugin_t  print
);

/**
 *
 *  This method is a help memalign implementation which does all
 *  error checking done by posix_memalign() EXCEPT it does NOT
 *  place numeric restrictions on the alignment value.
 *
 *  @param[in] pointer points to the user pointer
 *  @param[in] alignment is the desired alignment
 *  @param[in] size is the allocation request size in bytes
 *
 *  @return This methods returns zero on success and a POSIX errno
 *          value to indicate the failure condition.  On success
 *          *pointer will contain the address of the allocated memory.
 */
int epos_memalign(
  void   **pointer,
  size_t   alignment,
  size_t   size
);

/**
 * @brief Allocates a memory area of size @a size bytes from the heap.
 *
 * If the alignment parameter @a alignment is not equal to zero, the allocated
 * memory area will begin at an address aligned by this value.
 *
 * If the boundary parameter @a boundary is not equal to zero, the allocated
 * memory area will fulfill a boundary constraint.  The boundary value
 * specifies the set of addresses which are aligned by the boundary value.  The
 * interior of the allocated memory area will not contain an element of this
 * set.  The begin or end address of the area may be a member of the set.
 *
 * A size value of zero will return a unique address which may be freed with
 * free().
 *
 * The memory allocated by this function can be released with a call to free().
 *
 * @return A pointer to the begin of the allocated memory area, or @c NULL if
 * no memory is available or the parameters are inconsistent.
 */
void *epos_heap_allocate_aligned_with_boundary(
  size_t size,
  uintptr_t alignment,
  uintptr_t boundary
);

#ifdef __cplusplus
}
#endif

#endif
