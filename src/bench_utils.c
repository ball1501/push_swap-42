/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bench_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wngamkri <wngamkri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/30 09:46:14 by wngamkri          #+#    #+#             */
/*   Updated: 2026/10/30 10:32:09 by wngamkri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*   TEMP COMMENTS FOR TEAMMATE - remove before submission (Norm forbids      */
/*   comments inside function bodies).                                        */
/*                                                                            */
/*   bench_utils.c : small helpers for the --bench output (bench.c).         */
/*                                                                            */
/*   strat vs used (both are fields of t_ctx):                                */
/*     ctx->strat : the strategy REQUESTED by the user (default adaptive)    */
/*     ctx->used  : the algorithm that REALLY ran                             */
/*   They are the same, except in adaptive mode, where "used" is the         */
/*   algorithm chosen from the disorder:                                      */
/*     disorder < 0.2        -> STRAT_SIMPLE   O(n^2)                        */
/*     0.2 <= disorder < 0.5 -> STRAT_MEDIUM   O(n sqrt(n))                  */
/*     disorder >= 0.5       -> STRAT_COMPLEX  O(n log n)                    */
/*   --bench prints the requested name and the complexity of the one that    */
/*   really ran, e.g. "Adaptive / O(n sqrt(n))".                              */
/*   So run_strategy (sort_adaptive.c) MUST set ctx->used in every case.     */
/* ************************************************************************** */

#include "push_swap.h"

/*
** strat_name
** Name printed after "strategy:" in --bench. Same if/return pattern as
** op_name (no switch in the Norm).
** Called with ctx->strat, so "Adaptive" is a valid answer here.
*/
char	*strat_name(t_strat s)
{
	if (s == STRAT_SIMPLE)
		return ("Simple");
	if (s == STRAT_MEDIUM)
		return ("Medium");
	if (s == STRAT_COMPLEX)
		return ("Complex");
	if (s == STRAT_ADAPTIVE)
		return ("Adaptive");
	return ("");
}

/*
** strat_complexity
** Complexity class printed in --bench (plain ASCII on purpose).
** Must be called with ctx->used, NOT ctx->strat: adaptive has no single
** complexity of its own, it takes the one of the algorithm it picked.
** STRAT_ADAPTIVE is not handled (returns ""): it should never be passed.
*/
char	*strat_complexity(t_strat s)
{
	if (s == STRAT_SIMPLE)
		return ("O(n^2)");
	if (s == STRAT_MEDIUM)
		return ("O(n sqrt(n))");
	if (s == STRAT_COMPLEX)
		return ("O(n log n)");
	return ("");
}

/*
** total_ops
** Total number of operations played: the sum of the 11 counters.
** counts[] has OP_COUNT slots (one per operation, indexed by the t_op
** value), so the loop goes from 0 to OP_COUNT - 1.
** ss, rr and rrr each count as ONE operation (they are printed once).
*/
int	total_ops(t_ctx *ctx)
{
	int	total;
	int	i;

	total = 0;
	i = 0;
	while (i < OP_COUNT)
	{
		total += ctx->counts[i];
		i++;
	}
	return (total);
}
