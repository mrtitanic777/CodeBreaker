// codebreaker/invert/known_differences.hpp -- C++ port of codebreaker.invert.known_differences.
//
// known_differences.py -- the divergence micro-pattern knowledge base (roadmap 2c).
//
// Every time a candidate MISS is diagnosed and fixed, the (divergence signature -> cause
// -> fix) mapping is recorded here so it is never re-derived by hand on another DLL. The
// refine loop consults `diagnose()` on a miss: it matches the localized divergence against
// known patterns and reports the likely cause + the fix that resolved it before, turning
// "stare at the bytes" into "apply the known rule."
//
// Entries are seeded from the fixes proven this project. `msc6dec_construct` names the
// recognizer that now handles it (so the KB doubles as a map from divergence to engine
// code); `example` is a real function it was proven on.
//
// Typing notes:
//   The Python's patterns are dicts with seven fixed keys: here the record `Entry`.  The key "auto"
//   is the C++ keyword, so the field is `auto_`.  diagnose() returns copies of the entries, where the
//   Python returns the dicts themselves (a caller that edits a returned dict edits the KB's: not done
//   anywhere).
#pragma once

#include "py.hpp"

namespace codebreaker::invert::known_differences {

struct Entry {
    py::str id;
    py::str match;
    py::str cause;
    py::str fix;
    py::str msc6dec_construct;
    bool auto_ = false;
    py::str example;
};

// Each pattern: a regex over a normalized divergence string (from refine.localize /
// the miss histogram), the human cause, the fix (engine construct or C transform), and
// whether msc6dec now handles it automatically or it's still a manual refine transform.
extern py::list<Entry> KB;

// Match a localized divergence against the KB; return the matching pattern(s).
py::list<Entry> diagnose(const std::optional<py::str>& divergence_text);

// Append a newly-solved divergence pattern (id, match, cause, fix, ...).
Entry record(const Entry& pattern);

// The module's `if __name__ == "__main__":` block (it has no function of that name in the Python): print the
// KB, each entry's id, cause and fix.
void main();

}  // namespace codebreaker::invert::known_differences
