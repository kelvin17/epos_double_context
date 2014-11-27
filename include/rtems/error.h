/**
 * @file rtems/error.h
 */

/*
 *  Defines and externs for rtems error reporting
 *
 *  $Id: error.h,v 1.10 2009/10/22 11:20:44 ralf Exp $
 */

#ifndef __RTEMS_ERROR_h
#define __RTEMS_ERROR_h

#include <rtems/score/interr.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef Internal_errors_t epos_error_code_t;

/*
 * epos_error() and epos_panic() support
 */

#if 0
/* not 16bit-int host clean */
#define RTEMS_ERROR_ERRNO  (1<<((sizeof(epos_error_code_t) * CHAR_BIT) - 2)) /* hi bit; use 'errno' */
#define RTEMS_ERROR_PANIC  (RTEMS_ERROR_ERRNO / 2)       /* err fatal; no return */
#define RTEMS_ERROR_ABORT  (RTEMS_ERROR_ERRNO / 4)       /* err is fatal; panic */
#else
#define RTEMS_ERROR_ERRNO  (0x40000000) /* hi bit; use 'errno' */
#define RTEMS_ERROR_PANIC  (0x20000000) /* err fatal; no return */
#define RTEMS_ERROR_ABORT  (0x10000000) /* err is fatal; panic */
#endif

#define RTEMS_ERROR_MASK  (RTEMS_ERROR_ERRNO | RTEMS_ERROR_ABORT | \
                             RTEMS_ERROR_PANIC) /* all */

const char *epos_status_text(epos_status_code);
int   epos_error(epos_error_code_t error_code, const char *printf_format, ...);
#ifdef __GNUC__
void  epos_panic(const char *printf_format, ...);
/*
 *  We should be able to use this attribute but gcc complains that
 *  epos_panic does in fact return. :(
 *
 *      __attribute__ ((__noreturn__));
 */
#else
void  epos_panic(const char *printf_format, ...);
#endif

extern int epos_panic_in_progress;

#ifdef __cplusplus
}
#endif


#endif
/* end of include file */
