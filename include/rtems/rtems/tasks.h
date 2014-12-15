/**
 * @file tasks.h
 *
 * @ingroup ClassicTasks
 *
 *  This include file contains all constants and structures associated
 *  with RTEMS tasks.  This manager provides a comprehensive set of directives
 *  to create, delete, and administer tasks.
 *
 *  Directives provided are:
 *
 *     - create a task
 *     - get an ID of a task
 *     - start a task
 *     - restart a task
 *     - delete a task
 *     - suspend a task
 *     - resume a task
 *     - set a task's priority
 *     - change the current task's mode
 *     - get a task notepad entry
 *     - set a task notepad entry
 *     - wake up after interval
 *     - wake up when specified
 */

/*  COPYRIGHT (c) 1989-2008.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: tasks.h,v 1.44 2009/12/15 18:26:41 humph Exp $
 */

#ifndef _RTEMS_RTEMS_TASKS_H
#define _RTEMS_RTEMS_TASKS_H

#include <rtems/score/object.h>
#include <rtems/score/states.h>
#include <rtems/score/thread.h>
#include <rtems/rtems/types.h>
#include <rtems/rtems/eventset.h>
#include <rtems/rtems/asr.h>
#include <rtems/rtems/attr.h>
#include <rtems/rtems/status.h>

/**
 *  This constant is defined to extern most of the time when using
 *  this header file.  However by defining it to nothing, the data
 *  declared in this header file can be instantiated.  This is done
 *  in a single per manager file.
 */
#ifndef RTEMS_TASKS_EXTERN
#define RTEMS_TASKS_EXTERN extern
#endif

/**
 *  @defgroup ClassicTasks Tasks
 *
 *  @ingroup ClassicRTEMS
 *
 *  @brief RTEMS任务管理服务模块
 *
 *  This encapsulates functionality which X XX
 */
/**@{*/

#ifdef __cplusplus
extern "C" {
#endif

/**
 *  Constant to be used as the ID of current task
 */
#define RTEMS_SELF                OBJECTS_ID_OF_SELF

/**
 *  This constant is passed to the epos_task_wake_after directive as the
 *  interval when a task wishes to yield the CPU.
 */
#define RTEMS_YIELD_PROCESSOR WATCHDOG_NO_TIMEOUT

/**
 *  Define the type for an RTEMS API task priority.
 */
typedef Priority_Control epos_task_priority;

/**
 *  This is the constant used with the epos_task_set_priority
 *  directive to indicate that the caller wants to obtain its
 *  current priority rather than set it as the name of the
 *  directive indicates.
 */
#define RTEMS_NO_PRIORITY           RTEMS_CURRENT_PRIORITY

/**
 *  This constant is the least valid value for a Classic API
 *  task priority.
 */
#define RTEMS_MINIMUM_PRIORITY      (PRIORITY_MINIMUM + 1)

/**
 *  This constant is the maximum valid value for a Classic API
 *  task priority.
 *
 *  @note This is actually the priority of the IDLE thread so
 *        using this priority will result in having a task
 *        which never executes.  This could be useful if you
 *        want to ensure that a task does not executes during
 *        certain operations such as a system mode change.
 */
#define RTEMS_MAXIMUM_PRIORITY      PRIORITY_MAXIMUM

/**
 *  The following constant is passed to epos_task_set_priority when the
 *  caller wants to obtain the current priority.
 */
#define RTEMS_CURRENT_PRIORITY      PRIORITY_MINIMUM

/** This is used to indicate the lowest numbered notepad */
#define RTEMS_NOTEPAD_FIRST 0
/** This is used to indicate the notepad location 0. */
#define RTEMS_NOTEPAD_0    0
/** This is used to indicate the notepad location 1. */
#define RTEMS_NOTEPAD_1    1
/** This is used to indicate the notepad location 2. */
#define RTEMS_NOTEPAD_2    2
/** This is used to indicate the notepad location 3. */
#define RTEMS_NOTEPAD_3    3
/** This is used to indicate the notepad location 4. */
#define RTEMS_NOTEPAD_4    4
/** This is used to indicate the notepad location 5. */
#define RTEMS_NOTEPAD_5    5
/** This is used to indicate the notepad location 6. */
#define RTEMS_NOTEPAD_6    6
/** This is used to indicate the notepad location 7. */
#define RTEMS_NOTEPAD_7    7
/** This is used to indicate the notepad location 8. */
#define RTEMS_NOTEPAD_8    8
/** This is used to indicate the notepad location 9. */
#define RTEMS_NOTEPAD_9    9
/** This is used to indicate the notepad location 10. */
#define RTEMS_NOTEPAD_10   10
/** This is used to indicate the notepad location 11. */
#define RTEMS_NOTEPAD_11   11
/** This is used to indicate the notepad location 12. */
#define RTEMS_NOTEPAD_12   12
/** This is used to indicate the notepad location 13. */
#define RTEMS_NOTEPAD_13   13
/** This is used to indicate the notepad location 14. */
#define RTEMS_NOTEPAD_14   14
/** This is used to indicate the notepad location 15. */
#define RTEMS_NOTEPAD_15   15
/** This is used to indicate the highest numbered notepad. */
#define RTEMS_NOTEPAD_LAST RTEMS_NOTEPAD_15

/** This is used to indicate the number of notepads available. */
#define RTEMS_NUMBER_NOTEPADS  (RTEMS_NOTEPAD_LAST+1)

/**
 *  External API name for Thread_Control
 */
typedef Thread_Control epos_tcb;

/**
 *  The following defines the "return type" of an RTEMS task.
 */
typedef void epos_task;

/**
 *  The following defines the argument to an RTEMS task.
 */
typedef Thread_Entry_numeric_type epos_task_argument;

/**
 *  The following defines the type for the entry point of an RTEMS task.
 */
typedef epos_task ( *epos_task_entry )(
                      epos_task_argument
                   );

/**
 *  The following records define the Initialization Tasks Table.
 *  Each entry contains the information required by RTEMS to
 *  create and start a user task automatically at executive
 *  initialization time.
 */
typedef struct {
  /** This is the Initialization Task's name. */
  epos_name            name;
  /** This is the Initialization Task's stack size. */
  size_t                stack_size;
  /** This is the Initialization Task's priority. */
  epos_task_priority   initial_priority;
  /** This is the Initialization Task's attributes. */
  epos_attribute       attribute_set;
  /** This is the Initialization Task's entry point. */
  epos_task_entry      entry_point;
  /** This is the Initialization Task's initial mode. */
  epos_mode            mode_set;
  /** This is the Initialization Task's argument. */
  epos_task_argument	argument;
} epos_initialization_tasks_table;

/**
 *  This is the API specific information required by each thread for
 *  the RTEMS API to function correctly.
 *
 *  @note Notepads must be the last entry in the structure and memory
 *        will be taken away from this structure when allocated if
 *        notespads are disabled by the application configuration.
 */
typedef struct {
  /** This field contains the pending events for this task. */
  epos_event_set          pending_events;
  /** This field contains the event wait condition for this task. */
  epos_event_set          event_condition;
  /** This field contains the Classic API Signal information for this task. */
  ASR_Information          Signal;
  /**
   *  This field contains the notepads for this task.
   *
   *  @note MUST BE LAST ENTRY.
   */
  uint32_t                 Notepads[ RTEMS_NUMBER_NOTEPADS ];
}  RTEMS_API_Control;

/**
 *  The following instantiates the information control block used to
 *  manage this class of objects.
 */
RTEMS_TASKS_EXTERN Objects_Information _RTEMS_tasks_Information;

/**
 *  When the user configures a set of Classic API initialization tasks,
 *  This variable will point to the method used to initialize them.
 *
 *  @note It is instantiated and initialized by confdefs.h based upon
 *        application requirements.
 */
extern void (*_RTEMS_tasks_Initialize_user_tasks_p)(void);

/**
 *  @brief _RTEMS_tasks_Manager_initialization
 *
 *  This routine initializes all Task Manager related data structures.
 */
void _RTEMS_tasks_Manager_initialization(void);

/**
 *  @brief epos_task_create
 *
 *  This routine implements the epos_task_create directive.  The task
 *  will have the name name.  The attribute_set can be used to indicate
 *  that the task will be globally accessible or utilize floating point.
 *  The task's stack will be stack_size bytes.   The task will begin
 *  execution with initial_priority and initial_modes.  It returns the
 *  id of the created task in ID.
 */
epos_status_code epos_task_create(
  epos_name           name,
  epos_task_priority  initial_priority,
  size_t               stack_size,
  epos_mode           initial_modes,
  epos_attribute      attribute_set,
  epos_id            *id
);

/**
 *  @brief epos_task_ident
 *
 *  This routine implements the epos_task_ident directive.
 *  This directive returns the task ID associated with name.
 *  If more than one task is named name, then the task to
 *  which the ID belongs is arbitrary.  node indicates the
 *  extent of the search for the ID of the task named name.
 *  The search can be limited to a particular node or allowed to
 *  encompass all nodes.
 */
epos_status_code epos_task_ident(
  epos_name    name,
  uint32_t      node,
  epos_id     *id
);

/**
 *  @brief epos_task_delete
 *
 *  This routine implements the epos_task_delete directive.  The
 *  task indicated by ID is deleted.
 */
epos_status_code epos_task_delete(
  epos_id   id
);

/**
 *  @brief epos_task_get_note
 *
 *  This routine implements the epos_task_get_note directive.  The
 *  value of the indicated notepad for the task associated with ID
 *  is returned in note.
 */
epos_status_code epos_task_get_note(
  epos_id    id,
  uint32_t    notepad,
  uint32_t   *note
);

/**
 *  @brief epos_task_set_note
 *
 *  This routine implements the epos_task_set_note directive.  The
 *  value of the indicated notepad for the task associated with ID
 *  is returned in note.
 */
epos_status_code epos_task_set_note(
  epos_id   id,
  uint32_t   notepad,
  uint32_t   note
);

/**
 *  @brief epos_task_mode
 *
 *  This routine implements the epos_task_mode directive.  The current
 *  values of the modes indicated by mask of the calling task are changed
 *  to that indicated in mode_set.  The former mode of the task is
 *  returned in mode_set.
 */
epos_status_code epos_task_mode(
  epos_mode  mode_set,
  epos_mode  mask,
  epos_mode *previous_mode_set
);

/**
 *  @brief epos_task_restart
 *
 *  This routine implements the epos_task_restart directive.  The
 *  task associated with ID is restarted at its initial entry
 *  point with the new argument.
 */
epos_status_code epos_task_restart(
  epos_id   id,
  uint32_t   arg
);

/**
 *  @brief epos_task_suspend
 *
 *  This routine implements the epos_task_suspend directive.  The
 *  SUSPENDED state is set for task associated with ID.
 */
epos_status_code epos_task_suspend(
  epos_id   id
);

/**
 *  @brief epos_task_resume
 *
 *  This routine implements the epos_task_resume Directive.  The
 *  SUSPENDED state is cleared for task associated with ID.
 */
epos_status_code epos_task_resume(
  epos_id   id
);

/**
 *  @brief epos_task_set_priority
 *
 *  This routine implements the epos_task_set_priority directive.  The
 *  current priority of the task associated with ID is set to
 *  new_priority.  The former priority of that task is returned
 *  in old_priority.
 */
epos_status_code epos_task_set_priority(
  epos_id             id,
  epos_task_priority  new_priority,
  epos_task_priority *old_priority
);

/**
 *  @brief epos_task_start
 *
 *  This routine implements the epos_task_start directive.  The
 *  starting execution point of the task associated with ID is
 *  set to entry_point with the initial argument.
 */
epos_status_code epos_task_start(
  epos_id     id,
  epos_task_entry entry_point,
  epos_task_argument     argument
);

/**
 *  @brief epos_task_wake_when
 *
 *  This routine implements the epos_task_wake_when directive.  The
 *  calling task is blocked until the current time of day is
 *  equal to that indicated by time_buffer.
 */
epos_status_code epos_task_wake_when(
  epos_time_of_day *time_buffer
);

/**
 *  @brief epos_task_wake_after
 *
 *  This routine implements the epos_task_wake_after directive.  The
 *  calling task is blocked until the indicated number of clock
 *  ticks have occurred.
 */
epos_status_code epos_task_wake_after(
  epos_interval  ticks
);

/**
 *  @brief epos_task_is_suspended

 *  This directive returns a status indicating whether or not
 *  the specified task is suspended.
 */
epos_status_code epos_task_is_suspended(
  epos_id   id
);

/**
 *  @brief epos_task_variable_add
 *
 *  This directive adds a per task variable.
 */
epos_status_code epos_task_variable_add(
  epos_id  tid,
  void    **ptr,
  void    (*dtor)(void *)
);

/**
 *  @brief epos_task_variable_get
 *
 *  This directive gets the value of a task variable.
 */
epos_status_code epos_task_variable_get(
  epos_id tid,
  void **ptr,
  void **result
);

/**
 *  @brief epos_task_variable_delete
 *
 *  This directive removes a per task variable.
 */
epos_status_code epos_task_variable_delete(
  epos_id  tid,
  void    **ptr
);

/**
 *  @brief epos_task_self
 *
 *  This directive returns the ID of the currently executing task.
 */
epos_id epos_task_self(void);

/**
 *  @brief _RTEMS_tasks_Initialize_user_tasks
 *
 *  This routine creates and starts all configured user
 *  initialzation threads.
 *
 *  Input parameters: NONE
 *
 *  Output parameters:  NONE
 */
void _RTEMS_tasks_Initialize_user_tasks( void );

/**
 *  @brief _RTEMS_Tasks_Invoke_task_variable_dtor(
 *
 *  This routine invokes the optional user provided destructor on the
 *  task variable and frees the memory for the task variable.
 *
 *  Input parameters: NONE
 *
 *  Output parameters:  NONE
 */
void _RTEMS_Tasks_Invoke_task_variable_dtor(
  Thread_Control        *the_thread,
  epos_task_variable_t *tvp
);

#ifndef __RTEMS_APPLICATION__
#ifndef _OCC_NO_INLINE_
#include <rtems/rtems/tasks.inl>
#endif

#ifdef _OCC_NO_INLINE_
RTEMS_INLINE_ROUTINE Thread_Control *_RTEMS_tasks_Allocate( void );
RTEMS_INLINE_ROUTINE void _RTEMS_tasks_Free (
  Thread_Control *the_task
);
RTEMS_INLINE_ROUTINE Priority_Control _RTEMS_tasks_Priority_to_Core(
  epos_task_priority   priority
);
RTEMS_INLINE_ROUTINE bool _RTEMS_tasks_Priority_is_valid (
  epos_task_priority the_priority
);

#endif
#endif


#ifdef __cplusplus
}
#endif

/**@}*/

#endif
/* end of include file */
