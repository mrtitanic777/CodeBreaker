// cb/analyze/function_evidence.hpp -- the facts that could name each function of a module: its calls, its strings, its callers, its shape.
//
//     const std::vector<symbols::FunctionEvidence> all = function_evidence::of_module(path, environment);
//
// One entry per function the analyses find, in address order, addressed as the namer addresses them (a 32-bit module's virtual address; a 16-bit
// module's (selector << 16) + offset, which is what decompile::describe::address lays out). The facts are read from the module as the `analyze`
// command reads it (call graph, string table); nothing is decompiled.
#pragma once

#include <filesystem>
#include <vector>

#include "cb/analyze/report.hpp"
#include "cb/symbols/evidence.hpp"

namespace cb::analyze::function_evidence {

// UnsupportedError for a module that is neither a 32-bit nor a 16-bit module of the analyses.
std::vector<symbols::FunctionEvidence> of_module(const std::filesystem::path& module, const report::Environment& environment);

}  // namespace cb::analyze::function_evidence
