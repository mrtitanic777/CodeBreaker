// codebreaker/source/ne_writer.cpp -- typed C++ of codebreaker.source.ne_writer.

#include "codebreaker/source/ne_writer.hpp"

#include <algorithm>

#include "codebreaker/source/assembly.hpp"

namespace codebreaker::source::ne_writer {

namespace w = codebreaker::source::image_writer;
namespace l2 = codebreaker::translate::l2;
using pe_writer::branch_target;
using pe_writer::decode;

const py::str DOC(R"DOC(ne_writer.py -- a 16-bit Windows (or OS/2) program as assembly source that builds back into it, byte for byte.

    python -m codebreaker ne-source <program> [--out DIR] [--keep-stub]

    from codebreaker.source import ne_writer
    r = ne_writer.reconstruct(path, outdir)
    r["source"], r["image"], r["same"], r["instructions"], r["raw"], r["rounds"]

The way pe_writer writes a 32-bit image, for the segmented kind: one NASM source assembled flat (-f bin),
each segment a section at its place in the file (start=) whose addresses are its own offsets (vstart=0),
followed by its relocation table. Whatever the linker made is written as what it refers to, not as a
number: the segment table's sectors and lengths as expressions of the segments' places, every relocation
chain (LINK writes, in each place a relocation patches, where the next place of the same target is) as the
label of the next place, every record's first place as its label, and an internal reference's offset as
the label in its segment. So the file is the linker's output as written down, and stands without the linker
that made it -- whichever version that was.

Code is what is reached from the entry table, from the program's start, and from every internal reference
into a code segment; what is not reached is data. An instruction is written as its mnemonic where NASM
gives back the original's bytes (checked every round from the assembler's map, as pe_writer does); one that
holds a place a relocation patches is written as its bytes, with those places as expressions and its
mnemonic beside it. The DOS program in front (the stub) comes from its own source when that builds back.
)DOC");

const py::set<py::str> _STOP{py::str("ret"), py::str("retf"), py::str("jmp"), py::str("ljmp"), py::str("hlt"), py::str("iret"), py::str("iretd"), py::str("ud2")};
const py::set<int64_t> _SEGS{0x26, 0x2E, 0x36, 0x3E, 0x64, 0x65};
const py::dict<int64_t, int64_t> _WIDTH{{2, 2}, {3, 4}, {5, 2}, {11, 6}, {13, 4}, {0, 1}};   // bytes a relocation patches, by source type

namespace {

// _REGS16: the 16, 32 and 8-bit register names, in encoding order
const std::vector<std::vector<const char*>>& regs16_() {
    static const std::vector<std::vector<const char*>> r = {{"ax", "cx", "dx", "bx", "sp", "bp", "si", "di"},
                                                           {"eax", "ecx", "edx", "ebx", "esp", "ebp", "esi", "edi"},
                                                           {"al", "cl", "dl", "bl", "ah", "ch", "dh", "bh"}};
    return r;
}

bool is_reg_(const py::str& x) {
    for (const auto& g : regs16_()) {
        for (const char* n : g) {
            if (x == py::str(n)) {
                return true;
            }
        }
    }
    return false;
}

py::list<py::str> make_prelude() {
    py::list<py::str> p;
    p.append(py::str(""));
    p.append(py::str("; op_ r, r: the register-to-register form with the direction bit set (opcode r, r/m), the one"));
    p.append(py::str("; Microsoft's compiler writes; NASM itself writes the other"));
    const int64_t widths[3] = {1, 2, 0};
    for (int g = 0; g < 3; ++g) {
        const auto& names = regs16_()[static_cast<size_t>(g)];
        for (size_t k = 0; k < names.size(); ++k) {
            p.append(py::percent("%%assign _r_%s %d", py::str(names[k]), static_cast<int64_t>(k)));
        }
        for (size_t k = 0; k < names.size(); ++k) {
            p.append(py::percent("%%assign _w_%s %d", py::str(names[k]), widths[g]));
        }
    }
    p.append(py::str("%macro _rr 3"));
    p.append(py::str("  %if _w_%2 == 2"));
    p.append(py::str("    db 0x66"));
    p.append(py::str("  %endif"));
    p.append(py::str("  db (%1) | 2 | (_w_%2 > 0), 0xC0 | (_r_%2 << 3) | _r_%3"));
    p.append(py::str("%endmacro"));
    py::list<py::str> keys;
    for (const py::str& k : pe_writer::RR().keys()) {
        keys.append(k);
    }
    keys.sort();
    for (const py::str& m : keys) {
        p.append(py::percent("%%define %s_ _rr 0x%02x,", m, pe_writer::RR().at(m)));
    }
    return p;
}

py::str spaces_(int64_t n) {
    return py::str(std::u32string(static_cast<size_t>(std::max<int64_t>(n, 0)), U' '));
}

py::str db_bytes_(const py::bytes& b, int64_t lo, int64_t hi) {
    std::vector<py::str> parts;
    for (int64_t k = lo; k < hi; ++k) {
        parts.push_back(py::percent("0x%02x", b[k]));
    }
    return py::join(py::str(","), parts);
}

// the bytes of an instruction from lo to hi, as `db` data
py::str db_slice_(const py::bytes& b, int64_t lo, int64_t hi) {
    return py::str("db ") + db_bytes_(b, lo, std::min(hi, py::len(b)));
}

bool same_reloc_(const ne::Relocation& a, const ne::Relocation& b) {
    return a.src == b.src && a.rel_type == b.rel_type && a.additive == b.additive && a.offset == b.offset && a.kind == b.kind && a.seg == b.seg &&
           a.entry == b.entry && a.module == b.module && a.ordinal == b.ordinal && a.name_off == b.name_off && a.ftype == b.ftype;
}

}  // namespace

const py::list<py::str> _PRELUDE = make_prelude();

Writer::Writer(const py::str& p) : path(p), ne(p) {
    d = ne.data;
    shift = ne.h.align_shift;
    for (const ne::Segment& s : ne.segments()) {
        if (!s.file_off) {
            continue;
        }
        Seg g;
        g.idx = s.idx;
        g.file_off = s.file_off;
        g.length = s.length;
        g.flags = s.flags;
        g.minalloc = s.minalloc;
        g.kind = s.kind;
        g.relocs = ne.relocations(s);
        g.table = (s.flags & 0x100) ? 2 + 8 * py::len(g.relocs) : 0;
        g.code = !(s.flags & 1);
        by_idx.set(g.idx, static_cast<int64_t>(segs.size()));
        segs.push_back(g);
    }
    // every place a relocation patches: (segment, offset) -> (record, width, chained)
    for (const Seg& s : segs) {
        for (int64_t k = 0; k < py::len(s.relocs); ++k) {
            const ne::Relocation& r = s.relocs[k];
            int64_t wd = _WIDTH.get(r.src, 2);
            int64_t at = r.offset;
            py::set<int64_t> seen;
            while (!seen.contains(at) && at + wd <= s.length) {
                seen.add(at);
                int64_t nxt = !r.additive ? py::unpack_from1<uint16_t>("<H", d, s.file_off + at) : 0xFFFF;
                sites.set(std::make_tuple(s.idx, at), Site{k, wd, !r.additive, nxt});
                if (r.additive || nxt == 0xFFFF) {
                    break;
                }
                at = nxt;
            }
        }
    }
    _decode();
}

// -- code ----------------------------------------------------------------------------------
void Writer::_decode() {
    py::list<Place> seeds;
    for (const ne::Entry& e : ne.entries()) {
        if ((e.kind == py::str("FIXED") || e.kind == py::str("MOVEABLE")) && has_seg(*e.seg)) {
            seeds.append(std::make_tuple(*e.seg, *e.offset));
        }
    }
    int64_t cs = ne.h.csip >> 16;
    if (has_seg(cs)) {
        seeds.append(std::make_tuple(cs, ne.h.csip & 0xFFFF));
    }
    py::list<Place> entries = py::list<Place>::from(seeds);   // (where the module is entered: each a function)
    for (const Seg& s : segs) {
        for (const ne::Relocation& r : s.relocs) {
            if (r.kind == py::str("INTERNAL") && *r.seg != 0xFF && has_seg(*r.seg)) {
                seeds.append(std::make_tuple(*r.seg, *r.entry));
            }
        }
    }
    py::list<Place> work;
    for (const Place& x : seeds) {
        if (has_seg(std::get<0>(x)) && seg(std::get<0>(x)).code) {
            work.append(x);
        }
    }
    py::dict<Place, int64_t> sitebytes;
    for (const auto& [key, site] : sites.items()) {
        for (int64_t j = 0; j < site.w; ++j) {
            sitebytes.set(std::make_tuple(std::get<0>(key), std::get<1>(key) + j), std::get<1>(key));
        }
    }
    while (py::truthy(work)) {
        auto [si, a] = work.pop();
        const Seg& s = seg(si);
        while (0 <= a && a < s.length && !insns.contains(std::make_tuple(si, a)) && !covered.contains(std::make_tuple(si, a))) {
            int64_t o = s.file_off + a;
            std::optional<Insn> one = decode(16, d.slice(o, std::min(o + 15, s.file_off + s.length)), a);
            if (!one.has_value()) {
                break;
            }
            bool overlap = false;
            for (int64_t k = 0; k < one->size; ++k) {
                if (covered.contains(std::make_tuple(si, a + k))) {
                    overlap = true;
                    break;
                }
            }
            if (overlap) {
                break;
            }
            // a relocated place must lie wholly inside the instruction
            py::set<int64_t> starts_in;
            for (int64_t k = 0; k < one->size; ++k) {
                std::optional<int64_t> sb = sitebytes.get(std::make_tuple(si, a + k));
                if (sb.has_value()) {
                    starts_in.add(*sb);
                }
            }
            bool cut = false;
            for (int64_t st : starts_in) {
                if (st < a || st + sites.at(std::make_tuple(si, st)).w > a + one->size) {
                    cut = true;
                    break;
                }
            }
            if (cut) {
                break;
            }
            insns.set(std::make_tuple(si, a), *one);
            for (int64_t k = 0; k < one->size; ++k) {
                covered.set(std::make_tuple(si, a + k), a);
            }
            std::optional<int64_t> tgt = _target(*one);
            if (tgt.has_value() && !py::truthy(starts_in)) {
                work.append(std::make_tuple(si, *tgt));
            }
            if (_STOP.contains(one->mnemonic)) {
                break;
            }
            a += one->size;
        }
    }
    starts = _function_starts(entries);
}

py::set<Place> Writer::_function_starts(const py::list<Place>& entries) const {
    py::set<Place> out = py::set<Place>::from(entries);
    for (const auto& [key, i] : insns.items()) {
        std::optional<int64_t> t = _target(i);
        if (i.mnemonic == py::str("call") && t.has_value()) {
            out.add(std::make_tuple(std::get<0>(key), *t));
        }
    }
    for (const auto& [key, site] : sites.items()) {
        auto [si, at] = key;
        std::optional<int64_t> st = covered.get(key);
        const ne::Relocation& r = seg(si).relocs[site.k];
        if (st.has_value() && *st == at - 1 && insns.at(std::make_tuple(si, *st)).mnemonic == py::str("lcall") && r.kind == py::str("INTERNAL") &&
            *r.seg != 0xFF) {
            out.add(std::make_tuple(*r.seg, *r.entry));
        }
    }
    py::set<Place> found;
    for (const Place& x : out) {
        if (insns.contains(x)) {
            found.add(x);
        }
    }
    return found;
}

std::optional<int64_t> Writer::_target(const Insn& i) {
    return branch_target(i);
}

// -- names ---------------------------------------------------------------------------------
py::str Writer::label(int64_t si, int64_t a) const {
    return py::percent(insns.contains(std::make_tuple(si, a)) ? "s%d_L%x" : "s%d_D%x", si, a);
}

py::str Writer::ref(int64_t si, int64_t a) const {
    std::optional<py::str> pl = placed.get(std::make_tuple(si, a));
    if (pl.has_value()) {
        return *pl;
    }
    std::optional<int64_t> st = covered.get(std::make_tuple(si, a));
    if (st.has_value()) {
        return py::percent("%s+%d", placed.at(std::make_tuple(si, *st)), a - *st);
    }
    return py::percent("0x%x", a);
}

py::set<Place> Writer::wanted(bool mark_all) const {
    py::set<Place> want;
    for (const Seg& s : segs) {
        for (const ne::Relocation& r : s.relocs) {
            want.add(std::make_tuple(s.idx, r.offset));
            if (r.kind == py::str("INTERNAL") && has_seg(*r.seg)) {
                want.add(std::make_tuple(*r.seg, *r.entry));
            }
        }
    }
    for (const auto& [key, site] : sites.items()) {
        want.add(key);
        if (site.chained && site.nxt != 0xFFFF) {
            want.add(std::make_tuple(std::get<0>(key), site.nxt));
        }
    }
    for (const ne::Entry& e : ne.entries()) {
        if ((e.kind == py::str("FIXED") || e.kind == py::str("MOVEABLE")) && has_seg(*e.seg)) {
            want.add(std::make_tuple(*e.seg, *e.offset));
        }
    }
    for (const auto& [key, i] : insns.items()) {
        std::optional<int64_t> t = _target(i);
        if (t.has_value()) {
            want.add(std::make_tuple(std::get<0>(key), *t));
        }
    }
    if (mark_all) {
        for (const Place& k : insns.keys()) {
            want.add(k);
        }
    }
    return want;
}

// -- source --------------------------------------------------------------------------------
py::list<py::str> Writer::text(int64_t si, const Insn& i, bool raw) const {
    int64_t a = i.address;
    const py::bytes& b = i.bytes;
    std::optional<int64_t> tgt = _target(i);
    py::list<int64_t> inside;
    for (int64_t k = 0; k < i.size; ++k) {
        if (sites.contains(std::make_tuple(si, a + k))) {
            inside.append(k);
        }
    }
    if (!raw && !py::truthy(inside)) {
        std::optional<py::str> t = _nasm(si, i, tgt);
        if (t.has_value() && py::truthy(*t)) {
            return py::list<py::str>{*t};
        }
    }
    py::list<py::str> out;
    int64_t k = 0;
    int64_t n = py::len(b);
    while (k < n) {
        if (inside.contains(k)) {
            const Site& site = sites.at(std::make_tuple(si, a + k));
            if (k) {
                out.append(py::percent("%s:", placed.at(std::make_tuple(si, a + k))));
            }
            if (site.chained) {
                out.append(py::percent("dw %s", site.nxt == 0xFFFF ? py::str("0xffff") : ref(si, site.nxt)));
                if (site.w > 2) {
                    out.append(py::str("db ") + db_bytes_(b, k + 2, std::min(k + site.w, n)));
                }
            } else {
                out.append(py::str("db ") + db_bytes_(b, k, std::min(k + site.w, n)));
            }
            k += site.w;
        } else if (tgt.has_value() && k == i.imm_offset) {
            const char* wd = i.imm_size == 4 ? "dd" : i.imm_size == 2 ? "dw" : "db";
            if (i.imm_size != 4 && i.imm_size != 2 && i.imm_size != 1) {
                throw py::KeyError(py::to_str(i.imm_size).utf8());
            }
            out.append(py::percent("%s %s-($+%d)", py::str(wd), ref(si, *tgt), i.imm_size));
            k += i.imm_size;
        } else {
            int64_t stop = n;
            for (int64_t x : inside) {
                if (x > k) {
                    stop = std::min(stop, x);
                }
            }
            if (tgt.has_value() && i.imm_offset > k) {
                stop = std::min(stop, i.imm_offset);
            }
            out.append(py::str("db ") + db_bytes_(b, k, stop));
            k = stop;
        }
    }
    int64_t first = 0;
    while (first < py::len(out) && out[first].endswith(py::str(":"))) {
        first += 1;
    }
    if (first >= py::len(out)) {
        throw py::StopIteration();
    }
    out[first] = out[first] + py::percent("%s; %s %s", spaces_(std::max<int64_t>(1, 40 - py::len(out[first]))), i.mnemonic, i.op_str);
    return out;
}

std::optional<py::str> Writer::_nasm(int64_t si, const Insn& i, const std::optional<int64_t>& tgt) const {
    static const py::re::Pattern string_ops = py::re::compile(assembly::STRING_OPS);
    static const py::re::Pattern string_ops2 = py::re::compile(py::str("^(rep |repe |repne )?(movs|cmps|stos|lods|scas|ins|outs)[bwd]$"));
    static const py::re::Pattern rr_ops = py::re::compile(py::str("^(\\w+), (\\w+)$"));
    static const py::re::Pattern st_reg = py::re::compile(py::str("\\bst\\((\\d)\\)"));
    static const py::re::Pattern seg_br = py::re::compile(py::str("\\b([cdefgs]s):\\["));
    static const py::re::Pattern bracket_in = py::re::compile(py::str("\\[([^\\]]*)\\]"));
    static const py::re::Pattern imm_tail = py::re::compile(py::str("(-?0x[0-9a-f]+|-?\\d+)$"));
    static const py::re::Pattern base_idx = py::re::compile(py::str("[a-z]{2}\\s*[+-]"));
    static const py::re::Pattern disp_tail = py::re::compile(py::str("[+-] (0x[0-9a-f]+|\\d+)$"));
    const py::str& m = i.mnemonic;
    py::str ops = i.op_str;
    if (m == py::str("ljmp") || m == py::str("lcall") ||
        (m.startswith(py::str("l")) && (m.slice_from(1) == py::str("call") || m.slice_from(1) == py::str("jmp")))) {
        return std::nullopt;
    }
    if (string_ops.match(m).has_value() || string_ops2.match(m).has_value()) {
        for (int64_t p : i.prefix) {
            if (_SEGS.contains(p)) {
                return std::nullopt;
            }
        }
        return m;
    }
    if (tgt.has_value()) {
        if (m == py::str("call")) {
            if (i.imm_size == 2 && i.size == 3) {
                return py::percent("call %s", ref(si, *tgt));
            }
            return std::nullopt;
        }
        if (m == py::str("loop") || m == py::str("loope") || m == py::str("loopne") || m == py::str("jcxz") || m == py::str("jecxz")) {
            if (i.size == 2) {
                return py::percent("%s %s", m, ref(si, *tgt));
            }
            return std::nullopt;
        }
        if (i.size != 2 && i.size != 3 && i.size != 4) {
            return std::nullopt;
        }
        return py::percent("%s %s %s", m, py::str(i.imm_size == 1 ? "short" : "near"), ref(si, *tgt));
    }
    std::optional<py::re::Match> rr = rr_ops.match(ops);
    if (pe_writer::RR().contains(m) && rr.has_value() && is_reg_(rr->group(1)) && is_reg_(rr->group(2))) {
        int64_t op = i.bytes[i.bytes[0] == 0x66 ? 1 : 0];
        if (op & 2) {
            return py::percent("%s_ %s", m, ops);
        }
        return py::percent("%s %s", m, ops);
    }
    ops = ops.replace(py::str(" ptr "), py::str(" ")).replace(py::str("xmmword "), py::str("oword ")).replace(py::str("xword "), py::str("tword "));
    ops = st_reg.sub(py::str("st\\1"), ops);
    ops = seg_br.sub(py::str("[\\1:"), ops);
    if (i.imm_size == 2 && pe_writer::_ARITH.contains(m)) {
        std::optional<py::re::Match> mm = imm_tail.search(ops);
        if (mm.has_value()) {
            int64_t v = py::int_parse(mm->group(1), 0);
            if ((-128 <= v && v <= 127) || v >= 0xFF80) {
                ops = ops.slice(0, mm->start()) + py::str("strict word ") + mm->group(1);
            }
        }
    }
    if (i.disp_size == 2) {
        std::optional<py::re::Match> br = bracket_in.search(ops);
        if (br.has_value() && base_idx.search(br->group(1)).has_value()) {
            std::optional<py::re::Match> mm = disp_tail.search(br->group(1));
            if (mm.has_value() && py::int_parse(mm->group(1), 0) <= 0x7F) {
                ops = ops.slice(0, br->start()) + py::str("[word ") + br->group(1) + py::str("]") + ops.slice_from(br->end());
            }
        }
    }
    return py::percent("%s %s", m, ops).strip();
}

std::tuple<py::str, py::dict<int64_t, Place>> Writer::source(const py::set<Place>& raw_in, bool mark_all, const std::optional<py::str>& stub, bool exact) {
    py::set<Place> raw = raw_in;
    py::set<Place> want = wanted(mark_all);
    placed = py::dict<Place, py::str>();
    for (const Place& key : want) {
        auto [si, a] = key;
        if (has_seg(si) && (insns.contains(key) || !covered.contains(key)) && a <= seg(si).length) {
            placed.set(key, label(si, a));
        }
    }
    for (const Place& key : want) {
        auto [si, a] = key;
        std::optional<int64_t> st = covered.get(key);
        if (st.has_value() && !insns.contains(key)) {
            if (sites.contains(key) && insns.contains(std::make_tuple(si, *st))) {
                placed.set(key, py::percent("s%d_S%x", si, a));   // (a place inside an instruction written as bytes)
                raw = raw | py::set<Place>{std::make_tuple(si, *st)};
            }
            placed.setdefault(std::make_tuple(si, *st), label(si, *st));
        }
    }
    _raw = raw;
    py::str name = py::os::path::basename(path);
    w::Assembly L(py::list<py::str>(), py::str("nasm"), name);   // (the file as one L2 tree, as it is written)
    py::dict<int64_t, Place> lines_of;
    L.append(py::percent("; %s -- assembled with NASM: nasm -f bin -o %s this-file", name, name));
    L.append(py::str("bits 16"));
    L.append(py::percent("SHIFT equ %d", shift));
    L.append(py::str("; where each segment lies in the file: its section's place, and its sector in the segment table"));
    for (const Seg& s : segs) {
        L.append(py::percent("seg%d_start_file equ 0x%x", s.idx, s.file_off));
    }
    L.extend(_PRELUDE);
    struct Region {
        int64_t lo, hi;
        const Seg* s;
    };
    std::vector<Region> regions;
    for (const Seg& s : segs) {
        regions.push_back({s.file_off, s.file_off + s.length + s.table, &s});
    }
    std::stable_sort(regions.begin(), regions.end(), [](const Region& x, const Region& y) { return x.lo < y.lo; });
    regions.push_back({py::len(d), py::len(d), nullptr});
    int64_t pos = 0, n = 0;
    std::optional<py::str> prev;
    int64_t segtab = ne.ne_off + ne.h.seg_tab;
    // "start=0x%x" % pos if exact or not prev else "follows=" + prev
    auto section_place = [&](const py::str& start_text) -> py::str {
        if (exact || !prev.has_value()) {
            return start_text;
        }
        return py::str("follows=") + *prev;
    };
    for (const Region& rg : regions) {
        int64_t lo = rg.lo, hi = rg.hi;
        const Seg* s = rg.s;
        if (lo > pos) {
            L.part(py::percent("part%d", n), py::str("outside the segments"));
            L.append(py::str(""));
            L.append(py::percent("section part%d %s vstart=0x%x align=1", n, section_place(py::percent("start=0x%x", pos)), pos));
            prev = py::percent("part%d", n);
            if (pos == 0 && stub.has_value() && py::truthy(*stub)) {
                L.append(py::percent("incbin \"%s\"", *stub));
                pos = py::os::path::getsize(*stub);
            }
            _data_plain(L, pos, lo, segtab);
            n += 1;
        }
        if (s == nullptr) {
            break;
        }
        {
            l2::Dict origin;
            origin.set(py::str("module"), l2::Val(name));
            origin.set(py::str("segment"), l2::Val(s->idx));
            origin.set(py::str("offset"), l2::Val(static_cast<int64_t>(0)));
            L.part(py::percent("seg%d", s->idx), py::str("segment"), origin, s->length);
        }
        L.append(py::str(""));
        // (while the rounds look for what NASM does not give back, a section that came out longer
        // must not run into the next: each follows the one before; the last round places them)
        py::str where;
        if (exact) {
            where = py::percent("start=seg%d_start_file", s->idx);
        } else {
            if (!prev.has_value()) {
                throw py::TypeError("can only concatenate str (not \"NoneType\") to str");
            }
            where = py::str("follows=") + *prev;
        }
        L.append(py::percent("section seg%d %s vstart=0 align=1", s->idx, where));
        prev = py::percent("seg%d", s->idx);
        L.append(py::percent("seg%d_start:", s->idx));
        _segment(L, *s, lines_of);
        pos = hi;
    }
    L.append(py::str(""));
    l2 = L.document();
    return std::make_tuple(w::text(L), lines_of);
}

void Writer::_data_plain(w::Assembly& L, int64_t a, int64_t end, int64_t segtab) const {
    int64_t n = ne.h.n_segs;
    py::dict<int64_t, int64_t> rows;
    for (int64_t k = 0; k < n; ++k) {
        rows.set(segtab + 8 * k, k + 1);
    }
    int64_t p = a;
    while (p < end) {
        std::optional<int64_t> idx = rows.get(p);
        if (idx.has_value() && has_seg(*idx) && p + 8 <= end) {
            const Seg& s = seg(*idx);
            L.append(py::percent("    dw seg%d_start_file >> SHIFT, seg%d_end%s", *idx, *idx, py::str(s.length < 0x10000 ? "" : " & 0xffff")));
            for (const py::str& x : assembly::data_lines(d.slice(p + 4, p + 8))) {
                L.append(py::str("    ") + x);
            }
            p += 8;
            continue;
        }
        int64_t nxt = end;
        for (int64_t r : rows.keys()) {
            if (r > p) {
                nxt = std::min(nxt, r);
            }
        }
        for (const py::str& x : assembly::data_lines(d.slice(p, nxt))) {
            L.append(py::str("    ") + x);
        }
        p = nxt;
    }
}

void Writer::_segment(w::Assembly& L, const Seg& s, py::dict<int64_t, Place>& lines_of) const {
    int64_t si = s.idx, base = s.file_off, length = s.length;
    int64_t a = 0;
    py::list<int64_t> mine;
    for (const Place& x : starts) {
        if (std::get<0>(x) == si && std::get<1>(x) < length) {
            mine.append(std::get<1>(x));
        }
    }
    mine.sort();
    py::dict<int64_t, int64_t> ends;   // (a function runs to the next one)
    for (int64_t k = 0; k < py::len(mine); ++k) {
        ends.set(mine[k], k + 1 < py::len(mine) ? mine[k + 1] : length);
    }
    py::str module = py::os::path::basename(path);
    while (a < length) {
        Place key = std::make_tuple(si, a);
        if (insns.contains(key)) {
            const Insn& i = insns.at(key);
            std::optional<int64_t> fe = ends.get(a);
            if (fe.has_value()) {   // (each function a fragment of its own)
                std::optional<py::str> pl = placed.get(key);
                l2::Dict origin;
                origin.set(py::str("module"), l2::Val(module));
                origin.set(py::str("segment"), l2::Val(si));
                origin.set(py::str("offset"), l2::Val(a));
                L.part(pl.has_value() && py::truthy(*pl) ? *pl : label(si, a), py::str("function"), origin, *fe - a);
            }
            if (placed.contains(key)) {
                L.append(py::percent("%s:", placed.at(key)));
            }
            for (const py::str& t : text(si, i, _raw.contains(key))) {
                lines_of.set(py::len(L) + 1, key);
                L.append(t.endswith(py::str(":")) ? t : py::str("    ") + t);
            }
            a += i.size;
            continue;
        }
        int64_t stop = a + 1;
        while (stop < length && !insns.contains(std::make_tuple(si, stop))) {
            stop += 1;
        }
        _data(L, si, base, a, stop);
        a = stop;
    }
    if (placed.contains(std::make_tuple(si, length))) {
        L.append(py::percent("%s:", placed.at(std::make_tuple(si, length))));
    }
    L.append(py::percent("seg%d_end:", si));
    if (s.table) {
        L.append(py::percent("    dw %d                                   ; relocations", py::len(s.relocs)));
        for (const ne::Relocation& r : s.relocs) {
            int64_t index = 0;   // (s["relocs"].index(r): the first record equal to it)
            for (int64_t q = 0; q < py::len(s.relocs); ++q) {
                if (same_reloc_(s.relocs[q], r)) {
                    index = q;
                    break;
                }
            }
            int64_t p = base + length + 2 + 8 * index;
            py::bytes rec = d.slice(p, p + 8);
            py::str head = py::percent("db 0x%02x,0x%02x", rec[0], rec[1]);
            py::str where = ref(si, r.offset);
            if (r.kind == py::str("INTERNAL") && *r.seg != 0xFF && has_seg(*r.seg)) {
                L.append(py::str("    ") + head);
                L.append(py::str("    dw ") + where);
                L.append(py::percent("    db 0x%02x,0x%02x", rec[4], rec[5]));
                L.append(py::str("    dw ") + ref(*r.seg, *r.entry));
            } else {
                L.append(py::str("    ") + head);
                L.append(py::str("    dw ") + where);
                L.append(py::str("    db ") + db_bytes_(rec, 4, 8));
            }
        }
    }
}

void Writer::_data(w::Assembly& L, int64_t si, int64_t base, int64_t a, int64_t end) const {
    py::set<int64_t> cut_set;
    for (const Place& k : placed.keys()) {
        if (std::get<0>(k) == si && a <= std::get<1>(k) && std::get<1>(k) < end) {
            cut_set.add(std::get<1>(k));
        }
    }
    for (const Place& k : sites.keys()) {
        if (std::get<0>(k) == si && a <= std::get<1>(k) && std::get<1>(k) < end) {
            cut_set.add(std::get<1>(k));
        }
    }
    py::list<int64_t> cuts = py::list<int64_t>::from(cut_set);
    cuts.sort();
    cuts.append(end);
    int64_t pos = a;
    for (int64_t c : cuts) {
        Place key = std::make_tuple(si, c);
        if (c < pos) {
            if (placed.contains(key)) {
                L.append(py::percent("%s equ $-%d", placed.at(key), pos - c));
            }
            continue;
        }
        if (c > pos) {
            for (const py::str& x : assembly::data_lines(d.slice(base + pos, base + c))) {
                L.append(py::str("    ") + x);
            }
            pos = c;
        }
        if (c == end) {
            break;
        }
        if (placed.contains(key)) {
            L.append(py::percent("%s:", placed.at(key)));
        }
        if (sites.contains(key)) {
            const Site& site = sites.at(key);
            if (c + site.w <= end) {
                if (site.chained) {
                    L.append(py::percent("    dw %s", site.nxt == 0xFFFF ? py::str("0xffff") : ref(si, site.nxt)));
                    if (site.w > 2) {
                        for (const py::str& x : assembly::data_lines(d.slice(base + c + 2, base + c + site.w))) {
                            L.append(py::str("    ") + x);
                        }
                    }
                } else {
                    for (const py::str& x : assembly::data_lines(d.slice(base + c, base + c + site.w))) {
                        L.append(py::str("    ") + x);
                    }
                }
                pos = c + site.w;
            }
        }
    }
}

py::set<Place> _differing(const Writer& wr, const py::bytes& data, const py::dict<py::str, std::tuple<int64_t, int64_t>>& syms,
                          const py::set<Place>& raw) {
    py::set<Place> bad;
    py::list<Place> order = py::list<Place>::from(wr.insns.keys());
    order.sort();
    int64_t count = py::len(order);
    for (int64_t n = 0; n < count; ++n) {
        Place key = order[n];
        if (raw.contains(key)) {
            continue;
        }
        auto [si, a] = key;
        const Insn& i = wr.insns.at(key);
        const Seg& s = wr.seg(si);
        std::optional<py::str> pl = wr.placed.get(key);
        std::optional<std::tuple<int64_t, int64_t>> hit = syms.get(pl.has_value() ? *pl : py::str(""));
        if (!hit.has_value()) {
            bad.add(key);
            continue;
        }
        auto [at, real] = *hit;
        if (n + 1 < count) {
            Place nxt = order[n + 1];
            if (std::get<0>(nxt) == si && std::get<1>(nxt) == a + i.size) {
                std::optional<py::str> npl = wr.placed.get(nxt);
                std::optional<std::tuple<int64_t, int64_t>> nh = syms.get(npl.has_value() ? *npl : py::str(""));
                if (nh.has_value() && std::get<0>(*nh) - at != i.size) {
                    bad.add(key);
                    continue;
                }
            }
        }
        int64_t o = real, ob = s.file_off + a;
        py::bytes mine = data.slice(o, o + i.size), orig = wr.d.slice(ob, ob + i.size);
        py::set<int64_t> skip;
        if (Writer::_target(i).has_value()) {
            for (int64_t k = i.imm_offset; k < i.imm_offset + i.imm_size; ++k) {
                skip.add(k);
            }
        }
        bool differs = py::len(mine) != py::len(orig);
        for (int64_t k = 0; !differs && k < py::len(orig); ++k) {
            if (!skip.contains(k) && mine[k] != orig[k]) {
                differs = true;
            }
        }
        if (differs) {
            bad.add(key);
        }
    }
    return bad;
}

w::FlatReport Result::report() const {
    w::FlatReport r;
    r.instructions = instructions;
    r.raw = raw;
    r.rounds = rounds;
    r.same = same;
    r.image = image;
    r.source = source;
    return r;
}

Result reconstruct(const py::str& path, const py::str& outdir_, const pe_writer::StubArg& stub_arg, const std::function<void(const py::str&)>& log) {
    std::function<void(const py::str&)> say = log ? log : std::function<void(const py::str&)>([](const py::str&) {});
    if (!py::truthy(w::nasm())) {
        throw py::SystemExit(py::str("no assembler (settings.json: nasm)"));
    }
    py::str outdir = py::os::path::abspath(outdir_);
    py::os::makedirs(outdir, 0777, true);
    Writer wr(path);
    py::str name = py::os::path::basename(path).lower();
    std::optional<py::str> stub;
    if (std::holds_alternative<std::monostate>(stub_arg)) {
        stub = pe_writer::_stub(wr.d, wr.ne.ne_off, outdir, say);
    } else if (std::holds_alternative<py::str>(stub_arg)) {
        stub = std::get<py::str>(stub_arg);
    }
    if (stub.has_value() && !py::truthy(*stub)) {
        stub = std::nullopt;
    }
    w::FlatResult<Place> fr = w::flat<Place>(
        wr.d,
        [&](const py::set<Place>& raw, bool final) {
            return final ? wr.source(raw, false, stub) : wr.source(raw, true, stub, false);
        },
        [&](const py::bytes& data, const py::dict<py::str, std::tuple<int64_t, int64_t>>& labels, const py::set<Place>& raw) {
            return _differing(wr, data, labels, raw | wr._raw);
        },
        outdir, name, say);
    Result r;
    r.source = fr.source;
    r.image = fr.image;
    r.same = fr.same;
    r.rounds = fr.rounds;
    r.log = fr.log;
    r.instructions = py::len(wr.insns);
    r.raw = py::len(fr.raw | wr._raw);
    return r;
}

int64_t main(const py::list<py::str>& argv) {
    std::vector<py::str> args;
    for (const py::str& a : argv) {
        if (!a.startswith(py::str("--"))) {
            args.push_back(a);
        }
    }
    if (argv.contains(py::str("--out"))) {
        py::str v = argv[argv.index(py::str("--out")) + 1];
        auto it = std::find(args.begin(), args.end(), v);
        if (it == args.end()) {
            throw py::ValueError("list.remove(x): x not in list");
        }
        args.erase(it);
    }
    if (args.empty()) {
        py::print(DOC);
        return 2;
    }
    py::str path = args[0];
    py::str outdir;
    if (argv.contains(py::str("--out"))) {
        outdir = argv[argv.index(py::str("--out")) + 1];
    } else {
        py::str base = std::get<0>(py::os::path::splitext(py::os::path::basename(path))).lower();
        outdir = py::os::path::join(py::settings::get_str(py::str("state")), py::str("ne_source"), base);
        py::os::makedirs(py::os::path::dirname(outdir), 0777, true);
    }
    pe_writer::StubArg stub = std::monostate();
    if (argv.contains(py::str("--keep-stub"))) {
        stub = false;
    }
    return w::flat_report(path, reconstruct(path, outdir, stub, [](const py::str& s) { py::print(s); }).report());
}

}  // namespace codebreaker::source::ne_writer
