// codebreaker/verify/authored/match.cpp -- typed C++ of codebreaker.verify.authored.match (see match.hpp).

#include "py_difflib.hpp"
#include "codebreaker/verify/authored/match.hpp"

#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>

#include <algorithm>
#include <atomic>
#include <exception>
#include <filesystem>
#include <thread>

#include "codebreaker/base/cache.hpp"
#include "codebreaker/base/corpus.hpp"
#include "codebreaker/emulate/capstone_x86.hpp"
#include "codebreaker/formats/pe.hpp"
#include "codebreaker/formats/ne.hpp"
#include "codebreaker/formats/xex.hpp"
#include "codebreaker/verify/verdicts.hpp"
#include "py_json.hpp"
#include "py_settings.hpp"
#include "py_stdlib.hpp"
#include "py_subprocess.hpp"

namespace codebreaker::verify::authored::match {

namespace V = verify::verdicts;
namespace masks = compare::mask;
namespace coff = symbols::libraries::coff;

const py::list<py::str> FLAG_SETS = {py::str("/O2 /Gz"),        py::str("/Oxs /Gz"), py::str("/O1 /Gz"), py::str("/Ox /Gz"),
                                     py::str("/O2 /Gz /Oy-"), py::str("/Od /Gz"), py::str("/O2 /Gd")};
// Microsoft C 8.00c (Visual C++ 1.52c) built Setup's SUWIN, without /Gw, with switches that differ
// from one source file to the next: its segment 1 at /Od, its segment 3 at /G2 /Ox.
const py::list<py::str> MSC8_FLAG_SETS = {py::str("/AM /Gs /Od"),   py::str("/AM /G2s /Ox"), py::str("/AM /G2s /Oxs"), py::str("/AM /G2s /Os"),
                                          py::str("/AM /Gs /Ox"),   py::str("/AM /Gs /Oxs"), py::str("/AM /Gsw /Od"),   py::str("/AM /G2sw /Ox"),
                                          py::str("/AM /G3s /Ox"),  py::str("/AM /G3s /Oxs")};   // (/G3: push dword ptr) (/Oxs: SUWIN's sub_2_5CFC, a 2-byte cleanup as pop bx)
const py::list<py::str> POWERPC_FLAG_SETS = {py::str("/O2"), py::str("/Ox"), py::str("/O1"), py::str("/Os"), py::str("/O2 /Oy-"), py::str("/Od")};
const py::list<py::str> _POWERPC_FLAGS = {py::str("/nologo"), py::str("/c"),  py::str("/TC"), py::str("/GS-"),
                                          py::str("/Zl"),     py::str("/Gy"), py::str("/W0"), py::str("/FIm.h")};

// IMAGE_REL_PPC_* (winnt.h) -> which bits of the 4-byte word the linker fills in
const py::dict<int64_t, int64_t> _FIELD = {
    {0x03, _LI},   {0x06, _LI},                               // ADDR24, REL24 (b/bl)
    {0x05, _BD},   {0x07, _BD},                               // ADDR14, REL14 (bc)
    {0x04, _HALF}, {0x08, _HALF}, {0x09, _HALF}, {0x0F, _HALF}, {0x10, _HALF}, {0x11, _HALF},
    {0x13, _HALF}, {0x14, _HALF},                             // ADDR16, TOCREL, SECREL16, REFHI/LO ...
    {0x02, _WORD}, {0x0B, _WORD}};                            // ADDR32, SECREL
const py::dict<int64_t, int64_t> _NE_SIZE = {{0, 1}, {2, 2}, {3, 4}, {5, 2}, {11, 6}, {13, 4}};   // bytes a 16-bit relocation of each source type covers

// ---- helpers ----------------------------------------------------------------------------------------------------------
namespace {

// settings.state(*parts)
py::str state_(const std::vector<py::str>& parts) {
    py::str path = py::settings::get_str(py::str("state"));
    for (const py::str& p : parts) path = py::os::path::join(path, p);
    py::os::makedirs(parts.empty() ? path : py::os::path::dirname(path), 0777, true);
    return path;
}

// dict(os.environ): the process environment (on Windows os.environ has its names upper-cased)
py::dict<py::str, py::str> environment_() {
    py::dict<py::str, py::str> out;
    (void)_wgetenv(L"PATH");   // (_wenviron exists once the wide environment has been asked)
    for (wchar_t** e = _wenviron; e && *e; ++e) {
        std::wstring entry(*e);
        size_t eq = entry.find(L'=', 1);   // (a name may begin with '=': "=C:")
        if (entry.empty() || eq == std::wstring::npos) continue;
        std::string key = std::filesystem::path(entry.substr(0, eq)).u8string();
        std::string value = std::filesystem::path(entry.substr(eq + 1)).u8string();
        out.set(py::str(key).upper(), py::str(value));
    }
    return out;
}

py::subprocess::RunOptions capture_(const std::optional<py::str>& cwd) {
    py::subprocess::RunOptions o;
    o.cwd = cwd;
    o.encoding = "cp1252";
    o.errors = "replace";
    return o;
}

py::bytes read_bytes_(const py::str& path) {
    py::file f = py::open(path, py::str("rb"));
    py::bytes b = f.read();
    f.close();
    return b;
}

// open(path, "w", newline="\n", encoding="utf-8").write(text)
void write_text_nl_(const py::str& path, const py::str& text) {
    py::file f = py::open(path, py::str("w"), py::str("utf-8"), std::nullopt, py::str("\n"));
    f.write(text);
    f.close();
}

void try_remove_(const py::str& path) {
    try {
        py::os::remove(path);
    } catch (const py::OSError&) {
    }
}

int64_t u16_(const py::bytes& d, int64_t at) { return py::unpack_from1<int64_t>("<H", d, at); }
int64_t s16_(const py::bytes& d, int64_t at) { return py::unpack_from1<int64_t>("<h", d, at); }
int64_t u32le_(const py::bytes& d, int64_t at) { return py::unpack_from1<int64_t>("<I", d, at); }

// bytearray slice assignment `a[at:at + n] = ones(n)`, growing the bytearray as Python's does past its end
void set_ones_(const py::bytearray& a, int64_t at, int64_t n) { a.set_slice(at, at + n, py::bytes(std::string(static_cast<size_t>(n), '\1'))); }

// build/functions.py's powerpc_tools(): (compiler, linker, runtime library), or None when settings.json does not name them
// (build/functions.hpp declares it; it has no typed implementation yet)
std::optional<std::tuple<py::str, py::str, py::str>> powerpc_tools_() {
    py::str d = py::settings::get_str(py::str("powerpc_tools")), lib = py::settings::get_str(py::str("powerpc_lib"));
    py::str cl = py::os::path::join(d, py::str("cl.exe")), link = py::os::path::join(d, py::str("link.exe"));
    if (!d.empty() && py::os::path::isfile(cl) && py::os::path::isfile(link) && py::os::path::isfile(lib)) return std::make_tuple(cl, link, lib);
    return std::nullopt;
}

std::function<bool(const coff::Symbol&)> any_() { return [](const coff::Symbol&) { return true; }; }

// str(x) of a double as json.dump writes it (float.__repr__)
void put_float_(std::string& o, double d) { o += py::to_str(d).utf8(); }

}  // namespace

EnvGuard::EnvGuard(const py::dict<py::str, py::str>& env) {
    for (const auto& kv : env.items()) {
        std::optional<py::str> old = py::os::environ_::get(std::get<0>(kv));
        if (old && *old == std::get<1>(kv)) continue;
        saved_.push_back({std::get<0>(kv).utf8(), old ? std::optional<std::string>(old->utf8()) : std::nullopt});
        _putenv_s(std::get<0>(kv).utf8().c_str(), std::get<1>(kv).utf8().c_str());
    }
}
EnvGuard::~EnvGuard() {
    for (auto it = saved_.rbegin(); it != saved_.rend(); ++it) _putenv_s(it->first.c_str(), it->second ? it->second->c_str() : "");
}

// ---- the decoder, the span source ---------------------------------------------------------------------------------------
Decoder capstone_decoder(int64_t bits) {
    return [bits](const py::bytes& code, int64_t address) {
        py::list<Insn> out;
        const emulate::capstone_x86::Api& api = emulate::capstone_x86::api();
        csh h = emulate::capstone_x86::handle(bits, false);
        cs_insn* insn = nullptr;
        size_t n = api.disasm(h, reinterpret_cast<const uint8_t*>(code.raw().data()), code.raw().size(), static_cast<uint64_t>(address), 0, &insn);
        for (size_t i = 0; i < n; i++)
            out.append(Insn{static_cast<int64_t>(insn[i].address), static_cast<int64_t>(insn[i].size), py::str(insn[i].mnemonic), py::str(insn[i].op_str)});
        if (insn) api.free_(insn, n);
        return out;
    };
}

namespace {
std::map<int64_t, Decoder>& decoders_() {
    static std::map<int64_t, Decoder> d;
    return d;
}
FunctionSpans& spans_hook_() {
    static FunctionSpans f;
    return f;
}
}  // namespace

void set_decoder(int64_t bits, const Decoder& d) { decoders_()[bits] = d; }
const Decoder& decoder(int64_t bits) {
    auto& m = decoders_();
    auto it = m.find(bits);
    if (it == m.end()) it = m.emplace(bits, capstone_decoder(bits)).first;
    return it->second;
}
void set_function_spans(const FunctionSpans& f) { spans_hook_() = f; }

// ---- what a compiler made: the functions of its object ---------------------------------------------------------------
py::str compiler() {
    py::str d = py::settings::get_str(py::str("period_compiler"));
    py::str exe = py::os::path::join(d, py::str("CL.EXE"));
    return (!d.empty() && py::os::path::isfile(exe)) ? exe : py::str("");
}

py::dict<int64_t, py::list<std::tuple<int64_t, py::str>>> _code_symbols(const coff::Coff& obj, const std::function<bool(const coff::Symbol&)>& also) {
    py::dict<int64_t, py::list<std::tuple<int64_t, py::str>>> funcs;
    for (const std::optional<coff::Symbol>& s : obj.symbols) {
        if (s && s->sec > 0 && (obj.sections[s->sec - 1].chars & 0x20) && s->type == 0x20 && (!also || also(*s))) {
            funcs.setdefault(s->sec, py::list<std::tuple<int64_t, py::str>>()).append({s->value, s->name});
        }
    }
    for (const auto& kv : funcs.items()) {
        auto& v = std::get<1>(kv).vec();
        std::sort(v.begin(), v.end());
    }
    return funcs;
}

py::dict<py::str, ByteMasked> object_functions(const py::bytes& obj) {
    coff::Coff o(obj);
    py::dict<py::str, ByteMasked> out;
    for (const auto& sl : _code_symbols(o, any_()).items()) {
        int64_t sect = std::get<0>(sl);
        const auto& lst = std::get<1>(sl);
        const coff::Section& sec = o.sections[sect - 1];
        py::bytearray relocated = py::bytearray::zeros(sec.size);
        for (const coff::Relocation& r : o.relocations(sec)) set_ones_(relocated, std::get<0>(r), 4);
        for (int64_t k = 0; k < py::len(lst); k++) {
            int64_t value = std::get<0>(lst[k]);
            const py::str& name = std::get<1>(lst[k]);
            int64_t end = k + 1 < py::len(lst) ? std::get<0>(lst[k + 1]) : sec.size;
            out[name] = ByteMasked(obj.slice(sec.ptr + value, sec.ptr + end), py::bytes(relocated.slice(value, end).view()));
        }
    }
    return out;
}

py::dict<py::str, WordMasked> _powerpc_objects(const py::bytes& obj) {
    int64_t machine = py::unpack_from1<int64_t>("<H", obj, 0);
    if (machine != coff::POWERPC) throw py::ValueError(py::percent(py::str("not a PowerPC object (machine %#x)"), machine).utf8());
    coff::Coff o(obj);
    py::dict<py::str, WordMasked> out;
    auto not_label = [](const coff::Symbol& s) { return s.cls != 6; };
    for (const auto& sl : _code_symbols(o, not_label).items()) {
        int64_t sect = std::get<0>(sl);
        const auto& lst = std::get<1>(sl);
        const coff::Section& sec = o.sections[sect - 1];
        py::list<int64_t> ignore;
        for (int64_t i = 0; i < (sec.size + 3) / 4; i++) ignore.append(0);
        for (const coff::Relocation& r : o.relocations(sec)) {
            int64_t at = std::get<0>(r), kind = std::get<2>(r);
            if (at / 4 < py::len(ignore)) ignore[at / 4] = ignore[at / 4] | _FIELD.get(kind, 0);
        }
        for (int64_t k = 0; k < py::len(lst); k++) {
            int64_t value = std::get<0>(lst[k]);
            const py::str& name = std::get<1>(lst[k]);
            int64_t end = k + 1 < py::len(lst) ? std::get<0>(lst[k + 1]) : sec.size;
            out[name] = WordMasked(obj.slice(sec.ptr + value, sec.ptr + end), ignore.slice(value / 4, (end + 3) / 4));
        }
    }
    return out;
}

ByteMasked _trim(const py::bytes& code, const py::bytes& mask) {
    int64_t n = py::len(code);
    auto pad = [](int64_t b) { return b == 0xCC || b == 0x90 || b == 0x00; };
    while (n && !mask[n - 1] && pad(code[n - 1]) && n > 1) {
        int64_t p = code[n - 2];
        if (!(p == 0xC3 || p == 0xCC || p == 0x90 || p == 0x00)) break;
        n -= 1;
    }
    return ByteMasked(code.slice(0, n), mask.slice(0, n));
}

WordMasked _trim_words(const py::bytes& code, const py::list<int64_t>& mask) {
    int64_t n = py::len(code) / 4;   // (len(code) // 4: floor)
    while (n > 1 && code.slice(4 * n - 4, 4 * n) == py::bytes::zeros(4)) n -= 1;
    return WordMasked(code.slice(0, 4 * n), mask.slice(0, n));
}

// ---- the original's functions ------------------------------------------------------------------------------------------
namespace {
// the places of a PE's base relocations of type HIGHLOW (3), as virtual addresses
py::set<int64_t> highlow_places_(const formats::pe::PEFile& pe, bool as_target_values) {
    py::set<int64_t> out;
    auto [va, size] = pe.directory(5);
    std::optional<int64_t> off = va ? pe.file_offset(va) : std::nullopt;
    int64_t end = off.value_or(0) + size;
    while (off && *off + 8 <= std::min<int64_t>(end, py::len(pe.data))) {
        auto [page, block] = py::unpack_from<int64_t, int64_t>("<II", pe.data, *off);
        if (block < 8) break;
        for (int64_t k = 0; k < (block - 8) / 2; k++) {
            int64_t e = u16_(pe.data, *off + 8 + 2 * k);
            if ((e >> 12) == 3) {
                if (!as_target_values) {
                    out.add(static_cast<int64_t>(pe.image_base) + page + (e & 0xFFF));
                } else {
                    std::optional<int64_t> o = pe.file_offset(page + (e & 0xFFF));
                    if (o && *o + 4 <= py::len(pe.data)) out.add(u32le_(pe.data, *o));
                }
            }
        }
        *off += block;
    }
    return out;
}
}  // namespace

py::dict<int64_t, ByteMasked> original_functions(const py::str& path, const py::list<int64_t>& addresses) {
    formats::pe::PEFile pe = formats::pe::open_pe(path);
    if (!spans_hook_()) throw py::RuntimeError("match.original_functions: no function_spans hook (pe_functions has no typed port): call set_function_spans");
    py::dict<int64_t, int64_t> spans = spans_hook_()(path);
    py::set<int64_t> relocated = highlow_places_(pe, false);
    const Decoder& md = decoder(32);
    py::dict<int64_t, ByteMasked> out;
    for (int64_t start : addresses) {
        if (!spans.contains(start)) continue;
        int64_t stop = spans.at(start);
        std::optional<int64_t> o = pe.file_offset(start - static_cast<int64_t>(pe.image_base));
        if (!o) continue;
        py::bytes code = pe.data.slice(*o, *o + (stop - start));
        py::bytearray mask = py::bytearray::zeros(py::len(code));
        for (int64_t a = start; a < stop; a++)
            if (relocated.contains(a)) set_ones_(mask, a - start, 4);
        for (const Insn& i : md(code, start)) {
            int64_t first = code[i.address - start];
            if ((first == 0xE8 || first == 0xE9) && i.size == 5) set_ones_(mask, i.address - start + 1, 4);
        }
        out[start] = _trim(code, py::bytes(mask.slice(0, py::len(code)).view()));
    }
    return out;
}

py::dict<int64_t, ByteMasked> _originals_ne(const py::str& module_path, const Where16& where, const Spans16& spans) {
    formats::ne::NEFile nef(module_path);
    py::dict<int64_t, formats::ne::Segment> segs;
    for (const formats::ne::Segment& s : nef.segments()) segs[s.idx] = s;
    py::dict<int64_t, std::tuple<py::bytes, py::bytearray>> by_seg;
    for (const auto& kv : segs.items()) {
        int64_t idx = std::get<0>(kv);
        const formats::ne::Segment& s = std::get<1>(kv);
        py::bytes code = nef.data.slice(s.file_off, s.file_off + s.length);
        py::bytearray relocated = py::bytearray::zeros(py::len(code));
        for (const formats::ne::Relocation& r : nef.relocations(s)) {
            int64_t n = _NE_SIZE.get(r.src, 2);
            int64_t at = r.offset, seen = 0;
            while (0 <= at && at < py::len(code) && seen < 0x4000) {   // the places are chained through the code
                int64_t nxt = at + 2 <= py::len(code) ? u16_(code, at) : 0xFFFF;
                set_ones_(relocated, at, std::min<int64_t>(n, py::len(code) - at));
                if (r.additive || nxt == 0xFFFF) break;
                at = nxt;
                seen += 1;
            }
        }
        by_seg[idx] = std::make_tuple(code, relocated);
    }
    const Decoder& md = decoder(16);
    py::dict<int64_t, ByteMasked> out;
    for (const auto& kv : where.items()) {
        int64_t va = std::get<0>(kv);
        int64_t seg = std::get<0>(std::get<1>(kv));
        if (!spans.contains(va) || !by_seg.contains(seg)) continue;
        auto [startend, _n] = spans.at(va);
        auto [start, end] = startend;
        const auto& cr = by_seg.at(seg);
        py::bytes piece = std::get<0>(cr).slice(start, end);
        py::bytearray m(std::get<1>(cr).slice(start, end));
        for (const Insn& i : md(piece, start)) {
            int64_t first = piece[i.address - start];
            if ((first == 0xE8 || first == 0xE9) && i.size == 3) {   // a near call or jump: its distance
                int64_t target = (i.address + 3 + s16_(piece, i.address - start + 1)) & 0xFFFF;
                if (!(start <= target && target < end)) set_ones_(m, i.address - start + 1, 2);
            }
        }
        out[va] = _trim(piece, py::bytes(m.view()));
    }
    return out;
}

py::dict<int64_t, WordMasked> _originals_powerpc(const formats::powerpc_image::Image& img, const py::list<int64_t>& addresses) {
    py::set<int64_t> all;
    for (int64_t f : img.functions) all.add(f);
    for (const auto& kv : img.imports.items()) all.add(std::get<0>(kv));
    for (const auto& kv : img.inline_.items()) all.add(std::get<0>(kv));
    std::vector<int64_t> starts;
    for (int64_t x : all) starts.push_back(x);
    std::sort(starts.begin(), starts.end());
    py::dict<int64_t, int64_t> nxt;
    for (size_t i = 0; i < starts.size(); i++) nxt[starts[i]] = i + 1 < starts.size() ? starts[i + 1] : std::get<1>(img.code);
    py::dict<int64_t, WordMasked> out;
    for (int64_t va : addresses) {
        int64_t size = img.extents.contains(va) ? std::get<0>(img.extents.at(va)) : 0;
        if (!size) size = nxt.get(va, va) - va;
        py::bytes code = img.bytes_at(va, size);
        for (int64_t i = 0; i < py::len(code) - 3; i += 4) {
            if (code.slice(i, i + 4) == py::bytes::zeros(4)) {
                code = code.slice(0, i);
                break;
            }
        }
        if (py::len(code) < 4) continue;
        py::list<int64_t> ignored;
        for (int64_t i = 0; i < py::len(code) / 4; i++) {
            int64_t w = py::unpack_from1<int64_t>(">I", code, 4 * i);
            int64_t ignore = 0;
            if ((w >> 26) == 18) {   // b / bl
                int64_t d = w & 0x03FFFFFC;
                d -= (d & 0x02000000) ? 0x04000000 : 0;
                if (!(w & 2) && !(va <= va + 4 * i + d && va + 4 * i + d < va + py::len(code))) ignore = _LI;
            }
            ignored.append(ignore);
        }
        out[va] = _trim_words(code, ignored);
    }
    return out;
}

// ---- the processors ----------------------------------------------------------------------------------------------------
const py::dict<py::str, Arch>& ARCH() {
    static const py::dict<py::str, Arch> table = [] {
        py::dict<py::str, Arch> t;
        auto trim_x86 = [](const py::bytes& code, const Mask& mask) -> Masked { return _trim(code, std::get<py::bytes>(mask)); };
        auto trim_words = [](const py::bytes& code, const Mask& mask) -> Masked { return _trim_words(code, std::get<py::list<int64_t>>(mask)); };
        Arch x86{1, py::str("little"), FLAG_SETS, trim_x86,
                 [](const py::bytes& obj) {
                     py::dict<py::str, Masked> out;
                     for (const auto& kv : object_functions(obj).items()) out.set(std::get<0>(kv), Masked(std::get<0>(std::get<1>(kv)), std::get<1>(std::get<1>(kv))));
                     return out;
                 },
                 Originals32(original_functions)};
        Arch x86_16{1, py::str("little"), MSC8_FLAG_SETS, trim_x86, nullptr, Originals16(_originals_ne)};
        Arch ppc{4, py::str("big"), POWERPC_FLAG_SETS, trim_words,
                 [](const py::bytes& obj) {
                     py::dict<py::str, Masked> out;
                     for (const auto& kv : _powerpc_objects(obj).items()) out.set(std::get<0>(kv), Masked(std::get<0>(std::get<1>(kv)), std::get<1>(std::get<1>(kv))));
                     return out;
                 },
                 OriginalsPPC([](const formats::powerpc_image::Image& img, const py::list<int64_t>& a) { return _originals_powerpc(img, a); })};
        t.set(py::str("x86"), x86);
        t.set(py::str("x86-16"), x86_16);
        t.set(py::str("powerpc"), ppc);
        return t;
    }();
    return table;
}

// ---- comparing -----------------------------------------------------------------------------------------------------------
double sequence_ratio(const py::bytes& a, const py::bytes& b) {
    // difflib.SequenceMatcher(None, a, b, autojunk=False).ratio()
    const std::vector<unsigned char> av(a.raw().begin(), a.raw().end()), bv(b.raw().begin(), b.raw().end());
    return py::difflib::SequenceMatcher<unsigned char>(av, bv, nullptr, false).ratio();
}

std::tuple<py::str, double> compare(const ByteMasked& built, const ByteMasked& original) {
    auto [a, ma] = _trim(std::get<0>(built), std::get<1>(built));
    const py::bytes& b = std::get<0>(original);
    const py::bytes& mb = std::get<1>(original);
    if (py::len(a) != py::len(b)) return {V::DIFFERENT, sequence_ratio(a, b)};
    int64_t free_ = 0;
    for (int64_t i = 0; i < py::len(a); i++)
        if (!ma[i] && !mb[i]) free_ += 1;
    int64_t same = free_ - py::len(masks::differing(a, b, py::list<masks::MaybeMask>{Mask(ma), Mask(mb)}));
    return {same == free_ ? V::IDENTICAL : V::SAME_SIZE, free_ ? static_cast<double>(same) / static_cast<double>(free_) : 1.0};
}

std::tuple<py::str, int64_t, int64_t, int64_t> distance(const Masked& built, const Masked& original, const py::str& arch) {
    const Arch& a = ARCH().at(arch);
    auto [x, mx] = a.trim(std::get<0>(built), std::get<1>(built));
    return masks::distance(x, std::get<0>(original), mx, std::get<1>(original), a.unit, a.byteorder);
}

py::set<int64_t> relocation_targets(const py::str& path) {
    formats::pe::PEFile pe = formats::pe::open_pe(path);
    return highlow_places_(pe, true);
}

// ---- period_source ------------------------------------------------------------------------------------------------------
const py::re::Pattern _BOUND(py::str("(?m)^#define (\\w+) \\(\\*\\(([^()]*?) ?\\*\\)0x([0-9a-fA-F]+)\\)[ \\t]*$"), 0);
const py::re::Pattern _CONSTANT(py::str("(-\\s*)?\\b0x([0-9a-fA-F]{5,8})\\b"), 0);
const py::re::Pattern _RESULT_OF_NOTHING(py::str("(?m)^[ \\t]*return in_EAX;[ \\t]*\\n"), 0);

std::tuple<py::str, py::dict<py::str, py::str>> period_source(const py::str& header, const py::dict<py::str, py::str>& texts,
                                                              const py::set<py::str>& imports, const py::set<int64_t>& targets) {
    py::list<py::str> lines;
    py::dict<py::str, py::str> bound;
    for (const py::re::Match& m : _BOUND.finditer(header)) {
        if (targets.contains(py::int_parse(m.group(3), 16))) bound[m.group(1)] = m.group(2);
    }
    py::dict<int64_t, py::str> names;
    auto at = [&](int64_t address) {
        names[address] = py::percent(py::str("codebreaker_at_%08x"), address);
        return names.at(address);
    };
    auto constant = [&](const py::re::Match& m, const py::str& text) -> py::str {
        int64_t value = py::int_parse(m.group(2), 16);
        std::optional<py::str> sign = m.group_opt(1);
        if (sign && !sign->empty()) value = (-value) & 0xFFFFFFFF;
        if (!targets.contains(value)) return m.group(0);
        py::str before = text.slice(0, m.start()).rstrip().slice(-1, py::none);
        bool joined = sign && !sign->empty() && (before.isalnum() || before.empty() || before == py::str("_") || before == py::str(")") || before == py::str("]"));
        return py::str(joined ? "+ " : "") + py::str("(int4)&") + at(value);
    };
    py::dict<py::str, py::str> out;
    py::set<py::str> nothing;
    py::list<py::str> bodies_parts;
    for (const auto& kv : texts.items()) bodies_parts.append(std::get<2>(std::get<1>(kv).partition(py::str("{"))));
    py::str bodies = py::join(py::str("\n"), bodies_parts);

    auto used = [&](const py::str& name) {
        // Is the function's result used anywhere (a call that is not a statement of its own)?
        py::re::Pattern p(py::str("\\b") + py::re::escape(name) + py::str("\\s*\\("), 0);
        for (const py::re::Match& m : p.finditer(bodies)) {
            py::str before = bodies.slice(0, m.start()).rstrip();
            py::str last = before.slice(-1, py::none);
            bool in_set = last.empty() || last == py::str(";") || last == py::str("{") || last == py::str("}") || last == py::str(")");   // (last in ";{})")
            if (!in_set && !before.endswith(py::str("else"))) return true;
        }
        return false;
    };
    py::re::Pattern returns_rx(py::str("\\breturn\\b[^;]*;"), 0);
    py::re::Pattern in_eax_decl(py::str("(?m)^[ \\t]*\\w+ in_EAX;[ \\t]*\\n"), 0);
    py::re::Pattern first_word(py::str("^\\s*\\w+\\b"), 0);
    for (const auto& kv : texts.items()) {
        const py::str& name = std::get<0>(kv);
        py::str text = std::get<1>(kv);
        {
            py::str cur = text;
            text = _CONSTANT.sub([&](const py::re::Match& m) { return constant(m, cur); }, cur);
        }
        py::list<py::str> returns = returns_rx.findall(text);
        bool all_eax = !returns.empty();
        for (const py::str& r : returns)
            if (r.replace(py::str(" "), py::str("")) != py::str("returnin_EAX;")) all_eax = false;
        if (all_eax && !used(name)) {
            auto [head, brace, body0] = text.partition(py::str("{"));
            py::str body = _RESULT_OF_NOTHING.sub(py::str(""), in_eax_decl.sub(py::str(""), body0));
            if (!body.contains(py::str("in_EAX"))) {   // (given a value somewhere: it is a result)
                text = first_word.sub(py::str("void"), head, 1) + brace + body;
                nothing.add(name);
            }
        }
        out[name] = text;
    }
    py::re::Pattern decl(py::str("^(\\w+)((?: __\\w+)?) (\\w+)\\((.*)\\);[ \\t]*$"), 0);
    for (py::str line : py::split(header, py::str("\n"))) {
        std::optional<py::re::Match> m = _BOUND.match(line);
        if (m && bound.contains(m->group(1))) {
            line = py::percent(py::str("extern %s %s;"), bound.at(m->group(1)), m->group(1));
        } else {
            std::optional<py::re::Match> f = decl.match(line);
            if (f && imports.contains(f->group(3)) && f->group(4).empty()) {
                line = py::str("__declspec(dllimport) ") + line;
            } else if (f && nothing.contains(f->group(3))) {
                line = py::percent(py::str("void%s %s(%s);"), f->group(2), f->group(3), f->group(4));
            }
        }
        lines.append(line);
    }
    std::vector<int64_t> addrs;
    for (const auto& kv : names.items()) addrs.push_back(std::get<0>(kv));
    std::sort(addrs.begin(), addrs.end());
    for (int64_t a : addrs) lines.append(py::percent(py::str("extern char %s;"), names.at(a)));
    return {py::join(py::str("\n"), lines), out};
}

const py::re::Pattern _DECLARES(
    py::str("^(?:__declspec\\(dllimport\\) )?(?:extern )?[\\w ]*?[ *](\\w+)(?:\\(.*\\)|\\[\\d*\\])?;[ \\t]*$|^#define (\\w+) \\(\\*"), 0);

py::str needed_header(const py::str& header, const py::str& text) {
    py::set<py::str> words;
    for (const py::str& w : py::re::findall(py::str("[A-Za-z_]\\w*"), text)) words.add(w);
    py::list<py::str> keep;
    bool inside = false, wanted = false;
    for (const py::str& line : py::split(header, py::str("\n"))) {
        if (inside || line.startswith(py::str("typedef struct"))) {   // a structure, whole or not at all
            if (line.startswith(py::str("typedef struct"))) {
                inside = true;
                wanted = words.contains(py::split(line)[2]);
            }
            if (wanted) keep.append(line);
            inside = !line.startswith(py::str("}"));
            continue;
        }
        std::optional<py::re::Match> m = _DECLARES.match(line);
        if (m) {
            std::optional<py::str> g1 = m->group_opt(1);
            py::str word = (g1 && !g1->empty()) ? *g1 : m->group_opt(2).value_or(py::str(""));
            if (!words.contains(word) && !line.startswith(py::str("typedef"))) continue;
        }
        keep.append(line);
    }
    return py::join(py::str("\n"), keep);
}

// ---- compiling ------------------------------------------------------------------------------------------------------------
namespace {

// What the compiler made of a text, kept (base/cache.py): a search tries the same text many times.
template <class T> std::optional<T> cached_(const py::str& key, const std::function<std::optional<T>()>& build) {
    using base::cache::pickle::Codec;
    std::optional<py::bytes> had = base::cache::get(py::str("period"), key);
    if (had) {
        std::optional<base::cache::pickle::Node> node = base::cache::pickle::loads(*had);
        std::optional<T> value;
        if (node && Codec<std::optional<T>>::load(*node, value)) return value;
    }
    std::optional<T> built = build();
    std::string data("\x80\x03", 2);
    Codec<std::optional<T>>::dump(data, built);
    data += '.';
    base::cache::put(py::str("period"), key, py::bytes(data));
    return built;
}

bool is_name_(const py::str& k, const py::str& name) {
    // k.lstrip("_@").split("@")[0] == name
    return py::split(k.lstrip(py::str("_@")), py::str("@"))[0] == name;
}

template <class M> std::optional<M> pick_function_(const py::dict<py::str, M>& funcs, const py::str& name) {
    for (const auto& kv : funcs.items())
        if (is_name_(std::get<0>(kv), name)) return std::get<1>(kv);
    if (funcs.size() == 1) return std::get<1>(funcs.items()[0]);
    return std::nullopt;
}

}  // namespace

std::optional<ByteMasked> compile_function(const py::str& cl, const py::dict<py::str, py::str>& env, const py::str& header, const py::str& source,
                                           const py::str& name, const py::str& work, const py::str& flags) {
    py::str key = base::cache::key({base::cache::Part(base::cache::tool(cl)), base::cache::Part(flags), base::cache::Part(header),
                                    base::cache::Part(source), base::cache::Part(name)});
    return cached_<ByteMasked>(key, [&] { return _compile_function(cl, env, header, source, name, work, flags); });
}

std::optional<ByteMasked> _compile_function(const py::str& cl, const py::dict<py::str, py::str>& env, const py::str& header, const py::str& source,
                                            const py::str& name, const py::str& work, const py::str& flags) {
    py::os::makedirs(work, 0777, true);
    write_text_nl_(py::os::path::join(work, py::str("m.h")),
                   header.replace(py::str("unsigned long long"), py::str("unsigned __int64")).replace(py::str("long long"), py::str("__int64")));
    py::str stem("one");
    for (const char* ext : {".obj", ".c"}) try_remove_(py::os::path::join(work, stem + py::str(ext)));
    write_text_nl_(py::os::path::join(work, stem + py::str(".c")), py::str("#include \"m.h\"\n\n") + source + py::str("\n"));
    py::list<py::str> args{cl, py::str("/nologo"), py::str("/c"), py::str("/W0")};
    args.extend(py::split(flags));
    args.append(stem + py::str(".c"));
    EnvGuard guard(env);
    py::subprocess::CompletedProcess r = py::subprocess::run(args, capture_(work));
    if (r.returncode || !py::os::path::isfile(py::os::path::join(work, stem + py::str(".obj")))) return std::nullopt;
    py::dict<py::str, ByteMasked> funcs = object_functions(read_bytes_(py::os::path::join(work, stem + py::str(".obj"))));
    return pick_function_(funcs, name);
}

py::dict<py::str, py::str> compiler_env(const py::str& cl) {
    py::dict<py::str, py::str> env = environment_();
    py::str dir = py::os::path::dirname(cl);
    env.set(py::str("PATH"), dir + py::os::pathsep + env.get(py::str("PATH"), py::str("")));
    env.set(py::str("INCLUDE"), py::os::path::join(py::os::path::dirname(dir), py::str("INCLUDE")));
    return env;
}

namespace {
// ThreadPoolExecutor(max_workers=jobs).map(fn, range(n)): the results in order; the first exception in order goes out
template <class R, class F> std::vector<R> pool_map_(int64_t jobs, int64_t n, F fn) {
    std::vector<std::optional<R>> results(static_cast<size_t>(n));
    std::vector<std::exception_ptr> errors(static_cast<size_t>(n));
    std::atomic<int64_t> next{0};
    auto worker = [&] {
        for (;;) {
            int64_t i = next.fetch_add(1);
            if (i >= n) return;
            try {
                results[static_cast<size_t>(i)] = fn(i);
            } catch (...) {
                errors[static_cast<size_t>(i)] = std::current_exception();
            }
        }
    };
    std::vector<std::thread> threads;
    for (int64_t t = 0; t < std::min<int64_t>(std::max<int64_t>(jobs, 1), n); t++) threads.emplace_back(worker);
    for (auto& t : threads) t.join();
    std::vector<R> out;
    for (int64_t i = 0; i < n; i++) {
        if (errors[static_cast<size_t>(i)]) std::rethrow_exception(errors[static_cast<size_t>(i)]);
        out.push_back(*results[static_cast<size_t>(i)]);
    }
    return out;
}
}  // namespace

py::dict<py::str, Result32> run(const py::str& module_path, const py::str& flags, const py::str& work, const Prepare& win32_prepare, int64_t jobs) {
    py::str cl = compiler();
    if (cl.empty()) throw py::SystemExit("no period compiler (settings.json: period_compiler -- the folder holding CL.EXE)");
    Prepared m = win32_prepare(module_path, py::os::path::join(work, py::str("source")));
    py::str out_dir = py::re::sub(py::str("\\W+"), py::str("_"), flags).strip(py::str("_"));
    if (out_dir.empty()) out_dir = py::str("default");
    out_dir = py::os::path::join(work, out_dir);
    py::os::makedirs(out_dir, 0777, true);
    {
        // (the 32-bit C 8.00 of 1993 -- C13232 -- has no 64-bit integer: a function using one does not match anyway)
        py::str wide = py::os::path::isfile(py::os::path::join(py::os::path::dirname(cl), py::str("C13232.EXE"))) ? py::str("double") : py::str("__int64");
        write_text_nl_(py::os::path::join(out_dir, py::str("m.h")),
                       m.header.replace(py::str("unsigned long long"), py::str("unsigned ") + wide).replace(py::str("long long"), wide).replace(py::str("unsigned double"), py::str("double")));
    }
    py::dict<py::str, py::str> env = compiler_env(cl);
    std::vector<py::str> names;
    for (const py::str& n : m.order) names.push_back(n);
    std::vector<std::optional<py::dict<py::str, ByteMasked>>> objects;
    {
        EnvGuard guard(env);   // (the compiler's environment, set for the length of the pool)
        auto one = [&](int64_t i) -> std::optional<py::dict<py::str, ByteMasked>> {
            py::str stem = py::percent(py::str("f%04d"), i);
            write_text_nl_(py::os::path::join(out_dir, stem + py::str(".c")), py::str("#include \"m.h\"\n\n") + m.texts.at(names[static_cast<size_t>(i)]) + py::str("\n"));
            py::list<py::str> args{cl, py::str("/nologo"), py::str("/c"), py::str("/W0")};
            args.extend(py::split(flags));
            args.append(stem + py::str(".c"));
            py::subprocess::CompletedProcess r = py::subprocess::run(args, capture_(out_dir));
            if (r.returncode) return std::nullopt;
            return object_functions(read_bytes_(py::os::path::join(out_dir, stem + py::str(".obj"))));
        };
        objects = pool_map_<std::optional<py::dict<py::str, ByteMasked>>>(jobs, static_cast<int64_t>(names.size()), one);
    }
    py::dict<py::str, int64_t> address;
    for (const auto& kv : m.by_va.items()) address[std::get<1>(kv)] = std::get<0>(kv);
    py::list<int64_t> addrs;
    {
        std::vector<int64_t> v;
        for (const auto& kv : address.items()) v.push_back(std::get<1>(kv));
        std::sort(v.begin(), v.end());
        addrs = py::list<int64_t>(v);
    }
    py::dict<int64_t, ByteMasked> originals = original_functions(module_path, addrs);
    py::dict<py::str, Result32> results;
    for (size_t i = 0; i < names.size(); i++) {
        const py::str& name = names[i];
        const auto& funcs = objects[i];
        std::optional<ByteMasked> orig;
        if (originals.contains(address.at(name))) orig = originals.at(address.at(name));
        if (!funcs || !orig) {
            results[name] = Result32(!funcs ? V::NO_COMPILE : V::NO_EXTENT, 0.0, 0, orig ? py::len(std::get<0>(*orig)) : 0);
            continue;
        }
        std::optional<ByteMasked> mine = pick_function_(*funcs, name);
        if (!mine) {
            results[name] = Result32(V::NO_COMPILE, 0.0, 0, py::len(std::get<0>(*orig)));
            continue;
        }
        auto [verdict, share] = compare(*mine, *orig);
        results[name] = Result32(verdict, share, py::len(std::get<0>(_trim(std::get<0>(*mine), std::get<1>(*mine)))), py::len(std::get<0>(*orig)));
    }
    return results;
}

std::optional<WordMasked> compile_powerpc(const py::str& cl, const py::str& header, const py::str& text, const py::str& name, const py::str& work,
                                          const py::str& flags) {
    py::str key = base::cache::key({base::cache::Part(base::cache::tool(cl)), base::cache::Part(py::str("powerpc")), base::cache::Part(POWERPC_REV),
                                    base::cache::Part(flags), base::cache::Part(header), base::cache::Part(text), base::cache::Part(name)});
    return cached_<WordMasked>(key, [&] { return _compile_powerpc(cl, header, text, name, work, flags); });
}

std::optional<WordMasked> _compile_powerpc(const py::str& cl, const py::str& header, const py::str& text, const py::str& name, const py::str& work,
                                           const py::str& flags) {
    py::os::makedirs(work, 0777, true);
    write_text_nl_(py::os::path::join(work, py::str("m.h")), header);
    for (const char* ext : {".obj", ".c"}) try_remove_(py::os::path::join(work, py::str("one") + py::str(ext)));
    write_text_nl_(py::os::path::join(work, py::str("one.c")), text + py::str("\n"));
    py::list<py::str> args{cl};
    args.extend(_POWERPC_FLAGS);
    args.extend(py::split(flags));
    args.append(py::str("/Foone.obj"));
    args.append(py::str("one.c"));
    py::subprocess::CompletedProcess r = py::subprocess::run(args, capture_(work));
    if (r.returncode || !py::os::path::isfile(py::os::path::join(work, py::str("one.obj")))) return std::nullopt;
    py::dict<py::str, WordMasked> funcs = _powerpc_objects(read_bytes_(py::os::path::join(work, py::str("one.obj"))));
    if (funcs.contains(name)) return funcs.at(name);
    if (funcs.size() == 1) return std::get<1>(funcs.items()[0]);
    return std::nullopt;
}

py::dict<py::str, ResultPPC> run_powerpc(const py::str& module_path, const py::str& flags, const py::str& work, const Prepare& powerpc_prepare, int64_t jobs) {
    std::optional<std::tuple<py::str, py::str, py::str>> tools = powerpc_tools_();
    if (!tools) throw py::SystemExit("no PowerPC compiler (settings.json: powerpc_tools, powerpc_lib)");
    py::str cl = std::get<0>(*tools);
    Prepared m = powerpc_prepare(module_path, py::os::path::join(work, py::str("source")));
    py::dict<py::str, int64_t> address;
    for (const auto& kv : m.by_va.items()) address[std::get<1>(kv)] = std::get<0>(kv);
    py::list<int64_t> addrs;
    {
        std::vector<int64_t> v;
        for (const auto& kv : address.items()) v.push_back(std::get<1>(kv));
        std::sort(v.begin(), v.end());
        addrs = py::list<int64_t>(v);
    }
    py::dict<int64_t, WordMasked> originals = _originals_powerpc(*m.image, addrs);
    std::vector<py::str> names;
    for (const py::str& n : m.order) names.push_back(n);
    std::vector<std::vector<size_t>> by_slot(static_cast<size_t>(std::max<int64_t>(jobs, 1)));
    for (size_t i = 0; i < names.size(); i++) by_slot[i % static_cast<size_t>(std::max<int64_t>(jobs, 1))].push_back(i);
    std::vector<std::optional<WordMasked>> built(names.size());
    std::vector<std::exception_ptr> errors(by_slot.size());
    {
        std::vector<std::thread> threads;
        for (size_t s = 0; s < by_slot.size(); s++) {
            if (by_slot[s].empty()) continue;
            threads.emplace_back([&, s] {   // one working folder per worker
                try {
                    for (size_t i : by_slot[s])
                        built[i] = compile_powerpc(cl, m.header, m.texts.at(names[i]), names[i], py::os::path::join(work, py::str("w") + py::to_str(int64_t(i % by_slot.size()))), flags);
                } catch (...) {
                    errors[s] = std::current_exception();
                }
            });
        }
        for (auto& t : threads) t.join();
        for (const auto& e : errors)
            if (e) std::rethrow_exception(e);
    }
    py::dict<py::str, ResultPPC> results;
    for (size_t i = 0; i < names.size(); i++) {
        const py::str& name = names[i];
        std::optional<WordMasked> orig;
        if (address.contains(name) && originals.contains(address.at(name))) orig = originals.at(address.at(name));
        if (!built[i] || !orig) {
            results[name] = ResultPPC(!built[i] ? V::NO_COMPILE : V::NO_EXTENT, 0, 0, orig ? py::len(std::get<0>(*orig)) / 4 : 0);
            continue;
        }
        auto [verdict, diff, words, _first] = distance(Masked(std::get<0>(*built[i]), std::get<1>(*built[i])), Masked(std::get<0>(*orig), std::get<1>(*orig)), py::str("powerpc"));
        results[name] = ResultPPC(verdict, diff, words, py::len(std::get<0>(*orig)) / 4);
    }
    return results;
}

const py::list<py::str>& _ORDER() { return V::MATCHES(); }   // (best first)

// ---- the command ----------------------------------------------------------------------------------------------------------
namespace {
std::string json_key_(const py::str& s) {
    std::string o;
    py::json::put(o, s);
    return o;
}
}  // namespace

const py::str DOC(R"DOC(match.py -- does a period compiler turn the reconstruction back into the original's bytes?

    python -m codebreaker match <module> [--flags "/O2 /Gz"] [--try] [--json FILE]

This is NOT the measure of a reconstruction: that is whether it behaves as the original
(`measure`). It is a second, optional signal. Where a function's reconstructed C, compiled with
a compiler of the original's time, comes out as the very bytes the original has, the C is as
close to what was written as a binary can show; where it does not, nothing follows -- the same
source gives different bytes under another compiler version or another switch.

Each reconstructed function is compiled on its own with a compiler of the original's time, its
bytes are read from the object file with the places the linker fills in masked out, and compared
with the original function's bytes, masked at the same places (compare/mask.py):

    identical     every byte outside the masked places is the same
    same size     as long as the original, some bytes differ (registers, instruction choice)
    different     another length

`--try` compiles under several sets of switches and reports, per function, the best; which set
matches most functions says how the module was built.

What differs from one processor to the next is in ARCH, one entry each:

    "x86"       a 32-bit Windows module: the compiler `period_compiler` in settings.json names (a
                32-bit Microsoft C compiler that runs natively: the folder holding CL.EXE)
    "x86-16"    a 16-bit module: Microsoft C 6.0a or 8.00c (build/msc6.py); match16.py drives it
    "powerpc"   an Xbox 360 image (or .xex): the compiler `powerpc_tools` names

-- how its code is compared, the switches tried, the padding a compiler leaves after a function,
what the compiler's object says the linker fills in, and the original's functions with the places
masked that depend on where things are: relocated words, and the distance of a call or jump.

On PowerPC what may differ is masked at the level of bits, not bytes. Every instruction is one
word, and the object says, as a relocation, which instructions hold something the linker fills
in -- the 24-bit distance of a `bl`, the 14 bits of a conditional branch to another function, the
16-bit halves of an address built by `lis`/`addi` -- so only that field of the word is ignored and
its opcode and registers are still compared. The image has no such records; the object's mask is
laid over the original's function at the same places (where the two agree that far, they agree on
which words are relocated), and the original's own calls out of the function are masked as well.
)DOC");

int64_t _main_powerpc(const py::list<py::str>& argv, const Prepare& powerpc_prepare) {
    py::list<py::str> args;
    for (const py::str& a : argv)
        if (!a.startswith(py::str("--"))) args.append(a);
    py::str flags = argv.contains(py::str("--flags")) ? argv[argv.index(py::str("--flags")) + 1] : POWERPC_FLAG_SETS[0];
    std::optional<py::str> out_json;
    if (argv.contains(py::str("--json"))) out_json = argv[argv.index(py::str("--json")) + 1];
    {
        py::list<py::str> kept;
        for (const py::str& a : args)
            if (!(a == flags || (out_json && a == *out_json))) kept.append(a);
        args = kept;
    }
    py::str path = args[0].lower().endswith(py::str(".xex")) ? formats::xex::basefile(args[0]) : args[0];
    py::str work = state_({py::str("match"), py::os::path::basename(path).lower()});
    py::list<py::str> sets = argv.contains(py::str("--try")) ? POWERPC_FLAG_SETS : py::list<py::str>{flags};
    struct Best {
        std::tuple<int64_t, int64_t> rank;
        ResultPPC v;
        py::str fl;
    };
    py::dict<py::str, Best> best;
    for (const py::str& fl : sets) {
        py::dict<py::str, ResultPPC> r = run_powerpc(path, fl, work, powerpc_prepare);
        if (py::len(sets) > 1) {
            int64_t n = 0;
            for (const auto& kv : r.items())
                if (std::get<0>(std::get<1>(kv)) == V::IDENTICAL) n++;
            py::print(py::percent(py::str("  %-12s %d identical"), fl, n));
            std::fflush(stdout);
        }
        for (const auto& kv : r.items()) {
            const py::str& name = std::get<0>(kv);
            const ResultPPC& v = std::get<1>(kv);
            std::tuple<int64_t, int64_t> rank(_ORDER().index(std::get<0>(v)), std::get<1>(v));
            if (!best.contains(name) || rank < best.at(name).rank) best[name] = Best{rank, v, fl};
        }
    }
    py::dict<py::str, int64_t> tally;
    for (const auto& kv : best.items()) {
        const py::str& verdict = std::get<0>(std::get<1>(kv).v);
        tally[verdict] = tally.get(verdict, 0) + 1;
    }
    py::print(py::percent(py::str("%s: %d functions"), py::os::path::basename(args[0]), best.size()));
    for (const py::str& verdict : _ORDER())
        if (tally.get(verdict, 0)) py::print(py::percent(py::str("  %-20s %d"), V::MATCH_LABEL().at(verdict), tally.at(verdict)));
    if (out_json) {
        std::vector<std::tuple<py::str, Best>> items;
        for (const auto& kv : best.items()) items.push_back(kv);
        std::sort(items.begin(), items.end(), [](const auto& a, const auto& b) { return std::get<0>(a) < std::get<0>(b); });
        std::string o = items.empty() ? "{}" : "{\n";
        for (size_t i = 0; i < items.size(); i++) {
            const Best& b = std::get<1>(items[i]);
            o += " " + json_key_(std::get<0>(items[i])) + ": {\n";
            o += "  \"verdict\": " + json_key_(std::get<0>(b.v)) + ",\n";
            o += "  \"words differing\": " + std::to_string(std::get<1>(b.v)) + ",\n";
            o += "  \"words\": " + std::to_string(std::get<2>(b.v)) + ",\n";
            o += "  \"original words\": " + std::to_string(std::get<3>(b.v)) + ",\n";
            o += "  \"flags\": " + json_key_(b.fl) + "\n }";
            o += i + 1 < items.size() ? ",\n" : "\n}";
        }
        py::file f = py::open(*out_json, py::str("w"), py::str("utf-8"));
        f.write(py::str(o));
        f.close();
    }
    return 0;
}

int64_t main(const py::list<py::str>& argv, const Prepare& win32_prepare, const Prepare& powerpc_prepare,
             const std::function<int64_t(const py::list<py::str>&)>& match16_main) {
    py::list<py::str> args;
    for (const py::str& a : argv)
        if (!a.startswith(py::str("--"))) args.append(a);
    py::str flags = argv.contains(py::str("--flags")) ? argv[argv.index(py::str("--flags")) + 1] : FLAG_SETS[0];
    {
        py::list<py::str> kept;
        for (const py::str& a : args)
            if (!(a == flags || a.endswith(py::str(".json")))) kept.append(a);
        args = kept;
    }
    if (args.empty()) {
        py::print(DOC);
        return 2;
    }
    std::optional<py::str> path = py::os::path::isfile(args[0]) ? std::optional<py::str>(args[0]) : base::corpus::retail(args[0]);
    bool path_ok = path && !path->empty();
    if (args[0].lower().endswith(py::str(".xex")) ||
        (path_ok && base::corpus::lane(*path) == std::optional<py::str>(py::str("pe")) && formats::pe::open_pe(*path).machine == 0x01F2))   // PowerPC: the Xbox 360 compiler
        return _main_powerpc(argv, powerpc_prepare);
    if (path_ok && base::corpus::lane(*path) == std::optional<py::str>(py::str("ne"))) return match16_main(argv);   // a 16-bit module: Microsoft C 6
    if (!path_ok || base::corpus::lane(*path) != std::optional<py::str>(py::str("pe")))
        throw py::SystemExit(py::percent(py::str("%s: a 32-bit or 16-bit Windows module is needed"), args[0]));
    py::str work = state_({py::str("match"), py::os::path::basename(*path).lower()});
    py::list<py::str> sets = argv.contains(py::str("--try")) ? FLAG_SETS : py::list<py::str>{flags};
    struct Best {
        std::tuple<int64_t, double> rank;
        Result32 v;
        py::str fl;
    };
    py::dict<py::str, Best> best;
    py::dict<py::str, int64_t> per_set;
    for (const py::str& fl : sets) {
        py::dict<py::str, Result32> r = run(*path, fl, work, win32_prepare);
        int64_t n = 0;
        for (const auto& kv : r.items())
            if (std::get<0>(std::get<1>(kv)) == V::IDENTICAL) n++;
        per_set[fl] = n;
        for (const auto& kv : r.items()) {
            const py::str& name = std::get<0>(kv);
            const Result32& v = std::get<1>(kv);
            int64_t rank = _ORDER().index(std::get<0>(v));
            std::tuple<int64_t, double> key(rank, -std::get<1>(v));
            if (!best.contains(name) || key < best.at(name).rank) best[name] = Best{key, v, fl};
        }
        if (py::len(sets) > 1) {
            py::print(py::percent(py::str("  %-16s %d identical"), fl, n));
            std::fflush(stdout);
        }
    }
    py::dict<py::str, int64_t> counts;
    for (const auto& kv : best.items()) {
        const py::str& verdict = std::get<0>(std::get<1>(kv).v);
        counts[verdict] = counts.get(verdict, 0) + 1;
    }
    {
        std::vector<py::str> parts;
        for (const py::str& k : _ORDER())
            if (counts.get(k, 0)) parts.push_back(py::percent(py::str("%s %d"), V::MATCH_LABEL().at(k), counts.at(k)));
        py::print(py::percent(py::str("%s: %d functions compiled with the period compiler: %s"), py::os::path::basename(*path), best.size(), py::join(py::str(", "), parts)));
    }
    {
        struct Close {
            double share;
            py::str name;
            Result32 v;
        };
        std::vector<Close> close;
        for (const auto& kv : best.items()) {
            const Result32& v = std::get<1>(kv).v;
            if (std::get<0>(v) != V::IDENTICAL) close.push_back({std::get<1>(v), std::get<0>(kv), v});
        }
        // sorted(..., reverse=True) of (share, name, v) tuples
        std::sort(close.begin(), close.end(), [](const Close& a, const Close& b) {
            return std::tie(b.share, b.name, std::get<0>(b.v), std::get<1>(b.v), std::get<2>(b.v), std::get<3>(b.v)) <
                   std::tie(a.share, a.name, std::get<0>(a.v), std::get<1>(a.v), std::get<2>(a.v), std::get<3>(a.v));
        });
        for (size_t i = 0; i < std::min<size_t>(8, close.size()); i++) {
            const Close& c = close[i];
            py::print(py::percent(py::str("  %-28s %-9s %3d%% of bytes equal  (%d bytes, original %d)"), c.name, V::MATCH_LABEL().at(std::get<0>(c.v)),
                                  static_cast<int64_t>(100 * c.share), std::get<2>(c.v), std::get<3>(c.v)));
        }
    }
    if (argv.contains(py::str("--json"))) {
        std::vector<std::tuple<py::str, Best>> items;
        for (const auto& kv : best.items()) items.push_back(kv);
        std::sort(items.begin(), items.end(), [](const auto& a, const auto& b) { return std::get<0>(a) < std::get<0>(b); });   // (sort_keys=True)
        std::string o = items.empty() ? "{}" : "{\n";
        for (size_t i = 0; i < items.size(); i++) {
            const Best& b = std::get<1>(items[i]);
            o += " " + json_key_(std::get<0>(items[i])) + ": {\n";
            o += "  \"flags\": " + json_key_(b.fl) + ",\n";
            o += "  \"original\": " + std::to_string(std::get<3>(b.v)) + ",\n";
            o += "  \"share\": ";
            put_float_(o, std::get<1>(b.v));
            o += ",\n";
            o += "  \"size\": " + std::to_string(std::get<2>(b.v)) + ",\n";
            o += "  \"verdict\": " + json_key_(std::get<0>(b.v)) + "\n }";
            o += i + 1 < items.size() ? ",\n" : "\n}";
        }
        py::file f = py::open(argv[argv.index(py::str("--json")) + 1], py::str("w"), py::str("utf-8"));
        f.write(py::str(o));
        f.close();
    }
    return 0;
}

}  // namespace codebreaker::verify::authored::match
