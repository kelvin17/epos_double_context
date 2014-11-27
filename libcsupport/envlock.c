/**
 * @file
 *
 * @ingroup libcsupport
 * 
 * @brief provide locking for the global environment 'environ' 
 */

/*
 *  Author: Till Straumann <strauman@slac.stanford.edu>, 3/2002
 *
 *  $Id: envlock.c,v 1.5 2010/04/30 08:55:40 sh Exp $
 */

/* provide locking for the global environment 'environ' */

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <rtems.h>
#include <sys/reent.h>

#include <assert.h>

/*
 * NOTES:
 *  - although it looks like a classical multiple-readers / single writer (MRSW)
 *    locking problem, we still use a single lock for the following reasons:
 *     1) newlib has no provision / hook for calling different locking routines
 *        from setenv/putenv and getenv, respectively.
 *     2) MRSW involves calling several semaphore-primitives, even in the most
 *        likely case of a first-reader's access. This probably takes more CPU
 *        time than just waiting until another reader is done; environment
 *        access is fast.
 *  - the lock implementation must allow nesting (same thread may call
 *    lock-lock-unlock-unlock).
 *  - NEWLIB-1.8.2 has an ugly BUG: if environ is NULL, _findenv_r() bails
 *    out leaving the lock held :-(
 *
 *  Used by the following functions:
 *    findenv_r(), setenv_r(), and unsetenv_r() which are called by
 *    getenv(), getenv_r(), setenv(), and unsetenv().
 *
 */

#if defined(ENVLOCK_DEDIDCATED_MUTEX)
static epos_id envLock=0;

static void
__epos_envlock_init(void)
{
  extern char        **environ;
  epos_status_code    rc;

  if (envLock) /* already initialized */
    return;

  assert(environ && "MUST have non-NULL 'environ' due to newlib bug");

  rc = epos_semaphore_create(
      epos_build_name('E','N','V','S'),
      1,
      RTEMS_BINARY_SEMAPHORE | RTEMS_PRIORITY | RTEMS_INHERIT_PRIORITY,
      0,
      &envLock);
  if (RTEMS_SUCCESSFUL!=rc)
    epos_fatal_error_occurred(rc);
}

void
__env_lock(struct _reent *r)
{
  /* Do lazy init */
  if (!envLock)
    __epos_envlock_init();
  /*
   *  Must not use a semaphore before pre-tasking hook is called.
   *  - it will corrupt memory :-(
   */

  if (_Thread_Executing)
    epos_semaphore_obtain(envLock, RTEMS_WAIT, RTEMS_NO_TIMEOUT);
}

void
__env_unlock(struct _reent *r)
{
  /*
   *  Must not use a semaphore before pre-tasking hook is called.
   * - it will corrupt memory :-(
   */
  if (_Thread_Executing)
    epos_semaphore_release(envLock);
}
#else

/*
 *  Reuse the libio mutex -- it is always initialized before we
 *  could possibly run.
 */

#include <rtems/libio_.h>

void
__env_lock(struct _reent *r __attribute__((unused)))
{
  epos_libio_lock();
}

void
__env_unlock(struct _reent *r __attribute__((unused)))
{
  epos_libio_unlock();
}
#endif