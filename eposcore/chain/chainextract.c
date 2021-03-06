/**
 * @file
 *
 * @ingroup ScoreChain
 *
 * @brief 核心抽象层双向链表chain管理器实现。(SuperCore Chain Handler implementation.)
 */

/*
 *  COPYRIGHT (c) 1989-2007.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: chainextract.c,v 1.1 2007/05/09 18:27:26 joel Exp $
 */

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <rtems/system.h>
#include <rtems/score/address.h>
#include <rtems/score/chain.h>
#include <rtems/score/isr.h>

/*
 *  _Chain_Extract
 *
 *  This kernel routine deletes the given node from a chain.
 *
 *  Input parameters:
 *    node - pointer to node in chain to be deleted
 *
 *  Output parameters:  NONE
 *
 *  INTERRUPT LATENCY:
 *    only case
 */

void _Chain_Extract(
  Chain_Node *node
)
{
  ISR_Level level;

  _ISR_Disable( level );
    _Chain_Extract_unprotected( node );
  _ISR_Enable( level );
}
