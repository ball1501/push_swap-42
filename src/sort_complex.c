/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_complex.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: susikong <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/05 20:01:19 by susikong          #+#    #+#             */
/*   Updated: 2026/11/05 20:01:29 by susikong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

void	sort_complex(t_ctx *ctx)
{
	int	n;
	int	i;
	int	bit;

	n = ctx->a.size;
	bit = 0;
	while (!stack_is_sorted(&ctx->a))
	{
		i = 0;
		while (i < n)
		{
			if ((ctx->a.top->index >> bit) & 1)
				do_op(ctx, RA);
			else
				do_op(ctx, PB);
			i++;
		}
		while (ctx->b.size > 0)
			do_op(ctx, PA);
		bit++;
	}
}
