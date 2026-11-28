// engine/pcode_cpu_tlcs900.cpp -- the Neo Geo Pocket lane's TLCS-900/H on the one p-code emulator, on languages/tlcs900exact.sla (see the header).
//
// The emulator holds every register: the current bank (XWA XBC XDE XHL) and the four banks (XWA0 .. XHL3, the current one's own copy stale while it is current),
// XIX XIY XIZ XSP, the flags S Z H V N C and F' apart, SR's other bits (RFP, IFF, and SRX for the bits nothing names), the DMA control registers and INTNEST, and the pc.
// The description runs every instruction; what it leaves to the machine is the BIOS (`swi`: logged, with RunOptions::on_swi free to supply a result) and HALT, which
// stops the run. A block instruction that repeats (ldir lddr cpir cpdr) runs to its end as one instruction does on the processor: each repeat is a step, and the run's limit
// is looked at only between instructions.
//
// The bytes of an instruction are read through the Bus as the processor reads them (twelve, one at a time), once for each instruction, so the log of device accesses is the same.
#include "engine/pcode_cpu_tlcs900.hpp"

#include <array>
#include <cstdint>
#include <format>
#include <string>
#include <vector>

#include "decompile/engine/pcode_emulator.hh"
#include "engine/pcode_cpu_internal.hpp"

namespace cb::engine::pcode_cpu {

namespace t = cb::emulate::tlcs900;
namespace sl = cb::decompile::sleigh;

namespace tlcs900_adapter {   // (named: the file shares a unity unit)

constexpr const char* kTlcsNames[] = {
    "XWA",  "XBC",  "XDE",  "XHL",  "XIX",  "XIY",  "XIZ",  "XSP",                                                          // 0-7
    "XWA0", "XBC0", "XDE0", "XHL0", "XWA1", "XBC1", "XDE1", "XHL1", "XWA2", "XBC2", "XDE2", "XHL2", "XWA3", "XBC3", "XDE3", "XHL3",   // 8-23
    "fS",   "fZ",   "fH",   "fV",   "fN",   "fC",   "aS",   "aZ",   "aH",   "aV",   "aN",   "aC",   "aX",   "SRX",  "RFP",  "IFF",    // 24-39
    "DMAS0", "DMAS1", "DMAS2", "DMAS3", "DMAD0", "DMAD1", "DMAD2", "DMAD3", "DMAC0", "DMAC1", "DMAC2", "DMAC3", "DMAM0", "DMAM1", "DMAM2", "DMAM3",   // 40-55
    "INTNEST"};
enum TlcsReg : unsigned { kXWA = 0, kXIX = 4, kXIY = 5, kXIZ = 6, kXSP = 7, kBank0 = 8, kFlags = 24, kAlt = 30, kAX = 36, kSRX = 37, kRFP = 38, kIFF = 39, kDMAS = 40, kDMAD = 44, kDMAC = 48, kDMAM = 52, kINTNEST = 56 };
using RegTable = std::array<sl::Emulator::RegRef, std::size(kTlcsNames)>;
constexpr std::uint8_t kFlagOrder[6] = {t::flag::S, t::flag::Z, t::flag::H, t::flag::V, t::flag::N, t::flag::C};
constexpr std::uint16_t kNamedSrBits = 0x7300 | t::flag::S | t::flag::Z | t::flag::H | t::flag::V | t::flag::N | t::flag::C;   // (IFF, RFP and the flags; SRX holds the rest)

void put_registers(sl::Emulator& emu, const RegTable& r, const t::Registers& g) {
    const unsigned rfp = g.rfp();
    for (unsigned b = 0; b < 4; ++b)
        for (unsigned k = 0; k < 4; ++k) emu.setReg(r[kBank0 + 4 * b + k], g.bank[b][k]);
    for (unsigned k = 0; k < 4; ++k) emu.setReg(r[kXWA + k], g.bank[rfp][k]);
    emu.setReg(r[kXIX], g.xix);
    emu.setReg(r[kXIY], g.xiy);
    emu.setReg(r[kXIZ], g.xiz);
    emu.setReg(r[kXSP], g.xsp);
    for (unsigned k = 0; k < 6; ++k) {
        emu.setReg(r[kFlags + k], (g.sr & kFlagOrder[k]) ? 1 : 0);
        emu.setReg(r[kAlt + k], (g.f_alt & kFlagOrder[k]) ? 1 : 0);
    }
    emu.setReg(r[kAX], g.f_alt & 0x28);
    emu.setReg(r[kSRX], g.sr & ~kNamedSrBits & 0xFFFF);
    emu.setReg(r[kRFP], rfp);
    emu.setReg(r[kIFF], (g.sr >> 12) & 7);
    for (unsigned k = 0; k < 4; ++k) {
        emu.setReg(r[kDMAS + k], g.dmas[k]);
        emu.setReg(r[kDMAD + k], g.dmad[k]);
        emu.setReg(r[kDMAC + k], g.dmac[k]);
        emu.setReg(r[kDMAM + k], g.dmam[k]);
    }
    emu.setReg(r[kINTNEST], g.intnest);
}

void get_registers(const sl::Emulator& emu, const RegTable& r, t::Registers& g) {
    const unsigned rfp = static_cast<unsigned>(emu.reg(r[kRFP])) & 3u;
    for (unsigned b = 0; b < 4; ++b)
        for (unsigned k = 0; k < 4; ++k) g.bank[b][k] = static_cast<std::uint32_t>(emu.reg(r[(b == rfp ? kXWA : kBank0 + 4 * b) + k]));
    g.xix = static_cast<std::uint32_t>(emu.reg(r[kXIX]));
    g.xiy = static_cast<std::uint32_t>(emu.reg(r[kXIY]));
    g.xiz = static_cast<std::uint32_t>(emu.reg(r[kXIZ]));
    g.xsp = static_cast<std::uint32_t>(emu.reg(r[kXSP]));
    std::uint16_t sr = static_cast<std::uint16_t>(emu.reg(r[kSRX]) & ~kNamedSrBits);
    std::uint8_t alt = static_cast<std::uint8_t>(emu.reg(r[kAX]) & 0x28);
    for (unsigned k = 0; k < 6; ++k) {
        if (emu.reg(r[kFlags + k])) sr |= kFlagOrder[k];
        if (emu.reg(r[kAlt + k])) alt |= kFlagOrder[k];
    }
    sr |= static_cast<std::uint16_t>(rfp << 8);
    sr |= static_cast<std::uint16_t>((emu.reg(r[kIFF]) & 7u) << 12);
    g.sr = sr;
    g.f_alt = alt;
    for (unsigned k = 0; k < 4; ++k) {
        g.dmas[k] = static_cast<std::uint32_t>(emu.reg(r[kDMAS + k]));
        g.dmad[k] = static_cast<std::uint32_t>(emu.reg(r[kDMAD + k]));
        g.dmac[k] = static_cast<std::uint16_t>(emu.reg(r[kDMAC + k]));
        g.dmam[k] = static_cast<std::uint8_t>(emu.reg(r[kDMAM + k]));
    }
    g.intnest = static_cast<std::uint16_t>(emu.reg(r[kINTNEST]));
    g.pc = static_cast<std::uint32_t>(emu.pc()) & 0xFFFFFF;
}

// Whether the instruction in `window` is a block move or compare that repeats (ldir lddr cpir cpdr): (XRR) of a byte or a word and the operation byte.
bool repeating_block(const std::uint8_t* window) {
    const std::uint8_t first = window[0], op = window[1];
    if ((first & 0xE8) != 0x80) return false;   // ($80-$87, $90-$97)
    return op == 0x11 || op == 0x13 || op == 0x15 || op == 0x17;
}

}  // namespace tlcs900_adapter

t::RunResult run(t::Bus& bus, std::uint32_t entry, const t::RunOptions& options) {
    using namespace tlcs900_adapter;
    sl::Emulator& emu = emulator_named("tlcs900exact.sla");
    const RegTable& r = reg_refs(emu, kTlcsNames);
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

    // the registers as the own core starts a function
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
    put_registers(emu, r, g);
    emu.setPc(g.pc);
    const std::size_t logged_before = bus.log().size();

    // the BIOS: `swi n` logged with the function asked for (bank 3's W) and the current bank, and the hook free to change the registers
    std::vector<t::SoftwareInterrupt> swis;
    std::uint32_t at = g.pc;   // (the instruction running)
    emu.onUserOp([&](sl::Emulator& self, const std::string& name, const std::vector<std::uint64_t>& args, std::uint64_t&) {
        if (name != "swi") return false;
        t::Registers now;
        get_registers(self, r, now);
        t::SoftwareInterrupt call;
        call.number = static_cast<std::uint8_t>(args.empty() ? 0 : args[0] & 7u);
        call.pc = at;
        call.function = static_cast<std::uint8_t>(now.bank[3][0] >> 8);   // RW3
        call.regs = now.bank[now.rfp()];
        swis.push_back(call);
        if (options.on_swi) {
            options.on_swi(now, call);
            put_registers(self, r, now);
        }
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

    t::RunResult result;
    std::uint64_t steps = 0;
    Bytes window(12);
    while (steps < options.max_steps) {
        const std::uint32_t pc = static_cast<std::uint32_t>(emu.pc()) & 0xFFFFFF;
        if (pc == t::kReturnAddress) {
            result.end = t::End::Return;
            break;
        }
        if (options.on_instruction) {
            get_registers(emu, r, g);
            options.on_instruction(g);
        }
        // the instruction's bytes as the own core reads them
        for (unsigned k = 0; k < window.size(); ++k) window[k] = static_cast<std::uint8_t>(bus.read((pc + k) & 0xFFFFFF, 1));
        if (window[0] == 0x05) {   // halt
            result.end = t::End::Stopped;
            result.fault = t::Fault{pc, std::format("halt at {:06X}", pc)};
            break;
        }
        emu.write(pc, window.data(), window.size());
        emu.setPc(pc);
        at = pc;
        const bool repeats = repeating_block(window.data());
        sl::Emulator::Status status = emu.step();
        // a repeat: the instruction is not over. It ends after 65,536 rounds, as BC's count would (BC as the pointer too never counts down)
        for (unsigned round = 1; status == sl::Emulator::Status::Ok && repeats && (emu.pc() & 0xFFFFFF) == pc; ++round) {
            ++steps;
            if (round == 0x10000) {
                emu.setPc((pc + 2) & 0xFFFFFF);
                break;
            }
            status = emu.step();
        }
        if (status != sl::Emulator::Status::Ok) {
            result.end = t::End::Stopped;
            result.fault = status == sl::Emulator::Status::BadInstruction ? t::Fault{pc, std::format("undefined instruction at {:06X}", pc)}
                                                                          : t::Fault{pc, std::format("{} at {:06X}", emu.lastError(), pc)};
            break;
        }
        ++steps;
    }
    get_registers(emu, r, g);
    if (result.end == t::End::Limit && g.pc == t::kReturnAddress) result.end = t::End::Return;
    result.regs = g;
    result.steps = steps;
    result.swis = std::move(swis);
    result.io.assign(bus.log().begin() + static_cast<std::ptrdiff_t>(logged_before), bus.log().end());
    return result;
}

}  // namespace cb::engine::pcode_cpu
