/*
 *  Extension Manager
 *
 *
 *  COPYRIGHT (c) 1989-1998.
 *  On-Line Applications Research Corporation (OAR).
 *  Copyright assigned to U.S. Government, 1994.
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.OARcorp.com/rtems/license.html.
 *
 *  $Id: extension.c,v 1.12 1998/02/17 23:44:14 joel Exp $
 */

#include <rtems/system.h>
#include <rtems/config.h>
#include <rtems/rtems/support.h>
#include <rtems/score/object.h>
#include <rtems/score/thread.h>
#include <rtems/extension.h>

/*PAGE
 *
 *  _Extension_Manager_initialization
 *
 *  This routine initializes all extension manager related data structures.
 *
 *  Input parameters:
 *    maximum_extensions - number of extensions to initialize
 *
 *  Output parameters:  NONE
 */

void _Extension_Manager_initialization(
)
{
  _Objects_Initialize_information(
    &_Extension_Information,	 /* object information table */
    OBJECTS_CLASSIC_API,	 	 /* object API */
    OBJECTS_RTEMS_EXTENSIONS,	 /* object class */
    Configuration.maximum_extensions,/* maximum objects of this class */
    sizeof( Extension_Control ),/* size of this object's control block */
    FALSE,						/* true if the name is a string */
    RTEMS_MAXIMUM_NAME_LENGTH	/* maximum length of an object name */
#if defined(RTEMS_MULTIPROCESSING)
	  ,
	  true, 					   /* true if this is a global object class */
	  NULL						   /* Proxy extraction support callout */
#endif
  );
}

/*PAGE
 *
 *  rtems_extension_create
 *
 *  This directive creates a extension and performs some initialization.
 *
 *  Input parameters:
 *    name            - extension name
 *    extension_table - pointer to extension set information
 *    id              - pointer to extension id
 *
 *  Output parameters:
 *    id                - extension id
 *    RTEMS_SUCCESSFUL - if successful
 *    error code        - if unsuccessful
 */

epos_status_code epos_extension_create(
	epos_name					 name,
	const epos_extensions_table *extension_table,
	epos_id 					*id
)
{
  Extension_Control *the_extension;

  if ( !epos_is_name_valid( name ) )
    return RTEMS_INVALID_NAME;

  _Thread_Disable_dispatch();         /* to prevent deletion */

  the_extension = _Extension_Allocate();

  if ( !the_extension ) {
    _Thread_Enable_dispatch();
    return RTEMS_TOO_MANY;
  }

  _User_extensions_Add_set_with_table( &the_extension->Extension, extension_table );

  _Objects_Open( &_Extension_Information, &the_extension->Object, &name );

  *id = the_extension->Object.id;
  _Thread_Enable_dispatch();
  return RTEMS_SUCCESSFUL;
}

/*PAGE
 *
 *  rtems_extension_ident
 *
 *  This directive returns the system ID associated with
 *  the extension name.
 *
 *  Input parameters:
 *    name - user defined message queue name
 *    id   - pointer to extension id
 *
 *  Output parameters:
 *    *id               - message queue id
 *    RTEMS_SUCCESSFUL - if successful
 *    error code        - if unsuccessful
 */

epos_status_code epos_extension_ident(
  epos_name    name,
  Objects_Id   *id
)
{
  Objects_Name_or_id_lookup_errors  status;

  status = _Objects_Name_to_id_u32(
    &_Extension_Information,
    &name,
    OBJECTS_SEARCH_LOCAL_NODE,
    id
  );

  return _Status_Object_name_errors_to_status[ status ];
}

/*PAGE
 *
 *  rtems_extension_delete
 *
 *  This directive allows a thread to delete a extension.
 *
 *  Input parameters:
 *    id - extension id
 *
 *  Output parameters:
 *    RTEMS_SUCCESSFUL - if successful
 *    error code - if unsuccessful
 */

epos_status_code epos_extension_delete(
  Objects_Id id
)
{
  Extension_Control   *the_extension;
  Objects_Locations    location;

  the_extension = _Extension_Get( id, &location );
  switch ( location ) {
    case OBJECTS_ERROR:
#if defined(RTEMS_MULTIPROCESSING)		
    case OBJECTS_REMOTE:            /* should never return this */
#endif
      return RTEMS_INVALID_ID;
    case OBJECTS_LOCAL:
      _User_extensions_Remove_set( &the_extension->Extension );
      _Objects_Close( &_Extension_Information, &the_extension->Object );
      _Extension_Free( the_extension );
      _Thread_Enable_dispatch();
      return RTEMS_SUCCESSFUL;
  }

  return RTEMS_INTERNAL_ERROR;   /* unreached - only to remove warnings */
}

