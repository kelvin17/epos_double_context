/*
 * @(#)hostcomm.h	3304.6 07/06/03
 * 
 * COPYRIGHT NOTICE (NOT TO BE REMOVED):
 * 
 * This file, or parts of it, or modified versions of it, may not be copied,
 * reproduced or transmitted in any form, including reprinting, translation,
 * photocopying or microfilming, or by any means, electronic, mechanical or
 * otherwise, or stored in a retrieval system, or used for any purpose, without
 * the prior written permission of all Owners unless it is explicitly marked as
 * having Classification `Public'.
 * 
 * Owners of this file give notice:
 *   (c) Copyright 1998 ACE Associated Compiler Experts bv
 *   (c) Copyright 1991-1993,1998 ACE Associated Computer Experts bv
 * All rights, including copyrights, reserved.
 * 
 * This file contains or may contain restricted information and is UNPUBLISHED
 * PROPRIETARY SOURCE CODE OF THE Owners.  The Copyright Notice(s) above do not
 * evidence any actual or intended publication of such source code.  This file
 * is additionally subject to the conditions listed in the RESTRICTIONS file
 * and is with NO WARRANTY.
 * 
 * END OF COPYRIGHT NOTICE
 */

/*
 *  Host communication Link I/O interface module header file.
 *
 */

#ifndef _hostcomm_included
#define _hostcomm_included
#define SINGLE_THREAD // by zwj


/* Some headers that are always needed for host communication.          */
#include    <bsp/drivers/hostcomm/hxproto.h>                 /* proto defs */
//#include    <sem.h>                     /* to grab comm sema */by zwj
#include    <errno.h>                   /* errno set in case of any failure */

#ifndef SINGLE_THREAD
extern  sem_t   _serversem;
#endif

/* Message buffer used to compose request and return reply.             */
//char   _sbuffer[MAXMAXPACKSZ];
extern char * _sbuffer;

/* Server channel semaphore also guards message buffer.                 */
#ifndef SINGLE_THREAD
#define PSERVER()   (__semp (&_serversem))
#define VSERVER()   (__semv (&_serversem))
#else
#define PSERVER()   /* empty */
#define VSERVER()   /* empty */
#endif

/* Value to set errno to in case request packet is too large to handle  */
/* (due to long names).                                                 */
#define EPACKSZ     EINVAL          /* XXX - should do better than this */

/* Handle iserver request.  Request size (in _sbuffer) is reqsz,        */
/* and, if successful, at least expsz bytes should be returned.         */
/* Returns request status (== _sbuffer[0]).                             */
extern  int     _iserver (int reqsz, int expsz);

/* Get errno if service returned SP_ERROR.                              */
/* XXX - this info should really be part of the reply packet            */
extern  int _ferror (int fd);
extern	int _eof (int fd);

#endif  /* _hostcomm_included */

