#include "cb/translate/judge_py_cpp_command.hpp"

#include <algorithm>
#include <format>
#include <optional>

#include <nlohmann/json.hpp>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/json.hpp"
#include "cb/core/log.hpp"
#include "cb/core/process.hpp"
#include "cb/core/text.hpp"
#include "cb/translate/backends/cpp17.hpp"
#include "cb/translate/frontends/python/methods.hpp"
#include "cb/translate/frontends/python/package.hpp"
#include "cb/translate/native_l2.hpp"

namespace cb::translate::judge_py_cpp {

namespace stdfs = std::filesystem;
namespace pk = cb::translate::frontends::python::package;
namespace in = cb::translate::frontends::python::infer;
namespace lo = cb::translate::frontends::python::lower;
namespace me = cb::translate::frontends::python::methods;

namespace {

std::vector<std::string> lines_of(const std::string& out) {
    std::vector<std::string> lines;
    std::size_t at = 0;
    while (at < out.size()) {
        std::size_t end = out.find('\n', at);
        if (end == std::string::npos) end = out.size();
        std::string line = out.substr(at, end - at);
        at = end + 1;
        if (!line.empty() && line.back() == '\r') line.pop_back();
        if (!line.empty()) lines.push_back(std::move(line));
    }
    return lines;
}

std::string clip(const std::string& s, std::size_t n) { return s.size() > n ? s.substr(0, n) : s; }

}  // namespace

Score judge_module(const Environment& environment, const judge_cs_cpp::Cxx& cxx, const stdfs::path& file, const stdfs::path& work) {
    const std::string name = fs::to_text(file.stem());
    Score score{.program = name};
    std::error_code ec;
    stdfs::remove_all(work, ec);
    fs::make_dirs(work);
    const stdfs::path checkout = environment.config.checkout_root();

    const judge_cs_cpp::Observer& watch = environment.observer;
    const auto translating = [&watch](std::uint64_t done, std::uint64_t total, std::string_view what) {
        if (watch.translating) watch.translating(done, total, what);
    };
    const auto checking = [&watch](std::uint64_t done, std::uint64_t total, std::string_view what) {
        if (watch.checking) watch.checking(done, total, what);
    };

    // 1. the front end: every run_* function must lower (what else is refused is only listed)
    translating(0, 0, fs::to_text(file.filename()));
    const std::string source_text = fs::read_text(file);
    auto program = pk::program_of_sources({{name, source_text}});
    in::Inference inference(me::library());
    program->solve(inference);
    pk::Options options;
    options.modules = std::set<std::string>{name};
    pk::Translation done = pk::translate(*program, options);
    const lo::ModuleResult* module = done.find(name);
    if (module == nullptr) {
        score.lowers = false;
        score.detail = "front end: the module was not translated";
        return score;
    }
    std::vector<std::string> runs, why, others;
    for (const auto& [qname, node] : module->translated) {
        (void)node;
        if (qname.starts_with(name + ".run_")) runs.push_back(qname.substr(name.size() + 1));
    }
    for (const auto& [qname, refusal] : module->refused) {
        if (qname.starts_with(name + ".run_")) why.push_back(qname + ": " + refusal.reason());
        else others.push_back(qname + ": " + refusal.reason());
    }
    const std::uint64_t functions = module->translated.size() + module->refused.size();   // every function of the module, translated or not
    translating(0, functions, {});
    score.lowers = why.empty() && !runs.empty();
    if (!*score.lowers) {
        score.detail = runs.empty() && why.empty() ? "no run_* function" : "refused by the front end: " + text::join(why, "; ") + (others.empty() ? "" : " | the others refused: " + text::join(others, "; "));
        return score;
    }

    // 2. the back end: the L2 library's C++17 emitter (translate/l2, linked in)
    namespace nl = cb::translate::native_l2;
    const std::optional<nl::Library> lib = nl::Library::builtin();
    backends::cpp17::UnitText unit_text;
    try {
        unit_text = backends::cpp17::write_unit(*lib, *module->unit);
    } catch (const Error& e) {
        score.writes = false;
        score.detail = std::string("declined: ") + e.what();
        return score;
    }
    score.writes = true;
    translating(module->translated.size(), functions, {});

    // 3. the C++: the module's header and source, then a main that prints each answer as JSON (an exception as {"error": class})
    const stdfs::path cpp_dir = work / "cpp";
    fs::make_dirs(cpp_dir);
    fs::write_text(cpp_dir / (name + ".hpp"), unit_text.header);
    std::string main_text = unit_text.source;
    main_text += "\n#include \"py_json.hpp\"\n#include <cstdio>\nusing namespace " + name + ";\nint main() {\n";
    for (const std::string& r : runs) {
        main_text += "    std::printf(\"%s\\n\", py::json::attempt([&](std::string& v) { py::json::put(v, " + r + "()); }).c_str());\n";
    }
    main_text += "    return 0;\n}\n";
    const stdfs::path source = cpp_dir / (name + ".cpp");
    const stdfs::path exe = cpp_dir / (name + ".exe");
    fs::write_text(source, main_text);
    std::string log;
    checking(0, 4, "Building the C++ translation");
    score.compiles = judge_cs_cpp::build_program(cxx, checkout / "config" / "translate" / "runtime" / "cpp17", source, exe, log);
    if (!*score.compiles) {
        score.detail = "compile: " + clip(log, 700);
        return score;
    }

    // 4. the original: CPython runs the same functions
    checking(1, 4, "Running the Python module");
    const stdfs::path module_dir = work / "py";
    fs::make_dirs(module_dir);
    fs::write_text(module_dir / (name + ".py"), source_text);
    std::string driver = "import json, sys\nsys.path.insert(0, sys.argv[1])\nimport " + name + " as m\n"
                         "def attempt(f):\n    try:\n        return f()\n    except Exception as e:\n        return {'error': type(e).__name__}\n";
    for (const std::string& r : runs) driver += "print(json.dumps(attempt(m." + r + ")))\n";
    fs::write_text(module_dir / "driver.py", driver);
    process::ProcessSpec py;
    const auto python = fs::get_env("CB_PYTHON");
    py.argv = {python ? *python : std::string("python"), "-I", "-X", "utf8", fs::to_text(module_dir / "driver.py"), fs::to_text(module_dir)};
    py.timeout = std::chrono::minutes(2);
    process::ProcessResult original;
    try {
        original = process::run(py);
    } catch (const std::exception& e) {
        throw ToolError("python", 0, "", std::string("judge-py-cpp needs python on the PATH (or CB_PYTHON): ") + e.what());
    }
    checking(2, 4, "Running the translation");
    process::ProcessSpec run;
    run.argv = {fs::to_text(exe)};
    run.timeout = std::chrono::minutes(2);
    const process::ProcessResult translated = process::run(run);
    checking(3, 4, "Comparing the answers");
    const std::string want_text(as_text(original.out)), got_text(as_text(translated.out));
    fs::write_text(work / "original.out.txt", want_text);
    fs::write_text(work / "translated.out.txt", got_text);
    if (original.exit_code != 0) {
        score.detail = "original: python failed: " + clip(std::string(as_text(original.err)), 400);
        return score;
    }
    const std::vector<std::string> want = lines_of(want_text), got = lines_of(got_text);
    if (want.size() != runs.size() || got.size() != runs.size()) {
        score.agrees = false;
        score.detail = std::format("{} answers expected, python gave {}, the C++ gave {} (exit {})", runs.size(), want.size(), got.size(), translated.exit_code);
        return score;
    }
    score.agrees = true;
    for (std::size_t i = 0; i < runs.size(); ++i) {
        Json a, b;
        const bool ok = json::try_parse(want[i], a) && json::try_parse(got[i], b);
        if (!ok || a != b) {
            score.agrees = false;
            score.detail = runs[i] + ": CPython gave " + clip(want[i], 120) + ", the C++ gave " + clip(got[i], 120);
            break;
        }
    }
    checking(4, 4, {});
    return score;
}

namespace {

int run_judge(const cli::Args& args, const Environment& environment) {
    const auto cxx = judge_cs_cpp::find_cxx(args.value_or("cxx", ""));
    if (!cxx) throw ToolError("c++", 0, "", "judge-py-cpp needs a C++17 compiler (g++, clang++ or Visual Studio's cl): none found");
    const stdfs::path work = stdfs::absolute(args.value("work") ? fs::from_text(*args.value("work")) : environment.config.state() / "translate" / "judge-py-cpp");
    std::vector<stdfs::path> files;
    if (args.values("files").empty()) {
        const stdfs::path corpus = environment.config.checkout_root() / "tests" / "translate" / "corpus" / "python";
        for (const auto& e : stdfs::directory_iterator(corpus)) {
            if (e.path().extension() == ".py") files.push_back(e.path());
        }
        std::sort(files.begin(), files.end());
    } else {
        for (const std::string& g : args.values("files")) files.push_back(stdfs::absolute(fs::from_text(g)));
    }
    std::vector<Score> scores;
    for (const stdfs::path& f : files) scores.push_back(judge_module(environment, *cxx, f, work / fs::to_text(f.stem())));
    cb::out("{}", judge_cs_cpp::table(scores));
    for (const Score& s : scores) {
        if (!s.agrees.value_or(false) && !s.detail.empty()) cb::out("{}: {}\n", s.program, s.detail);   // (the table clips the detail)
    }
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
    c.name = "judge-py-cpp";
    c.summary = "Python -> C++17 judged: each module's run_* functions lowered, written, compiled and run beside CPython";
    c.area = "Translate";
    c.params = {cli::Positional{.name = "files", .help = "Python modules (default: the Python corpus)", .required = false, .variadic = true},
                cli::Option{.name = "cxx", .help = "the C++ compiler: gcc, clang, msvc or a path (default: the first of those found)", .arg = cli::Arg::Value, .value_name = "COMPILER"},
                cli::Option{.name = "work", .help = "the working folder", .arg = cli::Arg::Value, .value_name = "DIR"},
                cli::Option{.name = "keep", .help = "keep the working folder"}};
    c.handler = [environment](const cli::Args& args) { return run_judge(args, environment); };
    return c;
}

}  // namespace cb::translate::judge_py_cpp
