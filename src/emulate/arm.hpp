// emulate/arm.hpp -- the Game Boy Advance's ARM7TDMI, to run functions on (small, deterministic, and it logs what a function does to the
// machine's registers and to the BIOS).
//
//     arm::Bus bus;                                  // the memory map; bus.set_rom(image) holds the cartridge; map(lo, hi, read, write) hands ranges to callbacks
//     arm::Cpu cpu(bus);
//     cpu.step();                                    // one instruction
//     arm::RunResult r = arm::run(bus, entry, {.a = {1, 2, 3, 4}});   // call a function as BL would: run to its return
//     cb::emulate::Run record = arm::record(r);      // the one run record the judge compares
//
// Every instruction cb/decompile/arm decodes, in both states: the condition codes and the shifter's carry, the flags of the arithmetic
// (N Z C V), MUL and the long multiplies, the loads and stores of every size with their quirks (a misaligned word load rotates, a
// misaligned halfword load rotates, a misaligned LDRSH is an LDRSB, stores align down), LDM and STM in all four modes with writeback and
// the base in the list, SWP, B BL BX with the state switch, MRS and MSR with the banked registers of the seven modes (r13 and r14 of
// each exception mode, r8-r12 of FIQ, the SPSRs), and the flags restored from the SPSR by `movs pc, lr` and `ldm ^`. A run starts in
// System mode (what the GBA's BIOS leaves a game in).
//
// The memory map is the console's: BIOS ROM ($00000000, 16 KB), the 256 KB of work RAM ($02000000) and 32 KB of internal RAM
// ($03000000) -- each seen again every time its size repeats, I/O registers ($04000000-$040003FE: kept, and every access to them logged),
// palette ($05000000, 1 KB), video RAM ($06000000, 96 KB), object attributes ($07000000, 1 KB), the cartridge ROM ($08000000, $0A000000,
// $0C000000: up to 32 MB each; a read past the image gives the address bus's value) and the save memory ($0E000000, 64 KB). Writes
// of a byte to the palette or the background part of video RAM write the halfword, and a byte write to the object attributes is dropped,
// as the hardware does. Any other address, and a fetch from the BIOS or I/O, ends the run with the reason.
//
// A software interrupt is the BIOS's: it is logged with its number (the comment's second byte in ARM state, its first in Thumb) and its
// four argument registers, and the calls that are pure arithmetic or memory moves run as the BIOS does them (Div, DivArm, Sqrt, CpuSet,
// CpuFastSet); any other returns 0 in r0 and changes nothing else. What a function asks of the BIOS is the effect the judge compares.
//
// The Nintendo DS's two processors run on the same code (`System::Nds9`, the ARM946E-S: ARMv5TE, with BLX, CLZ, the saturating and 16-bit multiplies,
// LDRD and STRD, PLD, and coprocessor 15; a load or pop into the pc switches state; `System::Nds7`, the ARM7TDMI: as the GBA's) over the DS's memory
// map: 4 MB of main RAM ($02000000, repeated), the 32 KB of shared WRAM ($03000000; the ARM7 also has 64 KB of its own at $03800000), the ARM9's
// instruction TCM (32 KB at $00000000) and data TCM (16 KB, at $027C0000 until a write to CP15 register 9 moves it), I/O registers
// ($04000000-$04001FFF: kept, every access logged), palette (2 KB), video memory (the two engines' background and object memory and the LCDC
// window, not bank-switched: each is one block), object attributes (2 KB). A program is placed with `load()` (the cartridge is not mapped). Byte
// writes to palette, video and OAM are dropped on the ARM9, as the hardware does. Coprocessor 15 reads and writes are kept as accesses at
// $FFF00000 + (opcode1 << 12 | CRn << 8 | CRm << 4 | opcode2). BIOS calls are the DS's numbering (Div is 9, Sqrt 13; CpuSet and CpuFastSet
// run). A Bus, a Cpu and a run belong to one thread.
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
#include "decompile/arm.hpp"
#include "emulate/paged_memory.hpp"
#include "emulate/run.hpp"

namespace cb::emulate::arm {

// Which machine the memory map and the instruction set are of.
enum class System : std::uint8_t { Gba, Nds9, Nds7, N3ds, Vita };

inline constexpr std::uint64_t kMaxSteps = 200000;
inline constexpr std::uint32_t kBiosBase = 0x00000000, kBiosSize = 0x4000;
inline constexpr std::uint32_t kEwramBase = 0x02000000, kEwramSize = 0x40000;
inline constexpr std::uint32_t kIwramBase = 0x03000000, kIwramSize = 0x8000;
inline constexpr std::uint32_t kIoBase = 0x04000000, kIoSize = 0x400;
inline constexpr std::uint32_t kPaletteBase = 0x05000000, kPaletteSize = 0x400;
inline constexpr std::uint32_t kVramBase = 0x06000000, kVramSize = 0x18000;
inline constexpr std::uint32_t kOamBase = 0x07000000, kOamSize = 0x400;
inline constexpr std::uint32_t kRomBase = 0x08000000;
inline constexpr std::uint32_t kSramBase = 0x0E000000, kSramSize = 0x10000;
// The return address run() gives: reaching it ends the run (an address past the memory map).
inline constexpr std::uint32_t kReturnAddress = 0x10000000;
// The DS: the main RAM, the shared WRAM, the ARM7's own WRAM, the TCMs and where the data TCM starts.
inline constexpr std::uint32_t kNdsMainSize = 0x400000, kNdsSharedSize = 0x8000, kNdsWram7Base = 0x03800000, kNdsWram7Size = 0x10000;
inline constexpr std::uint32_t kNdsItcmSize = 0x8000, kNdsDtcmSize = 0x4000, kNdsDtcmBase = 0x027C0000, kNdsIoSize = 0x2000;
inline constexpr std::uint32_t kNdsCp15Base = 0xFFF00000, kNdsVramSize = 0x1A4000;

// One access to an I/O register (or to a range a map() call took over): a read or a write of 1, 2 or 4 bytes at an address.
struct BusAccess {
    enum class Kind : std::uint8_t { Read, Write };
    Kind kind = Kind::Read;
    std::uint8_t size = 1;
    std::uint32_t address = 0;
    std::uint32_t value = 0;
    std::string text() const;   // "r1" "r2" "r4" "w1" "w2" "w4"
};

// A call into the BIOS: `function` is the number of the software interrupt.
struct BiosCall {
    std::uint32_t function = 0;
    std::string name;   // an imported function's name (the Vita lane): the call is by name, not by number
    std::array<std::uint32_t, 4> args{};
    std::string text() const;   // "SWI:06(1,2,3,4)"
};

// The memory map. The last range mapped that holds an address answers it (the registers, and any region): a range with no read callback
// reads what the I/O register or memory holds, with no write callback writes it.
class Bus {
public:
    using ReadFn = std::function<std::uint32_t(std::uint32_t address, unsigned size)>;
    using WriteFn = std::function<void(std::uint32_t address, unsigned size, std::uint32_t value)>;

    explicit Bus(System system = System::Gba);

    System system() const { return system_; }
    decompile::arm::Arch arch() const {
        switch (system_) {
            case System::Nds9: return decompile::arm::Arch::V5TE;
            case System::N3ds: return decompile::arm::Arch::V6K;
            case System::Vita: return decompile::arm::Arch::V7A;
            default: return decompile::arm::Arch::V4T;
        }
    }
    // The 3DS and the Vita have a whole address space of paged memory (emulate/paged_memory) instead of the handheld maps above: the program's loaded
    // segments are shared, ranges mapped with map() are the logged hardware, and the rest of the process's addresses are RAM.
    bool paged() const { return system_ == System::N3ds || system_ == System::Vita; }
    void set_regions(paged_memory::Regions regions) { paged_ = paged_memory::Memory(std::move(regions)); }
    paged_memory::Memory& memory() { return paged_; }
    const paged_memory::Memory& memory() const { return paged_; }

    // The cartridge ROM, seen at $08000000 (up to 32 MB). The bytes are borrowed and must outlive the bus.
    void set_rom(ByteView rom) { rom_ = rom; }
    // Addresses lo..hi (inclusive, as the program writes them) go to the callbacks and are logged.
    void map(std::uint32_t lo, std::uint32_t hi, ReadFn read = {}, WriteFn write = {});
    // Bytes placed at an address in memory with no access logged (a program loaded; the part of it that lies in no memory is dropped).
    void load(std::uint32_t address, ByteView data);
    // Where the ARM9's data TCM is (a write to CP15 register 9 does this): a base and a size in bytes (0 disables it).
    void set_dtcm(std::uint32_t base, std::uint32_t size) { dtcm_base_ = base; dtcm_size_ = size; }
    // Coprocessor 15: what an MRC reads and an MCR writes (the first access in a run is logged by the CPU).
    std::uint32_t cp15_read(std::uint32_t reg);
    void cp15_write(std::uint32_t reg, std::uint32_t value);
    void log_access(BusAccess::Kind kind, unsigned size, std::uint32_t address, std::uint32_t value) { log_.push_back({kind, static_cast<std::uint8_t>(size), address, value}); }

    // A big-endian system (the 3DO's ARM60): the memory holds each aligned word with its bytes reversed (the caller loads it that way, so a
    // word access sees the program's value), and a byte access goes to the byte `address ^ 3` of it, a halfword to `address ^ 2`.
    void set_big_endian(bool on) { big_endian_ = on; }

    // Does an address reach memory, a register, or a mapped range?
    bool mapped(std::uint32_t address) const;
    // The bytes of `size` at an address when they are plain memory of the Game Boy Advance (the ROM, EWRAM or IWRAM, no mapped hardware in the bus): read without a
    // log or a call; null otherwise.
    const std::uint8_t* plain_block(std::uint32_t address, std::size_t size);
    // `size` is 1, 2 or 4 and the address is aligned to it (the CPU aligns; a read of an address that is not mapped reads 0).
    std::uint32_t read(std::uint32_t address, unsigned size);
    void write(std::uint32_t address, unsigned size, std::uint32_t value);

    ByteSpan ewram() { return ewram_; }
    ByteSpan iwram() { return iwram_; }
    ByteSpan io() { return io_; }
    ByteSpan palette() { return palette_; }
    ByteSpan vram() { return vram_; }
    ByteSpan oam() { return oam_; }
    ByteSpan sram() { return sram_; }
    ByteSpan itcm() { return itcm_; }
    ByteSpan dtcm() { return dtcm_; }
    ByteSpan wram7() { return wram7_; }
    const std::vector<BusAccess>& log() const { return log_; }

private:
    struct Range {
        std::uint32_t lo, hi;
        ReadFn read;
        WriteFn write;
    };
    const Range* range_of(std::uint32_t address) const;
    std::uint8_t* place(std::uint32_t address, std::size_t& room);   // writable memory at an address (null for none)

    System system_;
    ByteView rom_;
    Bytes ewram_, iwram_, io_, palette_, vram_, oam_, sram_;
    Bytes itcm_, dtcm_, wram7_;                      // the DS's
    std::uint32_t dtcm_base_ = kNdsDtcmBase, dtcm_size_ = kNdsDtcmSize;
    paged_memory::Memory paged_;
    std::map<std::uint32_t, std::uint32_t> cp15_;
    bool big_endian_ = false;
    std::vector<Range> ranges_;
    std::vector<BusAccess> log_;
};

// Why the CPU cannot go on: the run ends with this reason.
struct Fault {
    enum class Kind : std::uint8_t {
        NoInstruction,   // the bytes at pc are not an instruction
        NotRun,          // an instruction this interpreter does not run (`mnemonic`)
        BusError,        // an address nothing answers (a fetch, a load or a store)
        Swi,             // a software interrupt (run() answers it as the BIOS)
    };
    Kind kind = Kind::NoInstruction;
    std::uint32_t pc = 0;        // where the instruction is
    std::uint32_t address = 0;   // BusError: the address
    std::string mnemonic;        // NotRun: which
    std::uint32_t code = 0;      // Swi: the comment field
    // "no instruction at 08000000" | "not run: ldm^" | "bus error at 12345678" | "swi"
    std::string text() const;
};

class Cpu {
public:
    explicit Cpu(Bus& bus);

    std::array<std::uint32_t, 16> r{};   // r0-r15 as the current mode sees them (r15 is the address of the instruction about to run)
    std::uint32_t cpsr = 0x1F;           // System mode, ARM state, interrupts enabled
    std::uint64_t steps = 0;             // instructions executed

    bool thumb() const { return cpsr & 0x20; }
    bool flag_n() const { return cpsr & 0x80000000; }
    bool flag_z() const { return cpsr & 0x40000000; }
    bool flag_c() const { return cpsr & 0x20000000; }
    bool flag_v() const { return cpsr & 0x10000000; }

    // Start at `address`: bit 0 set is Thumb state, clear is ARM.
    void jump_to(std::uint32_t address);
    // Switch the mode (the banked registers) and set the whole of the CPSR.
    void set_cpsr(std::uint32_t value);
    // The current mode's saved status register (0 in User and System mode, which have none).
    std::uint32_t spsr() const;

    // Execute the instruction at pc. nullopt: it ran. A Fault: it did not finish (the step is not counted; for a Swi pc is already
    // past it).
    std::optional<Fault> step();

    Bus& bus() { return *bus_; }

private:
    struct Bank {
        std::uint32_t sp = 0, lr = 0, spsr = 0;
    };
    std::optional<Fault> execute(const decompile::arm::Insn& insn);
    std::uint32_t operand_register(const decompile::arm::Insn& insn, unsigned n, bool shifted_by_register) const;
    struct Shifted {
        std::uint32_t value;
        bool carry;
    };
    Shifted shifter(const decompile::arm::Insn& insn) const;
    Shifted shift(decompile::arm::Shift type, std::uint32_t value, std::uint32_t amount, bool by_register, bool rrx) const;
    void set_nz(std::uint32_t v);
    void write_register(const decompile::arm::Insn& insn, unsigned n, std::uint32_t value);
    void branch(std::uint32_t address);
    void restore_from_spsr();
    static unsigned bank_of(std::uint32_t mode);
    bool condition(unsigned cond) const;
    bool v5() const { return bus_->arch() >= decompile::arm::Arch::V5TE; }
    std::optional<Fault> execute_ext(const decompile::arm::Insn& insn, std::uint32_t at);
    std::optional<Fault> execute_thumb_ext(const decompile::arm::Insn& insn, std::uint32_t at);   // IT, CBZ, the 16-bit extensions and the Thumb VFP words
    std::optional<Fault> execute_vfp(const decompile::arm::Insn& insn, std::uint32_t at);   // ... and the VFP rows   // the ARMv6 rows of decompile/arm_ext
    void branch_interworking(std::uint32_t address);   // a load into the pc: ARMv5 takes the state from bit 0
    std::uint32_t saturate(std::int64_t value);       // to 32 bits, setting the Q flag when it clipped

    Bus* bus_;
    std::array<std::uint32_t, 64> vfp_{};   // the VFP registers s0-s63 (a double is a pair; d0-d31)
    std::uint32_t fpscr_ = 0;
    std::uint8_t it_first_ = 0, it_mask_ = 0, it_index_ = 0, it_left_ = 0;   // the IT block in progress
    bool branched_ = false;                 // this instruction changed the pc
    std::array<Bank, 6> banks_{};           // user/system, FIQ, IRQ, supervisor, abort, undefined
    std::array<std::uint32_t, 5> fiq_high_{};   // r8-r12 of FIQ mode while another mode is current
    std::array<std::uint32_t, 5> user_high_{};  // r8-r12 of the other modes while FIQ is current
};

// What run() starts from: r0-r3 and r4-r11 (the arguments and the callee-saved registers, absent: zero), the stack pointer, and the other
// registers as zero.
struct RunOptions {
    std::uint64_t max_steps = kMaxSteps;
    std::array<std::uint32_t, 4> a{};
    std::array<std::uint32_t, 8> s{};   // r4-r11
    std::uint32_t sp = 0x03007F00;
    std::uint32_t return_address = kReturnAddress;   // reaching it ends the run (the 3DS and Vita lanes pick one below their images)
    std::map<std::uint32_t, std::string> imports;    // the stubs of imported functions by address: a call is logged by name and returns 0
    std::vector<std::string> ending_imports;         // ... the names of the ones that end the run
    std::vector<std::uint32_t> ending;               // software interrupt numbers that end the run instead of returning 0 (ExitProcess ...)
};

enum class End : std::uint8_t { Return, Stopped, Limit };

struct RunResult {
    End end = End::Limit;
    std::optional<Fault> fault;                   // why it stopped
    std::array<std::uint32_t, 16> r{};            // as the run left them
    std::uint32_t cpsr = 0;
    std::uint64_t steps = 0;
    std::vector<BusAccess> io;                    // the register accesses of this run, in order
    std::vector<BiosCall> bios;                   // the BIOS calls, in order
    std::string exit() const;                     // "return" | "stopped: <why>" | "limit"
};

// Call the function at `entry` (bit 0 set: Thumb) as BL would ($lr = kReturnAddress) and run it until it returns, stops, or takes
// `max_steps` instructions. What the function wrote is in the bus's memory.
RunResult run(Bus& bus, std::uint32_t entry, const RunOptions& options = {});

// The run as the one run record: the registers as results (R0 R1 R4-R11 SP), the bus's accesses as io, the BIOS calls as calls,
// little-endian, truncated when it hit the step limit; the RunResult is the record's `raw`.
cb::emulate::Run record(const RunResult& result);

// run + record: the platform's runner.
cb::emulate::Run recorded(Bus& bus, std::uint32_t entry, const RunOptions& options = {});

// The BIOS's answer to a software interrupt, as run() gives it (Div, DivArm, Sqrt, CpuSet and CpuFastSet are done as the BIOS does them, any other call returns 0 in r0):
// it reads and writes `cpu`'s registers r0-r3 and the bus's memory. For the p-code adapter (engine/pcode_cpu_arm.cpp).
void bios_answer(Cpu& cpu, std::uint32_t function);

}  // namespace cb::emulate::arm
