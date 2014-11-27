/**
 *  @file  address.h
 *
 *  @ingroup ScoreAddress
 *
 *  @brief Header file to manipulate physical addresses.
 *
 *  This include file contains the information required to manipulate
 *  physical addresses.
 */

/*
 *  COPYRIGHT (c) 1989-2006.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: address.h,v 1.17 2009/11/28 05:58:53 ralf Exp $
 */

#ifndef _RTEMS_SCORE_ADDRESS_H
#define _RTEMS_SCORE_ADDRESS_H

/**
 *  @defgroup ScoreAddress Address Handler
 *
 *  @ingroup Score
 *
 *  @brief 核心抽象层之Address抽象管理模块
 *
 *  This handler encapsulates functionality which abstracts address
 *  manipulation in a portable manner.
 */
/**@{*/

#ifdef __cplusplus
extern "C" {
#endif

#ifndef _OCC_NO_INLINE_
#include <rtems/score/address.inl>
#endif

#ifdef _OCC_NO_INLINE_
RTEMS_INLINE_ROUTINE void *_Addresses_Add_offset (const void *base,uintptr_t   offset);

RTEMS_INLINE_ROUTINE void *_Addresses_Subtract_offset (const void *base,uintptr_t   offset);

RTEMS_INLINE_ROUTINE int32_t _Addresses_Subtract (const void *left,const void *right);

RTEMS_INLINE_ROUTINE bool _Addresses_Is_aligned (const void *address);

RTEMS_INLINE_ROUTINE bool _Addresses_Is_in_range (const void *address,  const void *base,  const void *limit);
#endif

#ifdef __cplusplus
}
#endif

/**@}*/

#endif
/* end of include file */
