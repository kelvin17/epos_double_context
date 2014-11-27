/**
 * @file
 *
 * @ingroup libcsupport
 *
 * @brief  epos_filesystem_evaluate_path()
 *
 *  Routine to seed the evaluate path routine.
 *
 *  COPYRIGHT (c) 1989-1999.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: eval.c,v 1.16 2010/05/27 16:29:37 ralf Exp $
 */

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <rtems.h>
#include <rtems/libio_.h>
#include <rtems/seterr.h>
#include <errno.h>

int epos_filesystem_evaluate_relative_path(
  const char                        *pathname,
  size_t                             pathnamelen,
  int                                flags,
  epos_filesystem_location_info_t  *pathloc,
  int                                follow_link
)
{
  int                           result;
  epos_filesystem_node_types_t type;

  /*
   * Verify Input parameters.
   */

  if ( !pathname )
    epos_set_errno_and_return_minus_one( EFAULT );

  if ( !pathloc )
    epos_set_errno_and_return_minus_one( EIO );       /* should never happen */

  if ( !pathloc->ops->evalpath_h )
    epos_set_errno_and_return_minus_one( ENOTSUP );

  result = (*pathloc->ops->evalpath_h)( pathname, pathnamelen, flags, pathloc );

  /*
   * Get the Node type and determine if you need to follow the link or
   * not.
   */

  if ( (result == 0) && follow_link ) {

    if ( !pathloc->ops->node_type_h ){
      epos_filesystem_freenode( pathloc );
      epos_set_errno_and_return_minus_one( ENOTSUP );
    }

    type = (*pathloc->ops->node_type_h)( pathloc );

    if ( ( type == RTEMS_FILESYSTEM_HARD_LINK ) ||
         ( type == RTEMS_FILESYSTEM_SYM_LINK ) ) {

        if ( !pathloc->ops->eval_link_h ){
          epos_filesystem_freenode( pathloc );
          epos_set_errno_and_return_minus_one( ENOTSUP );
        }

        /* what to do with the valid node pathloc points to
         * if eval_link_h fails?
         * Let the FS implementation deal with this case.  It
         * should probably free pathloc in either case:
         *  - if the link evaluation fails, it must free the
         *    original (valid) pathloc because we are going
         *    to return -1 and hence the FS generics won't
         *    cleanup pathloc.
         *  - if the link evaluation is successful, the updated
         *    pathloc will be passed up (and eventually released).
         *    Hence, the (valid) originial node that we submit to
         *    eval_link_h() should be released by the handler.
         */

        result =  (*pathloc->ops->eval_link_h)( pathloc, flags );
    }
  }

  return result;
}

int epos_filesystem_evaluate_path(
  const char                        *pathname,
  size_t                             pathnamelen,
  int                                flags,
  epos_filesystem_location_info_t  *pathloc,
  int                                follow_link
)
{
  int                           i = 0;

  /*
   * Verify Input parameters.
   */

  if ( !pathname )
    epos_set_errno_and_return_minus_one( EFAULT );

  if ( !pathloc )
    epos_set_errno_and_return_minus_one( EIO );       /* should never happen */

  /*
   * Evaluate the path using the optable evalpath.
   */

  epos_filesystem_get_start_loc( pathname, &i, pathloc );

  /*
   * We evaluation the path relative to the start location we get got.
   */
  return epos_filesystem_evaluate_relative_path( &pathname[i],
                                                  pathnamelen - i,
                                                  flags,
                                                  pathloc,
                                                  follow_link );
}

int epos_filesystem_dirname(
  const char  *pathname
)
{
  int len = strlen( pathname );

  while ( len ) {
    len--;
    if ( epos_filesystem_is_separator( pathname[len] ) )
      break;
  }

  return len;
}

int epos_filesystem_prefix_separators(
  const char  *pathname,
  int          pathnamelen
)
{
  /*
   * Eat any separators at start of the path.
   */
  int stripped = 0;
  while ( *pathname && pathnamelen && epos_filesystem_is_separator( *pathname ) )
  {
    pathname++;
    pathnamelen--;
    stripped++;
  }
  return stripped;
}
