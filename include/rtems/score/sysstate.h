/**
 * @file
 *
 * @ingroup ScoreSysState
 *
 * @brief System State Handler API.
 */

/*
 *  COPYRIGHT (c) 1989-2011.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: sysstate.h,v 1.25.2.1 2011/05/25 14:17:52 ralf Exp $
 */

#ifndef _RTEMS_SCORE_SYSSTATE_H
#define _RTEMS_SCORE_SYSSTATE_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup ScoreSysState System State Handler
 *
 * @ingroup Score
 *
 *  @brief 核心抽象层之系统内部状态管理模块
 *
 * @brief Management of the internal system state of RTEMS.
 *
 * @{
 */

/**
 * @brief System states.
 */
typedef enum {
  /**
   * @brief The system is before the end of the first phase of initialization.
   */
  SYSTEM_STATE_BEFORE_INITIALIZATION,

  /**
   * @brief The system is between end of the first phase of initialization but
   * before  multitasking is started.
   */
  SYSTEM_STATE_BEFORE_MULTITASKING,

  /**
   * @brief The system is attempting to initiate multitasking.
   */
  SYSTEM_STATE_BEGIN_MULTITASKING,

  /**
   * @brief The system is up and operating normally.
   */
  SYSTEM_STATE_UP,

  /**
   * @brief The system is in the midst of a shutdown.
   */
  SYSTEM_STATE_SHUTDOWN,

  /**
   * @brief A fatal error has occurred.
   */
  SYSTEM_STATE_FAILED
} System_state_Codes;

#define SYSTEM_STATE_CODES_FIRST SYSTEM_STATE_BEFORE_INITIALIZATION

#define SYSTEM_STATE_CODES_LAST SYSTEM_STATE_FAILED


SCORE_EXTERN System_state_Codes _System_state_Current;

/*
 *  Make it possible for the application to get the system state information.
 */
#ifndef _OCC_NO_INLINE_
#include <rtems/score/sysstate.inl>
#endif

#ifdef _OCC_NO_INLINE_
RTEMS_INLINE_ROUTINE void _System_state_Set (
  System_state_Codes state
);

RTEMS_INLINE_ROUTINE void _System_state_Handler_initialization (
 bool  is_multiprocessing __attribute__((unused))
);

RTEMS_INLINE_ROUTINE System_state_Codes _System_state_Get ( void );

RTEMS_INLINE_ROUTINE bool _System_state_Is_before_initialization (
  System_state_Codes state
);
RTEMS_INLINE_ROUTINE bool _System_state_Is_before_multitasking (
  System_state_Codes state
);
RTEMS_INLINE_ROUTINE bool _System_state_Is_begin_multitasking (
  System_state_Codes state
);
RTEMS_INLINE_ROUTINE bool _System_state_Is_shutdown (
  System_state_Codes state
);
RTEMS_INLINE_ROUTINE bool _System_state_Is_up (
  System_state_Codes state
);
RTEMS_INLINE_ROUTINE bool _System_state_Is_failed (
  System_state_Codes state
);

#endif
/** @} */

#ifdef __cplusplus
}
#endif

#endif
/* end of include file */
