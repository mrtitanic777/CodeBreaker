// cb/recompile/xenon.cpp -- see xenon.hpp.

#include "cb/recompile/xenon.hpp"

#include <algorithm>
#include <array>
#include <format>
#include <set>
#include <thread>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/log.hpp"
#include "cb/core/process.hpp"
#include "cb/core/text.hpp"

namespace cb::recompile::xenon {
namespace {

// A program's output as text: UTF-8 (what the tools speak), line ends as \n.
std::string output_text(const Bytes& raw) {
    return text::replace_all(text::sanitize_utf8(as_text(raw)), "\r\n", "\n");
}

// The last `n` characters (the part of a long output that says why it failed).
std::string tail(std::string_view s, std::size_t n) { return std::string(s.size() > n ? s.substr(s.size() - n) : s); }

struct Run {
    std::uint32_t code = 0;
    std::string out, err;
};

Run run_program(const Config& config, std::vector<std::string> argv, const std::optional<stdfs::path>& cwd, const Environment* env,
                std::chrono::minutes timeout) {
    process::ProcessSpec spec;
    spec.argv = std::move(argv);
    spec.cwd = cwd;
    if (env) spec.env = *env;
    spec.timeout = timeout;
    const process::ProcessResult r = config.runner ? config.runner(spec) : process::run(spec);
    if (r.timed_out) throw TimeoutError(spec.argv.front(), output_text(r.out) + output_text(r.err));
    return {r.exit_code, output_text(r.out), output_text(r.err)};
}

std::string slashes(const stdfs::path& p) { return fs::to_text(p); }   // the assembler and objdump want '/'

bool is_hidden(const stdfs::path& p) {
    const std::string name = p.filename().string();
    return !name.empty() && name[0] == '.';
}

// The newest modification time of any file under the sources (names starting with a dot are left out, as a glob
// leaves them out).
stdfs::file_time_type newest_source(const stdfs::path& source) {
    stdfs::file_time_type newest = stdfs::file_time_type::min();
    auto note = [&](const stdfs::path& p) {
        std::error_code ec;
        const auto t = stdfs::last_write_time(p, ec);
        if (!ec) newest = std::max(newest, t);
    };
    for (const char* sub : {"XenonRecomp", "XenonUtils", "XenonAnalyse", "CMakeLists.txt"}) {
        const stdfs::path path = source / sub;
        std::error_code ec;
        if (stdfs::is_regular_file(path, ec)) {
            note(path);
        } else if (stdfs::is_directory(path, ec)) {
            for (stdfs::recursive_directory_iterator it(path, stdfs::directory_options::skip_permission_denied, ec), end; it != end; it.increment(ec)) {
                if (is_hidden(it->path())) {
                    if (it->is_directory(ec)) it.disable_recursion_pending();
                    continue;
                }
                if (it->is_regular_file(ec)) note(it->path());
            }
        }
    }
    return newest;
}

bool same_content(const stdfs::path& a, const stdfs::path& b) {
    std::error_code ec;
    if (stdfs::file_size(a, ec) != stdfs::file_size(b, ec) || ec) return false;
    return fs::read_file(a) == fs::read_file(b);
}

// RAII removal of a file this module made (the configuration copy), including on an exception.
class RemoveOnExit {
public:
    explicit RemoveOnExit(stdfs::path p) : path_(std::move(p)) {}
    RemoveOnExit(const RemoveOnExit&) = delete;
    RemoveOnExit& operator=(const RemoveOnExit&) = delete;
    ~RemoveOnExit() {
        std::error_code ec;
        stdfs::remove(path_, ec);
    }

private:
    stdfs::path path_;
};

// A line `out_directory_path = "..."` of the configuration (at the start of the line): the position of the value
// and its text.
struct OutLine {
    bool matches = false;
    std::string value;   // the text inside the quotes, if the line has them
    bool has_value = false;
};
OutLine parse_out_line(std::string_view line) {
    constexpr std::string_view kKey = "out_directory_path";
    OutLine r;
    if (!line.starts_with(kKey)) return r;
    std::string_view rest = line.substr(kKey.size());
    while (!rest.empty() && (rest.front() == ' ' || rest.front() == '\t')) rest.remove_prefix(1);
    if (rest.empty() || rest.front() != '=') return r;
    r.matches = true;
    rest.remove_prefix(1);
    while (!rest.empty() && (rest.front() == ' ' || rest.front() == '\t')) rest.remove_prefix(1);
    if (rest.empty() || rest.front() != '"') return r;
    rest.remove_prefix(1);
    const auto close = rest.find('"');
    if (close == std::string_view::npos) return r;
    r.value = std::string(rest.substr(0, close));
    r.has_value = true;
    return r;
}

constexpr std::array<std::string_view, 2> kSkipTests = {"ppc_testing_native_thunks", "seq_jumptable_constants"};

std::string first_line(std::string_view s) { return std::string(s.substr(0, s.find('\n'))); }

}  // namespace

std::span<const std::string_view> skipped_instruction_tests() { return kSkipTests; }

std::optional<std::string> out_directory_path(std::string_view toml_text) {
    for (const std::string_view line : text::split(toml_text, '\n')) {
        const OutLine l = parse_out_line(line);
        if (l.has_value) return l.value;
    }
    return std::nullopt;
}

Xenon::Xenon(Config config) : config_(std::move(config)) {}

stdfs::path Xenon::visual_studio() const {
    if (config_.visual_studio && !config_.visual_studio->empty()) return *config_.visual_studio;
    const std::string program_files = fs::get_env("ProgramFiles(x86)").value_or("C:\\Program Files (x86)");
    const stdfs::path vswhere = fs::from_text(program_files) / "Microsoft Visual Studio" / "Installer" / "vswhere.exe";
    std::error_code ec;
    if (stdfs::is_regular_file(vswhere, ec)) {
        const Run r = run_program(config_, {vswhere.string(), "-latest", "-prerelease", "-products", "*", "-requires",
                                   "Microsoft.VisualStudio.Component.VC.Tools.x86.x64", "-property", "installationPath"},
                                  std::nullopt, nullptr, config_.tool_timeout);
        const std::string_view out = text::trim(r.out);
        if (!out.empty()) return fs::from_text(first_line(out));
    }
    throw ToolError("vswhere", 0, "", "no Visual Studio with the C++ x64 tools found (setting visual_studio)");
}

stdfs::path Xenon::clang_cl() const {
    const stdfs::path path = visual_studio() / "VC" / "Tools" / "Llvm" / "x64" / "bin" / "clang-cl.exe";
    std::error_code ec;
    if (!stdfs::is_regular_file(path, ec)) {
        throw ToolError("clang-cl", 0, "",
                        "clang-cl is not installed: add \"C++ Clang Compiler for Windows\" in the Visual Studio Installer");
    }
    return path;
}

const Environment& Xenon::msvc_env() {
    if (msvc_env_) return *msvc_env_;
    const stdfs::path bat = visual_studio() / "VC" / "Auxiliary" / "Build" / "vcvars64.bat";
    // cmd cannot take a quoted batch file and a redirection on its own command line through an argv, so the two
    // lines go into a small batch file of ours (UTF-8 output, so that environment values survive).
    const stdfs::path script = config_.state_dir / "xenonrecomp" / "msvc_env.cmd";
    fs::write_file_atomic(script, as_bytes(std::format("@echo off\r\nchcp 65001 >nul\r\ncall \"{}\" >nul && set\r\n", bat.string())));
    const std::string shell = fs::get_env("COMSPEC").value_or("cmd.exe");
    const Run r = run_program(config_, {shell, "/d", "/c", script.string()}, std::nullopt, nullptr, config_.tool_timeout);
    Environment env;
    for (const std::string_view line : text::lines(r.out)) {
        const auto eq = line.find('=');
        if (eq == std::string_view::npos || eq == 0) continue;   // (the hidden per-drive entries start with '=')
        env[std::string(line.substr(0, eq))] = std::string(line.substr(eq + 1));
    }
    // DIVERGES: the original went on with an empty environment when vcvars64.bat failed, and the build then failed
    // for a reason that did not name it
    if (env.empty()) throw ToolError("vcvars64.bat", r.code, r.out + r.err, "gave no environment");
    msvc_env_ = std::move(env);
    return *msvc_env_;
}

stdfs::path Xenon::build(bool force) {
    const stdfs::path recomp = recomp_exe();
    std::error_code ec;
    if (!force && stdfs::is_regular_file(recomp, ec) && stdfs::is_regular_file(analyse_exe(), ec)) {
        const auto oldest = std::min(stdfs::last_write_time(recomp, ec), stdfs::last_write_time(analyse_exe(), ec));
        if (oldest >= newest_source(source_dir())) return recomp;
    }
    const Environment& env = msvc_env();
    const std::string cc = clang_cl().string();
    if (force || !stdfs::is_regular_file(build_dir() / "CMakeCache.txt", ec)) {
        const Run r = run_program(config_, {"cmake", "-S", source_dir().string(), "-B", build_dir().string(), "-G", "Ninja", "-DCMAKE_BUILD_TYPE=Release",
                                   "-DCMAKE_C_COMPILER=" + cc, "-DCMAKE_CXX_COMPILER=" + cc},
                                  std::nullopt, &env, config_.build_timeout);
        if (r.code != 0) throw ToolError("cmake", r.code, tail(r.out + r.err, 3000), "configuring XenonRecomp failed");
    }
    const unsigned jobs = config_.build_jobs.value_or(std::max(1u, std::thread::hardware_concurrency() / 2));
    const Run r = run_program(config_, {"cmake", "--build", build_dir().string(), "-j", std::to_string(jobs)}, std::nullopt, &env, config_.build_timeout);
    if (r.code != 0) throw ToolError("cmake", r.code, tail(r.out + r.err, 3000), "building XenonRecomp failed");
    return recomp;
}

std::string Xenon::recompile(const stdfs::path& config_toml) {
    build();
    // it runs in the configuration's folder: a relative path would not reach it
    const stdfs::path config = stdfs::absolute(config_toml);
    const Run r = run_program(config_, {recomp_exe().string(), config.string(), context_header().string()}, config.parent_path(), nullptr,
                              config_.recompile_timeout);
    if (r.code != 0) throw ToolError("XenonRecomp", r.code, tail(r.out + r.err, 3000), "failed");
    return r.out;
}

SyncResult Xenon::recompile_synced(const stdfs::path& config_toml, const ReportFn& report) {
    build();
    const stdfs::path config = stdfs::absolute(config_toml);
    const stdfs::path folder = config.parent_path();
    const std::string toml = fs::read_text(config);
    const std::optional<std::string> out_rel = out_directory_path(toml);
    if (!out_rel) throw ParseError(fs::to_text(config), 0, 0, "no out_directory_path");
    const stdfs::path out_dir = (folder / fs::from_text(*out_rel)).lexically_normal();
    fs::make_dirs(out_dir.parent_path());

    SyncResult result;
    fs::ScopedTempDir tmp("recompile", out_dir.parent_path());
    const stdfs::path fresh = tmp.path() / "ppc";
    fs::make_dirs(fresh);
    // the same configuration beside the original (its relative paths still reach), with the fresh folder; passed by
    // its name, as XenonRecomp joins the paths inside to the configuration's own
    const std::string work_name = "_recompile_tmp.toml";
    stdfs::path rel = fresh.lexically_relative(folder);
    const std::string fresh_text = rel.empty() ? fs::to_text(fresh) : fs::to_text(rel);
    std::vector<std::string> rewritten_lines;
    for (const std::string_view line : text::split(toml, '\n')) {
        rewritten_lines.push_back(parse_out_line(line).matches ? "out_directory_path = \"" + fresh_text + "\"" : std::string(line));
    }
    const std::string rewritten = text::join(rewritten_lines, "\n");
    Run run;
    {
        const stdfs::path work = folder / work_name;
        const RemoveOnExit remove_work(work);
        fs::write_text(work, rewritten);
        run = run_program(config_, {recomp_exe().string(), work_name, context_header().string()}, folder, nullptr, config_.recompile_timeout);
    }
    for (const std::string_view line : text::lines(run.out)) {
        if (!line.starts_with("Recompiling functions")) result.log.emplace_back(line);
    }
    if (run.code != 0) {
        const std::size_t from = result.log.size() > 40 ? result.log.size() - 40 : 0;
        const std::vector<std::string> last(result.log.begin() + static_cast<std::ptrdiff_t>(from), result.log.end());
        throw ToolError("XenonRecomp", run.code, text::join(last, "\n") + run.err, "failed");
    }

    fs::make_dirs(out_dir);
    std::set<std::string> made;
    for (const auto& entry : stdfs::directory_iterator(fresh)) {
        if (entry.is_regular_file()) made.insert(entry.path().filename().string());
    }
    result.generated = made.size();
    for (const std::string& name : made) {
        const stdfs::path dst = out_dir / name;
        const stdfs::path src = fresh / name;
        if (!stdfs::exists(dst) || !same_content(src, dst)) {
            stdfs::copy_file(src, dst, stdfs::copy_options::overwrite_existing);
            stdfs::last_write_time(dst, stdfs::last_write_time(src));   // (as shutil.copy2: the copy keeps the time)
            ++result.changed;
        }
    }
    for (const auto& entry : stdfs::directory_iterator(out_dir)) {
        const std::string name = entry.path().filename().string();
        if (!made.contains(name)) result.stale.push_back(name);
    }
    std::sort(result.stale.begin(), result.stale.end());
    for (const std::string& name : result.stale) {
        std::error_code ec;
        if (stdfs::is_regular_file(out_dir / name, ec)) stdfs::remove(out_dir / name, ec);
    }
    if (report) {
        for (const std::string& line : result.log) report(line);
        report(std::format("{} files generated, {} changed, {} stale removed{}; {} warning lines", result.generated, result.changed,
                           result.stale.size(), result.stale.empty() ? std::string() : ": " + text::join(result.stale, " "), result.log.size()));
    }
    return result;
}

InstructionTestResult Xenon::instruction_tests(const InstructionTestOptions& options) {
    if (!config_.ppc_binutils || config_.ppc_binutils->empty()) {
        throw ToolError("powerpc-none-elf-as", 0, "", "no PowerPC binutils (setting ppc_binutils)");
    }
    const stdfs::path binutils = *config_.ppc_binutils;
    const stdfs::path as = binutils / "powerpc-none-elf-as.exe";
    const stdfs::path objdump = binutils / "powerpc-none-elf-objdump.exe";
    std::error_code ec;
    if (!stdfs::is_regular_file(as, ec)) throw ToolError("powerpc-none-elf-as", 0, "", "no PowerPC binutils at " + fs::to_text(binutils) + " (setting ppc_binutils)");
    build();
    const stdfs::path work = options.work.value_or(config_.state_dir / "xenonrecomp" / "ppc_tests");
    const stdfs::path src = work / "src", objs = work / "src" / "bin", out = work / "out";
    for (const stdfs::path& d : {objs, out}) {
        fs::remove_tree(d);
        fs::make_dirs(d);
    }

    // the tests: tests_dir/<group>/<name>.s, in path order
    std::vector<stdfs::path> sources;
    for (const auto& group : stdfs::directory_iterator(tests_dir(), ec)) {
        if (!group.is_directory() || is_hidden(group.path())) continue;
        for (const auto& file : stdfs::directory_iterator(group.path())) {
            std::string ext = file.path().extension().string();
            if (file.is_regular_file() && text::lower(ext) == ".s") sources.push_back(file.path());
        }
    }
    std::sort(sources.begin(), sources.end(), [](const stdfs::path& a, const stdfs::path& b) { return fs::to_text(a) < fs::to_text(b); });

    std::vector<std::string> tested;
    for (const stdfs::path& s : sources) {
        const std::string name = s.stem().string();
        const bool wanted = options.names.empty() || std::find(options.names.begin(), options.names.end(), name) != options.names.end();
        if (!wanted || std::find(kSkipTests.begin(), kSkipTests.end(), name) != kSkipTests.end()) continue;
        stdfs::copy_file(s, src / (name + ".s"), stdfs::copy_options::overwrite_existing);
        const stdfs::path o = objs / (name + ".o");
        const Run asm_run = run_program(config_, {as.string(), "-a32", "-be", "-mregnames", "-mpower7", "-maltivec", "-mvsx", "-mvmx128", "-R",
                                         "-o" + slashes(o), slashes(src / (name + ".s"))},
                                        std::nullopt, nullptr, config_.tool_timeout);
        if (asm_run.code != 0) {
            throw ToolError("powerpc-none-elf-as", asm_run.code, asm_run.err,
                            "assembling " + name + " failed: " + std::string(text::trim(asm_run.err).substr(0, 300)));
        }
        const Run dump = run_program(config_, {objdump.string(), "--adjust-vma=0x100000", "-Mpower7", "-Mvmx128", "-D", "-EB", slashes(o)}, std::nullopt,
                                     nullptr, config_.tool_timeout);
        // the listing without the four header lines
        std::size_t skip = 0;
        for (int i = 0; i < 4 && skip != std::string::npos; ++i) {
            skip = dump.out.find('\n', skip);
            if (skip != std::string::npos) ++skip;
        }
        fs::write_text(objs / (name + ".dis"), skip == std::string::npos ? std::string() : dump.out.substr(skip));
        tested.push_back(name);
    }

    // a test of an instruction XenonRecomp does not implement is left out (and listed): a title's own recompile
    // reports any instruction it uses
    Run recomp = run_program(config_, {recomp_exe().string(), objs.string(), out.string()}, std::nullopt, nullptr, config_.recompile_timeout);
    std::set<std::string> skipped_set;
    for (const std::string_view line : text::lines(recomp.out)) {
        if (line.find("has unimplemented") == std::string_view::npos) continue;
        const auto in = line.find(" in ");
        if (in == std::string_view::npos) continue;
        const std::vector<std::string_view> words = text::split_ws(line.substr(in + 4));
        if (!words.empty()) skipped_set.emplace(words[0]);
    }
    InstructionTestResult result;
    result.skipped.assign(skipped_set.begin(), skipped_set.end());
    if (!result.skipped.empty()) {
        for (const std::string& name : result.skipped) {
            for (const char* ext : {".o", ".dis"}) {
                std::error_code rm;
                stdfs::remove(objs / (name + ext), rm);
            }
        }
        fs::remove_tree(out);
        fs::make_dirs(out);
        recomp = run_program(config_, {recomp_exe().string(), objs.string(), out.string()}, std::nullopt, nullptr, config_.recompile_timeout);
    }

    const stdfs::path exe = work / "ppc_tests.exe";
    const stdfs::path utils = source_dir() / "XenonUtils";
    std::vector<std::string> cc = {clang_cl().string(), "/nologo", "/std:c++20", "/O1", "/fp:strict", "-fno-strict-aliasing",
                                   "-march=" + options.march, "-Wno-everything", "/DFMT_HEADER_ONLY", "/I" + utils.string(),
                                   "/I" + (source_dir() / "thirdparty" / "simde").string(),
                                   "/I" + (source_dir() / "thirdparty" / "fmt" / "include").string(), "/Fe" + exe.string(),
                                   "/Fo" + work.string() + "\\"};
    std::vector<std::string> generated;
    for (const auto& entry : stdfs::directory_iterator(out)) {
        if (entry.is_regular_file() && text::lower(entry.path().extension().string()) == ".cpp") generated.push_back(entry.path().string());
    }
    std::sort(generated.begin(), generated.end());
    cc.insert(cc.end(), generated.begin(), generated.end());
    const Run compiled = run_program(config_, std::move(cc), work, &msvc_env(), config_.build_timeout);
    if (compiled.code != 0) throw ToolError("clang-cl", compiled.code, tail(compiled.out + compiled.err, 4000), "building the instruction tests failed");

    const Run ran = run_program(config_, {exe.string()}, work, nullptr, config_.tool_timeout);
    std::set<std::string> failed;
    for (const std::string_view line : text::lines(ran.out)) {
        if (text::trim(line).empty()) continue;
        result.mismatches.emplace_back(line);
        failed.emplace(text::split_ws(line).front());
    }
    result.tested = tested.size() - result.skipped.size();
    result.failed.assign(failed.begin(), failed.end());
    return result;
}

void xenon_recompile_command(Xenon& xenon, std::span<const std::string> args) {
    if (args.size() != 1) throw UsageError("usage: codebreaker xenon-recompile <config.toml>");
    xenon.recompile_synced(fs::from_text(args[0]), [](std::string_view line) { out("{}\n", line); });
}

}  // namespace cb::recompile::xenon
