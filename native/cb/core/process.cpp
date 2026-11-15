#include "cb/core/process.hpp"


#include <algorithm>
#include <format>
#include <thread>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/text.hpp"

namespace cb::process {

// ---- quoting ------------------------------------------------------------------------------------------

std::string quote_arg(std::string_view arg) {
    if (!arg.empty() && arg.find_first_of(" \t\n\v\"") == std::string_view::npos) return std::string(arg);
    std::string out = "\"";
    std::size_t backslashes = 0;
    for (char c : arg) {
        if (c == '\\') {
            ++backslashes;
            continue;
        }
        if (c == '"') {
            out.append(backslashes * 2 + 1, '\\');   // double the run before a quote, then escape the quote
            out.push_back('"');
        } else {
            out.append(backslashes, '\\');
            out.push_back(c);
        }
        backslashes = 0;
    }
    out.append(backslashes * 2, '\\');   // a trailing run must not escape the closing quote
    out.push_back('"');
    return out;
}

std::string command_line(const std::vector<std::string>& argv) {
    std::string out;
    for (const auto& a : argv) {
        if (!out.empty()) out.push_back(' ');
        out += quote_arg(a);
    }
    return out;
}
ProcessResult run_checked(const ProcessSpec& spec) {
    ProcessResult r = run(spec);
    std::string tool = spec.argv.empty() ? std::string() : spec.argv[0];
    std::string output(as_text(r.out));
    output += as_text(r.err);
    if (r.timed_out) throw TimeoutError(tool, output);
    if (r.exit_code != 0) throw ToolError(tool, r.exit_code, output);
    return r;
}
}  // namespace cb::process
