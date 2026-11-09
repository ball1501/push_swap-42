*This project has been created as part of the 42 curriculum by wngamkri, susikong.*

# push_swap

## Description

`push_swap` sorts a list of integers using two stacks (`a` and `b`) and a small
set of operations. The program does **not** print the sorted numbers: it prints
the **list of operations** that, when applied to stack `a`, leave it sorted in
ascending order (smallest number on top) with `b` empty. The goal is to produce
as few operations as possible.

The 11 operations:

| Operation | Effect |
|---|---|
| `sa` / `sb` / `ss` | swap the top two elements of a / b / both |
| `pa` / `pb` | move the top element of b to a / of a to b |
| `ra` / `rb` / `rr` | rotate up (the top element goes to the bottom) |
| `rra` / `rrb` / `rrr` | reverse rotate (the bottom element goes to the top) |

The program embeds **four strategies**:

| Flag | Strategy | Complexity (in number of operations) |
|---|---|---|
| `--simple` | selection sort | O(n²) |
| `--medium` | chunk-based sort | O(n√n) |
| `--complex` | radix sort | O(n log n) |
| `--adaptive` (default) | picks one of the three above from the measured *disorder* | depends on the regime |

An optional `--bench` flag prints statistics on `stderr` after sorting.

## Instructions

### Build

```
make        # builds libft, then push_swap (no relink if nothing changed)
make clean  # removes the object files
make fclean # removes the object files and the binary
make re     # fclean + all
```

### Run

```
./push_swap [--simple | --medium | --complex | --adaptive] [--bench] <integers>
```

The integers can be given as separate arguments, as one quoted argument, or
mixed. The first number is the top of stack `a`.

```
./push_swap 4 67 3 87 23
./push_swap "4 67 3 87 23"
./push_swap --complex 4 67 3 87 23
./push_swap --bench 4 67 3 87 23 > /dev/null
```

Behaviour:

- No numbers (no argument, or only flags): prints nothing.
- Already sorted: prints nothing.
- Invalid input: prints `Error` followed by a newline on `stderr` and exits
  with status 1. Invalid means: not an integer, outside the `int` range,
  duplicate values (`7` and `007` are the same value), an unknown flag, or an
  empty argument (`""`).
- Operations are printed on `stdout`, one per line, nothing else.

### Benchmark mode

`--bench` prints on **stderr** (so `stdout` can still be piped into a checker):

```
[bench] disorder: 40.00%
[bench] strategy: Adaptive / O(n sqrt(n))
[bench] total_ops: 13
[bench] sa: 0 sb: 0 ss: 0 pa: 5 pb: 5
[bench] ra: 2 rb: 1 rr: 0 rra: 0 rrb: 0 rrr: 0
```

The strategy line shows the name requested by the user, and the complexity class
of the algorithm that really ran (for `--adaptive`, the one it selected).

### Checking the result

With the checker provided by the school:

```
ARG="4 67 3 87 23"; ./push_swap $ARG | ./checker_linux $ARG
```

`tests/bench.sh` runs a strategy on several random sets and reports the number
of operations and whether the checker answers `OK`.

## Project structure

| File | Content |
|---|---|
| `includes/push_swap.h` | data structures and prototypes |
| `src/main.c` | program flow |
| `src/parsing.c`, `src/parsing_utils.c` | argument parsing and validation, indexing |
| `src/error.c` | error exit |
| `src/stack.c` | linked-list stack primitives |
| `src/ops_raw.c`, `src/ops.c` | the 11 operations, counting and printing |
| `src/disorder.c` | disorder metric |
| `src/bench.c`, `src/bench_utils.c` | `--bench` output |
| `src/sort_simple.c` | O(n²) strategy |
| `src/sort_medium.c`, `src/sort_complex.c`, `src/sort_adaptive.c` | other strategies and strategy selection (see learner B) |
| `libft/` | our libft |

## Contributions

**Learner A (wngamkri)**

- Data structures (linked-list stacks, a single `t_ctx` structure passed to all
  functions, no global variable) and stack primitives.
- The 11 operations, with the operation counters used by `--bench`.
- Argument parsing and validation (strict `int` parsing, duplicates, flags,
  quoted arguments) and error handling without memory leaks.
- Index normalization, disorder metric, `--bench` output.
- The `--simple` strategy (O(n²)).
- Makefile, `main`, integration and test scripts.

**Learner B (<login_B>)**

_To be written by learner B._

## Design (learner A part)

### Stacks and the shared context

Each stack is a singly linked list (`t_stack`: `top` and `size`; `t_node`:
`value`, `index`, `next`). Everything the program needs (both stacks, the
counters of each operation, the selected strategy, the disorder data) lives in a
single `t_ctx` structure that is passed to every function, because global
variables are forbidden.

### Operations

The four basic moves (`raw_swap`, `raw_push`, `raw_rotate`, `raw_reverse`) only
re-wire pointers and print nothing. `do_op` is the only way the strategies move
the stacks: it applies the move, increments the counter of that operation and
prints its name. Because of this, an operation can never be played without being
counted and printed. `ss`, `rr` and `rrr` count as one operation each.

### Indexes

After parsing, each number receives an **index** (its rank among all the
values, from 0 to n-1). The sorting strategies work on indexes instead of the
real values (negative or very large numbers are then not a problem; radix sort
looks at the bits of the index, chunk sort cuts the index range into pieces).
Indexes are only computed after the duplicate check.

### Parsing and errors

`parse_number` is stricter than `atoi`: optional sign, digits only, at least one
digit, value inside the `int` range (the digits are accumulated in a `long`
with an overflow check at every digit). The duplicate check compares values,
not strings. An argument that is not a flag is split on spaces with `ft_split`
so that `"4 67 3"` and `4 67 3` behave the same. Functions that can fail return a
status instead of exiting, so the caller can free what it allocated before
calling `exit_error`; no path leaks.

### Disorder

Disorder is measured **before** any move:

```
disorder = mistakes / pairs
pairs    = n * (n - 1) / 2            (all pairs i < j)
mistakes = number of pairs with a[i] > a[j]
```

`mistakes` and `pairs` are stored as integers, so no floating-point number is
needed: `disorder < 0.2` is tested as `mistakes * 5 < pairs`, `disorder < 0.5` as
`mistakes * 2 < pairs`, and the percentage shown by `--bench` is computed with
integer arithmetic (`mistakes * 10000 / pairs`). Computing it is O(n²) in time
on the input, but it costs no push_swap operation.

## Algorithms

### Simple: selection sort (O(n²))

Learner A.

1. For `target` = 0, 1, ..., n-1: find the node whose index is `target` in `a`,
   rotate `a` (with `ra` or `rra`, whichever is shorter) until it is on top,
   then `pb`.
2. When `a` is empty, `b` is sorted from the biggest (top) to the smallest
   (bottom). Do `pa` until `b` is empty: `a` ends up sorted.

Complexity, counted in push_swap operations: n `pb` + n `pa` + at most n/2
rotations per round, so O(n²) operations in total. Extra space: only stack `b`
(the n nodes are shared between `a` and `b`; no other array is allocated).
The strategy does not take advantage of an input that is already almost sorted.
Reference: `5 4 3 2 1` is sorted in 14 operations, as in the example of the
subject.

### Medium (O(n√n))

_To be written by learner B._

### Complex (O(n log n))

_To be written by learner B._

### Adaptive

_To be written by learner B (rationale for the thresholds, techniques used in
each regime, and a brief time and space complexity argument)._

The subject fixes the regimes:

| Disorder | Required complexity |
|---|---|
| below 0.2 | O(n²) |
| from 0.2 to below 0.5 | O(n√n) |
| 0.5 and above | O(n log n) |

## Benchmarks

Number of operations measured with `tests/bench.sh` (one random set per cell for
the table; random lists have a disorder close to 50 %). To be refreshed by both
learners after the final changes.

| Strategy | 100 numbers | 500 numbers |
|---|---|---|
| `--simple` | 1441 | 33297 |
| `--medium` | 833 | 8072 |
| `--complex` | 1084 | 6784 |
| `--adaptive` | 1084 | 6784 |

Targets of the subject: 100 numbers: < 2000 (pass), < 1500 (good), < 700
(excellent); 500 numbers: < 12000 (pass), < 8000 (good), < 5500 (excellent).

Observations:

- Radix sort (`--complex`) works on indexes, so its operation count only depends
  on n (always 6784 for 500 numbers).
- On 10 random sets of 500 numbers, `--adaptive` selected `--complex` 7 times
  (6784 operations) and `--medium` 3 times (disorder just under 50 %): 7938,
  8125 and 8167 operations.

## Known limitations and planned improvements

_To be completed by learner B (Medium results close to the 8000 limit for 500
numbers, number of operations for very small stacks of 2 or 3 numbers, and the
decision about the "excellent" level)._

## Testing

- `checker_linux` (provided) on random sets, for every strategy.
- Error cases: non-numeric argument, `2147483648`, `-2147483649`, duplicates,
  `7 007`, empty argument, unknown flag, flag without numbers.
- Memory leaks checked with `valgrind` (Linux) and `leaks` (macOS), including
  paths that fail in the middle of parsing (e.g. `./push_swap "3 2 abc"`).
- `norminette` on every source file.

## Resources

### References

- The `push_swap` subject (42 curriculum).
- Wikipedia: *Selection sort*, *Radix sort*, *Big O notation*, *Sorting algorithm*.
- *Introduction to Algorithms* (Cormen, Leiserson, Rivest, Stein) for sorting
  and complexity analysis.
- GNU Make manual (pattern rules, phony targets).
- The 42 Norm and `norminette`.

### Use of AI

**Learner A**

AI (Claude) was used as a tutor and reviewer, not to write the program:

- To explain the subject and the notions needed (what the program must output,
  linked lists and pointers, the meaning of each operation, Big-O, the disorder
  metric).
- To suggest the split of work between the two learners, the file organisation
  and the first draft of `push_swap.h` (structures and prototypes), which we
  then adapted.
- To guide the implementation step by step: for each function, it gave an
  explanation and a skeleton with blanks that I completed myself, then reviewed
  my code and pointed out bugs (wrong pointer types, order of pointer updates,
  off-by-one conditions, an `int` range edge case, memory leaks on error paths).
  The functions in `stack.c`, `ops_raw.c`, `ops.c`, `error.c`, `parsing*.c`,
  `disorder.c`, `bench*.c`, `sort_simple.c`, `main.c` and the Makefile were
  written and tested by me.
- To suggest test programs and shell commands (including the benchmark script),
  and to write temporary explanatory comments for my teammate (removed before
  submission).
- To help draft this README. I checked its content against the code.

**Learner B**

_To be written by learner B._
