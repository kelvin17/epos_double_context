/*
 *  Exercise Object Manager Services
 *
 *  COPYRIGHT (c) 1989-2008.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id$
 */

#define CONFIGURE_INIT
#include "system.h"

/*
#undef epos_build_id
#undef epos_build_name
#undef epos_object_id_api_maximum
#undef epos_object_id_api_minimum
#undef epos_object_id_get_api
#undef epos_object_id_get_class
#undef epos_object_id_get_index
#undef epos_object_id_get_node
*/
 
epos_id         main_task;
epos_name       main_name;

void print_class_info(
  uint32_t                            api,
  uint32_t                            class,
  epos_object_api_class_information *info
)
{
  printk(
    "%s API %s Information\n"
    "    minimum id  : 0x%08x maximum id: 0x%08x\n"
    "    maximum     :    %7d available : %d\n"
    "    auto_extend : %s\n",
    epos_object_get_api_name(api),
    epos_object_get_api_class_name(api, class),
    info->minimum_id,
    info->maximum_id,
    info->maximum,
    info->unallocated,
    ((info->auto_extend) ? "yes" : "no")
  );
}

void change_name(
  epos_id    id,
  const char *newName,
  bool        printable
)
{
  epos_status_code    status;
  char                 name[ 5 ];
  char                *ptr;
  const char          *c;

  printk( "epos_object_set_name - change name of init task to " );
  if ( printable )
    printk( "(%s)\n", newName );
  else {
    printk( "(" );
    for (c=newName ; *c ; ) {
       if (isprint(*c)) printk( "%c", *c );
       else             printk( "0x%02x", *c );
       c++;
       if ( *c )
         printk( "-" );
    }
    printk( ")\n" );
  }

  status = epos_object_set_name( id, newName );
  directive_failed( status, "epos_object_set_name" );

  status = epos_object_get_classic_name( id, &main_name );
  directive_failed( status, "epos_object_get_classic_name" );
  put_name( main_name, FALSE );
  printk( " - name returned by epos_object_get_classic_name" );

  ptr = epos_object_get_name( id, 5, name );
  epos_test_assert(ptr != NULL);
  printk( "epos_object_get_name returned (%s) for init task\n", ptr );
}

epos_task Init(
  epos_task_argument argument
)
{
  epos_status_code                   status;
  epos_id                            tmpId;
  epos_name                          tmpName;
  char                                name[5];
  char                               *ptr;
  const char                          newName[5] = "New1";
  char                                tmpNameString[5];
  uint32_t                            part;
  epos_object_api_class_information  info;

  printk( "\n\n*** TEST 43 ***" );

 // printk( "RTEMS Version: %s\n", epos_get_version_string() );

  main_task = epos_task_self();

  printk( "epos_object_get_classic_name - INVALID_ADDRESS" );
  status = epos_object_get_classic_name( main_task, NULL );
  fatal_directive_status(
    status,
    RTEMS_INVALID_ADDRESS,
    "epos_object_get_classic_name #1"
  );

  printk( "epos_object_get_classic_name - INVALID_ID (bad index)" );
  status = epos_object_get_classic_name( main_task + 5, &main_name );
  fatal_directive_status(
    status,
    RTEMS_INVALID_ID,
    "epos_object_get_classic_name #2"
  );

  printk( "epos_object_get_classic_name - INVALID_ID (unallocated index)" );
  status = epos_object_get_classic_name( main_task + 1, &main_name );
  fatal_directive_status(
    status,
    RTEMS_INVALID_ID,
    "epos_object_get_classic_name #4"
  );

  printk( "epos_object_get_classic_name - INVALID_ID (bad API)" );
  tmpId = epos_build_id( 0xff, OBJECTS_RTEMS_TASKS, 1, 1 ),
  status = epos_object_get_classic_name( tmpId, &main_name );
  fatal_directive_status(
    status,
    RTEMS_INVALID_ID,
    "epos_object_get_classic_name #5"
  );

  status = epos_object_get_classic_name( main_task, &main_name );
  directive_failed( status, "epos_object_get_classic_name" );
  put_name( main_name, FALSE );
  printk( " - name returned by epos_object_get_classic_name for Init task id" );

  status = epos_object_get_classic_name( RTEMS_SELF, &main_name );
  directive_failed( status, "epos_object_get_classic_name" );
  put_name( main_name, FALSE );
  printk( " - name returned by epos_object_get_classic_name for RTEMS_SELF" );

  tmpName = epos_build_name( 'T', 'E', 'M', 'P' );
  put_name( tmpName, FALSE );
  printk( " - epos_build_name for TEMP" );

  
  /*
   * epos_object_get_name - cases
   */
  printk( "epos_object_get_name - bad id for class with instances" );
  ptr = epos_object_get_name( main_task + 5, 5, name );
  epos_test_assert(ptr == NULL);

  printk( "epos_object_get_name - bad id for class without instances" );
  ptr = epos_object_get_name(
    epos_build_id( OBJECTS_CLASSIC_API, OBJECTS_RTEMS_BARRIERS, 1, 1 ),
    5,
    name
  );
  epos_test_assert(ptr == NULL);

  printk( "epos_object_get_name - bad length" );
  ptr = epos_object_get_name( main_task, 0, name );
  epos_test_assert(ptr == NULL);

  printk( "epos_object_get_name - bad pointer" );
  ptr = epos_object_get_name( main_task, 5, NULL );
  epos_test_assert(ptr == NULL);

  ptr = epos_object_get_name( main_task, 5, name );
  epos_test_assert(ptr != NULL);
  printk( "epos_object_get_name returned (%s) for init task id\n", ptr );

  ptr = epos_object_get_name( RTEMS_SELF, 5, name );
  epos_test_assert(ptr != NULL);
  printk( "epos_object_get_name returned (%s) for RTEMS_SELF\n", ptr );

  /*
   * epos_object_set_name - errors
   */

  printk( "epos_object_set_name - INVALID_ID (bad API)" );
  tmpId = epos_build_id( 0xff, OBJECTS_RTEMS_TASKS, 1, 1 ),
  status = epos_object_set_name( tmpId, newName );
  fatal_directive_status(
    status,
    RTEMS_INVALID_ID,
    "epos_object_set_name #1"
  );

  printk( "epos_object_set_name - INVALID_ID (bad index)" );
  status = epos_object_set_name( main_task + 10, newName );
  fatal_directive_status(
    status,
    RTEMS_INVALID_ID,
    "epos_object_set_name #2"
  );

  /*
   * epos_object_set_name - change name of init task in various ways.
   *
   * This is strange but pushes the SuperCore code to do different things.
   */

  change_name( main_task,  "New1", TRUE );
  change_name( main_task, "Ne1", TRUE );
  change_name( main_task, "N1", TRUE );
  change_name( main_task, "N", TRUE );
  change_name( main_task, "", TRUE );
  tmpNameString[0] = 'N';
  tmpNameString[1] = 0x07;
  tmpNameString[2] = 0x09;
  tmpNameString[3] = '1';
  tmpNameString[4] = '\0';
  change_name( main_task, tmpNameString, FALSE );

  /*
   * Change object name using SELF ID
   */

  change_name( RTEMS_SELF,  "SELF", TRUE );

  ptr = epos_object_get_name( main_task, 5, name );
  epos_test_assert(ptr != NULL);
  printk( "epos_object_get_name returned (%s) for init task id\n", ptr );


  /*
   * Exercise id build and extraction routines
   */

  printk( "epos_build_id - build an id to match init task" );
  tmpId = epos_build_id( OBJECTS_CLASSIC_API, OBJECTS_RTEMS_TASKS, 1, 1 );
  epos_test_assert( tmpId == main_task );
  
  printk( "epos_object_id_get_api - OK" );
  part = epos_object_id_get_api( main_task );
  epos_test_assert( part == OBJECTS_CLASSIC_API );

  printk( "epos_object_id_get_class - OK" );
  part = epos_object_id_get_class( main_task );
  epos_test_assert( part == OBJECTS_RTEMS_TASKS );

  printk( "epos_object_id_get_node - OK" );
  part = epos_object_id_get_node( main_task );
  epos_test_assert( part == 1 );

  printk( "epos_object_id_get_index - OK" );
  part = epos_object_id_get_index( main_task );
  epos_test_assert( part == 1 );

  /*
   * Start another screen and do the API/Class min/max routines
   */
  epos_test_pause();

  printk( "epos_object_id_api_minimum returned %d\n",
          epos_object_id_api_minimum() );
  printk( "epos_object_id_api_maximum returned %d\n",
          epos_object_id_api_maximum() );

  printk( "epos_object_api_minimum_class(0) returned %d\n",
          epos_object_api_minimum_class(0) );
  printk( "epos_object_api_maximum_class(0) returned %d\n",
          epos_object_api_maximum_class(0) );

  printk( "epos_object_api_minimum_class(0) returned %d\n",
          epos_object_api_minimum_class(0) );
  printk( "epos_object_api_maximum_class(0) returned %d\n",
          epos_object_api_maximum_class(0) );
  printk( "epos_object_api_minimum_class(255) returned %d\n",
          epos_object_api_minimum_class(255) );
  printk( "epos_object_api_maximum_class(255) returned %d\n",
          epos_object_api_maximum_class(255) );

  printk( "epos_object_api_minimum_class(OBJECTS_INTERNAL_API) returned %d\n",
          epos_object_api_minimum_class(OBJECTS_INTERNAL_API) );
  printk( "epos_object_api_maximum_class(OBJECTS_INTERNAL_API) returned %d\n",
          epos_object_api_maximum_class(OBJECTS_INTERNAL_API) );

  printk( "epos_object_api_minimum_class(OBJECTS_CLASSIC_API) returned %d\n",
          epos_object_api_minimum_class(OBJECTS_CLASSIC_API) );
  printk( "epos_object_api_maximum_class(OBJECTS_CLASSIC_API) returned %d\n",
          epos_object_api_maximum_class(OBJECTS_CLASSIC_API) );

  printk( "epos_object_api_minimum_class(OBJECTS_ITRON_API) returned %d\n",
          epos_object_api_minimum_class(OBJECTS_ITRON_API) );
  printk( "epos_object_api_maximum_class(OBJECTS_ITRON_API) returned %d\n",
          epos_object_api_maximum_class(OBJECTS_ITRON_API) );

  /*
   *  Another screen break for the API and class name tests
   */
  epos_test_pause();
 
  printk( "epos_object_get_api_name(0) = %s\n", epos_object_get_api_name(0) );
  printk( "epos_object_get_api_name(255) = %s\n",
    epos_object_get_api_name(255));

  printk( "epos_object_get_api_name(INTERNAL_API) = %s\n",
     epos_object_get_api_name(OBJECTS_INTERNAL_API) );
  printk( "epos_object_get_api_name(CLASSIC_API) = %s\n",
     epos_object_get_api_name(OBJECTS_CLASSIC_API) );
  printk( "epos_object_get_api_name(ITRON_API) = %s\n",
     epos_object_get_api_name(OBJECTS_ITRON_API) );

  printk( "epos_object_get_api_class_name(0, RTEMS_TASKS) = %s\n", 
    epos_object_get_api_class_name( 0, OBJECTS_RTEMS_TASKS ) );
  printk( "epos_object_get_api_class_name(CLASSIC_API, 0) = %s\n", 
    epos_object_get_api_class_name( OBJECTS_CLASSIC_API, 0 ) );
  printk("epos_object_get_api_class_name(INTERNAL_API, MUTEXES) = %s\n", 
    epos_object_get_api_class_name(
       OBJECTS_INTERNAL_API, OBJECTS_INTERNAL_MUTEXES));
  printk("epos_object_get_api_class_name(CLASSIC_API, RTEMS_BARRIERS) = %s\n", 
    epos_object_get_api_class_name(
       OBJECTS_CLASSIC_API, OBJECTS_RTEMS_BARRIERS));

  /*
   *  Another screen break for the information
   */

  epos_test_pause();
  
  printk( "epos_object_get_class_information - INVALID_ADDRESS" );
  status = epos_object_get_class_information(
             OBJECTS_INTERNAL_API, OBJECTS_INTERNAL_THREADS, NULL );
  fatal_directive_status(
    status,
    RTEMS_INVALID_ADDRESS,
    "epos_object_get_class_information"
  );

  printk( "epos_object_get_class_information - INVALID_NUMBER (bad API)" );
  status =
    epos_object_get_class_information(0, OBJECTS_INTERNAL_THREADS, &info);
  fatal_directive_status(
    status,
    RTEMS_INVALID_NUMBER,
    "epos_object_get_class_information (API)"
  );

  printk( "epos_object_get_class_information - INVALID_NUMBER (bad class)" );
  status =
    epos_object_get_class_information( OBJECTS_INTERNAL_API, 0, &info );
  fatal_directive_status(
    status,
    RTEMS_INVALID_NUMBER,
    "epos_object_get_class_information (API)"
  );

  printk( "epos_object_get_class_information - Classic Tasks - OK" );
  status = epos_object_get_class_information(
             OBJECTS_CLASSIC_API, OBJECTS_RTEMS_TASKS, &info );
  directive_failed( status, "epos_object_get_class_information" );
  print_class_info( OBJECTS_CLASSIC_API, OBJECTS_RTEMS_TASKS, &info );

  printk( "epos_object_get_class_information - Classic Timers - OK" );
  status = epos_object_get_class_information(
             OBJECTS_CLASSIC_API, OBJECTS_RTEMS_TIMERS, &info );
  directive_failed( status, "epos_object_get_class_information" );
  print_class_info( OBJECTS_CLASSIC_API, OBJECTS_RTEMS_TIMERS, &info );


  printk( "*** END OF TEST 43 ***" );
  epos_test_exit( 0 );
}
