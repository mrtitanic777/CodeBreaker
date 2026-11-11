// codebreaker/source/vxd/link.cpp -- typed C++ of codebreaker.source.vxd.link (see link.hpp).

#include "codebreaker/source/vxd/link.hpp"

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "codebreaker/build/dos_tools.hpp"
#include "codebreaker/source/vxd/idiom.hpp"
#include "codebreaker/source/vxd/le_writer.hpp"
#include "codebreaker/source/vxd/masm.hpp"
#include "py_stdlib.hpp"
#include "py_subprocess.hpp"

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#endif

namespace codebreaker::source::vxd::link {

namespace dis = codebreaker::source::vxd::disassembly;
namespace vxd_masm = codebreaker::source::vxd::masm;
namespace vxd_idiom = codebreaker::source::vxd::idiom;
namespace vxd_le_writer = codebreaker::source::vxd::le_writer;

using dis::NONE_TARGET;
using dis::Place;
using dis::PlaceHash;
using dis::PlaceSet;
using dis::Row;

// MS LINK 2.50 (= the VC++ 2.0 linker). Retained only for the legacy byte-exact object experiments (build_and_compare /
// build_functional). It is the WRONG linker for a loadable VxD: its /VXD leaves the device id (LE 0xc0) blank and permutes the
// page map -- the exact defects the DDK shipped LINK 2.60 to fix. The from-source loadable path uses link_vxd_260().
const py::str _LINK = py::str("toolchains/period/compilers/msvc-2.0/BIN/LINK.EXE");

const py::list<py::str> _LINK260_FILES = {py::str("LINK.EXE"), py::str("DBI.DLL"), py::str("LINK.ERR"), py::str("IMAGEHLP.DLL")};

// standard DDK VxD segment map (verbatim from BASE/SAMPLES/*/*.DEF -- this is what boots VMD from source); LINK ignores
// segments absent from the object, so one template serves every VxD.
const py::str _VXD_DEF_SEGS = py::str(
    "    _LPTEXT     CLASS 'LCODE'   PRELOAD NONDISCARDABLE\r\n"
    "    _LTEXT      CLASS 'LCODE'   PRELOAD NONDISCARDABLE\r\n"
    "    _LDATA      CLASS 'LCODE'   PRELOAD NONDISCARDABLE\r\n"
    "    _TEXT       CLASS 'LCODE'   PRELOAD NONDISCARDABLE\r\n"
    "    _DATA       CLASS 'LCODE'   PRELOAD NONDISCARDABLE\r\n"
    "    CONST       CLASS 'LCODE'   PRELOAD NONDISCARDABLE\r\n"
    "    _TLS        CLASS 'LCODE'   PRELOAD NONDISCARDABLE\r\n"
    "    _BSS        CLASS 'LCODE'   PRELOAD NONDISCARDABLE\r\n"
    "    _ITEXT      CLASS 'ICODE'   DISCARDABLE\r\n"
    "    _IDATA      CLASS 'ICODE'   DISCARDABLE\r\n"
    "    _PTEXT      CLASS 'PCODE'   NONDISCARDABLE\r\n"
    "    _PDATA      CLASS 'PDATA'   NONDISCARDABLE SHARED\r\n"
    "    _STEXT      CLASS 'SCODE'   RESIDENT\r\n"
    "    _SDATA      CLASS 'SCODE'   RESIDENT\r\n"
    "    _16ICODE    CLASS '16ICODE' PRELOAD DISCARDABLE\r\n"
    "    _RCODE      CLASS 'RCODE'\r\n");

namespace {

// the last n characters of s (Python's s[-n:])
py::str tail(const py::str& s, int64_t n) { return s.slice(std::max<int64_t>(0, s.size() - n), s.size()); }

// the label a place has, "" when none
py::str label_at(const VxDDisasm& D, const Place& k) {
    const auto l = D.labels.get(k);
    return l ? *l : py::str("");
}
py::str label_at(const VxDDisasm& D, int64_t oi, int64_t a) { return label_at(D, Place(oi, a)); }

inline bool is_internal_(const le::Fixup& f) { return std::holds_alternative<le::InternalFixup>(f); }
inline const le::InternalFixup& internal_(const le::Fixup& f) { return std::get<le::InternalFixup>(f); }

// round(x, ndigits): correctly rounded, half to even on the exact value (Python's)
double round_nd(double x, int nd) {
    char buf[64];
    std::snprintf(buf, sizeof buf, "%.*f", nd, x);
    return std::strtod(buf, nullptr);
}

// open(path, "wb").write(data)
void write_bytes(const py::str& path, const py::bytes& data) {
    py::file f = py::open(path, py::str("wb"));
    f.write(data);
    f.close();
}
// open(path, "w").write(text)  (text mode: "\n" is written as the platform's line end)
void write_text(const py::str& path, const py::str& text) {
    py::file f = py::open(path, py::str("w"), py::str("cp1252"));
    f.write(text);
    f.close();
}
py::bytes read_bytes(const py::str& path) {
    py::file f = py::open_rb(path);
    py::bytes b = f.read();
    f.close();
    return b;
}
py::str what_of(const std::exception& e) { return py::str(std::string(e.what())); }

#ifdef _WIN32
// Sets PATH of the process for the life of the object, then puts back what it was (the child gets the environment of the process).
class ScopedPath {
  public:
    explicit ScopedPath(const py::str& value) {
        const std::wstring v = py::subprocess::to_wide(value);
        DWORD n = GetEnvironmentVariableW(L"PATH", nullptr, 0);
        if (n == 0) {
            had_ = false;
        } else {
            std::wstring buf(n, L'\0');
            const DWORD got = GetEnvironmentVariableW(L"PATH", buf.data(), n);
            buf.resize(got);
            old_ = buf;
        }
        SetEnvironmentVariableW(L"PATH", v.c_str());
    }
    ~ScopedPath() { SetEnvironmentVariableW(L"PATH", had_ ? old_.c_str() : nullptr); }
    ScopedPath(const ScopedPath&) = delete;
    ScopedPath& operator=(const ScopedPath&) = delete;

  private:
    bool had_ = true;
    std::wstring old_;
};
#endif

}  // namespace

py::str _seg_attrs(int64_t flags) {
    py::list<py::str> a;
    a.append(py::str((flags & 0x40) ? "PRELOAD" : ""));
    a.append(py::str((flags & 0x10) ? "DISCARDABLE" : "NONDISCARDABLE"));
    if (flags & 0x20) {
        a.append(py::str("SHARED"));
    }
    if (flags & 0x8000) {
        a.append(py::str("IOPL"));
    }
    if (flags & 0x4000) {
        a.append(py::str("CONFORMING"));
    }
    if (flags & 0x200) {
        a.append(py::str("RESIDENT"));
    }
    py::list<py::str> kept;
    for (const py::str& x : a) {
        if (!x.empty()) {
            kept.append(x);
        }
    }
    return py::join(py::str(" "), kept);
}

void _label_fixup_targets(VxDDisasm& D) {
    std::unordered_map<int64_t, std::unordered_set<int64_t>> covered;   // oi -> offsets INSIDE an instruction
    for (const auto& kv : D.insns.items()) {
        auto& s = covered[std::get<0>(std::get<0>(kv))];
        const int64_t a = std::get<1>(std::get<0>(kv));
        for (int64_t k = a + 1; k < a + std::get<1>(kv).size; ++k) {
            s.insert(k);
        }
    }
    std::unordered_map<int64_t, int64_t> ovs;
    for (const le::Object& o : D.v->objects) {
        ovs[o.idx] = o.vsize;
    }
    for (const auto& kv : D.fx.items()) {
        const le::Fixup& f = std::get<1>(kv);
        if (!is_internal_(f) || !internal_(f).target) {
            continue;
        }
        // A target with bit 0x80000000 set is a HIGH-LINEAR ALIAS reference: Win95 maps a VxD object into the shared 0x80000000+
        // system arena, and these fixups point at obj:(off) through that alias. The real object offset is the low 31 bits --
        // label THAT; the +0x80000000 is restored at emit.
        const int64_t base = *internal_(f).target & 0x7FFFFFFF;
        const Place tgt(internal_(f).obj, base);
        if (D.labels.contains(tgt)) {
            continue;
        }
        const auto ov = ovs.find(std::get<0>(tgt));
        if (ov != ovs.end() && std::get<1>(tgt) >= ov->second) {         // genuinely beyond the object -> keep raw
            continue;
        }
        const auto cv = covered.find(std::get<0>(tgt));
        const bool is_covered = cv != covered.end() && cv->second.count(std::get<1>(tgt)) != 0;
        if (D.insns.contains(tgt) || !is_covered) {                      // insn start or a data byte
            D.labels.set(tgt, py::percent(py::str("loc_%d_%04X"), std::get<0>(tgt), std::get<1>(tgt)));
        }
    }
}

py::list<py::str> _link_body(const VxDDisasm& D, int64_t oi, const std::optional<py::dict<int64_t, py::str>>& extra_labels_in) {
    const py::dict<int64_t, py::str> extra_labels = extra_labels_in ? *extra_labels_in : py::dict<int64_t, py::str>();
    const py::bytes& code = D.code.at(oi);
    const int64_t n = code.size();
    py::list<py::str> L;
    int64_t a = 0;

    // the label of an internal fixup's target ("" when it has none)
    auto _lbl = [&](const le::InternalFixup& f) -> py::str {
        return label_at(D, Place(f.obj, f.target ? *f.target : NONE_TARGET));
    };
    auto _reloc = [&](int64_t k) -> bool {                // an internal fixup at k with a known label
        const auto f = D.fx.get(Place(oi, k));
        return f && is_internal_(*f) && !_lbl(internal_(*f)).empty();
    };

    while (a < n) {
        if (extra_labels.contains(a)) {
            L.append(py::str("public ") + extra_labels.at(a));
            L.append(extra_labels.at(a) + py::str(" label byte"));
        }
        if (!label_at(D, oi, a).empty()) {
            L.append(label_at(D, oi, a) + py::str(":"));
        }
        if (!D.insns.contains(Place(oi, a))) {            // data
            if (_reloc(a)) {
                L.append(py::percent(py::str("        dd  offset %s"), _lbl(internal_(D.fx.at(Place(oi, a))))));
                a += 4;
                continue;
            }
            int64_t b = a;                                // raw bytes to the next label / fixup / insn
            while (b < n && !D.insns.contains(Place(oi, b)) && !(b > a && !label_at(D, oi, b).empty()) &&
                   !(b > a && D.fx.contains(Place(oi, b))) && !(b > a && extra_labels.contains(b))) {
                b += 1;
            }
            for (int64_t i = a; i < b; i += 16) {
                L.append(vxd_masm::_db(code.slice(i, std::min<int64_t>(i + 16, b))));
            }
            a = b;
            continue;
        }
        const Row& row = D.insns.at(Place(oi, a));
        const int64_t size = row.size;
        const int64_t op = code[a];
        // near call/jmp whose rel32 carries the (self-relative) fixup -> symbolic branch
        if (size == 5 && (op == 0xE8 || op == 0xE9) && _reloc(a + 1)) {
            const le::InternalFixup& f = internal_(D.fx.at(Place(oi, a + 1)));
            L.append(py::percent(py::str("        %-6s %s"), py::str(op == 0xE8 ? "call" : "jmp"), _lbl(f)));
            a += size;
            continue;
        }
        py::list<int64_t> fixes;
        for (int64_t k = a; k < a + size; ++k) {
            if (D.fx.contains(Place(oi, k))) {
                fixes.append(k);
            }
        }
        bool all_reloc = true;
        for (int64_t k : fixes) {
            all_reloc = all_reloc && _reloc(k);
        }
        if (!fixes.empty() && !row.vxdcall && all_reloc) {
            int64_t prev = a;                             // 32-bit-offset fixups: split around fields
            for (int64_t k : fixes) {
                if (k > prev) {
                    L.append(vxd_masm::_db(code.slice(prev, k)));
                }
                L.append(py::percent(py::str("        dd  offset %s"), _lbl(internal_(D.fx.at(Place(oi, k))))));
                prev = k + 4;
            }
            if (prev < a + size) {
                L.append(vxd_masm::_db(code.slice(prev, a + size)));
            }
        } else if (row.vxdcall || !fixes.empty() || row.fx) {
            L.append(vxd_masm::_db(row.raw));             // int 20h service, or a fixup we can't symbolize
        } else {
            const auto line = vxd_masm::_masm_line(D, oi, row);
            L.append(line ? *line : vxd_masm::_db(row.raw));
        }
        a += size;
    }
    return L;
}

std::tuple<py::str, py::str> emit_source_and_def(VxDDisasm& D, const py::str& vxdname, const std::optional<ForceRaw>& force_raw, bool functional) {
    const std::optional<le::Ddb> db = D.ddb;
    const std::vector<le::Object> objs = D.v->objects.vec();
    if (functional) {
        _label_fixup_targets(D);                          // every internal fixup target gets a label
    }
    py::list<py::str> asm_ = {py::str(".386p")};
    py::list<py::str> deflines = {py::percent(py::str("VXD %s"), vxdname), py::percent(py::str("DESCRIPTION '%s'"), vxdname), py::str(""),
                                  py::str("SEGMENTS")};
    for (const le::Object& o : objs) {
        const int64_t oi = o.idx;
        const py::str seg = py::percent(py::str("SEG%02d"), oi);
        const py::str cls = py::percent(py::str("LCODE%02d"), oi);               // 'CODE' -> LINK marks it executable;
        asm_.append(py::percent(py::str("%s segment dword public use32 '%s'"), seg, cls));   // zero-pad sorts in order
        py::dict<int64_t, py::str> extra;
        if (db && db->obj == oi) {
            extra.set(db->rva, py::str("VXD_DDB"));
        }
        py::list<py::str> body;
        if (functional) {
            body = _link_body(D, oi, extra);
        } else {
            std::optional<py::set<int64_t>> fr;
            if (force_raw && force_raw->contains(oi)) {
                fr = force_raw->at(oi);
            }
            body = std::get<0>(vxd_masm::object_body(D, oi, fr, 0, extra));
        }
        asm_.extend(body);                                // instructions -> COFF code section
        asm_.append(py::percent(py::str("%s ends"), seg));
        deflines.append(py::percent(py::str("    %-8s CLASS '%s' %s"), seg, cls, _seg_attrs(o.flags)));
    }
    asm_.append(py::str("        end"));
    deflines.append(py::str(""));
    deflines.append(py::str("EXPORTS"));
    deflines.append(py::str("        VXD_DDB @1"));
    return {py::join(py::str("\r\n"), asm_) + py::str("\r\n"), py::join(py::str("\r\n"), deflines) + py::str("\r\n")};
}

std::tuple<std::optional<py::bytes>, py::str, std::optional<py::str>> _assemble_coff(const py::str& asm_text, const py::str& root) {
    const py::str tc = vxd_masm::_stage_toolchain(root);
    namespace dt = codebreaker::build::dos_tools;
    py::dict<py::str, dt::FileData> files;
    files.set(py::str("m.asm"), dt::FileData(asm_text));
    dt::Mounts mounts;
    mounts.append({py::str("c"), tc});
    auto [ok, out, work] = dt::run(mounts, py::list<py::str>{py::str("c:\\ml.exe -coff -c -Fom.obj m.asm > asm.txt")},
                                   py::list<py::str>{py::str("m.obj"), py::str("asm.txt")}, 180, files);
    py::str log;
    if (work && py::os::path::exists(py::os::path::join(*work, py::str("asm.txt")))) {
        py::file f = py::open(py::os::path::join(*work, py::str("asm.txt")), py::str("r"), py::str("cp1252"), py::str("replace"));
        log = f.read_text();
        f.close();
    }
    return {out.get(py::str("m.obj")), log, work};
}

std::tuple<std::optional<py::bytes>, py::str> _link_vxd(const py::bytes& obj_bytes, const py::str& def_text, const py::str& root, const py::str& work) {
    write_bytes(py::os::path::join(work, py::str("m.obj")), obj_bytes);
    write_text(py::os::path::join(work, py::str("m.def")), def_text);
    const py::str link = py::os::path::join(root, _LINK);
    py::subprocess::CompletedProcess p;
    try {
        py::subprocess::RunOptions o;
        o.cwd = work;
        p = py::subprocess::run(py::list<py::str>{link, py::str("/VXD"), py::str("/NOD"), py::str("/OUT:m.vxd"), py::str("/DEF:m.def"), py::str("m.obj")}, o);
    } catch (const py::Exception& e) {
        return {std::nullopt, py::str("link exec failed: ") + what_of(e)};
    }
    const py::str vp = py::os::path::join(work, py::str("m.vxd"));
    const py::str out = (p.stdout_ ? *p.stdout_ : py::str("")) + (p.stderr_ ? *p.stderr_ : py::str(""));
    if (py::os::path::exists(vp)) {
        return {read_bytes(vp), out};
    }
    return {std::nullopt, out};
}

py::str _stage_link260(const py::str& root) {
    (void)root;
    const py::str src = py::os::path::join(vxd_masm::_ddk_root(), py::str("MSVC20"));
    const py::str dst = py::os::path::join(py::tempfile::gettempdir(), py::str("cb_link260"));
    py::os::makedirs(dst, 0777, true);
    for (const py::str& f : _LINK260_FILES) {
        const py::str d = py::os::path::join(dst, f);
        if (!py::os::path::exists(d)) {
            py::shutil::copy(py::os::path::join(src, f), d);
        }
    }
    return py::os::path::join(dst, py::str("LINK.EXE"));
}

std::tuple<std::optional<py::bytes>, py::str> link_vxd_260(const py::bytes& obj_bytes, const py::str& def_text, const py::str& root, const py::str& work,
                                                           int64_t align) {
    const py::str link = _stage_link260(root);
    write_bytes(py::os::path::join(work, py::str("m.obj")), obj_bytes);
    write_text(py::os::path::join(work, py::str("m.def")), def_text);
    const py::str oldpath = py::os::getenv(py::str("PATH"), py::str(""));
    const py::str newpath = py::os::path::dirname(link) + py::os::pathsep + oldpath;   // self-spawn + DLLs
    py::subprocess::CompletedProcess p;
    try {
#ifdef _WIN32
        ScopedPath env(newpath);
#endif
        py::subprocess::RunOptions o;
        o.cwd = work;
        p = py::subprocess::run(py::list<py::str>{link, py::str("/VXD"), py::str("/NOD"), py::percent(py::str("/ALIGN:0x%x"), align),
                                                  py::str("/OUT:m.vxd"), py::str("/DEF:m.def"), py::str("m.obj")},
                                o);
    } catch (const py::Exception& e) {
        return {std::nullopt, py::str("link260 exec failed: ") + what_of(e)};
    }
    const py::str vp = py::os::path::join(work, py::str("m.vxd"));
    const py::str out = (p.stdout_ ? *p.stdout_ : py::str("")) + (p.stderr_ ? *p.stderr_ : py::str(""));
    if (py::os::path::exists(vp)) {
        return {read_bytes(vp), out};
    }
    return {std::nullopt, out};
}

std::tuple<ForceRaw, py::dict<int64_t, bool>> byte_exact_force_raw(const le::LEFile& v, const py::str& root) {
    ForceRaw fr;
    py::dict<int64_t, bool> ok;
    for (const le::Object& o : v.objects) {
        if (!o.exec) {
            continue;
        }
        vxd_masm::CompareObject r = vxd_masm::compare_object(v, o.idx, root);
        fr.set(o.idx, r.force_raw_set);
        ok.set(o.idx, r.match);
    }
    return {fr, ok};
}

CompareObjects compare_objects(const le::LEFile& orc, const le::LEFile& mine) {
    std::vector<std::pair<int64_t, py::bytes>> ob;
    for (const le::Object& o : orc.objects) {
        ob.push_back({o.idx, orc.object_bytes(o)});
    }
    CompareObjects out;
    int64_t exact = 0;
    for (const le::Object& mo : mine.objects) {
        const py::bytes mb = mine.object_bytes(mo);
        std::optional<std::tuple<int64_t, int64_t, int64_t>> best;       // (oracle idx, equal, n)
        for (const auto& [oi, ocb] : ob) {
            if (ocb.size() == mb.size()) {
                int64_t eq = 0;
                for (int64_t i = 0; i < mb.size(); ++i) {
                    if (mb.byte_(i) == ocb.byte_(i)) {
                        eq += 1;
                    }
                }
                if (!best || eq > std::get<1>(*best)) {
                    best = std::make_tuple(oi, eq, mb.size());
                }
            }
        }
        if (best) {
            exact += std::get<1>(*best) == std::get<2>(*best) ? 1 : 0;
            ObjectMatch r;
            r.mine = mo.idx;
            r.oracle = std::get<0>(*best);
            r.equal = std::get<1>(*best);
            r.n = std::get<2>(*best);
            r.pct = std::get<2>(*best) ? round_nd(100.0 * static_cast<double>(std::get<1>(*best)) / static_cast<double>(std::get<2>(*best)), 1) : 0;
            out.objects.append(r);
        }
    }
    out.exact_objects = exact;
    out.total = mine.objects.size();
    return out;
}

BuildAndCompare build_and_compare(const le::LEFile& v, const py::str& vxdname, const py::str& root_in, const std::optional<ForceRaw>& force_raw) {
    VxDDisasm D(v);
    D.run();
    const py::str root = py::os::path::abspath(root_in);
    auto [asm_text, deftext] = emit_source_and_def(D, vxdname, force_raw);
    auto [obj, alog, work] = _assemble_coff(asm_text, root);
    BuildAndCompare r;
    if (!obj || obj->empty()) {
        r.ok = false;
        r.stage = py::str("assemble");
        r.log = tail(alog, 800);
        return r;
    }
    auto [vxd, llog] = _link_vxd(*obj, deftext, root, *work);
    if (!vxd || vxd->empty()) {
        r.ok = false;
        r.stage = py::str("link");
        r.log = tail(llog, 1200);
        r.asm_ = asm_text;
        r.def_ = deftext;
        return r;
    }
    const py::str tmp = py::os::path::join(*work, py::str("out.vxd"));
    write_bytes(tmp, *vxd);
    const le::LEFile mine = le::LEFile::from_file(tmp);
    r.ok = true;
    r.vxd = *vxd;
    r.vxd_len = vxd->size();
    r.oracle_len = v.d().size();
    r.link_log = tail(llog, 400);
    r.report = compare_objects(v, mine);
    return r;
}

BuildFunctional build_functional(const le::LEFile& v, const py::str& vxdname, const py::str& root_in) {
    VxDDisasm D(v);
    D.run();
    const py::str root = py::os::path::abspath(root_in);
    auto [asm_text, deftext] = emit_source_and_def(D, vxdname, std::nullopt, true);
    auto [obj, alog, work] = _assemble_coff(asm_text, root);
    BuildFunctional r;
    if (!obj || obj->empty()) {
        r.ok = false;
        r.stage = py::str("assemble");
        r.log = tail(alog, 1500);
        r.asm_ = asm_text;
        return r;
    }
    auto [vxd, llog] = _link_vxd(*obj, deftext, root, *work);
    if (!vxd || vxd->empty()) {
        r.ok = false;
        r.stage = py::str("link");
        r.log = tail(llog, 1500);
        r.asm_ = asm_text;
        r.def_ = deftext;
        return r;
    }
    const py::str tmp = py::os::path::join(*work, py::str("func.vxd"));
    write_bytes(tmp, *vxd);
    const le::LEFile mine = le::LEFile::from_file(tmp);
    const auto ofx = v.fixups();
    const auto mfx = mine.fixups();
    int64_t oint = 0, mint = 0;
    for (const auto& f : ofx.values()) {
        oint += is_internal_(f) ? 1 : 0;
    }
    for (const auto& f : mfx.values()) {
        mint += is_internal_(f) ? 1 : 0;
    }
    r.ok = true;
    r.vxd = *vxd;
    r.vxd_len = vxd->size();
    r.oracle_len = v.d().size();
    r.oracle_fixups = ofx.size();
    r.oracle_internal = oint;
    r.rebuilt_fixups = mfx.size();
    r.rebuilt_internal = mint;
    r.link_log = tail(llog, 400);
    return r;
}

std::tuple<std::optional<py::bytes>, py::str, py::str> _assemble_idiomatic(const py::str& asm_text, const py::str& root) {
    (void)root;
    const py::str inc = py::os::path::join(vxd_masm::_ddk_root(), py::str("INC32"));
    const py::str ml = vxd_masm::native_ml();
    const py::str work = py::tempfile::mkdtemp(std::nullopt, py::str("cb_idiom_"));
    {
        py::file f = py::open(py::os::path::join(work, py::str("m.asm")), py::str("w"), py::str("cp1252"), std::nullopt, py::str("\r\n"));
        f.write(asm_text);
        f.close();
    }
    py::subprocess::CompletedProcess p;
    try {
        py::subprocess::RunOptions o;
        o.cwd = work;
        p = py::subprocess::run(py::list<py::str>{ml, py::str("-coff"), py::str("-c"), py::str("-DBLD_COFF"), py::str("-DIS_32"), py::str("-DMASM6"),
                                                  py::str("-I") + inc, py::str("-Fom.obj"), py::str("m.asm")},
                                o);
    } catch (const py::Exception& e) {
        return {std::nullopt, py::str("ml exec failed: ") + what_of(e), work};
    }
    const py::str log = (p.stdout_ ? *p.stdout_ : py::str("")) + (p.stderr_ ? *p.stderr_ : py::str(""));
    const py::str objp = py::os::path::join(work, py::str("m.obj"));
    if (py::os::path::exists(objp)) {
        return {read_bytes(objp), log, work};
    }
    return {std::nullopt, log, work};
}

std::tuple<py::bytes, py::list<OverlayRow>> _source_code_into_container(VxDDisasm& D, const le::LEFile& v, const py::bytes& fs_bytes, double min_score) {
    const le::LEFile fs = le::LEFile::from_buffer(fs_bytes);
    // macro-absorbed offsets (regenerated by the DDK macros; never emitted as source code), per object
    PlaceSet emittable;
    for (const Place& k : D.insns) {
        emittable.insert(k);
    }
    auto [handlers, cp_skip] = vxd_idiom::control_dispatch(D, emittable);
    (void)handlers;
    const std::optional<le::Ddb> db = D.ddb;
    if (db) {
        const int64_t sz = vxd_idiom::_ddb_size(D, *db);
        for (int64_t k = db->rva; k < db->rva + sz; ++k) {
            cp_skip.insert(Place(db->obj, k));
        }
    }
    PlaceSet avail;
    for (const Place& k : emittable) {
        if (cp_skip.find(k) == cp_skip.end()) {
            avail.insert(k);
        }
    }
    const auto svc = vxd_idiom::service_table(D, avail);
    if (svc) {
        const auto& [so, sp, sc] = svc->loc;
        for (int64_t k = sp; k < sp + 4 * sc; ++k) {
            cp_skip.insert(Place(so, k));
        }
    }
    std::unordered_map<int64_t, std::unordered_set<int64_t>> macro_by_obj, fx_by_obj;
    for (const Place& k : cp_skip) {
        macro_by_obj[std::get<0>(k)].insert(std::get<1>(k));
    }
    for (const Place& k : D.fx) {
        auto& s = fx_by_obj[std::get<0>(k)];
        for (int64_t j = std::get<1>(k); j < std::get<1>(k) + 4; ++j) {   // whole 4-byte field is loader-written
            s.insert(j);
        }
    }
    std::vector<std::pair<int64_t, py::bytes>> asm_bytes;                  // {object idx: its bytes}
    for (const le::Object& fo : fs.objects) {
        asm_bytes.push_back({fo.idx, fs.object_bytes(fo)});
    }

    // Overlay `cand` (assembled true-code) onto `rec` (recovered object), skipping macro offsets in rec and never
    // comparing/substituting a fixup-field byte. Returns (new_bytes, agree, comp): agree over comp comparable bytes = how much
    // already matched -- the match quality that ranks/gates.
    auto walk = [](const py::bytes& rec, const std::unordered_set<int64_t>& macro, const py::bytes& cand, const std::unordered_set<int64_t>& fx_off)
        -> std::tuple<py::bytes, int64_t, int64_t> {
        std::string b = rec.raw();
        int64_t si = 0, agree = 0, comp = 0;
        const int64_t n = static_cast<int64_t>(b.size());
        for (int64_t off = 0; off < n; ++off) {
            if (macro.count(off)) {
                continue;
            }
            if (si >= cand.size()) {
                break;
            }
            if (!fx_off.count(off)) {
                comp += 1;
                if (static_cast<unsigned char>(b[static_cast<size_t>(off)]) == cand.byte_(si)) {
                    agree += 1;
                } else {
                    b[static_cast<size_t>(off)] = static_cast<char>(cand.byte_(si));
                }
            }
            si += 1;
        }
        return {py::bytes(b), agree, comp};
    };

    // per-object context, and the full (recovered x assembled) score matrix
    static const std::unordered_set<int64_t> none_;
    struct Meta {
        py::bytes rec;
        const std::unordered_set<int64_t>* macro;
        const std::unordered_set<int64_t>* fx_off;
        int64_t expected;
    };
    std::unordered_map<int64_t, Meta> meta;
    struct Score {
        double score;
        int64_t oi, aoi, clen;
    };
    std::vector<Score> scores;
    for (const le::Object& o : v.objects) {
        const int64_t oi = o.idx;
        const py::bytes rec = D.code.at(oi);
        const auto mi = macro_by_obj.find(oi);
        const auto fi = fx_by_obj.find(oi);
        const std::unordered_set<int64_t>* macro = mi != macro_by_obj.end() ? &mi->second : &none_;
        const std::unordered_set<int64_t>* fx_off = fi != fx_by_obj.end() ? &fi->second : &none_;
        meta[oi] = Meta{rec, macro, fx_off, rec.size() - static_cast<int64_t>(macro->size())};   // expected = bytes the asm must supply
        for (const auto& [aoi, cb] : asm_bytes) {
            const auto [nb, agree, comp] = walk(rec, *macro, cb, *fx_off);
            (void)nb;
            scores.push_back({comp ? static_cast<double>(agree) / static_cast<double>(comp) : 0.0, oi, aoi, cb.size()});
        }
    }
    std::stable_sort(scores.begin(), scores.end(), [](const Score& a, const Score& b) { return a.score > b.score; });   // strongest pairs first
    std::unordered_map<int64_t, std::tuple<int64_t, double, int64_t>> pick;
    std::unordered_set<int64_t> taken;
    for (const Score& s : scores) {
        if (!pick.count(s.oi) && !taken.count(s.aoi)) {
            pick[s.oi] = std::make_tuple(s.aoi, s.score, s.clen);
            taken.insert(s.aoi);
        }
    }

    py::list<OverlayRow> report;
    for (const le::Object& o : v.objects) {
        const int64_t oi = o.idx;
        const Meta& m = meta[oi];
        OverlayRow row;
        row.obj = oi;
        const auto pk = pick.find(oi);
        if (pk == pick.end()) {
            row.asm_obj = std::nullopt;
            row.score = 0.0;
            row.cmp = 0;
            row.expected = m.expected;
            row.asm_len = 0;
            row.macro = static_cast<int64_t>(m.macro->size());
            row.ok = false;
            report.append(row);
            continue;
        }
        const auto [aoi, score, clen] = pk->second;
        const py::bytes* cb = nullptr;
        for (const auto& [i, b] : asm_bytes) {
            if (i == aoi) {
                cb = &b;
            }
        }
        const auto [newb, agree, comp] = walk(m.rec, *m.macro, *cb, *m.fx_off);
        (void)agree;
        // comp==0: object is entirely macro/fixup (its bytes are loader-written) -> match on length alone
        const bool ok = clen >= m.expected && (comp == 0 || score >= min_score);
        if (ok) {
            D.code.set(oi, newb);
        }
        row.asm_obj = aoi;
        row.score = round_nd(score, 4);
        row.cmp = comp;
        row.expected = m.expected;
        row.asm_len = clen;
        row.macro = static_cast<int64_t>(m.macro->size());
        row.ok = ok;
        report.append(row);
    }

    py::list<py::str> bad;
    for (const OverlayRow& r : report) {
        if (!r.ok) {
            bad.append(py::percent(py::str("obj%d(asm=%s score=%.3f cmp=%d need_len>=%d got=%d)"), r.obj,
                                   r.asm_obj ? py::str(std::to_string(*r.asm_obj)) : py::str("None"), r.score, r.cmp, r.expected, r.asm_len));
        }
    }
    if (!bad.empty()) {
        py::list<py::str> have;
        std::vector<std::pair<int64_t, py::bytes>> sorted_asm = asm_bytes;
        std::stable_sort(sorted_asm.begin(), sorted_asm.end(), [](const auto& a, const auto& b) { return a.first < b.first; });
        for (const auto& [aoi, cb] : sorted_asm) {
            have.append(py::percent(py::str("%d:%d"), aoi, cb.size()));
        }
        throw py::ValueError((py::str("from-source overlay could not confidently match every object (refusing to ship scrambled code). Weak/unmatched: ") +
                              py::join(py::str("; "), bad) + py::str(". Assembled objects (idx:len): ") + py::join(py::str(", "), have))
                                 .utf8());
    }
    return {vxd_le_writer::build_le(D, v, db ? db->name : py::str("DEVICE")), report};
}

LoadableIdiomatic build_loadable_idiomatic(const le::LEFile& v, const std::optional<py::str>& name_in, const py::str& root_in) {
    VxDDisasm D(v);
    D.run();
    py::str name;
    if (name_in && !name_in->empty()) {
        name = *name_in;
    } else if (D.ddb && !D.ddb->name.empty()) {
        name = D.ddb->name;
    } else {
        name = py::str("None");   // (the Python formats a None name as "None")
    }
    const py::str root = py::os::path::abspath(root_in);
    const py::str asm_text = vxd_idiom::emit_idiomatic(D, true);
    auto [obj, alog, work] = _assemble_idiomatic(asm_text, root);
    LoadableIdiomatic res;
    if (!obj || obj->empty()) {
        res.ok = false;
        res.stage = py::str("assemble");
        res.log = tail(alog, 1500);
        res.asm_ = asm_text;
        return res;
    }
    py::str perobj;                                       // one .DEF entry per object (unique class) so
    for (const le::Object& o : v.objects) {               // LINK keeps each object separate instead of
        const auto [sn, cl] = vxd_idiom::obj_seg(o.idx, o.flags);   // merging by class. Fixed PRELOAD NONDISCARDABLE
        perobj = perobj + py::percent(py::str("    %-11s CLASS '%s' PRELOAD NONDISCARDABLE\r\n"), sn, cl);   // (real flags come from the
    }                                                     // byte-exact writer, not LINK): the DISCARDABLE attribute makes LINK
    //                                                       reorder objects (discardable to the end), which desyncs the code overlay.
    const py::str deftext = py::percent(py::str("VXD %s\r\nDESCRIPTION '%s reconstructed from source'\r\n\r\nSEGMENTS\r\n%s%s\r\nEXPORTS\r\n        %s_DDB @1\r\n"),
                                        name, name, _VXD_DEF_SEGS, perobj, name);
    auto [vxd, llog] = _link_vxd(*obj, deftext, root, work);
    if (!vxd || vxd->empty()) {
        res.ok = false;
        res.stage = py::str("link");
        res.log = tail(llog, 1500);
        res.asm_ = asm_text;
        res.def_ = deftext;
        return res;
    }
    const le::LEFile link_mine = le::LEFile::from_buffer(*vxd);
    const auto mfx = link_mine.fixups();
    const auto ofx = v.fixups();
    py::bytes final_bytes;
    py::list<OverlayRow> overlay;
    try {
        std::tie(final_bytes, overlay) = _source_code_into_container(D, v, *vxd);   // assembled code -> loader-accepted container
    } catch (const py::ValueError& e) {                                             // low-confidence object match -> clean failure
        res.ok = false;
        res.stage = py::str("overlay");
        res.log = what_of(e);
        res.asm_ = asm_text;
        res.link_log = tail(llog, 300);
        return res;
    }
    int64_t mint = 0, oint = 0;
    for (const auto& f : mfx.values()) {
        mint += is_internal_(f) ? 1 : 0;
    }
    for (const auto& f : ofx.values()) {
        oint += is_internal_(f) ? 1 : 0;
    }
    double min_score = 0.0;
    bool first = true;
    for (const OverlayRow& r : overlay) {
        if (first || r.score < min_score) {
            min_score = r.score;
        }
        first = false;
    }
    res.ok = true;
    res.vxd = final_bytes;
    res.vxd_len = final_bytes.size();
    res.oracle_len = v.d().size();
    res.asm_ = asm_text;
    res.objects = le::LEFile::from_buffer(final_bytes).objects.size();
    res.link_log = tail(llog, 300);
    res.overlay = overlay;
    res.min_overlay_score = min_score;
    res.rebuilt_internal = mint;
    res.oracle_internal = oint;
    return res;
}

}  // namespace codebreaker::source::vxd::link
