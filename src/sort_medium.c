/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_medium.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: susikong <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/08 13:48:20 by susikong          #+#    #+#             */
/*   Updated: 2026/11/08 13:48:28 by susikong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

/*
** ถ้ายังไม่ครบก้อน และเลขอยู่ในครึ่งล่างของก้อน
** ให้หมุน rb ลงไปก้นกอง b
*/
void	sort_medium(t_ctx *ctx)
{
	int	chuck;
	int	limit;

	chuck = 1;
	while (chuck * chuck < ctx->a.size)
		chuck++;
	limit = chuck;
	while (ctx->a.size > 0)
	{
		if (ctx->a.top->index < limit)
		{
			do_op(ctx, PB);
			if (ctx->b.size == limit)
				limit = limit + chuck;
			else if (ctx->b.top->index < limit - (chuck / 2))
				do_op(ctx, RB);
		}
		else
			do_op(ctx, RA);
	}
	while (ctx->b.size > 0)
	{
		bring_to_top_b(ctx, stack_find_pos(&ctx->b, ctx->b.size - 1));
		do_op(ctx, PA);
	}
}
