// emulate/a64.hpp -- AArch64 (A64) as an interpreter, to run functions of Nintendo Switch programs (small, deterministic, and it logs what a function does to the system).
//
//     a64::Memory memory(regions);                       // the program's loaded pieces, shared between runs; writes go to private pages
//     memory.map_writable(0x100000, 0x10000);            // stack, heap, thread-local storage
//     a64::RunResult r = a64::run(memory, entry, options);   // call the function as `bl` would, run to its `ret`
//
// Every instruction of decompile/a64 the table says how to run: the integer data processing (immediate, shifted and extended register, flags, bitfields, multiplies and divides,
// the bit and byte reversals), the branches, the loads and stores of every addressing form (pairs, exclusive, ordered), the scalar floating point of half (moves and conversions),
// single and double precision (arithmetic, fused multiply-add, compares, conversions to and from integers and fixed point, rounding), the system register moves (nzcv, fpcr, fpsr,
// tpidr_el0, tpidrro_el0, the counter), and the vector forms listed in a64_simd. Timing is not modelled. What is not run stops the run with its reason: an instruction without
// semantics here ("simd: ..."), an access to an address no region holds or a write to one that is read-only ("data abort"), `brk`, `hlt` and `udf`, a system register that is not
// modelled. `svc` calls the run's handler: it logs the Horizon kernel call (by number), and the default handler answers success (r0 = 0) after setting up what a homebrew
// program asks of the kernel first (the heap size, the main thread handle, the system tick); ExitProcess, ExitThread and Break end the run.
#pragma once

#include <array>
#include <cstdint>
#include <functional>
#include <map>
#include <memory>
#include <optional>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "core/bytes.hpp"

namespace cb::emulate::a64 {

inline constexpr std::uint64_t kMaxSteps = 200000;
inline constexpr std::uint64_t kReturnAddress = 0xFFFFFFF0u;   // where a called function's `ret` lands: nothing is mapped there

// Bytes loaded at an address (shared, read-only unless the region is made writable by map_writable's copy-on-write pages).
struct Region {
    std::uint64_t address = 0;
    Bytes bytes;
    bool writable = false;
};
using Regions = std::shared_ptr<const std::vector<Region>>;

// The address space. A Memory belongs to one thread; the shared regions are never written.
class Memory {
public:
    Memory() = default;
    explicit Memory(Regions regions);

    // Make [address, address + size) writable (zero filled, or what a shared region holds there).
    void map_writable(std::uint64_t address, std::uint64_t size);
    // Make [address, address + size) readable zeros (read-only).
    void map_zero(std::uint64_t address, std::uint64_t size);

    // `size` bytes at `address` (little-endian, up to 8); false: an address no region holds. A 16-byte access is two.
    bool read(std::uint64_t address, unsigned size, std::uint64_t& value) const;
    bool write(std::uint64_t address, unsigned size, std::uint64_t value);
    bool read_bytes(std::uint64_t address, std::size_t n, std::uint8_t* out) const;
    bool write_bytes(std::uint64_t address, const std::uint8_t* data, std::size_t n);
    bool mapped(std::uint64_t address) const;
    bool writable(std::uint64_t address) const;

    // Every byte that now differs from what was loaded (zero where nothing was), in address order, at most `limit` of them.
    std::vector<std::pair<std::uint64_t, std::uint8_t>> changes(std::size_t limit = 4000) const;

private:
    struct Range {
        std::uint64_t lo, hi;   // [lo, hi)
        bool writable;
    };
    using Page = std::array<std::uint8_t, 4096>;
    const Range* find(std::uint64_t address) const;
    std::uint8_t base_byte(std::uint64_t address) const;
    std::uint8_t byte(std::uint64_t address) const;
    Page& page_for_write(std::uint64_t address);

    Regions shared_;
    std::vector<Range> ranges_;
    std::map<std::uint64_t, std::unique_ptr<Page>> pages_;
};

struct V128 {
    std::uint64_t lo = 0, hi = 0;
    bool operator==(const V128&) const = default;
};

struct Registers {
    std::array<std::uint64_t, 31> x{};
    std::uint64_t sp = 0, pc = 0;
    std::uint32_t nzcv = 0;     // bits 31:28
    std::array<V128, 32> v{};
    std::uint32_t fpcr = 0, fpsr = 0;
    std::uint64_t tpidr_el0 = 0, tpidrro_el0 = 0;
};

// One kernel call the program made.
struct SvcCall {
    std::uint32_t number = 0;
    std::array<std::uint64_t, 8> args{};
    std::string note;                   // what the handler made of it (an IPC request's command and service)
};

class Cpu;

// What a run does about an `svc`: fill in the results (x0..x7) and say whether the run goes on.
using SvcHandler = std::function<bool(Cpu&, SvcCall&)>;

class Cpu {
public:
    Cpu(Memory& memory) : mem_(&memory) {}

    Registers regs;
    Memory& memory() { return *mem_; }
    std::uint64_t steps = 0;
    std::uint64_t counter = 0x1000;    // what cntvct_el0 reads, advancing by 1000 each read
    SvcHandler svc;
    std::vector<SvcCall> calls;        // the kernel calls so far

    // Execute the instruction at pc. nullopt: it ran. A string: why the run cannot go on.
    std::optional<std::string> step();

private:
    Memory* mem_;
    std::uint64_t monitor_ = 0;
    bool monitor_set_ = false;
};

struct RunOptions {
    std::array<std::uint64_t, 8> x{};      // the starting x0..x7
    std::uint64_t x18 = 0, x19 = 0, x29 = 0;
    std::uint64_t sp = 0;
    std::uint64_t tpidr_el0 = 0, tpidrro_el0 = 0;
    std::uint64_t max_steps = kMaxSteps;
    SvcHandler svc;                        // default: Horizon (see horizon_svc)
    std::function<void(const Registers&)> on_instruction;
};

enum class End : std::uint8_t { Return, Stopped, Limit };

struct RunResult {
    End end = End::Limit;
    std::string reason;                    // why it stopped
    Registers regs;                        // as the run left them (pc: where it stopped)
    std::uint64_t steps = 0;
    std::vector<SvcCall> calls;
    std::string exit() const;              // "return" | "stopped: <why>" | "limit"
};

// Call the function at `entry` as `bl` would and run it until it returns (ret to kReturnAddress), stops or takes `max_steps` instructions.
RunResult run(Memory& memory, std::uint64_t entry, const RunOptions& options = {});

// The Horizon kernel's calls by number ("SendSyncRequest" for 0x21; empty for a number not named).
std::string_view svc_name(std::uint32_t number);

// The default handler: ExitProcess, ExitThread, Break end the run; SetHeapSize answers with a heap at `kHeapBase`; the rest answer success.
inline constexpr std::uint64_t kHeapBase = 0x0000000200000000ull;
bool horizon_svc(Cpu& cpu, SvcCall& call);

// A decoded hipc request at the thread-local storage's message buffer, for the log: "ipc request cmd 1 (GetService) 'fsp-srv'" or empty when the buffer holds no request.
std::string describe_ipc(Cpu& cpu);

}  // namespace cb::emulate::a64
