/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ops.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wngamkri <wngamkri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/23 16:21:45 by wngamkri          #+#    #+#             */
/*   Updated: 2026/10/26 18:12:55 by wngamkri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*   TEMP COMMENTS FOR TEAMMATE - remove before submission (Norm forbids      */
/*   comments inside function bodies).                                        */
/*                                                                            */
/*   ops.c : the "official" way to play a move.                              */
/*                                                                            */
/*   Every sorting algorithm must move the stacks ONLY through do_op().      */
/*   That way a move can never be played without being counted and printed. */
/*                                                                            */
/*   do_op(ctx, op) = 1) apply the move with the raw_* functions             */
/*                    2) ctx->counts[op]++        (used by --bench)          */
/*                    3) print the name on stdout (this IS the output of     */
/*                       push_swap, one operation per line)                  */
/*                                                                            */
/*   t_op is an enum, so SA = 0, SB = 1, ... RRR = 10 (see push_swap.h).    */
/*   That is why op can be used as an index of counts[].                     */
/* ************************************************************************** */

#include "push_swap.h"

/*
** op_name
** Converts an operation (enum) into the text we print.
**   RA  ->  "ra"
** if/return is used instead of switch because the Norm forbids switch.
** return: the name, or "" if op is not a valid operation (should never
** happen, but every path of a function must return something).
*/
char	*op_name(t_op op)
{
	if (op == SA)
		return ("sa");
	if (op == SB)
		return ("sb");
	if (op == SS)
		return ("ss");
	if (op == PA)
		return ("pa");
	if (op == PB)
		return ("pb");
	if (op == RA)
		return ("ra");
	if (op == RB)
		return ("rb");
	if (op == RR)
		return ("rr");
	if (op == RRA)
		return ("rra");
	if (op == RRB)
		return ("rrb");
	if (op == RRR)
		return ("rrr");
	return ("");
}

/*
** do_op
** Plays ONE operation: applies it, counts it, prints it.
**
** Double operations (ss, rr, rrr) are handled by the "||":
**   op == SS  matches both  "SA || SS"  and  "SB || SS"
**   so both stacks are swapped, but it still counts and prints as ONE
**   operation ("ss"), as the subject requires.
**
** Direction of raw_push(from, to): the LAST letter of the name is the
** destination.
**   pa = push to a = take from b, put on a  -> raw_push(&b, &a)
**   pb = push to b = take from a, put on b  -> raw_push(&a, &b)
**
** Note: the move is counted and printed even if it changes nothing (for
** example pa when b is empty). The algorithms must never ask for a move
** that does nothing, it would only waste operations.
*/
void	do_op(t_ctx *ctx, t_op op)
{
	if (op == SA || op == SS)
		raw_swap(&ctx->a);
	if (op == SB || op == SS)
		raw_swap(&ctx->b);
	if (op == PA)
		raw_push(&ctx->b, &ctx->a);
	if (op == PB)
		raw_push(&ctx->a, &ctx->b);
	if (op == RA || op == RR)
		raw_rotate(&ctx->a);
	if (op == RB || op == RR)
		raw_rotate(&ctx->b);
	if (op == RRA || op == RRR)
		raw_reverse(&ctx->a);
	if (op == RRB || op == RRR)
		raw_reverse(&ctx->b);
	ctx->counts[op]++;
	ft_putendl_fd(op_name(op), 1);
}

/*
** bring_to_top_a
** Rotates stack a so that the node at position pos (top = 0) ends up on
** top, using the cheaper direction.
**
**   pos in the upper half -> press ra  "pos" times
**   pos in the lower half -> press rra "size - pos" times
**
** Example with 5 nodes (A B C D E, positions 0..4):
**   pos 1 -> ra  x1        pos 3 -> rra x2
**   pos 2 -> ra  x2        pos 4 -> rra x1
** When both ways cost the same (pos == size / 2 with an even size) we
** take ra.
** If pos is 0 nothing happens. If pos is -1 (stack_find_pos did not find
** the node), n is negative, the loop never runs and nothing happens.
**
** The function first decides WHICH button (op) and HOW MANY times (n),
** then presses it n times.
*/
void	bring_to_top_a(t_ctx *ctx, int pos)
{
	int		n;
	t_op	op;

	if (pos <= ctx->a.size / 2)
	{
		n = pos;
		op = RA;
	}
	else
	{
		n = ctx->a.size - pos;
		op = RRA;
	}
	while (n > 0)
	{
		do_op(ctx, op);
		n--;
	}
}

/*
** bring_to_top_b
** Same as bring_to_top_a, but for stack b: it should use ctx->b.size
** and the b operations (rb / rrb).
*/
void	bring_to_top_b(t_ctx *ctx, int pos)
{
	int		n;
	t_op	op;

	if (pos <= ctx->b.size / 2)
	{
		n = pos;
		op = RB;
	}
	else
	{
		n = ctx->b.size - pos;
		op = RRB;
	}
	while (n > 0)
	{
		do_op(ctx, op);
		n--;
	}
}
