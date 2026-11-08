/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_adaptive.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: susikong <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/04 20:34:40 by susikong          #+#    #+#             */
/*   Updated: 2026/11/04 20:34:45 by susikong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "push_swap.h"

void	sort_adaptive(t_ctx *ctx)
{
	if (ctx->pairs == 0 || ctx->mistakes * 10 < ctx->pairs * 2)
		ctx->used = STRAT_SIMPLE;
	else if (ctx->mistakes * 10 < ctx->pairs * 5)
		ctx->used = STRAT_MEDIUM;
	else
		ctx->used = STRAT_COMPLEX;
	if (stack_is_sorted(&(ctx->a)))
		return ;
	if (ctx->used == STRAT_SIMPLE)
		sort_simple(ctx);
	else if (ctx->used == STRAT_MEDIUM)
		sort_medium(ctx);
	else
		sort_complex(ctx);
}

void	run_strategy(t_ctx *ctx)
{
	if (ctx->strat == STRAT_ADAPTIVE)
	{
		sort_adaptive(ctx);
		return ;
	}
	ctx->used = ctx->strat;
	if (stack_is_sorted(&(ctx->a)))
		return ;
	if (ctx->strat == STRAT_SIMPLE)
		sort_simple(ctx);
	else if (ctx->strat == STRAT_MEDIUM)
		sort_medium(ctx);
	else if (ctx->strat == STRAT_COMPLEX)
		sort_complex(ctx);
}
