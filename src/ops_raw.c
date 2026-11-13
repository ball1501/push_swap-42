/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ops_raw.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wngamkri <wngamkri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/23 16:21:49 by wngamkri          #+#    #+#             */
/*   Updated: 2026/11/10 16:37:54 by wngamkri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

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
