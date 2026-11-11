// codebreaker/source/vxd/symbols.cpp -- typed C++ of codebreaker.source.vxd.symbols (see symbols.hpp).
#include "codebreaker/source/vxd/symbols.hpp"

#include <filesystem>
#include <optional>
#include <vector>

#include "codebreaker/base/corpus.hpp"
#include "py_glob.hpp"
#include "py_settings.hpp"

namespace codebreaker::source::vxd::symbols {

using py::glob::glob;   // glob.glob

namespace {

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
