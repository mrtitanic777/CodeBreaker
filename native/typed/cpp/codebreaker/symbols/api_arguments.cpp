// codebreaker/symbols/api_arguments.cpp -- the shim's translation unit (see api_arguments.hpp). The new module
// (cb/symbols/api_arguments) walks the code and chooses between the headers and the code; this file gives it the corpus, the
// knowledge base and the 16-bit readers the old layer has, and converts types.

#include "codebreaker/symbols/api_arguments.hpp"

#include <algorithm>
#include <mutex>
#include <set>

#include "cb/compat/py_bridge.hpp"
#include "cb/symbols/api_arguments.hpp"
#include "codebreaker/base/corpus.hpp"
#include "codebreaker/decompile/describe.hpp"
#include "codebreaker/engine/facts.hpp"
#include "codebreaker/knowledge/package.hpp"
#include "codebreaker/knowledge/reads.hpp"
#include "codebreaker/knowledge/sources.hpp"
#include "codebreaker/knowledge/store.hpp"
#include "py_json.hpp"
#include "py_stdlib.hpp"

namespace codebreaker::symbols::api_arguments {

namespace {

namespace aa = cb::symbols::api_arguments;
using cb::compat::from_py;
using cb::compat::to_py;
using engine::facts::Json;
using engine::facts::Value;

std::mutex g_mods_lock;
py::dict<py::str, std::shared_ptr<_Mod>> _MODS;                 // DLL stem (upper) -> _Mod | None
py::dict<py::str, py::dict<py::str, std::optional<int64_t>>> _CACHE;   // DLL stem (upper) -> {name: bytes|None}

Json json_none() { return Json(); }
Json json_int(int64_t v) {
    Json j;
    j.kind = Json::Int;
    j.i = v;
    return j;
}
Json json_of(const std::optional<int64_t>& v) { return v ? json_int(*v) : json_none(); }
std::optional<int64_t> int_of(const Json& j) {
    if (j.kind == Json::Int) return j.i;
    return std::nullopt;
}

std::optional<int64_t> widen(const std::optional<std::uint32_t>& v) { return v ? std::optional<int64_t>(*v) : std::nullopt; }
std::optional<std::uint32_t> narrow(const std::optional<int64_t>& v) {
    return v && *v >= 0 && *v <= 0xFFFFFFFFLL ? std::optional<std::uint32_t>(static_cast<std::uint32_t>(*v)) : std::nullopt;
}

const aa::ExportingModule* cb_module_of(const _Mod& m) { return static_cast<const aa::ExportingModule*>(m.cb_module.get()); }

// The corpus as the stack walk asks for it: the modules the old layer found.
class OldModules final : public aa::ModuleSource {
public:
    std::shared_ptr<const aa::ExportingModule> find(std::string_view stem) override {
        const std::shared_ptr<_Mod> m = _mod(to_py(stem));
        return m ? std::static_pointer_cast<const aa::ExportingModule>(m->cb_module) : nullptr;
    }
};

// The one scanner (a scan is serialised: the decoders and the modules' memos belong to one thread at a time).
std::mutex g_scan_lock;
aa::StackScanner& scanner() {
    static OldModules modules;
    static aa::StackScanner s(modules);
    return s;
}

}  // namespace

const py::list<py::str> _EXTS{py::str(".dll"), py::str(".exe"), py::str(".drv"), py::str(".cpl"), py::str(".ocx")};

std::optional<py::dict<py::str, std::optional<int64_t>>> _TABLE;

// ---- _Mod --------------------------------------------------------------------------------------------------------

_Mod::_Mod(const py::str& path) {
    pe = std::make_shared<formats::pe::PEFile>(path);
    base = pe->image_base;
    for (const auto& e : pe->exports()) {
        int64_t o = std::get<0>(e);
        const std::optional<py::str>& n = std::get<1>(e);
        int64_t rva = std::get<2>(e);
        ords.set(o, rva);
        if (n && !n->empty()) exports.set(*n, rva);
    }
    imports = pe->imports();
    cb_module = std::make_shared<aa::ExportingModule>(cb::formats::pe::Image::parse(cb::to_bytes(pe->data.raw()), from_py(path)));
    std::tuple<int64_t, int64_t> ed = pe->datadirs.empty() ? std::tuple<int64_t, int64_t>{0, 0} : pe->datadirs[0];
    exp_lo = std::get<0>(ed);
    exp_hi = std::get<0>(ed) + std::get<1>(ed);
}

py::bytes _Mod::bytes_at(int64_t rva, int64_t n) const {
    return to_py(cb_module_of(*this)->bytes_at(rva, static_cast<std::size_t>(std::max<int64_t>(n, 0))));
}

std::optional<py::str> _Mod::forwarder(int64_t rva) const {
    if (rva < 0 || rva > 0xFFFFFFFFLL) return std::nullopt;
    const std::optional<std::string> fwd = cb_module_of(*this)->forwarder(static_cast<std::uint32_t>(rva));
    return fwd ? std::optional<py::str>(to_py(*fwd)) : std::nullopt;
}

// ---- the DLL that exports a function -----------------------------------------------------------------------------

py::str _stem(const py::str& dll) { return to_py(aa::stem(from_py(dll))); }

std::shared_ptr<_Mod> _mod(const py::str& stem) {
    std::lock_guard<std::mutex> hold(g_mods_lock);
    if (!_MODS.contains(stem)) {
        std::shared_ptr<_Mod> m;
        for (const py::str& e : _EXTS) {
            std::optional<py::str> p = base::corpus::retail(stem.lower() + e);
            if (p && !p->empty()) {
                try {
                    m = std::make_shared<_Mod>(*p);
                    break;
                } catch (const std::exception&) {
                    m = nullptr;
                }
            }
        }
        _MODS.set(stem, m);
    }
    return _MODS.at(stem);
}

std::optional<int64_t> _scan(_Mod& m, int64_t rva, int64_t hops) {
    std::lock_guard<std::mutex> hold(g_scan_lock);
    return widen(scanner().scan(*cb_module_of(m), rva, static_cast<int>(std::clamp<int64_t>(hops, -1, aa::kMaxHops))));
}

std::optional<int64_t> _resolve(const py::str& dll, const py::str& name, int64_t hops) {
    std::lock_guard<std::mutex> hold(g_scan_lock);
    return widen(scanner().resolve(from_py(dll), from_py(name), static_cast<int>(std::clamp<int64_t>(hops, -1, aa::kMaxHops))));
}

// The table an older CodeBreaker kept for `stem` beside the corpus (apiarity/<stem>.json), or {}: carried into the
// knowledge base once, then not read again.
py::dict<py::str, std::optional<int64_t>> _apiarity(const py::str& stem) {
    py::dict<py::str, std::optional<int64_t>> out;
    try {
        py::str path = py::os::path::join(base::corpus::design_dir(), py::str("apiarity"), stem.lower() + py::str(".json"));
        py::str text = py::open(path, py::str("r"), py::str("utf-8")).read_text();
        Json j = engine::facts::json_loads(text);
        if (j.kind != Json::Dict) return {};
        for (const auto& kv : j.dict.items()) out.set(std::get<0>(kv), int_of(std::get<1>(kv)));
    } catch (const std::exception&) {
        return {};
    }
    return out;
}

namespace {

// {name: bytes or None} found for the exports of `stem` (the knowledge base's, this process's view)
py::dict<py::str, std::optional<int64_t>> load_(const py::str& stem) {
    if (!_CACHE.contains(stem)) {
        auto kb = knowledge::store::open();
        if (std::holds_alternative<std::monostate>(kb->carried(py::str("apiarity ") + stem))) {
            py::dict<py::str, std::optional<int64_t>> old = _apiarity(stem);
            if (!old.empty()) {
                py::dict<py::str, Value> rows;
                for (const auto& kv : old.items()) rows.set(stem + py::str(".") + std::get<0>(kv), Value(json_of(std::get<1>(kv))));
                kb->put_many(py::str("win32"), py::str("argument_bytes.code"), rows,
                             py::percent(py::str("%s's code (carried over from apiarity/%s.json)"), stem.lower(), stem.lower()));
            }
            kb->carried(py::str("apiarity ") + stem, py::str(std::to_string(old.size())));
        }
        py::str prefix = stem + py::str(".");
        py::dict<py::str, std::optional<int64_t>> found;
        for (const auto& kv : kb->table(py::str("win32"), py::str("argument_bytes.code")).items()) {
            const py::str& k = std::get<0>(kv);
            if (k.startswith(prefix)) {
                const Value& v = std::get<1>(kv);
                found.set(k.slice(prefix.size(), k.size()), std::holds_alternative<Json>(v) ? int_of(std::get<Json>(v)) : std::nullopt);
            }
        }
        _CACHE.set(stem, found);
    }
    return _CACHE.at(stem);
}

void save_(const py::str& stem, const py::str& name) {
    knowledge::store::open()->put(py::str("win32"), py::str("argument_bytes.code"), stem + py::str(".") + name,
                                  Value(json_of(_CACHE.at(stem).at(name))), stem.lower() + py::str("'s code"));
}

// What the headers and the knowledge base hold of 32-bit exports.
class OldWin32Memory final : public aa::Win32Memory {
public:
    std::optional<std::uint32_t> header_bytes(std::string_view name) override {
        // win32_arguments.json beside this file (tools/make_api_table.py regenerates it), as the knowledge base has it
        if (!_TABLE) {
            py::dict<py::str, std::optional<int64_t>> t;
            for (const auto& kv : knowledge::_table(py::str("win32"), py::str("argument_bytes.header")).items())
                t.set(std::get<0>(kv), int_of(std::get<Json>(std::get<1>(kv))));
            _TABLE = t;
        }
        knowledge::reads::record(py::str("win32"), py::str("argument_bytes.header"), to_py(name));       // (knowledge/reads.py)
        return narrow(_TABLE->get(to_py(name)).value_or(std::nullopt));
    }
    std::optional<aa::Known> scanned(std::string_view stem, std::string_view name) override {
        const py::str s = to_py(stem), n = to_py(name);
        py::dict<py::str, std::optional<int64_t>> c = load_(s);
        if (!c.contains(n)) return std::nullopt;
        knowledge::reads::record(py::str("win32"), py::str("argument_bytes.code"), s + py::str(".") + n);
        return aa::Known{narrow(c.at(n))};
    }
    void remember(std::string_view stem, std::string_view name, std::optional<std::uint32_t> bytes) override {
        const py::str s = to_py(stem), n = to_py(name);
        load_(s);
        _CACHE.at(s).set(n, widen(bytes));
        save_(s, n);
        knowledge::reads::record(py::str("win32"), py::str("argument_bytes.code"), s + py::str(".") + n);
    }
};

}  // namespace

std::optional<int64_t> header_bytes(const py::str& name) {
    OldWin32Memory memory;
    return widen(memory.header_bytes(from_py(name)));
}

std::optional<int64_t> stack_bytes(const py::str& dll, const py::str& name) {
    OldWin32Memory memory;
    std::lock_guard<std::mutex> hold(g_scan_lock);
    return widen(aa::Win32Arguments(memory, scanner()).stack_bytes(from_py(dll), from_py(name)));
}

std::optional<int64_t> argc(const py::str& dll, const py::str& name) {
    OldWin32Memory memory;
    std::lock_guard<std::mutex> hold(g_scan_lock);
    return widen(aa::Win32Arguments(memory, scanner()).argc(from_py(dll), from_py(name)));
}

// ---- 16-bit ----------------------------------------------------------------------------------------------------

namespace {

const py::dict<py::str, py::list<py::str>>& files_() {
    // (a driver exports under its interface's name, not its file's: Setup's display driver is VGA.DRV, module
    // DISPLAY -- found by its file, USER's ShowCursor calling DISPLAY.102 (SetCursor) was given no argument size,
    // and its C "faulted" in the gate)
    static const py::dict<py::str, py::list<py::str>> f = [] {
        py::dict<py::str, py::list<py::str>> d;
        d.set(py::str("KERNEL"), py::list<py::str>{py::str("krnl386.exe"), py::str("kernel.exe")});
        d.set(py::str("USER"), py::list<py::str>{py::str("user.exe")});
        d.set(py::str("GDI"), py::list<py::str>{py::str("gdi.exe")});
        d.set(py::str("DISPLAY"), py::list<py::str>{py::str("vga.drv"), py::str("display.drv")});
        d.set(py::str("MOUSE"), py::list<py::str>{py::str("msmous31.drv"), py::str("lmouse31.drv"), py::str("mouse.drv")});
        return d;
    }();
    return f;
}

std::optional<py::dict<py::str, std::optional<int64_t>>> g_cache;     // _cache
py::str g_scoped;                                                      // _scoped
py::dict<py::str, std::optional<NeModule>> g_modules;                  // _modules
std::optional<py::str> g_product;                                      // _product

// The version of the reading of an export (arg_bytes, describe.returns and .forwards): the Python hashes the source
// text of those three functions, so that a table made by an older reading is not reused; here the reading is named
// by this constant, to be changed with any of them.
const char* const READING = "api_arguments.arg_bytes/describe.returns/describe.forwards:typed-cpp-1";

py::str reading_() {
    // (and by the folder of binaries read: a product's folder can be pointed elsewhere -- CB_CORPUS_ROOT at Setup's
    // MINI.CAB, whose USER and KRNL386 are other files than Windows' own -- and the sizes read from one must not be
    // handed to the other under the same name)
    py::str src = py::str(READING) + py::os::path::normcase(py::os::path::abspath(base::corpus::root()));
    return py::hashlib::sha1(src.encode("utf-8")).hexdigest().slice(0, 10);
}

// One table per product: USER.171 is not the same function in every Windows -- and per way of reading it.
py::str scope_() { return py::percent(py::str("%s:%s"), base::corpus::current(), reading_()); }

py::dict<py::str, std::optional<int64_t>>& load16_() {
    py::str current = base::corpus::current();
    if (!g_product || *g_product != current) {                 // another product was selected
        g_product = current;
        g_cache = std::nullopt;
        g_modules.clear();
    }
    if (!g_cache) {
        g_scoped = scope_() + py::str("|");
        py::dict<py::str, std::optional<int64_t>> t;
        for (const auto& kv : knowledge::store::open()->table(py::str("win16"), py::str("argument_bytes")).items()) {
            const py::str& k = std::get<0>(kv);
            if (k.startswith(g_scoped)) {
                const Value& v = std::get<1>(kv);
                t.set(k.slice(g_scoped.size(), k.size()), std::holds_alternative<Json>(v) ? int_of(std::get<Json>(v)) : std::nullopt);
            }
        }
        g_cache = t;
    }
    return *g_cache;
}

// The sizes of this product and reading, as the knowledge base has them.
class OldWin16Memory final : public aa::Win16Memory {
public:
    std::optional<aa::Known> remembered(std::string_view key) override {
        const py::str k = to_py(key);
        py::dict<py::str, std::optional<int64_t>>& cache = load16_();
        if (!cache.contains(k)) return std::nullopt;
        knowledge::reads::record(py::str("win16"), py::str("argument_bytes"), g_scoped + k);
        return aa::Known{narrow(cache.at(k))};
    }
    std::map<std::string, std::optional<std::uint32_t>> table() override {
        std::map<std::string, std::optional<std::uint32_t>> out;
        for (const auto& kv : load16_().items()) out[from_py(std::get<0>(kv))] = narrow(std::get<1>(kv));
        return out;
    }
    void remember(std::string_view key, std::optional<std::uint32_t> bytes, std::string_view source) override {
        const py::str k = to_py(key);
        load16_().set(k, widen(bytes));
        knowledge::store::open()->put(py::str("win16"), py::str("argument_bytes"), g_scoped + k, Value(json_of(widen(bytes))), to_py(source));
        knowledge::reads::record(py::str("win16"), py::str("argument_bytes"), g_scoped + k);
    }
};

// How the exports of the corpus's 16-bit modules return, read by describe.
class OldWin16Exports final : public aa::Win16Exports {
public:
    std::optional<Found> find(std::string_view module, std::uint32_t ordinal) override {
        std::optional<NeModule> mod = _module(to_py(module));
        if (!mod) return std::nullopt;
        const formats::ne::NEFile& nef = std::get<0>(*mod);
        const auto& entries = std::get<1>(*mod);
        const auto& segs = std::get<2>(*mod);
        Found found;
        found.file = from_py(nef.path);
        std::optional<formats::ne::Entry> e = entries.get(ordinal);
        std::optional<formats::ne::Segment> s;
        if (e && e->seg) s = segs.get(*e->seg);
        if (s && s->kind == py::str("CODE") && s->file_off) {
            py::bytes code = nef.data.slice(s->file_off, s->file_off + s->length);
            // (a thunk that puts a word of its own under the return address and goes on: what its callers push
            // is what the body removes less what it added -- Win95 USER.471, lstrcmpi, read as its body's `retf
            // 0Ah` gave the C two arguments too many)
            decompile::describe::Description own{py::str(""), {}, {}, {}};
            for (const auto& kv : segs.items()) {
                const formats::ne::Segment& g = std::get<1>(kv);
                if (g.kind == py::str("CODE") && g.file_off)
                    own.segment_bytes.set(std::get<0>(kv), nef.data.slice(g.file_off, g.file_off + g.length));
            }
            std::optional<int64_t> fwd = decompile::describe::forwards(own, *e->seg, e->offset.value_or(0), py::str("far"));
            auto [kind, pop] = decompile::describe::returns(code, e->offset.value_or(0));
            aa::ExportReturn r;
            r.forwarded = narrow(fwd);
            r.far_return = kind && *kind == py::str("far");
            r.pop = pop;
            found.what = r;
        }
        return found;
    }
};

}  // namespace

py::bytes sizes16() {
    // The table of sizes found so far for this product and reading, as bytes (sorted JSON): what a cache of the
    // checker's verdicts is keyed by, since the checker answers calls by it.
    OldWin16Memory memory;
    OldWin16Exports exports;
    return py::bytes(aa::Win16Arguments(memory, exports).table_text());
}

std::optional<NeModule> _module(const py::str& name_in) {
    // (NEFile, {ordinal: entry}, {segment: segment}) of the module that exports under `name`, or None.
    py::str name = name_in.upper();
    if (!g_modules.contains(name)) {
        std::optional<NeModule> found;
        py::list<py::str> cands;
        if (files_().contains(name)) cands.extend(files_().at(name));
        for (const char* e : {".dll", ".exe", ".drv"}) cands.append(name.lower() + py::str(e));
        for (const py::str& cand : cands) {
            std::optional<py::str> p = base::corpus::retail(cand);
            std::optional<py::str> lane = (p && !p->empty()) ? base::corpus::lane(*p) : std::nullopt;
            if (lane && *lane == py::str("ne")) {
                try {
                    formats::ne::NEFile nef(*p);
                    py::dict<int64_t, formats::ne::Entry> entries;
                    for (const auto& e : nef.entries())
                        if (e.kind == py::str("FIXED") || e.kind == py::str("MOVEABLE")) entries.set(e.ordinal, e);
                    py::dict<int64_t, formats::ne::Segment> segs;
                    for (const auto& s : nef.segments()) segs.set(s.idx, s);
                    found = NeModule(std::move(nef), entries, segs);
                    break;
                } catch (const std::exception&) {
                }
            }
        }
        g_modules.set(name, found);
    }
    return g_modules.at(name);
}

std::optional<int64_t> arg_bytes(const py::str& module, int64_t ordinal) {
    // Bytes of arguments the export removes on return; None when unknown.
    if (ordinal < 0 || ordinal > 0xFFFFFFFFLL) return std::nullopt;
    OldWin16Memory memory;
    OldWin16Exports exports;
    return widen(aa::Win16Arguments(memory, exports).arg_bytes(from_py(module), static_cast<std::uint32_t>(ordinal)));
}

std::optional<int64_t> of(const std::optional<py::str>& api) {
    // arg_bytes for a name as the emulator reports it: 'USER.@171' or 'USER_171'.
    OldWin16Memory memory;
    OldWin16Exports exports;
    return widen(aa::Win16Arguments(memory, exports).of(api ? from_py(*api) : std::string()));
}

}  // namespace codebreaker::symbols::api_arguments
