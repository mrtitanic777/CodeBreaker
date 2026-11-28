// emulate/cpu/pcode_ops_ppc.hpp -- what the PowerPC descriptions leave to the host, written once for the adapter (pcode_cpu_ppc.cpp):
//   * the vector operations of AltiVec (the Cell's PowerPC core of the PlayStation 3) and VMX128 (the Xbox 360's Xenon): the descriptions name them (`vectorAddUnsignedByteSaturate`)
//     and give them no p-code; bind() resolves each name once to the host function that does it (pcode_ops_ppc_vector.cpp), built from the shared lane library (pcode_vector.hpp);
//   * the AltiVec instructions the compiled descriptions have no instruction for (serve_undecoded);
//   * Gekko's paired singles and quantized loads and stores (the GameCube, Wii and Wii U's PowerPC 750 family), which the descriptions have no encoding for: opcode 4 is AltiVec
//     in them, and psq_l/psq_st share opcodes with the floating loads of the other machines.
#pragma once

#include <array>
#include <cstdint>
#include <string>
#include <vector>

#include "decompile/engine/pcode_emulator.hh"
#include "emulate/powerpc.hpp"
#include "emulate/cpu/pcode_ops.hpp"

namespace cb::emulate::pcode_ops_ppc {

using Emu = decompile::sleigh::Emulator;
namespace pw = cb::emulate::powerpc;

enum class Flavor : std::uint8_t { AltiVec, Xenon };   // ppc_64_isa_altivec_be.sla, ppc_64_xenon_be.sla

// The registers of both descriptions the host code reads and writes, found once per emulator.
struct Regs {
    std::array<Emu::RegRef, 32> gpr, fpr;
    std::array<Emu::RegRef, 8> cr;
    Emu::RegRef lr, ctr, ca, so, ov, ov32, ca32, count, xer, reserve, reserve_address, reserve_length, vrsave, msr;
    std::array<Emu::RegRef, 32> vr;        // vr0..vr31 (the description names them vs32..vs63)
    std::vector<Emu::RegRef> vr_wide;      // VMX128's registers v32 to v127 where the description has them
    std::vector<Emu::RegRef> status;       // every floating status flag (fp_*) and the other small registers a run starts from zero
};
Regs find_regs(const Emu& emu);

// What the bound handlers share (one per emulator).
struct VectorContext {
    std::uint32_t word = 0;     // the instruction being run (a handler may need its record bit)
    std::uint32_t vscr = 0;     // the vector status and control register: NJ (bit 16), SAT (bit 0)
    const Regs* regs = nullptr; // the registers of the emulator the operations are bound to (vpkd3d128 reads the register it inserts into)
};

// The vector operations' names and handlers (pcode_ops_ppc_vector.cpp), added to `out`.
void add_vector_entries(std::vector<pcode_ops::Entry>& out, Flavor flavor);

// An AltiVec instruction the description has no instruction for (vaddubm, vadduhm, vsubuhm), run on the host: the emulator's bad-instruction hook calls this with the word the
// adapter keeps in the context. False: not one of these.
bool serve_undecoded(Emu& emu, const Regs& regs, const VectorContext& context, std::uint64_t pc);

// The description's user operations that this file serves, bound by number (once, when the emulator is made). `context` outlives the emulator.
pcode_ops::Binding bind(Emu& emu, Flavor flavor, VectorContext& context);

// ---- Gekko ----------------------------------------------------------------------------------------------------------------

// What the paired-single instructions work on: the floating registers are the emulator's, the second halves and the quantization registers the Machine's (as the
// interpreter keeps them), every memory access goes through the emulator's bus (the adapter's hooks).
struct PairedState {
    Emu& emu;
    const Regs& regs;
    pw::Machine& machine;
};
// Run one paired-single instruction (opcode 4, or the psq loads and stores 56, 57, 60, 61): the registers, the condition register and the memory as the interpreter leaves them. A fault of the memory
// the adapter's hooks raise passes through.
void paired_single(PairedState& state, std::uint32_t word);

}  // namespace cb::emulate::pcode_ops_ppc
