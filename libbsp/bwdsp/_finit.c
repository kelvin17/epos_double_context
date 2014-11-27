/*
 * @(#)_finit.c	3304.17 07/06/03
 * 
 * COPYRIGHT NOTICE (NOT TO BE REMOVED):
 * 
 * This file, or parts of it, or modified versions of it, may not be copied,
 * reproduced or transmitted in any form, including reprinting, translation,
 * photocopying or microfilming, or by any means, electronic, mechanical or
 * otherwise, or stored in a retrieval system, or used for any purpose, without
 * the prior written permission of all Owners unless it is explicitly marked as
 * having Classification `Public'.
 *   Distribution: Run-Time Module, Object.
 * 
 * Owners of this file give notice:
 *   (c) Copyright 1998 ACE Associated Compiler Experts bv
 *   (c) Copyright 1991,1992,1998 ACE Associated Computer Experts bv
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

#include    <stdio.h>
#if 0

FILE    __iotbl[FOPEN_MAX] = { { 0 } };

#define	stdbuf	(&__iotbl[3])
void    _finit (void)
{
    int flags;

    /* Flags for all standard text streams indicate no buffering.       */
    flags = _IONBF;

    /* If host OS is known to be MS-DOS, set _IOTXT flag.               */
    /* flags |= _IOTXT; */

    /* Init stdin, stdout and stderr.                                   */
    stdin->fd     = 0;
    stdin->flags  = _IORD | flags;
    stdout->fd    = 1;
    stdout->flags = _IOWR | flags;
    stderr->fd    = 2;
    stderr->flags = _IOWR | flags;
    /*wanghao add at 2011-4-14*/
    stdbuf->fd    = 4;
    stdbuf->flags = _IOWR | flags;
}
#else
/*TODO: a new _finit is necessary..*/
void    _finit (void)
{
	;
}
#endif

