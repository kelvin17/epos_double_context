/**
 * @file interrupts.h
 *
 * @ingroup ScoreBWDSP
 *
 *  @brief Header file of the interrupt of BWDSP.
 */

/*
 * 
 *
 *  COPYRIGHT (c) 1998 valette@crf.canon.fr
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: interrupts.h,v 1.6 2005/01/28 15:56:09 ralf Exp $
 *
 *  Applications must not include this file directly.
 */
	 
#ifndef DSP_INTERRUPT_H
#define DSP_INTERRUPT_H

/**
 *  @addtogroup ScoreBWDSP
 *  @{
 */	

#define SIGSWI          61

#define SIGHINT         54
#define SIGITIH0		53
#define SIGITIH1		52
#define SIGTIH0			51
#define SIGTIH1			50
#define SIGTIH2			49
#define SIGTIH3			48
#define SIGINT0			47
#define SIGINT1			46
#define SIGINT2			45
#define SIGINT3			44
#define SIGINT4			43
#define SIGINT5			42
#define SIGINT6			41
#define SIGINT7			40

#define SIGIINT			38

#define SIGCDMA0		35
#define SIGCDMA1		34
#define SIGCDMA2		33
#define SIGCDMA3		32
#define SIGCDMA4		31
#define SIGCDMA5		30
#define SIGCDMA6		29
#define SIGCDMA7		28
#define SIGCDMA8		27
#define SIGCDMA9		26
#define SIGCDMA10		25
#define SIGIODMA0		24
#define SIGIODMA1		23
#define SIGIODMA2		22
#define SIGIODMA3		21

#define SIGSRI			17
#define SIGSTI			16

#define SIGITIL0		13
#define SIGITIL1		12
#define SIGTIL0			11
#define SIGTIL1			10
#define SIGTIL2			9
#define SIGTIL3			8

#ifndef SIG_IGN	 
#define SIG_IGN         ((void (*)(epos_vector_number))1)
#endif



typedef void (*interrupt_fun_t)(int);
extern interrupt_fun_t isr_installf(int sig, interrupt_fun_t func);
extern interrupt_fun_t isr_installc(int sig, interrupt_fun_t func);

extern void init_swi(int);

/*
extern void (*interruptf(int sig, void *func(int)))(int);
extern void (*interruptn(int sig, void *func(int)))(int);
extern void (*interruptc(int sig, void *func(int)))(int);
*/
extern int raiseint(int sig);
#define INT_DFL     ((void (*) (int)) 0)
extern interrupt_fun_t __interrrupt_table[64];
extern interrupt_fun_t __interrupts[64];
	 
/*
extern void (*__interrupt_table[64]) (int);
extern void (*__interrupts[64]) (int);
*/

//extern Context_Control* _Int_Thread_Dispatch(Context_Control* this_context);
// added in 20120802
// TODO: define struct Context_Control* ----the pointer to saved context
// one candidate is: typedef int* Context_Control_p

#endif  /* of DSP_INTERRUPT_H */

#ifndef _RTEMS_SCORE_INTERRUPTS_H
#define _RTEMS_SCORE_INTERRUPTS_H

#ifndef ASM

struct 	__epos_raw_irq_connect_data__;

typedef void (*epos_raw_irq_hdl)		(void);
typedef void (*epos_raw_irq_enable)		(const struct __epos_raw_irq_connect_data__*);
typedef void (*epos_raw_irq_disable)		(const struct __epos_raw_irq_connect_data__*);
typedef int  (*epos_raw_irq_is_enabled)	(const struct __epos_raw_irq_connect_data__*);

/*
 *  Interrupt Level Macros
 */ 

void bwdsp_disable_interrupt(int _level);
void bwdsp_enable_interrupt( int _level);

#define _CPU_ISR_Disable( _level ) bwdsp_disable_interrupt( _level )
#define _CPU_ISR_Enable( _level ) bwdsp_enable_interrupt( _level )

#endif

/**@}*/
#endif
