// source/match_bytes.hpp -- the bytes behind a `match` verdict, for a function that compiled but did not come out identical: what the period compiler made, what the
// original holds, and which bytes of each a linker or loader fills in. `match --bytes FILE` keeps them, so a miss can be read byte by byte without compiling again.
// No operating-system API (match_bytes_test.cpp).
#pragma once

#include <map>
#include <string>

#include "core/bytes.hpp"

namespace cb::source {

struct MatchBytes {
    Bytes built, built_mask, original, original_mask;   // a mask byte is non-zero where the linker or loader fills the byte in
};

// {"function": {"built": hex, "built_mask": hex, "original": hex, "original_mask": hex}}
std::string match_bytes_json(const std::map<std::string, MatchBytes>& functions);

}  // namespace cb::source
