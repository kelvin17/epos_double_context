/**
 * @file
 *
 * @ingroup ClassicRTEMS
 *
 * @ingroup ClassicRTEMSWorkspace
 *
 * @brief Classic API support.
 */

/*  COPYRIGHT (c) 1989-2008.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: support.h,v 1.28 2009/08/05 18:17:12 joel Exp $
 */

#ifndef _RTEMS_RTEMS_SUPPORT_H
#define _RTEMS_RTEMS_SUPPORT_H

#ifdef __cplusplus
extern "C" {
#endif

#include <rtems/rtems/types.h>

/**
 * @addtogroup ClassicRTEMS
 *
 * @{
 */

/**
 * @brief Returns the number of micro seconds for the milli seconds value @a _ms.
 */
#define RTEMS_MILLISECONDS_TO_MICROSECONDS(_ms) \
        TOD_MILLISECONDS_TO_MICROSECONDS(_ms)

/**
 * @brief Returns the number of ticks for the milli seconds value @a _ms.
 */
#define RTEMS_MILLISECONDS_TO_TICKS(_ms) \
       (TOD_MILLISECONDS_TO_MICROSECONDS(_ms) / \
          epos_configuration_get_microseconds_per_tick())

/**
 * @brief Returns the number of ticks for the micro seconds value @a _us.
 */
#define RTEMS_MICROSECONDS_TO_TICKS(_us) \
       ((_us) / epos_configuration_get_microseconds_per_tick())

/** @} */

/**
 * @defgroup ClassicRTEMSWorkspace Workspace
 *
 * @ingroup ClassicRTEMS
 *
 *  @brief RTEMS Workspace(工作区：对象动态管理区域)管理服务模块。
 *
 * Workspace definitions.
 *
 * @{
 */

/**
 * @brief Gets workspace information.
 *
 * Returns information about the heap that is used as the RTEMS Executive
 * Workspace in @a the_info.
 *
 * Returns @c true if successful, and @a false otherwise.
 */
bool epos_workspace_get_information(
  Heap_Information_block  *the_info
);

/**
 * @brief Allocates memory from the workspace.
 *
 * A number of @a bytes bytes will be allocated from the RTEMS Executive
 * Workspace and returned in @a pointer.
 *
 * Returns @c true if successful, and @a false otherwise.
 */
bool epos_workspace_allocate(
  size_t   bytes,
  void   **pointer
);

/**
 * @brief Frees memory allocated from the workspace.
 *
 * This frees the memory indicated by @a pointer that was allocated from the
 * RTEMS Executive Workspace.
 *
 * Returns @c true if successful, and @a false otherwise.
 */
bool epos_workspace_free(
  void *pointer
);

/** @} */

#ifndef __RTEMS_APPLICATION__
#ifndef _OCC_NO_INLINE_
#include <rtems/rtems/support.inl>
#endif

#ifdef _OCC_NO_INLINE_
RTEMS_INLINE_ROUTINE bool epos_is_name_valid (
  epos_name name
);

RTEMS_INLINE_ROUTINE void epos_name_to_characters(
  epos_name    name,
  char         *c1,
  char         *c2,
  char         *c3,
  char         *c4
);

#endif


#endif

#ifdef __cplusplus
}
#endif

/**@}*/

#endif
/* end of include file */
