#include "cb/recompile/x360project.hpp"

#include <algorithm>
#include <format>
#include <set>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/log.hpp"
#include "cb/core/process.hpp"
#include "cb/core/text.hpp"

namespace cb::recompile::x360project {
namespace stdfs = std::filesystem;
namespace {

bool is_alnum(char c) { return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'); }
bool is_word(char c) { return is_alnum(c) || c == '_'; }

std::string output_text(const Bytes& bytes) { return text::sanitize_utf8(as_text(bytes)); }

std::string preferred(stdfs::path p) { return p.make_preferred().string(); }

// Copy a file, keeping its modification time (a rebuild goes by them).
void copy_keeping_time(const stdfs::path& from, const stdfs::path& to) {
    stdfs::copy_file(from, to, stdfs::copy_options::overwrite_existing);
    stdfs::last_write_time(to, stdfs::last_write_time(from));
}

void copy_tree_keeping_times(const stdfs::path& from, const stdfs::path& to) {
    fs::make_dirs(to);
    for (const auto& entry : stdfs::directory_iterator(from)) {
        const stdfs::path target = to / entry.path().filename();
        if (entry.is_directory()) copy_tree_keeping_times(entry.path(), target);
        else copy_keeping_time(entry.path(), target);
    }
}

bool same_content(const stdfs::path& a, const stdfs::path& b) {
    std::error_code ec;
    if (!stdfs::is_regular_file(b, ec) || stdfs::file_size(a) != stdfs::file_size(b, ec)) return false;
    return fs::read_file(a) == fs::read_file(b);
}

std::string module_info_source(std::uint32_t savegprlr_14) {
    std::string text = R"(// The recompiled title described to the runtime (modules.h), built with its own generated ppc_config.h.
// Written by CodeBreaker (cb/recompile/x360project.cpp).
#include "ppc_config.h"
#include "ppc_context.h"

#include "modules.h"

extern PPCFuncMapping PPCFuncMappings[];

extern const modules::RecompiledModule g_titleModule = {
    "default.xex",
    uint32_t(PPC_IMAGE_BASE),
    uint32_t(PPC_IMAGE_SIZE),
    uint32_t(PPC_CODE_BASE),
    uint32_t(PPC_CODE_SIZE),
    PPCFuncMappings,
    @ADDRESS@, // __savegprlr_14
};
)";
    return text::replace_first(text, "@ADDRESS@", std::format("0x{:08X}", savegprlr_14));
}

std::string cmake_source(std::string_view name, const stdfs::path& runtime_cmake) {
    return std::format(R"(# Written by CodeBreaker (cb/recompile/x360project.cpp).
cmake_minimum_required(VERSION 3.20)
project({0} CXX)
include("{1}")
x360_title({0} "${{CMAKE_CURRENT_SOURCE_DIR}}/src/ppc" "${{CMAKE_CURRENT_SOURCE_DIR}}/src/module_info.cpp")
)",
                       name, runtime_cmake.generic_string());
}

std::string build_script(std::string_view name, const stdfs::path& visual_studio) {
    const std::string vs = preferred(visual_studio);
    std::string out;
    const auto line = [&](const std::string& s) { out += s + "\r\n"; };
    line("@echo off");
    line(std::format("rem Builds {0} with clang-cl and Ninja (written by CodeBreaker). Run: build\\{0}.exe assets", name));
    line(std::format("call \"{}\\VC\\Auxiliary\\Build\\vcvars64.bat\" >nul 2>&1 || exit /b 1", vs));
    line(std::format("set \"LLVM={}\\VC\\Tools\\Llvm\\x64\\bin\"", vs));
    line("cd /d \"%~dp0\"");
    line("if not exist build\\CMakeCache.txt cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release \"-DCMAKE_C_COMPILER=%LLVM%\\clang-cl.exe\" "
         "\"-DCMAKE_CXX_COMPILER=%LLVM%\\clang-cl.exe\" || exit /b 1");
    line("if defined CB_BUILD_JOBS (cmake --build build -j %CB_BUILD_JOBS% || exit /b 1) else (cmake --build build || exit /b 1)");
    return out;
}

std::string play_script(std::string_view name) {
    return std::format("@echo off\r\nrem Plays {0} (written by CodeBreaker): keyboard or controller; the log is assets\\{0}.log\r\n"
                       "start \"\" \"%~dp0build\\{0}.exe\" \"%~dp0assets\"\r\n",
                       name);
}

std::size_t count_of(std::string_view haystack, std::string_view needle) {
    std::size_t n = 0;
    for (std::size_t at = haystack.find(needle); at != std::string_view::npos; at = haystack.find(needle, at + needle.size())) ++n;
    return n;
}

// The decimal number at `at` (digits only); nullopt when there is none.
std::optional<std::size_t> number_at(std::string_view text, std::size_t at, std::size_t* end = nullptr) {
    std::size_t j = at;
    while (j < text.size() && text[j] >= '0' && text[j] <= '9') ++j;
    if (j == at) return std::nullopt;
    if (end) *end = j;
    return text::parse_int<std::size_t>(text.substr(at, j - at));
}

}  // namespace

std::string default_name(const stdfs::path& xex_path) {
    std::string name = text::lower(fs::to_text(xex_path.stem()));
    for (char& c : name) {
        if (!is_alnum(c)) c = '_';
    }
    return name.empty() ? "title" : name;
}

std::string target_name(std::string_view name) {
    std::string out;
    bool pending = false;   // a separator was seen since the last kept character
    for (const char c : name) {
        if (is_alnum(c)) {
            if (pending && !out.empty()) out += '_';
            pending = false;
            out += c;
        } else {
            pending = true;
        }
    }
    return out.empty() || out == "default" ? "title" : out;
}

void sync_folder(const stdfs::path& src, const stdfs::path& dst) {
    fs::make_dirs(dst);
    std::set<std::string> have;
    for (const auto& entry : stdfs::directory_iterator(src)) {
        if (!entry.is_regular_file()) continue;
        have.insert(fs::to_text(entry.path().filename()));
        const stdfs::path target = dst / entry.path().filename();
        if (!same_content(entry.path(), target)) copy_keeping_time(entry.path(), target);
    }
    std::vector<stdfs::path> stale;
    for (const auto& entry : stdfs::directory_iterator(dst)) {
        if (!have.contains(fs::to_text(entry.path().filename()))) stale.push_back(entry.path());
    }
    for (const stdfs::path& p : stale) stdfs::remove_all(p);
}

ScaffoldInfo scaffold(const Recompiler& rc, const stdfs::path& xex_path, const stdfs::path& out_dir_in, const std::string& name_in,
                      const std::optional<stdfs::path>& title_files) {
    const std::string name = target_name(name_in.empty() ? default_name(xex_path) : name_in);
    const stdfs::path out_dir = stdfs::absolute(out_dir_in);   // XenonRecomp runs inside it and is given paths into it
    const stdfs::path recomp = out_dir / "src", game = out_dir / "assets", reports = out_dir / "reports";
    for (const stdfs::path& d : {recomp, game, reports}) fs::make_dirs(d);
    const stdfs::path basic = recomp / "default.xex";
    rc.write_basic(xex_path, basic);

    // switch tables: the finder's, with XenonAnalyse's for any bctr it leaves unresolved
    rc.ensure_built();
    const stdfs::path analysed = recomp / "switch_tables.analysed.toml";
    try {   // optional: the finder locates nearly all of them by itself
        process::run({.argv = {fs::to_text(rc.analyse), fs::to_text(basic), fs::to_text(analysed)}, .out = process::Capture::Discard, .err = process::Capture::Discard});
    } catch (const ToolError& e) {
        log::warn(std::format("XenonAnalyse did not run: {}; switch tables from the finder alone", e.what()));
    }
    std::error_code ec;
    const std::optional<std::string> with_analysed = stdfs::is_regular_file(analysed, ec) ? std::optional<std::string>(fs::read_text(analysed)) : std::nullopt;
    const Recompiler::SwitchTables tables = rc.write_switch_tables(recomp / "switch_tables.toml", basic, with_analysed);

    // the configuration, then XenonRecomp into a fresh folder, synced into recomp/ppc
    const stdfs::path cfg = recomp / (name + ".toml");
    fs::write_text(cfg, rc.make_config(basic, "ppc.new", "switch_tables.toml", "default.xex"));
    const stdfs::path fresh = recomp / "ppc.new";
    stdfs::remove_all(fresh, ec);
    fs::make_dirs(fresh);
    rc.ensure_built();
    const process::ProcessResult r = process::run({.argv = {fs::to_text(rc.recomp), fs::to_text(cfg), fs::to_text(rc.source / "XenonUtils" / "ppc_context.h")}, .cwd = recomp});
    const std::string log = output_text(r.out) + output_text(r.err);
    fs::write_text(reports / "recompile.log", log);
    if (r.exit_code != 0 || !stdfs::is_regular_file(fresh / "ppc_func_mapping.cpp", ec)) {
        throw ToolError("XenonRecomp", r.exit_code, log.size() > 3000 ? log.substr(log.size() - 3000) : log);
    }
    sync_folder(fresh, recomp / "ppc");
    stdfs::remove_all(fresh, ec);

    fs::write_text(recomp / "module_info.cpp", module_info_source(rc.savegprlr_14_address(basic)));
    fs::write_text(out_dir / "CMakeLists.txt", cmake_source(name, rc.runtime_cmake));
    fs::write_text(out_dir / ("Build-" + name + ".bat"), build_script(name, rc.visual_studio));
    fs::write_text(out_dir / ("Play-" + name + ".bat"), play_script(name));

    // the title folder: its files, and default.xex in basic form
    if (title_files) {
        for (const auto& entry : stdfs::directory_iterator(*title_files)) {
            if (text::iequals(fs::to_text(entry.path().filename()), "default.xex")) continue;
            const stdfs::path target = game / entry.path().filename();
            if (entry.is_directory()) copy_tree_keeping_times(entry.path(), target);
            else copy_keeping_time(entry.path(), target);
        }
    }
    copy_keeping_time(basic, game / "default.xex");

    const std::string mapping = fs::read_text(recomp / "ppc" / "ppc_func_mapping.cpp");
    return {name, out_dir, count_of(mapping, "{ 0x"), tables.found + tables.added, count_of(log, "unimplemented")};
}

BuildResult build(const stdfs::path& out_dir, std::string_view name, unsigned jobs) {
    const process::ProcessResult r = process::run({.argv = {"cmd", "/c", preferred(out_dir / ("Build-" + std::string(name) + ".bat"))},
                                                   .cwd = out_dir,
                                                   .env = {{"CB_BUILD_JOBS", std::to_string(jobs)}},
                                                   .err = process::Capture::Merge});
    const std::string all = output_text(r.out);
    std::vector<std::string> lines;
    for (const std::string_view line : text::lines(all)) lines.emplace_back(line);
    fs::make_dirs(out_dir / "reports");
    fs::write_text(out_dir / "reports" / "build.log", text::join(lines, "\n"));
    std::vector<std::string> errors;
    for (const std::string& line : lines) {
        if (line.find(" error") != std::string::npos || line.find("error:") != std::string::npos) errors.push_back(line);
    }
    const std::vector<std::string>& shown = errors.empty() ? lines : errors;
    BuildResult out;
    out.ok = r.exit_code == 0;
    out.tail.assign(shown.end() - static_cast<std::ptrdiff_t>(std::min<std::size_t>(12, shown.size())), shown.end());
    return out;
}

CheckResult read_log(std::string_view text) {
    CheckResult out;
    constexpr std::string_view kFrame = "GPU: frame ", kImport = "UNIMPLEMENTED IMPORT ", kFatal = "FATAL: ", kStall = "no frame presented for ";
    for (std::size_t at = text.find(kFrame); at != std::string_view::npos; at = text.find(kFrame, at + kFrame.size())) {
        if (const auto n = number_at(text, at + kFrame.size())) out.frames = std::max(out.frames, *n);
    }
    for (std::size_t at = text.find(kImport); at != std::string_view::npos; at = text.find(kImport, at + kImport.size())) {
        const std::size_t from = at + kImport.size();
        std::size_t end = from;
        while (end < text.size() && is_word(text[end])) ++end;
        if (end == from) continue;
        const std::string import(text.substr(from, end - from));
        if (std::size_t* calls = out.unimplemented.find(import)) ++*calls;
        else out.unimplemented.insert_or_assign(import, 1);
    }
    if (const std::size_t at = text.find(kFatal); at != std::string_view::npos) {
        const std::size_t from = at + kFatal.size(), eol = text.find('\n', from);
        std::string_view message = text.substr(from, eol == std::string_view::npos ? std::string_view::npos : eol - from);
        if (message.ends_with('\r')) message.remove_suffix(1);
        out.fatal = std::string(message);
    } else {
        // "no frame presented for <n> s after frame <m>"
        for (std::size_t s = text.find(kStall); s != std::string_view::npos && out.fatal.empty(); s = text.find(kStall, s + kStall.size())) {
            std::size_t end = 0;
            if (!number_at(text, s + kStall.size(), &end)) continue;
            constexpr std::string_view kAfter = " s after frame ";
            if (!text.substr(end).starts_with(kAfter)) continue;
            std::size_t last = 0;
            if (!number_at(text, end + kAfter.size(), &last)) continue;
            out.fatal = std::string(text.substr(s, last - s));
        }
    }
    return out;
}

CheckResult check(const stdfs::path& out_dir, std::string_view name, unsigned seconds) {
    const stdfs::path game = out_dir / "assets";
    const stdfs::path exe = out_dir / "build" / (std::string(name) + ".exe");
    const stdfs::path log_file = game / (std::string(name) + ".log");
    std::error_code ec;
    stdfs::remove(log_file, ec);
    const process::ProcessResult r = process::run({.argv = {fs::to_text(exe), fs::to_text(game)},
                                                   .cwd = game,
                                                   .env = {{"CB_HEADLESS", "1"}, {"CB_WATCHDOG", "20"}, {"CB_CONTINUE_UNIMPLEMENTED", "1"}},
                                                   .out = process::Capture::Discard,
                                                   .err = process::Capture::Discard,
                                                   .timeout = std::chrono::seconds(seconds)});
    const std::string text = stdfs::is_regular_file(log_file, ec) ? text::sanitize_utf8(fs::read_text(log_file)) : std::string();
    CheckResult out = read_log(text);
    if (!r.timed_out) out.exit = r.exit_code;
    out.log = log_file;
    return out;
}

}  // namespace cb::recompile::x360project
