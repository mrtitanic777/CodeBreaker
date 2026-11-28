// recompile/xenon.cpp -- see xenon.hpp.

#include "recompile/xenon.hpp"

#include <algorithm>
#include <array>
#include <format>
#include <set>

#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/log.hpp"
#include "core/process.hpp"
#include "core/text.hpp"
#include "recompile/pcode_emit.hpp"
#include "recompile/xenonrecomp/XenonRecomp/instruction_translator.h"

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

xenon_engine::Tool with_pcode(xenon_engine::Tool inner, stdfs::path sla, std::shared_ptr<Translation> counted) {
    return [inner = std::move(inner), sla = std::move(sla), counted = std::move(counted)](const std::vector<std::string>& args) {
        *counted = {};
        const pcode_emit::Translator translator(sla);
        xenon_translate::set([&](std::uint32_t address, std::uint32_t word, std::uint32_t next_word, const xenon_translate::Names& names)
                                 -> std::optional<xenon_translate::Translated> {
            const pcode_emit::Names ours{names.r, names.f, names.v, names.cr, names.ctr, names.xer, names.lr};
            std::string why;
            std::optional<pcode_emit::Statements> s = translator.instruction(address, word, next_word, ours, &why);
            if (!s) {
                ++counted->declined;
                ++counted->why[why];
                return std::nullopt;
            }
            ++counted->translated;
            return xenon_translate::Translated{std::move(s->text), s->fpu};
        });
        struct Unset {
            ~Unset() { xenon_translate::set({}); }
        } unset;
        return inner(args);
    };
}

xenon_engine::Run Xenon::run_recomp(const std::vector<std::string>& args) {
    translation_ = {};
    if (!config_.pcode_sla) return config_.recomp(args);
    const auto counted = std::make_shared<Translation>();
    const xenon_engine::Run r = with_pcode(config_.recomp, *config_.pcode_sla, counted)(args);
    translation_ = *counted;
    return r;
}

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
    // a vcvars64.bat that gives no environment is an error that names it (an empty environment would fail the build for a
    // reason that did not)
    if (env.empty()) throw ToolError("vcvars64.bat", r.code, r.out + r.err, "gave no environment");
    msvc_env_ = std::move(env);
    return *msvc_env_;
}

// XenonRecomp in-process over `args`: a ToolError carrying the end of what it printed when it fails.
static xenon_engine::Run recomp_or_throw(const Config& config, const std::vector<std::string>& args) {
    xenon_engine::Run r = config.recomp(args);
    if (r.code != 0) throw ToolError("XenonRecomp", static_cast<std::uint32_t>(r.code), tail(r.log, 3000), "failed");
    return r;
}

std::string Xenon::recompile(const stdfs::path& config_toml) {
    // the configuration's paths are joined to its own folder: given absolute, nothing depends on the working folder
    const stdfs::path config = stdfs::absolute(config_toml);
    const std::vector<std::string> args = {fs::to_text(config), fs::to_text(context_header())};
    if (!config_.pcode_sla) return recomp_or_throw(config_, args).log;
    const xenon_engine::Run r = run_recomp(args);
    if (r.code != 0) throw ToolError("XenonRecomp", static_cast<std::uint32_t>(r.code), tail(r.log, 4000), "failed");
    return r.log;
}

SyncResult Xenon::recompile_synced(const stdfs::path& config_toml, const ReportFn& report) {
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
    // the same configuration beside the original (its relative paths still reach), with the fresh folder: XenonRecomp joins
    // the paths inside to the configuration's own folder
    const std::string work_name = "_recompile_tmp.toml";
    stdfs::path rel = fresh.lexically_relative(folder);
    const std::string fresh_text = rel.empty() ? fs::to_text(fresh) : fs::to_text(rel);
    std::vector<std::string> rewritten_lines;
    for (const std::string_view line : text::split(toml, '\n')) {
        rewritten_lines.push_back(parse_out_line(line).matches ? "out_directory_path = \"" + fresh_text + "\"" : std::string(line));
    }
    const std::string rewritten = text::join(rewritten_lines, "\n");
    xenon_engine::Run run;
    {
        const stdfs::path work = folder / work_name;
        const RemoveOnExit remove_work(work);
        fs::write_text(work, rewritten);
        run = run_recomp({fs::to_text(work), fs::to_text(context_header())});
    }
    for (const std::string_view line : text::lines(run.log)) {
        if (!line.starts_with("Recompiling functions")) result.log.emplace_back(line);
    }
    if (run.code != 0) {
        const std::size_t from = result.log.size() > 40 ? result.log.size() - 40 : 0;
        const std::vector<std::string> last(result.log.begin() + static_cast<std::ptrdiff_t>(from), result.log.end());
        throw ToolError("XenonRecomp", static_cast<std::uint32_t>(run.code), text::join(last, "\n"), "failed");
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
            cb::fs::set_last_write_time(dst, cb::fs::last_write_time(src));   // (as shutil.copy2: the copy keeps the time)
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
    xenon_engine::Run recomp = run_recomp({fs::to_text(objs), fs::to_text(out)});
    std::set<std::string> skipped_set;
    for (const std::string_view line : text::lines(recomp.log)) {
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
        recomp = run_recomp({fs::to_text(objs), fs::to_text(out)});
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
    const Run compiled = run_program(config_, std::move(cc), work, &msvc_env(), config_.compile_timeout);
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
    result.translation = translation_;
    return result;
}

void xenon_recompile_command(Xenon& xenon, std::span<const std::string> args) {
    if (args.size() != 1) throw UsageError("usage: codebreaker xenon-recompile <config.toml>");
    xenon.recompile_synced(fs::from_text(args[0]), [](std::string_view line) { out("{}\n", line); });
}

}  // namespace cb::recompile::xenon
