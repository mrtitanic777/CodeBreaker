// emulate/sh2.hpp -- the SH-2 (SH7604), to run functions on (small, deterministic, and it logs what a function does to the machine's
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
//     reason; TRAPA and SLEEP end it ("trapa 6","trapa 6" | "sleep" | "fpu disabled at 8c010000"): a function is run on its own, without the program's handlers.
//
// The SH-4 (SH7750, the Dreamcast's CPU) is a variant of the same core (`sh2::Bus bus(Variant::Sh4)`): little-endian memory and instruction words, the
// SH-2's instructions, and its own (emulate/sh4.cpp):
//
//   * the FPU: 16 single registers in two banks (FPSCR.FR says which is FR0-FR15 and which XF0-XF15), single and double operations by FPSCR.PR, pair
//     moves by FPSCR.SZ (a pair is DRn or XDn, FR[n] the high word), FIPR, FTRV (XMTRX x a vector), FSCA, FSRRA, FMAC, FLOAT/FTRC, FCNVSD/FCNVDS,
//     FSCHG and FRCHG; the exception flags and the rounding mode are not modelled (FTRC truncates, the rest round to nearest);
//   * SR with MD, RB, BL and FD: writing RB swaps R0-R7 with the other bank (LDC and STC reach it as r0_bank..r7_bank), RTE restores PC and SR from
//     SPC and SSR, and with FD set an FPU instruction ends the run; SSR SPC SGR DBR and the FPU's FPUL and FPSCR are registers;
//   * SHAD SHLD, MOVCA.L, and the cache and TLB instructions (PREF OCBI OCBP OCBWB LDTLB) as logged state: each is one entry in the bus log. A PREF
//     of the store queue area (-) copies the 32 bytes written there to the address QACR0/QACR1 and the low bits of the address give;
//   * the on-chip registers (-, and the cache and TLB arrays at -: CCR, MMUCR, PTEH, the BSC, the DMAC, the TMU,
//     the SCI and SCIF ...) hold what is written and every access to them is logged; there is no divide unit.
//
// Addresses are physical: the top three bits (the cache's: $0xxxxxxx cached, $2xxxxxxx not) are ignored, so a region is seen the way the
// machine mirrors it. A Bus, a Cpu and a run belong to one thread.
#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <map>
#include <optional>
#include <string>
#include <vector>

#include "core/bytes.hpp"
#include "decompile/sh2.hpp"

namespace cb::emulate::sh2 {

using Variant = decompile::sh2::Variant;

inline constexpr std::uint64_t kMaxSteps = 200000;
inline constexpr std::uint32_t kPhysicalMask = 0x1FFFFFFF;
inline constexpr std::uint32_t kChipBase = 0xFFFFFE00;   // the SH-2's on-chip registers, up to $FFFFFFFF
inline constexpr std::uint32_t kP4Base = 0xE0000000;     // the SH-4's P4 area: the store queues, the cache and TLB arrays, the on-chip registers
// The return address run() gives: reaching it ends the run (no machine has memory there).
inline constexpr std::uint32_t kReturnAddress = 0x1FFF0000;

// The status register's bits that matter to a run.
namespace sr {
inline constexpr std::uint32_t T = 1, S = 2, Q = 0x100, M = 0x200;
inline constexpr std::uint32_t kMask = 0x3F3;   // the bits a write to SR keeps
inline constexpr std::uint32_t kMaskSh4 = 0x700083F3;   // the SH-4's: MD RB BL FD and the interrupt mask too
inline constexpr std::uint32_t FD = 0x8000, BL = 0x10000000, RB = 0x20000000, MD = 0x40000000;
}  // namespace sr

// FPSCR's bits that matter to a run.
namespace fpscr {
inline constexpr std::uint32_t PR = 1u << 19, SZ = 1u << 20, FR = 1u << 21;
inline constexpr std::uint32_t kMask = 0x003FFFFF;
}  // namespace fpscr

// One access to a device range or to the chip's registers: a read or a write of 1, 2 or 4 bytes at a physical address.
struct BusAccess {
    // Call: a BIOS stub was entered (the address is the stub's, the value its selector); Cache: a cache or TLB instruction ran (the value says
    // which: 0 PREF, 1 OCBI, 2 OCBP, 3 OCBWB, 4 MOVCA.L, 5 LDTLB)
    enum class Kind : std::uint8_t { Read, Write, Call, Cache };
    Kind kind = Kind::Read;
    std::uint8_t size = 1;
    std::uint32_t address = 0;
    std::uint32_t value = 0;
    std::string text() const;   // "r1" "r2" "r4" "w1" "w2" "w4"; "call" and "cache" for the other two
};

// The machine's memory: regions of RAM and ranges answered by callbacks. A device range is looked at first; a range with no read callback
// reads 0, with no write callback ignores the write. Memory the Bus owns starts as zeros.
class Bus {
public:
    using ReadFn = std::function<std::uint32_t(std::uint32_t address, unsigned size)>;
    using WriteFn = std::function<void(std::uint32_t address, unsigned size, std::uint32_t value)>;

    Bus();
    explicit Bus(Variant variant);
    Bus(Bus&&) noexcept = default;
    Bus& operator=(Bus&&) noexcept = default;

    // `size` bytes of memory at physical address `lo` (UsageError when it overlaps a region added already).
    void add_ram(std::uint32_t lo, std::size_t size);
    // Copy `data` to `address`, which must lie in one region (UsageError otherwise).
    void load(std::uint32_t address, ByteView data);
    // Physical addresses lo..hi (inclusive) go to the callbacks and are logged.
    void map(std::uint32_t lo, std::uint32_t hi, ReadFn read = {}, WriteFn write = {});
    // Running an instruction at an address in lo..hi calls `entered` first (with the registers): how the machines log a BIOS stub being called.
    using EnterFn = std::function<void(std::uint32_t pc, const std::array<std::uint32_t, 16>& regs)>;
    void on_enter(std::uint32_t lo, std::uint32_t hi, EnterFn entered);
    void enter(std::uint32_t pc, const std::array<std::uint32_t, 16>& regs) const;
    bool has_enter_hooks() const { return !enter_.empty(); }
    // One entry in the log that is not a memory access.
    void note(BusAccess::Kind kind, std::uint32_t address, std::uint32_t value);
    // The SH-4's store queue: a PREF of an address in $E0000000-$E3FFFFFF copies its 32 bytes to external memory.
    void flush_store_queue(std::uint32_t address);
    Variant variant() const { return variant_; }
    bool little_endian() const { return variant_ == Variant::Sh4; }

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
    bool is_chip(std::uint32_t address) const { return address >= (variant_ == Variant::Sh4 ? kP4Base : kChipBase); }
    std::uint32_t p4_read(std::uint32_t address, unsigned size);
    void p4_write(std::uint32_t address, unsigned size, std::uint32_t value);
    std::uint32_t chip_read(std::uint32_t offset, unsigned size) const;
    void chip_write(std::uint32_t offset, unsigned size, std::uint32_t value);
    std::uint32_t chip_word(std::uint32_t offset) const;
    void set_chip_word(std::uint32_t offset, std::uint32_t value);
    void divide32();
    void divide64();

    struct Enter {
        std::uint32_t lo, hi;
        EnterFn fn;
    };
    Variant variant_ = Variant::Sh2;
    std::vector<Region> regions_;
    std::vector<Range> ranges_;
    std::vector<Enter> enter_;
    Bytes chip_;
    std::map<std::uint32_t, std::uint8_t> p4_;                // the SH-4's P4 registers, byte by byte (what was written)
    std::array<std::uint32_t, 16> store_queue_{};              // SQ0 and SQ1: eight longwords each
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
        FpuDisabled,     // an FPU instruction with SR.FD set (SH-4)
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

    // The SH-4's (Variant::Sh4); sr then starts as a reset leaves it, 0x700000F0.
    std::uint32_t ssr = 0, spc = 0, sgr = 0, dbr = 0;
    std::array<std::uint32_t, 8> bank{};       // R0-R7 of the register bank SR.RB does not select (r0_bank..r7_bank to LDC and STC)
    std::uint32_t fpscr = 0, fpul = 0;
    std::array<std::array<std::uint32_t, 16>, 2> fp{};   // the two banks of floats: fp[FPSCR.FR] is FR0-FR15, the other XF0-XF15 (bit patterns)

    bool sh4() const { return sh4_; }
    std::uint32_t fr(unsigned n) const { return fp[(fpscr & fpscr::FR) != 0 ? 1 : 0][n & 15]; }
    std::uint32_t xf(unsigned n) const { return fp[(fpscr & fpscr::FR) != 0 ? 0 : 1][n & 15]; }
    // Write SR as LDC does: the bits the SH-4 keeps, with R0-R7 swapped with the other bank when RB changes.
    void set_sr(std::uint32_t value);
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
        bool restore_sr = false;   // RTE (SH-4): SR comes back from SSR after the delay slot, which runs in the handler's register bank
        std::uint32_t new_sr = 0;
    };
    std::optional<Fault> execute(std::uint16_t word, std::uint32_t at, bool in_slot, Flow& flow);
    std::optional<Fault> load(std::uint32_t at, std::uint32_t address, unsigned size, std::uint32_t& value);
    std::optional<Fault> store(std::uint32_t at, std::uint32_t address, unsigned size, std::uint32_t value);
    void div1(unsigned rm, unsigned rn);
    std::optional<Fault> mac_w(std::uint32_t at, unsigned rm, unsigned rn);
    std::optional<Fault> mac_l(std::uint32_t at, unsigned rm, unsigned rn);
    void set_t(bool v) { sr = v ? (sr | sr::T) : (sr & ~sr::T); }

    // The SH-4's instructions (emulate/sh4.cpp): handled = false leaves the word to the SH-2's code.
    struct Sh4Outcome {
        bool handled = false;
        std::optional<Fault> fault;
    };
    Sh4Outcome execute_sh4(std::uint16_t word, std::uint32_t at, bool in_slot, Flow& flow);
    std::optional<Fault> sh4_fpu(std::uint16_t word, std::uint32_t at);
    std::optional<Fault> sh4_fmov(std::uint16_t word, std::uint32_t at);

    Bus* bus_;
    bool sh4_ = false;
};

// What run() starts from: r4-r7 (the arguments), r8-r14 (callee-saved), the stack pointer, and GBR and VBR.
struct RunOptions {
    std::uint64_t max_steps = kMaxSteps;
    std::array<std::uint32_t, 4> a{};     // r4-r7
    std::array<std::uint32_t, 7> s{};     // r8-r14
    std::uint32_t sp = 0x06040000;
    std::uint32_t gbr = 0, vbr = 0;
    std::uint32_t r0 = 0;
    // SH-4: FPSCR, and FR4-FR11 (the float arguments of the compiler's calling convention; FR0 is the result)
    std::uint32_t fpscr = 0;
    std::array<std::uint32_t, 8> fa{};
};

enum class End : std::uint8_t { Return, Stopped, Limit };

struct RunResult {
    End end = End::Limit;
    std::optional<Fault> fault;                   // why it stopped
    std::array<std::uint32_t, 16> r{};            // as the run left them
    std::uint32_t pr = 0, gbr = 0, vbr = 0, mach = 0, macl = 0, sr = 0;
    std::uint32_t fpscr = 0, fpul = 0;            // SH-4
    std::array<std::uint32_t, 16> fr{};           // SH-4: FR0-FR15 (the bank FPSCR.FR selects) as the run left them
    std::uint64_t steps = 0;
    std::vector<BusAccess> io;                    // the device and chip accesses of this run, in order
    std::string exit() const;                     // "return" | "stopped: <why>" | "limit"
};

// Call the function at `entry` as JSR would (PR = kReturnAddress) and run it until it returns, stops, or takes `max_steps` instructions.
// What the function wrote is in the bus's memory.
RunResult run(Bus& bus, std::uint32_t entry, const RunOptions& options = {});

}  // namespace cb::emulate::sh2
