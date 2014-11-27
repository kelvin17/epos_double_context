/*-------------------------------------------------------------------------+
| bsp.h v1.0 for BWDSP104x*/

#ifndef _BSP_H
#define _BSP_H

#ifdef __cplusplus
extern "C" {
#endif

#include <bsp/bspopts.h>

#include <rtems.h>
#include <rtems/iosupp.h>
#include <bsp/drivers/clockdrv.h>
#include <rtems/bspIo.h>

/*
 * Command line.
 */
const char* bsp_cmdline(void);
const char* bsp_cmdline_arg(const char* arg);


#ifdef __cplusplus
}
#endif

#endif /* _BSP_H */
