/* hostbuffer driver for bwdsp104x
 * hostbuffer is a simple way to communicate with debugger.
 * for kernel usage, hostbuffer provide hostbuffer_output and hostbuffer_poll
 * for user usage, hostbuffer is a driver. 
 */

#if HAVE_CONFIG_H
#include "config.h"
#endif
#include <rtems/bspIo.h>

#include <rtems/libio_.h>
#include <rtems/seterr.h>
#include <sys/errno.h>

#include <string.h>
#include <sys/types.h>
#include <fcntl.h>

#include <bsp/drivers/hostcomm/hostcomm.h>
#include <unistd.h>

int _maxpacksz = MAXMAXPACKSZ; /* max packet size */
char _iobuffer[MAXMAXPACKSZ+1]= { 0 };
char* _global_pserver = _iobuffer;
char* _sbuffer = _iobuffer + 1;
char _obuffer[MAXOBUFFERSZ] = {0};
size_t _obufferid = 0;

/* mahongx2: only for kernel usage. ignore much of checks. ignore return value. */
int hostbuffer_output_buf (char *buf, size_t nbytes)
{
	ISR_Level       level;
    int res = SP_SUCCESS;

    /* mahongx2: nbytes must less than MAXOBUFFERSZ */
    if(nbytes > MAXOBUFFERSZ || nbytes <= 0)
    	return SP_ERROR;

    /* mahongx2: should be exclusive while writing...
     * TODO: PSERVER and VSERVER should be disable interrupts..*/
    PSERVER ();
    _ISR_Disable(level);
	/* Request PUTBLOCK.*/
	_sbuffer[0] = SP_PUTBLOCK;
	_sbuffer[1] = 1;	/* mahongx2: fd = 1 */
	_sbuffer[2] = nbytes;
	if(buf != &_sbuffer[3])
		memcpy (&_sbuffer[3], buf, nbytes);
	*_global_pserver |= 0x00000001;
	if ((*_global_pserver & 0x00000001) != 0)
		res = SP_NOHOST;
	/* mahongx2: should be exclusive while writing...
	 * TODO: PSERVER and VSERVER should be disable interrupts..*/
	VSERVER ();
	_ISR_Enable(level);

    return res;
}

void hostbuffer_output_char(const char c)
{
	_obuffer[_obufferid] = c;
	_obufferid++;
	if(_obufferid == MAXOBUFFERSZ || c == '\0')
	{
		hostbuffer_output_buf(_obuffer, _obufferid);
		_obufferid = 0;
	}
}

/*kernel usage only. actually a NULL function*/
int hostbuffer_poll_char(void)
{
	return 1;
}

BSP_polling_getchar_function_type BSP_poll_char = hostbuffer_poll_char;
BSP_output_char_function_type BSP_output_char = hostbuffer_output_char;


