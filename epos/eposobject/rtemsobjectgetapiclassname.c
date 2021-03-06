/*
 *  RTEMS Object Helper -- Obtain Name of API
 *
 *  COPYRIGHT (c) 1989-2008.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: rtemsobjectgetapiclassname.c,v 1.7 2009/11/30 15:59:55 ralf Exp $
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <rtems/system.h>
#include <rtems/score/object.h>
#include <rtems/rtems/object.h>

#include <rtems/assoc.h>

epos_assoc_t epos_object_api_internal_assoc[] = {
  { "Thread",                  OBJECTS_INTERNAL_THREADS, 0},
  { "Mutex",                   OBJECTS_INTERNAL_MUTEXES, 0},
};

epos_assoc_t epos_object_api_classic_assoc[] = {
  { "Task",                    OBJECTS_RTEMS_TASKS, 0},
  { "Timer",                   OBJECTS_RTEMS_TIMERS, 0},
  { "Semaphore",               OBJECTS_RTEMS_SEMAPHORES, 0},
  { "Message Queue",           OBJECTS_RTEMS_MESSAGE_QUEUES, 0},
  { "Partition",               OBJECTS_RTEMS_PARTITIONS, 0},
  { "Region",                  OBJECTS_RTEMS_REGIONS, 0},
  { "Port",                    OBJECTS_RTEMS_PORTS, 0},
  { "Period",                  OBJECTS_RTEMS_PERIODS, 0},
  { "Extension",               OBJECTS_RTEMS_EXTENSIONS, 0},
  { "Barrier",                 OBJECTS_RTEMS_BARRIERS, 0},
};

#ifdef RTEMS_POSIX_API
epos_assoc_t epos_object_api_posix_assoc[] = {
  { "Thread",                  OBJECTS_POSIX_THREADS, 0},
  { "Key",                     OBJECTS_POSIX_KEYS, 0},
  { "Interrupt",               OBJECTS_POSIX_INTERRUPTS, 0},
  { "Queued fd",               OBJECTS_POSIX_MESSAGE_QUEUE_FDS, 0},
  { "Message Queue",           OBJECTS_POSIX_MESSAGE_QUEUES, 0},
  { "Mutex",                   OBJECTS_POSIX_MUTEXES, 0},
  { "Semaphore",               OBJECTS_POSIX_SEMAPHORES, 0},
  { "Condition Variable",      OBJECTS_POSIX_CONDITION_VARIABLES, 0},
  { "Timer",                   OBJECTS_POSIX_TIMERS, 0},
  { "Barrier",                 OBJECTS_POSIX_BARRIERS, 0},
  { "Spinlock",                OBJECTS_POSIX_SPINLOCKS, 0},
  { "RWLock",                  OBJECTS_POSIX_RWLOCKS, 0},
};
#endif

const char *epos_object_get_api_class_name(
  int the_api,
  int the_class
)
{
  const epos_assoc_t *api_assoc;
  const epos_assoc_t *class_assoc;

  if ( the_api == OBJECTS_INTERNAL_API )
    api_assoc = epos_object_api_internal_assoc;
  else if ( the_api == OBJECTS_CLASSIC_API )
    api_assoc = epos_object_api_classic_assoc;
#ifdef RTEMS_POSIX_API
  else if ( the_api == OBJECTS_POSIX_API )
    api_assoc = epos_object_api_posix_assoc;
#endif

  else
    return "BAD API";
  class_assoc = epos_assoc_ptr_by_local( api_assoc, the_class );
  if ( class_assoc )
    return class_assoc->name;
  return "BAD CLASS";
}

