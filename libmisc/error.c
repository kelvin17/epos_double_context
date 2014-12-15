/**
 * @file
 *
 * @ingroup libcsupport
 *
 * @brief  report errors and panics to RTEMS' stderr.
 *  Currently just used by RTEMS monitor.
 *
 *  $Id: error.c,v 1.22 2009/10/12 16:17:37 ralf Exp $
 */

#if HAVE_CONFIG_H
#include "config.h"
#endif

/*
 * These routines provide general purpose error reporting.
 * epos_error reports an error to stderr and allows use of
 * printf style formatting.  A newline is appended to all messages.
 *
 * error_flag can be specified as any of the following:
 *
 *  	RTEMS_ERROR_ERRNO       -- include errno text in output
 *  	RTEMS_ERROR_PANIC       -- halts local system after output
 *  	RTEMS_ERROR_ABORT       -- abort after output
 *
 * It can also include a epos_status value which can be OR'd
 * with the above flags. *
 *
 * EXAMPLE
 *	#include "rtems.h"
 *	#include "rtems/error.h"
 *	epos_error(0, "stray interrupt %d", intr);
 *
 * EXAMPLE
 *        if ((status = epos_task_create(...)) != RTEMS_SUCCCESSFUL)
 *        {
 *            epos_error(status | RTEMS_ERROR_ABORT,
 *                        "could not create task");
 *        }
 *
 * EXAMPLE
 *        if ((fd = open(pathname, O_RDNLY)) < 0)
 *        {
 *            epos_error(RTEMS_ERROR_ERRNO, "open of '%s' failed", pathname);
 *            goto failed;
 *        }
 */

#define __RTEMS_VIOLATE_KERNEL_VISIBILITY__
#include <rtems.h>

#include <rtems/error.h>
#include <rtems/assoc.h>
#include <inttypes.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>		/* _exit() */

int          epos_panic_in_progress;

const epos_assoc_t epos_status_assoc[] = {
    { "successful completion",              RTEMS_SUCCESSFUL, 0 },
    { "returned from a thread",             RTEMS_TASK_EXITTED, 0 },
    { "multiprocessing not configured",     RTEMS_MP_NOT_CONFIGURED, 0 },
    { "invalid object name",                RTEMS_INVALID_NAME, 0 },
    { "invalid object id",                  RTEMS_INVALID_ID, 0 },
    { "too many",                           RTEMS_TOO_MANY, 0 },
    { "timed out waiting",                  RTEMS_TIMEOUT, 0 },
    { "object deleted while waiting",       RTEMS_OBJECT_WAS_DELETED, 0 },
    { "specified size was invalid",         RTEMS_INVALID_SIZE, 0 },
    { "address specified is invalid",       RTEMS_INVALID_ADDRESS, 0 },
    { "number was invalid",                 RTEMS_INVALID_NUMBER, 0 },
    { "item has not been initialized",      RTEMS_NOT_DEFINED, 0 },
    { "resources still outstanding",        RTEMS_RESOURCE_IN_USE, 0 },
    { "request not satisfied",              RTEMS_UNSATISFIED, 0 },
    { "thread is in wrong state",           RTEMS_INCORRECT_STATE, 0 },
    { "thread already in state",            RTEMS_ALREADY_SUSPENDED, 0 },
    { "illegal on calling thread",          RTEMS_ILLEGAL_ON_SELF, 0 },
    { "illegal for remote object",          RTEMS_ILLEGAL_ON_REMOTE_OBJECT, 0 },
    { "called from wrong environment",      RTEMS_CALLED_FROM_ISR, 0 },
    { "invalid thread priority",            RTEMS_INVALID_PRIORITY, 0 },
    { "invalid date/time",                  RTEMS_INVALID_CLOCK, 0 },
    { "invalid node id",                    RTEMS_INVALID_NODE, 0 },
    { "directive not configured",           RTEMS_NOT_CONFIGURED, 0 },
    { "not owner of resource",              RTEMS_NOT_OWNER_OF_RESOURCE , 0 },
    { "directive not implemented",          RTEMS_NOT_IMPLEMENTED, 0 },
    { "RTEMS inconsistency detected",       RTEMS_INTERNAL_ERROR, 0 },
    { "could not get enough memory",        RTEMS_NO_MEMORY, 0 },
    { "driver IO error",                    RTEMS_IO_ERROR, 0 },
    { "internal multiprocessing only",      THREAD_STATUS_PROXY_BLOCKING, 0 },
    { 0, 0, 0 },
};

const char *
epos_status_text(
    epos_status_code status
)
{
    return epos_assoc_name_by_local(epos_status_assoc, status);
}

static int epos_verror(
    epos_error_code_t error_flag,
    const char   *printf_format,
    va_list      arglist
)
{
    int               local_errno = 0;
    int               chars_written = 0;
    epos_status_code status;

    if (error_flag & RTEMS_ERROR_PANIC)
    {
        if (epos_panic_in_progress++)
            _Thread_Disable_dispatch();       /* disable task switches */

        /* don't aggravate things */
        if (epos_panic_in_progress > 2)
            return 0;
    }

    (void) fflush(stdout);  	    /* in case stdout/stderr same */

    status = error_flag & ~RTEMS_ERROR_MASK;
    if (error_flag & RTEMS_ERROR_ERRNO)     /* include errno? */
        local_errno = errno;


 
    chars_written += vfprintf(stderr, printf_format, arglist);

    if (status)
        chars_written += fprintf(stderr, " (status: %s)", epos_status_text(status));

    if (local_errno)
    {
      if ((local_errno > 0) && *strerror(local_errno))
        chars_written += fprintf(stderr, " (errno: %s)", strerror(local_errno));
      else
        chars_written += fprintf(stderr, " (unknown errno=%d)", local_errno);
    }

    chars_written += fprintf(stderr, "\n");

    (void) fflush(stderr);

    if (error_flag & (RTEMS_ERROR_PANIC | RTEMS_ERROR_ABORT))
    {
        if (error_flag & RTEMS_ERROR_PANIC)
        {
            epos_error(0, "fatal error, exiting");
            _exit(local_errno);
        }
        else
        {
            epos_error(0, "fatal error, aborting");
            abort();
        }
    }
    return chars_written;
}

/*
 * Report an error.
 * error_flag is as above; printf_format is a normal
 * printf(3) format string, with its concommitant arguments.
 *
 * Returns the number of characters written.
 */

int epos_error(
    epos_error_code_t error_flag,
    const char *printf_format,
    ...
  )
{
    va_list arglist;
    int chars_written;

    va_start(arglist, printf_format);
    chars_written = epos_verror(error_flag, printf_format, arglist);
    va_end(arglist);

    return chars_written;
}

/*
 * epos_panic is shorthand for epos_error(RTEMS_ERROR_PANIC, ...)
 */

void epos_panic(
    const char *printf_format,
    ...
  )
{
    va_list arglist;

    va_start(arglist, printf_format);
    (void) epos_verror(RTEMS_ERROR_PANIC, printf_format, arglist);
    va_end(arglist);
}

