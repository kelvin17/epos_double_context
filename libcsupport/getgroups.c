/**
 * @file
 *
 * @ingroup libcsupport
 *
 * @brief 
 */

/*
 *  $Id: getgroups.c,v 1.4 2009/09/30 04:46:31 ralf Exp $
 */

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <sys/types.h>
#include <unistd.h>

/*PAGE
 *
 *  4.2.3 Get Supplementary IDs, P1003.1b-1993, p. 86
 */

int getgroups(
  int    gidsetsize ,
  gid_t  grouplist[] 
)
{
  return 0;  /* no supplemental group ids */
}
