// codebreaker/symbols/api_arguments.cpp -- typed C++ port of codebreaker.symbols.api_arguments (see api_arguments.hpp).

#include "codebreaker/symbols/api_arguments.hpp"

#include <algorithm>
#include <mutex>
#include <set>

#include "codebreaker/base/corpus.hpp"
#include "codebreaker/decompile/describe.hpp"
#include "codebreaker/engine/facts.hpp"
#include "codebreaker/knowledge/reads.hpp"
#include "codebreaker/knowledge/sources.hpp"
#include "codebreaker/knowledge/store.hpp"
#include "codebreaker/symbols/x86.hpp"
#include "py_json.hpp"
#include "py_stdlib.hpp"

namespace codebreaker::symbols::api_arguments {

namespace {

using engine::facts::Json;
using engine::facts::Value;

const py::re::Pattern IAT("dword ptr \\[(0x[0-9a-f]+)\\]", 0);      // _IAT
const py::re::Pattern ESP_NUMBER("esp, (-?(?:0x)?[0-9a-f]+)", 0);
const py::re::Pattern ESP_EBP("esp, \\[ebp( [-+] (?:0x)?[0-9a-f]+)?\\]", 0);
const py::re::Pattern API_NAME("([A-Za-z0-9_]+?)[._]@?(\\d+)", 0);

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

// int(text, 0) as the Python parses a capstone operand (ValueError when it is not a number)
int64_t int0(const py::str& s) { return py::int_parse(s, 0); }

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
    md = std::make_shared<int>(32);   // platforms.decoder(32): symbols::x86 holds the real handle, per thread
    std::tuple<int64_t, int64_t> ed = pe->datadirs.empty() ? std::tuple<int64_t, int64_t>{0, 0} : pe->datadirs[0];
    exp_lo = std::get<0>(ed);
    exp_hi = std::get<0>(ed) + std::get<1>(ed);
}

py::bytes _Mod::bytes_at(int64_t rva, int64_t n) const {
    std::optional<int64_t> off = pe->_rva_to_off(rva);
    return off ? pe->data.slice(*off, *off + n) : py::bytes();
}

std::optional<py::str> _Mod::forwarder(int64_t rva) const {
    if (!(exp_lo <= rva && rva < exp_hi)) return std::nullopt;
    py::bytes raw_b = bytes_at(rva, 128);
    const std::string& r = raw_b.raw();
    py::str raw = py::bytes(r.substr(0, r.find('\0'))).decode("latin1", "replace");
    if (raw.contains(py::str("."))) return raw;
    return std::nullopt;
}

// ---- the DLL that exports a function -----------------------------------------------------------------------------

py::str _stem(const py::str& dll) {
    py::str s = py::os::path::basename(dll).upper();
    for (const py::str& e : _EXTS) {
        if (s.endswith(e.upper())) return s.slice(0, s.size() - e.size());
    }
    return s;
}

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
    // Stack bytes removed by the function at `rva`, or None.
    //
    // Every path from the entry is walked with the stack depth tracked, so the answer is right for
    // the hand-written wrappers Windows 95 is full of as well as for compiled code:
    //
    //     FindWindowA:  pop ecx / push 0 / push 0 / push ecx / jmp FindWindowExA     -> 16 - 8 = 8
    //
    // A `ret N` reached with D bytes still pushed means the function itself took N - D. Calls are
    // resolved the same way (the callee's own answer is what it pops). Paths whose depth is lost
    // (an indirect call) still report their `ret N`, but only count if no exact path was found.
    py::dict<int64_t, std::optional<int64_t>>& memo = m._memo;
    if (memo.contains(rva)) return memo.at(rva);
    memo.set(rva, std::nullopt);                    // recursion guard
    std::set<int64_t> thunk, strong, weak, bycl;
    // (rva, depth, depth at `mov ebp, esp`, cl, depth is exact, it was exact at `mov ebp, esp`)
    struct Work {
        int64_t at;
        std::optional<int64_t> d, dbp, cl;
        bool exact, xbp;
    };
    std::vector<Work> work{{rva, 0, std::nullopt, std::nullopt, true, true}};
    std::set<int64_t> seen;
    int64_t budget = _MAX_INSNS;
    while (!work.empty() && budget > 0 && strong.size() < 4) {
        Work w = work.back();
        work.pop_back();
        int64_t at = w.at;
        std::optional<int64_t> d = w.d, dbp = w.dbp, cl = w.cl;
        bool exact = w.exact, xbp = w.xbp;
        while (budget > 0) {
            if (seen.count(at)) break;
            py::bytes code = m.bytes_at(at, 16);
            std::optional<x86::Lite> ins;
            if (!code.empty()) ins = x86::first_lite(32, code.raw(), static_cast<int64_t>(m.base) + at);
            if (!ins) break;
            seen.insert(at);
            budget -= 1;
            int64_t size = ins->size;
            py::str mn(ins->mnemonic), ops(ins->op_str);
            int64_t nxt = at + size;
            if (mn == py::str("ret")) {
                int64_t n = !ops.empty() ? int0(ops) : 0;
                // Depth that came through a call is only as good as the callee's answer: compiled
                // code returns with the stack balanced, so trust `ret N` itself there.
                if (!d || (*d != 0 && !exact)) {
                    weak.insert(n);
                } else if (n - *d >= 0) {
                    strong.insert(n - *d);
                }
                break;
            }
            if (mn == py::str("retf") || mn == py::str("int3") || mn == py::str("hlt") || mn == py::str("iretd")) break;
            if (mn == py::str("jmp")) {
                std::optional<py::re::Match> mm = IAT.fullmatch(ops);
                if (mm) {                                               // tail call through an import
                    auto imp = m.imports.get(static_cast<uint64_t>(py::int_parse(mm->group(1), 16)));
                    std::optional<int64_t> r;
                    if (imp && hops > 0) r = _resolve(std::get<0>(*imp), std::get<1>(*imp), hops - 1);
                    if (r) {
                        (!d ? weak : strong).insert(*r - d.value_or(0));
                    } else if (cl && d && *d == 0 && exact) {
                        bycl.insert(4 * *cl);
                    }
                    break;
                }
                try {
                    at = int0(ops) - static_cast<int64_t>(m.base);
                    continue;
                } catch (const py::ValueError&) {
                    if (cl && d && *d == 0 && exact) bycl.insert(4 * *cl);      // `mov cl, N / jmp [common]`
                    break;
                }
            }
            if (mn.slice(0, 1) == py::str("j") || mn.startswith(py::str("loop"))) {   // conditional: walk both ways
                try {
                    work.push_back({int0(ops) - static_cast<int64_t>(m.base), d, dbp, cl, exact, xbp});
                } catch (const py::ValueError&) {
                }
            } else if (mn == py::str("call")) {
                cl = std::nullopt;
                exact = false;
                std::optional<int64_t> r;
                std::optional<py::re::Match> mm = IAT.fullmatch(ops);
                if (mm) {
                    auto imp = m.imports.get(static_cast<uint64_t>(py::int_parse(mm->group(1), 16)));
                    if (imp && hops > 0) r = _resolve(std::get<0>(*imp), std::get<1>(*imp), hops - 1);
                } else {
                    try {
                        r = hops > 0 ? _scan(m, int0(ops) - static_cast<int64_t>(m.base), hops - 1) : std::nullopt;
                    } catch (const py::ValueError&) {
                        r = std::nullopt;
                    }
                }
                d = (d && r) ? std::optional<int64_t>(*d - *r) : std::nullopt;
            } else if (mn == py::str("push")) {
                d = d ? std::optional<int64_t>(*d + 4) : std::nullopt;
            } else if (mn == py::str("pop")) {
                d = (!d || ops == py::str("esp")) ? std::nullopt : std::optional<int64_t>(*d - 4);
            } else if (mn == py::str("pushal")) {
                d = d ? std::optional<int64_t>(*d + 32) : std::nullopt;
            } else if (mn == py::str("popal")) {
                d = d ? std::optional<int64_t>(*d - 32) : std::nullopt;
            } else if (mn == py::str("pushfd") || mn == py::str("popfd")) {
                d = d ? std::optional<int64_t>(*d + (mn == py::str("pushfd") ? 4 : -4)) : std::nullopt;
            } else if (mn == py::str("leave")) {            // the frame pointer restores a known depth
                d = dbp ? std::optional<int64_t>(*dbp - 4) : std::nullopt;
                exact = xbp;
            } else if (mn == py::str("mov") && ops == py::str("ebp, esp")) {
                dbp = d;
                xbp = exact;
            } else if (mn == py::str("mov") && ops == py::str("esp, ebp")) {
                d = dbp;
                exact = xbp;
            } else if (mn == py::str("mov") && ops.startswith(py::str("edx, 0x")) && cl) {
                // The thunk compiler's 32->16 stub: `mov cl, index / mov edx, argc<<16|flags /
                // jmp common`. The argument count is in the instruction.
                if (d && exact) thunk.insert(4 * (py::int_parse(ops.slice(5, ops.size()), 16) >> 16) - *d);
                break;
            } else if (mn == py::str("mov") && ops.startswith(py::str("cl, "))) {
                try {
                    cl = int0(ops.slice(4, ops.size()));
                } catch (const py::ValueError&) {
                    cl = std::nullopt;
                }
            } else if (ops.startswith(py::str("esp, ")) || ops == py::str("esp")) {
                std::optional<py::re::Match> mm = ESP_NUMBER.fullmatch(ops);
                if ((mn == py::str("sub") || mn == py::str("add")) && mm && d) {
                    *d += int0(mm->group(1)) * (mn == py::str("sub") ? 1 : -1);
                } else {
                    mm = ESP_EBP.fullmatch(ops);
                    if (mn == py::str("lea") && mm && dbp) {
                        std::optional<py::str> g = mm->group_opt(1);
                        d = *dbp - int0((g ? *g : py::str("0")).replace(py::str(" "), py::str("")));
                        exact = xbp;
                    } else {
                        d = std::nullopt;
                    }
                }
            }
            at = nxt;
        }
    }
    for (const std::set<int64_t>* cand0 : {&thunk, &strong, &weak, &bycl}) {
        std::set<int64_t> cand;
        for (int64_t c : *cand0)
            if (c >= 0 && c % 4 == 0) cand.insert(c);
        if (!cand.empty()) {
            memo.set(rva, cand.size() == 1 ? std::optional<int64_t>(*cand.begin()) : std::nullopt);
            break;
        }
    }
    return memo.at(rva);
}

std::optional<int64_t> _resolve(const py::str& dll, const py::str& name, int64_t hops) {
    std::shared_ptr<_Mod> m = _mod(_stem(dll));
    if (!m || name.empty()) return std::nullopt;
    std::optional<int64_t> rva;
    if (name.startswith(py::str("#"))) {
        try {
            rva = m->ords.get(py::int_parse(name.slice(1, name.size()), 10));
        } catch (const py::ValueError&) {
            rva = std::nullopt;
        }
    } else {
        rva = m->exports.get(name);
    }
    if (!rva || *rva == 0) return std::nullopt;
    std::optional<py::str> fwd = m->forwarder(*rva);
    if (fwd) {
        if (hops <= 0) return std::nullopt;
        auto [d, sep, n] = fwd->partition(py::str("."));
        return _resolve(d, n, hops - 1);
    }
    try {
        return _scan(*m, *rva, hops);
    } catch (const std::exception&) {
        return std::nullopt;
    }
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

}  // namespace

py::dict<py::str, knowledge::sources::Json> _knowledge_table(const py::str& platform, const py::str& kind) {
    // knowledge._table(platform, kind): an authored kind is loaded from its file first when the file changed
    const auto& authored = knowledge::sources::AUTHORED();
    if (authored.contains(std::make_tuple(platform, kind))) return knowledge::sources::authored(platform, kind);
    py::dict<py::str, knowledge::sources::Json> out;
    for (const auto& kv : knowledge::store::open()->table(platform, kind).items()) {
        const Value& v = std::get<1>(kv);
        if (std::holds_alternative<Json>(v)) out.set(std::get<0>(kv), std::get<Json>(v));
    }
    return out;
}

std::optional<int64_t> header_bytes(const py::str& name) {
    // Argument bytes for `name` as the Win32 headers declare it, or None when the headers do not settle it (no
    // prototype; variadic; caller-cleans; a wide by-value parameter). Read from win32_arguments.json beside
    // this file (tools/make_api_table.py regenerates it), as the knowledge base has it.
    if (!_TABLE) {
        py::dict<py::str, std::optional<int64_t>> t;
        for (const auto& kv : _knowledge_table(py::str("win32"), py::str("argument_bytes.header")).items())
            t.set(std::get<0>(kv), int_of(std::get<1>(kv)));
        _TABLE = t;
    }
    knowledge::reads::record(py::str("win32"), py::str("argument_bytes.header"), name);       // (knowledge/reads.py)
    return _TABLE->get(name).value_or(std::nullopt);
}

std::optional<int64_t> stack_bytes(const py::str& dll, const py::str& name) {
    // Two sources, checked against each other over eight system DLLs (they agree on 1,669 of 1,695
    // functions): the Win32 header prototype, and the exporting DLL's own code. The header wins
    // where it applies -- the disagreements were all the code scan misreading a hand-written
    // wrapper -- and the scan covers what the headers do not: undocumented functions, and
    // functions taking a structure by value.
    std::optional<int64_t> h = header_bytes(name);
    if (h) return h;
    py::str stem = _stem(dll);
    py::dict<py::str, std::optional<int64_t>> c = load_(stem);
    if (!c.contains(name)) {
        c.set(name, _resolve(stem, name, _MAX_HOPS));
        save_(stem, name);
    }
    knowledge::reads::record(py::str("win32"), py::str("argument_bytes.code"), stem + py::str(".") + name);
    return c.at(name);
}

std::optional<int64_t> argc(const py::str& dll, const py::str& name) {
    std::optional<int64_t> b = stack_bytes(dll, name);
    if (!b || *b % 4 != 0) return std::nullopt;
    return *b / 4;
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

}  // namespace

py::bytes sizes16() {
    // The table of sizes found so far for this product and reading, as bytes (sorted JSON): what a cache of the
    // checker's verdicts is keyed by, since the checker answers calls by it.
    py::dict<py::str, std::optional<int64_t>>& t = load16_();
    std::vector<py::str> keys;
    for (const py::str& k : t.keys()) keys.push_back(k);
    std::sort(keys.begin(), keys.end(), [](const py::str& a, const py::str& b) { return a.u32() < b.u32(); });
    std::string out = "{";
    bool first = true;
    for (const py::str& k : keys) {
        if (!first) out += ", ";
        first = false;
        py::json::put(out, k);
        out += ": ";
        std::optional<int64_t> v = t.at(k);
        if (v) out += std::to_string(*v);
        else out += "null";
    }
    out += "}";
    return py::bytes(out);
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
    py::str key = py::percent(py::str("%s.%d"), module.upper(), ordinal);
    py::dict<py::str, std::optional<int64_t>>& cache = load16_();
    if (!cache.contains(key)) {
        std::optional<int64_t> value;
        std::optional<NeModule> mod = _module(module);
        if (mod) {
            const formats::ne::NEFile& nef = std::get<0>(*mod);
            const auto& entries = std::get<1>(*mod);
            const auto& segs = std::get<2>(*mod);
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
                if (fwd) value = fwd;
                else if (kind && *kind == py::str("far") && pop >= 0) value = pop;
            }
        }
        cache.set(key, value);
        knowledge::store::open()->put(py::str("win16"), py::str("argument_bytes"), g_scoped + key, Value(json_of(value)),
                                      py::percent(py::str("%s's code"),
                                                  mod ? py::os::path::basename(std::get<0>(*mod).path).lower() : module.upper()));
    }
    knowledge::reads::record(py::str("win16"), py::str("argument_bytes"), g_scoped + key);
    return cache.at(key);
}

std::optional<int64_t> of(const std::optional<py::str>& api) {
    // arg_bytes for a name as the emulator reports it: 'USER.@171' or 'USER_171'.
    std::optional<py::re::Match> m = API_NAME.fullmatch(api ? *api : py::str(""));
    if (!m) return std::nullopt;
    return arg_bytes(m->group(1), py::int_parse(m->group(2), 10));
}

}  // namespace codebreaker::symbols::api_arguments
