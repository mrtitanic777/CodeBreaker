// codebreaker/source/le_writer.cpp -- typed C++ of codebreaker.source.le_writer.

#include "codebreaker/source/le_writer.hpp"

#include <algorithm>

#include "codebreaker/source/assembly.hpp"

namespace codebreaker::source::le_writer {

namespace w = codebreaker::source::image_writer;
namespace l2 = codebreaker::translate::l2;
using pe_writer::branch_target;
using pe_writer::decode;

const py::str DOC(R"DOC(le_writer.py -- a virtual device driver (LE, and the W3 bundle VMM32.VXD) as assembly source that builds back byte for byte.

    python -m codebreaker le-source <driver> [--out DIR]

    from codebreaker.source import le_writer
    r = le_writer.reconstruct(path, outdir)
    r["source"], r["image"], r["same"], r["instructions"], r["raw"], r["rounds"]

driver_writer.py writes a driver as the driver kit's macro source, built by wasm and wlink: that is the
driver as it was written, and it comes back the same in structure. This writes the FILE, the way
pe_writer and ne_writer do: one NASM source assembled flat (-f bin). The headers and tables are a
section of their own; each object is a section at its first page's place in the file (start=) whose
addresses are the object's offsets (vstart=0), so a label is what a fix-up names. Every fix-up record's
place is written as its label less the start of its page, every internal target as the label it names,
every 32-bit offset the file stores at a fix-up's place (the target, as LINK386 and the driver linker
leave it) as that label, and the fix-up page table as the distances of each page's records from the
first. An object whose pages are not in the file in order, or not all there, is written as its pages'
bytes in the order the file has them.

Code is what is reached from the entry point, the device descriptor's control procedure, the entry
table and every internal fix-up that names a place in an executable object; an instruction is a
mnemonic where NASM gives back its bytes (checked each round from the assembler's map), its bytes
otherwise, and always its bytes when a fix-up patches a place inside it (that place labelled).
A W3 bundle (VMM32.VXD) is its header and each of its drivers, each written as above, its labels named
by the driver.
)DOC");

const py::set<py::str> _STOP{py::str("ret"), py::str("retf"), py::str("jmp"), py::str("ljmp"), py::str("hlt"), py::str("iret"), py::str("iretd"), py::str("ud2")};
const py::set<int64_t> _SEGS{0x26, 0x2E, 0x36, 0x3E, 0x64, 0x65};
const py::dict<int64_t, int64_t> _WIDTH{{0, 1}, {2, 2}, {3, 4}, {5, 2}, {6, 6}, {7, 4}, {8, 4}};

namespace {

// _REG: name -> (encoding number, width marker): 32-bit 4, 16-bit 2, 8-bit 0
py::dict<py::str, std::tuple<int64_t, int64_t>> make_reg() {
    py::dict<py::str, std::tuple<int64_t, int64_t>> r;
    const char* g4[8] = {"eax", "ecx", "edx", "ebx", "esp", "ebp", "esi", "edi"};
    const char* g2[8] = {"ax", "cx", "dx", "bx", "sp", "bp", "si", "di"};
    const char* g0[8] = {"al", "cl", "dl", "bl", "ah", "ch", "dh", "bh"};
    for (int64_t k = 0; k < 8; ++k) {
        r.set(py::str(g4[k]), std::make_tuple(k, static_cast<int64_t>(4)));
    }
    for (int64_t k = 0; k < 8; ++k) {
        r.set(py::str(g2[k]), std::make_tuple(k, static_cast<int64_t>(2)));
    }
    for (int64_t k = 0; k < 8; ++k) {
        r.set(py::str(g0[k]), std::make_tuple(k, static_cast<int64_t>(0)));
    }
    return r;
}

const py::dict<py::str, std::tuple<int64_t, int64_t>>& reg_() {
    static const py::dict<py::str, std::tuple<int64_t, int64_t>> r = make_reg();
    return r;
}

py::list<py::str> make_prelude() {
    py::list<py::str> p;
    p.append(py::str(""));
    p.append(py::str("; op_ r, r: the register-to-register form with the direction bit set (opcode r, r/m), the one"));
    p.append(py::str("; Microsoft's tools write; NASM itself writes the other"));
    py::list<py::str> names;
    for (const py::str& n : reg_().keys()) {
        names.append(n);
    }
    names.sort();   // sorted(_REG.items())
    for (const py::str& n : names) {
        p.append(py::percent("%%assign _r_%s %d", n, std::get<0>(reg_().at(n))));
    }
    for (const py::str& n : names) {
        p.append(py::percent("%%assign _w_%s %d", n, std::get<1>(reg_().at(n))));
    }
    p.append(py::str("%macro _rr 3"));
    p.append(py::str("  %if (_w_%2 == 2 && __?BITS?__ == 32) || (_w_%2 == 4 && __?BITS?__ == 16)"));
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

// int.from_bytes(b, "little")
int64_t from_le_(const py::bytes& b) {
    int64_t v = 0;
    for (int64_t k = py::len(b) - 1; k >= 0; --k) {
        v = (v << 8) | b[k];
    }
    return v;
}

}  // namespace

const py::list<py::str> _PRELUDE = make_prelude();

std::optional<int64_t> _target(const Insn& i) {
    return branch_target(i);
}

// ---- _Driver --------------------------------------------------------------------------------------------

Driver::Driver(const py::bytes& data, const std::shared_ptr<const py::bytes>& shared, int64_t lfa, const py::str& t)
    : d(data), tag(t), lf(shared, lfa) {
    pm = le::page_map(lf);
    int64_t top = 0;
    for (const auto& [n, k] : pm) {
        (void)k;
        top = std::max(top, n);
    }
    last = top;
    for (const le::Object& o : lf.objects) {
        py::list<std::tuple<int64_t, int64_t>> pages = pm.slice(o.pmidx - 1, o.pmidx - 1 + o.pcnt);
        py::list<int64_t> nums;
        for (const auto& [n, k] : pages) {
            (void)k;
            nums.append(n);
        }
        bool whole = false;
        if (py::truthy(nums)) {
            bool all_ok = true;
            for (const auto& [n, k] : pages) {
                if (!(k == 0 && n)) {
                    all_ok = false;
                    break;
                }
            }
            bool consecutive = true;
            for (int64_t q = 0; q < py::len(nums); ++q) {
                if (nums[q] != nums[0] + q) {
                    consecutive = false;
                    break;
                }
            }
            whole = all_ok && consecutive;
        }
        Obj g;
        g.o = o;
        g.whole = whole;
        if (whole) {
            g.start = lf.datapages + (nums[0] - 1) * lf.pagesize;
            int64_t size = 0;
            for (int64_t n : nums) {
                size += _page_len(n);
            }
            g.size = size;
        }
        g.bits = (o.flags & 0x2000) ? 32 : 16;
        by_idx.set(o.idx, static_cast<int64_t>(objs.size()));
        objs.push_back(g);
    }
    // fix-ups: the place each patches, (object, offset) -> (record index, width, record)
    records = le::fixup_records(lf);
    for (int64_t k = 0; k < py::len(records); ++k) {
        const auto& [pg, rec] = records[k];
        auto [ob, po] = lf.page_to_obj(pg);
        if (!ob.has_value()) {
            continue;
        }
        for (int64_t s : rec.sources) {
            sites.setdefault(std::make_tuple(ob->idx, po + s), Site{k, _WIDTH.get(rec.source, 4)});
        }
    }
    _decode();
}

int64_t Driver::_page_len(int64_t n) const {
    return n == last && lf.last_page_bytes ? lf.last_page_bytes : lf.pagesize;
}

py::str Driver::name(int64_t oi, int64_t a) const {
    return py::percent(insns.contains(std::make_tuple(oi, a)) ? "%so%d_L%x" : "%so%d_D%x", tag, oi, a);
}

void Driver::_decode() {
    py::list<Place> seeds;
    if (has_obj(lf.eip_obj)) {
        seeds.append(std::make_tuple(lf.eip_obj, lf.eip));
    }
    try {
        for (const auto& [_ordinal, ob, off, _flags] : le::entries(lf)) {
            if (has_obj(ob)) {
                seeds.append(std::make_tuple(ob, off));
            }
        }
    } catch (const std::exception&) {
    }
    try {
        std::optional<le::Descriptor> ddb = le::descriptor(lf);
        if (ddb.has_value() && ddb->control.has_value()) {
            seeds.append(*ddb->control);
        }
    } catch (const std::exception&) {
    }
    py::list<Place> entries = py::list<Place>::from(seeds);   // (where the system enters: each a function)
    for (const auto& [key, site] : sites.items()) {
        const le::FixupRecord& rec = std::get<1>(records[site.k]);
        if (rec.kind == py::str("internal") && rec.target.has_value() && has_obj(*rec.obj)) {
            seeds.append(std::make_tuple(*rec.obj, *rec.target));
        }
    }
    py::dict<Place, int64_t> sitebytes;
    for (const auto& [key, site] : sites.items()) {
        for (int64_t j = 0; j < site.w; ++j) {
            sitebytes.set(std::make_tuple(std::get<0>(key), std::get<1>(key) + j), std::get<1>(key));
        }
    }
    py::list<Place> work;
    for (const Place& s : seeds) {
        if (has_obj(std::get<0>(s)) && obj(std::get<0>(s)).o.exec && obj(std::get<0>(s)).whole) {
            work.append(s);
        }
    }
    while (py::truthy(work)) {
        auto [oi, a] = work.pop();
        const Obj& o = obj(oi);
        int64_t limit = std::min(o.o.vsize, o.size);
        while (0 <= a && a < limit && !insns.contains(std::make_tuple(oi, a)) && !covered.contains(std::make_tuple(oi, a))) {
            int64_t off = *o.start + a;
            std::optional<Insn> one = decode(o.bits == 32 ? 32 : 16, d.slice(off, *o.start + std::min(a + 15, limit)), a);
            if (!one.has_value()) {
                break;
            }
            bool overlap = false;
            for (int64_t k = 0; k < one->size; ++k) {
                if (covered.contains(std::make_tuple(oi, a + k))) {
                    overlap = true;
                    break;
                }
            }
            if (overlap) {
                break;
            }
            py::set<int64_t> starts_in;
            for (int64_t k = 0; k < one->size; ++k) {
                std::optional<int64_t> sb = sitebytes.get(std::make_tuple(oi, a + k));
                if (sb.has_value()) {
                    starts_in.add(*sb);
                }
            }
            bool cut = false;
            for (int64_t st : starts_in) {
                if (st < a || st + sites.at(std::make_tuple(oi, st)).w > a + one->size) {
                    cut = true;
                    break;
                }
            }
            if (cut) {
                break;
            }
            insns.set(std::make_tuple(oi, a), *one);
            for (int64_t k = 0; k < one->size; ++k) {
                covered.set(std::make_tuple(oi, a + k), a);
            }
            std::optional<int64_t> t = _target(*one);
            if (t.has_value() && !py::truthy(starts_in)) {
                work.append(std::make_tuple(oi, *t));
            }
            if (_STOP.contains(one->mnemonic)) {
                break;
            }
            a += one->size;
        }
    }
    starts = _function_starts(entries);
}

py::set<Place> Driver::_function_starts(const py::list<Place>& entries) const {
    py::set<Place> out = py::set<Place>::from(entries);
    try {   // (and the services, the API procedures)
        for (const Place& x : le::entry_points(lf).values()) {
            out.add(x);
        }
    } catch (const std::exception&) {
    }
    for (const auto& [key, i] : insns.items()) {
        std::optional<int64_t> t = _target(i);
        if (i.mnemonic == py::str("call") && t.has_value()) {
            out.add(std::make_tuple(std::get<0>(key), *t));
        }
    }
    for (const auto& [key, site] : sites.items()) {
        auto [oi, at] = key;
        std::optional<int64_t> st = covered.get(key);
        const le::FixupRecord& rec = std::get<1>(records[site.k]);
        if (rec.kind == py::str("internal") && rec.target.has_value() && st.has_value() && *st == at - 1 &&
            insns.at(std::make_tuple(oi, *st)).mnemonic == py::str("call")) {
            out.add(std::make_tuple(*rec.obj, *rec.target));
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

std::optional<py::str> _nasm(const Insn& i, int64_t bits, const std::function<py::str(int64_t)>& ref) {
    static const py::re::Pattern string_ops = py::re::compile(assembly::STRING_OPS);
    static const py::re::Pattern string_ops2 = py::re::compile(py::str("^(rep |repe |repne )?(movs|cmps|stos|lods|scas|ins|outs)[bwdq]$"));
    static const py::re::Pattern rr_ops = py::re::compile(py::str("^(\\w+), (\\w+)$"));
    static const py::re::Pattern st_reg = py::re::compile(py::str("\\bst\\((\\d)\\)"));
    static const py::re::Pattern seg_br = py::re::compile(py::str("\\b([cdefgs]s):\\["));
    static const py::re::Pattern nosplit = py::re::compile(py::str("\\[(\\w\\w:)?(e\\w\\w\\*[248])"));
    static const py::re::Pattern bracket_in = py::re::compile(py::str("\\[([^\\]]*)\\]"));
    static const py::re::Pattern imm_tail = py::re::compile(py::str("(-?0x[0-9a-f]+|-?\\d+)$"));
    static const py::re::Pattern base_idx = py::re::compile(py::str("[a-z]{2,3}\\s*[+-]"));
    const py::str& m = i.mnemonic;
    py::str ops = i.op_str;
    if (m == py::str("ljmp") || m == py::str("lcall")) {
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
    std::optional<int64_t> tgt = _target(i);
    if (tgt.has_value()) {
        int64_t near = bits == 32 ? 4 : 2;
        if (m == py::str("call")) {
            if (i.imm_size == near && i.size == 1 + near) {
                return py::percent("call %s", ref(*tgt));
            }
            return std::nullopt;
        }
        if (m == py::str("loop") || m == py::str("loope") || m == py::str("loopne") || m == py::str("jcxz") || m == py::str("jecxz")) {
            if (i.size == 2) {
                return py::percent("%s %s", m, ref(*tgt));
            }
            return std::nullopt;
        }
        if ((i.imm_size != 1 && i.imm_size != near) || (i.imm_size == 1 && i.size != 2)) {
            return std::nullopt;
        }
        return py::percent("%s %s %s", m, py::str(i.imm_size == 1 ? "short" : "near"), ref(*tgt));
    }
    std::optional<py::re::Match> rr = rr_ops.match(ops);
    if (pe_writer::RR().contains(m) && rr.has_value() && reg_().contains(rr->group(1)) && reg_().contains(rr->group(2))) {
        int64_t op = i.bytes[i.bytes[0] == 0x66 ? 1 : 0];
        if (op & 2) {
            return py::percent("%s_ %s", m, ops);
        }
        return py::percent("%s %s", m, ops);
    }
    ops = ops.replace(py::str(" ptr "), py::str(" ")).replace(py::str("xmmword "), py::str("oword ")).replace(py::str("xword "), py::str("tword "));
    ops = st_reg.sub(py::str("st\\1"), ops);
    ops = seg_br.sub(py::str("[\\1:"), ops);
    if (bits == 32) {
        ops = nosplit.sub(py::str("[\\1nosplit \\2"), ops);
    }
    if ((i.imm_size == 2 || i.imm_size == 4) && pe_writer::_ARITH.contains(m)) {
        std::optional<py::re::Match> mm = imm_tail.search(ops);
        if (mm.has_value()) {
            int64_t v = py::int_parse(mm->group(1), 0);
            if ((-128 <= v && v <= 127) || v >= (i.imm_size == 2 ? 0xFF80 : 0xFFFFFF80)) {
                ops = ops.slice(0, mm->start()) + py::str(i.imm_size == 2 ? "strict word " : "strict dword ") + mm->group(1);
            }
        }
    }
    if (i.disp_size == 2 || i.disp_size == 4) {
        std::optional<py::re::Match> br = bracket_in.search(ops);
        if (br.has_value() && base_idx.search(br->group(1)).has_value()) {
            ops = ops.slice(0, br->start()) + py::percent("[%s ", py::str(i.disp_size == 4 ? "dword" : "word")) + br->group(1) + py::str("]") +
                  ops.slice_from(br->end());
        }
    }
    return py::percent("%s %s", m, ops).strip();
}

// ---- Writer ---------------------------------------------------------------------------------------------

Writer::Writer(const py::str& p)
    : path(p),
      data_([&]() {
          py::file f = py::open_rb(p);
          std::shared_ptr<const py::bytes> out = std::make_shared<py::bytes>(f.read());
          f.close();
          return out;
      }()),
      d(*data_) {
    int64_t lfa = py::unpack_from1<uint32_t>("<I", d, 0x3C);
    w3 = d.slice(lfa, lfa + 2) == py::bytes("W3");
    if (w3) {
        le::W3File bundle(d, lfa);
        for (int64_t k = 0; k < py::len(bundle.entries); ++k) {
            const le::W3Entry& e = bundle.entries[k];
            py::bytes sig = d.slice(e.off, e.off + 2);
            if (sig == py::bytes("LE") || sig == py::bytes("LX")) {
                drivers.push_back(std::make_unique<Driver>(d, data_, e.off, py::percent("m%d_", k)));
            }
        }
    } else {
        drivers.push_back(std::make_unique<Driver>(d, data_, lfa, py::str("")));
    }
}

int64_t Writer::insn_count() const {
    int64_t n = 0;
    for (const auto& dr : drivers) {
        n += py::len(dr->insns);
    }
    return n;
}

void Writer::_place(Driver& dr, const py::set<Key>& raw) {
    (void)raw;
    py::set<Place> want;
    for (const auto& [key, site] : dr.sites.items()) {
        const le::FixupRecord& rec = std::get<1>(dr.records[site.k]);
        want.add(key);
        if (rec.kind == py::str("internal") && rec.target.has_value()) {
            want.add(std::make_tuple(*rec.obj, *rec.target));
        }
    }
    for (const auto& [key, i] : dr.insns.items()) {
        std::optional<int64_t> t = _target(i);
        if (t.has_value()) {
            want.add(std::make_tuple(std::get<0>(key), *t));
        }
    }
    if (_mark_all) {
        for (const Place& k : dr.insns.keys()) {
            want.add(k);
        }
    }
    dr.placed = py::dict<Place, py::str>();
    for (const Place& key : want) {
        auto [oi, a] = key;
        if (!dr.has_obj(oi)) {
            continue;
        }
        const Obj& o = dr.obj(oi);
        if (!o.whole || a > o.size) {
            continue;
        }
        if (dr.insns.contains(key) || !dr.covered.contains(key)) {
            dr.placed.set(key, dr.name(oi, a));
        }
    }
    for (const Place& key : want) {
        auto [oi, a] = key;
        std::optional<int64_t> st = dr.covered.get(key);
        if (st.has_value() && !dr.insns.contains(key) && dr.sites.contains(key)) {
            dr.placed.set(key, py::percent("%so%d_S%x", dr.tag, oi, a));
            dr.placed.setdefault(std::make_tuple(oi, *st), dr.name(oi, *st));
        }
    }
}

py::str Writer::ref(const Driver& dr, int64_t oi, int64_t a) const {
    std::optional<py::str> pl = dr.placed.get(std::make_tuple(oi, a));
    if (pl.has_value()) {
        return *pl;
    }
    std::optional<int64_t> st = dr.covered.get(std::make_tuple(oi, a));
    if (st.has_value() && dr.placed.contains(std::make_tuple(oi, *st))) {
        return py::percent("%s+%d", dr.placed.at(std::make_tuple(oi, *st)), a - *st);
    }
    if (dr.has_obj(oi) && dr.obj(oi).whole) {
        return py::percent("%so%d_start+0x%x", dr.tag, oi, a);
    }
    return py::percent("0x%x", a);
}

std::tuple<py::str, py::dict<int64_t, Key>> Writer::source(const py::set<Key>& raw, bool mark_all, bool exact) {
    _mark_all = mark_all;
    for (const auto& dr : drivers) {
        _place(*dr, raw);
    }
    struct Region {
        int64_t lo, hi;
        int kind;   // 0 obj, 1 fixrec, 2 fixpage
        const Driver* dr;
        const Obj* obj;
        py::list<int64_t> offs;
    };
    std::vector<Region> regions;   // (file offset, end, kind, driver, what)
    for (const auto& dr : drivers) {
        for (const Obj& o : dr->objs) {
            if (o.whole && o.size) {
                regions.push_back({*o.start, *o.start + o.size, 0, dr.get(), &o, py::list<int64_t>()});
            }
        }
        const le::LEFile& lf = dr->lf;
        py::list<int64_t> offs;
        for (int64_t i = 0; i < lf.mpages + 1; ++i) {
            offs.append(py::unpack_from1<uint32_t>("<I", d, lf.fixup_page + 4 * i));
        }
        if (offs[-1]) {
            regions.push_back({lf.fixup_rec, lf.fixup_rec + offs[-1], 1, dr.get(), nullptr, offs});
            regions.push_back({lf.fixup_page, lf.fixup_page + 4 * (lf.mpages + 1), 2, dr.get(), nullptr, offs});
        }
    }
    std::stable_sort(regions.begin(), regions.end(), [](const Region& x, const Region& y) { return x.lo < y.lo; });
    py::str name = py::os::path::basename(path);
    w::Assembly L(py::list<py::str>(), py::str("nasm"), name);   // (the file as one L2 tree, as it is written)
    py::dict<int64_t, Key> lines_of;
    L.append(py::percent("; %s -- assembled with NASM: nasm -f bin -o %s this-file", name, name));
    L.extend(_PRELUDE);
    for (const auto& dr : drivers) {
        for (const Obj& o : dr->objs) {
            if (o.whole && o.size) {
                L.append(py::percent("%so%d_file equ 0x%x", dr->tag, o.o.idx, *o.start));
            }
        }
    }
    int64_t pos = 0, n = 0;
    std::optional<py::str> prev;
    regions.push_back({py::len(d), py::len(d), -1, nullptr, nullptr, py::list<int64_t>()});
    for (const Region& rg : regions) {
        int64_t lo = rg.lo, hi = rg.hi;
        if (lo < pos) {   // (overlapping tables: written with what they overlap)
            continue;
        }
        if (lo > pos) {
            L.part(py::percent("part%d", n), py::str("outside the objects"), std::nullopt, lo - pos);
            L.append(py::str(""));
            L.append(py::percent("section part%d %s vstart=0x%x align=1", n,
                                 exact || !prev.has_value() ? py::percent("start=0x%x", pos) : py::str("follows=") + *prev, pos));
            prev = py::percent("part%d", n);
            for (const py::str& x : assembly::data_lines(d.slice(pos, lo))) {
                L.append(py::str("    ") + x);
            }
            n += 1;
        }
        if (rg.kind < 0) {
            break;
        }
        const Driver& dr = *rg.dr;
        if (rg.kind == 0) {
            l2::Dict origin;
            origin.set(py::str("module"), l2::Val(name));
            origin.set(py::str("segment"), l2::Val(rg.obj->o.idx));
            origin.set(py::str("offset"), l2::Val(static_cast<int64_t>(0)));
            L.part(py::percent("%so%d", dr.tag, rg.obj->o.idx), py::str("object"), origin, rg.obj->size);
        } else {
            L.part(dr.tag + py::str(rg.kind == 1 ? "fixrec" : "fixpage"), py::str("fixups"), std::nullopt, hi - lo);
        }
        L.append(py::str(""));
        if (rg.kind == 0) {
            const Obj& o = *rg.obj;
            py::str sec = py::percent("%so%d", dr.tag, o.o.idx);
            L.append(py::percent("section %s %s vstart=0 align=1", sec,
                                 exact || !prev.has_value() ? py::percent("start=%s_file", sec) : py::str("follows=") + *prev));
            prev = sec;
            L.append(py::percent("bits %d", o.bits));
            L.append(py::percent("%s_start:", sec));
            _object(L, dr, o, raw, lines_of);
        } else if (rg.kind == 1) {
            py::str sec = dr.tag + py::str("fixrec");
            L.append(py::percent("section %s %s vstart=0 align=1", sec,
                                 exact || !prev.has_value() ? py::percent("start=0x%x", lo) : py::str("follows=") + *prev));
            prev = sec;
            _fixrec(L, dr, rg.offs);
        } else {
            py::str sec = dr.tag + py::str("fixpage");
            L.append(py::percent("section %s %s vstart=0 align=1", sec,
                                 exact || !prev.has_value() ? py::percent("start=0x%x", lo) : py::str("follows=") + *prev));
            prev = sec;
            for (int64_t k = 0; k < py::len(rg.offs); ++k) {
                L.append(py::percent("    dd %sfr_p%d-%sfr_p0", dr.tag, k, dr.tag));
            }
        }
        pos = hi;
    }
    L.append(py::str(""));
    l2 = L.document();
    return std::make_tuple(w::text(L), lines_of);
}

void Writer::_object(w::Assembly& L, const Driver& dr, const Obj& o, const py::set<Key>& raw, py::dict<int64_t, Key>& lines_of) const {
    int64_t oi = o.o.idx, base = *o.start, size = o.size;
    int64_t limit = size;
    int64_t a = 0;
    py::list<int64_t> mine;
    for (const Place& x : dr.starts) {
        if (std::get<0>(x) == oi && std::get<1>(x) < size) {
            mine.append(std::get<1>(x));
        }
    }
    mine.sort();
    py::dict<int64_t, int64_t> ends;   // (a function runs to the next one)
    for (int64_t k = 0; k < py::len(mine); ++k) {
        ends.set(mine[k], k + 1 < py::len(mine) ? mine[k + 1] : size);
    }
    py::str module = py::os::path::basename(path);
    while (a < limit) {
        Place key = std::make_tuple(oi, a);
        if (dr.insns.contains(key)) {
            const Insn& i = dr.insns.at(key);
            std::optional<int64_t> fe = ends.get(a);
            if (fe.has_value()) {   // (each function a fragment of its own)
                std::optional<py::str> pl = dr.placed.get(key);
                l2::Dict origin;
                origin.set(py::str("module"), l2::Val(module));
                origin.set(py::str("segment"), l2::Val(oi));
                origin.set(py::str("offset"), l2::Val(a));
                L.part(pl.has_value() && py::truthy(*pl) ? *pl : dr.name(oi, a), py::str("function"), origin, *fe - a);
            }
            if (dr.placed.contains(key)) {
                L.append(py::percent("%s:", dr.placed.at(key)));
            }
            for (const py::str& t : _text(dr, oi, i, raw.contains(std::make_tuple(dr.tag, oi, a)), o.bits)) {
                lines_of.set(py::len(L) + 1, std::make_tuple(dr.tag, oi, a));
                L.append(t.endswith(py::str(":")) ? t : py::str("    ") + t);
            }
            a += i.size;
            continue;
        }
        int64_t stop = a + 1;
        while (stop < limit && !dr.insns.contains(std::make_tuple(oi, stop))) {
            stop += 1;
        }
        _data(L, dr, oi, base, a, stop);
        a = stop;
    }
    if (dr.placed.contains(std::make_tuple(oi, size))) {
        L.append(py::percent("%s:", dr.placed.at(std::make_tuple(oi, size))));
    }
    py::list<int64_t> over;
    for (const Place& x : dr.placed.keys()) {
        if (std::get<0>(x) == oi && std::get<1>(x) > size) {
            over.append(std::get<1>(x));
        }
    }
    over.sort();
    for (int64_t v : over) {
        L.append(py::percent("%s equ $+0x%x", dr.placed.at(std::make_tuple(oi, v)), v - size));
    }
}

py::str Writer::_site_value(const Driver& dr, int64_t oi, int64_t at, const le::FixupRecord& rec, int64_t wd, const py::bytes& b) const {
    (void)oi;
    (void)at;
    if (rec.kind == py::str("internal") && rec.source == 7 && wd == 4 && rec.target.has_value()) {
        if (py::unpack_from1<uint32_t>("<I", b) == *rec.target && dr.has_obj(*rec.obj)) {
            return py::str("dd ") + ref(dr, *rec.obj, *rec.target);
        }
    }
    return py::str("db ") + db_bytes_(b, 0, py::len(b));
}

py::list<py::str> Writer::_text(const Driver& dr, int64_t oi, const Insn& i, bool raw, int64_t bits) const {
    int64_t a = i.address;
    const py::bytes& b = i.bytes;
    std::optional<int64_t> tgt = _target(i);
    py::list<int64_t> inside;
    for (int64_t k = 0; k < i.size; ++k) {
        if (dr.sites.contains(std::make_tuple(oi, a + k))) {
            inside.append(k);
        }
    }
    if (!raw && !py::truthy(inside)) {
        std::optional<py::str> t = _nasm(i, bits, [&](int64_t v) { return ref(dr, oi, v); });
        if (t.has_value() && py::truthy(*t)) {
            return py::list<py::str>{*t};
        }
    }
    py::list<py::str> out;
    int64_t k = 0;
    int64_t n = py::len(b);
    while (k < n) {
        if (inside.contains(k)) {
            const Site& site = dr.sites.at(std::make_tuple(oi, a + k));
            const le::FixupRecord& rec = std::get<1>(dr.records[site.k]);
            if (k) {
                out.append(py::percent("%s:", dr.placed.at(std::make_tuple(oi, a + k))));
            }
            out.append(_site_value(dr, oi, a + k, rec, site.w, b.slice(k, k + site.w)));
            k += site.w;
        } else if (tgt.has_value() && k == i.imm_offset) {
            if (i.imm_size != 4 && i.imm_size != 2 && i.imm_size != 1) {
                throw py::KeyError(py::to_str(i.imm_size).utf8());
            }
            const char* wd = i.imm_size == 4 ? "dd" : i.imm_size == 2 ? "dw" : "db";
            out.append(py::percent("%s %s-($+%d)", py::str(wd), ref(dr, oi, *tgt), i.imm_size));
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

void Writer::_data(w::Assembly& L, const Driver& dr, int64_t oi, int64_t base, int64_t a, int64_t end) const {
    py::set<int64_t> cut_set;
    for (const Place& k : dr.placed.keys()) {
        if (std::get<0>(k) == oi && a <= std::get<1>(k) && std::get<1>(k) < end) {
            cut_set.add(std::get<1>(k));
        }
    }
    for (const Place& k : dr.sites.keys()) {
        if (std::get<0>(k) == oi && a <= std::get<1>(k) && std::get<1>(k) < end) {
            cut_set.add(std::get<1>(k));
        }
    }
    py::list<int64_t> cuts = py::list<int64_t>::from(cut_set);
    cuts.sort();
    cuts.append(end);
    int64_t pos = a;
    for (int64_t c : cuts) {
        Place key = std::make_tuple(oi, c);
        if (c < pos) {
            if (dr.placed.contains(key)) {
                L.append(py::percent("%s equ $-%d", dr.placed.at(key), pos - c));
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
        if (dr.placed.contains(key)) {
            L.append(py::percent("%s:", dr.placed.at(key)));
        }
        if (dr.sites.contains(key)) {
            const Site& site = dr.sites.at(key);
            const le::FixupRecord& rec = std::get<1>(dr.records[site.k]);
            if (c + site.w <= end) {
                L.append(py::str("    ") + _site_value(dr, oi, c, rec, site.w, d.slice(base + c, base + c + site.w)));
                pos = c + site.w;
            }
        }
    }
}

void Writer::_fixrec(w::Assembly& L, const Driver& dr, const py::list<int64_t>& offs) const {
    (void)offs;
    const le::LEFile& lf = dr.lf;
    py::dict<int64_t, py::list<int64_t>> by_page;   // page -> indices of its records
    for (int64_t k = 0; k < py::len(dr.records); ++k) {
        by_page.setdefault(std::get<0>(dr.records[k]), py::list<int64_t>()).append(k);
    }
    for (int64_t pg = 0; pg < lf.mpages + 1; ++pg) {
        L.append(py::percent("%sfr_p%d:", dr.tag, pg));
        if (pg == lf.mpages) {
            break;
        }
        auto [ob, po] = lf.page_to_obj(pg + 1);
        const Obj* obj = ob.has_value() ? &dr.obj(ob->idx) : nullptr;   // dr.by_idx.get(obj["idx"])
        std::optional<py::list<int64_t>> idxs = by_page.get(pg + 1);
        if (!idxs.has_value()) {
            continue;
        }
        for (int64_t ri : *idxs) {
            const le::FixupRecord& rec = std::get<1>(dr.records[ri]);
            const py::bytes& raw = rec.raw;
            int64_t src = raw[0], fl = raw[1];
            bool many = (src & 0x20) != 0;
            int64_t p = 2;
            py::list<py::str> parts{py::percent("db 0x%02x,0x%02x", src, fl)};
            auto source_text = [&](int64_t s) -> py::str {
                std::optional<py::str> lab;
                if (obj) {
                    lab = dr.placed.get(std::make_tuple(obj->o.idx, po + s));
                }
                if (lab.has_value() && py::truthy(*lab) && obj->whole) {
                    return py::percent("dw %s-0x%x", *lab, po);
                }
                return py::percent("dw 0x%04x", s & 0xFFFF);
            };
            if (many) {
                parts.append(py::percent("db %d", raw[2]));
                p = 3;
            } else {
                int64_t s = rec.sources[0];
                parts.append(source_text(s));
                p = 4;
            }
            int64_t wide = (fl & 0x40) ? 2 : 1;
            parts.append(py::percent(wide == 2 ? "dw 0x%04x" : "db 0x%02x", from_le_(raw.slice(p, p + wide))));
            p += wide;
            py::bytes rest = raw.slice(p, py::len(raw));
            if (rec.kind == py::str("internal") && rec.target.has_value() && dr.has_obj(*rec.obj) && dr.obj(*rec.obj).whole) {
                int64_t n = (fl & 0x10) ? 4 : 2;
                parts.append(py::percent(n == 4 ? "dd %s" : "dw %s", ref(dr, *rec.obj, *rec.target)));
                rest = rest.slice(n, py::len(rest));
            }
            if (many) {
                int64_t ns = py::len(rec.sources);
                py::bytes mid = rest.slice(0, -2 * ns);
                if (py::len(mid) > 0) {
                    parts.append(py::str("db ") + db_bytes_(mid, 0, py::len(mid)));
                }
                for (int64_t s : rec.sources) {
                    parts.append(source_text(s));
                }
            } else if (py::len(rest) > 0) {
                parts.append(py::str("db ") + db_bytes_(rest, 0, py::len(rest)));
            }
            for (const py::str& x : parts) {
                L.append(py::str("    ") + x);
            }
        }
    }
}

py::set<Key> _differing(const Writer& wr, const py::bytes& data, const py::dict<py::str, std::tuple<int64_t, int64_t>>& syms, const py::set<Key>& raw) {
    py::set<Key> bad;
    for (const auto& drp : wr.drivers) {
        const Driver& dr = *drp;
        py::list<Place> order = py::list<Place>::from(dr.insns.keys());
        order.sort();
        int64_t count = py::len(order);
        for (int64_t n = 0; n < count; ++n) {
            Place key = order[n];
            Key tk = std::make_tuple(dr.tag, std::get<0>(key), std::get<1>(key));
            if (raw.contains(tk)) {
                continue;
            }
            auto [oi, a] = key;
            const Insn& i = dr.insns.at(key);
            const Obj& o = dr.obj(oi);
            std::optional<py::str> pl = dr.placed.get(key);
            std::optional<std::tuple<int64_t, int64_t>> hit = syms.get(pl.has_value() ? *pl : py::str(""));
            if (!hit.has_value()) {
                bad.add(tk);
                continue;
            }
            auto [at, real] = *hit;
            if (n + 1 < count) {
                Place nxt = order[n + 1];
                if (std::get<0>(nxt) == oi && std::get<1>(nxt) == a + i.size) {
                    std::optional<py::str> npl = dr.placed.get(nxt);
                    if (npl.has_value() && syms.contains(*npl) && std::get<0>(syms.at(*npl)) - at != i.size) {
                        bad.add(tk);
                        continue;
                    }
                }
            }
            py::bytes mine = data.slice(real, real + i.size), orig = wr.d.slice(*o.start + a, *o.start + a + i.size);
            py::set<int64_t> skip;
            if (_target(i).has_value()) {
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
                bad.add(tk);
            }
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

Result reconstruct(const py::str& path, const py::str& outdir_, const std::function<void(const py::str&)>& log) {
    std::function<void(const py::str&)> say = log ? log : std::function<void(const py::str&)>([](const py::str&) {});
    if (!py::truthy(w::nasm())) {
        throw py::SystemExit(py::str("no assembler (settings.json: nasm)"));
    }
    py::str outdir = py::os::path::abspath(outdir_);
    py::os::makedirs(outdir, 0777, true);
    Writer wr(path);
    py::str name = py::os::path::basename(path).lower();
    w::FlatResult<Key> fr = w::flat<Key>(
        wr.d,
        [&](const py::set<Key>& raw, bool final) { return wr.source(raw, !final, final); },
        [&](const py::bytes& data, const py::dict<py::str, std::tuple<int64_t, int64_t>>& labels, const py::set<Key>& raw) {
            return _differing(wr, data, labels, raw);
        },
        outdir, name, say, true);
    Result r;
    r.source = fr.source;
    r.image = fr.image;
    r.same = fr.same;
    r.rounds = fr.rounds;
    r.log = fr.log;
    r.instructions = wr.insn_count();
    r.raw = py::len(fr.raw);
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
        outdir = py::os::path::join(py::settings::get_str(py::str("state")), py::str("le_source"), base);
        py::os::makedirs(py::os::path::dirname(outdir), 0777, true);
    }
    return w::flat_report(path, reconstruct(path, outdir, [](const py::str& s) { py::print(s); }).report());
}

}  // namespace codebreaker::source::le_writer
