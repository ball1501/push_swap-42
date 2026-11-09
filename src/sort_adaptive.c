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

/*
** เรียงกองขนาด 3 ตัว:
** 1) ถ้าตัวบนสุด (a) มากสุด -> หมุนลงล่างด้วย RA
** 2) ถ้าตัวกลาง (b) มากสุด -> ดึงตัวล่างขึ้นบนด้วย RRA
** 3) ถ้า 2 ตัวบนยังไม่เรียงกัน -> สลับด้วย SA
*/
static void	sort_three(t_ctx *ctx)
{
	int	a;
	int	b;
	int	c;

	a = ctx->a.top->index;
	b = ctx->a.top->next->index;
	c = ctx->a.top->next->next->index;
	if (a > b && a > c)
		do_op(ctx, RA);
	else if (b > a && b > c)
		do_op(ctx, RRA);
	if (ctx->a.top->index > ctx->a.top->next->index)
		do_op(ctx, SA);
}

/*
** โหมด Adaptive: เลือกกลยุทธ์ตามค่า disorder
** และจัดการกรณีพิเศษสำหรับกอง 2 และ 3 ตัว
*/
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
	if (ctx->a.size == 2)
		do_op(ctx, SA);
	else if (ctx->a.size == 3)
		sort_three(ctx);
	else if (ctx->used == STRAT_SIMPLE)
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
