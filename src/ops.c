/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ops.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wngamkri <wngamkri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/23 16:21:45 by wngamkri          #+#    #+#             */
/*   Updated: 2026/11/10 16:38:19 by wngamkri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

char	*op_name(t_op op)
{
	if (op == SA)
		return ("sa");
	if (op == SB)
		return ("sb");
	if (op == SS)
		return ("ss");
	if (op == PA)
		return ("pa");
	if (op == PB)
		return ("pb");
	if (op == RA)
		return ("ra");
	if (op == RB)
		return ("rb");
	if (op == RR)
		return ("rr");
	if (op == RRA)
		return ("rra");
	if (op == RRB)
		return ("rrb");
	if (op == RRR)
		return ("rrr");
	return ("");
}

void	do_op(t_ctx *ctx, t_op op)
{
	if (op == SA || op == SS)
		raw_swap(&ctx->a);
	if (op == SB || op == SS)
		raw_swap(&ctx->b);
	if (op == PA)
		raw_push(&ctx->b, &ctx->a);
	if (op == PB)
		raw_push(&ctx->a, &ctx->b);
	if (op == RA || op == RR)
		raw_rotate(&ctx->a);
	if (op == RB || op == RR)
		raw_rotate(&ctx->b);
	if (op == RRA || op == RRR)
		raw_reverse(&ctx->a);
	if (op == RRB || op == RRR)
		raw_reverse(&ctx->b);
	ctx->counts[op]++;
	ft_putendl_fd(op_name(op), 1);
}

void	bring_to_top_a(t_ctx *ctx, int pos)
{
	int		n;
	t_op	op;

	if (pos <= ctx->a.size / 2)
	{
		n = pos;
		op = RA;
	}
	else
	{
		n = ctx->a.size - pos;
		op = RRA;
	}
	while (n > 0)
	{
		do_op(ctx, op);
		n--;
	}
}

void	bring_to_top_b(t_ctx *ctx, int pos)
{
	int		n;
	t_op	op;

	if (pos <= ctx->b.size / 2)
	{
		n = pos;
		op = RB;
	}
	else
	{
		n = ctx->b.size - pos;
		op = RRB;
	}
	while (n > 0)
	{
		do_op(ctx, op);
		n--;
	}
}
