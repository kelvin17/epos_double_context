/**
 * @file rtems/libio_.h
 */

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
 *  $Id: libio_.h,v 1.34.2.4 2011/07/31 14:12:29 joel Exp $
 */

#ifndef _RTEMS_RTEMS_LIBIO_INTERNAL_H
#define _RTEMS_RTEMS_LIBIO_INTERNAL_H

#include <rtems.h>
#include <rtems/libio.h>                /* include before standard IO */

#include <sys/types.h>

#include <errno.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 *  Semaphore to protect the io table
 */

#define RTEMS_LIBIO_SEM         epos_build_name('L', 'B', 'I', 'O')
#define RTEMS_LIBIO_IOP_SEM(n)  epos_build_name('L', 'B', 'I', n)

extern epos_id                          epos_libio_semaphore;
extern const epos_filesystem_file_handlers_r epos_filesystem_null_handlers;

/*
 *  File descriptor Table Information
 */

extern uint32_t        epos_libio_number_iops;
extern epos_libio_t  *epos_libio_iops;
extern epos_libio_t  *epos_libio_last_iop;
extern epos_libio_t *epos_libio_iop_freelist;

/*
 *  epos_libio_iop
 *
 *  Macro to return the file descriptor pointer.
 */

#define epos_libio_iop(_fd) \
  ((((uint32_t)(_fd)) < epos_libio_number_iops) ? \
         &epos_libio_iops[_fd] : 0)

/*
 *  epos_libio_iop_to_descriptor
 *
 *  Macro to convert an internal file descriptor pointer (iop) into
 *  the integer file descriptor used by the "section 2" system calls.
 */

#define epos_libio_iop_to_descriptor(_iop) \
   ((!(_iop)) ? -1 : (_iop - epos_libio_iops))

/*
 *  epos_libio_check_is_open
 *
 *  Macro to check if a file descriptor is actually open.
 */

#define epos_libio_check_is_open(_iop) \
  do {                                               \
      if (((_iop)->flags & LIBIO_FLAGS_OPEN) == 0) { \
          errno = EBADF;                             \
          return -1;                                 \
      }                                              \
  } while (0)

/* 
 *  epos_libio_check_fd
 *
 *  Macro to check if a file descriptor number is valid.
 */

#define epos_libio_check_fd(_fd) \
  do {                                                     \
      if ((uint32_t) (_fd) >= epos_libio_number_iops) {   \
          errno = EBADF;                                   \
          return -1;                                       \
      }                                                    \
  } while (0)

/*
 *  epos_libio_check_buffer
 *
 *  Macro to check if a buffer pointer is valid.
 */

#define epos_libio_check_buffer(_buffer) \
  do {                                    \
      if ((_buffer) == 0) {               \
          errno = EINVAL;                 \
          return -1;                      \
      }                                   \
  } while (0)

/*
 *  epos_libio_check_count
 *
 *  Macro to check if a count or length is valid.
 */

#define epos_libio_check_count(_count) \
  do {                                  \
      if ((_count) == 0) {              \
          return 0;                     \
      }                                 \
  } while (0)

/*
 *  epos_libio_check_permissions_with_error
 *
 *  Macro to check if a file descriptor is open for this operation.
 *  On failure, return the user specified error.
 */

#define epos_libio_check_permissions_with_error(_iop, _flag, _errno) \
  do {                                                      \
      if (((_iop)->flags & (_flag)) == 0) {                 \
            epos_set_errno_and_return_minus_one( _errno ); \
            return -1;                                      \
      }                                                     \
  } while (0)

/*
 *  epos_libio_check_permissions
 *
 *  Macro to check if a file descriptor is open for this operation.
 *  On failure, return EINVAL
 */

#define epos_libio_check_permissions(_iop, _flag) \
   epos_libio_check_permissions_with_error(_iop, _flag, EINVAL )

/*
 *  epos_filesystem_freenode
 *
 *  Macro to free a node.
 */

#define epos_filesystem_freenode( _node ) \
  do { \
    if ( (_node)->ops )\
      if ( (_node)->ops->freenod_h ) \
        (*(_node)->ops->freenod_h)( (_node) ); \
  } while (0)


/*
 *  External structures
 */
#include <rtems/userenv.h>

extern epos_user_env_t * epos_current_user_env;
extern epos_user_env_t   epos_global_user_env;

/*
 *  Instantiate a private copy of the per user information for the calling task.
 */

epos_status_code epos_libio_set_private_env(void);
epos_status_code epos_libio_share_private_env(epos_id task_id) ;

static inline void epos_libio_lock( void )
{
  epos_semaphore_obtain( epos_libio_semaphore, RTEMS_WAIT, RTEMS_NO_TIMEOUT );
}

static inline void epos_libio_unlock( void )
{
  epos_semaphore_release( epos_libio_semaphore );
}

/*
 *  File Descriptor Routine Prototypes
 */

epos_libio_t *epos_libio_allocate(void);

uint32_t   epos_libio_fcntl_flags(
  uint32_t   fcntl_flags
);

uint32_t   epos_libio_to_fcntl_flags(
  uint32_t   flags
);

void epos_libio_free(
  epos_libio_t *iop
);

int epos_libio_is_open_files_in_fs(
  epos_filesystem_mount_table_entry_t *mt_entry
);

int epos_libio_is_file_open(
  void  *node_access
);

/*
 *  File System Routine Prototypes
 */

int epos_filesystem_evaluate_relative_path(
  const char                        *pathname,
  size_t                             pathnamelen,
  int                                flags,
  epos_filesystem_location_info_t  *pathloc,
  int                                follow_link
);

int epos_filesystem_evaluate_path(
  const char                        *pathname,
  size_t                             pathnamelen,
  int                                flags,
  epos_filesystem_location_info_t  *pathloc,
  int                                follow_link
);

int epos_filesystem_dirname(
  const char  *pathname
);

int epos_filesystem_prefix_separators(
  const char  *pathname,
  int          pathnamelen
);

void epos_filesystem_initialize(void);

int init_fs_mount_table(void);

int epos_filesystem_is_separator(char ch);

void epos_filesystem_get_start_loc(const char *path,
				    int *index,
				    epos_filesystem_location_info_t *loc);

void epos_filesystem_get_sym_start_loc(const char *path,
					int *index,
					epos_filesystem_location_info_t *loc);

#ifdef __cplusplus
}
#endif

#endif
/* end of include file */
