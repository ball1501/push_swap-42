/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wngamkri <wngamkri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/29 16:27:11 by wngamkri          #+#    #+#             */
/*   Updated: 2026/08/29 16:27:12 by wngamkri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	is_space(char c)
{
	if ((c >= 9 && c <= 13) || c == 32)
		return (1);
	return (0);
}

int	ft_atoi(const char *str)
{
	size_t	i;
	long	sum;
	int		sign;

	i = 0;
	sum = 0;
	sign = 1;
	while (is_space(str[i]))
		i++;
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (ft_isdigit(str[i]))
	{
<<<<<<< HEAD
		sum = (sum * 10) + (str[i] - '0');
=======
		sum  = (sum * 10) + (str[i] - '0');
>>>>>>> d03270e08dba5aae43e794737259f8576a99b6f0
		i++;
	}
	return ((int)(sum * sign));
}
