// codebreaker/build/data.cpp -- C++17 port of codebreaker.build.data (see data.hpp).

#include "codebreaker/build/data.hpp"

#include <algorithm>
#include <vector>

namespace codebreaker::build::data {

using formats::ne::Entry;
using formats::ne::NEFile;
using formats::ne::Relocation;
using formats::ne::Segment;
using formats::pe::PEFile;
using formats::pe::open_pe;

const py::list<int64_t> _REMADE = {0, 1, 2, 5, 6};

const py::list<py::str> _SELECTOR_OF = {py::str("#ifdef __WATCOMC__"), py::str("#define SELECTOR_OF(symbol, name) ((__segment)&symbol)"),
                                        py::str("#else"), py::str("#define SELECTOR_OF(symbol, name) __segname(name)"),
                                        py::str("#endif")};

namespace {

// (an empty name is falsy in Python, as None is)
bool named_(const std::optional<py::str>& s) {
    return s.has_value() && !s->empty();
}

const py::re::Pattern& re_global() {
    static const py::re::Pattern p = py::re::compile(py::str("(?m)^(#define \\w+ \\(\\*\\([^()]*\\*\\))0x([0-9a-f]+)\\)$"));
    return p;
}

const py::re::Pattern& re_constant() {
    static const py::re::Pattern p = py::re::compile(py::str("(?<![\\w.])(-?)0x([0-9a-fA-F]+)\\b"));
    return p;
}

const py::re::Pattern& re_nonword() {
    static const py::re::Pattern p = py::re::compile(py::str("\\W"));
    return p;
}

const py::re::Pattern& re_segment_number() {
    static const py::re::Pattern p = py::re::compile(py::str("\\bcodebreaker_segment_(\\d+)\\b"));
    return p;
}

// re.sub(r"\W", "_", text)
py::str word_only(const py::str& text) {
    return re_nonword().sub(py::str("_"), text);
}

// A hexadecimal constant as the text gives it: the low 64 bits, and whether it has more than 63 bits
// (such a value is in no set of int64_t addresses; its low 32 bits still say what -value & 0xFFFFFFFF is).
struct Hex {
    uint64_t low = 0;
    bool big = false;
};

Hex parse_hex_(const py::str& digits) {
    Hex h;
    for (char32_t c : digits.u32()) {
        uint64_t v = c >= '0' && c <= '9' ? c - '0' : (c | 0x20) - 'a' + 10;
        if (h.low >> 60) h.big = true;
        h.low = (h.low << 4) | v;
    }
    if (h.low >> 63) h.big = true;
    return h;
}

// int.from_bytes(b, "little") for a short slice
int64_t from_bytes_little(const py::bytes& b) {
    uint64_t v = 0;
    for (int64_t i = py::len(b) - 1; i >= 0; --i) v = (v << 8) | static_cast<uint64_t>(b[i]);
    return static_cast<int64_t>(v);
}

}  // namespace

std::optional<py::str> Data::place(int64_t va) const {
    for (const auto& [symbol, start, size, _n] : parts) {
        if (start <= va && va < start + size) {
            return py::percent("((char *)&%s + 0x%x)", symbol, va - start);
        }
    }
    return std::nullopt;
}

// ---------------------------------------------------------------------------------------------
// writing an object: bytes, runs of zeros, pointers
// ---------------------------------------------------------------------------------------------
py::list<py::str> _rows(const py::bytes& blob) {
    py::list<py::str> out;
    for (int64_t i = 0; i < py::len(blob); i += 16) {
        py::bytes row = blob.slice(i, i + 16);
        std::u32string text;
        py::str codes;
        for (int64_t b : row) {
            text.push_back(0x20 <= b && b < 0x7F && b != 0x2A && b != 0x2F && b != 0x5C ? static_cast<char32_t>(b) : U'.');
            codes += py::percent("0x%02x,", b);
        }
        out.append(py::percent("        %-96s/* %s */", codes, py::str(text)));
    }
    return out;
}

py::str _object(const py::str& struct_name, const py::str& name, const py::list<Piece>& pieces_in, bool far,
                const std::optional<py::str>& based) {
    py::list<Piece> pieces = pieces_in;
    if (py::len(pieces) > 1 && pieces[-1].kind == "zero") {   // zeros at the very end need no initialiser
        Piece tail = pieces[-1];
        tail.kind = py::str("tail");
        pieces = pieces.slice(0, py::len(pieces) - 1) + py::list<Piece>{tail};
    }
    py::list<py::str> members, values;
    for (const Piece& p : pieces) {
        if (p.kind == "bytes") {
            members.append(py::percent("    uint1 b_%05x[%d];", p.offset, py::len(p.blob)));
            values.append(py::percent("    {\n%s\n    },", py::join(py::str("\n"), _rows(p.blob))));
        } else if (p.kind == "zero") {
            members.append(py::percent("    uint1 z_%05x[%d];", p.offset, p.count));
            values.append(py::str("    { 0 },"));
        } else if (p.kind == "tail") {
            members.append(py::percent("    uint1 z_%05x[%d];", p.offset, p.count));
        } else {
            members.append(py::percent("    %s p_%05x;", p.ctype, p.offset));
            values.append(py::percent("    %s,%s", p.expr, !p.note.empty() ? py::percent("    /* %s */", p.note) : py::str("")));
        }
    }
    // packed: each member exactly at its place -- the compiler would otherwise put a word on an
    // even offset and move everything after it (the code reads the data at the original's places)
    py::str where = far ? py::str("__far ")
                        : (based.has_value() && !based->empty() ? py::percent("__based(__segname(\"%s\")) ", *based) : py::str(""));
    return py::percent("#pragma pack(push, 1)\nstruct %s {\n%s\n};\n#pragma pack(pop)\nstruct %s %s%s = {\n%s\n};\n", struct_name,
                       py::join(py::str("\n"), members), struct_name, where, name, py::join(py::str("\n"), values));
}

py::list<Piece> _pieces(const py::bytes& blob, int64_t size, const Pointers& pointers) {
    py::list<Piece> out;
    int64_t at = 0;

    auto plain = [&](int64_t lo, int64_t hi) {
        std::string buf = blob.slice(lo, hi).raw();
        if (hi - lo > static_cast<int64_t>(buf.size())) buf.resize(static_cast<size_t>(hi - lo), '\0');   // .ljust(hi - lo, b"\0")
        if (hi - lo < 0) buf.clear();
        int64_t done = 0, n = static_cast<int64_t>(buf.size());
        int64_t i = 0;
        while (i < n) {   // the runs of 64 or more zeros (the regex \x00{64,})
            if (buf[static_cast<size_t>(i)] != 0) {
                ++i;
                continue;
            }
            int64_t j = i;
            while (j < n && buf[static_cast<size_t>(j)] == 0) ++j;
            if (j - i >= 64) {
                if (i > done) {
                    Piece p;
                    p.kind = py::str("bytes");
                    p.offset = lo + done;
                    p.blob = py::bytes(buf.substr(static_cast<size_t>(done), static_cast<size_t>(i - done)));
                    out.append(p);
                }
                Piece z;
                z.kind = py::str("zero");
                z.offset = lo + i;
                z.count = j - i;
                out.append(z);
                done = j;
            }
            i = j;
        }
        if (done < n) {
            Piece p;
            p.kind = py::str("bytes");
            p.offset = lo + done;
            p.blob = py::bytes(buf.substr(static_cast<size_t>(done)));
            out.append(p);
        }
    };
    std::vector<int64_t> offsets = pointers.keys();
    std::sort(offsets.begin(), offsets.end());
    for (int64_t off : offsets) {
        if (off < at) {
            continue;   // overlaps the one before: left out
        }
        const auto& [width, ctype, expr, note] = pointers.at(off);
        plain(at, off);
        Piece p;
        p.kind = py::str("pointer");
        p.offset = off;
        p.ctype = ctype;
        p.expr = expr;
        p.note = note;
        out.append(p);
        at = off + width;
    }
    plain(at, size);
    return out;
}

// ---------------------------------------------------------------------------------------------
// 32-bit
// ---------------------------------------------------------------------------------------------
py::dict<int64_t, int64_t> relocated_words(const PEFile& pe) {
    py::dict<int64_t, int64_t> out;
    for (int64_t rva : pe.base_relocs()) {
        std::optional<int64_t> off = pe._rva_to_off(rva);
        if (off.has_value() && *off + 4 <= py::len(pe.data)) {
            out[static_cast<int64_t>(pe.image_base) + rva] = py::unpack_from1<int64_t>("<I", pe.data, *off);
        }
    }
    return out;
}

py::dict<int64_t, py::set<int64_t>> address_constants(const py::str& path, const py::list<int64_t>& starts_in) {
    std::vector<std::tuple<int64_t, int64_t>> words = relocated_words(open_pe(path)).items();
    std::sort(words.begin(), words.end());
    std::vector<int64_t> starts = starts_in.vec();
    std::sort(starts.begin(), starts.end());
    py::dict<int64_t, py::set<int64_t>> out;
    for (int64_t s : starts) out.set(s, py::set<int64_t>());
    size_t i = 0;
    for (size_t n = 0; n < starts.size(); ++n) {
        int64_t s = starts[n];
        int64_t end = n + 1 < starts.size() ? starts[n + 1] : s + 0x100000;
        while (i < words.size() && std::get<0>(words[i]) < s) ++i;
        size_t j = i;
        while (j < words.size() && std::get<0>(words[j]) < end) {
            out[s].add(std::get<1>(words[j]));
            ++j;
        }
    }
    return out;
}

Data data32(const py::str& path, const FunctionAt32& function_at, const std::optional<py::dict<int64_t, py::str>>& named_in) {
    PEFile pe = open_pe(path);
    Data d;
    int64_t base = static_cast<int64_t>(pe.image_base);
    py::set<int64_t> remade;
    for (int64_t i : _REMADE) {
        if (i < py::len(pe.datadirs) && std::get<0>(pe.datadirs[i]) != 0) remade.add(std::get<0>(pe.datadirs[i]));
    }
    d.movable = !pe.base_relocs().empty();
    py::dict<int64_t, py::str> named = named_in.has_value() ? named_in->copy() : py::dict<int64_t, py::str>();
    int64_t n = 0;
    for (const formats::pe::Section& s : pe.sections) {
        ++n;
        int64_t size = s.vsize != 0 ? py::max(s.vsize, s.rawsize) : s.rawsize;
        if ((s.chars & _CODE) != 0 || remade.contains(s.vaddr) || size == 0 || s.name == ".rsrc" || s.name == ".reloc" ||
            s.name == ".idata" || s.name == ".edata") {
            continue;
        }
        int64_t start = base + s.vaddr;
        d.sections.append(std::make_tuple(n, s.name, start, size));
        std::vector<int64_t> inner;
        for (int64_t va : named) {
            if (start < va && va < start + size) inner.push_back(va);
        }
        std::sort(inner.begin(), inner.end());
        std::vector<int64_t> cuts;
        cuts.push_back(start);
        cuts.insert(cuts.end(), inner.begin(), inner.end());
        cuts.push_back(start + size);
        for (size_t k = 0; k + 1 < cuts.size(); ++k) {
            int64_t lo = cuts[k], hi = cuts[k + 1];
            std::optional<py::str> given = named.get(lo);
            py::str symbol = named_(given) ? *given
                                           : py::percent("codebreaker_data_%d%s", n, lo > start ? py::percent("_%x", lo - start) : py::str(""));
            d.parts.append(std::make_tuple(symbol, lo, hi - lo, n));
        }
    }
    py::dict<int64_t, int64_t> words = relocated_words(pe);
    py::list<py::str> out = {
        py::percent("/* The initialised data of %s: each data section laid out as in the original. A member",
                    std::get<2>(pe.path.replace(py::str("\\"), py::str("/")).rpartition(py::str("/"))).lower()),
        py::str(" * p_NNNNN is a pointer the original's relocation table marks. */"), py::str("")};
    py::list<py::str> decls;
    for (const auto& [symbol, start, size, sec] : d.parts) {
        const formats::pe::Section& s = pe.sections[sec - 1];
        int64_t skip = start - (base + s.vaddr);
        py::bytes blob = pe.data.slice(s.rawptr + skip, s.rawptr + py::max(skip, py::min(s.rawsize, skip + size)));
        Pointers pointers;
        for (int64_t va = start; va < start + py::len(blob) - 3; ++va) {
            if (!words.contains(va)) {
                continue;
            }
            int64_t target = words.at(va);
            std::optional<py::str> fn = function_at(target);
            std::optional<py::str> where = d.place(target);
            if (named_(fn)) {
                pointers.set(va - start, std::make_tuple(int64_t(4), py::str("void *"), py::str("(void *)") + *fn, py::str("")));
            } else if (named_(where)) {
                pointers.set(va - start, std::make_tuple(int64_t(4), py::str("void *"), *where, py::str("")));
            } else {
                d.unresolved.append(std::make_tuple(
                    py::percent("%s+0x%x", s.name, va - start + skip),
                    py::percent("points at 0x%x, which is neither a reconstructed function nor carried data", target)));
            }
        }
        d.pointers += py::len(pointers);
        d.size += size;
        decls.append(py::percent("struct layout_of_%s; extern struct layout_of_%s %s;", symbol, symbol, symbol));
        out.append(py::percent("/* section %s + 0x%x: 0x%x bytes at 0x%x in the original */", s.name, skip, size, start));
        out.append(_object(py::percent("layout_of_%s", symbol), symbol, _pieces(blob, size, pointers)));
    }
    d.declarations = py::join(py::str("\n"), decls) + py::str("\n");
    d.source = py::join(py::str("\n"), out);
    return d;
}

py::dict<py::str, std::tuple<int64_t, int64_t>> _public_offsets(const py::bytes& obj) {
    py::dict<py::str, std::tuple<int64_t, int64_t>> out;
    int64_t i = 0;

    auto index = [&](int64_t p) -> std::tuple<int64_t, int64_t> {
        if ((obj[p] & 0x80) != 0) {
            return {((obj[p] & 0x7F) << 8) | obj[p + 1], p + 2};
        }
        return {obj[p], p + 1};
    };
    while (i + 3 <= py::len(obj)) {
        int64_t kind = obj[i];
        int64_t end = i + 3 + (obj[i + 1] | (obj[i + 2] << 8));
        if (end > py::len(obj)) {
            break;
        }
        if (kind == 0x90 || kind == 0x91 || kind == 0xB6 || kind == 0xB7) {
            auto [_group, p] = index(i + 3);
            (void)_group;
            auto [seg, p2] = index(p);
            p = p2;
            if (seg == 0) {
                p += 2;
            }
            int64_t wide = (kind & 1) != 0 ? 4 : 2;
            while (p < end - 1) {
                int64_t n = obj[p];
                py::str name = obj.slice(p + 1, p + 1 + n).decode("latin-1");
                p += 1 + n;
                out.set(name, std::make_tuple(seg, from_bytes_little(obj.slice(p, p + wide))));
                auto [_type, p3] = index(p + wide);
                (void)_type;
                p = p3;
            }
        }
        i = end;
    }
    return out;
}

bool layout_holds(const py::bytes& obj, const Data& d) {
    auto where = _public_offsets(obj);
    py::dict<int64_t, std::tuple<std::tuple<int64_t, int64_t>, int64_t>> first;
    for (const auto& [symbol, start, _size, n] : d.parts) {
        auto at = where.get(py::str("_") + symbol);
        if (!at.has_value()) {
            return false;
        }
        if (!first.contains(n)) {
            first.set(n, std::make_tuple(*at, start));
        }
        const auto& [seg_off, start0] = first.at(n);
        const auto& [seg, off] = seg_off;
        if (std::get<0>(*at) != seg || std::get<1>(*at) - off != start - start0) {
            return false;
        }
    }
    return true;
}

std::tuple<py::str, py::list<int64_t>> relocate_header(const py::str& header, const Data& d) {
    py::list<int64_t> lost;
    py::str fixed = re_global().sub_fn(
        [&](const py::re::Match& m) {
            int64_t va = py::int_parse(m.group(2), 16);
            std::optional<py::str> place = d.place(va);
            if (!named_(place)) {
                lost.append(va);
                return m.group(0);
            }
            return m.group(1) + *place + py::str(")");
        },
        header);
    return {d.declarations + fixed, lost};
}

std::tuple<py::str, py::list<int64_t>> relocate_text(const py::str& text, const py::set<int64_t>& addresses, const Data& d,
                                                     const FunctionAt32& function_at) {
    py::list<int64_t> lost;
    auto fix = [&](const py::re::Match& m) {
        Hex h = parse_hex_(m.group(2));
        int64_t value;
        if (!m.group(1).empty()) {
            value = static_cast<int64_t>((0 - (h.low & 0xFFFFFFFFULL)) & 0xFFFFFFFFULL);   // -value & 0xFFFFFFFF
        } else if (h.big) {
            return m.group(0);   // more than 63 bits: in no set of addresses
        } else {
            value = static_cast<int64_t>(h.low);
        }
        if (!addresses.contains(value)) {
            return m.group(0);
        }
        std::optional<py::str> fn = function_at(value);
        if (named_(fn)) {
            return py::str("((int4)") + *fn + py::str(")");
        }
        std::optional<py::str> place = d.place(value);
        if (named_(place)) {
            return py::str("((int4)") + *place + py::str(")");
        }
        lost.append(value);
        return m.group(0);
    };
    auto [head, brace, body] = text.partition(py::str("{"));
    return {head + brace + re_constant().sub_fn(fix, body), lost};
}

// ---------------------------------------------------------------------------------------------
// 16-bit
// ---------------------------------------------------------------------------------------------
py::list<std::tuple<int64_t, int64_t, int64_t, Relocation>> _places(const NEFile& nef, const Segment& seg, const py::bytes& blob) {
    py::list<std::tuple<int64_t, int64_t, int64_t, Relocation>> out;
    for (const Relocation& r : nef.relocations(seg)) {
        int64_t off = r.offset, hops = 0;
        while (off != 0xFFFF && off + 1 < py::len(blob) && hops < 0x10000) {
            int64_t word = py::unpack_from1<int64_t>("<H", blob, off);
            out.append(std::make_tuple(off, r.src, r.additive ? word : int64_t(0), r));
            if (r.additive) {
                break;
            }
            off = word;
            hops += 1;
        }
    }
    return out;
}

std::tuple<Pointers, py::list<py::str>> _relocated(const NEFile& nef, const Segment& s, const py::bytearray& blob, int64_t auto_,
                                                   const py::set<int64_t>& far_segments, const FunctionAt16& function_at,
                                                   const SegmentSymbol& segment_symbol, Data& d, const py::str& label,
                                                   const std::optional<int64_t>& own) {
    py::dict<int64_t, Entry> entries;
    for (const Entry& e : nef.entries()) {
        if (e.kind == "FIXED" || e.kind == "MOVEABLE") entries.set(e.ordinal, e);
    }
    py::list<py::str> refs = nef.module_refs();
    Pointers pointers;
    py::list<py::str> externs;
    py::list<std::tuple<int64_t, int64_t, int64_t, Relocation>> places = _places(nef, s, py::bytes(blob.view()));
    py::set<int64_t> relocated;
    for (const auto& pl : places) relocated.add(std::get<0>(pl));
    py::bytes original = blob.view();
    auto call_at = [&](int64_t seg, int64_t off) -> std::optional<py::str> {
        if (!function_at) return std::nullopt;
        return function_at(std::make_tuple(seg, off));
    };
    for (const auto& [off, src, add, r] : places) {
        int64_t width = src == 3 ? 4 : src == 2 ? 2 : src == 5 ? 2 : src == 0 ? 1 : 2;
        blob.set_slice(off, off + width, py::bytes::zeros(width));   // the file keeps a chain link there, not a value
        py::str where = py::percent("%s+0x%04x", label, off);
        if (r.kind == "IMPORTORD" || r.kind == "IMPORTNAME") {
            int64_t module = *r.module;
            py::str mod = 0 < module && module <= py::len(refs) ? refs[module - 1] : py::str("?");
            py::str symbol;
            if (r.kind == "IMPORTORD") {
                symbol = py::percent("%s_%d", word_only(mod), *r.ordinal);
                d.imports.set(symbol, std::make_tuple(mod, std::variant<int64_t, py::str>(*r.ordinal)));
            } else {
                symbol = word_only(nef.imported_name(*r.name_off));
                d.imports.set(symbol, std::make_tuple(mod, std::variant<int64_t, py::str>(nef.imported_name(*r.name_off))));
            }
            if (src == 3 && add == 0) {
                pointers.set(off, std::make_tuple(int64_t(4), py::str("void __far *"), py::str("(void __far *)") + symbol, py::str("")));
                externs.append(symbol);
            } else if (src == 5 && add == 0) {   // its offset: KERNEL's __WINFLAGS, a value
                pointers.set(off, std::make_tuple(int64_t(2), py::str("uint2"), py::str("(uint2)") + symbol, py::str("")));
                externs.append(symbol);
            } else if (src == 2) {   // its selector (KERNEL's __0040H)
                pointers.set(off, std::make_tuple(int64_t(2), py::str("__segment"), py::str("(__segment)") + symbol, py::str("")));
                externs.append(symbol);
            } else {
                d.unresolved.append(std::make_tuple(where, py::str("part of the address of ") + symbol));
            }
            continue;
        }
        if (r.kind != "INTERNAL") {
            d.unresolved.append(std::make_tuple(where, py::str("a floating-point fixup")));
            continue;
        }
        int64_t tseg, toff;
        if (*r.seg == 0xFF) {
            std::optional<Entry> e = entries.get(*r.entry);
            if (!e.has_value()) {
                d.unresolved.append(
                    std::make_tuple(where, py::percent("names entry %d, which the module does not have", *r.entry)));
                continue;
            }
            tseg = *e->seg;
            toff = *e->offset;
        } else {
            tseg = *r.seg;
            toff = *r.entry;
        }
        toff = (toff + add) & 0xFFFF;
        std::optional<py::str> fn = call_at(tseg, toff);
        // A selector of a code segment right after a word that is a function's offset there: a far
        // pointer to that function whose offset the linker wrote and whose selector the loader does
        // (SETUPX's table of section handlers, DS:0F66: 4:52D2, 4:55D2, 4:56F8). Written as the
        // pointer to the function by name, the linker writes its offset where the function is now --
        // as a number it stayed the original's, and when the function moved, GenInstallEx called
        // whatever was put in its place (Setup: SU99414 at "Preparing to copy files").
        int64_t before = off - 2;
        std::optional<py::str> named;
        if (src == 2 && before >= 0 && !relocated.contains(before) && !pointers.contains(before) && tseg != auto_ &&
            !(own.has_value() && tseg == *own)) {
            named = call_at(tseg, py::unpack_from1<int64_t>("<H", original, before));
        }
        if (named_(named)) {
            pointers.pop(off, std::make_tuple(int64_t(0), py::str(""), py::str(""), py::str("")));
            pointers.set(before, std::make_tuple(int64_t(4), py::str("void __far *"), py::str("(void __far *)") + *named,
                                                 py::percent("%d:%04X", tseg, py::unpack_from1<int64_t>("<H", original, before))));
            continue;
        }
        // The same for a selector of a data segment the build carries: with the word before it, a
        // far pointer into that segment -- the same bytes either way (the segment's object starts
        // at its offset 0), and a constant Microsoft C takes too, where a selector alone is not
        if (src == 2 && before >= 0 && !relocated.contains(before) && !pointers.contains(before) &&
            (tseg == auto_ || far_segments.contains(tseg))) {
            int64_t word = py::unpack_from1<int64_t>("<H", original, before);
            py::str base = tseg == auto_ ? py::str("(char __far *)&codebreaker_data")
                                         : py::percent("(uint1 __far *)&codebreaker_segment_%d", tseg);
            pointers.pop(off, std::make_tuple(int64_t(0), py::str(""), py::str(""), py::str("")));
            pointers.set(before, std::make_tuple(int64_t(4), py::str("void __far *"),
                                                 py::percent("(void __far *)(%s + 0x%x)", base, word), py::str("")));
            continue;
        }
        bool in_own = tseg == auto_ || (own.has_value() && tseg == *own);
        if (src == 5 && in_own) {   // an offset into this very segment: a number
            py::pack_into("<H", blob, off, toff);
        } else if (src == 3 && named_(fn)) {
            // (as void __far *: the same four bytes, and a constant Microsoft C takes for a __pascal
            // function as well, where it refused a cast to a pointer to a cdecl function)
            pointers.set(off, std::make_tuple(int64_t(4), py::str("void __far *"), py::str("(void __far *)") + *fn, py::str("")));
        } else if (src == 3 && tseg == auto_) {
            pointers.set(off, std::make_tuple(int64_t(4), py::str("void __far *"),
                                              py::percent("(void __far *)((char *)&codebreaker_data + 0x%x)", toff), py::str("")));
        } else if (src == 3 && far_segments.contains(tseg)) {
            pointers.set(off, std::make_tuple(int64_t(4), py::str("void __far *"),
                                              py::percent("(void __far *)((uint1 __far *)&codebreaker_segment_%d + 0x%x)", tseg, toff),
                                              py::str("")));
        } else if (src == 2 && tseg == auto_) {   // a segment's selector, which the loader
            pointers.set(off, std::make_tuple(int64_t(2), py::str("__segment"), py::str("(__segment)&codebreaker_data"),
                                              py::str("")));   // writes: its own,
        } else if (src == 2 && far_segments.contains(tseg)) {   // a segment carried,
            pointers.set(off, std::make_tuple(int64_t(2), py::str("__segment"),
                                              py::percent("(__segment)&codebreaker_segment_%d", tseg), py::str("")));
        } else if (src == 2 && segment_symbol && named_(segment_symbol(tseg))) {   // a code segment's
            // (OpenWatcom: the selector of a symbol in that segment; Microsoft C has no such
            // constant, but names the segment -- CODE<n>, as /NT CODE<n> builds it)
            pointers.set(off, std::make_tuple(int64_t(2), py::str("__segment"),
                                              py::percent("SELECTOR_OF(%s, \"CODE%d\")", *segment_symbol(tseg), tseg),
                                              py::percent("the selector of segment %d", tseg)));
        } else {
            py::str what = src == 5   ? py::percent("an offset in segment %d", tseg)
                           : src == 3 ? py::percent("a far pointer to %d:%04x", tseg, toff)
                                      : py::percent("a fixup of kind %d", src);
            d.unresolved.append(std::make_tuple(where, what + py::str(", which has no C form here")));
        }
    }
    return {pointers, externs};
}

Data data16(const py::str& path, const FunctionAt16& function_at, const py::set<int64_t>& far_segments,
            const SegmentSymbol& segment_symbol) {
    NEFile nef(path);
    Data d;
    int64_t auto_ = nef.h.autodata_seg;
    py::list<Segment> segs = nef.segments();
    if (auto_ == 0 || auto_ > py::len(segs)) {
        d.source = py::percent("/* %s has no automatic data segment. */\nchar codebreaker_data = 1;\n", nef.module_name());
        return d;
    }
    Segment s = segs[auto_ - 1];
    py::bytearray blob(s.file_off != 0 ? nef.data.slice(s.file_off, s.file_off + s.length) : py::bytes());
    int64_t size = s.file_off != 0 ? py::max(py::len(blob), py::min(s.minalloc, int64_t(0x10000))) : py::min(s.minalloc, int64_t(0x10000));
    py::dict<int64_t, Entry> entries;
    for (const Entry& e : nef.entries()) {
        if (e.kind == "FIXED" || e.kind == "MOVEABLE") entries.set(e.ordinal, e);
    }
    py::list<py::str> refs = nef.module_refs();
    (void)refs;
    auto [pointers, externs] = _relocated(nef, s, blob, auto_, far_segments, function_at, segment_symbol, d);
    d.pointers = py::len(pointers);
    d.size = size;
    py::list<py::str> head = {
        py::percent("/* The automatic data segment of %s: 0x%x bytes (0x%x of them in the file).", nef.module_name(), size, py::len(blob)),
        py::str(" * It must be the first thing linked into the data segment: the code addresses it by offset. */")};
    {
        std::vector<py::str> ext = externs.vec();
        std::sort(ext.begin(), ext.end());
        ext.erase(std::unique(ext.begin(), ext.end()), ext.end());
        for (const py::str& x : ext) head.append(py::percent("extern uint4 __far __stdcall %s();", x));
    }
    head.extend(_SELECTOR_OF);
    // data the module exports (KERNEL's THHOOK): the segment is cut there, and from there on it
    // is a variable of the export's own name -- the parts follow one another in the segment
    py::dict<int64_t, py::str> names;
    for (const auto& [name, ordinal] : nef.resident_names()) {
        if (ordinal != 0) names.set(ordinal, name);
    }
    for (const auto& [name, ordinal] : nef.nonresident_names()) {
        if (ordinal != 0) names.set(ordinal, name);
    }
    py::dict<int64_t, py::str> cuts;
    for (const auto& [o, e] : entries.items()) {
        if (e.seg.has_value() && *e.seg == auto_ && 0 < *e.offset && *e.offset < size) {
            cuts.set(*e.offset, names.get(o, py::percent("export_%d", o)));
        }
    }
    std::vector<int64_t> cut_offsets = cuts.keys();
    std::sort(cut_offsets.begin(), cut_offsets.end());
    py::list<Piece> pieces = _cut(_pieces(py::bytes(blob.view()), size, pointers), py::list<int64_t>(cut_offsets));
    d.pieces = pieces;
    d.cuts = py::dict<int64_t, py::str>();
    for (const auto& [o, n] : cuts.items()) d.cuts.set(o, word_only(n));
    d.exported = py::dict<std::tuple<int64_t, int64_t>, py::str>();
    py::list<py::str> parts;
    int64_t start = 0;
    std::vector<int64_t> ends = cut_offsets;
    ends.push_back(size);
    for (int64_t at : ends) {
        py::list<Piece> part;
        for (const Piece& piece : pieces) {
            if (start <= piece.offset && piece.offset < at) part.append(piece);
        }
        py::str name = start == 0 ? py::str("codebreaker_data") : word_only(cuts.at(start));
        if (!part.empty()) {
            parts.append(_object(py::percent("codebreaker_data_%s", start != 0 ? name : py::str("segment")), name, part));
            if (start != 0) {
                d.exported.set(std::make_tuple(auto_, start), name);
            }
        }
        start = at;
    }
    // (a far pointer into one of the module's other data segments names its bytes: declared here,
    // not left to the C's header -- with no C function using segment 41, SETUPX's data did not
    // compile, and the build fell back to a data segment of one byte)
    std::vector<int64_t> found;
    for (const py::re::Match& m : re_segment_number().finditer(py::join(py::str("\n"), parts))) {
        found.push_back(py::int_parse(m.group(1)));
    }
    std::sort(found.begin(), found.end());
    found.erase(std::unique(found.begin(), found.end()), found.end());
    for (int64_t n : found) head.append(_segment_declaration(n));
    d.source = py::join(py::str("\n"), head) + py::str("\n") + py::join(py::str("\n"), parts);
    return d;
}

py::list<Piece> _cut(const py::list<Piece>& pieces, const py::list<int64_t>& offsets) {
    py::list<Piece> out;
    for (const Piece& p : pieces) {
        bool is_bytes = p.kind == "bytes", is_zero = p.kind == "zero";
        int64_t length = is_bytes ? py::len(p.blob) : is_zero ? p.count : 0;   // (None for the other kinds: 0 here, both falsy)
        py::list<int64_t> inner;
        for (int64_t c : offsets) {
            if (length != 0 && p.offset < c && c < p.offset + length) inner.append(c);
        }
        if (inner.empty()) {
            out.append(p);
            continue;
        }
        std::vector<int64_t> bounds;
        bounds.push_back(p.offset);
        for (int64_t c : inner) bounds.push_back(c);
        bounds.push_back(p.offset + length);
        for (size_t k = 0; k + 1 < bounds.size(); ++k) {
            int64_t lo = bounds[k], hi = bounds[k + 1];
            Piece q;
            q.offset = lo;
            if (is_bytes) {
                q.kind = py::str("bytes");
                q.blob = p.blob.slice(lo - p.offset, hi - p.offset);
            } else {
                q.kind = py::str("zero");
                q.count = hi - lo;
            }
            out.append(q);
        }
    }
    return out;
}

py::str _segment_declaration(int64_t n) {
    return py::percent(
        "struct codebreaker_segment_%d_t;\nextern struct codebreaker_segment_%d_t "
        "__based(__segname(\"CODEBREAKER_SEGMENT_%d\")) codebreaker_segment_%d;",
        n, n, n, n);
}

py::str far_segments(const py::dict<int64_t, py::bytes>& segments, const std::optional<py::str>& path, const FunctionAt16& function_at,
                     const SegmentSymbol& segment_symbol) {
    std::optional<NEFile> nef;
    if (path.has_value() && !path->empty()) nef.emplace(*path);
    py::dict<int64_t, Segment> segs;
    if (nef.has_value()) {
        for (const Segment& sg : nef->segments()) segs.set(sg.idx, sg);
    }
    int64_t auto_ = nef.has_value() ? nef->h.autodata_seg : 0;
    py::list<py::str> out, externs;
    Data scratch;
    std::vector<int64_t> numbers = segments.keys();
    std::sort(numbers.begin(), numbers.end());
    for (int64_t n : numbers) out.append(_segment_declaration(n));
    py::set<int64_t> carried;
    for (int64_t n : numbers) carried.add(n);
    for (int64_t n : numbers) {
        py::bytes image = segments.at(n);
        std::optional<Segment> s = segs.get(n);
        int64_t size = py::len(image);
        Pointers pointers;
        py::bytearray blob;
        if (s.has_value() && s->file_off != 0) {
            blob = py::bytearray(nef->data.slice(s->file_off, s->file_off + s->length));
            blob += py::bytes::zeros(py::max(int64_t(0), size - py::len(blob)));
            auto [ptrs, more] = _relocated(*nef, *s, blob, auto_, carried, function_at, segment_symbol, scratch, py::percent("seg%d", n), n);
            pointers = ptrs;
            externs.extend(more);
        } else {
            blob = py::bytearray(image);
        }
        out.append(_object(py::percent("codebreaker_segment_%d_t", n), py::percent("codebreaker_segment_%d", n),
                           _pieces(py::bytes(blob.view().slice(0, size)), size, pointers), false, py::percent("CODEBREAKER_SEGMENT_%d", n)));
    }
    py::list<py::str> head;
    {
        std::vector<py::str> ext = externs.vec();
        std::sort(ext.begin(), ext.end());
        ext.erase(std::unique(ext.begin(), ext.end()), ext.end());
        for (const py::str& x : ext) head.append(py::percent("extern uint4 __far __stdcall %s();", x));
    }
    return py::join(py::str("\n"), head + out) + (!out.empty() ? py::str("\n") : py::str(""));
}

}  // namespace codebreaker::build::data
