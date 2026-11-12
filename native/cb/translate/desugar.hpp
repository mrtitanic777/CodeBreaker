// cb/translate/desugar.hpp -- modern constructs lowered into the IR's core, for back ends that cannot say them directly.
//
// The IR keeps the source's intent (a for-each stays a ForEach, a switch expression a SwitchExpr, a pattern an InstanceOf
// with its binding) and a back end that has the construct writes it as it is: C++17's range-for. One that does not -- the
// C++03 back end, the C back end -- runs this pass first, and sees only core statements:
//
//   * ForEach over an array      { T[] a = xs; for (int i = 0; i < a.length; i++) { T x = a[i]; ... } }
//   * ForEach over an Iterable   for (Iterator<T> it = xs.iterator(); it.hasNext(); ) { T x = it.next(); ... }
//   * a SwitchExpr, where it is a whole statement's value (a local's initialiser, a return, the right side of an
//     assignment, an expression statement) or an operand that can be taken out before its statement without changing
//     what happens: a labelled switch statement assigning a temporary, each `yield v` as `tmp = v; break label;`
//   * a Switch on a String (statement or expression), every arm leaving it: an if/else chain on equals()
//   * `x instanceof T v` where x is a plain name or field: v declared before the statement, the test as
//     `x instanceof T && (v = (T) x) != null`
//
// What it cannot lower it leaves as it was, and the back end declines that method as before (an honest gap, never a
// guess). The tree is rewritten in place; the nodes it makes (temporaries, loops, tests) are made in the arena it is given,
// and the names of the temporaries are `j2x_<stem><n>`, numbered in the order they are made across one run.
#pragma once

#include <cstddef>
#include <span>

#include "cb/core/arena.hpp"
#include "cb/translate/ir.hpp"

namespace cb::translate::desugar {

enum class Form { ForEach, SwitchExpr, StringSwitch, Pattern };

// Lower every form but those in `keep` (the ones the back end writes itself) in every method of every type of `units` (Unit
// nodes), and of the lambdas and anonymous classes inside them. Returns how many constructs it lowered.
std::size_t run(Arena& arena, std::span<ir::Node* const> units, std::span<const Form> keep = {});

}  // namespace cb::translate::desugar
