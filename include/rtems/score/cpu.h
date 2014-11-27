/**
 * @file cpu.h
 *
 * @ingroup ScoreBWDSP
 *
 *  @brief Header file of the BWDSP processor.
 */

/*
 *  This include file contains information pertaining to the Intel
 *  i386 processor.
 *
 *  COPYRIGHT (c) 1989-2008.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: cpu.h,v 1.35 2009/12/04 04:26:32 ralf Exp $
 */

#ifndef _RTEMS_SCORE_CPU_H
#define _RTEMS_SCORE_CPU_H

/**
 *  @defgroup ScoreIntelCPU	HAL of Intel CPU/I386 processor
 *
 *  @ingroup Score
 *
 *  @brief	核心抽象层之IntelCPU抽象管理模块
 *
 *  This include file contains information pertaining to BWDSP.
 */
/**@{*/

#ifndef ASM
#include <string.h> /* for memcpy */
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifndef ASM
#include <rtems/score/types.h>
#include <rtems/score/interrupts.h>
#include <rtems/score/registers.h>
#endif

#define CPU_NAME "BWDSP"
#define CPU_MODEL_NAME "104x"

/* conditional compilation parameters */

#define CPU_INLINE_ENABLE_DISPATCH       TRUE
#define CPU_UNROLL_ENQUEUE_PRIORITY      FALSE

#define CPU_HAS_SOFTWARE_INTERRUPT_STACK FALSE//Jo
#define CPU_HAS_HARDWARE_INTERRUPT_STACK FALSE
#define CPU_ALLOCATE_INTERRUPT_STACK     FALSE//Jo

/*
 *  Does the RTEMS invoke the user's ISR with the vector number and
 *  a pointer to the saved interrupt frame (1) or just the vector
 *  number (0)?
 */

#define CPU_ISR_PASSES_FRAME_POINTER 0

#define CPU_HARDWARE_FP     FALSE

#define CPU_SOFTWARE_FP     FALSE

#define CPU_ALL_TASKS_ARE_FP             FALSE
#define CPU_IDLE_TASK_IS_FP              FALSE
#define CPU_USE_DEFERRED_FP_SWITCH       TRUE

#define CPU_STACK_GROWS_UP               FALSE
#define CPU_STRUCTURE_ALIGNMENT

/*
 *  Does this port provide a CPU dependent IDLE task implementation?
 *
 *  If TRUE, then the routine _CPU_Thread_Idle_body
 *  must be provided and is the default IDLE thread body instead of
 *  _CPU_Thread_Idle_body.
 *
 *  If FALSE, then use the generic IDLE thread body if the BSP does
 *  not provide one.
 */

#define CPU_PROVIDES_IDLE_THREAD_BODY    TRUE

#define CPU_INTERRUPT_NUMBER_OF_VECTORS      32
#define CPU_INTERRUPT_MAXIMUM_VECTOR_NUMBER  (CPU_INTERRUPT_NUMBER_OF_VECTORS - 1)

typedef void CPU_Interrupt_frame;

/*
 *  Define what is required to specify how the network to host conversion
 *  routines are handled.
 */

#define CPU_BIG_ENDIAN                           FALSE
#define CPU_LITTLE_ENDIAN                        TRUE

/* structures */

#ifndef ASM

/*
 *  Basic integer context for the i386 family.
 */
typedef uint32_t REG_TYPE;

#define N_REGS 0x51D

typedef struct _Context_Control
{
	REG_TYPE current_stack_base;
	REG_TYPE _ira[44];
	REG_TYPE _ra;
	REG_TYPE _ba;
	REG_TYPE _u9;
	REG_TYPE _sr;
	REG_TYPE _pad1[0x4E0];
	REG_TYPE return_address;
	REG_TYPE _pad2[0xB];
}Context_Control;



// removed by 098
// typedef uint32_t Context_Control;

#define _CPU_Context_Get_SP( _context ) \
  (_context)->esp

typedef struct {
  unsigned int     reserved[1];
} Context_Control_fp;

#endif /* ASM */

#define CPU_ALIGNMENT                    2              
#define CPU_HEAP_ALIGNMENT               CPU_ALIGNMENT
#define CPU_PARTITION_ALIGNMENT          CPU_ALIGNMENT

#define CPU_STACK_ALIGNMENT             2

/* macros */

#ifndef ASM
/*
 *  ISR handler macros
 *
 *  These macros perform the following functions:
 *     + initialize the RTEMS vector table
 *     + disable all maskable CPU interrupts
 *     + restore previous interrupt level (enable)
 *     + temporarily restore interrupts (flash)
 *     + set a particular level
 */

#define _CPU_Initialize_vectors()

#define _CPU_ISR_Disable( _level ) bwdsp_disable_interrupt( _level )

#define _CPU_ISR_Enable( _level )  bwdsp_enable_interrupt( _level )

#define _CPU_ISR_Flash( _level )   //i386_flash_interrupts( _level )

void bwdsp_set_interrupt(void);
void bwdsp_clr_interrupt(void);

#define _CPU_ISR_Set_level( _new_level )\
{ \
    if ( _new_level ) bwdsp_clr_interrupt(); \
    else          bwdsp_set_interrupt(); \
  }

uint32_t   _CPU_ISR_Get_level( void );

/*  Make sure interrupt stack has space for ISR
 *  'vector' arg at the top and that it is aligned
 *  properly.
 */

#define _CPU_Interrupt_stack_setup( _lo, _hi ) 
#endif /* ASM */

/* end of ISR handler macros */

/*
 *  Context handler macros
 *
 *  These macros perform the following functions:
 *     + initialize a context area
 *     + restart the current thread
 *     + calculate the initial pointer into a FP context area
 *     + initialize an FP context area
 */

#define CPU_EFLAGS_INTERRUPTS_ON  0x00003202
#define CPU_EFLAGS_INTERRUPTS_OFF 0x00003002

#ifndef ASM


//stack_base points to the a place available

//#define _CPU_Context_Initialize( _the_context, _stack_base, _size,  _isr, _entry_point, _is_fp ) 


#define _CPU_Context_Restart_self( _the_context ) \
   _CPU_Context_restore_self( (_the_context) )

#define _CPU_Context_Fp_start( _base, _offset )// \
  // ( (void *) _Addresses_Add_offset( (_base), (_offset) ) )

#define _CPU_Context_Initialize_fp( _fp_area )// \
//  { \
//    memcpy( *_fp_area, &_CPU_Null_fp_context, CPU_CONTEXT_FP_SIZE ); \
//  }

/* end of Context handler macros */

/*
 *  Fatal Error manager macros
 *
 *  These macros perform the following functions:
 *    + disable interrupts and halt the CPU
 */

#define _CPU_Fatal_halt( _error )

#endif /* ASM */

/* end of Fatal Error manager macros */


#define CPU_USE_GENERIC_BITFIELD_CODE TRUE
#define CPU_USE_GENERIC_BITFIELD_DATA TRUE


#define _CPU_Bitfield_Find_first_bit( _value, _output )\
  { \
    register uint32_t   __value = (uint32_t) (_value); \
    register const unsigned char *__p = __log2table; \
    \
    if ( __value < 0x100 ) \
      (_bit_number) = (Priority_Bit_map_control)( __p[ __value ] + 8 );  \
    else \
      (_bit_number) = (Priority_Bit_map_control)( __p[ __value >> 8 ] ); \
  }

/* end of Bitfield handler macros */

/*
 *  Priority handler macros
 *
 *  These macros perform the following functions:
 *    + return a mask with the bit for this major/minor portion of
 *      of thread priority set.
 *    + translate the bit number returned by "Bitfield_find_first_bit"
 *      into an index into the thread ready chain bit maps
 */

#define _CPU_Priority_Mask( _bit_number ) \
  ( 1 << (_bit_number) )

#define _CPU_Priority_bits_index( _priority ) \
  (_priority)

/* functions */

#ifndef ASM
/*
 *  _CPU_Initialize
 *
 *  This routine performs CPU dependent initialization.
 */

void _CPU_Initialize(void);

/*
 *  _CPU_ISR_install_raw_handler
 *
 *  This routine installs a "raw" interrupt handler directly into the
 *  processor's vector table.
 */

void _CPU_ISR_install_raw_handler(
  uint32_t    vector,
  proc_ptr    new_handler,
  proc_ptr   *old_handler
);

/*
 *  _CPU_ISR_install_vector
 *
 *  This routine installs an interrupt vector.
 */

void _CPU_ISR_install_vector(
  uint32_t    vector,
  proc_ptr    new_handler,
  proc_ptr   *old_handler
);

/*
 *  _CPU_Thread_Idle_body
 *
 */

#if (CPU_PROVIDES_IDLE_THREAD_BODY == TRUE)

void *_CPU_Thread_Idle_body( uintptr_t ignored );

#endif /* CPU_PROVIDES_IDLE_THREAD_BODY */

/*
 *  _CPU_Context_switch
 *
 *  This routine switches from the run context to the heir context.
 */

void _CPU_Context_switch(
  Context_Control  **run,	// running task context saved here
  Context_Control  *heir
);

/*
 *  _CPU_Context_restore
 *
 *  This routine is generally used only to restart self in an
 *  efficient manner and avoid stack conflicts.
 */

void _CPU_Context_restore_self(
  Context_Control *new_context
) RTEMS_COMPILER_NO_RETURN_ATTRIBUTE;

/*
 *  _CPU_Context_save_fp
 *
 *  This routine saves the floating point context passed to it.
 */
void _CPU_Context_save_fp(
  Context_Control_fp **fp_context_ptr
);

/*
 *  _CPU_Context_restore_fp
 *
 *  This routine restores the floating point context passed to it.
 */
void _CPU_Context_restore_fp(
  Context_Control_fp **fp_context_ptr
);

#endif /* ASM */

#ifdef __cplusplus
}
#endif

/**@}*/
#endif
