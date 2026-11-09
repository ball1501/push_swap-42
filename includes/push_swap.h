/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   push_swap.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wngamkri <wngamkri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/09 10:04:35 by wngamkri          #+#    #+#             */
/*   Updated: 2026/11/09 10:05:40 by wngamkri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PUSH_SWAP_H
# define PUSH_SWAP_H

# include <stdlib.h>
# include <unistd.h>
# include <limits.h>
# include "libft.h"

typedef struct s_node
{
	int				value;
	int				index;
	struct s_node	*next;
}	t_node;

typedef struct s_stack
{
	t_node	*top;
	int		size;
}	t_stack;

typedef enum e_op
{
	SA,
	SB,
	SS,
	PA,
	PB,
	RA,
	RB,
	RR,
	RRA,
	RRB,
	RRR,
	OP_COUNT
}	t_op;

typedef enum e_strat
{
	STRAT_ADAPTIVE,
	STRAT_SIMPLE,
	STRAT_MEDIUM,
	STRAT_COMPLEX
}	t_strat;

typedef struct s_ctx
{
	t_stack	a;
	t_stack	b;
	int		counts[OP_COUNT];
	t_strat	strat;
	t_strat	used;
	int		bench;
	long	mistakes;
	long	pairs;
}	t_ctx;

void		parse_args(int argc, char **argv, t_ctx *ctx);
int			parse_flag(const char *arg, t_ctx *ctx);
int			parse_number(const char *str, int *out);
void		assign_index(t_stack *s);
void		free_words(char **words);
t_node		*node_new(int value);
int			stack_push_back(t_stack *s, int value);
void		stack_free(t_stack *s);
int			stack_is_sorted(t_stack *s);
int			stack_find_pos(t_stack *s, int index);
void		raw_swap(t_stack *s);
void		raw_push(t_stack *from, t_stack *to);
void		raw_rotate(t_stack *s);
void		raw_reverse(t_stack *s);
void		do_op(t_ctx *ctx, t_op op);
char		*op_name(t_op op);
void		bring_to_top_a(t_ctx *ctx, int pos);
void		bring_to_top_b(t_ctx *ctx, int pos);
void		compute_disorder(t_ctx *ctx);
void		print_bench(t_ctx *ctx);
char		*strat_name(t_strat s);
char		*strat_complexity(t_strat s);
int			total_ops(t_ctx *ctx);
void		sort_simple(t_ctx *ctx);
void		sort_medium(t_ctx *ctx);
void		sort_complex(t_ctx *ctx);
void		sort_adaptive(t_ctx *ctx);
void		run_strategy(t_ctx *ctx);
void		exit_error(t_ctx *ctx);

#endif
