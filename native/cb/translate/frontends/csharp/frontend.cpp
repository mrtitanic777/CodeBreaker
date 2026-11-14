#include "cb/translate/frontends/csharp/frontend.hpp"

#include <algorithm>
#include <format>

#include "cb/core/crypto.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/parallel.hpp"
#include "cb/core/text.hpp"

namespace cb::translate::frontends::csharp {

namespace stdfs = std::filesystem;
namespace dn = cb::translate::dotnet;

namespace {

constexpr std::string_view kRoslynDlls[] = {"Microsoft.CodeAnalysis.dll", "Microsoft.CodeAnalysis.CSharp.dll"};
constexpr std::chrono::minutes kToolCompileTimeout{5};

// What identifies one build of a Roslyn assembly: its name, size and time.
void add_file_identity(Bytes& key, const stdfs::path& p) {
    std::error_code ec;
    const auto size = stdfs::file_size(p, ec);
    const auto time = stdfs::last_write_time(p, ec).time_since_epoch().count();
    const std::string text = std::format("{}|{}|{}\n", fs::to_text(p.filename()), size, time);
    key.insert(key.end(), text.begin(), text.end());
}

std::string tail(std::string_view s, std::size_t n) { return std::string(s.size() > n ? s.substr(s.size() - n) : s); }

Diagnostic diagnostic_of(const Json& j) {
    Diagnostic d;
    d.kind = j.value("kind", std::string());
    d.file = j.value("file", std::string());
    d.line = j.value("line", 0u);
    d.message = j.value("message", std::string());
    return d;
}

}  // namespace

std::vector<Diagnostic> Dumped::errors() const {
    std::vector<Diagnostic> out;
    for (const Diagnostic& d : diagnostics) {
        if (d.kind == "ERROR") out.push_back(d);
    }
    return out;
}

stdfs::path Frontend::tool() const {
    const auto roslyn = sdk_.roslyn_dir();
    if (!sdk_.available() || !roslyn) throw ToolError("dotnet", 0, "", "the C# front end needs the .NET SDK (dotnet.exe with Roslyn and the targeting pack)");
    Bytes key = fs::read_file(source_);
    for (const std::string_view n : kRoslynDlls) add_file_identity(key, *roslyn / fs::from_text(n));
    add_file_identity(key, *roslyn / "csc.dll");
    const std::string runtime = sdk_.runtime()->major + "/" + sdk_.runtime()->version;
    key.insert(key.end(), runtime.begin(), runtime.end());
    const std::string hash = crypto::to_hex(crypto::sha1(key)).substr(0, 12);
    const stdfs::path parent = state_ / "translate";
    const stdfs::path folder = parent / ("csdump-" + hash);
    const stdfs::path dll = folder / "CsTreeDump.dll";
    std::error_code ec;
    if (stdfs::is_regular_file(dll, ec)) return dll;

    fs::make_dirs(folder);
    std::vector<stdfs::path> references;
    for (const std::string_view n : kRoslynDlls) {   // beside the tool, as the runtime loads them
        const stdfs::path to = folder / fs::from_text(n);
        stdfs::copy_file(*roslyn / fs::from_text(n), to, stdfs::copy_options::overwrite_existing);
        references.push_back(to);
    }
    const dn::Compiled built = sdk_.compile({.sources = {source_}, .out_dll = dll, .references = references, .nullable = false,
                                             .tmp = folder / "tmp"});
    if (!built.ok) {
        stdfs::remove_all(folder, ec);
        throw ToolError("csc", 1, built.log, "CsTreeDump did not compile: " + tail(built.log, 600));
    }
    for (stdfs::directory_iterator it(parent, ec), end; !ec && it != end; it.increment(ec)) {   // (those of another source or SDK: never used again)
        const std::string name = fs::to_text(it->path().filename());
        if (name.starts_with("csdump-") && it->path() != folder) {
            std::error_code gone;
            stdfs::remove_all(it->path(), gone);
        }
    }
    return dll;
}

std::size_t Frontend::run(const std::vector<stdfs::path>& files, const stdfs::path& out_dir, const std::optional<std::string>& langversion) const {
    const stdfs::path dll = tool();
    std::error_code ec;
    stdfs::remove_all(out_dir, ec);
    fs::make_dirs(out_dir);
    const stdfs::path listing = out_dir / "_files.txt";
    std::vector<std::string> names;
    for (const stdfs::path& f : files) names.push_back(text::replace_all(fs::to_text(stdfs::absolute(f).lexically_normal()), "/", "\\"));   // (as the platform spells a path: the dump names the file so)
    fs::write_text(listing, text::join(names, "\n"));

    std::vector<std::string> args = {"--out", fs::to_text(out_dir), "--refs", fs::to_text(*sdk_.reference_dir())};
    if (langversion) {
        args.push_back("--langversion");
        args.push_back(*langversion);
    }
    args.push_back("@" + fs::to_text(listing));
    const dn::RunResult r = sdk_.run(dll, {.args = args, .timeout = std::chrono::duration_cast<std::chrono::seconds>(kDumpTimeout), .tmp = state_ / "translate" / "tmp"});
    if (!r.exit_code) throw TimeoutError("CsTreeDump", r.err);
    if (*r.exit_code != 0) throw ToolError("CsTreeDump", *r.exit_code, r.err, "failed: " + tail(r.err, 800));
    if (r.out.starts_with("dumped ")) {
        std::size_t n = 0;
        for (const char c : std::string_view(r.out).substr(7)) {
            if (c < '0' || c > '9') break;
            n = n * 10 + static_cast<std::size_t>(c - '0');
        }
        return n;
    }
    return 0;
}

Dumped Frontend::load(const stdfs::path& out_dir) {
    Dumped out;
    std::vector<std::pair<std::string, stdfs::path>> found;
    std::error_code ec;
    for (stdfs::directory_iterator it(out_dir, ec), end; !ec && it != end; it.increment(ec)) {
        const std::string name = fs::to_text(it->path().filename());
        if (it->path().extension() == ".json" && !name.starts_with("_")) found.emplace_back(name.substr(0, name.size() - 5), it->path());
    }
    std::ranges::sort(found);
    out.files.resize(found.size());   // (a dump is megabytes of JSON for a real program: the files are read side by side)
    parallel::parallel_for(found.size(), [&](std::size_t i) { out.files[i] = {found[i].first, json::load(found[i].second)}; });
    const stdfs::path diag = out_dir / "_diagnostics.json";
    if (stdfs::is_regular_file(diag, ec)) {
        for (const Json& d : json::load(diag)) out.diagnostics.push_back(diagnostic_of(d));
    }
    return out;
}

Dumped Frontend::dump(const std::vector<stdfs::path>& files, const stdfs::path& out_dir, const std::optional<std::string>& langversion) const {
    run(files, out_dir, langversion);
    return load(out_dir);
}

Program Frontend::lower(Arena& arena, const std::vector<stdfs::path>& files, const stdfs::path& work) const {
    const Dumped dumped = dump(files, work / "dump");
    if (const auto errors = dumped.errors(); !errors.empty())
        throw CompileError(std::format("C# did not compile: {}:{} {}", errors[0].file, errors[0].line, errors[0].message));
    Program program;
    for (const DumpedFile& f : dumped.files) {
        Lowered one = csharp::lower(arena, f.tree, f.name);
        program.units.push_back(one.unit);
        for (const Refusal& r : one.refused)
            program.refused.push_back({std::format("{}:{}", f.name, r.line), r.kind.value_or(""), r.why});
    }
    return program;
}

std::vector<stdfs::path> sources(const stdfs::path& src) {
    std::error_code ec;
    if (stdfs::is_regular_file(src, ec)) return {src};
    std::vector<stdfs::path> out;
    for (stdfs::recursive_directory_iterator it(src, ec), end; !ec && it != end; it.increment(ec)) {
        if (it->is_regular_file() && it->path().extension() == ".cs") out.push_back(it->path());
    }
    // by the text of the path as the platform spells it (a folder's files are not kept together: "a.cs" sorts before "a\b.cs")
    std::ranges::sort(out, [](const stdfs::path& a, const stdfs::path& b) { return a.native() < b.native(); });
    return out;
}

}  // namespace cb::translate::frontends::csharp
