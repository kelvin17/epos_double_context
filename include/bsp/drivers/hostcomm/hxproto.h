/*
 * @(#)hxproto.h	3304.6 07/06/03
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
 *   (c) Copyright 1991-1994,1998 ACE Associated Computer Experts bv
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

#ifndef _hxproto_included
#define _hxproto_included


/* Max and min packet and message sizes.                                */
#define MINPACKSZ       8
#define MAXMAXPACKSZ    1024
#define MAXPACKSZ       MAXMAXPACKSZ
#define MINMSGSZ        (MINPACKSZ-VLENSZ)
#define MAXMSGSZ        (MAXPACKSZ-VLENSZ)
/* mahongx2: MAXOBUFFERSZ < MAXMAXPACKSZ - 7*/
#define MAXOBUFFERSZ	80


/* Server results.                                                      */
#define SP_SUCCESS          0
#define SP_UNIMPLEMENTED    1
#define SP_ERROR            129


/* Server tags.                                                         */
#define SP_ZERO         0

#define SP_OPEN         10
#define SP_CLOSE        11
#define SP_READ         12
#define SP_WRITE        13
#define SP_GETS         14
#define SP_PUTS         15
#define SP_FLUSH        16
#define SP_SEEK         17
#define SP_TELL         18
#define SP_EOF          19
#define SP_FERROR       20
#define SP_REMOVE       21
#define SP_RENAME       22
#define SP_GETBLOCK     23
#define SP_PUTBLOCK     24
#define SP_ISATTY       25

#define SP_GETKEY       30
#define SP_POLLKEY      31
#define SP_GETENV       32
#define SP_TIME         33
#define SP_SYSTEM       34
#define SP_EXIT         35

#define SP_COMMAND      40
#define SP_CORE         41
#define SP_ID           42

#define SP_MSDOS        50

#define SP_SUN          60      /* not used */
#define SP_MSC          61      /* not used */
#define SP_VMS          62      /* not used */
#define SP_ALSYS        100     /* not used */
#define SP_KPAR         101     /* not used */

/* All numbers up to 127 are reserved. */

/* Our error codes.                                                     */
#define SP_TOOLARGE     167     /* packet too large */
#define SP_TOOSMALL     168     /* reply packet too small */
#define SP_NOHOST       169     /* can't reach host */

/* Our services.                                                        */
#define SP_ARGC         167     /* get argc */
#define SP_ARGVx        168     /* get argv[x] */
#define SP_ABORT        169     /* abort */
#define SP_DBG_INIT     170     /* debugger init */
#define SP_DBG_BPT      171     /* debugger bpt hit */
#define SP_TIMES        172     /* report total user and system time    */
#define SP_OPEN2        173
#define SP_ACCESS       174
#define SP_FCNTL        175
#define SP_STAT         176
#define SP_GETPID       177
#define SP_GETUID       178
#define SP_SETUID       179
#define SP_KILL         180
#define SP_LINK         181
#define SP_UNLINK       182
#define SP_UNAME        183
#define SP_PUTENV       184


/* Various types, modes, etc.                                           */
#define SP_BIN          1               /* SP_OPEN type */
#define SP_TEXT         2
#define SP_FIRSTMODE    1               /* SP_OPEN mode */
#define SP_LASTMODE     6

#define SP_SEEK_SET     1
#define SP_SEEK_CUR     2               /* SP_SEEK whence */
#define SP_SEEK_END     3

#define SP_O_RDONLY     0x0001
#define SP_O_WRONLY     0x0002
#define SP_O_RDWR       0x0004
#define SP_O_NDELAY     0x0008
#define SP_O_APPEND     0x0010          /* SP_OPEN2 oflags */
#define SP_O_SYNC       0x0020
#define SP_O_CREAT      0x0040
#define SP_O_TRUNC      0x0080
#define SP_O_EXCL       0x0100
#define SP_O_BINARY     0x0200
#define SP_O_TEXT       0x0400

#define SP_PID          1
#define SP_PGRP         2               /* SP_GETPID type */
#define SP_PPID         3

#define SP_UID          1
#define SP_EUID         2               /* SP_GETUID and SP_SETUID type */
#define SP_GID          3
#define SP_EGID         4

#define SP_F_OK         0
#define SP_X_OK         1               /* SP_ACCESS amode */
#define SP_W_OK         2
#define SP_R_OK         4

#define SP_S_IFMT       0170000
#define SP_S_IFIFO      0040000
#define SP_S_IFCHR      0020000
#define SP_S_IFDIR      0060000
#define SP_S_IFBLK      0100000
#define SP_S_IFREG      0010000
#define SP_S_ISUID      04000
#define SP_S_ISGID      02000
#define SP_S_ISVTX      01000           /* SP_STAT mode */
#define SP_S_ENFMT      SP_S_ISGID
#define SP_S_IRUSR      00400
#define SP_S_IWUSR      00200
#define SP_S_IXUSR      00100
#define SP_S_IRGRP      00040
#define SP_S_IWGRP      00020
#define SP_S_IXGRP      00010
#define SP_S_IROTH      00004
#define SP_S_IWOTH      00002
#define SP_S_IXOTH      00001

#define SP_F_DUPFD      1
#define SP_F_GETFD      2
#define SP_F_SETFD      3
#define SP_F_GETFL      4               /* SP_FCNTL command */
#define SP_F_SETFL      5
#define SP_F_GETLK      6
#define SP_F_SETLK      7
#define SP_F_SETLKW     8

#define SP_F_RDLCK      1
#define SP_F_WRLCK      2               /* SP_FCNTL lock type */
#define SP_F_UNLCK      3


/* Macros to extract bytes, halfwords and words from message buffer.    */
#define BYTE(v)         (* (unsigned char *) (v))
#define INT16(v)        v
#define INT32(v)        v

/* Macros to store bytes, halfwords and words in message buffer.        */
#define SETBYTE(v,b)    (BYTE(v) = (b))
#define SETINT16(v,l)   (BYTE(v) = (l))
#define SETINT32(v,l)   (BYTE(v) = (l))


/* Vector is halfword length followed by data.                          */
#define VLEN(v)         INT16(v)
#define SETVLEN(v,l)    SETINT16((v),(l))
#define VLENSZ          1
#define VCODESZ      1
#define WORDSZ       4
#define INTSZ           1
#define VDAT(v)         ((v) + VLENSZ)


#endif  /* _hxproto_included */

