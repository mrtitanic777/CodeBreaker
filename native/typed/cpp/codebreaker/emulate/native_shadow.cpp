// codebreaker/emulate/native_shadow.cpp -- C++ port of codebreaker.emulate.native_shadow (see the header).
//
// The class is a thin binding to cbtrace.dll's shadow (native/cb/emulate/tracer/shadow.cpp): it hands the DLL the lifter's
// register file and the run's inputs, forwards enter/step/after_call/written/forget/segbase, and reads the node
// table of the run's path, trail, vars and lost back as the same values (the conditions as symbolic's expressions).

#include "codebreaker/emulate/native_shadow.hpp"

#include <windows.h>

#include <array>
#include <cstring>
#include <map>
#include <mutex>
#include <string>
#include <vector>

#include "codebreaker/emulate/native_trace.hpp"
#include "codebreaker/emulate/unicorn_uc.hpp"
#include "codebreaker/emulate/windows16.hpp"

namespace codebreaker::emulate::native_shadow {
namespace {

// ---- native/cb/emulate/tracer/cbtrace.h: cbt_sh_config and cbt_sh_result, field for field --------------------
using LiftFn = int (*)(const uint8_t* code, int32_t n, uint64_t offset, char* out, int32_t cap);
struct ShConfig {
    int32_t kind;
    const char* mode;
    int32_t nregs;
    const char* const* reg_names;
    const int64_t *reg_off, *reg_size;
    const int32_t *reg_uc, *reg_flag;
    int32_t eflags_id;
    const uint64_t* inputs;
    int64_t ninputs;
    uint64_t gdt_addr;
    LiftFn lift;
    int32_t real_mode;
};
struct ShResult {
    const uint64_t* nodes;
    int64_t nnodes;
    const uint64_t* path;
    int64_t npath;
    const uint64_t* trail;
    int64_t ntrail;
    const uint64_t* vars;
    int64_t nvars;
    const uint64_t* lost;
    int64_t nlost;
    const char* strings;
    int64_t strings_len;
    int64_t nstrings;
    int64_t pins, steps, off;
};

// The DLL's shadow entry points (the ctypes lib's cbt_sh_*).
struct ShLib {
    void* (*sh_new)(const ShConfig*) = nullptr;
    void (*sh_free)(void*) = nullptr;
    int (*sh_enter)(void*, const char*, int32_t, int64_t, const char*, uint64_t, int32_t, int32_t, uint64_t, int64_t) = nullptr;
    void (*sh_step)(void*, void*, uint64_t, uint64_t) = nullptr;
    void (*sh_after_call)(void*, void*, int32_t, uint64_t, int32_t, uint64_t, int64_t, const char*, int32_t, uint64_t) = nullptr;
    void (*sh_written)(void*, uint64_t, uint64_t) = nullptr;
    void (*sh_forget)(void*, const char*) = nullptr;
    uint64_t (*sh_segbase)(void*, void*, uint64_t) = nullptr;
    const ShResult* (*sh_export)(void*) = nullptr;
};

constexpr uint64_t _NONE = 0xFFFFFFFFFFFFFFFFULL;

const std::map<std::string, int> FLAGS = {{"CF", 0}, {"PF", 2}, {"AF", 4}, {"ZF", 6}, {"SF", 7},
                                          {"TF", 8}, {"IF", 9}, {"DF", 10}, {"OF", 11}};
const char* const OPS[] = {"var",   "const", "add",    "sub",   "and",  "or",    "xor",   "mul",   "udiv",
                           "urem",  "sdiv",  "srem",   "neg",   "not",  "shl",   "lshr",  "ashr",  "zext",
                           "sext",  "extract", "concat", "ite", "popcount", "bool", "eq", "ult",  "ule",
                           "slt",   "sle",   "bnot",   "band",  "bor",  "bxor",  "nz",    "true"};
const char* const KINDS[] = {"jump", "inner", "pin"};
const char* const VKINDS[] = {"arg", "reg", "mem", "ret", "carry"};

std::wstring wide(const py::str& s) {
    std::wstring w;
    for (char32_t c : s.u32()) {
        if (c >= 0x10000) {
            uint32_t v = static_cast<uint32_t>(c) - 0x10000;
            w.push_back(static_cast<wchar_t>(0xD800 + (v >> 10)));
            w.push_back(static_cast<wchar_t>(0xDC00 + (v & 0x3FF)));
        } else {
            w.push_back(static_cast<wchar_t>(c));
        }
    }
    return w;
}

// native_shadow._lib(): the loaded DLL's shadow entry points, or null when there is no DLL (or an old one)
const ShLib* _lib() {
    static std::vector<std::shared_ptr<ShLib>> ready;
    static bool tried = false;
    static std::recursive_mutex init;   // (first use may be on several gate threads at once)
    std::lock_guard<std::recursive_mutex> hold(init);
    if (tried) return ready.empty() ? nullptr : ready[0].get();
    std::shared_ptr<native_trace::Lib> lib = native_trace::_load();
    if (!lib) return nullptr;
    HMODULE dll = LoadLibraryW(wide(py::os::path::abspath(native_trace::dll_path())).c_str());   // (loaded already: the same module)
    tried = true;
    if (!dll) return nullptr;
    auto l = std::make_shared<ShLib>();
    auto bind = [&](auto& slot, const char* name) {
        slot = reinterpret_cast<std::remove_reference_t<decltype(slot)>>(GetProcAddress(dll, name));
        return slot != nullptr;
    };
    if (!bind(l->sh_new, "cbt_sh_new")) return nullptr;   // (hasattr(lib, "cbt_sh_new"))
    bind(l->sh_free, "cbt_sh_free");
    bind(l->sh_enter, "cbt_sh_enter");
    bind(l->sh_step, "cbt_sh_step");
    bind(l->sh_after_call, "cbt_sh_after_call");
    bind(l->sh_written, "cbt_sh_written");
    bind(l->sh_forget, "cbt_sh_forget");
    bind(l->sh_segbase, "cbt_sh_segbase");
    bind(l->sh_export, "cbt_sh_export");
    ready.push_back(l);
    return ready[0].get();
}

// The DLL's LIFT callback carries no context (a ctypes closure carried its Shadow): the lifter's answers depend
// only on its mode, so one trampoline per mode asks the lifter that mode was last given.
constexpr int kSlots = 4;
struct LiftSlot {
    std::string mode;
    std::shared_ptr<pcode::Lifter> lifter;
};
LiftSlot g_slots[kSlots];

int lift_for(int slot, const uint8_t* code, int32_t n, uint64_t offset, char* out, int32_t cap) {
    try {
        std::shared_ptr<pcode::Lifter> lifter = g_slots[slot].lifter;
        py::str line = lifter->line(py::bytes(reinterpret_cast<const char*>(code), static_cast<size_t>(n)),
                                    static_cast<int64_t>(offset));
        const std::string& data = line.utf8();
        if (static_cast<int64_t>(data.size()) < cap) std::memcpy(out, data.c_str(), data.size() + 1);
        return static_cast<int>(data.size());
    } catch (const py::Exception& e) {
        std::string name = e.type();
        if (static_cast<int64_t>(name.size()) > cap - 1) name.resize(static_cast<size_t>(cap - 1));
        std::memcpy(out, name.c_str(), name.size() + 1);
        return -2;
    } catch (...) {
        const char* name = "Exception";
        std::strncpy(out, name, static_cast<size_t>(cap - 1));
        out[cap - 1] = 0;
        return -2;
    }
}
template <int N> int lift_tramp(const uint8_t* code, int32_t n, uint64_t offset, char* out, int32_t cap) {
    return lift_for(N, code, n, offset, out, cap);
}
const LiftFn TRAMPS[kSlots] = {&lift_tramp<0>, &lift_tramp<1>, &lift_tramp<2>, &lift_tramp<3>};

LiftFn lift_callback(const std::shared_ptr<pcode::Lifter>& lifter) {
    std::string mode = lifter->mode.utf8();
    int slot = -1;
    for (int i = 0; i < kSlots; i++)
        if (!g_slots[i].mode.empty() && g_slots[i].mode == mode) slot = i;
    for (int i = 0; slot < 0 && i < kSlots; i++)
        if (g_slots[i].mode.empty()) slot = i;
    if (slot < 0) throw py::RuntimeError("native_shadow: more lift modes than the shadow keeps callbacks for");
    g_slots[slot].mode = mode;
    g_slots[slot].lifter = lifter;
    return TRAMPS[slot];
}

// struct.iter_unpack("<%dQ" % k, ...) over `n` records
std::vector<std::vector<uint64_t>> words(const void* ptr, int64_t n, int64_t k) {
    std::vector<std::vector<uint64_t>> out;
    if (!n) return out;
    const uint64_t* p = static_cast<const uint64_t*>(ptr);
    for (int64_t i = 0; i < n; i++) out.emplace_back(p + i * k, p + (i + 1) * k);
    return out;
}

}  // namespace

// ---- the module's state ---------------------------------------------------------------------------
bool enabled() { return _lib() != nullptr; }

std::optional<py::str> why_not() { return native_trace::why_not(); }

// What the DLL's configuration points at (ctypes kept the arrays on the _Config object).
struct Shadow::Config {
    ShConfig c{};
    std::vector<std::string> names;
    std::vector<const char*> name_ptrs;
    std::vector<int64_t> off, size;
    std::vector<int32_t> uc, flag;
    std::vector<uint64_t> inputs;
    std::string mode;
};

struct Shadow::Export {
    py::list<PathEntry> path;
    py::list<Step> trail;
    Vars vars;
    py::set<py::str> lost;
    int64_t pins = 0, steps = 0;
    bool off = false;
};

void* handle_of(Shadow* shadow) {
    if (shadow == nullptr) return nullptr;
    shadow->got_ = nullptr;   // (the recorder steps it from here on)
    return shadow->handle;
}

static void* uch(Uc& m) { return m.handle(); }

Shadow::Shadow(int64_t kind_, std::shared_ptr<pcode::Lifter> lifter_,
               const py::list<std::tuple<int64_t, int64_t>>& inputs, const std::optional<std::tuple<int64_t, int64_t>>& span_,
               bool real_mode)
    : lifter(lifter_), span(span_), kind(kind_) {
    const ShLib* lib = _lib();
    if (!lib) throw py::AttributeError("'NoneType' object has no attribute 'cbt_sh_new'");
    auto cfg = std::make_shared<Config>();
    std::vector<py::str> names;
    for (const auto& kv : lifter_->regs.items()) names.push_back(std::get<0>(kv));
    size_t n = names.size();
    cfg->mode = lifter_->mode.utf8();
    for (const py::str& nm : names) {
        cfg->names.push_back(nm.utf8());
        auto [off, size] = lifter_->regs.at(nm);
        cfg->off.push_back(off);
        cfg->size.push_back(size);
        auto fl = FLAGS.find(nm.utf8());
        cfg->flag.push_back(fl != FLAGS.end() ? fl->second : -1);
        std::optional<int64_t> id = fl != FLAGS.end() ? std::nullopt : unicorn_uc::x86_reg(nm);
        cfg->uc.push_back(id ? static_cast<int32_t>(*id) : -1);
    }
    for (const std::string& s : cfg->names) cfg->name_ptrs.push_back(s.c_str());
    for (size_t i = 0; i < std::max<size_t>(1, n); i++) {   // (ctypes arrays are at least one long)
        if (i >= n) {
            cfg->name_ptrs.push_back(nullptr);
            cfg->off.push_back(0);
            cfg->size.push_back(0);
            cfg->uc.push_back(0);
            cfg->flag.push_back(0);
        }
    }
    for (const auto& lo_hi : inputs) {
        cfg->inputs.push_back(static_cast<uint64_t>(std::get<0>(lo_hi)));
        cfg->inputs.push_back(static_cast<uint64_t>(std::get<1>(lo_hi)));
    }
    int64_t ninputs = static_cast<int64_t>(cfg->inputs.size()) / 2;
    if (cfg->inputs.empty()) cfg->inputs.push_back(0);
    ShConfig& c = cfg->c;
    c.kind = static_cast<int32_t>(kind_);
    c.mode = cfg->mode.c_str();
    c.nregs = static_cast<int32_t>(n);
    c.reg_names = cfg->name_ptrs.data();
    c.reg_off = cfg->off.data();
    c.reg_size = cfg->size.data();
    c.reg_uc = cfg->uc.data();
    c.reg_flag = cfg->flag.data();
    c.eflags_id = static_cast<int32_t>(unicorn_uc::x86_reg(py::str("EFLAGS")).value());
    c.inputs = cfg->inputs.data();
    c.ninputs = ninputs;
    c.gdt_addr = static_cast<uint64_t>(windows16::GDT_ADDR);
    c.real_mode = real_mode ? 1 : 0;
    c.lift = lift_callback(lifter_);
    config_ = cfg;
    handle = lib->sh_new(&cfg->c);
    got_ = nullptr;
}

Shadow::~Shadow() {
    void* h = handle;
    handle = nullptr;
    if (h) _lib()->sh_free(h);
}

void Shadow::enter(const py::list<Place>& places, int64_t bits) {
    const ShLib* lib = _lib();
    got_ = nullptr;
    int64_t position = 0;
    for (const Place& item : places) {
        int64_t n = position++;
        std::variant<int64_t, py::str> k = item.k ? *item.k : std::variant<int64_t, py::str>(n);
        std::string name;
        int32_t vkind;
        int64_t wi;
        std::optional<std::string> ws;
        if (std::holds_alternative<int64_t>(k)) {
            wi = std::get<int64_t>(k);
            name = "a" + std::to_string(wi);
            vkind = 0;
        } else {
            name = std::get<py::str>(k).utf8();
            vkind = 1;
            wi = 0;
            ws = name;
        }
        const char* wsp = ws ? ws->c_str() : nullptr;
        uint64_t value = static_cast<uint64_t>(item.value);
        if (std::holds_alternative<py::str>(item.where)) {
            auto [off, size] = lifter->regs.at(std::get<py::str>(item.where));
            lib->sh_enter(handle, name.c_str(), vkind, wi, wsp, value, static_cast<int32_t>(bits), 1,
                          static_cast<uint64_t>(off), size);
        } else {
            lib->sh_enter(handle, name.c_str(), vkind, wi, wsp, value, static_cast<int32_t>(bits), 0,
                          static_cast<uint64_t>(std::get<int64_t>(item.where)), 0);
        }
    }
}

void Shadow::step(Uc& m, int64_t address, int64_t cs_base) {
    got_ = nullptr;
    _lib()->sh_step(handle, uch(m), static_cast<uint64_t>(address), static_cast<uint64_t>(cs_base));
}

void Shadow::after_call(Uc& m, bool symbolic_result, int64_t value, const std::optional<int64_t>& args_at,
                        int64_t nargs, const std::optional<py::str>& why, int64_t word, int64_t carry) {
    got_ = nullptr;
    std::string w = why ? why->utf8() : std::string();
    _lib()->sh_after_call(handle, uch(m), symbolic_result ? 1 : 0, static_cast<uint64_t>(value),
                          args_at ? 1 : 0, static_cast<uint64_t>(args_at ? *args_at : 0), nargs,
                          !w.empty() ? w.c_str() : nullptr, static_cast<int32_t>(word), static_cast<uint64_t>(carry));
}

void Shadow::written(int64_t at, int64_t size) {
    got_ = nullptr;
    _lib()->sh_written(handle, static_cast<uint64_t>(at), static_cast<uint64_t>(size));
}

void Shadow::forget(const py::list<py::str>& names) {
    got_ = nullptr;
    for (const py::str& n : names) _lib()->sh_forget(handle, n.utf8().c_str());
}

int64_t Shadow::segbase(Uc& m, int64_t sel) {
    return static_cast<int64_t>(_lib()->sh_segbase(handle, uch(m), static_cast<uint64_t>(sel)));
}

// ---- what the run made ----------------------------------------------------------------------------
const Shadow::Export& Shadow::_export() {
    if (got_) return *got_;
    const ShResult& r = *_lib()->sh_export(handle);
    std::vector<std::string> strings;
    if (r.strings_len) {   // .decode().split("\0")[:nstrings]
        std::string all(r.strings, static_cast<size_t>(r.strings_len));
        size_t pos = 0;
        while (true) {
            size_t z = all.find('\0', pos);
            strings.push_back(all.substr(pos, z == std::string::npos ? std::string::npos : z - pos));
            if (z == std::string::npos) break;
            pos = z + 1;
        }
        if (static_cast<int64_t>(strings.size()) > r.nstrings) strings.resize(static_cast<size_t>(r.nstrings));
    }
    auto str_at = [&](uint64_t i) -> py::str {
        if (i >= strings.size()) throw py::IndexError("list index out of range");
        return py::str(strings[i]);
    };
    std::vector<symbolic::ExprP> nodes;
    for (const auto& w : words(r.nodes, r.nnodes, 8)) {
        uint64_t op = w[0];
        int64_t bits = static_cast<int64_t>(w[1]);   // (>= 2**63 is negative)
        uint64_t k0 = w[2], k1 = w[3], k2 = w[4], lo = w[5], vlo = w[6], vhi = w[7];
        if (op >= sizeof(OPS) / sizeof(OPS[0])) throw py::IndexError("tuple index out of range");
        std::string name = OPS[op];
        auto e = std::make_shared<symbolic::Expr>();
        e->op = py::str(name);
        e->bits = bits;
        auto node = [&](uint64_t k) -> const symbolic::ExprP& {
            if (k >= nodes.size()) throw py::IndexError("list index out of range");
            return nodes[k];
        };
        if (name == "var") {
            e->name = str_at(vlo);
        } else if (name == "const") {
            if (vhi != 0) throw py::OverflowError("a constant wider than 64 bits");
            e->value = static_cast<int64_t>(vlo);   // (a 64-bit constant keeps its bit pattern)
        } else if (name == "extract") {
            e->args.push_back(node(k0));
            e->lo = static_cast<int64_t>(lo);
        } else if (name == "true") {
            e->bits = 0;
        } else {
            for (uint64_t k : {k0, k1, k2})
                if (k != _NONE) e->args.push_back(node(k));
        }
        nodes.push_back(e);
    }
    auto x = std::make_shared<Export>();
    for (const auto& w : words(r.path, r.npath, 4)) {
        if (w[3] >= 3) throw py::IndexError("tuple index out of range");
        if (w[2] >= nodes.size()) throw py::IndexError("list index out of range");
        PathEntry pe;
        pe.site = static_cast<int64_t>(w[0]);
        pe.taken = w[1] != 0;
        pe.cond = nodes[w[2]];
        pe.kind = py::str(KINDS[w[3]]);
        x->path.append(pe);
    }
    for (const auto& w : words(r.trail, r.ntrail, 2)) x->trail.append(Step(static_cast<int64_t>(w[0]), w[1] != 0));
    for (const auto& w : words(r.vars, r.nvars, 7)) {
        if (w[1] >= 5) throw py::IndexError("tuple index out of range");
        Var v;
        v.kind = py::str(VKINDS[w[1]]);
        if (w[2] == 0) v.where = py::none_t{};
        else if (w[2] == 1) v.where = static_cast<int64_t>(w[3]);
        else v.where = str_at(w[3]);
        v.bits = static_cast<int64_t>(w[4]);
        v.value = static_cast<int64_t>(w[5]);
        x->vars.set(str_at(w[0]), v);
    }
    for (const auto& w : words(r.lost, r.nlost, 1)) x->lost.add(str_at(w[0]));
    x->pins = r.pins;
    x->steps = r.steps;
    x->off = r.off != 0;
    got_ = x;
    return *got_;
}

py::list<PathEntry> Shadow::path() { return _export().path; }
py::list<Step> Shadow::trail() { return _export().trail; }
Vars Shadow::vars() { return _export().vars; }
py::set<py::str> Shadow::lost() { return _export().lost; }
int64_t Shadow::pins() { return _export().pins; }
int64_t Shadow::steps() { return _export().steps; }
bool Shadow::off() { return _export().off; }

void require() {
    if (!enabled()) {
        std::optional<py::str> why = why_not();
        throw py::RuntimeError(py::str("the run recorder is native (native/cb/emulate/tracer, cbtrace.dll): ") +
                               (why ? *why : py::str("None")));
    }
}

}  // namespace codebreaker::emulate::native_shadow
