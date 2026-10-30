/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_simple.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wngamkri <wngamkri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/30 15:39:12 by wngamkri          #+#    #+#             */
/*   Updated: 2026/10/30 16:12:43 by wngamkri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*   TEMP COMMENTS FOR TEAMMATE - remove before submission (Norm forbids      */
/*   comments inside function bodies).                                        */
/*                                                                            */
/*   sort_simple.c : the O(n^2) strategy (selection sort).                   */
/* ************************************************************************** */

#include "push_swap.h"

/*
** sort_simple
** Selection sort with two stacks.
**
** Phase 1: move the numbers to b, smallest first.
**   Round 0: find the node with index 0 in a, rotate a until it is on top
**            (bring_to_top_a picks ra or rra, whichever is shorter), then pb.
**   Round 1: same with index 1. And so on, until a is empty.
**   Each pb puts the new node ON TOP of b, so at the end b is sorted from
**   biggest (top) to smallest (bottom).
**
** Phase 2: pa until b is empty.
**   The biggest comes first and every next one is put on top of it, so a
**   ends up sorted: smallest on top.
**
**   a = 3 1 2   (indexes 2 0 1)
**   ra  pb       a: 2 3      b: 1
**   pb           a: 3        b: 2 1
**   pb           a: -        b: 3 2 1
**   pa  pa  pa   a: 1 2 3    b: -          (7 operations)
**
** Why "n = ctx->a.size" is saved first: a.size shrinks at every pb while
** target grows. Using ctx->a.size in the loop condition would stop the
** loop too early and leave numbers in a.
** Why searching by index works: a node keeps its index when it is moved,
** so "the node with index target" is always found in a at round "target".
**
** Requirements: indexes are assigned (assign_index), no duplicates, b is
** empty at the start. Every move goes through do_op (counted + printed).
** If a is already sorted main should not call this function (it would
** still sort it, but with useless operations).
**
** Cost (in push_swap operations): n pb + n pa, plus at most n / 2
** rotations per round -> O(n^2) in total, which is what the subject
** asks for the "simple" strategy. Extra space: only the stack b (O(n)),
** no other array.
** It does NOT take advantage of an almost sorted input: a nearly sorted
** stack costs about the same as a random one. (Reference: sorting
** 5 4 3 2 1 gives 14 operations, same as the example of the subject.)
*/
void	sort_simple(t_ctx *ctx)
{
	int	target;
	int	n;

	n = ctx->a.size;
	target = 0;
	while (target < n)
	{
		bring_to_top_a(ctx, stack_find_pos(&ctx->a, target));
		do_op(ctx, PB);
		target++;
	}
	while (ctx->b.size > 0)
		do_op(ctx, PA);
}
