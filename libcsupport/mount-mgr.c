/**
 * @file
 *
 * @ingroup libcsupport
 *
 * @brief  mount()
 *
 *  Mange the mount table. You can iterate on mounts and file systems, as well
 *  as add and remove file systems not in the file system confiration table.
 *
 *  COPYRIGHT (c) Chris Johns <chrisj@rtems.org> 2010.
 *
 *  Copyright (c) 2010 embedded brains GmbH.
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: mount-mgr.c,v 1.2.2.2 2010/07/01 15:18:06 sh Exp $
 */

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <sys/types.h>
#include <sys/stat.h>
#include <rtems/chain.h>
#include <rtems/seterr.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <rtems/libio_.h>

typedef struct {
  epos_chain_node node;
  epos_filesystem_table_t entry;
} filesystem_node;

static RTEMS_CHAIN_DEFINE_EMPTY(filesystem_chain);

bool epos_filesystem_iterate(
  epos_per_filesystem_routine routine,
  void *routine_arg
)
{
  const epos_filesystem_table_t *table_entry = &epos_filesystem_table [0];
  epos_chain_node *node = NULL;
  bool stop = false;

  while ( table_entry->type && !stop ) {
    stop = (*routine)( table_entry, routine_arg );
    ++table_entry;
  }

  if ( !stop ) {
    epos_libio_lock();
    for (
      node = epos_chain_first( &filesystem_chain );
      !epos_chain_is_tail( &filesystem_chain, node ) && !stop;
      node = epos_chain_next( node )
    ) {
      const filesystem_node *fsn = (filesystem_node *) node;

      stop = (*routine)( &fsn->entry, routine_arg );
    }
    epos_libio_unlock();
  }

  return stop;
}

typedef struct {
  const char *type;
  epos_filesystem_fsmount_me_t mount_h;
} find_arg;

static bool find_handler(const epos_filesystem_table_t *entry, void *arg)
{
  find_arg *fa = arg;

  if ( strcmp( entry->type, fa->type ) != 0 ) {
    return false;
  } else {
    fa->mount_h = entry->mount_h;

    return true;
  }
}

epos_filesystem_fsmount_me_t
epos_filesystem_get_mount_handler(
  const char *type
)
{
/*
  find_arg fa = {
    .type = type,
    .mount_h = NULL
  }; change to below,by zwj*/
  find_arg fa;
fa.type=type;
fa.mount_h=NULL;

  if ( type != NULL ) {
    epos_filesystem_iterate( find_handler, &fa );
  }

  return fa.mount_h;
}

int
epos_filesystem_register(
  const char                    *type,
  epos_filesystem_fsmount_me_t  mount_h
)
{
  size_t fsn_size = sizeof( filesystem_node ) + strlen(type) + 1;
  filesystem_node *fsn = malloc( fsn_size );
  char *type_storage = (char *) fsn + sizeof( filesystem_node );

  if ( fsn == NULL )
    epos_set_errno_and_return_minus_one( ENOMEM );

  strcpy(type_storage, type);
  fsn->entry.type = type_storage;
  fsn->entry.mount_h = mount_h;

  epos_libio_lock();
  if ( epos_filesystem_get_mount_handler( type ) == NULL ) {
    epos_chain_append( &filesystem_chain, &fsn->node );
  } else {
    epos_libio_unlock();
    free( fsn );

    epos_set_errno_and_return_minus_one( EINVAL );
  }
  epos_libio_unlock();

  return 0;
}

int
epos_filesystem_unregister(
  const char *type
)
{
  epos_chain_node *node = NULL;

  if ( type == NULL ) {
    epos_set_errno_and_return_minus_one( EINVAL );
  }

  epos_libio_lock();
  for (
    node = epos_chain_first( &filesystem_chain );
    !epos_chain_is_tail( &filesystem_chain, node );
    node = epos_chain_next( node )
  ) {
    filesystem_node *fsn = (filesystem_node *) node;

    if ( strcmp( fsn->entry.type, type ) == 0 ) {
      epos_chain_extract( node );
      free( fsn );
      epos_libio_unlock();

      return 0;
    }
  }
  epos_libio_unlock();

  epos_set_errno_and_return_minus_one( ENOENT );
}
