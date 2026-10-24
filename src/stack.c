/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stack.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wngamkri <wngamkri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/23 16:19:41 by wngamkri          #+#    #+#             */
/*   Updated: 2026/10/24 15:37:16 by wngamkri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*   TEMP COMMENTS FOR TEAMMATE - remove before submission (Norm forbids      */
/*   comments inside function bodies).                                        */
/*                                                                            */
/*   stack.c : basic operations on a stack stored as a singly linked list.   */
/*                                                                            */
/*   s (t_stack) is the "label" of a stack, it holds 2 things:               */
/*     - top  : pointer to the first node (NULL if the stack is empty)       */
/*     - size : how many nodes there are                                      */
/*   Each node (t_node) is one number and holds 3 things:                    */
/*     - value : the real integer from the user                               */
/*     - index : rank of the value (0 = smallest). Set later, starts at 0    */
/*     - next  : pointer to the next node (NULL for the last node)           */
/*                                                                            */
/*       s                                                                    */
/*   +----------+     +--------+     +--------+     +--------+              */
/*   | top  ----+---> | 32 | --+---> | -8 | --+---> |  5 | NULL              */
/*   | size : 3 |     +--------+     +--------+     +--------+              */
/*   +----------+                                                            */
/*                                                                            */
/*   Almost every function here uses the same pattern: start at s->top and   */
/*   follow ->next until we reach NULL.                                      */
/* ************************************************************************** */

#include "push_swap.h"

/*
** node_new
** Creates ONE node on the heap.
** value : the integer to store
** return: pointer to the new node, or NULL if malloc fails
** Note  : index starts at 0 (not known yet), next starts at NULL
**         (nobody is linked after this node yet).
*/
t_node	*node_new(int value)
{
	t_node	*node;

	node = malloc(sizeof(t_node));
	if (!node)
		return (NULL);
	node->value = value;
	node->index = 0;
	node->next = NULL;
	return (node);
}

/*
** stack_push_back
** Creates a node and appends it at the BOTTOM of the stack.
** Used by parsing: the first argument must be the top, so we add the
** arguments one by one at the end.
** return: 0 on success, -1 if malloc fails
**
**   empty stack : top becomes the new node
**   otherwise   : walk to the last node, then last->next = new node
*/
int	stack_push_back(t_stack *s, int value)
{
	t_node	*node;
	t_node	*last;

	node = node_new(value);
	if (!node)
		return (-1);
	if (!s->top)
		s->top = node;
	else
	{
		last = s->top;
		while (last->next != NULL)
			last = last->next;
		last->next = node;
	}
	s->size++;
	return (0);
}

/*
** stack_free
** Frees every node and leaves the stack empty (safe to reuse or re-free).
** Order matters: save cur->next BEFORE free(cur), because we can't read
** a node after it has been freed.
**
**   next = cur->next  ->  free(cur)  ->  cur = next
*/
void	stack_free(t_stack *s)
{
	t_node	*cur;
	t_node	*next;

	cur = s->top;
	while (cur)
	{
		next = cur->next;
		free(cur);
		cur = next;
	}
	s->top = NULL;
	s->size = 0;
}

/*
** stack_is_sorted
** Checks that values go from small (top) to big (bottom).
** return: 1 if sorted, 0 otherwise
** Compares each node with the one after it; one bad pair is enough to
** answer 0. An empty stack or a stack with 1 node counts as sorted (the
** loop never runs).
** The while condition checks cur->next BEFORE we read cur->next->value,
** otherwise we would read through a NULL pointer and crash.
*/
int	stack_is_sorted(t_stack *s)
{
	t_node	*cur;

	cur = s->top;
	while (cur && cur->next)
	{
		if (cur->value > cur->next->value)
			return (0);
		cur = cur->next;
	}
	return (1);
}

/*
** stack_find_pos
** Finds where the node with the given INDEX (not value) is in the stack.
** return: position counted from the top (top = 0), or -1 if not found
** Used by the sorting algorithms to decide how many times to rotate
** (ra/rra, rb/rrb) to bring a node to the top.
**
**   top -> [idx 4] -> [idx 0] -> [idx 2]
**   pos       0          1          2     find_pos(s, 2) == 2
*/
int	stack_find_pos(t_stack *s, int index)
{
	t_node	*cur;
	int		pos;

	cur = s->top;
	pos = 0;
	while (cur)
	{
		if (cur->index == index)
			return (pos);
		cur = cur->next;
		pos++;
	}
	return (-1);
}
