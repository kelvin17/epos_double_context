/**
 * @file
 *
 * @ingroup libcsupport
 *
 * @brief 
 */

/*
 *  Instantiate a private user environment for the calling thread.
 *
 *  Submitted by: fernando.ruiz@ctv.es (correo@fernando-ruiz.com)
 *
 *  COPYRIGHT (c) 1989-2000.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: privateenv.c,v 1.12 2010/05/31 13:56:36 ccj Exp $
 */

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdlib.h>	/* free */

#include <rtems.h>
#include <rtems/chain.h>
#include <rtems/libio.h>
#include <rtems/libio_.h>

/* cleanup a user environment
 * NOTE: this must be called with
 *       thread dispatching disabled!
 */
static void
free_user_env(void *venv)
{
  epos_user_env_t *env = (epos_user_env_t*) venv ;

	if (env != &epos_global_user_env
#ifdef HAVE_USERENV_REFCNT
		&& --env->refcnt <= 0
#endif
		) {
		epos_filesystem_freenode( &env->current_directory);
		epos_filesystem_freenode( &env->root_directory);
		free(env);
	}
}

epos_status_code epos_libio_set_private_env(void) {
  epos_status_code 					sc;
  epos_id          					task_id;
  epos_filesystem_location_info_t		loc;

  sc=epos_task_ident(RTEMS_SELF,0,&task_id);
  if (sc != RTEMS_SUCCESSFUL) return sc;

  /* Only for the first time a malloc is necesary */
  if (epos_current_user_env==&epos_global_user_env) {
   epos_user_env_t	*tmp = malloc(sizeof(epos_user_env_t));
   if (!tmp)
     return RTEMS_NO_MEMORY;

#ifdef HAVE_USERENV_REFCNT
   tmp->refcnt = 1;
#endif

   sc = epos_task_variable_add(RTEMS_SELF,(void*)&epos_current_user_env,(void(*)(void *))free_user_env);
   if (sc != RTEMS_SUCCESSFUL) {
	 /* don't use free_user_env because the pathlocs are
	  * not initialized yet
	  */
     free(tmp);
     return sc;
   }
   epos_current_user_env = tmp;
  };

  *epos_current_user_env = epos_global_user_env; /* get the global values*/
  epos_current_user_env->task_id=task_id;         /* mark the local values*/

  /* Clone the pathlocs. In contrast to most other
   * code we must _not_ free the original locs because
   * what we are trying to do here is forking off
   * clones. The reason is a pathloc can be allocated by the
   * file system and needs to be freed when deleting the environment.
   */

  epos_filesystem_evaluate_path("/", 1, 0, &loc, 0);
  epos_filesystem_root    = loc;
  epos_filesystem_evaluate_path("/", 1, 0, &loc, 0);
  epos_filesystem_current = loc;

  return RTEMS_SUCCESSFUL;
}

/*
 *  Share a same private environment beetween two task:
 *   Task_id (remote) and RTEMS_SELF(current).
 */

/* NOTE:
 *
 * THIS CODE HAS NO PROTECTION IMPLEMENTED
 *
 * Tasks who wish to share their environments must
 *
 *  a) assert that no participants are concurrently
 *     executing
 *     libio_share_private_env() and/or libio_set_private_env()
 *
 *  b) mutex access to epos_filesystem_current, epos_filesytem_root
 *     while changing any of those (chdir(), chroot()).
 */

#ifndef HAVE_USERENV_REFCNT
epos_status_code epos_libio_share_private_env(epos_id task_id) {
  epos_status_code  sc;
  epos_user_env_t * shared_user_env;
  epos_id           current_task_id;

  sc=epos_task_ident(RTEMS_SELF,0,&current_task_id);
  if (sc != RTEMS_SUCCESSFUL) return sc;

  if (epos_current_user_env->task_id==current_task_id) {
   /* kill the current user env & task_var*/
	epos_user_env_t 	*tmp = epos_current_user_env;
   sc = epos_task_variable_delete(RTEMS_SELF,(void*)&epos_current_user_env);
   if (sc != RTEMS_SUCCESSFUL) return sc;
   free_user_env(tmp);
  };

  /* AT THIS POINT, epos_current_user_env is DANGLING */

  sc = epos_task_variable_get(task_id,(void*)&epos_current_user_env,
		                       (void*)&shared_user_env       );
  if (sc != RTEMS_SUCCESSFUL)
    goto bailout;

  sc = epos_task_variable_add(RTEMS_SELF,(void*)&epos_current_user_env,free_user_env);
  if (sc != RTEMS_SUCCESSFUL)
    goto bailout;

  /* the current_user_env is the same pointer that remote env */
  epos_current_user_env = shared_user_env;

  /* increase the reference count */
#ifdef HAVE_USERENV_REFCNT
  epos_current_user_env->refcnt++;
#endif

  return RTEMS_SUCCESSFUL;

bailout:
  /* fallback to the global env */
  epos_current_user_env = &epos_global_user_env;
  return sc;
}
#endif
