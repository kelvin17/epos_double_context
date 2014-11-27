/*
 * task1.c
 *
 *  Created on: 2012-11-21
 *      Author: zwj
 */

#include "sp13.h"
#include <string.h>             /* for memcmp */

void dope_buffer(unsigned char *buff, int buff_size, unsigned char v);

unsigned char big_send_buffer[2048];
unsigned char big_receive_buffer[2048];

long buffer[MESSAGE_SIZE / sizeof(long)];

void dope_buffer(unsigned char *buff, int buff_size, unsigned char v) {
	int i;
	unsigned char ch;

	ch = (' ' + (v % (0x7f - ' ')));

	for (i = 0; i < buff_size; i++) {
		*buff++ = ch++;
		if (ch >= 0x7f)
			ch = ' ';
	}
}

epos_task Task_1(epos_task_argument argument) {
	epos_id qid;
	uint32_t index;
	uint32_t count;
	epos_status_code status;
	size_t size;
	size_t queue_size;
	unsigned char *cp;

	status = epos_message_queue_ident(Queue_name[1], RTEMS_SEARCH_ALL_NODES,
			&qid);
	printk("TA1 - epos_message_queue_ident - qid => %08" PRIxepos_id "\n",
			qid);
	// directive_failed( status, "epos_message_queue_ident" );
  
	Fill_buffer("USTC", buffer); 
	put("TA1 - epos_message_queue_send USTC TO Q 1");
	
	status = epos_message_queue_send(Queue_id[1], buffer, MESSAGE_SIZE);
	//directive_failed( status, "epos_message_queue_send" );
	
	put("TA1 - epos_task_wake_after - sleep 5 seconds");
	status = epos_task_wake_after(5 * epos_clock_get_ticks_per_second());
	
	/*
	puts_nocr("TA1 - epos_message_queue_receive - receive from queue 1 - ");
	puts("10 second timeout");
	status = epos_message_queue_receive(Queue_id[1], buffer, &size,
			RTEMS_DEFAULT_OPTIONS, 10 * epos_clock_get_ticks_per_second());
			*/
	
}
