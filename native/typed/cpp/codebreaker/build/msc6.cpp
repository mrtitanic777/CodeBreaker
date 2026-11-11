// codebreaker/build/msc6.cpp -- typed C++17 port of codebreaker.build.msc6 (see msc6.hpp).
//
// msc6.py -- Microsoft C 6.0a, the compiler of the 16-bit Windows modules, run for comparison.
//
// The compiler is a DOS program. It is run in DOSBox-X (settings: dosbox_x -- the emulator's program; msc6 -- the
// folder holding the compiler's BIN and INCLUDE), every source of a call in ONE session: starting the emulator costs
// about two seconds, compiling a function a fraction of one. Each function is compiled as a file of its own and its
// code read out of the object file, with the bytes the linker fills in (an address, a segment, the distance of a call
// to another file) marked in `mask` and left out of every comparison -- as build/period.py's other compilers.
//
// What the reconstruction is written in for judging is OpenWatcom's dialect; `dialect()` is the same text said the way
// this compiler takes it (its keywords for far and the calling conventions, no 64-bit integers).

#include "codebreaker/build/msc6.hpp"

#include <algorithm>
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <map>
#include <mutex>
#include <set>
#include <sstream>
#include <thread>

#include "codebreaker/base/cache.hpp"
#include "codebreaker/build/dos_tools.hpp"
#include "codebreaker/formats/omf.hpp"
#include "codebreaker/symbols/ordinals.hpp"
#include "py_settings.hpp"
#include "py_subprocess.hpp"

namespace codebreaker::build::msc6 {

using py::str;
namespace fs = std::filesystem;
namespace re = py::re;

// /Gc: a function declared without a convention is pascal, as the reconstruction's own default
const py::list<py::str> FLAG_SETS = {
    py::str("/AM /Gcsw /Os"), py::str("/AM /Gcsw /Ox"), py::str("/AS /Gcsw /Os"),  py::str("/AL /Gcsw /Os"), py::str("/AM /Gcsw /Od"),
    py::str("/AM /Gc2sw /Os"), py::str("/AM /Gc2sw /Ox"), py::str("/AM /Gcs /Os"), py::str("/AS /Gcs /Os")};   // (/G2: SUWIN has push imm and leave)

namespace {

constexpr int64_t RE_I = 2, RE_M = 8, RE_S = 16;

// ---- small helpers the Python gets from its builtins -------------------------------------------------------------

// "%0*x" % (width, value)
str hex_(int64_t v, int width = 0) {
    char buf[40];
    std::snprintf(buf, sizeof buf, "%0*llx", width, static_cast<unsigned long long>(v));
    return str(std::string(buf));
}

str num_(int64_t v) { return str(std::to_string(v)); }

// a pattern compiled once
const re::Pattern& rx(const char* pattern, int64_t flags = 0) {
    thread_local std::map<std::string, re::Pattern> held;
    auto it = held.find(std::string(pattern) + "\n" + std::to_string(flags));
    if (it == held.end()) it = held.emplace(std::string(pattern) + "\n" + std::to_string(flags), re::Pattern(pattern, flags)).first;
    return it->second;
}

// bytes of a file, or None
std::optional<py::bytes> read_(const str& path) {
    std::ifstream f(fs::u8path(path.utf8()), std::ios::binary);
    if (!fs::is_regular_file(fs::u8path(path.utf8()))) return std::nullopt;
    if (!f) return std::nullopt;
    std::ostringstream o;
    o << f.rdbuf();
    return py::bytes(o.str());
}

// open(path, encoding="latin1").read(): universal newlines
std::optional<str> read_text_latin1_(const str& path) {
    auto raw = read_(path);
    if (!raw) return std::nullopt;
    str t = raw->decode("latin1");
    t = t.replace(str("\r\n"), str("\n"));
    t = t.replace(str("\r"), str("\n"));
    return t;
}

// open(path, "w", newline="\r\n", encoding="latin1", errors="replace").write(text)
void write_text_latin1_(const str& path, const str& text) {
    py::bytes b = text.replace(str("\n"), str("\r\n")).encode("latin1", "replace");
    std::ofstream f(fs::u8path(path.utf8()), std::ios::binary | std::ios::trunc);
    if (!f) throw py::FileNotFoundError("cannot write " + path.utf8());
    f.write(b.raw().data(), static_cast<std::streamsize>(b.raw().size()));
}

// os.listdir(folder)
std::vector<str> listdir_(const str& folder) {
    std::vector<str> out;
    std::error_code ec;
    for (auto it = fs::directory_iterator(fs::u8path(folder.utf8()), ec); !ec && it != fs::directory_iterator(); it.increment(ec))
        out.push_back(str(it->path().filename().u8string()));
    return out;
}

bool ends_with_any_(const str& name, std::initializer_list<const char*> suffixes) {
    str low = name.lower();
    for (const char* s : suffixes)
        if (low.endswith(str(s))) return true;
    return false;
}

// the setting as `str(settings.get(key) or "")`
str setting_text_or_empty_(const char* key) {
    py::settings::Value v = py::settings::get(str(key));
    if (auto* s = std::get_if<str>(&v)) return *s;
    if (auto* i = std::get_if<int64_t>(&v)) return *i ? num_(*i) : str("");
    if (auto* d = std::get_if<double>(&v)) return *d != 0.0 ? py::to_str(*d) : str("");
    if (auto* b = std::get_if<bool>(&v)) return *b ? str("True") : str("");
    return str("");
}

}  // namespace

// ---------------------------------------------------------------------------------------------------------------------

str module_file() {
    // (build/module.py hashes the source text of this module into its cache stamp: the Python module's path)
    auto root = py::os::getenv(str("CB_CHECKOUT_ROOT"));
    return py::os::path::join(root ? *root : str("."), str("codebreaker"), str("build"), str("msc6.py"));
}

std::tuple<std::optional<str>, std::optional<str>> _tools(const str& compiler) {
    // (DOSBox-X, the compiler's folder). compiler: "msc6" (Microsoft C 6.0a) or "msvc152" (Microsoft C/C++ 8.00c of
    // Visual C++ 1.52c, which built a few of the 16-bit modules); both are a BIN\CL.EXE run the same way.
    return {py::settings::get_opt_str(str("dosbox_x")), py::settings::get_opt_str(compiler)};
}

bool available(const str& compiler) {
    auto [emulator, home] = _tools(compiler);
    return emulator && !emulator->empty() && py::os::path::isfile(*emulator) && home && !home->empty() &&
           py::os::path::isfile(py::os::path::join(*home, str("BIN"), str("CL.EXE")));
}

Code object_code(const py::bytes& obj) {
    // (bytes, mask) of the first code segment of an OMF object file: the data records of that segment laid end to end,
    // and 1 for every byte a fix-up record names.
    std::string code, mask;
    int64_t p = 0;
    std::optional<std::tuple<int64_t, int64_t>> last;   // (segment, offset of the record's data in `code`)
    const int64_t n = obj.size();
    while (p + 3 <= n) {
        int64_t kind = obj.byte_(p);
        int64_t size = obj.byte_(p + 1) | (obj.byte_(p + 2) << 8);
        py::bytes body = obj.slice(p + 3, p + 3 + size - 1);
        p += 3 + size;
        if (kind == 0xA0 || kind == 0xA1) {   // LEDATA: segment, offset, bytes
            int64_t i = 0;
            int64_t seg = body[i];
            i += 1;
            if (seg & 0x80) {
                seg = ((seg & 0x7F) << 8) | body[i];
                i += 1;
            }
            int64_t width = kind == 0xA1 ? 4 : 2;
            py::bytes offb = body.slice(i, i + width);
            int64_t off = 0;
            for (int64_t b = offb.size() - 1; b >= 0; --b) off = (off << 8) | offb.byte_(b);
            i += width;
            if (seg != 1) {
                last.reset();
                continue;
            }
            if (off > static_cast<int64_t>(code.size())) {
                code.resize(static_cast<size_t>(off), '\0');
                mask.resize(static_cast<size_t>(off), '\0');
            }
            int64_t count = body.size() - i;
            if (count < 0) throw py::ValueError("negative count");   // (bytes(len(body) - i))
            if (static_cast<int64_t>(code.size()) < off + count) code.resize(static_cast<size_t>(off + count), '\0');
            if (static_cast<int64_t>(mask.size()) < off + count) mask.resize(static_cast<size_t>(off + count), '\0');
            for (int64_t b = 0; b < count; ++b) {
                code[static_cast<size_t>(off + b)] = body.raw()[static_cast<size_t>(i + b)];
                mask[static_cast<size_t>(off + b)] = '\0';
            }
            last = std::make_tuple(seg, off);
        } else if ((kind == 0x9C || kind == 0x9D) && last) {   // FIXUPP: the places in the record before it
            int64_t i = 0;
            const int64_t m = body.size();
            while (i < m) {
                int64_t b = body[i];
                if (!(b & 0x80)) {   // a thread: one byte, and an index for most methods
                    i += 1;
                    if (((b >> 2) & 7) < 4 && i < m) i += (body[i] & 0x80) ? 2 : 1;
                    continue;
                }
                int64_t where = ((b & 3) << 8) | body[i + 1];
                int64_t loc = (b >> 2) & 0xF;
                int64_t fix = body[i + 2];
                i += 3;
                if (!(fix & 0x80) && ((fix >> 4) & 7) < 3) i += (body[i] & 0x80) ? 2 : 1;   // the frame is given here
                if (!(fix & 0x08)) i += (body[i] & 0x80) ? 2 : 1;                           // the target is given here
                if (!(fix & 0x04)) i += kind == 0x9D ? 4 : 2;                               // with a displacement
                int64_t width = 2;
                switch (loc) {
                    case 0: width = 1; break;
                    case 1: width = 2; break;
                    case 2: width = 2; break;
                    case 3: width = 4; break;
                    case 4: width = 1; break;
                    case 5: width = 2; break;
                    case 9: width = 4; break;
                    case 11: width = 6; break;
                    case 13: width = 4; break;
                    default: width = 2;
                }
                int64_t at = std::get<1>(*last) + where;
                int64_t len = static_cast<int64_t>(mask.size());
                int64_t fill = std::min(width, std::max<int64_t>(0, len - at));
                for (int64_t b2 = 0; b2 < fill; ++b2) mask[static_cast<size_t>(at + b2)] = '\1';
            }
        } else if (kind == 0x8A || kind == 0x8B) {
            break;
        }
    }
    return {py::bytes(code), py::bytes(mask)};
}

std::tuple<str, str> dialect(const str& header, const str& text) {
    // (header, text) as Microsoft C 6 takes them.
    py::list<str> said = {
        str("#define __far _far"), str("#define __near _near"), str("#define __stdcall _pascal"), str("#define __cdecl _cdecl"),
        str("#define codebreaker_ss() 0"), str("#define codebreaker_ds() 0"), str("#define codebreaker_cs() 0"),
        // OpenWatcom's spellings of what Microsoft C 6 has with one underscore, and the far-pointer macros OpenWatcom's i86.h
        // gives (here without a header)
        str("#define __based _based"), str("#define __segname _segname"), str("#define __export _export"),
        str("#define __loadds _loadds"), str("#define __interrupt _interrupt"), str("#define __saveregs _saveregs"),
        // (OpenWatcom's _Packed: the frame structure c_text writes is of words -- no padding to lose)
        str("#define _Packed"),
        // (Microsoft's _fastcall: arguments in AX, DX, BX -- describe's __fastcall16 model)
        str("#define __fastcall _fastcall"), str("#define __fastcall16near __near __fastcall"),
        str("#define __fastcall16far __far __fastcall"),
        str("#define MK_FP(s,o) ((void _far *)(((unsigned long)(unsigned)(s) << 16) | (unsigned)(o)))"),
        str("#define FP_OFF(p) ((unsigned)(unsigned long)(void _far *)(p))"),
        str("#define FP_SEG(p) ((unsigned)((unsigned long)(void _far *)(p) >> 16))")};
    py::list<str> keep;
    for (const str& line0 : py::split(header, str("\n"))) {
        if (line0.contains(str("#pragma aux")) || rx(R"RX(uint2 codebreaker_[sdc]s\(void\);)RX").match(line0.strip()))
            continue;
        str line = line0.replace(str("unsigned long long"), str("unsigned long"))
                       .replace(str("long long"), str("long"))
                       .replace(str("long double"), str("double"));
        keep.append(line);
    }
    py::list<str> all = said;
    all.extend(keep);
    return {py::join(str("\n"), all) + str("\n"), text};
}

// The reconstruction says a callee-pops convention as OpenWatcom's __stdcall, whose arguments go on the stack right to
// left: the C lists them in that order. Microsoft's _pascal pushes them left to right, so the same list said as _pascal is
// backwards -- the Windows API and every pascal function of a 1990s module were written with the first-pushed argument first.
namespace {
const re::Pattern& PASCAL_() {
    return rx(R"RX(^(?!\s*#)[^\n]*?__stdcall(?:16(?:far|near))?\b[^;(){}=\n]*?\b(\w+)\s*\()RX", RE_M);
}
const char* SEGS_[] = {"unaff_SS", "codebreaker_ss()", "unaff_DS", "codebreaker_ds()"};
bool is_seg_(const str& s) {
    for (const char* x : SEGS_)
        if (s == x) return true;
    return false;
}
}  // namespace

py::set<str> pascal_names(const py::list<str>& texts) {
    py::set<str> out;
    for (const str& t : texts)
        for (const re::Match& m : PASCAL_().finditer(t)) out.add(m.group(1));
    return out;
}

namespace {

// the index past the parenthesis that closes the one before `i` (string and character literals skipped), or -1
int64_t close_(const str& text, int64_t i) {
    const std::u32string& t = text.u32();
    const int64_t n = static_cast<int64_t>(t.size());
    int64_t depth = 1;
    while (i < n) {
        char32_t c = t[static_cast<size_t>(i)];
        if (c == U'"' || c == U'\'') {
            int64_t j = i + 1;
            while (j < n && t[static_cast<size_t>(j)] != c) j += t[static_cast<size_t>(j)] == U'\\' ? 2 : 1;
            i = j + 1;
            continue;
        }
        depth += c == U'(';
        depth -= c == U')';
        i += 1;
        if (!depth) return i;
    }
    return -1;
}

// an argument list cut at its top-level commas
py::list<str> arguments_(const str& inner) {
    py::list<str> out;
    const std::u32string& t = inner.u32();
    const int64_t n = static_cast<int64_t>(t.size());
    int64_t depth = 0, start = 0, i = 0;
    while (i < n) {
        char32_t c = t[static_cast<size_t>(i)];
        if (c == U'"' || c == U'\'') {
            int64_t j = i + 1;
            while (j < n && t[static_cast<size_t>(j)] != c) j += t[static_cast<size_t>(j)] == U'\\' ? 2 : 1;
            i = j + 1;
            continue;
        }
        depth += (c == U'(' || c == U'[' || c == U'{');
        depth -= (c == U')' || c == U']' || c == U'}');
        if (c == U',' && !depth) {
            out.append(inner.slice(start, i));
            start = i + 1;
        }
        i += 1;
    }
    if (!inner.strip().empty()) out.append(inner.slice_from(start));
    return out;
}

// an offset followed by the segment register it is in (right-to-left order: the offset is pushed last, so it comes first)
// is one far pointer, as the source had it
py::list<str> far_pairs_(const py::list<str>& args) {
    py::list<str> out;
    int64_t k = 0;
    const int64_t n = args.size();
    while (k < n) {
        std::optional<re::Match> lo, hi;
        if (k + 1 < n) lo = rx(R"RX(^\s*\(\s*u?int2\s*\)\s*(.+?)\s*$)RX").match(args[k]);
        if (lo)
            hi = rx(R"RX(^\s*\(\s*u?int2\s*\)\s*\(\s*\(\s*u?int4\s*\)\s*(.+?)\s*>>\s*(?:0x10|16)\s*\)\s*$)RX").match(args[k + 1]);
        if (lo && hi && lo->group(1).strip(str("() ")) == hi->group(1).strip(str("() "))) {
            out.append(str(" ") + lo->group(1));   // a 4-byte value split into its words: the value
            k += 2;
            continue;
        }
        if (k + 1 < n && is_seg_(args[k + 1].strip())) {
            str off = rx(R"RX(^\s*\(\s*u?int2\s*\)\s*)RX").sub(str(""), args[k]).strip();
            out.append(str(" (void __far *)") + (off.startswith(str("&")) ? off : str("(char __near *)(") + off + str(")")));
            k += 2;
        } else {
            out.append(args[k]);
            k += 1;
        }
    }
    return out;
}

}  // namespace

str pascal_order(const str& text, const py::set<str>& names) {
    // `text` with the arguments of every call to, and the parameters of every definition of, a function in `names` in Pascal
    // order: first pushed first, an offset and its segment one far pointer.
    std::vector<str> out;
    int64_t i = 0;
    const re::Pattern& call = rx(R"RX(\b(\w+)\s*\()RX");
    for (;;) {
        std::optional<re::Match> m = call.search(text, i);
        if (!m) {
            out.push_back(text.slice_from(i));
            str r;
            for (const str& p : out) r += p;
            return r;
        }
        int64_t end = close_(text, m->end());
        if (!names.contains(m->group(1)) || end < 0) {
            out.push_back(text.slice(i, m->end()));
            i = m->end();
            continue;
        }
        py::list<str> parts;
        for (const str& a : arguments_(text.slice(m->end(), end - 1))) parts.append(pascal_order(a, names));
        py::list<str> args = far_pairs_(parts);
        std::reverse(args.vec().begin(), args.vec().end());
        py::list<str> stripped;
        for (const str& a : args) stripped.append(a.strip());
        out.push_back(text.slice(i, m->end()) + py::join(str(", "), stripped) + str(")"));
        i = end;
    }
}

namespace {
const re::Pattern& IMPORT_() { return rx(R"RX(^extern [^;\n]*__stdcall\s+([A-Z][A-Z0-9]*)_(\d+)\s*\(\s*\);[ \t]*$)RX", RE_M); }
}  // namespace

std::optional<str> windows_h() {
    // the Windows SDK's WINDOWS.H (Visual C++ 1.52c's, the Windows 3.1 one), or None
    std::optional<str> home = py::settings::get_opt_str(str("msvc152"));
    str path = py::os::path::join(home ? *home : str(""), str("INCLUDE"), str("WINDOWS.H"));
    if (!home || home->empty() || !py::os::path::isfile(path)) return std::nullopt;
    std::optional<str> t = read_text_latin1_(path);
    if (!t) return std::nullopt;
    return t->replace(str("\r\n"), str("\n"));
}

str cdecl_imports(const str& header, const std::optional<str>& sdk_in) {
    // `header` with every import by number that is really a C function declared __cdecl, not __stdcall: one its module
    // exports under a C name (`_SUFORMATMESSAGEBOX` -- a pascal export is in capitals, a cdecl one keeps the compiler's
    // underscore), or one WINDOWS.H declares CDECL (wsprintf). The caller removes such a function's arguments (`add sp,
    // 12h` after the call), and its argument list is in C order.
    str sdk;
    if (sdk_in) sdk = *sdk_in;
    else {
        std::optional<str> w = windows_h();
        sdk = w && !w->empty() ? *w : str("");
    }
    py::set<str> cdecl_names;
    for (const str& s : rx(R"RX(\bCDECL\s+(\w+)\s*\()RX").findall(sdk)) cdecl_names.add(s);
    return IMPORT_().sub(
        [&](const re::Match& m) {
            std::optional<str> nm0 = symbols::ordinals::name(m.group(1), py::int_parse(m.group(2)));
            str nm = nm0 && !nm0->empty() ? *nm0 : str("");
            if (nm.startswith(str("_")) || cdecl_names.contains(nm)) return m.group(0).replace(str("__stdcall"), str("__cdecl"));
            return m.group(0);
        },
        header);
}

std::tuple<str, py::dict<str, str>> wine_typing(const str& header, const py::dict<str, str>& texts, const ExportLookup& exports) {
    // (header, {name: text}) with every import by number that Wine's .spec types (symbols/wine16.py) declared with its
    // parameters and its result's width, and each call of it said with one argument to a parameter -- a 4-byte parameter's
    // two words joined (high first, as a pascal call pushes them; low first in a C call's list). The texts are in Pascal
    // order (pascal_order). An import with a call whose arguments fit neither way keeps its declaration without parameters.
    // `exports`: (module, ordinal) -> wine16.Export, for tests.
    namespace wine16 = symbols::wine16;
    ExportLookup look = exports;
    if (!look)
        look = [](const str& mod, int64_t n) -> std::optional<wine16::Export> {
            py::dict<int64_t, wine16::Export> table = wine16::exports(mod);
            if (!table.contains(n)) return std::nullopt;
            return table.at(n);
        };
    py::dict<str, wine16::Export> typed;
    for (const re::Match& m : IMPORT_().finditer(header)) {
        std::optional<wine16::Export> e = look(m.group(1), py::int_parse(m.group(2)));
        if (e && e->args && (e->convention == "pascal" || e->convention == "cdecl")) typed.set(m.group(1) + str("_") + m.group(2), *e);
    }
    py::list<str> names = py::sorted(py::list<str>::from(typed.keys()));
    const re::Pattern call = typed.empty() ? re::Pattern(R"RX((?!x)x)RX", 0)
                                           : re::Pattern(str("\\b(") + py::join(str("|"), names) + str(")\\s*\\("), 0);

    struct Call {
        re::Match mm;
        int64_t end;
        py::list<str> args;
    };
    auto calls = [&](const str& text) {
        std::vector<Call> out;
        for (const re::Match& mm : call.finditer(text)) {
            int64_t end = close_(text, mm.end());
            if (end > 0) out.push_back(Call{mm, end, arguments_(text.slice(mm.end(), end - 1))});
        }
        return out;
    };
    py::dict<str, bool> fits;
    for (const str& key : texts.keys()) {
        for (const Call& c : calls(texts.at(key))) {
            const wine16::Export& e = typed.at(c.mm.group(1));
            int64_t halves = 0;
            for (const str& a : *e.args) halves += wine16::WIDTH.at(a) / 2;
            bool ok = c.args.size() == e.args->size() || c.args.size() == halves;
            fits.set(c.mm.group(1), fits.get(c.mm.group(1), true) && ok);
        }
    }
    py::dict<str, wine16::Export> use;
    for (const auto& kv : typed.items())
        if (fits.get(std::get<0>(kv), true)) use.set(std::get<0>(kv), std::get<1>(kv));

    auto said = [&](const str& text) {
        std::vector<str> out;
        int64_t i = 0;
        for (const Call& c : calls(text)) {
            if (!use.contains(c.mm.group(1))) continue;
            const wine16::Export& e = use.at(c.mm.group(1));
            py::list<str> args;
            for (const str& a : c.args) args.append(a.strip());
            if (args.size() != e.args->size()) {
                py::list<str> joined;
                int64_t k = 0;
                for (const str& kind : *e.args) {
                    if (wine16::WIDTH.at(kind) == 4) {
                        str hi, lo;
                        if (e.convention == "pascal") {
                            hi = args[k];
                            lo = args[k + 1];
                        } else {
                            hi = args[k + 1];
                            lo = args[k];
                        }
                        str v = str("(((uint4)(uint2)(") + hi + str(") << 16) | (uint2)(") + lo + str("))");
                        joined.append(wine16::POINTER.contains(kind) ? str("(void __far *)") + v : v);
                        k += 2;
                    } else {
                        joined.append(args[k]);
                        k += 1;
                    }
                }
                args = joined;
            }
            out.push_back(text.slice(i, c.mm.end()) + py::join(str(", "), args) + str(")"));
            i = c.end;
        }
        out.push_back(text.slice_from(i));
        str r;
        for (const str& p : out) r += p;
        return r;
    };

    str header_out = IMPORT_().sub(
        [&](const re::Match& m) {
            str nm = m.group(1) + str("_") + m.group(2);
            if (!use.contains(nm)) return m.group(0);
            const wine16::Export& e = use.at(nm);
            str conv = e.convention == "pascal" ? str("__stdcall") : str("__cdecl");
            py::list<str> types;
            for (const str& a : *e.args) types.append(wine16::c_type(a));
            str plist = py::join(str(", "), types);
            return str("extern ") + (e.ret16 ? str("uint2") : str("uint4")) + str(" __far ") + conv + str(" ") + nm + str("(") +
                   (plist.empty() ? str("void") : plist) + str(");");
        },
        header);
    py::dict<str, str> texts_out;
    for (const auto& kv : texts.items()) texts_out.set(std::get<0>(kv), said(std::get<1>(kv)));
    return {header_out, texts_out};
}

str wine_definition(const str& text, const std::optional<symbols::wine16::Export>& export_) {
    // `text` (as decompiled: parameters in stack order, a 4-byte one as its low word then its high) with the parameters of
    // an exported function typed as Wine's .spec has them (`export`, a wine16.Export): each 4-byte parameter's two words one
    // parameter -- a far pointer or a long -- used as such where the two are passed on together, and as FP_OFF/FP_SEG (or
    // its halves) where one is used alone. Unchanged when the words do not add up to the spec's parameters.
    namespace wine16 = symbols::wine16;
    if (!export_ || !export_->args) return text;
    std::optional<re::Match> m = rx(R"RX((?s)(.*?\b\w+\s*\()([\w\s,]*)(\)\s*\n)(.*?)(\{.*))RX").match(text);
    if (!m) return text;
    str head = m->group(1), plist = m->group(2), close = m->group(3), decls = m->group(4), body = m->group(5);
    py::list<str> params;
    for (const str& p : py::split(plist, str(",")))
        if (!p.strip().empty()) params.append(p.strip());
    py::list<str> kinds = py::list<str>::from(*export_->args);   // (a new list: reversing it must not turn the export's own)
    if (export_->convention == "pascal") std::reverse(kinds.vec().begin(), kinds.vec().end());
    int64_t halves = 0;
    for (const str& k : kinds) halves += wine16::WIDTH.at(k) / 2;
    if (params.size() != halves || params.size() == kinds.size()) return text;
    py::list<str> out;
    int64_t k = 0;
    for (const str& kind : kinds) {
        if (wine16::WIDTH.at(kind) == 2) {
            out.append(params[k]);
            k += 1;
            continue;
        }
        str lo = params[k], hi = params[k + 1];
        k += 2;
        out.append(lo);
        bool ptr = wine16::POINTER.contains(kind);
        str whole = str(std::string(1, '\0')) + num_(k) + str(std::string(1, '\0'));   // (the pair, kept out of the way while its halves change)
        auto fixed = [&](const str& with) { return [with](const re::Match&) { return with; }; };
        body = re::Pattern(str("(?<![\\w.])") + re::escape(lo) + str("\\s*,\\s*") + re::escape(hi) + str("\\b"), 0).sub(fixed(whole), body);
        str hi_text = ptr ? str("FP_SEG(") + whole + str(")") : str("((uint2)((uint4)") + whole + str(" >> 16))");
        body = re::Pattern(str("(?<![\\w.])") + re::escape(hi) + str("\\b"), 0).sub(fixed(hi_text), body);
        str lo_text = ptr ? str("FP_OFF(") + whole + str(")") : str("((uint2)") + whole + str(")");
        body = re::Pattern(str("(?<![\\w.])") + re::escape(lo) + str("\\b"), 0).sub(fixed(lo_text), body);
        body = body.replace(whole, lo);
        decls = re::Pattern(str("^\\s*\\w+\\s+") + re::escape(hi) + str("\\s*;[ \\t]*\\n"), RE_M).sub(str(""), decls);
        str ctype = wine16::c_type(kind);
        decls = re::Pattern(str("^(\\s*)\\w+(\\s+)") + re::escape(lo) + str("\\s*;"), RE_M)
                    .sub([&](const re::Match& mm) { return mm.group(1) + ctype + mm.group(2) + lo + str(";"); }, decls);
    }
    return head + py::join(str(","), out) + close + decls + body;
}

str sdk_header(const str& header, const std::optional<str>& sdk_in) {
    // WINDOWS.H, then `header` (whose imports by number keep their own declarations: a call that cannot be said through the
    // SDK's prototype -- sdk_calls -- stays as it was)
    std::optional<str> sdk = sdk_in ? sdk_in : windows_h();
    return sdk && !sdk->empty() ? *sdk + str("\n") + header : header;
}

namespace {
const re::Pattern& WIDE_() {
    return rx(R"RX(FAR\s*\*|\bLP|\bH?PSTR\b|\bLONG\b|\bDWORD\b|\bFARPROC\b|\bSEGPTR\b|\bCOLORREF\b|\bLRESULT\b)RX");
}
const re::Pattern& POINTERS_() { return rx(R"RX(FAR\s*\*|\bLP(?!ARAM\b)|\bH?PSTR\b|\bFARPROC\b|\bSEGPTR\b)RX"); }
}  // namespace

Prototypes sdk_prototypes(const std::optional<str>& sdk_in) {
    // {API name: [parameter type, ...]} of every WINAPI function WINDOWS.H declares on a line, or None for one with a
    // variable argument list; a function that returns nothing has its name in the set `.void` of the result
    std::optional<str> sdk = sdk_in ? sdk_in : windows_h();
    Prototypes out;
    for (const re::Match& m : rx(R"RX(^([^\n#]*?)\bWINAPI\s+(\w+)\s*\(([^()]*)\)\s*;)RX", RE_M).finditer(sdk && !sdk->empty() ? *sdk : str(""))) {
        py::list<str> params;
        for (const str& p : py::split(m.group(3), str(","))) params.append(p.strip());
        std::optional<py::list<str>> value;
        bool variadic = false, none_ = false;
        for (const str& p : params) variadic = variadic || p == "...";
        if (params.size() == 1 && (params[0] == "void" || params[0] == "VOID" || params[0] == "")) none_ = true;
        if (variadic) value = std::nullopt;
        else if (none_) value = py::list<str>();
        else value = params;
        out.params.set(m.group(2), value);
        if (rx(R"RX(\s*(void|VOID)\s*)RX").fullmatch(m.group(1))) out.void_.add(m.group(2));
    }
    return out;
}

str sdk_calls(const str& text, const Prototypes& protos) {
    // `text` (in Pascal order) with each call of an import by number that the SDK declares said by its name, `USER_33(...)`
    // -> `GetClientRect(...)` -- the module's maker called the API through its prototype, which decides how every argument
    // is passed. A call whose arguments are one to a parameter is renamed; one whose arguments are one to a word of the
    // parameters has each 4-byte parameter's two words (high first, as pushed) joined -- MAKELP for a pointer, MAKELONG for
    // a number; any other call keeps its number and its declaration without parameters.
    std::vector<str> out;
    int64_t i = 0;
    const re::Pattern& call = rx(R"RX(\b([A-Z][A-Z0-9]*)_(\d+)\s*\()RX");
    for (;;) {
        std::optional<re::Match> m = call.search(text, i);
        if (!m) {
            out.push_back(text.slice_from(i));
            str r;
            for (const str& p : out) r += p;
            return r;
        }
        int64_t end = close_(text, m->end());
        std::optional<str> nm = symbols::ordinals::name(m->group(1), py::int_parse(m->group(2)));
        bool have_nm = nm && !nm->empty();
        std::optional<py::list<str>> params;
        if (have_nm && protos.params.contains(*nm)) params = protos.params.at(*nm);
        if (params && protos.void_.contains(*nm) &&
            !(rx(R"RX((^|[;{}])\s*$)RX").search(text.slice(0, m->start())) && rx(R"RX(\s*;)RX").match(text.slice_from(end)))) {
            params.reset();   // (a value taken from a function of no value: not the API)
        }
        if (end < 0 || !params) {
            out.push_back(text.slice(i, m->end()));
            i = m->end();
            continue;
        }
        py::list<str> args;
        for (const str& a : arguments_(text.slice(m->end(), end - 1))) args.append(sdk_calls(a, protos).strip());
        std::vector<bool> wide;
        int64_t wides = 0;
        for (const str& p : *params) {
            bool w = static_cast<bool>(WIDE_().search(p));
            wide.push_back(w);
            wides += w;
        }
        py::list<str> said;
        if (args.size() == params->size()) {
            said = args;
        } else if (args.size() == params->size() + wides) {
            int64_t k = 0;
            for (size_t pi = 0; pi < wide.size(); ++pi) {
                const str& p = (*params)[static_cast<int64_t>(pi)];
                if (wide[pi]) {
                    str hi = args[k], lo = args[k + 1];
                    if (POINTERS_().search(p)) said.append(str("MAKELP(") + hi + str(", ") + lo + str(")"));
                    else said.append(str("MAKELONG(") + lo + str(", ") + hi + str(")"));
                    k += 2;
                } else {
                    said.append(args[k]);
                    k += 1;
                }
            }
        } else {
            out.push_back(text.slice(i, m->end()));
            i = m->end();
            continue;
        }
        out.push_back(text.slice(i, m->start()) + *nm + str("(") + py::join(str(", "), said) + str(")"));
        i = end;
    }
}

// ---------------------------------------------------------------------------------------------------------------------
namespace {

const re::Pattern& DECL_() { return rx(R"RX(^\s+(?:[\w]+\s+)+\**\s*(\w+)\s*;\s*$)RX"); }
const re::Pattern& ASSIGN_() { return rx(R"RX(^(\s*)(\w+) = (.+);\s*$)RX"); }
const re::Pattern& LOOP_() { return rx(R"RX(^\s*(?:\}\s*)?(?:while|for|do)\b)RX"); }

int64_t indent_of_(const str& line) { return line.size() - line.lstrip().size(); }

// lines a < b are in one block: nothing between them closes it, opens one, or is a label
bool same_block_(const py::list<str>& lines, int64_t a, int64_t b) {
    int64_t indent = indent_of_(lines[a]);
    for (int64_t k = a + 1; k < b; ++k) {
        const str& ln = lines[k];
        str s = ln.strip();
        if (s.empty()) continue;
        if (indent_of_(ln) != indent || s.endswith(str("{")) || s.endswith(str(":")) || s.startswith(str("}"))) return false;
    }
    return true;
}

int64_t decl_line_(const py::list<str>& lines, const str& name) {
    for (int64_t i = 0; i < lines.size(); ++i) {
        std::optional<re::Match> m = DECL_().match(lines[i]);
        if (m && m->group(1) == name) return i;
    }
    return -1;
}

}  // namespace

str fold_temporaries(const str& text) {
    // `text` with each temporary the decompiler made -- a local assigned once and used once, in the statement right after,
    // in the same block -- put back into the expression it came from: `n = lstrlen(s); p = LocalAlloc(0x40, n + 1);` ->
    // `p = LocalAlloc(0x40, lstrlen(s) + 1);`, `iVar1 = f(x); return iVar1;` -> `return f(x);`. The source wrote one
    // expression; the compiler evaluates it in the source's order (a pascal call's arguments first to last), which the
    // temporaries change. Not into a loop's condition (that is evaluated again each time round).
    py::list<str> lines = py::split(text, str("\n"));
    py::set<str> locals_;
    int64_t body = 0;
    for (int64_t k = 0; k < lines.size(); ++k) {
        const str& ln = lines[k];
        if (ln.strip() == "{" && !body) {
            body = k + 1;
            continue;
        }
        if (body && k >= body) {
            std::optional<re::Match> m = DECL_().match(ln);
            if (m && !ln.contains(str("(")) && !ln.contains(str("="))) locals_.add(m->group(1));
            else if (rx(R"RX(^\s+(?:\w+\s+)+\**\s*\w+ = [^;]*;\s*$)RX").match(ln)) continue;   // (one declared with its value: not a temporary)
            else if (!ln.strip().empty()) break;
        }
    }
    bool changed = true;
    while (changed) {
        changed = false;
        for (int64_t k = 0; k < lines.size(); ++k) {
            std::optional<re::Match> m = ASSIGN_().match(lines[k]);
            if (!m || !locals_.contains(m->group(2))) continue;
            str name = m->group(2), expr = m->group(3);
            re::Pattern word(str("(?<![\\w.>])") + re::escape(name) + str("\\b(?!\\s*\\[)"), 0);
            int64_t decl = decl_line_(lines, name);
            std::vector<int64_t> uses;
            for (int64_t j = 0; j < lines.size(); ++j)
                if (j != k && j != decl && word.search(lines[j])) uses.push_back(j);
            std::optional<int64_t> nxt;
            for (int64_t i = k + 1; i < lines.size(); ++i)
                if (!lines[i].strip().empty()) {
                    nxt = i;
                    break;
                }
            if (uses.empty() || !nxt) continue;
            std::optional<re::Match> again = ASSIGN_().match(lines[*nxt]);
            bool constant = static_cast<bool>(rx(R"RX(-?(?:0x[0-9a-fA-F]+|\d+))RX").fullmatch(expr.strip()));
            int64_t j;
            if (again && again->group(2) == name && word.findall(again->group(3)).size() == 1) {
                j = *nxt;   // `v = f(); v = g(v + 1);`: the first value dies there
            } else if (uses.size() == 1 && uses[0] == *nxt) {
                j = *nxt;
            } else if (constant && uses.size() == 1 && same_block_(lines, k, uses[0])) {
                j = uses[0];   // a constant moves past what comes between
            } else {
                continue;
            }
            std::optional<re::Match> target = ASSIGN_().match(lines[j]);
            str head, tail;
            if (target && target->group(2) == name && j == *nxt && again) {
                head = lines[j].slice(0, target->start(3));
                tail = target->group(3) + str(";");
            } else {
                head = str("");
                tail = lines[j];
            }
            if (word.findall(tail).size() != 1 || LOOP_().match(lines[j]) || rx(R"RX(\s*\})RX").match(lines[j])) continue;
            int64_t indent = indent_of_(lines[j]);
            if (indent != m->group(1).size()) continue;
            bool simple = static_cast<bool>(
                rx(R"RX([\w.]+|0x[0-9a-fA-F]+|-?\d+|[\w.]+\([^()]*\))RX").fullmatch(expr.strip()));
            str put = simple ? expr : str("(") + expr + str(")");
            lines[j] = head + word.sub([&](const re::Match&) { return put; }, tail, 1);
            lines.vec().erase(lines.vec().begin() + k);
            changed = true;
            break;
        }
    }
    str out = py::join(str("\n"), lines);
    py::list<str> sorted_locals = py::sorted(py::list<str>::from(locals_));
    for (const str& name : sorted_locals) {   // a declaration left with nothing to declare
        if (re::Pattern(str("(?<![\\w.>])") + re::escape(name) + str("\\b"), 0).findall(out).size() == 1)
            out = re::Pattern(str("\\n\\s+(?:[\\w]+\\s+)+\\**\\s*") + re::escape(name) + str("\\s*;[ \\t]*(?=\\n)"), 0).sub(str(""), out, 1);
    }
    return out;
}

str join_far_parameters(const str& text) {
    // `text` with each pair of a function's own parameters that is used only as a pair -- the lower one (the offset) then the
    // next (its segment), side by side in an argument list -- made the one far pointer the source had: `f(a1,a2) int2 a1;
    // int2 a2; ... lstrlen(a1,a2)` -> `f(a1) void __far *a1; ... lstrlen(a1)`. The words go on the stack as they did; what
    // changes is that the compiler knows them for one value (it pushes them straight from the frame, not through registers,
    // and a prototype takes them as the pointer they are).
    std::optional<re::Match> m = rx(R"RX((?s)(.*?\b\w+\s*\()([\w\s,]*)(\)\s*\n)(.*?)(\{.*))RX").match(text);
    if (!m) return text;
    str head = m->group(1), plist = m->group(2), close = m->group(3), decls = m->group(4), body = m->group(5);
    py::list<str> params;
    for (const str& p : py::split(plist, str(",")))
        if (!p.strip().empty()) params.append(p.strip());
    std::vector<std::pair<str, str>> joined;
    for (int64_t i = 0; i + 1 < params.size(); ++i) {
        str lo = params[i], hi = params[i + 1];
        bool seen = false;
        for (const auto& j : joined)
            if (lo == j.first || lo == j.second || hi == j.first || hi == j.second) seen = true;
        if (seen) continue;
        re::Pattern pair(str("\\b") + re::escape(lo) + str("\\s*,\\s*") + re::escape(hi) + str("\\b"), 0);
        re::Pattern alone(str("(?<![\\w.])(") + re::escape(lo) + str("|") + re::escape(hi) + str(")\\b"), 0);
        int64_t pairs = pair.findall(body).size();
        if (pairs && alone.findall(body).size() == 2 * pairs &&
            re::Pattern(str("^\\s*\\w+\\s+") + re::escape(hi) + str("\\s*;"), RE_M).search(decls))
            joined.emplace_back(lo, hi);
    }
    if (joined.empty()) return text;
    for (const auto& j : joined) {
        const str &lo = j.first, &hi = j.second;
        for (size_t at = 0; at < params.vec().size(); ++at)
            if (params.vec()[at] == hi) {
                params.vec().erase(params.vec().begin() + at);
                break;
            }
        body = re::Pattern(str("\\b") + re::escape(lo) + str("\\s*,\\s*") + re::escape(hi) + str("\\b"), 0)
                   .sub([&](const re::Match&) { return lo; }, body);
        decls = re::Pattern(str("^\\s*\\w+\\s+") + re::escape(hi) + str("\\s*;[ \\t]*\\n"), RE_M).sub(str(""), decls);
        decls = re::Pattern(str("^(\\s*)\\w+(\\s+)") + re::escape(lo) + str("\\s*;"), RE_M)
                    .sub([&](const re::Match& mm) { return mm.group(1) + str("void __far *") + mm.group(2) + lo + str(";"); }, decls);
    }
    return head + py::join(str(","), params) + close + decls + body;
}

namespace {

int64_t size_of_(const str& t, int64_t dflt = -1) {
    static const std::map<std::string, int64_t> sizes = {
        {"char", 1},       {"int1", 1},       {"uint1", 1},      {"xunknown1", 1}, {"bool", 1},       {"undefined", 1}, {"undefined1", 1},
        {"int2", 2},       {"uint2", 2},      {"xunknown2", 2},  {"undefined2", 2}, {"short", 2},     {"int4", 4},      {"uint4", 4},
        {"xunknown4", 4},  {"undefined4", 4}, {"long", 4}};
    auto it = sizes.find(t.utf8());
    return it == sizes.end() ? dflt : it->second;
}

struct FrameMember {
    int64_t base;
    str typ;
    int64_t count;
};

struct FrameVar {
    str name, typ;
    int64_t count;
};

const re::Pattern& FRAME_() {
    return rx(R"RX(\n[ \t]*_Packed struct \{\n((?:[ \t]*[\w ]+\s+\w+\s*(?:\[\d+\])?\s*;\n)+)[ \t]*\} codebreaker_frame;)RX");
}
const re::Pattern& MEMBER_() { return rx(R"RX(([\w ]+?)\s+(\w+?_([0-9a-f]+))\s*(?:\[(\d+)\])?\s*;)RX"); }
const re::Pattern& AT_() {
    return rx(R"RX((&)?\(\*\((\w+) \*\)\(\(uint1 \*\)codebreaker_frame\.(\w+) \+ (0x[0-9a-f]+|\d+)\)\))RX");
}
const re::Pattern& INDEX_() { return rx(R"RX(codebreaker_frame\.(\w+)\[(0x[0-9a-f]+|\d+)\])RX"); }
const re::Pattern& PTR_() { return rx(R"RX(\(codebreaker_frame\.(\w+) \+ (-?(?:0x[0-9a-f]+|\d+))\))RX"); }
const re::Pattern& BARE_() { return rx(R"RX(codebreaker_frame\.(\w+)\b)RX"); }

}  // namespace

str split_frame(const str& text) {
    // `text` with the decompiler's frame structure -- every local of a function in one packed structure of byte arrays,
    // reached as `(*(int2 *)((uint1 *)codebreaker_frame.axStack_12 + 8))` -- taken apart into the locals a source declares: a
    // variable for each place read or written (of the widest type it is read as), a buffer for each place whose address is
    // taken (to the next variable). The structure keeps the frame's layout for a build; a compiler given locals lays them
    // out itself, as it did the source's. Declared nearest the frame pointer first (search16 tries other orders).
    // A function whose frame is reached in a way not read here is left as it was.
    std::optional<re::Match> fm = FRAME_().search(text);
    if (!fm) return text;
    py::dict<str, FrameMember> members;
    for (const re::Match& mm : MEMBER_().finditer(fm->group(1))) {
        str typ = mm.group(1).strip(), name = mm.group(2), hexoff = mm.group(3);
        std::optional<str> count = mm.group_opt(4);
        members.set(name, FrameMember{-py::int_parse(hexoff, 16), typ, count ? py::int_parse(*count) : 1});
    }
    if (members.empty()) return text;
    str body = text.slice_from(fm->end());
    std::map<int64_t, int64_t> scalars;
    std::set<int64_t> starts;
    for (const re::Match& m : AT_().finditer(body)) {
        str amp = m.group(1), typ = m.group(2), mem = m.group(3), k = m.group(4);
        if (!members.contains(mem) || size_of_(typ) < 0) return text;
        int64_t at = members.at(mem).base + py::int_parse(k, 0);
        scalars[at] = std::max(scalars.count(at) ? scalars[at] : 0, size_of_(typ));
        if (!amp.empty()) starts.insert(at);
    }
    for (const re::Match& m : INDEX_().finditer(body)) {
        str mem = m.group(1), k = m.group(2);
        if (!members.contains(mem)) return text;
        const FrameMember& fmem = members.at(mem);
        int64_t at = fmem.base + py::int_parse(k, 0) * size_of_(fmem.typ, 1);
        scalars[at] = std::max(scalars.count(at) ? scalars[at] : 0, size_of_(fmem.typ, 1));
    }
    for (const re::Match& m : PTR_().finditer(body)) {   // (a member's address moved on: a place of its own)
        str mem = m.group(1), k = m.group(2);
        if (!members.contains(mem)) return text;
        starts.insert(members.at(mem).base + py::int_parse(k, 0));
    }
    str rest = PTR_().sub(str(""), INDEX_().sub(str(""), AT_().sub(str(""), body)));
    for (const str& mem : BARE_().findall(rest)) {
        if (!members.contains(mem)) return text;
        starts.insert(members.at(mem).base);
    }
    int64_t high = INT64_MIN;
    for (const auto& kv : members.items()) {
        const FrameMember& fmem = std::get<1>(kv);
        high = std::max(high, fmem.base + size_of_(fmem.typ, 1) * fmem.count);
    }
    std::set<int64_t> point_set(starts);
    for (const auto& kv : scalars) point_set.insert(kv.first);
    std::vector<int64_t> points(point_set.begin(), point_set.end());   // (sorted)
    if (points.empty()) return text;
    std::map<int64_t, FrameVar> var;
    for (size_t i = 0; i < points.size(); ++i) {
        int64_t at = points[i];
        int64_t end = i + 1 < points.size() ? points[i + 1] : high;
        int64_t span = end - at;
        str name = at < 0 ? str("local_") + hex_(-at) : str("local_p") + hex_(at);
        if (starts.count(at) && (!scalars.count(at) || span > scalars[at])) {
            var[at] = FrameVar{name, str("xunknown1"), span};   // a buffer: its address is passed on
        } else {
            int64_t size = scalars.count(at) ? scalars[at] : span;
            if (size > span) return text;   // (reads that overlap: not locals)
            str typ = size == 2 ? str("int2") : size == 4 ? str("int4") : str("xunknown1");
            var[at] = FrameVar{name, typ, (size == 1 || size == 2 || size == 4) ? 1 : span};
        }
    }

    // the variable holding `at`, and how far into it
    auto place = [&](int64_t at) -> std::pair<const FrameVar*, int64_t> {
        bool any = false;
        int64_t b = 0;
        for (int64_t p : points)
            if (p <= at && (!any || p > b)) {
                b = p;
                any = true;
            }
        if (!any) throw py::ValueError("max() arg is an empty sequence");
        return {&var.at(b), at - b};
    };

    auto at_text = [&](const re::Match& m) {
        str amp = m.group(1), typ = m.group(2), mem = m.group(3), k = m.group(4);
        auto [v, d] = place(members.at(mem).base + py::int_parse(k, 0));
        str ref;
        if (!d && v->count == 1 && v->typ == typ) ref = v->name;
        else if (!d && v->count == 1 && size_of_(v->typ) == size_of_(typ)) ref = str("(*(") + typ + str(" *)&") + v->name + str(")");
        else ref = str("(*(") + typ + str(" *)((uint1 *)&") + v->name + str(" + ") + num_(d) + str("))");
        return !amp.empty() ? str("&") + ref : ref;
    };
    auto index_text = [&](const re::Match& m) {
        str mem = m.group(1), k = m.group(2);
        const FrameMember& fmem = members.at(mem);
        auto [v, d] = place(fmem.base + py::int_parse(k, 0) * size_of_(fmem.typ, 1));
        return !d && v->count == 1 && v->typ == fmem.typ
                   ? v->name
                   : str("(*(") + fmem.typ + str(" *)((uint1 *)&") + v->name + str(" + ") + num_(d) + str("))");
    };
    auto bare_text = [&](const re::Match& m) {
        const FrameMember& fmem = members.at(m.group(1));
        auto [v, d] = place(fmem.base);
        return d ? str("((") + fmem.typ + str(" *)((uint1 *)&") + v->name + str(" + ") + num_(d) + str("))")
                 : str("((") + fmem.typ + str(" *)&") + v->name + str(")");
    };
    auto ptr_text = [&](const re::Match& m) {
        const FrameMember& fmem = members.at(m.group(1));
        auto [v, d] = place(fmem.base + py::int_parse(m.group(2), 0));
        return d ? str("((") + fmem.typ + str(" *)((uint1 *)&") + v->name + str(" + ") + num_(d) + str("))")
                 : str("((") + fmem.typ + str(" *)&") + v->name + str(")");
    };
    body = BARE_().sub(bare_text, PTR_().sub(ptr_text, INDEX_().sub(index_text, AT_().sub(at_text, body))));
    str decls;
    for (auto it = var.rbegin(); it != var.rend(); ++it) {   // (sorted by -at)
        const FrameVar& v = it->second;
        decls += str("\n  ") + v.typ + str(" ") + v.name + (v.count > 1 ? str("[") + num_(v.count) + str("]") : str("")) + str(";");
    }
    return text.slice(0, fm->start()) + decls + body;
}

std::tuple<str, str> ds_globals(const str& text) {
    // (declarations, text): every read or write through a constant near address, `*(T *)0x63b2`, made one through a variable
    // of the data segment, `(*(T *)codebreaker_d63b2)`. Microsoft C addresses a variable directly (`mov ax, [63b2]`, the
    // offset a fix-up); a constant pointer it loads into a register first -- the source had the variable. One variable per
    // address, read as each place reads it.
    py::set<str> seen;
    str out = rx(R"RX(\*\s*\(\s*((?:unsigned |signed )?\w+)\s*\*\s*\)\s*(0x[0-9a-fA-F]+|\d+)\b)RX").sub(
        [&](const re::Match& m) {
            str typ = m.group(1);
            int64_t at = py::int_parse(m.group(2), 0);
            if (at < 0x10 || typ == "void" || typ == "code") return m.group(0);
            str nm = str("codebreaker_d") + hex_(at, 4);
            seen.add(nm);
            return str("(*(") + typ + str(" *)") + nm + str(")");
        },
        text);
    str decls;
    for (const str& n : py::sorted(py::list<str>::from(seen))) decls += str("extern char ") + n + str("[];\n");
    return {decls, out};
}

std::optional<Code> function_code(const py::bytes& obj, const str& name) {
    // (bytes, mask) of the public function `name` in an OMF object file -- cut from its code segment between its public name
    // and the next -- or None. A header's own functions (static helpers, the reconstruction's stubs) come first in the
    // segment, so the first code segment is not the function.
    str want = name.lstrip(str("_@")).lower();   // (a C name _f, a fastcall one @f)
    for (const auto& kv : formats::omf::function_bytes(obj).items()) {
        const str& pub = std::get<0>(kv);
        if (pub.lstrip(str("_@")).lower() == want) {
            const py::bytes& code = std::get<0>(std::get<1>(kv));
            std::string mask(static_cast<size_t>(code.size()), '\0');
            for (int64_t at : std::get<1>(std::get<1>(kv)))
                if (at >= 0 && at < static_cast<int64_t>(mask.size())) mask[static_cast<size_t>(at)] = '\1';
            return Code{code, py::bytes(mask)};
        }
    }
    return std::nullopt;
}

// ---------------------------------------------------------------------------------------------------------------------
namespace {

struct Ran {
    bool timed_out = false;
    std::string out, err;
};

// subprocess.run(args, cwd=, env=, capture_output=True, timeout=): the environment is exactly `env`
Ran run_with_env_(const py::list<str>& args, const str& cwd, const std::vector<std::pair<str, str>>& env, double timeout) {
    SECURITY_ATTRIBUTES sa{sizeof sa, nullptr, TRUE};
    HANDLE out_r = nullptr, out_w = nullptr, err_r = nullptr, err_w = nullptr;
    if (!CreatePipe(&out_r, &out_w, &sa, 0) || !CreatePipe(&err_r, &err_w, &sa, 0)) py::raise_winerror(GetLastError());
    SetHandleInformation(out_r, HANDLE_FLAG_INHERIT, 0);
    SetHandleInformation(err_r, HANDLE_FLAG_INHERIT, 0);
    HANDLE in_r = CreateFileW(L"NUL", GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, &sa, OPEN_EXISTING, 0, nullptr);
    STARTUPINFOW si{};
    si.cb = sizeof si;
    si.dwFlags = STARTF_USESTDHANDLES;
    si.hStdInput = in_r;
    si.hStdOutput = out_w;
    si.hStdError = err_w;
    std::wstring block;
    for (const auto& kv : env) {
        block += py::subprocess::to_wide(kv.first) + L"=" + py::subprocess::to_wide(kv.second);
        block.push_back(L'\0');
    }
    block.push_back(L'\0');
    std::wstring cmd = py::subprocess::to_wide(py::subprocess::list2cmdline(args));
    std::wstring wcwd = py::subprocess::to_wide(cwd);
    PROCESS_INFORMATION pi{};
    BOOL ok = CreateProcessW(nullptr, cmd.data(), nullptr, nullptr, TRUE, CREATE_UNICODE_ENVIRONMENT, block.data(), wcwd.c_str(), &si, &pi);
    DWORD err = GetLastError();
    CloseHandle(out_w);
    CloseHandle(err_w);
    if (in_r != INVALID_HANDLE_VALUE) CloseHandle(in_r);
    if (!ok) {
        CloseHandle(out_r);
        CloseHandle(err_r);
        py::raise_winerror(err);
    }
    CloseHandle(pi.hThread);
    Ran r;
    auto drain = [](HANDLE h, std::string& dst) {
        char buf[8192];
        DWORD got = 0;
        while (ReadFile(h, buf, sizeof buf, &got, nullptr) && got > 0) dst.append(buf, got);
    };
    std::thread t_out([&] { drain(out_r, r.out); });
    std::thread t_err([&] { drain(err_r, r.err); });
    DWORD waited = WaitForSingleObject(pi.hProcess, static_cast<DWORD>(timeout * 1000.0));
    if (waited == WAIT_TIMEOUT) {
        r.timed_out = true;
        TerminateProcess(pi.hProcess, 1);   // (our own child)
        WaitForSingleObject(pi.hProcess, INFINITE);
    }
    t_out.join();
    t_err.join();
    CloseHandle(pi.hProcess);
    CloseHandle(out_r);
    CloseHandle(err_r);
    return r;
}

}  // namespace

std::optional<std::tuple<str, str>> _native(const str& compiler) {
    // (the compiler's folder, a work folder), both by short paths, when `compiler` runs on Windows itself -- else None.
    // Visual C++ 1.52c's CL, C1 and C2 (C/C++ 8.00c) are Win32 console programs (KERNEL32 only; the Phar Lap stub is for
    // DOS), and run on 64-bit Windows given two things: paths short enough for a buffer in CL (a long one overruns it:
    // CL.EXE+0xC46B reads part of the path as a pointer) and an environment small enough for its pass launcher (with a
    // modern environment it says "Out of memory" before starting C2). The short paths are directory junctions, made the
    // first time (settings: native16_tools, native16_work; native16 = off keeps the emulator). Microsoft C 6.0a is a DOS
    // program: always the emulator.
    if (compiler != "msvc152") return std::nullopt;
    str mode = setting_text_or_empty_("native16").lower();
    if (mode == "off" || mode == "0" || mode == "false" || mode == "no") return std::nullopt;
    auto [_emulator, home] = _tools(compiler);
    if (!home || home->empty() || !py::os::path::isfile(py::os::path::join(*home, str("BIN"), str("CL.EXE")))) return std::nullopt;
    str profile = py::os::getenv(str("USERPROFILE"), str(""));
    if (profile.empty()) profile = py::os::path::expanduser(str("~"));
    std::optional<str> t = py::settings::get_opt_str(str("native16_tools"));
    std::optional<str> w = py::settings::get_opt_str(str("native16_work"));
    str tools = t && !t->empty() ? *t : py::os::path::join(profile, str(".cbt"));
    str root = w && !w->empty() ? *w : py::os::path::join(profile, str(".cbw"));
    if (!py::os::path::isdir(tools)) {
        py::subprocess::RunOptions o;
        o.stdin_ = py::subprocess::Stream::Inherit;
        o.stdout_ = py::subprocess::Stream::Devnull;
        o.stderr_ = py::subprocess::Stream::Devnull;
        py::subprocess::run(py::list<str>{str("cmd"), str("/c"), str("mklink"), str("/J"), tools, py::os::path::abspath(*home)}, o);
    }
    py::os::makedirs(root, 0777, true);
    if (!py::os::path::isfile(py::os::path::join(tools, str("BIN"), str("CL.EXE")))) return std::nullopt;
    return std::make_tuple(tools, root);
}

namespace {

// `files` compiled by the compiler run on Windows itself, one CL a file, in a short work folder; the objects and messages
// copied back into `work` as an emulator session leaves them.
bool native_session_(const str& work, const str& header, const py::list<std::tuple<str, str>>& files, const str& flags, const str& tools,
                     const str& root, int64_t timeout) {
    str short_ = py::os::path::join(
        root, py::hashlib::sha1(py::os::path::abspath(work).encode("utf-8")).hexdigest().slice(0, 8));
    for (const str& folder : {work, short_}) {
        py::os::makedirs(folder, 0777, true);
        for (const str& f : listdir_(folder))
            if (ends_with_any_(f, {".obj", ".c", ".txt", ".h"})) py::os::remove(py::os::path::join(folder, f));
    }
    write_text_latin1_(py::os::path::join(short_, str("M.H")), header);
    std::vector<std::pair<str, str>> env = {
        {str("PATH"), py::os::path::join(tools, str("BIN"))},
        {str("INCLUDE"), py::os::path::join(tools, str("INCLUDE"))},
        {str("TMP"), short_},
        {str("TEMP"), short_},
        {str("SystemRoot"), py::os::getenv(str("SystemRoot"), str("C:\\Windows"))}};
    for (const auto& file : files) {
        const str& stem = std::get<0>(file);
        const str& text = std::get<1>(file);
        write_text_latin1_(py::os::path::join(short_, stem + str(".C")), str("#include \"m.h\"\n\n") + text + str("\n"));
        py::list<str> args = {py::os::path::join(tools, str("BIN"), str("CL.EXE")), str("/nologo"), str("/c"), str("/W0")};
        for (const str& f : py::split(flags)) args.append(f);
        args.append(str("/Fo") + stem + str(".OBJ"));
        args.append(stem + str(".C"));
        std::string said;
        Ran r = run_with_env_(args, short_, env, static_cast<double>(timeout));
        if (r.timed_out) said = "(timed out)";
        else said = r.out + r.err;
        {
            std::ofstream f(fs::u8path(py::os::path::join(work, stem + str(".TXT")).utf8()), std::ios::binary | std::ios::trunc);
            f.write(said.data(), static_cast<std::streamsize>(said.size()));
        }
        str made = py::os::path::join(short_, stem + str(".OBJ"));
        if (py::os::path::isfile(made)) py::shutil::copyfile(made, py::os::path::join(work, stem + str(".OBJ")));
    }
    return true;
}

}  // namespace

bool _session(const str& work, const str& header, const py::list<std::tuple<str, str>>& files, const str& flags, const str& compiler,
              int64_t timeout) {
    // One emulator session compiling `files` [(stem, C text)] -> whether it ran to its end. A compiler that runs on Windows
    // itself (_native) is run so instead -- no emulator.
    std::optional<std::tuple<str, str>> native = _native(compiler);
    if (native) return native_session_(work, header, files, flags, std::get<0>(*native), std::get<1>(*native), timeout);
    auto [_emulator, home] = _tools(compiler);
    py::os::makedirs(work, 0777, true);
    for (const str& f : listdir_(work))
        if (ends_with_any_(f, {".obj", ".c", ".txt"})) py::os::remove(py::os::path::join(work, f));
    write_text_latin1_(py::os::path::join(work, str("M.H")), header);
    py::list<str> lines;
    for (const auto& file : files) {
        const str& stem = std::get<0>(file);
        write_text_latin1_(py::os::path::join(work, stem + str(".C")), str("#include \"m.h\"\n\n") + std::get<1>(file) + str("\n"));
        lines.append(str("cl /nologo /c /W0 ") + flags + str(" /Fo") + stem + str(".OBJ ") + stem + str(".C > ") + stem + str(".TXT"));
    }
    dos_tools::Mounts mounts = {{str("c"), home ? *home : str("")}, {str("d"), work}};
    py::dict<str, str> cpu = {{str("core"), str("dynamic")}, {str("cputype"), str("386")}, {str("cycles"), str("max")}};
    py::dict<str, str> variables = {{str("PATH"), str("C:\\BIN")}, {str("INCLUDE"), str("C:\\INCLUDE")}, {str("LIB"), str("C:\\LIB")}};
    dos_tools::run(mounts, lines, py::list<str>(), static_cast<double>(timeout), py::dict<str, dos_tools::FileData>(), cpu, std::nullopt,
                   variables, work);
    return py::os::path::isfile(py::os::path::join(work, str("done.txt")));
}

// ---------------------------------------------------------------------------------------------------------------------

py::dict<Key, std::optional<Code>> compile_many(const py::dict<Key, str>& sources, const str& header, const str& flags,
                                                const std::optional<str>& work_in, int64_t timeout, const str& compiler,
                                                const std::optional<py::dict<Key, str>>& names, int64_t batch,
                                                const std::optional<py::list<py::list<Key>>>& together) {
    // Compile each of `sources` {key: C text} with `header` before it. -> {key: (bytes, mask) or None}
    // names: {key: function name} -- each key's code is that public function's (function_code), not the object's first code
    // segment.
    //
    // With names, functions share source files: `together` [[key, ...], ...] says which (the source files of the original, as
    // far as they are known), else `batch` at a time in their order. Starting the compiler's passes (CL, C1, C2) is most of the
    // cost of compiling a function in the emulator. What a function compiles to depends on its file: a far call to a function
    // defined in the same file is `push cs; call` (near, the segment known), to one elsewhere `lcall` -- so a function's result
    // is kept (cached) for its whole file, and a file with one function not yet known is compiled again whole. A file the
    // compiler refuses has the functions its errors are in compiled on their own, the rest together, so one function cannot
    // sink the rest.
    namespace cache = base::cache;
    if (!available(compiler)) throw py::RuntimeError(compiler.utf8() + " is not set up (settings.json: dosbox_x, " + compiler.utf8() + ")");
    py::dict<Key, std::optional<Code>> out;
    py::dict<Key, std::tuple<str, str>> todo;
    auto [_emulator, home_opt] = _tools(compiler);
    str home = *home_opt;
    str stamp = cache::tool(py::os::path::join(home, str("BIN"), str("CL.EXE")));
    const bool have_names = names && !names->empty();
    auto named_has = [&](const Key& k) { return have_names && names->contains(k); };
    std::vector<Key> named;
    for (const Key& k : sources.keys())
        if (named_has(k)) named.push_back(k);
    std::vector<std::vector<Key>> files_of;
    if (have_names && batch > 1) {
        if (together && !together->empty()) {
            for (const auto& g : *together) {
                std::vector<Key> in;
                for (const Key& k : g)
                    if (sources.contains(k)) in.push_back(k);
                files_of.push_back(in);
            }
        } else {
            for (size_t i = 0; i < named.size(); i += static_cast<size_t>(batch))
                files_of.emplace_back(named.begin() + i, named.begin() + std::min(named.size(), i + static_cast<size_t>(batch)));
        }
        py::set<Key> seen;
        for (const auto& g : files_of)
            for (const Key& k : g) seen.add(k);
        std::vector<std::vector<Key>> nonempty;
        for (const auto& g : files_of)
            if (!g.empty()) nonempty.push_back(g);
        files_of = nonempty;
        for (const Key& k : named)
            if (!seen.contains(k)) files_of.push_back({k});
    } else {
        for (const Key& k : named) files_of.push_back({k});
    }
    py::dict<Key, str> context;
    for (const auto& g : files_of) {
        str whole;
        if (g.size() > 1) {
            std::vector<cache::Part> parts;
            for (const Key& k : g) parts.push_back(cache::Part(sources.at(k)));
            whole = cache::key(parts);
        }
        for (const Key& k : g) context.set(k, whole);
    }
    for (const auto& kv : sources.items()) {
        const Key& key = std::get<0>(kv);
        const str& text = std::get<1>(kv);
        str fn = named_has(key) ? str("function ") + names->at(key) : str("");
        str k = cache::key({cache::Part(stamp), cache::Part(flags), cache::Part(header), cache::Part(text), cache::Part(fn),
                            cache::Part(context.get(key, str("")))});
        std::optional<py::bytes> had = cache::get(str("msc6"), k);
        if (had) {
            if (*had == py::bytes("-", 1)) {
                out.set(key, std::nullopt);
            } else {
                int64_t n = 0;
                for (int64_t b = 3; b >= 0; --b) n = (n << 8) | had->byte_(b);
                out.set(key, Code{had->slice(4, 4 + n), had->slice_from(4 + n)});
            }
        }
        todo.set(key, std::make_tuple(k, text));
    }
    for (const auto& g : files_of) {   // (a file not wholly known is compiled again whole)
        bool all = true;
        for (const Key& k : g) all = all && out.contains(k);
        if (all)
            for (const Key& k : g)
                if (todo.contains(k)) todo.pop(k);
    }
    for (const Key& key : todo.keys())
        if (!context.contains(key) && out.contains(key)) todo.pop(key);
    if (todo.empty()) return out;
    std::vector<std::vector<Key>> groups;
    for (const auto& g : files_of) {
        std::vector<Key> in;
        for (const Key& k : g)
            if (todo.contains(k)) in.push_back(k);
        if (!in.empty()) groups.push_back(in);
    }
    str work;
    if (work_in && !work_in->empty()) {
        work = py::os::path::abspath(*work_in);
    } else {
        // settings.state(compiler, "work")
        str path = py::os::path::join(py::settings::get_str(str("state")), compiler, str("work"));
        py::os::makedirs(py::os::path::dirname(path), 0777, true);
        work = py::os::path::abspath(path);
    }
    std::vector<Key> keys = todo.keys();
    py::dict<Key, std::optional<Code>> built;
    py::set<Key> unsettled;   // (unsettled: no verdict -- the session was cut off -- so not cached)
    std::vector<Key> single;
    for (const Key& k : keys)
        if (!named_has(k)) single.push_back(k);
    if (!have_names) groups.clear();
    // A refused file: the functions its errors are in go on their own, the rest together again -- round after round, for the
    // compiler stops at a file's first errors and a second bad function shows only once the first is out (one emulator
    // session a round).
    for (int round = 0; round < 6; ++round) {
        if (groups.empty()) break;
        py::list<py::list<Key>> group_lists;
        for (const auto& g : groups) group_lists.append(py::list<Key>(g));
        auto [files, starts] = _batch_files(group_lists, todo, str("B"));
        bool finished = _session(work, header, files, flags, compiler, timeout);
        std::vector<std::vector<Key>> again;
        for (size_t i = 0; i < groups.size(); ++i) {
            const std::vector<Key>& g = groups[i];
            std::optional<py::bytes> obj = read_(py::os::path::join(work, str("B") + num_(static_cast<int64_t>(i)) + str(".OBJ")));
            if (obj) {
                for (const Key& k : g) built.set(k, function_code(*obj, names->at(k)));
            } else if (!finished) {
                for (const Key& k : g) unsettled.add(k);
            } else if (g.size() == 1) {
                built.set(g[0], std::nullopt);   // (refused on its own: it does not compile)
            } else {
                py::list<Key> bad =
                    _refused(py::os::path::join(work, str("B") + num_(static_cast<int64_t>(i)) + str(".TXT")), starts[static_cast<int64_t>(i)]);
                if (bad.empty()) {
                    for (const Key& k : g) again.push_back({k});   // (errors not told apart: each on its own)
                    continue;
                }
                for (const Key& k : bad) again.push_back({k});
                std::vector<Key> rest;
                for (const Key& k : g) {
                    bool is_bad = false;
                    for (const Key& b : bad) is_bad = is_bad || b == k;
                    if (!is_bad) rest.push_back(k);
                }
                if (!rest.empty()) again.push_back(rest);
            }
        }
        groups = again;
    }
    for (const auto& g : groups)
        for (const Key& k : g) unsettled.add(k);   // (still refused after every round: no verdict)
    if (!single.empty()) {
        py::list<std::tuple<str, str>> files;
        for (size_t j = 0; j < single.size(); ++j) files.append({str("F") + num_(static_cast<int64_t>(j)), std::get<1>(todo.at(single[j]))});
        bool finished = _session(work, header, files, flags, compiler, timeout);
        for (size_t j = 0; j < single.size(); ++j) {
            const Key& k = single[j];
            std::optional<py::bytes> obj = read_(py::os::path::join(work, str("F") + num_(static_cast<int64_t>(j)) + str(".OBJ")));
            if (!obj) {
                built.set(k, std::nullopt);
                if (!finished) unsettled.add(k);
            } else if (named_has(k)) {
                built.set(k, function_code(*obj, names->at(k)));
            } else {
                Code c = object_code(*obj);
                if (std::get<0>(c).size() > 0) built.set(k, c);
                else built.set(k, std::nullopt);
            }
        }
    }
    for (const auto& kv : todo.items()) {
        const Key& key = std::get<0>(kv);
        const str& k = std::get<0>(std::get<1>(kv));
        std::optional<Code> got = built.contains(key) ? built.at(key) : std::nullopt;
        out.set(key, got);
        if (got) {
            int64_t n = std::get<0>(*got).size();
            std::string head;
            for (int b = 0; b < 4; ++b) head.push_back(static_cast<char>((n >> (8 * b)) & 0xFF));
            cache::put(str("msc6"), k, py::bytes(head) + std::get<0>(*got) + std::get<1>(*got));
        } else if (built.contains(key) && !unsettled.contains(key)) {
            cache::put(str("msc6"), k, py::bytes("-", 1));   // the compiler refused it (not: the session was cut off)
        }
    }
    return out;
}

std::tuple<py::list<std::tuple<str, str>>, py::list<py::list<std::tuple<int64_t, Key>>>> _batch_files(
    const py::list<py::list<Key>>& groups, const py::dict<Key, std::tuple<str, str>>& todo, const str& prefix) {
    // ([(stem, text)], [[(first line, key)]]): each group's functions in one file, and the line each begins on (the file
    // starts with the #include and a blank line)
    py::list<std::tuple<str, str>> files;
    py::list<py::list<std::tuple<int64_t, Key>>> starts;
    for (int64_t i = 0; i < groups.size(); ++i) {
        int64_t at = 3;
        py::list<std::tuple<int64_t, Key>> here;
        py::list<str> texts;
        for (const Key& k : groups[i]) {
            here.append({at, k});
            const str& t = std::get<1>(todo.at(k));
            at += t.count(str("\n")) + 2;   // (its lines, and the blank one between)
            texts.append(t);
        }
        files.append({prefix + num_(i), py::join(str("\n\n"), texts)});
        starts.append(here);
    }
    return {files, starts};
}

py::list<Key> _refused(const str& path, const py::list<std::tuple<int64_t, Key>>& starts) {
    // the keys of the functions a refused file's errors are in -- [] when an error is not in the file itself (the header's:
    // then none can be told from another)
    std::optional<str> text = read_text_latin1_(path);
    if (!text) return py::list<Key>();
    py::list<Key> bad;
    for (const re::Match& m : rx(R"RX(^(\S+)\((\d+)\) : (?:fatal )?error)RX", RE_M).finditer(*text)) {
        if (!rx(R"RX([BR]\d+\.C$)RX", RE_I).match(py::os::path::basename(m.group(1)))) return py::list<Key>();
        int64_t at = py::int_parse(m.group(2));
        const std::tuple<int64_t, Key>* k = nullptr;
        for (const auto& s : starts)
            if (std::get<0>(s) <= at && (!k || std::get<0>(s) > std::get<0>(*k))) k = &s;
        if (k) {
            bool have = false;
            for (const Key& b : bad) have = have || b == std::get<1>(*k);
            if (!have) bad.append(std::get<1>(*k));
        }
    }
    return bad;
}

}  // namespace codebreaker::build::msc6
