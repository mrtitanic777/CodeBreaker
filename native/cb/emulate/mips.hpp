// cb/emulate/mips.hpp -- the PlayStation's R3000A, to run functions on (small, deterministic, and it logs what a function does to the
// machine's devices and to the BIOS).
//
//     mips::Bus bus(ram_image);                          // 2 MB RAM, scratchpad, devices; map(lo, hi, read, write) hands ranges to callbacks
//     mips::Cpu cpu(bus);
//     cpu.step();                                        // one instruction
//     mips::RunResult r = mips::run(bus, entry, {.a = {1, 2, 3, 4}});   // call a function as JAL would: run to its return
//     cb::emulate::Run record = mips::record(r);         // the one run record the judge compares
//
// Every instruction cb/decompile/mips decodes. The GTE's registers move (MFC2 CFC2 MTC2 CTC2 LWC2 SWC2) and its commands that need no
// division run (NCLIP AVSZ3 AVSZ4 SQR OP GPF GPL MVMVA, with the MAC and IR saturation and FLAG bits); RTPS RTPT and the colour commands
// (NCS NCT NCDS NCDT NCCS NCCT CC CDP DCPL DPCS DPCT INTPL) end the run with "not run" until they are written. It runs as the R3000A does:
//
//   * a branch or jump takes effect after the instruction behind it (the delay slot); JAL/JALR/BLTZAL/BGEZAL link to the instruction
//     after the delay slot;
//   * a load's value is not there for the next instruction (the load delay slot); a register the delay slot writes keeps what it
//     wrote; LWL/LWR merge with a load that is still in flight;
//   * ADD ADDI SUB trap on overflow (the run ends with the reason); DIV and DIVU by zero give what the hardware does (LO = -1 or 1
//     for DIV, all ones for DIVU, HI the dividend);
//   * a load or store that is not aligned, or an address no device answers, ends the run with the reason.
//
// Addresses: the 2 MB of RAM is seen four times in the first 8 MB, and at $00000000 (KUSEG), $80000000 (KSEG0) and $A0000000 (KSEG1);
// the scratchpad is 1 KB at $1F800000; devices are at $1F801000-$1F802FFF; the BIOS ROM at $1FC00000 reads zeros unless an image is given.
// Every access to a device range is logged. A call into the BIOS (a jump to $A0, $B0 or $C0 with the function number in $t1) is
// logged with its arguments and returns 0, and `syscall` with $a0 of 1 or 2 (EnterCriticalSection, ExitCriticalSection) returns;
// the BIOS's own code is not run -- what a function asks of it is the effect the judge compares.
// A Bus, a Cpu and a run belong to one thread.
#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "cb/core/bytes.hpp"
#include "cb/decompile/mips.hpp"
#include "cb/emulate/run.hpp"

namespace cb::emulate::mips {

inline constexpr std::uint64_t kMaxSteps = 200000;
inline constexpr std::size_t kRamSize = 0x200000;
inline constexpr std::size_t kScratchSize = 0x400;
inline constexpr std::uint32_t kScratchBase = 0x1F800000;
inline constexpr std::uint32_t kDeviceLow = 0x1F801000;
inline constexpr std::uint32_t kDeviceHigh = 0x1F802FFF;
// The return address run() gives: reaching it ends the run (a word of KSEG1 no program has code at).
inline constexpr std::uint32_t kReturnAddress = 0xBFC0FF00;

// One access to a device range: a read or a write of 1, 2 or 4 bytes at a physical address.
struct BusAccess {
    enum class Kind : std::uint8_t { Read, Write };
    Kind kind = Kind::Read;
    std::uint8_t size = 1;
    std::uint32_t address = 0;
    std::uint32_t value = 0;
    std::string text() const;   // "r1" "r2" "r4" "w1" "w2" "w4"
};

// A call into the BIOS: `table` is 0xA0, 0xB0 or 0xC0, `function` the number in $t1.
struct BiosCall {
    std::uint8_t table = 0;
    std::uint32_t function = 0;
    std::array<std::uint32_t, 4> args{};
    std::string text() const;   // "A0:3C(1,2,3,4)"
};

// The RAM, the scratchpad and the devices. The last range mapped that holds an address answers it; a range with no read callback reads
// 0 and with no write callback ignores the write. Memory the Bus owns starts as zeros (or the image given).
class Bus {
public:
    using ReadFn = std::function<std::uint32_t(std::uint32_t address, unsigned size)>;
    using WriteFn = std::function<void(std::uint32_t address, unsigned size, std::uint32_t value)>;

    Bus();
    // A copy of `ram` at address 0 (UsageError when it is larger than 2 MB).
    explicit Bus(ByteView ram);

    // Physical addresses lo..hi (inclusive) go to the callbacks and are logged. The devices ($1F801000-$1F802FFF) are mapped
    // as a range that reads zeros; a map() over them takes over.
    void map(std::uint32_t lo, std::uint32_t hi, ReadFn read = {}, WriteFn write = {});
    // The BIOS ROM image ($1FC00000, up to 512 KB).
    void set_bios(ByteView image);

    // Does an address reach RAM, the scratchpad, a device or the BIOS?
    bool mapped(std::uint32_t address) const;
    // `size` is 1, 2 or 4 (the address is virtual: any of the three views). A read or write of an address that is not mapped is the
    // caller's to have asked `mapped` about first; here it reads 0 and is dropped.
    std::uint32_t read(std::uint32_t address, unsigned size);
    void write(std::uint32_t address, unsigned size, std::uint32_t value);

    ByteSpan memory() { return ram_; }
    ByteView memory() const { return ram_; }
    ByteSpan scratchpad() { return scratch_; }
    const std::vector<BusAccess>& log() const { return log_; }

private:
    struct Range {
        std::uint32_t lo, hi;
        ReadFn read;
        WriteFn write;
    };
    const Range* range_of(std::uint32_t physical) const;

    Bytes ram_;
    Bytes scratch_;
    Bytes bios_;
    std::vector<Range> ranges_;
    std::vector<BusAccess> log_;
};

// Why the CPU cannot go on: the run ends with this reason.
struct Fault {
    enum class Kind : std::uint8_t {
        NoInstruction,   // the bytes at pc are not an instruction
        NotRun,          // an instruction this interpreter does not run (`mnemonic`)
        AddressError,    // a load or store that is not aligned, or an instruction fetch that is not
        BusError,        // an address nothing answers
        Overflow,        // ADD, ADDI or SUB overflowed
        Break,
        Syscall,
    };
    Kind kind = Kind::NoInstruction;
    std::uint32_t pc = 0;        // where the instruction is
    std::uint32_t address = 0;   // AddressError, BusError: the address
    std::string mnemonic;        // NotRun: which
    std::uint32_t code = 0;      // Break, Syscall: the code
    // "no instruction at 80010000" | "not run: rtps" | "address error at 80010002" | "bus error at 12345678" | "arithmetic overflow" |
    // "break 7" | "syscall"
    std::string text() const;
};

class Cpu {
public:
    explicit Cpu(Bus& bus);

    std::array<std::uint32_t, 32> r{};        // $0-$31 ($0 stays zero)
    std::uint32_t pc = 0;                      // the instruction about to run
    std::uint32_t next_pc = 4;                 // the one after it (what a branch in the delay slot's place changes)
    std::uint32_t hi = 0, lo = 0;
    std::array<std::uint32_t, 32> cop0{};      // the system control coprocessor's registers
    std::array<std::uint32_t, 32> gte_data{};  // the GTE's data registers
    std::array<std::uint32_t, 32> gte_control{};
    std::uint64_t steps = 0;                   // instructions executed

    // Start at `address`: pc there, the instruction after it next (no branch pending).
    void jump_to(std::uint32_t address);

    // Execute the instruction at pc. nullopt: it ran. A Fault: it did not finish (the step is not counted; for a Break or Syscall pc is
    // already past it).
    std::optional<Fault> step();

    Bus& bus() { return *bus_; }

private:
    struct Load {
        std::uint8_t reg = 0;
        std::uint32_t value = 0;
    };
    std::optional<Fault> execute(const decompile::mips::Insn& insn, std::uint32_t at);
    std::optional<Fault> load(const decompile::mips::Insn& insn, std::uint32_t at, std::uint32_t address);
    std::optional<Fault> store(const decompile::mips::Insn& insn, std::uint32_t at, std::uint32_t address);
    void set(unsigned reg, std::uint32_t value);
    void queue_load(unsigned reg, std::uint32_t value);

    Bus* bus_;
    std::optional<Load> pending_;   // the load whose value arrives after the next instruction
    std::optional<Load> issued_;    // the load this instruction made
};

// What run() starts from: $a0-$a3 and $s0-$s7 (the arguments and the callee-saved registers, absent: zero), $sp, $gp and the other
// registers as zero.
struct RunOptions {
    std::uint64_t max_steps = kMaxSteps;
    std::array<std::uint32_t, 4> a{};
    std::array<std::uint32_t, 8> s{};
    std::uint32_t sp = 0x801FFF00;
    std::uint32_t gp = 0;
    std::array<std::uint32_t, 8> t{};   // $t0-$t7
};

enum class End : std::uint8_t { Return, Stopped, Limit };

struct RunResult {
    End end = End::Limit;
    std::optional<Fault> fault;                   // why it stopped
    std::array<std::uint32_t, 32> r{};            // as the run left them
    std::array<std::uint32_t, 32> gte_data{}, gte_control{};   // the GTE's registers, as the run left them
    std::uint32_t hi = 0, lo = 0;
    std::uint64_t steps = 0;
    std::vector<BusAccess> io;                    // the device accesses of this run, in order
    std::vector<BiosCall> bios;                   // the BIOS calls, in order
    std::string exit() const;                     // "return" | "stopped: <why>" | "limit"
};

// Call the function at `entry` as JAL would ($ra = kReturnAddress) and run it until it returns, stops, or takes `max_steps`
// instructions. What the function wrote is in the bus's memory.
RunResult run(Bus& bus, std::uint32_t entry, const RunOptions& options = {});

// The run as the one run record: the registers as results ($v0 $v1 $s0-$s7 $gp $sp $fp $ra HI LO), the bus's accesses as io, the BIOS
// calls as calls, little-endian, truncated when it hit the step limit; the RunResult is the record's `raw`.
cb::emulate::Run record(const RunResult& result);

// run + record: the platform's runner.
cb::emulate::Run recorded(Bus& bus, std::uint32_t entry, const RunOptions& options = {});

}  // namespace cb::emulate::mips
