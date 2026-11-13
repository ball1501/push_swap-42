/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_simple.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wngamkri <wngamkri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/30 15:39:12 by wngamkri          #+#    #+#             */
/*   Updated: 2026/11/10 16:40:01 by wngamkri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

void	sort_simple(t_ctx *ctx)
{
	int	target;
	int	n;

	n = ctx->a.size;
	target = 0;
	while (target < n)
	{
		bring_to_top_a(ctx, stack_find_pos(&ctx->a, target));
		do_op(ctx, PB);
		target++;
	}
	while (ctx->b.size > 0)
		do_op(ctx, PA);
}
