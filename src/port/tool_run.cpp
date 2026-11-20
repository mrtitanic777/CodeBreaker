// cb-unity: alone (windows.h is included here)
#include "port/tool_run.hpp"

#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <format>
#include <fstream>
#include <iostream>
#include <set>

#if defined(_WIN32)
#include <windows.h>
#include <psapi.h>   // NOLINT: after windows.h
#endif

#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/parallel.hpp"
#include "core/text.hpp"
#include "port/tool_build.hpp"


namespace cb::port::tool {

namespace stdfs = std::filesystem;

// ---- the command line ------------------------------------------------------------------------------------------------------

std::string usage() {
    return "usage: codebreaker port SOURCE_DIR OUT_DIR [--compiler gcc|clang|msvc] [--runtime NAME] [--cc PATH]\n"
           "                                   [--cases CASES.json] [--asm16 ASSEMBLER] [--types TYPES.json]\n";
}

Options parse_options(const std::vector<std::string>& args) {
    if (args.size() < 2) throw UsageError("port needs SOURCE_DIR and OUT_DIR\n" + usage());
    Options o;
    o.source = fs::from_text(args[0]);
    o.out = stdfs::absolute(fs::from_text(args[1]));   // (the compilers run in OUT_DIR: its own path whole)
    for (std::size_t i = 2; i < args.size(); ++i) {
        const std::string& a = args[i];
        const bool known = a == "--compiler" || a == "--runtime" || a == "--cc" || a == "--cases" || a == "--asm16" || a == "--types";
        if (!known) throw UsageError("port: unknown option " + a + "\n" + usage());
        if (i + 1 >= args.size()) throw UsageError("port: " + a + " needs a value");
        const std::string& v = args[++i];
        if (a == "--compiler") o.compiler = v;
        else if (a == "--runtime") o.runtime = v;
        else if (a == "--cc") o.cc = v;
        else if (a == "--cases") o.cases = fs::from_text(v);
        else if (a == "--asm16") o.asm16 = fs::from_text(v);
        else o.types = fs::from_text(v);
    }
    const stdfs::path src = stdfs::absolute(o.source).lexically_normal();
    const stdfs::path out = o.out.lexically_normal();
    const std::string s = text::lower(fs::to_text(src)), t = text::lower(fs::to_text(out));
    if (t == s || t.starts_with(s.ends_with("/") ? s : s + "/"))
        throw UsageError("the port is written beside the source, never into it: give another OUT_DIR");
    return o;
}

void write_platform_layer(const stdfs::path& out, const stdfs::path& platform_dir, const RuntimeWays& ways) {
    stdfs::create_directories(out);
    for (const char* name : {"dosrt.h", "dosrt.c"}) fs::write_file(out / name, fs::read_file(platform_dir / name));
    const bool console = ways.console_text.value_or(ways.text_mode);
    fs::write_text(out / "dosrt_config.h",
                   std::format("/* dosrt_config.h -- the old runtime's ways (made by CodeBreaker's port from the profile {}) */\n"
                               "#define DOSRT_TEXT_MODE {}\n#define DOSRT_CONSOLE_TEXT {}\n#define DOSRT_EXIT_STATUS {}\n",
                               ways.profile_name, ways.text_mode ? 1 : 0, console ? 1 : 0, ways.exit_status ? 1 : 0));
}

// ---- the check rounds ------------------------------------------------------------------------------------------------------

CheckResult run_check_rounds(cache::Cache& cache, const std::vector<CheckedCompiler>& compilers, const stdfs::path& out, const CheckHooks& hooks,
                             int jobs) {
    CheckResult result;
    constexpr int kMaxRounds = 8;
    for (std::size_t k = 0; k < compilers.size(); ++k) {
        const CheckedCompiler& checker = compilers[k];
        const int start = result.rounds;
        Files seen_text;
        std::map<std::string, std::vector<Diagnostic>> seen_diags;   // what the compiler said of each file, as the files stand
        int same = 0;
        std::optional<std::vector<std::tuple<std::string, std::size_t, std::string>>> last_said;
        auto round_began = std::chrono::steady_clock::now();
        while (true) {
            result.texts = hooks.write();
            std::vector<Diagnostic> diags;
            std::vector<std::string> c_files;
            for (const auto& [n, t] : result.texts)
                if (n.ends_with(".c")) c_files.push_back(n);
            std::sort(c_files.begin(), c_files.end());
            // the files whose text, or a header's they include, changed: the others said it already
            const std::vector<std::string> todo = changed_files(result.texts, seen_text, c_files);
            struct Said {
                std::vector<Diagnostic> found;
                bool ran = false;
            };
            parallel::ThreadPool pool(static_cast<std::size_t>(std::max(1, std::min<int>(jobs, static_cast<int>(todo.empty() ? 1 : todo.size())))));
            const std::vector<Said> said = parallel::parallel_map(pool, todo, [&](const std::string& n) {
                Said s;
                s.found = check_file(cache, *checker.spec, checker.cc, out, n, result.texts, s.ran);
                return s;
            });
            for (std::size_t i = 0; i < todo.size(); ++i) seen_diags[todo[i]] = said[i].found;
            // in the files' order, as one compiler would say them
            for (const std::string& n : c_files) {
                const auto it = seen_diags.find(n);
                if (it != seen_diags.end()) diags.insert(diags.end(), it->second.begin(), it->second.end());
            }
            seen_text = result.texts;
            const std::size_t errors = static_cast<std::size_t>(std::count_if(diags.begin(), diags.end(), [](const Diagnostic& d) { return d.kind == "error"; }));
            const auto seconds = std::chrono::duration<double>(std::chrono::steady_clock::now() - round_began).count();
            if (hooks.say) {
                hooks.say(std::format("port: round {} ({}): {} of {} files checked, {} errors, {} warnings, {:.0f}s", result.rounds + 1, checker.name,
                                      todo.size(), c_files.size(), errors, diags.size() - errors, seconds));
            }
            round_began = std::chrono::steady_clock::now();
            if (k != 0 && errors == 0) {   // the second compiler: its errors only
                result.diagnostics = std::move(diags);
                break;
            }
            if (k != 0) {
                std::erase_if(diags, [](const Diagnostic& d) { return d.kind != "error"; });
            }
            ++result.rounds;
            std::vector<std::tuple<std::string, std::size_t, std::string>> now;
            for (const Diagnostic& d : diags) now.emplace_back(d.file, d.line, d.text);
            std::sort(now.begin(), now.end());
            // a round that left the compiler saying all it said before, twice: the rules have nothing more
            same = (last_said && *last_said == now) ? same + 1 : 0;
            last_said = std::move(now);
            result.diagnostics = diags;
            if (same >= 2) break;
            if (result.rounds - start > kMaxRounds || !hooks.mend(result.texts, diags)) break;
        }
    }
    return result;
}

// ---- the build script ------------------------------------------------------------------------------------------------------

std::string build_script(const std::string& compiler_name, const Compiler& spec, const std::string& cc_name, const std::string& exe_name,
                         const std::vector<std::string>& sources) {
    Values v;
    v.cc = cc_name.empty() ? compiler_name : cc_name;
    v.inc = ".";
    v.out = exe_name;
    v.srcs = sources;
    return std::format("#!/bin/sh\n# the port built by {} (made by CodeBreaker's port)\ncd \"$(dirname \"$0\")\"\n{}\n", compiler_name,
                       text::join(expand(spec.build, v), " "));
}

// ---- the watch on the port's own memory ---------------------------------------------------------------------------------------

std::optional<std::uint64_t> MemoryGuard::committed_bytes() {
#if defined(_WIN32)
    PROCESS_MEMORY_COUNTERS_EX counters{};
    counters.cb = sizeof(counters);
    if (!GetProcessMemoryInfo(GetCurrentProcess(), reinterpret_cast<PROCESS_MEMORY_COUNTERS*>(&counters), sizeof(counters))) return std::nullopt;
    return static_cast<std::uint64_t>(counters.PrivateUsage);
#else
    // Linux: the resident set from /proc/self/statm (pages); elsewhere the system does not say.
    std::ifstream statm("/proc/self/statm");
    std::uint64_t size_pages = 0, resident_pages = 0;
    if (!(statm >> size_pages >> resident_pages)) return std::nullopt;
    return resident_pages * 4096u;
#endif
}

MemoryGuard::MemoryGuard(stdfs::path out, double limit_gb, std::function<std::optional<std::uint64_t>()> used)
    : out_(std::move(out)), limit_bytes_(static_cast<std::uint64_t>(limit_gb * 1024.0 * 1024.0 * 1024.0)), used_(used ? std::move(used) : committed_bytes) {
    if (!used_().has_value()) return;   // the system does not say: nothing to watch
    thread_ = std::thread([this] { watch(); });
}

MemoryGuard::~MemoryGuard() {
    stop_ = true;
    if (thread_.joinable()) thread_.join();
}

bool MemoryGuard::over_limit() const { return used_().value_or(0) > limit_bytes_; }

void MemoryGuard::watch() {
    while (!stop_) {
        if (over_limit()) {
            const double gb = static_cast<double>(used_().value_or(0)) / (1024.0 * 1024.0 * 1024.0);
            const double limit = static_cast<double>(limit_bytes_) / (1024.0 * 1024.0 * 1024.0);
            const std::string said = std::format("the port stopped itself: {:.1f} GB used, the ceiling {:.1f} GB (settings.json port.max_memory_gb)\n", gb, limit);
            try {
                stdfs::create_directories(out_);
                fs::write_text(out_ / "port-memory.txt", said);
            } catch (const Error&) {
            }
            std::cerr << "port: stopped - " << said << "(" << fs::to_text(out_ / "port-memory.txt") << ")\n";
            std::cerr.flush();
            std::_Exit(3);
        }
        for (int i = 0; i < 10 && !stop_; ++i) std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

}  // namespace cb::port::tool
