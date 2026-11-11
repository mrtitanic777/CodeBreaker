// codebreaker/symbols/x86.cpp -- see x86.hpp.

#include "codebreaker/symbols/x86.hpp"
#include "codebreaker/base/capstone_dll.hpp"

#include <windows.h>

#include <algorithm>
#include <cstdlib>
#include <map>
#include <mutex>
#include <set>

#include <capstone/capstone.h>

#include "codebreaker/base/corpus.hpp"

namespace codebreaker::symbols::x86 {

namespace {

struct CsLib {
    HMODULE module = nullptr;
    decltype(&cs_open) open = nullptr;
    decltype(&cs_option) option = nullptr;
    decltype(&cs_disasm) disasm = nullptr;
    decltype(&cs_free) free_ = nullptr;
};

std::wstring& dll_path() {
    static std::wstring p;
    return p;
}

template <class F> F sym_(HMODULE m, const char* name) {
    FARPROC p = GetProcAddress(m, name);
    if (!p) throw py::ImportError(std::string("capstone.dll has no ") + name);
    return reinterpret_cast<F>(p);
}

CsLib& lib_ready() {
    static CsLib lib;
    static std::mutex m;
    if (lib.module) return lib;
    std::lock_guard<std::mutex> hold(m);
    if (lib.module) return lib;
    if (dll_path().empty()) set_capstone_dll(base::capstone_dll::find());   // (nobody named one: found as `import capstone` would)
    HMODULE h = LoadLibraryW(dll_path().c_str());
    if (!h) throw py::ImportError("capstone: cannot load the DLL");
    lib.open = sym_<decltype(&cs_open)>(h, "cs_open");
    lib.option = sym_<decltype(&cs_option)>(h, "cs_option");
    lib.disasm = sym_<decltype(&cs_disasm)>(h, "cs_disasm");
    lib.free_ = sym_<decltype(&cs_free)>(h, "cs_free");
    lib.module = h;
    return lib;
}

// one handle per thread, bits and detail (platforms._DECODERS)
csh handle_(int bits, bool detail) {
    CsLib& lib = lib_ready();
    thread_local std::map<std::pair<int, bool>, csh> held;
    auto it = held.find({bits, detail});
    if (it != held.end()) return it->second;
    csh h = 0;
    if (lib.open(CS_ARCH_X86, bits == 16 ? CS_MODE_16 : CS_MODE_32, &h) != CS_ERR_OK) throw py::RuntimeError("capstone: cs_open failed");
    lib.option(h, CS_OPT_DETAIL, detail ? CS_OPT_ON : CS_OPT_OFF);
    held[{bits, detail}] = h;
    return h;
}

Insn make_(const cs_insn& c, bool detail) {
    Insn out;
    out.address = static_cast<int64_t>(c.address);
    out.size = c.size;
    out.mnemonic = c.mnemonic;
    out.op_str = c.op_str;
    if (detail && c.detail) {
        const cs_x86& x = c.detail->x86;
        out.disp = x.disp;
        out.disp_size = x.encoding.disp_size;
        out.disp_offset = x.encoding.disp_offset;
        out.imm_size = x.encoding.imm_size;
        out.imm_offset = x.encoding.imm_offset;
        if (x.op_count > 0 && x.operands[0].type == X86_OP_IMM) {
            out.first_is_imm = true;
            out.first_imm = x.operands[0].imm;
        }
    }
    return out;
}

}  // namespace

void set_capstone_dll(const py::str& path) {
    std::string u = path.utf8();
    int n = MultiByteToWideChar(CP_UTF8, 0, u.data(), static_cast<int>(u.size()), nullptr, 0);
    std::wstring w(static_cast<size_t>(n), L'\0');
    MultiByteToWideChar(CP_UTF8, 0, u.data(), static_cast<int>(u.size()), &w[0], n);
    dll_path() = w;
}

std::vector<Insn> disasm(int bits, bool detail, const std::string& code, int64_t address, size_t count) {
    CsLib& lib = lib_ready();
    csh h = handle_(bits, detail);
    cs_insn* insn = nullptr;
    size_t n = lib.disasm(h, reinterpret_cast<const uint8_t*>(code.data()), code.size(), static_cast<uint64_t>(address), count, &insn);
    std::vector<Insn> out;
    out.reserve(n);
    for (size_t k = 0; k < n; ++k) out.push_back(make_(insn[k], detail));
    if (n) lib.free_(insn, n);
    return out;
}

std::vector<Lite> disasm_lite(int bits, const std::string& code, int64_t address) {
    std::vector<Lite> out;
    for (const Insn& i : disasm(bits, false, code, address)) out.push_back({i.address, i.size, i.mnemonic, i.op_str});
    return out;
}

std::optional<Lite> first_lite(int bits, const std::string& code, int64_t address) {
    std::vector<Insn> v = disasm(bits, false, code, address, 1);
    if (v.empty()) return std::nullopt;
    return Lite{v[0].address, v[0].size, v[0].mnemonic, v[0].op_str};
}

// ---- the carve (native/formats/win/pe_functions.cc, which formats.pe_functions asks) --------------------------------------
namespace {

constexpr size_t kWindow = 1 << 16;

// the target of `call imm` (the detail decoder's first operand)
bool call_target(const Insn& ins, int64_t& t) {
    t = ins.first_imm;
    return ins.mnemonic == "call" && ins.first_is_imm;
}

}  // namespace

PeCarve::PeCarve(const py::str& path) {
    pe = std::make_shared<formats::pe::PEFile>(base::corpus::resolve(path));
    sections = pe->code_sections();
    py::dict<int64_t, py::str> exports;      // export_va -> name
    for (const auto& e : pe->exports()) {
        int64_t va = static_cast<int64_t>(pe->image_base) + std::get<2>(e);
        const auto& n = std::get<1>(e);
        exports.set(va, (n && !n->empty()) ? *n : py::percent(py::str("Ord%d"), std::get<0>(e)));
    }
    struct Sec {
        int64_t base;
        const std::string* data;
        int64_t end() const { return base + static_cast<int64_t>(data->size()); }
    };
    std::vector<Sec> secs;
    for (const auto& s : sections) secs.push_back({static_cast<int64_t>(std::get<0>(s)), &std::get<1>(s).raw()});
    auto sec_of = [&](int64_t va) -> const Sec* {
        for (const Sec& s : secs)
            if (s.base <= va && va < s.end()) return &s;
        return nullptr;
    };
    std::unordered_set<int64_t> decoded, called;
    // md.disasm(data[off:], base + off), in windows: f(ins) returns false to stop
    auto walk = [&](const Sec& s, int64_t off, const std::function<bool(const Insn&)>& f) {
        int64_t n = static_cast<int64_t>(s.data->size());
        while (off < n) {
            size_t w = static_cast<size_t>(std::min<int64_t>(n - off, kWindow));
            std::vector<Insn> run = disasm(32, true, s.data->substr(static_cast<size_t>(off), w), s.base + off);
            if (run.empty()) return;
            for (const Insn& ins : run)
                if (!f(ins)) return;
            off = run.back().address - s.base + run.back().size;
        }
    };
    // the linear sweep of every code section
    for (const Sec& s : secs) {
        int64_t n = static_cast<int64_t>(s.data->size()), off = 0;
        while (off < n) {
            size_t w = static_cast<size_t>(std::min<int64_t>(n - off, kWindow));
            std::vector<Insn> run = disasm(32, true, s.data->substr(static_cast<size_t>(off), w), s.base + off);
            if (run.empty()) {
                off += 1;
                continue;
            }
            for (const Insn& ins : run) {
                decoded.insert(ins.address);
                int64_t t;
                if (call_target(ins, t)) called.insert(t);
            }
            off = run.back().address - s.base + run.back().size;
        }
    }
    int64_t image_base = static_cast<int64_t>(pe->image_base), entry = pe->entry;
    std::set<int64_t> starts;
    std::unordered_set<int64_t> export_set;
    for (const auto& kv : exports.items()) {
        starts.insert(std::get<0>(kv));
        export_set.insert(std::get<0>(kv));
    }
    if (entry) starts.insert(image_base + entry);
    for (int64_t t : called)
        if (sec_of(t)) starts.insert(t);
    for (const Sec& s : secs) {
        int64_t n = static_cast<int64_t>(s.data->size());
        const uint8_t* d = reinterpret_cast<const uint8_t*>(s.data->data());
        for (int64_t i = 0; i < n - 4; ++i) {
            if (d[i] == 0x55 && d[i + 1] == 0x8b && d[i + 2] == 0xec) starts.insert(s.base + i);
            else if (d[i] == 0x8b && d[i + 1] == 0xff && d[i + 2] == 0x55 && d[i + 3] == 0x8b && d[i + 4] == 0xec) starts.insert(s.base + i);
        }
    }
    std::vector<int64_t> pending;
    for (int64_t s : starts)
        if (!decoded.count(s)) pending.push_back(s);
    for (int64_t s : pending) {
        if (!(export_set.count(s) || called.count(s) || s == image_base + entry)) continue;
        const Sec* sec = sec_of(s);
        if (!sec) continue;
        walk(*sec, s - sec->base, [&](const Insn& ins) {
            if (decoded.count(ins.address) && ins.address != s) return false;
            int64_t t;
            if (call_target(ins, t)) called.insert(t);
            decoded.insert(ins.address);
            return true;
        });
    }
    std::vector<int64_t> order;
    for (int64_t s : starts)
        if (decoded.count(s)) order.push_back(s);
    for (size_t i = 0; i < order.size(); ++i) {
        const Sec* s = sec_of(order[i]);
        int64_t end = s->end();
        if (i + 1 < order.size() && order[i + 1] < end) end = order[i + 1];
        Func f;
        f.start = order[i];
        f.end = end;
        f.name = exports.get(order[i]);
        funcs.set(order[i], f);
    }
    addresses.assign(decoded.begin(), decoded.end());
    std::sort(addresses.begin(), addresses.end());
    is_decoded_ = decoded;
}

std::pair<int64_t, const py::bytes*> PeCarve::sec_of(int64_t va) const {
    for (const auto& s : sections) {
        int64_t base = static_cast<int64_t>(std::get<0>(s));
        if (base <= va && va < base + std::get<1>(s).size()) return {base, &std::get<1>(s)};
    }
    return {-1, nullptr};
}

bool PeCarve::has_insn(int64_t va) const { return is_decoded_.count(va) != 0; }

Insn PeCarve::insn(int64_t va) const {
    auto [base, data] = sec_of(va);
    if (!data || !has_insn(va)) throw py::KeyError(std::to_string(va));
    std::vector<Insn> v = disasm(32, true, data->raw().substr(static_cast<size_t>(va - base), 15), va, 1);
    if (v.empty()) throw py::StopIteration();
    return v[0];
}

std::shared_ptr<PeCarve> carve(const py::str& path) {
    static std::mutex m;
    static std::map<std::string, std::shared_ptr<PeCarve>> kept;
    std::lock_guard<std::mutex> hold(m);
    auto it = kept.find(path.utf8());
    if (it != kept.end()) return it->second;
    auto c = std::make_shared<PeCarve>(path);
    kept[path.utf8()] = c;
    return c;
}

}  // namespace codebreaker::symbols::x86
