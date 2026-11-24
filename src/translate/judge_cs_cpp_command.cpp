#include "translate/judge_cs_cpp_command.hpp"

#include <algorithm>
#include <format>

#include "core/arena.hpp"
#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/json.hpp"
#include "core/log.hpp"
#include "core/process.hpp"
#include "core/text.hpp"
#include "translate/backends/csharp_cpp17.hpp"
#include "translate/dotnet/judge.hpp"
#include "translate/frontends/csharp/frontend.hpp"
#include "translate/l2.hpp"

namespace cb::translate::judge_cs_cpp {

namespace stdfs = std::filesystem;
namespace dj = cb::translate::dotnet::judge;
namespace ir = cb::translate::ir;

namespace {

constexpr std::chrono::minutes kBuildTimeout{10};
constexpr std::string_view kVsLlvm[] = {R"(C:\Program Files\Microsoft Visual Studio\18\Community\VC\Tools\Llvm\x64\bin\clang++.exe)",
                                        R"(C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\Llvm\x64\bin\clang++.exe)"};
constexpr std::string_view kVcvars[] = {R"(C:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvars64.bat)",
                                        R"(C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat)"};

bool is_file(const stdfs::path& p) {
    std::error_code ec;
    return stdfs::is_regular_file(p, ec);
}

std::optional<stdfs::path> on_path(std::string_view exe) {
    const auto path_var = fs::get_env("PATH");
    if (!path_var) return std::nullopt;
    for (const std::string_view dir : text::split(*path_var, ';')) {
        const stdfs::path candidate = fs::from_text(dir) / std::string(exe);
        if (!dir.empty() && is_file(candidate)) return candidate;
    }
    return std::nullopt;
}

std::optional<stdfs::path> vcvars() {
    for (const std::string_view p : kVcvars) {
        if (is_file(fs::from_text(p))) return fs::from_text(p);
    }
    return std::nullopt;
}

std::optional<Cxx> gcc() {
    if (const auto p = on_path("g++.exe")) return Cxx{Compiler::Gcc, *p};
    return std::nullopt;
}
std::optional<Cxx> clang() {
    for (const std::string_view p : kVsLlvm) {
        if (is_file(fs::from_text(p))) return Cxx{Compiler::Clang, fs::from_text(p)};
    }
    if (const auto p = on_path("clang++.exe")) return Cxx{Compiler::Clang, *p};
    return std::nullopt;
}
std::optional<Cxx> msvc() {
    if (vcvars()) return Cxx{Compiler::Msvc, {}};
    return std::nullopt;
}

std::string clip(const std::string& s, std::size_t n) { return s.size() > n ? s.substr(0, n) : s; }

std::string one_line(const std::string& s, std::size_t n) { return text::replace_all(text::replace_all(clip(s, n), "|", "\\|"), "\n", " "); }

std::string yes_no(const std::optional<bool>& v) { return !v ? "-" : *v ? "yes" : "NO"; }

// Build `source` (with the runtime) into `exe`; `log` is the compiler's output. false: it did not compile.
bool build(const Cxx& cxx, const stdfs::path& runtime, const stdfs::path& source, const stdfs::path& exe, std::string& log) {
    process::ProcessSpec spec;
    spec.err = process::Capture::Merge;
    spec.timeout = std::chrono::duration_cast<std::chrono::milliseconds>(kBuildTimeout);
    spec.cwd = source.parent_path();
    if (cxx.kind == Compiler::Msvc) {
        // cl needs Visual Studio's environment: a small batch file sets it and builds
        const stdfs::path bat = source.parent_path() / "build.cmd";
        fs::write_text(bat, std::format("@echo off\r\ncall \"{}\" >nul\r\ncl /nologo /std:c++17 /EHsc /utf-8 /permissive- /bigobj /W3/wd4244 /wd4267 /wd4018 /wd4146 /wd4309 /wd4305 /I\"{}\" /Fe\"{}\" /Fo\"{}\\\\\" \"{}\"\r\n",
                                        fs::to_text(*vcvars()), fs::to_text(runtime), fs::to_text(exe), fs::to_text(source.parent_path()), fs::to_text(source)));
        spec.argv = {"cmd", "/c", fs::to_text(bat)};
    } else {
        spec.argv = {fs::to_text(cxx.exe), "-std=c++17", "-fwrapv", "-w", "-O1", "-Wa,-mbig-obj", "-I", fs::to_text(runtime), "-o", fs::to_text(exe), fs::to_text(source)};   // (a whole game is more sections than a plain object file holds)
        if (cxx.kind == Compiler::Gcc) spec.argv.insert(spec.argv.end(), {"-static"});
    }
    const process::ProcessResult r = process::run(spec);
    log = std::string(as_text(r.out));
    return r.exit_code == 0 && is_file(exe);
}

}  // namespace

bool build_program(const Cxx& cxx, const stdfs::path& runtime, const stdfs::path& source, const stdfs::path& exe, std::string& log) {
    return build(cxx, runtime, source, exe, log);
}

std::optional<Cxx> find_cxx(const std::string& want) {
    if (want == "gcc") return gcc();
    if (want == "clang") return clang();
    if (want == "msvc") return msvc();
    if (!want.empty()) {
        const stdfs::path p = fs::from_text(want);
        if (!is_file(p)) return std::nullopt;
        const std::string name = text::lower(fs::to_text(p.filename()));
        return Cxx{name.starts_with("clang") ? Compiler::Clang : name.starts_with("cl.") ? Compiler::Msvc : Compiler::Gcc, p};
    }
    if (auto c = gcc()) return c;
    if (auto c = clang()) return c;
    return msvc();
}

Score judge_program(const dotnet::Sdk& sdk, const Environment& environment, const Cxx& cxx, const std::string& name, const std::vector<stdfs::path>& files,
                    const stdfs::path& work, bool write_l2) {
    Score score{.program = name};
    dj::fresh_folder(work);   // (a compiler runs in the folder of the source, so every path given is made absolute)
    const stdfs::path checkout = environment.config.checkout_root();
    const frontends::csharp::Frontend frontend(sdk, checkout / "config" / "translate" / "frontends" / "csharp" / "CsTreeDump.cs", environment.config.state());

    const Observer& watch = environment.observer;
    const auto translating = [&watch](std::uint64_t done, std::uint64_t total, std::string_view what) {
        if (watch.translating) watch.translating(done, total, what);
    };
    const auto checking = [&watch](std::uint64_t done, std::uint64_t total, std::string_view what) {
        if (watch.checking) watch.checking(done, total, what);
    };
    translating(0, 0, files.empty() ? std::string("the program") : fs::to_text(files.front().filename()));
    Arena arena;
    frontends::csharp::Program program;
    try {
        program = frontend.lower(arena, files, work);
    } catch (const Error& e) {
        score.lowers = false;
        score.detail = std::string("front end: ") + e.what();
        return score;
    }
    if (write_l2) {
        Json docs = Json::array();
        for (ir::Node* u : program.units) docs.push_back(l2::to_json(*u));
        fs::write_text(work / "program.l2.json", docs.dump(1));
    }
    score.lowers = program.refused.empty();
    if (!program.refused.empty()) {
        std::vector<std::string> why;
        for (const auto& r : program.refused) why.push_back(std::format("{} {}: {}", r.where, r.kind, r.why));
        score.detail = "refused by the front end: " + text::join(why, "; ");
        return score;
    }

    std::uint64_t functions = 0;   // what there is to write: every method the front end lowered
    for (ir::Node* u : program.units)
        for (ir::Node* n : ir::walk(u))
            if (ir::get<ir::MethodDecl>(n) != nullptr) ++functions;
    translating(0, functions, {});
    const backends::csharp_cpp17::Output out = backends::csharp_cpp17::write(program.units);
    score.writes = out.declined.empty();
    if (!out.declined.empty()) {
        std::vector<std::string> why;
        for (const auto& g : out.declined) why.push_back(g.text());
        score.detail = "declined: " + text::join(why, "; ");
        return score;
    }
    translating(functions, functions, {});
    const stdfs::path cpp_dir = work / "cpp";
    fs::make_dirs(cpp_dir);
    const stdfs::path source = cpp_dir / (name + ".cpp");
    const stdfs::path exe = cpp_dir / (name + ".exe");
    fs::write_text(source, out.source);
    std::string log;
    checking(0, 4, "Building the C++ translation");
    score.compiles = build(cxx, checkout / "config" / "translate" / "runtime" / "cpp17", source, exe, log);
    if (!*score.compiles) {
        score.detail = "compile: " + dj::first_errors(log);
        return score;
    }

    checking(1, 4, "Running the C# program");
    const dj::CSharpToolchain tools = dj::CSharpToolchain::of(sdk);
    const dj::Run original = dj::csharp(tools, files, work / "run-orig");
    checking(2, 4, "Running the translation");
    dj::Run translation;
    translation.ok = true;
    {
        process::ProcessSpec run;
        run.argv = {fs::to_text(exe)};
        run.timeout = std::chrono::duration_cast<std::chrono::milliseconds>(dj::kTimeout);
        const process::ProcessResult r = process::run(run);
        translation.exit = r.timed_out ? std::nullopt : std::optional<std::uint32_t>(r.exit_code);
        translation.out = std::string(as_text(r.out));
        translation.err = std::string(as_text(r.err));
    }
    fs::write_text(work / "original.out.txt", original.out);
    fs::write_text(work / "translated.out.txt", translation.out);
    checking(3, 4, "Comparing what both print");
    const dj::Judgement j = dj::compare(original, translation);
    checking(4, 4, {});
    score.agrees = j.verdict == dj::Verdict::Equivalent;
    if (!*score.agrees) score.detail = std::string(dj::word(j.verdict)) + ": " + j.detail;
    return score;
}

std::string table(const std::vector<Score>& scores) {
    std::string out = "| program | lowers | writes | compiles | agrees | detail |\n|---|---|---|---|---|---|\n";
    for (const Score& s : scores) {
        out += std::format("| {} | {} | {} | {} | {} | {} |\n", s.program, yes_no(s.lowers), yes_no(s.writes), yes_no(s.compiles), yes_no(s.agrees), one_line(s.detail, 240));
    }
    return out;
}

namespace {

struct Program {
    std::string name;
    std::vector<stdfs::path> files;
};

std::vector<Program> programs_of(const std::vector<std::string>& given, const stdfs::path& corpus) {
    std::vector<Program> out;
    if (given.empty()) {
        for (const stdfs::path& f : frontends::csharp::sources(corpus)) out.push_back({fs::to_text(f.stem()), {f}});
        return out;
    }
    for (const std::string& g : given) {
        const stdfs::path p = fs::from_text(g);
        out.push_back({fs::to_text(p.stem()), frontends::csharp::sources(stdfs::absolute(p))});
    }
    return out;
}

int run_judge(const cli::Args& args, const Environment& environment) {
    const auto sdk = dotnet::Sdk::find();
    if (!sdk || !sdk->available()) throw ToolError("dotnet", 0, "", "judge-cs-cpp needs the .NET SDK (dotnet.exe with Roslyn and the targeting pack)");
    const auto cxx = find_cxx(args.value_or("cxx", ""));
    if (!cxx) throw ToolError("c++", 0, "", "judge-cs-cpp needs a C++17 compiler (g++, clang++ or Visual Studio's cl): none found");
    const stdfs::path work = stdfs::absolute(args.value("work") ? fs::from_text(*args.value("work")) : environment.config.state() / "translate" / "judge-cs-cpp");
    const std::vector<Program> programs = programs_of(args.values("files"), environment.config.checkout_root() / "tests" / "translate" / "corpus" / "csharp");
    std::vector<Score> scores;
    for (const Program& p : programs) scores.push_back(judge_program(*sdk, environment, *cxx, p.name, p.files, work / p.name, args.flag("l2")));
    cb::out("{}", table(scores));
    const auto agreeing = std::count_if(scores.begin(), scores.end(), [](const Score& s) { return s.agrees.value_or(false); });
    cb::out("{} of {} programs agree\n", agreeing, scores.size());
    if (!args.flag("keep")) {
        std::error_code ec;
        stdfs::remove_all(work, ec);
    }
    return static_cast<std::size_t>(agreeing) == scores.size() ? 0 : 1;
}

}  // namespace

cli::Command command(const Environment& environment) {
    cli::Command c;
    c.name = "judge-cs-cpp";
    c.summary = "C# -> C++17 judged: lowered, written, compiled, run beside the C# program";
    c.area = "Translate";
    c.params = {cli::Positional{.name = "files", .help = "C# files or folders, each one program (default: the C# corpus)", .required = false, .variadic = true},
                cli::Option{.name = "cxx", .help = "the C++ compiler: gcc, clang, msvc or a path (default: the first of those found)", .arg = cli::Arg::Value, .value_name = "COMPILER"},
                cli::Option{.name = "work", .help = "the working folder", .arg = cli::Arg::Value, .value_name = "DIR"},
                cli::Option{.name = "l2", .help = "write each program's IR (program.l2.json) into its working folder"},
                cli::Option{.name = "keep", .help = "keep the working folder"}};
    c.handler = [environment](const cli::Args& args) { return run_judge(args, environment); };
    return c;
}

int main(const std::vector<std::string>& args, const Environment& environment) {
    cli::Cli cli("codebreaker", {command(environment)});
    std::vector<std::string> full = {"judge-cs-cpp"};
    full.insert(full.end(), args.begin(), args.end());
    return cli.run(full);
}

}  // namespace cb::translate::judge_cs_cpp
