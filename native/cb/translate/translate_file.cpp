// cb/translate/translate_file.cpp -- see translate_file.hpp.
#include "cb/translate/translate_file.hpp"

#include <algorithm>
#include <format>

#include "cb/core/arena.hpp"
#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/text.hpp"
#include "cb/translate/backends/csharp.hpp"
#include "cb/translate/dotnet/judge.hpp"
#include "cb/translate/dotnet/pairs.hpp"
#include "cb/translate/frontends/csharp/frontend.hpp"
#include "cb/translate/judge_py_cpp_command.hpp"

namespace cb::translate::translate_file {

namespace stdfs = std::filesystem;
namespace dj = cb::translate::dotnet::judge;
namespace ir = cb::translate::ir;
namespace cs = cb::translate::backends::csharp;

namespace {

// ---- what a judge's Score says ---------------------------------------------------------------------------------------------------------

// The verdict of a Score of judge-cs-cpp or judge-py-cpp (the steps lowers, writes, compiles, agrees; the detail says what stopped it).
Verdict tf_verdict_of(const judge_cs_cpp::Score& score) {
    if (score.lowers && !*score.lowers) return score.detail.starts_with("front end:") ? Verdict::BadInput : Verdict::Refused;
    if (score.writes && !*score.writes) return Verdict::Refused;
    if (score.compiles && !*score.compiles) return Verdict::CompileFailed;
    if (score.agrees && *score.agrees) return Verdict::Agrees;
    if (score.agrees) return score.detail.starts_with("no-original") ? Verdict::BadInput : Verdict::Differs;
    return Verdict::BadInput;   // (a step that was not reached and no verdict: the original did not run)
}

// ---- what is handed over -------------------------------------------------------------------------------------------------------------------

void tf_copy(const stdfs::path& from, const stdfs::path& to, std::vector<stdfs::path>& files, const stdfs::path& out) {
    fs::make_dirs(to.parent_path());
    std::error_code ec;
    stdfs::copy_file(from, to, stdfs::copy_options::overwrite_existing, ec);
    if (ec) throw Error("cannot write " + fs::to_text(to) + ": " + ec.message());
    files.push_back(stdfs::relative(to, out));
}

// The runtime headers the translation includes, and the line that builds it.
void tf_write_cpp17(const stdfs::path& built_dir, const std::string& name, const std::vector<std::string>& headers, const std::vector<std::string>& sources,
                    const stdfs::path& out, Result& result) {
    const stdfs::path checkout = config::current().checkout_root();
    const stdfs::path runtime = checkout / "data" / "translate" / "runtime" / "cpp17";
    for (const std::string& s : sources) tf_copy(built_dir / s, out / s, result.files, out);
    for (const std::string& h : headers) tf_copy(runtime / h, out / "runtime" / h, result.files, out);
    const stdfs::path note = out / "BUILD.txt";
    fs::write_text(note, "The C++17 translation of " + name + ".\n\nIt builds with any C++17 compiler; the runtime it needs is in runtime/.\n\n"
                         "  g++ -std=c++17 -fwrapv -O1 -I runtime " + name + ".cpp -o " + name + "\n"
                         "  clang++ -std=c++17 -fwrapv -O1 -I runtime " + name + ".cpp -o " + name + "\n"
                         "  cl /std:c++17 /EHsc /utf-8 /I runtime " + name + ".cpp\n");
    result.files.push_back(stdfs::relative(note, out));
}

// ---- C# -> C++17 -----------------------------------------------------------------------------------------------------------------------

Result tf_csharp_cpp17(const stdfs::path& input, const stdfs::path& out, const stdfs::path& work, const judge_cs_cpp::Environment& environment) {
    const auto sdk = dotnet::Sdk::find();
    if (!sdk || !sdk->available()) throw Error("translating C# needs the .NET SDK (dotnet.exe with Roslyn and the targeting pack)");
    const auto cxx = judge_cs_cpp::find_cxx("");
    if (!cxx) throw Error("translating C# to C++17 needs a C++17 compiler (g++, clang++ or Visual Studio's cl): none found");
    const std::string name = fs::to_text(input.stem());
    const stdfs::path judged = work / "judge";
    const judge_cs_cpp::Score score = judge_cs_cpp::judge_program(*sdk, environment, *cxx, name, {input}, judged);
    Result result;
    result.verdict = tf_verdict_of(score);
    result.detail = score.detail;
    if (result.verdict != Verdict::Agrees) return result;
    tf_write_cpp17(judged / "cpp", name, {"csrt.hpp", "coro.hpp"}, {name + ".cpp"}, out, result);
    result.checked = "The C# program and its C++17 translation printed the same output and exited with the same code.";
    return result;
}

// ---- C# -> C# (the round trip through the hub) ---------------------------------------------------------------------------------

Result tf_csharp_csharp(const stdfs::path& input, const stdfs::path& out, const stdfs::path& work, const judge_cs_cpp::Environment& environment) {
    const auto sdk = dotnet::Sdk::find();
    if (!sdk || !sdk->available()) throw Error("translating C# needs the .NET SDK (dotnet.exe with Roslyn and the targeting pack)");
    const judge_cs_cpp::Observer& watch = environment.observer;
    const auto translating = [&watch](std::uint64_t done, std::uint64_t total, std::string_view what) {
        if (watch.translating) watch.translating(done, total, what);
    };
    const auto checking = [&watch](std::uint64_t done, std::uint64_t total, std::string_view what) {
        if (watch.checking) watch.checking(done, total, what);
    };
    const stdfs::path checkout = environment.config.checkout_root();
    const stdfs::path judged = work / "judge";
    const frontends::csharp::Frontend frontend(*sdk, checkout / "data" / "translate" / "frontends" / "csharp" / "CsTreeDump.cs", environment.config.state());
    Arena arena;
    dotnet::pairs::Pairs pairs{.sdk = *sdk, .csharp = frontend, .arena = arena};
    Result result;

    translating(0, 0, fs::to_text(input.filename()));
    frontends::csharp::Program program;
    try {
        program = pairs.units({input}, judged);
    } catch (const Error& e) {
        result.verdict = Verdict::BadInput;
        result.detail = std::string("front end: ") + e.what();
        return result;
    }
    if (!program.refused.empty()) {
        std::vector<std::string> why;
        for (const auto& r : program.refused) why.push_back(std::format("{} {}: {}", r.where, r.kind, r.why));
        result.verdict = Verdict::Refused;
        result.detail = "refused by the front end: " + text::join(why, "; ");
        return result;
    }
    std::uint64_t functions = 0;
    for (ir::Node* u : program.units)
        for (ir::Node* n : ir::walk(u))
            if (ir::get<ir::MethodDecl>(n) != nullptr) ++functions;
    translating(0, functions, {});
    cs::Program written(program.units);
    const std::vector<stdfs::path> paths = written.write(judged / "cs");
    if (!written.ledger().empty()) {
        std::vector<std::string> why;
        for (const cs::Gap& g : written.ledger()) why.push_back(std::format("{} {}: {}", g.where, g.kind, g.why));
        result.verdict = Verdict::Refused;
        result.detail = "declined: " + text::join(why, "; ");
        return result;
    }
    translating(functions, functions, {});

    const dj::CSharpToolchain tools = dj::CSharpToolchain::of(*sdk);
    checking(0, 3, "Running the C# program");
    const dj::Run original = dj::csharp(tools, {input}, judged / "run-orig");
    checking(1, 3, "Running the translation");
    const dj::Run translation = dj::csharp(tools, paths, judged / "run-trans");
    checking(2, 3, "Comparing what both print");
    const dj::Judgement j = dj::compare(original, translation);
    checking(3, 3, {});
    result.detail = j.verdict == dj::Verdict::Equivalent ? std::string() : std::string(dj::word(j.verdict)) + ": " + j.detail;
    switch (j.verdict) {
        case dj::Verdict::Equivalent: result.verdict = Verdict::Agrees; break;
        case dj::Verdict::Divergent: result.verdict = Verdict::Differs; break;
        case dj::Verdict::Refused: result.verdict = Verdict::Refused; break;
        case dj::Verdict::NoCompile: result.verdict = Verdict::CompileFailed; break;
        case dj::Verdict::NoOriginal: result.verdict = Verdict::BadInput; break;
    }
    if (result.verdict != Verdict::Agrees) return result;
    for (const stdfs::path& p : paths) tf_copy(p, out / p.filename(), result.files, out);
    result.checked = "The C# program and the C# written from it printed the same output and exited with the same code.";
    return result;
}

// ---- Python -> C++17 ---------------------------------------------------------------------------------------------------------------------

Result tf_python_cpp17(const stdfs::path& input, const stdfs::path& out, const stdfs::path& work, const judge_cs_cpp::Environment& environment) {
    const auto cxx = judge_cs_cpp::find_cxx("");
    if (!cxx) throw Error("translating Python to C++17 needs a C++17 compiler (g++, clang++ or Visual Studio's cl): none found");
    const std::string name = fs::to_text(input.stem());
    const stdfs::path judged = work / "judge";
    const judge_cs_cpp::Score score = judge_py_cpp::judge_module(environment, *cxx, input, judged);
    Result result;
    result.verdict = tf_verdict_of(score);
    result.detail = score.detail;
    if (score.detail == "no run_* function")
        result.detail = "this module has no run_* function. CodeBreaker proves a Python translation by running the module's zero-argument run_* functions "
                        "in CPython and in the C++, and comparing their answers; with none there is nothing to compare";
    if (result.verdict != Verdict::Agrees) return result;
    tf_write_cpp17(judged / "cpp", name, {"py.hpp", "py_json.hpp", "py_os.hpp", "py_settings.hpp", "py_stdlib.hpp", "py_unicode.hpp", "py_unicode_print.hpp", "coro.hpp"},
                   {name + ".hpp", name + ".cpp"}, out, result);
    result.checked = "CPython and the C++17 translation gave the same answer for every run_* function of the module.";
    return result;
}

}  // namespace

bool can_run(const capability::Path& path) {
    return (path.from == "csharp" && (path.to == "cpp17" || path.to == "csharp")) || (path.from == "python" && path.to == "cpp17");
}

Result run(const capability::Path& path, const stdfs::path& input, const stdfs::path& out, const stdfs::path& work, const judge_cs_cpp::Environment& environment) {
    const stdfs::path absolute_input = stdfs::absolute(input);
    const stdfs::path absolute_work = stdfs::absolute(work);
    fs::make_dirs(absolute_work);
    fs::make_dirs(out);
    Result result;
    if (path.from == "csharp" && path.to == "cpp17") result = tf_csharp_cpp17(absolute_input, out, absolute_work, environment);
    else if (path.from == "csharp" && path.to == "csharp") result = tf_csharp_csharp(absolute_input, out, absolute_work, environment);
    else if (path.from == "python" && path.to == "cpp17") result = tf_python_cpp17(absolute_input, out, absolute_work, environment);
    else throw Error("no way to translate " + path.from + " to " + path.to + " is wired");
    if (result.verdict != Verdict::Agrees) result.files.clear();
    return result;
}

}  // namespace cb::translate::translate_file
