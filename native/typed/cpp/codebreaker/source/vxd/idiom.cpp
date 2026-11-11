// codebreaker/source/vxd/idiom.cpp -- typed C++ of codebreaker.source.vxd.idiom (see idiom.hpp).

#include "codebreaker/emulate/capstone_x86.hpp"
#include "codebreaker/platforms/package.hpp"
#include "codebreaker/source/vxd/idiom.hpp"

#include <algorithm>
#include <cstring>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <capstone/capstone.h>

#include "codebreaker/base/corpus.hpp"
#include "codebreaker/build/dos_tools.hpp"
#include "codebreaker/formats/detect.hpp"
#include "codebreaker/source/vxd/link.hpp"
#include "codebreaker/source/vxd/masm.hpp"
#include "codebreaker/source/vxd/services.hpp"
#include "codebreaker/source/vxd/symbols.hpp"
#include "py_stdlib.hpp"

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#endif

namespace codebreaker::source::vxd::idiom {

namespace dis = codebreaker::source::vxd::disassembly;
namespace vxd_masm = codebreaker::source::vxd::masm;
namespace vxd_link = codebreaker::source::vxd::link;
namespace vxd_services = codebreaker::source::vxd::services;
namespace vxd_symbols = codebreaker::source::vxd::symbols;
namespace paths = codebreaker::base::corpus;

using dis::NONE_TARGET;

void set_capstone_dll(const py::str& path) { emulate::capstone_x86::load_capstone(path); }

namespace {

// ---- small helpers -------------------------------------------------------------------------------------------

inline bool in_(const PlaceSet& s, int64_t oi, int64_t off) { return s.find(Place(oi, off)) != s.end(); }

inline bool is_internal_(const le::Fixup& f) { return std::holds_alternative<le::InternalFixup>(f); }
inline const le::InternalFixup& internal_(const le::Fixup& f) { return std::get<le::InternalFixup>(f); }
// an internal fix-up's target as the place key holds it (a selector's None stays one place: NONE_TARGET)
inline int64_t target_key_(const le::InternalFixup& f) { return f.target ? *f.target : NONE_TARGET; }
// f["obj"] / f["target"]: an import fix-up has no such key
inline const le::InternalFixup& need_internal_(const le::Fixup& f, const char* key) {
    if (!is_internal_(f)) {
        throw py::KeyError(key);
    }
    return internal_(f);
}
// the label a place has, "" when none (`D.labels.get(k)` falsy)
py::str label_at(const VxDDisasm& D, const Place& k) {
    const auto l = D.labels.get(k);
    return l ? *l : py::str("");
}
py::str label_at(const VxDDisasm& D, int64_t oi, int64_t a) { return label_at(D, Place(oi, a)); }
// "loc_%d_%04X" % place
py::str loc_name(int64_t oi, int64_t off) { return py::percent(py::str("loc_%d_%04X"), oi, off); }
py::str sub_name(const Place& k) { return py::percent(py::str("sub_%d_%04X"), std::get<0>(k), std::get<1>(k)); }

// the shared service-name tables (vxd_services.load() at import time)
const vxd_services::Services& svc() {
    static const vxd_services::Services s = vxd_services::load();
    return s;
}

}  // namespace

// VxD device id -> the DDK .INC that defines its services (so `VxDcall <name>` resolves).
const py::dict<int64_t, py::str> DEVICE_INC = {
    {0x0001, "VMM"},     {0x0003, "VPICD"},   {0x0004, "VDMAD"},   {0x0005, "VTD"},      {0x0006, "V86MMGR"}, {0x0007, "PAGESWAP"},
    {0x000B, "VSD"},     {0x000C, "VMD"},     {0x000D, "VKD"},     {0x000E, "VCD"},      {0x0010, "BLOCKDEV"}, {0x0011, "VMCPD"},
    {0x0014, "VNETBIOS"}, {0x0015, "DOSMGR"}, {0x0016, "VMPOLL"},  {0x0017, "SHELL"},    {0x001E, "VXDLDR"},  {0x0021, "PAGEFILE"},
    {0x0028, "VPOWERD"}, {0x002C, "VWIN32"},  {0x002D, "VCOMM"},   {0x0033, "CONFIGMG"}, {0x0036, "VFBACKUP"}, {0x0038, "VCOND"},
    {0x0040, "IFSMGR"},  {0x0442, "VTDAPI"}};

py::list<py::str> device_incs(const VxDDisasm& D, int64_t oi) {
    py::set<py::str> incs = {py::str("VMM")};
    for (const auto& kv : D.insns.items()) {
        const Row& row = std::get<1>(kv);
        if (std::get<0>(std::get<0>(kv)) == oi && row.vxdcall) {
            const int64_t dev = std::get<0>(*row.vxdcall);
            if (DEVICE_INC.contains(dev)) {
                incs.add(DEVICE_INC.at(dev));
            }
        }
    }
    return py::sorted(incs);
}

std::optional<py::str> svc_macro(int64_t dev, int64_t svcn) {
    if (!DEVICE_INC.contains(dev)) {                     // no .INC to define the service @@equate ->
        return std::nullopt;                             //  keep the raw int 20h+DWORD (e.g. VDD)
    }
    const auto devname = dis::VXD_DEVICES.get(dev);
    std::optional<py::str> name;
    if (devname && !devname->empty()) {
        name = svc().name(*devname, svcn & 0x7FFF);
    }
    if (!name || name->empty()) {
        return std::nullopt;
    }
    const bool jmp = (svcn & 0x8000) != 0;               // DL_Jmp_Mask -> VMMJmp/VxDJmp, not call
    const char* verb;
    if (dev == 0x0001) {
        verb = jmp ? "VMMJmp" : "VMMcall";
    } else {
        verb = jmp ? "VxDJmp" : "VxDcall";
    }
    return py::percent(py::str("        %s %s"), py::str(verb), *name);
}

py::str ddb_macro(const VxDDisasm& D, const le::Ddb& db, bool loadable) {
    py::list<py::str> args = {db.name, py::percent(py::str("%d"), db.major), py::percent(py::str("%d"), db.minor), db.name + py::str("_Control"),
                              py::percent(py::str("0%04Xh"), db.device_id), py::percent(py::str("0%08Xh"), db.init_order)};
    if (loadable) {
        const int64_t rva = db.rva, oi = db.obj;
        auto _proc = [&](int64_t fo) -> py::str {            // the handler label a DDB pointer field targets
            const auto f = D.fx.get(Place(oi, rva + fo));
            if (f && is_internal_(*f)) {
                return label_at(D, Place(internal_(*f).obj, target_key_(internal_(*f))));
            }
            return py::str("");
        };
        const py::str v86 = _proc(0x1c), pm = _proc(0x20), ref = _proc(0x2c);
        if (!v86.empty() || !pm.empty() || !ref.empty()) {   // append trailing args only as far as needed,
            args.append(v86);                                //  leaving earlier ones blank (macro default)
            if (!pm.empty() || !ref.empty()) {
                args.append(pm);
                if (!ref.empty()) {
                    args.append(ref);   // bare label: the macro wraps it (Ref_Data_Offset EQU <OFFSET32 ..>)
                }
            }
        }
    }
    return py::str("Declare_Virtual_Device ") + py::join(py::str(", "), args);
}

std::optional<ServiceTable> service_table(const VxDDisasm& D, const PlaceSet& emittable) {
    if (!D.ddb) {
        return std::nullopt;
    }
    const le::Ddb& db = *D.ddb;
    int64_t obj = db.obj;
    const int64_t r = db.rva;
    // Service_Table_Ptr (+0x30) is stored as a zero-addend relocation on many VxDs: the raw dword is 0 and the real base
    // lives in the fixup. Prefer the fixup, fall back to the raw dword. The table's object may differ from the DDB's, so
    // read Size from the DDB object and index the table in the object the fixup names.
    const auto st = D.fsite(obj, r + 0x30);
    std::optional<int64_t> ptr;
    if (st && is_internal_(*st)) {
        obj = internal_(*st).obj;
        ptr = internal_(*st).target;
    } else {
        ptr = py::from_bytes(D.code.at(obj).slice(r + 0x30, r + 0x34), "little", false);
    }
    const py::bytes& code = D.code.at(obj);
    const int64_t count = py::from_bytes(D.code.at(db.obj).slice(r + 0x34, r + 0x38), "little", false);
    if (!(ptr && 0 < count && count < 8192 && *ptr + 4 * count <= code.size())) {
        return std::nullopt;
    }
    const int64_t p = *ptr;
    const vxd_services::Services& sv = svc();
    ServiceTable out;
    py::set<py::str> used;
    for (int64_t i = 0; i < count; ++i) {
        const auto f = D.fx.get(Place(obj, p + 4 * i));
        if (!f) {
            if (py::from_bytes(code.slice(p + 4 * i, p + 4 * i + 4), "little", false) == 0) {
                out.names.append(py::str("RESERVED"));       // genuine null slot -> macro emits dd 0
                continue;
            }
            return std::nullopt;                             // non-zero without a fixup -> unexpected, keep raw
        }
        const le::InternalFixup& fi = need_internal_(*f, "obj");
        const Place tgt(fi.obj, target_key_(fi));
        if (!in_(emittable, std::get<0>(tgt), std::get<1>(tgt))) {   // handler left mid-instruction -> can't be labelled
            return std::nullopt;
        }
        auto nm0 = sv.name(db.name, i);                               // key off the DDB name
        const py::str nm = (nm0 && !nm0->empty()) ? *nm0 : py::percent(py::str("%s_Service_%03d"), db.name, i);
        if (used.contains(nm)) {                                      // two services with the identical name -> bail
            return std::nullopt;
        }
        used.add(nm);
        out.names.append(nm);
        if (nm != label_at(D, tgt)) {                                 // emit nm as an extra label at the handler unless it
            out.extra.setdefault(tgt, py::list<py::str>()).append(nm);   //  already IS the disasm label there
        }
    }
    out.loc = {obj, p, count};
    return out;
}

std::optional<py::str> _branch_label(const VxDDisasm& D, int64_t oi, int64_t a, const Row& row, const PlaceSet& emittable) {
    const auto t = dis::_direct_target_row(row);
    if (t && in_(emittable, oi, *t)) {
        return D.labels.get(Place(oi, *t));
    }
    for (int64_t k = a + 1; k < a + row.size; ++k) {
        const auto f = D.fx.get(Place(oi, k));
        if (f && is_internal_(*f) && internal_(*f).target && in_(emittable, internal_(*f).obj, *internal_(*f).target)) {
            return D.labels.get(Place(internal_(*f).obj, *internal_(*f).target));
        }
    }
    return std::nullopt;
}

namespace {
// int(row["ops"].split(",")[1].strip(), 0); ValueError for text that is no number
int64_t second_operand_int(const py::str& ops) {
    const py::list<py::str> parts = py::split(ops, py::str(","));
    return py::int_parse(parts[1].strip(), 0);
}
bool ops_start_eax(const py::str& ops) {
    return ops.replace(py::str(" "), py::str("")).startswith(py::str("eax,"));
}
bool is_stub_mnem(const py::str& m) {
    return m == "clc" || m == "stc" || m == "cmc" || m == "ret" || m == "retn" || m == "retf";
}
}  // namespace

std::tuple<Handlers, PlaceSet> control_dispatch(const VxDDisasm& D, const PlaceSet& emittable) {
    if (!D.ddb) {
        return {Handlers(), PlaceSet()};
    }
    const le::Ddb& db = *D.ddb;
    std::optional<Place> cp;
    for (const auto& kv : D.labels.items()) {
        if (std::get<1>(kv) == db.name + py::str("_Control")) {
            cp = std::get<0>(kv);
            break;
        }
    }
    if (!cp) {
        return {Handlers(), PlaceSet()};
    }
    const py::dict<int64_t, py::str> msgs = vxd_symbols::control_messages();
    const int64_t oi = std::get<0>(*cp), a0 = std::get<1>(*cp);
    // --- jump-table form ---
    int64_t a = a0;
    int64_t base = 0;
    std::optional<Place> deflt;                              // the range-check (jae/ja) branch target: the do-nothing stub
    for (int k8 = 0; k8 < 8; ++k8) {                         // the many UNHANDLED messages fall through to. Real handlers
        if (!D.insns.contains(Place(oi, a))) {               // (even one SHARED by several messages) are kept; only this
            break;                                           // stub is dropped.
        }
        const Row& row = D.insns.at(Place(oi, a));
        if (row.mnem == "sub" && ops_start_eax(row.ops)) {
            try {
                base = second_operand_int(row.ops);
            } catch (const py::ValueError&) {
            }
        }
        if (!deflt && (row.mnem == "jae" || row.mnem == "ja" || row.mnem == "jnb" || row.mnem == "jnbe" || row.mnem == "jbe" ||
                       row.mnem == "jb" || row.mnem == "jg" || row.mnem == "jge")) {
            const auto t = dis::_direct_target_row(row);
            if (t) {
                deflt = Place(oi, *t);
            }
        }
        if (row.mnem == "jmp" && row.ops.contains(py::str("*4"))) {
            PlaceSet skip;                                   // prologue bytes
            for (int64_t k = a0; k < a + row.size; ++k) {
                skip.insert(Place(oi, k));
            }
            std::optional<py::list<Place>> jt;
            for (int64_t k = a; k < a + row.size; ++k) {
                const auto f = D.fx.get(Place(oi, k));
                if (f && is_internal_(*f) && internal_(*f).obj == oi) {
                    const Place key(internal_(*f).obj, target_key_(internal_(*f)));
                    const auto j = D.jtables.get(key);
                    if (j && !j->empty()) {
                        jt = *j;
                        for (int64_t b = 0; b < j->size() * 4; ++b) {   // table bytes
                            skip.insert(Place(oi, std::get<1>(key) + b));
                        }
                    }
                    break;
                }
            }
            int64_t b = a + row.size;                        // trailing unhandled stub
            while (D.insns.contains(Place(oi, b)) && is_stub_mnem(D.insns.at(Place(oi, b)).mnem)) {
                for (int64_t k = b; k < b + D.insns.at(Place(oi, b)).size; ++k) {
                    skip.insert(Place(oi, k));
                }
                b += D.insns.at(Place(oi, b)).size;
            }
            Handlers out;
            if (jt) {
                int64_t i = 0;
                for (const Place& tk : *jt) {
                    const int64_t msg = i + base;
                    // keep every real handler, INCLUDING one shared by several messages; drop only the default stub. Fall
                    // back to the old uniqueness test when the stub can't be identified.
                    const bool keep = deflt ? (tk != *deflt) : (jt->count(tk) == 1);
                    if (msgs.contains(msg) && keep && D.insns.contains(tk)) {
                        const py::str lbl = label_at(D, tk);
                        out.append({msgs.at(msg), !lbl.empty() ? lbl : loc_name(std::get<0>(tk), std::get<1>(tk))});
                    }
                    i += 1;
                }
            }
            if (!out.empty()) {
                return {out, skip};
            }
            return {Handlers(), PlaceSet()};
        }
        a += row.size;
    }
    // --- compare-chain form: cmp eax,N ; je handler (repeated) ---
    a = a0;
    PlaceSet skip;
    Handlers out;
    py::set<int64_t> seen;
    while (D.insns.contains(Place(oi, a))) {
        const Row& row = D.insns.at(Place(oi, a));
        if (row.mnem != "cmp" || !ops_start_eax(row.ops)) {
            break;
        }
        int64_t m;
        try {
            m = second_operand_int(row.ops);
        } catch (const py::ValueError&) {
            break;
        }
        if (!D.insns.contains(Place(oi, a + row.size))) {
            break;
        }
        const Row& jr = D.insns.at(Place(oi, a + row.size));
        if (jr.mnem != "je" && jr.mnem != "jz") {
            break;
        }
        const auto lbl = _branch_label(D, oi, a + row.size, jr, emittable);
        if (!msgs.contains(m) || !lbl || lbl->empty() || seen.contains(m)) {
            return {Handlers(), PlaceSet()};                 // can't reproduce this arm -> emit verbatim
        }
        seen.add(m);
        out.append({msgs.at(m), *lbl});
        for (int64_t k = a; k < a + row.size + jr.size; ++k) {
            skip.insert(Place(oi, k));
        }
        a += row.size + jr.size;
    }
    if (out.size() < 2) {                                    // a lone cmp/je is not a dispatch chain
        return {Handlers(), PlaceSet()};
    }
    while (D.insns.contains(Place(oi, a)) && (is_stub_mnem(D.insns.at(Place(oi, a)).mnem) || D.insns.at(Place(oi, a)).mnem == "jmp")) {
        const Row& r = D.insns.at(Place(oi, a));             // trailing default (fall-through / jmp)
        for (int64_t k = a; k < a + r.size; ++k) {
            skip.insert(Place(oi, k));
        }
        a += r.size;
        if (r.mnem == "jmp") {
            break;
        }
    }
    return {out, skip};
}

const py::dict<py::str, py::str> _SEG_MACRO = {{py::str("LCODE"), py::str("VxD_LOCKED_CODE")}, {py::str("ICODE"), py::str("VxD_ICODE")},
                                               {py::str("PCODE"), py::str("VxD_PAGEABLE_CODE")}, {py::str("MCODE"), py::str("VxD_LOCKED_CODE")}};

// mnemonics whose _masm_line text the period DDK ML (MASM 6.11) rejects or re-encodes -- almost all are byte runs the
// linear disasm reads as exotic/system instructions where the real bytes are data.
const py::set<py::str> _RAW_MNEMS = {py::str("aam"),    py::str("aad"),     py::str("lcall"),   py::str("ljmp"),   py::str("lar"),     py::str("lsl"),
                                     py::str("int3"),   py::str("int1"),    py::str("ud0"),     py::str("ud1"),    py::str("ud2"),     py::str("salc"),
                                     py::str("les"),    py::str("lds"),     py::str("lfs"),     py::str("lgs"),    py::str("lss"),
                                     py::str("fisttp"), py::str("fxsave"),  py::str("fxrstor"),   // SSE-era x87 the 1994 assembler predates (also data)
                                     py::str("lidt"),   py::str("lgdt"),    py::str("sidt"),    py::str("sgdt"),   py::str("lldt"),    py::str("sldt"),   // capstone drops the `fword ptr` size
                                     py::str("sysret"), py::str("sysenter"), py::str("sysexit")};                                     // post-1994 fast-syscall (data misread)

const py::set<int64_t> _PREFIX_BYTES = {0x66, 0x67, 0xF0, 0xF2, 0xF3, 0x2E, 0x26, 0x36, 0x3E, 0x64, 0x65};
const py::set<py::str> _SHIFTS = {py::str("rcl"), py::str("rcr"), py::str("rol"), py::str("ror"),
                                  py::str("shl"), py::str("shr"), py::str("sal"), py::str("sar")};

bool _shift_no_count(const Row& row) {
    return _SHIFTS.contains(row.mnem) && !row.ops.contains(py::str(","));
}

const py::str _XMM_MM = py::str("\\b(?:xmm|mm)[0-7]\\b");

bool _ml611_unknown(const Row& row) {
    static const py::re::Pattern pat = py::re::compile(_XMM_MM);
    const py::str& m = row.mnem;
    return m.startswith(py::str("cmov")) || m.startswith(py::str("fcmov")) || pat.search(row.ops).has_value();
}

bool _x87_stack(const Row& row) {
    return row.mnem.slice(0, 1) == py::str("f") && row.ops.contains(py::str("st("));
}

bool _indirect_branch(const Row& row) {
    return (row.mnem == "jmp" || row.mnem == "call") && row.ops.contains(py::str("["));
}

bool _cap_size(const Row& row) {
    return row.ops.contains(py::str("xword")) || row.ops.contains(py::str("tword"));
}

bool _addr16(const Row& row) {
    const py::bytes head = row.raw.slice(0, 4);
    for (int64_t b : head) {
        if (b == 0x67) {
            return true;
        }
        if (!_PREFIX_BYTES.contains(b)) {
            break;
        }
    }
    return false;
}

bool _disp0(const Row& row) {
    static py::dict<py::bytes, bool> cache;                  // _DISP0_CACHE: by raw bytes -- detail-disasm is costly
    const py::bytes raw = row.raw;
    const auto hit = cache.get(raw);
    if (hit) {
        return *hit;
    }
    bool res = false;
    try {
        std::shared_ptr<platforms::Decoder> md = platforms::decoder(32, py::str("x86"), true);   // (platforms.decoder(32, detail=True))
        size_t n = 0;
        cs_insn* found = md->raw(reinterpret_cast<const uint8_t*>(raw.raw().data()), static_cast<size_t>(raw.size()), 0, 1, n);
        if (n != 0) {
            const cs_insn* insn = found;
            if (insn->detail) {
                const int mod = (insn->detail->x86.modrm >> 6) & 3;
                if (mod == 1 || mod == 2) {
                    for (int k = 0; k < insn->detail->x86.op_count; ++k) {
                        const cs_x86_op& op = insn->detail->x86.operands[k];
                        if (op.type == X86_OP_MEM && op.mem.disp == 0) {
                            res = true;
                        }
                    }
                }
            }
            md->release(found, n);
        }
    } catch (const py::ImportError&) {
        throw;                                               // (a missing decoder is not a CsError)
    } catch (const std::exception&) {
        res = false;
    }
    cache.set(raw, res);
    return res;
}

bool _opnd16(const Row& row) {
    const py::bytes head = row.raw.slice(0, 4);
    for (int64_t b : head) {
        if (b == 0x66) {
            return true;
        }
        if (!_PREFIX_BYTES.contains(b)) {
            break;
        }
    }
    return false;
}

int64_t _ddb_size(const VxDDisasm& D, const le::Ddb& db) {
    const py::bytes& code = D.code.at(db.obj);
    const int64_t r = db.rva;
    if (code.slice(r + 0x3C, r + 0x40) == py::bytes("verP")) {   // 'Prev' packed by `dd` -> little-endian bytes
        const int64_t sz = py::from_bytes(code.slice(r + 0x40, r + 0x44), "little", false);
        if (0x34 <= sz && sz <= 0x80) {
            return sz;
        }
        return 0x50;   // modern DDB (Prev marker present) but its DDB_Size field is zeroed in the image (the loader fills it)
    }
    return 0x34;
}

py::list<py::str> _all_incs(const VxDDisasm& D) {
    py::set<py::str> incs;
    for (const le::Object& o : D.objs.values()) {
        if (o.exec) {
            incs.update(device_incs(D, o.idx));
        }
    }
    incs.discard(py::str("VMM"));                            // VMM.INC MUST come first: it defines the
    py::list<py::str> out = {py::str("VMM")};                //  macros (Begin_Service_Table, ...) the
    out.extend(py::sorted(incs));                            //  other device INCs invoke at include time
    return out;
}

py::list<py::str> _db_data(const py::bytes& bs, int64_t width) {
    auto printable = [](int64_t b) { return 0x20 <= b && b <= 0x7E; };
    py::list<py::str> out;
    const int64_t n = bs.size();
    int64_t i = 0;
    while (i < n) {
        if (printable(bs[i])) {                              // a printable run of >=4 -> string literal
            int64_t j = i;
            while (j < n && printable(bs[j])) {
                j += 1;
            }
            if (j - i >= 4) {
                for (int64_t c = i; c < j; c += width) {
                    const py::str txt = bs.slice(c, std::min(c + width, j)).decode("latin1").replace(py::str("'"), py::str("''"));
                    out.append(py::percent(py::str("        db  '%s'"), txt));
                }
                i = j;
                continue;
            }
        }
        int64_t j = i;                                       // else a hex chunk up to the next such run
        while (j < n) {
            if (printable(bs[j])) {
                int64_t k = j;
                while (k < n && printable(bs[k])) {
                    k += 1;
                }
                if (k - j >= 4) {
                    break;
                }
            }
            j += 1;
        }
        for (int64_t c = i; c < j; c += 16) {
            py::list<py::str> parts;
            for (int64_t b : bs.slice(c, std::min<int64_t>(c + 16, j))) {
                parts.append(py::percent(py::str("0%02Xh"), b));
            }
            out.append(py::str("        db  ") + py::join(py::str(", "), parts));
        }
        i = j;
    }
    return out;
}

const py::dict<int64_t, py::str> _JCC = {{0x80, "jo"}, {0x81, "jno"}, {0x82, "jb"}, {0x83, "jae"}, {0x84, "je"},  {0x85, "jne"}, {0x86, "jbe"}, {0x87, "ja"},
                                         {0x88, "js"}, {0x89, "jns"}, {0x8a, "jp"}, {0x8b, "jnp"}, {0x8c, "jl"},  {0x8d, "jge"}, {0x8e, "jle"}, {0x8f, "jg"}};

// a branch operand that is a bare number (optionally SHORT/NEAR PTR-pinned) -- ML's "jump destination must specify a label"
const py::str _NUM_TARGET = py::str("^(?:SHORT |NEAR PTR )?[0-9][0-9A-Fa-f]*h?$");

std::tuple<py::str, py::str> obj_seg(int64_t oi, int64_t flags, const std::optional<int64_t>& ddb_obj) {
    // The object that carries the DDB uses class 'LCODE' -- the same class the DDK macros give the DDB/service-table segments
    // (_LDATA/_LTEXT, all 'LCODE'), so wlink merges the recovered locked code with the macro-emitted DDB+service table into ONE
    // locked object. Every other object keeps a unique class so it stays a separate LE object.
    if (ddb_obj && oi == *ddb_obj) {
        return {py::str("_LTEXT"), py::str("LCODE")};
    }
    return {py::percent(py::str("VSEG%02d"), oi), py::percent(py::str("S%02d%s"), oi, py::str((flags & 0x0004) ? "CODE" : "DATA"))};
}

std::optional<py::str> _fx_label(const VxDDisasm& D, int64_t oi, int64_t k) {
    const auto f = D.fx.get(Place(oi, k));
    if (!f || !is_internal_(*f) || !internal_(*f).target) {
        return std::nullopt;
    }
    return D.labels.get(Place(internal_(*f).obj, *internal_(*f).target));
}

std::optional<py::str> _dd_target_expr(const VxDDisasm& D, int64_t tobj, const std::optional<int64_t>& toff_opt, const LoadCtx* ctx) {
    if (!toff_opt) {
        throw py::TypeError("unsupported operand type(s) for &: 'NoneType' and 'int'");
    }
    const int64_t toff = *toff_opt;
    const py::str alias((toff & 0x80000000) ? " + 80000000h" : "");
    const int64_t base = toff & 0x7FFFFFFF;
    if (ctx && tobj == ctx->ddb_obj && ctx->ddb_rva <= base && base < ctx->ddb_hi) {
        const int64_t d = base - ctx->ddb_rva;
        return py::percent(py::str("offset %s_DDB%s%s"), ctx->name, d ? py::percent(py::str(" + %02Xh"), d) : py::str(""), alias);
    }
    if (ctx && in_(ctx->skip, tobj, base)) {                 // target lands in a macro-absorbed region (the service table /
        return std::nullopt;                                 //  control-dispatch table the Begin_Service_Table /
        //  Begin_Control_Dispatch macros regenerate): no loc_ label is emitted there, so a symbolic `offset loc_..' would be
        //  undefined (A2006). Byte-preserve the pointer instead -- it loses its relocation, but the region is macro-owned.
    }
    // A target BEYOND the object's vsize is a reference to obj_base + a large offset (e.g. a VxD's DOS/V86 memory window at
    // obj+0x400000). No loc_ label exists that far in; anchor on the object's base label (obj:0) plus the full offset.
    for (const le::Object& o : D.v->objects) {
        if (o.idx == tobj && base >= o.vsize) {
            const py::str anchor = label_at(D, tobj, 0);
            if (!anchor.empty()) {
                return py::percent(py::str("offset %s + %Xh%s"), anchor, base, alias);
            }
            return std::nullopt;
        }
    }
    const py::str lab = label_at(D, tobj, base);
    if (!lab.empty()) {
        return py::percent(py::str("offset %s%s"), lab, alias);
    }
    return std::nullopt;
}

std::optional<py::list<py::str>> _loadable_insn(const VxDDisasm& D, int64_t oi, int64_t a, const Row& row, const LoadCtx* ctx) {
    py::list<int64_t> fixes;
    for (int64_t k = a; k < a + row.size; ++k) {
        if (D.fx.contains(Place(oi, k))) {
            fixes.append(k);
        }
    }
    if (fixes.empty() || row.vxdcall) {
        return std::nullopt;
    }
    const py::bytes& code = D.code.at(oi);
    const int64_t op = code[a];
    if (row.size == 5 && (op == 0xE8 || op == 0xE9) && fixes.size() == 1 && fixes[0] == a + 1) {   // self-relative near branch
        const auto lab = _fx_label(D, oi, a + 1);
        if (lab && !lab->empty()) {
            return py::list<py::str>{py::percent(py::str("        %-6s %s"), py::str(op == 0xE8 ? "call" : "jmp"), *lab)};
        }
        return std::nullopt;
    }
    if (row.size == 6 && op == 0x0F && _JCC.contains(code[a + 1]) && fixes.size() == 1 && fixes[0] == a + 2) {
        const auto lab = _fx_label(D, oi, a + 2);
        if (lab && !lab->empty()) {
            return py::list<py::str>{py::percent(py::str("        %-6s %s"), _JCC.at(code[a + 1]), *lab)};
        }
        return std::nullopt;
    }
    py::dict<int64_t, py::str> exprs;                        // absolute 32-bit offset field(s)
    for (int64_t k : fixes) {
        const le::Fixup f = D.fx.at(Place(oi, k));
        std::optional<py::str> e;
        if (is_internal_(f)) {
            e = _dd_target_expr(D, internal_(f).obj, internal_(f).target, ctx);
        }
        if (!e) {
            return std::nullopt;
        }
        exprs.set(k, *e);
    }
    py::list<py::str> out;
    int64_t prev = a;
    for (int64_t k : fixes) {
        if (k > prev) {
            out.append(vxd_masm::_db(code.slice(prev, k)));
        }
        out.append(py::percent(py::str("        dd  %s"), exprs.at(k)));
        prev = k + 4;
    }
    if (prev < a + row.size) {
        out.append(vxd_masm::_db(code.slice(prev, a + row.size)));
    }
    return out;
}

py::list<py::str> _db_split_labels(const py::bytes& code, int64_t a, int64_t b, const py::dict<int64_t, py::str>& labels) {
    py::list<py::str> out;
    int64_t run = a;
    for (int64_t off : py::sorted(labels.keys())) {
        if (!(a <= off && off < b)) {
            continue;
        }
        if (off > run) {
            out.append(vxd_masm::_db(code.slice(run, off)));
        }
        out.append(labels.at(off) + py::str(" label near"));  // `near' (a code address) not `byte': a branch target
        run = off;                                            //  needs a near label (`je <byte>' -> A2024); `dd
        //  offset' and data pointers accept a near label too. Emits no bytes, so byte-exact is unchanged.
    }
    if (run < b) {
        out.append(vxd_masm::_db(code.slice(run, b)));
    }
    return out;
}

py::list<py::str> _loadable_data(const VxDDisasm& D, int64_t oi, int64_t a, int64_t b, const LoadCtx* ctx) {
    py::list<py::str> out;
    int64_t run = a, k = a;
    const py::bytes& code = D.code.at(oi);
    // Interior labels INSIDE this data span (a fixup target elsewhere that lands here -- common when the span is an in-place
    // DDB / service-table block holding a pointer table). _db_data does not define labels, so without this those
    // `dd offset loc_..' / `call [loc_..]' are undefined (A2006). Emit `<name> label near' at each, splitting the readable db
    // runs -- byte-preserving. Offset `a' is excluded: the caller emits it.
    py::dict<int64_t, py::str> dl;
    for (const Place& key : D.labels) {
        if (std::get<0>(key) == oi && a < std::get<1>(key) && std::get<1>(key) < b) {
            dl.set(std::get<1>(key), D.labels.at(key));
        }
    }
    auto _flush = [&](int64_t lo, int64_t hi) {              // emit code[lo:hi] as readable data, inserting interior
        int64_t r = lo;                                      //  `label near' at any dl offset inside (lo,hi) -- never at
        for (int64_t off : py::sorted(dl.keys())) {          //  a dd interior (run jumps past those)
            if (!(lo < off && off < hi)) {
                continue;
            }
            if (off > r) {
                out.extend(_db_data(code.slice(r, off)));
            }
            out.append(dl.at(off) + py::str(" label near"));
            r = off;
        }
        if (r < hi) {
            out.extend(_db_data(code.slice(r, hi)));
        }
    };

    while (k < b) {
        const auto fo = D.fx.get(Place(oi, k));
        if (fo && is_internal_(*fo)) {
            const le::InternalFixup& f = internal_(*fo);
            if (f.srctype == 8 && k >= a + 1 && (code[k - 1] == 0xE8 || code[k - 1] == 0xE9) && k + 4 <= b) {
                const py::str lab = label_at(D, f.obj, (f.target ? *f.target : 0) & 0x7FFFFFFF);
                if (!lab.empty()) {
                    if (k - 1 > run) {
                        _flush(run, k - 1);
                    }
                    if (dl.contains(k - 1)) {                // label on the branch opcode byte
                        out.append(dl.at(k - 1) + py::str(" label near"));
                    }
                    out.append(py::percent(py::str("        %-6s %s"), py::str(code[k - 1] == 0xE8 ? "call" : "jmp"), lab));
                    k += 4;
                    run = k;
                    continue;
                }
            }
            // a two-byte jcc (0F 8x rel32) self-relative branch in a data gap -> emit the jcc, not dd offset
            if (f.srctype == 8 && k >= a + 2 && code[k - 2] == 0x0F && _JCC.contains(code[k - 1]) && k + 4 <= b) {
                const py::str lab = label_at(D, f.obj, (f.target ? *f.target : 0) & 0x7FFFFFFF);
                if (!lab.empty()) {
                    if (k - 2 > run) {
                        _flush(run, k - 2);
                    }
                    if (dl.contains(k - 2)) {
                        out.append(dl.at(k - 2) + py::str(" label near"));
                    }
                    out.append(py::percent(py::str("        %-6s %s"), _JCC.at(code[k - 1]), lab));
                    k += 4;
                    run = k;
                    continue;
                }
            }
            const auto expr = _dd_target_expr(D, f.obj, f.target, ctx);
            if (expr && k + 4 <= b) {
                if (k > run) {
                    _flush(run, k);
                }
                if (dl.contains(k)) {                        // label at the dd (pointer-table entry) start
                    out.append(dl.at(k) + py::str(" label near"));
                }
                out.append(py::percent(py::str("        dd  %s"), *expr));
                k += 4;
                run = k;
                continue;
            }
        }
        k += 1;
    }
    if (run < b) {
        _flush(run, b);
    }
    return out;
}

py::str emit_idiomatic(VxDDisasm& D, bool loadable) {
    const std::optional<le::Ddb> db = D.ddb;
    const py::str name = db ? db->name : py::str("DEVICE");
    // a reconstructed label (often from a .SYM) can collide with a symbol the DDK macros generate -- <name>_DDB
    // (Declare_Virtual_Device) or <name>_Service_Table (Begin_Service_Table). Rename it to the offset-based sub_ name
    // (consistently, in D.labels, so its references follow) or ML reports A2005 redefinition.
    for (const auto& kv : D.labels.items()) {
        if (std::get<1>(kv) == name + py::str("_DDB") || std::get<1>(kv) == name + py::str("_Service_Table")) {
            D.labels.set(std::get<0>(kv), sub_name(std::get<0>(kv)));
        }
    }
    py::dict<Place, py::str> force_lbl;                      // (oi,off) -> label, for a fixup target that
    py::dict<Place, int64_t> fx_interior;                    // (oi,off) inside a fixup-src dword -> its start
    std::optional<LoadCtx> ldctx;                            // lands MID-INSTRUCTION (a data byte the disassembler swept
    if (loadable) {                                          // into an instruction). loadable: every internal
        vxd_link::_label_fixup_targets(D);                   // fixup target needs a resolvable operand;
        if (db) {                                            // one landing in the DDB struct resolves to _DDB
            LoadCtx c;
            c.name = name;
            c.ddb_obj = db->obj;
            c.ddb_rva = db->rva;
            c.ddb_hi = db->rva + _ddb_size(D, *db);
            ldctx = c;
        }
        std::unordered_map<Place, int64_t, dis::PlaceHash> interior_of;   // fixup target needs a label the symbolic
        for (const auto& kv : D.insns.items()) {              // emit can reference; a mid-instruction
            const int64_t o2 = std::get<0>(std::get<0>(kv)), a2 = std::get<1>(std::get<0>(kv));
            for (int64_t k = a2 + 1; k < a2 + std::get<1>(kv).size; ++k) {   //  target forces its instruction to raw db
                interior_of[Place(o2, k)] = a2;              //  so the label can sit at the exact byte
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
            const Place tgt(internal_(f).obj, *internal_(f).target & 0x7FFFFFFF);   // mask the 0x80000000 high-linear-alias bit;
            const auto ov = ovs.find(std::get<0>(tgt));
            if (ov != ovs.end() && std::get<1>(tgt) >= ov->second) {                //  the low 31 bits are the real object offset
                D.labels.setdefault(Place(std::get<0>(tgt), 0), loc_name(std::get<0>(tgt), 0));   // beyond-object: anchor the ref
                continue;                                                           //  on the object-base label (+addend)
            }
            const py::str have = label_at(D, tgt);
            const py::str lab = !have.empty() ? have : loc_name(std::get<0>(tgt), std::get<1>(tgt));
            D.labels.setdefault(tgt, lab);
            if (interior_of.find(tgt) != interior_of.end()) {   // target lands MID-INSTRUCTION -> force its instruction to raw
                force_lbl.set(tgt, lab);                     //  db so the label sits at the exact byte. Do this even when
                //  _label_fixup_targets already assigned the label (a misdecoded pointer table decodes run-to-run into different
                //  instruction boundaries, so a target skipped as a `data byte' there can be mid-instruction here). Idempotent.
            }
        }
        // NARROW overlapping-pointer case: a LABEL strictly inside an internal-fixup SOURCE dword [S+1,S+4) -- two relocated
        // pointers overlap in a mis-decoded data region. The interior label can't be emitted inline (that splits the dd and
        // drops a relocation). Record it, anchor the dword start, and emit it later as `EQU <S-label> + delta'.
        for (const auto& kv : D.fx.items()) {
            const int64_t soi = std::get<0>(std::get<0>(kv)), soff = std::get<1>(std::get<0>(kv));
            const le::Fixup& f = std::get<1>(kv);
            if (!is_internal_(f)) {
                continue;
            }
            if (soff < 0) {
                continue;        // a fixup-source dword straddling IN from the previous object (recorded at a negative offset
                //  here): its dword start lives in that other object, so there is no valid `loc_<obj>_<off>` anchor here.
            }
            py::bytes code;
            if (D.code.contains(soi)) {
                code = D.code.at(soi);
            }
            // The emittable ANCHOR of a fixup unit: for a data pointer (srctype 7) it is the dword start; for a self-relative
            // BRANCH (srctype 8) it is the branch OPCODE (E8/E9 one byte before the rel32, or 0F 8x two bytes before) --
            // because _loadable_data re-emits the whole branch from the opcode. Any LABEL inside the unit (but not at the
            // anchor) is emitted as an EQU alias.
            int64_t anchor = soff;
            if (internal_(f).srctype == 8) {
                if (soff >= 1 && soff - 1 < code.size() && (code[soff - 1] == 0xE8 || code[soff - 1] == 0xE9)) {
                    anchor = soff - 1;
                } else if (soff >= 2 && soff - 1 < code.size() && code[soff - 2] == 0x0F && _JCC.contains(code[soff - 1])) {
                    anchor = soff - 2;
                }
            }
            for (int64_t o = anchor + 1; o < soff + 4; ++o) {
                if (D.labels.contains(Place(soi, o)) && !fx_interior.contains(Place(soi, o))) {
                    fx_interior.set(Place(soi, o), anchor);
                    D.labels.setdefault(Place(soi, anchor), loc_name(soi, anchor));
                }
            }
        }
    }
    // base set of offsets that will carry an emitted label -- a label at an instruction start, or on a data byte, but NOT one
    // landing mid-instruction. control_dispatch needs it to only reference handlers we actually emit; the branch gate below
    // needs it minus the macro-absorbed regions.
    std::unordered_map<int64_t, std::unordered_set<int64_t>> starts, interior;
    for (const auto& kv : D.insns.items()) {
        const int64_t o2 = std::get<0>(std::get<0>(kv)), a2 = std::get<1>(std::get<0>(kv));
        starts[o2].insert(a2);
        auto& iv = interior[o2];
        for (int64_t k = a2 + 1; k < a2 + std::get<1>(kv).size; ++k) {
            iv.insert(k);
        }
    }
    PlaceSet emittable;
    for (const Place& k : D.labels) {
        const int64_t o2 = std::get<0>(k), a2 = std::get<1>(k);
        const bool is_start = starts.count(o2) && starts[o2].count(a2);
        const bool is_interior = interior.count(o2) && interior[o2].count(a2);
        if (is_start || !is_interior) {
            emittable.insert(k);
        }
    }
    // Resolve the control dispatch and DDB skip FIRST, so the service table is validated against what the walk will actually
    // emit (a handler inside the dispatch region would be referenced but skipped).
    auto [handlers_all, cp_skip] = control_dispatch(D, emittable);   // ([], set()) unless a recognised jump
    std::optional<Place> cp;                                         //  table / compare chain -> then emit
    if (!handlers_all.empty()) {                                     //  Begin_Control_Dispatch and skip it; else emit the proc verbatim
        for (const auto& kv : D.labels.items()) {
            if (std::get<1>(kv) == name + py::str("_Control")) {
                cp = std::get<0>(kv);
                break;
            }
        }
    }
    if (db && !loadable) {                                           // readable/byte-exact: the macro re-emits
        const int64_t sz = _ddb_size(D, *db);                        //  the DDB, so skip it in the walk. LOADABLE
        for (int64_t k = db->rva; k < db->rva + sz; ++k) {           //  emits it IN-PLACE (below) at its real rva instead -- so a
            cp_skip.insert(Place(db->obj, k));                       //  pointer handed to a VxD service carries the oracle's offset
        }
    }
    PlaceSet avail;                                                  // emittable - cp_skip
    for (const Place& k : emittable) {
        if (cp_skip.find(k) == cp_skip.end()) {
            avail.insert(k);
        }
    }
    const std::optional<ServiceTable> svc_table = service_table(D, avail);   // own exported service table; handlers must be emitted
    const bool has_svc = svc_table.has_value();
    py::list<py::str> svc_labels;
    std::tuple<int64_t, int64_t, int64_t> svc_loc{0, 0, 0};
    py::dict<Place, py::list<py::str>> svc_extra;
    if (has_svc) {
        svc_labels = svc_table->names;
        svc_loc = svc_table->loc;
        svc_extra = svc_table->extra;
    }
    if (has_svc && !loadable) {
        const auto& [so, sp, sc] = svc_loc;                          // skip the raw service-table pointers;
        for (int64_t k = sp; k < sp + 4 * sc; ++k) {                 //  Begin_Service_Table re-emits them (LOADABLE
            cp_skip.insert(Place(so, k));                            //  emits the table in-place, below)
        }
    }
    if (ldctx) {                                                     // let _dd_target_expr raw-emit a pointer whose
        ldctx->skip = cp_skip;                                       //  target sits in a macro-absorbed region
    }
    const LoadCtx* ctxp = ldctx ? &*ldctx : nullptr;
    // .586p (privileged, superset of .386p) so ring-0 486/Pentium ops -- invd/wbinvd/cpuid/cmpxchg/rdmsr/wrmsr/rdtsc -- assemble
    // as themselves instead of hitting A2085 (not-in-current-CPU-mode).
    py::list<py::str> L = {py::str(".586p")};
    for (const py::str& i : _all_incs(D)) {
        L.append(py::str("include ") + i.lower() + py::str(".inc"));
    }
    L.append(py::str(""));
    // EQU aliases for labels that land inside a relocated-pointer dword (overlapping pointers in mis-decoded data): defined here
    // at the TOP so every forward reference resolves, while the dword itself emits as one whole `dd offset' (its interior label
    // is NOT emitted inline). `<interior> EQU <dword-start> + delta`.
    {
        py::list<std::tuple<Place, int64_t>> fi = py::sorted(fx_interior.items());
        for (const auto& [key, start] : fi) {
            const py::str ilab = label_at(D, key);
            const py::str slab = label_at(D, std::get<0>(key), start);
            if (!ilab.empty() && !slab.empty() && ilab != slab) {
                L.append(py::percent(py::str("%s EQU %s + %d"), ilab, slab, std::get<1>(key) - start));
            }
        }
    }
    if (!fx_interior.empty()) {
        L.append(py::str(""));
    }
    const std::vector<le::Object> objs_list = D.v->objects.vec();
    if (loadable) {                                                  // pre-declare each object's own FLAT segment
        for (const le::Object& o : objs_list) {                      // (unique class -> LINK keeps it a separate
            const auto [sn, cl] = obj_seg(o.idx, o.flags, db ? std::optional<int64_t>(db->obj) : std::nullopt);   // object; writable
            L.append(py::percent(py::str("%s SEGMENT DWORD PUBLIC _FLAT '%s'"), sn, cl));                          // data stays writable
            L.append(py::percent(py::str("%s ENDS"), sn));                                                         // is not merged into read-only code)
        }
        L.append(py::str(""));
    }
    if (!svc_labels.empty()) {
        py::set<py::str> svcset;
        for (const py::str& lbl : svc_labels) {
            if (lbl != "RESERVED") {
                svcset.add(lbl);
            }
        }
        // Begin_Service_Table regenerates a PUBLIC caller-thunk `_<service>` for each service; drop any reconstructed label of
        // that exact name (a .SYM ships those thunks) or ML reports A2005.
        for (const auto& kv : D.labels.items()) {
            const py::str& nm2 = std::get<1>(kv);
            if (nm2.startswith(py::str("_")) && svcset.contains(nm2.slice_from(1))) {
                D.labels.set(std::get<0>(kv), sub_name(std::get<0>(kv)));
            }
        }
        L.append(py::percent(py::str("IFNDEF %s_Device_ID"), name));    // the service macro forms each ordinal from
        if (db) {                                                       //  <name>_Device_ID; define it from the DDB
            L.append(py::percent(py::str("%s_Device_ID EQU 0%04Xh"), name, db->device_id));   // since the device's own
        }
        L.append(py::str("ENDIF"));                                     //  INC (which would define it) may be absent
        L.append(py::percent(py::str("Create_%s_Service_Table EQU 1"), name));   // (Create_ before the DDB so Declare_Virtual_
    }
    // The service table + DDB emission. In loadable mode DEFER it to AFTER the object code so the locked object comes out
    // [code][DDB+table] -- byte-for-byte the retail intra-object order. Retail = code first.
    py::list<py::str> tail_macros;
    if (!svc_labels.empty() && !loadable) {
        tail_macros.append(py::percent(py::str("Begin_Service_Table %s"), name));   //  Device fills Service_Table_Ptr/Size. All
        for (const py::str& lbl : svc_labels) {                                     //  LOCAL: every handler is in this one file.
            tail_macros.append(lbl != "RESERVED" ? py::percent(py::str("        %s_Service %s, LOCAL"), name, lbl)
                                                 : py::percent(py::str("        %s_Service RESERVED"), name));
        }
        tail_macros.append(py::percent(py::str("End_Service_Table %s"), name));
        tail_macros.append(py::str(""));
    }
    if (db && !loadable) {
        tail_macros.append(ddb_macro(D, *db, loadable));
        tail_macros.append(py::str(""));
    }
    L.extend(tail_macros);                                              // emit DDB + service table inline (deferring it
    tail_macros = py::list<py::str>();                                  //  past the code regressed VMM device-init)
    // a symbolic branch may resolve only to a label the walk emits: emittable minus the control-proc / DDB / service-table
    // regions the macros absorb. A branch to anything else is byte-preserved.
    PlaceSet emitted_now;                                               // emittable - cp_skip, as it stands now
    for (const Place& k : emittable) {
        if (cp_skip.find(k) == cp_skip.end()) {
            emitted_now.insert(k);
        }
    }
    // a control handler whose target label we do not actually emit (a stub VxD whose messages all dispatch into the
    // control-proc region, or a shared do-nothing stub) would make Control_Dispatch reference an undefined symbol -- drop it,
    // and that message falls through to the macro default.
    py::set<py::str> emitted_names;
    for (const Place& k : emitted_now) {
        emitted_names.add(D.labels.at(k));
    }
    Handlers handlers;
    for (const auto& [m, h] : handlers_all) {
        if (emitted_names.contains(h)) {
            handlers.append({m, h});
        }
    }
    PlaceSet inplace_stops;                                             // loadable: offsets the walk must LAND on so
    if (loadable && db) {                                               //  the in-place DDB / service-table branches
        inplace_stops.insert(Place(db->obj, db->rva));                  //  fire (else a data span coalesces past them
    }                                                                   //  and the public <name>_DDB label is dropped)
    if (loadable && has_svc) {
        inplace_stops.insert(Place(std::get<0>(svc_loc), std::get<1>(svc_loc)));
    }
    // A data byte just before the DDB/table can be mis-decoded as an instruction that SPANS the region start, so the walk jumps
    // over the landing offset. Drop any insn that overlaps an in-place region (start or interior) -- it is data, and the
    // in-place branch emits the region correctly.
    std::vector<std::tuple<int64_t, int64_t, int64_t>> regions;
    if (loadable && db) {
        regions.push_back({db->obj, db->rva, db->rva + _ddb_size(D, *db)});
    }
    if (loadable && has_svc) {
        regions.push_back({std::get<0>(svc_loc), std::get<1>(svc_loc), std::get<1>(svc_loc) + 4 * std::get<2>(svc_loc)});
    }
    for (const auto& [roi, rlo, rhi] : regions) {
        for (const Place& k : D.insns.keys()) {
            if (std::get<0>(k) == roi && std::get<1>(k) < rhi && std::get<1>(k) + D.insns.at(k).size > rlo) {
                D.insns.pop(k);
            }
        }
    }
    for (const le::Object& o : objs_list) {
        const int64_t oi = o.idx;
        py::str segn, segm;
        if (loadable) {                                                 // each object -> its own FLAT segment
            segn = std::get<0>(obj_seg(oi, o.flags, db ? std::optional<int64_t>(db->obj) : std::nullopt));
            L.append(segn + py::str(" SEGMENT"));
            L.append(py::str("   ASSUME cs:FLAT, ds:FLAT, es:FLAT, ss:FLAT"));
        } else {
            // seg_class(flags) of the tree writer: the DDK segment class an LE object's flags name
            py::str cls;
            if (!(o.flags & 0x04)) {                                    // not executable -> data
                cls = (o.flags & 0x10) ? py::str("IDATA") : py::str("LDATA");
            } else if (o.flags & 0x8000) {                              // IOPL -> message-hook table
                cls = py::str("MCODE");
            } else if (o.flags & 0x10) {                                // discardable -> init code
                cls = py::str("ICODE");
            } else if (o.flags & 0x40) {                                // preload+locked
                cls = py::str("LCODE");
            } else {
                cls = py::str("PCODE");                                 // pageable
            }
            segm = _SEG_MACRO.get(cls, py::str("VxD_LOCKED_CODE"));
            L.append(segm + py::str("_SEG"));
        }
        const py::bytes code = D.code.at(oi);
        const int64_t n = code.size();
        std::optional<py::str> cur_proc;
        int64_t a = 0;
        while (a < n) {
            if (cp && Place(oi, a) == *cp) {                            // emit the control dispatch macro block
                L.append(py::percent(py::str("Begin_Control_Dispatch %s"), name));
                for (const auto& [msg, h] : handlers) {
                    L.append(py::percent(py::str("        Control_Dispatch %-22s %s"), msg + py::str(","), h));
                }
                L.append(py::percent(py::str("End_Control_Dispatch %s"), name));
                a += 1;
                continue;
            }
            if (loadable && db && oi == db->obj && a == db->rva) {
                // LOADABLE: emit the DDB struct IN-PLACE at its real rva as relocatable data (dd offset for the pointer fields
                // via _loadable_data), with the exported public label -- so the DDB lands at the oracle offset and a pointer
                // to it (passed to a VxD service) is right.
                const int64_t end = db->rva + _ddb_size(D, *db);
                L.append(py::percent(py::str("public %s_DDB"), name));
                L.append(py::percent(py::str("%s_DDB label byte"), name));
                L.extend(_loadable_data(D, oi, a, end, ctxp));
                a = end;
                continue;
            }
            if (loadable && has_svc && oi == std::get<0>(svc_loc) && a == std::get<1>(svc_loc)) {
                const auto& [so, sp, sc] = svc_loc;                     // the exported service table, in-place: each
                const py::str slbl = label_at(D, so, sp);               //  entry is a fixup -> `dd offset <handler>`
                if (!slbl.empty()) {
                    L.append(slbl + py::str(":"));
                }
                L.extend(_loadable_data(D, oi, a, a + 4 * sc, ctxp));
                a = a + 4 * sc;
                continue;
            }
            if (in_(cp_skip, oi, a)) {                                  // rest of the control proc / dispatch
                a += 1;                                                 //  table is generated by the macros above
                continue;
            }
            if (svc_extra.contains(Place(oi, a))) {
                for (const py::str& xnm : svc_extra.at(Place(oi, a))) {   // extra service-name labels: several services
                    L.append(xnm + py::str(":"));                          //  may share one handler routine
                }
            }
            const py::str lbl = fx_interior.contains(Place(oi, a)) ? py::str("") : label_at(D, oi, a);   // interior label -> EQU, not inline
            if (!lbl.empty() && (lbl.startswith(py::str("sub_")) || lbl.startswith(name + py::str("_"))) && !lbl.endswith(py::str("_Control"))) {
                if (cur_proc) {
                    L.append(py::str("EndProc ") + *cur_proc);
                }
                cur_proc = lbl;
                L.append(py::str("BeginProc ") + lbl);
            } else if (!lbl.empty()) {
                L.append(lbl + py::str(":"));
            }
            if (!D.insns.contains(Place(oi, a))) {                      // data
                int64_t b = a;
                while (b < n && !D.insns.contains(Place(oi, b)) && !in_(cp_skip, oi, b) &&
                       (b == a || (!in_(inplace_stops, oi, b) && (label_at(D, oi, b).empty() || fx_interior.contains(Place(oi, b))) &&
                                   !svc_extra.contains(Place(oi, b))))) {
                    b += 1;
                }
                if (loadable) {                                         // data pointers / jump tables -> real relocations
                    L.extend(_loadable_data(D, oi, a, b, ctxp));
                } else {
                    L.extend(_db_data(code.slice(a, b)));               // readable `db 'text'` for the carved strings
                }
                a = b;
                continue;
            }
            const Row row = D.insns.at(Place(oi, a));
            if (loadable) {
                bool forced = false;
                for (int64_t k = a + 1; k < a + row.size; ++k) {
                    if (force_lbl.contains(Place(oi, k))) {
                        forced = true;
                        break;
                    }
                }
                if (forced) {
                    py::dict<int64_t, py::str> lbls;
                    for (int64_t k = a; k < a + row.size; ++k) {
                        if (force_lbl.contains(Place(oi, k))) {
                            lbls.set(k, force_lbl.at(Place(oi, k)));
                        }
                    }
                    L.extend(_db_split_labels(code, a, a + row.size, lbls));   // target sits mid-instruction
                    a += row.size;
                    continue;
                }
            }
            if (loadable) {                                             // a relocation whose 4-byte field runs PAST
                py::list<int64_t> sp;                                   // this (mis-decoded, short) instruction
                for (int64_t k = a; k < a + row.size; ++k) {            // = a data pointer decoded as
                    if (D.fx.contains(Place(oi, k)) && k + 4 > a + row.size) {   // code. Emit it as ONE 4-byte dd (else the
                        sp.append(k);                                   // `dd offset` would overflow and shift the
                    }                                                   // whole table -> corrupt data -> init fault)
                }
                if (!sp.empty()) {
                    const int64_t k = sp[0];
                    const int64_t e2 = k + 4;
                    py::list<int64_t> swallowed;
                    for (int64_t j = a + 1; j < e2; ++j) {
                        if (D.labels.contains(Place(oi, j))) {
                            swallowed.append(j);
                        }
                    }
                    if (!swallowed.empty()) {   // this single-dd span would bury a referenced label (an overlapping / mis-decoded
                        // pointer table: another dword points AT a byte inside this pointer, or a branch/`dd offset' targets an
                        // instruction start the jump steps over). The emit loop's `a' jumps to k+4 in one step and never lands on
                        // that offset, so the label placement never fires, and the `dd offset loc_..' / `jcc loc_..' elsewhere is
                        // left undefined (A2006). Emit the whole span as raw db WITH the label(s) so every target is DEFINED and
                        // byte-exact; only this one pointer loses its relocation.
                        py::dict<int64_t, py::str> lbls;
                        for (int64_t j : swallowed) {
                            lbls.set(j, D.labels.at(Place(oi, j)));
                        }
                        L.extend(_db_split_labels(code, a, e2, lbls));   // a's own label (if any) is already
                        a = e2;
                        continue;
                    }
                    if (k > a) {
                        L.append(vxd_masm::_db(code.slice(a, k)));
                    }
                    const le::Fixup f = D.fx.at(Place(oi, k));
                    std::optional<py::str> e;
                    if (is_internal_(f)) {
                        e = _dd_target_expr(D, internal_(f).obj, internal_(f).target, ctxp);
                    }
                    L.append(e ? py::percent(py::str("        dd  %s"), *e) : vxd_masm::_db(code.slice(k, k + 4)));
                    a = k + 4;                                          // skip the instructions the pointer overlaps
                    continue;
                }
            }
            const std::optional<py::list<py::str>> load_lines = loadable ? _loadable_insn(D, oi, a, row, ctxp) : std::nullopt;
            if (row.vxdcall) {
                const auto m = svc_macro(std::get<0>(*row.vxdcall), std::get<1>(*row.vxdcall));
                L.append(m && !m->empty() ? *m : vxd_masm::_db(row.raw));
            } else if (load_lines) {                                    // loadable: internal fixup -> symbolic ref
                L.extend(*load_lines);
            } else if (loadable) {
                // LOADABLE: byte-preserve EVERY remaining (non-relocation) instruction. Any instruction reached here carries no
                // internal fixup (those went through load_lines), so its exact bytes are reproducible as `db' and need no symbol.
                // Re-encoding it through ML can pick a shorter form (a mis-decoded string/data region is the common case), which
                // shifts every later label and leaves data pointers holding wrong offsets -> mid-init fault. Byte-preserving
                // makes the object layout exact by construction.
                L.append(vxd_masm::_db(row.raw));
            } else if (row.fx || vxd_masm::has_abs_mem(row) || vxd_masm::has_seg_prefix(row) || _addr16(row) || _opnd16(row) ||
                       _disp0(row) || _shift_no_count(row) || _ml611_unknown(row) || _x87_stack(row) || _indirect_branch(row) ||
                       _cap_size(row) || _RAW_MNEMS.contains(row.mnem)) {
                L.append(vxd_masm::_db(row.raw));                       // byte-preserve the encodings ML will
            } else {                                                    //  not reproduce (abs-mem, seg-override,
                std::optional<py::str> ln = vxd_masm::_masm_line(D, oi, row);   //  16-bit addr, aam/aad, int3, far ...)
                if (ln && !ln->empty() && dis::_BRANCHES.contains(row.mnem)) {
                    // line.split(None, 1): [mnemonic, rest]
                    const std::u32string& s = ln->u32();
                    size_t i0 = 0;
                    while (i0 < s.size() && py::str(std::u32string(1, s[i0])).isspace()) {
                        ++i0;
                    }
                    size_t i1 = i0;
                    while (i1 < s.size() && !py::str(std::u32string(1, s[i1])).isspace()) {
                        ++i1;
                    }
                    size_t i2 = i1;
                    while (i2 < s.size() && py::str(std::u32string(1, s[i2])).isspace()) {
                        ++i2;
                    }
                    const bool has_rest = i2 < s.size();
                    const py::str rest = has_rest ? py::str(s.substr(i2)) : py::str("");
                    static const py::re::Pattern numt = py::re::compile(_NUM_TARGET);
                    const auto tgt = dis::_direct_target_row(row);
                    if ((has_rest && numt.match(rest.strip()).has_value()) || (tgt && !in_(emitted_now, oi, *tgt))) {
                        ln = std::nullopt;                              // target we don't emit a label for
                    } else {                                            //  (bare number / mid-instruction) -> raw
                        ln = ln->replace(py::str("SHORT "), py::str("")).replace(py::str("NEAR PTR "), py::str(""));
                        // Preserve the ORIGINAL near/short encoding. A near branch (jcc rel32 = 6 bytes, jmp rel32 = 5) is
                        // re-emitted `near ptr` so the assembler keeps rel32 instead of shrinking it to rel8 -- reproducing the
                        // oracle's bytes with NONE of the out-of-range risk that pinning SHORT carries (rel32 always reaches).
                        // Short originals stay unsized. loop*/jcxz have no near form, so gate on jmp(>=5)/jcc(>=6).
                        const bool nearb = (row.mnem == "jmp" && row.size >= 5) ||
                                           (row.mnem.slice(0, 1) == py::str("j") && row.mnem != "jmp" && row.size >= 6);
                        if (nearb) {
                            const auto [mn, sep, opnd] = ln->strip().partition(py::str(" "));
                            (void)sep;
                            ln = py::percent(py::str("        %-6s near ptr %s"), mn, opnd.strip());
                        }
                    }
                }
                L.append(ln && !ln->empty() ? *ln : vxd_masm::_db(row.raw));
            }
            a += row.size;
        }
        if (cur_proc) {
            L.append(py::str("EndProc ") + *cur_proc);
        }
        const int64_t vs = o.vsize;                                     // BSS tail: vsize beyond the physical bytes is
        int64_t k = n;                                                  //  uninitialised data. Reserve it and emit any
        while (k < vs) {                                                //  label there -- a data-only object may be purely
            const py::str lb = label_at(D, oi, k);                      //  a relocation target (a DDB/service/jump-table
            if (!lb.empty()) {                                          //  pointer -> loc_<obj>_0000), whose label must be
                L.append(lb + py::str(":"));                            //  DEFINED or the `dd offset ..' is undefined and
            }
            int64_t nk = k + 1;                                         //  the segment links empty.
            while (nk < vs && label_at(D, oi, nk).empty()) {
                nk += 1;
            }
            L.append(py::percent(py::str("        db %d dup (0)"), nk - k));
            k = nk;
        }
        L.append(loadable ? segn + py::str(" ENDS") : segm + py::str("_ENDS"));
        L.append(py::str(""));
    }
    if (!tail_macros.empty()) {                                         // loadable: DDB + service table AFTER the code,
        L.extend(tail_macros);                                          //  so the locked object is [code][DDB+table]
    }
    L.append(py::str("        end"));
    return py::join(py::str("\r\n"), L) + py::str("\r\n");
}

const py::str _HEADER = py::str(
    "; {name}.ASM -- {name} reconstructed in idiomatic VMM DDK-macro source form.\n"
    ";\n"
    "; Recovered from the stripped LE binary by recursive-descent disassembly and re-emitted with the\n"
    "; period Virtual Device DDK macros (Declare_Virtual_Device, Begin_Control_Dispatch / Control_\n"
    "; Dispatch / End_Control_Dispatch, VMMcall / VxDcall <service>, BeginProc / EndProc, VxD_..._SEG) so\n"
    "; it reads like Microsoft's own VxD source. It assembles through the 1996 DDK ML.EXE (COFF) with the\n"
    "; stock INC32 headers. Encodings ML will not reproduce byte-for-byte (mov eAX,[disp], segment/16-bit-\n"
    "; address prefixes, aam/aad, shift-by-1, self-relative call) are kept as `db` to stay byte-exact.\n"
    ";\n"
    "; The original source file names, comments and most internal labels are not present in a stripped LE\n"
    "; image: internal routines are sub_<offset> except where a debug .SYM gives the real name. Exported\n"
    "; service names and control-dispatch handler names ARE the real DDK names.\n");

py::str emit_idiomatic_tree(const le::LEFile& v, const py::str& name, const py::str& outdir) {
    VxDDisasm D(v);
    D.run();
    const py::str d = py::os::path::join(outdir, name.lower());
    py::os::makedirs(d, 0777, true);
    const py::str path = py::os::path::join(d, py::percent(py::str("%s.asm"), name.lower()));
    const py::str head = _HEADER.replace(py::str("{name}"), name);
    const py::str text = head + py::str("\r\n") + emit_idiomatic(D);
    py::file f = py::open(path, py::str("w"), py::str("cp1252"), std::nullopt, py::str(""));
    f.write(text);
    f.close();
    return path;
}

namespace {
// glob.glob(os.path.join(srcdir, pattern)) for the `*.ext` patterns of vxd_corpus._VXD_EXTS (case-insensitive, as
// Windows matches); sorted by the caller
py::list<py::str> glob_ext(const py::str& srcdir, const py::str& ext) {
    py::list<py::str> out;
    if (!py::os::path::isdir(srcdir)) {
        return out;
    }
    for (const auto& entry : py::os::walk(srcdir)) {            // (the first yield: the folder's own files)
        for (const py::str& n : std::get<2>(entry)) {
            if (n.lower().endswith(ext.lower())) {
                out.append(py::os::path::join(srcdir, n));
            }
        }
        break;
    }
    return out;
}
}  // namespace

py::dict<py::str, bool> verify_corpus(const py::str& srcdir, const py::str& root, int64_t budget, const std::optional<py::set<py::str>>& only) {
    const py::str tc = vxd_masm::_stage_toolchain(root);
    const py::str incsrc = paths::reference(py::list<py::str>{py::str("Microsoft Windows 95 DDK"), py::str("INC32")});
    const py::str incdst = py::os::path::join(tc, py::str("inc"));
    py::os::makedirs(incdst, 0777, true);
    for (const py::str& f : glob_ext(incsrc, py::str(".INC"))) {
        const py::str dst = py::os::path::join(incdst, py::os::path::basename(f));
        if (!py::os::path::exists(dst)) {
            py::shutil::copy(f, dst);
        }
    }
    // vxd_corpus._VXD_EXTS / _EXTRA_FILES: the complete VxD file enumeration
    py::list<py::str> cand;
    for (const char* ext : {".vxd", ".386", ".pdr", ".mpd"}) {
        cand.extend(glob_ext(srcdir, py::str(ext)));
    }
    for (const char* f : {"csmapper.sys", "bigmem.drv", "drvspace.bin"}) {
        cand.append(py::os::path::join(srcdir, py::str(f)));
    }
    py::set<py::str> uniq = py::set<py::str>::from(cand);
    py::list<std::tuple<py::str, py::str>> mods;                       // (name, emitted source)
    for (const py::str& p : py::sorted(uniq)) {
        if (!(py::os::path::exists(p) && codebreaker::formats::detect::holds_drivers(p))) {
            continue;
        }
        py::list<std::tuple<py::str, le::LEFile>> ms;
        try {
            ms = le::open_vxds(p);
        } catch (const py::Exception&) {
            continue;
        }
        for (const auto& [nm, v] : ms) {
            const py::str k = py::split(nm, py::str("."))[0].lower();
            if (only && !only->contains(k)) {
                continue;
            }
            VxDDisasm D(v);
            D.run();
            mods.append({k, emit_idiomatic(D)});
        }
    }
    auto lines_of = [](const py::str& s) { return s.count(py::str("\n")); };
    py::list<py::list<std::tuple<py::str, py::str>>> batches;
    py::list<std::tuple<py::str, py::str>> cur;
    int64_t cl = 0;
    for (const auto& m : py::sorted(mods, [&](const auto& x) { return lines_of(std::get<1>(x)); })) {
        const int64_t nl = lines_of(std::get<1>(m));
        if (nl > budget) {
            batches.append(py::list<std::tuple<py::str, py::str>>{m});
            continue;
        }
        if (cl + nl > budget && !cur.empty()) {
            batches.append(cur);
            cur = py::list<std::tuple<py::str, py::str>>();
            cl = 0;
        }
        cur.append(m);
        cl += nl;
    }
    if (!cur.empty()) {
        batches.append(cur);
    }
    const py::str cmd = py::str("c:\\ml.exe -coff -c -DBLD_COFF -DIS_32 -DMASM6 -Ic:\\inc -Fo%s.obj %s.asm > %s.txt");
    namespace dt = codebreaker::build::dos_tools;
    py::dict<py::str, bool> res;
    for (const auto& batch : batches) {
        py::dict<py::str, dt::FileData> files;
        py::list<py::str> bat, arts;
        int64_t i = 0;
        for (const auto& m : batch) {
            const py::str fn = py::percent(py::str("m%d"), i);
            files.set(fn + py::str(".asm"), dt::FileData(std::get<1>(m)));
            bat.append(py::percent(cmd, fn, fn, fn));
            arts.append(fn + py::str(".obj"));
            arts.append(fn + py::str(".txt"));
            i += 1;
        }
        int64_t tot = 0;
        for (const auto& m : batch) {
            tot += lines_of(std::get<1>(m));
        }
        dt::Mounts mounts;
        mounts.append({py::str("c"), tc});
        auto [ok, out, work] = dt::run(mounts, bat, arts, static_cast<double>(std::min<int64_t>(600, 40 + tot / 100)), files);
        i = 0;
        for (const auto& m : batch) {
            const auto o = out.get(py::percent(py::str("m%d.obj"), i));
            res.set(std::get<0>(m), o.has_value() && !o->empty());
            i += 1;
        }
    }
    return res;
}

}  // namespace codebreaker::source::vxd::idiom
