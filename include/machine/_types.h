/*
 *  $Id: _types.h,v 1.3 2007/09/07 21:16:25 jjohnstn Exp $
 */

#ifndef _MACHINE__TYPES_H
#define _MACHINE__TYPES_H
#include <machine/_default_types.h>


/*add by mahongx@2014-09-12, for bwdsp104x and epos use only*/
#if defined(__rtems__)
#define __OPTIMIZE_SIZE__ 1

typedef __int32_t wint_t;
typedef __int32_t blksize_t;
typedef __int32_t blkcnt_t;

typedef __int32_t _ssize_t;
#define __ssize_t_defined 1

typedef __int32_t _off_t;
#define __off_t_defined 1

typedef __int32_t _fpos_t;
#define __fpos_t_defined 1

typedef __uint32_t _mode_t;
#define __mode_t_defined 1

#endif

#endif
