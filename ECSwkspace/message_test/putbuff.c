/*
 * putbuff.c
 *
 *  Created on: 2012-11-21
 *      Author: zwj
 */

#include "sp13.h"

void Put_buffer(void *buffer) {
	int i, j;
	char c;
long *b = buffer;

	printk("\nhello i am received message :%d %d %d %d\n",*b,*(b+1),*(b+2),*(b+3));
	//b[0]=66;
	for (i = 0; i < (int) (MESSAGE_SIZE / sizeof(long)); i++) {
		for (j = sizeof(long) - 1; j >= 0; j--) {
			if ((c = (char) (b[i] >> (8 * j)) & 0xFF) == 0)
				return;
			printk("%d %c\n", i, c);
		}
	}
}
