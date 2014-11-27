/**
 * @file
 *
 * @ingroup libcsupport
 *
 * @brief _rename_r() - POSIX 1003.1b - 5.3.4 - Rename a file
 */

/*
 *  _rename_r() - POSIX 1003.1b - 5.3.4 - Rename a file
 *
 *  COPYRIGHT (c) 1989-2007.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: _rename_r.c,v 1.6 2010/05/20 03:10:41 ccj Exp $
 */

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <rtems.h>
#if defined(RTEMS_NEWLIB) && !defined(HAVE__RENAME_R)
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/errno.h>
#include <unistd.h>

#include <rtems/libio_.h>
#include <rtems/seterr.h>
 
int  _rename_r(
  struct _reent *ptr ,
  const char    *old,
  const char    *new
)
{
  int                                 old_parent_pathlen;
  epos_filesystem_location_info_t    old_loc;
  epos_filesystem_location_info_t    old_parent_loc;
  epos_filesystem_location_info_t    new_parent_loc;
  int                                 i;
  int                                 result;
  const char                         *name;
  bool                                free_old_parentloc = false;

  /*
   *  Get the parent node of the old path to be renamed. Find the parent path.
   */

  old_parent_pathlen = epos_filesystem_dirname ( old );

  if ( old_parent_pathlen == 0 )
    epos_filesystem_get_start_loc( old, &i, &old_parent_loc );
  else {
    result = epos_filesystem_evaluate_path( old, old_parent_pathlen,
                                             RTEMS_LIBIO_PERMS_WRITE,
                                             &old_parent_loc,
                                             false );
    if ( result != 0 )
      return -1;

    free_old_parentloc = true;
  }

  /*
   * Start from the parent to find the node that should be under it.
   */

  old_loc = old_parent_loc;
  name = old + old_parent_pathlen;
  name += epos_filesystem_prefix_separators( name, strlen( name ) );

  result = epos_filesystem_evaluate_relative_path( name , strlen( name ),
                                                    0, &old_loc, false );
  if ( result != 0 ) {
    if ( free_old_parentloc )
      epos_filesystem_freenode( &old_parent_loc );
    return -1;
  }
  
  /*
   * Get the parent of the new node we are renaming to.
   */

  epos_filesystem_get_start_loc( new, &i, &new_parent_loc );

  if ( !new_parent_loc.ops->evalformake_h ) {
    if ( free_old_parentloc )
      epos_filesystem_freenode( &old_parent_loc );
    epos_filesystem_freenode( &old_loc );
    epos_set_errno_and_return_minus_one( ENOTSUP );
  }

  result = (*new_parent_loc.ops->evalformake_h)( &new[i], &new_parent_loc, &name );
  if ( result != 0 ) {
    epos_filesystem_freenode( &new_parent_loc );
    if ( free_old_parentloc )
      epos_filesystem_freenode( &old_parent_loc );
    epos_filesystem_freenode( &old_loc );
    return -1;
  }

  /*
   *  Check to see if the caller is trying to rename across file system
   *  boundaries.
   */

  if ( old_parent_loc.mt_entry != new_parent_loc.mt_entry ) {
    epos_filesystem_freenode( &new_parent_loc );
    if ( free_old_parentloc )
      epos_filesystem_freenode( &old_parent_loc );
    epos_filesystem_freenode( &old_loc );
    epos_set_errno_and_return_minus_one( EXDEV );
  }

  if ( !new_parent_loc.ops->rename_h ) {
    epos_filesystem_freenode( &new_parent_loc );
    if ( free_old_parentloc )
      epos_filesystem_freenode( &old_parent_loc );
    epos_filesystem_freenode( &old_loc );
    epos_set_errno_and_return_minus_one( ENOTSUP );
  }

  result = (*new_parent_loc.ops->rename_h)( &old_parent_loc, &old_loc, &new_parent_loc, name );

  epos_filesystem_freenode( &new_parent_loc );
  if ( free_old_parentloc )
    epos_filesystem_freenode( &old_parent_loc );
  epos_filesystem_freenode( &old_loc );

  return result;
}

#if 0
  struct stat sb;
  int s;

  s = stat( old, &sb);
  if ( s < 0 )
    return s;
  s = link( old, new );
  if ( s < 0 )
    return s;
  return S_ISDIR(sb.st_mode) ? rmdir( old ) : unlink( old );
#endif
                                            
#endif
