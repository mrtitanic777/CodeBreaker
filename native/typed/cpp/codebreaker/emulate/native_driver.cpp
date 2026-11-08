// codebreaker/emulate/native_driver.cpp -- C++ port of codebreaker.emulate.native_driver (see the header).

#include "codebreaker/emulate/native_driver.hpp"

#include <algorithm>
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

namespace codebreaker::emulate::native_driver {

namespace nt = codebreaker::emulate::native_trace;

py::list<std::shared_ptr<Lib>> _READY = py::list<std::shared_ptr<Lib>>{};

const py::dict<int64_t, py::str> _EXITS = py::dict<int64_t, py::str>{
    {1, py::str("wild")}, {2, py::str("exception: service number unreadable")}};

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

// ---- the closure ANSWERS(self._resolve): one C entry point per live Run -------------------------
constexpr int kSlots = 32;
std::array<Run*, kSlots> g_runs{};

template <int I> int answer_slot(int32_t device, int32_t service, int32_t* out) {
    Run* r = g_runs[I];
    if (!r) return 1;
    return r->_resolve(device, service, out);
}
template <int... I> constexpr std::array<ANSWERS, sizeof...(I)> make_slots(std::integer_sequence<int, I...>) {
    return {&answer_slot<I>...};
}
const std::array<ANSWERS, kSlots> g_entry = make_slots(std::make_integer_sequence<int, kSlots>{});

// "%02x" % v, as Python formats an int (a negative one with a minus sign)
py::str hex02(int64_t v) {
    static const char* digits = "0123456789abcdef";
    bool neg = v < 0;
    uint64_t u = neg ? 0 - static_cast<uint64_t>(v) : static_cast<uint64_t>(v);
    std::string s;
    do {
        s.insert(s.begin(), digits[u & 15]);
        u >>= 4;
    } while (u);
    if (neg) s.insert(s.begin(), '-');   // (the width of 2 counts the sign: "%02x" % -1 is "-1")
    else if (s.size() < 2) s.insert(s.begin(), '0');
    return py::str(s);
}

}  // namespace

// The cbtrace.dll native_trace loaded, with the driver entry points bound, or None.
std::shared_ptr<Lib> _lib() {
    std::shared_ptr<nt::Lib> base = nt::_load();
    if (!base) {
        return nullptr;
    }
    if (_READY.empty()) {
        auto l = std::make_shared<Lib>();
        void* dll = GetModuleHandleW(wide(py::os::path::abspath(nt::dll_path())).c_str());
        if (!dll) throw py::OSError(py::str("could not find the loaded ") + nt::dll_path());
        entry(dll, "cbt_drv_begin", l->cbt_drv_begin);
        entry(dll, "cbt_drv_attach", l->cbt_drv_attach);
        entry(dll, "cbt_drv_detach", l->cbt_drv_detach);
        entry(dll, "cbt_drv_free", l->cbt_drv_free);
        entry(dll, "cbt_drv_state", l->cbt_drv_state);
        entry(dll, "cbt_drv_result_get", l->cbt_drv_result_get);
        _READY.append(l);
    }
    return _READY[0];
}

// The driver recorder for this process: the DLL loads here (the Python hooks it replaced are gone;
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

// One driver run's native hooks on `uc`. `spans` the image's [(lo, hi)]; `scratch` the
// scratch region (lo, hi); `reg_ids` the Unicorn ids of eax,ebx,ecx,edx,esi,edi,ebp,esp,eip,
// eflags; `answers(device, service)` -> the place in REGISTERS the service answers in (or -1).
// `gov` emulator._GOV.
Run::Run(const nt::Engine& uc_, const RunArgs& a) : uc(uc_), gov(a.gov), answers_fn(a.answers) {
    lib = _lib();
    std::vector<uint64_t> flat;
    for (const auto& s : a.spans.vec()) {
        flat.push_back(static_cast<uint64_t>(std::get<0>(s)));
        flat.push_back(static_cast<uint64_t>(std::get<1>(s)));
    }
    int64_t nspans = 0;
    std::tie(keep_spans_, nspans) = _u64s(flat);
    _Config& c = config;
    c.uch = uc.uch;
    if (a.reg_ids.size() > 10) throw py::IndexError("invalid index");   // (a ctypes array of ten)
    for (int64_t i = 0; i < a.reg_ids.size(); i++) c.reg_ids[i] = static_cast<int32_t>(a.reg_ids[i]);
    c.ins_in = static_cast<int32_t>(a.ins_in);
    c.ins_out = static_cast<int32_t>(a.ins_out);
    c.vm_addr = static_cast<uint64_t>(a.vm_addr);
    c.ret = static_cast<uint64_t>(a.ret);
    c.scratch_lo = static_cast<uint64_t>(std::get<0>(a.scratch));
    c.scratch_hi = static_cast<uint64_t>(std::get<1>(a.scratch));
    c.spans = keep_spans_->data();
    c.nspans = a.spans.size();
    c.cap = a.cap;
    c.service_eax = static_cast<int64_t>(static_cast<uint64_t>(a.service_eax) & 0xFFFFFFFFull);
    c.service_carry = a.service_carry;
    c.gov_n = gov->n;
    c.gov_ceil = gov->ceil;
    for (int i = 0; i < kSlots && slot_ < 0; i++) {   // (the closure of self._resolve)
        if (!g_runs[i]) slot_ = i;
    }
    if (slot_ < 0) throw py::RuntimeError("cbtrace: too many driver runs are alive at once");
    g_runs[slot_] = this;
    c.answers = g_entry[slot_];
    c.shadow = a.shadow;
    if (!lib) {
        g_runs[slot_] = nullptr;
        throw py::AttributeError("'NoneType' object has no attribute 'cbt_drv_begin'");
    }
    handle = lib->cbt_drv_begin(&c);
    if (!handle) {
        g_runs[slot_] = nullptr;
        throw py::RuntimeError("cbtrace: a driver run could not be set up");
    }
    int e = lib->cbt_drv_attach(handle);
    if (e) {
        void* h = handle;   // (the half-made run is freed, as the Python object's __del__ does)
        handle = nullptr;
        g_runs[slot_] = nullptr;
        lib->cbt_drv_free(h);
        throw nt::UcError(e);
    }
}

// Which register the service answers in (driver.answers_in), or -1.
int Run::_resolve(int32_t device, int32_t service, int32_t* out) {
    try {
        out[0] = static_cast<int32_t>(answers_fn(device, service));
        return 0;
    } catch (...) {   // (BaseException)
        if (!exc) {
            exc = std::current_exception();
        }
        return 1;
    }
}

// The exception the resolver raised, if any (driver.run has no try around the hook, so a
// resolver that raised comes out of emu_start as a UcError; this is the original).
std::exception_ptr Run::raised() {
    lib->cbt_drv_state(handle, state_);
    gov->n = state_[2];
    std::exception_ptr e = exc;
    exc = nullptr;
    return e;
}

// The rec fields the Python hooks would have left, as data.
void Run::collect(DriverRecord& rec) {
    const _Result& r = *lib->cbt_drv_result_get(handle);
    gov->n = r.gov_n;
    rec.insns = r.insns;
    rec.truncated = r.truncated != 0;
    if (r.governed) {
        rec.governed = true;
    }
    rec.outside = r.outside;
    if (r.exit_code == 3) {
        rec.exit = py::str("int:") + hex02(r.exit_int);
    } else if (_EXITS.contains(r.exit_code)) {
        rec.exit = _EXITS.at(r.exit_code);
    }
    {
        py::set<uint64_t> cover;
        if (r.ncover) {
            const uint64_t* p = static_cast<const uint64_t*>(r.cover);
            for (int64_t i = 0; i < r.ncover; i++) cover.add(p[i]);
        }
        rec.cover = cover;
    }
    {
        py::list<Call> calls;
        for (const auto& w : _words(r.calls, r.ncalls, _CALL_WORDS)) {
            std::optional<uint64_t> first = w[0] ? std::nullopt : std::optional<uint64_t>(w[1]);
            py::list<uint64_t> regs, words;   // w[4:4 + w[3]] and w[11:11 + w[10]] (a slice stops at the end)
            if (w[3]) {
                for (uint64_t k = 4; k - 4 < w[3] && k < w.size(); k++) regs.append(w[k]);
            }
            if (w[10]) {
                for (uint64_t k = 11; k - 11 < w[10] && k < w.size(); k++) words.append(w[k]);
            }
            calls.append(Call{first, w[2], regs, words});
        }
        rec.calls = calls;
    }
    {
        py::list<Port> ports;
        for (const auto& w : _words(r.ports, r.nports, _PORT_WORDS)) {
            if (w[0] > 1) throw py::IndexError("tuple index out of range");   // ("in", "out")[w[0]]
            ports.append(Port{w[0] ? py::str("out") : py::str("in"), w[1], w[2], w[3]});
        }
        rec.ports = ports;
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
            if (w[3] > 3) throw py::IndexError("tuple index out of range");   // ("read", "write", "fetch", "?")[w[3]]
            lazy.append(nt::LazyMap{w[0], w[1], w[2], nt::_ACCESS[static_cast<int64_t>(w[3])], w[4]});
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
    if (lib->cbt_drv_detach(handle)) {
        throw py::RuntimeError("cbtrace: a driver hook could not be taken off");
    }
}

Run::~Run() {
    void* h = handle;
    handle = nullptr;
    if (slot_ >= 0 && g_runs[slot_] == this) g_runs[slot_] = nullptr;
    if (h) {
        lib->cbt_drv_free(h);
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

}  // namespace codebreaker::emulate::native_driver
