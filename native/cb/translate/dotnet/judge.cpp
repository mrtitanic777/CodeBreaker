#include "cb/translate/dotnet/judge.hpp"

#include <algorithm>
#include <format>
#include <set>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/process.hpp"
#include "cb/core/text.hpp"

namespace cb::translate::dotnet::judge {

namespace stdfs = std::filesystem;

namespace {

// std::string::contains is C++23; this answers for strings and for sets and maps alike
template <class Haystack, class Needle>
bool contains(const Haystack& haystack, const Needle& needle) {
    if constexpr (requires { haystack.find(needle) != Haystack::npos; })
        return haystack.find(needle) != Haystack::npos;
    else
        return haystack.contains(needle);
}

constexpr std::chrono::minutes kJavacTimeout{10};
constexpr std::size_t kRefusedShown = 400;
constexpr std::size_t kLogHead = 600;       // of a compiler's log when the translation did not compile
constexpr std::size_t kWhyTail = 400;       // of the original's log when it did not run
constexpr std::size_t kLineShown = 160;     // of a differing line
constexpr std::size_t kErrorLines = 12;
constexpr std::size_t kErrorLogTail = 800;
constexpr std::size_t kJavacLogTail = 1500;

bool is_continuation(char c) { return (static_cast<unsigned char>(c) & 0xC0) == 0x80; }

// The first `n` bytes, shortened to end between characters.
std::string_view head(std::string_view s, std::size_t n) {
    if (s.size() <= n) return s;
    while (n > 0 && is_continuation(s[n])) --n;
    return s.substr(0, n);
}

// The last `n` bytes, shortened to begin between characters.
std::string_view tail(std::string_view s, std::size_t n) {
    if (s.size() <= n) return s;
    std::size_t start = s.size() - n;
    while (start < s.size() && is_continuation(s[start])) ++start;
    return s.substr(start);
}

// A line in single quotes, with what would make the message unreadable written out.
std::string quoted(std::string_view line) {
    std::string out = "'";
    for (const char c : line) {
        switch (c) {
            case '\\': out += "\\\\"; break;
            case '\'': out += "\\'"; break;
            case '\n': out += "\\n"; break;
            case '\r': out += "\\r"; break;
            case '\t': out += "\\t"; break;
            default:
                if (static_cast<unsigned char>(c) < 0x20) out += std::format("\\x{:02x}", static_cast<unsigned>(c));
                else out += c;
        }
    }
    return out + "'";
}

std::string decode(const Bytes& b) { return text::sanitize_utf8(as_text(b)); }

std::string exit_text(const std::optional<std::uint32_t>& code) { return code ? std::to_string(*code) : "none"; }

Run timed_out_run() { return {.ok = true, .exit = std::nullopt, .out = "", .err = "timeout", .log = ""}; }

}  // namespace

std::string_view word(Verdict v) {
    switch (v) {
        case Verdict::Equivalent: return "equivalent";
        case Verdict::Divergent: return "divergent";
        case Verdict::Refused: return "refused";
        case Verdict::NoCompile: return "no-compile";
        case Verdict::NoOriginal: return "no-original";
    }
    return "";
}

std::string normalise(std::string_view output) {
    std::string out = text::replace_all(output, "\r\n", "\n");
    while (out.ends_with('\n')) out.pop_back();
    return out;
}

Judgement compare(const Run& original, const Run& translation, std::span<const std::string> refused) {
    if (!refused.empty()) {
        const std::set<std::string> distinct(refused.begin(), refused.end());   // sorted, once each
        return {Verdict::Refused, std::string(head(text::join(distinct, "; "), kRefusedShown))};
    }
    if (!original.ok || !original.exit) {
        const std::string& why = !original.log.empty() ? original.log : original.err;
        return {Verdict::NoOriginal, std::string(tail(why, kWhyTail))};
    }
    if (!translation.ok) return {Verdict::NoCompile, std::string(head(translation.log, kLogHead))};
    if (!translation.exit) return {Verdict::Divergent, "the translation did not finish (timeout)"};

    const std::string a = normalise(original.out), b = normalise(translation.out);
    if (a == b && original.exit == translation.exit) {
        const auto lines = a.empty() ? 0 : std::ranges::count(a, '\n') + 1;
        return {Verdict::Equivalent, std::format("{} lines", lines)};
    }
    const auto al = text::split(a, '\n'), bl = text::split(b, '\n');
    for (std::size_t i = 0; i < std::max(al.size(), bl.size()); ++i) {
        const std::string_view x = i < al.size() ? al[i] : "<end>", y = i < bl.size() ? bl[i] : "<end>";
        if (x != y) {
            return {Verdict::Divergent, std::format("line {}: original {}, translation {}", i + 1,
                                                    quoted(head(x, kLineShown)), quoted(head(y, kLineShown)))};
        }
    }
    return {Verdict::Divergent, std::format("exit code {}, translation {}", exit_text(original.exit),
                                            exit_text(translation.exit))};
}

std::string first_errors(std::string_view log) {
    std::vector<std::string_view> errors;
    for (const std::string_view line : text::lines(log)) {
        if (contains(line, ": error ")) errors.push_back(line);
        if (errors.size() == kErrorLines) break;
    }
    return errors.empty() ? std::string(tail(log, kErrorLogTail)) : text::join(errors, "\n");
}

void fresh_folder(const stdfs::path& work) {
    std::error_code ec;   // (a folder that cannot be removed is made use of as it is)
    stdfs::remove_all(work, ec);
    fs::make_dirs(work);
}

CSharpToolchain CSharpToolchain::of(const Sdk& sdk) {
    return {[&sdk](const CompileOptions& o) { return sdk.compile(o); },
            [&sdk](const stdfs::path& dll, const RunOptions& o) { return sdk.run(dll, o); }};
}

Run csharp(const CSharpToolchain& tools, const std::vector<stdfs::path>& files, const stdfs::path& work,
           const CSharpOptions& options) {
    fresh_folder(work);
    const stdfs::path dll = work / (options.name + ".dll");
    const stdfs::path tmp = work / "tmp";
    const Compiled compiled = tools.compile({.sources = files, .out_dll = dll, .tmp = tmp, .main = options.main});
    if (!compiled.ok) return {.log = first_errors(compiled.log)};
    const RunResult ran = tools.run(dll, {.args = options.args, .timeout = options.timeout, .tmp = tmp});
    if (!ran.exit_code) return {.ok = true, .exit = std::nullopt, .out = "", .err = "timeout", .log = compiled.log};
    return {.ok = true, .exit = ran.exit_code, .out = ran.out, .err = ran.err, .log = compiled.log};
}

Run java(const std::optional<Jdk>& jdk, const stdfs::path& src_dir, std::string_view main, const stdfs::path& work,
         const JavaOptions& options) {
    if (!jdk) return {.log = "no JDK"};
    fresh_folder(work);
    const stdfs::path classes = work / "classes";
    fs::make_dirs(classes);

    std::vector<std::string> sources;
    std::error_code ec;
    for (stdfs::recursive_directory_iterator it(src_dir, ec), end; !ec && it != end; it.increment(ec)) {
        if (it->is_regular_file() && it->path().extension() == ".java") sources.push_back(text::replace_all(fs::to_text(it->path()), "\\", "/"));
    }
    std::ranges::sort(sources);
    std::vector<std::string> listed;
    for (const std::string& s : sources) listed.push_back("\"" + s + "\"");
    const stdfs::path listing = work / "files.txt";
    fs::write_text(listing, text::join(listed, "\n"));

    std::string cp = fs::to_text(classes);
    for (const stdfs::path& entry : options.classpath) cp += ";" + fs::to_text(entry);
    const process::ProcessResult built = process::run({
        .argv = {fs::to_text(jdk->javac), "-nowarn", "-encoding", "UTF-8", "-d", fs::to_text(classes), "-cp", cp,
                 "@" + fs::to_text(listing)},
        .err = process::Capture::Merge,
        .timeout = std::chrono::duration_cast<std::chrono::milliseconds>(kJavacTimeout)});
    if (built.timed_out) throw TimeoutError("javac", decode(built.out));
    if (built.exit_code != 0) return {.log = std::string(tail(decode(built.out), kJavacLogTail))};

    process::ProcessSpec spec;
    spec.argv = {fs::to_text(jdk->jvm), "-cp", cp, std::string(main)};
    spec.argv.insert(spec.argv.end(), options.args.begin(), options.args.end());
    spec.timeout = std::chrono::duration_cast<std::chrono::milliseconds>(options.timeout);
    const process::ProcessResult ran = process::run(spec);
    if (ran.timed_out) return timed_out_run();
    return {.ok = true, .exit = ran.exit_code, .out = decode(ran.out), .err = decode(ran.err)};
}

Run cpp(const std::optional<HostBuild>& host, const stdfs::path& cpp_dir, std::string_view main_class,
        const stdfs::path& work, const CppOptions& options) {
    if (!host) return {.log = "no host C++ compiler (settings.json: host_cxx)"};
    fresh_folder(work);
    const auto library = host->library(cpp_dir, work, options.jobs);
    if (!library) return {.log = "the program did not compile into a library"};

    const std::string simple(main_class.substr(main_class.rfind('.') == std::string_view::npos ? 0 : main_class.rfind('.') + 1));
    const stdfs::path harness = work / "cb_main.cpp";
    fs::write_text(harness, std::format("#include \"stdafx.h\"\n#include \"JavaRT.h\"\n#include \"{0}.h\"\n#include \"j2x_init.h\"\n"
                                        "int main() {{\n\tj2x_initAll();\n\t{0}::main(JArray<JavaString >());\n\treturn 0;\n}}\n",
                                        simple));
    const stdfs::path exe = work / "program.exe";
    const HostBuild::Native native = host->program(cpp_dir, harness, exe, *library);
    if (!native.ok) {
        std::vector<std::string> missing;
        std::error_code ec;
        for (stdfs::directory_iterator it(cpp_dir, ec), end; !ec && it != end; it.increment(ec)) {
            const stdfs::path& p = it->path();
            if (p.extension() != ".cpp") continue;
            if (!stdfs::is_regular_file(host->objects_folder(work) / (p.stem().string() + ".o"), ec)) missing.push_back(p.filename().generic_string());
        }
        std::ranges::sort(missing);
        return {.log = native.why + (missing.empty() ? "" : "; did not compile: " + text::join(missing, ", "))};
    }

    process::ProcessSpec spec;
    spec.argv = {fs::to_text(exe)};
    spec.argv.insert(spec.argv.end(), options.args.begin(), options.args.end());
    spec.cwd = work;
    spec.timeout = std::chrono::duration_cast<std::chrono::milliseconds>(options.timeout);
    const process::ProcessResult ran = process::run(spec);
    if (ran.timed_out) return timed_out_run();
    return {.ok = true, .exit = ran.exit_code, .out = decode(ran.out), .err = decode(ran.err)};
}

}  // namespace cb::translate::dotnet::judge
