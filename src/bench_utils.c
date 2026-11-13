/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bench_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wngamkri <wngamkri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/30 09:46:14 by wngamkri          #+#    #+#             */
/*   Updated: 2026/11/10 16:36:47 by wngamkri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

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
