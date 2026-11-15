// cb/emulate/sh2.hpp -- the SH-2 (SH7604), to run functions on (small, deterministic, and it logs what a function does to the machine's
// devices).
//
//     sh2::Bus bus;                                    // no memory until the machine adds some
//     bus.add_ram(0x06000000, 0x100000);               // a region; the machine says where its memory is (the Saturn's, the 32X's)
//     bus.load(0x06004000, program);                   // the program as it lies in memory
//     bus.map(0x05FE0000, 0x05FEFFFF, read, write);    // a device range: every access is logged, the callbacks answer
//     sh2::Cpu cpu(bus);
//     cpu.step();                                      // one instruction (a delayed branch and its delay slot are one step)
//     sh2::RunResult r = sh2::run(bus, entry, {.a = {1, 2, 3, 4}});   // call a function as JSR would: run to its RTS
//
// Every instruction cb/decompile/sh2 decodes, run as the SH7604 runs it:
//
//   * a branch with a delay slot (BRA BSR BRAF BSRF JMP JSR RTS RTE BT/S BF/S) runs the instruction behind it first, which sees the registers
//     the branch has not changed (BSR and JSR have set PR; the slot reads PC as its own address + 4); a branch in a delay slot, or an
//     instruction that reads PC and is not allowed there, ends the run ("illegal slot");
//   * DIV0S DIV0U DIV1 step a division exactly as the manual's code does; MAC.W and MAC.L saturate when S is set (MAC.W: MACL with MACH
//     set to 1 on overflow; MAC.L: the 48-bit range), MULS.W MULU.W MUL.L DMULS.L DMULU.L fill MACL and MACH;
//   * the divide unit on the chip (DVSR DVDNT DVCR VCRDIV DVDNTH DVDNTL, at $FFFFFF00 and again at $FFFFFF20) divides when DVDNT (32 / 32)
//     or DVDNTL (64 / 32) is written; its overflow sets DVCR's OVF. The rest of the chip's registers ($FFFFFE00-$FFFFFFFF) hold what is
//     written. Every access to them is logged, like a device's;
//   * a word or longword access that is not aligned, an address nothing answers and an instruction that is not one end the run with the
//     reason; TRAPA and SLEEP end it ("trapa 6", "sleep"): a function is run on its own, without the program's handlers.
//
// Addresses are physical: the top three bits (the cache's: $0xxxxxxx cached, $2xxxxxxx not) are ignored, so a region is seen the way the
// machine mirrors it. A Bus, a Cpu and a run belong to one thread.
#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <optional>
#include <string>
#include <vector>

#include "cb/core/bytes.hpp"

namespace cb::emulate::sh2 {

inline constexpr std::uint64_t kMaxSteps = 200000;
inline constexpr std::uint32_t kPhysicalMask = 0x1FFFFFFF;
inline constexpr std::uint32_t kChipBase = 0xFFFFFE00;   // the on-chip registers, up to $FFFFFFFF
// The return address run() gives: reaching it ends the run (no machine has memory there).
inline constexpr std::uint32_t kReturnAddress = 0x1FFF0000;

// The status register's bits that matter to a run.
namespace sr {
inline constexpr std::uint32_t T = 1, S = 2, Q = 0x100, M = 0x200;
inline constexpr std::uint32_t kMask = 0x3F3;   // the bits a write to SR keeps
}  // namespace sr

// One access to a device range or to the chip's registers: a read or a write of 1, 2 or 4 bytes at a physical address.
struct BusAccess {
    enum class Kind : std::uint8_t { Read, Write };
    Kind kind = Kind::Read;
    std::uint8_t size = 1;
    std::uint32_t address = 0;
    std::uint32_t value = 0;
    std::string text() const;   // "r1" "r2" "r4" "w1" "w2" "w4"
};

// The machine's memory: regions of RAM and ranges answered by callbacks. A device range is looked at first; a range with no read callback
// reads 0, with no write callback ignores the write. Memory the Bus owns starts as zeros.
class Bus {
public:
    using ReadFn = std::function<std::uint32_t(std::uint32_t address, unsigned size)>;
    using WriteFn = std::function<void(std::uint32_t address, unsigned size, std::uint32_t value)>;

    Bus();
    Bus(Bus&&) noexcept = default;
    Bus& operator=(Bus&&) noexcept = default;

    // `size` bytes of memory at physical address `lo` (UsageError when it overlaps a region added already).
    void add_ram(std::uint32_t lo, std::size_t size);
    // Copy `data` to `address`, which must lie in one region (UsageError otherwise).
    void load(std::uint32_t address, ByteView data);
    // Physical addresses lo..hi (inclusive) go to the callbacks and are logged.
    void map(std::uint32_t lo, std::uint32_t hi, ReadFn read = {}, WriteFn write = {});

    // Does an address reach RAM, a device or the chip's registers?
    bool mapped(std::uint32_t address) const;
    // Is it RAM (an instruction can be fetched from it)?
    bool is_ram(std::uint32_t address) const;
    // `size` is 1, 2 or 4; the address is virtual (any cache view). Big-endian. An address that is not mapped reads 0 and drops a write: the
    // caller asks `mapped` first.
    std::uint32_t read(std::uint32_t address, unsigned size);
    void write(std::uint32_t address, unsigned size, std::uint32_t value);
    // A read that is not logged and does not reach a device (an instruction fetch, a test): 0 where there is no RAM.
    std::uint32_t peek(std::uint32_t address, unsigned size) const;

    // The bytes of the region that holds `address` (empty where there is none).
    ByteSpan region(std::uint32_t address);
    ByteView region(std::uint32_t address) const;
    // The physical address where it starts.
    std::uint32_t region_base(std::uint32_t address) const;

    const std::vector<BusAccess>& log() const { return log_; }
    // The chip's registers as the run left them (512 bytes from $FFFFFE00).
    ByteView chip() const { return chip_; }

private:
    struct Region {
        std::uint32_t lo;
        Bytes data;
    };
    struct Range {
        std::uint32_t lo, hi;
        ReadFn read;
        WriteFn write;
    };
    const Region* region_of(std::uint32_t physical) const;
    Region* region_of(std::uint32_t physical);
    const Range* range_of(std::uint32_t physical) const;
    std::uint32_t chip_read(std::uint32_t offset, unsigned size) const;
    void chip_write(std::uint32_t offset, unsigned size, std::uint32_t value);
    std::uint32_t chip_word(std::uint32_t offset) const;
    void set_chip_word(std::uint32_t offset, std::uint32_t value);
    void divide32();
    void divide64();

    std::vector<Region> regions_;
    std::vector<Range> ranges_;
    Bytes chip_;
    std::vector<BusAccess> log_;
};

// Why the CPU cannot go on: the run ends with this reason.
struct Fault {
    enum class Kind : std::uint8_t {
        NoInstruction,   // the bytes at pc are not an instruction
        AddressError,    // a word or longword access that is not aligned, or an instruction fetch that is not
        BusError,        // an address nothing answers
        IllegalSlot,     // a branch, or an instruction that reads PC, in a delay slot
        Trap,            // TRAPA
        Sleep,           // SLEEP
    };
    Kind kind = Kind::NoInstruction;
    std::uint32_t pc = 0;        // where the instruction is
    std::uint32_t address = 0;   // AddressError, BusError: the address
    std::uint32_t code = 0;      // Trap: the vector number
    // "no instruction at 06004000" | "address error at 06004002" | "bus error at 12345678" | "illegal slot at 06004000" | "trapa 6" | "sleep"
    std::string text() const;
};

class Cpu {
public:
    explicit Cpu(Bus& bus);

    std::array<std::uint32_t, 16> r{};
    std::uint32_t pc = 0;                      // the instruction about to run
    std::uint32_t pr = 0, gbr = 0, vbr = 0;
    std::uint32_t mach = 0, macl = 0;
    std::uint32_t sr = 0xF0;                   // interrupts masked, as a reset leaves it
    std::uint64_t steps = 0;                   // instructions executed (a delay slot counts)

    bool t() const { return (sr & sr::T) != 0; }

    // Execute the instruction at pc (with its delay slot when it has one). nullopt: it ran. A Fault: it did not finish.
    std::optional<Fault> step();

    Bus& bus() { return *bus_; }

private:
    // What an instruction does to the flow: nothing, a branch (taken now), or a branch after the delay slot.
    struct Flow {
        bool taken = false;
        bool delayed = false;
        std::uint32_t target = 0;
    };
    std::optional<Fault> execute(std::uint16_t word, std::uint32_t at, bool in_slot, Flow& flow);
    std::optional<Fault> load(std::uint32_t at, std::uint32_t address, unsigned size, std::uint32_t& value);
    std::optional<Fault> store(std::uint32_t at, std::uint32_t address, unsigned size, std::uint32_t value);
    void div1(unsigned rm, unsigned rn);
    std::optional<Fault> mac_w(std::uint32_t at, unsigned rm, unsigned rn);
    std::optional<Fault> mac_l(std::uint32_t at, unsigned rm, unsigned rn);
    void set_t(bool v) { sr = v ? (sr | sr::T) : (sr & ~sr::T); }

    Bus* bus_;
};

// What run() starts from: r4-r7 (the arguments), r8-r14 (callee-saved), the stack pointer, and GBR and VBR.
struct RunOptions {
    std::uint64_t max_steps = kMaxSteps;
    std::array<std::uint32_t, 4> a{};     // r4-r7
    std::array<std::uint32_t, 7> s{};     // r8-r14
    std::uint32_t sp = 0x06040000;
    std::uint32_t gbr = 0, vbr = 0;
    std::uint32_t r0 = 0;
};

enum class End : std::uint8_t { Return, Stopped, Limit };

struct RunResult {
    End end = End::Limit;
    std::optional<Fault> fault;                   // why it stopped
    std::array<std::uint32_t, 16> r{};            // as the run left them
    std::uint32_t pr = 0, gbr = 0, vbr = 0, mach = 0, macl = 0, sr = 0;
    std::uint64_t steps = 0;
    std::vector<BusAccess> io;                    // the device and chip accesses of this run, in order
    std::string exit() const;                     // "return" | "stopped: <why>" | "limit"
};

// Call the function at `entry` as JSR would (PR = kReturnAddress) and run it until it returns, stops, or takes `max_steps` instructions.
// What the function wrote is in the bus's memory.
RunResult run(Bus& bus, std::uint32_t entry, const RunOptions& options = {});

}  // namespace cb::emulate::sh2
