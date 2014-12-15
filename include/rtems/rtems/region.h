/**
 * @file region.h
 *
 * @ingroup ClassicRegion
 *
 *  This include file contains all the constants and structures associated
 *  with the Region Manager.  This manager provides facilities to dynamically
 *  allocate memory in variable sized units which are returned as segments.
 *
 *  Directives provided are:
 *
 *     - create a region
 *     - get an ID of a region
 *     - delete a region
 *     - get a segment from a region
 *     - return a segment to a region
 */

/*  COPYRIGHT (c) 1989-2009.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: region.h,v 1.35 2009/12/15 18:26:41 humph Exp $
 */

#ifndef _RTEMS_RTEMS_REGION_H
#define _RTEMS_RTEMS_REGION_H

#include <rtems/score/object.h>
#include <rtems/score/threadq.h>
#include <rtems/score/heap.h>
#ifdef RTEMS_DEBUG
#include <rtems/debug.h>
#endif
#include <rtems/rtems/attr.h>
#include <rtems/rtems/options.h>
#include <rtems/rtems/status.h>
#include <rtems/rtems/support.h>
#include <rtems/rtems/types.h>

/**
 *  @defgroup ClassicRegion Regions
 *
 *  @ingroup ClassicRTEMS
 *
 *  @brief RTEMS内存管理服务模块之动态区域管理：动态划分。
 *
 *  This encapsulates functionality which XXX
 */
/**@{*/

/**
 *  This constant is defined to extern most of the time when using
 *  this header file.  However by defining it to nothing, the data
 *  declared in this header file can be instantiated.  This is done
 *  in a single per manager file.
 */
#ifndef RTEMS_REGION_EXTERN
#define RTEMS_REGION_EXTERN extern
#endif

#ifdef __cplusplus
extern "C" {
#endif

/**
 *  The following records define the control block used to manage
 *  each region.
 */

typedef struct {
  Objects_Control       Object;
  Thread_queue_Control  Wait_queue;            /* waiting threads        */
  void                 *starting_address;      /* physical start addr    */
  uintptr_t             length;                /* physical length(bytes) */
  uintptr_t             page_size;             /* in bytes               */
  uintptr_t             maximum_segment_size;  /* in bytes               */
  epos_attribute       attribute_set;
  uint32_t              number_of_used_blocks; /* blocks allocated       */
  Heap_Control          Memory;
}  Region_Control;

/**
 *  The following defines the information control block used to
 *  manage this class of objects.
 */
RTEMS_REGION_EXTERN Objects_Information _Region_Information;

/**
 *  @brief _Region_Manager_initialization
 *
 *  This routine performs the initialization necessary for this manager.
 */
void _Region_Manager_initialization(void);

/**
 *  @brief epos_region_create
 *
 *  This routine implements the epos_region_create directive.  The
 *  region will have the name name.  The memory area managed by
 *  the region is of length bytes and starts at starting_address.
 *  The memory area will be divided into as many allocatable units of
 *  page_size bytes as possible.   The attribute_set determines which
 *  thread queue discipline is used by the region.  It returns the
 *  id of the created region in ID.
 */
epos_status_code epos_region_create(
  epos_name          name,
  void               *starting_address,
  uintptr_t           length,
  uintptr_t           page_size,
  epos_attribute     attribute_set,
  epos_id           *id
);

/**
 *  @brief epos_region_extend
 *
 *  This routine implements the epos_region_extend directive.  The
 *  region will have the name name.  The memory area managed by
 *  the region will be attempted to be grown by length bytes using
 *  the memory starting at starting_address.
 */
epos_status_code epos_region_extend(
  epos_id            id,
  void               *starting_address,
  uintptr_t           length
);

/**
 *  @brief epos_region_ident
 *
 *  This routine implements the epos_region_ident directive.
 *  This directive returns the region ID associated with name.
 *  If more than one region is named name, then the region
 *  to which the ID belongs is arbitrary.
 */
epos_status_code epos_region_ident(
  epos_name    name,
  epos_id     *id
);

/**
 *  @brief epos_region_get_information
 *
 *  This routine implements the epos_region_get_information directive.
 *  This directive returns information about the heap associated with
 *  this region.
 */
epos_status_code epos_region_get_information(
  epos_id                id,
  Heap_Information_block *the_info
);

/**
 *  @brief epos_region_get_free_information
 *
 *  This routine implements the epos_region_get_free_information directive.
 *  This directive returns information about the free blocks in the
 *  heap associated with this region.
 */
epos_status_code epos_region_get_free_information(
  epos_id                id,
  Heap_Information_block *the_info
);

/**
 *  @brief epos_region_delete
 *
 *  This routine implements the epos_region_delete directive.  The
 *  region indicated by ID is deleted.
 */
epos_status_code epos_region_delete(
  epos_id   id
);

/**
 *  @brief epos_region_get_segment
 *
 *  This routine implements the epos_region_get_segment directive.  It
 *  attempts to allocate a segment from the region associated with ID.
 *  If a segment of the requested size can be allocated, its address
 *  is returned in segment.  If no segment is available, then the task
 *  may return immediately or block waiting for a segment with an optional
 *  timeout of timeout clock ticks.  Whether the task blocks or returns
 *  immediately is based on the no_wait option in the option_set.
 */
epos_status_code epos_region_get_segment(
  epos_id           id,
  uintptr_t          size,
  epos_option       option_set,
  epos_interval     timeout,
  void             **segment
);

/**
 *  @brief epos_region_get_segment_size
 *
 *  This routine implements the epos_region_get_segment_size directive.  It
 *  returns the size in bytes of the specified user memory area.
 */
epos_status_code epos_region_get_segment_size(
  epos_id           id,
  void              *segment,
  uintptr_t         *size
);

/**
 *  @brief epos_region_return_segment
 *
 *  This routine implements the epos_region_return_segment directive.  It
 *  frees the segment to the region associated with ID.  The segment must
 *  have been previously allocated from the same region.  If freeing the
 *  segment results in enough memory being available to satisfy the
 *  epos_region_get_segment of the first blocked task, then that task and as
 *  many subsequent tasks as possible will be unblocked with their requests
 *  satisfied.
 */
epos_status_code epos_region_return_segment(
  epos_id    id,
  void       *segment
);

/**
 *  @brief epos_region_resize_segment
 *
 *  This routine implements the epos_region_resize_segment directive.  It
 *  tries to resize segment in the region associated with 'id' to the new size
 *  'size' in place. The first 'size' or old size bytes of the segment
 *  (whatever is less) are guaranteed to remain unmodified. The segment must
 *  have been previously allocated from the same region.  If resizing the
 *  segment results in enough memory being available to satisfy the
 *  epos_region_get_segment of the first blocked task, then that task and as
 *  many subsequent tasks as possible will be unblocked with their requests
 *  satisfied.
 *  Returns:
 *    RTEMS_SUCCESSFUL  - operation successful
 *    RTEMS_UNSATISFIED - the segment can't be resized in place
 *    any other code    - failure.
 *  On RTEMS_SUCCESSFUL or RTEMS_UNSATISFIED exit it returns into the
 *  'old_size' the old size in bytes of the user memory area of the specified
 *  segment.
 */
epos_status_code epos_region_resize_segment(
  epos_id    id,
  void       *segment,
  uintptr_t   size,
  uintptr_t  *old_size
);

#ifndef __RTEMS_APPLICATION__
#ifndef _OCC_NO_INLINE_
#include <rtems/rtems/region.inl>
#endif

#ifdef _OCC_NO_INLINE_
RTEMS_INLINE_ROUTINE Region_Control *_Region_Allocate( void );
RTEMS_INLINE_ROUTINE void _Region_Free (
  Region_Control *the_region
);
RTEMS_INLINE_ROUTINE Region_Control *_Region_Get (
  Objects_Id         id,
  Objects_Locations *location
);
RTEMS_INLINE_ROUTINE void *_Region_Allocate_segment (
  Region_Control *the_region,
  uintptr_t       size
);
RTEMS_INLINE_ROUTINE bool _Region_Free_segment (
  Region_Control *the_region,
  void           *the_segment
);
RTEMS_INLINE_ROUTINE bool _Region_Is_null (
  Region_Control *the_region
);


#endif
/**
 *  @brief Region_Process_queue
 *
 *  This is a helper routine which is invoked any time memory is
 *  freed.  It looks at the set of waiting tasks and attempts to
 *  satisfy all outstanding requests.
 */
extern void _Region_Process_queue(Region_Control *the_region);

#endif

/**
 *  @brief _Region_Debug_Walk
 *
 *  This routine is invoked to verify the integrity of a heap associated
 *  with the_region.
 */
#ifdef RTEMS_DEBUG

#define _Region_Debug_Walk( _the_region, _source ) \
  do { \
    if ( epos_debug_is_enabled( RTEMS_DEBUG_REGION ) ) \
      _Heap_Walk( &(_the_region)->Memory, _source, false ); \
  } while ( 0 )

#else

#define _Region_Debug_Walk( _the_region, _source )

#endif

#ifdef __cplusplus
}
#endif

/**@}*/

#endif
/* end of include file */
