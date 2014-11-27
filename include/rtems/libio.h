/**
 * @file
 *
 * @ingroup LibIO
 *
 * @brief Basic IO API.
 */

/*
 *  COPYRIGHT (c) 1989-2008.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: libio.h,v 1.65.2.5 2010/08/27 17:33:11 joel Exp $
 */

#ifndef _RTEMS_RTEMS_LIBIO_H
#define _RTEMS_RTEMS_LIBIO_H

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/statvfs.h>

#include <unistd.h>
#include <termios.h>

#include <rtems.h>
#include <rtems/fs.h>
#include <rtems/chain.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup LibIO IO Library
 *
 * @brief Provides system call and file system interface definitions.
 *
 * General purpose communication channel for RTEMS to allow UNIX/POSIX
 * system call behavior under RTEMS.  Initially this supported only
 * IO to devices but has since been enhanced to support networking
 * and support for mounted file systems.
 *
 * @{
 */

/**
 * @brief A 64-bit file offset for internal use by RTEMS. Based on the Newlib
 * type.
 */
typedef _off64_t epos_off64_t;

/**
 * @name File System Node Types
 *
 * @{
 */

#define RTEMS_FILESYSTEM_DIRECTORY   1
#define RTEMS_FILESYSTEM_DEVICE      2
#define RTEMS_FILESYSTEM_HARD_LINK   3
#define RTEMS_FILESYSTEM_SYM_LINK    4
#define RTEMS_FILESYSTEM_MEMORY_FILE 5

/** @} */

typedef int epos_filesystem_node_types_t;

/**
 * @name File System Node Operations
 *
 * @{
 */

typedef int (*epos_filesystem_open_t)(
  epos_libio_t *iop,
  const char    *pathname,
  uint32_t       flag,
  uint32_t       mode
);

typedef int (*epos_filesystem_close_t)(
  epos_libio_t *iop
);

typedef ssize_t (*epos_filesystem_read_t)(
  epos_libio_t *iop,
  void          *buffer,
  size_t         count
);

typedef ssize_t (*epos_filesystem_write_t)(
  epos_libio_t *iop,
  const void    *buffer,
  size_t         count
);

typedef int (*epos_filesystem_ioctl_t)(
  epos_libio_t *iop,
  uint32_t       command,
  void          *buffer
);

typedef epos_off64_t (*epos_filesystem_lseek_t)(
  epos_libio_t *iop,
  epos_off64_t  length,
  int            whence
);

typedef int (*epos_filesystem_fstat_t)(
  epos_filesystem_location_info_t *loc,
  struct stat                      *buf
);

typedef int (*epos_filesystem_fchmod_t)(
  epos_filesystem_location_info_t *loc,
  mode_t                            mode
);

typedef int (*epos_filesystem_ftruncate_t)(
  epos_libio_t *iop,
  epos_off64_t  length
);

typedef int (*epos_filesystem_fpathconf_t)(
  epos_libio_t *iop,
  int name
);

typedef int (*epos_filesystem_fsync_t)(
  epos_libio_t *iop
);

typedef int (*epos_filesystem_fdatasync_t)(
  epos_libio_t *iop
);

typedef int (*epos_filesystem_fcntl_t)(
  int            cmd,
  epos_libio_t *iop
);

typedef int (*epos_filesystem_rmnod_t)(
 epos_filesystem_location_info_t      *parent_loc,   /* IN */
 epos_filesystem_location_info_t      *pathloc       /* IN */
);

/** @} */

/**
 * @brief File system node operations table.
 */
struct _epos_filesystem_file_handlers_r {
    epos_filesystem_open_t         open_h;
    epos_filesystem_close_t        close_h;
    epos_filesystem_read_t         read_h;
    epos_filesystem_write_t        write_h;
    epos_filesystem_ioctl_t        ioctl_h;
    epos_filesystem_lseek_t        lseek_h;
    epos_filesystem_fstat_t        fstat_h;
    epos_filesystem_fchmod_t       fchmod_h;
    epos_filesystem_ftruncate_t    ftruncate_h;
    epos_filesystem_fpathconf_t    fpathconf_h;
    epos_filesystem_fsync_t        fsync_h;
    epos_filesystem_fdatasync_t    fdatasync_h;
    epos_filesystem_fcntl_t        fcntl_h;
    epos_filesystem_rmnod_t        rmnod_h;
};

/**
 * @name File System Operations
 *
 * @{
 */

/*
 *  XXX
 *  This routine does not allocate any space and epos_filesystem_freenode_t
 *  is not called by the generic after calling this routine.
 *  ie. node_access does not have to contain valid data when the
 *      routine returns.
 */
typedef int (*epos_filesystem_mknod_t)(
   const char                        *path,       /* IN */
   mode_t                             mode,       /* IN */
   dev_t                              dev,        /* IN */
   epos_filesystem_location_info_t  *pathloc     /* IN/OUT */
);

/*
 *  epos_filesystem_freenode_t must be called by the generic after
 *  calling this routine
 */

typedef int (*epos_filesystem_evalpath_t)(
  const char                        *pathname,      /* IN     */
  size_t                             pathnamelen,   /* IN     */
  int                                flags,         /* IN     */
  epos_filesystem_location_info_t  *pathloc        /* IN/OUT */
);

typedef int (*epos_filesystem_evalmake_t)(
   const char                       *path,       /* IN */
   epos_filesystem_location_info_t *pathloc,    /* IN/OUT */
   const char                      **name        /* OUT    */
);

typedef int (*epos_filesystem_link_t)(
 epos_filesystem_location_info_t  *to_loc,      /* IN */
 epos_filesystem_location_info_t  *parent_loc,  /* IN */
 const char                        *name         /* IN */
);

typedef int (*epos_filesystem_unlink_t)(
 epos_filesystem_location_info_t  *parent_pathloc, /* IN */
 epos_filesystem_location_info_t  *pathloc         /* IN */
);

typedef int (*epos_filesystem_chown_t)(
 epos_filesystem_location_info_t  *pathloc,       /* IN */
 uid_t                              owner,         /* IN */
 gid_t                              group          /* IN */
);

typedef int (*epos_filesystem_freenode_t)(
 epos_filesystem_location_info_t      *pathloc       /* IN */
);

typedef int (* epos_filesystem_mount_t ) (
   epos_filesystem_mount_table_entry_t *mt_entry     /* IN */
);

typedef int (* epos_filesystem_fsmount_me_t )(
  epos_filesystem_mount_table_entry_t *mt_entry,     /* IN */
  const void                           *data          /* IN */
);

typedef int (* epos_filesystem_unmount_t ) (
  epos_filesystem_mount_table_entry_t *mt_entry     /* IN */
);

typedef int (* epos_filesystem_fsunmount_me_t ) (
   epos_filesystem_mount_table_entry_t *mt_entry    /* IN */
);

typedef epos_filesystem_node_types_t (* epos_filesystem_node_type_t) (
  epos_filesystem_location_info_t    *pathloc      /* IN */
);

typedef int (* epos_filesystem_utime_t)(
  epos_filesystem_location_info_t  *pathloc,       /* IN */
  time_t                             actime,        /* IN */
  time_t                             modtime        /* IN */
);

typedef int (*epos_filesystem_evaluate_link_t)(
  epos_filesystem_location_info_t *pathloc,     /* IN/OUT */
  int                               flags        /* IN     */
);

typedef int (*epos_filesystem_symlink_t)(
 epos_filesystem_location_info_t  *loc,         /* IN */
 const char                        *link_name,   /* IN */
 const char                        *node_name
);

typedef int (*epos_filesystem_readlink_t)(
 epos_filesystem_location_info_t  *loc,     /* IN  */
 char                              *buf,     /* OUT */
 size_t                            bufsize
);

typedef int (*epos_filesystem_rename_t)(
 epos_filesystem_location_info_t  *old_parent_loc,  /* IN */
 epos_filesystem_location_info_t  *old_loc,         /* IN */
 epos_filesystem_location_info_t  *new_parent_loc,  /* IN */
 const char                        *name             /* IN */
);

typedef int (*epos_filesystem_statvfs_t)(
 epos_filesystem_location_info_t  *loc,     /* IN  */
 struct statvfs                    *buf      /* OUT */
);

/** @} */

/**
 * @brief File system operations table.
 */
struct _epos_filesystem_operations_table {
    epos_filesystem_evalpath_t      evalpath_h;
    epos_filesystem_evalmake_t      evalformake_h;
    epos_filesystem_link_t          link_h;
    epos_filesystem_unlink_t        unlink_h;
    epos_filesystem_node_type_t     node_type_h;
    epos_filesystem_mknod_t         mknod_h;
    epos_filesystem_chown_t         chown_h;
    epos_filesystem_freenode_t      freenod_h;
    epos_filesystem_mount_t         mount_h;
    epos_filesystem_fsmount_me_t    fsmount_me_h;
    epos_filesystem_unmount_t       unmount_h;
    epos_filesystem_fsunmount_me_t  fsunmount_me_h;
    epos_filesystem_utime_t         utime_h;
    epos_filesystem_evaluate_link_t eval_link_h;
    epos_filesystem_symlink_t       symlink_h;
    epos_filesystem_readlink_t      readlink_h;
    epos_filesystem_rename_t        rename_h;
    epos_filesystem_statvfs_t       statvfs_h;
};

/**
 * @brief Gets the mount handler for the file system @a type.
 *
 * @return The file system mount handler associated with the @a type, or
 * @c NULL if no such association exists.
 */
epos_filesystem_fsmount_me_t
epos_filesystem_get_mount_handler(
  const char *type
);

/**
 * @brief Contain file system specific information which is required to support
 * fpathconf().
 */
typedef struct {
  int    link_max;                 /* count */
  int    max_canon;                /* max formatted input line size */
  int    max_input;                /* max input line size */
  int    name_max;                 /* max name length */
  int    path_max;                 /* max path */
  int    pipe_buf;                 /* pipe buffer size */
  int    posix_async_io;           /* async IO supported on fs, 0=no, 1=yes */
  int    posix_chown_restrictions; /* can chown: 0=no, 1=yes */
  int    posix_no_trunc;           /* error on names > max name, 0=no, 1=yes */
  int    posix_prio_io;            /* priority IO, 0=no, 1=yes */
  int    posix_sync_io;            /* file can be sync'ed, 0=no, 1=yes */
  int    posix_vdisable;           /* special char processing, 0=no, 1=yes */
} epos_filesystem_limits_and_options_t;

/**
 * @brief Default pathconf settings.
 *
 * Override in a filesystem.
 */
extern const epos_filesystem_limits_and_options_t epos_filesystem_default_pathconf;

/**
 * @brief An open file data structure.
 *
 * It will be indexed by 'fd'.
 *
 * @todo Should really have a separate per/file data structure that this points
 * to (eg: size, offset, driver, pathname should be in that)
 */
struct epos_libio_tt {
  epos_driver_name_t                    *driver;
  epos_off64_t                           size;      /* size of file */
  epos_off64_t                           offset;    /* current offset into file */
  uint32_t                                flags;
  epos_filesystem_location_info_t        pathinfo;
  epos_id                                sem;
  uint32_t                                data0;     /* private to "driver" */
  void                                   *data1;     /* ... */
  void                                   *file_info; /* used by file handlers */
  const epos_filesystem_file_handlers_r *handlers;  /* type specific handlers */
};

/**
 * @brief Paramameter block for read/write.
 *
 * It must include 'offset' instead of using iop's offset since we can have
 * multiple outstanding i/o's on a device.
 */
typedef struct {
  epos_libio_t          *iop;
  epos_off64_t           offset;
  char                   *buffer;
  uint32_t                count;
  uint32_t                flags;
  uint32_t                bytes_moved;
} epos_libio_rw_args_t;

/**
 * @brief Parameter block for open/close.
 */
typedef struct {
  epos_libio_t          *iop;
  uint32_t                flags;
  uint32_t                mode;
} epos_libio_open_close_args_t;

/**
 * @brief Parameter block for ioctl.
 */
typedef struct {
  epos_libio_t          *iop;
  uint32_t                command;
  void                   *buffer;
  uint32_t                ioctl_return;
} epos_libio_ioctl_args_t;

/**
 * @name Flag Values
 *
 * @{
 */

#define LIBIO_FLAGS_NO_DELAY      0x0001  /* return immediately if no data */
#define LIBIO_FLAGS_READ          0x0002  /* reading */
#define LIBIO_FLAGS_WRITE         0x0004  /* writing */
#define LIBIO_FLAGS_OPEN          0x0100  /* device is open */
#define LIBIO_FLAGS_APPEND        0x0200  /* all writes append */
#define LIBIO_FLAGS_CREATE        0x0400  /* create file */
#define LIBIO_FLAGS_CLOSE_ON_EXEC 0x0800  /* close on process exec() */
#define LIBIO_FLAGS_READ_WRITE    (LIBIO_FLAGS_READ | LIBIO_FLAGS_WRITE)

/** @} */

void epos_libio_init(void);

/**
 * @name External I/O Handlers
 *
 * @{
 */

typedef int (*epos_libio_open_t)(
  const char  *pathname,
  uint32_t    flag,
  uint32_t    mode
);

typedef int (*epos_libio_close_t)(
  int  fd
);

typedef int (*epos_libio_read_t)(
  int         fd,
  void       *buffer,
  uint32_t    count
);

typedef int (*epos_libio_write_t)(
  int         fd,
  const void *buffer,
  uint32_t    count
);

typedef int (*epos_libio_ioctl_t)(
  int         fd,
  uint32_t    command,
  void       *buffer
);

typedef epos_off64_t (*epos_libio_lseek_t)(
  int           fd,
  epos_off64_t offset,
  int           whence
);

/** @} */

/**
 * @name Permission Macros
 *
 * @{
 */

/*
 *  The following macros are used to build up the permissions sets
 *  used to check permissions.  These are similar in style to the
 *  mode_t bits and should stay compatible with them.
 */
#define RTEMS_LIBIO_PERMS_READ   S_IROTH
#define RTEMS_LIBIO_PERMS_WRITE  S_IWOTH
#define RTEMS_LIBIO_PERMS_RDWR   (S_IROTH|S_IWOTH)
#define RTEMS_LIBIO_PERMS_EXEC   S_IXOTH
#define RTEMS_LIBIO_PERMS_SEARCH RTEMS_LIBIO_PERMS_EXEC
#define RTEMS_LIBIO_PERMS_RWX    S_IRWXO

/** @} */

union __epos_dev_t {
  dev_t device;
  struct {
     epos_device_major_number major;
     epos_device_minor_number minor;
  } __overlay;
};

static inline dev_t epos_filesystem_make_dev_t(
  epos_device_major_number _major,
  epos_device_minor_number _minor
)
{
  union __epos_dev_t temp;

  temp.__overlay.major = _major;
  temp.__overlay.minor = _minor;
  return temp.device;
}

static inline epos_device_major_number epos_filesystem_dev_major_t(
  dev_t device
)
{
  union __epos_dev_t temp;

  temp.device = device;
  return temp.__overlay.major;
}


static inline epos_device_minor_number epos_filesystem_dev_minor_t(
  dev_t device
)
{
  union __epos_dev_t temp;

  temp.device = device;
  return temp.__overlay.minor;
}

#define epos_filesystem_split_dev_t( _dev, _major, _minor ) \
  do { \
    (_major) = epos_filesystem_dev_major_t ( _dev ); \
    (_minor) = epos_filesystem_dev_minor_t( _dev ); \
  } while(0)

/*
 * Verifies that the permission flag is valid.
 */
#define epos_libio_is_valid_perms( _perm )     \
 (((~RTEMS_LIBIO_PERMS_RWX) & _perm ) == 0)

/*
 *  Prototypes for filesystem
 */

void epos_filesystem_initialize( void );

typedef void (*epos_libio_init_functions_t)(void);
extern  epos_libio_init_functions_t epos_libio_init_helper;

void    open_dev_console(void);

typedef void (*epos_libio_supp_functions_t)(void);
extern  epos_libio_supp_functions_t epos_libio_supp_helper;

typedef void (*epos_fs_init_functions_t)(void);
extern  epos_fs_init_functions_t    epos_fs_init_helper;

/**
 * @brief Creates a directory and all its parent directories according to
 * @a path.
 *
 * The @a mode value selects the access permissions of the directory.
 *
 * @retval 0 Successful operation.
 * @retval -1 An error occured.  The @c errno indicates the error.
 */
extern int epos_mkdir(const char *path, mode_t mode);

/** @} */

/**
 * @defgroup FileSystemTypesAndMount File System Types and Mount
 *
 * @ingroup LibIO
 *
 * @brief File system types and mount.
 *
 * @{
 */

/**
 * @name File System Types
 *
 * @{
 */

#define RTEMS_FILESYSTEM_TYPE_IMFS "imfs"
#define RTEMS_FILESYSTEM_TYPE_MINIIMFS "mimfs"
#define RTEMS_FILESYSTEM_TYPE_DEVFS "devfs"
#define RTEMS_FILESYSTEM_TYPE_FTPFS "ftpfs"
#define RTEMS_FILESYSTEM_TYPE_TFTPFS "tftpfs"
#define RTEMS_FILESYSTEM_TYPE_NFS "nfs"
#define RTEMS_FILESYSTEM_TYPE_DOSFS "dosfs"
#define RTEMS_FILESYSTEM_TYPE_RFS "rfs"

/** @} */

/**
 * @brief Mount table entry.
 */
struct epos_filesystem_mount_table_entry_tt {
  epos_chain_node                       Node;
  epos_filesystem_location_info_t       mt_point_node;
  epos_filesystem_location_info_t       mt_fs_root;
  int                                    options;
  void                                  *fs_info;

  epos_filesystem_limits_and_options_t  pathconf_limits_and_options;

  /*
   * The target or mount point of the file system.
   */
  const char                            *target;

  /*
   * The type of filesystem or the name of the filesystem.
   */
  const char                            *type;

  /*
   *  When someone adds a mounted filesystem on a real device,
   *  this will need to be used.
   *
   *  The lower layers can manage how this is managed. Leave as a
   *  string.
   */
  char                                  *dev;
};

/**
 * @brief File system options.
 */
typedef enum {
  RTEMS_FILESYSTEM_READ_ONLY,
  RTEMS_FILESYSTEM_READ_WRITE,
  RTEMS_FILESYSTEM_BAD_OPTIONS
} epos_filesystem_options_t;

/**
 * @brief File system table entry.
 */
typedef struct epos_filesystem_table_t {
  const char                    *type;
  epos_filesystem_fsmount_me_t  mount_h;
} epos_filesystem_table_t;

/**
 * @brief Static table of file systems.
 *
 * Externally defined by confdefs.h or the user.
 */
extern const epos_filesystem_table_t epos_filesystem_table [];

/**
 * @brief Registers a file system @a type.
 *
 * The @a mount_h handler will be used to mount a file system of this @a type.
 *
 * @retval 0 Successful operation.
 * @retval -1 An error occured.  The @c errno indicates the error.
 */
int epos_filesystem_register(
  const char                    *type,
  epos_filesystem_fsmount_me_t  mount_h
);

/**
 * @brief Unregisters a file system @a type.
 *
 * @retval 0 Successful operation.
 * @retval -1 An error occured.  The @c errno indicates the error.
 */
int epos_filesystem_unregister(
  const char *type
);

/**
 * @brief Unmounts the file system at @a mount_path.
 *
 * @todo Due to file system implementation shortcomings it is possible to
 * unmount file systems in use.  This likely leads to heap corruption.  Unmount
 * only file systems which are not in use by the application.
 *
 * @retval 0 Successful operation.
 * @retval -1 An error occured.  The @c errno indicates the error.
 */
int unmount(
  const char *mount_path
);

/**
 * @brief Mounts a file system at @a target.
 *
 * The @a source may be a path to the corresponding device file, or @c NULL.
 * The @a target path must lead to an existing directory, or @c NULL.  In case
 * @a target is @c NULL, the root file system will be mounted.  The @a data
 * parameter will be forwarded to the file system initialization handler.  The
 * file system type is selected by @a filesystemtype and may be one of
 * - RTEMS_FILESYSTEM_TYPE_DEVFS,
 * - RTEMS_FILESYSTEM_TYPE_DOSFS,
 * - RTEMS_FILESYSTEM_TYPE_FTPFS,
 * - RTEMS_FILESYSTEM_TYPE_IMFS,
 * - RTEMS_FILESYSTEM_TYPE_MINIIMFS,
 * - RTEMS_FILESYSTEM_TYPE_NFS,
 * - RTEMS_FILESYSTEM_TYPE_RFS, or
 * - RTEMS_FILESYSTEM_TYPE_TFTPFS.
 *
 * Only configured or registered file system types are available.  You can add
 * file system types to your application configuration with
 * - CONFIGURE_FILESYSTEM_DEVFS,
 * - CONFIGURE_FILESYSTEM_DOSFS,
 * - CONFIGURE_FILESYSTEM_FTPFS,
 * - CONFIGURE_FILESYSTEM_IMFS,
 * - CONFIGURE_FILESYSTEM_MINIIMFS,
 * - CONFIGURE_FILESYSTEM_NFS,
 * - CONFIGURE_FILESYSTEM_RFS, and
 * - CONFIGURE_FILESYSTEM_TFTPFS.
 *
 * @see epos_filesystem_register() and mount_and_make_target_path().
 *
 * @retval 0 Successful operation.
 * @retval -1 An error occured.  The @c errno indicates the error.
 */
int mount(
  const char                 *source,
  const char                 *target,
  const char                 *filesystemtype,
  epos_filesystem_options_t options,
  const void                 *data
);

/**
 * @brief Mounts a file system and makes the @a target path.
 *
 * The @a target path will be created with epos_mkdir() and must not be
 * @c NULL.
 *
 * @see mount().
 *
 * @retval 0 Successful operation.
 * @retval -1 An error occured.  The @c errno indicates the error.
 */
int mount_and_make_target_path(
  const char                 *source,
  const char                 *target,
  const char                 *filesystemtype,
  epos_filesystem_options_t options,
  const void                 *data
);

/**
 * @brief Per file system type routine.
 *
 * @see epos_filesystem_iterate().
 *
 * @retval true Stop the iteration.
 * @retval false Continue the iteration.
 */
typedef bool (*epos_per_filesystem_routine)(
  const epos_filesystem_table_t *fs_entry,
  void *arg
);

/**
 * @brief Iterates over all file system types.
 *
 * For each file system type the @a routine will be called with the entry and
 * the @a routine_arg parameter.
 *
 * Do not register or unregister file system types in @a routine.
 *
 * The iteration is protected by the IO library mutex.
 *
 * @retval true Iteration stopped due to @a routine return status.
 * @retval false Iteration through all entries.
 */
bool epos_filesystem_iterate(
  epos_per_filesystem_routine routine,
  void *routine_arg
);

/**
 * @brief Per file system mount routine.
 *
 * @see epos_filesystem_mount_iterate().
 *
 * @retval true Stop the iteration.
 * @retval false Continue the iteration.
 */
typedef bool (*epos_per_filesystem_mount_routine)(
  const epos_filesystem_mount_table_entry_t *mt_entry,
  void *arg
);

/**
 * @brief Iterates over all file system mounts.
 *
 * For each file system mount the @a routine will be called with the entry and
 * the @a routine_arg parameter.
 *
 * Do not mount or unmount file systems in @a routine.
 *
 * The iteration is protected by the IO library mutex.
 *
 * @retval true Iteration stopped due to @a routine return status.
 * @retval false Iteration through all entries.
 */
bool
epos_filesystem_mount_iterate(
  epos_per_filesystem_mount_routine routine,
  void *routine_arg
);

/**
 * @brief Boot time mount table entry.
 */
typedef struct {
  const char                              *type;
  epos_filesystem_options_t               fsoptions;
  const char                              *device;
  const char                              *mount_point;
} epos_filesystem_mount_table_t;

extern const epos_filesystem_mount_table_t *epos_filesystem_mount_table;
extern const int                             epos_filesystem_mount_table_size;

/**
 * @brief Creates a directory and all its parrent directories according to
 * @a path.
 *
 * The @a mode value selects the access permissions of the directory.
 *
 * @retval 0 Successful operation.
 * @retval -1 An error occured.  The @c errno indicates the error.
 */
extern int epos_mkdir(const char *path, mode_t mode);

/** @} */

/**
 * @defgroup Termios Termios
 *
 * @ingroup LibIO
 *
 * @brief Termios
 *
 * @{
 */

typedef struct epos_termios_callbacks {
  int    (*firstOpen)(int major, int minor, void *arg);
  int    (*lastClose)(int major, int minor, void *arg);
  int    (*pollRead)(int minor);
  ssize_t (*write)(int minor, const char *buf, size_t len);
  int    (*setAttributes)(int minor, const struct termios *t);
  int    (*stopRemoteTx)(int minor);
  int    (*startRemoteTx)(int minor);
  int    outputUsesInterrupts;
} epos_termios_callbacks;

void epos_termios_initialize (void);

/*
 * CCJ: Change before opening a tty. Newer code from Eric is coming
 * so extra work to handle an open tty is not worth it. If the tty
 * is open, close then open it again.
 */
epos_status_code epos_termios_bufsize (
  int cbufsize,     /* cooked buffer size */
  int raw_input,    /* raw input buffer size */
  int raw_output    /* raw output buffer size */
);

epos_status_code epos_termios_open (
  epos_device_major_number      major,
  epos_device_minor_number      minor,
  void                          *arg,
  const epos_termios_callbacks *callbacks
);

epos_status_code epos_termios_close(
  void *arg
);

epos_status_code epos_termios_read(
  void *arg
);

epos_status_code epos_termios_write(
  void *arg
);

epos_status_code epos_termios_ioctl(
  void *arg
);

int epos_termios_enqueue_raw_characters(
  void *ttyp,
  char *buf,
  int   len
);

int epos_termios_dequeue_characters(
  void *ttyp,
  int   len
);

/** @} */

/**
 * @brief The pathconf setting for a file system.
 */
#define epos_filesystem_pathconf(_mte) ((_mte)->pathconf_limits_and_options)

/**
 * @brief The type of file system. Its name.
 */
#define epos_filesystem_type(_mte) ((_mte)->type)

/**
 * @brief The mount point of a file system.
 */
#define epos_filesystem_mount_point(_mte) ((_mte)->target)

/**
 * @brief The device entry of a file system.
 */
#define epos_filesystem_mount_device(_mte) ((_mte)->dev)

#ifdef __cplusplus
}
#endif

#endif /* _RTEMS_LIBIO_H */
