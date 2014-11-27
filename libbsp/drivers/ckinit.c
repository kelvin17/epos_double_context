/*  ckinit.c
 *
 *  This file provides a template for the clock device driver initialization.
 *
 *  COPYRIGHT (c) 1989-1999.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: ckinit.c,v 1.17 2008/09/16 19:09:53 joel Exp $
 */

#include <stdlib.h>

#include <rtems.h>
#include <bsp.h>

#include <rtems/rtems/clock.h>
#include <stdio.h>

void Clock_exit( void );
epos_isr Clock_isr( epos_vector_number vector );

/*
 *  The interrupt vector number associated with the clock tick device
 *  driver.
 */

#define CLOCK_VECTOR    4

/*
 *  Clock_driver_ticks is a monotonically increasing counter of the
 *  number of clock ticks since the driver was initialized.
 */

volatile uint32_t         Clock_driver_ticks;

/*
 *  Clock_isrs is the number of clock ISRs until the next invocation of
 *  the RTEMS clock tick routine.  The clock tick device driver
 *  gets an interrupt once a millisecond and counts down until the
 *  length of time between the user configured microseconds per tick
 *  has passed.
 */

uint32_t         Clock_isrs;              /* ISRs until next tick */

/*
 * These are set by clock driver during its init
 */

epos_device_major_number epos_clock_major = ~0;
epos_device_minor_number epos_clock_minor;
#define Clock_driver_nanoseconds_since_last_tick epos_bsp_clock_nanoseconds_since_last_tick
/*
 *  The previous ISR on this clock tick interrupt vector.
 */

epos_isr_entry  Old_ticker;

void Clock_exit( void );
static uint32_t epos_bsp_clock_nanoseconds_since_last_tick()
{
 static uint32_t i=1000000;
// i+= 500000;
 //printf("i=%d\n",i);
 
	return i;
}
/*
 *  Isr Handler
 */

epos_isr Clock_isr(
  epos_vector_number vector
)
{
int level=0;


  Clock_driver_ticks += 1;
 // if ( Clock_isrs == 1 ) {
    epos_clock_tick();
 //   Clock_isrs = epos_configuration_get_microseconds_per_tick() / 1000;
 // }
//  else  
 //   Clock_isrs -= 1;	

} 
epos_isr_entry set_vector( 					/* returns old vector */
	  epos_isr_entry	  handler,					/* isr routine		  */
	  epos_vector_number vector,					/* vector number	  */
	  int				  type						/* RTEMS or RAW intr  */
	);

void init_timer0_int(int);
void install_irqhandler(void);

void Install_clock(
  epos_isr_entry clock_isr
) 
{ 
  /*
   *  Initialize the clock tick device driver variables
   */
  Clock_driver_ticks = 0;
  Clock_isrs = epos_configuration_get_microseconds_per_tick() / 1000;
 // printf("clock_isrs is %d\n",Clock_isrs);
 // install_irqhandler();
 isr_installc(SIGITIH0, Clock_isr);
  init_timer0_int(SIGITIH0);

 // Old_ticker = (epos_isr_entry) set_vector( clock_isr, CLOCK_VECTOR, 1 );
  /*
   *  Hardware specific initialize goes here
   */

  /* XXX */

  /*
   *  Schedule the clock cleanup routine to execute if the application exits.
   */
epos_clock_set_nanoseconds_extension(Clock_driver_nanoseconds_since_last_tick);
 // atexit( Clock_exit );
}

/*
 *  Clean up before the application exits
 */

void Clock_exit( void )
{
  /* XXX: turn off the timer interrupts */

  /* XXX: If necessary, restore the old vector */
}

/*
 *  Clock_initialize
 *
 *  Device driver entry point for clock tick driver initialization.
 */

epos_device_driver Clock_initialize(
  epos_device_major_number major,
  epos_device_minor_number minor,
  void *pargp
)
{
  Install_clock( Clock_isr );

  /*
   * make major/minor avail to others such as shared memory driver
   */

  epos_clock_major = major;
  epos_clock_minor = minor;

  return RTEMS_SUCCESSFUL;
}

