// cb-unity: alone (file-local helpers named for this module)
#include "cb/translate/judge_native_jvm.hpp"

#include <algorithm>
#include <format>

#include "cb/core/crypto.hpp"
#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/process.hpp"
#include "cb/core/text.hpp"

namespace cb::translate::judge_native_jvm {

namespace stdfs = std::filesystem;
namespace jback = backends::java;

namespace {

constexpr std::chrono::seconds kCompileTimeout{300};
constexpr std::size_t kMessageLimit = 300;   // how much of a message goes into a reason

std::string clipped(std::string_view s, std::size_t n = kMessageLimit) { return std::string(s.substr(0, n)); }

std::string tail(std::string_view s, std::size_t n) { return std::string(s.size() > n ? s.substr(s.size() - n) : s); }

std::string text_of(const Bytes& b) { return text::from_latin1(b); }

// A tool of the judge: its output as text, stdout then stderr.
struct ToolRun {
    int exit_code = 0;
    std::string out, err;
};
ToolRun run_tool(const std::vector<std::string>& argv, const std::optional<stdfs::path>& cwd = std::nullopt) {
    const process::ProcessResult r = process::run({.argv = argv, .cwd = cwd, .timeout = kCompileTimeout});
    return {r.timed_out ? -1 : static_cast<int>(r.exit_code), text_of(r.out), text_of(r.err)};
}

std::string quoted(ByteView bytes) {
    std::string out = "\"";
    for (const std::uint8_t b : bytes) {
        switch (b) {
            case '\n': out += "\\n"; break;
            case '\r': out += "\\r"; break;
            case '\t': out += "\\t"; break;
            case '"': out += "\\\""; break;
            case '\\': out += "\\\\"; break;
            default:
                if (b >= 0x20 && b < 0x7F) out += static_cast<char>(b);
                else out += std::format("\\x{:02x}", b);
        }
    }
    return out + "\"";
}

Bytes unix_line_ends(const Bytes& b) {
    Bytes out;
    out.reserve(b.size());
    for (std::size_t i = 0; i < b.size(); ++i) {
        if (b[i] == '\r' && i + 1 < b.size() && b[i + 1] == '\n') continue;
        out.push_back(b[i]);
    }
    return out;
}

ByteView window(const Bytes& b, std::size_t from, std::size_t to) {
    from = std::min(from, b.size());
    to = std::min(std::max(to, from), b.size());
    return ByteView(b).subspan(from, to - from);
}

bool is_source_file(const stdfs::path& p) {
    const std::string ext = text::lower(fs::to_text(p.extension()));
    return ext == ".c" || ext == ".cpp" || ext == ".cc" || ext == ".cxx";
}

bool is_header(const stdfs::path& p) {
    const std::string ext = text::lower(fs::to_text(p.extension()));
    return ext == ".h" || ext == ".hpp";
}

// "front end: <kind> at <file>:<line> (<raw>)" for the first construct left unsupported in `units`, or "".
std::string first_unsupported(std::span<ir::Node* const> units, bool with_file) {
    for (ir::Node* unit : units) {
        const std::vector<ir::Node*> bad = ir::unsupported(unit);
        if (bad.empty()) continue;
        const ir::Unsupported* x = ir::get<ir::Unsupported>(bad.front());
        const ir::Unit* u = ir::get<ir::Unit>(unit);
        const std::string kind = x->kind.value_or("?");
        const std::string where = with_file ? std::format("{}:{}", u != nullptr && u->path ? fs::to_text(fs::from_text(*u->path).filename()) : std::string(), bad.front()->line)
                                            : std::format("line {}", bad.front()->line);
        return std::format("front end: {} at {}{}", kind, where, x->raw ? " (" + *x->raw + ")" : std::string());
    }
    return {};
}

void write_lf(const stdfs::path& path, const std::string& text_value) { fs::write_file(path, as_bytes(text_value)); }

// javac on `sources`, classes into `out`: the exit code and what it printed.
ToolRun javac(const Host& host, const std::vector<std::string>& options, const std::vector<stdfs::path>& sources) {
    std::vector<std::string> argv = {fs::to_text(host.jdk.javac), "-nowarn", "-encoding", "UTF-8"};
    argv.insert(argv.end(), options.begin(), options.end());
    for (const stdfs::path& s : sources) argv.push_back(fs::to_text(s));
    return run_tool(argv);
}

std::string path_list(const stdfs::path& first, const stdfs::path& second) { return fs::to_text(first) + ";" + fs::to_text(second); }   // Windows' path separator

}  // namespace

std::string_view word(Kind kind) {
    switch (kind) {
        case Kind::Equivalent: return "equivalent";
        case Kind::Divergent: return "divergent";
        case Kind::Refused: return "refused";
        case Kind::NoCompile: return "no-compile";
        case Kind::NoOriginal: return "no-original";
    }
    return "";
}

std::string Verdict::describe() const { return std::format("{}: {}{}", name, word(verdict), reason.empty() ? std::string() : " -- " + reason); }

stdfs::path runtime_classes(const Host& host) {
    const stdfs::path source = host.runtime_sources / "crt";
    std::vector<stdfs::path> files;
    std::error_code ec;
    for (const auto& entry : stdfs::directory_iterator(source, ec)) {
        if (entry.is_regular_file() && entry.path().extension() == ".java") files.push_back(entry.path());
    }
    std::sort(files.begin(), files.end());
    Bytes key = to_bytes(fs::to_text(host.jdk.javac));   // the folder is named by the JDK and the sources, so either changing compiles again
    for (const stdfs::path& f : files) append(key, fs::read_file(f));
    const stdfs::path folder = host.state_folder / ("java-crt-" + crypto::to_hex(crypto::sha1(key)).substr(0, 12));
    if (stdfs::is_regular_file(folder / "crt" / "LibC.class", ec)) return folder;
    fs::make_dirs(folder);
    const ToolRun r = javac(host, {"--release", "17", "-d", fs::to_text(folder)}, files);
    if (r.exit_code != 0) {
        stdfs::remove_all(folder, ec);
        throw ToolError("javac", r.exit_code, r.out + r.err, "the C runtime for Java does not compile");
    }
    return folder;
}

Run run_program(const std::vector<std::string>& argv, ByteView input, const stdfs::path& cwd, std::chrono::seconds timeout) {
    const process::ProcessResult r = process::run({.argv = argv, .cwd = cwd, .stdin_data = Bytes(input.begin(), input.end()), .timeout = timeout});
    if (r.timed_out) return {{}, to_bytes("(timed out)"), std::nullopt};
    return {r.out, r.err, static_cast<std::int64_t>(r.exit_code)};
}

Compiled build_native(const Host& host, std::span<const stdfs::path> files, const stdfs::path& out_exe, const std::string& language, const std::optional<std::string>& standard,
                      const std::vector<std::string>& defines, const std::vector<std::string>& includes) {
    const std::optional<stdfs::path> clang = host.clang ? host.clang() : std::nullopt;
    if (!clang) return {false, "no clang was found"};
    std::vector<std::string> argv = {fs::to_text(*clang)};
    for (std::string& flag : host.flags(language, standard, includes, {}, defines)) argv.push_back(std::move(flag));
    argv.push_back("-O0");
    argv.push_back("-o");
    argv.push_back(fs::to_text(out_exe));
    for (const stdfs::path& f : files) argv.push_back(fs::to_text(f));
    const ToolRun r = run_tool(argv);
    return {r.exit_code == 0, tail(r.err.empty() ? r.out : r.err, 1500)};
}

jback::Program translate(const Host& host, Arena& arena, std::span<const stdfs::path> files, const std::string& name, const std::optional<std::string>& language,
                         const std::optional<std::string>& standard, const std::vector<std::string>& defines, const std::vector<std::string>& includes) {
    std::vector<ir::Node*> units;
    for (const stdfs::path& f : files) {
        if (!is_source_file(f)) continue;
        LoweringRequest request;
        request.path = f;
        request.language = language ? *language : host.language_of(f);
        request.standard = standard;
        request.includes = includes;
        request.defines = defines;
        units.push_back(host.lower(arena, request));
    }
    if (const std::string bad = first_unsupported(units, true); !bad.empty()) throw jback::Refused(bad);
    return host.write_java ? host.write_java(units, name) : jback::program(units, name, host.java_options);
}

std::optional<std::string> compare(const Run& native, const Run& jvm) {
    const Bytes x = unix_line_ends(native.out), y = unix_line_ends(jvm.out);
    if (x != y) {
        std::size_t i = 0;
        while (i < std::min(x.size(), y.size()) && x[i] == y[i]) ++i;
        const std::size_t line = static_cast<std::size_t>(std::count(x.begin(), x.begin() + static_cast<std::ptrdiff_t>(std::min(i, x.size())), '\n')) + 1;
        const std::size_t from = i > 20 ? i - 20 : 0;
        return std::format("stdout differs at line {}: C {}, Java {}", line, quoted(window(x, from, i + 30)), quoted(window(y, from, i + 30)));
    }
    if (native.exit_code != jvm.exit_code) {
        const auto code = [](const Run& r) { return r.exit_code ? std::to_string(*r.exit_code) : std::string("timeout"); };
        return std::format("exit code: C {}, Java {}{}", code(native), code(jvm), jvm.err.empty() ? std::string() : " (" + first_error(text_of(jvm.err)) + ")");
    }
    const Bytes ae = unix_line_ends(native.err), be = unix_line_ends(jvm.err);
    if (ae != be) return std::format("stderr differs: C {}, Java {}", quoted(window(ae, 0, 120)), quoted(window(be, 0, 120)));
    return std::nullopt;
}

std::string first_error(std::string_view text_value) {
    for (const std::string_view line : text::lines(text_value)) {
        const std::string lowered = text::lower(line);
        if (lowered.find("error") != std::string::npos || lowered.find("exception") != std::string::npos) return clipped(text::trim(line));
    }
    const std::string_view stripped = text::trim(text_value);
    if (stripped.empty()) return {};
    return clipped(text::trim(text::lines(stripped).front()));
}

std::string class_name(std::string_view stem) {
    std::string n;
    for (const char c : stem) n += ((c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_') ? c : '_';
    if (n.empty() || (n[0] >= '0' && n[0] <= '9')) n = "P" + n;
    if (n[0] >= 'a' && n[0] <= 'z') n[0] = static_cast<char>(n[0] - 'a' + 'A');
    return n;
}

Verdict judge(const Host& host, std::span<const stdfs::path> files_in, const stdfs::path& work, const JudgeOptions& options) {
    std::vector<stdfs::path> files;
    for (const stdfs::path& f : files_in) files.push_back(stdfs::absolute(f));
    const std::string language = options.language ? *options.language : host.language_of(files.front());
    const std::string name = options.name ? *options.name : class_name(fs::to_text(files.front().stem()));
    fs::make_dirs(work);
    const stdfs::path exe = work / (name + ".exe");
    std::vector<stdfs::path> sources;
    for (const stdfs::path& f : files) {
        if (!is_header(f)) sources.push_back(f);
    }
    const Compiled original = build_native(host, sources, exe, language, options.standard, options.defines, options.includes);
    Verdict v;
    v.name = name;
    if (!original.ok) {
        v.verdict = Kind::NoOriginal;
        v.reason = "clang: " + first_error(original.message);
        return v;
    }
    std::vector<std::string> native_argv = {fs::to_text(exe)};
    native_argv.insert(native_argv.end(), options.args.begin(), options.args.end());
    const Run nat = run_program(native_argv, options.stdin_data, work);
    v.native = nat;

    Arena arena;
    std::optional<jback::Program> program;
    try {
        program = translate(host, arena, files, name, options.language, options.standard, options.defines, options.includes);
    } catch (const jback::Refused& e) {
        v.verdict = Kind::Refused;
        v.reason = e.what();
        return v;
    } catch (const ToolError& e) {   // the front end could not read a file
        v.verdict = Kind::Refused;
        v.reason = "front end: " + clipped(e.what());
        return v;
    }
    const stdfs::path jdir = work / "java";
    std::error_code ec;
    stdfs::remove_all(jdir, ec);
    fs::make_dirs(jdir);
    const stdfs::path source = jdir / (name + ".java");
    write_lf(source, program->source);
    const stdfs::path crt = runtime_classes(host);
    const stdfs::path classes = jdir / "classes";
    fs::make_dirs(classes);
    v.tier = std::string(jback::to_text(program->tier));
    v.java = program->source;
    v.notes = program->notes;
    const ToolRun compiled = javac(host, {"--release", "17", "-cp", fs::to_text(crt), "-d", fs::to_text(classes)}, {source});
    if (compiled.exit_code != 0) {
        v.verdict = Kind::NoCompile;
        v.reason = "javac: " + first_error(compiled.out + compiled.err);
        return v;
    }
    // (argv[0] is where the program runs from: the native one's own path, given to both)
    std::vector<std::string> jvm_argv = {fs::to_text(host.jdk.java), "-Xss64m", "-Dc.argv0=" + fs::to_text(exe), "-cp", path_list(classes, crt), name};
    jvm_argv.insert(jvm_argv.end(), options.args.begin(), options.args.end());
    const Run jvm = run_program(jvm_argv, options.stdin_data, work);
    v.jvm = jvm;
    const std::optional<std::string> difference = compare(nat, jvm);
    std::vector<std::string> gaps;
    for (const std::string& note : program->notes) {
        if (note.starts_with("gap: ")) gaps.push_back(note.substr(5));
    }
    const std::string jvm_err = text_of(jvm.err);
    if (difference && jvm_err.find("CodeBreaker gap: ") != std::string::npos) {   // the run reached a part the translator declined: refused, with that part's reason
        const std::string_view after = std::string_view(jvm_err).substr(jvm_err.find("CodeBreaker gap: ") + 17);
        const auto first_line = text::lines(after);
        v.verdict = Kind::Refused;
        v.reason = "gap reached: " + std::string(first_line.empty() ? std::string_view() : first_line.front());
        return v;
    }
    std::string reason = difference.value_or("");
    if (!gaps.empty()) reason += (reason.empty() ? "" : "; ") + std::string("gaps: ") + text::join(gaps, "; ");   // a gap the run did not reach still says it is there
    v.verdict = difference ? Kind::Divergent : Kind::Equivalent;
    v.reason = reason;
    return v;
}

namespace {

// The package a Java source declares (`package a.b;`), "" for none.
std::string package_of(std::string_view source_text) {
    for (const std::string_view raw : text::lines(source_text)) {
        std::string_view line = text::ltrim(raw);
        if (!line.starts_with("package") || line.size() <= 7 || !text::is_space(line[7])) continue;
        line = text::ltrim(line.substr(7));
        std::size_t n = 0;
        while (n < line.size() && ((line[n] >= '0' && line[n] <= '9') || (line[n] >= 'a' && line[n] <= 'z') || (line[n] >= 'A' && line[n] <= 'Z') || line[n] == '_' || line[n] == '.')) ++n;
        if (n > 0 && text::ltrim(line.substr(n)).starts_with(';')) return std::string(line.substr(0, n));
    }
    return {};
}

}  // namespace

Verdict roundtrip(const Host& host, std::span<const stdfs::path> java_files, const std::string& main_class, const stdfs::path& work, const std::string& name) {
    std::error_code ec;
    stdfs::remove_all(work, ec);   // the judge's own folder: it starts empty
    const stdfs::path src = work / "src";
    std::vector<std::string> classes;
    for (const stdfs::path& f : java_files) {
        const std::string content = fs::read_text(f);
        const std::string package = package_of(content);
        stdfs::path dir = src;
        if (!package.empty()) {
            for (const std::string_view part : text::split(package, '.')) dir /= std::string(part);
        }
        fs::make_dirs(dir);
        write_lf(dir / f.filename(), content);
        classes.push_back(fs::to_text(f.stem()));
    }
    Verdict v;
    v.name = name;
    // the original, on the JVM
    const stdfs::path orig = work / "orig";
    fs::make_dirs(orig);
    std::vector<stdfs::path> staged;
    for (const auto& entry : stdfs::recursive_directory_iterator(src, ec)) {
        if (entry.is_regular_file() && entry.path().extension() == ".java") staged.push_back(entry.path());
    }
    const ToolRun original = javac(host, {"-d", fs::to_text(orig)}, staged);
    if (original.exit_code != 0) {
        v.verdict = Kind::NoOriginal;
        v.reason = "javac: " + first_error(original.out + original.err);
        return v;
    }
    const Run nat = run_program({fs::to_text(host.jdk.java), "-cp", fs::to_text(orig), main_class}, {}, work);
    v.native = nat;
    // Java -> C++ (the Java lane)
    const stdfs::path cpp = work / "cpp";
    std::string declined;
    try {
        declined = host.java_to_cpp(src, cpp);
    } catch (const Error& e) {
        v.verdict = Kind::Refused;
        v.reason = "Java -> C++: " + clipped(e.what());
        return v;
    }
    // C++ -> L2 (clang) -> Java
    Arena arena;
    std::optional<jback::Program> program;
    try {
        std::vector<ir::Node*> units;
        for (const std::string& c : classes) {
            LoweringRequest request;
            request.path = cpp / (c + ".cpp");
            request.language = "c++";
            request.standard = "c++14";
            request.quote_includes = {fs::to_text(cpp)};
            request.own = std::vector<stdfs::path>{cpp / (c + ".cpp"), cpp / (c + ".h")};
            units.push_back(host.lower(arena, request));
        }
        host.lift_java_runtime(arena, units);
        if (const std::string bad = first_unsupported(units, false); !bad.empty()) throw jback::Refused(bad);
        program = host.write_java ? host.write_java(units, name) : jback::program(units, name, host.java_options);
    } catch (const jback::Refused& e) {
        v.verdict = Kind::Refused;
        v.reason = std::string("C++ -> Java: ") + e.what();
        return v;
    } catch (const ToolError& e) {
        v.verdict = Kind::Refused;
        v.reason = "C++ -> Java: front end: " + clipped(e.what());
        return v;
    }
    const stdfs::path jdir = work / "java";
    fs::make_dirs(jdir);
    const stdfs::path source = jdir / (name + ".java");
    write_lf(source, program->source);
    const stdfs::path crt = runtime_classes(host);
    const stdfs::path out_classes = jdir / "classes";
    fs::make_dirs(out_classes);
    v.tier = std::string(jback::to_text(program->tier));
    v.java = program->source;
    v.notes = program->notes;
    if (!declined.empty()) v.notes.push_back("Java -> C++ declined bodies: " + declined);
    const ToolRun compiled = javac(host, {"--release", "17", "-cp", fs::to_text(crt), "-d", fs::to_text(out_classes)}, {source});
    if (compiled.exit_code != 0) {
        v.verdict = Kind::NoCompile;
        v.reason = "javac: " + first_error(compiled.out + compiled.err);
        return v;
    }
    const Run jvm = run_program({fs::to_text(host.jdk.java), "-cp", path_list(out_classes, crt), name}, {}, work);
    v.jvm = jvm;
    const std::optional<std::string> difference = compare(nat, jvm);
    v.verdict = difference ? Kind::Divergent : Kind::Equivalent;
    v.reason = difference.value_or("");
    return v;
}

}  // namespace cb::translate::judge_native_jvm
