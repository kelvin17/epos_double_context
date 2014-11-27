/*
 * @(#)stdarg.h	
 */

/* This file is target dependent, given contents are only an example
 * and should be made compliant to the actual target
 *
 * Three possible implementation of variable arguments are presented
 */

/* Specialized version, works in combination with the use of the
 * 'lowervarargs' engine
 */
#if defined(_LOWERVARARGS)

typedef void **va_list;

extern void	**__builtin_va_start(void);
#define va_start(ap, parmN)	((void) ((ap) = __builtin_va_start()))
#define va_arg(ap, type)	(*(type*)*(ap)++)
#define va_end(ap)		((void) 0)

#elif defined(_STACK_DECREMENT)

/* This is a possible implementation, assuming variables are nicely
 * placed on the stack (e.g. structures are passed as structures, not
 * as pointers to a structure), no alignment issues exist, and
 * a next parameter is placed at a lower address
 * 
 * Under all different circumstances, a different targetted set of macros is
 * required.
 */
typedef void *va_list;

#define va_start(ap, parmN) ((void) ((ap) = (void *) (&(parmN))))
#define va_arg(ap, type)    (* --(* (type **) &(ap)))
#define va_end(ap)          ((void) 0)

#else

/* This is a possible implementation, assuming variables are nicely
 * placed on the stack (e.g. structures are passed as structures, not
 * as pointers to a structure), no alignment issues exist, and
 * a next parameter is placed at a higher address
 * 
 * Under all different circumstances, a different targetted set of macros is
 * required.
 */
typedef char *va_list;

#define va_start(ap, parmN) ((void) ((ap) = (void *) (&(parmN)+1)))
#define va_arg(ap, type)    (* (* (type **) &(ap))++)
#define va_end(ap)          ((void) 0)

#endif	/* _LOWERVARARGS _STACK_DECREMENT */
