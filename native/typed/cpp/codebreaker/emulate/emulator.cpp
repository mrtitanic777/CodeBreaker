// codebreaker/emulate/emulator.cpp -- C++ port of codebreaker.emulate.emulator (see the header).
//
// Run one 32-bit function in Unicorn and record a structured trace.  The hooks are C++ (native_trace); this
// module lays the machine out, writes the frame, starts each run and reads the result.

#include "codebreaker/emulate/emulator.hpp"

#include <algorithm>
#include <set>
#include <unordered_set>

#include "codebreaker/base/corpus.hpp"
#include "codebreaker/emulate/unicorn_uc.hpp"
#include "codebreaker/formats/pe.hpp"
#include "codebreaker/formats/pe_functions.hpp"
#include "codebreaker/knowledge/package.hpp"
#include "codebreaker/symbols/prototypes.hpp"
#include "py_os.hpp"

namespace codebreaker::emulate::emulator {

native_trace::Gov _GOV;
DefaultStub _DEFAULT_STUB;

const py::list<py::str> _COMPARES = py::list<py::str>{py::str("cmp"), py::str("test"), py::str("sub"), py::str("and"), py::str("or"),
                                                      py::str("dec"), py::str("inc"), py::str("add"), py::str("xor")};
const py::list<py::str> _FLAGLESS = py::list<py::str>{py::str("mov"), py::str("movzx"), py::str("movsx"), py::str("lea"),
                                                      py::str("push"), py::str("pop"), py::str("nop"), py::str("xchg"),
                                                      py::str("not"), py::str("cdq"), py::str("cwde"), py::str("cbw"),
                                                      py::str("bswap")};
namespace {
py::dict<py::str, py::str> make_parent() {
    py::dict<py::str, py::str> parent;
    for (char r : std::string("abcd")) {
        std::string full = std::string("e") + r + "x";
        for (const std::string& n : {std::string(1, r) + "l", std::string(1, r) + "h", std::string(1, r) + "x", full})
            parent.set(py::str(n), py::str(full));
    }
    for (const char* r : {"si", "di", "bp", "sp"}) {
        parent.set(py::str(r), py::str(std::string("e") + r));
        parent.set(py::str(std::string("e") + r), py::str(std::string("e") + r));
    }
    return parent;
}
}  // namespace
const py::dict<py::str, py::str> _PARENT = make_parent();

py::dict<py::str, std::shared_ptr<const Loaded>> _LOADED;
std::map<std::string, ResolvedFunc> _RESOLVED;
py::dict<MachineKey, std::shared_ptr<Machine>> _MACHINES;

namespace {

// ---- loading a module ----------------------------------------------------------------------------------------------
py::dict<py::str, std::shared_ptr<const Loaded>> _PE_CACHE;      // (by the path of the file)
py::dict<py::str, py::list<std::tuple<int64_t, int64_t>>> _CODE_RANGES;   // module file -> [(lo, hi)] of its code sections, as loaded
constexpr int64_t _MAX_CODE_SPAN = 64 << 20;                      // give up joining code sections spread wider than this

py::str _sha256(const py::str& path) {
    py::file fh = py::open_rb(path);
    py::hashlib::Hash h = py::hashlib::sha256();
    while (true) {
        py::bytes chunk = fh.read(1 << 20);
        if (chunk.empty()) break;
        h.update(chunk);
    }
    return h.hexdigest();
}

// (va0, code, secname, funcs) covering EVERY executable section of a PE.
//
// A PE can have several code sections (kernel32: _FREQASM, .text, _INIT). The sections are laid out at their real
// addresses in one buffer starting at the lowest; the gaps between them are zero-filled, which is exactly what an
// unmapped page reads as on demand. Function offsets are relative to that buffer, as callers expect (`va0 + off`).
struct CodeImage {
    int64_t va0 = 0;
    py::bytes code;
    py::str secname;
    py::dict<int64_t, FuncExtent> funcs;
};

CodeImage _pe_code_image(const py::str& module, const py::str& path, const formats::pe::PEFile& pe) {
    auto all = pe.code_sections();   // [(va, bytes, name)]
    formats::pe_functions::PeCarve carve(path);
    if (all.empty()) throw py::ValueError("min() iterable argument is empty");
    std::vector<std::tuple<uint64_t, py::bytes, py::str>> secs = all.vec();
    int64_t lo = INT64_MAX, hi = INT64_MIN;
    for (const auto& s : secs) {
        lo = std::min<int64_t>(lo, static_cast<int64_t>(std::get<0>(s)));
        hi = std::max<int64_t>(hi, static_cast<int64_t>(std::get<0>(s)) + std::get<1>(s).size());
    }
    if (secs.size() > 1 && hi - lo > _MAX_CODE_SPAN) {   // pathological layout: first only
        secs.resize(1);
        lo = static_cast<int64_t>(std::get<0>(secs[0]));
        hi = lo + std::get<1>(secs[0]).size();
    }
    py::list<std::tuple<int64_t, int64_t>> ranges;
    for (const auto& s : secs) {
        int64_t va = static_cast<int64_t>(std::get<0>(s));
        ranges.append(std::make_tuple(va, va + std::get<1>(s).size()));
    }
    _CODE_RANGES.set(path, ranges);
    std::string code(static_cast<size_t>(hi - lo), '\0');
    CodeImage img;
    img.va0 = lo;
    base::corpus::resolve(module);   // (PeSeg(module, ...) asks the corpus for its file)
    std::string names;
    for (size_t i = 0; i < secs.size(); i++) {
        int64_t va = static_cast<int64_t>(std::get<0>(secs[i]));
        const std::string& raw = std::get<1>(secs[i]).raw();
        int64_t base = va - lo;
        std::copy(raw.begin(), raw.end(), code.begin() + base);
        names += (i ? "+" : "") + std::get<2>(secs[i]).utf8();
        // PeSeg(module, section=name, carve=carve): the first of the PE's sections that has the name; the
        // carve's functions that lie inside it, as offsets from its start
        const std::tuple<uint64_t, py::bytes, py::str>* pick = nullptr;
        for (const auto& s : all)
            if (std::get<2>(s) == std::get<2>(secs[i])) {
                pick = &s;
                break;
            }
        int64_t pva = static_cast<int64_t>(std::get<0>(*pick)), phi = pva + std::get<1>(*pick).size();
        for (const auto& kv : carve.funcs.items()) {
            const formats::pe_functions::Func& f = std::get<1>(kv);
            if (pva <= f.start && f.start < phi) {
                int64_t s = f.start - pva;
                int64_t e = std::min(f.end, phi) - pva;
                img.funcs.set(base + s, FuncExtent{base + e});
            }
        }
    }
    img.code = py::bytes(code);
    img.secname = py::str(names);
    return img;
}

}  // namespace

// _load(module): (path, va0, code, secname, funcs, sha, imports) for a module, carved once and cached.
// A "name#member" selector addresses one module inside a W3 bundle (the core VxDs live in vmm32.vxd, not as loose
// files). Resolve the bundle file by its base name; keep the full selector as the cache key.
std::shared_ptr<const Loaded> _load(const py::str& module) {
    if (auto got = _LOADED.get(module)) return got;
    int64_t hash = module.find(py::str("#"));
    py::str base_module = hash >= 0 ? module.slice(0, hash) : module;
    bool is_member = hash >= 0;
    py::str path = base::corpus::resolve(base_module);
    py::str cache_key = is_member ? module : path;
    std::shared_ptr<const Loaded> ce;
    ce = _PE_CACHE.get(cache_key);
    if (!ce) {
        formats::pe::PEFile pe = formats::pe::open_pe(path);
        CodeImage img = _pe_code_image(module, path, pe);
        auto l = std::make_shared<Loaded>();
        try {
            for (const auto& kv : pe.imports().items()) l->imports.set(static_cast<int64_t>(std::get<0>(kv)), std::get<1>(kv));
        } catch (const py::Exception&) {
            l->imports = py::dict<int64_t, std::tuple<py::str, py::str>>();
        }
        l->path = path;
        l->va0 = img.va0;
        l->code = img.code;
        l->secname = img.secname;
        l->funcs = img.funcs;
        l->sha = _sha256(path);
        ce = l;
        _PE_CACHE.set(path, ce);
    }
    _LOADED.set(module, ce);
    return ce;
}

namespace {
// sigrecover.arities(module) lives in recompile95/tools, which this tree does not carry: imported lazily and
// tolerated absent (a corpus/data-shape dependency, not a hard one for tracing) -- nothing is known
py::dict<int64_t, int64_t> _arities(const py::str&) { return py::dict<int64_t, int64_t>(); }

std::string resolved_key(const py::str& module, const FuncSelector& func) {
    std::string k = module.utf8();
    k.push_back('\x01');
    if (std::holds_alternative<py::none_t>(func)) k += "N";
    else if (std::holds_alternative<int64_t>(func)) k += "I" + std::to_string(std::get<int64_t>(func));
    else k += "S" + std::get<py::str>(func).utf8();
    return k;
}

ResolvedFunc _resolve_func_(const py::str& module, const FuncSelector& func) {
    std::shared_ptr<const Loaded> l = _load(module);
    py::dict<int64_t, FuncExtent> funcs;
    for (const auto& kv : l->funcs.items())
        if (std::get<1>(kv).end - std::get<0>(kv) >= 8) funcs.set(std::get<0>(kv), std::get<1>(kv));
    if (funcs.empty())
        throw py::ValueError(py::percent(py::str("%s: no functions recovered in section %s"), module, l->secname));
    int64_t off;
    if (std::holds_alternative<py::none_t>(func)) {
        off = INT64_MAX;
        for (int64_t k : funcs.keys()) off = std::min(off, k);
    } else {
        int64_t val = std::holds_alternative<py::str>(func) ? py::int_parse(std::get<py::str>(func), 0) : std::get<int64_t>(func);
        off = val >= l->va0 ? val - l->va0 : val;
        if (!funcs.contains(off)) {
            // snap to the enclosing function if the address lands inside one
            std::optional<int64_t> holder;
            for (const auto& kv : funcs.items()) {
                if (std::get<0>(kv) <= off && off < std::get<1>(kv).end) {
                    holder = std::get<0>(kv);
                    break;
                }
            }
            if (holder) {
                off = *holder;
            } else if (0 <= off && off < l->code.size()) {
                // An explicit VA/offset the recovery didn't mark as a function (e.g. an EXPORT not reached by
                // call-graph traversal). The caller knows it's a function, so accept it as a standalone start:
                // end = the next recovered function start, else section end.
                std::optional<int64_t> next;
                for (int64_t o : funcs.keys())
                    if (o > off && (!next || o < *next)) next = o;
                ResolvedFunc r;
                r.off = off;
                r.va0 = l->va0;
                r.code = l->code;
                r.end = next ? *next : l->code.size();
                r.secname = l->secname;
                return r;
            } else {
                throw py::ValueError(py::percent(py::str("%s: no function at offset 0x%x (VA 0x%x)"), module, off, l->va0 + off));
            }
        }
    }
    ResolvedFunc r;
    r.off = off;
    r.va0 = l->va0;
    r.code = l->code;
    r.end = funcs.at(off).end;
    r.secname = l->secname;
    return r;
}
}  // namespace

// Map a user-supplied func selector to (off, va0, code, end, secname).  func may be: None (first function), a
// section offset or VA as hex ('0x...') or int, or a decimal offset. A VA (>= va0) is converted to an offset; an
// offset is taken as-is.
ResolvedFunc _resolve_func(const py::str& module, const FuncSelector& func) {
    std::string memo = resolved_key(module, func);
    auto it = _RESOLVED.find(memo);
    if (it != _RESOLVED.end()) return it->second;
    ResolvedFunc out = _resolve_func_(module, func);
    _RESOLVED[memo] = out;
    return out;
}

// ---- stubbed APIs -----------------------------------------------------------------------------------------------
namespace {
py::dict<py::str, py::list<std::tuple<int64_t, std::optional<int64_t>>>> _OUTS;

// knowledge._bare: `KERNEL.GLOBALALLOC`, `USER32!FindWindowA` -> the function's own name; anything else as it is
py::str bare(const py::str& name) {
    auto m = py::re::fullmatch(py::str("\\w+[.!](\\w+)"), name);
    return m ? m->group(1) : name;
}
}  // namespace

// [(place, value)] of an API's parameters through which it gives something back, by its declaration: a handle
// (value None: _OUT_HANDLE by its place) or a number (0).
py::list<std::tuple<int64_t, std::optional<int64_t>>> _outs(const py::str& name) {
    if (!_OUTS.contains(name)) {
        std::optional<symbols::prototypes::Prototype> proto;
        try {
            proto = symbols::prototypes::of(bare(name), 32);   // knowledge.prototype("win32", name)
        } catch (const py::Exception&) {                       // (no headers here: nothing is given back)
            proto.reset();
        }
        py::list<std::tuple<int64_t, std::optional<int64_t>>> out;
        if (proto) {
            int64_t i = 0;
            for (const auto& p : proto->parameters) {
                const std::string ty = std::get<0>(p).utf8();
                size_t e = ty.find_last_not_of(" \t\r\n\f\v");
                if (e == std::string::npos) throw py::IndexError("list index out of range");
                size_t b = ty.find_last_of(" \t\r\n\f\v", e);
                size_t from = b == std::string::npos ? 0 : b + 1;
                std::string t = ty.substr(from, e - from + 1);
                if (t == "PHKEY" || t == "PHANDLE" || t == "LPHANDLE") out.append(std::make_tuple(i, std::optional<int64_t>()));
                else if (t == "LPDWORD" || t == "PDWORD" || t == "LPLONG" || t == "PLONG" || t == "PULONG" ||
                         t == "LPUINT" || t == "PUINT")
                    out.append(std::make_tuple(i, std::optional<int64_t>(0)));
                i++;
            }
        }
        _OUTS.set(name, out);
    }
    return _OUTS.at(name);
}

std::optional<int64_t> _uc_reg(const py::str& name) { return unicorn_uc::x86_reg(name.upper()); }

namespace {
// (module file, its hash, section start) -> its decode table: an instruction there is decoded once per process,
// not once per run (identity of the table is what the recorder keys its static tables on)
std::map<std::tuple<std::string, std::string, int64_t>, std::shared_ptr<void>> _INSN;
std::shared_ptr<void> insn_table(const py::str& path, const py::str& sha, int64_t lo) {
    auto key = std::make_tuple(path.utf8(), sha.utf8(), lo);
    auto it = _INSN.find(key);
    if (it == _INSN.end()) it = _INSN.emplace(key, std::make_shared<char>(0)).first;
    return it->second;
}

int64_t reg(const char* name) { return unicorn_uc::x86_reg(py::str(name)).value(); }

bool in_image(int64_t base, int64_t secbase, int64_t size) {
    return secbase <= base && base < secbase + std::max(size, _PAGE);
}
bool in_scratch(int64_t base) { return _SCRATCH_BASE <= base && base < _SCRATCH_BASE + _SCRATCH_SIZE; }

// The module's code ranges (_CODE_RANGES), or the whole buffer when none were recorded
std::vector<std::tuple<int64_t, int64_t>> code_ranges(const py::str& file, int64_t base, int64_t length) {
    if (auto got = _CODE_RANGES.get(file)) {
        if (!got->empty()) return got->vec();
    }
    return {std::make_tuple(base, base + length)};
}
}  // namespace

// A fresh emulator with the module laid out: its image, the original beside it (`overlay`), the caller's memory
// (`first`), and a landing place in every import slot.
//
// When stubbing externals, a unique SENTINEL address is planted in each IAT slot so a `call [iat_slot]` resolves to
// a NAMED import (not a null pointer), and its stdcall argument count can be looked up for correct stack cleanup.
static std::shared_ptr<Machine> _machine(const py::str& path, const py::str& sha, int64_t va0, const py::bytes& code,
                                         int64_t secbase, int64_t size, py::dict<int64_t, std::tuple<py::str, py::str>> imports,
                                         const std::optional<py::str>& overlay, bool stub_externals,
                                         const py::list<std::tuple<int64_t, py::bytes>>& first) {
    auto mach = std::make_shared<Machine>();
    mach->uc = unicorn_uc::open_x86(32);
    Uc& uc = *mach->uc;
    uc.mem_map(secbase, std::max(size, _PAGE));
    uc.mem_write(va0, code);
    uc.mem_map(_SCRATCH_BASE, _SCRATCH_SIZE);
    uc.mem_map(_STACK_BASE, _STACK_SIZE);
    py::set<int64_t>& mapped = mach->mapped;   // page-group bases we've mapped
    mapped.add(secbase);
    mapped.add(_SCRATCH_BASE);
    for (int64_t a = _STACK_BASE; a < _STACK_BASE + _STACK_SIZE; a += _PAGE) mapped.add(a);
    py::dict<int64_t, std::tuple<py::str, py::str>>& sentinel_map = mach->sentinels;   // sentinel addr -> (dll, name)
    std::vector<std::tuple<int64_t, py::bytes>> mem_init = first.vec();
    // The module's code as it is in the file: an instruction there is decoded once per process, not once per run --
    // unless something was written over it (then memory is what is read).
    for (const auto& r : code_ranges(path, va0, code.size())) {
        int64_t lo = std::get<0>(r), hi = std::get<1>(r);
        mach->statics.emplace_back(static_cast<uint64_t>(lo), static_cast<uint64_t>(hi), code.slice(lo - va0, hi - va0),
                                   insn_table(path, sha, lo));
    }
    // `overlay` is another module whose code is mapped alongside at its own addresses. A rebuilt body refers to the
    // ORIGINAL's code and data by literal address (a function pointer, a thunk target); without the original there,
    // those addresses read as zeros in the rebuilt run only, and the two runs differ for a reason that has nothing to
    // do with the body.
    std::optional<size_t> laid;   // the position of the overlay's image in mem_init
    if (overlay) {
        try {
            std::shared_ptr<const Loaded> o = _load(*overlay);
            int64_t own_hi = secbase + std::max(size, _PAGE);
            if (o->va0 + o->code.size() <= secbase || o->va0 >= own_hi) {   // never over the traced image
                mem_init.insert(mem_init.begin(), std::make_tuple(o->va0, o->code));
                laid = 0;
                for (const auto& r : code_ranges(o->path, o->va0, o->code.size())) {
                    int64_t lo = std::get<0>(r), hi = std::get<1>(r);
                    mach->statics.emplace_back(static_cast<uint64_t>(lo), static_cast<uint64_t>(hi),
                                               o->code.slice(lo - o->va0, hi - o->va0), insn_table(o->path, o->sha, lo));
                }
                // dict(oimports, **imports): the keyword form needs text keys, which an import table's are not --
                // it raises (and the overlay's imports are not merged) unless the module has no imports of its own
                if (imports.empty()) imports = o->imports.copy();
            }
        } catch (const py::Exception&) {
        }
    }
    auto& slots = mach->slots;   // page -> [(address, bytes)] of import slots
    if (stub_externals && !imports.empty()) {
        std::vector<std::tuple<int64_t, std::tuple<py::str, py::str>>> sorted_imports = imports.items();
        std::sort(sorted_imports.begin(), sorted_imports.end(),
                  [](const auto& a, const auto& b) { return std::get<0>(a) < std::get<0>(b); });
        int64_t i = 0;
        auto page_list = [&](int64_t page) {
            if (!slots.contains(page)) slots.set(page, py::list<std::tuple<int64_t, py::bytes>>());
            return slots.at(page);
        };
        for (const auto& kv : sorted_imports) {
            int64_t iat_va = std::get<0>(kv);
            int64_t sent = _SENT_BASE + i * _SENT_STRIDE;
            std::string le(4, '\0');
            for (int k = 0; k < 4; k++) le[k] = static_cast<char>((sent >> (8 * k)) & 0xFF);
            auto entry = std::make_tuple(iat_va, py::bytes(le));
            mem_init.push_back(entry);
            page_list(iat_va & ~(_PAGE - 1)).append(entry);
            if (((iat_va + 3) & ~(_PAGE - 1)) != (iat_va & ~(_PAGE - 1))) page_list((iat_va + 3) & ~(_PAGE - 1)).append(entry);
            sentinel_map.set(sent, std::get<1>(kv));
            i++;
        }
        int64_t span = (imports.size() * _SENT_STRIDE + (_PAGE - 1)) & ~(_PAGE - 1);
        mem_init.emplace_back(_SENT_BASE, py::bytes::zeros(span));   // map+zero the sentinel landing region
    }
    // pages given other contents before the run are not "as in the file"
    py::set<int64_t>& init_pages = mach->init_pages;
    for (size_t idx = 0; idx < mem_init.size(); idx++) {
        if (laid && *laid == idx) continue;
        int64_t addr = std::get<0>(mem_init[idx]);
        int64_t len = std::get<1>(mem_init[idx]).size();
        for (int64_t pg = addr >> 12; pg < ((addr + std::max<int64_t>(len, 1) - 1) >> 12) + 1; pg++) init_pages.add(pg);
    }
    // pre-load memory (e.g. a controlled struct buffer a pointer arg points at); pages that lie side by side are
    // mapped together -- every mapping makes every later one slower
    std::set<int64_t> need;
    for (const auto& item : mem_init) {
        int64_t addr = std::get<0>(item);
        int64_t a = addr;
        while (a < addr + std::get<1>(item).size()) {
            int64_t base = a & ~(_PAGE - 1);
            if (!mapped.contains(base) && !in_image(base, secbase, size) && !in_scratch(base)) need.insert(base);
            a = base + _PAGE;
        }
    }
    std::optional<std::array<int64_t, 2>> run;
    std::vector<std::optional<int64_t>> bases(need.begin(), need.end());
    bases.push_back(std::nullopt);
    for (const std::optional<int64_t>& base : bases) {
        if (run && base && *base == (*run)[0] + (*run)[1]) {
            (*run)[1] += _PAGE;
            continue;
        }
        if (run) uc.mem_map((*run)[0], (*run)[1]);
        if (base) run = std::array<int64_t, 2>{*base, _PAGE};
        else run.reset();
    }
    for (int64_t b : need) mapped.add(b);
    for (const auto& item : mem_init) uc.mem_write(std::get<0>(item), std::get<1>(item));
    mach->context = uc.context_save();
    return mach;
}

Trace trace_function(const py::str& module, const FuncSelector& func, const TraceOptions& options) {
    using namespace native_trace;
    bool stub_externals = options.stub_externals ? *options.stub_externals : _DEFAULT_STUB.enabled;   // inherit the process default
    SigResolver sig_resolver = options.sig_resolver ? *options.sig_resolver : _DEFAULT_STUB.resolver;
    std::shared_ptr<const Loaded> loaded = _load(module);
    const py::str& path = loaded->path;
    const py::str& sha = loaded->sha;
    ResolvedFunc rf = _resolve_func(module, func);
    int64_t off = rf.off, va0 = rf.va0, end = rf.end;
    const py::bytes& code = rf.code;
    int64_t fva = va0 + off;
    py::list<int64_t> args;
    if (options.args) {
        args = *options.args;
    } else {
        int64_t n = options.arity ? *options.arity : (_arities(module).get(off, 0) ? _arities(module).get(off, 0) : 4);
        for (int64_t i = 0; i < n; i++) args.append(0);
    }
    int64_t insn_cap = options.insn_cap;

    int64_t secbase = va0 & ~(_PAGE - 1);
    int64_t size = (code.size() + (va0 - secbase) + (_PAGE - 1)) & ~(_PAGE - 1);
    py::list<std::tuple<int64_t, py::bytes>> mem_init = options.mem_init ? options.mem_init->copy()
                                                                        : py::list<std::tuple<int64_t, py::bytes>>();
    // A machine with the module laid out is kept between runs (reuse=True) and put back exactly as it was after
    // each: laying a module out costs more than most runs of a function do.
    bool reuse = options.reuse;
    MachineKey mkey(path, sha, options.overlay, stub_externals, options.fast);
    std::shared_ptr<Machine> mach;
    if (reuse && _MACHINES.contains(mkey)) mach = _MACHINES.pop(mkey);
    py::dict<int64_t, std::optional<py::bytes>> saved;   // page -> what it held (None: not there before)
    if (!mach)
        mach = _machine(path, sha, va0, code, secbase, size, loaded->imports, options.overlay, stub_externals,
                        reuse ? py::list<std::tuple<int64_t, py::bytes>>() : mem_init);
    Uc& uc = *mach->uc;
    py::dict<int64_t, std::tuple<py::str, py::str>> sentinel_map = mach->sentinels;
    bool stubbing_at = options.stub_at && !options.stub_at->empty() && stub_externals;
    if (stubbing_at) {   // functions entered as if they were imports: {address: (dll, name)}
        sentinel_map = mach->sentinels.copy();
        for (const auto& kv : options.stub_at->items()) sentinel_map.set(std::get<0>(kv), std::get<1>(kv));
    }
    py::set<int64_t> mapped = reuse ? py::set<int64_t>::from(mach->mapped) : mach->mapped;
    py::set<int64_t> init_pages = py::set<int64_t>::from(mach->init_pages);
    for (const auto& item : mem_init) {
        int64_t addr = std::get<0>(item), len = std::get<1>(item).size();
        for (int64_t pg = addr >> 12; pg < ((addr + std::max<int64_t>(len, 1) - 1) >> 12) + 1; pg++) init_pages.add(pg);
    }
    if (reuse) {
        uc.context_restore(mach->context);
        py::set<int64_t> again;
        for (const auto& item : mem_init) {   // this run's memory, over the kept machine
            int64_t addr = std::get<0>(item);
            const py::bytes& data = std::get<1>(item);
            int64_t a = addr;
            while (a < addr + data.size()) {
                int64_t base = a & ~(_PAGE - 1);
                if (!saved.contains(base)) {
                    if (!mapped.contains(base) && !in_image(base, secbase, size) && !in_scratch(base)) {
                        uc.mem_map(base, _PAGE);
                        mapped.add(base);
                        saved.set(base, std::nullopt);
                    } else {
                        saved.set(base, uc.mem_read(base, _PAGE));
                    }
                }
                if (mach->slots.contains(base)) again.add(base);
                a = base + _PAGE;
            }
            uc.mem_write(addr, data);
        }
        for (int64_t base : again) {   // import slots are written last, as ever
            for (const auto& slot : mach->slots.at(base)) uc.mem_write(std::get<0>(slot), std::get<1>(slot));
        }
        int64_t top = (_STACK_TOP - 4 * (args.size() + 1)) & ~(_PAGE - 1);
        for (int64_t base = top; base < _STACK_TOP + 1; base += _PAGE) {
            if (!saved.contains(base) && mapped.contains(base)) saved.set(base, uc.mem_read(base, _PAGE));
        }
    }

    // Build the stdcall frame: args in order above a RET sentinel the emulation halts on.
    auto le32 = [](int64_t v) {
        std::string s(4, '\0');
        for (int k = 0; k < 4; k++) s[k] = static_cast<char>((v >> (8 * k)) & 0xFF);
        return py::bytes(s);
    };
    int64_t esp = _STACK_TOP;
    for (int64_t i = args.size() - 1; i >= 0; i--) {
        esp -= 4;
        uc.mem_write(esp, le32(args[i] & 0xFFFFFFFF));
    }
    esp -= 4;
    uc.mem_write(esp, le32(_RET));
    const int64_t ESP = reg("ESP"), EAX = reg("EAX"), EIP = reg("EIP"), ECX = reg("ECX"), EDX = reg("EDX");
    const int64_t EBX = reg("EBX"), ESI = reg("ESI"), EDI = reg("EDI"), EBP = reg("EBP");
    uc.reg_write(ESP, esp);
    // Sentinels in the registers a function must preserve: one that hands a register back changed is caught even
    // though its caller here holds nothing of value there. (A function's callers keep live values in these; a rebuilt
    // body that loses one breaks them, not itself.)
    std::array<int64_t, 4> sents = options.sentinels ? *options.sentinels : SAVED_SENTINELS;
    const int64_t preserved_regs[4] = {EBX, ESI, EDI, EBP};
    for (int k = 0; k < 4; k++) uc.reg_write(preserved_regs[k], sents[k]);
    if (options.ecx) uc.reg_write(ECX, *options.ecx & 0xFFFFFFFF);   // an argument in ECX: a member function's object
    if (options.edx) uc.reg_write(EDX, *options.edx & 0xFFFFFFFF);
    int64_t entry_esp = esp;

    Trace rec;
    rec.module = py::os::path::basename(module);   // clean label (module may be a path)
    rec.module_path = path;                         // the resolved file actually traced
    rec.target_sha256 = sha;
    rec.section = rf.secname;
    rec.func_off = off;
    rec.func_va = fva;
    rec.func_end_va = va0 + end;
    for (int64_t a : args) rec.input.args.append(a & 0xFFFFFFFF);
    rec.input.arity = args.size();
    rec.entry_esp = entry_esp;
    int64_t func_lo = fva, func_hi = va0 + end;

    int64_t sec_lo = secbase, sec_hi = secbase + std::max(size, _PAGE);
    bool fast = options.fast && !_GOV.ceil && options.shadow == nullptr;
    bool lean = options.lean;
    if (lean) rec.pages0 = py::dict<uint64_t, py::bytes>();

    // The hooks are C++ (native/cb/emulate/tracer: cbtrace.dll; emulate/native_trace), attached to this Unicorn in
    // trace_function's order, keeping the record -- no Python per instruction, and a shadow (the path explorer's)
    // stepped there too.
    native_trace::require();
    RunArgs ra;
    ra.statics = mach->statics;
    for (int64_t p : init_pages) ra.init_pages.append(static_cast<uint64_t>(p));
    for (int64_t p : mapped) ra.mapped.append(static_cast<uint64_t>(p));
    for (const auto& kv : sentinel_map.items()) ra.sentinels.set(static_cast<uint64_t>(std::get<0>(kv)), std::get<1>(kv));
    if (reuse) {
        std::vector<uint64_t> keys;
        for (int64_t k : saved.keys()) keys.push_back(static_cast<uint64_t>(k));
        ra.saved = keys;
    }
    ra.sha = sha;
    ra.gov = &_GOV;
    ra.fast = fast;
    ra.lean = lean;
    ra.reads = options.reads;
    if (options.quiet_stack)
        ra.quiet = std::make_tuple(static_cast<uint64_t>(_STACK_BASE), static_cast<uint64_t>(_STACK_BASE + _STACK_SIZE));
    ra.stop_wild = options.stop_wild;
    ra.stub = stub_externals;
    ra.unique = options.stub_ret_unique;
    ra.branches = options.branches;
    ra.pads = options.pads;
    ra.insn_cap = insn_cap;
    ra.stub_ret = options.stub_ret;
    ra.stub_cleanup = options.stub_cleanup;
    ra.func = std::make_tuple(static_cast<uint64_t>(func_lo), static_cast<uint64_t>(func_hi));
    ra.sec = std::make_tuple(static_cast<uint64_t>(sec_lo), static_cast<uint64_t>(sec_hi));
    ra.lean_ranges = std::make_tuple(static_cast<uint64_t>(_STACK_BASE - 1), static_cast<uint64_t>(_STACK_BASE + _STACK_SIZE));
    ra.sig_resolver = sig_resolver;
    ra.outs = _outs;
    ra.cache = &mach->cache;
    // the landing places are kept with the machine (by identity of what they were made from: its sentinel map and
    // stub_at -- a stub_at given this once never matches another)
    static uint64_t fresh = 0;
    ra.sentinel_from.push_back(&mach->sentinels);
    ra.sentinel_from.push_back(stubbing_at ? reinterpret_cast<const void*>(++fresh) : nullptr);
    ra.shadow = native_shadow::handle_of(options.shadow);
    Engine eng;
    eng.uch = uc.handle();
    eng.emu_start = [&uc](int64_t begin, int64_t until, int64_t count) { uc.emu_start(begin, until, 0, count); };
    auto nrun = std::make_unique<Run>(eng, ra);
    auto start = [&](int64_t b, int64_t u, int64_t count = 0) { nrun->start(eng, b, u, count); };

    TraceRecord tr;
    auto settle = [&]() { nrun->settle(tr); };

    try {
        if (fast) {
            start(fva, _RET);
            if (nrun->switched()) {   // from here on, one instruction at a time
                nrun->attach_code();
                uc.ctl_flush_tb();
                start(uc.reg_read(EIP), _RET, insn_cap + 1);
            }
        } else {
            start(fva, _RET, insn_cap + 1);
        }
        settle();
        rec.truncated = tr.truncated.value_or(false);
        rec.exit = rec.truncated ? py::str("truncated") : py::str("ret");
        // The emulator can stop without raising (a divide by zero does): it only RETURNED if it stopped at the return
        // sentinel. Anything else is a fault, wherever it stopped.
        if (!rec.truncated && uc.reg_read(EIP) != _RET)
            rec.exit = py::percent(py::str("exception: stopped at 0x%x"), uc.reg_read(EIP));
        rec.ret = uc.reg_read(EAX);
        // The registers a function must hand back unchanged (they are 0 on entry here), and where the stack pointer
        // ended up relative to entry: 4 = cdecl, 4 + N = stdcall with N bytes.
        try {
            std::array<int64_t, 4> regs = {uc.reg_read(EBX), uc.reg_read(ESI), uc.reg_read(EDI), uc.reg_read(EBP)};
            rec.saved_regs = regs;
            rec.esp_delta = (uc.reg_read(ESP) - entry_esp) & 0xFFFFFFFF;
        } catch (const py::Exception&) {
        }
        try {   // x87 return (float functions -> st0)
            rec.st0 = uc.reg_read_float(reg("ST0"));
        } catch (const py::Exception&) {
        }
    } catch (const UcError& e) {
        settle();
        rec.truncated = tr.truncated.value_or(false);
        rec.exit = py::str("exception: ") + unicorn_uc::strerror(e.errno_);
        try {
            rec.ret = uc.reg_read(EAX);
        } catch (const py::Exception&) {
        }
    }

    nrun->collect(tr);
    // what the hooks recorded, in the record
    if (tr.insn_count) rec.insn_count = *tr.insn_count;
    if (tr.truncated) rec.truncated = *tr.truncated;
    if (tr.path) rec.path = *tr.path;
    // mnemonics keyed by int va -> stringify for JSON friendliness
    if (tr.mnemonics) {
        for (const auto& kv : tr.mnemonics->items())
            rec.mnemonics.set(py::percent(py::str("0x%x"), static_cast<int64_t>(std::get<0>(kv))), std::get<1>(kv));
    }
    if (tr.calls) rec.calls = *tr.calls;
    if (tr.mem_reads) rec.mem_reads = *tr.mem_reads;
    if (tr.mem_writes) rec.mem_writes = *tr.mem_writes;
    if (tr.lazy_maps) rec.lazy_maps = *tr.lazy_maps;
    if (tr.pages0) rec.pages0 = *tr.pages0;
    rec.governed = tr.governed;
    rec.memops = tr.memops;
    rec.writes_seen = tr.writes_seen;
    rec._dirty = tr._dirty;
    rec._stuck = tr._stuck;
    rec.bogus_stubs = tr.bogus_stubs;
    rec.ran_into_zeros = tr.ran_into_zeros;
    rec.api_deps = tr.api_deps;
    rec.api_calls = tr.api_calls;
    rec.stubbed_externals = tr.stubbed_externals;
    rec.vxd_services = tr.vxd_services;
    rec.branches = tr.branches;
    if (lean) rec.writes_seen = nrun->lean_seen;
    if (reuse) {   // the machine, as it was, for the next run
        try {
            if (lean) uc.mem_write(_STACK_BASE, py::bytes::zeros(_STACK_SIZE));   // (its writes were not followed)
            nrun->detach();
            for (const auto& kv : saved.items()) {
                if (!std::get<1>(kv)) uc.mem_unmap(std::get<0>(kv), _PAGE);
                else uc.mem_write(std::get<0>(kv), *std::get<1>(kv));
            }
            nrun->restore();   // (the pages its hooks saved, after Python's)
            while (_MACHINES.size() >= _MAX_MACHINES) _MACHINES.pop(_MACHINES.keys()[0]);
            _MACHINES.set(mkey, mach);
        } catch (const py::Exception&) {   // not put back: the next run makes its own
        }
    }
    return rec;
}

namespace {
const py::str _HARNESS = py::str("add byte ptr [eax], al");   // (a call through nothing lands in zeros: the stand-in's write)
}

// trace_function's record as the one run record (emulate/run.py): EAX (and x87 ST0) the results, the imports and
// callbacks entered with their arguments, EBX ESI EDI EBP against the sentinels they were given (`sentinels`, else
// SAVED_SENTINELS), every instruction run (first seen first). A write made on the way out of zeros is the harness's.
// A lean run's writes carry what their bytes held before the run (its pages as first written).
run::Run record(const Trace& rec, const std::optional<std::array<int64_t, 4>>& sentinels) {
    auto mn_at = [&](int64_t pc) -> std::optional<py::str> { return rec.mnemonics.get(py::percent(py::str("0x%x"), pc)); };
    py::list<run::Write> writes;
    if (rec.pages0) {
        for (const auto& w : std::get<py::list<native_trace::LeanWrite>>(rec.mem_writes)) {
            int64_t a = static_cast<int64_t>(std::get<0>(w)), n = static_cast<int64_t>(std::get<1>(w));
            int64_t v = static_cast<int64_t>(std::get<2>(w)), pc = static_cast<int64_t>(std::get<3>(w));
            int64_t old = 0;
            for (int64_t i = 0; i < std::min<int64_t>(n, 8); i++) {
                int64_t b = a + i;
                uint64_t page = static_cast<uint64_t>(b & ~(_PAGE - 1));
                old |= rec.pages0->at(page)[b & (_PAGE - 1)] << (8 * i);
            }
            std::optional<py::str> mn = pc ? mn_at(pc) : std::nullopt;
            writes.append(run::Write{a, n, v, old, pc != 0 && mn && *mn == _HARNESS});
        }
    } else {
        for (const auto& w : std::get<py::list<native_trace::MemWrite>>(rec.mem_writes)) {
            std::optional<py::str> mn = mn_at(static_cast<int64_t>(w.pc));
            writes.append(run::Write{static_cast<int64_t>(w.addr), static_cast<int64_t>(w.size), static_cast<int64_t>(w.value),
                                     static_cast<int64_t>(w.old), mn && *mn == _HARNESS});
        }
    }
    std::array<int64_t, 4> given = sentinels ? *sentinels : SAVED_SENTINELS;
    std::optional<run::Preserved> preserved;
    if (rec.saved_regs) {
        run::Preserved p;
        const char* names[4] = {"EBX", "ESI", "EDI", "EBP"};
        for (int k = 0; k < 4; k++)
            p.set(py::str(names[k]), std::make_tuple(std::optional<int64_t>(given[k]), std::optional<int64_t>((*rec.saved_regs)[k])));
        preserved = p;
    }
    py::dict<py::str, run::Register> results;
    results.set(py::str("EAX"), rec.ret ? run::Register(*rec.ret) : run::Register(py::none_t{}));
    results.set(py::str("ST0"), rec.st0 ? run::Register(*rec.st0) : run::Register(py::none_t{}));
    py::list<run::Call> calls;
    if (rec.api_calls) {
        for (const auto& c : *rec.api_calls) calls.append(run::Call{std::get<0>(c), std::get<1>(c), run::Detail(py::none_t{})});
    }
    py::list<int64_t> path;
    for (const auto& k : rec.mnemonics.keys()) path.append(py::int_parse(k, 16));   // int(k, 16)
    int64_t seen = rec.writes_seen ? *rec.writes_seen
                                   : std::visit([](const auto& l) { return static_cast<int64_t>(l.size()); }, rec.mem_writes);
    return run::Run(rec.exit, results, writes, seen >= _MAX_MEM, calls, py::list<run::IO>(), rec.esp_delta, preserved, path,
                    rec.insn_count, rec.truncated, py::str("little"), std::make_shared<Trace>(rec));
}

// The platform's runner (platforms/win32.py emulator.run): trace_function(module, **kw) as a Run.
run::Run recorded(const py::str& module, const FuncSelector& func, const TraceOptions& options) {
    return record(trace_function(module, func, options), options.sentinels);
}

// Argument count from the knowledge base's win32 api_model (knowledge/models/win32.json: documented counts for
// common APIs, A/W variants sharing one), or None.
std::optional<int64_t> _builtin_argc(const std::optional<py::str>& name) {
    py::str n = name ? *name : py::str("");
    auto model = knowledge::api_model(py::str("win32"), n);
    if (!model && !n.empty() && (n.slice_from(-1) == py::str("A") || n.slice_from(-1) == py::str("W")))   // ANSI/Wide variant
        model = knowledge::api_model(py::str("win32"), n.slice(0, n.size() - 1));
    if (!model) return std::nullopt;
    auto argc = model->get(py::str("argc"));
    if (!argc || argc->kind != knowledge::sources::Json::Int) return std::nullopt;
    return argc->i;
}

}  // namespace codebreaker::emulate::emulator

