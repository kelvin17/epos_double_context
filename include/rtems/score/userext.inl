#ifdef _OCC_NO_INLINE_
#include <rtems/system.h>
#include <rtems/score/userext.h>
#endif

#ifndef _RTEMS_SCORE_USEREXT_H
# error "Never use "userext.inl" directly; include "userext.h" instead."
#endif

#ifndef _RTEMS_SCORE_USEREXT_INL
#define _RTEMS_SCORE_USEREXT_INL

RTEMS_INLINE_ROUTINE void _User_extensions_Add_API_set(
  User_extensions_Control *extension
)
{
  _User_extensions_Add_set( extension );
}

RTEMS_INLINE_ROUTINE void _User_extensions_Add_set_with_table(
  User_extensions_Control     *extension,
  const User_extensions_Table *extension_table
)
{
  extension->Callouts = *extension_table;

  _User_extensions_Add_set( extension );
}

#endif
