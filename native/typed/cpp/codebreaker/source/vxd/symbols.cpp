// codebreaker/source/vxd/symbols.cpp -- typed C++ of codebreaker.source.vxd.symbols (see symbols.hpp).
#include "codebreaker/source/vxd/symbols.hpp"

#include <filesystem>
#include <optional>
#include <vector>

#include "codebreaker/base/corpus.hpp"
#include "py_settings.hpp"

namespace codebreaker::source::vxd::symbols {

namespace {

// ---- glob.glob (the runtime has none) ---------------------------------------------------------

bool has_magic_(const py::str& s) {
    for (char32_t c : s.u32())
        if (c == U'*' || c == U'?' || c == U'[') return true;
    return false;
}

bool lexists_(const py::str& p) {
    if (p.u32().find(U'\0') != std::u32string::npos) return false;
    std::error_code ec;
    std::filesystem::file_status st = std::filesystem::symlink_status(std::filesystem::u8path(p.utf8()), ec);
    return !ec && st.type() != std::filesystem::file_type::not_found;
}

// fnmatch's pattern against a name, both already normcase'd
bool fn_match_(const std::u32string& p, size_t pi, const std::u32string& n, size_t ni) {
    while (pi < p.size()) {
        char32_t c = p[pi];
        if (c == U'*') {
            while (pi < p.size() && p[pi] == U'*') pi++;
            if (pi == p.size()) return true;
            for (size_t k = ni; k <= n.size(); k++)
                if (fn_match_(p, pi, n, k)) return true;
            return false;
        }
        if (c == U'?') {
            if (ni >= n.size()) return false;
            pi++;
            ni++;
            continue;
        }
        if (c == U'[') {
            size_t j = pi + 1;
            if (j < p.size() && p[j] == U'!') j++;
            if (j < p.size() && p[j] == U']') j++;
            while (j < p.size() && p[j] != U']') j++;
            if (j >= p.size()) {   // no closing bracket: a literal [
                if (ni >= n.size() || n[ni] != U'[') return false;
                pi++;
                ni++;
                continue;
            }
            if (ni >= n.size()) return false;
            size_t a = pi + 1;
            bool neg = false;
            if (p[a] == U'!') {
                neg = true;
                a++;
            }
            bool hit = false;
            for (size_t i = a; i < j; i++) {
                if (i + 2 < j && p[i + 1] == U'-') {
                    if (p[i] <= n[ni] && n[ni] <= p[i + 2]) hit = true;
                    i += 2;
                } else if (p[i] == n[ni]) {
                    hit = true;
                }
            }
            if (hit == neg) return false;
            pi = j + 1;
            ni++;
            continue;
        }
        if (ni >= n.size() || n[ni] != c) return false;
        pi++;
        ni++;
    }
    return ni == n.size();
}

bool fnmatch_(const py::str& name, const py::str& pat) {
    return fn_match_(py::os::path::normcase(pat).u32(), 0, py::os::path::normcase(name).u32(), 0);
}

bool ishidden_(const py::str& s) { return !s.empty() && s.u32()[0] == U'.'; }

// glob._glob1(dirname, pattern): the names in dirname matching a wildcard pattern
py::list<py::str> glob1_(const py::str& dirname, const py::str& pattern) {
    py::list<py::str> names;
    try {
        names = py::os::listdir(dirname.empty() ? py::os::curdir : dirname);
    } catch (const py::OSError&) {
        return py::list<py::str>{};
    }
    py::list<py::str> out;
    bool hidden_ok = ishidden_(pattern);
    for (const py::str& n : names)
        if ((hidden_ok || !ishidden_(n)) && fnmatch_(n, pattern)) out.append(n);
    return out;
}

// glob._glob0(dirname, basename): a name that is not a wildcard
py::list<py::str> glob0_(const py::str& dirname, const py::str& basename) {
    py::list<py::str> out;
    if (basename.empty()) {
        if (py::os::path::isdir(dirname)) out.append(basename);
    } else if (lexists_(py::os::path::join(dirname, basename))) {
        out.append(basename);
    }
    return out;
}

// glob.glob(pathname)
py::list<py::str> glob(const py::str& pathname) {
    py::list<py::str> out;
    auto [dirname, basename] = py::os::path::split(pathname);
    if (!has_magic_(pathname)) {
        if (!basename.empty()) {
            if (lexists_(pathname)) out.append(pathname);
        } else if (py::os::path::isdir(dirname)) {
            out.append(pathname);
        }
        return out;
    }
    if (dirname.empty()) {
        for (const py::str& n : glob1_(py::str(""), basename)) out.append(n);
        return out;
    }
    py::list<py::str> dirs;
    if (dirname != pathname && has_magic_(dirname)) dirs = glob(dirname);
    else dirs = py::list<py::str>{dirname};
    for (const py::str& d : dirs) {
        py::list<py::str> names = has_magic_(basename) ? glob1_(d, basename) : glob0_(d, basename);
        for (const py::str& n : names) out.append(py::os::path::join(d, n));
    }
    return out;
}

// open(path, encoding="latin1").read(): text mode, so \r\n and a lone \r are \n
py::str read_latin1_text_(const py::str& path) {
    py::bytes raw;
    {
        py::file f = py::open_rb(path);
        raw = f.read();
    }
    return raw.decode("latin1").replace(py::str("\r\n"), py::str("\n")).replace(py::str("\r"), py::str("\n"));
}

// int(text, 16) of hex digits, held at INT64_MAX where a Python int would be beyond it (the caller only
// asks whether the value is under 0x40)
int64_t hex_value_(const py::str& digits) {
    int64_t v = 0;
    for (char32_t c : digits.u32()) {
        int64_t d = c >= U'0' && c <= U'9' ? c - U'0' : (c | 0x20) - U'a' + 10;
        if (v > (INT64_MAX - d) / 16) return INT64_MAX;
        v = v * 16 + d;
    }
    return v;
}

struct Paths {
    py::list<py::str> inc, debug;
};

const Paths& paths_() {
    static const Paths p = [] {
        py::str dr = py::os::path::dirname(py::settings::checkout_root());   // dirname(settings.ROOT)
        Paths out;
        out.inc.append(base::corpus::reference(py::list<py::str>{py::str("Microsoft Windows 95 DDK"), py::str("INC32")}));
        out.inc.append(dr + py::str("/docs/reference/win95ddk/INC32"));
        out.debug.append(base::corpus::reference(py::list<py::str>{py::str("Microsoft Windows 95 DDK"), py::str("DEBUG")}));
        out.debug.append(dr + py::str("/docs/reference/win95ddk/DEBUG"));
        return out;
    }();
    return p;
}

}  // namespace

py::str HERE() {
    static const py::str h = py::os::path::join(py::settings::checkout_root(), py::str("codebreaker"), py::str("source"), py::str("vxd"));
    return h;
}

py::str ROOT() {
    static const py::str r = py::os::path::dirname(py::os::path::dirname(HERE()));
    return r;
}

const py::list<py::str>& _INC() { return paths_().inc; }
const py::list<py::str>& _DEBUG() { return paths_().debug; }

std::optional<py::str> _find(const py::list<py::str>& cands, const py::str& leaf) {
    for (const py::str& c : cands) {
        py::str p = !leaf.empty() ? py::os::path::join(ROOT(), c, leaf) : py::os::path::join(ROOT(), c);
        if (py::os::path::exists(p)) return p;
    }
    return std::nullopt;
}

py::dict<int64_t, py::str> control_messages() {
    static std::optional<py::dict<int64_t, py::str>> kept;   // functools.lru_cache(maxsize=1)
    if (kept) return *kept;
    py::dict<int64_t, py::str> out;
    std::optional<py::str> vmm = _find(_INC(), py::str("VMM.INC"));
    if (!vmm) {
        kept = out;
        return out;
    }
    py::str txt = read_latin1_text_(*vmm);
    bool started = false;
    for (const py::str& line : py::splitlines(txt)) {
        auto m = py::re::match(py::str("\\s*([A-Za-z_][A-Za-z0-9_]*)\\s+EQU\\s+([0-9A-Fa-f]+)H\\s*$"), line);
        if (!m) continue;
        py::str name = m->group(1);
        int64_t val = hex_value_(m->group(2));
        if (name.upper() == py::str("SYS_CRITICAL_INIT")) started = true;
        if (!started || val >= 0x40) continue;
        py::str u = name.upper();
        if (u.endswith(py::str("_BIT")) || u.endswith(py::str("_FLAG")) || u.endswith(py::str("_ID")) ||
            u.startswith(py::str("DIOC")))
            continue;
        out.setdefault(val, name);
        if (u == py::str("SYSTEM_EXIT2")) break;   // tail of the message block
    }
    kept = out;
    return out;
}

py::dict<int64_t, py::str> sym_names(const py::str& device) {
    std::optional<py::str> dbg = _find(_DEBUG());
    if (!dbg) return py::dict<int64_t, py::str>();
    py::list<py::str> hits = glob(py::os::path::join(*dbg, device.upper() + py::str(".SYM")));
    if (hits.empty()) hits = glob(py::os::path::join(*dbg, device.lower() + py::str(".sym")));
    if (hits.empty()) return py::dict<int64_t, py::str>();
    py::bytes d;
    {
        py::file f = py::open_rb(hits[0]);
        d = f.read();
    }
    py::dict<int64_t, py::str> out;
    int64_t p = 0;
    while (p < py::len(d) - 3) {
        int64_t off = py::unpack_from1<int64_t>("<H", d, p);
        int64_t ln = d[p + 2];
        py::bytes nm = d.slice(p + 3, p + 3 + ln);
        bool ok = 3 <= ln && ln <= 40;
        if (ok) {
            for (int64_t i = 0; i < py::len(nm); i++) {
                int64_t c = nm[i];
                if (!(c == 0x5F || (48 <= c && c <= 57) || (65 <= c && c <= 90) || (97 <= c && c <= 122))) {
                    ok = false;
                    break;
                }
            }
        }
        if (ok) {
            out.setdefault(off, nm.decode("latin1"));
            p += 3 + ln;
        } else {
            p += 1;
        }
    }
    // drop the segment/group markers (offsets 0xff00/0x1a00-style with class names)
    py::dict<int64_t, py::str> kept;
    for (const auto& kv : out.items()) {
        const py::str& n = std::get<1>(kv);
        if (!n.startswith(py::str("_")) || n.count(py::str("_")) > 1) kept.set(std::get<0>(kv), n);
    }
    return kept;
}

}  // namespace codebreaker::source::vxd::symbols
