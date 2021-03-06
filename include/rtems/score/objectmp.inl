/** 
 *  @file  objectmp.inl
 *
 *  @ingroup ScoreObjectMP
 *
 *  @brief The include file of the manipulation of Global RTEMS Objects.
 *
 *  This include file contains the bodies of all inlined routines
 *  which deal with global objects.
 */

/*
 *  COPYRIGHT (c) 1989-2004.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: objectmp.inl,v 1.15 2008/09/04 17:38:26 ralf Exp $
 */

#ifdef _OCC_NO_INLINE_
#include <rtems/system.h>
#include <rtems/score/object.h>
#include <rtems/score/objectmp.h>
#endif

#ifndef _RTEMS_SCORE_OBJECTMP_H
# error "Never use "objectmp.inl" directly; include "objectmp.h" instead."
#endif

#ifndef _RTEMS_SCORE_OBJECTMP_INL
#define _RTEMS_SCORE_OBJECTMP_INL

/**
 *  @addtogroup ScoreObjectMP 
 *  @{
 */

/**
 *  This function allocates a Global Object control block.
 */

RTEMS_INLINE_ROUTINE Objects_MP_Control *_Objects_MP_Allocate_global_object (
  void
)
{
  return (Objects_MP_Control *)
           _Chain_Get( &_Objects_MP_Inactive_global_objects );
}

/**
 *  This routine deallocates a Global Object control block.
 */

RTEMS_INLINE_ROUTINE void _Objects_MP_Free_global_object (
  Objects_MP_Control *the_object
)
{
  _Chain_Append(
    &_Objects_MP_Inactive_global_objects,
    &the_object->Object.Node
  );
}

/**
 *  This function returns whether the global object is NULL or not.
 */

RTEMS_INLINE_ROUTINE bool _Objects_MP_Is_null_global_object (
  Objects_MP_Control *the_object
)
{
  return( the_object == NULL );
}

/**@}*/

#endif
/* end of include file */
