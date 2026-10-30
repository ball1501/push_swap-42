/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bench.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wngamkri <wngamkri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/30 13:54:00 by wngamkri          #+#    #+#             */
/*   Updated: 2026/10/30 16:11:30 by wngamkri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*   TEMP COMMENTS FOR TEAMMATE - remove before submission (Norm forbids      */
/*   comments inside function bodies).                                        */
/*                                                                            */
/*   bench.c : the --bench output.                                            */
/*                                                                            */
/*   Everything goes to STDERR (fd 2), never to stdout: stdout must only    */
/*   contain the operations (they are piped into the checker).               */
/*   Format (see the subject, section VI.5.1):                                */
/*     [bench] disorder: 49.93%                                               */
/*     [bench] strategy: Adaptive / O(n sqrt(n))                              */
/*     [bench] total_ops: 7997                                                */
/*     [bench] sa: 0 sb: 0 ss: 0 pa: 500 pb: 500                              */
/*     [bench] ra: 4840 rb: 1098 rr: 0 rra: 0 rrb: 1059 rrr: 0                */
/*                                                                            */
/*   Only print_bench is public (declared in push_swap.h). It calls the 3    */
/*   static helpers, one per part of the output.                              */
/*   The helpers are written above print_bench because C must know a         */
/*   function before it is called.                                            */
/* ************************************************************************** */

#include "push_swap.h"

/*
** print_disorder  (static)
** Prints the disorder as a percentage with 2 decimals, using integers only
** (our ft_putnbr_fd cannot print a float).
**
**   disorder = mistakes / pairs               e.g. 0.4993
**   x = mistakes * 10000 / pairs              e.g. 4993
**       (x is "percent * 100": 100 for the percent, 100 for 2 decimals)
**   integer part  = x / 100                   49
**   decimal part  = x % 100                   93
**
** Multiply BEFORE dividing: with integers, mistakes / pairs would be 0.
** mistakes and pairs are long, so mistakes * 10000 does not overflow.
** If the decimal part is below 10 we print a leading '0', otherwise
** 40.05% would be printed as "40.5%" (and 40.00% as "40.0%").
** pairs is 0 when the stack has fewer than 2 numbers: the check avoids a
** division by zero (disorder is shown as 0.00%).
*/
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

/*
** print_strategy  (static)
** Prints the strategy line and the total number of operations.
**   name       : ctx->strat (what the USER asked for, may be "Adaptive")
**   complexity : ctx->used  (the algorithm that REALLY ran)
** Adaptive has no complexity of its own, it takes the one of the algorithm
** it chose, so ctx->used must have been set by run_strategy.
** total_ops sums the 11 counters (ss, rr and rrr count as one each).
*/
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

/*
** print_counts  (static)
** Prints how many times each operation was played, on 2 lines, with ONE
** loop over the counters. It relies on the order of the t_op enum, which
** is the same order as the subject output:
**   line 1: SA SB SS PA PB        line 2: RA RB RR RRA RRB RRR
**   i == SA or i == RA -> a new line starts, print the "[bench] " prefix
**   i == PB or i == RRR -> last of its line, print '\n'
**   otherwise           -> print a space (so no space at the end of a line)
** op_name gives the printed name ("sa", "rr"...), (t_op)i turns the loop
** number into an operation.
*/
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

/*
** print_bench
** Called by main AFTER the sort, only when the --bench flag was given.
** (disorder uses ctx->mistakes / ctx->pairs, measured BEFORE sorting.)
*/
void	print_bench(t_ctx *ctx)
{
	print_disorder(ctx);
	print_strategy(ctx);
	print_counts(ctx);
}
