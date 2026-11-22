// translate/translate_file.cpp -- see translate_file.hpp.
#include "translate/translate_file.hpp"

#include <algorithm>
#include <format>
#include <functional>
#include <sstream>

#include "core/arena.hpp"
#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/process.hpp"
#include "core/text.hpp"
#include "decompile/jar.hpp"
#include "java/apimap.hpp"
#include "java/frontend.hpp"
#include "translate/backends/c_java.hpp"
#include "translate/backends/cpp17.hpp"
#include "translate/backends/csharp.hpp"
#include "translate/backends/csharp_java.hpp"
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
                         "  cl /std:c++17 /EHsc /utf-8 /bigobj /I runtime " + name + ".cpp\n");
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

// The folder a Java file's package starts in: its own, or as many above it as the file's `package` line has names.
stdfs::path tf_java_root(const stdfs::path& file) {
    stdfs::path root = stdfs::absolute(file).parent_path();
    std::istringstream lines(fs::read_text(file));
    for (std::string line; std::getline(lines, line);) {
        const std::string_view trimmed = text::trim(line);
        if (trimmed.starts_with("import ") || trimmed.find(" class ") != std::string_view::npos || trimmed.starts_with("class ")) break;
        if (!trimmed.starts_with("package ")) continue;
        const std::size_t levels = 1 + static_cast<std::size_t>(std::count(trimmed.begin(), trimmed.end(), '.'));
        for (std::size_t i = 0; i < levels; ++i) root = root.parent_path();
        break;
    }
    return root;
}

// The name on a Java file's `package` line, empty for none.
std::string tf_java_package(const stdfs::path& file) {
    std::istringstream lines(fs::read_text(file));
    for (std::string line; std::getline(lines, line);) {
        const std::string_view trimmed = text::trim(line);
        if (trimmed.starts_with("import ") || trimmed.starts_with("class ") || trimmed.find(" class ") != std::string_view::npos) break;
        if (trimmed.starts_with("package ")) return std::string(text::trim(trimmed.substr(8), " \t;"));
    }
    return {};
}

// A file whose folder does not spell its package: javac cannot find its classes by path, so the files beside it in the same package are named.
std::vector<std::string> tf_java_flat_siblings(const stdfs::path& file) {
    const std::string package = tf_java_package(file);
    if (package.empty()) return {};
    std::string spelled = package;
    std::replace(spelled.begin(), spelled.end(), '.', '/');
    const stdfs::path folder = stdfs::absolute(file).parent_path();
    if (folder.generic_string().ends_with("/" + spelled)) return {};
    std::vector<std::string> out;
    for (const auto& entry : stdfs::directory_iterator(folder)) {
        if (entry.path().extension() != ".java" || entry.path().filename() == file.filename()) continue;
        if (tf_java_package(entry.path()) == package) out.push_back(fs::to_text(entry.path()));
    }
    std::sort(out.begin(), out.end());
    return out;
}

// The source files javac read, from its -verbose log: the file itself and every class of the program it reached through -sourcepath.
std::vector<stdfs::path> tf_java_read(std::string_view log) {
    static constexpr std::string_view kStart = "[parsing started ";
    std::vector<stdfs::path> out;
    for (std::size_t at = log.find(kStart); at != std::string_view::npos; at = log.find(kStart, at + 1)) {
        const std::size_t open = log.find('[', at + 1);
        const std::size_t close = log.find("]]", at);
        if (open == std::string_view::npos || close == std::string_view::npos || open > close) continue;
        const std::string_view named = log.substr(open + 1, close - open - 1);
        const std::size_t joint = named.rfind(':');   // (a file found on the source path is written `folder:relative/path`)
        if (joint != std::string_view::npos && joint > 1) out.push_back(fs::from_text(std::string(named.substr(0, joint))) / fs::from_text(std::string(named.substr(joint + 1))));
        else out.push_back(fs::from_text(std::string(named)));
    }
    return out;
}

// javac's log without its -verbose lines.
std::string tf_java_errors(std::string_view log) {
    std::string out;
    for (std::size_t at = 0; at < log.size();) {
        const std::size_t end = std::min(log.find('\n', at), log.size());
        const std::string_view line = log.substr(at, end - at);
        if (!line.starts_with("[")) {
            out.append(line);
            out.push_back('\n');
        }
        at = end + 1;
    }
    return out;
}

// A built program run, as the judge compares it.
dj::Run tf_java_run(process::ProcessSpec spec) {
    spec.timeout = dj::kTimeout;
    const process::ProcessResult ran = process::run(spec);
    if (ran.timed_out) return {.ok = true, .log = "it did not finish in two minutes"};
    return {.ok = true, .exit = ran.exit_code, .out = text::sanitize_utf8(as_text(ran.out)), .err = text::sanitize_utf8(as_text(ran.err))};
}

// Java as C, C++17 or C#: the program and its translation are both run, and only a translation that does what the program does is handed over.
Result tf_java(const std::string& to, const stdfs::path& input, const stdfs::path& out, const stdfs::path& work, const judge_cs_cpp::Environment& environment) {
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
    const stdfs::path sources = judged / "src";
    fs::make_dirs(sources);
    Result result;

    translating(0, 0, fs::to_text(input.filename()));
    // the original first: a program javac refuses has nothing to be compared with. The classes it uses are found beside it, as javac finds them
    const std::vector<std::string> flat = tf_java_flat_siblings(input);
    const stdfs::path root = flat.empty() ? tf_java_root(input) : stdfs::absolute(input).parent_path();
    process::ProcessSpec compile;
    compile.argv = {fs::to_text(javac), "-nowarn", "-verbose", "-encoding", "UTF-8", "-sourcepath", fs::to_text(root), "-d", fs::to_text(judged / "classes"),
                    fs::to_text(stdfs::absolute(input))};
    compile.argv.insert(compile.argv.end(), flat.begin(), flat.end());
    compile.err = process::Capture::Merge;
    compile.timeout = std::chrono::seconds(300);
    const process::ProcessResult compiled = process::run(compile);
    const std::string compile_log = text::sanitize_utf8(as_text(compiled.out));
    if (compiled.exit_code != 0) {
        result.verdict = Verdict::BadInput;
        result.detail = "front end: javac: " + tf_java_errors(compile_log).substr(0, 400);
        return result;
    }
    std::vector<stdfs::path> read = tf_java_read(compile_log);
    if (read.empty()) read.push_back(stdfs::absolute(input));
    for (const stdfs::path& file : read) {
        const stdfs::path relative = file.lexically_relative(root);
        const stdfs::path copy = sources / (relative.empty() || *relative.begin() == ".." ? file.filename() : relative);
        fs::make_dirs(copy.parent_path());
        fs::write_file(copy, fs::read_file(file));
    }
    Arena arena;
    const java::frontend::DumpTools tools{*java, javac, environment.config.checkout_root() / "config" / "translate" / "frontends" / "java" / "TreeDump.java",
                                          environment.config.state(), {}};
    std::vector<ir::Node*> units = java::frontend::lower_program(arena, sources, tools);
    const JavaMain main_class = tf_java_main(units, name);
    if (main_class.simple.empty()) {
        result.verdict = Verdict::BadInput;
        result.detail = main_class.found == 0 ? "this file has no main method. CodeBreaker proves a Java translation by running the program and its translation, "
                                                "and comparing what they print; with no main there is nothing to run"
                                              : "this file has several classes with a main and none named for the file, so which one to run is not known";
        return result;
    }
    // write and build the translation; `run_it` then runs it
    std::function<dj::Run()> run_it;
    std::vector<stdfs::path> csharp_files;
    const auto native_run = [&main_class](const stdfs::path& exe) {
        process::ProcessSpec native;
        native.argv = {fs::to_text(exe)};
        native.env["JRT_MAIN"] = main_class.simple;
        return tf_java_run(std::move(native));
    };
    if (to == "c") {
        const backends::c_java::Built built = backends::c_java::build(arena, units, judged / "c");
        if (!built.exe) {
            result.verdict = built.why.starts_with("Decline:") ? Verdict::Refused : Verdict::CompileFailed;
            result.detail = built.why;
            return result;
        }
        run_it = [&native_run, exe = *built.exe] { return native_run(exe); };
    } else if (to == "cpp17") {
        const auto cxx = judge_cs_cpp::find_cxx("");
        if (!cxx) throw Error("translating Java to C++17 needs a C++17 compiler (g++, clang++ or Visual Studio's cl): none found");
        std::string text_of_program;
        try {
            const native_l2::Library library = native_l2::Library::builtin();
            text_of_program = backends::cpp17::program(library, arena, units);
        } catch (const backends::cpp17::Decline& declined) {
            result.verdict = Verdict::Refused;
            result.detail = std::string("Decline: ") + declined.what();
            return result;
        }
        const stdfs::path written = judged / "cpp" / (name + ".cpp");
        fs::make_dirs(written.parent_path());
        fs::write_text(written, text_of_program);
        const stdfs::path exe = judged / "cpp" / (name + ".exe");
        std::string log;
        if (!judge_cs_cpp::build_program(*cxx, backends::cpp17::runtime_dir(), written, exe, log)) {
            result.verdict = Verdict::CompileFailed;
            result.detail = dj::first_errors(log);
            return result;
        }
        run_it = [&native_run, exe] { return native_run(exe); };
    } else {
        const auto sdk = dotnet::Sdk::find();
        if (!sdk || !sdk->available()) throw Error("translating Java to C# needs the .NET SDK (dotnet.exe with Roslyn and the targeting pack)");
        for (ir::Node* unit : units) ir::get<ir::Unit>(unit)->lang = "java";
        const java::apimap::Registry registry = java::apimap::Registry::load(backends::cpp17::api_maps_dir());
        cs::Options options;
        options.libraries["java"] = backends::csharp_java::factory(registry);
        options.runtime_dir = environment.config.checkout_root() / "config" / "translate" / "dotnet" / "runtime";
        cs::Program written(units, options);
        csharp_files = written.write(judged / "cs");
        std::vector<std::string> why;
        for (const cs::Gap& g : written.ledger()) why.push_back(std::format("{} {}: {}", g.where, g.kind, g.why));
        for (ir::Node* unit : units)
            for (ir::Node* n : ir::unsupported(unit)) why.push_back(ir::get<ir::Unsupported>(n)->raw.value_or("a construct the front end does not read"));
        if (!why.empty()) {
            result.verdict = Verdict::Refused;
            result.detail = "declined: " + text::join(why, "; ");
            return result;
        }
        const dj::Run csharp_run = dj::csharp(dj::CSharpToolchain::of(*sdk), csharp_files, judged / "run-trans", {.main = main_class.binary});
        run_it = [csharp_run] { return csharp_run; };
    }
    translating(1, 1, {});

    checking(0, 3, "Running the Java program");
    process::ProcessSpec jvm;
    jvm.argv = {fs::to_text(*java), "-Dfile.encoding=UTF-8", "-Dstdout.encoding=UTF-8", "-cp", fs::to_text(judged / "classes"), main_class.binary};
    const dj::Run original = tf_java_run(std::move(jvm));
    checking(1, 3, "Running the translation");
    const dj::Run translation = run_it();
    checking(2, 3, "Comparing what both print");
    const dj::Judgement j = dj::compare(original, translation);
    checking(3, 3, {});
    switch (j.verdict) {
        case dj::Verdict::Equivalent: result.verdict = Verdict::Agrees; break;
        case dj::Verdict::Divergent: result.verdict = Verdict::Differs; break;
        case dj::Verdict::Refused: result.verdict = Verdict::Refused; break;
        case dj::Verdict::NoCompile: result.verdict = Verdict::CompileFailed; break;
        case dj::Verdict::NoOriginal: result.verdict = Verdict::BadInput; break;
    }
    if (result.verdict != Verdict::Agrees) {
        result.detail = std::string(dj::word(j.verdict)) + ": " + j.detail;
        return result;
    }

    if (to == "c") {
        tf_copy(judged / "c" / "program.c", out / (name + ".c"), result.files, out);
        const stdfs::path runtime = backends::c_java::default_runtime_dir();
        for (const char* file : {"jrt.h", "jrt.c"}) tf_copy(runtime / file, out / "runtime" / file, result.files, out);
        const stdfs::path note = out / "BUILD.txt";
        fs::write_text(note, "The C translation of " + name + ".java.\n\nIt builds with a C11 compiler; the runtime it needs is in runtime/.\n\n"
                             "  gcc -std=c11 -fwrapv -O1 -I runtime " + name + ".c runtime/jrt.c -o " + name + " -lm\n"
                             "  clang -std=c11 -fwrapv -O1 -I runtime " + name + ".c runtime/jrt.c -o " + name + "\n");
        result.files.push_back(stdfs::relative(note, out));
    } else if (to == "cpp17") {
        tf_write_cpp17(judged / "cpp", name, {"jrt.hpp"}, {name + ".cpp"}, out, result);
    } else {
        for (const stdfs::path& p : csharp_files) tf_copy(p, out / p.filename(), result.files, out);
    }
    const std::string_view language = to == "c" ? "C" : to == "cpp17" ? "C++17" : "C#";
    result.checked = std::format("The Java program and its {} translation printed the same output and exited with the same code.", language);
    return result;
}

}  // namespace

bool can_run(const capability::Path& path) {
    return (path.from == "csharp" && (path.to == "cpp17" || path.to == "csharp")) || (path.from == "python" && path.to == "cpp17") ||
           (path.from == "java" && (path.to == "c" || path.to == "cpp17" || path.to == "csharp"));
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
    else if (path.from == "java" && (path.to == "c" || path.to == "cpp17" || path.to == "csharp")) result = tf_java(path.to, absolute_input, out, absolute_work, environment);
    else throw Error("no way to translate " + path.from + " to " + path.to + " is wired");
    if (result.verdict != Verdict::Agrees) result.files.clear();
    return result;
}

}  // namespace cb::translate::translate_file
