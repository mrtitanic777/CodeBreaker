// formats/format_gen_check.hpp -- the proof of the generated readers (formats/gen: format_gen's output, docs/FORMAT_CODEGEN.md)
// against the engine they stand for: for a file, each description's engine tree and its generated reader's values()
// must say the same (the same error class, or the same numbers, offsets, sizes and counts at the same paths).
// format_check --gen runs it over a corpus.
#pragma once

#include <string>
#include <vector>

#include "core/bytes.hpp"
#include "formats/format_engine.hpp"

namespace cb::formats::engine {

struct GenComparison {
    std::string format;
    bool agree = false;
    std::string engine, generated;   // "truncated", "format error", or "read (N values)"
    std::string difference;          // the first path whose value differs (empty when they agree)
};

// Every generated reader on `data`, each against the engine reading the same description.
std::vector<GenComparison> compare_generated(const Registry& registry, ByteView data);

}  // namespace cb::formats::engine
