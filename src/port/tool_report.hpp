// port/tool_report.hpp -- PORT.md: every change the port made and why, what the compiler still says, what could not be ported.
//
// The report is a pure function of what the run found (`ReportInput`); `write_report` puts it in the port's folder.
#pragma once

#include <filesystem>
#include <map>
#include <optional>
#include <string>
#include <vector>

#include "core/ordered_map.hpp"
#include "port/pre_ansi_types.hpp"
#include "port/tool_asm.hpp"
#include "port/tool_compilers.hpp"

namespace cb::port::tool {

struct ReportInput {
    std::filesystem::path source;                         // the tree that was ported (its last folder names the report)
    std::string compiler;                                 // gcc, clang, msvc
    std::optional<std::filesystem::path> compiler_path;
    OrderedMap<std::string, std::string> left_out;        // file: why (the old runtime's)
    std::map<std::string, Role> roles;                    // the assembler modules
    pre_ansi::Notes notes;                                // change -> where
    std::vector<std::string> assembler_notes;
    std::vector<std::string> sanitizer_notes;
    OrderedMap<std::string, std::string> behaviour;       // what may differ, from the runtime's profile: topic -> text
    int int_bits = 16;                                    // an int in the original
    bool char_signed = false;
    std::vector<Diagnostic> diagnostics;                  // what the compiler still said
    int rounds = 0;
    bool built = false;
    std::string build_log;
};

// The text of PORT.md.
std::string render_report(const ReportInput& input);

// PORT.md into `out`.
void write_report(const std::filesystem::path& out, const ReportInput& input);

}  // namespace cb::port::tool
