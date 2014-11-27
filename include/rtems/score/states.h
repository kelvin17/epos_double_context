/**
 *  @file  states.h
 *
 *  @ingroup ScoreStates
 *
 *  @brief Header file of the thread execution state information.
 *
 *  This include file contains thread execution state information.
 */

/*
 *  COPYRIGHT (c) 1989-2006.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: states.h,v 1.22 2009/11/28 05:58:54 ralf Exp $
 */

#ifndef _RTEMS_SCORE_STATES_H
#define _RTEMS_SCORE_STATES_H

/**
 *  @defgroup ScoreStates Thread States Handler
 *
 *  @ingroup Score
 *
 *  @brief 核心抽象层之线程状态抽象管理模块
 *
 *  This handler encapsulates functionality which relates to the management of
 *  the state bitmap associated with each thread.
 */
/**@{*/

#ifdef __cplusplus
extern "C" {
#endif

/**
 *  The following type defines the control block used to manage a
 *  thread's state.
 */
typedef uint32_t   States_Control;

/*
 *  The following constants define the individual states which may be
 *  be used to compose and manipulate a thread's state.
 */

/** This macro corresponds to all states being set. */
#define STATES_ALL_SET                         0xfffff
/** This macro corresponds to a task being ready. */
#define STATES_READY                           0x00000
/** This macro corresponds to a task being created but not yet started. */
#define STATES_DORMANT                         0x00001
/** This macro corresponds to a task being suspended. */
#define STATES_SUSPENDED                       0x00002
/** This macro corresponds to a task being in an internal state transition. */
#define STATES_TRANSIENT                       0x00004
/** This macro corresponds to a task which is waiting for a timeout. */
#define STATES_DELAYING                        0x00008
/** This macro corresponds to a task waiting until a specific TOD. */
#define STATES_WAITING_FOR_TIME                0x00010
/** This macro corresponds to a task waiting for a variable length buffer. */
#define STATES_WAITING_FOR_BUFFER              0x00020
/** This macro corresponds to a task waiting for a fixed size segment. */
#define STATES_WAITING_FOR_SEGMENT             0x00040
/** This macro corresponds to a task waiting for a message. */
#define STATES_WAITING_FOR_MESSAGE             0x00080
/** This macro corresponds to a task waiting for an event. */
#define STATES_WAITING_FOR_EVENT               0x00100
/** This macro corresponds to a task waiting for a semaphore. */
#define STATES_WAITING_FOR_SEMAPHORE           0x00200
/** This macro corresponds to a task waiting for a mutex. */
#define STATES_WAITING_FOR_MUTEX               0x00400
/** This macro corresponds to a task waiting for a condition variable. */
#define STATES_WAITING_FOR_CONDITION_VARIABLE  0x00800
/** This macro corresponds to a task waiting for a join while exiting. */
#define STATES_WAITING_FOR_JOIN_AT_EXIT        0x01000
/** This macro corresponds to a task waiting for a reply to an MPCI request. */
#define STATES_WAITING_FOR_RPC_REPLY           0x02000
/** This macro corresponds to a task waiting for a period. */
#define STATES_WAITING_FOR_PERIOD              0x04000
/** This macro corresponds to a task waiting for a signal. */
#define STATES_WAITING_FOR_SIGNAL              0x08000
/** This macro corresponds to a task waiting for a barrier. */
#define STATES_WAITING_FOR_BARRIER             0x10000
/** This macro corresponds to a task waiting for a RWLock. */
#define STATES_WAITING_FOR_RWLOCK              0x20000

/** This macro corresponds to a task which is in an interruptible
 *  blocking state.
 */
#define STATES_INTERRUPTIBLE_BY_SIGNAL         0x10000000

/** This macro corresponds to a task waiting for a local object operation. */
#define STATES_LOCALLY_BLOCKED ( STATES_WAITING_FOR_BUFFER             | \
                                 STATES_WAITING_FOR_SEGMENT            | \
                                 STATES_WAITING_FOR_MESSAGE            | \
                                 STATES_WAITING_FOR_SEMAPHORE          | \
                                 STATES_WAITING_FOR_MUTEX              | \
                                 STATES_WAITING_FOR_CONDITION_VARIABLE | \
                                 STATES_WAITING_FOR_JOIN_AT_EXIT       | \
                                 STATES_WAITING_FOR_SIGNAL             | \
                                 STATES_WAITING_FOR_BARRIER            | \
                                 STATES_WAITING_FOR_RWLOCK             )

/** This macro corresponds to a task waiting which is blocked on
 *  a thread queue. */
#define STATES_WAITING_ON_THREAD_QUEUE \
                               ( STATES_LOCALLY_BLOCKED         | \
                                 STATES_WAITING_FOR_RPC_REPLY   )

/** This macro corresponds to a task waiting which is blocked. */
#define STATES_BLOCKED         ( STATES_DELAYING                | \
                                 STATES_WAITING_FOR_TIME        | \
                                 STATES_WAITING_FOR_PERIOD      | \
                                 STATES_WAITING_FOR_EVENT       | \
                                 STATES_WAITING_ON_THREAD_QUEUE | \
                                 STATES_INTERRUPTIBLE_BY_SIGNAL )

#ifndef __RTEMS_APPLICATION__
#ifndef _OCC_NO_INLINE_
#include <rtems/score/states.inl>
#endif

#ifdef _OCC_NO_INLINE_
RTEMS_INLINE_ROUTINE States_Control _States_Set (
  States_Control states_to_set,
  States_Control current_state
);
RTEMS_INLINE_ROUTINE States_Control _States_Set (
  States_Control states_to_set,
  States_Control current_state
);
RTEMS_INLINE_ROUTINE bool _States_Is_ready (
  States_Control the_states
);
RTEMS_INLINE_ROUTINE bool _States_Is_only_dormant (
  States_Control the_states
);
RTEMS_INLINE_ROUTINE bool _States_Is_dormant (
  States_Control the_states
);
RTEMS_INLINE_ROUTINE bool _States_Is_suspended (
  States_Control the_states
);
RTEMS_INLINE_ROUTINE bool _States_Is_transient (
  States_Control the_states
);
RTEMS_INLINE_ROUTINE bool _States_Is_delaying (
  States_Control the_states
);
RTEMS_INLINE_ROUTINE bool _States_Is_waiting_for_buffer (
  States_Control the_states
);
RTEMS_INLINE_ROUTINE bool _States_Is_waiting_for_segment (
  States_Control the_states
);
RTEMS_INLINE_ROUTINE bool _States_Is_waiting_for_message (
  States_Control the_states
);
RTEMS_INLINE_ROUTINE bool _States_Is_waiting_for_event (
  States_Control the_states
);
RTEMS_INLINE_ROUTINE bool _States_Is_waiting_for_mutex (
  States_Control the_states
);
RTEMS_INLINE_ROUTINE bool _States_Is_waiting_for_semaphore (
  States_Control the_states
);
RTEMS_INLINE_ROUTINE bool _States_Is_waiting_for_time (
  States_Control the_states
);
RTEMS_INLINE_ROUTINE bool _States_Is_waiting_for_rpc_reply (
  States_Control the_states
);
RTEMS_INLINE_ROUTINE bool _States_Is_waiting_for_period (
  States_Control the_states
);
RTEMS_INLINE_ROUTINE bool _States_Is_interruptible_by_signal (
  States_Control the_states
);
RTEMS_INLINE_ROUTINE bool _States_Is_locally_blocked (
  States_Control the_states
);
RTEMS_INLINE_ROUTINE bool _States_Is_waiting_on_thread_queue (
  States_Control the_states
);
RTEMS_INLINE_ROUTINE bool _States_Is_blocked (
  States_Control the_states
);
RTEMS_INLINE_ROUTINE bool _States_Are_set (
  States_Control the_states,
  States_Control mask
);

#endif
#endif

#ifdef __cplusplus
}
#endif

/**@}*/

#endif
/* end of include file */
