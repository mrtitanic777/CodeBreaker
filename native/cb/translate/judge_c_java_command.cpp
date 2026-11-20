#include "cb/translate/judge_c_java_command.hpp"

#include <algorithm>
#include <format>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/log.hpp"
#include "cb/core/process.hpp"
#include "cb/core/text.hpp"
#include "cb/java/apimap.hpp"
#include "cb/java/assembly.hpp"
#include "cb/java/paths.hpp"
#include "cb/java/production.hpp"
#include "cb/translate/backends/java_libc.hpp"
#include "cb/translate/backends/java_objects.hpp"
#include "cb/translate/frontends/clang/j2x.hpp"
#include "cb/translate/frontends/clang/lower.hpp"

namespace cb::translate::judge_c_java {

namespace stdfs = std::filesystem;
namespace jj = cb::translate::judge_native_jvm;

namespace {

// clang's target is fixed: LLP64 Windows x64, the data model the judge's own native build runs with.
constexpr std::string_view kTriple = "x86_64-pc-windows-msvc";
constexpr std::chrono::minutes kClangTimeout{5};
constexpr std::string_view kVsLlvm[] = {R"(C:\Program Files\Microsoft Visual Studio\18\Community\VC\Tools\Llvm\x64\bin\clang.exe)",
                                        R"(C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\Llvm\x64\bin\clang.exe)"};

bool jc_is_file(const stdfs::path& p) {
    std::error_code ec;
    return stdfs::is_regular_file(p, ec);
}

// clang as the settings (clang), Visual Studio's bundled LLVM, or PATH have it.
std::optional<stdfs::path> jc_find_clang(const cb::config::Config& config) {
    if (const auto set = config.path("clang"); set && jc_is_file(*set)) return set;
    for (const std::string_view p : kVsLlvm) {
        if (jc_is_file(fs::from_text(p))) return fs::from_text(p);
    }
    const auto path_var = fs::get_env("PATH");
    if (path_var) {
        for (const std::string_view dir : text::split(*path_var, ';')) {
            const stdfs::path candidate = fs::from_text(dir) / "clang.exe";
            if (!dir.empty() && jc_is_file(candidate)) return candidate;
        }
    }
    return std::nullopt;
}

std::vector<std::string> jc_flags(const std::string& language, const std::optional<std::string>& standard, const std::vector<std::string>& includes,
                                  const std::vector<std::string>& quote_includes, const std::vector<std::string>& defines) {
    std::vector<std::string> out = {"--target=" + std::string(kTriple)};
    if (language == "c") {
        out.insert(out.end(), {"-x", "c", "-std=" + standard.value_or("c11")});
    } else {
        out.insert(out.end(), {"-x", "c++", "-std=" + standard.value_or("c++17")});
    }
    out.insert(out.end(), {"-Wno-everything", "-D_CRT_SECURE_NO_WARNINGS", "-D_CRT_NONSTDC_NO_WARNINGS"});
    for (const std::string& d : includes) out.insert(out.end(), {"-I", d});
    for (const std::string& d : quote_includes) out.insert(out.end(), {"-iquote", d});
    for (const std::string& d : defines) out.push_back("-D" + d);
    return out;
}

std::string jc_language_of(const stdfs::path& p) { return text::lower(fs::to_text(p)).ends_with(".c") ? "c" : "c++"; }

// What the host's functions use: the registry and the library mapping the Java back end writes library calls with, and the transpiler.
struct JcState {
    cb::config::Config config;
    cb::java::JavaData data;
    cb::java::apimap::Registry registry;
    translate::backends::java_libc::LibC libc;
    std::unique_ptr<cb::java::assembly::Assembly> assembly;
    JcState(cb::config::Config c, cb::java::JavaData d, cb::java::apimap::Registry r)
        : config(std::move(c)), data(std::move(d)), registry(std::move(r)), libc(registry), assembly(cb::java::production::make_assembly(config, data)) {}
};

std::string jc_clip(const std::string& s, std::size_t n) { return s.size() > n ? s.substr(0, n) : s; }

}  // namespace

ProductionHost make_host(const Environment& environment) {
    const cb::config::Config& config = environment.config;
    const stdfs::path data_root = config.checkout_root() / "config";
    auto state = std::make_shared<JcState>(config, cb::java::JavaData(data_root / "java"), cb::java::apimap::Registry::load(data_root / "java" / "api_maps"));

    jj::Host host;
    if (const auto java = cb::java::production::find_java(config)) {
        host.jdk.java = *java;
        const stdfs::path sibling = java->parent_path() / "javac.exe";
        host.jdk.javac = jc_is_file(sibling) ? sibling : stdfs::path("javac");
    }
    host.runtime_sources = data_root / "translate" / "backends" / "java_runtime";
    host.state_folder = config.state() / "translate";
    host.java_options.library = &state->libc;
    host.java_options.idiomatic = translate::backends::java_objects::factory(&state->libc);
    host.clang = [state] { return jc_find_clang(state->config); };
    host.language_of = jc_language_of;
    host.flags = jc_flags;
    host.lower = [state](Arena& arena, const jj::LoweringRequest& request) -> ir::Node* {
        const auto clang = jc_find_clang(state->config);
        if (!clang) throw ToolError("clang", 0, "", "the C/C++ front end needs clang (settings.json: clang)");
        std::vector<std::string> argv = {fs::to_text(*clang)};
        for (std::string& f : jc_flags(request.language, request.standard, request.includes, request.quote_includes, request.defines)) argv.push_back(std::move(f));
        argv.insert(argv.end(), {"-Xclang", "-ast-dump=json", "-fsyntax-only", fs::to_text(request.path)});
        const process::ProcessResult r = process::run({.argv = argv, .timeout = std::chrono::duration_cast<std::chrono::milliseconds>(kClangTimeout)});
        if (r.exit_code != 0) {
            const std::string err(r.err.begin(), r.err.end());
            throw ToolError("clang", r.exit_code, err, "clang: " + (err.size() > 1500 ? err.substr(err.size() - 1500) : err));
        }
        translate::frontends::clang::Options options;
        options.main_path = fs::to_text(request.path);
        if (request.own) {
            std::vector<std::string> own;
            for (const stdfs::path& p : *request.own) own.push_back(fs::to_text(p));
            options.own = std::move(own);
        }
        options.language = request.language;
        options.standard = request.standard;
        options.triple = std::string(kTriple);
        const std::string dump(r.out.begin(), r.out.end());
        return translate::frontends::clang::lower_text(arena, dump, options).unit;
    };
    // the Java lane writes the classes as C++03, no platform; what it declined is a note
    host.java_to_cpp = [state](const stdfs::path& src, const stdfs::path& cpp_out) {
        const cb::java::transpile::Summary done = state->assembly->converter("C++03")(src, cpp_out);
        std::vector<std::string> reasons;
        for (const auto& [why, count] : done.declined) reasons.push_back(std::format("{}: {}", why, count));
        return text::join(reasons, "; ");
    };
    translate::frontends::clang::j2x::bind(host);
    return {std::move(host), state};
}

RoundTripInputs default_inputs(const Environment& environment, const stdfs::path& /*staging: the classes are kept as files now*/) {
    const stdfs::path root = environment.config.checkout_root();
    RoundTripInputs inputs;
    // the fixture classes, the clang front end's round-trip program, and Base and Sub (the C++03 back end's test classes, package `sample`)
    const stdfs::path data = root / "config" / "translate" / "judge_c_java";
    for (const char* name : {"Vec3D", "MathHelper", "AxisAlignedBB"}) inputs.files.push_back(data / (std::string(name) + ".java"));
    inputs.files.push_back(root / "native" / "cb" / "translate" / "frontends" / "clang" / "testdata" / "roundtrip" / "RoundTrip.java");
    for (const char* name : {"Base", "Sub"}) inputs.files.push_back(data / (std::string(name) + ".java"));
    inputs.main_class = "sample.RoundTrip";
    return inputs;
}

namespace {

std::string jc_row(const std::string& program, const jj::Verdict& v) {
    const std::string reason = text::replace_all(text::replace_all(jc_clip(v.reason, 220), "|", "\\|"), "\n", " ");
    return std::format("| {} | {} | {} | {} |", program, jj::word(v.verdict), v.tier.value_or(""), reason);
}

int jc_roundtrip(const cli::Args& args, const Environment& environment) {
    const stdfs::path work = args.value("work") ? fs::from_text(*args.value("work")) : environment.config.state() / "translate" / "judge-c-java" / "roundtrip";
    std::vector<stdfs::path> files;
    std::string main_class = args.value_or("main", "");
    std::string program;
    for (const std::string& f : args.values("files")) files.push_back(fs::from_text(f));
    if (files.empty()) {
        RoundTripInputs inputs = default_inputs(environment, work.parent_path() / "roundtrip-in");
        files = std::move(inputs.files);
        if (main_class.empty()) main_class = inputs.main_class;
        program = "Java -> C++ -> Java: Vec3D, MathHelper, AxisAlignedBB, Base, Sub, RoundTrip";
    } else {
        if (main_class.empty()) throw UsageError("roundtrip: name the class whose main runs with --main when files are given");
        std::vector<std::string> names;
        for (const stdfs::path& f : files) names.push_back(fs::to_text(f.stem()));
        program = "Java -> C++ -> Java: " + text::join(names, ", ");
    }
    const ProductionHost made = make_host(environment);
    const jj::Verdict v = jj::roundtrip(made.host, files, main_class, work);
    const std::string head = std::string("Round trip: ") + std::string(jj::word(v.verdict));
    cb::out("{}\n| program | verdict | tier | reason |\n|---|---|---|---|\n{}\n", head, jc_row(program, v));
    if (!args.flag("keep")) {
        std::error_code ec;
        stdfs::remove_all(work, ec);
    }
    return v.verdict == jj::Kind::Equivalent ? 0 : 1;
}

}  // namespace

cli::Command command(const Environment& environment) {
    cli::Command roundtrip;
    roundtrip.name = "roundtrip";
    roundtrip.summary = "Java -> C++ -> Java, judged on the JVM";
    roundtrip.params = {cli::Positional{.name = "files", .help = "Java files of the program (default: the Java lane's fixtures and the driver)", .required = false, .variadic = true},
                        cli::Option{.name = "main", .help = "the qualified class whose main runs (default sample.RoundTrip with the default files)", .arg = cli::Arg::Value, .value_name = "CLASS"},
                        cli::Option{.name = "work", .help = "the working folder", .arg = cli::Arg::Value, .value_name = "DIR"},
                        cli::Option{.name = "keep", .help = "keep the working folder"}};
    roundtrip.handler = [environment](const cli::Args& args) { return jc_roundtrip(args, environment); };
    cli::Command c;
    c.name = "judge-c-java";
    c.summary = "the Java -> C++ -> Java round trip judged (roundtrip)";
    c.area = "Translate";
    c.subcommands.push_back(std::move(roundtrip));
    return c;
}

int main(const std::vector<std::string>& args, const Environment& environment) {
    cli::Cli cli("codebreaker", {command(environment)});
    std::vector<std::string> full = {"judge-c-java"};
    full.insert(full.end(), args.begin(), args.end());
    return cli.run(full);
}

}  // namespace cb::translate::judge_c_java
