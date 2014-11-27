/*
 * fiilbuff.c
 *
 *  Created on: 2012-11-21
 *      Author: zwj
 */

#include "sp13.h"

void Fill_buffer(char *source, long *buffer) {
	char *p;
	int i, j;

	p = source;
	for (i = 0; i < MESSAGE_SIZE / sizeof(long); i++) {
		buffer[i] = 0;
		for (j = sizeof(long) - 1; j >= 0; j--) {
			buffer[i] |= *p++ << (8 * j);
			
		}
	}
}
