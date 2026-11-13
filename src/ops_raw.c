/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ops_raw.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wngamkri <wngamkri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/23 16:21:49 by wngamkri          #+#    #+#             */
/*   Updated: 2026/11/13 14:07:30 by wngamkri         ###   ########.fr       */
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
