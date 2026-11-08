// codebreaker/exact/draft/shapes.hpp -- typed C++ from codebreaker.exact.draft.shapes.
//
// shapes.py -- the control flow a compiler gives a statement, read back into the statement: a comparison's
// value, the returns, the loops and their tests, a switch's arms, the gotos the model leaves.
//
// Every pass is a text rewrite over a C-like draft body (lines of text).  C++ spelling notes:
//  * a pass that Python gives back "the text, or None when it changes nothing" returns std::optional<py::str>;
//  * a pass that gives back a list of variants returns py::list<py::str>;
//  * the module-level tables the passes fill (UCHAR, PTRSIZE, LONGINFO, ...) are the ones of exact/draft/text
//    (its typed port, codebreaker::exact::draft::text): shared handles, filled in place as Python's are;
//  * `most` (unsigned_fields, adjacent_swaps, goto_tails) keeps Python's defaults.
#pragma once

#include <optional>

#include "codebreaker/exact/draft/text.hpp"
#include "py.hpp"

namespace codebreaker::exact::draft::shapes {

// if (C) { r = 1; } else { r = 0; } - a comparison's value (the compiler's 1, then 0 unless it holds): r = C
py::str compare_values(const py::str& text);

// a || or && taken as a value - the compiler's 1/0 behind the tests - read back
py::str logical_values(const py::str& text);

// C ? A : B - one test's two values; a value that is itself a test of the same two values joins the test
py::str _ternary(const py::str& c, const py::str& a, const py::str& b);

// a register stepped to test it - inc al; jne - read back as the compare it is
py::str step_tests(const py::str& text);

// a register given one value or another by a test, then read once by the next statement - C's ?: read back
py::str conditional_values(const py::str& text);

// if (C) { r = E1; } else { ..; r = E2; } return r;  ->  if (C) return E1; ..; return E2;
py::str branch_returns(const py::str& text);

// a switch whose cases are one run of values - LO..HI - the compiler tests as a range
py::str range_switches(const py::str& text);

// `return r;` at the end, every way to it giving r a value last: each of those a return of its value
py::str push_returns(const py::str& text);

// if (A) { ..; r = E1; } else if (B) { ..; r = E2; } .. return r;  -> each arm returns its value
py::str chain_returns(const py::str& text);

// al = E; ah = 0;  - a byte widened with zeros: ax = E, E an unsigned char
py::str zero_extends(const py::str& text);

// al given a byte while ah is still the 0 an earlier widening left
py::str high_zero(const py::str& text);

// al = E; ... ax read before ax or al is given again, al never read: the byte widened for its use (cbw)
py::str widen_bytes(const py::str& text);

// r = E(*p); p = p + n; X uses r once - the load and the step are *p++ in its place
py::str post_increments(const py::str& text);

// r = E0; for (;;) { v = r; .. r = E1; continue; .. } - a register carried round a loop into a variable
py::str loop_registers(const py::str& text);

// x = K; if/while (.. x ..) at once - the test of what x was just given: of K (a variant)
std::optional<py::str> const_tests(const py::str& text);

// in a loop: if (C) { S; continue; } else { R } (R leaving) last in its body - the leaving test first
std::optional<py::str> loop_guards(const py::str& text);

// for (;;) { if (C) break; S } - the loop's test first: while (!C) { S }
py::str first_breaks(const py::str& text);

// while (C) { if (D) { S } else { break; } } - the loop's own test the two
std::optional<py::str> loop_tests(const py::str& text);

// goto L; with L: in another arm, a few plain statements to that arm's end after it: the statements in place
std::optional<py::str> goto_tails(const py::str& text, int64_t most = 3);

// ax = E; r = ax; switch (ax) - the selector an assignment; a call whose value the next test reads
py::str call_tests(const py::str& text);

// a switch arm ending with what the next arm is, word for word, and leaves: that tail gone
py::str fall_dups(const py::str& text);

// goto L; where what comes after it is L: itself - the goto only falls there: gone
py::str fall_gotos(const py::str& text);

// if ((f(..) != 0)) { .. return ..; } then the arm's end, the function's value read after: ax = 0;
py::str tested_zero(const py::str& text);

// for (;;) { [x = E;] if (C1) continue; .. if (Cn) { leave } } - a loop of tests going round while one fails
py::str continue_loops(const py::str& text);

// for (;;) { [x = E;] if (C1) break; .. if (Cn) { break; } } - a loop of tests alone
py::str empty_loops(const py::str& text);

// M = E; x = M; - a value stored and read straight back from where it went: x = M = E
py::str stored_reads(const py::str& text);

// r = E; x = r; if ((r != 0)) - a value stored, then tested in the register still holding it
py::str stored_tests(const py::str& text);

// the returns as a source with early exits writes them (a variant)
std::optional<py::str> flat_returns(const py::str& text);

// switch (x) { default: L: S .. case K: .. goto L; .. } - the default arm the code after the switch
py::str switch_follow(const py::str& text);

// switch (..) { .. case K: return V; } - the arms that only return written first (a variant)
std::optional<py::str> return_cases_first(const py::str& text);

// switch (x) { case K: .. f(ax) .. } - an arm reading the register the selector was compared in: x itself
py::str selector_reads(const py::str& text);

// [the text with one int field of a struct unsigned] - a variant each
py::list<py::str> unsigned_fields(const py::str& text, int64_t most = 3);

// [the text with two next statements swapped] - a variant each
py::list<py::str> adjacent_swaps(const py::str& text, int64_t most = 4);

// ((int *)((char *)p + N))[E] - words from N bytes into what an int *p points to: p[E + N/2] (a variant)
py::str word_offsets(const py::str& text);

// *(int *)buf = E; - a buffer's first word given: its first byte (a variant)
py::str byte_heads(const py::str& text);

// if (C) { A; T } else { T } - both arms ending in the same statements: if (C) { A } T
py::str common_tails(const py::str& text);

// if (C) { S; A } else { S; B } - both arms beginning with the same statement: S; if (C) { A } else { B }
py::str common_heads(const py::str& text);

// t = A; v = F(B); .. (G(t) op v) - two calls an operator joins, the compiler's order (a variant)
std::optional<py::str> call_pairs(const py::str& text);

// *(C ? A : B) = E; - one store the compiler shares between two ways to its place
py::str lvalue_choices(const py::str& text);

// L: S; goto H; inside a loop H: while (..) { .. } - the code a goto in the loop jumps to (a variant)
std::optional<py::str> label_sinks(const py::str& text);

// a function ending in a loop that never ends, returning a variable from inside it: the return at the end (a variant)
std::optional<py::str> shared_exit(const py::str& text);

// bx = E; bx = (bx << 1) + P; .. *bx .. - a word element through a pointer P held in the data: *(P + E)
py::str pointer_index(const py::str& text);

// [switch (x) { .. case K: return V; .. } with that case an if before it] (variants)
py::list<py::str> case_guards(const py::str& text);

// common_heads, then the ?: values it leaves and the registers and temporaries folded (a variant)
std::optional<py::str> heads_variant(const py::str& text);

// if (C1) { A; T } else if (C2) { B; T } else { return R; } - every way on ending in the same tail T
std::optional<py::str> chain_tails(const py::str& text);

// goto L; ... L: return x; - a jump to a plain return: the return in the goto's place
py::str goto_returns(const py::str& text);

// a char local read in a test as (int)c (a variant)
std::optional<py::str> char_widens(const py::str& text);

// a continue last in the last statement of a loop's body: nothing
py::str tail_continues(const py::str& text);

// for (;;) { x = E; if (C(x)) break; S } - the value tested as it is given: while (!C((x = E))) { S }
std::optional<py::str> assign_loops(const py::str& text);

// .. r = A; L: X = r; } .. } else { .. r = B; goto L; } - one store shared between the two arms of an if
py::str store_joins(const py::str& text);

// L: for (;;) { for (;;) { S } break; } - an outer loop only restarting the inner one
py::str loop_collapse(const py::str& text);

// switch (x) { default: S continue; case K: break; } last in a loop's body - one value passed over
py::str switch_ifs(const py::str& text);

// if (C) { r = A; L: V = r; S } else { r = B; goto L; } - the value of a ?: the compiler gives in each arm
py::str ternary_joins(const py::str& text);

// V = ax; .. f(.., ax) - the register read again while it still holds what was stored from it: V there
py::str stored_values(const py::str& text);

// r = E; if ((r != K1)) { if ((r == K2)) { L: S } } else { goto L; } - two values to one arm
py::str two_case_switches(const py::str& text);

// ax = E; X = ax; X' = dx; ax = (ax | dx); if ((ax == 0)) - a long given and tested at once
py::str long_tests(const py::str& text);

// the long two word expressions are, the low and the high, or None (fills text::LONGINFO)
std::optional<py::str> _pair_long(const py::str& lo, const py::str& hi);

// if (C) { .. ax = E; } else { .. ax = L; dx = H; } X = ax; X' = dx; - a long both arms leave in dx:ax
py::str sunk_pairs(const py::str& text);

// if (C) { .. r = E1; } else { .. r = E2; } S(r); - a statement both arms ended with: S in each arm
py::str sunk_tails(const py::str& text);

// r = E; A = r; B = r; - r given once and stored twice at once: B = A = E
py::str register_chains(const py::str& text);

// do { r = E; [p = p + 1;] M = r; } while (r != 0) - an empty loop on the assignment, while (M = E) ;
py::str store_loops(const py::str& text);

// R = X; (R a scratch register, X a variable): the reads of R that follow are X
py::str copy_props(const py::str& text);

// X = E; X = X op F; (F not reading X): one expression - X = E op F
py::str self_updates(const py::str& text);

// R = X; X = X + 1 (or - 1); then R read once: the value before the step - X++ (X--) in its place
py::str post_steps(const py::str& text);

// a byte copied and the copy tested: if ((*d++ = *--s) == 0) break;
py::str copy_tests(const py::str& text);

// for (;;) { if (C) break; ..} - the loop's test first: while (!C) { ..}
py::str test_first_loops(const py::str& text);

// r = f(..); LV = r; and r read no more: LV = f(..)
py::str stored_calls(const py::str& text);

// a while loop whose body ends by stepping a variable - a for loop (a variant)
std::optional<py::str> for_loops(const py::str& text);

// the model's temporaries (tN) read once, by the next statement: the value in its place
py::str temporaries(const py::str& text);

// what a model leaves of the machine's registers, folded where the text alone shows how
py::str fold_registers(const py::str& text);

}  // namespace codebreaker::exact::draft::shapes
