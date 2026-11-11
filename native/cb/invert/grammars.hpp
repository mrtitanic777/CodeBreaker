// cb/invert/grammars.hpp -- enumerative expression synthesis: candidate C texts for a function's shape.
//
// The step that removes the human from the loop. Instead of a person reading a gap, analyzing the idiom and
// hand-writing a rule, a grammar enumerates candidate C for the function's shape and the compiler oracle proves
// each byte-exact. MSC 6.0's code generation is deterministic and its idiom vocabulary is finite, so a bounded
// search terminates on the exact source -- no analysis required. Whatever verifies is, by construction, a correct
// rule; it can be banked into the templates automatically.
//
// A grammar encodes the finite parameter space of one MSC-6 idiom class; its candidates are the cross-product of its
// holes, and the oracle rejects all but the byte-exact instantiation. Each grammar returns every candidate, in the
// order of its holes (the order the search tries them).
#pragma once

#include <cstddef>
#include <span>
#include <string>
#include <string_view>
#include <vector>

namespace cb::invert::grammars {

// What a caller knows of the function's shape. Only the thunk grammar reads it.
struct Shape {
    std::size_t nargs = 3;   // how many arguments the function forwards
};

// Pure forwarder: `return SIB(args)`. Holes: calling convention (pascal or plain), argument order (as declared or
// reversed). Four candidates; with fewer than two arguments the two orders read the same text.
std::vector<std::string> thunk(const Shape& shape = {});

// A sibling fills a stack buffer, the function reads it back and stores to *out. Holes: buffer size, read expression,
// return flow (nested `if` or early return), out-pointer width. One grammar covers both the 2-byte WORD and the
// 4-byte long hand-fixes seen in OLECLI (segment 11, offsets 0x7f4 and 0x830): 8 + 4 candidates.
std::vector<std::string> bufread(const Shape& shape = {});

struct Grammar {
    std::string_view name;
    std::vector<std::string> (*generate)(const Shape&);
};

// Every grammar, in the order the search runs them: thunk, bufread.
std::span<const Grammar> all();

}  // namespace cb::invert::grammars
