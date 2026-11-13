/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wngamkri <wngamkri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/27 10:53:44 by wngamkri          #+#    #+#             */
/*   Updated: 2026/11/10 16:39:19 by wngamkri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

int	parse_flag(const char *arg, t_ctx *ctx)
{
	if (!ft_strncmp(arg, "--simple", 9))
		ctx->strat = STRAT_SIMPLE;
	else if (!ft_strncmp(arg, "--medium", 9))
		ctx->strat = STRAT_MEDIUM;
	else if (!ft_strncmp(arg, "--complex", 10))
		ctx->strat = STRAT_COMPLEX;
	else if (!ft_strncmp(arg, "--adaptive", 11))
		ctx->strat = STRAT_ADAPTIVE;
	else if (!ft_strncmp(arg, "--bench", 8))
		ctx->bench = 1;
	else
		return (-1);
	return (0);
}

static int	has_duplicate(t_stack *s, int value)
{
	t_node	*cur;

	cur = s->top;
	while (cur)
	{
		if (cur->value == value)
			return (1);
		cur = cur->next;
	}
	return (0);
}

static int	add_number(const char *arg, t_ctx *ctx)
{
	int	value;

	if (parse_number(arg, &value) == -1)
		return (-1);
	if (has_duplicate(&ctx->a, value))
		return (-1);
	if (stack_push_back(&ctx->a, value) == -1)
		return (-1);
	return (0);
}

static int	add_args(const char *arg, t_ctx *ctx)
{
	char	**words;
	int		i;

	words = ft_split(arg, ' ');
	if (!words)
		return (-1);
	if (!words[0])
	{
		free_words(words);
		return (-1);
	}
	i = 0;
	while (words[i])
	{
		if (add_number(words[i], ctx) == -1)
		{
			free_words(words);
			return (-1);
		}
		i++;
	}
	free_words(words);
	return (0);
}

void	parse_args(int argc, char **argv, t_ctx *ctx)
{
	int	i;

	i = 1;
	while (i < argc)
	{
		if (argv[i][0] == '-' && argv[i][1] == '-')
		{
			if (parse_flag(argv[i], ctx) == -1)
				exit_error(ctx);
		}
		else if (add_args(argv[i], ctx) == -1)
			exit_error(ctx);
		i++;
	}
	assign_index(&ctx->a);
}
