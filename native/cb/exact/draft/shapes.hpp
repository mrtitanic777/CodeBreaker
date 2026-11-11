// cb/exact/draft/shapes.hpp -- the control flow a compiler gives a statement, read back into the statement: a
// comparison's value, the returns, the loops and their tests, a switch's arms, the gotos the model leaves.
//
// Every pass is a rewrite of a draft's text (the layout cb/exact/draft/text.hpp reads: a statement to a line, blocks
// indented, tests parenthesised, `L_xxx:` labels alone on a line). A pass given a text gives back the rewritten text
// (the same text when it finds nothing to rewrite); a pass that is only one *variant* of the draft - the compiler may
// or may not have laid the code out the way the pass reads it - gives back `std::nullopt` when it changes nothing, so
// the candidates stage keeps only the variants that differ. A pass that offers several variants gives them as a list.
//
// What a pass learns of the function (a byte widened with zeros makes the place an unsigned char, a pointer stepped
// through by words, a long's two words) is recorded in the `FunctionFacts` the caller holds for the function; the one
// fact a pass reads that the model made (the ranges a loaded register is tested against) comes the same way. There are
// no tables at namespace scope.
//
// The module is in six files by what the passes read back: values (shapes_values.cpp), registers
// (shapes_registers.cpp), widths and longs (shapes_widths.cpp), returns and gotos (shapes_returns.cpp), loops
// (shapes_loops.cpp) and switches (shapes_switch.cpp), over the readers of shapes_scan.hpp.
#pragma once

#include <cstddef>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "cb/exact/draft/text.hpp"

namespace cb::exact::draft::shapes {

using text::FunctionFacts;

// ---- values: a test's value, a register's, a call's -------------------------------------------------------------

// if (C) { r = 1; } else { r = 0; } - a comparison's value (the compiler's 1, then 0 unless it holds): r = C
std::string compare_values(std::string_view text);

// a || or && taken as a value - the compiler's 1/0 behind the tests - read back
std::string logical_values(std::string_view text);

// C ? A : B - one test's two values; a value that is itself a test of the same two values joins the test
std::string ternary(std::string_view c, std::string_view a, std::string_view b);

// a register stepped to test it - inc al; jne - read back as the compare it is
std::string step_tests(std::string_view text);

// a register given one value or another by a test, then read once by the next statement - C's ?: read back
std::string conditional_values(std::string_view text);

// ax = E; r = ax; switch (ax) - the selector an assignment; a call whose value the next test reads
std::string call_tests(std::string_view text);

// M = E; x = M; - a value stored and read straight back from where it went: x = M = E
std::string stored_reads(std::string_view text);

// r = E; x = r; if ((r != 0)) - a value stored, then tested in the register still holding it
std::string stored_tests(std::string_view text);

// V = ax; .. f(.., ax) - the register read again while it still holds what was stored from it: V there
std::string stored_values(std::string_view text);

// r = E; A = r; B = r; - r given once and stored twice at once: B = A = E
std::string register_chains(std::string_view text);

// a byte copied and the copy tested: if ((*d++ = *--s) == 0) break;
std::string copy_tests(std::string_view text);

// r = E(*p); p = p + n; X uses r once - the load and the step are *p++ in its place
std::string post_increments(std::string_view text);

// R = X; X = X + 1 (or - 1); then R read once: the value before the step - X++ (X--) in its place
std::string post_steps(std::string_view text);

// X = E; X = X op F; (F not reading X): one expression - X = E op F
std::string self_updates(std::string_view text);

// ---- registers the model leaves of the machine --------------------------------------------------------------

// R = X; (R a scratch register, X a variable): the reads of R that follow are X
std::string copy_props(std::string_view text);

// r = f(..); LV = r; and r read no more: LV = f(..)
std::string stored_calls(std::string_view text);

// the model's temporaries (tN) read once, by the next statement: the value in its place
std::string temporaries(std::string_view text);

// what a model leaves of the machine's registers, folded where the text alone shows how
std::string fold_registers(std::string_view text);

// ---- widths and longs ------------------------------------------------------------------------------------------

// al = E; ah = 0;  - a byte widened with zeros: ax = E, E an unsigned char (recorded in facts)
std::string zero_extends(std::string_view text, FunctionFacts& facts);

// al given a byte while ah is still the 0 an earlier widening left
std::string high_zero(std::string_view text, FunctionFacts& facts);

// al = E; ... ax read before ax or al is given again, al never read: the byte widened for its use (cbw)
std::string widen_bytes(std::string_view text);

// a char local read in a test as (int)c (a variant)
std::optional<std::string> char_widens(std::string_view text);

// [the text with one int field of a struct unsigned] - a variant each
std::vector<std::string> unsigned_fields(std::string_view text, std::size_t most = 3);

// ((int *)((char *)p + N))[E] - words from N bytes into what an int *p points to: p[E + N/2] (a variant)
std::string word_offsets(std::string_view text);

// *(int *)buf = E; - a buffer's first word given: its first byte (a variant)
std::string byte_heads(std::string_view text);

// bx = E; bx = (bx << 1) + P; .. *bx .. - a word element through a pointer P held in the data: *(P + E) (P recorded
// in facts as a pointer to words)
std::string pointer_index(std::string_view text, FunctionFacts& facts);

// the long two word expressions are, the low and the high, or nothing (records the long in facts)
std::optional<std::string> pair_long(std::string_view lo, std::string_view hi, FunctionFacts& facts);

// ax = E; X = ax; X' = dx; ax = (ax | dx); if ((ax == 0)) - a long given and tested at once
std::string long_tests(std::string_view text, FunctionFacts& facts);

// if (C) { .. ax = E; } else { .. ax = L; dx = H; } X = ax; X' = dx; - a long both arms leave in dx:ax
std::string sunk_pairs(std::string_view text, FunctionFacts& facts);

// if (C) { .. r = E1; } else { .. r = E2; } S(r); - a statement both arms ended with: S in each arm
std::string sunk_tails(std::string_view text);

// t = A; v = F(B); .. (G(t) op v) - two calls an operator joins, the compiler's order (a variant)
std::optional<std::string> call_pairs(std::string_view text);

// *(C ? A : B) = E; - one store the compiler shares between two ways to its place
std::string lvalue_choices(std::string_view text);

// [the text with two next statements swapped] - a variant each
std::vector<std::string> adjacent_swaps(std::string_view text, std::size_t most = 4);

// ---- returns and gotos --------------------------------------------------------------------------------------

// if (C) { r = E1; } else { ..; r = E2; } return r;  ->  if (C) return E1; ..; return E2;
std::string branch_returns(std::string_view text);

// `return r;` at the end, every way to it giving r a value last: each of those a return of its value
std::string push_returns(std::string_view text);

// if (A) { ..; r = E1; } else if (B) { ..; r = E2; } .. return r;  -> each arm returns its value
std::string chain_returns(std::string_view text);

// the returns as a source with early exits writes them (a variant)
std::optional<std::string> flat_returns(std::string_view text);

// goto L; ... L: return x; - a jump to a plain return: the return in the goto's place
std::string goto_returns(std::string_view text);

// if ((f(..) != 0)) { .. return ..; } then the arm's end, the function's value read after: ax = 0;
std::string tested_zero(std::string_view text);

// goto L; with L: in another arm, a few plain statements to that arm's end after it: the statements in place
std::optional<std::string> goto_tails(std::string_view text, std::size_t most = 3);

// goto L; where what comes after it is L: itself - the goto only falls there: gone
std::string fall_gotos(std::string_view text);

// if (C) { A; T } else { T } - both arms ending in the same statements: if (C) { A } T
std::string common_tails(std::string_view text);

// if (C) { S; A } else { S; B } - both arms beginning with the same statement: S; if (C) { A } else { B }
std::string common_heads(std::string_view text);

// common_heads, then the ?: values it leaves and the registers and temporaries folded (a variant)
std::optional<std::string> heads_variant(std::string_view text);

// if (C1) { A; T } else if (C2) { B; T } else { return R; } - every way on ending in the same tail T
std::optional<std::string> chain_tails(std::string_view text);

// .. r = A; L: X = r; } .. } else { .. r = B; goto L; } - one store shared between the two arms of an if
std::string store_joins(std::string_view text);

// if (C) { r = A; L: V = r; S } else { r = B; goto L; } - the value of a ?: the compiler gives in each arm
std::string ternary_joins(std::string_view text);

// L: S; goto H; inside a loop H: while (..) { .. } - the code a goto in the loop jumps to (a variant)
std::optional<std::string> label_sinks(std::string_view text);

// a function ending in a loop that never ends, returning a variable from inside it: the return at the end (a variant)
std::optional<std::string> shared_exit(std::string_view text);

// ---- loops ---------------------------------------------------------------------------------------------------

// r = E0; for (;;) { v = r; .. r = E1; continue; .. } - a register carried round a loop into a variable
std::string loop_registers(std::string_view text);

// x = K; if/while (.. x ..) at once - the test of what x was just given: of K (a variant)
std::optional<std::string> const_tests(std::string_view text);

// in a loop: if (C) { S; continue; } else { R } (R leaving) last in its body - the leaving test first
std::optional<std::string> loop_guards(std::string_view text);

// for (;;) { if (C) break; S } - the loop's test first: while (!C) { S }
std::string first_breaks(std::string_view text);

// while (C) { if (D) { S } else { break; } } - the loop's own test the two
std::optional<std::string> loop_tests(std::string_view text);

// for (;;) { [x = E;] if (C1) continue; .. if (Cn) { leave } } - a loop of tests going round while one fails
std::string continue_loops(std::string_view text);

// for (;;) { [x = E;] if (C1) break; .. if (Cn) { break; } } - a loop of tests alone
std::string empty_loops(std::string_view text);

// for (;;) { x = E; if (C(x)) break; S } - the value tested as it is given: while (!C((x = E))) { S }
std::optional<std::string> assign_loops(std::string_view text);

// do { r = E; [p = p + 1;] M = r; } while (r != 0) - an empty loop on the assignment, while (M = E) ;
std::string store_loops(std::string_view text);

// for (;;) { if (C) break; ..} - the loop's test first: while (!C) { ..}
std::string test_first_loops(std::string_view text);

// a while loop whose body ends by stepping a variable - a for loop (a variant)
std::optional<std::string> for_loops(std::string_view text);

// L: for (;;) { for (;;) { S } break; } - an outer loop only restarting the inner one
std::string loop_collapse(std::string_view text);

// a continue last in the last statement of a loop's body: nothing
std::string tail_continues(std::string_view text);

// ---- switches ------------------------------------------------------------------------------------------------

// a switch whose cases are one run of values - LO..HI - the compiler tests as a range (reads facts.ranges)
std::string range_switches(std::string_view text, const FunctionFacts& facts);

// a switch arm ending with what the next arm is, word for word, and leaves: that tail gone
std::string fall_dups(std::string_view text);

// switch (x) { default: L: S .. case K: .. goto L; .. } - the default arm the code after the switch
std::string switch_follow(std::string_view text);

// switch (..) { .. case K: return V; } - the arms that only return written first (a variant)
std::optional<std::string> return_cases_first(std::string_view text);

// switch (x) { case K: .. f(ax) .. } - an arm reading the register the selector was compared in: x itself
std::string selector_reads(std::string_view text);

// [switch (x) { .. case K: return V; .. } with that case an if before it] (variants)
std::vector<std::string> case_guards(std::string_view text);

// switch (x) { default: S continue; case K: break; } last in a loop's body - one value passed over
std::string switch_ifs(std::string_view text);

// r = E; if ((r != K1)) { if ((r == K2)) { L: S } } else { goto L; } - two values to one arm
std::string two_case_switches(std::string_view text);

}  // namespace cb::exact::draft::shapes
