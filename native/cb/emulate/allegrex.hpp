// cb/emulate/allegrex.hpp -- the PSP, to run functions on: the Allegrex (emulate/mips_le) in the console's memory map, with the hardware's blocks named and
// logged and the imported functions stubbed and logged.
//
//     allegrex::Bus bus(regions, {.imports = {{0x08804100, "sceDisplaySetMode"}}});
//     allegrex::RunResult r = allegrex::run(bus, entry, {.a = {1, 2, 3, 4}});   // call a function as JAL would: run to its return
//     r.io, r.calls                                                              // what it did to the hardware, what it asked of the system
//
// The map: 32 MB of RAM at physical $08000000 (the user area from $08800000; seen again at $88000000 and $A8000000), 2 MB of video memory at $04000000, the scratchpad
// (16 KB at $00010000, off for a module linked at 0, which is loaded where the file says), and the hardware at $1C000000-$1FFFFFFF in blocks named for what they are
// (formats/psp: `GE_0100`, `INTC_0000`): every access is logged with its name, and a register reads what was written to it. The graphics engine, the DMA controller
// and the display controller are not run.
//
// An imported function is an 8-byte stub the loader fills in; a call to one is logged with its name (`sceDisplaySetMode(0,480,272,0)`) and returns 0 ($v0 = 0), and
// the ones that end a program (sceKernelExitGame, sceKernelExitThread, sceKernelExitDeleteThread, sceKernelSelfStopUnloadModule) end the run ("stopped: syscall
// sceKernelExitGame"). A SYSCALL the program executes itself is logged as `syscall <code>` and returns 0. The VFPU is not run (the run ends with "not run:
// <mnemonic>"). A Bus, a Cpu and a run belong to one thread.
#pragma once

#include <array>
#include <cstdint>
#include <map>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "cb/emulate/mips_le.hpp"
#include "cb/emulate/paged_memory.hpp"

namespace cb::emulate::allegrex {

inline constexpr std::uint64_t kMaxSteps = 200000;
inline constexpr std::uint32_t kRamBase = 0x08000000;
inline constexpr std::uint32_t kRamSize = 0x02000000;
inline constexpr std::uint32_t kScratchpad = 0x00010000;
inline constexpr std::uint32_t kScratchpadSize = 0x4000;
inline constexpr std::uint32_t kVram = 0x04000000;
inline constexpr std::uint32_t kVramSize = 0x00200000;
// The return address run() gives: reaching it ends the run (an address of the kernel's mirror no program has code at).
inline constexpr std::uint32_t kReturnAddress = 0x89FFFF00;

// One access to the hardware: a read or a write of 1, 2 or 4 bytes at a physical address.
struct BusAccess {
    enum class Kind : std::uint8_t { Read, Write };
    Kind kind = Kind::Read;
    std::uint8_t size = 4;
    std::uint32_t address = 0;
    std::uint32_t value = 0;
    std::string name;
    std::string text() const;   // "r4 GE_0100" "w4 INTC_0004"
};

// One call into the system a run made: an imported function by name, or a SYSCALL by its code.
struct Call {
    std::string name;
    std::array<std::uint32_t, 4> args{};
    std::string text() const;   // "sceDisplaySetMode(0,1e0,110,0)"
};

struct BusOptions {
    std::map<std::uint32_t, std::string> imports;   // the stubs of the imported functions, by address
    bool scratchpad = true;
};

class Bus final : public mipsle::Port {
public:
    Bus(paged_memory::Regions regions, BusOptions options = {});

    bool read(std::uint32_t address, unsigned size, std::uint64_t& value) override;
    bool write(std::uint32_t address, unsigned size, std::uint64_t value) override;
    bool syscall(mipsle::Cpu& cpu, std::uint32_t code, std::string& stopped) override;
    Intercept intercept(mipsle::Cpu& cpu, std::string& stopped) override;

    const paged_memory::Memory& memory() const { return memory_; }
    paged_memory::Memory& memory() { return memory_; }
    const std::vector<BusAccess>& log() const { return log_; }
    const std::vector<Call>& calls() const { return calls_; }

private:
    static std::optional<std::uint32_t> physical(std::uint32_t address);
    paged_memory::Memory memory_;
    BusOptions options_;
    std::uint32_t stub_low_ = 0xFFFFFFFF, stub_high_ = 0;
    std::vector<std::uint8_t> scratch_ = std::vector<std::uint8_t>(kScratchpadSize, 0);
    std::vector<std::pair<std::uint32_t, std::uint32_t>> latched_;
    std::vector<BusAccess> log_;
    std::vector<Call> calls_;
};

// What run() starts from: $a0-$a3 and $s0-$s7 (the arguments and the callee-saved registers, absent: zero), $sp, $gp and $t0-$t7.
struct RunOptions {
    std::uint64_t max_steps = kMaxSteps;
    std::array<std::uint32_t, 4> a{};
    std::array<std::uint32_t, 8> s{};
    std::uint32_t sp = 0x09F00000;
    std::uint32_t gp = 0;
    std::array<std::uint32_t, 8> t{};
};

enum class End : std::uint8_t { Return, Stopped, Limit };

struct RunResult {
    End end = End::Limit;
    std::optional<mipsle::Fault> fault;
    std::array<std::uint64_t, 32> r{};
    std::array<std::uint64_t, 2> hi{}, lo{};
    std::uint32_t f0 = 0;
    std::uint64_t steps = 0;
    std::vector<BusAccess> io;
    std::vector<Call> calls;
    std::string exit() const;   // "return" | "stopped: <why>" | "limit"
};

// Call the function at `entry` as JAL would ($ra = kReturnAddress) and run it until it returns, stops, or takes `max_steps` instructions.
RunResult run(Bus& bus, std::uint32_t entry, const RunOptions& options = {});

}  // namespace cb::emulate::allegrex
