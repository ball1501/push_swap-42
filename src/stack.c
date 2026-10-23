/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stack.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wngamkri <wngamkri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/23 16:19:41 by wngamkri          #+#    #+#             */
/*   Updated: 2026/10/23 18:22:35 by wngamkri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

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
