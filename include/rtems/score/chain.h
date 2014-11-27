/**
 *  @file
 *
 *  @ingroup ScoreChain
 *
 *  @brief Header file of the Doubly-Linked Chain Handler.
 *
 *  This include file contains all the constants and structures associated
 *  with the Doubly-Linked Chain Handler.
 */

/*
 *  COPYRIGHT (c) 1989-2006.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: chain.h,v 1.24 2009/11/30 13:20:29 thomas Exp $
 */

#ifndef _RTEMS_SCORE_CHAIN_H
#define _RTEMS_SCORE_CHAIN_H

/**
 *  @defgroup ScoreChain Chain Handler
 *
 *  @ingroup Score
 *
 *  @brief 核心抽象层之Chain（链表）抽象管理模块
 *
 *  The Chain Handler is used to manage sets of entities.  This handler
 *  provides two data structures.  The Chain Node data structure is included
 *  as the first part of every data structure that will be placed on
 *  a chain.  The second data structure is Chain Control which is used
 *  to manage a set of Chain Nodes.
 */
/**@{*/

#ifdef __cplusplus
extern "C" {
#endif

#include <rtems/score/address.h>

/**
 *  @typedef Chain_Node
 *
 *  This type definition promotes the name for the Chain Node used by
 *  all RTEMS code.  It is a separate type definition because a forward
 *  reference is required to define it.  See @ref Chain_Node_struct for
 *  detailed information.
 */
typedef struct Chain_Node_struct Chain_Node;

/**
 *  @struct Chain_Node_struct
 *
 *  This is used to manage each element (node) which is placed
 *  on a chain.
 *
 *  @note Typically, a more complicated structure will use the
 *        chain package.  The more complicated structure will
 *        include a chain node as the first element in its
 *        control structure.  It will then call the chain package
 *        with a pointer to that node element.  The node pointer
 *        and the higher level structure start at the same address
 *        so the user can cast the pointers back and forth.
 *
 */
struct Chain_Node_struct {
  /** This points to the node after to this one on this chain. */
  Chain_Node *next;
  /** This points to the node immediate prior to this one on this chain. */
  Chain_Node *previous;
};

/**
 *  @struct Chain_Control
 *
 * This is used to manage a chain.  A chain consists of a doubly
 * linked list of zero or more nodes.
 *
 * @note This implementation does not require special checks for
 *   manipulating the first and last elements on the chain.
 *   To accomplish this the @a Chain_Control structure is
 *   treated as two overlapping @ref Chain_Node structures.
 *   The permanent head of the chain overlays a node structure on the
 *   @a first and @a permanent_null fields.  The permanent tail
 *   of the chain overlays a node structure on the
 *   @a permanent_null and @a last elements of the structure.
 *
 */
typedef struct {
  /** This points to the first node on this chain. */
  Chain_Node *first;
  /** This field is always 0. */
  Chain_Node *permanent_null;
  /** This points to the last node on this chain. */
  Chain_Node *last;
} Chain_Control;

/**
 *  @brief Chain initializer for an empty chain with designator @a name.
 */
#define CHAIN_INITIALIZER_EMPTY(name) \
  { \
    (Chain_Node *) &(name).permanent_null, \
    NULL, \
    (Chain_Node *) &(name) \
  }

/**
 *  @brief Chain definition for an empty chain with designator @a name.
 */
#define CHAIN_DEFINE_EMPTY(name) \
  Chain_Control name = CHAIN_INITIALIZER_EMPTY(name)

/**
 *  @brief Initialize a Chain Header
 *
 *  This routine initializes @a the_chain structure to manage the
 *  contiguous array of @a number_nodes nodes which starts at
 *  @a starting_address.  Each node is of @a node_size bytes.
 *
 *  @param[in] the_chain specifies the chain to initialize
 *  @param[in] starting_address is the starting address of the array
 *         of elements
 *  @param[in] number_nodes is the numebr of nodes that will be in the chain
 *  @param[in] node_size is the size of each node
 */
void _Chain_Initialize(
  Chain_Control *the_chain,
  void          *starting_address,
  size_t         number_nodes,
  size_t         node_size
);

/**
 *  @brief Extract the specified node from a chain
 *
 *  This routine extracts @a the_node from the chain on which it resides.
 *  It disables interrupts to ensure the atomicity of the
 *  extract operation.
 *
 *  @arg the_node specifies the node to extract
 */
void _Chain_Extract(
  Chain_Node *the_node
);

/**
 *  @brief Obtain the first node on a chain
 *
 *  This function removes the first node from @a the_chain and returns
 *  a pointer to that node.  If @a the_chain is empty, then NULL is returned.
 *
 *  @return This method returns a pointer a node.  If a node was removed,
 *          then a pointer to that node is returned.  If @a the_chain was
 *          empty, then NULL is returned.
 *
 *  @note It disables interrupts to ensure the atomicity of the get operation.
 */
Chain_Node *_Chain_Get(
  Chain_Control *the_chain
);

/**
 *  @brief Insert a node on a chain
 *
 *  This routine inserts @a the_node on a chain immediately following
 *  @a after_node.
 *
 *  @note It disables interrupts to ensure the atomicity
 *  of the extract operation.
 */
void _Chain_Insert(
  Chain_Node *after_node,
  Chain_Node *the_node
);

/**
 *  @brief Append a node on the end of a chain
 *
 *  This routine appends @a the_node onto the end of @a the_chain.
 *
 *  @note It disables interrupts to ensure the atomicity of the
 *  append operation.
 */
void _Chain_Append(
  Chain_Control *the_chain,
  Chain_Node    *the_node
);

#ifndef __RTEMS_APPLICATION__
#ifndef _OCC_NO_INLINE_
#include <rtems/score/chain.inl>
#endif
#endif

#ifdef _OCC_NO_INLINE_

/** @brief Set off chain
 *
 *  This function sets the next and previous fields of the @a node to NULL
 *  indicating the @a node is not part of a chain.
 *
 *  @param[in] node the node set to off chain.
 */
RTEMS_INLINE_ROUTINE void _Chain_Set_off_chain(
  Chain_Node *node
);

/** @brief Is the Node off Chain
 *
 *  This function returns true if the @a node is not on a chain. A @a node is
 *  off chain if the next and previous fields are set to NULL.
 *
 *  @param[in] node is the node off chain.
 *
 *  @return This function returns true if the @a node is off chain.
 */
RTEMS_INLINE_ROUTINE bool _Chain_Is_node_off_chain(
  const Chain_Node *node
);

/** @brief Are Two Nodes Equal
 *
 *  This function returns true if @a left and @a right are equal,
 *  and false otherwise.
 *
 *  @param[in] left is the node on the left hand side of the comparison.
 *  @param[in] right is the node on the left hand side of the comparison.
 *
 *  @return This function returns true if @a left and @a right are equal,
 *          and false otherwise.
 */
RTEMS_INLINE_ROUTINE bool _Chain_Are_nodes_equal(
  const Chain_Node *left,
  const Chain_Node *right
);

/** @brief Is this Chain Control Pointer Null
 *
 *  This function returns true if the_chain is NULL and false otherwise.
 *
 *  @param[in] the_chain is the chain to be checked for empty status.
 *
 *  @return This method returns true if the_chain is NULL and false otherwise.
 */
RTEMS_INLINE_ROUTINE bool _Chain_Is_null(
  const Chain_Control *the_chain
);

/** @brief Is the Chain Node Pointer NULL
 *
 *  This function returns true if the_node is NULL and false otherwise.
 *
 *  @param[in] the_node is the node pointer to check.
 *
 *  @return This method returns true if the_node is NULL and false otherwise.
 */
RTEMS_INLINE_ROUTINE bool _Chain_Is_null_node(
  const Chain_Node *the_node
);

/** @brief Return pointer to Chain Head
 *
 *  This function returns a pointer to the head node on the chain.
 *
 *  @param[in] the_chain is the chain to be operated upon.
 *
 *  @return This method returns the permanent head node of the chain.
 */
RTEMS_INLINE_ROUTINE Chain_Node *_Chain_Head(
  Chain_Control *the_chain
);

/** @brief Return pointer to Chain Tail
 *
 *  This function returns a pointer to the last node on the chain.
 *
 *  @param[in] the_chain is the chain to be operated upon.
 *
 *  @return This method returns the permanent tail node of the chain.
 */
RTEMS_INLINE_ROUTINE Chain_Node *_Chain_Tail(
  Chain_Control *the_chain
);

/** @brief Return pointer to Chain's First node
 *
 *  This function returns a pointer to the first node on the chain after the
 *  head.
 *
 *  @param[in] the_chain is the chain to be operated upon.
 *
 *  @return This method returns the first node of the chain.
 */
RTEMS_INLINE_ROUTINE Chain_Node *_Chain_First(
  Chain_Control *the_chain
);

/** @brief Return pointer to Chain's Last node
 *
 *  This function returns a pointer to the last node on the chain just before
 *  the tail.
 *
 *  @param[in] the_chain is the chain to be operated upon.
 *
 *  @return This method returns the last node of the chain.
 */
RTEMS_INLINE_ROUTINE Chain_Node *_Chain_Last(
  Chain_Control *the_chain
);

/** @brief Return pointer the next node from this node
 *
 *  This function returns a pointer to the next node after this node.
 *
 *  @param[in] the_node is the node to be operated upon.
 *
 *  @return This method returns the next node on the chain.
 */
RTEMS_INLINE_ROUTINE Chain_Node *_Chain_Next(
  Chain_Node *the_node
);

/** @brief Return pointer the previous node from this node
 *
 *  This function returns a pointer to the previous node on this chain.
 *
 *  @param[in] the_node is the node to be operated upon.
 *
 *  @return This method returns the previous node on the chain.
 */
RTEMS_INLINE_ROUTINE Chain_Node *_Chain_Previous(
  Chain_Node *the_node
);

/** @brief Is the Chain Empty
 *
 *  This function returns true if there a no nodes on @a the_chain and
 *  false otherwise.
 *
 *  @param[in] the_chain is the chain to be operated upon.
 *
 *  @return This function returns true if there a no nodes on @a the_chain and
 *  false otherwise.
 */
RTEMS_INLINE_ROUTINE bool _Chain_Is_empty(
  Chain_Control *the_chain
);

/** @brief Is this the First Node on the Chain
 *
 *  This function returns true if the_node is the first node on a chain and
 *  false otherwise.
 *
 *  @param[in] the_node is the node the caller wants to know if it is
 *             the first node on a chain.
 *
 *  @return This function returns true if @a the_node is the first node on
 *          a chain and false otherwise.
 */
RTEMS_INLINE_ROUTINE bool _Chain_Is_first(
  const Chain_Node *the_node
);

/** @brief Is this the Last Node on the Chain
 *
 *  This function returns true if @a the_node is the last node on a chain and
 *  false otherwise.
 *
 *  @param[in] the_node is the node to check as the last node.
 *
 *  @return This function returns true if @a the_node is the last node on
 *          a chain and false otherwise.
 */
RTEMS_INLINE_ROUTINE bool _Chain_Is_last(
  const Chain_Node *the_node
);

/** @brief Does this Chain have only One Node
 *
 *  This function returns true if there is only one node on @a the_chain and
 *  false otherwise.
 *
 *  @param[in] the_chain is the chain to be operated upon.
 *
 *  @return This function returns true if there is only one node on
 *          @a the_chain and false otherwise.
 */
RTEMS_INLINE_ROUTINE bool _Chain_Has_only_one_node(
  const Chain_Control *the_chain
);

/** @brief Is this Node the Chain Head
 *
 *  This function returns true if @a the_node is the head of the_chain and
 *  false otherwise.
 *
 *  @param[in] the_chain is the chain to be operated upon.
 *  @param[in] the_node is the node to check for being the Chain Head.
 *
 *  @return This function returns true if @a the_node is the head of
 *          @a the_chain and false otherwise.
 */
RTEMS_INLINE_ROUTINE bool _Chain_Is_head(
  Chain_Control *the_chain,
  const Chain_Node    *the_node
);

/** @brief Is this Node the Chail Tail
 *
 *  This function returns true if the_node is the tail of the_chain and
 *  false otherwise.
 *
 *  @param[in] the_chain is the chain to be operated upon.
 *  @param[in] the_node is the node to check for being the Chain Tail.
 */
RTEMS_INLINE_ROUTINE bool _Chain_Is_tail(
  Chain_Control *the_chain,
  const Chain_Node    *the_node
);

/** @brief Initialize this Chain as Empty
 *
 *  This routine initializes the specified chain to contain zero nodes.
 *
 *  @param[in] the_chain is the chain to be initialized.
 */
RTEMS_INLINE_ROUTINE void _Chain_Initialize_empty(
  Chain_Control *the_chain
);

/** @brief Extract this Node (unprotected)
 *
 *  This routine extracts the_node from the chain on which it resides.
 *  It does NOT disable interrupts to ensure the atomicity of the
 *  extract operation.
 *
 *  @param[in] the_node is the node to be extracted.
 */
RTEMS_INLINE_ROUTINE void _Chain_Extract_unprotected(
  Chain_Node *the_node
);

/** @brief Get the First Node (unprotected)
 *
 *  This function removes the first node from the_chain and returns
 *  a pointer to that node.  It does NOT disable interrupts to ensure
 *  the atomicity of the get operation.
 *
 *  @param[in] the_chain is the chain to attempt to get the first node from.
 *
 *  @return This method returns the first node on the chain even if it is
 *          the Chain Tail.
 *
 *  @note This routine assumes that there is at least one node on the chain
 *        and always returns a node even if it is the Chain Tail.
 */
RTEMS_INLINE_ROUTINE Chain_Node *_Chain_Get_first_unprotected(
  Chain_Control *the_chain
);

/** @brief Get the First Node (unprotected)
 *
 *  This function removes the first node from the_chain and returns
 *  a pointer to that node.  If the_chain is empty, then NULL is returned.
 *
 *  @param[in] the_chain is the chain to attempt to get the first node from.
 *
 *  @return This method returns the first node on the chain or NULL if the
 *          chain is empty.
 *
 *  @note It does NOT disable interrupts to ensure the atomicity of the
 *        get operation.
 */
RTEMS_INLINE_ROUTINE Chain_Node *_Chain_Get_unprotected(
  Chain_Control *the_chain
);

/** @brief Insert a Node (unprotected)
 *
 *  This routine inserts the_node on a chain immediately following
 *  after_node.
 *
 *  @param[in] after_node is the node which will precede @a the_node on the
 *             chain.
 *  @param[in] the_node is the node to be inserted.
 *
 *  @note It does NOT disable interrupts to ensure the atomicity
 *        of the extract operation.
 */
RTEMS_INLINE_ROUTINE void _Chain_Insert_unprotected(
  Chain_Node *after_node,
  Chain_Node *the_node
);

/** @brief Append a Node (unprotected) 
 *
 *  This routine appends the_node onto the end of the_chain.
 *
 *  @param[in] the_chain is the chain to be operated upon.
 *  @param[in] the_node is the node to be appended.
 *
 *  @note It does NOT disable interrupts to ensure the atomicity of the
 *        append operation.
 */
RTEMS_INLINE_ROUTINE void _Chain_Append_unprotected(
  Chain_Control *the_chain,
  Chain_Node    *the_node
);

/** @brief Prepend a Node (unprotected)
 *
 *  This routine prepends the_node onto the front of the_chain.
 *
 *  @param[in] the_chain is the chain to be operated upon.
 *  @param[in] the_node is the node to be prepended.
 *
 *  @note It does NOT disable interrupts to ensure the atomicity of the
 *        prepend operation.
 */
RTEMS_INLINE_ROUTINE void _Chain_Prepend_unprotected(
  Chain_Control *the_chain,
  Chain_Node    *the_node
);

/** @brief Prepend a Node (protected)
 *
 *  This routine prepends the_node onto the front of the_chain.
 *
 *  @param[in] the_chain is the chain to be operated upon.
 *  @param[in] the_node is the node to be prepended.
 *
 *  @note It disables interrupts to ensure the atomicity of the
 *        prepend operation.
 */
RTEMS_INLINE_ROUTINE void _Chain_Prepend(
  Chain_Control *the_chain,
  Chain_Node    *the_node
);

#endif

#ifdef __cplusplus
}
#endif

/**@}*/

#endif
/* end of include file */
