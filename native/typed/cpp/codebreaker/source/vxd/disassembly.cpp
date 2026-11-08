// codebreaker/source/vxd/disassembly.cpp -- typed C++ of codebreaker.source.vxd.disassembly (see disassembly.hpp).

#include "codebreaker/source/vxd/disassembly.hpp"

#include <algorithm>
#include <cstring>
#include <string>
#include <unordered_map>
#include <vector>

#include <capstone/capstone.h>

#include "codebreaker/source/vxd/services.hpp"
#include "codebreaker/source/vxd/symbols.hpp"
#include "py_stdlib.hpp"

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#endif

namespace codebreaker::source::vxd::disassembly {

namespace vxd_services = codebreaker::source::vxd::services;
namespace vxd_symbols = codebreaker::source::vxd::symbols;

const py::dict<int64_t, py::str> VXD_DEVICES = {
    {0x0001, "VMM"}, {0x0002, "DEBUG"}, {0x0003, "VPICD"}, {0x0004, "VDMAD"}, {0x0005, "VTD"},
    {0x0006, "V86MMGR"}, {0x0007, "PAGESWAP"}, {0x0008, "PARITY"}, {0x0009, "REBOOT"}, {0x000A, "VDD"},
    {0x000B, "VSD"}, {0x000C, "VMD"}, {0x000D, "VKD"}, {0x000E, "VCD"}, {0x000F, "VPD"},
    {0x0010, "BLOCKDEV"}, {0x0011, "VMCPD"}, {0x0012, "EBIOS"}, {0x0013, "BIOSXLAT"}, {0x0014, "VNETBIOS"},
    {0x0015, "DOSMGR"}, {0x0016, "VMPOLL"}, {0x0017, "SHELL"}, {0x0018, "VMM_STRING"}, {0x0019, "VPROD"},
    {0x001A, "DOSNET"}, {0x001B, "VFD"}, {0x001C, "VDD2"}, {0x001D, "WINLOAD"}, {0x001E, "VXDLDR"},
    {0x0020, "INT13"}, {0x0021, "PAGEFILE"}, {0x0022, "SCSI"}, {0x0023, "MCA_POS"}, {0x0024, "SCSIFD"},
    {0x0026, "VPEND"}, {0x0027, "APM"}, {0x0028, "VPOWERD"}, {0x0029, "VXDLDR"}, {0x002A, "NDIS"},
    {0x002B, "BIOS"}, {0x002C, "VWIN32"}, {0x002D, "VCOMM"}, {0x002E, "SPOOLER"}, {0x0032, "VCOMM"},
    {0x0033, "CONFIGMG"}, {0x0036, "VFBACKUP"}, {0x0037, "ENABLE"}, {0x0038, "VCOND"}, {0x003D, "BIOS"},
    {0x0040, "IFSMgr"}, {0x0041, "VCDFSD"}, {0x0442, "VTDAPI"}, {0x0448, "PERF"}, {0x0449, "VWIN32"},
    {0x044A, "VFAT"}, {0x0450, "MRCI2"}, {0x0454, "NWLINK"}, {0x0480, "NDIS2SUP"},
};

const int64_t DDB_CONTROL_PROC = 0x18;
const int64_t DDB_V86_API_PROC = 0x1C;
const int64_t DDB_PM_API_PROC = 0x20;
const int64_t DDB_SERVICE_TABLE = 0x30;
const int64_t DDB_SERVICE_SIZE = 0x34;

const py::set<py::str> _TERMS = {py::str("ret"), py::str("retn"), py::str("retf"), py::str("iret"), py::str("iretd"), py::str("iretw")};

const py::set<py::str> _BRANCHES = {
    py::str("call"), py::str("jmp"), py::str("je"), py::str("jne"), py::str("jz"), py::str("jnz"), py::str("ja"),
    py::str("jae"), py::str("jb"), py::str("jbe"), py::str("jg"), py::str("jge"), py::str("jl"), py::str("jle"),
    py::str("jo"), py::str("jno"), py::str("js"), py::str("jns"), py::str("jp"), py::str("jnp"), py::str("jc"),
    py::str("jnc"), py::str("jcxz"), py::str("jecxz"), py::str("loop"), py::str("loope"), py::str("loopne")};

namespace {

// ---- the capstone decoders (platforms.decoder(32, detail=True) and platforms.decoder(16, detail=True)) -----------
struct CsLib {
#ifdef _WIN32
    HMODULE module = nullptr;
#endif
    decltype(&cs_open) open = nullptr;
    decltype(&cs_option) option = nullptr;
    decltype(&cs_malloc) malloc_ = nullptr;
    decltype(&cs_disasm_iter) disasm_iter = nullptr;
    csh handle[2] = {0, 0};          // [0] 16-bit, [1] 32-bit
    cs_insn* insn[2] = {nullptr, nullptr};
};

CsLib& cs_lib() {
    static CsLib lib;
    return lib;
}
std::wstring& dll_path() {
    static std::wstring p;
    return p;
}

template <class F> F sym_(HMODULE m, const char* name) {
    FARPROC p = GetProcAddress(m, name);
    if (!p) throw py::ImportError(std::string("capstone.dll has no ") + name);
    return reinterpret_cast<F>(p);
}

CsLib& cs_ready(bool is32) {
    CsLib& lib = cs_lib();
    const int k = is32 ? 1 : 0;
    if (lib.handle[k]) return lib;
    if (!lib.module) {
        if (dll_path().empty()) throw py::ImportError("capstone: no DLL given (set_capstone_dll)");
        lib.module = LoadLibraryW(dll_path().c_str());
        if (!lib.module) throw py::ImportError("capstone: cannot load the DLL");
        lib.open = sym_<decltype(&cs_open)>(lib.module, "cs_open");
        lib.option = sym_<decltype(&cs_option)>(lib.module, "cs_option");
        lib.malloc_ = sym_<decltype(&cs_malloc)>(lib.module, "cs_malloc");
        lib.disasm_iter = sym_<decltype(&cs_disasm_iter)>(lib.module, "cs_disasm_iter");
    }
    csh h = 0;
    if (lib.open(CS_ARCH_X86, is32 ? CS_MODE_32 : CS_MODE_16, &h) != CS_ERR_OK) throw py::RuntimeError("capstone: cs_open failed");
    lib.option(h, CS_OPT_DETAIL, CS_OPT_ON);
    lib.handle[k] = h;
    lib.insn[k] = lib.malloc_(h);
    return lib;
}

// The first instruction of next(md.disasm(code[a:a+16], a), None): false when nothing decodes.
struct Decoded {
    int64_t size = 0;
    std::string mnemonic, op_str;
};
bool decode_one(bool is32, const std::string& code, int64_t a, Decoded& out) {
    CsLib& lib = cs_ready(is32);
    const int k = is32 ? 1 : 0;
    const int64_t avail = static_cast<int64_t>(code.size()) - a;
    const uint8_t* p = reinterpret_cast<const uint8_t*>(code.data()) + a;
    size_t sz = static_cast<size_t>(avail < 16 ? avail : 16);
    uint64_t address = static_cast<uint64_t>(a);
    if (!lib.disasm_iter(lib.handle[k], &p, &sz, &address, lib.insn[k])) return false;
    out.size = lib.insn[k]->size;
    out.mnemonic = lib.insn[k]->mnemonic;
    out.op_str = lib.insn[k]->op_str;
    return true;
}

// ---- small helpers -------------------------------------------------------------------------------------------

inline bool in_(const PlaceSet& s, int64_t oi, int64_t off) { return s.find(Place(oi, off)) != s.end(); }

inline bool is_internal_(const le::Fixup& f) { return std::holds_alternative<le::InternalFixup>(f); }
inline const le::InternalFixup& internal_(const le::Fixup& f) { return std::get<le::InternalFixup>(f); }
inline const le::ImportFixup& import_(const le::Fixup& f) { return std::get<le::ImportFixup>(f); }
// an internal fix-up's target as the place key holds it
inline int64_t target_key_(const le::InternalFixup& f) { return f.target ? *f.target : NONE_TARGET; }

inline bool starts_with_(const std::string& s, const char* p) { return s.compare(0, std::strlen(p), p) == 0; }
inline bool has_(const std::string& s, const char* p) { return s.find(p) != std::string::npos; }

// the shared service-name tables (vxd_services.load() at import time)
const vxd_services::Services& svc() {
    static const vxd_services::Services s = vxd_services::load();
    return s;
}

// Python's `x or y` for a str
py::str str_or_(const py::str& a, const char* b) { return a.empty() ? py::str(b) : a; }

}  // namespace

void set_capstone_dll(const py::str& path) {
    std::string u = path.utf8();
    int n = MultiByteToWideChar(CP_UTF8, 0, u.c_str(), -1, nullptr, 0);
    std::wstring w(static_cast<size_t>(n > 0 ? n - 1 : 0), L'\0');
    if (n > 0) MultiByteToWideChar(CP_UTF8, 0, u.c_str(), -1, &w[0], n);
    dll_path() = w;
}

py::list<std::tuple<int64_t, int64_t>> _string_ranges(const py::bytes& code) {
    auto P = [](int64_t b) { return 0x20 <= b && b <= 0x7E; };
    auto A = [](int64_t b) { return (0x41 <= b && b <= 0x5A) || (0x61 <= b && b <= 0x7A); };
    auto V = [](int64_t b) {
        return b == 'A' || b == 'E' || b == 'I' || b == 'O' || b == 'U' || b == 'a' || b == 'e' || b == 'i' || b == 'o' || b == 'u';
    };
    py::list<std::tuple<int64_t, int64_t>> out;
    const int64_t n = code.size();
    int64_t i = 0;
    while (i < n) {
        if (!P(code.byte_(i))) {
            i += 1;
            continue;
        }
        int64_t j = i;
        while (j < n && P(code.byte_(j))) j += 1;
        const int64_t L = j - i;
        if (L >= 8 && ((j < n && code.byte_(j) == 0) || L >= 20)) {   // NUL-terminated or long
            int64_t alpha = 0;
            bool vowel = false;
            for (int64_t k = i; k < j; ++k) {
                const int64_t b = code.byte_(k);
                if (A(b)) alpha += 1;
                if (V(b)) vowel = true;
            }
            if (static_cast<double>(alpha) >= 0.60 * static_cast<double>(L)   // alpha-majority
                && vowel)                                                      // contains a vowel
                out.append({i, j});
        }
        i = j;
    }
    return out;
}

// ---- VxDDisasm ---------------------------------------------------------------------------------------------------

VxDDisasm::VxDDisasm(const le::LEFile& v_) : v(&v_) {
    fx = v_.fixups();
    ddb = v_.ddb();
    for (const le::Object& o : v_.objects) code.set(o.idx, v_.object_bytes(o));
    for (const le::Object& o : v_.objects) objs.set(o.idx, o);
}

std::optional<le::Fixup> VxDDisasm::fsite(int64_t oi, int64_t off) const {
    const Place k(oi, off);
    if (!fx.contains(k)) return std::nullopt;
    return fx.at(k);
}

bool VxDDisasm::_free(int64_t oi, int64_t a, int64_t sz) const {
    for (int64_t k = a; k < a + sz; ++k)
        if (in_(mark, oi, k)) return false;
    return true;
}

void VxDDisasm::_mark_data(int64_t oi, int64_t start, int64_t length) {
    for (int64_t k = start; k < start + length; ++k) data.insert(Place(oi, k));
}

namespace {

// (object, byte) -> the start of the instruction covering it, for the instructions as they are
struct Cover {
    std::unordered_map<int64_t, std::vector<int64_t>> by_obj;   // -1: none
    void build(const py::dict<Place, Row>& insns, const py::dict<int64_t, py::bytes>& code) {
        by_obj.clear();
        for (const Place& k : insns) {
            const int64_t oi = std::get<0>(k);
            const int64_t a = std::get<1>(k);
            const Row& row = insns.at(k);
            auto it = by_obj.find(oi);
            if (it == by_obj.end()) {
                // (an instruction always lies inside its object's bytes)
                it = by_obj.emplace(oi, std::vector<int64_t>(static_cast<size_t>(code.contains(oi) ? code.at(oi).size() : 0), -1)).first;
            }
            std::vector<int64_t>& cv = it->second;
            for (int64_t b = a; b < a + row.size; ++b) {
                if (b >= static_cast<int64_t>(cv.size())) cv.resize(static_cast<size_t>(b) + 1, -1);
                cv[static_cast<size_t>(b)] = a;
            }
        }
    }
    // cover.get((oi, off)); -1 for None
    int64_t get(int64_t oi, int64_t off) const {
        auto it = by_obj.find(oi);
        if (it == by_obj.end() || off < 0 || off >= static_cast<int64_t>(it->second.size())) return -1;
        return it->second[static_cast<size_t>(off)];
    }
};

}  // namespace

void VxDDisasm::_remark_data_from_fixups() {
    for (int i = 0; i < 32; ++i) {
        Cover cover;
        cover.build(insns, code);
        std::vector<Place> victims;
        std::unordered_set<Place, PlaceHash> seen;
        for (const Place& k : fx) {
            const le::Fixup& f = fx.at(k);
            if (!is_internal_(f) || !internal_(f).target) continue;
            const int64_t start = cover.get(internal_(f).obj, *internal_(f).target);
            if (start >= 0 && start != *internal_(f).target) {   // target sits mid-instruction
                Place vk(internal_(f).obj, start);
                if (seen.insert(vk).second) victims.push_back(vk);
            }
        }
        if (victims.empty()) break;
        for (const Place& vk : victims) {
            const int64_t oi = std::get<0>(vk), a = std::get<1>(vk);
            if (insns.contains(vk)) {
                const int64_t sz = insns.at(vk).size;
                insns.pop(vk);
                for (int64_t k = a; k < a + sz; ++k) mark.erase(Place(oi, k));
                _mark_data(oi, a, sz);
            }
        }
    }
}

void VxDDisasm::_remark_pointer_tables() {
    // srcs = defaultdict(list): the internal-fixup sources by object, in fixup order
    py::dict<int64_t, py::list<int64_t>> srcs;
    for (const Place& k : fx) {
        const le::Fixup& f = fx.at(k);
        if (is_internal_(f) && internal_(f).target) srcs.setdefault(std::get<0>(k), py::list<int64_t>()).append(std::get<1>(k));
    }
    for (const int64_t oi : srcs.keys()) {
        std::vector<int64_t> offs = srcs.at(oi).vec();
        std::sort(offs.begin(), offs.end());
        offs.erase(std::unique(offs.begin(), offs.end()), offs.end());
        // this object's instructions by start, as they are now (those popped below are skipped when met again)
        std::vector<std::pair<int64_t, int64_t>> rows;
        for (const Place& k : insns)
            if (std::get<0>(k) == oi) rows.emplace_back(std::get<1>(k), insns.at(k).size);
        std::sort(rows.begin(), rows.end());
        size_t i = 0;
        while (i < offs.size()) {
            size_t j = i;
            while (j + 1 < offs.size() && offs[j + 1] == offs[j] + 4) j += 1;
            if (j - i + 1 >= 3) {                                   // >=3 stride-4 pointers = a table
                const int64_t lo = offs[i], hi = offs[j] + 4;
                // an instruction is at most 15 bytes long, so one overlapping [lo, hi) starts after lo - 16
                auto it = std::lower_bound(rows.begin(), rows.end(), std::make_pair(lo - 16, INT64_MIN));
                for (; it != rows.end() && it->first < hi; ++it) {
                    const int64_t a = it->first;
                    if (a < hi && a + it->second > lo) {            // instruction overlaps the table -> mis-decode
                        const Place rk(oi, a);
                        if (!insns.contains(rk)) continue;
                        const int64_t sz = insns.at(rk).size;
                        insns.pop(rk);
                        for (int64_t k = a; k < a + sz; ++k) mark.erase(Place(oi, k));
                        _mark_data(oi, a, sz);
                    }
                }
            }
            i = j + 1;
        }
    }
}

void VxDDisasm::_remark_split_fixup_sources() {
    for (int it = 0; it < 8; ++it) {
        Cover cover;
        cover.build(insns, code);
        bool any_victim = false;
        for (const Place& k : fx.keys()) {
            const le::Fixup& f = fx.at(k);
            if (!is_internal_(f) || !internal_(f).target) continue;
            const int64_t soi = std::get<0>(k), soff = std::get<1>(k);
            std::vector<int64_t> starts;                           // {cover.get(...) for d in range(4)} - {None}
            for (int64_t d = 0; d < 4; ++d) {
                const int64_t s = cover.get(soi, soff + d);
                if (s >= 0 && std::find(starts.begin(), starts.end(), s) == starts.end()) starts.push_back(s);
            }
            if (starts.empty()) continue;                          // dword entirely in a gap -> _loadable_data handles
            if (starts.size() == 1) {
                const int64_t st = starts[0];
                const Place rk(soi, st);
                if (insns.contains(rk)) {
                    const Row& row = insns.at(rk);
                    if (st <= soff && soff + 4 <= st + row.size) continue;   // dword fully inside one instruction's operand -> ok
                }
            }
            for (const int64_t st : starts) {                      // straddles a boundary -> reclassify the overlap
                const Place rk(soi, st);
                if (insns.contains(rk)) {
                    const int64_t sz = insns.at(rk).size;
                    insns.pop(rk);
                    for (int64_t b = st; b < st + sz; ++b) mark.erase(Place(soi, b));
                    _mark_data(soi, st, sz);
                }
            }
            _mark_data(soi, soff, 4);
            any_victim = true;
        }
        if (!any_victim) break;
    }
}

VxDDisasm& VxDDisasm::run() {
    py::list<Place> seeds = _seeds();                              // resolve entry points (control/service) FIRST
    for (const int64_t idx : objs.keys()) {                        // then carve ASCII string data out of every
        const le::Object& o = objs.at(idx);                        // exec object BEFORE descent, so the gap
        if (!o.exec) continue;                                     // seeder's fixup-to-string targets can't
        const int64_t oi = o.idx;                                  // decode strings as bogus code and cascade
        for (const auto& rg : _string_ranges(code.at(oi))) {       // past them. NEVER carve across an
            const int64_t rs = std::get<0>(rg), re = std::get<1>(rg);   // authoritative entry point.
            bool hit = false;
            for (int64_t k = rs; k < re && !hit; ++k) hit = in_(entries, oi, k);
            if (!hit) _mark_data(oi, rs, re - rs);
        }
    }
    _descend(seeds);
    for (int i = 0; i < 8; ++i) {                                  // gap recovery: reach code the control
        py::list<Place> more = _gap_seeds();                       // graph doesn't (indirect/computed calls)
        if (more.empty()) break;
        _descend(more);
    }
    _remark_data_from_fixups();                                    // reclassify data the sweep decoded as code
    _remark_pointer_tables();                                      // reclassify mis-swept pointer tables as data
    _remark_split_fixup_sources();                                 // reclassify pointers straddling a mis-decode
    _collect_labels();
    _apply_ddk_names();
    return *this;
}

void VxDDisasm::_apply_ddk_names() {
    if (!ddb) return;
    const le::Ddb& db = *ddb;
    const py::str dev = db.name;
    py::set<py::str> used;                                         // never introduce a duplicate label name
    for (const Place& k : labels) used.add(labels.at(k));

    auto _assign = [&](const Place& key, const py::str& name) {
        if (used.contains(name) || !insns.contains(key)) return;
        const py::str cur = labels.get(key, py::str(""));
        if (!(cur.startswith(py::str("sub_")) || cur.startswith(py::str("loc_")))) return;   // keep semantic names
        labels.set(key, name);
        used.add(name);
    };

    std::optional<Place> cp;
    {
        const py::str want = dev + "_Control";
        for (const Place& k : labels)
            if (labels.at(k) == want) {
                cp = k;
                break;
            }
    }
    if (cp) {                                                      // name control handlers by their message
        const py::dict<int64_t, py::str> msgs = vxd_symbols::control_messages();
        const int64_t oi = std::get<0>(*cp);
        int64_t a = std::get<1>(*cp);
        int64_t base = 0;
        for (int i = 0; i < 10; ++i) {
            const Place rk(oi, a);
            if (!insns.contains(rk)) break;
            const Row& row = insns.at(rk);
            if (row.mnem == "sub" && row.ops.replace(py::str(" "), py::str("")).startswith(py::str("eax,"))) {
                try {
                    base = py::int_parse(py::split(row.ops, py::str(","))[1].strip(), 0);
                } catch (const py::ValueError&) {
                }
            }
            if (row.mnem == "jmp" && row.ops.contains(py::str("*4"))) {
                std::optional<py::list<Place>> jt;
                for (int64_t k = a + 1; k < a + row.size; ++k) {
                    const Place fk(oi, k);
                    if (fx.contains(fk) && is_internal_(fx.at(fk))) {
                        const Place jk(internal_(fx.at(fk)).obj, target_key_(internal_(fx.at(fk))));
                        if (jtables.contains(jk)) jt = jtables.at(jk);
                        break;
                    }
                }
                if (jt) {
                    int64_t i2 = 0;
                    for (const Place& tk : *jt) {
                        const int64_t msg = i2 + base;
                        i2 += 1;
                        if (msgs.contains(msg) && jt->count(tk) == 1)           // unique handler only
                            _assign(tk, py::percent(py::str("%s_%s"), dev, msgs.at(msg)));
                    }
                }
                break;
            }
            a += row.size;
        }
    }
    const py::dict<int64_t, py::str> names = vxd_symbols::sym_names(dev);   // real internal names where the offset
    for (const int64_t off : names.keys()) _assign(Place(1, off), names.at(off));   //  lands on a function start
}

py::list<Place> VxDDisasm::_gap_seeds() {
    py::list<Place> seeds;
    for (const int64_t oi : objs.keys()) {
        const le::Object& o = objs.at(oi);
        if (!o.exec) continue;
        const py::bytes& cb = code.at(oi);
        const int64_t n = cb.size();
        // internal fixup targets that are unclaimed and look like an instruction start
        for (const Place& fk : fx) {
            const le::Fixup& f = fx.at(fk);
            if (!is_internal_(f) || internal_(f).obj != oi) continue;
            if (!internal_(f).target) continue;
            const int64_t t = *internal_(f).target & 0x7FFFFFFF;   // mask alias bit
            if (!(0 <= t && t < n) || in_(mark, oi, t) || in_(data, oi, t)) continue;
            if (_looks_code(oi, cb, t)) seeds.append(Place(oi, t));
        }
        // bare `push ebp; mov ebp, esp` prologues in gaps
        int64_t i = 0;
        const std::string& raw = cb.raw();
        while (i < n - 2) {
            if (static_cast<unsigned char>(raw[i]) == 0x55 && static_cast<unsigned char>(raw[i + 1]) == 0x8B &&
                static_cast<unsigned char>(raw[i + 2]) == 0xEC && !in_(mark, oi, i) && !in_(data, oi, i)) {
                seeds.append(Place(oi, i));
                i += 3;
            } else {
                i += 1;
            }
        }
    }
    return seeds;
}

bool VxDDisasm::_is32(int64_t oi) const {
    // (objs.get(oi): None when there is no such object -> the 32-bit decoder)
    if (!objs.contains(oi)) return true;
    return (objs.at(oi).flags & 0x2000) != 0;
}

bool VxDDisasm::_looks_code(int64_t oi, const py::bytes& cb, int64_t t) const {
    if (cb.byte_(t) == 0x00) return false;
    Decoded ins;
    return decode_one(_is32(oi), cb.raw(), t, ins);
}

py::list<Place> VxDDisasm::_seeds() {
    py::list<Place> seeds;
    if (!ddb) {
        for (const int64_t idx : objs.keys())                      // no DDB: fall back to object entry 0
            if (objs.at(idx).exec) seeds.append(Place(idx, 0));
        return seeds;
    }
    const le::Ddb& db = *ddb;
    const int64_t oi = db.obj;
    _mark_data(oi, db.rva, 0x54);                                  // the DDB itself is data, not code
    struct Proc {
        int64_t fld;
        const char* nm;
    };
    const Proc procs[] = {{DDB_CONTROL_PROC, "_Control"}, {DDB_V86_API_PROC, "_V86_API"}, {DDB_PM_API_PROC, "_PM_API"}};
    for (const Proc& p : procs) {
        const auto f = fsite(oi, db.rva + p.fld);
        if (f && is_internal_(*f) && objs.contains(internal_(*f).obj) && objs.at(internal_(*f).obj).exec) {
            const Place key(internal_(*f).obj, target_key_(internal_(*f)));
            seeds.append(key);
            entries.insert(key);
            labels.set(key, str_or_(db.name, "Device") + p.nm);
        }
    }
    // service table: DDB_Service_Table_Ptr -> array of DDB_Service_Size dword service procs
    const auto st = fsite(oi, db.rva + DDB_SERVICE_TABLE);
    if (st && is_internal_(*st)) {
        if (!internal_(*st).target) throw py::TypeError("unsupported operand type(s) for +: 'NoneType' and 'int'");
        const int64_t base = *internal_(*st).target;
        const int64_t boi = internal_(*st).obj;
        int64_t cnt = 0;
        const int64_t size = py::from_bytes(code.at(oi).slice(db.rva + DDB_SERVICE_SIZE, db.rva + DDB_SERVICE_SIZE + 4), "little", false);
        const bool known = 0 < size && size <= 8192;               // DDB_Service_Table_Size is authoritative
        const int64_t count = known ? size : 4096;                 // (VMM has 406; a fixed 256 cap dropped 256+)
        for (int64_t j = 0; j < count; ++j) {
            const auto e = fsite(boi, base + j * 4);
            if (!(e && is_internal_(*e))) {                        // a reserved slot (dd 0, no fixup): with a
                if (known) {                                       // known size skip it and keep going, else the
                    cnt += 1;                                      // first gap marks the end of the run
                    continue;
                }
                break;
            }
            const le::InternalFixup& ie = internal_(*e);
            if (objs.contains(ie.obj) && objs.at(ie.obj).exec) {
                const Place key(ie.obj, target_key_(ie));
                seeds.append(key);
                entries.insert(key);                               // a service handler always starts an insn
                const auto sname = svc().name(db.name, j);         // this driver's own DDK service name
                const py::str lbl = (sname && !sname->empty()) ? *sname
                                                               : py::percent(py::str("%s_Service_%02d"), str_or_(db.name, "Dev"), j);   // already qualified
                labels.setdefault(key, lbl);
            }
            cnt += 1;
        }
        if (cnt) {
            svctab = std::make_tuple(boi, base, cnt);
            _mark_data(boi, base, cnt * 4);                        // the service table is a pointer array
        }
    }
    return seeds;
}

void VxDDisasm::_descend(const py::list<Place>& seeds) {
    std::vector<Place> work(seeds.vec());
    while (!work.empty()) {
        const Place cur = work.back();
        work.pop_back();
        const int64_t oi = std::get<0>(cur);
        int64_t a = std::get<1>(cur);
        if (!code.contains(oi)) continue;
        if (a == NONE_TARGET) throw py::TypeError("'<' not supported between instances of 'NoneType' and 'int'");
        const py::bytes& cb = code.at(oi);
        const std::string& cs = cb.raw();
        const int64_t n = cb.size();
        if (a < 0 || a >= n) continue;
        const bool is32 = _is32(oi);
        while (a < n) {
            const Place key(oi, a);
            if (insns.contains(key)) break;
            if (mark.count(key) || data.count(key)) break;         // mid-instruction of prior code, or a
            //                                                        known data table -- abandon this walk
            // VxD service call: CD 20 + inline DWORD
            if (static_cast<unsigned char>(cs[a]) == 0xCD && a + 5 < n && static_cast<unsigned char>(cs[a + 1]) == 0x20) {
                if (!_free(oi, a, 6)) break;
                if (!entries.empty()) {
                    bool hit = false;
                    for (int64_t k = 1; k < 6 && !hit; ++k) hit = in_(entries, oi, a + k);
                    if (hit) break;                                // must not span a service/DDB entry point
                }
                const auto imp = fsite(oi, a + 2);
                int64_t dev, svcn;
                const char* via;
                if (imp && std::holds_alternative<le::ImportFixup>(*imp)) {
                    dev = import_(*imp).module;
                    svcn = import_(*imp).ordinal;
                    via = "import";
                } else {
                    const int64_t dw = py::unpack_from1<int64_t>("<I", cb, a + 2);
                    dev = dw >> 16;
                    svcn = dw & 0xFFFF;
                    via = "inline";
                }
                Row row;
                row.off = a;
                row.size = 6;
                row.mnem = py::str("int");
                row.ops = py::str("20h");
                row.raw = py::bytes(cs.substr(static_cast<size_t>(a), 6));
                row.vxdcall = std::make_tuple(dev, svcn, py::str(via));
                insns.set(key, row);
                for (int64_t k = a; k < a + 6; ++k) mark.insert(Place(oi, k));
                a += 6;
                continue;
            }
            Decoded ins;
            if (!decode_one(is32, cs, a, ins)) break;
            if (!_free(oi, a, ins.size)) break;                    // would overlap authoritative code
            if (!entries.empty()) {
                bool hit = false;
                for (int64_t k = 1; k < ins.size && !hit; ++k) hit = in_(entries, oi, a + k);
                if (hit) break;                                    // would swallow an authoritative entry point
                //                                                    (service proc / DDB proc) -> let it start fresh
            }
            std::optional<le::Fixup> fxr;
            for (int64_t k = a + 1; k < a + ins.size; ++k) {
                const Place fk(oi, k);
                if (fx.contains(fk)) {
                    fxr = fx.at(fk);
                    break;
                }
            }
            Row row;
            row.off = a;
            row.size = ins.size;
            row.mnem = py::str(ins.mnemonic);
            row.ops = py::str(ins.op_str);
            row.raw = py::bytes(cs.substr(static_cast<size_t>(a), static_cast<size_t>(ins.size)));
            row.fx = fxr;
            insns.set(key, row);
            for (int64_t k = a; k < a + ins.size; ++k) mark.insert(Place(oi, k));
            const std::string& m = ins.mnemonic;
            // indirect jump through a table: jmp dword ptr [reg*4 (+ base)]
            if (m == "jmp" && has_(ins.op_str, "[") && has_(ins.op_str, "*4")) {
                _jump_table(oi, a, ins.size, work);
                break;
            }
            if (m == "ret" || m == "retn" || m == "retf" || m == "iret" || m == "iretd" || m == "iretw") break;
            if (m == "jmp") {                                      // direct unconditional jump = tail
                _follow(oi, row.ops, fxr, work);
                break;
            }
            if (m == "call" || m[0] == 'j' || starts_with_(m, "loop")) {
                _follow(oi, row.ops, fxr, work);                   // cross-object call/jcc uses the fixup
                // call / jcc fall through to the next instruction
            }
            a += ins.size;
        }
    }
}

void VxDDisasm::_follow(int64_t oi, const py::str& op_str, const std::optional<le::Fixup>& fxr, std::vector<Place>& work) {
    if (fxr && is_internal_(*fxr)) {
        if (!internal_(*fxr).target) throw py::TypeError("unsupported operand type(s) for &: 'NoneType' and 'int'");
        work.emplace_back(internal_(*fxr).obj, *internal_(*fxr).target & 0x7FFFFFFF);   // mask high-linear-alias bit
        return;
    }
    const auto t = _direct_target(op_str);
    if (t) work.emplace_back(oi, *t);
}

void VxDDisasm::_jump_table(int64_t oi, int64_t a, int64_t size, std::vector<Place>& work) {
    std::optional<int64_t> base;                                   // the table base = the fixup on this insn
    int64_t boi = 0;
    for (int64_t k = a + 1; k < a + size; ++k) {
        const Place fk(oi, k);
        if (fx.contains(fk) && is_internal_(fx.at(fk))) {
            base = internal_(fx.at(fk)).target;
            boi = internal_(fx.at(fk)).obj;
            break;
        }
    }
    if (!base) return;
    py::list<Place> targets;
    for (int64_t j = 0; j < 1024; ++j) {
        const auto e = fsite(boi, *base + j * 4);
        if (!(e && is_internal_(*e))) break;
        const le::InternalFixup& ie = internal_(*e);
        targets.append(Place(ie.obj, target_key_(ie)));
        if (objs.contains(ie.obj) && objs.at(ie.obj).exec) work.emplace_back(ie.obj, target_key_(ie));
    }
    if (!targets.empty()) {
        jtables.set(Place(boi, *base), targets);
        _mark_data(boi, *base, targets.size() * 4);                // the table bytes are data, not code
    }
}

void VxDDisasm::_collect_labels() {
    std::unordered_set<Place, PlaceHash> calltgt;
    for (const Place& k : insns) {                                 // a call destination reads as a proc
        const Row& row = insns.at(k);
        if (row.mnem == "call") {
            if (row.fx && is_internal_(*row.fx)) {
                calltgt.insert(Place(internal_(*row.fx).obj, target_key_(internal_(*row.fx))));
            } else {
                const auto t = _direct_target_row(row);
                if (t) calltgt.insert(Place(std::get<0>(k), *t));
            }
        }
    }
    for (const Place& k : insns) {                                 // branch/call targets
        const Row& row = insns.at(k);
        const int64_t oi = std::get<0>(k);
        const auto t = _direct_target_row(row);
        if (t && insns.contains(Place(oi, *t))) labels.setdefault(Place(oi, *t), _lname(Place(oi, *t), calltgt));
        if (row.fx && is_internal_(*row.fx)) {                     // cross-object branch/ref target
            const Place key(internal_(*row.fx).obj, target_key_(internal_(*row.fx)));
            if (insns.contains(key)) labels.setdefault(key, _lname(key, calltgt));
        }
    }
    for (const Place& jk : jtables) {                              // jump-table destinations
        for (const Place& tk : jtables.at(jk))
            if (insns.contains(tk)) labels.setdefault(tk, _lname(tk, calltgt));
    }
    for (const Place& fk : fx) {                                   // fixup targets that are code
        const le::Fixup& f = fx.at(fk);
        if (is_internal_(f)) {
            const Place key(internal_(f).obj, target_key_(internal_(f)));
            if (insns.contains(key)) labels.setdefault(key, _lname(key, calltgt));
        }
    }
}

py::str VxDDisasm::_lname(const Place& key, const std::unordered_set<Place, PlaceHash>& calltgt) const {
    return py::percent(py::str(calltgt.count(key) ? "sub_%d_%04X" : "loc_%d_%04X"), std::get<0>(key), std::get<1>(key));
}

// ---- operands ------------------------------------------------------------------------------------------------------

std::optional<int64_t> _direct_target(const py::str& op_str) {
    const py::str t = op_str.strip();
    if (t.startswith(py::str("0x"))) {
        try {
            return py::int_parse(t, 16);
        } catch (const py::ValueError&) {
            return std::nullopt;
        }
    }
    return std::nullopt;
}

std::optional<int64_t> _direct_target_row(const Row& row) {
    const py::str& m = row.mnem;
    if (m == "call" || m.slice(0, 1) == "j" || m.startswith(py::str("loop"))) {
        const py::str t = row.ops.strip();
        if (t.startswith(py::str("0x"))) {
            try {
                return py::int_parse(t, 16);
            } catch (const py::ValueError&) {
                return std::nullopt;
            }
        }
    }
    return std::nullopt;
}

py::str _svc_name(int64_t dev, int64_t svcn, const py::str& via) {
    const py::str dname = VXD_DEVICES.get(dev, py::percent(py::str("Dev_%04X"), dev));
    const char* kind = dev == 0x0001 ? "VMMCall" : "VxDCall";
    const auto sname = svc().name(dname, svcn);                    // DDK name if the device+ordinal is known
    const py::str ref = (sname && !sname->empty()) ? py::percent(py::str("%s.%s"), dname, *sname)
                                                   : py::percent(py::str("%s.Service_%04Xh"), dname, svcn);
    const py::str tag = via == "inline" ? py::str("") : py::str("  ; (import-fixup)");
    return py::percent(py::str("%s  %s"), py::str(kind), ref) + tag;
}

py::str emit_asm(const le::LEFile& s_v) {
    VxDDisasm D(s_v);
    D.run();
    return emit_asm(D, s_v);
}

py::str emit_asm(const VxDDisasm& D, const le::LEFile& v) {
    const std::optional<le::Ddb>& db = D.ddb;
    const py::str name = db && !db->name.empty() ? db->name : v.resident_name();
    py::list<py::str> L;
    L.append(py::percent(py::str("; %s -- reconstructed 32-bit disassembly (VxD / LE format)"), name));
    L.append(py::percent(py::str("; %d objects, %d instructions recovered by recursive descent"), v.objects.size(), D.insns.size()));
    if (db) {
        L.append(py::str(""));
        L.append(py::percent(py::str("Declare_Virtual_Device %s, %d, %d, %s_Control, 0%04Xh, Undefined_Init_Order"), db->name, db->major,
                             db->minor, db->name, db->device_id));
        L.append(py::percent(py::str(";   DDK version 0%03Xh   init_order 0%08Xh   flags 0%04Xh"), db->version, db->init_order, db->flags));
        if (D.svctab) {
            const auto& [boi, base, cnt] = *D.svctab;
            L.append(py::percent(py::str(";   service table: obj%d:0x%x, %d services"), boi, base, cnt));
        }
    }
    L.append(py::str(""));
    for (const le::Object& o : v.objects) {
        const int64_t oi = o.idx;
        const char* seg = o.exec ? "_LTEXT" : (o.write ? "_LDATA" : "_LRDATA");
        L.append(py::percent(py::str("VxD_%s SEGMENT   ; object %d  vsize=0x%x  %s%s%s"), py::str(seg), oi, o.vsize,
                             py::str(o.read ? "R" : "-"), py::str(o.write ? "W" : "-"), py::str(o.exec ? "X" : "-")));
        const py::bytes& code = D.code.at(oi);
        int64_t a = 0;
        const int64_t n = code.size();
        while (a < n) {
            const Place key(oi, a);
            const bool has_row = D.insns.contains(key);
            if (!has_row) {                                        // a data span: coalesce to next code/label
                const int64_t start = a;
                while (a < n && !in_(D.mark, oi, a)) {
                    a += 1;
                    if (D.labels.contains(Place(oi, a)) && D.insns.contains(Place(oi, a))) break;
                }
                for (const py::str& s : _emit_data(D, oi, start, a)) L.append(s);
                continue;
            }
            const Row& row = D.insns.at(key);
            if (D.labels.contains(key)) {
                const py::str& lab = D.labels.at(key);
                if (!lab.empty()) L.append(lab + ":");
            }
            if (row.vxdcall) {
                const auto& [dev, svcn, via] = *row.vxdcall;
                L.append(py::percent(py::str("        %-40s ; %s"), _svc_name(dev, svcn, via), row.raw.hex()));
            } else {
                const auto [ops, resolved] = _render_ops(D, oi, row);
                py::str line = py::percent(py::str("        %-6s %s"), row.mnem, ops);
                if (row.fx && !resolved)                           // non-branch fixup: annotate the symbol
                    line = py::percent(py::str("%-44s ; -> %s"), line, _fx_sym(D, *row.fx));
                L.append(line);
            }
            a += row.size;
        }
        L.append(py::percent(py::str("VxD_%s ENDS"), py::str(seg)));
        L.append(py::str(""));
    }
    return py::join(py::str("\n"), L);
}

std::tuple<py::str, bool> _render_ops(const VxDDisasm& D, int64_t oi, const Row& row) {
    if (_BRANCHES.contains(row.mnem)) {
        if (row.fx && is_internal_(*row.fx)) {
            const Place k(internal_(*row.fx).obj, target_key_(internal_(*row.fx)));
            if (D.labels.contains(k) && !D.labels.at(k).empty()) return {D.labels.at(k), true};
        }
        const auto t = _direct_target_row(row);
        if (t && D.labels.contains(Place(oi, *t))) return {D.labels.at(Place(oi, *t)), true};
    }
    return {row.ops, false};
}

py::str _fx_sym(const VxDDisasm& D, const le::Fixup& f) {
    if (is_internal_(f)) {
        const le::InternalFixup& i = internal_(f);
        const Place k(i.obj, target_key_(i));
        if (D.labels.contains(k) && !D.labels.at(k).empty()) return D.labels.at(k);
        return py::percent(py::str("obj%d:0x%04X"), i.obj, i.target ? *i.target : 0);
    }
    const le::ImportFixup& m = import_(f);
    const py::str dname = VXD_DEVICES.get(m.module, py::percent(py::str("Mod_%04X"), m.module));
    return py::percent(py::str("%s.Service_%04Xh"), dname, m.ordinal);
}

py::list<py::str> _emit_data(const VxDDisasm& D, int64_t oi, int64_t start, int64_t end) {
    py::list<py::str> out;
    const py::bytes& code = D.code.at(oi);
    int64_t a = start;
    const std::optional<le::Ddb>& db = D.ddb;
    auto is_ddb = [&](int64_t at) { return db && oi == db->obj && at == db->rva; };
    while (a < end) {
        if (D.labels.contains(Place(oi, a)) && !D.labels.at(Place(oi, a)).empty()) out.append(D.labels.at(Place(oi, a)) + ":");
        if (is_ddb(a)) {                                           // the DDB itself
            out.append(py::str("        ; ---- Device Descriptor Block ----"));
            out.append(py::str("        dd  0                          ; DDB_Next"));
            out.append(py::percent(py::str("        dw  0%04Xh                      ; DDB_SDK_Version"), db->version));
            out.append(py::percent(py::str("        dw  0%04Xh                      ; DDB_Req_Device_Number"), db->device_id));
            out.append(py::percent(py::str("        db  %d, %d                       ; DDB_Dev_Major/Minor"), db->major, db->minor));
            out.append(py::percent(py::str("        dw  0%04Xh                      ; DDB_Flags"), db->flags));
            out.append(py::percent(py::str("        db  '%-8s'                  ; DDB_Name"), db->name));
            out.append(py::percent(py::str("        dd  0%08Xh                  ; DDB_Init_Order"), db->init_order));
            struct Fld {
                int64_t fld;
                const char* nm;
            };
            const Fld flds[] = {{DDB_CONTROL_PROC, "DDB_Control_Proc"}, {DDB_V86_API_PROC, "DDB_V86_API_Proc"},
                                {DDB_PM_API_PROC, "DDB_PM_API_Proc"}, {DDB_SERVICE_TABLE, "DDB_Service_Table_Ptr"}};
            for (const Fld& fl : flds) {
                const auto f = D.fsite(oi, db->rva + fl.fld);
                const py::str sym = f ? _fx_sym(D, *f) : py::str("0");
                out.append(py::percent(py::str("        dd  %-24s ; %s"), sym, py::str(fl.nm)));
            }
            a = db->rva + 0x38;
            continue;
        }
        const Place ak(oi, a);
        if (D.jtables.contains(ak) && !D.jtables.at(ak).empty()) {   // a jump/pointer table
            const py::list<Place>& jt = D.jtables.at(ak);
            out.append(py::percent(py::str("        ; ---- dispatch table (%d entries) ----"), jt.size()));
            for (const Place& t : jt) {
                py::str sym;
                if (D.labels.contains(t) && !D.labels.at(t).empty()) sym = D.labels.at(t);
                else sym = py::percent(py::str("obj%d:0x%04X"), std::get<0>(t), std::get<1>(t));
                out.append(py::str("        dd  ") + sym);
            }
            a += jt.size() * 4;
            continue;
        }
        const auto f = D.fsite(oi, a);                             // a lone pointer slot
        if (f && is_internal_(*f)) {
            out.append(py::percent(py::str("        dd  %-24s ; fixup"), _fx_sym(D, *f)));
            a += 4;
            continue;
        }
        // raw bytes up to the next interesting boundary
        int64_t b = a;
        while (b < end) {
            const Place bk(oi, b);
            if (D.labels.contains(bk) && !D.labels.at(bk).empty()) break;
            if (D.jtables.contains(bk) && !D.jtables.at(bk).empty()) break;
            const auto fb = D.fsite(oi, b);
            if (fb && is_internal_(*fb)) break;
            if (is_ddb(b)) break;
            b += 1;
        }
        const py::bytes chunk = code.slice(a, b > a ? b : a + 1);
        for (int64_t i = 0; i < chunk.size(); i += 16) {
            std::string line = "        db  ";
            for (int64_t k = i; k < i + 16 && k < chunk.size(); ++k) {
                if (k > i) line += ", ";
                char buf[8];
                std::snprintf(buf, sizeof buf, "0%02Xh", static_cast<unsigned>(chunk.byte_(k)));
                line += buf;
            }
            out.append(py::str(line));
        }
        a = b > a ? b : a + 1;
    }
    return out;
}

py::list<VerifyRow> verify(const le::LEFile& v) {
    VxDDisasm D(v);
    D.run();
    py::list<VerifyRow> rep;
    for (const le::Object& o : v.objects) {
        if (!o.exec) continue;
        const int64_t oi = o.idx;
        const int64_t n = o.vsize;
        std::vector<uint8_t> occ(static_cast<size_t>(n > 0 ? n : 0), 0);
        int64_t overlap = 0, overrun = 0;
        for (const Place& k : D.insns) {
            if (std::get<0>(k) != oi) continue;
            const int64_t off = std::get<1>(k);
            const int64_t size = D.insns.at(k).size;
            for (int64_t b = off; b < off + size; ++b) {
                if (b >= n) overrun += 1;
                else if (occ[static_cast<size_t>(b)]) overlap += 1;
                else occ[static_cast<size_t>(b)] = 1;
            }
        }
        int64_t codeb = 0;
        for (uint8_t c : occ) codeb += c;
        VerifyRow r;
        r.obj = oi;
        r.n = n;
        r.code = codeb;
        r.data = n - codeb;
        r.overlap = overlap;
        r.overrun = overrun;
        rep.append(r);
    }
    return rep;
}

py::list<FileStat> disassemble_file(const py::str& path, const std::optional<py::str>& outdir) {
    py::list<FileStat> stats;
    for (const auto& nv : le::open_vxds(path)) {
        const py::str& name = std::get<0>(nv);
        const le::LEFile& v = std::get<1>(nv);
        VxDDisasm D(v);
        D.run();
        int64_t tot = 0;
        for (const le::Object& o : v.objects)
            if (o.exec) tot += o.vsize;
        if (tot == 0) tot = 1;
        FileStat st;
        st.name = name;
        st.insns = D.insns.size();
        st.cov = py::truediv(100 * static_cast<int64_t>(D.mark.size()), tot);
        st.labels = D.labels.size();
        st.jtables = D.jtables.size();
        if (D.ddb) {
            st.device = D.ddb->name;
            st.device_id = D.ddb->device_id;
        }
        if (outdir && !outdir->empty()) {
            py::os::makedirs(*outdir, 0777, true);
            py::file f = py::open(py::os::path::join(*outdir, name.lower() + ".asm"), py::str("w"));
            f.write(emit_asm(D, v));
            f.close();
        }
        stats.append(st);
    }
    return stats;
}

}  // namespace codebreaker::source::vxd::disassembly
