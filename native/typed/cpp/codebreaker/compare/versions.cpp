// codebreaker/compare/versions.cpp -- typed C++ of codebreaker.compare.versions (see versions.hpp).
//
// versions.py -- two builds of a module, function by function: what is the same, what changed, what is new, and what is
// already known about the new one because of the old one.

#include <windows.h>

#undef small
#undef near
#undef far
#undef VOID
#undef ERROR
#undef DELETE
#undef IN
#undef OUT
#undef OPTIONAL
#undef CONST

#include "codebreaker/compare/versions.hpp"
#include "py_difflib.hpp"

#include <capstone/capstone.h>

#include <algorithm>
#include <array>
#include <cstring>
#include <cmath>
#include <cstdio>
#include <mutex>
#include <unordered_map>
#include <unordered_set>

#include "codebreaker/analyze/dos_functions.hpp"
#include "codebreaker/base/corpus.hpp"
#include "codebreaker/compare/mask.hpp"
#include "codebreaker/decompile/describe.hpp"
#include "codebreaker/formats/detect.hpp"
#include "codebreaker/formats/dos.hpp"
#include "codebreaker/formats/ne.hpp"
#include "codebreaker/formats/pe.hpp"
#include "codebreaker/formats/pe_functions.hpp"
#include "codebreaker/jobs/pool.hpp"
#include "codebreaker/store/functions.hpp"
#include "codebreaker/verify/reconstruct.hpp"
#include "codebreaker/verify/verdicts.hpp"
#include "py_json.hpp"
#include "py_stdlib.hpp"

namespace codebreaker::compare::versions {

using formats::inspect::Value;
using py::str;
namespace F = store::functions;
namespace V = verify::verdicts;

const py::list<str> LEVELS = {str("bytes"), str("text"), str("shape")};
const str ENGINE = str("carried");

std::function<double()>& clock() {
    static std::function<double()> c = [] { return py::time::time(); };
    return c;
}

namespace {

constexpr int64_t X86_OP_REG = 1, X86_OP_IMM = 2, X86_OP_MEM = 3;

bool less_str(const str& a, const str& b) { return a.u32() < b.u32(); }

// ---------------------------------------------------------------------------------------------------------------------
// hashlib.blake2b(data, digest_size=8), from the runtime
// ---------------------------------------------------------------------------------------------------------------------
std::string blake2b8(const std::string& data) { return py::hashlib::blake2b(py::bytes(data), 8).digest().raw(); }

str hex8(const std::string& d) {
    static const char* H = "0123456789abcdef";
    std::string o;
    for (unsigned char c : d) {
        o += H[c >> 4];
        o += H[c & 15];
    }
    return str(o);
}

// _digest(parts): blake2b(b"\n".join(parts), digest_size=8).hexdigest()
str digest(const std::vector<std::string>& parts) {
    std::string all;
    for (size_t i = 0; i < parts.size(); i++) {
        if (i) all += '\n';
        all += parts[i];
    }
    return hex8(blake2b8(all));
}

// ---------------------------------------------------------------------------------------------------------------------
// difflib.SequenceMatcher(None, a, b, autojunk=False) over sequences of numbers (the runtime's, with the int shapes this
// module reads)
// ---------------------------------------------------------------------------------------------------------------------
class SeqMatcher {
  public:
    struct Block {
        int i, j, k;
    };
    SeqMatcher(const std::vector<int>& a, const std::vector<int>& b) : sm_(a, b, nullptr, false) {}

    std::vector<Block> get_matching_blocks() const {
        std::vector<Block> out;
        for (const py::difflib::Match& m : sm_.get_matching_blocks()) out.push_back({static_cast<int>(m.a), static_cast<int>(m.b), static_cast<int>(m.size)});
        return out;
    }
    // (tag, i1, i2, j1, j2)
    std::vector<std::tuple<const char*, int, int, int, int>> get_opcodes() const {
        std::vector<std::tuple<const char*, int, int, int, int>> out;
        for (const py::difflib::Opcode& op : sm_.get_opcodes()) {
            const str& t = std::get<0>(op);
            const char* tag = t == str("equal") ? "equal" : t == str("replace") ? "replace" : t == str("delete") ? "delete" : "insert";
            out.emplace_back(tag, static_cast<int>(std::get<1>(op)), static_cast<int>(std::get<2>(op)), static_cast<int>(std::get<3>(op)),
                             static_cast<int>(std::get<4>(op)));
        }
        return out;
    }
    double ratio() const { return sm_.ratio(); }
    double real_quick_ratio() const { return sm_.real_quick_ratio(); }
    double quick_ratio() const { return sm_.quick_ratio(); }

  private:
    py::difflib::SequenceMatcher<int> sm_;
};

// one id per distinct element
struct Interner {
    std::unordered_map<std::string, int> ids;
    int of(const std::string& key) {
        auto it = ids.find(key);
        if (it != ids.end()) return it->second;
        int n = static_cast<int>(ids.size());
        ids.emplace(key, n);
        return n;
    }
};

std::vector<int> intern(Interner& in, const std::vector<str>& v) {
    std::vector<int> out;
    out.reserve(v.size());
    for (const str& s : v) out.push_back(in.of(s.utf8()));
    return out;
}

// _ratio(sa, sb)
double ratio_of(const std::vector<str>& sa, const std::vector<str>& sb) {
    Interner in;
    std::vector<int> a = intern(in, sa), b = intern(in, sb);
    SeqMatcher m(a, b);
    return m.real_quick_ratio() >= 0.3 && m.quick_ratio() >= 0.3 ? m.ratio() : 0.0;
}

// ---------------------------------------------------------------------------------------------------------------------
// capstone
// ---------------------------------------------------------------------------------------------------------------------
struct CsLib {
    HMODULE module = nullptr;
    decltype(&cs_open) open = nullptr;
    decltype(&cs_option) option = nullptr;
    decltype(&cs_disasm) disasm = nullptr;
    decltype(&cs_free) free_ = nullptr;
    decltype(&cs_reg_name) reg_name = nullptr;
};
std::wstring& dll_path() {
    static std::wstring p;
    return p;
}
template <class Fn> Fn sym_(HMODULE m, const char* name) {
    FARPROC p = GetProcAddress(m, name);
    if (!p) throw py::ImportError(std::string("capstone.dll has no ") + name);
    return reinterpret_cast<Fn>(p);
}
CsLib& lib_ready() {
    static CsLib lib;
    static std::mutex m;
    std::lock_guard<std::mutex> g(m);
    if (lib.module) return lib;
    if (dll_path().empty()) throw py::ImportError("capstone: no DLL given (set_capstone_dll)");
    HMODULE h = LoadLibraryW(dll_path().c_str());
    if (!h) throw py::ImportError("capstone: cannot load the DLL");
    lib.open = sym_<decltype(&cs_open)>(h, "cs_open");
    lib.option = sym_<decltype(&cs_option)>(h, "cs_option");
    lib.disasm = sym_<decltype(&cs_disasm)>(h, "cs_disasm");
    lib.free_ = sym_<decltype(&cs_free)>(h, "cs_free");
    lib.reg_name = sym_<decltype(&cs_reg_name)>(h, "cs_reg_name");
    lib.module = h;
    return lib;
}

// a handle per thread and mode, detail on
csh handle_(int64_t bits) {
    CsLib& lib = lib_ready();
    thread_local std::map<int64_t, csh> held;
    auto it = held.find(bits);
    if (it != held.end()) return it->second;
    csh h = 0;
    cs_mode mode = bits == 16 ? CS_MODE_16 : bits == 64 ? CS_MODE_64 : CS_MODE_32;
    if (lib.open(CS_ARCH_X86, mode, &h) != CS_ERR_OK) throw py::RuntimeError("capstone: cs_open failed");
    lib.option(h, CS_OPT_DETAIL, CS_OPT_ON);
    held[bits] = h;
    return h;
}

}  // namespace

void set_capstone_dll(const str& path) {
    std::string u = path.utf8();
    int n = MultiByteToWideChar(CP_UTF8, 0, u.data(), static_cast<int>(u.size()), nullptr, 0);
    std::wstring w(static_cast<size_t>(n), L'\0');
    MultiByteToWideChar(CP_UTF8, 0, u.data(), static_cast<int>(u.size()), &w[0], n);
    dll_path() = w;
}

Decoder::Decoder(int64_t bits) : bits_(bits) {}

std::vector<Ins> Decoder::disasm(const py::bytes& code, int64_t address) const {
    CsLib& lib = lib_ready();
    csh h = handle_(bits_);
    cs_insn* insn = nullptr;
    const std::string& raw = code.raw();
    size_t n = lib.disasm(h, reinterpret_cast<const uint8_t*>(raw.data()), raw.size(), static_cast<uint64_t>(address), 0, &insn);
    std::vector<Ins> out;
    out.reserve(n);
    for (size_t i = 0; i < n; i++) {
        const cs_insn& c = insn[i];
        Ins x;
        x.address = static_cast<int64_t>(c.address);
        x.size = c.size;
        x.mnemonic = str(std::string(c.mnemonic));
        x.op_str = str(std::string(c.op_str));
        x.bytes = py::bytes(std::string(reinterpret_cast<const char*>(c.bytes), c.size));
        if (c.detail) {
            const cs_x86& d = c.detail->x86;
            for (int k = 0; k < d.op_count; k++) {
                const cs_x86_op& o = d.operands[k];
                Operand p;
                p.type = o.type;
                if (o.type == X86_OP_REG) p.reg = o.reg;
                else if (o.type == X86_OP_IMM) p.imm = o.imm;
                else if (o.type == X86_OP_MEM) {
                    p.segment = o.mem.segment;
                    p.base = o.mem.base;
                    p.index = o.mem.index;
                    p.scale = o.mem.scale;
                    p.disp = o.mem.disp;
                }
                x.operands.push_back(p);
            }
            x.imm_offset = d.encoding.imm_offset;
            x.imm_size = d.encoding.imm_size;
            x.disp_offset = d.encoding.disp_offset;
            x.disp_size = d.encoding.disp_size;
        }
        out.push_back(std::move(x));
    }
    if (insn) lib.free_(insn, n);
    return out;
}

str Decoder::reg_name(int64_t reg) const {
    CsLib& lib = lib_ready();
    const char* n = lib.reg_name(handle_(bits_), static_cast<unsigned>(reg));
    return str(std::string(n ? n : ""));
}

std::shared_ptr<const Decoder> decoder(int64_t bits) { return std::make_shared<Decoder>(bits); }

const str& Ids::at(const str& level) const {
    if (level == str("bytes")) return bytes;
    if (level == str("text")) return text;
    return shape;
}

Side::Side(const str& path_, const str& kind_, int64_t code_) : path(path_), kind(kind_), code(code_) {
    name = py::os::path::basename(path_).lower();
}

void Side::add(const std::shared_ptr<Function>& f) {
    functions.set(f->key, f);
    order.append(f->key);
}

std::shared_ptr<Side> Side::done_() {
    for (const auto& kv : functions.items()) {
        const Function& f = *std::get<1>(kv);
        for (const Call& c : f.calls)
            if (c.kind == str("f") && functions.contains(c.key)) callers[c.key.u32()].insert(f.key.u32());
    }
    return nullptr;
}

int64_t Side::size() const {
    int64_t n = 0;
    for (const auto& kv : functions.items()) n += std::get<1>(kv)->size;
    return n;
}

namespace {

// a reference a relocated byte has: ("i", "KERNEL.5") an import, ("f", key) a function of the module, ("d", "") anything else
struct Ref {
    char kind;
    str key;
};

const std::unordered_set<std::string>& ret_set() {
    static const std::unordered_set<std::string> s = {"ret", "retf", "lret", "iret", "iretd"};
    return s;
}

// _body(stream, lo, hi): the instructions of the function at `lo`: up to the return or jump away after which no jump inside it
// still points ahead (or to `hi`, where the next function starts)
std::vector<Ins> body_of(const std::vector<Ins>& stream, int64_t lo, int64_t hi) {
    std::vector<Ins> out;
    int64_t reach = lo;
    for (const Ins& ins : stream) {
        out.push_back(ins);
        std::string mn = ins.mnemonic.utf8();
        int64_t nxt = ins.address + ins.size;
        if (mn[0] == 'j' || mn.compare(0, 4, "loop") == 0) {
            if (ins.operands.size() == 1 && ins.operands[0].type == X86_OP_IMM && lo <= ins.operands[0].imm && ins.operands[0].imm < hi)
                reach = std::max(reach, ins.operands[0].imm);
        }
        bool leaves = mn == "jmp" || mn == "ljmp" || ret_set().count(mn);
        if (nxt >= hi || (leaves && nxt > reach)) break;
    }
    return out;
}

str alike(const std::string& mn, bool jumps) {
    static const std::map<std::string, std::string> A = {{"test", "cmp"}, {"cmp", "cmp"}, {"mov", "mov"}, {"lea", "mov"}, {"movzx", "mov"}, {"movsx", "mov"}};
    auto it = A.find(mn);
    if (it != A.end()) return str(it->second);
    if (jumps && mn != "jmp" && mn != "ljmp") return str("jcc");
    return str(mn);
}

using RelocatedFn = std::function<bool(int64_t)>;
using SiteFn = std::function<std::optional<Ref>(int64_t)>;
using DirectFn = std::function<std::optional<str>(int64_t)>;
using SlotFn = std::function<std::optional<str>(int64_t)>;
using InsideFn = std::function<bool(int64_t)>;

// _function(key, name, named, body, lo, hi, relocated, direct, site, slot, inside): fingerprint one function
std::shared_ptr<Function> make_function(const str& key, const str& name, bool named, const std::vector<Ins>& body, int64_t lo, int64_t hi,
                                        const RelocatedFn& relocated, const DirectFn& direct, const SiteFn& site, const SlotFn& slot,
                                        const InsideFn& inside, const Decoder& md) {
    auto f = std::make_shared<Function>();
    f->key = key;
    f->name = name;
    f->named = named;
    f->at = lo;
    f->size = body.empty() ? 0 : body.back().address + body.back().size - lo;
    std::unordered_map<int64_t, int64_t> index;
    for (size_t i = 0; i < body.size(); i++) index[body[i].address] = static_cast<int64_t>(i);
    std::vector<std::string> raw, texts, shapes;
    for (size_t ii = 0; ii < body.size(); ii++) {
        const Ins& ins = body[ii];
        int64_t i = static_cast<int64_t>(ii);
        std::string mn = ins.mnemonic.utf8();
        std::vector<Operand> ops = ins.operands;
        int64_t addr = ins.address;
        std::vector<int64_t> hits;
        for (int64_t k = 0; k < ins.size; k++)
            if (relocated(addr + k)) hits.push_back(k);
        std::set<int64_t> out(hits.begin(), hits.end());                 // the bytes left out of the `bytes` digest
        std::optional<Ref> ref;
        for (int64_t k : hits) {
            auto r = site(addr + k);
            if (r) { ref = r; break; }
        }
        bool calls = mn == "call" || mn == "lcall";
        bool jumps = mn[0] == 'j' || mn.compare(0, 4, "loop") == 0 || mn == "ljmp";
        std::vector<std::string> parts, kinds;
        std::optional<Call> called;
        if ((mn == "lcall" || mn == "ljmp") && ops.size() == 2) {          // far, to segment:offset
            if (ref && (ref->kind == 'f' || ref->kind == 'i')) called = Call{str(std::string(1, ref->kind)), ref->key};
            parts.push_back(ref && ref->kind == 'i' ? "!" + ref->key.utf8() : std::string(">"));
            kinds.push_back("t");
            ops.clear();
        }
        for (const Operand& op : ops) {
            if (op.type == X86_OP_REG) {
                std::string reg = md.reg_name(op.reg).utf8();
                parts.push_back(reg);
                kinds.push_back(reg == "bp" || reg == "ebp" ? "f" : reg == "sp" || reg == "esp" ? "s" : (reg.size() == 2 && reg.back() == 's') ? "g" : "r");
            } else if (op.type == X86_OP_IMM) {
                int64_t lo_f = ins.imm_offset, n_f = ins.imm_size;
                bool any_hit = false;
                for (int64_t k : hits)
                    if (lo_f <= k && k < lo_f + n_f) any_hit = true;
                if ((calls || jumps) && ops.size() == 1) {
                    if (!calls && lo <= op.imm && op.imm < hi && index.count(op.imm)) {
                        char buf[32];
                        std::snprintf(buf, sizeof buf, "%+lld", static_cast<long long>(index[op.imm] - i));
                        parts.push_back(buf);
                        f->jumps += 1;
                    } else {                                             // leaves the function
                        for (int64_t k = lo_f; k < lo_f + n_f; k++) out.insert(k);
                        auto tgt = direct(op.imm);
                        called = tgt ? std::optional<Call>(Call{str("f"), *tgt}) : std::nullopt;
                        parts.push_back(">");
                    }
                    kinds.push_back("t");
                } else if (any_hit || (n_f == 4 && inside(op.imm & 0xFFFFFFFFLL))) {
                    for (int64_t k = lo_f; k < lo_f + n_f; k++) out.insert(k);
                    parts.push_back(ref && ref->kind == 'i' ? "!" + ref->key.utf8() : direct(op.imm) ? std::string(">") : std::string("@"));
                    kinds.push_back("a");
                } else {
                    int64_t v = op.imm & 0xFFFFFFFFLL;
                    char buf[32];
                    std::snprintf(buf, sizeof buf, "%llx", static_cast<unsigned long long>(v));
                    parts.push_back(buf);
                    kinds.push_back("i");
                    if (0x100 <= v && v < 0xFFFF0000LL && v != 0xFFFF && v != 0xFF00 && v != 0x8000 && v != 0x7FFF) {
                        std::snprintf(buf, sizeof buf, "c%llx", static_cast<unsigned long long>(v));
                        f->marks.insert(str(std::string(buf)).u32());
                    }
                }
            } else if (op.type == X86_OP_MEM) {
                int64_t lo_f = ins.disp_offset, n_f = ins.disp_size;
                bool bare = !op.base && !op.index;
                bool any_hit = false;
                for (int64_t k : hits)
                    if (lo_f <= k && k < lo_f + n_f) any_hit = true;
                std::string disp;
                if (bare || any_hit || (n_f == 4 && inside(op.disp & 0xFFFFFFFFLL))) {
                    if (n_f == 4)
                        for (int64_t k = lo_f; k < lo_f + n_f; k++) out.insert(k);
                    std::optional<str> imp;
                    if (bare) imp = slot(op.disp & 0xFFFFFFFFLL);
                    if (imp && !imp->empty() && (calls || jumps)) called = Call{str("i"), *imp};
                    disp = imp && !imp->empty() ? "!" + imp->utf8() : ref && ref->kind == 'i' ? "!" + ref->key.utf8() : std::string("@");
                } else {
                    char buf[32];
                    std::snprintf(buf, sizeof buf, "%llx", static_cast<unsigned long long>(op.disp & 0xFFFFFFFFLL));
                    disp = buf;
                }
                parts.push_back("[" + (op.segment ? md.reg_name(op.segment).utf8() : std::string()) + ":" + (op.base ? md.reg_name(op.base).utf8() : std::string()) +
                                "+" + (op.index ? md.reg_name(op.index).utf8() : std::string()) + "*" + std::to_string(op.scale) + "+" + disp + "]");
                kinds.push_back("m");
            }
        }
        if (called) {
            f->calls.push_back(*called);
            if (called->kind == str("i")) f->marks.insert((str("i") + called->key).u32());
        }
        py::set<int64_t> blank_at;
        for (int64_t k : out) blank_at.add(k);
        raw.push_back(mask::blank(ins.bytes, blank_at).raw());
        std::string row = mn + " " + ins.op_str.utf8();
        {   // ("%s %s" % (mn, ins.op_str)).strip()
            str s = str(row).strip();
            f->rows.emplace_back(addr, s);
        }
        std::string joined;
        for (size_t k = 0; k < parts.size(); k++) joined += (k ? "," : "") + parts[k];
        std::string kind_text;
        for (const auto& k : kinds) kind_text += k;
        texts.push_back(mn + " " + joined);
        f->text.push_back(str(mn + " " + joined));
        f->shape.push_back(str(alike(mn, jumps).utf8() + " " + kind_text));
        shapes.push_back(f->shape.back().utf8());
    }
    f->ids.bytes = digest(raw);
    f->ids.text = digest(texts);
    f->ids.shape = digest(shapes);
    return f;
}

str key16(int64_t seg, int64_t off) { return py::percent("%d:%04x", seg, off); }

// _sites(nef, seg, segs, entries, refs): {offset in the segment: what the relocated byte there refers to}
std::unordered_map<int64_t, Ref> sites_of(const formats::ne::NEFile& nef, const formats::ne::Segment& seg,
                                          const std::map<int64_t, formats::ne::Segment>& segs,
                                          const std::map<int64_t, formats::ne::Entry>& entries, const py::list<str>& refs) {
    int64_t base = seg.file_off;
    std::unordered_map<int64_t, Ref> out;
    for (const formats::ne::Relocation& r : nef.relocations(seg)) {
        const str& kind = r.kind;
        Ref what{'d', str("")};
        if (kind == str("IMPORTORD") || kind == str("IMPORTNAME")) {
            str mod = (0 < *r.module && *r.module <= refs.size()) ? refs[*r.module - 1] : str("?");
            what = Ref{'i', py::percent("%s.%s", mod, kind == str("IMPORTORD") ? py::to_str(*r.ordinal) : nef.imported_name(*r.name_off))};
        } else if (kind == str("INTERNAL")) {
            std::optional<std::pair<int64_t, int64_t>> e;                  // (seg, offset)
            if (*r.seg == 0xFF) {
                auto it = entries.find(*r.entry);
                if (it != entries.end()) e = std::make_pair(*it->second.seg, *it->second.offset);
            } else {
                e = std::make_pair(*r.seg, *r.entry);
            }
            bool code = false;
            if (e) {
                auto s = segs.find(e->first);
                code = s != segs.end() && s->second.kind == str("CODE") && r.src == 3;
            }
            if (code) what = Ref{'f', key16(e->first, e->second)};
        }
        int64_t width = r.src == 3 ? 4 : r.src == 2 ? 2 : r.src == 5 ? 2 : r.src == 0 ? 1 : 2;
        int64_t off = r.offset, hops = 0;
        while (off != 0xFFFF && off + 1 < seg.length && hops < 0x10000) {
            for (int64_t k = 0; k < width; k++) out[off + k] = what;
            if (r.additive) break;
            off = nef.u16(base + off);
            hops++;
        }
    }
    return out;
}

std::shared_ptr<Side> load_pe(const str& path) {
    formats::pe::PEFile pe = formats::pe::open_pe(path);
    auto spans = formats::pe_functions::function_spans(path);
    auto sections = pe.code_sections();
    std::unordered_set<int64_t> relocated;
    for (int64_t rva : pe.base_relocs())
        for (int64_t k = 0; k < 4; k++) relocated.insert(static_cast<int64_t>(pe.image_base) + rva + k);
    std::unordered_map<int64_t, str> imports;
    for (const auto& kv : pe.imports().items()) {
        const auto& dll = std::get<0>(std::get<1>(kv));
        imports[static_cast<int64_t>(std::get<0>(kv))] = py::percent("%s.%s", std::get<0>(py::os::path::splitext(dll)).upper(), std::get<1>(std::get<1>(kv)));
    }
    int64_t low = static_cast<int64_t>(pe.image_base);
    int64_t span_max = 0;
    for (const auto& s : pe.sections) span_max = std::max(span_max, s.vaddr + std::max(s.vsize, s.rawsize));
    int64_t high = low + span_max;
    auto md = decoder(32);
    int64_t code = 0;
    for (const auto& s : sections) code += std::get<1>(s).size();
    auto side = std::make_shared<Side>(path, str("pe"), code);
    std::vector<int64_t> starts;
    for (const auto& kv : spans.items()) starts.push_back(std::get<0>(kv));
    std::sort(starts.begin(), starts.end());
    for (int64_t va : starts) {
        const auto& sp = spans.at(va);
        int64_t hi = std::get<0>(sp);
        const std::optional<str>& name = std::get<1>(sp);
        int64_t base = 0;
        const py::bytes* data = nullptr;
        for (const auto& s : sections) {
            int64_t b = static_cast<int64_t>(std::get<0>(s));
            if (b <= va && va < b + std::get<1>(s).size()) { base = b; data = &std::get<1>(s); break; }
        }
        if (!data) throw py::StopIteration();
        bool named = name && !name->empty() && !name->startswith(str("Ord"));
        auto stream = md->disasm(data->slice(va - base, hi - base), va);
        side->add(make_function(py::percent("%x", va), named ? *name : py::percent("sub_%06X", va - static_cast<int64_t>(pe.image_base)), named,
                                body_of(stream, va, hi), va, hi, [&](int64_t a) { return relocated.count(a) != 0; },
                                [&](int64_t t) -> std::optional<str> {
                                    return spans.contains(t) ? std::optional<str>(py::percent("%x", t)) : std::nullopt;
                                },
                                [](int64_t) -> std::optional<Ref> { return std::nullopt; },
                                [&](int64_t a) -> std::optional<str> {
                                    auto it = imports.find(a);
                                    return it == imports.end() ? std::nullopt : std::optional<str>(it->second);
                                },
                                [&](int64_t v) { return low <= v && v < high; }, *md));
    }
    side->done_();
    return side;
}

std::shared_ptr<Side> load_ne(const str& path) {
    formats::ne::NEFile nef(path);
    std::map<int64_t, formats::ne::Segment> segs;
    for (const auto& s : nef.segments()) segs[s.idx] = s;
    std::map<int64_t, formats::ne::Entry> entries;
    for (const auto& e : nef.entries())
        if (e.kind == str("FIXED") || e.kind == str("MOVEABLE")) entries[e.ordinal] = e;
    py::list<str> refs = nef.module_refs();
    std::map<std::pair<int64_t, int64_t>, str> exported;
    auto names_of = nef.resident_names();
    auto non = nef.nonresident_names();
    std::vector<std::tuple<str, int64_t>> all;
    for (int64_t k = 1; k < names_of.size(); k++) all.push_back(names_of[k]);
    for (int64_t k = 1; k < non.size(); k++) all.push_back(non[k]);
    for (const auto& no : all) {
        auto it = entries.find(std::get<1>(no));
        if (it != entries.end()) exported.emplace(std::make_pair(*it->second.seg, *it->second.offset), std::get<0>(no));   // (setdefault)
    }
    std::map<int64_t, std::set<int64_t>> starts;
    for (const auto& kv : decompile::describe::functions(nef).items()) starts[std::get<0>(std::get<0>(kv))].insert(std::get<1>(std::get<0>(kv)));
    auto md = decoder(16);
    int64_t code_total = 0;
    for (const auto& kv : segs)
        if (kv.second.kind == str("CODE") && kv.second.file_off) code_total += kv.second.length;
    auto side = std::make_shared<Side>(path, str("ne"), code_total);
    for (auto& ks : starts) {
        int64_t idx = ks.first;
        auto sit = segs.find(idx);
        if (sit == segs.end() || sit->second.kind != str("CODE") || !sit->second.file_off) continue;
        const formats::ne::Segment& s = sit->second;
        py::bytes code = nef.data.slice(s.file_off, s.file_off + s.length);
        auto sites = sites_of(nef, s, segs, entries, refs);
        std::set<int64_t> offs;
        for (int64_t o : ks.second)
            if (o < code.size()) offs.insert(o);
        std::map<int64_t, std::pair<std::vector<Ins>, int64_t>> bodies;
        for (int round = 0; round < 8; round++) {                    // near calls name more starts
            std::vector<int64_t> order(offs.begin(), offs.end());
            std::set<int64_t> fresh;
            for (size_t n = 0; n < order.size(); n++) {
                int64_t lo = order[n];
                int64_t hi = n + 1 < order.size() ? order[n + 1] : code.size();
                auto b = bodies.find(lo);
                if (b != bodies.end() && b->second.second == hi) continue;
                auto body = body_of(md->disasm(code.slice(lo, hi), lo), lo, hi);
                bodies[lo] = {body, hi};
                for (const Ins& ins : body) {
                    if (ins.mnemonic == str("call") && ins.operands[0].type == X86_OP_IMM) {
                        int64_t t = ins.operands[0].imm;
                        if (t < code.size() && !offs.count(t)) fresh.insert(t);
                    }
                }
            }
            if (fresh.empty() || round == 7) break;
            offs.insert(fresh.begin(), fresh.end());
        }
        for (int64_t lo : offs) {
            auto& bh = bodies.at(lo);
            auto ex = exported.find({idx, lo});
            std::optional<str> name;
            if (ex != exported.end()) name = ex->second;
            bool named = name && !name->empty();
            side->add(make_function(key16(idx, lo), named ? *name : py::percent("sub_%d_%04X", idx, lo), named, bh.first, lo, bh.second,
                                    [&](int64_t a) { return sites.count(a) != 0; },
                                    [&](int64_t t) -> std::optional<str> { return offs.count(t) ? std::optional<str>(key16(idx, t)) : std::nullopt; },
                                    [&](int64_t a) -> std::optional<Ref> {
                                        auto it = sites.find(a);
                                        return it == sites.end() ? std::nullopt : std::optional<Ref>(it->second);
                                    },
                                    [](int64_t) -> std::optional<str> { return std::nullopt; }, [](int64_t) { return false; }, *md));
        }
    }
    side->done_();
    return side;
}

std::shared_ptr<Side> load_dos(const str& path) {
    formats::dos::Program p = formats::dos::read(path);
    auto analysis = analyze::dos_functions::discover(p);
    auto& found = analysis.functions;
    std::vector<int64_t> starts;
    for (const auto& kv : found.items()) starts.push_back(std::get<0>(kv));
    std::sort(starts.begin(), starts.end());
    int64_t end = p.base + p.image.size();
    auto md = decoder(16);
    int64_t code = 0;
    for (const auto& sp : p.spans) code += std::get<1>(sp);
    auto side = std::make_shared<Side>(path, str("dos"), code);
    std::vector<int64_t> relocs;
    for (int64_t r : p.relocations) relocs.push_back(r);
    std::sort(relocs.begin(), relocs.end());
    for (size_t n = 0; n < starts.size(); n++) {
        int64_t lin = starts[n];
        auto fn = found.at(lin);
        int64_t seg = fn->cs;
        int64_t hi = n + 1 < starts.size() ? starts[n + 1] : end;
        if (!(p.base <= lin && lin < end) || !(0 <= lin - seg && lin - seg < 0x10000)) continue;
        int64_t lo_off = lin - seg, hi_off = std::min<int64_t>(hi - seg, 0x10000);
        std::unordered_set<int64_t> relocated;
        for (auto it = std::lower_bound(relocs.begin(), relocs.end(), lin); it != relocs.end() && *it < hi; ++it) relocated.insert(*it - seg);
        auto body = body_of(md->disasm(p.image.slice(lin - p.base, hi - p.base), lo_off), lo_off, hi_off);
        bool named = !fn->name.empty() && !fn->name.startswith(str("sub_"));
        side->add(make_function(py::percent("%05x", lin), named ? fn->name : py::percent("sub_%05X", lin), named, body, lo_off, hi_off,
                                [&](int64_t a) { return relocated.count(a) != 0; },
                                [&](int64_t t) -> std::optional<str> {
                                    return found.contains(seg + t) ? std::optional<str>(py::percent("%05x", seg + t)) : std::nullopt;
                                },
                                [](int64_t) -> std::optional<Ref> { return std::nullopt; },
                                [](int64_t) -> std::optional<str> { return std::nullopt; }, [](int64_t) { return false; }, *md));
    }
    side->done_();
    return side;
}

}  // namespace

std::shared_ptr<Side> load(const str& path) {
    std::optional<str> kind = formats::detect::kind(path);
    if (kind && *kind == str("pe")) return load_pe(path);
    if (kind && *kind == str("ne")) return load_ne(path);
    if (kind && *kind == str("dos")) return load_dos(path);
    throw py::ValueError(py::percent("%s: functions of a %s file are not compared yet", py::os::path::basename(path),
                                     kind && !kind->empty() ? *kind : str("non-executable")).utf8());
}

std::vector<std::pair<int64_t, Where>> shared(const py::list<str>& paths, const str& level, int64_t least) {
    struct Entry {
        Where where;
        int64_t size = 0;
    };
    std::vector<std::pair<str, Entry>> table;                  // hash -> {program name: [keys]}, in the order met
    for (const str& path : paths) {
        auto side = load(path);
        for (const auto& kv : side->functions.items()) {
            const Function& f = *std::get<1>(kv);
            if (static_cast<int64_t>(f.text.size()) >= least) {
                const str& h = f.ids.at(level);
                auto it = std::find_if(table.begin(), table.end(), [&](const auto& p) { return p.first == h; });
                if (it == table.end()) {
                    table.emplace_back(h, Entry());
                    it = table.end() - 1;
                }
                auto w = std::find_if(it->second.where.begin(), it->second.where.end(), [&](const auto& p) { return p.first == side->name; });
                if (w == it->second.where.end()) {
                    it->second.where.emplace_back(side->name, py::list<str>());
                    w = it->second.where.end() - 1;
                }
                w->second.append(f.key);
                it->second.size = static_cast<int64_t>(f.text.size());
            }
        }
    }
    std::vector<std::pair<int64_t, Where>> out;
    for (auto& e : table)
        if (e.second.where.size() >= 2) out.emplace_back(e.second.size, e.second.where);
    std::stable_sort(out.begin(), out.end(), [](const auto& a, const auto& b) {
        if (a.first != b.first) return a.first > b.first;
        std::vector<std::u32string> na, nb;
        for (const auto& w : a.second) na.push_back(w.first.u32());
        for (const auto& w : b.second) nb.push_back(w.first.u32());
        std::sort(na.begin(), na.end());
        std::sort(nb.begin(), nb.end());
        return na < nb;
    });
    return out;
}

// ---- matching
double likeness(const Function& fa, const Function& fb, std::map<std::pair<std::u32string, std::u32string>, double>* memo) {
    if (fa.ids.shape == fb.ids.shape) return 1.0;
    auto key = std::make_pair(fa.key.u32(), fb.key.u32());
    if (memo) {
        auto it = memo->find(key);
        if (it != memo->end()) return it->second;
    }
    double v = ratio_of(fa.shape, fb.shape);
    if (memo) (*memo)[key] = v;
    return v;
}

namespace {

// _grams(f): the runs of five shapes, each as a number that is the same in every process (see the header)
std::set<uint64_t> grams_of(const Function& f, int n = 5) {
    std::set<uint64_t> out;
    int64_t len = f.count();
    for (int64_t i = 0; i + n <= len; i++) {
        std::string joined;
        for (int k = 0; k < n; k++) joined += (k ? "\n" : "") + f.shape[static_cast<size_t>(i + k)].utf8();
        std::string d = blake2b8(joined);
        uint64_t v = 0;
        for (unsigned char c : d) v = (v << 8) | c;
        out.insert(v);
    }
    return out;
}

}  // namespace

py::dict<str, std::tuple<str, str>> match(const Side& a, const Side& b, double least) {
    py::dict<str, std::tuple<str, str>> pair;
    py::dict<str, str> back;
    std::map<std::pair<std::u32string, std::u32string>, double> memo;
    const auto& A = a.functions;
    const auto& B = b.functions;

    auto link = [&](const str& x, const str& y, const char* how) {
        const Function& fa = *A.at(x);
        const Function& fb = *B.at(y);
        if (pair.contains(x) || back.contains(y) || (fa.named && fb.named && fa.name != fb.name)) return;
        pair.set(x, std::make_tuple(y, str(how)));
        back.set(y, x);
    };

    auto unique = [&](const std::function<std::optional<str>(const Function&)>& of, const char* how, int64_t least_count) {
        std::vector<std::pair<std::optional<str>, std::vector<const Function*>>> ga, gb;
        auto group = [&](std::vector<std::pair<std::optional<str>, std::vector<const Function*>>>& g, const std::optional<str>& k, const Function* f) {
            auto it = std::find_if(g.begin(), g.end(), [&](const auto& p) { return p.first == k; });
            if (it == g.end()) {
                g.emplace_back(k, std::vector<const Function*>());
                it = g.end() - 1;
            }
            it->second.push_back(f);
        };
        // (hash maps keep this linear: the original's dicts are)
        std::unordered_map<std::string, size_t> ia, ib;
        auto add = [&](std::vector<std::pair<std::optional<str>, std::vector<const Function*>>>& g, std::unordered_map<std::string, size_t>& idx,
                       const std::optional<str>& k, const Function* f) {
            std::string key = k ? "s" + k->utf8() : std::string("n");
            auto it = idx.find(key);
            if (it == idx.end()) {
                idx[key] = g.size();
                g.emplace_back(k, std::vector<const Function*>{f});
            } else {
                g[it->second].second.push_back(f);
            }
        };
        (void)group;
        for (const auto& kv : A.items()) {
            const Function& f = *std::get<1>(kv);
            if (!pair.contains(f.key) && f.count() >= least_count) add(ga, ia, of(f), &f);
        }
        for (const auto& kv : B.items()) {
            const Function& f = *std::get<1>(kv);
            if (!back.contains(f.key) && f.count() >= least_count) add(gb, ib, of(f), &f);
        }
        for (const auto& g : ga) {
            std::string key = g.first ? "s" + g.first->utf8() : std::string("n");
            auto it = ib.find(key);
            if (g.first && g.second.size() == 1 && it != ib.end() && gb[it->second].second.size() == 1)
                link(g.second[0]->key, gb[it->second].second[0]->key, how);
        }
    };

    auto calls_agree = [&](const Function& fa, const Function& fb) {
        int64_t plus = 0, minus = 0;
        if (fa.calls.size() == fb.calls.size()) {
            for (size_t i = 0; i < fa.calls.size(); i++) {
                const Call& ca = fa.calls[i];
                const Call& cb = fb.calls[i];
                if (ca.kind != cb.kind) minus += 1;
                else if (ca.kind == str("i")) {
                    plus += ca == cb;
                    minus += !(ca == cb);
                } else if (pair.contains(ca.key)) {
                    bool same = std::get<0>(pair.at(ca.key)) == cb.key;
                    plus += same;
                    minus += !same;
                } else if (back.contains(cb.key)) {
                    minus += 1;
                }
            }
        }
        return std::make_pair(plus, minus);
    };

    auto by_calls = [&]() {
        struct Vote {
            str x, y;
            int64_t n;
        };
        std::vector<Vote> votes;                                  // the Counter, in the order keys were first voted for
        std::unordered_map<std::string, size_t> vindex;
        auto vote = [&](const str& x, const str& y, int64_t n) {
            std::string k = x.utf8() + std::string(1, '\0') + y.utf8();
            auto it = vindex.find(k);
            if (it == vindex.end()) {
                vindex[k] = votes.size();
                votes.push_back({x, y, n});
            } else {
                votes[it->second].n += n;
            }
        };
        for (const auto& kv : pair.items()) {
            const str& x = std::get<0>(kv);
            const str y = std::get<0>(std::get<1>(kv));
            const Function& fx = *A.at(x);
            const Function& fy = *B.at(y);
            const auto& ca = fx.calls;
            const auto& cb = fy.calls;
            if (!ca.empty() && !cb.empty()) {
                Interner in;
                std::vector<int> la, lb;
                std::vector<bool> la_unknown, lb_unknown;
                for (const Call& c : ca) {
                    if (c.kind == str("i")) { la.push_back(in.of("i\n" + c.key.utf8())); la_unknown.push_back(false); }
                    else if (pair.contains(c.key)) { la.push_back(in.of("p\n" + std::get<0>(pair.at(c.key)).utf8())); la_unknown.push_back(false); }
                    else { la.push_back(in.of("?")); la_unknown.push_back(true); }
                }
                for (const Call& c : cb) {
                    if (c.kind == str("i")) { lb.push_back(in.of("i\n" + c.key.utf8())); lb_unknown.push_back(false); }
                    else if (back.contains(c.key)) { lb.push_back(in.of("p\n" + c.key.utf8())); lb_unknown.push_back(false); }
                    else { lb.push_back(in.of("?")); lb_unknown.push_back(true); }
                }
                std::vector<std::tuple<const char*, int, int, int, int>> ops;
                if (la == lb) ops.emplace_back("equal", 0, static_cast<int>(la.size()), 0, static_cast<int>(lb.size()));
                else ops = SeqMatcher(la, lb).get_opcodes();
                for (const auto& op : ops) {
                    std::string tag = std::get<0>(op);
                    int i1 = std::get<1>(op), i2 = std::get<2>(op), j1 = std::get<3>(op), j2 = std::get<4>(op);
                    if (tag == "equal" || (tag == "replace" && i2 - i1 == j2 - j1)) {
                        for (int i = i1, j = j1; i < i2; i++, j++) {
                            if (la_unknown[static_cast<size_t>(i)] && lb_unknown[static_cast<size_t>(j)] && A.contains(ca[static_cast<size_t>(i)].key) &&
                                B.contains(cb[static_cast<size_t>(j)].key))
                                vote(ca[static_cast<size_t>(i)].key, cb[static_cast<size_t>(j)].key, tag == "equal" ? 2 : 1);
                        }
                    }
                }
            }
            std::vector<std::u32string> ua, ub;
            auto ac = a.callers.find(x.u32());
            if (ac != a.callers.end())
                for (const auto& c : ac->second)
                    if (!pair.contains(str(c))) ua.push_back(c);
            auto bc = b.callers.find(y.u32());
            if (bc != b.callers.end())
                for (const auto& c : bc->second)
                    if (!back.contains(str(c))) ub.push_back(c);
            if (ua.size() == 1 && ub.size() == 1) vote(str(ua[0]), str(ub[0]), 1);
        }
        // best_a / best_b
        struct Best {
            std::pair<int64_t, double> score;
            str other;
        };
        std::vector<std::pair<str, Best>> best_a;
        std::unordered_map<std::string, size_t> ia;
        std::vector<std::pair<str, Best>> best_b;
        std::unordered_map<std::string, size_t> ib;
        auto offer = [](std::vector<std::pair<str, Best>>& v, std::unordered_map<std::string, size_t>& idx, const str& k, const std::pair<int64_t, double>& score, const str& other) {
            auto it = idx.find(k.utf8());
            std::pair<int64_t, double> cur{0, 0};
            if (it != idx.end()) cur = v[it->second].second.score;
            if (score > cur) {
                if (it != idx.end()) v[it->second].second = {score, other};
                else {
                    idx[k.utf8()] = v.size();
                    v.emplace_back(k, Best{score, other});
                }
            }
        };
        for (const Vote& vt : votes) {
            const Function& fx = *A.at(vt.x);
            const Function& fy = *B.at(vt.y);
            bool same = false;
            for (const str& lv : LEVELS)
                if (fx.ids.at(lv) == fy.ids.at(lv)) same = true;
            std::pair<int64_t, double> score{vt.n + (same ? 2 : 0), !same ? likeness(fx, fy, &memo) : 1.0};
            if ((!same && score.second < 0.4) || (fx.named && fy.named && fx.name != fy.name)) continue;
            offer(best_a, ia, vt.x, score, vt.y);
            offer(best_b, ib, vt.y, score, vt.x);
        }
        for (const auto& e : best_a) {
            const str& y = e.second.other;
            if (best_b[ib.at(y.utf8())].second.other == e.first) link(e.first, y, "calls");
        }
    };

    auto by_likeness = [&]() {
        std::unordered_map<std::string, std::vector<const Function*>> posting_marks;
        std::map<uint64_t, std::vector<const Function*>> posting_grams;
        for (const auto& kv : B.items()) {
            const Function& f = *std::get<1>(kv);
            if (!back.contains(f.key) && f.count() >= 5) {
                for (const auto& m : f.marks) posting_marks[str(m).utf8()].push_back(&f);
                for (uint64_t g : grams_of(f)) posting_grams[g].push_back(&f);
            }
        }
        struct Scored {
            double score;
            str other;
        };
        std::vector<std::pair<str, std::vector<Scored>>> of_a, of_b;
        std::unordered_map<std::string, size_t> ia, ib;
        auto push = [](std::vector<std::pair<str, std::vector<Scored>>>& v, std::unordered_map<std::string, size_t>& idx, const str& k, const Scored& s) {
            auto it = idx.find(k.utf8());
            if (it == idx.end()) {
                idx[k.utf8()] = v.size();
                v.emplace_back(k, std::vector<Scored>{s});
            } else {
                v[it->second].second.push_back(s);
            }
        };
        for (const auto& kv : A.items()) {
            const Function& f = *std::get<1>(kv);
            if (pair.contains(f.key) || f.count() < 5) continue;
            std::vector<std::pair<const Function*, int64_t>> cand;           // the Counter, in the order keys were first met
            std::unordered_map<const Function*, size_t> cidx;
            auto hit = [&](const std::vector<const Function*>& hits) {
                if (hits.size() > 40) return;
                for (const Function* h : hits) {
                    auto it = cidx.find(h);
                    if (it == cidx.end()) {
                        cidx[h] = cand.size();
                        cand.emplace_back(h, 1);
                    } else {
                        cand[it->second].second++;
                    }
                }
            };
            for (const auto& m : f.marks) {                                    // sorted marks, then sorted run ids
                auto it = posting_marks.find(str(m).utf8());
                if (it != posting_marks.end()) hit(it->second);
            }
            for (uint64_t g : grams_of(f)) {
                auto it = posting_grams.find(g);
                if (it != posting_grams.end()) hit(it->second);
            }
            std::stable_sort(cand.begin(), cand.end(), [](const auto& x, const auto& y) { return x.second > y.second; });   // most_common
            for (size_t ci = 0; ci < cand.size() && ci < 10; ci++) {
                const Function& g = *cand[ci].first;
                if (2 * std::min(f.count(), g.count()) < std::max(f.count(), g.count()) || (f.named && g.named && f.name != g.name)) continue;
                double v = likeness(f, g, &memo);
                auto pm = calls_agree(f, g);
                int64_t plus = pm.first, minus = pm.second;
                bool share = false;
                for (const auto& m : f.marks)
                    if (g.marks.count(m)) share = true;
                if (v < least || minus > plus || (f.count() < 12 && !plus && !share)) continue;   // a small one needs more than its shape
                double t = f.ids.text == g.ids.text ? 1.0 : ratio_of(f.text, g.text);
                double score = (v + t) / 2 + (plus ? 0.1 : 0.0);
                push(of_a, ia, f.key, Scored{score, g.key});
                push(of_b, ib, g.key, Scored{score, f.key});
            }
        }
        auto clear = [](std::vector<Scored>& found) {                          // one candidate, or one well ahead
            std::stable_sort(found.begin(), found.end(), [](const Scored& a, const Scored& b) {
                if (a.score != b.score) return a.score > b.score;
                return b.other.u32() < a.other.u32();
            });
            return found.size() == 1 || found[0].score - found[1].score >= 0.03;
        };
        for (auto& e : of_a) {
            if (clear(e.second)) {
                str y = e.second[0].other;
                auto& fb = of_b[ib.at(y.utf8())].second;
                if (clear(fb) && fb[0].other == e.first) link(e.first, y, "likeness");
            }
        }
    };

    auto by_order = [&]() {
        // Unmatched functions of the same shape standing in the same place in both builds.
        Interner in;
        std::vector<int> la, lb;
        for (const str& x : a.order.vec()) la.push_back(pair.contains(x) ? in.of("p\n" + std::get<0>(pair.at(x)).utf8()) : in.of("s\n" + A.at(x)->ids.shape.utf8()));
        for (const str& y : b.order.vec()) lb.push_back(back.contains(y) ? in.of("p\n" + y.utf8()) : in.of("s\n" + B.at(y)->ids.shape.utf8()));
        for (const auto& blk : SeqMatcher(la, lb).get_matching_blocks()) {
            std::vector<std::pair<str, str>> run;
            for (int k = 0; k < blk.k; k++) run.emplace_back(a.order[blk.i + k], b.order[blk.j + k]);
            std::vector<std::pair<str, str>> free;
            for (const auto& r : run)
                if (!pair.contains(r.first)) free.push_back(r);
            if (!free.empty() && (static_cast<int>(free.size()) < blk.k || blk.k >= 3))        // beside a matched one, or three in a row
                for (const auto& r : free) link(r.first, r.second, "position");
        }
    };

    auto by_gaps = [&]() {
        // The same few unmatched functions between two matched neighbours, if at all alike.
        std::unordered_map<std::string, int64_t> where;
        for (int64_t i = 0; i < b.order.size(); i++) where[b.order[i].utf8()] = i;
        std::vector<str> run;
        int64_t last = -1;
        std::vector<std::optional<str>> seq;
        for (const str& x : a.order.vec()) seq.push_back(x);
        seq.push_back(std::nullopt);
        for (const auto& x : seq) {
            if (x && !pair.contains(*x)) {
                run.push_back(*x);
                continue;
            }
            int64_t here = x ? where.at(std::get<0>(pair.at(*x)).utf8()) : b.order.size();
            std::vector<str> gap;
            if (here > last)
                for (int64_t k = last + 1; k < here; k++)
                    if (!back.contains(b.order[k])) gap.push_back(b.order[k]);
            if (!run.empty() && run.size() == gap.size() && run.size() <= 4) {
                for (size_t i = 0; i < run.size(); i++)
                    if (likeness(*A.at(run[i]), *B.at(gap[i]), &memo) >= 0.4) link(run[i], gap[i], "position");
            }
            run.clear();
            last = std::max(last, here);
        }
    };

    unique([](const Function& f) { return f.named ? std::optional<str>(f.name) : std::nullopt; }, "name", 0);
    unique([](const Function& f) { return std::optional<str>(f.ids.bytes); }, "bytes", 4);
    unique([](const Function& f) { return std::optional<str>(f.ids.text); }, "text", 4);
    unique([](const Function& f) { return std::optional<str>(f.ids.shape); }, "shape", 8);
    for (int round = 0; round < 64; round++) {
        int64_t n = pair.size();
        for (const std::function<void()>& step : {std::function<void()>(by_calls), std::function<void()>(by_likeness), std::function<void()>(by_order)}) {
            step();
            if (pair.size() > n) break;
        }
        if (pair.size() == n) break;
    }
    by_gaps();
    by_calls();
    return pair;
}

// ---- Comparison
namespace {

double round_to(double x, int digits) {
    char buf[64];
    std::snprintf(buf, sizeof buf, "%.*f", digits, x);
    return std::strtod(buf, nullptr);
}

}  // namespace

Comparison::Comparison(const std::shared_ptr<Side>& a_, const std::shared_ptr<Side>& b_, const py::dict<str, std::tuple<str, str>>& pair, double seconds_)
    : a(a_), b(b_), seconds(seconds_) {
    for (const str& x : a->order) {
        if (!pair.contains(x)) continue;
        const auto& yh = pair.at(x);
        const str& y = std::get<0>(yh);
        const Function& fa = *a->functions.at(x);
        const Function& fb = *b->functions.at(y);
        str level;
        for (const str& lv : LEVELS)
            if (fa.ids.at(lv) == fb.ids.at(lv)) { level = lv; break; }
        bool same = level == str("bytes") || level == str("text");
        Pair p;
        p.from = x;
        p.to = y;
        p.from_name = fa.name;
        p.to_name = fb.name;
        p.status = same ? str("identical") : str("changed");
        p.level = level;
        p.how = std::get<1>(yh);
        p.score = same ? 1.0 : round_to(likeness(fa, fb), 3);
        p.size_from = fa.size;
        p.size_to = fb.size;
        pairs.push_back(p);
    }
    std::set<std::u32string> matched;
    for (const Pair& p : pairs) matched.insert(p.to.u32());
    for (const str& x : a->order)
        if (!pair.contains(x)) removed.append(x);
    for (const str& y : b->order)
        if (!matched.count(y.u32())) added.append(y);
}

Counts Comparison::counts() const {
    Counts c;
    for (const Pair& p : pairs) {
        if (p.status == str("identical")) {
            c.identical++;
            if (p.level == str("bytes")) c.identical_bytes++;
            if (p.level == str("text")) c.identical_text++;
        } else {
            c.changed++;
        }
    }
    c.added = added.size();
    c.removed = removed.size();
    return c;
}

CodeBytes Comparison::code() const {
    CodeBytes out;
    out.to_bytes = b->size();
    for (const str& y : added) out.added += b->functions.at(y)->size;
    for (const Pair& p : pairs) (p.status == str("identical") ? out.identical : out.changed) += p.size_to;
    return out;
}

std::vector<NameRow> Comparison::names() const {
    std::vector<NameRow> out;
    for (const Pair& p : pairs)
        if (a->functions.at(p.from)->named && !b->functions.at(p.to)->named) out.push_back(NameRow{p.to, p.from_name, p.from, p.status == str("identical")});
    return out;
}

std::vector<Reconstruction> Comparison::reconstructions(const Store& fs_in) const {
    std::tuple<str, str> id;
    try {
        id = verify::reconstruct::identity(a->path);
    } catch (const py::ValueError&) {
        return {};
    } catch (const py::OSError&) {
        return {};
    }
    Store fs = fs_in ? fs_in : F::open();
    auto best = fs->best(std::get<0>(id), std::get<1>(id));
    std::vector<Reconstruction> out;
    for (const Pair& p : pairs) {
        if (!best.contains(p.from)) continue;
        const auto& row = best.at(p.from);
        str klass = engine::native_engine::cell_str(row.klass);
        bool has_source = false;
        if (std::holds_alternative<str>(row.source)) has_source = !std::get<str>(row.source).empty();
        if (p.status == str("identical") && (klass == str("verified") || klass == str("weak")) && has_source) {
            Reconstruction r;
            r.from = p.from;
            r.to = p.to;
            r.klass = klass;
            r.engine = engine::native_engine::cell_opt_str(row.engine);
            r.name = engine::native_engine::cell_opt_str(row.name);
            r.source = std::get<str>(row.source);
            out.push_back(r);
        }
    }
    return out;
}

std::optional<int64_t> Comparison::carry_names(const Store& fs_in) const {
    Store fs = fs_in ? fs_in : F::open();
    auto bodies = fs->bodies(base::corpus::sha(b->path).value_or(str("")));
    if (bodies.empty()) return std::nullopt;
    int64_t n = 0;
    for (const NameRow& row : names()) {
        if (row.identical && bodies.contains(row.to)) {
            std::optional<str> owner = base::corpus::owner(a->path);
            fs->name_body(std::get<0>(bodies.at(row.to)), row.name,
                          py::percent("the same function in %s of %s", a->name, owner && !owner->empty() ? *owner : str("another build")));
            n++;
        }
    }
    return n;
}

int64_t Comparison::carry(const Store& fs_in) const {
    Store fs = fs_in ? fs_in : F::open();
    auto found = reconstructions(fs);
    if (found.empty()) return 0;
    auto id = verify::reconstruct::identity(b->path);
    fs->add_module(std::get<0>(id), b->name, b->path, b->kind);
    fs->add_functions(std::get<0>(id), b->order);
    str old = base::corpus::sha(a->path).value_or(str(""));
    py::list<engine::native_engine::FunctionStore::RecordRow> rows;
    for (const Reconstruction& r : found) {
        engine::native_engine::FunctionStore::RecordRow row;
        row.addr = r.to;
        const Function& fb = *b->functions.at(r.to);
        row.name = fb.named ? engine::native_engine::Cell(fb.name) : (r.name ? engine::native_engine::Cell(*r.name) : engine::native_engine::Cell());
        row.source = engine::native_engine::Cell(r.source);
        row.sig = engine::native_engine::Cell(py::hashlib::sha1(r.source.encode("utf-8", "replace")).hexdigest());
        row.verdict = V::INCONCLUSIVE;
        row.reason = engine::native_engine::Cell(py::percent("carried: identical to %s %s (%s there); not judged here", a->name, r.from, r.klass));
        rows.append(row);
    }
    fs->record_many(std::get<0>(id), ENGINE, engine::native_engine::Cell(old.slice(0, 12)), std::get<1>(id), rows);
    return static_cast<int64_t>(found.size());
}

str Comparison::report() const {
    Counts c = counts();
    CodeBytes code_ = code();
    auto pc = [&](int64_t n) { return code_.to_bytes ? 100.0 * static_cast<double>(n) / static_cast<double>(code_.to_bytes) : 0.0; };
    return py::percent(
        "%s: %d functions -> %s: %d\n  identical %d (same bytes %d, same instructions %d)   changed %d   added %d   removed %d\n  of the new build's %d bytes "
        "of code: %.1f%% identical, %.1f%% changed, %.1f%% added   (%.1fs)",
        a->name, a->order.size(), b->name, b->order.size(), c.identical, c.identical_bytes, c.identical_text, c.changed, c.added, c.removed, code_.to_bytes,
        pc(code_.identical), pc(code_.changed), pc(code_.added), seconds);
}

Value Comparison::as_json(bool listings, const Store& fs) const {
    auto about = [](const Side& side) {
        Value v = Value::dict();
        v.set("path", Value(side.path));
        v.set("name", Value(side.name));
        std::optional<str> owner = base::corpus::owner(side.path);
        v.set("corpus", owner ? Value(*owner) : Value());
        std::optional<str> sha = base::corpus::sha(side.path);
        v.set("sha", sha ? Value(*sha) : Value());
        v.set("kind", Value(side.kind));
        v.set("functions", Value(side.order.size()));
        v.set("bytes", Value(side.size()));
        v.set("code", Value(side.code));
        return v;
    };
    auto lines = [](const Side& side, const std::vector<str>& keys) {
        Value d = Value::dict();
        for (const str& k : keys) {
            const Function& f = *side.functions.at(k);
            Value l = Value::list();
            for (size_t i = 0; i < f.rows.size() && i < f.text.size(); i++) {
                Value row = Value::list();
                char buf[32];
                std::snprintf(buf, sizeof buf, "%llx", static_cast<unsigned long long>(std::get<0>(f.rows[i])));
                row.push(Value(str(std::string(buf))));
                row.push(Value(std::get<1>(f.rows[i])));
                row.push(Value(f.text[i]));
                l.push(std::move(row));
            }
            d.set(k, std::move(l));
        }
        return d;
    };
    Value out = Value::dict();
    out.set("from", about(*a));
    out.set("to", about(*b));
    Counts c = counts();
    Value cv = Value::dict();
    cv.set("identical", Value(c.identical));
    cv.set("identical_bytes", Value(c.identical_bytes));
    cv.set("identical_text", Value(c.identical_text));
    cv.set("changed", Value(c.changed));
    cv.set("added", Value(c.added));
    cv.set("removed", Value(c.removed));
    out.set("counts", cv);
    CodeBytes cb = code();
    Value cbv = Value::dict();
    cbv.set("to_bytes", Value(cb.to_bytes));
    cbv.set("identical", Value(cb.identical));
    cbv.set("changed", Value(cb.changed));
    cbv.set("added", Value(cb.added));
    out.set("code", cbv);
    out.set("seconds", Value(round_to(seconds, 2)));
    Value pv = Value::list();
    for (const Pair& p : pairs) {
        Value d = Value::dict();
        d.set("from", Value(p.from));
        d.set("to", Value(p.to));
        d.set("from_name", Value(p.from_name));
        d.set("to_name", Value(p.to_name));
        d.set("status", Value(p.status));
        d.set("level", Value(p.level));
        d.set("how", Value(p.how));
        d.set("score", Value(p.score));
        Value sizes = Value::list();
        sizes.push(Value(p.size_from));
        sizes.push(Value(p.size_to));
        d.set("sizes", sizes);
        if (p.status == str("changed") && listings) {
            Interner in;
            std::vector<int> ta = intern(in, a->functions.at(p.from)->text), tb = intern(in, b->functions.at(p.to)->text);
            Value ops = Value::list();
            for (const auto& op : SeqMatcher(ta, tb).get_opcodes()) {
                Value o = Value::list();
                o.push(Value(std::get<0>(op)));
                o.push(Value(std::get<1>(op)));
                o.push(Value(std::get<2>(op)));
                o.push(Value(std::get<3>(op)));
                o.push(Value(std::get<4>(op)));
                ops.push(std::move(o));
            }
            d.set("ops", std::move(ops));
        }
        pv.push(std::move(d));
    }
    out.set("pairs", pv);
    auto listed = [](const Side& side, const py::list<str>& keys) {
        Value l = Value::list();
        for (const str& k : keys) {
            Value d = Value::dict();
            d.set("key", Value(k));
            d.set("name", Value(side.functions.at(k)->name));
            d.set("size", Value(side.functions.at(k)->size));
            l.push(std::move(d));
        }
        return l;
    };
    out.set("added", listed(*b, added));
    out.set("removed", listed(*a, removed));
    Value nv = Value::list();
    for (const NameRow& n : names()) {
        Value d = Value::dict();
        d.set("to", Value(n.to));
        d.set("name", Value(n.name));
        d.set("from", Value(n.from));
        d.set("identical", Value(n.identical));
        nv.push(std::move(d));
    }
    out.set("names", nv);
    Value rv = Value::list();
    for (const Reconstruction& r : reconstructions(fs)) {
        Value d = Value::dict();
        d.set("from", Value(r.from));
        d.set("to", Value(r.to));
        d.set("class", Value(r.klass));
        d.set("engine", r.engine ? Value(*r.engine) : Value());
        d.set("name", r.name ? Value(*r.name) : Value());
        d.set("source", Value(r.source));
        rv.push(std::move(d));
    }
    out.set("reconstructions", rv);
    if (listings) {
        std::vector<str> from_keys, to_keys;
        for (const Pair& p : pairs)
            if (p.status == str("changed")) {
                from_keys.push_back(p.from);
                to_keys.push_back(p.to);
            }
        for (const str& k : removed) from_keys.push_back(k);
        for (const str& k : added) to_keys.push_back(k);
        Value l = Value::dict();
        l.set("from", lines(*a, from_keys));
        l.set("to", lines(*b, to_keys));
        out.set("listings", l);
    }
    return out;
}

Comparison compare(const str& path_a, const str& path_b, double least) {
    double t = clock()();
    auto a = load(path_a);
    auto b = load(path_b);
    auto pair = match(*a, *b, least);
    return Comparison(a, b, pair, clock()() - t);
}

// ---- a whole product
namespace {

ModuleRow one_module(double least, const std::tuple<str, str, str>& item) {
    const str& name = std::get<0>(item);
    const str& old = std::get<1>(item);
    const str& new_ = std::get<2>(item);
    ModuleRow row;
    row.name = name;
    row.status = str("compared");
    try {
        if (base::corpus::sha(old) == base::corpus::sha(new_)) {
            auto side = load(new_);
            row.status = str("same file");
            row.functions = side->order.size();
            row.bytes = side->size();
            row.identical = side->order.size();
            row.identical_code = side->size();
            row.changed = 0;
            row.changed_code = 0;
            row.added = 0;
            row.removed = 0;
            row.seconds = 0.0;
            return row;
        }
        Comparison c = compare(old, new_, least);
        Counts n = c.counts();
        CodeBytes code = c.code();
        row.functions = c.b->order.size();
        row.bytes = code.to_bytes;
        row.identical = n.identical;
        row.identical_code = code.identical;
        row.changed = n.changed;
        row.changed_code = code.changed;
        row.added = n.added;
        row.removed = n.removed;
        row.seconds = round_to(c.seconds, 1);
    } catch (const py::Exception& e) {                          // one module must not end the run
        row.status = str("failed");
        row.error = py::percent("%s: %s", str(e.type()), str(std::string(e.what())).slice(0, 120));
    }
    return row;
}

}  // namespace

Value to_value(const ModuleRow& r) {
    Value v = Value::dict();
    v.set("name", Value(r.name));
    v.set("status", Value(r.status));
    if (r.error) v.set("error", Value(*r.error));
    auto put = [&](const char* k, const std::optional<int64_t>& x) {
        if (x) v.set(k, Value(*x));
    };
    put("functions", r.functions);
    put("bytes", r.bytes);
    put("identical", r.identical);
    put("identical_code", r.identical_code);
    put("changed", r.changed);
    put("changed_code", r.changed_code);
    put("added", r.added);
    put("removed", r.removed);
    if (r.seconds) v.set("seconds", Value(*r.seconds));
    return v;
}

Value to_value(const Product& p) {
    Value v = Value::dict();
    v.set("from", Value(p.from));
    v.set("to", Value(p.to));
    Value mods = Value::list();
    for (const ModuleRow& r : p.modules) mods.push(to_value(r));
    v.set("modules", std::move(mods));
    Value t = Value::dict();
    for (const auto& kv : p.totals) t.set(kv.first, Value(kv.second));
    v.set("totals", std::move(t));
    return v;
}

Product product(const str& old, const str& new_, int64_t sample, std::optional<int64_t> jobs, double least, const std::function<void(const str&)>& log_in) {
    auto log = log_in ? log_in : std::function<void(const str&)>([](const str& s) {
        py::print(s);
        std::fflush(stdout);
    });
    py::list<str> paths;
    {
        base::corpus::selected scope(new_);
        std::vector<str> all;
        for (const str& p : base::corpus::modules(str("pe"))) all.push_back(p);
        for (const str& p : base::corpus::modules(str("ne"))) all.push_back(p);
        std::stable_sort(all.begin(), all.end(), [](const str& a, const str& b) { return py::os::path::basename(a).lower().u32() < py::os::path::basename(b).lower().u32(); });
        if (sample && sample < static_cast<int64_t>(all.size())) {
            std::vector<str> pick;
            for (int64_t i = 0; i < sample; i++) pick.push_back(all[static_cast<size_t>(i * static_cast<int64_t>(all.size()) / sample)]);
            all = pick;
        }
        for (const str& p : all) paths.append(p);
    }
    py::list<std::tuple<str, str, str>> items;
    std::vector<ModuleRow> rows;
    struct NewModule {
        ModuleRow row;
        str path;
    };
    std::vector<NewModule> fresh;
    {
        base::corpus::selected scope(old);
        for (const str& p : paths) {
            str name = py::os::path::basename(p).lower();
            std::optional<str> before = base::corpus::retail(name);
            if (before && !before->empty()) {
                std::optional<str> lane = base::corpus::lane(*before);
                if (lane && (*lane == str("pe") || *lane == str("ne"))) {
                    items.append(std::make_tuple(name, *before, p));
                    continue;
                }
            }
            ModuleRow r;
            r.name = name;
            r.status = str("new module");
            fresh.push_back({r, p});
        }
    }
    jobs::pool::Options<std::tuple<str, str, str>, ModuleRow> opts;
    opts.jobs = std::min<int64_t>(jobs && *jobs ? *jobs : jobs::pool::default_jobs(), 6);
    opts.small = 4;
    opts.crashed = [](const std::tuple<str, str, str>& item) {
        ModuleRow r;
        r.name = std::get<0>(item);
        r.status = str("failed");
        r.error = str("the worker died");
        return r;
    };
    auto done = jobs::pool::run_first(
        [](const double& l, const std::tuple<str, str, str>& item) { return one_module(l, item); }, items, least, opts);
    for (auto& f : fresh) {                                     // a new module is all added code
        ModuleRow r = f.row;
        try {
            auto side = load(f.path);
            r.functions = side->order.size();
            r.bytes = side->size();
            r.added = side->order.size();
        } catch (const py::Exception& e) {
            r.status = str("failed");
            r.error = py::percent("%s: %s", str(e.type()), str(std::string(e.what())).slice(0, 120));
        }
        rows.push_back(r);
    }
    for (const auto& kv : done.items()) rows.push_back(std::get<1>(kv));
    std::stable_sort(rows.begin(), rows.end(), [](const ModuleRow& a, const ModuleRow& b) { return a.name.u32() < b.name.u32(); });
    Product out;
    out.from = old;
    out.to = new_;
    auto bump = [&](const str& k, int64_t n) {
        for (auto& e : out.totals)
            if (e.first == k) { e.second += n; return; }
        out.totals.emplace_back(k, n);
    };
    for (const ModuleRow& r : rows) {
        bump(str("modules"), 1);
        bump(r.status, 1);
        bump(str("functions"), r.functions.value_or(0));
        bump(str("bytes"), r.bytes.value_or(0));
        bump(str("identical"), r.identical.value_or(0));
        bump(str("identical_code"), r.identical_code.value_or(0));
        bump(str("changed"), r.changed.value_or(0));
        bump(str("changed_code"), r.changed_code.value_or(0));
        bump(str("added"), r.added.value_or(0));
        bump(str("removed"), r.removed.value_or(0));
    }
    for (const ModuleRow& r : rows) {
        if (r.status == str("failed")) {
            log(py::percent("  %s FAILED %s", r.name.ljust(14), r.error ? *r.error : str("")));
        } else {
            log(py::percent("  %s %s %5d functions: identical %5d  changed %5d  added %5d  removed %5d", r.name.ljust(14), r.status.ljust(10),
                            r.functions.value_or(0), r.identical.value_or(0), r.changed.value_or(0), r.added.value_or(0), r.removed.value_or(0)));
        }
        out.modules.append(r);
    }
    return out;
}

// ---- the command
const char* DOC =
    "python -m codebreaker versions <module> --from CORPUS --to CORPUS [--json FILE] [--carry]\n"
    "python -m codebreaker versions <fileA> <fileB> [--json FILE] [--carry]\n"
    "python -m codebreaker versions --all --from CORPUS --to CORPUS [--sample N] [--jobs N] [--json FILE]\n"
    "python -m codebreaker versions --shared <program> <program> ... [--level text|shape|bytes] [--json FILE]\n"
    "\n"
    "Match the functions of two builds of a module and say which are identical, changed, added\n"
    "and removed; with --all, do it for every module of a product and total it; with --shared,\n"
    "list the functions several different programs have in common.";

namespace {

std::optional<str> take_option(py::list<str>& argv, const char* name, const std::optional<str>& dflt = std::nullopt) {
    for (int64_t i = 0; i < argv.size(); i++) {
        if (argv[i] == str(name)) {
            str value = argv[i + 1];                       // (IndexError when there is none, as in the original)
            argv.set_slice(i, i + 2, py::list<str>());
            return value;
        }
    }
    return dflt;
}

void save(const std::optional<str>& out, const Value& data) {
    if (!out) return;
    py::file f = py::open(*out, str("w"), str("utf-8"), std::nullopt, str("\n"));
    f.write(formats::inspect::dumps(data));
    f.close();
    py::print(py::percent("wrote %s", *out));
}

}  // namespace

int64_t main(const py::list<str>& argv_in) {
    py::list<str> argv = argv_in.copy();
    std::optional<str> old = take_option(argv, "--from"), new_ = take_option(argv, "--to");
    std::optional<str> out = take_option(argv, "--json");
    double least = py::float_from_str(*take_option(argv, "--least", str("0.6")));
    int64_t sample = py::int_parse(*take_option(argv, "--sample", str("0")));
    int64_t jobs = py::int_parse(*take_option(argv, "--jobs", str("0")));
    str level = *take_option(argv, "--level", str("text"));
    py::set<str> flags;
    py::list<str> names;
    for (const str& a : argv) {
        if (a.startswith(str("--"))) flags.add(a);
        else names.append(a);
    }

    if (flags.contains(str("--shared"))) {
        if (names.size() < 2) throw py::SystemExit("--shared needs two programs or more");
        if (!LEVELS.contains(level)) throw py::SystemExit(str("--level is one of ") + py::join(str(", "), LEVELS));
        py::list<str> paths;
        for (const str& n : names) {
            std::optional<str> r = base::corpus::retail(n);
            paths.append(r && !r->empty() ? *r : n);
        }
        auto found = shared(paths, level);
        py::list<str> stems;
        for (const str& p : paths) stems.append(py::os::path::basename(p).lower());
        py::print(py::percent("%s: %d functions in two programs or more (%s)", py::join(str(", "), stems), static_cast<int64_t>(found.size()), level));
        for (int64_t i = 0; i < stems.size(); i++) {
            for (int64_t j = i + 1; j < stems.size(); j++) {
                int64_t both = 0;
                for (const auto& nw : found) {
                    bool hi = false, hj = false;
                    for (const auto& w : nw.second) {
                        if (w.first == stems[i]) hi = true;
                        if (w.first == stems[j]) hj = true;
                    }
                    if (hi && hj) both++;
                }
                py::print(py::percent("  %s and %s: %d", stems[i], stems[j], both));
            }
        }
        if (!found.empty()) py::print(str("largest (instructions: program@keys):"));
        for (size_t k = 0; k < found.size() && k < 12; k++) {
            Where sorted_where = found[k].second;
            std::stable_sort(sorted_where.begin(), sorted_where.end(), [](const auto& a, const auto& b) { return a.first.u32() < b.first.u32(); });
            py::list<str> parts;
            for (const auto& w : sorted_where) parts.append(py::percent("%s@%s", w.first, py::join(str(","), w.second)));
            py::print(py::percent("  %4d  %s", found[k].first, py::join(str("  "), parts)));
        }
        if (out) {
            Value d = Value::dict();
            d.set("level", Value(level));
            d.set("programs", Value::from_strings(stems));
            Value sh = Value::list();
            for (const auto& nw : found) {
                Value e = Value::dict();
                e.set("instructions", Value(nw.first));
                Value where = Value::dict();
                for (const auto& w : nw.second) where.set(w.first, Value::from_strings(w.second));
                e.set("where", std::move(where));
                sh.push(std::move(e));
            }
            d.set("shared", std::move(sh));
            save(out, d);
        }
        return 0;
    }
    if (flags.contains(str("--all")) && old && new_) {
        double t = clock()();
        Product result = product(*old, *new_, sample, jobs ? std::optional<int64_t>(jobs) : std::nullopt, least);
        auto total = [&](const char* k) {
            for (const auto& e : result.totals)
                if (e.first == str(k)) return e.second;
            return int64_t(0);
        };
        double seconds = round_to(clock()() - t, 1);
        int64_t code = total("bytes") ? total("bytes") : 1;
        py::print(py::percent(
            "%s -> %s: %d modules (%d compared, %d the same file, %d new, %d failed)\n  %d functions: identical %d, changed %d, added %d; removed from the old "
            "ones %d\n  of %d bytes of code: %.1f%% identical to %s, %.1f%% changed   (%.0fs)",
            *old, *new_, total("modules"), total("compared"), total("same file"), total("new module"), total("failed"), total("functions"), total("identical"),
            total("changed"), total("added"), total("removed"), total("bytes"), 100.0 * static_cast<double>(total("identical_code")) / static_cast<double>(code), *old,
            100.0 * static_cast<double>(total("changed_code")) / static_cast<double>(code), seconds));
        if (out) {
            Value d = to_value(result);
            d.get("totals")->set("seconds", Value(seconds));
            save(out, d);
        }
        return total("failed") ? 1 : 0;
    }
    str path_a, path_b;
    if (names.size() == 2 && py::os::path::isfile(names[0]) && py::os::path::isfile(names[1])) {
        path_a = names[0];
        path_b = names[1];
    } else if (names.size() == 1 && old && new_) {
        std::optional<str> pa, pb;
        {
            base::corpus::selected scope(*old);
            pa = base::corpus::retail(names[0]);
        }
        {
            base::corpus::selected scope(*new_);
            pb = base::corpus::retail(names[0]);
        }
        if (!pa || pa->empty() || !pb || pb->empty()) throw py::SystemExit(py::percent("no %s in %s", names[0], !pa || pa->empty() ? *old : *new_));
        path_a = *pa;
        path_b = *pb;
    } else {
        py::print(str(DOC));
        return 2;
    }
    Comparison c = compare(path_a, path_b, least);
    py::print(c.report());
    auto carried = c.names();
    auto known = c.reconstructions();
    int64_t onto = 0;
    for (const auto& n : carried) onto += n.identical;
    py::print(py::percent("  names carried to the new build: %d (%d onto identical bodies)   verified reconstructions that carry: %d",
                          static_cast<int64_t>(carried.size()), onto, static_cast<int64_t>(known.size())));
    if (flags.contains(str("--carry"))) {
        py::print(py::percent("  filed %d reconstructions for %s in the store, to be judged", c.carry(), c.b->name));
        auto named = c.carry_names();
        py::print(py::percent("  names given to its bodies in the store: %s",
                              named ? py::to_str(*named) : str("none -- its bodies are not recorded there yet")));
    }
    if (out) save(out, c.as_json());
    return 0;
}

}  // namespace codebreaker::compare::versions
