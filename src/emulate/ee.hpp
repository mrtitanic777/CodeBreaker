// emulate/ee.hpp -- the PlayStation 2's Emotion Engine, to run functions on: the R5900 (emulate/mips_le) in the console's memory map, with the registers of
// its units named and logged and the kernel's system calls stubbed and logged.
//
//     ee::Bus bus(regions);                                   // the program's loaded pieces (formats/ps2: the ELF's segments)
//     ee::RunResult r = ee::run(bus, entry, {.a = {1, 2, 3, 4}});   // call a function as JAL would: run to its return
//     r.io, r.syscalls                                        // what it did to the hardware, what it asked of the kernel
//
// The map: 32 MB of RAM at physical $00000000 (seen at $00000000, $20000000 uncached, $30100000 uncached accelerated, $80000000, $A0000000), the scratchpad (16 KB at
// $70000000), the hardware registers $10000000-$1000FFFF (timers, IPU, GIF, VIF0 and VIF1, the DMA controller, the interrupt controller, the SIF and SBUS) and
// the Graphics Synthesizer's privileged registers $12000000-$12001FFF, the IOP's RAM at $1C000000 and the BIOS at $1FC00000. Every access to a register is
// logged with its name (formats/ps2: `T0_COUNT`, `GS_CSR`, `D2_CHCR`); a register reads what was written to it, except the ones a program polls: GS_CSR reads
// with the vertical-blank and finish flags set, a DMA channel's CHCR reads with its start bit clear (the transfer is done), a timer's COUNT counts reads. The
// DMA controller, the GS, the VIFs and the IPU are not run: what a function sends them is the effect (the registers it wrote and the memory it prepared). The
// vector units' micro-programs and the IOP's modules are data.
//
// A SYSCALL is a kernel call numbered in $v1: it is logged with its name and $a0-$a3 (formats/ps2: `syscall_name`) and returns 0, except GetMemorySize (the 32 MB),
// SetupThread (its stack argument) and SetupHeap (its heap argument); Exit, ExitThread, ExitDeleteThread, LoadExecPS2 and ExecPS2 end the run ("stopped: syscall
// Exit"). A Bus, a Cpu and a run belong to one thread.
#pragma once

#include <array>
#include <cstdint>
#include <optional>
#include <string>
#include <vector>

#include "emulate/mips_le.hpp"
#include "emulate/paged_memory.hpp"

namespace cb::emulate::ee {

inline constexpr std::uint64_t kMaxSteps = 200000;
inline constexpr std::uint32_t kRamSize = 0x02000000;
inline constexpr std::uint32_t kScratchpad = 0x70000000;
inline constexpr std::uint32_t kScratchpadSize = 0x4000;
// The return address run() gives: reaching it ends the run (a word of the BIOS no program has code at).
inline constexpr std::uint32_t kReturnAddress = 0xBFC0FF00;

// One access to a register: a read or a write of 1, 2, 4, 8 or 16 bytes at a physical address.
struct BusAccess {
    enum class Kind : std::uint8_t { Read, Write };
    Kind kind = Kind::Read;
    std::uint8_t size = 4;
    std::uint32_t address = 0;
    std::uint64_t value = 0;
    std::string name;
    std::string text() const;   // "r4 GS_CSR" "w4 D2_CHCR"
};

// One kernel call a run made.
struct Syscall {
    std::int32_t number = 0;
    std::string name;
    std::array<std::uint32_t, 4> args{};
    std::string text() const;   // "CreateThread(20000,0,0,0)"
};

class Bus final : public mipsle::Port {
public:
    explicit Bus(paged_memory::Regions regions) : memory_(std::move(regions)) {}

    bool read(std::uint32_t address, unsigned size, std::uint64_t& value) override;
    bool write(std::uint32_t address, unsigned size, std::uint64_t value) override;
    bool syscall(mipsle::Cpu& cpu, std::uint32_t code, std::string& stopped) override;

    const paged_memory::Memory& memory() const { return memory_; }
    paged_memory::Memory& memory() { return memory_; }
    const std::vector<BusAccess>& log() const { return log_; }
    const std::vector<Syscall>& syscalls() const { return syscalls_; }

private:
    // The physical address of a virtual one, or nothing for an address no region answers.
    static std::optional<std::uint32_t> physical(std::uint32_t address);
    paged_memory::Memory memory_;
    std::vector<std::uint8_t> scratch_ = std::vector<std::uint8_t>(kScratchpadSize, 0);
    std::vector<std::pair<std::uint32_t, std::uint64_t>> latched_;   // register contents, by physical address
    std::vector<BusAccess> log_;
    std::vector<Syscall> syscalls_;
    std::uint32_t timer_ = 0;
};

// What run() starts from: $a0-$a3 and $s0-$s7 (the arguments and the callee-saved registers, absent: zero), $sp, $gp and $t0-$t7.
struct RunOptions {
    std::uint64_t max_steps = kMaxSteps;
    std::array<std::uint32_t, 4> a{};
    std::array<std::uint32_t, 8> s{};
    std::uint32_t sp = 0x01FFFF00;
    std::uint32_t gp = 0;
    std::array<std::uint32_t, 8> t{};
};

enum class End : std::uint8_t { Return, Stopped, Limit };

struct RunResult {
    End end = End::Limit;
    std::optional<mipsle::Fault> fault;
    std::array<std::uint64_t, 32> r{};
    std::array<std::uint64_t, 32> rh{};          // the upper halves of the 128-bit registers
    std::array<std::uint64_t, 2> hi{}, lo{};
    std::uint32_t f0 = 0;
    std::uint64_t steps = 0;
    std::vector<BusAccess> io;
    std::vector<Syscall> syscalls;
    std::string exit() const;                    // "return" | "stopped: <why>" | "limit"
};

// Call the function at `entry` as JAL would ($ra = kReturnAddress) and run it until it returns, stops, or takes `max_steps` instructions.
RunResult run(Bus& bus, std::uint32_t entry, const RunOptions& options = {});

}  // namespace cb::emulate::ee
