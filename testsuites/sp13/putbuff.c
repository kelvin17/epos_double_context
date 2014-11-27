/*  Put_buffer
 *
 *  This test routine prints the given buffer.
 *  buffer.
 *
 *  Input parameters:
 *    buffer - pointer to message buffer to be printer
 *
 *  Output parameters:  NONE
 *
 *  COPYRIGHT (c) 1989-1999.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id$
 */

#include "system.h"

void Put_buffer(
  long *buffer
)
{
  int i, j;
  char c;

  for ( i = 0; i < MESSAGE_SIZE / sizeof(long); i++ ) {
    for ( j = sizeof(long) - 1; j >= 0; j-- ) {
      if ( (c = (buffer[i] >> (8 * j)) & 0xFF) == 0 )
        return;
      printk( "%c", c );
    }
  }
}
