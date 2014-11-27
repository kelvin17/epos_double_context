/**
 * @file
 *
 * @ingroup libcsupport
 *
 * @brief  write() - POSIX 1003.1b 6.4.2 - Write to a File
 */

/*
 *  write() - POSIX 1003.1b 6.4.2 - Write to a File
 *
 *  COPYRIGHT (c) 1989-2007.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: write.c,v 1.16.4.1 2011/07/31 14:12:29 joel Exp $
 */

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <rtems/libio_.h>
#include <rtems/seterr.h>
#include <errno.h>

#include <string.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

/*
 * write
 *
 * This routine writes count bytes from from buffer pointed to by buffer
 * to the file associated with the open file descriptor, fildes.
 */
	
#if 0
extern	int _eof (int fd);
#endif

ssize_t write(
  int         fd,
  const void *buffer,
  size_t      count
)
{
  ssize_t  rc;
  epos_libio_t     *iop;

  epos_libio_check_fd( fd );
  iop = epos_libio_iop( fd );
  epos_libio_check_is_open( iop );
  epos_libio_check_buffer( buffer );
  epos_libio_check_count( count );
  epos_libio_check_permissions_with_error( iop, LIBIO_FLAGS_WRITE, EBADF );

  
   //  Now process the write() request.
   

  if ( !iop->handlers->write_h )
    epos_set_errno_and_return_minus_one( ENOTSUP );

  rc = (*iop->handlers->write_h)( iop, buffer, count );

  if ( rc > 0 )
    iop->offset += rc;

  return rc;



#if 0
  int res = SP_SUCCESS;
int writecount = 0;
int len;


len = -1;
/* Grab server connection.											*/
PSERVER ();
/* Do a number of blocks, each packet max 1024 bytes.				*/
while (nbytes > 0 && res == SP_SUCCESS) {
int count = MAXMSGSZ - 7;
if (nbytes < count)
	count = nbytes;

/* Request PUTBLOCK.											*/
_sbuffer[0] = SP_PUTBLOCK;
/*SETINT32 (&_sbuffer[1], fd);*/
/*SETINT16 (&_sbuffer[5], count);*/
/*memcpy (&_sbuffer[7], buf, count);*/
_sbuffer[1] = fd;
_sbuffer[2] = count;
if(buf != &_sbuffer[3])
   memcpy (&_sbuffer[3], buf, count);
/*res = _iserver (7+count, 3);*/
res = _iserver (4+count, 3);

/* If successful, update counters.								*/
if (res == SP_SUCCESS) {
	/*len = INT16 (&_sbuffer[1]);*/
	len = _sbuffer[1];
	if (len == (unsigned short) -1)
	break;
	buf = (char *) buf + len;
	writecount += len;
	nbytes -= len;
}
}

/* Write error (pserver extension). 								*/
if (len == (unsigned short) -1) {
errno = -1;
(void) _ferror (fd);
res = -1;
}

/* EOF/error before any bytes written - go check.					*/
else if (res != SP_SUCCESS && writecount == 0) {
if (_eof (fd))
	res = 0;
else {
	errno = -1;
	(void) _ferror (fd);
	res = -1;
}
}
else
res = writecount;

/* Release server connection.										*/
VSERVER ();

/* Return number of bytes written, or -1 in case of an error.		*/
return res;
#endif
}
