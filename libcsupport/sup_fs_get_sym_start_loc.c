/**
  * @file src/sup_fs_get_sym_start_loc.c
  *
  * @ingroup libcsupport
  *
  * @brief 
  */

/*
 *
 *  COPYRIGHT (c) 1989-1999.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: sup_fs_get_sym_start_loc.c,v 1.1.2.2 2010/07/01 14:10:52 sh Exp $
 */

/*
 *  epos_filesystem_get_sym_start_loc
 *
 *  Function to determine if path is absolute or relative
 *
 *  Parameters:
 *
 *  path : IN  - path to be checked
 *  index: OUT - 0, if relative, 1 if absolute
 *  loc  : OUT - location info of root fs if absolute
 *               location info of current fs if relative
 *
 *  Returns: void
 */

/* Includes */

#include <rtems/libio_.h>
  
void epos_filesystem_get_sym_start_loc(const char *path,
					int *index,
					epos_filesystem_location_info_t *loc)
{
  if (epos_filesystem_is_separator(path[0])) {
      *loc = epos_filesystem_root;
      *index = 1;
    }
    else {
      *index = 0;
    }
}
