/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   disorder.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wngamkri <wngamkri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/30 09:33:01 by wngamkri          #+#    #+#             */
/*   Updated: 2026/10/30 09:45:51 by wngamkri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

void	compute_disorder(t_ctx *ctx)
{
	t_node	*cur;
	t_node	*other;

	ctx->mistakes = 0;
	ctx->pairs = 0;
	cur = ctx->a.top;
	while (cur)
	{
		other = cur->next;
		while (other)
		{
			ctx->pairs++;
			if (cur->value > other->value)
				ctx->mistakes++;
			other = other->next;
		}
		cur = cur->next;
	}
}

// just check pairs and mistakes
