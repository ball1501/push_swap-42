/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wngamkri <wngamkri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/27 09:11:32 by wngamkri          #+#    #+#             */
/*   Updated: 2026/10/30 18:24:39 by wngamkri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*   TEMP COMMENTS FOR TEAMMATE - remove before submission (Norm forbids      */
/*   comments inside function bodies).                                        */
/*                                                                            */
/*   parsing_utils.c : helpers used by parse_args (parsing.c).               */
/*                                                                            */
/*   Why not just ft_atoi?                                                    */
/*   ft_atoi cannot tell us that the input is wrong: "12a" gives 12, "abc"   */
/*   gives 0 and "2147483648" silently wraps around. push_swap must print    */
/*   "Error" in all those cases, so we write a stricter version:             */
/*   it validates the format AND the int range, and reports failure.         */
/*                                                                            */
/*   Return convention: 0 = ok, -1 = invalid.                                 */
/* ************************************************************************** */

#include "push_swap.h"

/*
** read_digits  (helper of parse_number, static = only usable in this file)
** Reads the digits of str starting at position i and returns their value
** as a positive long. The sign is handled by parse_number.
** return: the value, or -1 if a character is not a digit or the number
**         is too big. -1 can safely mean "error" because a real value
**         read here is never negative.
**
** The check "result > 2147483648" is done at EVERY digit. This stops the
** loop early, so the long can never overflow on very long inputs
** (e.g. 30 digits). The limit is 2147483648 (not 2147483647) on purpose:
** -2147483648 is a valid int (INT_MIN) and here the value is still
** positive. parse_number rejects the positive 2147483648 afterwards.
*/
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

/*
** parse_number
** Checks that str is a valid int and stores it in *out.
** Accepted : optional '+' or '-', then digits only, value in
**            [INT_MIN, INT_MAX]  ("42" "-8" "+5" "007" "-0")
** Rejected : "" "+" "-" "12a" "abc" "2147483648" "-2147483649"
** return: 0 if valid (and *out is set), -1 otherwise (*out untouched)
**
** Steps:
**   1. read the optional sign
**   2. there must be at least one digit (this is what rejects "", "+", "-";
**      without it read_digits would return 0 for them)
**   3. read_digits does the digits and the overflow protection
**   4. apply the sign, then the final range check with a long, which can
**      hold values bigger than an int
** Note: "7" and "007" give the same value, so ./push_swap 7 007 must be
** an Error (duplicate). The duplicate check must compare VALUES, not
** strings.
*/
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

/*
** assign_index
** Gives each node its index: its rank among all the values.
** smallest value -> 0, biggest -> size - 1.
**
**   values : 32  -8   5  100   9
**   index  :  3   0   1    4   2
**
** For each node (cur), count how many nodes (other) have a smaller value:
** that count is its index. Two nested loops, O(n^2), fine for our sizes.
** Comparing a node with itself is never "smaller", so no special case.
**
** WHY: the algorithms work with indexes (0..n-1) instead of real values.
** Radix looks at the bits of the index, and chunk sort cuts the index
** range into pieces. Real values (negative, huge) would be awkward.
**
** MUST be called AFTER the duplicate check: with duplicates two nodes
** would get the same index and radix/chunk would break.
*/
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

/*
** free_words
** Frees the result of ft_split: the array AND every string inside it.
**
**   words --> [ ptr0 ][ ptr1 ][ ptr2 ][ NULL ]     (the array, on the heap)
**                |       |       |
**               "3"     "2"    "abc"                (each string, on the heap)
**
** Two levels, so two kinds of free:
**   1. free each words[i], until the NULL that ends the array
**      (ft_split uses ft_calloc, so the last slot is always NULL)
**   2. free the array itself, LAST. Freeing it first and then reading
**      words[i] would read freed memory.
** The "if (!words)" lets us call this safely with the NULL returned by a
** failed ft_split.
*/
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
