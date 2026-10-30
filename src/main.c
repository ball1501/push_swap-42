/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wngamkri <wngamkri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/30 17:16:47 by wngamkri          #+#    #+#             */
/*   Updated: 2026/10/30 17:36:50 by wngamkri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

int	main(int argc, char **argv)
{
	t_ctx	ctx;

	if (argc < 2)
		return (0);
	ft_memset(&ctx, 0, sizeof(t_ctx));
	parse_args(argc, argv, &ctx);
	if (ctx.a.size == 0)
		return (0);
	compute_disorder(&ctx);
	ctx.used = STRAT_SIMPLE;
	if (!stack_is_sorted(&ctx.a))
		sort_simple(&ctx);
	if (ctx.bench)
		print_bench(&ctx);
	stack_free(&ctx.a);
	stack_free(&ctx.b);
	return (0);
}
