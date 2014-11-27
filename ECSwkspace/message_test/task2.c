/*
 * task2.c
 *
 *  Created on: 2012-11-21
 *      Author: zwj
 */

#include "sp13.h"

epos_task Task_2(epos_task_argument argument) {
	long buffer[4];
	size_t size;
	epos_task_priority previous_priority;
	epos_status_code status;
 
	put("TA2 - epos_message_queue_receive - receive from queue 1 - RTEMS_NO_WAIT");
	status = epos_message_queue_receive(Queue_id[1], buffer, &size,
			RTEMS_DEFAULT_OPTIONS, RTEMS_NO_TIMEOUT);
	
	printk("TA2 - buffer received: ");
	Put_buffer(buffer);
//	new_line;
	put("TA2 - epos_task_wake_after - sleep 3 seconds");
	status = epos_task_wake_after(3 * epos_clock_get_ticks_per_second());
}
