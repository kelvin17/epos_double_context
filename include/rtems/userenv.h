/*
 *  Libio Internal Information
 *
 *  COPYRIGHT (c) 1989-1999.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: userenv.h,v 1.6 2009/11/29 11:52:52 ralf Exp $
 */

#ifndef _RTEMS_USERENV_H
#define _RTEMS_USERENV_H

#include <rtems.h>
#include <rtems/fs.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 *  External structures
 */

/*
 * According to IEEE Std 1003.1-2001,
 * limits.h is supposed to provide _POSIX_LOGIN_NAME_MAX
 * FIXME: We should not rely on this.
 */
#include <limits.h>

#ifndef LOGIN_NAME_MAX
#ifdef _POSIX_LOGIN_NAME_MAX
#define LOGIN_NAME_MAX _POSIX_LOGIN_NAME_MAX
#else
/* Fallback */
#define LOGIN_NAME_MAX 9
#endif
#endif

typedef struct {
 epos_id                         task_id;
 epos_filesystem_location_info_t current_directory;
 epos_filesystem_location_info_t root_directory;
 /* Default mode for all files. */
 mode_t                           umask;
 nlink_t                          link_counts;
 /* _POSIX_types */
 uid_t                            uid;
 gid_t                            gid;
 uid_t                            euid;
 gid_t                            egid;
 char      login_buffer[LOGIN_NAME_MAX];

 pid_t                            pgrp; /* process group id */
} epos_user_env_t;

extern epos_user_env_t * epos_current_user_env;
extern epos_user_env_t   epos_global_user_env;

#define epos_filesystem_current     (epos_current_user_env->current_directory)
#define epos_filesystem_root        (epos_current_user_env->root_directory)
#define epos_filesystem_link_counts (epos_current_user_env->link_counts)
#define epos_filesystem_umask       (epos_current_user_env->umask)

#define _POSIX_types_Uid             (epos_current_user_env->uid)
#define _POSIX_types_Gid             (epos_current_user_env->gid)
#define _POSIX_types_Euid            (epos_current_user_env->euid)
#define _POSIX_types_Egid            (epos_current_user_env->egid)
#define _POSIX_types_Getlogin_buffer (epos_current_user_env->login_buffer)


/*
 *  Instantiate a private copy of the per user information for the calling task.
 */

epos_status_code epos_libio_set_private_env(void);
epos_status_code epos_libio_share_private_env(epos_id task_id) ;

#ifdef __cplusplus
}
#endif

#endif
/* end of include file */
