// cb/emulate/r4300.hpp -- the Nintendo 64's R4300i, to run functions on (small, deterministic, and it logs what a function does to the
// machine's registers).
//
//     r4300::Bus bus(rdram_image, rom);                  // 8 MB RDRAM, the cartridge, SP memory, the RCP's registers (logged)
//     r4300::Cpu cpu(bus);
//     cpu.step();                                        // one instruction
//     r4300::RunResult r = r4300::run(bus, entry, {.a = {1, 2, 3, 4}});   // call a function as JAL would: run to its return
//     cb::emulate::Run record = r4300::record(r);        // the one run record the judge compares
//
// Every instruction cb/decompile/mips decodes for the R4300 variant, as user-mode programs run them: the 64-bit registers (every 32-bit
// operation sign-extends its result to 64 bits), the doubleword loads, stores and arithmetic, LL/SC, the unaligned loads and stores (big-endian),
// branch likely (the delay slot is skipped when the branch is not taken), the traps, MULT/DIV in 32 and 64 bits, COP0 (the registers, the
// TLB with its page sizes, TLBR TLBWI TLBWR TLBP, ERET), and the FPU: single and double (and the integer conversions), with the status
// register's FR bit choosing between 32 and 16 double registers, the control registers FCR0 and FCR31, and the compare condition bit.
//
// What is approximate, and why it does not matter to a function's behaviour as the judge sees it:
//
//   * an exception is not delivered: a SYSCALL, BREAK, trap, arithmetic overflow, TLB miss, address error or bus error ends the run with its
//     reason (the run's `fault`), and what the program's own handler would have done after it is not run;
//   * the Count register counts one per instruction (the hardware counts one per two cycles), Random is derived from the step count, and the
//     Compare interrupt is never raised; there is no cache (CACHE does nothing: KSEG0 and KSEG1 are one memory);
//   * FPU arithmetic uses the host's IEEE operations in round-to-nearest whatever FCR31's rounding mode says (the integer conversions obey it),
//     and the FPU's exception flags are not set; an operation the R4300 reports as "unimplemented" (a conversion of a NaN or a value out of
//     range) ends the run as one this interpreter does not run.
//
// The machine: 8 MB of RDRAM at physical $00000000 (the console has 4 MB, 8 MB with the Expansion Pak), the RCP's register blocks (RDRAM,
// SP, DP, MI, VI, AI, PI, RI, SI: every access is logged with its physical address; a register keeps what was written to it and reads back
// as written, except the status registers a program polls: SP_STATUS reads "halted", VI_CURRENT counts scanlines, the others read zero),
// SP DMEM and IMEM (8 KB of memory), PIF RAM, and the cartridge ROM at $10000000 (read-only, and the source of a PI DMA: a write to PI_WR_LEN
// copies from the ROM to RDRAM at once). The RSP and the RDP are not run: what a function writes to their registers (and to DMEM/IMEM, where
// the RSP's microcode and the RDP's display lists are put) is logged, and that is the effect compared.
//
// Addresses: KSEG0 ($80000000) and KSEG1 ($A0000000) are the first 512 MB of the physical space; the rest of the 32-bit space goes through the
// TLB. A Bus, a Cpu and a run belong to one thread.
#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#include "cb/core/bytes.hpp"
#include "cb/emulate/run.hpp"

namespace cb::emulate::r4300 {

inline constexpr std::uint64_t kMaxSteps = 200000;
inline constexpr std::size_t kRdramSize = 0x800000;
inline constexpr std::uint32_t kRomBase = 0x10000000;           // the cartridge, as the PI sees it (physical)
inline constexpr std::uint32_t kRomEnd = 0x1FBFFFFF;
inline constexpr std::uint32_t kDmem = 0x04000000;               // SP DMEM, then IMEM
inline constexpr std::uint32_t kPifRam = 0x1FC007C0;
// The return address run() gives: reaching it ends the run (a word of KSEG1 no program has code at).
inline constexpr std::uint32_t kReturnAddress = 0xBFC0FF00;

// The RCP's register blocks, by physical address (inclusive ranges).
struct Block {
    const char* name;
    std::uint32_t lo, hi;
};
inline constexpr std::array<Block, 12> kBlocks = {{
    {"RDRAM", 0x03F00000, 0x03FFFFFF}, {"SP", 0x04040000, 0x0408FFFF}, {"DP", 0x04100000, 0x042FFFFF}, {"MI", 0x04300000, 0x043FFFFF},
    {"VI", 0x04400000, 0x044FFFFF},    {"AI", 0x04500000, 0x045FFFFF}, {"PI", 0x04600000, 0x046FFFFF}, {"RI", 0x04700000, 0x047FFFFF},
    {"SI", 0x04800000, 0x048FFFFF},    {"DOM2", 0x05000000, 0x07FFFFFF}, {"SRAM", 0x08000000, 0x0FFFFFFF}, {"PIF", 0x1FC00000, 0x1FC007BF},
}};

// One access to a register block: a read or a write of 1, 2, 4 or 8 bytes at a physical address.
struct BusAccess {
    enum class Kind : std::uint8_t { Read, Write };
    Kind kind = Kind::Read;
    std::uint8_t size = 4;
    std::uint32_t address = 0;
    std::uint64_t value = 0;
    std::string text() const;   // "r1" "r2" "r4" "r8" "w1" ...
};

// The memory and the registers. The last range mapped that holds an address answers it; a range with no read callback reads what was last
// written there, and a write with no write callback is kept.
class Bus {
public:
    using ReadFn = std::function<std::uint64_t(std::uint32_t address, unsigned size)>;
    using WriteFn = std::function<void(std::uint32_t address, unsigned size, std::uint64_t value)>;

    Bus();
    // A copy of `rdram` at address 0 (UsageError when it is larger than 8 MB), and the cartridge's bytes.
    explicit Bus(ByteView rdram, ByteView rom = {});

    // Physical addresses lo..hi (inclusive) go to the callbacks and are logged; this takes over from the default registers.
    void map(std::uint32_t lo, std::uint32_t hi, ReadFn read = {}, WriteFn write = {});

    // Is the physical address RDRAM, SP memory, the cartridge or a register?
    bool mapped(std::uint32_t physical) const;
    // `size` is 1, 2, 4 or 8, big-endian. A read of an address that is not mapped is 0 and a write is dropped: ask `mapped` first.
    std::uint64_t read(std::uint32_t physical, unsigned size);
    void write(std::uint32_t physical, unsigned size, std::uint64_t value);

    ByteSpan rdram() { return rdram_; }
    ByteView rdram() const { return rdram_; }
    ByteSpan sp_memory() { return sp_; }      // DMEM then IMEM
    ByteView rom() const { return rom_; }
    const std::vector<BusAccess>& log() const { return log_; }

private:
    struct Range {
        std::uint32_t lo, hi;
        ReadFn read;
        WriteFn write;
    };
    const Range* range_of(std::uint32_t physical) const;
    std::uint64_t register_read(std::uint32_t physical, unsigned size);
    void register_write(std::uint32_t physical, unsigned size, std::uint64_t value);
    void pi_dma(std::uint32_t length);

    Bytes rdram_;
    Bytes sp_;
    Bytes pif_;
    Bytes rom_;
    std::vector<Range> ranges_;
    std::vector<std::pair<std::uint32_t, std::uint8_t>> latched_;   // register bytes written (kept sorted by address)
    std::uint32_t scanline_ = 0;
    std::vector<BusAccess> log_;
};

// Why the CPU cannot go on: the run ends with this reason.
struct Fault {
    enum class Kind : std::uint8_t {
        NoInstruction,   // the word at pc is not an instruction (a reserved one)
        NotRun,          // an instruction this interpreter does not run (`mnemonic`)
        AddressError,    // a load or store that is not aligned, or an instruction fetch that is not
        BusError,        // a physical address nothing answers
        TlbMiss,         // a mapped address with no valid TLB entry (or a write to a page that is not dirty)
        Overflow,        // ADD, ADDI, SUB, DADD, DADDI or DSUB overflowed
        Trap,            // TEQ, TNE, TGE ... taken
        Break,
        Syscall,
    };
    Kind kind = Kind::NoInstruction;
    std::uint32_t pc = 0;        // where the instruction is
    std::uint32_t address = 0;   // AddressError, BusError, TlbMiss: the address
    std::string mnemonic;        // NotRun: which
    std::uint32_t code = 0;      // Break, Syscall: the code
    // "no instruction at 80001000" | "not run: cvt.w.s" | "address error at 80001002" | "bus error at 12345678" | "tlb miss at 00400000" |
    // "arithmetic overflow" | "trap" | "break 7" | "syscall"
    std::string text() const;
};

// One TLB entry as TLBWI writes it.
struct TlbEntry {
    std::uint32_t mask = 0;     // PageMask: 0 (4 KB), 0x6000 (16 KB) ... 0x1FFE000 (16 MB)
    std::uint64_t hi = 0;       // EntryHi: VPN2 and ASID
    std::uint64_t lo0 = 0, lo1 = 0;
};

class Cpu {
public:
    explicit Cpu(Bus& bus);

    std::array<std::uint64_t, 32> r{};         // $0-$31 ($0 stays zero)
    std::uint32_t pc = 0;                      // the instruction about to run (its address, as the low 32 bits of the sign-extended address)
    std::uint32_t next_pc = 4;                 // the one after it (what a branch in the delay slot's place changes)
    std::uint64_t hi = 0, lo = 0;
    std::array<std::uint64_t, 32> cop0{};      // the system control coprocessor's registers
    std::array<std::uint64_t, 32> fpr{};       // the FPU's registers, 64 bits each (FR = 1; with FR = 0 a double is an even and an odd register)
    std::uint32_t fcr0 = 0x0A00;               // the FPU's implementation and revision
    std::uint32_t fcr31 = 0;                   // its control and status: rounding mode, flags, bit 23 the compare condition
    std::array<TlbEntry, 32> tlb{};
    bool ll_bit = false;
    std::uint64_t steps = 0;                   // instructions executed

    // Start at `address`: pc there, the instruction after it next (no branch pending).
    void jump_to(std::uint32_t address);

    // Execute the instruction at pc. nullopt: it ran. A Fault: it did not finish (the step is not counted; for a Break or Syscall pc is
    // already past it).
    std::optional<Fault> step();

    Bus& bus() { return *bus_; }

    // The physical address of a virtual one, or the fault that failed it.
    std::optional<Fault> translate(std::uint32_t virtual_address, bool write, std::uint32_t at, std::uint32_t& physical) const;

private:
    std::optional<Fault> execute(std::uint32_t word, std::uint32_t at);
    std::optional<Fault> special(std::uint32_t word, std::uint32_t at);
    std::optional<Fault> regimm(std::uint32_t word, std::uint32_t at);
    std::optional<Fault> memory(std::uint32_t word, std::uint32_t at);
    std::optional<Fault> system_control(std::uint32_t word, std::uint32_t at);
    std::optional<Fault> fpu(std::uint32_t word, std::uint32_t at);
    std::optional<Fault> fpu_arithmetic(std::uint32_t word, std::uint32_t at);
    void branch(bool taken, std::uint32_t target, bool likely);
    std::optional<Fault> load(std::uint32_t address, unsigned size, std::uint32_t at, std::uint64_t& value);
    std::optional<Fault> store(std::uint32_t address, unsigned size, std::uint32_t at, std::uint64_t value);
    void set(unsigned reg, std::uint64_t value) {
        if (reg != 0) r[reg] = value;
    }
    bool fr() const { return (cop0[12] & (1u << 26)) != 0; }
    std::uint32_t fpu_read32(unsigned n) const;
    void fpu_write32(unsigned n, std::uint32_t v);
    std::uint64_t fpu_read64(unsigned n) const;
    void fpu_write64(unsigned n, std::uint64_t v);

    Bus* bus_;
};

// What run() starts from: $a0-$a3 and $s0-$s7 (the arguments and the callee-saved registers, absent: zero), $sp, $gp and the other
// registers as zero. A 32-bit argument is sign-extended into its 64-bit register.
struct RunOptions {
    std::uint64_t max_steps = kMaxSteps;
    std::array<std::uint32_t, 4> a{};
    std::array<std::uint32_t, 8> s{};
    std::uint32_t sp = 0x803FFF00;
    std::uint32_t gp = 0;
    std::array<std::uint32_t, 8> t{};   // $t0-$t7
};

enum class End : std::uint8_t { Return, Stopped, Limit };

struct RunResult {
    End end = End::Limit;
    std::optional<Fault> fault;                   // why it stopped
    std::array<std::uint64_t, 32> r{};            // as the run left them
    std::array<std::uint64_t, 32> fpr{};
    std::uint64_t hi = 0, lo = 0;
    std::uint32_t fcr31 = 0;
    std::uint64_t steps = 0;
    std::vector<BusAccess> io;                    // the register accesses of this run, in order
    std::string exit() const;                     // "return" | "stopped: <why>" | "limit"
};

// Call the function at `entry` as JAL would ($ra = kReturnAddress) and run it until it returns, stops, or takes `max_steps`
// instructions. What the function wrote is in the bus's memory. The CPU starts as the boot code leaves it: Status = $34000000 (COP0, COP1
// and FR on), Config, PRId.
RunResult run(Bus& bus, std::uint32_t entry, const RunOptions& options = {});

// The run as the one run record: the registers as results ($v0 $v1 $s0-$s7 $gp $sp $fp $ra HI LO, and $f0 and $f20-$f30 as bits), the
// register accesses as io, big-endian, truncated when it hit the step limit; the RunResult is the record's `raw`.
cb::emulate::Run record(const RunResult& result);

// run + record: the platform's runner.
cb::emulate::Run recorded(Bus& bus, std::uint32_t entry, const RunOptions& options = {});

}  // namespace cb::emulate::r4300
