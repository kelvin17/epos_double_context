#ifdef _OCC_NO_INLINE_
#include <rtems/system.h>
#include <rtems/score/protectedheap.h>
#endif

#ifndef _RTEMS_SCORE_PROTECTED_HEAP_H
# error "Never use "protectedheap.inl" directly; include "protectedheap.h" instead."
#endif

#ifndef _RTEMS_SCORE_PROTECTEDHEAP_INL
#define _RTEMS_SCORE_PROTECTEDHEAP_INL


/**
 * @brief See _Heap_Initialize().
 */
RTEMS_INLINE_ROUTINE uintptr_t _Protected_heap_Initialize(
  Heap_Control *heap,
  void *area_begin,
  uintptr_t area_size,
  uintptr_t page_size
)
{
  return _Heap_Initialize( heap, area_begin, area_size, page_size );
}

/**
 * @brief See _Heap_Allocate_aligned_with_boundary() with boundary equals zero.
 */
RTEMS_INLINE_ROUTINE void *_Protected_heap_Allocate_aligned(
  Heap_Control *heap,
  uintptr_t size,
  uintptr_t alignment
)
{
  return
    _Protected_heap_Allocate_aligned_with_boundary( heap, size, alignment, 0 );
}

/**
 * @brief See _Heap_Allocate_aligned_with_boundary() with alignment and
 * boundary equals zero.
 */
RTEMS_INLINE_ROUTINE void *_Protected_heap_Allocate(
  Heap_Control *heap,
  uintptr_t size
)
{
  return _Protected_heap_Allocate_aligned_with_boundary( heap, size, 0, 0 );
}

#endif
