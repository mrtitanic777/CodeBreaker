#include "core/process.hpp"


#include <algorithm>
#include <cstdio>
#include <filesystem>
#include <format>
#include <thread>

#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/text.hpp"

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

namespace {
InProcess& in_process_hook() {
    static InProcess hook;
    return hook;
}
}  // namespace

void set_in_process(InProcess hook) { in_process_hook() = std::move(hook); }

std::optional<ProcessResult> run_in_process(const ProcessSpec& spec) {
    const InProcess& hook = in_process_hook();
    if (!hook || spec.argv.empty()) return std::nullopt;
    std::optional<ProcessResult> r = hook(spec);
    if (!r) return r;
    // the streams where a started program's would have gone
    if (spec.err.mode == Capture::Merge) {   // stderr into stdout
        r->out.insert(r->out.end(), r->err.begin(), r->err.end());
        r->err.clear();
    }
    auto deliver = [](const Capture& where, Bytes& text, std::FILE* console) {
        switch (where.mode) {
            case Capture::Collect:
            case Capture::Merge: break;
            case Capture::Inherit:
                if (!text.empty()) std::fwrite(text.data(), 1, text.size(), console);
                std::fflush(console);
                text.clear();
                break;
            case Capture::Discard: text.clear(); break;
            case Capture::File: {
                Bytes all = where.append && std::filesystem::exists(where.file) ? fs::read_file(where.file) : Bytes{};
                all.insert(all.end(), text.begin(), text.end());
                fs::write_file(where.file, all);
                text.clear();
                break;
            }
        }
    };
    deliver(spec.out, r->out, stdout);
    deliver(spec.err, r->err, stderr);
    return r;
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
