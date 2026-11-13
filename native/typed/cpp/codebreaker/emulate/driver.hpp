// codebreaker/emulate/driver.hpp -- COMPAT SHIM over cb/emulate/driver.hpp (docs/CPP_DESIGN.md 6.3), for the port that still uses the old py::
// surface (verify/lanes/driver, platforms/vxd). Conversion and forwarding only; deleted with its last py:: consumer.
//
// The module (the image, the run, the record) is native/cb/emulate/driver. Here: the old Image (py containers over the new one, shared), the
// old Record, `run`'s `registers` dict and word lists, the process-wide governor (`emulator::_GOV`, copied to the run's before it and back after
// it), the resolver over the old driver-kit names and knowledge base, and the translation each way.
//
// Not carried over: `registers` names other than eax ebx ecx edx esi edi ebp (esp, eip and eflags were accepted; no caller sets them -- every
// other name was a KeyError and still is).
#pragma once

#include <cstdint>
#include <memory>
#include <optional>
#include <tuple>

#include "cb/compat/py_bridge.hpp"
#include "cb/emulate/driver.hpp"
#include "codebreaker/emulate/emulator.hpp"
#include "codebreaker/emulate/native_driver.hpp"
#include "codebreaker/emulate/native_shadow.hpp"
#include "codebreaker/emulate/run.hpp"
#include "codebreaker/emulate/unicorn_uc.hpp"
#include "codebreaker/formats/le.hpp"
#include "codebreaker/knowledge/package.hpp"
#include "codebreaker/knowledge/sources.hpp"
#include "codebreaker/symbols/driver_services.hpp"
#include "py.hpp"

namespace codebreaker::emulate::native_shadow {
class Shadow;
}

namespace codebreaker::emulate::driver {

namespace nd = ::cb::emulate::driver;

inline constexpr int64_t MAX_CALLS = static_cast<int64_t>(nd::kMaxCalls);   // service calls compared per run
inline const py::list<py::str> RESULT_REGISTERS = {py::str("ebx"), py::str("ecx"), py::str("edx"), py::str("esi"), py::str("edi"), py::str("ebp")};   // EAX is the fingerprint's `ret`
inline constexpr int64_t INSN_CAP = nd::kInstructionCap;
inline const py::list<py::str> REGISTERS = {py::str("eax"), py::str("ebx"), py::str("ecx"), py::str("edx"), py::str("esi"), py::str("edi"), py::str("ebp")};

// emulator._STACK_BASE ... (the stack and scratch regions of the 32-bit tracer's machine)
inline constexpr int64_t STACK_LO = nd::kStackLow, STACK_HI = nd::kStackHigh;

// Places the inputs point at, inside the emulator's scratch region.
inline constexpr int64_t SCRATCH = nd::kScratch;
inline constexpr int64_t VM = nd::kVm, CLIENT = nd::kClient, BUFFER = nd::kBuffer, OBJECT = nd::kObject, SAFE = nd::kSafe;
inline constexpr int64_t RETURN = nd::kReturn;

// The register a service answers in when it is not EAX, or None: without the answer the code
// after it has nothing to work on. (What the kit documents -- the knowledge base's vxd
// api_model, knowledge/models/vxd.json; everything else returns EAX and carry.)
inline std::optional<py::str> answers_in(const std::optional<py::str>& service) {
    const auto row = knowledge::api_model(py::str("vxd"), (service && !service->empty()) ? *service : py::str(""));
    if (!row || !row->contains(py::str("answers_in"))) return std::nullopt;
    const auto& v = row->at(py::str("answers_in"));
    if (v.kind == knowledge::sources::Json::Str) return v.s;
    return std::nullopt;
}

// A driver as the loader leaves it in memory: regions [(address, bytes)], spans
// [(address, end, object)], entries {label: address}, shape (sizes of its objects).
class Image {
  public:
    explicit Image(const formats::le::LEFile& lf) : impl_(std::make_shared<const nd::Image>(::cb::compat::guard([&] { return nd::Image::of(lf.module()); }))) {
        for (const nd::Region& r : impl_->regions) regions.append(std::make_tuple(static_cast<int64_t>(r.address), ::cb::compat::to_py(::cb::ByteView(r.bytes))));
        for (const nd::Span& s : impl_->spans) spans.append(std::make_tuple(static_cast<int64_t>(s.low), static_cast<int64_t>(s.high), static_cast<int64_t>(s.object)));
        for (const auto& [label, address] : impl_->entries) entries.set(::cb::compat::to_py(label), static_cast<int64_t>(address));
        for (const uint32_t v : impl_->shape) shape.append(v);
        digest = ::cb::compat::to_py(impl_->digest);
    }

    py::list<std::tuple<int64_t, py::bytes>> regions;
    py::list<std::tuple<int64_t, int64_t, int64_t>> spans;
    py::dict<py::str, int64_t> entries;
    py::list<int64_t> shape;
    py::str digest;

    bool holds(int64_t address) const {
        for (const auto& s : spans) {
            if (std::get<0>(s) <= address && address < std::get<1>(s)) return true;
        }
        return false;
    }
    const nd::Image& impl() const { return *impl_; }

  private:
    std::shared_ptr<const nd::Image> impl_;
};

// The Image of a driver file (of one member of a container: by name, else the first).
inline Image load(const py::str& path, const std::optional<py::str>& member = std::nullopt) {
    const auto found = formats::le::members(path);
    if (found.empty()) throw py::IndexError("list index out of range");
    for (const auto& nx : found) {
        if (member && !member->empty() && std::get<0>(nx).lower() == member->lower()) return Image(std::get<1>(nx));
    }
    return Image(std::get<1>(found[0]));
}

// The rec dict of one run: {"exit", "eax", "flags", "registers", "esp_delta", "calls" [(device, service, registers, stack words)],
// "ports" [...], "mem_reads", "mem_writes", "cover" (addresses run inside the image), "truncated", "insns", "outside"
// and what the hooks add: governed, lazy_maps, memops, writes_seen, _stuck}.
struct Record {
    std::optional<py::str> exit;
    std::optional<int64_t> eax;
    py::list<native_driver::Call> calls;
    py::list<native_driver::Port> ports;
    py::list<native_trace::MemRead> mem_reads;
    py::list<native_trace::MemWrite> mem_writes;
    bool truncated = false;
    int64_t insns = 0;
    py::set<uint64_t> cover;
    int64_t outside = 0;
    int64_t flags = 0;                     // the carry and zero flags
    py::list<int64_t> registers;           // RESULT_REGISTERS, in order
    int64_t esp_delta = 0;
    std::optional<bool> governed;
    std::optional<py::list<native_trace::LazyMap>> lazy_maps;
    std::optional<int64_t> memops, writes_seen, _stuck;
};

namespace shim {

inline ::cb::emulate::native_trace::Governor& governor() {
    static ::cb::emulate::native_trace::Governor g;
    return g;
}

inline nd::Environment& environment() {
    static nd::Environment env = [] {
        nd::Environment e;
        e.governor = &governor();
        e.resolver = nd::resolver_of(
            [](uint32_t device, uint32_t service) -> std::optional<std::string> {
                const std::optional<py::str> name = symbols::driver_services::service_name(device, service);
                if (!name) return std::nullopt;
                return ::cb::compat::from_py(*name);
            },
            [](std::string_view name) -> std::optional<std::string> {
                const std::optional<py::str> where = answers_in(std::optional<py::str>(::cb::compat::to_py(name)));
                if (!where) return std::nullopt;
                return ::cb::compat::from_py(*where);
            });
        e.open_engine = []() -> std::unique_ptr<::cb::core16::Uc> {
            native_trace::require();   // points the recorder at the host's libraries, or raises saying why not
            return ::cb::compat::guard([] { return ::cb::emulate::unicorn_uc::open_x86_from(unicorn_uc::shim_detail::dll(), 32); });
        };
        return e;
    }();
    return env;
}

inline Record record_of(const nd::Record& r) {
    Record out;
    out.exit = ::cb::compat::to_py(r.exit_text());
    out.eax = r.eax;
    for (const uint32_t v : r.registers) out.registers.append(v);
    out.flags = r.flags;
    out.esp_delta = r.esp_delta;
    const auto& seen = r.observed;
    out.insns = seen.insns;
    out.truncated = seen.truncated;
    if (seen.governed) out.governed = true;
    out.outside = seen.outside;
    for (const uint64_t a : seen.cover) out.cover.add(a);
    for (const auto& c : seen.calls) {
        py::list<uint64_t> regs, words;
        for (const uint64_t v : c.registers) regs.append(v);
        for (const uint64_t v : c.stack_words) words.append(v);
        out.calls.append(native_driver::Call{c.target.device ? std::optional<uint64_t>(*c.target.device) : std::nullopt, c.target.number, regs, words});
    }
    for (const auto& p : seen.ports)
        out.ports.append(native_driver::Port{py::str(p.kind == ::cb::emulate::native_driver::PortAccess::Kind::Out ? "out" : "in"), p.port, p.size, p.value});
    for (const auto& x : seen.mem_reads) out.mem_reads.append(native_trace::MemRead{x.pc, x.addr, x.size, x.value, x.n});
    for (const auto& x : seen.mem_writes) out.mem_writes.append(native_trace::MemWrite{x.pc, x.addr, x.size, x.value, x.n, x.old});
    py::list<native_trace::LazyMap> lazy;
    for (const auto& x : seen.lazy_maps)
        lazy.append(native_trace::LazyMap{x.pc, x.addr, x.page, py::str(std::string(::cb::emulate::native_trace::to_text(x.access))), x.size});
    out.lazy_maps = lazy;
    out.memops = seen.memops;
    out.writes_seen = seen.writes_seen;
    out._stuck = seen.stuck;
    return out;
}

inline uint32_t word(int64_t v) { return static_cast<uint32_t>(static_cast<uint64_t>(v) & 0xFFFFFFFFull); }

}  // namespace shim

// Run flat 32-bit code at `entry` of `image` until it returns, and record what it did.
// `registers` {name: value}; `stack` words above the return address; `memory` [(address, bytes)] set before the run;
// `flags` the carry (1) and zero (0x40) flags on entry. Every `int 20h` service call is recorded and
// returns EAX = service_eax, carry = service_carry; a call that leaves the image for
// nowhere is recorded and returned from the same way. `shadow`: a native shadow kept up beside the run (the path
// explorer's): stepped before each instruction run, told of each call answered. The run is the same run with it or without.
inline nd::Record run_new(const Image& image, int64_t entry, const std::optional<py::dict<py::str, int64_t>>& registers, const py::list<int64_t>& stack,
                          const py::list<std::tuple<int64_t, py::bytes>>& memory, int64_t service_eax, int64_t service_carry, int64_t cap,
                          int64_t flags, native_shadow::Shadow* shadow) {
    nd::RunOptions o;
    if (registers) {
        for (const auto& [name, value] : registers->items()) {
            const std::string n = name.utf8();
            const uint32_t v = shim::word(value);
            if (n == "eax") o.registers.eax = v;
            else if (n == "ebx") o.registers.ebx = v;
            else if (n == "ecx") o.registers.ecx = v;
            else if (n == "edx") o.registers.edx = v;
            else if (n == "esi") o.registers.esi = v;
            else if (n == "edi") o.registers.edi = v;
            else if (n == "ebp") o.registers.ebp = v;
            else throw py::KeyError(py::repr(name).utf8());
        }
    }
    for (const int64_t w : stack.vec()) o.stack.push_back(shim::word(w));
    for (const auto& m : memory) o.memory.push_back({static_cast<uint32_t>(std::get<0>(m)), ::cb::compat::from_py(std::get<1>(m))});
    o.service_eax = shim::word(service_eax);
    o.service_carry = (service_carry & 1) != 0;
    o.cap = cap;
    o.flags = shim::word(flags);
    if (shadow != nullptr) o.shadow = shadow->new_shadow();   // the recorder steps it from here on
    shim::governor().n = emulator::_GOV.n;
    shim::governor().ceil = emulator::_GOV.ceil;
    struct Back {
        ~Back() { emulator::_GOV.n = shim::governor().n; }
    } back;
    return unicorn_uc::shim_detail::uc_guard([&] { return nd::run(shim::environment(), image.impl(), static_cast<uint32_t>(entry), o); });
}

inline Record run(const Image& image, int64_t entry, const std::optional<py::dict<py::str, int64_t>>& registers = std::nullopt,
                  const py::list<int64_t>& stack = py::list<int64_t>(),
                  const py::list<std::tuple<int64_t, py::bytes>>& memory = py::list<std::tuple<int64_t, py::bytes>>(), int64_t service_eax = 0,
                  int64_t service_carry = 0, int64_t cap = INSN_CAP, int64_t flags = 0, native_shadow::Shadow* shadow = nullptr) {
    return shim::record_of(run_new(image, entry, registers, stack, memory, service_eax, service_carry, cap, flags, shadow));
}

// run's record as the one run record (emulate/run.py): EAX, the carry and zero flags and the
// result registers; each service call (detail: (device, service), or (None, address) for a call
// out through a pointer) with its registers and stack words; each port access; the addresses
// of the driver's own code that ran.
inline run::Run record(const Record& rec) {
    py::list<run::Write> writes;
    for (const auto& w : rec.mem_writes) {
        run::Write x;
        x.address = static_cast<int64_t>(w.addr);
        x.size = static_cast<int64_t>(w.size);
        x.value = static_cast<int64_t>(w.value);
        x.old = static_cast<int64_t>(w.old);
        x.harness = false;
        writes.append(x);
    }
    py::dict<py::str, run::Register> results;
    results.set(py::str("eax"), rec.eax ? run::Register(*rec.eax) : run::Register(py::none));
    results.set(py::str("flags"), run::Register(rec.flags));
    for (int64_t i = 0; i < RESULT_REGISTERS.size() && i < rec.registers.size(); i++) results.set(RESULT_REGISTERS[i], run::Register(rec.registers[i]));
    py::list<run::Call> calls;
    for (const auto& c : rec.calls) {
        const auto& [device, service, regs, words] = c;
        run::Call call;
        py::list<int64_t> args;
        for (const uint64_t r : regs) args.append(static_cast<int64_t>(r));
        for (const uint64_t w : words) args.append(static_cast<int64_t>(w));
        call.args = args;
        // (device, service): the device as its decimal text (run::Detail has no integer device), None through a pointer
        call.detail = std::make_tuple(device ? std::optional<py::str>(py::to_str(static_cast<int64_t>(*device))) : std::nullopt,
                                      run::Word(static_cast<int64_t>(service)));
        calls.append(call);
    }
    py::list<run::IO> io;
    for (const auto& p : rec.ports) {
        const auto& [kind, port, size, value] = p;
        run::IO x;
        x.kind = kind;
        x.port = static_cast<int64_t>(port);
        x.value = static_cast<int64_t>(value);
        x.size = static_cast<int64_t>(size);
        io.append(x);
    }
    std::vector<int64_t> cover;
    for (const uint64_t a : rec.cover) cover.push_back(static_cast<int64_t>(a));
    std::sort(cover.begin(), cover.end());   // (the Python holds the set; a list is in address order)
    py::list<int64_t> path;
    for (const int64_t a : cover) path.append(a);
    return run::Run(rec.exit, results, writes, rec.mem_writes.size() >= emulator::_MAX_MEM, calls, io, rec.esp_delta, std::nullopt, path, rec.insns,
                    rec.truncated, py::str("little"), std::make_shared<Record>(rec));
}

// The platform's runner (platforms/vxd.py emulator.run): run(...) as a Run.
inline run::Run recorded(const Image& image, int64_t entry, const std::optional<py::dict<py::str, int64_t>>& registers = std::nullopt,
                         const py::list<int64_t>& stack = py::list<int64_t>(),
                         const py::list<std::tuple<int64_t, py::bytes>>& memory = py::list<std::tuple<int64_t, py::bytes>>(),
                         int64_t service_eax = 0, int64_t service_carry = 0, int64_t cap = INSN_CAP, int64_t flags = 0,
                         native_shadow::Shadow* shadow = nullptr) {
    return record(run(image, entry, registers, stack, memory, service_eax, service_carry, cap, flags, shadow));
}

}  // namespace codebreaker::emulate::driver
