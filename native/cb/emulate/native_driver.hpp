// cb/emulate/native_driver.hpp -- the flat ring-0 driver recorder: the hooks of one VxD entry point's run, in C++.
//
// A run of a VxD entry point calls back into the recorder on every instruction and every memory access. The emulator
// (emulate/driver) lays the regions out, sets the registers and starts the run itself; the recorder (emulate/tracer's
// driver recorder) is the hooks, attached to the Unicorn engine the emulator laid out, and this header is the typed face
// of it:
//
//     native_driver::Options opt;  opt.governor = &governor;  opt.spans = {{lo, hi}};  opt.resolver = ...;  ...
//     native_driver::DriverRun run(engine, opt);          // the hooks attached
//     uc_emu_start(engine, entry, opt.return_address, 0, cap + 1);   (or run.start(entry, cap + 1))
//     run.after_host_start();                              // throws what the resolver threw
//     DriverRecord rec = run.collect();                    // what the hooks recorded
//     run.detach();
//
// The hooks, in the order they are attached: the code (the driver's own, told from a call out through a pointer), memory,
// the INT 20h service calls, the ports. During a run the caller is asked one thing: which register a service answers in,
// once per distinct service (the ServiceResolver). The engine must outlive the DriverRun.
//
// One DriverRun belongs to one thread, as the engine it hooks does.
#pragma once

#include <array>
#include <cstdint>
#include <exception>
#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "cb/emulate/native_trace.hpp"
#include "cb/emulate/run.hpp"

struct uc_struct;   // Unicorn's engine (uc_engine)

namespace cb::emulate::native_driver {

// The place in the driver's result registers (0 eax, 1 ebx, 2 ecx, 3 edx, 4 esi, 5 edi, 6 ebp) a service answers in, or
// nullopt when it answers in EAX like most do. Asked once per (device, service) in a run; what it throws comes out of
// after_host_start().
using ServiceResolver = std::function<std::optional<unsigned>(std::uint32_t device, std::uint32_t service)>;

struct Options {
    std::uint64_t vm_address = 0;            // an answering register is set to it (driver.VM)
    std::uint64_t return_address = 0;        // where the entry point returns to (emulator._RET)
    native_trace::AddressRange scratch;      // the scratch region
    std::vector<native_trace::AddressRange> spans;   // the image's own code
    std::int64_t cap = 0;                    // the instruction cap
    std::uint32_t service_eax = 0;           // what a service answers in EAX
    bool service_carry = false;              // ... and the carry flag it leaves
    native_trace::Governor* governor = nullptr;   // required: the machine's instruction budget, carried through the run
    ServiceResolver resolver;                // empty: every service answers in EAX
    tracer::Shadow* shadow = nullptr;        // the path explorer's, stepped beside the run (kind 33), or none
};

// ---- what the run recorded -----------------------------------------------------------------------------------------

// How the hooks ended the run, when they did: a jump to unmapped code ("wild"), a service number that could not be read, or
// an interrupt that is not a service (INT n).
struct DriverExit {
    enum class Kind : std::uint8_t { Wild, ServiceUnreadable, Interrupt };
    Kind kind = Kind::Wild;
    std::uint8_t interrupt = 0;              // Kind::Interrupt
    std::string text() const;                // "wild" | "exception: service number unreadable" | "int:21"
};

// A call out of the driver's code: a service of a device (INT 20h), or through a pointer (no device; `number` is the
// address it reached); with the registers and the top stack words at the call.
struct DriverCall {
    ServiceCall target;
    std::vector<std::uint64_t> registers;    // eax ebx ecx edx esi edi
    std::vector<std::uint64_t> stack_words;  // four words at esp
};

struct PortAccess {
    enum class Kind : std::uint8_t { In, Out };
    Kind kind = Kind::In;
    std::uint64_t port = 0;
    std::uint64_t size = 0;
    std::uint64_t value = 0;
};

struct DriverRecord {
    std::int64_t insns = 0;
    bool truncated = false;
    bool governed = false;
    std::int64_t outside = 0;                // instructions outside the image's code
    std::optional<DriverExit> exit;
    std::vector<std::uint64_t> cover;        // the addresses of the image that ran, each once, in order of first arrival
    std::vector<DriverCall> calls;
    std::vector<PortAccess> ports;
    std::vector<native_trace::MemRead> mem_reads;
    std::vector<native_trace::MemWrite> mem_writes;
    std::vector<native_trace::LazyMap> lazy_maps;
    std::optional<std::int64_t> memops;
    std::optional<std::int64_t> writes_seen;
    std::optional<std::int64_t> stuck;
};

// ---- one run ---------------------------------------------------------------------------------------------------------

class DriverRun {
public:
    // Attaches the hooks to `engine`. Throws EmulationError when Unicorn refuses one.
    DriverRun(uc_struct* engine, Options options);
    DriverRun(const DriverRun&) = delete;
    DriverRun& operator=(const DriverRun&) = delete;
    ~DriverRun();

    // uc_emu_start(begin, options.return_address, no time limit, `count` instructions at most: 0 = unlimited). Returns
    // Unicorn's own status (0 = ran to the end; a fault is its error number: the emulator says what it ended as); then
    // after_host_start().
    int start(std::uint64_t begin, std::size_t count = 0);

    // For a host that starts the engine itself (its wrapper has its own error model): around its emu_start. The first
    // clears what an earlier run carried; the second brings the governor's count back and throws what the resolver threw.
    void before_host_start();
    void after_host_start();

    // The exception the resolver threw, if any, taken (a host whose emu_start failed too reports this one first).
    std::exception_ptr take_raised();

    // What the hooks recorded. Also carries the governor's count back to the Options::governor.
    DriverRecord collect();

    void detach();                           // hooks off

    struct Impl;

private:
    std::unique_ptr<Impl> impl_;
};

}  // namespace cb::emulate::native_driver
