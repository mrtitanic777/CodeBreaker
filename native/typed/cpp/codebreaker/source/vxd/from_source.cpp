// codebreaker/source/vxd/from_source.cpp -- typed C++ of codebreaker.source.vxd.from_source.

#include "codebreaker/source/vxd/from_source.hpp"

#include <algorithm>
#include <ostream>
#include <vector>

#include "codebreaker/base/corpus.hpp"
#include "codebreaker/build/dos_tools.hpp"
#include "codebreaker/formats/le.hpp"
#include "codebreaker/source/vxd/idiom.hpp"
#include "codebreaker/source/vxd/le_writer.hpp"
#include "codebreaker/source/vxd/link.hpp"
#include "py_glob.hpp"
#include "py_os.hpp"
#include "py_stdlib.hpp"

namespace codebreaker::source::vxd::from_source {

namespace vxd_idiom = codebreaker::source::vxd::idiom;
namespace vxd_link = codebreaker::source::vxd::link;
namespace vxd_le_writer = codebreaker::source::vxd::le_writer;
using disassembly::Place;

const py::str _RC_DIR("toolchains/period/compilers/msvc-1.52c/BIN");
const py::list<py::str> _ADRC_REF = {py::str("Microsoft Windows 95 DDK"), py::str("BIN"), py::str("ADRC2VXD.EXE")};
const py::list<py::str> _INC16_REF = {py::str("Microsoft Windows 95 DDK"), py::str("INC16")};

const py::set<py::str> _MASM_RESERVED = {
    py::str("LOCK"),  py::str("REP"),    py::str("REPE"),  py::str("REPZ"),   py::str("REPNE"), py::str("REPNZ"), py::str("REPT"),  py::str("AND"),
    py::str("OR"),    py::str("XOR"),    py::str("NOT"),   py::str("SHL"),    py::str("SHR"),   py::str("MOD"),   py::str("EQ"),    py::str("NE"),
    py::str("LT"),    py::str("GT"),     py::str("LE"),    py::str("GE"),     py::str("BYTE"),  py::str("WORD"),  py::str("DWORD"), py::str("NEAR"),
    py::str("FAR"),   py::str("PTR"),    py::str("OFFSET"), py::str("SEG"),   py::str("PROC"),  py::str("ENDP"),  py::str("IF"),    py::str("ELSE"),
    py::str("ENDIF"), py::str("END"),    py::str("SIZE"),  py::str("LENGTH"), py::str("TYPE"),  py::str("THIS"),  py::str("SHORT"), py::str("HIGH"),
    py::str("LOW"),   py::str("GROUP"),  py::str("ASSUME"), py::str("PUBLIC")};

py::str _seg_name(const py::str& tag) { return _MASM_RESERVED.contains(tag.upper()) ? py::str("_") + tag : tag; }

namespace {

// str.isalpha / isalnum of a single character
bool alpha_(char32_t c) { return py::str(std::u32string(1, c)).isalpha(); }
bool alnum_(char32_t c) { return py::str(std::u32string(1, c)).isalnum(); }

// Python's str.rstrip("\x00 ").strip()
py::str strip_tag(const py::str& tag) {
    std::u32string u = tag.u32();
    while (!u.empty() && (u.back() == U'\0' || u.back() == U' ')) u.pop_back();
    return py::str(u).strip();
}

}  // namespace

py::dict<int64_t, std::tuple<py::str, py::str>> _object_segs(const py::list<py::str>& names, int64_t ddb_obj) {
    py::dict<int64_t, std::tuple<py::str, py::str>> segs;
    py::set<py::str> seen;
    for (int64_t k = 0; k < names.size(); ++k) {
        const int64_t idx = k + 1;
        if (idx == ddb_obj) continue;
        const py::str t = strip_tag(names[k]);
        bool valid = !t.empty() && (alpha_(t.at_(0)) || t.at_(0) == U'_');
        if (valid) {
            for (char32_t c : t.u32())
                if (!(alnum_(c) || c == U'_' || c == U'$' || c == U'?' || c == U'@')) valid = false;
        }
        const py::str base = valid ? _seg_name(t) : py::percent(py::str("SEG%d"), idx);
        py::str name = base;
        int64_t n = 2;
        while (seen.contains(name)) {   // de-duplicate a repeated tag (e.g. two PCOD objects)
            name = py::percent(py::str("%s_%d"), base, n);
            n += 1;
        }
        seen.add(name);
        segs.set(idx, std::make_tuple(name, name + py::str("CODE")));
    }
    return segs;
}

py::list<py::str> object_names(const le::LEFile& v) {
    const py::bytes& d = v.d();
    const int64_t lfa = v.lfa;
    const int64_t ot = py::unpack_from1<int64_t>("<I", d, lfa + 0x40);
    const int64_t oc = py::unpack_from1<int64_t>("<I", d, lfa + 0x44);
    py::list<py::str> out;
    for (int64_t i = 0; i < oc; ++i) out.append(d.slice(lfa + ot + i * 24 + 20, lfa + ot + i * 24 + 24).decode("latin1", "replace"));
    return out;
}

std::optional<int64_t> _reclassify_ddb_tail_as_data(VxDDisasm& D, const le::LEFile& v) {
    if (!D.ddb) return std::nullopt;
    const int64_t oi = D.ddb->obj;
    const int64_t vsize = v.objects[oi - 1].vsize;
    const int64_t tail0 = D.ddb->rva + vxd_idiom::_ddb_size(D, *D.ddb);
    if (tail0 >= vsize) return std::nullopt;
    for (const Place& k : D.insns.keys())
        if (std::get<0>(k) == oi && tail0 <= std::get<1>(k) && std::get<1>(k) < vsize) D.insns.pop(k);
    for (const Place& k : D.labels.keys())   // a proc label on data confuses the emitter into a BeginProc -> demote to a plain data label
        if (std::get<0>(k) == oi && tail0 <= std::get<1>(k) && std::get<1>(k) < vsize && D.labels.at(k).startswith(py::str("sub_")))
            D.labels.set(k, py::percent(py::str("loc_%d_%04X"), std::get<0>(k), std::get<1>(k)));
    return tail0;
}

py::str realseg_source(VxDDisasm& D, const le::LEFile& v, const py::list<py::str>& names, const std::optional<int64_t>& tail0) {
    (void)v;
    (void)tail0;
    static const py::re::Pattern vseg_decl = py::re::compile(py::str("^(VSEG\\d\\d) SEGMENT DWORD PUBLIC _FLAT"));
    static const py::re::Pattern vseg_open = py::re::compile(py::str("^VSEG(\\d\\d) SEGMENT$"));
    static const py::re::Pattern vseg_end = py::re::compile(py::str("^VSEG(\\d\\d) ENDS$"));
    const int64_t ddb_obj = D.ddb ? D.ddb->obj : 1;
    const auto segs = _object_segs(names, ddb_obj);   // {idx: (name, class)} for the non-DDB objects

    auto open_close = [&](int64_t idx) -> std::tuple<py::str, py::str> {
        if (idx == ddb_obj) return std::make_tuple(py::str("VxD_LOCKED_CODE_SEG"), py::str("VxD_LOCKED_CODE_ENDS"));   // the locked-code macro (_LTEXT)
        const py::str& nm = std::get<0>(segs.at(idx));
        return std::make_tuple(nm + py::str(" SEGMENT"), nm + py::str(" ENDS"));
    };

    const py::list<py::str> lines = py::split(vxd_idiom::emit_idiomatic(D, true), py::str("\r\n"));
    // 1) drop the synthetic-VSEG predeclarations
    py::list<py::str> out;
    int64_t i = 0;
    while (i < lines.size()) {
        const auto m = vseg_decl.match(lines[i]);
        if (m && i + 1 < lines.size() && lines[i + 1].strip() == m->group(1) + py::str(" ENDS")) {
            i += 2;
            continue;
        }
        out.append(lines[i]);
        i += 1;
    }
    // 2) retarget each object-walk VSEGnn open/close to the real segment
    py::list<py::str> res;
    for (const py::str& ln : out) {
        const auto mo = vseg_open.match(ln.strip());
        const auto me = vseg_end.match(ln.strip());
        if (mo) {
            res.append(std::get<0>(open_close(py::int_parse(mo->group(1)))));
            continue;
        }
        if (me) {
            res.append(std::get<1>(open_close(py::int_parse(me->group(1)))));
            continue;
        }
        res.append(ln);
    }
    // 3) The DDB object stays ONE contiguous `_LTEXT` segment (code + DDB struct + post-DDB data tail). An earlier version split the tail
    // (>= tail0) into a separate `_LDATA` segment, but LINK /VXD orders same-class ('LCODE') segments ALPHABETICALLY, and "_LDATA" sorts before
    // "_LTEXT" -- so the data tail landed at object offset 0 and the code after it, shifting EVERY tail label by -tail0. Then every
    // cross-object `dd/mov offset <tail-label>` resolved to the wrong offset and the VxD faulted mid-init (dosmgr: 727 relocated-byte diffs ->
    // Device_Init triple-fault). Keeping the object contiguous makes the compile-lane output byte-exact (relocated-diff 0); the tail is data via
    // db/dd regardless of the segment's class, and the final LE object attributes are restored from the oracle (restore_oracle_object_flags),
    // so nothing depends on the tail living in a 'DATA'-class segment.
    // 4) predeclare the custom segments in the _FLAT model, right after the includes
    py::list<py::str> decls;
    std::vector<int64_t> idxs = segs.keys();
    std::sort(idxs.begin(), idxs.end());
    for (int64_t idx : idxs) {
        const auto& nc = segs.at(idx);
        decls.append(py::percent(py::str("%s SEGMENT DWORD PUBLIC _FLAT '%s'"), std::get<0>(nc), std::get<1>(nc)));
        decls.append(py::percent(py::str("%s ENDS"), std::get<0>(nc)));
    }
    int64_t j = 0;
    while (j < res.size() && !res[j].startswith(py::str("include"))) j += 1;
    while (j < res.size() && (res[j].startswith(py::str("include")) || res[j] == py::str(""))) j += 1;
    py::list<py::str> all = res.slice(0, j);
    all.extend(decls);
    all.append(py::str(""));
    all.extend(res.slice(j, res.size()));
    return py::join(py::str("\r\n"), all) + py::str("\r\n");
}

py::str realseg_def(const py::str& name, const py::list<py::str>& names, int64_t ddb_obj) {
    py::list<py::str> seg = {py::str("    _LTEXT   CLASS 'LCODE'  PRELOAD NONDISCARDABLE"), py::str("    _LDATA   CLASS 'LCODE'  PRELOAD NONDISCARDABLE")};
    const auto segs = _object_segs(names, ddb_obj);
    std::vector<int64_t> idxs = segs.keys();
    std::sort(idxs.begin(), idxs.end());
    for (int64_t idx : idxs) {
        const auto& nc = segs.at(idx);
        seg.append(py::percent(py::str("    %-8s CLASS '%s' PRELOAD NONDISCARDABLE"), std::get<0>(nc), std::get<1>(nc)));
    }
    return py::percent(py::str("VXD %s\r\nDESCRIPTION '%s reconstructed from source'\r\n\r\nSEGMENTS\r\n%s\r\n\r\nEXPORTS\r\n        %s_DDB @1\r\n"), name, name,
                       py::join(py::str("\r\n"), seg), name);
}

py::dict<py::str, py::str> ver_strings(const py::bytes& res) {
    py::dict<py::str, py::str> out;
    for (const char* key : {"FileDescription", "LegalCopyright", "InternalName", "OriginalFilename"}) {
        const int64_t i = res.find(py::bytes(key));
        if (i < 0) continue;
        int64_t j = i + static_cast<int64_t>(std::string(key).size());
        while (j < res.size() && res[j] == 0) j += 1;   // skip key terminator + alignment pad
        int64_t e = j;
        while (e < res.size() && res[e] != 0) e += 1;   // value runs to its null
        out.set(py::str(key), res.slice(j, e).decode("latin1", "replace"));
    }
    return out;
}

py::str version_rc(const VxDDisasm& D, const py::bytes& res) {
    static const py::re::Pattern years = py::re::compile(py::str("\\d{4}-\\d{4}|\\d{4}"));
    const auto s = ver_strings(res);
    const auto m = years.search(s.get(py::str("LegalCopyright"), py::str("")));
    if (!D.ddb) throw py::TypeError("'NoneType' object is not subscriptable");   // (D.ddb["device_id"])
    const le::Ddb& db = *D.ddb;
    return py::percent(py::str("#include <version.h>\r\n#define VER_FILETYPE             VFT_VXD\r\n#define VER_FILESUBTYPE          0x%08X\r\n"
                               "#define VER_FILEDESCRIPTION_STR  \"%s\"\r\n#define VER_INTERNALNAME_STR     \"%s\"\r\n"
                               "#define VER_LEGALCOPYRIGHT_YEARS \"%s\"\r\n#define VER_ORIGINALFILENAME_STR \"%s.VXD\"\r\n#include <common.ver>\r\n"),
                       db.device_id, s.get(py::str("FileDescription"), py::str("")), db.name, m ? m->group(0) : py::str(""), db.name);
}

std::tuple<py::str, py::str> _stage_res_tools(const py::str& root) {
    const py::str tools = py::os::path::join(py::tempfile::gettempdir(), py::str("cb_restools"));
    const py::str inc = py::os::path::join(py::tempfile::gettempdir(), py::str("cb_inc16"));
    py::os::makedirs(tools, 0777, true);
    py::os::makedirs(inc, 0777, true);
    const py::str rcbin = py::os::path::join(root, _RC_DIR.replace(py::str("/"), py::str("\\")));
    const std::tuple<const char*, py::str> items[] = {{"RC.EXE", py::os::path::join(rcbin, py::str("RC.EXE"))},
                                                      {"RCPP.EXE", py::os::path::join(rcbin, py::str("RCPP.EXE"))},
                                                      {"ADRC2VXD.EXE", base::corpus::reference(_ADRC_REF)}};
    for (const auto& it : items) {
        const py::str dst = py::os::path::join(tools, py::str(std::get<0>(it)));
        if (!py::os::path::exists(dst)) py::shutil::copy(std::get<1>(it), dst);
    }
    const py::str incsrc = base::corpus::reference(_INC16_REF);
    py::list<py::str> hs = py::glob::glob(incsrc + py::str("\\*.H"));
    hs.extend(py::glob::glob(incsrc + py::str("\\*.VER")));
    for (const py::str& f : hs) {
        const py::str dst = py::os::path::join(inc, py::os::path::basename(f));
        if (!py::os::path::exists(dst)) py::shutil::copy(f, dst);
    }
    return std::make_tuple(tools, inc);
}

std::tuple<std::optional<py::bytes>, py::str> embed_version_resource(const py::bytes& vxd_bytes, const VxDDisasm& D, const py::str& root) {
    namespace dt = build::dos_tools;
    const int64_t lfa = D.v->lfa;
    const int64_t woff = py::unpack_from1<int64_t>("<I", D.v->d(), lfa + 0xb8);   // absolute file offset of the oracle resource
    const int64_t wlen = py::unpack_from1<int64_t>("<I", D.v->d(), lfa + 0xbc);
    if (!wlen) return std::make_tuple(std::optional<py::bytes>(vxd_bytes), py::str("(no version resource in oracle; skipped)"));   // ~1/7 of VxDs ship none
    const py::str rc_text = version_rc(D, D.v->d().slice(woff, woff + wlen));
    const auto ti = _stage_res_tools(root);
    py::list<py::str> bat = {py::str("set INCLUDE=e:\\"), py::str("copy in.vxd out.vxd"), py::str("c:\\RC.EXE -r m.rc > rc.txt"),
                             py::str("c:\\ADRC2VXD.EXE out.vxd m.res > adrc.txt")};
    dt::Mounts mounts;
    mounts.append({py::str("c"), std::get<0>(ti)});
    mounts.append({py::str("e"), std::get<1>(ti)});
    py::dict<py::str, dt::FileData> files;
    files.set(py::str("m.rc"), dt::FileData(rc_text));
    files.set(py::str("in.vxd"), dt::FileData(vxd_bytes));
    const auto r = dt::run(mounts, bat, py::list<py::str>{py::str("out.vxd"), py::str("m.res"), py::str("rc.txt"), py::str("adrc.txt")}, 180, files);
    const py::dict<py::str, py::bytes>& out = std::get<1>(r);
    py::str log;
    for (const char* t : {"rc.txt", "adrc.txt"})
        if (out.contains(py::str(t))) log += py::percent(py::str("[%s] %s\n"), py::str(t), out.at(py::str(t)).decode("latin1", "replace").strip());
    if (std::get<2>(r)) py::shutil::rmtree(*std::get<2>(r), true);   // (the DOS tools' work folder was left behind: one temp folder per call)
    std::optional<py::bytes> res_bytes;
    if (out.contains(py::str("out.vxd"))) res_bytes = out.at(py::str("out.vxd"));
    return std::make_tuple(res_bytes, rc_text + py::str("\n") + log);
}

Built _result(const py::bytes& final_, const le::LEFile& v, const VxDDisasm& D, const std::optional<py::str>& asm_, const std::optional<py::str>& deftext,
              const py::str& llog, const py::str& rlog) {
    const le::LEFile m = le::LEFile::from_buffer(final_);
    const py::bytes& d = m.d();
    const int64_t lfa = m.lfa;
    const int64_t pm = py::unpack_from1<int64_t>("<I", d, lfa + 0x48);
    const int64_t npg = py::unpack_from1<int64_t>("<I", d, lfa + 0x14);
    bool identity = true;
    for (int64_t k = 0; k < npg; ++k) {
        const int64_t phys = (d[lfa + pm + k * 4] << 16) | (d[lfa + pm + k * 4 + 1] << 8) | d[lfa + pm + k * 4 + 2];
        if (phys != k + 1) identity = false;
    }
    const std::optional<le::Ddb> dd = m.ddb();
    const int64_t dev = py::unpack_from1<int64_t>("<H", d, lfa + 0xc0);   // device id stamped from the DDB
    const int64_t reslen = py::unpack_from1<int64_t>("<I", d, lfa + 0xbc);   // version resource embedded
    const py::bytes o = v.object_bytes(v.objects[0]);
    const py::bytes r = m.object_bytes(m.objects[0]);
    int64_t o1diffs = 0;
    for (int64_t k = 0; k < std::min(o.size(), r.size()); ++k)
        if (o[k] != r[k]) o1diffs += 1;
    Built b;
    b.ok = true;
    b.vxd = final_;
    b.vxd_len = final_.size();
    b.oracle_len = v.d().size();
    b.asm_ = asm_;
    b.def_ = deftext;
    b.objects = m.objects.size();
    b.page_map_identity = identity;
    if (dd) b.ddb_rva = dd->rva;
    if (D.ddb) b.oracle_ddb_rva = D.ddb->rva;
    b.device_id = dev;
    b.device_id_ok = D.ddb.has_value() && dev == D.ddb->device_id;
    b.resource_len = reslen;
    b.obj1_diffs = o1diffs;
    b.link_log = llog.size() > 300 ? llog.slice(llog.size() - 300, llog.size()) : llog;
    b.res_log = rlog.size() > 300 ? rlog.slice(rlog.size() - 300, rlog.size()) : rlog;
    return b;
}

Built build_from_source(const le::LEFile& v, const std::optional<py::str>& name_in, const py::str& root_in) {
    const py::str root = py::os::path::abspath(root_in);
    VxDDisasm D(v);
    D.run();
    py::str name = (name_in && !name_in->empty()) ? *name_in : (D.ddb && !D.ddb->name.empty() ? D.ddb->name : py::str("VXD"));
    // Page-size fallback: our compile+link lane always emits 4096-byte LE pages, but the original linker paginated a few modules differently --
    // BIOS.VXD uses 512-byte pages (33 of them). VMM faults on BIOS specifically when its pagination differs from retail's (its early real-mode-init
    // path is page-granularity sensitive; proven by bisect: all 34 other from-source VxDs boot, BIOS alone triple-faults at Device_Init, and
    // re-paginating it to 512 fixes the full-set boot). For such a module, emit the LE directly from the recovered disassembly at the original
    // page size via the proven synthesis path, which preserves our recovered code/fixups while matching the original's page layout. (This also
    // sidesteps the compile lane's segment-assumption on BIOS.)
    if (v.pagesize != 0x1000) return _result(vxd_le_writer::build_le(D, v, name), v, D);
    const py::list<py::str> names = object_names(v);
    const std::optional<int64_t> tail0 = _reclassify_ddb_tail_as_data(D, v);
    const py::str asm_ = realseg_source(D, v, names, tail0);   // readable, idiomatic DDK-macro source
    const py::str deftext = realseg_def(name, names, D.ddb ? D.ddb->obj : 1);
    // Compile the readable source with the authentic toolchain -- native ML.EXE (MASM 6.11d) + LINK 2.60 /VXD -- to VALIDATE it assembles + links
    // (the compiled object code is byte-exact to the original, relocated-diff 0). We keep the compile result for the status but do NOT ship
    // LINK's container: LINK 2.60's /VXD page-map + data-page layout + fixup-record encoding differ from the original linker's and VMM's init does
    // not tolerate that layout (it faults mid-Device_Init even with byte-exact code). The BOOTABLE binary is emitted by the LE writer (build_le),
    // which reproduces the ORIGINAL container exactly from the same recovered code -- so build_from_source returns a VxD that BOOTS, accompanied
    // by readable source proven to compile to the identical object code.
    auto ai = vxd_link::_assemble_idiomatic(asm_, root);
    const std::optional<py::bytes>& obj = std::get<0>(ai);
    const py::str& alog = std::get<1>(ai);
    const py::str& work = std::get<2>(ai);
    const bool compile_ok = obj.has_value() && !obj->empty();
    bool link_ok = false;
    py::str llog = alog;
    if (compile_ok) {
        const auto lk = vxd_link::link_vxd_260(*obj, deftext, root, work);
        llog = std::get<1>(lk);
        link_ok = std::get<0>(lk).has_value() && !std::get<0>(lk)->empty();
    }
    if (!work.empty()) py::shutil::rmtree(work, true);   // (the compile/link work folder was left behind: one temp folder per call)
    const py::bytes final_ = vxd_le_writer::build_le(D, v, name);   // bootable container (original-exact layout)
    Built res = _result(final_, v, D, asm_, deftext, llog.size() > 300 ? llog.slice(llog.size() - 300, llog.size()) : llog, py::str(""));
    res.compile_ok = compile_ok;   // readable source assembled with native ML
    res.link_ok = link_ok;         // ... and linked with LINK 2.60 /VXD
    return res;
}

int64_t main(const py::list<py::str>& argv, std::ostream& out, std::ostream& err) {
    (void)err;
    if (argv.size() < 1) throw py::IndexError("list index out of range");
    const auto vs = le::open_vxds(argv[0]);
    if (vs.empty()) throw py::IndexError("list index out of range");
    const Built r = build_from_source(std::get<1>(vs[0]));
    out << py::percent(py::str("built %d bytes (oracle %d); objects=%d; page map identity=%s; DDB @0x%x (oracle 0x%x); obj1 diffs=%d"), r.vxd_len, r.oracle_len,
                       r.objects, r.page_map_identity ? py::str("True") : py::str("False"), r.ddb_rva.value_or(0), r.oracle_ddb_rva.value_or(0), r.obj1_diffs)
               .utf8()
        << "\n";
    if (argv.size() > 1) {
        py::file f = py::open(argv[1], py::str("wb"));
        f.write(r.vxd);
        f.close();
        out << "wrote " << argv[1].utf8() << "\n";
    }
    return 0;
}

}  // namespace codebreaker::source::vxd::from_source
