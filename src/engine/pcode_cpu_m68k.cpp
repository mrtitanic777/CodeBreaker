// engine/pcode_cpu_m68k.cpp -- the lanes' 68000 (Genesis, Sega CD, Neo Geo, Jaguar, CD-i) on the one p-code emulator: emulate::m68k::run's call, on
// the description languages/68020.sla (the 68000's instruction set is its subset).
#include <algorithm>
#include <cstring>
#include <string>
#include <string_view>
#include <array>
#include <memory>
#include <vector>

#include "decompile/engine/pcode_emulator.hh"
#include "decompile/m68k.hpp"
#include "engine/pcode_cpu.hpp"
#include "engine/pcode_cpu_internal.hpp"

namespace cb::engine::pcode_cpu {

namespace m = cb::emulate::m68k;
namespace sl = cb::decompile::sleigh;

namespace {

constexpr std::uint32_t kBlock = 0x1000;   // the page the code mirror is kept current by

// What the emulator decodes instructions from: the bus's memory as the last run left it, kept per thread and brought up to date a page at a time
// (the same ROM image is run again and again; only the pages that differ are handed over).
struct Mirror {
    std::vector<std::uint8_t> seen;
    std::vector<std::uint32_t> epoch;   // by page: the run it was last made current in
    std::uint32_t run = 0;
    // What each instruction address holds, by 4 KB page and the byte in it: 0 not looked at yet, else 1 + (0 not a 68000 instruction, 1 one to run, 2 STOP, 3 ILLEGAL, 4 a
    // branch with a displacement byte of $FF). A page's table is dropped when the page changes.
    std::vector<std::unique_ptr<std::array<std::uint8_t, kBlock>>> kinds;
};


Mirror& mirror() {
    thread_local Mirror m;
    return m;
}

}  // namespace

m::RunResult run(m::Bus& bus, std::uint32_t entry, const m::RunOptions& options) {
    sl::Emulator& emu = emulator_named("68020.sla");
    const std::size_t log_start = bus.log().size();
    const std::span<std::uint8_t> memory = bus.memory();
    Mirror& mir = mirror();
    if (mir.seen.size() != memory.size()) {
        emu.clearMemory();
        mir.seen.assign(memory.size(), 0);
        mir.kinds.clear();
        mir.kinds.resize((m::kAddressMask + 1) / kBlock);   // (every address the processor can reach, memory or not)
        mir.epoch.assign((m::kAddressMask + 1) / kBlock, 0);
    }
    ++mir.run;
    // A page of the code mirror is made current the first time an instruction is fetched from it in a run (what the mirror holds is compared with the bus's
    // memory then, and only a page that differs is handed over): data pages are never looked at.
    const auto make_current = [&](std::uint32_t at) {
        const std::size_t page = at / kBlock;
        if (page >= mir.epoch.size() || mir.epoch[page] == mir.run) return;
        mir.epoch[page] = mir.run;
        const std::size_t from = page * kBlock;
        if (from >= memory.size()) return;
        const std::size_t n = std::min<std::size_t>(kBlock, memory.size() - from);
        if (std::memcmp(mir.seen.data() + from, memory.data() + from, n) == 0) return;
        std::memcpy(mir.seen.data() + from, memory.data() + from, n);
        mir.kinds[page].reset();
        emu.write(from, memory.data() + from, n);
    };
    emu.clearStops();

    emu.onRead([&](std::string_view space, std::uint64_t addr, unsigned size, std::uint64_t& value) {
        if (!is_ram(space)) return false;
        value = bus.read(static_cast<std::uint32_t>(addr), size);
        return true;
    });
    emu.onWrite([&](std::string_view space, std::uint64_t addr, unsigned size, std::uint64_t value) {
        if (!is_ram(space)) return false;
        bus.write(static_cast<std::uint32_t>(addr), size, static_cast<std::uint32_t>(value));
        // what the program wrote is what instructions are read from from now on (a function can run what it, or its caller, put in RAM)
        const std::uint32_t at = static_cast<std::uint32_t>(addr) & m::kAddressMask;
        // (the own core decodes an instruction when it first reaches it and keeps it: bytes of an instruction that has run, or may overlap one, are not
        // changed under it here either)
        if (at + size <= memory.size()) {
            bool decoded = false;
            for (std::uint32_t a = at >= 8 ? at - 8 : 0; a < at + size && !decoded; ++a) {
                const auto& table = mir.kinds[a / kBlock];
                decoded = table && (*table)[a & (kBlock - 1)] != 0;
            }
            if (!decoded) {
                emu.write(at, memory.data() + at, size);
                std::memcpy(mir.seen.data() + at, memory.data() + at, size);
            }
        }
        return true;
    });
    // what the description leaves to the machine: an exception (TRAP #n, CHK, TRAPV) and STOP end the run, as the own core's do
    std::optional<m::Fault::Kind> raised;
    std::uint32_t current = 0;   // the instruction being run
    emu.onUserOp([&](sl::Emulator&, const std::string& name, const std::vector<std::uint64_t>&, std::uint64_t&) {
        if (name == "__m68k_trap") raised = emu.text(current).rfind("chk", 0) == 0 ? m::Fault::Kind::Check : m::Fault::Kind::Trap;
        else if (name == "__m68k_trapv") raised = m::Fault::Kind::TrapOverflow;
        else if (name == "stop") raised = m::Fault::Kind::Stop;
        else return false;
        return true;
    });
    struct Unhook {
        sl::Emulator& e;
        ~Unhook() {
            e.onRead({});
            e.onWrite({});
            e.onUserOp({});
        }
    } unhook{emu};

    static constexpr const char* kM68kNames[] = {"D0", "D1", "D2", "D3", "D4", "D5", "D6", "D7", "A0", "A1", "A2", "A3", "A4", "A5", "A6",
                                                 "TF", "SVF", "IPL", "XF", "NF", "ZF", "VF", "CF", "SR", "USP", "SP"};
    enum : unsigned { kA = 8, kTF = 15, kSVF, kIPL, kXF, kNF, kZF, kVF, kCF, kSR, kUSP, kSP };
    const auto& refs = reg_refs(emu, kM68kNames);
    for (unsigned i = 0; i < 8; ++i) {
        emu.setReg(refs[i], options.d ? (*options.d)[i] : 0);
        if (i < 7) emu.setReg(refs[kA + i], options.a ? (*options.a)[i] : 0);
    }
    const std::uint16_t sr = options.sr;
    emu.setReg(refs[kTF], (sr & 0x8000) != 0);
    emu.setReg(refs[kSVF], (sr & 0x2000) != 0);
    emu.setReg(refs[kIPL], (sr >> 8) & 7);
    emu.setReg(refs[kXF], (sr & 0x10) != 0);
    emu.setReg(refs[kNF], (sr & 8) != 0);
    emu.setReg(refs[kZF], (sr & 4) != 0);
    emu.setReg(refs[kVF], (sr & 2) != 0);
    emu.setReg(refs[kCF], (sr & 1) != 0);
    emu.setReg(refs[kSR], sr);
    emu.setReg(refs[kUSP], 0);
    // the call as JSR makes it: the return address pushed through the bus
    const std::uint32_t sp = options.sp - 4;
    bus.write(sp, 4, m::kReturnAddress);
    emu.setReg(refs[kSP], sp);
    emu.setPc(entry);

    m::RunResult out;
    out.end = m::End::Limit;
    std::uint64_t steps = 0;
    while (steps < options.max_steps) {
        const std::uint32_t pc = static_cast<std::uint32_t>(emu.pc());
        if (pc == m::kReturnAddress) {
            out.end = m::End::Return;
            break;
        }
        if (pc > m::kAddressMask) {   // (the 68000's address bus is 24 bits: the instruction comes from the address without its top byte)
            emu.setPc(pc & m::kAddressMask);
            continue;
        }
        raised.reset();
        current = pc;
        make_current(pc);
        make_current(pc + 15);   // (an instruction can run over into the next page)
        std::unique_ptr<std::array<std::uint8_t, kBlock>>& table = mir.kinds[pc / kBlock];
        if (!table) table = std::make_unique<std::array<std::uint8_t, kBlock>>();
        std::uint8_t& known = (*table)[pc & (kBlock - 1)];
        if (known == 0) {
            // The description is the 68020's (there is no 68000 one) and takes encodings the 68000 does not have: its 68010/68020 instructions and
            // addressing modes, and modes an instruction does not allow (a PC-relative or immediate destination, a byte op on An). The 68000's
            // instruction set is what decompile/m68k's decoder holds; an opcode outside it is no instruction here.
            // (a branch with a displacement byte of $FF is the 68020's 32-bit form; the 68000's is a branch to an odd address)
            const std::uint32_t at = pc & m::kAddressMask;
            const bool long_branch = at + 1 < memory.size() && (memory[at] & 0xF0) == 0x60 && memory[at + 1] == 0xFF;
            // (memory past the end of the bus reads zeros: the bytes are padded with them)
            std::uint8_t window[16] = {};
            for (std::size_t i = 0; i < sizeof window && at + i < memory.size(); ++i) window[i] = memory[at + i];
            std::uint8_t kind = decompile::m68k::decode(ByteView(window, sizeof window), 0, pc).has_value() ? 1 : 0;
            if (kind == 1 && long_branch) kind = 4;
            if (kind == 1) {   // (the description loads SR and halts for STOP, and does nothing for ILLEGAL; the own core ends the run with the registers as they were)
                emu.length(pc);   // (decoding first: it drops what a changed page made stale, which text() does not)
                const std::string text = emu.text(pc);
                if (text.rfind("stop", 0) == 0) kind = 2;
                else if (text.rfind("illegal", 0) == 0) kind = 3;
            }
            known = static_cast<std::uint8_t>(kind + 1);
        }
        const std::uint8_t what = static_cast<std::uint8_t>(known - 1);
        if (what == 4) {
            // BRA, BSR or Bcc with a displacement byte of $FF: the description reads the 68020's 32-bit form; the 68000 branches by -1, to an odd address. The
            // instruction is run as the byte form with a displacement of 2 (the condition and BSR's push are the description's), and when it branches the
            // run goes on from the odd address the 68000 reaches (where the next step refuses it).
            const std::uint32_t at = pc & m::kAddressMask;
            const std::uint8_t original = memory[at + 1], patched = 2;
            emu.write(at + 1, &patched, 1);
            const sl::Emulator::Status status = emu.step();
            emu.write(at + 1, &original, 1);
            if (status != sl::Emulator::Status::Ok) {
                out.end = m::End::Stopped;
                out.fault = m::Fault{m::Fault::Kind::NoInstruction, pc, {}};
                break;
            }
            if (emu.pc() != pc + 2) emu.setPc(pc + 1);
            ++steps;
            continue;
        }
        if (what != 1) {   // an instruction of a later 68000-family chip (the 68000 raises the illegal-instruction exception), STOP, ILLEGAL
            out.end = m::End::Stopped;
            out.fault = m::Fault{what == 2 ? m::Fault::Kind::Stop : what == 3 ? m::Fault::Kind::Illegal : m::Fault::Kind::NoInstruction, pc, {}};
            break;
        }
        const sl::Emulator::Status status = emu.step();
        if (status == sl::Emulator::Status::BadInstruction) {
            out.end = m::End::Stopped;
            out.fault = m::Fault{m::Fault::Kind::NoInstruction, pc, {}};
            break;
        }
        if (status == sl::Emulator::Status::Error) {
            out.end = m::End::Stopped;
            const std::string& why = emu.lastError();
            const bool divide = why.find("ivi") != std::string::npos;
            out.fault = m::Fault{divide ? m::Fault::Kind::DivideByZero : m::Fault::Kind::Illegal, pc, {}};
            break;
        }
        if (raised) {
            out.end = m::End::Stopped;
            out.fault = m::Fault{*raised, pc, {}};
            break;
        }
        ++steps;
    }
    for (unsigned i = 0; i < 8; ++i) out.d[i] = static_cast<std::uint32_t>(emu.reg(refs[i]));
    for (unsigned i = 0; i < 7; ++i) out.a[i] = static_cast<std::uint32_t>(emu.reg(refs[kA + i]));
    out.a[7] = static_cast<std::uint32_t>(emu.reg(refs[kSP]));
    out.sr = static_cast<std::uint16_t>((emu.reg(refs[kTF]) << 15) | (emu.reg(refs[kSVF]) << 13) | (emu.reg(refs[kIPL]) << 8) | (emu.reg(refs[kXF]) << 4) | (emu.reg(refs[kNF]) << 3) |
                                        (emu.reg(refs[kZF]) << 2) | (emu.reg(refs[kVF]) << 1) | emu.reg(refs[kCF]));
    out.steps = steps;
    out.io.assign(bus.log().begin() + static_cast<std::ptrdiff_t>(log_start), bus.log().end());
    return out;
}

m::RunResult call(m::Bus& bus, std::uint32_t entry, const m::RunOptions& options) { return enabled(Core::M68000) ? pcode_cpu::run(bus, entry, options) : m::run(bus, entry, options); }

}  // namespace cb::engine::pcode_cpu
