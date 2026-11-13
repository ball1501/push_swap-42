/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bench.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wngamkri <wngamkri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/30 13:54:00 by wngamkri          #+#    #+#             */
/*   Updated: 2026/11/10 16:37:18 by wngamkri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

static void	print_disorder(t_ctx *ctx)
{
	int	x;

	x = 0;
	if (ctx->pairs > 0)
		x = ctx->mistakes * 10000 / ctx->pairs;
	ft_putstr_fd("[bench] disorder: ", 2);
	ft_putnbr_fd(x / 100, 2);
	ft_putchar_fd('.', 2);
	if (x % 100 < 10)
		ft_putchar_fd('0', 2);
	ft_putnbr_fd(x % 100, 2);
	ft_putstr_fd("%\n", 2);
}

static void	print_strategy(t_ctx *ctx)
{
	ft_putstr_fd("[bench] strategy: ", 2);
	ft_putstr_fd(strat_name(ctx->strat), 2);
	ft_putstr_fd(" / ", 2);
	ft_putstr_fd(strat_complexity(ctx->used), 2);
	ft_putchar_fd('\n', 2);
	ft_putstr_fd("[bench] total_ops: ", 2);
	ft_putnbr_fd(total_ops(ctx), 2);
	ft_putchar_fd('\n', 2);
}

static void	print_counts(t_ctx *ctx)
{
	int	i;

	i = 0;
	while (i < OP_COUNT)
	{
		if (i == SA || i == RA)
			ft_putstr_fd("[bench] ", 2);
		ft_putstr_fd(op_name((t_op)i), 2);
		ft_putstr_fd(": ", 2);
		ft_putnbr_fd(ctx->counts[i], 2);
		if (i == PB || i == RRR)
			ft_putchar_fd('\n', 2);
		else
			ft_putchar_fd(' ', 2);
		i++;
	}
}

void	print_bench(t_ctx *ctx)
{
	print_disorder(ctx);
	print_strategy(ctx);
	print_counts(ctx);
}
