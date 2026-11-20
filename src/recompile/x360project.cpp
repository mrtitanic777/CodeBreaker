#include "recompile/x360project.hpp"

#include <algorithm>
#include <format>
#include <fstream>
#include <set>

#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/log.hpp"
#include "core/process.hpp"
#include "core/text.hpp"

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
    cb::fs::set_last_write_time(to, cb::fs::last_write_time(from));
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

// One module's description for the runtime. The title's carries the list of its extra modules (`extras`: the keys of the
// modules `translate_modules` made), and every extra module its file name and the path it is found at (relative to the title
// folder, '/' separated); an extra module's library is built with its own ppc_config.h and its fixed symbols carry `suffix`.
std::string module_info_source(std::uint32_t savegprlr_14, std::string_view file_name, std::string_view variable, std::string_view suffix,
                               std::string_view path, const std::vector<std::string>& extras) {
    std::string text = R"(// @WHAT@ described to the runtime (modules.h), built with its own generated ppc_config.h.
// Written by CodeBreaker (cb/recompile/x360project.cpp).
#include "ppc_config.h"
#include "ppc_context.h"

#include "modules.h"

extern PPCFuncMapping PPCFuncMappings@SUFFIX@[];
@EXTERN@
extern const modules::RecompiledModule @VARIABLE@ = {
    "@FILE@",
    uint32_t(PPC_IMAGE_BASE),
    uint32_t(PPC_IMAGE_SIZE),
    uint32_t(PPC_CODE_BASE),
    uint32_t(PPC_CODE_SIZE),
    PPCFuncMappings@SUFFIX@,
    @ADDRESS@, // __savegprlr_14@TAIL@
};
)";
    std::string externs, list;
    for (const std::string& key : extras) {
        externs += std::format("extern const modules::RecompiledModule g_module_{};\n", key);
        list += std::format("&g_module_{}, ", key);
    }
    if (!extras.empty()) externs = std::format("{}static const modules::RecompiledModule* const kExtraModules[] = {{ {}nullptr }};\n", externs, list);
    text = text::replace_first(text, "@EXTERN@", externs);
    // the path and the list of extra modules follow the address only when there is one (a title without them is as it was)
    std::string tail;
    if (!path.empty() || !extras.empty())
        tail = std::format("\n    {},\n    {},", path.empty() ? "nullptr" : std::format("\"{}\"", path), extras.empty() ? "nullptr" : "kExtraModules");
    text = text::replace_first(text, "@TAIL@", tail);
    text = text::replace_first(text, "@WHAT@", variable == "g_titleModule" ? "The recompiled title" : "A recompiled module of the title");
    text = text::replace_first(text, "@FILE@", file_name);
    text = text::replace_first(text, "@VARIABLE@", variable);
    text = text::replace_all(text, "@SUFFIX@", suffix);
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


struct Translated {
    std::size_t functions = 0, switches = 0, unimplemented = 0;
    std::string log;
};

// XenonRecomp over `basic` (a module in basic form) inside `work` (the configuration `<name>.toml`, the switch tables, and
// the C++ synced into work/ppc). `suffix` (empty for the title) goes on the module's fixed symbols and function names, so that
// several modules link into one program. The tool's output goes to `log_file`. ToolError when XenonRecomp fails or leaves no function
// mapping.
Translated translate(const Recompiler& rc, const stdfs::path& basic, const stdfs::path& work, const std::string& name, const std::string& suffix,
                     const stdfs::path& log_file) {
    // switch tables: the finder's, with XenonAnalyse's for any bctr it leaves unresolved
    const stdfs::path analysed = work / "switch_tables.analysed.toml";
    // optional: the finder locates nearly all of them by itself
    if (const xenon_engine::Run a = rc.analyse({fs::to_text(stdfs::absolute(basic)), fs::to_text(stdfs::absolute(analysed))}); a.code != 0) {
        log::warn(std::format("XenonAnalyse failed (exit {}); switch tables from the finder alone", a.code));
    }
    std::error_code ec;
    const std::optional<std::string> with_analysed = stdfs::is_regular_file(analysed, ec) ? std::optional<std::string>(fs::read_text(analysed)) : std::nullopt;
    const Recompiler::SwitchTables tables = rc.write_switch_tables(work / "switch_tables.toml", basic, with_analysed);

    // the configuration, then XenonRecomp into a fresh folder, synced into work/ppc
    const stdfs::path cfg = work / (name + ".toml");
    std::string config = rc.make_config(basic, "ppc.new", "switch_tables.toml", fs::to_text(basic.filename()));
    if (!suffix.empty()) {
        const std::string head = "[main]\n";
        const std::size_t at = config.find(head);
        if (at != std::string::npos) config.insert(at + head.size(), std::format("symbol_suffix = \"{0}\"\nsuffix_function_names = true\n", suffix));
    }
    fs::write_text(cfg, config);
    const stdfs::path fresh = work / "ppc.new";
    stdfs::remove_all(fresh, ec);
    fs::make_dirs(fresh);
    const xenon_engine::Run r = rc.recomp({fs::to_text(stdfs::absolute(cfg)), fs::to_text(rc.source / "XenonUtils" / "ppc_context.h")});
    Translated out;
    out.log = r.log;
    fs::write_text(log_file, out.log);   // also when it failed: the log says why
    if (r.code != 0 || !stdfs::is_regular_file(fresh / "ppc_func_mapping.cpp", ec)) {
        throw ToolError("XenonRecomp", static_cast<std::uint32_t>(r.code), out.log.size() > 3000 ? out.log.substr(out.log.size() - 3000) : out.log);
    }
    sync_folder(fresh, work / "ppc");
    stdfs::remove_all(fresh, ec);
    const std::string mapping = fs::read_text(work / "ppc" / "ppc_func_mapping.cpp");
    out.functions = count_of(mapping, "{ 0x");
    out.switches = tables.found + tables.added;
    out.unimplemented = count_of(out.log, "unimplemented");
    return out;
}

bool is_xex_file(const stdfs::path& file) {
    std::ifstream in(file, std::ios::binary);
    char magic[4] = {};
    return in.read(magic, 4) && std::string_view(magic, 4) == "XEX2";
}

// A name for a module's target symbols and folder: its path under the title folder as letters, digits and underscores.
std::string module_key(const std::string& relative) {
    std::string key = text::lower(relative);
    for (char& c : key) {
        if (!is_alnum(c)) c = '_';
    }
    return key;
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

namespace {

// Every module the title ships besides default.xex (an XEX file in its folder, any depth) that is a PowerPC program, translated
// into recomp/modules/<key>/ with its own configuration and symbol suffix; the module's image in `game` is replaced by its basic
// form (what the runtime loads). Returns the keys; the module_info of each is written here, the title's by the caller.
std::vector<std::string> translate_modules(const Recompiler& rc, const stdfs::path& recomp, const stdfs::path& game, const stdfs::path& reports,
                                           std::string& report) {
    std::vector<std::string> keys;
    if (!rc.has_native_code) return keys;
    std::vector<stdfs::path> files;
    for (const auto& entry : stdfs::recursive_directory_iterator(game)) {
        if (!entry.is_regular_file() || !is_xex_file(entry.path())) continue;
        const std::string relative = fs::to_text(stdfs::relative(entry.path(), game).generic_string());
        if (text::iequals(relative, "default.xex")) continue;
        files.push_back(entry.path());
    }
    std::sort(files.begin(), files.end());
    const stdfs::path modules = recomp / "modules";
    std::error_code ec;
    stdfs::remove_all(modules, ec);   // a module the title no longer ships leaves no stale library behind
    for (const stdfs::path& file : files) {
        const std::string relative = fs::to_text(stdfs::relative(file, game).generic_string());
        const std::string key = module_key(relative);
        const stdfs::path work = modules / key;
        fs::make_dirs(work);
        const stdfs::path basic = work / "image.xex";
        try {
            rc.write_basic(file, basic);
            if (!rc.has_native_code(basic)) {
                stdfs::remove_all(work, ec);
                report += std::format("module {}: not a PowerPC program (data or managed code), left as it is\n", relative);
                continue;
            }
            const Translated t = translate(rc, basic, work, key, "_" + key, reports / ("recompile." + key + ".log"));
            fs::write_text(work / "module_info.cpp",
                           module_info_source(rc.savegprlr_14_address(basic), fs::to_text(file.filename()), "g_module_" + key, "_" + key, relative, {}));
            copy_keeping_time(basic, file);
            keys.push_back(key);
            report += std::format("module {}: {} functions, {} switch tables\n", relative, t.functions, t.switches);
        } catch (const Error& e) {
            stdfs::remove_all(work, ec);
            report += std::format("module {}: not translated ({})\n", relative, std::string(e.what()).substr(0, 300));
            log::warn(std::format("module {} was not translated: {}", relative, e.what()));
        }
    }
    return keys;
}

}  // namespace

ScaffoldInfo scaffold(const Recompiler& rc, const stdfs::path& xex_path, const stdfs::path& out_dir_in, const std::string& name_in,
                      const std::optional<stdfs::path>& title_files) {
    const std::string name = target_name(name_in.empty() ? default_name(xex_path) : name_in);
    const stdfs::path out_dir = stdfs::absolute(out_dir_in);   // XenonRecomp is given paths into it
    const stdfs::path recomp = out_dir / "src", game = out_dir / "assets", reports = out_dir / "reports";
    for (const stdfs::path& d : {recomp, game, reports}) fs::make_dirs(d);
    const stdfs::path basic = recomp / "default.xex";
    rc.write_basic(xex_path, basic);

    const Translated main = translate(rc, basic, recomp, name, "", reports / "recompile.log");

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

    // the other modules the title ships (HostLoader.dll, tier0_360.dll, ...): each one translated, linked and named
    std::string report;
    const std::vector<std::string> extras = translate_modules(rc, recomp, game, reports, report);
    if (!report.empty()) fs::write_text(reports / "modules.txt", report);
    fs::write_text(recomp / "module_info.cpp", module_info_source(rc.savegprlr_14_address(basic), "default.xex", "g_titleModule", "", "", extras));
    return {name, out_dir, main.functions, main.switches, main.unimplemented, extras.size()};
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
