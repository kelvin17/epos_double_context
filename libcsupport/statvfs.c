/**
 * @file
 *
 * @ingroup libcsupport
 *
 * @brief 
 */

/*
 *  COPYRIGHT (c) 2009 Chris Johns <chrisj@rtems.org>
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: statvfs.c,v 1.2.2.1 2010/07/01 15:18:06 sh Exp $
 */
/*
 * The statvfs as defined by the SUS:
 *    http://www.opengroup.org/onlinepubs/009695399/basedefs/sys/statvfs.h.html
 */

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <rtems/libio_.h>
#include <rtems/seterr.h>

#include <sys/statvfs.h>
#include <sys/errno.h>

int
statvfs (const char *path, struct statvfs *sb)
{
  epos_filesystem_location_info_t      loc;
  epos_filesystem_location_info_t     *fs_mount_root;
  epos_filesystem_mount_table_entry_t *mt_entry;
  int                                   result;

  /*
   *  Get
   *    The root node of the mounted filesytem.
   *    The node for the directory that the fileystem is mounted on.
   *    The mount entry that is being refered to.
   */

  if ( epos_filesystem_evaluate_path( path, strlen( path ), 0x0, &loc, true ) )
    return -1;

  mt_entry      = loc.mt_entry;
  fs_mount_root = &mt_entry->mt_fs_root;

  if ( !fs_mount_root->ops->statvfs_h )
    epos_set_errno_and_return_minus_one( ENOTSUP );

  memset (sb, 0, sizeof (struct statvfs));

  result = ( fs_mount_root->ops->statvfs_h )( fs_mount_root, sb );

  epos_filesystem_freenode( &loc );

  return result;
}
