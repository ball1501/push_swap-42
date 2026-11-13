/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wngamkri <wngamkri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/27 09:11:32 by wngamkri          #+#    #+#             */
/*   Updated: 2026/11/10 16:38:47 by wngamkri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

static long	read_digits(const char *str, int i)
{
	long	result;

	result = 0;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (-1);
		result = result * 10 + (str[i] - '0');
		if (result > 2147483648)
			return (-1);
		i++;
	}
	return (result);
}

int	parse_number(const char *str, int *out)
{
	long	result;
	int		sign;
	int		i;

	sign = 1;
	i = 0;
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	if (!ft_isdigit(str[i]))
		return (-1);
	result = read_digits(str, i);
	if (result == -1)
		return (-1);
	result = result * sign;
	if (result < INT_MIN || result > INT_MAX)
		return (-1);
	*out = (int)result;
	return (0);
}

void	assign_index(t_stack *s)
{
	t_node	*cur;
	t_node	*other;
	int		rank;

	cur = s->top;
	while (cur)
	{
		rank = 0;
		other = s->top;
		while (other)
		{
			if (other->value < cur->value)
				rank++;
			other = other->next;
		}
		cur->index = rank;
		cur = cur->next;
	}
}

void	free_words(char **words)
{
	int	i;

	if (!words)
		return ;
	i = 0;
	while (words[i])
	{
		free(words[i]);
		i++;
	}
	free(words);
}
