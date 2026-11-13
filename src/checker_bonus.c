/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checker_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wngamkri <wngamkri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/13 10:00:00 by wngamkri          #+#    #+#             */
/*   Updated: 2026/11/13 10:00:00 by wngamkri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

static int	read_line(char *buf)
{
	char	c;
	int		i;
	int		r;

	i = 0;
	r = read(0, &c, 1);
	while (r > 0)
	{
		if (c == '\n')
		{
			buf[i] = '\0';
			return (1);
		}
		if (i >= 4)
			return (-1);
		buf[i++] = c;
		r = read(0, &c, 1);
	}
	if (r < 0 || i > 0)
		return (-1);
	return (0);
}

static int	find_op(char *buf)
{
	int	op;

	op = 0;
	while (op < OP_COUNT)
	{
		if (!ft_strncmp(buf, op_name(op), 5))
			return (op);
		op++;
	}
	return (-1);
}

static void	run_ops(t_ctx *ctx)
{
	char	buf[5];
	int		r;
	int		op;

	r = read_line(buf);
	while (r > 0)
	{
		op = find_op(buf);
		if (op == -1)
			exit_error(ctx);
		apply_op(ctx, op);
		r = read_line(buf);
	}
	if (r < 0)
		exit_error(ctx);
}

int	main(int argc, char **argv)
{
	t_ctx	ctx;
	int		i;

	if (argc < 2)
		return (0);
	ft_memset(&ctx, 0, sizeof(t_ctx));
	i = 0;
	while (++i < argc)
		if (argv[i][0] == '-' && argv[i][1] == '-')
			exit_error(&ctx);
	parse_args(argc, argv, &ctx);
	run_ops(&ctx);
	if (stack_is_sorted(&ctx.a) && ctx.b.size == 0)
		ft_putendl_fd("OK", 1);
	else
		ft_putendl_fd("KO", 1);
	stack_free(&ctx.a);
	stack_free(&ctx.b);
	return (0);
}
