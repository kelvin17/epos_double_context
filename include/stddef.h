/*
 * @(#)stddef.h	
 */

#ifndef __stddef_included
#define __stddef_included

#ifndef __ptrdiff_t_defined     
#define __ptrdiff_t_defined     
#	include <target/pdiff.h>
#endif

#ifndef __size_t_defined
#define __size_t_defined
#	include <target/size.h>
#endif

#ifndef __wchar_t_defined
#define __wchar_t_defined
#	include <target/wchar.h>
#endif

#define NULL	( (void*) 0 )

#define offsetof(type,member) \
	((size_t)((char *)(&((type *)0)->member) - (char *)0))

#endif  /* __stddef_included */
