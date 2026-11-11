// codebreaker/symbols/runtime_library.cpp -- typed C++ port of codebreaker.symbols.runtime_library (see runtime_library.hpp).

#include "codebreaker/symbols/runtime_library.hpp"

#include <algorithm>
#include <set>

#include "codebreaker/base/corpus.hpp"
#include "codebreaker/build/flat_thunk.hpp"
#include "codebreaker/compare/mask.hpp"
#include "codebreaker/decompile/describe.hpp"
#include "codebreaker/formats/ne.hpp"
#include "codebreaker/formats/pe.hpp"
#include "codebreaker/formats/pe_functions.hpp"
#include "codebreaker/knowledge/package.hpp"
#include "codebreaker/knowledge/package.hpp"
#include "codebreaker/knowledge/reads.hpp"
#include "codebreaker/knowledge/sources.hpp"
#include "codebreaker/symbols/api_arguments.hpp"
#include "codebreaker/symbols/libraries/coff.hpp"
#include "codebreaker/symbols/libraries/omf.hpp"
#include "codebreaker/symbols/ordinals.hpp"
#include "codebreaker/symbols/x86.hpp"
#include "py_json.hpp"
#include "py_stdlib.hpp"

namespace codebreaker::symbols::runtime_library {

namespace {

constexpr int64_t _LONGEST = 1024;      // bytes of a function that are compared
constexpr int64_t _FEWEST = 10;         // bytes the linker does not touch that a signature must have
constexpr int64_t _LEAD = 6;            // signatures are looked up by this many leading bytes (fewer when they are masked)
std::optional<Index> _index;

py::str sha16(const py::bytes& b) { return py::hashlib::sha1(b).hexdigest().slice(0, 16); }

std::vector<unsigned char> uchars(const py::bytes& b) { return std::vector<unsigned char>(b.raw().begin(), b.raw().end()); }

}  // namespace

// modules that are a runtime library themselves: stem -> what their exports are
const py::dict<py::str, py::str> ITSELF = [] {
    py::dict<py::str, py::str> d;
    const char* lib = " (this module is the runtime library)";
    for (const char* k : {"CRTDLL", "MSVCRT", "MSVCRT10", "MSVCRT20", "MSVCRT40"})
        d.set(py::str(k), py::str("C runtime") + py::str(lib));
    d.set(py::str("MSVCIRT"), py::str("C++ stream runtime") + py::str(lib));
    const char* cls = " (this module is the class library)";
    d.set(py::str("MFC30"), py::str("MFC 3.0") + py::str(cls));
    d.set(py::str("MFCO30"), py::str("MFC 3.0 OLE") + py::str(cls));
    d.set(py::str("MFCD30"), py::str("MFC 3.0 database") + py::str(cls));
    d.set(py::str("MFCN30"), py::str("MFC 3.0 sockets") + py::str(cls));
    d.set(py::str("MFC40"), py::str("MFC 4.0") + py::str(cls));
    d.set(py::str("MFCO40"), py::str("MFC 4.0 OLE") + py::str(cls));
    d.set(py::str("MFC250"), py::str("MFC 2.5") + py::str(cls));
    return d;
}();

const py::str THUNK_COMPILER("Microsoft thunk compiler: a 32-to-16-bit flat thunk");

// the instructions every function's frame is made of
const py::re::Pattern _FRAME(
    "(push|pop) (e?bp|e?si|e?di|e?bx|ds|es)$|mov (e?bp, e?sp|e?sp, e?bp|ds, ax|ax, ds|es, ax)$|(inc|dec) bp$|nop|leave|"
    "ret|retf|mov (e?ax|e?dx), (0x)?[0-9a-f]+$|xor (e?ax, e?ax|e?dx, e?dx)$|(sub|add) (e?sp|bp), |lea sp, |int3",
    0);

// ---- signatures ----------------------------------------------------------------------------------

std::optional<std::tuple<int64_t, py::str, py::str>> signature(const py::bytes& code, const py::bytes& mask) {
    // (size, mask text, digest) of a function's bytes with what its byte mask leaves out left out
    // (compare/mask.py), or None when too little is left to tell it from anything else.
    int64_t size = std::min<int64_t>(code.size(), _LONGEST);
    py::list<int64_t> masked;
    for (int64_t o = 0; o < std::min(size, mask.size()); o++)
        if (mask.raw()[static_cast<size_t>(o)] != 0) masked.append(o);
    if (size - masked.size() < _FEWEST) return std::nullopt;
    py::bytes body = compare::mask::blank(code.slice(0, size), masked);
    std::set<unsigned char> distinct(body.raw().begin(), body.raw().end());
    if (distinct.size() < 4) return std::nullopt;                // padding, a table of zeros
    py::list<py::str> runs;
    int64_t i = 0;
    while (i < masked.size()) {
        int64_t j = i;
        while (j + 1 < masked.size() && masked[j + 1] == masked[j] + 1) j += 1;
        runs.append(py::percent(py::str("%x.%x"), masked[i], j - i + 1));
        i = j + 1;
    }
    return std::make_tuple(size, py::join(py::str(","), runs), sha16(body));
}

namespace {

py::list<int64_t> _offsets(const py::str& mask_text) {
    py::list<int64_t> out;
    if (mask_text.empty()) return out;
    for (const py::str& run : py::split(mask_text, py::str(","))) {
        auto [start, dot, n] = run.partition(py::str("."));
        int64_t s = py::int_parse(start, 16), len = py::int_parse(n, 16);
        for (int64_t k = s; k < s + len; k++) out.append(k);
    }
    return out;
}

}  // namespace

Index& _signatures() {
    // {"32"|"16": {leading bytes: [(size, masked offsets, digest, name, library)]}}
    if (!_index) {
        Index index;
        for (const char* bits : {"32", "16"}) {                    // (runtime_signatures.json, as the knowledge base has it)
            knowledge::reads::record_kind(py::str(std::string(bits) == "16" ? "win16" : "win32"), py::str("signature"));
            py::dict<py::bytes, py::list<Entry>> by_lead;
            for (const auto& kv : knowledge::_table(py::str(std::string(bits) == "16" ? "win16" : "win32"), py::str("signature")).items()) {
                if (!std::holds_alternative<knowledge::sources::Json>(std::get<1>(kv))) continue;
                const knowledge::sources::Json& row = std::get<knowledge::sources::Json>(std::get<1>(kv));            // lead, size, mask, digest, name, library
                Entry e;
                e.size = row.list.at(1).i;
                e.masked = _offsets(row.list.at(2).s);
                e.digest = row.list.at(3).s;
                e.name = row.list.at(4).s;
                e.library = row.list.at(5).s;
                by_lead.setdefault(py::bytes::fromhex(row.list.at(0).s), py::list<Entry>()).append(e);
            }
            index.set(py::str(bits), by_lead);
        }
        _index = index;
    }
    return *_index;
}

std::optional<std::tuple<py::str, py::str>> match(int64_t bits, const py::bytes& code) {
    // (name, library) of the library function `code` starts with, or None.
    knowledge::reads::record_kind(py::str(bits == 16 ? "win16" : "win32"), py::str("signature"));
    py::dict<py::bytes, py::list<Entry>> table = _signatures().at(py::to_str(bits));
    for (int64_t n = _LEAD; n > 0; n--) {
        std::optional<py::list<Entry>> hits = table.get(code.slice(0, n));
        if (!hits) continue;
        for (const Entry& e : *hits) {
            if (e.size <= code.size()) {
                if (sha16(compare::mask::blank(code.slice(0, e.size), e.masked)) == e.digest) return std::make_tuple(e.name, e.library);
            }
        }
    }
    return std::nullopt;
}

// ---- identifying -----------------------------------------------------------------------------------

py::str _c_name(const py::str& symbol) {
    // A linker symbol as the C name: `_memcpy` -> memcpy, `__allmul` -> _allmul, `_f@8` -> f.
    if (symbol.startswith(py::str("?"))) return symbol;
    py::str name = symbol.startswith(py::str("_")) ? symbol.slice(1, symbol.size()) : symbol;
    if (name.contains(py::str("@"))) {
        auto [head, sep, tail] = name.rpartition(py::str("@"));
        if (tail.isdigit()) return std::get<0>(name.partition(py::str("@")));
    }
    return name;
}

py::list<int64_t> _flat_thunk_stubs(const py::str& module_path, const formats::pe::PEFile& pe,
                                    const py::list<std::tuple<uint64_t, py::bytes, py::str>>& sections) {
    // The functions of a flat-thunk module (build/flat_thunk.classify) that the thunk compiler generated: each
    // reaches the 16-bit side through a pointer kept in the code section itself (QT_Thunk's, filled in by
    // ThunkConnect32 when the DLL is loaded), which no compiled C does. They are the thunk compiler's output,
    // rebuilt from the .thk script (`flat-thunk`), not C -- and cannot run here: mnmcpi32.dll's 297 such
    // functions all called into nothing.
    (void)pe;
    try {
        if (build::flat_thunk::classify(module_path).subtype != py::str("flat-thunk")) return py::list<int64_t>();
    } catch (const std::exception&) {                       // (not readable as one: not one)
        return py::list<int64_t>();
    }
    auto in_code = [&](int64_t a) {
        for (const auto& s : sections) {
            int64_t b = static_cast<int64_t>(std::get<0>(s));
            if (b <= a && a < b + std::get<1>(s).size()) return true;
        }
        return false;
    };
    // Whether the code at `va`, followed through `hops` more jumps, calls through such a pointer.
    std::function<bool(int64_t, int64_t)> reaches = [&](int64_t va, int64_t hops) -> bool {
        for (const auto& s : sections) {
            int64_t base = static_cast<int64_t>(std::get<0>(s));
            const py::bytes& raw = std::get<1>(s);
            if (base <= va && va < base + raw.size()) {
                for (const x86::Insn& ins : x86::disasm(32, false, raw.slice(va - base, va - base + 512).raw(), va)) {
                    py::str op(ins.op_str);
                    if (ins.mnemonic == "call" && op.startswith(py::str("dword ptr [0x"))) {
                        if (in_code(py::int_parse(op.slice(static_cast<int64_t>(std::string("dword ptr [").size()), op.size() - 1), 16))) return true;
                    }
                    if (ins.mnemonic == "jmp" && op.startswith(py::str("0x"))) {
                        // (a short stub sets up and jumps to the common thunk code: `mov cl, 3 / jmp`)
                        return hops > 0 && reaches(py::int_parse(op, 16), hops - 1);
                    }
                    if (ins.mnemonic == "ret" || ins.mnemonic == "retn" || ins.mnemonic == "int3") return false;
                }
            }
        }
        return false;
    };
    py::list<int64_t> out;
    std::vector<int64_t> starts;
    for (int64_t k : formats::pe_functions::carve(module_path)->funcs.keys()) starts.push_back(k);
    std::sort(starts.begin(), starts.end());
    for (int64_t va : starts)
        if (reaches(va, 1)) out.append(va);
    return out;
}

py::dict<int64_t, std::tuple<py::str, py::str>> identify(const py::str& module_path) {
    // {address: (name, library)} for the functions of a module that belong to a runtime library.
    std::optional<py::str> lane = base::corpus::lane(module_path);
    py::str stem = std::get<0>(py::os::path::splitext(py::os::path::basename(module_path))).upper();
    py::dict<int64_t, std::tuple<py::str, py::str>> out;
    if (lane && *lane == py::str("pe")) {
        formats::pe::PEFile pe = formats::pe::open_pe(module_path);
        if (pe.machine != 0x014C) return out;
        auto sections = pe.code_sections();
        std::vector<int64_t> starts;
        for (int64_t k : formats::pe_functions::carve(module_path)->funcs.keys()) starts.push_back(k);
        std::sort(starts.begin(), starts.end());
        for (int64_t va : starts) {
            for (const auto& s : sections) {
                int64_t base = static_cast<int64_t>(std::get<0>(s));
                const py::bytes& raw = std::get<1>(s);
                if (base <= va && va < base + raw.size()) {
                    auto hit = match(32, raw.slice(va - base, va - base + _LONGEST));
                    if (hit) out.set(va, *hit);
                    break;
                }
            }
        }
        for (int64_t va : _flat_thunk_stubs(module_path, pe, sections)) out.setdefault(va, std::make_tuple(py::str("thunk stub"), THUNK_COMPILER));
        if (ITSELF.contains(stem)) {
            py::dict<int64_t, py::str> unnamed = ordinals::table(stem);
            for (const auto& e : pe.exports()) {
                int64_t o = std::get<0>(e), rva = std::get<2>(e);
                std::optional<py::str> nm = std::get<1>(e);
                if (!nm || nm->empty()) nm = unnamed.get(o);
                int64_t at = static_cast<int64_t>(pe.image_base) + rva;
                bool inside = false;
                for (const auto& s : sections) {
                    int64_t b = static_cast<int64_t>(std::get<0>(s));
                    if (b <= at && at < b + std::get<1>(s).size()) inside = true;
                }
                if (nm && !nm->empty() && inside) out.setdefault(at, std::make_tuple(*nm, ITSELF.at(stem)));
            }
        }
    } else if (lane && *lane == py::str("ne")) {
        formats::ne::NEFile nef(module_path);
        py::dict<int64_t, formats::ne::Segment> segs;
        for (const auto& s : nef.segments()) segs.set(s.idx, s);
        std::vector<decompile::describe::SegOff> funcs;
        for (const auto& k : decompile::describe::functions(nef).keys()) funcs.push_back(k);
        std::sort(funcs.begin(), funcs.end());
        for (const auto& so : funcs) {
            int64_t seg = std::get<0>(so), off = std::get<1>(so);
            formats::ne::Segment s = segs.at(seg);
            py::bytes code = nef.data.slice(s.file_off + off, s.file_off + std::min(s.length, off + _LONGEST));
            auto hit = match(16, code);
            if (hit) out.set(decompile::describe::address(seg, off), *hit);
        }
        stem = nef.module_name().upper();
        if (ITSELF.contains(stem)) {
            py::dict<int64_t, py::str> names = ordinals::table(stem);
            for (const auto& e : nef.entries()) {
                if ((e.kind == py::str("FIXED") || e.kind == py::str("MOVEABLE")) && e.seg && segs.contains(*e.seg) &&
                    segs.at(*e.seg).kind == py::str("CODE") && names.contains(e.ordinal) && !names.at(e.ordinal).empty())
                    out.setdefault(decompile::describe::address(*e.seg, e.offset.value_or(0)), std::make_tuple(names.at(e.ordinal), ITSELF.at(stem)));
            }
        }
    }
    return out;
}

// ---- which compiler ----------------------------------------------------------------------------------

namespace {

// (major, minor) -> what the linker's version says
const char* linker32(int64_t major, int64_t minor) {
    static const std::map<std::pair<int64_t, int64_t>, const char*> t = {
        {{2, 50}, "Microsoft Visual C++ 2.x"}, {{2, 55}, "Microsoft Visual C++ 2.x"}, {{2, 60}, "Microsoft Visual C++ 2.x"},
        {{3, 0}, "Microsoft Visual C++ 4.0"},  {{3, 10}, "Microsoft Visual C++ 4.0/4.1"}, {{4, 20}, "Microsoft Visual C++ 4.2"},
        {{5, 0}, "Microsoft Visual C++ 5.0"},  {{5, 10}, "Microsoft Visual C++ 5.0"}, {{5, 12}, "Microsoft Visual C++ 5.0"},
        {{6, 0}, "Microsoft Visual C++ 6.0"},  {{2, 25}, "Borland C++ (TLINK32)"}};
    auto it = t.find({major, minor});
    return it == t.end() ? "" : it->second;
}

// 16-bit: the segmented linker's version says which generation of Microsoft's tools linked the module (the compiler
// or assembler itself is not recorded in the file)
const char* linker16(int64_t major, int64_t minor) {
    static const std::map<std::pair<int64_t, int64_t>, const char*> t = {
        {{5, 10}, "Microsoft tools of the C 6.0 generation"},
        {{5, 13}, "Microsoft tools of the C 6.0 generation"},
        {{5, 15}, "Microsoft tools of the C 6.0 generation"},
        {{5, 30}, "Microsoft tools of the C/C++ 7.0 generation"},
        {{5, 50}, "Microsoft tools of the Visual C++ 1.0 generation (16-bit)"},
        {{5, 60}, "Microsoft tools of the Visual C++ 1.5 generation (16-bit)"}};
    auto it = t.find({major, minor});
    return it == t.end() ? "" : it->second;
}

const std::vector<std::pair<std::string, std::string>>& marks() {
    static const std::vector<std::pair<std::string, std::string>> m = {
        {"Borland C++ - Copyright", "Borland C++"}, {"Borland Intl", "Borland C++"},
        {"WATCOM C/C++", "Watcom C/C++"}, {"Microsoft Visual C++ Runtime Library", "Microsoft Visual C++"},
        {"MS Run-Time Library", "Microsoft C"}, {"Microsoft (R) C ", "Microsoft C"},
        {"__CxxFrameHandler", "Microsoft Visual C++ (C++ exception handling)"}};
    return m;
}

const char* runtime_dll(const py::str& stem) {
    static const std::map<std::string, const char*> t = {
        {"CRTDLL", "the system C runtime (CRTDLL)"}, {"MSVCRT20", "Visual C++ 2.x's runtime DLL"},
        {"MSVCRT40", "Visual C++ 4.x's runtime DLL"}, {"MSVCRT", "Visual C++ 4.2 or later's runtime DLL"},
        {"MSVCRT10", "Visual C++ 1.0 (32-bit)'s runtime DLL"}, {"MFC30", "MFC 3.0 (Visual C++ 2.x)"},
        {"MFC40", "MFC 4.0 (Visual C++ 4.x)"}, {"MFC250", "MFC 2.5 (Visual C++ 1.5)"},
        {"VBRUN300", "Visual Basic 3.0"}, {"MSVBVM50", "Visual Basic 5.0"}};
    auto it = t.find(stem.utf8());
    return it == t.end() ? nullptr : it->second;
}

// [(product id, build, count)] from the block Visual C++ 6 and later leave before the PE header, or [].
std::vector<std::tuple<int64_t, int64_t, int64_t>> _rich(const py::bytes& data, int64_t end) {
    const std::string& d = data.raw();
    size_t last = std::string::npos;
    if (end > 0) {
        size_t limit = static_cast<size_t>(std::min<int64_t>(end, data.size()));
        if (limit >= 4) last = d.rfind("Rich", limit - 4);
    }
    if (last == std::string::npos || last < 0x40) return {};
    int64_t at = static_cast<int64_t>(last);
    int64_t key = py::unpack_from1<int64_t>("<I", data, at + 4);
    std::vector<std::tuple<int64_t, int64_t, int64_t>> out;
    int64_t p = at - 8;
    while (p >= 0x40) {
        auto [x, y] = py::unpack_from<int64_t, int64_t>("<II", data, p);
        int64_t a = x ^ key, b = y ^ key;
        if (a == 0x536E6144) break;                             // "DanS": where the block starts
        out.emplace_back(a >> 16, a & 0xFFFF, b);
        p -= 8;
    }
    std::reverse(out.begin(), out.end());
    return out;
}

}  // namespace

Compiler compiler(const py::str& module_path, const std::optional<py::dict<int64_t, std::tuple<py::str, py::str>>>& identified) {
    // What built a module, from what the file itself says: the linker's version in the header, marks the runtime
    // library leaves in the image, the runtime DLLs it imports, and which library its identified functions came from.
    std::optional<py::str> lane = base::corpus::lane(module_path);
    py::bytes data = py::open_rb(module_path).read();
    py::list<py::str> evidence;
    py::str guess, linker;
    std::vector<py::str> uses;
    if (lane && *lane == py::str("pe")) {
        formats::pe::PEFile pe = formats::pe::open_pe(module_path);
        py::list<py::str> parts = py::split(pe.build_meta().linker_version, py::str("."));
        int64_t major = py::int_parse(parts[0], 10), minor = py::int_parse(parts[1], 10);
        linker = py::percent(py::str("%d.%02d"), major, minor);
        guess = py::str(linker32(major, minor));
        evidence.append(py::percent(py::str("linker version %s in the header"), linker));
        auto rich = _rich(data, pe.e_lfanew);
        if (!rich.empty()) {
            py::list<py::str> shown;
            for (size_t k = 0; k < std::min<size_t>(6, rich.size()); k++)
                shown.append(py::percent(py::str("%d.%d x%d"), std::get<0>(rich[k]), std::get<1>(rich[k]), std::get<2>(rich[k])));
            evidence.append(py::str("tool block before the header: ") + py::join(py::str(", "), shown));
        }
        std::set<std::string> seen;
        for (const auto& kv : pe.imports().items()) {
            py::str u = std::get<0>(py::os::path::splitext(std::get<0>(std::get<1>(kv)))).upper();
            seen.insert(u.utf8());
        }
        for (const std::string& u : seen) uses.push_back(py::str(u));
    } else if (lane && *lane == py::str("ne")) {
        formats::ne::NEFile nef(module_path);
        linker = py::percent(py::str("%d.%02d"), nef.h.linker_ver, nef.h.linker_rev);
        guess = py::str(linker16(nef.h.linker_ver, nef.h.linker_rev));
        evidence.append(py::percent(py::str("linker version %s in the header"), linker));
        for (const py::str& m : nef.module_refs()) uses.push_back(m.upper());
    }
    for (const auto& mw : marks()) {
        if (data.raw().find(mw.first) != std::string::npos) {
            evidence.append(py::str("the text ") + py::repr(py::bytes(mw.first).decode("latin1")));
            py::str who(mw.second);
            if (guess.empty() || who.startswith(py::str("Borland")) || who.startswith(py::str("Watcom"))) guess = who;
        }
    }
    for (const py::str& stem : uses) {
        const char* r = runtime_dll(stem);
        if (r) evidence.append(py::percent(py::str("imports %s: %s"), stem, py::str(r)));
    }
    py::dict<py::str, int64_t> libs;
    py::dict<int64_t, std::tuple<py::str, py::str>> found = identified ? *identified : identify(module_path);
    for (const auto& kv : found.items()) {
        const py::str& library = std::get<1>(std::get<1>(kv));
        libs.set(library, libs.get(library, 0) + 1);
    }
    std::vector<std::pair<py::str, int64_t>> items;
    for (const auto& kv : libs.items()) items.emplace_back(std::get<0>(kv), std::get<1>(kv));
    std::stable_sort(items.begin(), items.end(), [](const auto& a, const auto& b) { return -a.second < -b.second; });
    for (size_t k = 0; k < std::min<size_t>(3, items.size()); k++)
        evidence.append(py::percent(py::str("%d functions from %s"), items[k].second, items[k].first));
    if (guess.empty() && !items.empty()) {
        // max(libs, key=libs.get): the first of the greatest
        py::str best = std::get<0>(items[0]);
        int64_t top = -1;
        for (const auto& kv : libs.items())
            if (std::get<1>(kv) > top) {
                top = std::get<1>(kv);
                best = std::get<0>(kv);
            }
        guess = std::get<0>(best.partition(py::str(":")));      // a library is described as "<compiler>: <library>"
    }
    Compiler c;
    c.linker = linker;
    c.compiler = guess.empty() ? py::str("not established") : guess;
    c.evidence = evidence;
    return c;
}

// ---- making runtime_signatures.json ------------------------------------------------------------

int64_t substance(int64_t bits, const py::bytes& code, int64_t size) {
    // How many instructions of a function do something of its own: not the frame every function sets up and takes
    // down, not `return 0`. An empty function is the same bytes in every program and every library; only one with
    // substance can be told to be a particular library function.
    int64_t n = 0;
    for (const x86::Lite& l : x86::disasm_lite(bits == 32 ? 32 : 16, code.slice(0, size).raw(), 0)) {
        py::str text = (py::str(l.mnemonic) + py::str(" ") + py::str(l.op_str)).strip();
        if (!_FRAME.match(text)) n += 1;
    }
    return n;
}

namespace {

// libraries.functions(path): (16 | 32 | "powerpc", [Function]) of a library or object file. Only the kinds learn() uses
// are told apart here: the x86 kinds (16 for an OMF file, 32 for COFF) and the rest (bits 0).
struct LibFunction {
    py::str name;
    py::bytes code, mask;
};
std::pair<int64_t, std::vector<LibFunction>> library_functions(const py::str& path) {
    py::bytes data = py::open_rb(path).read();
    std::vector<LibFunction> out;
    auto add_coff = [&](const py::bytes& body) {
        py::list<libraries::coff::Function> fs = libraries::coff::functions(body);
        for (const auto& f : fs) {
            LibFunction lf;
            lf.name = f.name;
            lf.code = f.code;
            if (std::holds_alternative<py::bytearray>(f.mask)) lf.mask = std::get<py::bytearray>(f.mask).view();
            out.push_back(lf);
        }
        return fs.size();
    };
    if (data.slice(0, 8) == py::bytes("!<arch>\n")) {
        int64_t kind = 0;
        bool kind_set = false;
        for (const auto& m : libraries::coff::members(data)) {
            const py::bytes& body = std::get<1>(m);
            int64_t found = 0;
            try {
                found = add_coff(body);
            } catch (const py::Exception&) {                   // (struct.error, IndexError)
                continue;
            }
            if (found && !kind_set) {
                int64_t machine = py::unpack_from1<int64_t>("<H", body, 0);
                kind = machine == 0x014C ? 32 : machine == 0x01F2 ? -1 : 0;     // -1: "powerpc"
                kind_set = true;
            }
        }
        return {kind ? kind : 32, out};
    }
    if (data.slice(0, 2) == py::bytes("\x4c\x01")) {
        add_coff(data);
        return {32, out};
    }
    if (data.slice(0, 2) == py::bytes("\xf2\x01")) {
        add_coff(data);
        return {-1, out};
    }
    for (const auto& f : libraries::omf::functions(data)) out.push_back({f.name, f.code, f.mask.view()});
    return {16, out};
}

}  // namespace

py::dict<py::str, int64_t> learn(const py::list<std::tuple<py::str, py::str>>& sources, const std::function<void(const py::str&)>& log0) {
    // Rewrite runtime_signatures.json from [(what the library is, file)], earlier ones first: when two libraries hold
    // the same function, it is reported as the earlier one's.
    //
    // Kept out: a function with too little substance to be told from anybody's (fewer than 4 instructions of its own;
    // 8 for a class library, whose thousands of small methods look like everyone's small methods), and any signature
    // two differently named functions share once the linker's bytes are left out -- which of them a match would be is
    // not known.
    std::function<void(const py::str&)> log = log0 ? log0 : [](const py::str& s) { py::print(s); };
    using Sig = std::tuple<int64_t, py::str, py::str>;
    using Key = std::tuple<int64_t, int64_t, py::str, py::str>;
    py::list<py::str> labels;
    struct Found {
        py::str lead;
        int64_t size;
        py::str mask, digest, name;
        int64_t label;
    };
    py::dict<Key, Found> found;
    py::dict<Key, py::set<py::str>> names;
    for (const auto& src : sources) {
        const py::str& tag = std::get<0>(src);
        const py::str& path = std::get<1>(src);
        int64_t bits;
        std::vector<LibFunction> functions;
        try {
            auto r = library_functions(path);
            bits = r.first;
            functions = std::move(r.second);
        } catch (const py::OSError& e) {
            log(py::percent(py::str("%s: %s"), path, py::str(e.what())));
            continue;
        }
        py::str label = py::percent(py::str("%s (%s)"), tag, py::os::path::basename(path).upper());
        int64_t need = tag.contains(py::str("MFC")) ? 8 : 4;
        int64_t added = 0;
        if (bits != 16 && bits != 32) {
            log(py::percent(py::str("%s: not x86 code"), path));
            continue;
        }
        for (const LibFunction& f : functions) {
            auto sig = signature(f.code, f.mask);
            int64_t lead = 0;
            while (lead < _LEAD && lead < f.code.size() && !(lead < f.mask.size() && f.mask.raw()[static_cast<size_t>(lead)] != 0)) lead += 1;
            if (!sig || !lead) continue;
            Key key(bits, std::get<0>(*sig), std::get<1>(*sig), std::get<2>(*sig));
            names.setdefault(key, py::set<py::str>()).add(_c_name(f.name));
            if (found.contains(key) || substance(bits, f.code, std::get<0>(*sig)) < need) continue;
            if (!labels.contains(label)) labels.append(label);
            found.set(key, Found{f.code.slice(0, lead).hex(), std::get<0>(*sig), std::get<1>(*sig), std::get<2>(*sig), _c_name(f.name), labels.index(label)});
            added += 1;
        }
        log(py::percent(py::str("%-60s %5d functions, %5d new signatures"), label, static_cast<int64_t>(functions.size()), added));
    }
    py::list<Key> unsure;
    for (const Key& k : found.keys())
        if (!std::get<2>(k).empty() && names.at(k).size() > 1) unsure.append(k);
    for (const Key& k : unsure) found.pop(k);
    log(py::percent(py::str("%d signatures left out: shared by differently named functions"), unsure.size()));
    py::dict<py::str, int64_t> counts;
    std::string json_out = "{\"libraries\": ";
    {
        std::string l = "[";
        for (int64_t i = 0; i < labels.size(); i++) {
            if (i) l += ", ";
            py::json::put(l, labels[i]);
        }
        json_out += l + "],\n";
    }
    std::vector<std::string> blocks;
    for (const char* bits : {"32", "16"}) {
        std::vector<std::string> rows;
        for (const auto& kv : found.items()) {
            if (std::to_string(std::get<0>(std::get<0>(kv))) != bits) continue;
            const Found& e = std::get<1>(kv);
            std::string r = "[";
            py::json::put(r, e.lead);
            r += "," + std::to_string(e.size) + ",";
            py::json::put(r, e.mask);
            r += ",";
            py::json::put(r, e.digest);
            r += ",";
            py::json::put(r, e.name);
            r += "," + std::to_string(e.label) + "]";
            rows.push_back(r);
        }
        counts.set(py::str(bits), static_cast<int64_t>(rows.size()));
        std::string block = std::string("\"") + bits + "\": [\n";
        for (size_t i = 0; i < rows.size(); i++) block += rows[i] + (i + 1 < rows.size() ? ",\n" : "");
        block += "\n]";
        blocks.push_back(block);
    }
    json_out += blocks[0] + ",\n" + blocks[1] + "\n}\n";
    py::str file = py::os::path::join(knowledge::sources::_SYMBOLS(), py::str("runtime_signatures.json"));
    py::file f = py::open(file, py::str("w"), py::str("utf-8"), std::nullopt, py::str("\n"));
    f.write(py::str(json_out));
    f.close();
    knowledge::sources::changed(file);                         // the knowledge base loads it again
    _index = std::nullopt;
    return counts;
}

int64_t main(const py::list<py::str>& argv) {
    // python -m codebreaker identify <module>                  its runtime-library functions and its compiler
    // python -m codebreaker identify --learn "WHAT=FILE" ...   make the signatures from library files
    if (argv.empty()) {
        py::print(py::str("python -m codebreaker identify <module>                  its runtime-library functions and its compiler\n"
                          "    python -m codebreaker identify --learn \"WHAT=FILE\" ...   make the signatures from library files"));
        return 2;
    }
    if (argv[0] == py::str("--learn")) {
        py::list<std::tuple<py::str, py::str>> sources;
        for (int64_t i = 1; i < argv.size(); i++) {
            if (argv[i].contains(py::str("="))) {
                auto [a, sep, b] = argv[i].partition(py::str("="));
                sources.append({a, b});
            }
        }
        py::dict<py::str, int64_t> r = learn(sources);
        py::print(py::percent(py::str("%d 32-bit and %d 16-bit signatures"), r.at(py::str("32")), r.at(py::str("16"))));
        return 0;
    }
    std::optional<py::str> path = base::corpus::retail(argv[0]);
    if (!path || path->empty()) throw py::SystemExit(py::percent(py::str("no module %r in the corpus"), argv[0]));
    auto found = identify(*path);
    std::optional<py::str> lane = base::corpus::lane(*path);
    std::vector<int64_t> addrs;
    for (int64_t a : found.keys()) addrs.push_back(a);
    std::sort(addrs.begin(), addrs.end());
    for (int64_t address : addrs) {
        py::str name = std::get<0>(found.at(address)), library = std::get<1>(found.at(address));
        py::str where;
        if (lane && *lane == py::str("ne")) {
            auto so = decompile::describe::where(address);
            where = py::percent(py::str("%d:%04x"), std::get<0>(so), std::get<1>(so));
        } else {
            where = py::percent(py::str("%08x"), address);
        }
        py::print(py::percent(py::str("%s  %-40s %s"), where, name.slice(0, 40), library));
    }
    int64_t total = 0;
    if (lane && *lane == py::str("ne")) total = decompile::describe::functions(formats::ne::NEFile(*path)).size();
    else if (lane && *lane == py::str("pe")) total = decompile::describe::function_starts(*path).size();
    Compiler info = compiler(*path, found);
    py::print(py::percent(py::str("%s: %d of %d functions are a runtime library's"), py::os::path::basename(*path).lower(),
                          static_cast<int64_t>(found.size()), total));
    py::print(py::percent(py::str("built with: %s (linker %s)"), info.compiler, info.linker.empty() ? py::str("?") : info.linker));
    for (const py::str& line : info.evidence) py::print(py::str("    ") + line);
    return 0;
}

}  // namespace codebreaker::symbols::runtime_library
