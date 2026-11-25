// engine/pcode_cpu_tlcs900.cpp -- the Neo Geo Pocket lane's TLCS-900/H on the one p-code emulator: emulate::tlcs900::run's call, on languages/tlcs900.sla (see the header).
//
// What is where. The registers the emulator runs on are the current bank (XWA XBC XDE XHL), XIX XIY XIZ XSP, the flags S Z H V N C and the pc. Everything else of the
// lane's Registers (the other three banks, F', the status register's other bits, the DMA control registers) is kept here, and the two are made one again whenever
// the run goes from one to the other. An instruction runs on the emulator when its description gives the processor's result and it names registers of the current bank
// only (the description keeps the banks in storage of their own, as a register code names them, apart from the current one); the others run on the adapter's own semantics
// (pcode_ops_tlcs900::step) on the registers as they stand. On the emulator: moves, push and pop, jumps, calls and returns, LINK and UNLK, ADD SUB CP AND OR XOR,
// INC and DEC, NEG CPL, EXTZ EXTS, MUL MULS, shifts by a count, the bit instructions and the carry-flag ones, SCC, DJNZ, EX, LDA and PAA. On the adapter's own semantics: the status
// register and F as operands, the register bank switches, SWI, HALT, RETI and EI, ADC and SBC (their overflow flag), DIV and DIVS, the rotates, the decimal and bit-field
// instructions (DAA MIRR MULA BS1F BS1B MINC MDEC RLD RRD), the block moves and compares, the DMA control registers, the bit instructions on a byte register (the bit
// number wraps at 8) and TSET on memory (the description reads the byte twice). The description's flags that differ from the processor's are put right after the
// instruction: no half carry or parity for a long, and INC and DEC of a word or long register keep every flag. Nothing goes to the lane's old core.
//
// The bytes of an instruction are read through the Bus as the processor reads them (twelve, one at a time), once for each instruction, so the log of device accesses is the same.
#include "engine/pcode_cpu_tlcs900.hpp"

#include <array>
#include <format>
#include <map>
#include <cstdio>
#include "core/fs.hpp"
#include <cstdint>
#include <string>
#include <vector>

#include "decompile/engine/pcode_emulator.hh"
#include "decompile/tlcs900.hpp"
#include "engine/pcode_cpu.hpp"
#include "engine/pcode_cpu_internal.hpp"
#include "engine/pcode_ops_tlcs900.hpp"

namespace cb::engine::pcode_cpu {

namespace t = cb::emulate::tlcs900;
namespace sl = cb::decompile::sleigh;
namespace dt = cb::decompile::tlcs900;

namespace {

constexpr const char* kNames[] = {"XWA", "XBC", "XDE", "XHL", "XIX", "XIY", "XIZ", "XSP", "fS", "fZ", "fH", "fV", "fN", "fC"};
enum Reg { kXWA, kXBC, kXDE, kXHL, kXIX, kXIY, kXIZ, kXSP, kS, kZ, kH, kV, kN, kC };
constexpr std::uint16_t kFlagBits = t::flag::S | t::flag::Z | t::flag::H | t::flag::V | t::flag::N | t::flag::C;

// Whether an operand is a register the emulator holds: a code of the current bank or the global registers.
constexpr bool current_code(std::uint8_t code) { return code >= 0xE0; }

// Whether the description gives the own core's result for the instruction, and the emulator holds everything it uses.
bool emulated(const dt::Insn& i) {
    const dt::Row& row = *i.row;
    for (const dt::Tok tok : {row.a, row.b})
        if (tok == dt::Tok::Sr || tok == dt::Tok::F || tok == dt::Tok::Fp || tok == dt::Tok::Cr) return false;
    if (row.ctx == dt::Ctx::Reg && !current_code(i.reg)) return false;
    if (row.ctx == dt::Ctx::Src || row.ctx == dt::Ctx::Dst) {
        if (!(row.flags & dt::kBare)) {
            const dt::Mem& m = i.m;
            switch (m.kind) {
                case dt::MemKind::Abs8: case dt::MemKind::Abs16: case dt::MemKind::Abs24: case dt::MemKind::PcRel: break;
                case dt::MemKind::RegIdx8: case dt::MemKind::RegIdx16: if (!current_code(m.idx)) return false; [[fallthrough]];
                default: if (!current_code(m.reg)) return false; break;
            }
        }
    }
    using dt::Op;
    // (a bit instruction on a byte register takes the bit number modulo 8, the description shifts by it; TSET on memory reads the byte twice)
    if (row.ctx == dt::Ctx::Reg && i.z == 1 && (row.id == Op::Res || row.id == Op::Set || row.id == Op::Chg || row.id == Op::Bit || row.id == Op::Tset || row.id == Op::Andcf || row.id == Op::Orcf || row.id == Op::Xorcf || row.id == Op::Ldcf || row.id == Op::Stcf)) return false;
    if (row.id == Op::Tset && row.ctx != dt::Ctx::Reg) return false;
    if ((row.id == Op::Link || row.id == Op::Unlk) && (i.reg & 0xFC) == 0xFC) return false;   // (the stack pointer as the register LINK and UNLK work on: the description sets it twice)
    // (ADC and SBC stay on the own core: it takes the overflow from the operands without the carry, the description from the sum with it)
    switch (row.id) {
        case Op::Nop: case Op::Push: case Op::Pop: case Op::Ld: case Op::Ldx: case Op::Ret: case Op::Retd: case Op::Rcf: case Op::Scf: case Op::Ccf: case Op::Zcf: case Op::Ex:
        case Op::Jp: case Op::Call: case Op::Calr: case Op::Jr: case Op::Jrl: case Op::Lda: case Op::Link: case Op::Unlk: case Op::Cpl: case Op::Neg: case Op::Extz: case Op::Exts:
        case Op::Djnz: case Op::Res: case Op::Set: case Op::Chg: case Op::Bit: case Op::Tset: case Op::Inc: case Op::Dec: case Op::Scc: case Op::Add: case Op::Sub:
        case Op::And: case Op::Xor: case Op::Or: case Op::Cp: case Op::Sla: case Op::Sra: case Op::Sll: case Op::Srl: case Op::Mul: case Op::Muls:
        case Op::Paa: case Op::Andcf: case Op::Orcf: case Op::Xorcf: case Op::Ldcf: case Op::Stcf:
            return true;
        default: return false;
    }
}

}  // namespace

t::RunResult run(t::Bus& bus, std::uint32_t entry, const t::RunOptions& options) {
    sl::Emulator& emu = emulator_named("tlcs900.sla");
    const auto& r = reg_refs(emu, kNames);
    emu.flushDecoded();
    emu.clearStops();
    emu.onRead([&](std::string_view space, std::uint64_t addr, unsigned size, std::uint64_t& value) {
        if (!is_ram(space)) return false;
        value = bus.read(static_cast<std::uint32_t>(addr), size);
        return true;
    });
    emu.onWrite([&](std::string_view space, std::uint64_t addr, unsigned size, std::uint64_t value) {
        if (!is_ram(space)) return false;
        bus.write(static_cast<std::uint32_t>(addr), size, static_cast<std::uint32_t>(value));
        return true;
    });
    struct Unhook {
        sl::Emulator& e;
        ~Unhook() {
            e.onRead({});
            e.onWrite({});
        }
    } unhook{emu};

    // the registers as the own core starts a function
    pcode_ops_tlcs900::Host host;   // (what the adapter's own semantics keep besides the registers)
    host.on_swi = options.on_swi;
    t::Registers g;
    g.sr = options.sr;
    g.xwa() = options.xwa;
    g.xbc() = options.xbc;
    g.xde() = options.xde;
    g.xhl() = options.xhl;
    g.xix = options.xix;
    g.xiy = options.xiy;
    g.xiz = options.xiz;
    g.xsp = options.sp;
    for (const std::uint16_t w : options.args) {
        g.xsp -= 2;
        bus.write(g.xsp, 2, w);
    }
    g.xsp -= 4;
    bus.write(g.xsp, 4, t::kReturnAddress);
    g.pc = entry;
    const std::size_t logged_before = bus.log().size();

    // which of the two holds the registers now (both, when a step has not changed either since they were made one)
    bool g_valid = true, emu_valid = false;
    const auto to_emu = [&] {
        const auto& bank = g.bank[g.rfp()];
        for (unsigned k = 0; k < 4; ++k) emu.setReg(r[kXWA + k], bank[k]);
        emu.setReg(r[kXIX], g.xix);
        emu.setReg(r[kXIY], g.xiy);
        emu.setReg(r[kXIZ], g.xiz);
        emu.setReg(r[kXSP], g.xsp);
        const std::uint8_t f = g.f();
        emu.setReg(r[kS], (f & t::flag::S) ? 1 : 0);
        emu.setReg(r[kZ], (f & t::flag::Z) ? 1 : 0);
        emu.setReg(r[kH], (f & t::flag::H) ? 1 : 0);
        emu.setReg(r[kV], (f & t::flag::V) ? 1 : 0);
        emu.setReg(r[kN], (f & t::flag::N) ? 1 : 0);
        emu.setReg(r[kC], (f & t::flag::C) ? 1 : 0);
        emu.setPc(g.pc);
        emu_valid = true;
    };
    const auto to_g = [&] {
        auto& bank = g.bank[g.rfp()];
        for (unsigned k = 0; k < 4; ++k) bank[k] = static_cast<std::uint32_t>(emu.reg(r[kXWA + k]));
        g.xix = static_cast<std::uint32_t>(emu.reg(r[kXIX]));
        g.xiy = static_cast<std::uint32_t>(emu.reg(r[kXIY]));
        g.xiz = static_cast<std::uint32_t>(emu.reg(r[kXIZ]));
        g.xsp = static_cast<std::uint32_t>(emu.reg(r[kXSP]));
        std::uint16_t f = 0;
        f |= emu.reg(r[kS]) ? t::flag::S : 0;
        f |= emu.reg(r[kZ]) ? t::flag::Z : 0;
        f |= emu.reg(r[kH]) ? t::flag::H : 0;
        f |= emu.reg(r[kV]) ? t::flag::V : 0;
        f |= emu.reg(r[kN]) ? t::flag::N : 0;
        f |= emu.reg(r[kC]) ? t::flag::C : 0;
        g.sr = static_cast<std::uint16_t>((g.sr & ~kFlagBits) | f);
        g.pc = static_cast<std::uint32_t>(emu.pc()) & 0xFFFFFF;
        g_valid = true;
    };

    t::RunResult result;
    std::uint64_t steps = 0;
    std::vector<t::SoftwareInterrupt> swis;
    while (steps < options.max_steps) {
        const std::uint32_t pc = g_valid ? g.pc : static_cast<std::uint32_t>(emu.pc());
        if (pc == t::kReturnAddress) {
            result.end = t::End::Return;
            break;
        }
        if (options.on_instruction) {
            if (!g_valid) to_g();
            options.on_instruction(g);
        }
        // the instruction's bytes as the own core reads them
        Bytes window(12);
        for (unsigned k = 0; k < window.size(); ++k) window[k] = static_cast<std::uint8_t>(bus.read((pc + k) & 0xFFFFFF, 1));
        const std::optional<dt::Insn> insn = dt::decode(window, 0, pc);
        bool ran = false;
        if (insn && emulated(*insn)) {
            if (!emu_valid) to_emu();
            emu.write(pc, window.data(), window.size());
            const dt::Op id = insn->row->id;
            // INC and DEC of a word or long register keep every flag; the description sets them
            const bool keeps_flags = (id == dt::Op::Inc || id == dt::Op::Dec) && insn->row->ctx == dt::Ctx::Reg && insn->z >= 2;
            std::array<std::uint64_t, 6> flags_before{};
            if (keeps_flags)
                for (unsigned k = 0; k < 6; ++k) flags_before[k] = emu.reg(r[kS + k]);
            const sl::Emulator::Status status = emu.step();
            if (status == sl::Emulator::Status::Ok) {
                if (keeps_flags)
                    for (unsigned k = 0; k < 6; ++k) emu.setReg(r[kS + k], flags_before[k]);
                // a long has no half carry and no parity of its own: the description computes both
                if (insn->z == 4) {
                    if (id == dt::Op::Add || id == dt::Op::Adc || id == dt::Op::Sub || id == dt::Op::Sbc || id == dt::Op::Cp) emu.setReg(r[kH], 0);
                    else if (id == dt::Op::And || id == dt::Op::Or || id == dt::Op::Xor) emu.setReg(r[kV], 0);
                }
                ++steps;
                g_valid = false;
                ran = true;
            }
        }
        if (ran) continue;
        // the adapter's own semantics (pcode_ops_tlcs900): one instruction (a block move is its whole run), on the registers as they stand
        if (!g_valid) to_g();
        host.steps = 0;
        host.swis.clear();
        t::Registers working = g;
        std::optional<t::Fault> fault;
        if (!insn) fault = t::Fault{g.pc, std::format("undefined instruction at {:06X}", g.pc)};
        else fault = pcode_ops_tlcs900::step(host, working, bus, *insn);
        if (fault) {
            result.end = t::End::Stopped;
            result.fault = fault;
            break;
        }
        g = working;
        steps += host.steps;
        swis.insert(swis.end(), host.swis.begin(), host.swis.end());
        g_valid = true;
        emu_valid = false;
    }
    if (!g_valid) to_g();
    if (result.end == t::End::Limit && g.pc == t::kReturnAddress) result.end = t::End::Return;
    result.regs = g;
    result.steps = steps;
    result.swis = std::move(swis);
    result.io.assign(bus.log().begin() + static_cast<std::ptrdiff_t>(logged_before), bus.log().end());
    return result;
}

t::RunResult call(t::Bus& bus, std::uint32_t entry, const t::RunOptions& options) { return enabled() ? pcode_cpu::run(bus, entry, options) : t::run(bus, entry, options); }

}  // namespace cb::engine::pcode_cpu
