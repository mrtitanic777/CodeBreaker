// codebreaker/build/functions.cpp -- typed C++ of codebreaker.build.functions (see functions.hpp).
//
// functions.py -- compile a module's reconstructed functions and link a DLL from them.

#include "codebreaker/build/functions.hpp"

#include <algorithm>
#include <condition_variable>
#include <ctime>
#include <iostream>
#include <mutex>
#include <string>
#include <vector>

#include "codebreaker/base/cache.hpp"
#include "codebreaker/base/parallel.hpp"
#include "codebreaker/build/watcom.hpp"
#include "codebreaker/decompile/c_text.hpp"
#include "codebreaker/decompile/describe.hpp"
#include "codebreaker/formats/omf.hpp"
#include "codebreaker/formats/pe.hpp"
#include "codebreaker/jobs/pool.hpp"
#include "py_subprocess.hpp"

namespace codebreaker::build::functions {

namespace cache = codebreaker::base::cache;
namespace omf = codebreaker::formats::omf;
namespace pool = codebreaker::jobs::pool;

const py::list<py::str> CFLAGS16 = py::list<py::str>(std::vector<py::str>{
    py::str("-bt=windows"), py::str("-mm"), py::str("-ecd"), py::str("-zu"), py::str("-zc"), py::str("-zl"), py::str("-zq"),
    py::str("-w0"), py::str("-s"), py::str("-3"), py::str("-zfp"), py::str("-zgp"), py::str("-zastd=c99"), py::str("-e=20"),
    py::str("-j")});   // (char is signed, as the decompiler means it: OpenWatcom's own is unsigned, and a byte
                       // compared with -1 never matched -- USER's dialog-item builder missed an icon's FFh mark)
const py::list<py::str> CFLAGS_POWERPC = py::list<py::str>(std::vector<py::str>{
    py::str("/nologo"), py::str("/c"), py::str("/TC"), py::str("/O2"), py::str("/GS-"), py::str("/Zl"), py::str("/Gy"),
    py::str("/W0"), py::str("/FIm.h")});

py::dict<py::str, py::str> _HEADER;
py::dict<py::str, py::dict<py::str, py::str>> OBJECTS;

namespace {

// the build cache's database is one connection shared by every thread: its reads and writes go one at a time
std::mutex& cache_lock() {
    static std::mutex m;
    return m;
}
std::optional<py::bytes> cache_get(const py::str& kind, const py::str& k) {
    std::lock_guard<std::mutex> g(cache_lock());
    return cache::get(kind, k);
}
void cache_put(const py::str& kind, const py::str& k, const py::bytes& v) {
    std::lock_guard<std::mutex> g(cache_lock());
    cache::put(kind, k, v);
}


const char* const ERR_RX = "\\((\\d+)\\): Error! (E\\d+: [^\\r\\n]*)";
const char* const SYM_RX = "(?m)^([0-9A-Fa-f]{4}):([0-9A-Fa-f]{4,8})[+*]?\\s+(\\S+)\\s*$";
const char* const NEVER_SET_RX = "W200: '(\\w+)' has been referenced but never assigned a value";
const char* const CL_ERR_RX = "\\((\\d+)\\)\\s*:\\s*(?:fatal )?error (C\\d+: [^\\r\\n]*)";
const char* const MAP_SYM_RX = "(?m)^ [0-9A-Fa-f]{4}:[0-9A-Fa-f]{8}\\s+(\\S+)\\s+([0-9A-Fa-f]{8}) ";

py::str S(const char* s) { return py::str(s); }

py::list<py::str> concat(const py::list<py::str>& a, const py::list<py::str>& b) {
    std::vector<py::str> v = a.vec();
    for (const py::str& x : b.vec()) v.push_back(x);
    return py::list<py::str>(v);
}

py::list<py::str> plus(const py::list<py::str>& a, const py::str& x) {
    std::vector<py::str> v = a.vec();
    v.push_back(x);
    return py::list<py::str>(v);
}

py::list<py::str> flags_of(const py::list<py::str>& cflags, const std::optional<py::dict<py::str, py::list<py::str>>>& more,
                           const py::str& name) {
    if (more && more->contains(name)) return concat(cflags, more->at(name));
    return cflags;
}

// open(path, "w", newline="\n", encoding="utf-8").write(text)
void write_text(const py::str& path, const py::str& text) {
    py::file f = py::open(path, S("w"), S("utf-8"), std::nullopt, S("\n"));
    f.write(text);
    f.close();
}

// the child process sees the environment the Python gave it (`env` is os.environ with the compiler's additions): the
// variables that differ are set while any run needs them. Threads running at once share one scope (a second environment
// waits until the first is no longer needed), so one run never puts the variables back under another.
struct EnvGate {
    std::mutex m;
    std::condition_variable cv;
    int users = 0;
    std::vector<std::pair<std::wstring, std::wstring>> wanted;   // what the active scope set
    struct Saved {
        std::wstring name, old;
        bool had;
    };
    std::vector<Saved> saved;
};
EnvGate& gate() {
    static EnvGate g;
    return g;
}

struct EnvScope {
    std::vector<std::pair<std::wstring, std::wstring>> want;
    explicit EnvScope(const py::dict<py::str, py::str>& env) {
        for (const auto& kv : env.items()) {
            std::wstring name = py::subprocess::to_wide(std::get<0>(kv));
            if (name.empty() || name[0] == L'=') continue;
            want.emplace_back(name, py::subprocess::to_wide(std::get<1>(kv)));
        }
        std::sort(want.begin(), want.end());
        EnvGate& g = gate();
        std::unique_lock<std::mutex> lock(g.m);
        g.cv.wait(lock, [&] { return g.users == 0 || g.wanted == want; });
        if (g.users == 0) {
            g.wanted = want;
            g.saved.clear();
            for (const auto& w : want) {
                std::wstring buf(32768, L'\0');
                DWORD n = GetEnvironmentVariableW(w.first.c_str(), buf.data(), static_cast<DWORD>(buf.size()));
                bool had = !(n == 0 && GetLastError() == ERROR_ENVVAR_NOT_FOUND);
                std::wstring old = had ? buf.substr(0, n) : std::wstring();
                if (had && old == w.second) continue;
                g.saved.push_back({w.first, old, had});
                SetEnvironmentVariableW(w.first.c_str(), w.second.c_str());
            }
        }
        g.users += 1;
    }
    ~EnvScope() {
        EnvGate& g = gate();
        std::lock_guard<std::mutex> lock(g.m);
        g.users -= 1;
        if (g.users == 0) {
            for (auto it = g.saved.rbegin(); it != g.saved.rend(); ++it)
                SetEnvironmentVariableW(it->name.c_str(), it->had ? it->old.c_str() : nullptr);
            g.saved.clear();
            g.cv.notify_all();
        }
    }
};

// subprocess.run([...], cwd=, env=, capture_output=True, text=True, errors="replace")
py::subprocess::CompletedProcess run_tool(const py::list<py::str>& args, const py::str& cwd,
                                          const std::optional<py::dict<py::str, py::str>>& env) {
    py::subprocess::RunOptions o;
    o.cwd = cwd;
    o.errors = "replace";
    if (env) {
        EnvScope scope(*env);
        py::subprocess::CompletedProcess r = py::subprocess::run(args, o);
        return r;
    }
    return py::subprocess::run(args, o);
}

py::str log_of(const py::subprocess::CompletedProcess& r) {
    return (r.stdout_ ? *r.stdout_ : S("")) + (r.stderr_ ? *r.stderr_ : S(""));
}

void remove_quietly(const py::str& path) {
    try {
        py::os::remove(path);
    } catch (const py::OSError&) {
    }
}

// the last `n` characters of a string (s[-n:])
py::str tail(const py::str& s, int64_t n) { return s.size() <= n ? s : s.slice(s.size() - n, s.size()); }

py::str head(const py::str& s, int64_t n) { return s.size() <= n ? s : s.slice(0, n); }

// a "%04d" of a number
py::str f_stem(int64_t i) {
    char buf[32];
    std::snprintf(buf, sizeof buf, "f%04lld", static_cast<long long>(i));
    return py::str(std::string(buf));
}

py::str hex_text(int64_t v, int width) {
    char buf[32];
    std::snprintf(buf, sizeof buf, "%0*llX", width, static_cast<unsigned long long>(v));
    return py::str(std::string(buf));
}

py::str read_text_errors_replace(const py::str& path) {
    py::file f = py::open(path, S("r"), S("cp1252"), S("replace"));
    py::str t = f.read_text();
    f.close();
    return t;
}

}  // namespace

// ---- texts ----------------------------------------------------------------------------------------

py::str prototype(const py::str& text) {
    py::str h = py::split(text, S("{"), 1)[0];
    if (h.contains(S(")"))) h = h.slice(0, h.index(S(")")) + 1);   // without old-style argument lines
    return py::join(S(" "), py::split(h)) + S(";");
}

py::str stand_in(const py::str& text) {
    py::str hd = prototype(text).rstrip(S(";"));
    // (with the old-style argument lines kept: a parameter they declare, left out, is an int --
    // not what the header's prototype of the function says)
    py::str whole = py::split(text, S("{"), 1)[0].rstrip();
    return whole + (py::split(hd)[0] == "void" ? S("\n{\n}\n") : S("\n{\n  return 0;\n}\n"));
}

py::str _plain(const py::str& symbol) { return py::split(symbol, S("@"))[0].strip(S("_")); }

py::list<py::str> _adjust_flags(const py::list<py::str>& cflags0, const py::str& text) {
    py::list<py::str> cflags = cflags0;
    if (text.contains(S("__export ")) && !cflags.contains(S("-zW")))
        cflags = plus(cflags, S("-zW"));   // (the Windows prologue, which the loader rewrites for an export)
    if ((text.contains(S("codebreaker_entry_sp(")) || text.contains(S("codebreaker_stack("))) && !cflags.contains(S("-of+")))
        cflags = plus(cflags, S("-of+"));  // (it reads the frame: BP+2 is where SP stood on entry)
    return cflags;
}

// ---- compiling one function -----------------------------------------------------------------------

std::tuple<py::str, bool> _run_compile(const py::str& cc, const py::list<py::str>& cflags, const py::dict<py::str, py::str>& env,
                                       const py::str& outdir, const py::str& stem, const py::str& text) {
    write_text(py::os::path::join(outdir, stem + S(".c")), S("#include \"m.h\"\n\n") + text + S("\n"));
    for (const char* ext : {".obj", ".err"}) remove_quietly(py::os::path::join(outdir, stem + S(ext)));
    py::subprocess::CompletedProcess r = run_tool(concat(py::list<py::str>(std::vector<py::str>{cc}), plus(cflags, stem + S(".c"))),
                                                  outdir, env);
    if (r.returncode == 0 && py::os::path::isfile(py::os::path::join(outdir, stem + S(".obj")))) return {S(""), true};
    py::str log = log_of(r);
    try {
        log += read_text_errors_replace(py::os::path::join(outdir, stem + S(".err")));
    } catch (const py::OSError&) {
    }
    std::optional<py::re::Match> m = py::re::search(ERR_RX, log);
    py::str why;
    if (m) {
        why = m->group(2);
    } else {
        py::str t = tail(log.strip(), 100);
        why = t.empty() ? S("the compiler failed") : t;
    }
    why = head(why, 100);
    if (m) {   // with the line it is about
        std::vector<py::str> src{S("#include \"m.h\""), S("")};
        for (const py::str& l : py::splitlines(text).vec()) src.push_back(l);
        int64_t n = py::int_parse(m->group(1), 10);
        if (0 < n && n <= static_cast<int64_t>(src.size())) why += S(" -- ") + head(src[static_cast<size_t>(n - 1)].strip(), 200);
    }
    return {why, m.has_value()};
}

namespace {
py::dict<py::str, py::str>& TOOL_INPUTS() {
    static py::dict<py::str, py::str> d;
    return d;
}
std::mutex& tool_inputs_lock() {
    static std::mutex m;
    return m;
}

// (time, date) as DOS keeps them, of a file's modification (OpenWatcom's dependency record)
std::tuple<int64_t, int64_t> dos_stamp(const py::str& path) {
    // (OpenWatcom's C library: with no TZ, its local time is UTC)
    std::time_t t = static_cast<std::time_t>(py::os::path::getmtime(path));
    std::tm tm{};
    gmtime_s(&tm, &t);
    return {(static_cast<int64_t>(tm.tm_hour) << 11) | (static_cast<int64_t>(tm.tm_min) << 5) | (tm.tm_sec / 2),
            ((static_cast<int64_t>(tm.tm_year) + 1900 - 1980) << 9) | ((static_cast<int64_t>(tm.tm_mon) + 1) << 5) | tm.tm_mday};
}

const char* const HERE = "\x01here\x01";
const std::string HERE_MARK = std::string("\0here\0", 6);

void put_u16(std::string& out, int64_t v) {
    out.push_back(static_cast<char>(v & 0xFF));
    out.push_back(static_cast<char>((v >> 8) & 0xFF));
}
}  // namespace

std::optional<py::bytes> omf_here(const py::bytes& obj_b, const py::str& folder) {
    if (py::os::getenv(S("TZ"))) return std::nullopt;   // (its dates are then of that zone: the tool says)
    const std::string& obj = obj_b.raw();
    struct Rec {
        int kind;
        std::string body;
        int check;
    };
    std::vector<Rec> recs;
    size_t i = 0, n = obj.size();
    auto u = [&](size_t k) { return static_cast<unsigned char>(obj[k]); };
    while (i + 3 <= n) {
        size_t end = i + 3 + (u(i + 1) | (u(i + 2) << 8));
        if (end > n || end < i + 4) return std::nullopt;
        recs.push_back({static_cast<int>(u(i)), obj.substr(i + 3, end - 1 - (i + 3)), static_cast<int>(u(end - 1))});
        i = end;
    }
    if (i != n || recs.empty() || recs[0].kind != 0x80 || recs[0].body.empty()) return std::nullopt;
    size_t namelen = static_cast<unsigned char>(recs[0].body[0]);
    std::string name = recs[0].body.substr(1, namelen);
    size_t cut = name.rfind('\\');
    if (cut == std::string::npos) return std::nullopt;
    std::string was = name.substr(0, cut);
    std::string now = py::os::path::abspath(folder).encode("latin1", "replace").raw();
    std::string out;
    for (Rec& r : recs) {
        std::string body = r.body;
        auto b = [&](size_t k) { return static_cast<unsigned char>(body[k]); };
        if (r.kind == 0x80) {
            if (now.size() + name.size() - cut > 255) return std::nullopt;
            body = std::string(1, static_cast<char>(now.size() + name.size() - cut)) + now + name.substr(cut);
        } else if (r.kind == 0x88 && body.size() > 7 && b(1) == 0xE9) {
            std::string dep = body.substr(7, b(6));
            if (dep.compare(0, was.size() + 1, was + "\\") == 0) {
                dep = now + dep.substr(was.size());
                if (dep.size() > 255) return std::nullopt;
                py::str there = py::bytes(dep).decode("latin1");
                if (!py::os::path::isfile(there)) return std::nullopt;
                auto [tm, dt] = dos_stamp(there);
                std::string nb = body.substr(0, 2);
                put_u16(nb, tm);
                put_u16(nb, dt);
                nb.push_back(static_cast<char>(dep.size()));
                nb += dep;
                nb += body.substr(7 + b(6));
                body = nb;
            }
        }
        std::string rec(1, static_cast<char>(r.kind));
        put_u16(rec, static_cast<int64_t>(body.size()) + 1);
        rec += body;
        int sum = 0;
        for (char c : rec) sum += static_cast<unsigned char>(c);
        out += rec;
        out.push_back(static_cast<char>(r.check ? ((-sum) & 0xFF) : 0));
    }
    return py::bytes(out);
}

py::bytes message_kept(const py::str& msg, const py::str& folder) {
    return msg.replace(py::os::path::abspath(folder), py::str(HERE_MARK)).encode("utf-8", "replace");
}

py::str message_here(const py::bytes& kept, const py::str& folder) {
    return kept.decode("utf-8", "replace").replace(py::str(HERE_MARK), py::os::path::abspath(folder));
}

py::str tool_inputs(const py::str& cc, const std::optional<py::dict<py::str, py::str>>& env0, const py::str& variable) {
    py::dict<py::str, py::str> env = env0 ? *env0 : py::dict<py::str, py::str>();
    py::str include = env.get(S("INCLUDE"), S(""));
    py::str opts = env.get(variable, S(""));
    // the Python's tuple key (cc, INCLUDE, variable's value), joined unambiguously
    py::str k = cc + S("\x01") + include + S("\x01") + opts + S("\x01") + variable;
    std::lock_guard<std::mutex> lock(tool_inputs_lock());   // (held while it is worked out: the hashing runs once, and not in two threads at a time)

        if (TOOL_INPUTS().contains(k)) return TOOL_INPUTS().at(k);

    std::vector<py::str> folders;
    for (const py::str& d : py::split(include, py::os::pathsep).vec())
        if (!d.empty() && py::os::path::isdir(d)) folders.push_back(d);
    py::str v = cache::key({cache::Part(cache::tool_bytes(cc)),
                            cache::Part(cache::files_bytes(py::list<py::str>(folders))), cache::Part(variable),
                            cache::Part(opts)});

    TOOL_INPUTS().set(k, v);
    return v;
}

std::tuple<py::str, bool> compile_kept(const py::str& cc, const py::list<py::str>& cflags, const py::dict<py::str, py::str>& env,
                                       const py::str& outdir, const py::str& stem, const py::str& text,
                                       const std::optional<py::str>& header_key) {
    std::optional<py::str> k;
    if (header_key && !header_key->empty())
        k = cache::key({cache::Part(S("wcc-object")), cache::Part(tool_inputs(cc, env, S("WCC"))), cache::Part(cflags),
                        cache::Part(*header_key), cache::Part(stem), cache::Part(text)});
    std::optional<py::bytes> had = k ? cache_get(S("objects"), *k) : std::nullopt;
    if (had) {
        write_text(py::os::path::join(outdir, stem + S(".c")), S("#include \"m.h\"\n\n") + text + S("\n"));
        const std::string& h = had->raw();
        if (h.compare(0, 2, "ER") == 0) return {message_here(py::bytes(h.substr(2)), outdir), true};
        // (as wcc writes it here, now)
        std::optional<py::bytes> here =
            omf_here(py::bytes(h.substr(2)), py::os::path::dirname(py::os::path::join(outdir, stem + S(".c"))));
        if (here) {
            py::file f = py::open(py::os::path::join(outdir, stem + S(".obj")), S("wb"));
            f.write(*here);
            f.close();
            return {S(""), true};
        }
    }
    auto [why, keep] = _run_compile(cc, cflags, env, outdir, stem, text);
    if (why.empty()) {
        if (k) {
            py::file f = py::open(py::os::path::join(outdir, stem + S(".obj")), S("rb"));
            py::bytes data = f.read();
            f.close();
            cache_put(S("objects"), *k, py::bytes(std::string("OK") + data.raw()));
        }
    } else if (k && keep) {   // a rejection is kept; a compiler that fell over is not
        cache_put(S("objects"), *k, py::bytes(std::string("ER") + message_kept(why, outdir).raw()));
    }
    return {why, keep};
}

py::str _compile(const py::str& cc, const py::list<py::str>& cflags, const py::dict<py::str, py::str>& env, const py::str& outdir,
                 const py::str& stem, const py::str& text) {
    std::optional<py::str> header = _HEADER.contains(outdir) ? std::optional<py::str>(_HEADER.at(outdir)) : std::nullopt;
    return std::get<0>(compile_kept(cc, _adjust_flags(cflags, text), env, outdir, stem, text, header));
}

py::list<py::str> _run_never_set(const py::str& cc, const py::list<py::str>& cflags, const py::dict<py::str, py::str>& env,
                                 const py::str& outdir, const py::str& stem, const py::str& text) {
    std::vector<py::str> fl;
    for (const py::str& f : cflags.vec())
        if (!f.startswith(S("-w"))) fl.push_back(f);
    fl.push_back(S("-w2"));
    fl.push_back(S("-zs"));
    py::str name = stem + S("_w.c");
    write_text(py::os::path::join(outdir, name), S("#include \"m.h\"\n\n") + text + S("\n"));
    py::list<py::str> args(std::vector<py::str>{cc});
    for (const py::str& f : fl) args.append(f);
    args.append(name);
    py::subprocess::CompletedProcess r = run_tool(args, outdir, env);
    remove_quietly(py::os::path::join(outdir, name));
    py::set<py::str> found;
    for (const py::str& s : py::re::findall(NEVER_SET_RX, log_of(r)).vec()) found.add(s);
    return py::sorted(found);
}

py::list<py::str> never_set_kept(const py::str& cc, const py::list<py::str>& cflags, const py::dict<py::str, py::str>& env,
                                 const py::str& outdir, const py::str& stem, const py::str& text,
                                 const std::optional<py::str>& header_key) {
    py::list<py::str> flags;
    for (const py::str& f : cflags.vec())
        if (!f.startswith(S("-w"))) flags.append(f);
    flags.append(S("-w2"));
    flags.append(S("-zs"));
    std::optional<py::str> k;
    if (header_key && !header_key->empty())
        k = cache::key({cache::Part(S("wcc-never-set")), cache::Part(tool_inputs(cc, env, S("WCC"))), cache::Part(flags),
                        cache::Part(*header_key), cache::Part(text)});
    std::optional<py::bytes> had = k ? cache_get(S("never_set"), *k) : std::nullopt;
    if (had) {
        py::list<py::str> out;
        for (const py::str& s : py::split(had->decode("utf-8"), S(",")).vec())
            if (!s.empty()) out.append(s);
        return out;
    }
    py::list<py::str> found = _run_never_set(cc, cflags, env, outdir, stem, text);
    if (k) cache_put(S("never_set"), *k, py::join(S(","), found).encode("utf-8"));
    return found;
}

py::list<py::str> never_set(const py::str& cc, const py::list<py::str>& cflags, const py::dict<py::str, py::str>& env,
                            const py::str& outdir, const py::str& stem, const py::str& text) {
    std::optional<py::str> header = _HEADER.contains(outdir) ? std::optional<py::str>(_HEADER.at(outdir)) : std::nullopt;
    return never_set_kept(cc, cflags, env, outdir, stem, text, header);
}

std::optional<py::str> object_digest(const py::str& path) {
    std::string data;
    try {
        py::file f = py::open(path, S("rb"));
        data = f.read().raw();
        f.close();
    } catch (const py::OSError&) {
        return std::nullopt;
    }
    py::hashlib::Hash h = py::hashlib::sha1();
    auto u = [&](size_t k) { return static_cast<unsigned char>(data[k]); };
    if (!data.empty() && u(0) == 0x80) {   // OMF: records; skip the header and comments
        size_t p = 0;
        while (p + 3 <= data.size()) {
            int kind = u(p);
            size_t size = u(p + 1) | (u(p + 2) << 8);
            if (kind != 0x80 && kind != 0x88 && kind != 0x8A && kind != 0x8B)
                h.update(py::bytes(data.substr(p, 3 + size - 1)));   // (without the record's checksum)
            p += 3 + size;
        }
    } else {   // COFF: all but the time it was made
        h.update(py::bytes(data.substr(0, 4)));
        h.update(py::bytes(data.size() > 8 ? data.substr(8) : std::string()));
    }
    return h.hexdigest();
}

namespace {

void note_objects(const py::str& outdir, const py::dict<py::str, py::str>& stems, const py::dict<py::str, py::str>& dropped,
                  const std::vector<py::str>& gone = {}) {
    py::dict<py::str, py::str> out;
    OBJECTS.set(outdir, out);
    for (const auto& kv : stems.items()) {
        const py::str& n = std::get<0>(kv);
        std::optional<py::str> d = object_digest(py::os::path::join(outdir, std::get<1>(kv) + S(".obj")));
        out.set(n, (dropped.contains(n) ? S("stand-in:") : S("")) + (d ? *d : S("none")));
    }
    for (const py::str& n : gone) out.set(n, S("the original's"));
}

}  // namespace

// ---- compiling every function ---------------------------------------------------------------------

Compiled compile_all(const py::str& cc, const py::list<py::str>& cflags, const py::dict<py::str, py::str>& env,
                     const py::str& header, const py::dict<py::str, py::str>& texts, const py::list<py::str>& order,
                     const py::str& outdir, const std::optional<int64_t>& jobs,
                     const std::optional<py::dict<py::str, py::list<py::str>>>& more) {
    py::list<Group> groups;
    groups.append(Group{cflags, order, outdir});
    return compile_groups(cc, env, header, texts, groups, jobs, more)[0];
}

py::list<Compiled> compile_groups(const py::str& cc, const py::dict<py::str, py::str>& env, const py::str& header,
                                  const py::dict<py::str, py::str>& texts, const py::list<Group>& groups,
                                  const std::optional<int64_t>& jobs0,
                                  const std::optional<py::dict<py::str, py::list<py::str>>>& more) {
    int64_t jobs = (jobs0 && *jobs0) ? *jobs0 : base::parallel::workers();
    struct Item {
        py::list<py::str> cflags;
        py::str outdir;
        py::dict<py::str, py::str> stems;
        py::str name;
    };
    std::vector<Item> work;
    for (const Group& g : groups.vec()) {
        py::os::makedirs(g.outdir, 0777, true);
        _HEADER.set(g.outdir, cache::key({cache::Part(header)}));
        write_text(py::os::path::join(g.outdir, S("m.h")), header);
        py::dict<py::str, py::str> stems;
        int64_t i = 0;
        for (const py::str& n : g.order.vec()) stems.set(n, f_stem(i++));
        for (const py::str& n : g.order.vec()) work.push_back({g.cflags, g.outdir, stems, n});
    }
    // (the largest first: a long function started last would be the one the rest wait for)
    std::vector<int64_t> order_run;
    for (size_t i = 0; i < work.size(); ++i) order_run.push_back(static_cast<int64_t>(i));
    std::stable_sort(order_run.begin(), order_run.end(), [&](int64_t a, int64_t b) {
        return texts.at(work[static_cast<size_t>(a)].name).size() > texts.at(work[static_cast<size_t>(b)].name).size();
    });
    py::list<py::str> got = pool::threads(
        [&](const int64_t& i) {
            const Item& w = work[static_cast<size_t>(i)];
            return _compile(cc, flags_of(w.cflags, more, w.name), env, w.outdir, w.stems.at(w.name), texts.at(w.name));
        },
        py::list<int64_t>(order_run), jobs);
    py::dict<int64_t, py::str> errors;
    for (size_t j = 0; j < order_run.size(); ++j) errors.set(order_run[j], got[static_cast<int64_t>(j)]);
    py::list<Compiled> out;
    size_t k = 0;
    for (const Group& g : groups.vec()) {
        py::dict<py::str, py::str> stems = !g.order.empty() ? work[k].stems : py::dict<py::str, py::str>();
        py::dict<py::str, py::str> dropped;
        int64_t i = 0;
        for (const py::str& n : g.order.vec()) {
            const py::str& e = errors.at(static_cast<int64_t>(k) + i);
            if (!e.empty()) dropped.set(n, e);
            ++i;
        }
        k += static_cast<size_t>(g.order.size());
        for (const py::str& n : dropped.keys()) {   // callers of a rejected function still link
            if (!_compile(cc, flags_of(g.cflags, more, n), env, g.outdir, stems.at(n), stand_in(texts.at(n))).empty())
                stems.pop(n);                       // not even a stand-in: left unresolved
        }
        out.append(Compiled{stems, dropped});
    }
    return out;
}

// ---- linking --------------------------------------------------------------------------------------

namespace {

py::dict<py::str, py::str>& LINKED() {
    static py::dict<py::str, py::str> d;
    return d;
}

std::tuple<std::optional<py::str>, py::str> link(const py::str& wlink, const py::dict<py::str, py::str>& env,
                                                 const py::str& outdir, const std::vector<py::str>& lines) {
    write_text(py::os::path::join(outdir, S("m.lnk")), py::join(S("\n"), py::list<py::str>(lines)) + S("\n"));
    for (const char* stale : {"m.dll", "m.map"}) remove_quietly(py::os::path::join(outdir, S(stale)));
    py::subprocess::CompletedProcess r = run_tool(py::list<py::str>(std::vector<py::str>{wlink, S("@m.lnk")}), outdir, env);
    py::str dll = py::os::path::join(outdir, S("m.dll"));
    LINKED().set(outdir, log_of(r));
    if (py::os::path::isfile(dll)) return {dll, S("")};
    py::str log = log_of(r).strip();
    std::optional<py::str> first;
    for (const py::str& ln : py::splitlines(log).vec())
        if (ln.contains(S("Error!"))) {
            first = ln.strip();
            break;
        }
    py::str why = first ? head(*first, 200) : tail(log, 160);
    return {std::nullopt, why.empty() ? S("the linker produced nothing") : why};
}

// The link lines for the routines the interrupt helpers call (decompile/c_text.py interrupt_routines, as build/module.py
// adds them) when the module's C takes an interrupt (the helpers are in the header): without them a helper's call went
// to 0:0 and its registers to offset 0 of the data segment.
std::vector<py::str> interrupt_routines_lines(const py::str& wcc, const py::dict<py::str, py::str>& env, const py::str& outdir,
                                              const std::optional<py::set<int64_t>>& interrupts) {
    if (!interrupts || interrupts->empty()) return {};
    auto [asm_text, c] = decompile::c_text::interrupt_routines(*interrupts);
    write_text(py::os::path::join(outdir, S("interrupts.asm")), asm_text);
    py::subprocess::CompletedProcess r = run_tool(
        py::list<py::str>(std::vector<py::str>{build::watcom::ow_tool(S("wasm")), S("-q"), S("interrupts.asm")}), outdir, env);
    if (r.returncode ||
        !_compile(wcc, CFLAGS16, env, outdir, S("interrupt_registers"), c.replace(S("#include \"m.h\"\n"), S(""))).empty())
        return {};
    return {S("file interrupts.obj"), S("file interrupt_registers.obj")};
}

std::string chunked(const py::bytes& image) {
    const std::string& r = image.raw();
    std::string out;
    for (size_t i = 0; i < r.size(); i += 32) {
        if (i) out += ",\n";
        for (size_t j = i; j < std::min(r.size(), i + 32); ++j) {
            if (j > i) out += ",";
            out += std::to_string(static_cast<unsigned char>(r[j]));
        }
    }
    return out;
}

bool has(const std::vector<py::str>& v, const py::str& x) { return std::find(v.begin(), v.end(), x) != v.end(); }

}  // namespace

Built16 build16(const py::str& header, const py::dict<py::str, py::str>& texts, const py::list<py::str>& order,
                const py::dict<py::str, std::tuple<py::str, int64_t>>& imports, const py::str& outdir,
                const std::optional<py::dict<int64_t, py::bytes>>& segments, const std::optional<int64_t>& jobs,
                const std::optional<py::dict<py::str, int64_t>>& code_segment, const std::optional<py::set<int64_t>>& interrupts,
                const std::optional<py::dict<int64_t, py::str>>& code_data,
                const std::optional<py::dict<int64_t, py::bytes>>& code_image) {
    // (16-bit Windows: its compiler and packager)
    py::str wcc = build::watcom::ow_tool(S("wcc")), wlink = build::watcom::ow_tool(S("wlink"));
    if (wcc.empty() || wlink.empty()) throw py::RuntimeError("OpenWatcom's 16-bit compiler is not set up (settings.json: openwatcom)");
    py::dict<py::str, py::str> env = build::watcom::ow_env(S("ne"));
    py::dict<py::str, py::list<py::str>> more;
    if (code_segment)
        for (const auto& kv : code_segment->items())
            more.set(std::get<0>(kv), py::list<py::str>(std::vector<py::str>{S("-nt=CODE") + py::to_str(std::get<1>(kv))}));
    Compiled c = compile_all(wcc, CFLAGS16, env, header, texts, order, outdir, jobs, more);
    py::dict<py::str, py::str> stems = c.stems, dropped = c.dropped;
    std::vector<py::str> live;
    for (const py::str& n : order.vec())
        if (!dropped.contains(n)) live.push_back(n);
    // One initialised byte, so the DLL has a data segment at all: that segment is where the
    // original's data is put when the DLL is run for judging.
    std::vector<py::str> data{S("char codebreaker_data_segment = 1;")};
    if (segments) {
        std::vector<std::tuple<int64_t, py::bytes>> segs;
        for (const auto& kv : segments->items()) segs.push_back(kv);
        std::sort(segs.begin(), segs.end(), [](const auto& a, const auto& b) { return std::get<0>(a) < std::get<0>(b); });
        for (const auto& [n, image] : segs) {   // other segments the code reads from
            py::str ns = py::to_str(n);
            data.push_back(S("uint1 __based(__segname(\"CODEBREAKER_SEGMENT_") + ns + S("\")) codebreaker_segment_") + ns +
                           S("[] = {\n") + py::str(chunked(image)) + S("\n};"));
        }
    }
    _compile(wcc, CFLAGS16, env, outdir, S("data"), py::join(S("\n"), py::list<py::str>(data)));
    std::vector<py::str> lines{S("system windows dll"), S("name m.dll"),
                               S("option quiet, map, nodefaultlibs, caseexact, undefsok"),
                               S("libpath ") + build::watcom::ow_lib(S("ne"))};
    std::vector<std::tuple<int64_t, py::str>> images;   // (first in their segments: at offset 0)
    if (code_image) {
        std::vector<std::tuple<int64_t, py::bytes>> segs;
        for (const auto& kv : code_image->items()) segs.push_back(kv);
        std::sort(segs.begin(), segs.end(), [](const auto& a, const auto& b) { return std::get<0>(a) < std::get<0>(b); });
        for (const auto& [seg, image] : segs) {
            py::str ss = py::to_str(seg);
            char nm[16];
            std::snprintf(nm, sizeof nm, "image%02lld", static_cast<long long>(seg));
            if (_compile(wcc, plus(CFLAGS16, S("-nt=CODE") + ss), env, outdir, py::str(std::string(nm)),
                         S("uint1 __based(__segname(\"_CODE\")) codebreaker_code_image_") + ss + S("[] = {\n") +
                             py::str(chunked(image)) + S("\n};"))
                    .empty())
                images.emplace_back(seg, S("file ") + py::str(std::string(nm)) + S(".obj"));
        }
    }
    if (!images.empty()) lines.push_back(S("option packcode=0"));   // (each its own segment, not packed after another)
    for (const auto& [_seg, line] : images) lines.push_back(line);
    for (const py::str& n : order.vec())
        if (stems.contains(n)) lines.push_back(S("file ") + stems.at(n) + S(".obj"));
    if (code_data) {
        std::vector<std::tuple<int64_t, py::str>> cds;
        for (const auto& kv : code_data->items()) cds.push_back(kv);
        std::sort(cds.begin(), cds.end(), [](const auto& a, const auto& b) { return std::get<0>(a) < std::get<0>(b); });
        for (const auto& [seg, text] : cds) {   // (beside the functions of its segment)
            char nm[16];
            std::snprintf(nm, sizeof nm, "code%02lld", static_cast<long long>(seg));
            if (_compile(wcc, plus(CFLAGS16, S("-nt=CODE") + py::to_str(seg)), env, outdir, py::str(std::string(nm)), text).empty())
                lines.push_back(S("file ") + py::str(std::string(nm)) + S(".obj"));
        }
    }
    lines.push_back(S("file data.obj"));
    lines.push_back(S("library clibm.lib"));
    for (const py::str& l : interrupt_routines_lines(wcc, env, outdir, interrupts)) lines.push_back(l);
    // a C name compiled as __stdcall with an open argument list is the symbol `_name`
    {
        std::vector<std::tuple<py::str, std::tuple<py::str, int64_t>>> imps;
        for (const auto& kv : imports.items()) imps.push_back(kv);
        std::sort(imps.begin(), imps.end(), [](const auto& a, const auto& b) { return std::get<0>(a) < std::get<0>(b); });
        for (const auto& [nm, mo] : imps)
            lines.push_back(S("import _") + nm + S(" ") + std::get<0>(mo) + S(".") + py::to_str(std::get<1>(mo)));
    }
    auto [dll, why] = link(wlink, env, outdir, lines);
    // A segment holds 64K. Where the rebuilt functions of one of the original's segments come to
    // more (other compiler; or the decompiler took a table for code and wrote pages of it), the
    // largest are replaced by stand-ins until the rest fit: those few are not built, the
    // others are judged.
    py::dict<py::str, py::str> by_stem;
    for (const auto& kv : stems.items()) by_stem.set(std::get<1>(kv), std::get<0>(kv));
    for (int attempt = 0; attempt < 80; ++attempt) {
        // ... and a function that reaches near for something that is in another segment (the
        // original called it far; the decompiler's text has lost that) cannot be linked at all
        std::optional<py::re::Match> apart = py::re::search("file (f[0-9]+)[.]obj.*not in the same segment", why);
        if (!dll && apart && by_stem.contains(apart->group(1)) && has(live, by_stem.at(apart->group(1)))) {
            py::str n = by_stem.at(apart->group(1));
            if (!_compile(wcc, flags_of(CFLAGS16, std::optional<py::dict<py::str, py::list<py::str>>>(more), n), env, outdir,
                          stems.at(n), stand_in(texts.at(n)))
                     .empty())
                break;
            dropped.set(n, S("reaches near for something in another segment"));
            std::vector<py::str> nl;
            for (const py::str& x : live)
                if (x != n) nl.push_back(x);
            live = nl;
            std::tie(dll, why) = link(wlink, env, outdir, lines);
            continue;
        }
        std::optional<py::re::Match> over = py::re::search("size of segment CODE(\\d+) exceeds 64k by (\\d+) bytes", why);
        if (dll || !over || !code_segment) break;
        int64_t seg = py::int_parse(over->group(1), 10), need = py::int_parse(over->group(2), 10) + 512;
        std::optional<py::str> image;
        for (const auto& [sg, line] : images)
            if (sg == seg && has(lines, line)) {
                image = line;
                break;
            }
        if (image) {   // (the original's image goes first)
            lines.erase(std::find(lines.begin(), lines.end(), *image));
            std::tie(dll, why) = link(wlink, env, outdir, lines);
            continue;
        }
        std::vector<std::tuple<int64_t, py::str>> sized;
        for (const py::str& n : live)
            if (code_segment->contains(n) && code_segment->at(n) == seg && stems.contains(n) && !dropped.contains(n))
                sized.emplace_back(py::os::path::getsize(py::os::path::join(outdir, stems.at(n) + S(".obj"))), n);
        std::sort(sized.begin(), sized.end(), [](const auto& a, const auto& b) { return a > b; });   // (reverse=True)
        int64_t freed = 0;
        for (const auto& [size, n] : sized) {
            if (freed >= need) break;
            if (_compile(wcc, flags_of(CFLAGS16, std::optional<py::dict<py::str, py::list<py::str>>>(more), n), env, outdir,
                         stems.at(n), stand_in(texts.at(n)))
                    .empty()) {
                freed += size;
                dropped.set(n, S("too large: the functions of its segment do not fit in 64K when rebuilt"));
            }
        }
        if (!freed) break;
        std::vector<py::str> nl;
        for (const py::str& n : live)
            if (!dropped.contains(n)) nl.push_back(n);
        live = nl;
        std::tie(dll, why) = link(wlink, env, outdir, lines);
    }
    note_objects(outdir, stems, dropped);
    if (!dll) {
        py::dict<py::str, py::str> d = dropped.copy();
        for (const py::str& n : live) d.set(n, S("link: ") + why);
        return Built16{std::nullopt, py::dict<py::str, std::tuple<int64_t, int64_t>>(), d};
    }
    py::dict<py::str, std::tuple<int64_t, int64_t>> where;
    py::str text;
    try {
        text = read_text_errors_replace(py::os::path::join(outdir, S("m.map")));
    } catch (const py::OSError&) {
        text = S("");
    }
    std::vector<py::str> live_set = live;
    for (const py::re::Match& mm : py::re::finditer(SYM_RX, text).vec()) {
        py::str n = _plain(mm.group(3));
        if (has(live_set, n) && !where.contains(n))
            where.set(n, std::make_tuple(py::int_parse(mm.group(1), 16), py::int_parse(mm.group(2), 16)));
    }
    return Built16{dll, where, dropped};
}

namespace {

// An object file in which each name in `undefined` that is a function of the original (`originals` {name: address}, and
// `func_0x<address>`) is a jump to it. -> its name, or None.
std::optional<py::str> in_the_original(const py::str& outdir, const py::dict<py::str, py::str>& env,
                                       const py::list<py::str>& undefined, const py::dict<py::str, int64_t>& originals) {
    std::vector<py::str> lines{S(".386"), S(".model flat"), S("_TEXT segment dword public use32 'CODE'")};
    int64_t n = 0;
    py::set<py::str> uniq;
    for (const py::str& s : undefined.vec()) uniq.add(s);
    for (const py::str& symbol : py::sorted(uniq).vec()) {
        py::str plain = _plain(symbol);
        std::optional<int64_t> at;
        if (originals.contains(plain)) at = originals.at(plain);
        std::optional<py::re::Match> named = py::re::fullmatch("func_0x([0-9a-fA-F]{6,8})", plain);
        if (!at && named) at = py::int_parse(named->group(1), 16);
        if (!at || !py::re::fullmatch("[A-Za-z_@?$][\\w@?$]*", symbol)) continue;
        lines.push_back(S("public ") + symbol);
        lines.push_back(symbol + S(":"));
        lines.push_back(S("        push 0") + hex_text(*at, 8) + S("h"));
        lines.push_back(S("        ret"));
        n += 1;
    }
    if (!n) return std::nullopt;
    lines.push_back(S("_TEXT ends"));
    lines.push_back(S("end"));
    write_text(py::os::path::join(outdir, S("original.asm")), py::join(S("\n"), py::list<py::str>(lines)) + S("\n"));
    py::str wasm = build::watcom::ow_tool(S("wasm"));
    if (wasm.empty()) return std::nullopt;
    remove_quietly(py::os::path::join(outdir, S("original.obj")));
    run_tool(py::list<py::str>(std::vector<py::str>{wasm, S("-zq"), S("-fo=original.obj"), S("original.asm")}), outdir, env);
    return py::os::path::isfile(py::os::path::join(outdir, S("original.obj"))) ? std::optional<py::str>(S("original.obj"))
                                                                              : std::nullopt;
}

}  // namespace

Built32 build32(const py::str& module_path, const py::str& header, const py::dict<py::str, py::str>& texts,
                const py::list<py::str>& order, const py::list<py::str>& used_imports, const py::str& outdir,
                const py::list<py::str>& cflags, const std::optional<int64_t>& jobs,
                const std::optional<py::dict<py::str, int64_t>>& originals) {
    namespace B = build::watcom;
    B::OW ow = B::_ow();
    if (!py::os::path::exists(ow.wcc)) throw py::RuntimeError("OpenWatcom's 32-bit compiler is not set up (settings.json: openwatcom)");
    py::dict<py::str, py::str> env = B::_ow_env(ow);
    py::dict<py::str, py::str> imports = B::import_map(module_path);
    B::Numbered numbered = decompile::describe::BY_NUMBER.contains(module_path) ? decompile::describe::BY_NUMBER.at(module_path)
                                                                               : B::Numbered();
    Compiled c = compile_all(ow.wcc, plus(cflags, S("-e=20")), env, header, texts, order, outdir, jobs);
    py::dict<py::str, py::str> stems = c.stems, dropped = c.dropped;
    std::vector<py::str> live;
    for (const py::str& n : order.vec())
        if (!dropped.contains(n)) live.push_back(n);
    std::vector<py::str> gone;
    for (const py::str& n : dropped.keys())
        if (originals && originals->contains(n) && stems.contains(n)) gone.push_back(n);
    for (const py::str& n : gone) stems.pop(n, S(""));   // no stand-in where the original itself can be called
    note_objects(outdir, stems, dropped, gone);
    // undefsok: a call to a function that is not there is left unresolved (it then fails its test)
    std::vector<py::str> lines{S("system nt_dll"), S("name m.dll"), S("option quiet"), S("option undefsok"), B::_BASE.strip()};
    for (const py::str& n : order.vec())
        if (stems.contains(n)) lines.push_back(S("file ") + stems.at(n) + S(".obj"));
    for (const py::str& x : py::split(B::_RTL.strip(), S("\n")).vec())
        if (!x.empty()) lines.push_back(x);
    lines.push_back(S("library math387r.lib"));   // sin, sqrt ... for a function that uses them
    py::set<py::str> seen;
    std::vector<py::str> exports;
    py::set<py::str> used;
    for (const py::str& s : used_imports.vec()) used.add(s);
    for (const py::str& n : order.vec()) {
        if (!stems.contains(n)) continue;
        py::file f = py::open(py::os::path::join(outdir, stems.at(n) + S(".obj")), S("rb"));
        py::bytes obj = f.read();
        f.close();
        for (const py::str& line : py::split(B::_import_lines(obj, used, imports, numbered).strip(), S("\n")).vec()) {
            if (!line.empty() && !seen.contains(line)) {
                seen.add(line);
                lines.push_back(line);
            }
        }
        if (dropped.contains(n)) continue;
        // the public symbol as the compiler decorated it: _name (cdecl), _name@N (stdcall)
        std::optional<py::str> sym;
        py::re::Pattern decorated = py::re::compile(S("[_@]") + py::re::escape(n) + S("@\\d+"));
        for (const py::str& s : omf::publics(obj)) {
            if (s == S("_") + n || s == n || decorated.fullmatch(s)) {
                sym = s;
                break;
            }
        }
        if (sym) exports.push_back(S("export ") + n + S("='") + *sym + S("'"));
    }
    auto concat_v = [](std::vector<py::str> a, const std::vector<py::str>& b) {
        a.insert(a.end(), b.begin(), b.end());
        return a;
    };
    auto [dll, why] = link(ow.wlink, env, outdir, concat_v(lines, exports));
    if (dll && originals) {
        py::list<py::str> missing = py::re::findall("W1028: (\\S+) is an undefined reference",
                                                    LINKED().get(outdir, S("")));
        std::optional<py::str> there;
        if (!missing.empty()) there = in_the_original(outdir, env, missing, *originals);
        if (there) {
            std::vector<py::str> more_lines = lines;
            more_lines.push_back(S("file ") + *there);
            std::tie(dll, why) = link(ow.wlink, env, outdir, concat_v(more_lines, exports));
        }
    }
    if (!dll) {
        py::dict<py::str, py::str> d = dropped.copy();
        for (const py::str& n : live) d.set(n, S("link: ") + why);
        return Built32{std::nullopt, py::dict<py::str, int64_t>(), d};
    }
    formats::pe::PEFile pb = formats::pe::open_pe(*dll);
    py::dict<py::str, int64_t> at = pb.export_addrs();
    py::dict<py::str, int64_t> where;
    for (const py::str& n : live)
        if (at.contains(n)) where.set(n, static_cast<int64_t>(pb.image_base) + at.at(n));
    return Built32{dll, where, dropped};
}

// ---- PowerPC --------------------------------------------------------------------------------------

std::optional<std::tuple<py::str, py::str, py::str>> powerpc_tools() {
    std::optional<py::str> d = py::settings::get_opt_str(S("powerpc_tools")), lib = py::settings::get_opt_str(S("powerpc_lib"));
    py::str dd = d ? *d : S(""), ll = lib ? *lib : S("");
    py::str cl = py::os::path::join(dd, S("cl.exe")), link_ = py::os::path::join(dd, S("link.exe"));
    if (d && !d->empty() && py::os::path::isfile(cl) && py::os::path::isfile(link_) && py::os::path::isfile(ll))
        return std::make_tuple(cl, link_, ll);
    return std::nullopt;
}

namespace {

py::str compile_powerpc(const py::str& cl, const py::str& outdir, const py::str& stem, const py::str& text) {
    // (the key, every input of the object: the compiler by its bytes and its folder -- cl.exe runs
    // c1/c2 beside it -- the INCLUDE headers and CL variable it reads, the options, the header,
    // the stem the object and an error name, and the text; kept in the shared build cache)
    py::dict<py::str, py::str> environ_;
    std::optional<py::str> k;
    if (_HEADER.contains(outdir)) {
        // os.environ as a dict
        py::dict<py::str, py::str> envd;
        for (const char* nm : {"INCLUDE", "CL"}) {
            std::optional<py::str> v = py::os::getenv(S(nm));
            if (v) envd.set(S(nm), *v);
        }
        k = cache::key({cache::Part(S("cl-ppc-object")), cache::Part(tool_inputs(cl, envd, S("CL"))),
                        cache::Part(cache::folder(py::os::path::dirname(cl))), cache::Part(CFLAGS_POWERPC),
                        cache::Part(_HEADER.at(outdir)), cache::Part(stem), cache::Part(text)});
    }
    std::optional<py::bytes> had = k ? cache_get(S("objects"), *k) : std::nullopt;
    if (had) {
        const std::string& h = had->raw();
        if (h.compare(0, 2, "ER") == 0) return message_here(py::bytes(h.substr(2)), outdir);
        py::file f = py::open(py::os::path::join(outdir, stem + S(".obj")), S("wb"));
        f.write(py::bytes(h.substr(2)));
        f.close();
        return S("");
    }
    write_text(py::os::path::join(outdir, stem + S(".c")), text + S("\n"));
    remove_quietly(py::os::path::join(outdir, stem + S(".obj")));
    py::list<py::str> args(std::vector<py::str>{cl});
    for (const py::str& f : CFLAGS_POWERPC.vec()) args.append(f);
    args.append(S("/Fo") + stem + S(".obj"));
    args.append(stem + S(".c"));
    py::subprocess::CompletedProcess r = run_tool(args, outdir, std::nullopt);
    if (r.returncode == 0 && py::os::path::isfile(py::os::path::join(outdir, stem + S(".obj")))) {
        if (k) {
            py::file f = py::open(py::os::path::join(outdir, stem + S(".obj")), S("rb"));
            py::bytes data = f.read();
            f.close();
            cache_put(S("objects"), *k, py::bytes(std::string("OK") + data.raw()));
        }
        return S("");
    }
    py::str log = log_of(r);
    std::optional<py::re::Match> m = py::re::search(CL_ERR_RX, log);
    py::str why;
    if (m) {
        why = m->group(2);
    } else {
        py::str t = tail(log.strip(), 100);
        why = t.empty() ? S("the compiler failed") : t;
    }
    why = head(why, 100);
    if (k && m) cache_put(S("objects"), *k, py::bytes(std::string("ER") + message_kept(why, outdir).raw()));
    return why;
}

}  // namespace

BuiltPowerPC build_powerpc(const py::str& header, const py::dict<py::str, py::str>& texts, const py::list<py::str>& order,
                           const py::dict<py::str, int64_t>& externs, const py::str& outdir, int64_t base_,
                           const std::optional<int64_t>& jobs0) {
    auto tools = powerpc_tools();
    if (!tools) throw py::RuntimeError("no PowerPC compiler is set up (settings.json: powerpc_tools, powerpc_lib)");
    auto [cl, link_, lib] = *tools;
    int64_t jobs = (jobs0 && *jobs0) ? *jobs0 : base::parallel::workers();
    py::os::makedirs(outdir, 0777, true);
    _HEADER.set(outdir, cache::key({cache::Part(header)}));
    write_text(py::os::path::join(outdir, S("m.h")), header);
    py::dict<py::str, py::str> stems;
    int64_t i = 0;
    for (const py::str& n : order.vec()) stems.set(n, f_stem(i++));
    py::list<py::str> errors = pool::threads([&](const py::str& n) { return compile_powerpc(cl, outdir, stems.at(n), texts.at(n)); },
                                             order, jobs);
    py::dict<py::str, py::str> dropped;
    for (int64_t j = 0; j < order.size(); ++j)
        if (!errors[j].empty()) dropped.set(order[j], errors[j]);
    for (const py::str& n : dropped.keys())
        if (!compile_powerpc(cl, outdir, stems.at(n), stand_in(texts.at(n))).empty()) stems.pop(n);
    // every extern is a function of its own that does nothing; running stops at its address
    {
        std::vector<py::str> names;
        for (const py::str& n : externs.keys()) names.push_back(n);
        std::sort(names.begin(), names.end());
        std::vector<py::str> parts;
        int64_t e = 0;
        for (const py::str& n : names)
            parts.push_back(S("#pragma optimize(\"\", off)\nuint8 ") + n + S("() { return ") + py::to_str(e++) + S("; }"));
        compile_powerpc(cl, outdir, S("externs"), py::join(S("\n"), py::list<py::str>(parts)));
    }
    note_objects(outdir, stems, dropped);
    std::vector<py::str> objs;
    for (const py::str& n : order.vec())
        if (stems.contains(n)) objs.push_back(stems.at(n) + S(".obj"));
    objs.push_back(S("externs.obj"));
    write_text(py::os::path::join(outdir, S("m.rsp")), py::join(S("\n"), py::list<py::str>(objs)) + S("\n"));
    for (const char* stale : {"m.dll", "m.map"}) remove_quietly(py::os::path::join(outdir, S(stale)));
    char basebuf[32];
    std::snprintf(basebuf, sizeof basebuf, "%llX", static_cast<unsigned long long>(base_));
    py::subprocess::CompletedProcess r = run_tool(
        py::list<py::str>(std::vector<py::str>{link_, S("/nologo"), S("/DLL"), S("/NOENTRY"), S("/NODEFAULTLIB"), S("/FIXED"),
                                               S("/XEX:NO"), S("/OPT:NOREF"), S("/OPT:NOICF"), S("/FORCE"),
                                               S("/IGNORE:4006,4088"), S("/MAP:m.map"), S("/BASE:0x") + py::str(std::string(basebuf)),
                                               S("/OUT:m.dll"), S("@m.rsp"), lib}),
        outdir, std::nullopt);
    py::str dll = py::os::path::join(outdir, S("m.dll"));
    std::vector<py::str> live;
    for (const py::str& n : order.vec())
        if (!dropped.contains(n)) live.push_back(n);
    if (!py::os::path::isfile(dll)) {
        py::str t = tail(log_of(r).strip(), 160);
        py::str why = t.empty() ? S("the linker produced nothing") : t;
        py::dict<py::str, py::str> d = dropped.copy();
        for (const py::str& n : live) d.set(n, S("link: ") + why);
        return BuiltPowerPC{std::nullopt, py::dict<py::str, int64_t>(), d};
    }
    py::str text;
    try {
        text = read_text_errors_replace(py::os::path::join(outdir, S("m.map")));
    } catch (const py::OSError&) {
        text = S("");
    }
    py::set<py::str> want;
    for (const py::str& n : order.vec()) want.add(n);
    for (const py::str& n : externs.keys()) want.add(n);
    py::dict<py::str, int64_t> where;
    for (const py::re::Match& mm : py::re::finditer(MAP_SYM_RX, text).vec())
        if (want.contains(mm.group(1)) && !where.contains(mm.group(1))) where.set(mm.group(1), py::int_parse(mm.group(2), 16));
    return BuiltPowerPC{dll, where, dropped};
}

std::tuple<int64_t, py::str, py::str> run_process(const py::list<py::str>& args, const std::optional<py::str>& cwd,
                                                  const py::dict<py::str, py::str>& env) {
    py::subprocess::RunOptions o;
    o.cwd = cwd;
    o.errors = "replace";
    EnvScope scope(env);
    py::subprocess::CompletedProcess r = py::subprocess::run(args, o);
    return {r.returncode, r.stdout_ ? *r.stdout_ : S(""), r.stderr_ ? *r.stderr_ : S("")};
}

}  // namespace codebreaker::build::functions
