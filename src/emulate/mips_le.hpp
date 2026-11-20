// emulate/mips_le.hpp -- the little-endian MIPS processors of the two Sony consoles after the PlayStation, one interpreter for both: the
// PlayStation 2's R5900 (the Emotion Engine) and the PSP's Allegrex. The machine around it -- the memory map, the registers that are logged,
// the system calls -- is the console's own (emulate/ee, emulate/allegrex): this is the processor, over a `Port` the machine implements.
//
//     mipsle::Cpu cpu(mipsle::Model::R5900, port);
//     cpu.jump_to(entry);
//     while (!(fault = cpu.step())) {}
//
// What it runs, as user-mode programs use it (the decoder is decompile/mips, `Variant::R5900` and `Variant::Allegrex`):
//
//   both       the 32-bit integer instructions (results sign-extended into the 64-bit registers), branches and jumps with a delay slot, branch
//              likely (the slot runs only when the branch is taken), MOVZ/MOVN, MULT/DIV in all signednesses with HI and LO, MADD/MADDU, the
//              unaligned loads and stores, the traps, and COP1 in single precision (add sub mul div sqrt abs mov neg, the conversions to and from
//              words, the compares, MFC1 MTC1 CFC1 CTC1, BC1F/T/FL/TL, LWC1 SWC1) with the host's float arithmetic
//   R5900      the 64-bit instructions (the doubleword loads, stores, shifts, adds and multiplies), 128-bit registers (LQ, SQ, and the multimedia
//              instructions: the packed add, subtract, compare, min, max, absolute value, saturating forms, logical operations, the extend,
//              pack, interleave, copy, exchange and reverse forms, the shifts, PLZCW, PMFHI PMFLO PMTHI PMTLO, QFSRV), the second multiplier
//              (MULT1 DIV1 MADD1 ... MFHI1 MFLO1 ...), the shift amount register (MFSA MTSA MTSAB MTSAH), the EE's FPU (its accumulator, MADD.S
//              MSUB.S ADDA.S ..., RSQRT.S, MAX.S MIN.S: no infinities or NaNs, an overflow gives the largest float, a denormal reads as zero),
//              COP0's registers and EI/DI (they only set and clear Status's EIE), and LQC2/SQC2/QMFC2/QMTC2/CFC2/CTC2 on VU0's registers
//   Allegrex   CLZ CLO MADD MSUB MAX MIN, EXT INS SEB SEH WSBH WSBW BITREV ROTR ROTRV, HALT, and IEEE single-precision arithmetic with the
//              rounding mode of FCR31 for the integer conversions
//
// Not run (the run ends with "not run: <mnemonic>"): the VU0's macro-mode arithmetic and the VFPU, the multimedia multiplies, divides and
// multiply-accumulates (PMULT* PDIV* PMADD* PMSUB* PHMADH PHMSBH PMFHL PMTHL), the TLB instructions, ERET, and the Allegrex's cache and
// internal-register moves. An exception is not delivered: a SYSCALL goes to the machine (`Port::syscall`), a BREAK, a trap, an overflow, an
// unaligned access or an address nothing answers ends the run with its reason. A Cpu and a run belong to one thread.
#pragma once

#include <array>
#include <cstdint>
#include <optional>
#include <string>

namespace cb::emulate::mipsle {

enum class Model : std::uint8_t { R5900, Allegrex };

// Why the CPU cannot go on: the run ends with this reason.
struct Fault {
    enum class Kind : std::uint8_t {
        NoInstruction,   // the word at pc is not an instruction of the processor
        NotRun,          // an instruction this interpreter does not run (`mnemonic`)
        AddressError,    // a load, store or fetch that is not aligned
        BusError,        // an address nothing answers
        Overflow,        // ADD, ADDI, SUB (or the doubleword forms) overflowed
        Trap,            // TEQ, TNE, TGE ... taken
        Break,
        Syscall,         // a SYSCALL the machine did not take (or one that ends the program: `mnemonic` says which)
    };
    Kind kind = Kind::NoInstruction;
    std::uint32_t pc = 0;        // where the instruction is
    std::uint32_t address = 0;   // AddressError, BusError: the address
    std::string mnemonic;        // NotRun: which instruction; Syscall: which call
    std::uint32_t code = 0;      // Break, Syscall: the code
    // "no instruction at 00100000" | "not run: vadd.xyzw" | "address error at 00100002" | "bus error at 12345678" | "arithmetic overflow" |
    // "trap" | "break 7" | "syscall Exit"
    std::string text() const;
};

class Cpu;

// The machine the processor runs in. Addresses are the program's virtual ones; the machine maps them (and logs what it should).
class Port {
public:
    virtual ~Port() = default;
    // Read or write `size` (1, 2, 4 or 8) bytes, little-endian; false: nothing answers there.
    virtual bool read(std::uint32_t address, unsigned size, std::uint64_t& value) = 0;
    virtual bool write(std::uint32_t address, unsigned size, std::uint64_t value) = 0;
    // A SYSCALL with this code: true when the machine did what the kernel does (and set the registers), false when the program cannot go on
    // (`stopped` says why, as the Fault's mnemonic).
    virtual bool syscall(Cpu& cpu, std::uint32_t code, std::string& stopped) = 0;
    // Called before the instruction at the CPU's pc is fetched. Handled: the machine ran it instead (a call to an imported function: it did its logging,
    // set the result and moved pc to the return address); Stop: the call ends the program (`stopped` says which); None: run the instruction.
    enum class Intercept : std::uint8_t { None, Handled, Stop };
    virtual Intercept intercept(Cpu&, std::string& /*stopped*/) { return Intercept::None; }
};

struct U128 {
    std::uint64_t lo = 0, hi = 0;
};

class Cpu {
public:
    Cpu(Model model, Port& port) : model_(model), port_(&port) {}

    std::array<std::uint64_t, 32> r{};         // $0-$31 ($0 stays zero): the low 64 bits
    std::array<std::uint64_t, 32> rh{};        // the R5900's upper 64 bits of each register
    std::uint32_t pc = 0;                      // the instruction about to run
    std::uint32_t next_pc = 4;                 // the one after it (what a branch in a delay slot's place changes)
    std::array<std::uint64_t, 2> hi{}, lo{};   // HI and LO (the R5900's second multiplier uses [1])
    std::uint32_t sa = 0;                      // the R5900's shift amount register
    std::array<std::uint32_t, 32> cop0{};
    std::array<std::uint32_t, 32> fpr{};       // the FPU's registers (single precision, as bits)
    std::uint32_t facc = 0;                    // the R5900 FPU's accumulator
    std::uint32_t fcr31 = 0;                   // control and status: rounding mode in bits 0-1, the condition in bit 23
    std::array<std::array<std::uint32_t, 4>, 32> vf{};   // VU0's registers (R5900): x y z w
    std::array<std::uint32_t, 32> vi{};        // VU0's control and integer registers
    std::uint64_t steps = 0;                   // instructions executed

    Model model() const { return model_; }
    // Start at `address`: pc there, the instruction after it next (no branch pending).
    void jump_to(std::uint32_t address) { pc = address; next_pc = address + 4; }

    // Execute the instruction at pc. nullopt: it ran. A Fault: it did not finish (the step is not counted; for a Break or Syscall pc is already
    // past it).
    std::optional<Fault> step();

    Port& port() { return *port_; }
    U128 reg128(unsigned n) const { return {r[n], rh[n]}; }

private:
    struct Exec;
    friend struct Exec;
    Model model_;
    Port* port_;
};

}  // namespace cb::emulate::mipsle
