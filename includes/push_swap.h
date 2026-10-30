#ifndef PUSH_SWAP_H
# define PUSH_SWAP_H

# include <stdlib.h>
# include <unistd.h>
# include <limits.h>
# include "libft.h"

/* ************************************************************************** */
/*   Data structures                                                          */
/* ************************************************************************** */

/*
** One element of a stack (singly linked list).
** value : the real integer given by the user
** index : rank of value among all values (0 = smallest, n-1 = biggest)
*/
typedef struct s_node
{
	int				value;
	int				index;
	struct s_node	*next;
}	t_node;

/* A stack: top is the first element. */
typedef struct s_stack
{
	t_node	*top;
	int		size;
}	t_stack;

/* The 11 push_swap operations. OP_COUNT is the number of operations. */
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

/* Strategy selector (flags --adaptive --simple --medium --complex). */
typedef enum e_strat
{
	STRAT_ADAPTIVE,
	STRAT_SIMPLE,
	STRAT_MEDIUM,
	STRAT_COMPLEX
}	t_strat;

/*
** Everything the program needs, passed to every function (no globals!).
** counts   : how many times each operation was printed (for --bench)
** strat    : strategy requested by the user (default STRAT_ADAPTIVE)
** used     : strategy actually run (differs from strat when adaptive)
** mistakes : number of pairs (i < j) where a[i] > a[j]
** pairs    : total number of pairs, n * (n - 1) / 2
**            disorder = mistakes / pairs (kept as integers, no float)
*/
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

/* ************************************************************************** */
/*   parsing.c / parsing_utils.c   (person A)                                 */
/* ************************************************************************** */

void 		parse_args(int argc, char **argv, t_ctx *ctx);
int			parse_flag(const char *arg, t_ctx *ctx);
int			parse_number(const char *str, int *out);
void		assign_index(t_stack *s);

/* ************************************************************************** */
/*   stack.c   (person A)                                                     */
/* ************************************************************************** */

t_node		*node_new(int value);
int			stack_push_back(t_stack *s, int value);
void		stack_free(t_stack *s);
int			stack_is_sorted(t_stack *s);
int			stack_find_pos(t_stack *s, int index);

/* ************************************************************************** */
/*   ops.c / ops_raw.c   (together)                                           */
/* ************************************************************************** */

/* Raw operations: change a stack only, print nothing, count nothing. */
void		raw_swap(t_stack *s);
void		raw_push(t_stack *from, t_stack *to);
void		raw_rotate(t_stack *s);
void		raw_reverse(t_stack *s);

/* Applies op on ctx, increments ctx->counts[op], prints the op name. */
void		do_op(t_ctx *ctx, t_op op);
char		*op_name(t_op op);

/* Helper: rotate (ra/rra) or (rb/rrb) by the shortest way to bring the
** element at position pos (0 = top) to the top. */
void		bring_to_top_a(t_ctx *ctx, int pos);
void		bring_to_top_b(t_ctx *ctx, int pos);

/* ************************************************************************** */
/*   disorder.c / bench.c   (person A)                                        */
/* ************************************************************************** */

void		compute_disorder(t_ctx *ctx);
void		print_bench(t_ctx *ctx);
char		*strat_name(t_strat s);
char		*strat_complexity(t_strat s);
int			total_ops(t_ctx *ctx);

/* ************************************************************************** */
/*   Sorting strategies                                                       */
/* ************************************************************************** */

void		sort_simple(t_ctx *ctx);
void		sort_medium(t_ctx *ctx);
void		sort_complex(t_ctx *ctx);
void		sort_adaptive(t_ctx *ctx);
void		run_strategy(t_ctx *ctx);

/* ************************************************************************** */
/*   errors                                                                   */
/* ************************************************************************** */

void		exit_error(t_ctx *ctx);

#endif
