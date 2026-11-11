// codebreaker/analyze/image.cpp -- typed C++ from codebreaker.analyze.image (see the header).
//
// A module as the analyses see it: its bytes at their addresses (one address space per module, whatever its
// format), the function starts, the imports, the pointers the file names; a function's instructions come from
// cbanalyze.dll (analyze/native.hpp), the loaders of the three formats are here.
//
// What differs from the Python, and why:
//   * `import capstone` has no C++ counterpart: set_capstone_dll() names the DLL (see the header).
//   * formats.pe_functions.PeCarve has no typed port; the carve it asks formats.dll for (native/formats/win/pe_functions.cc)
//     is run again here over core/cs32.h (the same 32-bit Capstone decoder): the linear sweep, the function starts
//     (exports, the entry point, direct-call targets, standard prologues) and their extents.
//   * The DLL's answers are JSON text; they are read with the l2 JSON reader (py::V).
#include "codebreaker/analyze/image.hpp"

#include <algorithm>
#include <cstdint>
#include <fstream>
#include <set>
#include <sstream>
#include <unordered_set>

#include "../../../../formats/core/cs32.h"
#include "codebreaker/base/corpus.hpp"
#include "codebreaker/decompile/describe.hpp"
#include "codebreaker/formats/detect.hpp"
#include "codebreaker/formats/le.hpp"
#include "codebreaker/formats/ne.hpp"
#include "codebreaker/formats/pe.hpp"
#include "json.hpp"
#include "py_os.hpp"
#include "py_settings.hpp"

namespace codebreaker::analyze::image {

namespace describe = decompile::describe;

namespace {

std::wstring& capstone_wide() {
    static std::wstring p = L"capstone.dll";   // (a bare name finds the DLL once it has been loaded by path)
    return p;
}

std::wstring widen(const py::str& s) {
    std::wstring w;
    for (char32_t c : s.u32()) {
        if (c >= 0x10000) {
            c -= 0x10000;
            w.push_back(static_cast<wchar_t>(0xD800 + (c >> 10)));
            w.push_back(static_cast<wchar_t>(0xDC00 + (c & 0x3FF)));
        } else {
            w.push_back(static_cast<wchar_t>(c));
        }
    }
    return w;
}

// _FAMILY
const std::map<std::string, std::string>& family_table() {
    static const std::map<std::string, std::string> t = [] {
        std::map<std::string, std::string> m;
        const char* rows[][2] = {{"eax", "rax eax ax al ah"}, {"ecx", "rcx ecx cx cl ch"}, {"edx", "rdx edx dx dl dh"},
                                 {"ebx", "rbx ebx bx bl bh"}, {"esp", "rsp esp sp spl"},   {"ebp", "rbp ebp bp bpl"},
                                 {"esi", "rsi esi si sil"},   {"edi", "rdi edi di dil"}};
        for (auto& r : rows) {
            std::istringstream in(r[1]);
            std::string n;
            while (in >> n) m[n] = r[0];
        }
        return m;
    }();
    return t;
}

// _FLOWS: Ins.flow by the DLL's number
std::optional<py::str> flow_of(int64_t n) {
    switch (n) {
        case 1: return py::str("call");
        case 2: return py::str("jmp");
        case 3: return py::str("jcc");
        case 4: return py::str("ret");
        default: return std::nullopt;
    }
}

std::optional<py::str> opt_str(const py::V& v) {
    if (v.is_none()) return std::nullopt;
    return py::str(v.str());
}

Op make_op(const py::V& o) {
    Op op;
    const std::string& kind = o[0].str();
    op.kind = py::str(kind);
    if (kind == "r") {
        op.name = py::str(o[1].str());
        op.size = o[2].as_int();
    } else if (kind == "i") {
        op.value = o[1].as_int();
        op.size = o[2].as_int();
    } else {
        op.base = opt_str(o[1]);
        op.index = opt_str(o[2]);
        op.scale = o[3].as_int();
        op.disp = o[4].as_int();
        op.seg = opt_str(o[5]);
        op.size = o[6].as_int();
    }
    return op;
}

std::optional<int64_t> opt_int(const py::V& v) {
    if (v.is_none()) return std::nullopt;
    return v.as_int();
}

py::str ascii_upper(const py::str& s) { return s.upper(); }

}  // namespace

// ---- Op ---------------------------------------------------------------------------------------------------------------
bool Op::operator==(const Op& o) const {
    return kind == o.kind && name == o.name && value == o.value && base == o.base && index == o.index && scale == o.scale &&
           disp == o.disp && seg == o.seg && size == o.size && target == o.target;
}

// ---- Function ---------------------------------------------------------------------------------------------------------
std::optional<int64_t> Function::at(int64_t addr) const {
    auto got = index.get(addr);
    return got;
}

// ---- Image ------------------------------------------------------------------------------------------------------------
struct Image::Impl {
    std::vector<int64_t> starts;                               // _starts
    std::map<int64_t, FunctionP> fn;                           // _fn
    std::map<int64_t, py::V> rows;                             // decoded functions the DLL handed back, not yet made objects
    std::unique_ptr<native::Handle> native;                    // _native
};

Image::Image(const py::str& path_, const py::str& kind_, int64_t bits_)
    : path(path_), kind(kind_), bits(bits_), impl_(std::make_shared<Impl>()) {
    name = py::os::path::basename(path_).lower();
}
Image::~Image() {}

bool Image::flat() const { return formats::detect::flat(kind); }

void Image::_finish() {
    std::stable_sort(spans.vec().begin(), spans.vec().end(),
                     [](const native::Span& a, const native::Span& b) { return std::get<0>(a) < std::get<0>(b); });
    impl_->starts.clear();
    for (const native::Span& s : spans.vec()) impl_->starts.push_back(std::get<0>(s));
    std::vector<int64_t> f = names.keys();
    std::sort(f.begin(), f.end());
    functions = py::list<int64_t>(f);
}

const native::Span* Image::span(int64_t va) const {
    const auto& st = impl_->starts;
    auto it = std::upper_bound(st.begin(), st.end(), va);
    int64_t i = static_cast<int64_t>(it - st.begin()) - 1;
    if (i >= 0) {
        const native::Span& s = spans.vec()[static_cast<size_t>(i)];
        if (va < std::get<0>(s) + std::get<1>(s).size()) return &s;
    }
    return nullptr;
}

py::bytes Image::read(int64_t va, int64_t n) const {
    const native::Span* s = span(va);
    if (!s) return py::bytes();
    int64_t lo = va - std::get<0>(*s);
    return std::get<1>(*s).slice(lo, lo + n);
}

std::optional<int64_t> Image::u16(int64_t va) const {
    py::bytes b = read(va, 2);
    if (b.size() != 2) return std::nullopt;
    return b.byte_(0) | (b.byte_(1) << 8);
}

std::optional<int64_t> Image::u32(int64_t va) const {
    py::bytes b = read(va, 4);
    if (b.size() != 4) return std::nullopt;
    return b.byte_(0) | (b.byte_(1) << 8) | (b.byte_(2) << 16) | (b.byte_(3) << 24);
}

bool Image::is_code(int64_t va) const {
    const native::Span* s = span(va);
    return s && std::get<2>(*s);
}

bool Image::is_data(int64_t va) const { return span(va) != nullptr && !imports.contains(va); }

bool Image::initialised(int64_t va) const {
    const native::Span* s = span(va);
    return s && va - std::get<0>(*s) < std::get<5>(*s);
}

std::optional<int64_t> Image::owner(int64_t va) const {
    const std::vector<int64_t>& f = functions.vec();
    auto it = std::upper_bound(f.begin(), f.end(), va);
    int64_t i = static_cast<int64_t>(it - f.begin()) - 1;
    if (i >= 0 && va < limits.at(f[static_cast<size_t>(i)])) return f[static_cast<size_t>(i)];
    return std::nullopt;
}

py::str Image::label(int64_t va) const {
    auto n = names.get(va);
    if (n && n->size() != 0) return *n;
    auto m = imports.get(va);
    if (m && m->size() != 0) return *m;
    return py::percent(py::str("%x"), va);
}

native::Image Image::as_native() const {
    native::Image n;
    n.kind = kind;
    n.bits = bits;
    n.flat = flat();
    n.pointers = pointers;
    n.spans = spans;
    n.names = names;
    n.limits = limits;
    n.imports = imports;
    n.data_segment = data_segment;
    n.roots = roots;
    return n;
}

native::Handle& Image::_handle() {
    if (!impl_->native) impl_->native.reset(new native::Handle(as_native()));
    return *impl_->native;
}

namespace {

FunctionP function_of(const Image& img, const py::V& row) {
    auto fn = std::make_shared<Function>();
    fn->start = row[0].as_int();
    fn->limit = row[1].as_int();
    fn->end = row[2].as_int();
    auto nm = img.names.get(fn->start);
    fn->name = nm ? *nm : py::percent(py::str("%x"), fn->start);
    int64_t k = 0;
    for (const py::V& r : row[3].seq()) {
        auto ins = std::make_shared<Ins>();
        ins->addr = r[0].as_int();
        ins->size = r[1].as_int();
        ins->mn = py::str(r[2].str());
        ins->text = py::str(r[3].str());
        for (const py::V& o : r[4].seq()) ins->ops.append(make_op(o));
        for (const py::V& x : r[5].seq()) ins->rd.add(py::str(x.str()));
        for (const py::V& x : r[6].seq()) ins->wr.add(py::str(x.str()));
        ins->flow = flow_of(r[7].as_int());
        ins->target = opt_int(r[8]);
        fn->index.set(ins->addr, k++);
        fn->insns.append(ins);
    }
    for (const py::V& t : row[4].seq()) {
        JumpTable jt;
        jt.site = t.get("site").as_int();
        jt.selector = py::str(t.get("selector").str());
        if (!t.get("bytes").is_none()) {
            py::list<int64_t> bs;
            for (const py::V& b : t.get("bytes").seq()) bs.append(b.as_int());
            jt.bytes = bs;
        }
        for (const py::V& x : t.get("targets").seq()) jt.targets.append(x.as_int());
        jt.default_ = opt_int(t.get("default"));
        jt.low = t.get("low").as_int();
        jt.table = t.get("table").as_int();
        jt.bounded = py::truthy(t.get("bounded"));
        fn->tables.append(jt);
    }
    return fn;
}

py::V parse(const std::optional<py::str>& text) {
    if (!text) return py::V();
    return py::json_parse(text->utf8());
}

}  // namespace

FunctionP Image::function(int64_t start) {
    auto it = impl_->fn.find(start);
    if (it != impl_->fn.end()) return it->second;
    py::V row;
    auto r = impl_->rows.find(start);
    if (r != impl_->rows.end()) {
        row = r->second;
        impl_->rows.erase(r);
    } else {
        row = parse(_handle().ask(py::str("image.function"), start));
    }
    FunctionP fn = function_of(*this, row);
    impl_->fn[start] = fn;
    return fn;
}

py::list<InsP> Image::linear(int64_t va, int64_t n) {
    py::V row = parse(_handle().ask(py::str("image.linear"), (n << 40) | va));
    return function_of(*this, row)->insns;
}

py::list<FunctionP> Image::each() {
    py::list<FunctionP> out;
    std::vector<int64_t> starts = functions.vec();   // (a copy: list(self.functions))
    for (int64_t s : starts) out.append(function(s));
    return out;
}

py::str Image::_auto_name(int64_t va) const {
    if (kind == "pe") return py::percent(py::str("sub_%06X"), va - base);
    if (kind == "ne") {
        auto w = describe::where(va);
        return py::percent(py::str("sub_%d_%04X"), std::get<0>(w), std::get<1>(w));
    }
    int64_t rel = va - describe::LE_BASE;
    int64_t q = rel / describe::LE_STRIDE, r = rel % describe::LE_STRIDE;
    if (r < 0) { r += describe::LE_STRIDE; q -= 1; }   // (Python's floor division and modulo)
    return py::percent(py::str("sub_%d_%05X"), q + 1, r);
}

const py::set<int64_t>& Image::discover() {
    py::V got = parse(_handle().ask(py::str("image.discover")));
    for (const py::V& v : got.get("discovered").seq()) {
        names.set(v.as_int(), _auto_name(v.as_int()));
        discovered.add(v.as_int());
    }
    functions = py::list<int64_t>();
    for (const py::V& fl : got.get("functions").seq()) {
        functions.append(fl[0].as_int());
        limits.set(fl[0].as_int(), fl[1].as_int());
    }
    impl_->fn.clear();
    impl_->rows.clear();
    for (const py::V& row : got.get("decoded").seq()) impl_->rows[row[0].as_int()] = row;
    impl_->native->names(as_native());
    return discovered;
}

InsP Image::covering(int64_t va) {
    auto f = owner(va);
    if (!f) return nullptr;
    FunctionP fn = function(*f);
    int64_t lo = 0, hi = fn->insns.size();
    while (lo < hi) {
        int64_t mid = (lo + hi) / 2;
        if (fn->insns[mid]->addr <= va) lo = mid + 1;
        else hi = mid;
    }
    if (lo && va < fn->insns[lo - 1]->addr + fn->insns[lo - 1]->size) return fn->insns[lo - 1];
    return nullptr;
}

// ---- arguments at a call ----------------------------------------------------------------------------------------------
py::str family(const py::str& reg) {
    auto it = family_table().find(reg.utf8());
    return it == family_table().end() ? reg : py::str(it->second);
}

int64_t signed_(int64_t value, int64_t size) {
    int64_t bits = 8 * size;
    if (bits >= 64) return value;
    return (value >> (bits - 1)) ? value - (int64_t(1) << bits) : value;
}

std::optional<int64_t> _slot(const Ins& ins) {
    if (!ins.ops.empty() && ins.ops[0].kind == "m" && !ins.ops[0].base && !ins.ops[0].index) return ins.ops[0].disp;
    return std::nullopt;
}

std::optional<int64_t> destination(const Image& img, const Ins& ins) {
    if (ins.target) return ins.target;
    auto slot = _slot(ins);
    if (slot && img.imports.contains(*slot)) return slot;
    return std::nullopt;
}

namespace {
bool flow_is(const Ins& i, const char* f) { return i.flow && *i.flow == f; }
}  // namespace

std::optional<Op> definition(const Function& fn, int64_t i, const py::str& reg, int64_t depth) {
    py::str fam = family(reg);
    for (int64_t j = i - 1; j > std::max<int64_t>(-1, i - 1 - depth); j--) {
        const Ins& ins = *fn.insns[j];
        if (flow_is(ins, "jmp") || flow_is(ins, "ret")) return std::nullopt;
        if (flow_is(ins, "call")) {
            if (fam == "eax" || fam == "ecx" || fam == "edx") {
                if (fam == "eax") {
                    Op c;
                    c.kind = py::str("call");
                    c.target = ins.target ? ins.target : _slot(ins);
                    return c;
                }
                return std::nullopt;
            }
            continue;
        }
        if (!ins.wr.contains(fam)) continue;
        const py::list<Op>& ops = ins.ops;
        if (ops.size() == 2 && ops[0].kind == "r" && family(ops[0].name) == fam) {
            if (ins.mn == "mov" || ins.mn == "movzx") {
                if (ops[1].kind == "r") {
                    auto d = definition(fn, j, ops[1].name, depth - (i - j));
                    if (d) return d;
                    return ops[1];
                }
                return ops[1];
            }
            if (ins.mn == "lea" && ops[1].kind == "m") {
                Op l;
                l.kind = py::str("lea");
                l.base = ops[1].base;
                l.index = ops[1].index;
                l.scale = ops[1].scale;
                l.disp = ops[1].disp;
                return l;
            }
            if ((ins.mn == "xor" || ins.mn == "sub") && ops[1].kind == "r" && ops[1].name == ops[0].name) {
                Op z;
                z.kind = py::str("i");
                z.value = 0;
                z.size = ops[0].size;
                return z;
            }
        }
        if (ins.mn == "pop" && j && fn.insns[j - 1]->mn == "push") {   // push K / pop reg
            const Ins& pu = *fn.insns[j - 1];
            if (!pu.ops.empty()) return pu.ops[0];
            return std::nullopt;
        }
        return std::nullopt;
    }
    return std::nullopt;
}

py::list<Op> arguments(const Function& fn, int64_t i, int64_t n) {
    py::list<Op> out;
    for (int64_t j = i - 1; j > std::max<int64_t>(-1, i - 60); j--) {
        if (out.size() >= n) break;
        const Ins& ins = *fn.insns[j];
        if (flow_is(ins, "call") || flow_is(ins, "jmp") || flow_is(ins, "ret") || ins.mn == "pop" || ins.mn == "leave" || ins.mn == "enter" || ins.mn == "pushal" || ins.mn == "popal") break;
        if (ins.mn == "push" && !ins.ops.empty()) {
            Op op = ins.ops[0];
            if (op.kind == "r") {
                auto d = definition(fn, j, op.name);
                if (d) op = *d;
            }
            out.append(op);
        } else if ((ins.mn == "add" || ins.mn == "sub") && !ins.ops.empty() && ins.ops[0].kind == "r" && family(ins.ops[0].name) == "esp") {
            break;
        }
    }
    return out;
}

// ---- loading ----------------------------------------------------------------------------------------------------------
namespace {

// _NE_FILES
std::vector<std::string> ne_candidates(const py::str& key) {
    std::vector<std::string> out;
    std::string k = key.utf8();
    if (k == "KERNEL") { out = {"krnl386.exe", "kernel.exe"}; }
    else if (k == "USER") { out = {"user.exe"}; }
    else if (k == "GDI") { out = {"gdi.exe"}; }
    std::string lo = key.lower().utf8();
    for (const char* e : {".dll", ".exe", ".drv"}) out.push_back(lo + e);
    return out;
}

// {ordinal: name} of the 16-bit module that exports under `module`, from its own name tables.
const py::dict<int64_t, py::str>& ne_export_names(const py::str& module) {
    static std::map<std::string, py::dict<int64_t, py::str>> cache;
    py::str key = module.upper();
    auto it = cache.find(key.utf8());
    if (it != cache.end()) return it->second;
    py::dict<int64_t, py::str> names;
    for (const std::string& cand : ne_candidates(key)) {
        auto p = base::corpus::retail(py::str(cand));
        if (!p) continue;
        auto lane = base::corpus::lane(*p);
        if (!lane || !(*lane == "ne")) continue;
        try {
            formats::ne::NEFile nef(*p);
            py::dict<int64_t, py::str> got;
            auto res = nef.resident_names();
            for (int64_t k = 1; k < res.size(); k++) got.set(std::get<1>(res[k]), std::get<0>(res[k]));
            auto non = nef.nonresident_names();
            for (int64_t k = 1; k < non.size(); k++) got.set(std::get<1>(non[k]), std::get<0>(non[k]));
            names = got;
            break;
        } catch (const py::ValueError&) {
        } catch (const py::IndexError&) {
        } catch (const py::StructError&) {
        }
    }
    return cache.emplace(key.utf8(), names).first->second;
}

// ---- the 32-bit function carve (pe_functions.PeCarve) ----
struct Sec {
    int64_t base;
    std::string data;
    int64_t end() const { return base + static_cast<int64_t>(data.size()); }
};

struct Carve {
    std::map<int64_t, std::tuple<int64_t, std::optional<py::str>>> funcs;   // start -> (end, name), ascending
    py::dict<int64_t, py::str> exports;                                       // export_va -> name
};

constexpr size_t kWindow = 1 << 16;

bool call_target(const cbf::CsIns& ins, int64_t* t) {
    *t = ins.imm;
    return ins.mnemonic == "call" && ins.imm0;
}

Carve pe_carve(const formats::pe::PEFile& pe) {
    Carve out;
    for (const auto& e : pe.exports()) {
        int64_t va = static_cast<int64_t>(pe.image_base) + std::get<2>(e);
        const auto& n = std::get<1>(e);
        out.exports.set(va, (n && n->size() != 0) ? *n : py::percent(py::str("Ord%d"), std::get<0>(e)));
    }
    std::vector<Sec> secs;
    for (const auto& s : pe.code_sections()) secs.push_back({static_cast<int64_t>(std::get<0>(s)), std::get<1>(s).raw()});
    const std::wstring& dll = capstone_wide();
    std::unordered_set<int64_t> decoded, called;
    auto sec_of = [&](int64_t va) -> const Sec* {
        for (const Sec& s : secs) if (s.base <= va && va < s.end()) return &s;
        return nullptr;
    };
    // sweep
    for (const Sec& s : secs) {
        int64_t n = static_cast<int64_t>(s.data.size()), off = 0;
        const uint8_t* p = reinterpret_cast<const uint8_t*>(s.data.data());
        while (off < n) {
            size_t w = static_cast<size_t>(std::min<int64_t>(n - off, kWindow));
            std::vector<cbf::CsIns> run = cbf::cs32_disasm(dll, p + off, w, s.base + off, 0, true);
            if (run.empty()) {
                off += 1;
                continue;
            }
            for (auto& ins : run) {
                decoded.insert(ins.address);
                int64_t t;
                if (call_target(ins, &t)) called.insert(t);
            }
            off = run.back().address - s.base + run.back().size;
        }
    }
    int64_t image_base = static_cast<int64_t>(pe.image_base), entry = pe.entry;
    std::set<int64_t> starts;
    for (int64_t va : out.exports.keys()) starts.insert(va);
    if (entry) starts.insert(image_base + entry);
    for (int64_t t : called) if (sec_of(t)) starts.insert(t);
    for (const Sec& s : secs) {
        int64_t n = static_cast<int64_t>(s.data.size());
        const uint8_t* d = reinterpret_cast<const uint8_t*>(s.data.data());
        for (int64_t i = 0; i < n - 4; ++i) {
            if (d[i] == 0x55 && d[i + 1] == 0x8b && d[i + 2] == 0xec) starts.insert(s.base + i);
            else if (d[i] == 0x8b && d[i + 1] == 0xff && d[i + 2] == 0x55 && d[i + 3] == 0x8b && d[i + 4] == 0xec) starts.insert(s.base + i);
        }
    }
    std::vector<int64_t> pending;
    for (int64_t s : starts) if (!decoded.count(s)) pending.push_back(s);
    auto decode_from = [&](int64_t va) {
        const Sec* s = sec_of(va);
        if (!s) return;
        int64_t off = va - s->base, n = static_cast<int64_t>(s->data.size());
        const uint8_t* p = reinterpret_cast<const uint8_t*>(s->data.data());
        while (off < n) {
            size_t w = static_cast<size_t>(std::min<int64_t>(n - off, kWindow));
            std::vector<cbf::CsIns> run = cbf::cs32_disasm(dll, p + off, w, s->base + off, 0, true);
            if (run.empty()) return;
            for (auto& ins : run) {
                if (decoded.count(ins.address) && ins.address != va) return;
                int64_t t;
                if (call_target(ins, &t)) called.insert(t);
                decoded.insert(ins.address);
            }
            off = run.back().address - s->base + run.back().size;
        }
    };
    for (int64_t s : pending) {
        if (out.exports.contains(s) || called.count(s) || s == image_base + entry) decode_from(s);
    }
    std::vector<int64_t> order;
    for (int64_t s : starts) if (decoded.count(s)) order.push_back(s);
    for (size_t i = 0; i < order.size(); ++i) {
        const Sec* s = sec_of(order[i]);
        int64_t end = s->end();
        if (i + 1 < order.size() && order[i + 1] < end) end = order[i + 1];
        std::optional<py::str> name = out.exports.get(order[i]);
        out.funcs[order[i]] = {end, name};
    }
    return out;
}

ImageP load_pe(const py::str& path) {
    formats::pe::PEFile pe(path);
    if (pe.machine != 0x014C) {
        throw py::ValueError(py::percent(py::str("%s: not an x86 module"), py::os::path::basename(path)).utf8());
    }
    auto img = std::make_shared<Image>(path, py::str("pe"), 32);
    int64_t base = static_cast<int64_t>(pe.image_base);
    img->base = base;
    for (const formats::pe::Section& s : pe.sections.vec()) {
        int64_t size = std::max(s.vsize, s.rawsize);
        int64_t held = s.rawptr ? std::min(s.rawsize, s.vsize ? s.vsize : s.rawsize) : 0;
        py::bytes raw = pe.data.slice(s.rawptr, s.rawptr + held);
        if (size) {
            py::bytes full = raw + py::bytes::zeros(size - raw.size());
            img->spans.append(native::Span(base + s.vaddr, full, (s.chars & 0x20000020) != 0, (s.chars & 0x80000000) != 0, s.name, raw.size()));
        }
    }
    Carve carve = pe_carve(pe);
    for (const auto& kv : carve.funcs) {
        int64_t va = kv.first;
        const auto& name = std::get<1>(kv.second);
        if (name && name->size() != 0 && py::re::fullmatch(py::str("[A-Za-z_?@$][\\w?@$]*"), *name)) {
            img->names.set(va, *name);
        } else {
            img->names.set(va, py::percent(py::str("sub_%06X"), va - base));
        }
        img->limits.set(va, std::get<0>(kv.second));
    }
    img->exports = carve.exports;
    for (const auto& kv : pe.imports().items()) {
        const auto& dn = std::get<1>(kv);
        py::str dll = std::get<0>(py::os::path::splitext(std::get<0>(dn))).upper();
        img->imports.set(static_cast<int64_t>(std::get<0>(kv)), dll + py::str("!") + std::get<1>(dn));
    }
    std::set<int64_t> roots;
    for (int64_t va : carve.exports.keys()) roots.insert(va);
    if (pe.entry) roots.insert(base + pe.entry);
    for (int64_t r : roots) img->roots.append(r);
    py::set<int64_t> relocs = pe.base_relocs();
    img->_finish();
    if (!relocs.empty()) {
        py::dict<int64_t, native::Pointer> ptrs;
        for (int64_t r : relocs) ptrs.set(base + r, native::Pointer(py::str("abs32"), img->u32(base + r)));
        img->pointers = ptrs;
    }
    return img;
}

ImageP load_ne(const py::str& path, const py::str& work) {
    formats::ne::NEFile nef(path);
    auto d = describe::describe16(path, work);
    auto img = std::make_shared<Image>(path, py::str("ne"), 16);
    std::map<int64_t, formats::ne::Segment> segs;
    for (const auto& s : nef.segments().vec()) segs[s.idx] = s;
    for (const auto& kv : d->images.items()) {
        int64_t idx = std::get<0>(kv);
        const py::bytes& data = std::get<1>(kv);
        auto sit = segs.find(idx);
        if (sit == segs.end()) throw py::KeyError(std::to_string(idx));
        const formats::ne::Segment& s = sit->second;
        if (data.size() != 0) {
            img->spans.append(native::Span(describe::address(idx, 0), data, s.kind == "CODE", s.kind != "CODE",
                                           s.kind.lower() + py::percent(py::str("%d"), idx), s.file_off ? s.length : 0));
        }
    }
    std::map<int64_t, std::vector<int64_t>> by_seg;
    std::vector<int64_t> seg_order;
    for (const auto& kv : d->where.items()) {
        int64_t va = std::get<0>(kv);
        const auto& so = std::get<1>(kv);
        if (!by_seg.count(std::get<0>(so))) seg_order.push_back(std::get<0>(so));
        by_seg[std::get<0>(so)].push_back(std::get<1>(so));
        img->names.set(va, d->names.at(va));
    }
    for (int64_t seg : seg_order) {
        std::vector<int64_t>& offs = by_seg[seg];
        std::sort(offs.begin(), offs.end());
        for (size_t k = 0; k < offs.size(); k++) {
            int64_t top = k + 1 < offs.size() ? offs[k + 1] : d->code.get(seg, int64_t(0));
            img->limits.set(describe::address(seg, offs[k]), describe::address(seg, top));
        }
    }
    int64_t k = 0;
    for (const py::str& name0 : d->imports.vec()) {
        py::str name = name0;
        // re.fullmatch(r"(.+)_(\d+)", name): the last underscore, with only digits after it and something before it
        const std::u32string& u = name0.u32();
        size_t us = u.rfind(U'_');
        if (us != std::u32string::npos && us >= 1 && us + 1 < u.size()) {
            bool digits = true;
            for (size_t q = us + 1; q < u.size(); q++) if (!(u[q] >= U'0' && u[q] <= U'9')) digits = false;
            if (digits) {
                py::str mod(u.substr(0, us)), num(u.substr(us + 1));
                auto real = ne_export_names(mod).get(py::int_parse(num, 10));
                name = mod.upper() + py::str("!") + (real ? *real : py::str("#") + num);
            }
        }
        img->imports.set((describe::EXTERN_SELECTOR << 16) + 16 * k, name);
        k++;
    }
    std::set<int64_t> roots;
    auto entries = nef.entries();
    for (const auto& e : entries.vec()) {
        if (e.kind == "FIXED" || e.kind == "MOVEABLE") {
            int64_t va = describe::address(*e.seg, *e.offset);
            auto nm = img->names.get(va);
            if (nm) {
                roots.insert(va);
                if (!nm->startswith(py::str("sub_"))) img->exports.set(va, *nm);
            }
        }
    }
    int64_t cs = nef.h.csip >> 16, ip = nef.h.csip & 0xFFFF;
    if (cs && img->names.contains(describe::address(cs, ip))) roots.insert(describe::address(cs, ip));
    for (int64_t r : roots) img->roots.append(r);
    img->data_segment = d->data_segment ? describe::address(d->data_segment, 0) : 0;
    // where the loader writes an address: every place a relocation record (and its chain) names
    py::dict<int64_t, native::Pointer> ptrs;
    std::map<int64_t, formats::ne::Entry> by_ord;
    for (const auto& e : entries.vec()) if (e.kind == "FIXED" || e.kind == "MOVEABLE") by_ord[e.ordinal] = e;
    py::list<py::str> refs = nef.module_refs();
    std::map<std::string, int64_t> extern_;
    {
        int64_t q = 0;
        for (const py::str& n : d->imports.vec()) extern_[n.utf8()] = q++;
    }
    for (const auto& s : nef.segments().vec()) {
        for (const auto& r : nef.relocations(s).vec()) {
            std::optional<int64_t> target;
            if (r.kind == "INTERNAL") {
                std::optional<std::tuple<int64_t, int64_t>> e;
                if (*r.seg == 0xFF) {
                    auto it = by_ord.find(*r.entry);
                    if (it != by_ord.end()) e = std::make_tuple(*it->second.seg, *it->second.offset);
                } else {
                    e = std::make_tuple(*r.seg, *r.entry);
                }
                if (e) target = describe::address(std::get<0>(*e), std::get<1>(*e));
            } else if (r.kind == "IMPORTORD" || r.kind == "IMPORTNAME") {
                py::str mod = (0 < *r.module && *r.module <= refs.size()) ? refs[*r.module - 1] : py::str("?");
                py::str nm = r.kind == "IMPORTORD" ? py::percent(py::str("%s_%d"), mod, *r.ordinal) : nef.imported_name(*r.name_off);
                auto it = extern_.find(py::re::sub(py::str("\\W"), py::str("_"), nm).utf8());
                if (it != extern_.end()) target = (describe::EXTERN_SELECTOR << 16) + 16 * it->second;
            } else {
                continue;
            }
            int64_t off = r.offset, hops = 0;
            while (off != 0xFFFF && off + 1 < s.length && hops < 0x10000) {
                const char* kind = r.src == 3 ? "far" : r.src == 2 ? "selector" : r.src == 5 ? "offset" : "byte";
                ptrs.set(describe::address(s.idx, off), native::Pointer(py::str(kind), target));
                if (r.additive) break;
                off = nef.u16(s.file_off + off);
                hops += 1;
            }
        }
    }
    img->pointers = ptrs;
    img->_finish();
    return img;
}

std::vector<std::string> split_tabs(const std::string& line) {
    std::vector<std::string> out;
    std::string cur;
    for (char c : line) {
        if (c == '\t') { out.push_back(cur); cur.clear(); }
        else cur += c;
    }
    out.push_back(cur);
    return out;
}

int64_t hex_int(const std::string& s) { return py::int_parse(py::str(s), 16); }

ImageP load_le(const py::str& path, const py::str& member, const py::str& work) {
    auto found = formats::le::open_vxds(path);
    if (member.size() != 0) {
        py::list<std::tuple<py::str, formats::le::LEFile>> keep;
        for (const auto& nf : found.vec()) if (std::get<0>(nf).lower() == member.lower()) keep.append(nf);
        found = keep;
    }
    if (found.empty()) {
        throw py::ValueError(py::percent(py::str("%s: no driver image%s"), py::os::path::basename(path),
                                         member.size() != 0 ? py::str(" named ") + member : py::str()).utf8());
    }
    py::str name = std::get<0>(found[0]);
    const formats::le::LEFile& lf = std::get<1>(found[0]);
    auto d = describe::describe_le(lf, name, work);
    auto img = std::make_shared<Image>(path, py::str("le"), 32);
    if (member.size() != 0) img->name = py::percent(py::str("%s#%s"), img->name, member.lower());
    std::set<int64_t> executable, writable;
    for (const auto& kv : d->spans.items()) executable.insert(std::get<0>(std::get<1>(kv)));
    for (const auto& o : lf.objects.vec()) if (o.write) writable.insert(describe::le_address(o.idx, 0));
    {
        std::ifstream f(d->path.utf8(), std::ios::binary);
        std::string line;
        while (std::getline(f, line)) {
            while (!line.empty() && (line.back() == '\n' || line.back() == '\r')) line.pop_back();
            std::vector<std::string> p = split_tabs(line);
            if (p[0] == "bytes") {
                int64_t va = hex_int(p[1]), off = hex_int(p[3]), size = hex_int(p[4]);
                std::ifstream blob(p[2], std::ios::binary);
                blob.seekg(off);
                std::string data(static_cast<size_t>(size), '\0');
                blob.read(&data[0], static_cast<std::streamsize>(size));
                data.resize(static_cast<size_t>(blob.gcount()));
                bool ex = executable.count(va) != 0;
                img->spans.append(native::Span(va, py::bytes(data), ex, writable.count(va) != 0 || !ex,
                                               py::percent(py::str("object%d"), (va - describe::LE_BASE) / describe::LE_STRIDE + 1),
                                               static_cast<int64_t>(data.size())));
            } else if (p[0] == "F") {
                img->names.set(hex_int(p[1]), py::str(p[2]));
            } else if (p[0] == "I") {
                bool vmm = p[2].compare(0, 4, "VMM_") == 0;
                img->imports.set(hex_int(p[1]), py::str(vmm ? "VMM" : "VXD") + py::str("!") + py::str(p[2]));
            }
        }
    }
    img->_finish();
    // what the device descriptor block names: the control procedure, the API procedures and the service table are entered
    // from outside and are function starts
    std::optional<formats::le::Ddb> ddb;
    try {
        ddb = lf.ddb();
    } catch (const py::IndexError&) {
    } catch (const py::StructError&) {
    }
    img->device = ddb;
    try {
        py::dict<int64_t, native::Pointer> ptrs;
        for (const auto& kv : lf.fixups().items()) {
            const auto& place = std::get<0>(kv);
            const auto* fx = std::get_if<formats::le::InternalFixup>(&std::get<1>(kv));
            if (fx && fx->srctype == 7) {
                int64_t at = describe::le_address(std::get<0>(place), std::get<1>(place));
                ptrs.set(at, native::Pointer(py::str("abs32"), img->u32(at)));
            }
        }
        img->pointers = ptrs;
    } catch (const py::IndexError&) {
        img->pointers = std::nullopt;
    } catch (const py::StructError&) {
        img->pointers = std::nullopt;
    }
    if (ddb) {
        int64_t at = describe::le_address(ddb->obj, ddb->rva);
        std::vector<std::optional<int64_t>> entries;
        for (int64_t o : {0x18, 0x1C, 0x20}) entries.push_back(img->u32(at + o));
        auto table = img->u32(at + 0x30), count = img->u32(at + 0x34);
        if (table && *table && count && *count && *count < 0x400 && img->span(*table)) {
            for (int64_t k = 0; k < *count; k++) entries.push_back(img->u32(*table + 4 * k));
        }
        for (const auto& va : entries) {
            if (va && *va && img->is_code(*va)) {
                if (!img->names.contains(*va)) {
                    int64_t rel = *va - describe::LE_BASE;
                    img->names.set(*va, py::percent(py::str("sub_%d_%05X"), rel / describe::LE_STRIDE + 1, rel % describe::LE_STRIDE));
                }
                img->roots.append(*va);
            }
        }
    }
    {
        std::vector<int64_t> f = img->names.keys();
        std::sort(f.begin(), f.end());
        img->functions = py::list<int64_t>(f);
    }
    for (size_t k = 0; k < img->functions.vec().size(); k++) {
        int64_t va = img->functions.vec()[k];
        const native::Span* s = img->span(va);
        std::optional<int64_t> nxt;
        if (k + 1 < img->functions.vec().size()) nxt = img->functions.vec()[k + 1];
        int64_t end = s ? std::get<0>(*s) + std::get<1>(*s).size() : va;
        img->limits.set(va, (nxt && *nxt < end) ? *nxt : end);
    }
    {
        std::set<int64_t> r(img->roots.vec().begin(), img->roots.vec().end());
        img->roots = py::list<int64_t>(std::vector<int64_t>(r.begin(), r.end()));
    }
    return img;
}

}  // namespace

void set_capstone_dll(const py::str& path) {
    capstone_wide() = widen(path);
    native::set_capstone_dll(path);
    describe::set_capstone_dll(path);
}

std::optional<py::str> kind_of(const py::str& path) {
    auto f = formats::detect::format_of(path);
    if (f && *f == "W3") return py::str("le");
    return base::corpus::lane(path);
}

ImageP load(const py::str& module, const std::optional<py::str>& work_in) {
    auto [name, sep, member] = module.partition(py::str("#"));
    (void)sep;
    py::str path = base::corpus::resolve(name);
    auto kind = kind_of(path);
    if (!kind || !(*kind == "pe" || *kind == "ne" || *kind == "le")) {
        throw py::ValueError(py::percent(py::str("%s: not a 32-bit module, a 16-bit module or a driver"), py::os::path::basename(path)).utf8());
    }
    py::str work;
    if (work_in && work_in->size() != 0) {
        work = *work_in;
    } else {
        py::str leaf = py::os::path::basename(path).lower() + (member.size() != 0 ? py::str("-") + member : py::str());
        work = py::os::path::join(py::os::path::join(py::settings::get_str(py::str("state")), py::str("analyze"), py::str("work")), leaf);
    }
    py::os::makedirs(work, 0777, true);
    ImageP img = *kind == "pe" ? load_pe(path) : *kind == "ne" ? load_ne(path, work) : load_le(path, member, work);
    img->discover();
    return img;
}

}  // namespace codebreaker::analyze::image

size_t std::hash<codebreaker::analyze::image::Op>::operator()(const codebreaker::analyze::image::Op& o) const {
    size_t h = std::hash<std::string>()(o.kind.utf8());
    auto mix = [&](size_t x) { h ^= x + 0x9e3779b97f4a7c15ull + (h << 6) + (h >> 2); };
    mix(std::hash<std::string>()(o.name.utf8()));
    mix(std::hash<int64_t>()(o.value));
    mix(std::hash<std::string>()(o.base ? o.base->utf8() : std::string("\1")));
    mix(std::hash<std::string>()(o.index ? o.index->utf8() : std::string("\1")));
    mix(std::hash<int64_t>()(o.scale));
    mix(std::hash<int64_t>()(o.disp));
    mix(std::hash<std::string>()(o.seg ? o.seg->utf8() : std::string("\1")));
    mix(std::hash<int64_t>()(o.size));
    mix(std::hash<int64_t>()(o.target ? *o.target : -1));
    return h;
}

