// codebreaker/emulate/native_trace.cpp -- C++ port of codebreaker.emulate.native_trace (see the header).

#include "codebreaker/emulate/native_trace.hpp"

#include <mutex>
#include "codebreaker/base/capstone_dll.hpp"
#include "codebreaker/emulate/unicorn_uc.hpp"

// the kernel32 calls _load needs, declared here: <windows.h> would bring its macros into every unit
#ifndef _WINDOWS_
extern "C" {
__declspec(dllimport) void* __stdcall LoadLibraryW(const wchar_t*);
__declspec(dllimport) void* __stdcall GetProcAddress(void*, const char*);
}
#endif

namespace codebreaker::emulate::native_trace {

const py::list<py::str> _COMPARES = py::list<py::str>{py::str("cmp"), py::str("test"), py::str("sub"), py::str("and"), py::str("or"),
                                                      py::str("dec"), py::str("inc"), py::str("add"), py::str("xor")};
const py::list<py::str> _ACCESS = py::list<py::str>{py::str("read"), py::str("write"), py::str("fetch"), py::str("?")};

py::list<std::shared_ptr<Lib>> _LIB = py::list<std::shared_ptr<Lib>>{};
py::list<py::str> _WHY = py::list<py::str>{};
std::map<const void*, std::tuple<int32_t, std::shared_ptr<void>>> _STATICS;
py::dict<py::str, int32_t> _SHAS;

namespace {

struct EngineConfig {
    std::optional<py::str> uc_dll, cs_dll;
    py::dict<py::str, int64_t> registers;
};
EngineConfig& engine_config() {
    static EngineConfig c;
    return c;
}

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

template <class F> void entry(void* dll, const char* name, F& slot) {
    void* p = GetProcAddress(dll, name);
    if (!p) throw py::AttributeError(std::string("function '") + name + "' not found");
    slot = reinterpret_cast<F>(p);
}

// the run whose start() is in progress: whom the DLL's callback (which carries no context) answers for
thread_local Run* g_running = nullptr;

int resolver_trampoline(int32_t index, int64_t* out, int32_t cap) {
    if (!g_running) return 1;
    return g_running->_resolve(index, out, cap);
}

}  // namespace

void set_engine(const py::str& uc_dll, const py::str& cs_dll, const py::dict<py::str, int64_t>& registers) {
    EngineConfig& c = engine_config();
    c.uc_dll = uc_dll;
    c.cs_dll = cs_dll;
    c.registers = registers;
}

// Where cbtrace.dll is: the `tracer_bin` setting (default native/tracer/build/bin).
py::str dll_path() {
    return py::os::path::join(py::settings::get_str(py::str("tracer_bin")), py::str("cbtrace.dll"));
}

std::shared_ptr<Lib> _load() {
    static std::recursive_mutex init;   // (first use may be on several gate threads at once)
    std::lock_guard<std::recursive_mutex> hold(init);
    if (!_LIB.empty()) {
        return _LIB[0];
    }
    std::shared_ptr<Lib> lib;
    try {
        py::str path = dll_path();
        if (!py::os::path::isfile(path)) {
            throw py::OSError(py::str("not built: ") + path + py::str(" (native/tracer/CMakeLists.txt)"));
        }
        EngineConfig& cfg = engine_config();   // (the DLLs Python loaded: the unicorn and capstone packages')
        if (!cfg.uc_dll || !cfg.cs_dll) {
            // nobody called set_engine(): find the packages' DLLs as `import unicorn` / `import capstone` would, and the
            // register table is unicorn.x86_const's (unicorn_uc carries it)
            cfg.uc_dll = base::capstone_dll::find_unicorn();
            cfg.cs_dll = base::capstone_dll::find();
            if (cfg.registers.empty()) cfg.registers = unicorn_uc::x86_register_table();
        }
        py::str uc_dll = *cfg.uc_dll, cs_dll = *cfg.cs_dll;
        auto l = std::make_shared<Lib>();
        std::wstring wpath = wide(py::os::path::abspath(path));
        void* dll = LoadLibraryW(wpath.c_str());
        if (!dll) throw py::OSError(py::str("could not load ") + path);
        entry(dll, "cbt_version", l->cbt_version);
        if (l->cbt_version() != 1) {
            throw py::OSError("cbtrace.dll is another version: rebuild native/tracer");
        }
        entry(dll, "cbt_init", l->cbt_init);
        char err[256] = {0};
        std::wstring wuc = wide(py::os::path::abspath(uc_dll)), wcs = wide(py::os::path::abspath(cs_dll));
        if (l->cbt_init(wuc.c_str(), wcs.c_str(), err, 256)) {
            throw py::OSError(py::bytes(err).decode("ascii", "replace"));
        }
        entry(dll, "cbt_register", l->cbt_register);
        for (const auto& kv : cfg.registers.items()) {
            const std::string& name = std::get<0>(kv).utf8();
            const std::string prefix = "UC_X86_REG_";
            if (name.compare(0, prefix.size(), prefix) == 0) {
                l->cbt_register(name.substr(prefix.size()).c_str(), static_cast<int32_t>(std::get<1>(kv)));
            }
        }
        entry(dll, "cbt_static", l->cbt_static);
        entry(dll, "cbt_intern", l->cbt_intern);
        entry(dll, "cbt_begin", l->cbt_begin);
        entry(dll, "cbt_attach", l->cbt_attach);
        entry(dll, "cbt_attach_code", l->cbt_attach_code);
        entry(dll, "cbt_switched", l->cbt_switched);
        entry(dll, "cbt_detach", l->cbt_detach);
        entry(dll, "cbt_restore", l->cbt_restore);
        entry(dll, "cbt_settle", l->cbt_settle);
        entry(dll, "cbt_free", l->cbt_free);
        entry(dll, "cbt_state", l->cbt_state);
        entry(dll, "cbt_result_get", l->cbt_result_get);
        lib = l;
    } catch (const py::Exception& e) {   // (no DLL: the Python hooks run)
        _WHY.append(py::str(e.type()) + py::str(": ") + py::str(e.what()));
        lib = nullptr;
    }
    _LIB.append(lib);
    return lib;
}

bool enabled() {
    return _load() != nullptr;
}

std::optional<py::str> why_not() {
    return !_WHY.empty() ? std::optional<py::str>(_WHY[0]) : std::nullopt;
}

int32_t _static_id(const Lib& lib, uint64_t lo, uint64_t hi, const py::bytes& image, const std::shared_ptr<void>& table) {
    auto it = _STATICS.find(table.get());
    if (it == _STATICS.end() || std::get<1>(it->second) != table) {
        int32_t id = lib.cbt_static(lo, hi, reinterpret_cast<const uint8_t*>(image.raw().data()));
        _STATICS[table.get()] = std::make_tuple(id, table);
        return id;
    }
    return std::get<0>(it->second);
}

int32_t _sha_id(const Lib& lib, const py::str& sha) {
    auto got = _SHAS.get(sha);
    if (!got) {
        got = lib.cbt_intern(sha.utf8().c_str());
        _SHAS.set(sha, *got);
    }
    return *got;
}

std::tuple<std::shared_ptr<std::vector<uint64_t>>, int64_t> _u64s(const std::vector<uint64_t>& values) {
    auto arr = std::make_shared<std::vector<uint64_t>>(std::max<size_t>(values.size(), 1), 0);
    std::copy(values.begin(), values.end(), arr->begin());
    return std::make_tuple(arr, static_cast<int64_t>(values.size()));
}

Run::Run(const Engine& uc_, const RunArgs& a) : uc(uc_), gov(a.gov), outs(a.outs), sig_resolver(a.sig_resolver) {
    lib = _load();
    if (!lib) throw py::AttributeError("'NoneType' object has no attribute 'cbt_static'");
    int64_t flags = ((a.fast ? _FAST : 0) | (a.lean ? _LEAN : 0) | (a.reads ? _READS : 0) | (a.quiet ? _QUIET : 0) |
                     (a.stop_wild ? _STOP_WILD : 0) | (a.stub ? _STUB : 0) | (a.unique ? _UNIQUE : 0) |
                     (a.branches ? _BRANCHES : 0) | (a.pads ? _PADS : 0) | (a.saved ? _SAVED : 0) | _RESOLVER);
    // the import landing places: kept with the machine (`cache`), made again only when what
    // they were made from (`sentinel_from`: the machine's map and stub_at, by identity) changes
    SentinelCache* cache = !a.sentinel_from.empty() ? a.cache : nullptr;
    std::shared_ptr<std::vector<uint64_t>> sent;
    int64_t nsent = 0;
    bool reuse = cache != nullptr && cache->has && cache->from.size() == a.sentinel_from.size() &&
                 cache->count == a.sentinels.size();
    if (reuse) {
        for (size_t i = 0; i < cache->from.size(); i++) {
            if (cache->from[i] != a.sentinel_from[i]) reuse = false;
        }
    }
    if (reuse) {
        names = cache->names;
        sent = cache->sent;
        nsent = cache->nsent;
    } else {
        names = Names{};
        for (const auto& v : a.sentinels.values()) names.append(v);
        std::tie(sent, nsent) = _u64s(a.sentinels.keys());
        if (cache != nullptr) {
            cache->has = true;
            cache->from = a.sentinel_from;
            cache->count = a.sentinels.size();
            cache->names = names;
            cache->sent = sent;
            cache->nsent = nsent;
        }
    }
    std::vector<int32_t> ids;
    for (const auto& s : a.statics) {
        ids.push_back(_static_id(*lib, std::get<0>(s), std::get<1>(s), std::get<2>(s), std::get<3>(s)));
    }
    keep_statics_ = std::make_shared<std::vector<int32_t>>(std::max<size_t>(ids.size(), 1), 0);
    std::copy(ids.begin(), ids.end(), keep_statics_->begin());
    int64_t ninit, nmapped, npresaved;
    std::tie(keep_init_, ninit) = _u64s(a.init_pages.vec());
    std::tie(keep_mapped_, nmapped) = _u64s(a.mapped.vec());
    std::tie(keep_presaved_, npresaved) = _u64s(a.saved ? *a.saved : std::vector<uint64_t>{});
    keep_sent_ = sent;
    _Config& c = config;
    c.uch = uc.uch;
    c.flags = static_cast<uint32_t>(flags);
    c.sha_id = _sha_id(*lib, a.sha);
    c.insn_cap = a.insn_cap;
    c.stub_ret = a.stub_ret;
    c.stub_cleanup = a.stub_cleanup;
    c.gov_n = gov->n;
    c.gov_ceil = gov->ceil;
    std::tie(c.func_lo, c.func_hi) = a.func;
    std::tie(c.sec_lo, c.sec_hi) = a.sec;
    if (a.pads) {
        c.pads_lo = static_cast<uint64_t>(std::get<0>(*a.pads));
        c.pads_hi = static_cast<uint64_t>(std::get<1>(*a.pads));
    }
    std::tie(c.quiet_lo, c.quiet_hi) = a.quiet ? *a.quiet : std::make_tuple<uint64_t, uint64_t>(0, 0);
    std::tie(c.lean_low_end, c.lean_high_begin) = a.lean_ranges;
    c.statics = keep_statics_->data();
    c.nstatics = static_cast<int32_t>(ids.size());
    c.init_pages = keep_init_->data();
    c.ninit = ninit;
    c.mapped = keep_mapped_->data();
    c.nmapped = nmapped;
    c.presaved = keep_presaved_->data();
    c.npresaved = npresaved;
    c.sentinels = sent->data();
    c.nsentinels = nsent;
    c.resolver = &resolver_trampoline;
    c.shadow = a.shadow;
    handle = lib->cbt_begin(&c);
    if (!handle) {
        throw py::RuntimeError("cbtrace: a run could not be set up");
    }
    int e = lib->cbt_attach(handle);
    if (e) {
        void* h = handle;   // (the half-made run is freed, as the Python object's __del__ does)
        handle = nullptr;
        lib->cbt_free(h);
        throw UcError(e);
    }
}

// An import entered: its argument count (sig_resolver) and what it gives back (_outs).
int Run::_resolve(int32_t index, int64_t* out, int32_t cap) {
    try {
        auto [dll, name] = names[index];
        std::optional<int64_t> argc;
        if (dll == "CALLBACK") {
            argc = 0;
        } else {
            argc = sig_resolver ? sig_resolver(dll, name) : std::nullopt;
        }
        out[0] = !argc ? -1 : *argc;
        out[1] = 0;
        if (argc && *argc != 0 && dll != "CALLBACK") {
            auto all = outs(name);
            int64_t n = all.size();
            int64_t stop = cap >= 0 ? std::min<int64_t>(cap, n) : std::max<int64_t>(0, n + cap);   // [:cap]
            out[1] = stop;
            for (int64_t k = 0; k < stop; k++) {
                auto [i, v] = all[k];
                out[2 + 2 * k] = i;
                out[3 + 2 * k] = !v ? -1 : *v;
            }
        }
        return 0;
    } catch (...) {   // raised where Python's hook would have
        if (!exc) exc = std::current_exception();
        return 1;
    }
}

const int64_t* Run::state() {
    lib->cbt_state(handle, state_);
    gov->n = state_[3];
    return state_;
}

// uc.emu_start, then raise what a Python hook would have raised in it.
void Run::start(const Engine& uc_, int64_t begin, int64_t until, int64_t count) {
    exc = nullptr;
    std::exception_ptr pending;
    Run* before = g_running;
    g_running = this;
    try {
        uc_.emu_start(begin, until, count);
    } catch (...) {
        pending = std::current_exception();
    }
    g_running = before;
    const int64_t* s = state();
    if (pending) std::rethrow_exception(pending);
    if (exc) {
        std::exception_ptr e = exc;
        exc = nullptr;
        std::rethrow_exception(e);
    }
    if (s[0] == 2) {   // a read a hook could not make
        throw UcError(s[1]);
    }
}

bool Run::switched() {
    return lib->cbt_switched(handle) != 0;
}

void Run::attach_code() {
    int e = lib->cbt_attach_code(handle);
    if (e) {
        throw UcError(e);
    }
}

void Run::settle(TraceRecord& rec) {
    lib->cbt_settle(handle);
    rec.truncated = state()[2] != 0;
}

// The record's fields, as trace_function's Python hooks leave them.
void Run::collect(TraceRecord& rec) {
    const _Result& r = *lib->cbt_result_get(handle);
    gov->n = r.gov_n;
    rec.insn_count = r.insn_count;
    rec.truncated = r.truncated != 0;
    if (r.governed) {
        rec.governed = true;
    }
    {
        py::list<uint64_t> path;
        if (r.npath) {
            const uint64_t* p = static_cast<const uint64_t*>(r.path);
            for (int64_t i = 0; i < r.npath; i++) path.append(p[i]);
        }
        rec.path = path;
    }
    {
        py::dict<uint64_t, py::str> mnemonics;
        if (r.nmn) {
            const uint64_t* addrs = static_cast<const uint64_t*>(r.mn_addr);
            py::str text = py::bytes(static_cast<const char*>(r.mn_text), static_cast<size_t>(r.mn_text_len)).decode("ascii", "replace");
            py::list<py::str> texts = py::split(text, py::str(std::string(1, '\0')));
            for (int64_t i = 0; i < std::min<int64_t>(r.nmn, texts.size()); i++) {   // dict(zip(addrs, texts))
                mnemonics.set(addrs[i], texts[i]);
            }
        }
        rec.mnemonics = mnemonics;
    }
    py::list<CallRecord> calls;
    for (const auto& w : _words<int64_t>(r.calls, r.ncalls, _CALL_WORDS)) {
        CallRecord cr;
        std::optional<int64_t> tgt = w[1] ? std::optional<int64_t>(w[2]) : std::nullopt;
        cr.pc = w[0];
        cr.target = tgt;
        cr.indirect = !tgt;
        cr.into_func = w[3] != 0;
        if (w[4] == 1) {
            cr.resolved_target = w[5];
            cr.external = w[6] != 0;
        } else if (w[4] == 2) {
            cr.resolved_target = w[5];
            cr.external = true;
            cr.api = _api(w[7]);
            cr.stubbed = true;
            cr.has_ret_sentinel = true;
            cr.ret_sentinel = w[8] ? std::optional<int64_t>(w[9]) : std::nullopt;
        }
        if (w[10]) {
            cr.args_stack = py::list<int64_t>{w[11], w[12], w[13], w[14], w[15], w[16]};
            cr.ecx = w[17];
            cr.edx = w[18];
        }
        if (w[19]) {
            cr.argc = w[20];
        }
        calls.append(cr);
    }
    rec.calls = calls;
    {
        py::list<MemRead> reads;
        for (const auto& w : _words<uint64_t>(r.reads, r.nreads, _MEM_WORDS)) {
            reads.append(MemRead{w[0], w[1], w[2], w[4] ? std::nullopt : std::optional<uint64_t>(w[3]), w[5]});
        }
        rec.mem_reads = reads;
    }
    if (config.flags & _LEAN) {
        py::list<LeanWrite> writes;
        for (const auto& w : _words<uint64_t>(r.writes, r.nwrites, _LEAN_WORDS)) {
            writes.append(LeanWrite{w[0], w[1], w[2], w[3]});
        }
        rec.mem_writes = writes;
        py::dict<uint64_t, py::bytes> pages0;
        if (r.npages0) {
            const char* data = static_cast<const char*>(r.pages0_data);
            const uint64_t* bases = static_cast<const uint64_t*>(r.pages0);
            for (int64_t i = 0; i < r.npages0; i++) {
                pages0.set(bases[i], py::bytes(data + i * _PAGE, static_cast<size_t>(_PAGE)));
            }
        }
        rec.pages0 = pages0;
    } else {
        py::list<MemWrite> writes;
        for (const auto& w : _words<uint64_t>(r.writes, r.nwrites, _MEM_WORDS)) {
            writes.append(MemWrite{w[0], w[1], w[2], w[3], w[5], w[7]});
        }
        rec.mem_writes = writes;
    }
    {
        py::list<LazyMap> lazy;
        for (const auto& w : _words<uint64_t>(r.lazy, r.nlazy, _LAZY_WORDS)) {
            if (w[3] >= static_cast<uint64_t>(_ACCESS.size())) throw py::IndexError("tuple index out of range");
            lazy.append(LazyMap{w[0], w[1], w[2], _ACCESS[static_cast<int64_t>(w[3])], w[4]});
        }
        rec.lazy_maps = lazy;
    }
    if (r.memops >= 0) {
        rec.memops = r.memops;
    }
    if (r.writes_seen >= 0) {
        rec.writes_seen = r.writes_seen;
    }
    if (r.dirty) {
        rec._dirty = true;
    }
    if (r.stuck >= 0) {
        rec._stuck = r.stuck;
    }
    if (r.bogus_stubs >= 0) {
        rec.bogus_stubs = r.bogus_stubs;
    }
    if (r.ran_into_zeros) {
        rec.ran_into_zeros = true;
    }
    if (r.ndeps) {
        py::list<py::str> deps;
        const int64_t* d = static_cast<const int64_t*>(r.deps);
        for (int64_t i = 0; i < r.ndeps; i++) deps.append(_api(d[i]));
        rec.api_deps = deps;
    }
    if (r.api_calls_set) {
        py::list<std::tuple<py::str, std::optional<py::list<int64_t>>>> seq;
        py::list<int64_t> words;
        if (r.napi_words) {
            const int64_t* p = static_cast<const int64_t*>(r.api_calls);
            for (int64_t i = 0; i < r.napi_words; i++) words.append(p[i]);
        }
        int64_t i = 0;
        while (i < words.size()) {
            int64_t code = words[i], argc = words[i + 1];   // (IndexError on a word missing)
            if (argc < 0) {
                seq.append(std::make_tuple(_api(code), std::optional<py::list<int64_t>>()));
                i += 2;
            } else {
                py::list<int64_t> args;   // words[i + 2:i + 2 + argc]
                for (int64_t k = i + 2; k < std::min<int64_t>(i + 2 + argc, words.size()); k++) args.append(words[k]);
                seq.append(std::make_tuple(_api(code), std::optional<py::list<int64_t>>(args)));
                i += 2 + argc;
            }
        }
        rec.api_calls = seq;
    }
    if (r.stubbed >= 0) {
        rec.stubbed_externals = r.stubbed;
    }
    if (r.nvxd) {
        py::list<uint64_t> vxd;
        const uint64_t* p = static_cast<const uint64_t*>(r.vxd);
        for (int64_t i = 0; i < r.nvxd; i++) vxd.append(p[i]);
        rec.vxd_services = vxd;
    }
    if (config.flags & _BRANCHES) {
        py::list<BranchEntry> log;
        for (const auto& w : _words<uint64_t>(r.branches, r.nbranches, _BRANCH_WORDS)) {
            std::optional<Compare> cmp;
            if (w[2]) {
                if (w[3] >= static_cast<uint64_t>(_COMPARES.size())) throw py::IndexError("tuple index out of range");
                cmp = Compare{_COMPARES[static_cast<int64_t>(w[3])], w[4], w[5], w[6],
                              w[7] ? std::optional<uint64_t>(w[8]) : std::nullopt,
                              w[9] ? std::optional<uint64_t>(w[10]) : std::nullopt};
            }
            log.append(BranchEntry{w[0], w[1] != 0, cmp});
        }
        rec.branches = log;
    }
    lean_seen = r.lean_seen;
}

py::str Run::_api(int64_t code) {
    if (code < 0) {
        return py::str("CALLBACK!") + py::to_str(-code - 1);
    }
    auto [dll, name] = names[code];
    return dll + py::str("!") + name;
}

void Run::detach() {
    if (lib->cbt_detach(handle)) {
        throw py::RuntimeError("cbtrace: a hook could not be taken off");
    }
}

void Run::restore() {
    int e = lib->cbt_restore(handle);
    if (e) {
        throw UcError(e);
    }
}

Run::~Run() {
    void* h = handle;
    handle = nullptr;
    if (h) {
        lib->cbt_free(h);
    }
}

// The recorder is C++ only (the Python hooks it replaced are gone): raise when the DLL cannot
// be used here, saying why.
void require() {
    if (!enabled()) {
        std::optional<py::str> why = why_not();
        throw py::RuntimeError(py::str("the run recorder is native (native/tracer, cbtrace.dll): ") + (why ? *why : py::str("None")));
    }
}

}  // namespace codebreaker::emulate::native_trace
