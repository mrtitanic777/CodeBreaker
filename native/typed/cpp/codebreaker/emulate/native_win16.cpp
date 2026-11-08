// codebreaker/emulate/native_win16.cpp -- C++ port of codebreaker.emulate.native_win16 (see the header).

#include "codebreaker/emulate/native_win16.hpp"

#include <array>
#include <string>
#include <utility>

// the kernel32 calls _lib needs, declared here: <windows.h> would bring its macros into every unit
#ifndef _WINDOWS_
extern "C" {
__declspec(dllimport) void* __stdcall GetModuleHandleW(const wchar_t*);
__declspec(dllimport) void* __stdcall GetProcAddress(void*, const char*);
}
#endif

namespace codebreaker::emulate::native_win16 {

namespace nt = codebreaker::emulate::native_trace;

const py::list<py::str> _ACCESS = py::list<py::str>{py::str("read"), py::str("write"), py::str("fetch"), py::str("?")};

py::list<std::shared_ptr<Lib>> _READY = py::list<std::shared_ptr<Lib>>{};

namespace {

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

// ---- the closures EVENT(self._wrap(f)) and EVENT(self._wrap_own(on_own)): C entry points per live Run ----
constexpr int kSlots = 16;
std::array<Run*, kSlots> g_runs{};

template <int I, int K> int event_slot(int64_t a, int64_t n) {
    Run* r = g_runs[I];
    if (!r) return K == 3 ? 2 : 1;
    return K == 3 ? r->_own(a, n) : r->_event(K, a, n);
}
template <int K, int... I> constexpr std::array<EVENT, sizeof...(I)> make_kind(std::integer_sequence<int, I...>) {
    return {&event_slot<I, K>...};
}
const std::array<EVENT, kSlots> g_sentinel = make_kind<0>(std::make_integer_sequence<int, kSlots>{});
const std::array<EVENT, kSlots> g_intercept = make_kind<1>(std::make_integer_sequence<int, kSlots>{});
const std::array<EVENT, kSlots> g_intr = make_kind<2>(std::make_integer_sequence<int, kSlots>{});
const std::array<EVENT, kSlots> g_own = make_kind<3>(std::make_integer_sequence<int, kSlots>{});

}  // namespace

std::shared_ptr<Lib> _lib() {
    std::shared_ptr<nt::Lib> base = nt::_load();
    if (!base) {
        return nullptr;
    }
    if (_READY.empty()) {
        auto l = std::make_shared<Lib>();
        void* dll = GetModuleHandleW(wide(py::os::path::abspath(nt::dll_path())).c_str());
        if (!dll) throw py::OSError(py::str("could not find the loaded ") + nt::dll_path());
        entry(dll, "cbt_w16_begin", l->cbt_w16_begin);
        entry(dll, "cbt_w16_attach", l->cbt_w16_attach);
        entry(dll, "cbt_w16_detach", l->cbt_w16_detach);
        entry(dll, "cbt_w16_free", l->cbt_w16_free);
        entry(dll, "cbt_w16_reset", l->cbt_w16_reset);
        entry(dll, "cbt_w16_state", l->cbt_w16_state);
        entry(dll, "cbt_w16_result_get", l->cbt_w16_result_get);
        _READY.append(l);
    }
    return _READY[0];
}

// The 16-bit recorder for this process: the DLL loads here (the Python hooks it replaced are gone;
// require() raises with why_not() when it does not).
bool enabled() {
    return _lib() != nullptr;
}

std::optional<py::str> why_not() {
    return nt::why_not();
}

std::tuple<std::shared_ptr<std::vector<uint64_t>>, int64_t> _u64s(const std::vector<uint64_t>& values) {
    return nt::_u64s(values);
}

py::list<std::vector<uint64_t>> _words(const void* ptr, int64_t n, int64_t count) {
    return nt::_words<uint64_t>(ptr, n, count);
}

// One NE function's native hooks on `uc`. `on_sentinel(addr, n)`, `on_intercept(addr, n)` and
// `on_intr(intno, n)` run the Python event handlers (import_stub / answer / the interrupt body);
// each may raise, and the exception comes back out of trace_function16. `gov` emulator._GOV.
Run::Run(const nt::Engine& uc_, const RunArgs& a) : uc(uc_), gov(a.gov), on_own_(a.on_own) {
    lib = _lib();
    fns_[0] = a.on_sentinel;
    fns_[1] = a.on_intercept;
    fns_[2] = a.on_intr;
    std::tie(keep_inter_, config.nintercept) = _u64s(a.intercept.vec());
    for (int i = 0; i < kSlots && slot_ < 0; i++) {   // (the closures over self)
        if (!g_runs[i]) slot_ = i;
    }
    if (slot_ < 0) throw py::RuntimeError("cbtrace: too many 16-bit runs are alive at once");
    g_runs[slot_] = this;
    _Config& c = config;
    c.uch = uc.uch;
    c.eip_id = static_cast<int32_t>(a.eip_id);
    c.near_fn = a.near_ ? 1 : 0;
    c.fill = a.fill ? 1 : 0;
    c.cap = a.cap;
    c.cs_base = static_cast<uint64_t>(a.cs_base);
    c.near_ret = static_cast<uint64_t>(a.near_ret);
    c.far_ret = static_cast<uint64_t>(a.far_ret);
    c.sent_lo = static_cast<uint64_t>(a.sent_lo);
    c.sent_hi = static_cast<uint64_t>(a.sent_hi);
    c.snap_base = static_cast<uint64_t>(a.snap_base);
    c.gov_n = gov->n;
    c.gov_ceil = gov->ceil;
    c.count = &count_;
    c.intercept = keep_inter_->data();
    c.on_sentinel = g_sentinel[slot_];
    c.on_intercept = g_intercept[slot_];
    c.on_intr = g_intr[slot_];
    c.shadow = a.shadow;
    if (a.own) {
        c.own_lo = static_cast<uint64_t>(std::get<0>(*a.own));
        c.own_hi = static_cast<uint64_t>(std::get<1>(*a.own));
        c.on_own = g_own[slot_];
    }
    if (!lib) {
        g_runs[slot_] = nullptr;
        throw py::AttributeError("'NoneType' object has no attribute 'cbt_w16_begin'");
    }
    handle = lib->cbt_w16_begin(&c);
    if (!handle) {
        g_runs[slot_] = nullptr;
        throw py::RuntimeError("cbtrace: a 16-bit run could not be set up");
    }
    int e = lib->cbt_w16_attach(handle);
    if (e) {
        void* h = handle;   // (the half-made run is freed, as the Python object's __del__ does)
        handle = nullptr;
        g_runs[slot_] = nullptr;
        lib->cbt_w16_free(h);
        throw nt::UcError(e);
    }
}

// _wrap(fn): the callback runs fn(a, n) and returns 0, or 1 when it raised (the first exception is kept).
int Run::_event(int kind, int64_t a, int64_t n) {
    try {
        if (!fns_[kind]) throw py::TypeError("'NoneType' object is not callable");
        fns_[kind](a, n);
        return 0;
    } catch (...) {   // (BaseException)
        if (!exc) {
            exc = std::current_exception();
        }
        return 1;
    }
}

// _wrap_own(fn): 1 when fn(a, n) is true, 0 when not, 2 when it raised.
int Run::_own(int64_t a, int64_t n) {
    try {
        if (!on_own_) throw py::TypeError("'NoneType' object is not callable");
        return on_own_(a, n) ? 1 : 0;
    } catch (...) {   // (BaseException)
        if (!exc) {
            exc = std::current_exception();
        }
        return 2;
    }
}

// A prelude ran: its path, count, reads and writes are dropped (cbt_w16_reset).
void Run::reset() {
    lib->cbt_w16_reset(handle);
}

int64_t Run::count() const {
    return count_;
}

std::exception_ptr Run::raised() {
    lib->cbt_w16_state(handle, state_);
    gov->n = state_[2];
    std::exception_ptr e = exc;
    exc = nullptr;
    return e;
}

void Run::collect(Win16Record& rec) {
    const _Result& r = *lib->cbt_w16_result_get(handle);
    gov->n = r.gov_n;
    rec.insn_count = r.insn_count;
    rec.truncated = r.truncated != 0;
    if (r.returned) {
        rec.returned = true;
    }
    if (r.limit_fault) {
        rec.limit_fault = true;
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
        py::list<nt::MemRead> reads;
        for (const auto& w : _words(r.reads, r.nreads, _MEM_WORDS)) {
            reads.append(nt::MemRead{w[0], w[1], w[2], w[4] ? std::nullopt : std::optional<uint64_t>(w[3]), w[5]});
        }
        rec.mem_reads = reads;
    }
    {
        py::list<nt::MemWrite> writes;
        for (const auto& w : _words(r.writes, r.nwrites, _MEM_WORDS)) {
            writes.append(nt::MemWrite{w[0], w[1], w[2], w[3], w[5], w[7]});
        }
        rec.mem_writes = writes;
    }
    {
        py::list<nt::LazyMap> lazy;
        for (const auto& w : _words(r.lazy, r.nlazy, _LAZY_WORDS)) {
            if (w[3] >= static_cast<uint64_t>(_ACCESS.size())) throw py::IndexError("tuple index out of range");
            lazy.append(nt::LazyMap{w[0], w[1], w[2], _ACCESS[static_cast<int64_t>(w[3])], w[4]});
        }
        rec.lazy_maps = lazy;
    }
    if (r.memops >= 0) {
        rec.memops = r.memops;
    }
    if (r.writes_seen >= 0) {
        rec.writes_seen = r.writes_seen;
    }
    if (r.stuck >= 0) {
        rec._stuck = r.stuck;
    }
}

void Run::detach() {
    if (lib->cbt_w16_detach(handle)) {
        throw py::RuntimeError("cbtrace: a 16-bit hook could not be taken off");
    }
}

Run::~Run() {
    void* h = handle;
    handle = nullptr;
    if (slot_ >= 0 && g_runs[slot_] == this) g_runs[slot_] = nullptr;
    if (h) {
        lib->cbt_w16_free(h);
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

}  // namespace codebreaker::emulate::native_win16
