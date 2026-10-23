/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wngamkri <wngamkri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/29 16:31:02 by wngamkri          #+#    #+#             */
/*   Updated: 2026/08/29 16:31:03 by wngamkri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	count_words(char const *s, char c)
{
	size_t	count;
	size_t	in_word;

	count = 0;
	in_word = 0;
	while (*s)
	{
		if (*s != c && in_word == 0)
		{
			in_word = 1;
			count++;
		}
		else if (*s == c)
			in_word = 0;
		s++;
	}
	return (count);
}

static void	free_all(char **lst, size_t i)
{
	while (i > 0)
	{
		i--;
		free(lst[i]);
	}
	free(lst);
}

static char	*next_word(char const **s, char c)
{
	char	*word;
	size_t	len;

	len = 0;
	while ((*s)[len] && (*s)[len] != c)
		len++;
	word = ft_substr(*s, 0, len);
	*s += len;
	return (word);
}

char	**ft_split(char const *s, char c)
{
	char	**lst;
	size_t	i;

	if (!s)
		return (NULL);
	lst = (char **)ft_calloc(count_words(s, c) + 1, sizeof(char *));
	if (!lst)
		return (NULL);
	i = 0;
	while (*s)
	{
		if (*s != c)
		{
			lst[i] = next_word(&s, c);
			if (!lst[i])
				return (free_all(lst, i), NULL);
			i++;
		}
		else
			s++;
	}
	return (lst);
}
