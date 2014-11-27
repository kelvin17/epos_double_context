/*
 *  Some basic filesystem types
 *
 *  COPYRIGHT (c) 1989-1999.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: fs.h,v 1.7 2010/05/31 13:56:36 ccj Exp $
 */

#ifndef _RTEMS_FS_H
#define _RTEMS_FS_H

#ifdef __cplusplus
extern "C" {
#endif

/*
 *  File descriptor Table Information
 */

/* Forward declarations */

/* FIXME: shouldn't this better not be here? */
typedef struct epos_libio_tt epos_libio_t;

typedef struct epos_filesystem_location_info_tt
    epos_filesystem_location_info_t;

struct epos_filesystem_mount_table_entry_tt;
typedef struct epos_filesystem_mount_table_entry_tt
    epos_filesystem_mount_table_entry_t;

typedef struct _epos_filesystem_file_handlers_r
    epos_filesystem_file_handlers_r;
typedef struct _epos_filesystem_operations_table
    epos_filesystem_operations_table;

/*
 * Structure used to determine a location/filesystem in the tree.
 */

struct epos_filesystem_location_info_tt
{
   void                                    *node_access;
   void                                    *node_access_2;
   const epos_filesystem_file_handlers_r  *handlers;
   const epos_filesystem_operations_table *ops;
   epos_filesystem_mount_table_entry_t    *mt_entry;
};

/*
 * Return the mount table entry for a path location.
 */
#define epos_filesystem_location_mount(_pl) ((_pl)->mt_entry)

#ifdef __cplusplus
}
#endif

#endif
/* end of include file */
