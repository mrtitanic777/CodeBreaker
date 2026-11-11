// codebreaker/translate/desugar.hpp -- typed C++ port of codebreaker.translate.desugar.
//
// desugar.py -- modern constructs lowered into the IR's core, for back ends that cannot say them directly.
//
// The IR keeps the source's intent (a for-each stays a ForEach, a switch expression a SwitchExpr, a pattern
// an InstanceOf with its binding) and a back end that has the construct writes it as it is: C++17's
// range-for. One that does not -- the C++03 back end, the C back end -- runs this pass first, and sees
// only core statements:
//
//   * for-each over an array      { T[] a = xs; for (int i = 0; i < a.length; i++) { T x = a[i]; ... } }
//   * for-each over an Iterable   for (Iterator<T> it = xs.iterator(); it.hasNext(); ) { T x = it.next(); ... }
//   * a switch expression, where it is a whole statement's value (a local's initialiser, a return, the
//     right side of an assignment, an expression statement): a labelled switch statement assigning a
//     temporary, each `yield v` as `tmp = v; break label;`
//   * a switch on a String (statement or expression), every arm leaving it: an if/else chain on equals()
//   * `x instanceof T v` where x is a plain name or field: v declared before the statement, the test as
//     `x instanceof T && (v = (T) x) != null`
//
// What it cannot lower it leaves as it was, and the back end declines that method as before (an honest gap,
// never a guess). `run(units, keep)` rewrites in place; `keep` names the forms the back end writes itself.
//
// Typing notes:
//   * `units` holds Unit nodes (a list of NodeP); `keep` is the Python's tuple of form names (a list of str).
//   * The Python's class _Desugar is internal to desugar.cpp. The statements it builds are the typed nodes of
//     ir.hpp; its helpers that read or rewrite a node's slots by name (`for f in s.fields: getattr/setattr`) walk
//     the node's typed slots in the order of its `fields`, as rewrite.hpp's replace() does.
//   * A statement body that holds a bare list of statements (the decompiler's form, NodeOrList) is not something
//     the Java and C# front ends make; where the Python would fail on one (AttributeError), so does this.
//   * Python ints are int64_t.
#pragma once

#include <cstdint>

#include "codebreaker/translate/ir.hpp"
#include "py.hpp"

namespace codebreaker::translate::desugar {

namespace ir = codebreaker::translate::ir;

extern const py::list<py::str> FORMS;

// Lower the modern forms in every method of `units` (in place). -> how many were lowered.
int64_t run(const py::list<ir::NodeP>& units, const py::list<py::str>& keep = py::list<py::str>());

}  // namespace codebreaker::translate::desugar
