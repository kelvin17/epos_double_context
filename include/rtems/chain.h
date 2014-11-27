/**
 * @file rtems/chain.h
 *
 *  This include file contains all the constants and structures associated
 *  with the Chain API in RTEMS. The chain is a double linked list that
 *  is part of the Super Core. This is the published interface to that
 *  code.
 *
 */

/*
 *  COPYRIGHT (c) 1989-2008.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: chain.h,v 1.3 2009/11/30 13:20:29 thomas Exp $
 */

#ifndef _RTEMS_CHAIN_H
#define _RTEMS_CHAIN_H

#ifdef __cplusplus
extern "C" {
#endif

#include <rtems/system.h>
#include <rtems/score/chain.h>

/**
 * @typedef epos_chain_node
 *
 * A node that can be manipulated in the chain.
 */
typedef Chain_Node epos_chain_node;

/**
 * @typedef epos_chain_control
 *
 * The chain's control anchors the chain.
 */
typedef Chain_Control epos_chain_control;

/**
 *  @brief Chain initializer for an empty chain with designator @a name.
 */
#define RTEMS_CHAIN_INITIALIZER_EMPTY(name) \
  CHAIN_INITIALIZER_EMPTY(name)

/**
 *  @brief Chain definition for an empty chain with designator @a name.
 */
#define RTEMS_CHAIN_DEFINE_EMPTY(name) \
  CHAIN_DEFINE_EMPTY(name)

#ifndef _OCC_NO_INLINE_
#include <rtems/chain.inl>
#endif

#ifdef _OCC_NO_INLINE_
RTEMS_INLINE_ROUTINE void epos_chain_initialize(
  epos_chain_control *the_chain,
  void                *starting_address,
  size_t               number_nodes,
  size_t               node_size
);

RTEMS_INLINE_ROUTINE void epos_chain_initialize_empty(
  epos_chain_control *the_chain
);

RTEMS_INLINE_ROUTINE void epos_chain_set_off_chain(
  epos_chain_node *node
);

RTEMS_INLINE_ROUTINE bool epos_chain_is_node_off_chain(
  const epos_chain_node *node
);
RTEMS_INLINE_ROUTINE bool epos_chain_is_null_node(
  const epos_chain_node *the_node
);



RTEMS_INLINE_ROUTINE epos_chain_node *epos_chain_head(
  epos_chain_control *the_chain
);
RTEMS_INLINE_ROUTINE epos_chain_node *epos_chain_tail(
  epos_chain_control *the_chain
);
RTEMS_INLINE_ROUTINE epos_chain_node *epos_chain_first(
  epos_chain_control *the_chain
);
RTEMS_INLINE_ROUTINE epos_chain_node *epos_chain_last(
  epos_chain_control *the_chain
);
RTEMS_INLINE_ROUTINE epos_chain_node *epos_chain_next(
  epos_chain_node *the_node
);
RTEMS_INLINE_ROUTINE epos_chain_node *epos_chain_previous(
  epos_chain_node *the_node
);

RTEMS_INLINE_ROUTINE bool epos_chain_are_nodes_equal(
  const epos_chain_node *left,
  const epos_chain_node *right
);

RTEMS_INLINE_ROUTINE bool epos_chain_is_empty(
  epos_chain_control *the_chain
);

RTEMS_INLINE_ROUTINE bool epos_chain_is_first(
  const epos_chain_node *the_node
);

RTEMS_INLINE_ROUTINE bool epos_chain_is_last(
  const epos_chain_node *the_node
);

RTEMS_INLINE_ROUTINE bool epos_chain_has_only_one_node(
  const epos_chain_control *the_chain
);

RTEMS_INLINE_ROUTINE bool epos_chain_is_head(
  epos_chain_control    *the_chain,
  const epos_chain_node *the_node
);

RTEMS_INLINE_ROUTINE bool epos_chain_is_tail(
  epos_chain_control    *the_chain,
  const epos_chain_node *the_node
);

RTEMS_INLINE_ROUTINE void epos_chain_extract(
  epos_chain_node *the_node
);

RTEMS_INLINE_ROUTINE void epos_chain_extract_unprotected(
  epos_chain_node *the_node
);

RTEMS_INLINE_ROUTINE epos_chain_node *epos_chain_get(
  epos_chain_control *the_chain
);

RTEMS_INLINE_ROUTINE epos_chain_node *epos_chain_get_unprotected(
  epos_chain_control *the_chain
);

RTEMS_INLINE_ROUTINE void epos_chain_insert(
  epos_chain_node *after_node,
  epos_chain_node *the_node
);

RTEMS_INLINE_ROUTINE void epos_chain_insert_unprotected(
  epos_chain_node *after_node,
  epos_chain_node *the_node
);

RTEMS_INLINE_ROUTINE void epos_chain_append(
  epos_chain_control *the_chain,
  epos_chain_node    *the_node
);

RTEMS_INLINE_ROUTINE void epos_chain_append_unprotected(
  epos_chain_control *the_chain,
  epos_chain_node    *the_node
);

RTEMS_INLINE_ROUTINE void epos_chain_prepend(
  epos_chain_control *the_chain,
  epos_chain_node    *the_node
);

RTEMS_INLINE_ROUTINE void epos_chain_prepend_unprotected(
  epos_chain_control *the_chain,
  epos_chain_node    *the_node
);

#endif

#ifdef __cplusplus
}
#endif

#endif
/* end of include file */
