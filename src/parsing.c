/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wngamkri <wngamkri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/27 10:53:44 by wngamkri          #+#    #+#             */
/*   Updated: 2026/10/28 11:03:11 by wngamkri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*   TEMP COMMENTS FOR TEAMMATE - remove before submission (Norm forbids      */
/*   comments inside function bodies).                                        */
/*                                                                            */
/*   parsing.c : reads argv and builds stack a.                              */
/*                                                                            */
/*   ./push_swap [--flag ...] n1 n2 n3 ...                                    */
/*     - an argument starting with "--" is a flag (strategy or --bench)      */
/*     - every other argument must be an int. The first number is the TOP    */
/*       of stack a.                                                          */
/*     - negative numbers like -8 start with ONE dash, so they are not flags */
/*                                                                            */
/*   Any error (bad number, out of int range, duplicate, unknown flag,       */
/*   empty argument "") ends the program through exit_error(): it prints    */
/*   "Error" on stderr, frees the stacks and exits. That is why none of the  */
/*   functions below need to return an error status to their caller.        */
/*                                                                            */
/*   REQUIREMENT: ctx must be zeroed (ft_memset) BEFORE parse_args is       */
/*   called. exit_error frees the stacks, and top must be NULL, not          */
/*   garbage. A zeroed ctx also gives the defaults: strat = STRAT_ADAPTIVE  */
/*   (first value of the enum), bench = 0.                                   */
/* ************************************************************************** */

#include "push_swap.h"

/*
** parse_flag
** Recognizes a flag and sets the matching field of ctx.
**   --simple / --medium / --complex / --adaptive -> ctx->strat
**   --bench                                       -> ctx->bench = 1
** return: 0 if the flag is known, -1 otherwise.
**
** ft_strncmp returns 0 when the strings are equal. The size n is the length
** of the flag + 1, so the '\0' is compared too: that makes the match EXACT.
** With a shorter n, "--simpleXYZ" would be accepted as "--simple".
** If several strategy flags are given, the last one wins (the subject does
** not say anything about it).
*/
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

/*
** has_duplicate  (static: only used in this file)
** return: 1 if value is already in stack s, 0 otherwise.
** It compares VALUES (ints), not strings, so "7" and "007" and "+7" are
** detected as the same number.
*/
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

/*
** add_number  (static: only used in this file)
** Checks one argument and appends it at the bottom of stack a.
** Order matters:
**   1. parse_number : is it a valid int? (value is written in "value")
**   2. has_duplicate: checked AFTER the conversion, on the int value
**   3. stack_push_back: can fail if malloc fails
** On any failure exit_error() ends the program, so there is no return value.
** If the error happens at the 4th argument, the first 3 nodes are already
** in stack a: exit_error frees them (no leak).
*/
static void	add_number(const char *arg, t_ctx *ctx)
{
	int	value;

	if (parse_number(arg, &value) == -1)
		exit_error(ctx);
	if (has_duplicate(&ctx->a, value))
		exit_error(ctx);
	if (stack_push_back(&ctx->a, value) == -1)
		exit_error(ctx);
}

/*
** parse_args
** Goes through argv (starting at 1, argv[0] is the program name).
** Flag arguments go to parse_flag, all the others to add_number.
** Test "starts with --": argv[i][0] == '-' && argv[i][1] == '-'.
** For an empty argument "", argv[i][0] is '\0', so the && stops there
** (we never read argv[i][1]) and "" goes to add_number, which rejects it.
**
** assign_index is called at the END, once all the numbers are in and
** the duplicate check is done (indexes are only valid without duplicates).
**
** After this function, stack a may be empty (no argument, or only flags):
** main must then print nothing and exit normally.
*/
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
		else
			add_number(argv[i], ctx);
		i++;
	}
	assign_index(&ctx->a);
}
