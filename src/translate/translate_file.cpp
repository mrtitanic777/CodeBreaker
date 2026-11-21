// translate/translate_file.cpp -- see translate_file.hpp.
#include "translate/translate_file.hpp"

#include <algorithm>
#include <format>

#include "core/arena.hpp"
#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/process.hpp"
#include "core/text.hpp"
#include "decompile/jar.hpp"
#include "java/frontend.hpp"
#include "translate/backends/c_java.hpp"
#include "translate/backends/csharp.hpp"
#include "translate/dotnet/judge.hpp"
#include "translate/dotnet/pairs.hpp"
#include "translate/frontends/csharp/frontend.hpp"
#include "translate/judge_py_cpp_command.hpp"

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
    const stdfs::path runtime = checkout / "config" / "translate" / "runtime" / "cpp17";
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
    const frontends::csharp::Frontend frontend(*sdk, checkout / "config" / "translate" / "frontends" / "csharp" / "CsTreeDump.cs", environment.config.state());
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

// ---- Java -> C ---------------------------------------------------------------------------------------------------------------------------------

// The top-level class of the file whose main runs: the one named for the file, else the only one with a main. Empty when that does not settle it.
struct JavaMain {
    std::string simple, binary;   // as the C program's JRT_MAIN names it, and as the JVM does
    std::size_t found = 0;
};

JavaMain tf_java_main(std::span<ir::Node* const> units, const std::string& stem) {
    JavaMain chosen, named;
    for (ir::Node* node : units) {
        const auto* unit = ir::get<ir::Unit>(node);
        if (unit == nullptr) continue;
        for (ir::Node* type : unit->types) {
            const auto* cls = ir::get<ir::ClassDecl>(type);
            if (cls == nullptr || !cls->name) continue;
            const bool has_main = std::any_of(cls->members.begin(), cls->members.end(), [](ir::Node* member) {
                const auto* m = ir::get<ir::MethodDecl>(member);
                return m != nullptr && m->name == "main" && m->sym && m->sym->is_static && m->params.size() == 1;
            });
            if (!has_main) continue;
            ++chosen.found;
            const std::string binary = unit->package && !unit->package->empty() ? *unit->package + "." + *cls->name : *cls->name;
            chosen.simple = *cls->name;
            chosen.binary = binary;
            if (*cls->name == stem) named = JavaMain{*cls->name, binary, 1};
        }
    }
    if (!named.simple.empty()) return named;
    if (chosen.found > 1) chosen.simple.clear();
    return chosen;
}

// What a program printed, with Windows' line ends as Java's own, and how it ended.
struct JavaRun {
    std::string out;
    std::uint32_t exit_code = 0;
    bool timed_out = false;
};

JavaRun tf_java_run(process::ProcessSpec spec) {
    spec.timeout = std::chrono::seconds(120);
    const process::ProcessResult ran = process::run(spec);
    return {text::replace_all(text::sanitize_utf8(as_text(ran.out)), "\r\n", "\n"), ran.exit_code, ran.timed_out};
}

Result tf_java_c(const stdfs::path& input, const stdfs::path& out, const stdfs::path& work, const judge_cs_cpp::Environment& environment) {
    const auto java = decompile::jar::find_java({});
    if (!java) throw Error("translating Java needs a JDK (java and javac): none found");
    const stdfs::path javac = java->parent_path() / (java->extension() == ".exe" ? "javac.exe" : "javac");
    if (!stdfs::is_regular_file(javac)) throw Error("translating Java needs a JDK: no javac beside " + fs::to_text(*java));
    const judge_cs_cpp::Observer& watch = environment.observer;
    const auto translating = [&watch](std::uint64_t done, std::uint64_t total, std::string_view what) {
        if (watch.translating) watch.translating(done, total, what);
    };
    const auto checking = [&watch](std::uint64_t done, std::uint64_t total, std::string_view what) {
        if (watch.checking) watch.checking(done, total, what);
    };
    const std::string name = fs::to_text(input.stem());
    const stdfs::path judged = work / "judge";
    const stdfs::path source = judged / "src" / input.filename();
    fs::make_dirs(source.parent_path());
    fs::write_file(source, fs::read_file(input));
    Result result;

    translating(0, 0, fs::to_text(input.filename()));
    process::ProcessSpec compile;   // the original first: a program javac refuses has nothing to be compared with
    compile.argv = {fs::to_text(javac), "-nowarn", "-encoding", "UTF-8", "-d", fs::to_text(judged / "classes"), fs::to_text(source)};
    compile.err = process::Capture::Merge;
    compile.timeout = std::chrono::seconds(300);
    const process::ProcessResult compiled = process::run(compile);
    if (compiled.exit_code != 0) {
        result.verdict = Verdict::BadInput;
        result.detail = "front end: javac: " + text::sanitize_utf8(as_text(compiled.out)).substr(0, 400);
        return result;
    }
    Arena arena;
    const java::frontend::DumpTools tools{*java, javac, environment.config.checkout_root() / "config" / "translate" / "frontends" / "java" / "TreeDump.java",
                                          environment.config.state(), {}};
    std::vector<ir::Node*> units = java::frontend::lower_program(arena, source.parent_path(), tools);
    const JavaMain main_class = tf_java_main(units, name);
    if (main_class.simple.empty()) {
        result.verdict = Verdict::BadInput;
        result.detail = main_class.found == 0 ? "this file has no main method. CodeBreaker proves a Java translation by running the program and its translation, "
                                                "and comparing what they print; with no main there is nothing to run"
                                              : "this file has several classes with a main and none named for the file, so which one to run is not known";
        return result;
    }
    const backends::c_java::Built built = backends::c_java::build(arena, units, judged / "c");
    if (!built.exe) {
        result.verdict = built.why.starts_with("Decline:") ? Verdict::Refused : Verdict::CompileFailed;
        result.detail = built.why;
        return result;
    }
    translating(1, 1, {});

    checking(0, 3, "Running the Java program");
    process::ProcessSpec jvm;
    jvm.argv = {fs::to_text(*java), "-Dfile.encoding=UTF-8", "-Dstdout.encoding=UTF-8", "-cp", fs::to_text(judged / "classes"), main_class.binary};
    const JavaRun original = tf_java_run(jvm);
    if (original.timed_out) {
        result.verdict = Verdict::BadInput;
        result.detail = "the Java program did not finish in two minutes";
        return result;
    }
    checking(1, 3, "Running the translation");
    process::ProcessSpec native;
    native.argv = {fs::to_text(*built.exe)};
    native.env["JRT_MAIN"] = main_class.simple;
    const JavaRun translation = tf_java_run(native);
    checking(2, 3, "Comparing what both print");
    if (translation.timed_out || translation.exit_code != original.exit_code || translation.out != original.out) {
        result.verdict = Verdict::Differs;
        if (translation.timed_out) {
            result.detail = "the translation did not finish in two minutes";
        } else if (translation.exit_code != original.exit_code) {
            result.detail = std::format("Java exited with {}, the translation with {}", original.exit_code, translation.exit_code);
        } else {
            const auto first = std::mismatch(original.out.begin(), original.out.end(), translation.out.begin(), translation.out.end());
            const auto line = 1 + std::count(original.out.begin(), first.first, '\n');
            result.detail = std::format("what they print first differs on line {}", line);
        }
        return result;
    }
    checking(3, 3, {});
    result.verdict = Verdict::Agrees;
    tf_copy(judged / "c" / "program.c", out / (name + ".c"), result.files, out);
    const stdfs::path runtime = backends::c_java::default_runtime_dir();
    for (const char* file : {"jrt.h", "jrt.c"}) tf_copy(runtime / file, out / "runtime" / file, result.files, out);
    const stdfs::path note = out / "BUILD.txt";
    fs::write_text(note, "The C translation of " + name + ".java.\n\nIt builds with a C11 compiler; the runtime it needs is in runtime/.\n\n"
                         "  gcc -std=c11 -fwrapv -O1 -I runtime " + name + ".c runtime/jrt.c -o " + name + " -lm\n"
                         "  clang -std=c11 -fwrapv -O1 -I runtime " + name + ".c runtime/jrt.c -o " + name + "\n");
    result.files.push_back(stdfs::relative(note, out));
    result.checked = "The Java program and its C translation printed the same output and exited with the same code.";
    return result;
}

}  // namespace

bool can_run(const capability::Path& path) {
    return (path.from == "csharp" && (path.to == "cpp17" || path.to == "csharp")) || (path.from == "python" && path.to == "cpp17") ||
           (path.from == "java" && path.to == "c");
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
    else if (path.from == "java" && path.to == "c") result = tf_java_c(absolute_input, out, absolute_work, environment);
    else throw Error("no way to translate " + path.from + " to " + path.to + " is wired");
    if (result.verdict != Verdict::Agrees) result.files.clear();
    return result;
}

}  // namespace cb::translate::translate_file
