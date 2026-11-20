// cb-unity: alone (helpers named for what they do here)
#include "port/tool_build.hpp"

#include <algorithm>
#include <set>

#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/io.hpp"
#include "core/parallel.hpp"
#include "core/regex.hpp"
#include "core/text.hpp"

namespace cb::port::tool {

namespace stdfs = std::filesystem;

namespace {

constexpr std::string_view kChecks = "port-checks";
constexpr std::string_view kObjects = "port-objects";
constexpr std::string_view kObjectLogs = "port-object-logs";

// The headers a text includes with quotes (`#include "x.h"`).
std::vector<std::string> quoted_includes(std::string_view text) {
    static const Regex re(R"re(#\s*include\s*"([^"]+)")re");
    std::vector<std::string> out;
    re.for_each_match(text, [&](const RegexMatch& m) {
        out.emplace_back(m[1]);
        return true;
    });
    return out;
}

// The same with quotes or angle brackets: what the build's key follows.
std::vector<std::string> any_includes(std::string_view text) {
    static const Regex re(R"re(#\s*include\s*[<"]([^>"]+)[>"])re");
    std::vector<std::string> out;
    re.for_each_match(text, [&](const RegexMatch& m) {
        out.emplace_back(m[1]);
        return true;
    });
    return out;
}

// The diagnostics as bytes (what a check kept): count, then each field.
Bytes diagnostics_bytes(const std::vector<Diagnostic>& ds) {
    io::ByteWriter w;
    w.u32(static_cast<std::uint32_t>(ds.size()));
    const auto str = [&](const std::string& s) {
        w.u32(static_cast<std::uint32_t>(s.size()));
        w.bytes(ByteView(reinterpret_cast<const std::uint8_t*>(s.data()), s.size()));
    };
    for (const Diagnostic& d : ds) {
        str(d.file);
        w.u64(d.line);
        w.u64(d.col);
        str(d.kind);
        str(d.text);
        w.u32(static_cast<std::uint32_t>(d.notes.size()));
        for (const std::string& n : d.notes) str(n);
        w.u8(d.macro_at ? 1 : 0);
        if (d.macro_at) {
            str(d.macro_at->file);
            w.u64(d.macro_at->line);
            w.u64(d.macro_at->col);
        }
    }
    return w.data();
}

std::optional<std::vector<Diagnostic>> diagnostics_of(const Bytes& data) {
    try {
        io::ByteReader r(data, "port check");
        const auto str = [&] {
            const std::uint32_t n = r.u32();
            const ByteView b = r.bytes(n);
            return std::string(as_text(b));
        };
        std::vector<Diagnostic> out(r.u32());
        for (Diagnostic& d : out) {
            d.file = str();
            d.line = r.u64();
            d.col = r.u64();
            d.kind = str();
            d.text = str();
            d.notes.resize(r.u32());
            for (std::string& n : d.notes) n = str();
            if (r.u8() != 0) {
                Diagnostic::Place p;
                p.file = str();
                p.line = r.u64();
                p.col = r.u64();
                d.macro_at = std::move(p);
            }
        }
        return out;
    } catch (const FormatError&) {
        return std::nullopt;   // a damaged entry is a miss
    }
}

void declare_kinds(cache::Cache& cache) {
    cache.declare_kind(kChecks, {.scope = cache::Scope::Shared, .generation = "1"});
    cache.declare_kind(kObjects, {.scope = cache::Scope::Shared, .generation = "1"});
    cache.declare_kind(kObjectLogs, {.scope = cache::Scope::Shared, .generation = "1"});
}

}  // namespace

bool object_is_whole(const stdfs::path& file) {
    std::error_code ec;
    if (!stdfs::is_regular_file(file, ec) || stdfs::file_size(file, ec) <= 64) return false;
    const Bytes head = fs::read_file(file);
    const std::size_t n = std::min<std::size_t>(16, head.size());
    return std::any_of(head.begin(), head.begin() + static_cast<std::ptrdiff_t>(n), [](std::uint8_t b) { return b != 0; });
}

std::string check_key(const Compiler& spec, const stdfs::path& cc, const std::string& file, const OrderedMap<std::string, std::string>& texts) {
    std::vector<cache::KeyPart> parts;
    parts.emplace_back(fs::to_text(stdfs::absolute(cc)));
    parts.emplace_back(spec.check);
    parts.emplace_back(file);
    std::vector<std::string> todo{file};
    std::set<std::string> seen;
    while (!todo.empty()) {
        const std::string x = todo.back();
        todo.pop_back();
        if (!seen.insert(x).second) continue;
        const std::string* t = texts.find(x);
        parts.emplace_back(x);
        parts.emplace_back(t != nullptr ? *t : std::string());
        if (t != nullptr)
            for (const std::string& h : quoted_includes(*t)) todo.push_back(h);
    }
    return cache::key(parts);
}

std::vector<std::string> changed_files(const OrderedMap<std::string, std::string>& texts, const OrderedMap<std::string, std::string>& before,
                                       const std::vector<std::string>& c_files) {
    if (before.empty()) return c_files;
    std::map<std::string, std::set<std::string>> includes;
    for (const auto& [n, t] : texts) {
        const std::vector<std::string> inc = quoted_includes(t);
        includes[n] = std::set<std::string>(inc.begin(), inc.end());
    }
    std::set<std::string> changed, headers;
    for (const auto& [n, t] : texts) {
        const std::string* old = before.find(n);
        if (old == nullptr || *old != t) {
            changed.insert(n);
            if (!n.ends_with(".c")) headers.insert(n);
        }
    }
    const std::function<bool(const std::string&, std::set<std::string>&)> reaches = [&](const std::string& n, std::set<std::string>& seen) {
        const auto it = includes.find(n);
        if (it == includes.end()) return false;
        for (const std::string& h : it->second) {
            if (headers.contains(h)) return true;
            if (seen.insert(h).second && reaches(h, seen)) return true;
        }
        return false;
    };
    std::vector<std::string> out;
    for (const std::string& n : c_files) {
        std::set<std::string> seen;
        if (changed.contains(n) || reaches(n, seen)) out.push_back(n);
    }
    return out;
}

std::vector<Diagnostic> check_file(cache::Cache& cache, const Compiler& spec, const stdfs::path& cc, const stdfs::path& out, const std::string& file,
                                   const OrderedMap<std::string, std::string>& texts, bool& ran) {
    declare_kinds(cache);
    const std::string key = check_key(spec, cc, file, texts);
    if (const std::optional<Bytes> hit = cache.get(kChecks, key)) {
        if (const auto d = diagnostics_of(*hit)) {
            ran = false;
            return *d;
        }
    }
    Values v;
    v.cc = fs::to_text(cc);
    v.inc = ".";
    v.src = file;
    const Run r = run(spec, expand(spec.check, v), out, cc);
    std::vector<Diagnostic> found = diagnostics(spec, r.output());
    cache.put(kChecks, key, diagnostics_bytes(found));
    ran = true;
    return found;
}

BuildResult build(cache::Cache& cache, const Compiler& spec, const stdfs::path& cc, const std::vector<std::string>& tpl, const stdfs::path& out,
                  const std::vector<std::string>& sources, const stdfs::path& exe, int jobs) {
    declare_kinds(cache);
    const auto at_it = std::find(tpl.begin(), tpl.end(), "{out}");
    const std::ptrdiff_t at = at_it == tpl.end() ? -1 : at_it - tpl.begin();
    const bool one_command = !spec.setup.empty() || std::find(tpl.begin(), tpl.end(), "{srcs}") == tpl.end() || at < 1 ||
                             tpl[static_cast<std::size_t>(at - 1)] != "-o";
    Values all;
    all.cc = fs::to_text(cc);
    all.inc = ".";
    all.out = fs::to_text(exe);
    all.srcs = sources;
    if (one_command) {
        const Run r = run(spec, expand(tpl, all), out, cc);
        return {r.ok(), r.output()};
    }

    // a C file at a time: the compile command is the template without `-o {out}` and the sources
    std::vector<std::string> compile_tpl;
    for (std::size_t i = 0; i < tpl.size(); ++i) {
        if (static_cast<std::ptrdiff_t>(i) == at - 1 || static_cast<std::ptrdiff_t>(i) == at || tpl[i] == "{srcs}") continue;
        compile_tpl.push_back(tpl[i]);
    }
    Values compile_values;
    compile_values.cc = fs::to_text(cc);
    compile_values.inc = ".";
    const std::vector<std::string> cargv = expand(compile_tpl, compile_values);
    fs::ScopedTempDir objects("port-obj", stdfs::absolute(out));   // its own: two builds of one folder at once apart

    const auto text_of = [&](const std::string& n) -> std::optional<std::string> {
        try {
            return fs::read_text(out / fs::from_text(n));
        } catch (const IoError&) {
            return std::nullopt;
        }
    };
    const auto key_of = [&](const std::string& n) {
        std::vector<cache::KeyPart> parts;
        parts.emplace_back(fs::to_text(stdfs::absolute(cc)));
        parts.emplace_back(std::vector<std::string>(cargv.begin() + (cargv.empty() ? 0 : 1), cargv.end()));
        parts.emplace_back(n);
        std::vector<std::string> todo{n};
        std::set<std::string> seen;
        while (!todo.empty()) {
            const std::string x = todo.back();
            todo.pop_back();
            if (!seen.insert(x).second) continue;
            const std::optional<std::string> t = text_of(x);
            if (!t) continue;
            parts.emplace_back(x);
            parts.emplace_back(*t);
            for (const std::string& h : any_includes(*t)) todo.push_back(h);
        }
        return cache::key(parts);
    };

    std::vector<std::string> c_files;
    for (const std::string& n : sources)
        if (n.ends_with(".c")) c_files.push_back(n);
    std::vector<std::string> keys;
    for (const std::string& n : c_files) keys.push_back(key_of(n));
    std::map<std::string, std::size_t> index;
    for (std::size_t i = 0; i < c_files.size(); ++i) index[c_files[i]] = i;

    struct One {
        bool ok = true;
        std::string log;
    };
    const auto compile_one = [&](const std::string& n) -> One {
        const std::string& key = keys[index.at(n)];
        const stdfs::path mine = objects.path() / (n.substr(0, n.size() - 2) + ".o");
        if (const std::optional<Bytes> hit = cache.get(kObjects, key)) {
            fs::write_file(mine, *hit);   // as the last time: its object, and what it said then
            std::string log;
            if (const std::optional<Bytes> said = cache.get(kObjectLogs, key)) log = std::string(as_text(*said));
            return {true, log};
        }
        std::vector<std::string> argv = cargv;
        argv.insert(argv.end(), {"-c", n, "-o", fs::to_text(mine)});
        const Run r = run(spec, std::move(argv), out, cc);
        if (r.ok() && object_is_whole(mine)) {
            cache.put(kObjects, key, fs::read_file(mine));
            const std::string log = r.output();
            cache.put(kObjectLogs, key, ByteView(reinterpret_cast<const std::uint8_t*>(log.data()), log.size()));
        }
        return {r.ok(), r.output()};
    };
    const int parallel_jobs = std::max(1, std::min({jobs, 4, static_cast<int>(c_files.size() == 0 ? 1 : c_files.size())}));
    parallel::ThreadPool pool(static_cast<std::size_t>(parallel_jobs));
    const std::vector<One> done = parallel::parallel_map(pool, c_files, compile_one);
    std::string logs;
    bool all_ok = true;
    for (const One& d : done) {
        logs += d.log;
        all_ok = all_ok && d.ok;
    }
    if (!all_ok) return {false, logs};

    Values link = all;
    link.srcs.clear();
    for (const std::string& n : sources) {
        link.srcs.push_back(n.ends_with(".c") ? fs::to_text(objects.path() / (n.substr(0, n.size() - 2) + ".o")) : n);
    }
    const Run r = run(spec, expand(tpl, link), out, cc);
    cache.hold();   // the kept objects count toward the cache cap
    return {r.ok(), logs + r.output()};
}

}  // namespace cb::port::tool
