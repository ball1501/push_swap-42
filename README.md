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

`tests/bench.sh` runs a strategy on several random sets of distinct numbers.
For each run it prints the number of operations and whether the checker answers
`OK`, then a summary (minimum, maximum, average and number of failed runs):

```
bash tests/bench.sh [SIZE] [RUNS] [FLAG]          # defaults: 100 10 --adaptive
CHECKER=./checker_linux bash tests/bench.sh 500 30 --medium
```

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
| `tests/bench.sh` | benchmark and checker script |

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

**Learner B (susikong)**

- The `--medium` strategy (O(n√n) chunk-based sort with top/bottom chunk
  splitting in stack `b` and shortest-path rotation back to `a`).
- The `--complex` strategy (O(n log n) LSD radix sort on normalized indexes).
- The `--adaptive` strategy (disorder-based regime selection and dedicated
  2-element and 3-element fast paths).
- Performance tuning and documentation of the sorting strategies.

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

### Medium: chunk-based sort (O(n√n))

Learner B.

1. Compute the chunk size `chuck = ceil(√n)` using integer arithmetic
   (`while (chuck * chuck < n) chuck++`), and set the initial upper bound
   `limit = chuck`.
2. **Phase 1 (push to `b` by chunks):** Scan `a` from the top:
   - If the top element belongs to the current chunk (`index < limit`), push it
     to `b` (`pb`). If `b` has reached `limit` elements, advance to the next
     chunk (`limit += chuck`). Otherwise, if the pushed element falls in the
     lower half of the current chunk (`index < limit - chuck / 2`), rotate `b`
     (`rb`) to send it to the bottom of `b`. This pre-sorts each chunk into two
     halves inside `b` (larger indexes stay near the top, smaller ones go toward
     the bottom).
   - If the top element belongs to a later chunk (`index >= limit`), rotate `a`
     (`ra`) to inspect the next element.
3. **Phase 2 (pull back to `a` in descending order):** While `b` is not empty,
   locate the current maximum index (`b.size - 1`) in `b`, rotate `b` along the
   shorter direction (`rb` or `rrb` via `bring_to_top_b`) to bring it to the
   top, and push it back to `a` (`pa`).

Complexity, counted in push_swap operations:
- Phase 1 performs n `pb` operations and at most n `rb` operations. The scan of
  `a` makes about √n passes (one per chunk), and each pass costs at most n
  `ra` rotations, so the `ra` rotations cost O(n√n) in total.
- Phase 2 performs n `pa` operations. Because elements are pushed to `b` chunk
  by chunk and split into upper/lower halves with `rb`, the next maximum
  (`b.size - 1`) is in practice within about O(√n) positions of the top or
  bottom of `b`, costing O(√n) rotations (`rb`/`rrb`) per element, or O(n√n) in
  total.
- Total time (operations): O(n√n). Extra space: O(n) in the two stacks (`a` and
  `b`), with O(1) auxiliary variables.

### Complex: LSD radix sort (O(n log n))

Learner B.

1. Work on the binary representation of the normalized indexes (`0` to `n - 1`),
   from the least significant bit (`bit = 0`) upward until stack `a` is sorted.
2. In pass `bit`, inspect each of the `n` elements currently in `a`:
   - If bit `bit` of `top->index` is `0`, push the element to `b` (`pb`).
   - If bit `bit` of `top->index` is `1`, keep it in `a` by rotating (`ra`).
3. Once all `n` elements have been processed for that bit, push everything from
   `b` back to `a` (`pa` until `b` is empty). Because `pb` followed by `pa`
   preserves the relative order of the `0`-bit group and places it in front of
   the `1`-bit group, each pass is a stable bucket pass on one bit.

Complexity, counted in push_swap operations:
- The maximum index `n - 1` has `ceil(log₂ n)` bits, so the outer loop runs at
  most `ceil(log₂ n)` times.
- Each pass performs exactly `n` operations (`ra` or `pb`) on `a`, plus at most
  `n` `pa` operations to empty `b` (at most `2n` operations per bit).
- Total time (operations): O(n log n). Extra space: O(n) shared between stacks
  `a` and `b`, with O(1) auxiliary variables.

### Adaptive

Learner B.

The adaptive strategy selects an internal sorting method based on the initial
disorder `mistakes / pairs` measured before any move:

| Disorder | Condition (integer math) | Selected method | Complexity |
|---|---|---|---|
| `< 0.2` | `mistakes * 10 < pairs * 2` | `sort_simple` | O(n²) |
| `0.2` to `< 0.5` | `mistakes * 10 < pairs * 5` | `sort_medium` | O(n√n) |
| `>= 0.5` | `mistakes * 10 >= pairs * 5` | `sort_complex` | O(n log n) |

- **Rationale for thresholds and regimes:** The thresholds (`0.2` and `0.5`)
  follow the three disorder regimes defined by the subject. All comparisons use
  cross-multiplication on `long` integers (`mistakes` and `pairs`) to avoid
  floating-point inaccuracies. The measurements below show why the regimes make
  sense: on an almost sorted input (a 100-number list with a single swapped
  pair, disorder 0.02 %) the simple strategy needs only 202 operations, about
  2n. On random lists (disorder around 50 %) the best method depends on the
  size: with 100 numbers `--medium` is better than `--complex` (about 775
  against 1084 operations), while with 500 numbers `--complex` is better than
  `--medium` (6784 against about 7400 operations). Random lists sit right next
  to the 0.5 threshold, so `--adaptive` alternates between the two methods.
- **Fast path for tiny stacks (`n == 2` and `n == 3`):** In `--adaptive` mode,
  after setting `ctx->used` (so `--bench` still reports the theoretical
  complexity class matching the disorder regime) and checking whether `a` is
  already sorted, stacks of size 2 and 3 are handled directly:
  - `n == 2`: a single `sa` (1 operation instead of 3 in radix).
  - `n == 3` (`sort_three`): rotates the largest of the three elements to the
    bottom with at most one `ra` or `rra`, then swaps the top two with `sa` if
    needed (at most 2 operations instead of up to 10 in radix).
  When the user explicitly forces `--simple`, `--medium`, or `--complex`, the
  forced algorithm runs without this shortcut.
- **Time and space complexity:** In the Push_swap operation model, the upper
  bound on the number of operations is O(n²) in the low-disorder regime,
  O(n√n) in the medium-disorder regime, and O(n log n) in the high-disorder
  regime. Extra space is O(n) for the nodes stored across stacks `a` and `b` and
  O(1) auxiliary memory.

## Benchmarks

Number of operations measured with `tests/bench.sh` on random lists of distinct
numbers (random lists have a disorder close to 50 %). The number of runs is
given in each cell; every run was verified by the checker (`OK`).

| Strategy | 100 numbers (min / avg / max) | 500 numbers (min / avg / max) |
|---|---|---|
| `--simple` | 1441 (1 set) | 31765 to 33297 (2 sets) |
| `--medium` | 747 / 775 / 805 (10 runs) | 7286 / 7423 / 7551 (30 runs) |
| `--complex` | 1084 (constant) | 6784 (constant) |
| `--adaptive` | 742 / 962 / 1084 (30 runs) | 6784 / 7063 / 7596 (30 runs) |

Targets of the subject: 100 numbers: < 2000 (pass), < 1500 (good), < 700
(excellent); 500 numbers: < 12000 (pass), < 8000 (good), < 5500 (excellent).
All measured runs reach the "good" level for both sizes.

Observations:

- Radix sort (`--complex`) works on indexes, so its operation count only depends
  on n (always 1084 for 100 numbers and 6784 for 500 numbers).
- With 100 numbers, `--adaptive` gives two groups of results: about 740 to 800
  operations when it selects `--medium`, and 1084 when it selects `--complex`.
  With 500 numbers it gives 6784 when it selects `--complex` and about 7300 to
  7600 when it selects `--medium`.
- Adding the `rb` lower-half chunk split in `--medium` lowered the 500-number
  operation count from about 8070 (first version) to 7423 on average (30 runs),
  so the result stays under the 8000 "good" threshold even when `--adaptive`
  picks `--medium` on random sets with disorder just below 50 %.
- The "excellent" level is not reached: the best 100-number result is 742
  (< 700 needed) and the 500-number results are above 6700 (< 5500 needed).

## Known limitations and planned improvements

- **Medium strategy on 500 numbers:** Initially, pushing chunks to `b` without
  pre-ordering gave ~8072 operations (occasionally slightly above the 8000
  "good" threshold). Rotating elements in the lower half of each chunk to the
  bottom of `b` (`rb`) reduced the average to 7423 operations; in the 30 runs of
  `--medium` the maximum was 7551, and in the 30 runs of `--adaptive` the
  maximum was 7596, both below 8000.
- **Very small stacks (2 or 3 numbers):** A reversed 2- or 3-element stack has a
  disorder >= 0.5, which caused `--adaptive` to pick `--complex` (radix sort)
  and spend 3 operations for `2 1` and 10 operations for `3 2 1`. Adding a
  dedicated fast path for `n == 2` and `n == 3` inside `sort_adaptive` reduced
  this to at most 1 operation for 2 numbers and at most 2 operations for 3
  numbers, while keeping `--simple`, `--medium`, and `--complex` true to their
  forced algorithms.
- **Stacks of 4 or 5 numbers:** these still go through the regular strategies.
  For example `5 4 3 2 1` takes 25 operations with `--adaptive` (radix sort),
  while `--simple` needs 14.
- **"Excellent" benchmark tier (< 700 for 100 numbers, < 5500 for 500):** We
  chose binary LSD radix sort for `--complex` and √n chunk sort for `--medium`
  because their complexity bounds are simple to prove and completely predictable,
  consistently achieving the "good" tier (< 1500 and < 8000). Reaching the
  "excellent" tier would require either larger tuned chunk sizes or a greedy
  cost-based insertion sort (such as the Turk algorithm with combined `rr`/`rrr`
  rotations).

## Testing

- The checker provided by the school (`checker_Mac` on macOS, `checker_linux` on Linux) on random sets, for every strategy, and on all the permutations of 5 numbers.
- Error cases: non-numeric argument, `2147483648`, `-2147483649`, duplicates,
  `7 007`, empty argument, unknown flag, flag without numbers.
- Memory leaks checked with `leaks` (macOS), including paths that fail in the
  middle of parsing (e.g. `./push_swap "3 2 abc"`).
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

AI (Claude) was used as a tutor and reviewer; the program was written by me
from its explanations and skeletons:

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
  completed by me from these skeletons and tested by me.
- To suggest test programs and shell commands. The small `main` files used to
  test each module were based on skeletons given by the AI. I wrote the first
  steps of `tests/bench.sh` myself; the final version (minimum, maximum,
  average and failure count) was written by the AI at my request.
- To write temporary explanatory comments for my teammate (removed before
  submission).
- To help draft this README, including the benchmark tables built from my own
  measurements. I checked its content against the code.

**Learner B**

AI was used as a learning assistant and code reviewer:

- To study and compare sorting approaches suitable for two stacks (√n chunk-based
  sorting, LSD radix sort on binary indexes, and disorder-based adaptive
  switching).
- To analyze why the initial chunk sort was close to 8000 operations for 500
  elements and how splitting each chunk in stack `b` with `rb` reduces the
  number of rotations during the push-back phase.
- To review the special cases for 2 and 3 elements in `--adaptive` mode and
  check compliance with the 42 Norm.
- To help draft and proofread Learner B's sections of this README.
