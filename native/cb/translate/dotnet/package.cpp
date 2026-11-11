#include "cb/translate/dotnet/package.hpp"

#include <algorithm>
#include <format>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/process.hpp"
#include "cb/core/text.hpp"

namespace cb::translate::dotnet {

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

constexpr std::string_view kStandardInstall = R"(C:\Program Files\dotnet\dotnet.exe)";
constexpr std::string_view kNoWarn = "-nowarn:CS8600,CS8601,CS8602,CS8603,CS8604,CS8618,CS8625,CS0168,CS0219,CS0162,CS8321,CS0649,CS0169";

bool is_file(const stdfs::path& p) {
    std::error_code ec;
    return stdfs::is_regular_file(p, ec);
}
bool is_dir(const stdfs::path& p) {
    std::error_code ec;
    return stdfs::is_directory(p, ec);
}

bool starts_with_digit(std::string_view s) { return !s.empty() && s.front() >= '0' && s.front() <= '9'; }

// dotnet on PATH, as a command line would find it: each folder of PATH, each extension of PATHEXT. (The current
// directory is not searched, as it is by a Windows command prompt: a dotnet.exe that happens to lie in the folder a
// tool is run from is not the toolchain.)
std::optional<stdfs::path> find_on_path() {
    const auto path_var = fs::get_env("PATH");
    if (!path_var) return std::nullopt;
    const auto pathext = fs::get_env("PATHEXT").value_or(".COM;.EXE;.BAT;.CMD");
    for (const std::string_view dir : text::split(*path_var, ';')) {
        if (dir.empty()) continue;
        for (const std::string_view ext : text::split(pathext, ';')) {
            const stdfs::path candidate = fs::from_text(dir) / fs::from_text(std::string("dotnet") + std::string(ext));
            if (is_file(candidate)) return candidate;
        }
    }
    return std::nullopt;
}

// The names in a folder; empty when it cannot be read.
std::vector<std::string> names_in(const stdfs::path& folder) {
    std::vector<std::string> out;
    std::error_code ec;
    for (stdfs::directory_iterator it(folder, ec), end; !ec && it != end; it.increment(ec))
        out.push_back(it->path().filename().generic_string());
    return out;
}

// The targeting pack's `ref/<tfm>` folder: the newest net<N>.<M> there, else the first name that starts with "net".
std::optional<std::string> target_framework_dir(const stdfs::path& base) {
    std::vector<std::string> names = names_in(base);
    std::ranges::sort(names);
    std::optional<std::string> newest, any;
    std::vector<std::uint64_t> newest_key;
    for (const std::string& n : names) {
        if (!n.starts_with("net")) continue;
        if (!any) any = n;
        const std::string_view version = std::string_view(n).substr(3);
        if (!starts_with_digit(version)) continue;
        const auto key = version_key(version);
        if (!newest || key > newest_key) {
            newest = n;
            newest_key = key;
        }
    }
    return newest ? newest : any;
}

// The environment the SDK's tools are given on top of ours.
std::map<std::string, std::string> tool_env(const std::optional<stdfs::path>& tmp) {
    std::map<std::string, std::string> env{{"DOTNET_CLI_TELEMETRY_OPTOUT", "1"},
                                           {"DOTNET_NOLOGO", "1"},
                                           {"DOTNET_SKIP_FIRST_TIME_EXPERIENCE", "1"},
                                           {"DOTNET_CLI_UI_LANGUAGE", "en"},
                                           {"DOTNET_gcServer", "0"}};
    if (tmp && !tmp->empty()) {
        fs::make_dirs(*tmp);
        env["TEMP"] = env["TMP"] = fs::to_text(*tmp);
    }
    return env;
}

std::string decode(const Bytes& b) { return text::sanitize_utf8(as_text(b)); }

// Removes a file when it goes out of scope.
struct ScopedFile {
    stdfs::path path;
    ~ScopedFile() {
        std::error_code ec;
        stdfs::remove(path, ec);
    }
};

[[noreturn]] void not_available() {
    throw ToolError("dotnet", 0, "", "the .NET SDK (compiler, targeting pack and runtime) is not installed");
}

}  // namespace

std::vector<std::uint64_t> version_key(std::string_view name) {
    std::vector<std::uint64_t> key;
    std::size_t start = 0;
    while (key.size() < 3) {
        const std::size_t end = name.find_first_of(".-", start);
        const std::string_view part = name.substr(start, end == std::string_view::npos ? end : end - start);
        std::uint64_t value = 0;
        if (!part.empty() && std::ranges::all_of(part, [](char c) { return c >= '0' && c <= '9'; })) {
            // (a number too long for 64 bits is as large as can be told apart)
            value = text::parse_int<std::uint64_t>(part).value_or(std::numeric_limits<std::uint64_t>::max());
        }
        key.push_back(value);
        if (end == std::string_view::npos) break;
        start = end + 1;
    }
    return key;
}

std::optional<std::string> newest_version_dir(const stdfs::path& folder, std::string_view prefix) {
    std::optional<std::string> best;
    std::vector<std::uint64_t> best_key;
    for (const std::string& name : names_in(folder)) {
        if (!starts_with_digit(name) || !name.starts_with(prefix)) continue;
        const auto key = version_key(name);
        if (!best || key > best_key) {   // the first of equals stays
            best = name;
            best_key = key;
        }
    }
    return best;
}

std::optional<Sdk> Sdk::find() {
    if (const auto set = fs::get_env("CB_DOTNET"); set && !set->empty() && is_file(fs::from_text(*set)))
        return at(fs::from_text(*set));
    if (const auto on_path = find_on_path()) return at(*on_path);
    if (const stdfs::path standard = fs::from_text(kStandardInstall); is_file(standard)) return at(standard);
    return std::nullopt;
}

Sdk Sdk::at(const stdfs::path& dotnet_exe) {
    Sdk sdk;
    sdk.exe_ = dotnet_exe;
    std::error_code ec;
    const stdfs::path real = stdfs::canonical(dotnet_exe, ec);
    sdk.home_ = (ec ? stdfs::absolute(dotnet_exe) : real).parent_path();

    if (const auto version = newest_version_dir(sdk.home_ / "sdk")) sdk.sdk_dir_ = sdk.home_ / "sdk" / *version;
    if (sdk.sdk_dir_) {
        const stdfs::path roslyn = *sdk.sdk_dir_ / "Roslyn" / "bincore";
        if (is_file(roslyn / "csc.dll")) sdk.roslyn_dir_ = roslyn;
    }

    const stdfs::path packs = sdk.home_ / "packs" / "Microsoft.NETCore.App.Ref";
    if (const auto pack = newest_version_dir(packs)) {
        const std::string major(text::split(*pack, '.').front());
        const auto installed = newest_version_dir(sdk.home_ / "shared" / "Microsoft.NETCore.App", major + ".");
        sdk.runtime_ = Runtime{major, installed ? *installed : *pack};
        const stdfs::path base = packs / *pack / "ref";
        if (is_dir(base)) {
            if (const auto tfm = target_framework_dir(base)) sdk.reference_dir_ = base / *tfm;
        }
    }
    return sdk;
}

stdfs::path Sdk::write_runtimeconfig(const stdfs::path& dll) const {
    if (!runtime_) not_available();
    stdfs::path path = dll;
    path.replace_extension(".runtimeconfig.json");
    fs::write_text(path, std::format(R"({{"runtimeOptions": {{"tfm": "net{}.0", "framework": {{"name": "Microsoft.NETCore.App", )"
                                     R"("version": "{}"}}, "rollForward": "LatestPatch"}}}})"
                                     "\n",
                                     runtime_->major, runtime_->version));
    return path;
}

Compiled Sdk::compile(const CompileOptions& options) const {
    if (!available()) not_available();
    fs::make_dirs(stdfs::absolute(options.out_dll).parent_path());
    ScopedFile rsp{options.out_dll};
    rsp.path.replace_extension(".rsp");

    std::vector<std::string> args{"-nologo", "-nostdlib+",
                                  std::string("-target:") + (options.target == Target::Exe ? "exe" : "library"),
                                  "-out:" + fs::to_text(options.out_dll), "-langversion:latest", "-deterministic",
                                  "-debug-", "-optimize-", "-utf8output", "-checked-"};
    if (options.nullable) args.push_back("-nullable:enable");
    if (options.allow_unsafe) args.push_back("-unsafe+");
    if (options.main) args.push_back("-main:" + *options.main);
    if (!options.warn) args.emplace_back(kNoWarn);
    std::vector<std::string> framework;
    for (const std::string& name : names_in(*reference_dir_)) {
        if (name.ends_with(".dll")) framework.push_back("-r:" + fs::to_text(*reference_dir_ / fs::from_text(name)));
    }
    std::ranges::sort(framework);
    args.insert(args.end(), framework.begin(), framework.end());
    for (const stdfs::path& r : options.references) args.push_back("-r:" + fs::to_text(r));
    for (const stdfs::path& s : options.sources) args.push_back(fs::to_text(stdfs::absolute(s)));

    std::vector<std::string> lines;
    for (const std::string& a : args) lines.push_back(contains(a, ' ') ? "\"" + a + "\"" : a);
    fs::write_text(rsp.path, text::join(lines, "\n"));

    const process::ProcessResult r = process::run({
        .argv = {fs::to_text(exe_), fs::to_text(*roslyn_dir_ / "csc.dll"), "-noconfig", "@" + fs::to_text(rsp.path)},
        .env = tool_env(options.tmp),
        .err = process::Capture::Merge,
        .timeout = std::chrono::duration_cast<std::chrono::milliseconds>(kCompileTimeout)});
    if (r.timed_out) throw TimeoutError("csc", decode(r.out));
    Compiled out;
    out.log = decode(r.out);
    out.ok = r.exit_code == 0 && is_file(options.out_dll);
    if (out.ok && options.target == Target::Exe) write_runtimeconfig(options.out_dll);
    return out;
}

RunResult Sdk::run(const stdfs::path& dll, const RunOptions& options) const {
    process::ProcessSpec spec;
    spec.argv = {fs::to_text(exe_), fs::to_text(dll)};
    spec.argv.insert(spec.argv.end(), options.args.begin(), options.args.end());
    spec.env = tool_env(options.tmp);
    spec.stdin_data = options.stdin_data;
    spec.timeout = std::chrono::duration_cast<std::chrono::milliseconds>(options.timeout);
    const process::ProcessResult r = process::run(spec);
    RunResult out;
    if (!r.timed_out) out.exit_code = r.exit_code;
    out.out = decode(r.out);
    out.err = decode(r.err);
    return out;
}

}  // namespace cb::translate::dotnet
