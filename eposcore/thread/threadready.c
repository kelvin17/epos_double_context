/**
 * @file
 *
 * @ingroup ScoreThread
 *
 * @brief 核心抽象层线程管理器实现: _Thread_Ready。(SuperCore Thread Handler implementation.)
 */

/*
 *  Thread Handler / Thread Ready
 *
 *  COPYRIGHT (c) 1989-2011.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: threadready.c,v 1.7.2.1 2011/05/25 14:17:52 ralf Exp $
 */

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <rtems/system.h>
#include <rtems/score/apiext.h>
#include <rtems/score/context.h>
#include <rtems/score/interr.h>
#include <rtems/score/isr.h>
#include <rtems/score/object.h>
#include <rtems/score/priority.h>
#include <rtems/score/states.h>
#include <rtems/score/sysstate.h>
#include <rtems/score/thread.h>
#include <rtems/score/threadq.h>
#include <rtems/score/userext.h>
#include <rtems/score/wkspace.h>

/*PAGE
 *
 *  _Thread_Ready
 *
 *  This kernel routine readies the requested thread, the thread chain
 *  is adjusted.  A new heir thread may be selected.
 *
 *  Input parameters:
 *    the_thread - pointer to thread control block
 *
 *  Output parameters:  NONE
 *
 *  NOTE:  This routine uses the "blocking" heir selection mechanism.
 *         This ensures the correct heir after a thread restart.
 *
 *  INTERRUPT LATENCY:
 *    ready chain
 *    select heir
 */

void _Thread_Ready(
  Thread_Control *the_thread
)
{
  ISR_Level              level;
  Thread_Control *heir;

  _ISR_Disable( level );

  the_thread->current_state = STATES_READY;

  _Priority_Add_to_bit_map( &the_thread->Priority_map );

  _Chain_Append_unprotected( the_thread->ready, &the_thread->Object.Node );

  _ISR_Flash( level );

 _Thread_Calculate_heir();
/*  Priority_Bit_map_control minor;
  Priority_Bit_map_control major;

  _Bitfield_Find_first_bit( _Priority_Major_bit_map, major );
  _Bitfield_Find_first_bit( _Priority_Bit_map[major], minor );
  Priority_Control My_Pri=(major<< 4) +minor ;
//  My_Pri=My_Pri+0x100;
  Chain_Control *xx;
  *xx=_Thread_Ready_chain[ My_Pri ];

  _Thread_Heir=(Thread_Control *)(xx->first);
 
//  _Thread_Heir = (Thread_Control *)_Thread_Ready_chain[ My_Pri ].first;
*/

  heir = _Thread_Heir;

  if ( !_Thread_Is_executing( heir ) && _Thread_Executing->is_preemptible )
    _Context_Switch_necessary = true;

  _ISR_Enable( level );
}

