/**
 *  @file  thread.h
 *
 *  @ingroup ScoreThread
 *
 *  @brief Header file of the thread control block.
 *
 *  This include file contains all constants and structures associated
 *  with the thread control block.
 */

/*
 *  COPYRIGHT (c) 1989-2009.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: thread.h,v 1.90 2009/12/02 18:15:16 humph Exp $
 */

#ifndef _RTEMS_SCORE_THREAD_H
#define _RTEMS_SCORE_THREAD_H

/**
 *  @defgroup ScoreThread Thread Handler
 *
 *  @ingroup Score
 *
 *  @brief 核心抽象层之线程抽象管理模块
 *
 *  This handler encapsulates functionality related to the management of
 *  threads.  This includes the creation, deletion, and scheduling of threads.
 */
/**@{*/



#if defined(RTEMS_POSIX_API) 
  #define RTEMS_SCORE_THREAD_ENABLE_EXHAUST_TIMESLICE
#endif

#if defined(RTEMS_POSIX_API)
  #define RTEMS_SCORE_THREAD_ENABLE_SCHEDULER_CALLOUT
#endif

#if defined(RTEMS_POSIX_API)
  #define RTEMS_SCORE_THREAD_ENABLE_USER_PROVIDED_STACK_VIA_API
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*
 *  The user can define this at configure time and go back to ticks
 *  resolution.
 */
 
#ifndef __RTEMS_USE_TICKS_FOR_STATISTICS__
  #include <rtems/score/timestamp.h>

  typedef Timestamp_Control Thread_CPU_usage_t;
#else
  typedef uint32_t Thread_CPU_usage_t;
#endif

#include <rtems/score/context.h>
#include <rtems/score/cpu.h>
#if defined(RTEMS_MULTIPROCESSING)
#include <rtems/score/mppkt.h>
#endif
#include <rtems/score/object.h>
#include <rtems/score/priority.h>
#include <rtems/score/stack.h>
#include <rtems/score/states.h>
#include <rtems/score/tod.h>
#include <rtems/score/tqdata.h>
#include <rtems/score/watchdog.h>


/*
 *  The following defines the "return type" of a thread.
 *
 *  @note  This cannot always be right.  Some APIs have void
 *         tasks/threads, others return pointers, others may
 *         return a numeric value.  Hopefully a pointer is
 *         always at least as big as an uint32_t  . :)
 */
typedef void *Thread;

/**
 *  @brief Type of the numeric argument of a thread entry function with at
 *  least one numeric argument.
 *
 *  This numeric argument type designates an unsigned integer type with the
 *  property that any valid pointer to void can be converted to this type and
 *  then converted back to a pointer to void.  The result will compare equal to
 *  the original pointer.
 */
typedef uintptr_t Thread_Entry_numeric_type;

/**
 *  The following defines the ways in which the entry point for a
 *  thread can be invoked.  Basically, it can be passed any
 *  combination/permutation of a pointer and an uint32_t   value.
 *
 *  @note For now, we are ignoring the return type.
 */
typedef enum {
  THREAD_START_NUMERIC,
  THREAD_START_POINTER,
  #if defined(FUNCTIONALITY_NOT_CURRENTLY_USED_BY_ANY_API)
    THREAD_START_BOTH_POINTER_FIRST,
    THREAD_START_BOTH_NUMERIC_FIRST
  #endif
} Thread_Start_types;

/** This type corresponds to a very simple style thread entry point. */
typedef Thread ( *Thread_Entry )( void );   /* basic type */

/** This type corresponds to a thread entry point which takes a single
 *  unsigned thirty-two bit integer as an argument.
 */
typedef Thread ( *Thread_Entry_numeric )( Thread_Entry_numeric_type );

/** This type corresponds to a thread entry point which takes a single
 *  untyped pointer as an argument.
 */
typedef Thread ( *Thread_Entry_pointer )( void * );

/** This type corresponds to a thread entry point which takes a single
 *  untyped pointer and an unsigned thirty-two bit integer as arguments.
 */
typedef Thread ( *Thread_Entry_both_pointer_first )( void *, Thread_Entry_numeric_type );

/** This type corresponds to a thread entry point which takes a single
 *  unsigned thirty-two bit integer and an untyped pointer and an
 *  as arguments.
 */
typedef Thread ( *Thread_Entry_both_numeric_first )( Thread_Entry_numeric_type, void * );

/**
 *  The following lists the algorithms used to manage the thread cpu budget.
 *
 *  Reset Timeslice:   At each context switch, reset the time quantum.
 *  Exhaust Timeslice: Only reset the quantum once it is consumed.
 *  Callout:           Execute routine when budget is consumed.
 */
typedef enum {
  THREAD_CPU_BUDGET_ALGORITHM_NONE,
  THREAD_CPU_BUDGET_ALGORITHM_RESET_TIMESLICE,
  #if defined(RTEMS_SCORE_THREAD_ENABLE_EXHAUST_TIMESLICE)
    THREAD_CPU_BUDGET_ALGORITHM_EXHAUST_TIMESLICE,
  #endif
  #if defined(RTEMS_SCORE_THREAD_ENABLE_SCHEDULER_CALLOUT)
    THREAD_CPU_BUDGET_ALGORITHM_CALLOUT
  #endif
}  Thread_CPU_budget_algorithms;

/** This type defines the Thread Control Block structure.
 */
typedef struct Thread_Control_struct Thread_Control;

/**  This defines thes the entry point for the thread specific timeslice
 *   budget management algorithm.
 */
typedef void (*Thread_CPU_budget_algorithm_callout )( Thread_Control * );

/** @brief Per Task Variable Manager Structure Forward Reference
 *
 *  Forward reference to the per task variable structure.
 */
struct epos_task_variable_tt;

/** @brief Per Task Variable Manager Structure
 *
 *  This is the internal structure used to manager per Task Variables.
 */
typedef struct {
  /** This field points to the next per task variable for this task. */
  struct epos_task_variable_tt  *next;
  /** This field points to the physical memory location of this per
   *  task variable.
   */
  void                          **ptr;
  /** This field is to the global value for this per task variable. */
  void                           *gval;
  /** This field is to this thread's value for this per task variable. */
  void                           *tval;
  /** This field points to the destructor for this per task variable. */
  void                          (*dtor)(void *);
} epos_task_variable_t;

/**
 *  The following structure contains the information which defines
 *  the starting state of a thread.
 */
typedef struct {
  /** This field is the starting address for the thread. */
  Thread_Entry                         entry_point;
  /** This field indicates the how task is invoked. */
  Thread_Start_types                   prototype;
  /** This field is the pointer argument passed at thread start. */
  void                                *pointer_argument;
  /** This field is the numeric argument passed at thread start. */
  Thread_Entry_numeric_type            numeric_argument;
  /*-------------- initial execution modes ----------------- */
  /** This field indicates whether the thread was preemptible when
    * it started.
    */
  bool                                 is_preemptible;
  /** This field indicates the CPU budget algorith. */
  Thread_CPU_budget_algorithms         budget_algorithm;
  /** This field is the routine to invoke when the CPU allotment is
   *  consumed.
   */
  Thread_CPU_budget_algorithm_callout  budget_callout;
  /** This field is the initial ISR disable level of this thread. */
  uint32_t                             isr_level;
  /** This field is the initial priority. */
  Priority_Control                     initial_priority;
  #if defined(RTEMS_SCORE_THREAD_ENABLE_USER_PROVIDED_STACK_VIA_API)
    /** This field indicates whether the SuperCore allocated the stack. */
    bool                                 core_allocated_stack;
  #endif
  /** This field is the stack information. */
  Stack_Control                        Initial_stack;
  #if ( CPU_HARDWARE_FP == TRUE ) || ( CPU_SOFTWARE_FP == TRUE )
    /** This field is the initial FP context area address. */
    Context_Control_fp                  *fp_context;
  #endif
  /** This field is the initial stack area address. */
  void                                *stack;
} Thread_Start_information;

/**
 *  The following structure contains the information necessary to manage
 *  a thread which it is  waiting for a resource.
 */
#define THREAD_STATUS_PROXY_BLOCKING 0x1111111

/**
 *  @brief Union type to hold a pointer to an immutable or a mutable object.
 *
 *  The main purpose is to enable passing of pointers to read-only send buffers
 *  in the message passing subsystem.  This approach is somewhat fragile since
 *  it prevents the compiler to check if the operations on objects are valid
 *  with respect to the constant qualifier.  An alternative would be to add a
 *  third pointer argument for immutable objects, but this would increase the
 *  structure size.
 */
typedef union {
  void       *mutable_object;
  const void *immutable_object;
} Thread_Wait_information_Object_argument_type;

/** @brief Thread Blocking Management Information
 *
 *  This contains the information required to manage a thread while it is
 *  blocked and to return information to it.
 */
typedef struct {
  /** This field is the Id of the object this thread is waiting upon. */
  Objects_Id            id;
  /** This field is used to return an integer while when blocked. */
  uint32_t              count;
  /** This field is for a pointer to a user return argument. */
  void                 *return_argument;
  /** This field is for a pointer to a second user return argument. */
  Thread_Wait_information_Object_argument_type
                        return_argument_second;
  /** This field contains any options in effect on this blocking operation. */
  uint32_t              option;
  /** This field will contain the return status from a blocking operation.
   *
   *  @note The following assumes that all API return codes can be
   *        treated as an uint32_t.
   */
  uint32_t              return_code;

  /** This field is the chain header for the second through Nth tasks
   *  of the same priority blocked waiting on the same object.
   */
  Chain_Control         Block2n;
  /** This field points to the thread queue on which this thread is blocked. */
  Thread_queue_Control *queue;
}   Thread_Wait_information;

/**
 *  The following defines the control block used to manage
 *  each thread proxy.
 *
 *  @note It is critical that proxies and threads have identical
 *        memory images for the shared part.
 */
typedef struct {
  /** This field is the object management structure for each proxy. */
  Objects_Control          Object;
  /** This field is the current execution state of this proxy. */
  States_Control           current_state;
  /** This field is the current priority state of this proxy. */
  Priority_Control         current_priority;
  /** This field is the base priority of this proxy. */
  Priority_Control         real_priority;
  /** This field is the number of mutexes currently held by this proxy. */
  uint32_t                 resource_count;

  /** This field is the blocking information for this proxy. */
  Thread_Wait_information  Wait;
  /** This field is the Watchdog used to manage proxy delays and timeouts. */
  Watchdog_Control         Timer;
#if defined(RTEMS_MULTIPROCESSING)
  /** This field is the received response packet in an MP system. */
  MP_packet_Prefix        *receive_packet;
#endif
     /****************** end of common block ********************/
  /** This field is used to manage the set of proxies in the system. */
  Chain_Node               Active;
}   Thread_Proxy_control;

/**
 *  The following record defines the control block used
 *  to manage each thread.
 *
 *  @note It is critical that proxies and threads have identical
 *        memory images for the shared part.
 */
typedef enum {
  /** This value is for the Classic RTEMS API. */
  THREAD_API_RTEMS,
  /** This value is for the POSIX API. */
  THREAD_API_POSIX
}  Thread_APIs;

/** This macro defines the first API which has threads. */
#define THREAD_API_FIRST THREAD_API_RTEMS

/** This macro defines the last API which has threads. */
#define THREAD_API_LAST  THREAD_API_POSIX

/**
 *  This structure defines the Thread Control Block (TCB).
 */
struct Thread_Control_struct {
  /** This field is the object management structure for each thread. */
  Objects_Control          Object;
  /** This field is the current execution state of this thread. */
  States_Control           current_state;
  /** This field is the current priority state of this thread. */
  Priority_Control         current_priority;
  /** This field is the base priority of this thread. */
  Priority_Control         real_priority;
  /** This field is the number of mutexes currently held by this thread. */
  uint32_t                 resource_count;
  /** This field is the blocking information for this thread. */
  Thread_Wait_information  Wait;
  /** This field is the Watchdog used to manage thread delays and timeouts. */
  Watchdog_Control         Timer;
#if defined(RTEMS_MULTIPROCESSING)
  /** This field is the received response packet in an MP system. */
  MP_packet_Prefix        *receive_packet;
#endif
#ifdef __RTEMS_STRICT_ORDER_MUTEX__
  /** This field is the head of queue of priority inheritance mutex
   *  held by the thread.
   */
  Chain_Control            lock_mutex;
#endif
     /*================= end of common block =================*/
  /** This field is the number of nested suspend calls. */
  uint32_t                              suspend_count;
#if defined(RTEMS_MULTIPROCESSING)
  /** This field is true if the thread is offered globally */
  bool                                  is_global;
#endif
  /** This field is is true if the post task context switch should be
   *  executed for this thread at the next context switch.
   */
  bool                                  do_post_task_switch_extension;
  /** This field is true if the thread is preemptible. */
  bool                                  is_preemptible;
#if __RTEMS_ADA__
  /** This field is the GNAT self context pointer. */
  void                                 *epos_ada_self;
#endif
  /** This field is the length of the time quantum that this thread is
   *  allowed to consume.  The algorithm used to manage limits on CPU usage
   *  is specified by budget_algorithm.
   */
  uint32_t                              cpu_time_budget;
  /** This field is the algorithm used to manage this thread's time
   *  quantum.  The algorithm may be specified as none which case,
   *  no limit is in place.
   */
  Thread_CPU_budget_algorithms          budget_algorithm;
  /** This field is the method invoked with the budgeted time is consumed. */
  Thread_CPU_budget_algorithm_callout   budget_callout;
  /** This field is the amount of CPU time consumed by this thread
   *  since it was created.
   */
  Thread_CPU_usage_t                    cpu_time_used;
  /** This field points to the Ready FIFO for this priority. */
  Chain_Control                        *ready;
  /** This field contains precalculated priority map indices. */
  Priority_Information                  Priority_map;
  /** This field contains information about the starting state of
   *  this thread.
   */
  Thread_Start_information              Start;
  /** This field contains the context of this thread. */
  Context_Control                       *Registers;
#if ( CPU_HARDWARE_FP == TRUE ) || ( CPU_SOFTWARE_FP == TRUE )
  /** This field points to the floating point context for this thread.
   *  If NULL, the thread is integer only.
   */
  Context_Control_fp                   *fp_context;
#endif
  /** This field points to the newlib reentrancy structure for this thread. */
  struct _reent                        *libc_reent;
  /** This array contains the API extension area pointers. */
  void                                 *API_Extensions[ THREAD_API_LAST + 1 ];
  /** This field points to the user extension pointers. */
  void                                **extensions;
  /** This field points to the set of per task variables. */
  epos_task_variable_t                *task_variables;
};

/**
 *  Self for the GNU Ada Run-Time
 */
SCORE_EXTERN void *epos_ada_self;

/**
 *  The following defines the information control block used to
 *  manage this class of objects.
 */
SCORE_EXTERN Objects_Information _Thread_Internal_information;

/**
 *  The following define the thread control pointers used to access
 *  and manipulate the idle thread.
 */
SCORE_EXTERN Thread_Control *_Thread_Idle;

/**
 *  The following context area contains the context of the "thread"
 *  which invoked the start multitasking routine.  This context is
 *  restored as the last action of the stop multitasking routine.  Thus
 *  control of the processor can be returned to the environment
 *  which initiated the system.
 */
SCORE_EXTERN Context_Control *_Thread_BSP_context;   //by zwj

/**
 *  The following declares the dispatch critical section nesting
 *  counter which is used to prevent context switches at inopportune
 *  moments.
 */
SCORE_EXTERN volatile uint32_t   _Thread_Dispatch_disable_level;

/**
 *  If this is non-zero, then the post-task switch extension
 *  is run regardless of the state of the per thread flag.
 */
SCORE_EXTERN uint32_t   _Thread_Do_post_task_switch_extension;

/**
 *  The following holds how many user extensions are in the system.  This
 *  is used to determine how many user extension data areas to allocate
 *  per thread.
 */
SCORE_EXTERN uint32_t   _Thread_Maximum_extensions;

/**
 *  The following is used to manage the length of a timeslice quantum.
 */
SCORE_EXTERN uint32_t   _Thread_Ticks_per_timeslice;

/**
 *  The following points to the array of FIFOs used to manage the
 *  set of ready threads.
 */
SCORE_EXTERN Chain_Control *_Thread_Ready_chain;

/**
 *  The following points to the thread which is currently executing.
 *  This thread is implicitly manipulated by numerous directives.
 */
SCORE_EXTERN Thread_Control *_Thread_Executing;

/**
 *  The following points to the highest priority ready thread
 *  in the system.  Unless the current thread is not preemptibl,
 *  then this thread will be context switched to when the next
 *  dispatch occurs.
 */
SCORE_EXTERN Thread_Control *_Thread_Heir;

/**
 *  The following points to the thread whose floating point
 *  context is currently loaded.
 */
#if ( CPU_HARDWARE_FP == TRUE ) || ( CPU_SOFTWARE_FP == TRUE )
SCORE_EXTERN Thread_Control *_Thread_Allocated_fp;
#endif

/**
 * The C library re-enter-rant global pointer. Some C library implementations
 * such as newlib have a single global pointer that changed during a context
 * switch. The pointer points to that global pointer. The Thread control block
 * holds a pointer to the task specific data.
 */
SCORE_EXTERN struct _reent **_Thread_libc_reent;

#ifndef __RTEMS_USE_TICKS_FOR_STATISTICS__

  /**
   * This contains the time since boot when the last context switch occurred.
   * By placing it in the BSS, it will automatically be zeroed out at
   * system initialization and does not need to be known outside this
   * file.
   */
  SCORE_EXTERN Timestamp_Control _Thread_Time_of_last_context_switch;
#endif


/**
 *  This routine performs the initialization necessary for this handler.
 */
void _Thread_Handler_initialization(void);

/**
 *  This routine creates the idle thread.
 *
 *  @warning No thread should be created before this one.
 */
void _Thread_Create_idle(void);

/**
 *  This routine initiates multitasking.  It is invoked only as
 *  part of initialization and its invocation is the last act of
 *  the non-multitasking part of the system initialization.
 */
void _Thread_Start_multitasking( void );

/**
 *  This routine is responsible for transferring control of the
 *  processor from the executing thread to the heir thread.  As part
 *  of this process, it is responsible for the following actions:
 *
 *     + saving the context of the executing thread
 *     + restoring the context of the heir thread
 *     + dispatching any signals for the resulting executing thread
 */
void _Thread_Dispatch( void );

/**
 *  Allocate the requested stack space for the thread.
 *  return the actual size allocated after any adjustment
 *  or return zero if the allocation failed.
 *  Set the Start.stack field to the address of the stack
 */

size_t _Thread_Stack_Allocate(
  Thread_Control *the_thread,
  size_t          stack_size
);

/**
 *  Deallocate the Thread's stack.
 */
void _Thread_Stack_Free(
  Thread_Control *the_thread, bool is_new_stack
);

/**
 *  This routine initializes the specified the thread.  It allocates
 *  all memory associated with this thread.  It completes by adding
 *  the thread to the local object table so operations on this
 *  thread id are allowed.
 *
 *  @note If stack_area is NULL, it is allocated from the workspace.
 *
 *  @note If the stack is allocated from the workspace, then it is
 *        guaranteed to be of at least minimum size.
 */
bool _Thread_Initialize(
  Objects_Information                  *information,
  Thread_Control                       *the_thread,
  void                                 *stack_area,
  size_t                                stack_size,
  bool                                  is_fp,
  Priority_Control                      priority,
  bool                                  is_preemptible,
  Thread_CPU_budget_algorithms          budget_algorithm,
  Thread_CPU_budget_algorithm_callout   budget_callout,
  uint32_t                              isr_level,
  Objects_Name                          name
);

/**
 *  This routine initializes the executable information for a thread
 *  and makes it ready to execute.  After this routine executes, the
 *  thread competes with all other threads for CPU time.
 */
bool _Thread_Start(
  Thread_Control            *the_thread,
  Thread_Start_types         the_prototype,
  void                      *entry_point,
  void                      *pointer_argument,
  Thread_Entry_numeric_type  numeric_argument
);

/**
 *  This support routine restarts the specified task in a way that the
 *  next time this thread executes, it will begin execution at its
 *  original starting point.
 *
 *  TODO:  multiple task arg profiles
 */
bool _Thread_Restart(
  Thread_Control            *the_thread,
  void                      *pointer_argument,
  Thread_Entry_numeric_type  numeric_argument
);

/**
 *  This routine resets a thread to its initial state but does
 *  not restart it.
 */
void _Thread_Reset(
  Thread_Control            *the_thread,
  void                      *pointer_argument,
  Thread_Entry_numeric_type  numeric_argument
);

/**
 *  This routine frees all memory associated with the specified
 *  thread and removes it from the local object table so no further
 *  operations on this thread are allowed.
 */
void _Thread_Close(
  Objects_Information  *information,
  Thread_Control       *the_thread
);

/**
 *  This routine removes any set states for the_thread.  It performs
 *  any necessary scheduling operations including the selection of
 *  a new heir thread.
 */
void _Thread_Ready(
  Thread_Control *the_thread
);

/**
 *  This routine clears the indicated STATES for the_thread.  It performs
 *  any necessary scheduling operations including the selection of
 *  a new heir thread.
 */
void _Thread_Clear_state(
  Thread_Control *the_thread,
  States_Control  state
);

/**
 *  This routine sets the indicated states for the_thread.  It performs
 *  any necessary scheduling operations including the selection of
 *  a new heir thread.
 */
void _Thread_Set_state(
  Thread_Control *the_thread,
  States_Control  state
);

/**
 *  This routine sets the TRANSIENT state for the_thread.  It performs
 *  any necessary scheduling operations including the selection of
 *  a new heir thread.
 */
void _Thread_Set_transient(
  Thread_Control *the_thread
);

/**
 *  This routine is invoked upon expiration of the currently
 *  executing thread's timeslice.  If no other thread's are ready
 *  at the priority of the currently executing thread, then the
 *  executing thread's timeslice is reset.  Otherwise, the
 *  currently executing thread is placed at the rear of the
 *  FIFO for this priority and a new heir is selected.
 */
void _Thread_Reset_timeslice( void );

/**
 *  This routine is invoked as part of processing each clock tick.
 *  It is responsible for determining if the current thread allows
 *  timeslicing and, if so, when its timeslice expires.
 */
void _Thread_Tickle_timeslice( void );

/**
 *  This routine is invoked when a thread wishes to voluntarily
 *  transfer control of the processor to another thread of equal
 *  or greater priority.
 */
void _Thread_Yield_processor( void );

/**
 *  This routine is invoked to rotate the ready queue for the
 *  given priority.  It can be used to yeild the processor
 *  by rotating the executing threads ready queue.
 */
void _Thread_Rotate_Ready_Queue(
  Priority_Control  priority
);

/**
 *  This routine initializes the context of the_thread to its
 *  appropriate starting state.
 */
bool _Thread_Load_environment(
  Thread_Control *the_thread, bool is_restart
);

/**
 *  This routine is the wrapper function for all threads.  It is
 *  the starting point for all threads.  The user provided thread
 *  entry point is invoked by this routine.  Operations
 *  which must be performed immediately before and after the user's
 *  thread executes are found here.
 */
void _Thread_Handler( void );

/**
 *  This routine is invoked when a thread must be unblocked at the
 *  end of a time based delay (i.e. wake after or wake when).
 */
void _Thread_Delay_ended(
  Objects_Id  id,
  void       *ignored
);

/**
 *  This routine changes the current priority of the_thread to
 *  new_priority.  It performs any necessary scheduling operations
 *  including the selection of a new heir thread.
 */
void _Thread_Change_priority (
  Thread_Control   *the_thread,
  Priority_Control  new_priority,
  bool              prepend_it
);

/**
 *  This routine updates the priority related fields in the_thread
 *  control block to indicate the current priority is now new_priority.
 */
void _Thread_Set_priority(
  Thread_Control   *the_thread,
  Priority_Control  new_priority
);

/**
 *  This routine updates the related suspend fields in the_thread
 *  control block to indicate the current nested level.
 */
void _Thread_Suspend(
  Thread_Control   *the_thread
);

/**
 *  This routine updates the related suspend fields in the_thread
 *  control block to indicate the current nested level.  A force
 *  parameter of true will force a resume and clear the suspend count.
 */
void _Thread_Resume(
  Thread_Control   *the_thread,
  bool              force
);

/**
 *  This routine evaluates the current scheduling information for the
 *  system and determines if a context switch is required.  This
 *  is usually called after changing an execution mode such as preemptability
 *  for a thread.
 */
bool _Thread_Evaluate_mode( void );

#if (CPU_PROVIDES_IDLE_THREAD_BODY == FALSE)
/**
 *  This routine is the body of the system idle thread.
 *
 *  NOTE: This routine is actually instantiated by confdefs.h when needed.
 */
void *_Thread_Idle_body(
  uintptr_t  ignored
);
#endif

/**  This defines the type for a method which operates on a single thread.
 */
typedef void (*epos_per_thread_routine)( Thread_Control * );

/**
 *  This routine iterates over all threads regardless of API and
 *  invokes the specified routine.
 */
void epos_iterate_over_all_threads(
  epos_per_thread_routine routine
);

/**
 *  This function maps thread IDs to thread control
 *  blocks.  If ID corresponds to a local thread, then it
 *  returns the_thread control pointer which maps to ID
 *  and location is set to OBJECTS_LOCAL.  If the thread ID is
 *  global and resides on a remote node, then location is set
 *  to OBJECTS_REMOTE, and the_thread is undefined.
 *  Otherwise, location is set to OBJECTS_ERROR and
 *  the_thread is undefined.
 *
 *  @note  The performance of many RTEMS services depends upon
 *         the quick execution of the "good object" path in this
 *         routine.  If there is a possibility of saving a few
 *         cycles off the execution time, this routine is worth
 *         further optimization attention.
 */
Thread_Control *_Thread_Get (
  Objects_Id         id,
  Objects_Locations *location
);

/**
 *  @brief Cancel a blocking operation due to ISR
 *
 *  This method is used to cancel a blocking operation that was
 *  satisfied from an ISR while the thread executing was in the
 *  process of blocking.
 *
 *  @param[in] sync_state is the synchronization state
 *  @param[in] the_thread is the thread whose blocking is canceled
 *  @param[in] level is the previous ISR disable level
 *
 *  @note This is a rare routine in RTEMS.  It is called with
 *        interrupts disabled and only when an ISR completed
 *        a blocking condition in process.
 */
void _Thread_blocking_operation_Cancel(
  Thread_blocking_operation_States  sync_state,
  Thread_Control                   *the_thread,
  ISR_Level                         level
);

#ifndef __RTEMS_APPLICATION__
#ifndef _OCC_NO_INLINE_
#include <rtems/score/thread.inl>
#endif

#ifdef _OCC_NO_INLINE_
RTEMS_INLINE_ROUTINE void _Thread_Stop_multitasking( void );
RTEMS_INLINE_ROUTINE bool _Thread_Is_executing (
  const Thread_Control *the_thread
);
RTEMS_INLINE_ROUTINE bool _Thread_Is_heir (
  const Thread_Control *the_thread
);
RTEMS_INLINE_ROUTINE bool _Thread_Is_executing_also_the_heir( void );
RTEMS_INLINE_ROUTINE void _Thread_Unblock (
  Thread_Control *the_thread
);
RTEMS_INLINE_ROUTINE void _Thread_Restart_self( void );
RTEMS_INLINE_ROUTINE void _Thread_Calculate_heir( void );
#if ( CPU_HARDWARE_FP == TRUE ) || ( CPU_SOFTWARE_FP == TRUE )
RTEMS_INLINE_ROUTINE bool _Thread_Is_allocated_fp (
  const Thread_Control *the_thread
);
RTEMS_INLINE_ROUTINE void _Thread_Deallocate_fp( void );
#endif
RTEMS_INLINE_ROUTINE void _Thread_Disable_dispatch( void );

#if ( (defined(CPU_INLINE_ENABLE_DISPATCH) && \
       (CPU_INLINE_ENABLE_DISPATCH == FALSE)) || \
      (__RTEMS_DO_NOT_INLINE_THREAD_ENABLE_DISPATCH__ == 1) )
#else
RTEMS_INLINE_ROUTINE void _Thread_Enable_dispatch( void );
#endif

RTEMS_INLINE_ROUTINE void _Thread_Unnest_dispatch( void );
RTEMS_INLINE_ROUTINE bool _Thread_Is_dispatching_enabled( void );
RTEMS_INLINE_ROUTINE bool _Thread_Is_context_switch_necessary( void );
RTEMS_INLINE_ROUTINE void _Thread_Dispatch_initialization( void );
RTEMS_INLINE_ROUTINE bool _Thread_Is_null (
  const Thread_Control *the_thread
);
RTEMS_INLINE_ROUTINE bool _Thread_Is_proxy_blocking (
  uint32_t   code
);
RTEMS_INLINE_ROUTINE Thread_Control *_Thread_Internal_allocate( void );
RTEMS_INLINE_ROUTINE void _Thread_Internal_free (
  Thread_Control *the_task
);
RTEMS_INLINE_ROUTINE struct _reent **_Thread_Get_libc_reent( void );
RTEMS_INLINE_ROUTINE void _Thread_Set_libc_reent (
  struct _reent **libc_reent
);

#endif

#endif
#if defined(RTEMS_MULTIPROCESSING)
#include <rtems/score/threadmp.h>
#endif

#ifdef __cplusplus
}
#endif

/**@}*/

#endif
/* end of include file */
