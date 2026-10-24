/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ops_raw.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wngamkri <wngamkri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/23 16:21:49 by wngamkri          #+#    #+#             */
/*   Updated: 2026/10/24 19:27:15 by wngamkri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*   TEMP COMMENTS FOR TEAMMATE - remove before submission (Norm forbids      */
/*   comments inside function bodies).                                        */
/*                                                                            */
/*   ops_raw.c : the 4 basic moves on a linked-list stack.                   */
/*                                                                            */
/*   "raw" = they only change the pointers of a stack. They do NOT print     */
/*   anything and do NOT count anything. do_op (ops.c) calls them, then      */
/*   counts and prints the operation name.                                    */
/*                                                                            */
/*   Mapping to the push_swap operations:                                     */
/*     sa / sb / ss   -> raw_swap    on a / b / both                         */
/*     pa / pb        -> raw_push    b->a / a->b                             */
/*     ra / rb / rr   -> raw_rotate  on a / b / both                         */
/*     rra/rrb/rrr    -> raw_reverse on a / b / both                         */
/*                                                                            */
/*   No data is copied: we only re-wire the ->next arrows and s->top.       */
/*   The ORDER of the re-wiring matters: always save/use the old arrow       */
/*   before overwriting it, otherwise a node gets lost (leak) or the list    */
/*   loops forever.                                                           */
/* ************************************************************************** */

#include "push_swap.h"

/*
** raw_swap  (sa / sb)
** Swaps the first two nodes. Does nothing if there are fewer than 2
** (otherwise we would read ->next of a NULL pointer and crash).
**
**   before: top -> [A] -> [B] -> [C] -> NULL
**   after : top -> [B] -> [A] -> [C] -> NULL
**
** 3 arrows change, in this order:
**   1. A->next = B->next   (A now points to C; B still points to C,
**                           so we can read it before overwriting B)
**   2. B->next = A
**   3. top     = B
** Doing step 2 before step 1 would lose C.
*/
void	raw_swap(t_stack *s)
{
	t_node	*first;
	t_node	*second;

	if (s->size < 2)
		return ;
	first = s->top;
	second = s->top->next;
	first->next = second->next;
	second->next = first;
	s->top = second;
}

/*
** raw_push  (pa / pb)
** Moves the top node of "from" onto the top of "to".
** Does nothing if "from" is empty.
**
**   before: from: [A] -> [B] -> NULL      to: [X] -> NULL
**   after : from: [B] -> NULL             to: [A] -> [X] -> NULL
**
** Steps:
**   1. remember A in "node"
**   2. from->top moves to the next node (B)
**   3. A->next = to->top   (A is placed on top of X)
**   4. to->top = A
** Step 3 must come before step 4, or we lose X.
** If "to" is empty, to->top is NULL, so A->next becomes NULL: correct,
** no special case needed.
*/
void	raw_push(t_stack *from, t_stack *to)
{
	t_node	*node;

	if (!from->top)
		return ;
	node = from->top;
	from->top = node->next;
	node->next = to->top;
	to->top = node;
	from->size--;
	to->size++;
}

/*
** raw_rotate  (ra / rb)
** The top node goes to the bottom, everything else moves up by one.
** Does nothing if there are fewer than 2 nodes.
**
**   before: top -> [A] -> [B] -> [C] -> NULL
**   after : top -> [B] -> [C] -> [A] -> NULL
**
** Steps:
**   1. remember A in "first"
**   2. top moves to B
**   3. walk to the last node (C)
**   4. C->next = A       (A is appended at the bottom)
**   5. A->next = NULL    (A is the last one now; without this the list
**                         would loop A -> B -> C -> A -> ...)
** size does not change.
*/
void	raw_rotate(t_stack *s)
{
	t_node	*first;
	t_node	*last;

	if (s->size < 2)
		return ;
	first = s->top;
	s->top = first->next;
	last = s->top;
	while (last->next != NULL)
		last = last->next;
	last->next = first;
	first->next = NULL;
}

/*
** raw_reverse  (rra / rrb)
** The opposite of rotate: the bottom node goes to the top.
** Does nothing if there are fewer than 2 nodes.
**
**   before: top -> [A] -> [B] -> [C] -> NULL
**   after : top -> [C] -> [A] -> [B] -> NULL
**
** We need the node BEFORE the last one (B), because B must become the
** new last node (B->next = NULL). So the loop stops one step early:
** "while the node after next is not NULL". The guard size < 2 makes
** prev->next->next safe to read.
** Steps:
**   1. prev walks until it is on B
**   2. last = prev->next    (C)
**   3. B->next = NULL       (B is the last one now)
**   4. C->next = top        (C goes on top of A; top is still A here)
**   5. top = C
** Step 4 must come before step 5, or we lose A.
** size does not change.
*/
void	raw_reverse(t_stack *s)
{
	t_node	*prev;
	t_node	*last;

	if (s->size < 2)
		return ;
	prev = s->top;
	while (prev->next->next != NULL)
		prev = prev->next;
	last = prev->next;
	prev->next = NULL;
	last->next = s->top;
	s->top = last;
}
