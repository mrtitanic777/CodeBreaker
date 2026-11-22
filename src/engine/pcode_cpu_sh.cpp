// engine/pcode_cpu_sh.cpp -- the SuperH lanes' SH-2 (the Saturn, the 32X) and SH-4 (the Dreamcast) on the one p-code emulator: emulate::sh2::run's call, on the descriptions
// languages/sh-2.sla and SuperH4_le.sla.
//
// Carried here: a delayed branch and its slot as one step of the emulator (counted as the own core counts them, two), the faults (an odd pc, an address nothing
// answers, a word or longword access that is not aligned, a word that is no instruction, a branch or a PC reader in a delay slot, TRAPA and SLEEP), the bus's
// memory, devices and on-chip registers (every data access is the bus's), the BIOS stubs the bus enters, and the registers as the own core hands them back. The
// instructions the descriptions leave out or do not carry the machine's state for (the SH-4's FPU, register banks, status register moves and cache instructions;
// MAC.W and MAC.L's saturation) and a delay slot the own core refuses run as the own core runs them, on the registers as they stand.
#include <algorithm>
#include <array>
#include <cstring>
#include <string>
#include <unordered_map>
#include <vector>

#include "decompile/engine/pcode_emulator.hh"
#include "decompile/sh2.hpp"
#include "engine/pcode_cpu.hpp"
#include "engine/pcode_cpu_internal.hpp"

namespace cb::engine::pcode_cpu {

namespace sh = cb::emulate::sh2;
namespace sl = cb::decompile::sleigh;
namespace ds = cb::decompile::sh2;

namespace {

constexpr std::uint32_t kShPage = 0x1000;

struct ShCodePage {
    std::vector<std::uint8_t> bytes = std::vector<std::uint8_t>(kShPage, 0);
    std::uint32_t run = 0;
};

struct ShMirror {
    std::unordered_map<std::uint32_t, ShCodePage> pages;   // by virtual page address
    std::uint32_t run = 0;
    bool configured = false;
    std::array<sl::Emulator::RegRef, 16> r{};   // the registers a step reads and writes, found once: a lookup by name each time is most of what the adapter costs
    sl::Emulator::RegRef pr{}, gbr{}, vbr{}, mach{}, macl{}, sr{};
};

ShMirror& sh_mirror(const void* emulator) {
    thread_local std::unordered_map<const void*, ShMirror> mirrors;
    return mirrors[emulator];
}

// What each word is, from the table the disassembler reads.
struct ShWord {
    bool valid = false;
    bool delayed = false;     // a branch with a delay slot
    bool slot_bad = false;    // a branch, or an instruction that reads the pc or traps: illegal in a delay slot
    bool trap = false, sleep = false;
    bool sysload = false;     // LDC.L / LDS.L: loads a system register from memory
    bool links = false;       // BSR, BSRF, JSR: set PR
    bool touches_memory = false;   // an operand is a memory access: a fault leaves the status (the T bit) as it was
    bool own = false;         // run by the own core: the description does not carry what it needs (see the file's head)
    std::uint8_t trap_code = 0;
};

const std::vector<ShWord>& sh_words(ds::Variant variant) {
    static const std::array<std::vector<ShWord>, 2> tables = [] {
        std::array<std::vector<ShWord>, 2> out;
        for (const ds::Variant v : {ds::Variant::Sh2, ds::Variant::Sh4}) {
            std::vector<ShWord>& row = out[v == ds::Variant::Sh4 ? 1 : 0];
            row.resize(0x10000);
            for (std::uint32_t w = 0; w < 0x10000; ++w) {
                const std::optional<ds::Insn> insn = ds::decode_word(static_cast<std::uint16_t>(w), 0, v);
                ShWord& e = row[w];
                if (!insn) continue;
                e.valid = true;
                e.delayed = insn->delay_slot;
                const std::string& m = insn->mnemonic;
                e.trap = m == "trapa";
                e.sleep = m == "sleep";
                e.sysload = m == "ldc.l" || m == "lds.l";
                e.links = m == "bsr" || m == "bsrf" || m == "jsr";
                const auto memory = [](ds::Operand o) {
                    switch (o) {
                        case ds::Operand::IndN: case ds::Operand::IndM: case ds::Operand::PostN: case ds::Operand::PostM: case ds::Operand::PreN: case ds::Operand::DispN:
                        case ds::Operand::DispM: case ds::Operand::R0IndexN: case ds::Operand::R0IndexM: case ds::Operand::GbrDisp: case ds::Operand::GbrR0: return true;
                        default: return false;
                    }
                };
                e.touches_memory = memory(insn->a) || memory(insn->b);
                e.trap_code = static_cast<std::uint8_t>(w & 0xFF);
                e.slot_bad = insn->flow != ds::Flow::None || insn->literal.has_value() || e.trap || e.sleep;
                e.own = m == "mac.w" || m == "mac.l";
                if (v == ds::Variant::Sh4) {
                    const auto special = [](ds::Operand o) {
                        switch (o) {
                            case ds::Operand::Sr: case ds::Operand::Ssr: case ds::Operand::Spc: case ds::Operand::Sgr: case ds::Operand::Dbr: case ds::Operand::BankM:
                            case ds::Operand::Fpscr: case ds::Operand::Fpul: case ds::Operand::FrN: case ds::Operand::FrM: case ds::Operand::MovN: case ds::Operand::MovM:
                            case ds::Operand::DrN: case ds::Operand::FvN: case ds::Operand::FvM: case ds::Operand::Xmtrx: return true;
                            default: return false;
                        }
                    };
                    if ((w >> 12) == 0xF || special(insn->a) || special(insn->b) || m == "rte" || m == "pref" || m == "ocbi" || m == "ocbp" || m == "ocbwb" || m == "movca.l" ||
                        m == "ldtlb" || m == "shad" || m == "shld" || m == "ldc" || m == "stc" || m == "ldc.l" || m == "stc.l")
                        e.own = true;
                }
            }
        }
        return out;
    }();
    return tables[variant == ds::Variant::Sh4 ? 1 : 0];
}

}  // namespace

sh::RunResult run(sh::Bus& bus, std::uint32_t entry, const sh::RunOptions& options) {
    const bool sh4 = bus.variant() == ds::Variant::Sh4;
    sl::Emulator& emu = emulator_named(sh4 ? "SuperH4_le.sla" : "sh-2.sla");
    const std::size_t log_start = bus.log().size();
    ShMirror& mir = sh_mirror(&emu);
    if (!mir.configured) {
        emu.keepAddressContext(false);
        static const char* const kShNames[16] = {"r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7", "r8", "r9", "r10", "r11", "r12", "r13", "r14", "r15"};
        for (unsigned i = 0; i < 16; ++i) mir.r[i] = emu.regRef(kShNames[i]);
        mir.pr = emu.regRef(sh4 ? "PR" : "pr");
        mir.gbr = emu.regRef(sh4 ? "GBR" : "gbr");
        mir.vbr = emu.regRef(sh4 ? "VBR" : "vbr");
        mir.mach = emu.regRef(sh4 ? "MACH" : "mach");
        mir.macl = emu.regRef(sh4 ? "MACL" : "macl");
        if (!sh4) mir.sr = emu.regRef("sr");   // (the SH-4's status is its flag registers: get_sr)
        mir.configured = true;
    }
    ++mir.run;
    const std::vector<ShWord>& words = sh_words(bus.variant());

    // The state only the own core carries (the SH-4's banks, FPU and system registers) lives in this CPU, which also runs what is left to the own core.
    sh::Cpu scratch(bus);
    scratch.gbr = options.gbr;
    scratch.vbr = options.vbr;
    if (sh4) {
        scratch.fpscr = options.fpscr & sh::fpscr::kMask;
        for (std::size_t i = 0; i < options.fa.size(); ++i) scratch.fp[(scratch.fpscr & sh::fpscr::FR) != 0 ? 1 : 0][4 + i] = options.fa[i];
    }

    // A page of code is read from the bus the first time an instruction is fetched from it in a run (and handed to the emulator when it differs from what the
    // emulator holds); what the program writes to a page already read is written to the emulator too. Addresses are virtual: the cache views mirror each other.
    std::uint32_t loaded_base = 1;   // (the page last looked at: nearly every fetch is on it. No page starts at an odd address.)
    const auto load_page = [&](std::uint32_t address) {
        const std::uint32_t base = address & ~(kShPage - 1);
        if (base == loaded_base) return;
        loaded_base = base;
        ShCodePage& page = mir.pages[base];
        if (page.run == mir.run) return;
        page.run = mir.run;
        std::uint8_t now[kShPage] = {};
        const sh::Bus& cbus = bus;
        const ByteView region = cbus.region(base);
        const std::uint32_t lo = bus.region_base(base);
        const std::uint32_t physical = base & sh::kPhysicalMask;
        if (!region.empty() && physical >= lo && physical - lo < region.size()) {
            const std::size_t n = std::min<std::size_t>(kShPage, region.size() - (physical - lo));
            std::memcpy(now, region.data() + (physical - lo), n);
        }
        if (std::memcmp(page.bytes.data(), now, kShPage) != 0) {
            std::memcpy(page.bytes.data(), now, kShPage);
            emu.write(base, now, kShPage);
        }
    };

    std::optional<sh::Fault> hook_fault;
    std::uint32_t current_pc = 0;
    emu.onRead([&](std::string_view space, std::uint64_t addr, unsigned size, std::uint64_t& value) {
        if (!is_ram(space)) return false;
        const std::uint32_t a = static_cast<std::uint32_t>(addr);
        if (size > 1 && a % size != 0) {
            if (!hook_fault) hook_fault = sh::Fault{sh::Fault::Kind::AddressError, current_pc, a, 0};
            value = 0;
            return true;
        }
        if (!bus.mapped(a)) {
            if (!hook_fault) hook_fault = sh::Fault{sh::Fault::Kind::BusError, current_pc, a, 0};
            value = 0;
            return true;
        }
        value = bus.read(a, size);
        return true;
    });
    emu.onWrite([&](std::string_view space, std::uint64_t addr, unsigned size, std::uint64_t value) {
        if (!is_ram(space)) return false;
        const std::uint32_t a = static_cast<std::uint32_t>(addr);
        if (size > 1 && a % size != 0) {
            if (!hook_fault) hook_fault = sh::Fault{sh::Fault::Kind::AddressError, current_pc, a, 0};
            return true;
        }
        if (!bus.mapped(a)) {
            if (!hook_fault) hook_fault = sh::Fault{sh::Fault::Kind::BusError, current_pc, a, 0};
            return true;
        }
        bus.write(a, size, static_cast<std::uint32_t>(value));
        const auto page = mir.pages.find(a & ~(kShPage - 1));
        if (page != mir.pages.end() && page->second.run == mir.run) {   // (the program may run what it wrote: every view of the memory)
            const std::uint32_t physical = a & sh::kPhysicalMask;
            for (const auto& [base, p] : mir.pages) {
                if (p.run != mir.run || (base & sh::kPhysicalMask) != (physical & ~(kShPage - 1))) continue;
                ShCodePage& target = mir.pages[base];
                const sh::Bus& cbus = bus;
                const ByteView region = cbus.region(base);
                const std::uint32_t lo = bus.region_base(base);
                if (region.empty()) continue;
                const std::size_t offset = (base & sh::kPhysicalMask) - lo;
                const std::size_t n = std::min<std::size_t>(kShPage, region.size() - offset);
                std::memcpy(target.bytes.data(), region.data() + offset, n);
                emu.write(base, region.data() + offset, n);
            }
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
    emu.onUserOp([&](sl::Emulator&, const std::string&, const std::vector<std::uint64_t>&, std::uint64_t&) { return true; });

    // ---- the registers
    const auto flag = [&](const char* name) { return static_cast<std::uint32_t>(emu.reg(name)) & 1; };
    std::uint32_t sr_rest = 0;   // SH-4: the bits of SR the flag registers do not carry
    const auto get_sr = [&]() -> std::uint32_t {
        if (!sh4) return static_cast<std::uint32_t>(emu.reg(mir.sr));
        return (sr_rest & ~0x303u) | flag("T") | flag("S") << 1 | flag("Q") << 8 | flag("M") << 9;
    };
    const auto set_sr = [&](std::uint32_t v) {
        if (!sh4) {
            emu.setReg(mir.sr, v);
            return;
        }
        sr_rest = v;
        emu.setReg("T", v & 1);
        emu.setReg("S", v >> 1 & 1);
        emu.setReg("Q", v >> 8 & 1);
        emu.setReg("M", v >> 9 & 1);
        emu.setReg("SR", v);
    };
    for (unsigned i = 0; i < 16; ++i) emu.setReg(mir.r[i], 0);
    emu.setReg(mir.r[0], options.r0);
    emu.setReg(mir.r[15], options.sp);
    for (std::size_t i = 0; i < options.a.size(); ++i) emu.setReg(mir.r[4 + i], options.a[i]);
    for (std::size_t i = 0; i < options.s.size(); ++i) emu.setReg(mir.r[8 + i], options.s[i]);
    emu.setReg(mir.pr, sh::kReturnAddress);
    emu.setReg(mir.gbr, options.gbr);
    emu.setReg(mir.vbr, options.vbr);
    emu.setReg(mir.mach, 0);
    emu.setReg(mir.macl, 0);
    set_sr(sh4 ? 0x700000F0u : 0xF0u);
    emu.setPc(entry);

    // The own core's view of the registers, for what it runs: and back.
    const auto to_scratch = [&](std::uint32_t pc) {
        for (unsigned i = 0; i < 16; ++i) scratch.r[i] = static_cast<std::uint32_t>(emu.reg(mir.r[i]));
        scratch.pc = pc;
        scratch.pr = static_cast<std::uint32_t>(emu.reg(mir.pr));
        scratch.gbr = static_cast<std::uint32_t>(emu.reg(mir.gbr));
        scratch.vbr = static_cast<std::uint32_t>(emu.reg(mir.vbr));
        scratch.mach = static_cast<std::uint32_t>(emu.reg(mir.mach));
        scratch.macl = static_cast<std::uint32_t>(emu.reg(mir.macl));
        scratch.sr = get_sr();
    };
    const auto from_scratch = [&]() {
        for (unsigned i = 0; i < 16; ++i) emu.setReg(mir.r[i], scratch.r[i]);
        emu.setReg(mir.pr, scratch.pr);
        emu.setReg(mir.gbr, scratch.gbr);
        emu.setReg(mir.vbr, scratch.vbr);
        emu.setReg(mir.mach, scratch.mach);
        emu.setReg(mir.macl, scratch.macl);
        set_sr(scratch.sr);
        emu.setPc(scratch.pc);
    };

    sh::RunResult out;
    out.end = sh::End::Limit;
    std::uint64_t steps = 0;
    const auto stop = [&](const sh::Fault& fault) {
        out.end = sh::End::Stopped;
        out.fault = fault;
    };
    while (steps < options.max_steps) {
        const std::uint32_t pc = static_cast<std::uint32_t>(emu.pc());
        if (pc == sh::kReturnAddress) {
            out.end = sh::End::Return;
            break;
        }
        current_pc = pc;
        if (pc % 2 != 0) {
            stop({sh::Fault::Kind::AddressError, pc, pc, 0});
            break;
        }
        if (!bus.is_ram(pc)) {
            stop({sh::Fault::Kind::BusError, pc, pc, 0});
            break;
        }
        load_page(pc);
        const std::uint16_t word = static_cast<std::uint16_t>(bus.peek(pc, 2));
        const ShWord& w = words[word];
        // a delayed branch whose slot the own core refuses (or whose slot is no memory): the own core runs both, with the faults it gives
        bool own = w.valid && w.own;
        if (w.valid && w.delayed) {
            const std::uint32_t slot = pc + 2;
            if (!bus.is_ram(slot)) own = true;
            else {
                load_page(slot);
                const ShWord& s = words[static_cast<std::uint16_t>(bus.peek(slot, 2))];
                if (!s.valid || s.slot_bad || s.own) own = true;
            }
        }
        if (!own && bus.has_enter_hooks()) {   // (what the own core runs enters the bus's stubs itself)
            std::array<std::uint32_t, 16> regs{};
            for (unsigned i = 0; i < 16; ++i) regs[i] = static_cast<std::uint32_t>(emu.reg(mir.r[i]));
            bus.enter(pc, regs);
        }
        if (!w.valid) {
            stop({sh::Fault::Kind::NoInstruction, pc, 0, 0});
            break;
        }
        if (w.sleep) {
            stop({sh::Fault::Kind::Sleep, pc, 0, 0});
            break;
        }
        if (w.trap) {
            stop({sh::Fault::Kind::Trap, pc, 0, w.trap_code});
            break;
        }
        if (own) {
            to_scratch(pc);
            const std::uint64_t before = scratch.steps;
            const std::optional<sh::Fault> fault = scratch.step();
            steps += scratch.steps - before;
            from_scratch();
            if (fault) {
                stop(*fault);
                break;
            }
            continue;
        }
        // an access that fails leaves the registers it would have changed as they were: the description has written back by then
        // (a delayed branch's slot is the one that accesses memory: its registers are the ones to keep)
        const std::uint16_t slot_word = w.delayed ? static_cast<std::uint16_t>(bus.peek(pc + 2, 2)) : word;
        const unsigned n = word >> 8 & 15, m = word >> 4 & 15, n2 = slot_word >> 8 & 15, m2 = slot_word >> 4 & 15;
        const std::uint32_t before_n = static_cast<std::uint32_t>(emu.reg(mir.r[n])), before_m = static_cast<std::uint32_t>(emu.reg(mir.r[m])),
                            before_n2 = static_cast<std::uint32_t>(emu.reg(mir.r[n2])), before_m2 = static_cast<std::uint32_t>(emu.reg(mir.r[m2])),
                            before_r0 = static_cast<std::uint32_t>(emu.reg(mir.r[0])), before_sp = static_cast<std::uint32_t>(emu.reg(mir.r[15]));
        const ShWord& slot_kind = words[slot_word];
        std::uint32_t before_sys[6] = {};
        if (slot_kind.touches_memory) before_sys[0] = get_sr();
        if (slot_kind.sysload) {   // (LDC.L and LDS.L load a system register from memory)
            before_sys[1] = static_cast<std::uint32_t>(emu.reg(mir.gbr));
            before_sys[2] = static_cast<std::uint32_t>(emu.reg(mir.vbr));
            before_sys[3] = static_cast<std::uint32_t>(emu.reg(mir.mach));
            before_sys[4] = static_cast<std::uint32_t>(emu.reg(mir.macl));
            before_sys[5] = static_cast<std::uint32_t>(emu.reg(mir.pr));
        }
        hook_fault.reset();
        const sl::Emulator::Status status = emu.step();
        if (status != sl::Emulator::Status::Ok) {
            stop({sh::Fault::Kind::NoInstruction, pc, 0, 0});
            break;
        }
        if (hook_fault) {
            emu.setReg(mir.r[n], before_n);
            emu.setReg(mir.r[m], before_m);
            emu.setReg(mir.r[n2], before_n2);
            emu.setReg(mir.r[m2], before_m2);
            emu.setReg(mir.r[0], before_r0);
            emu.setReg(mir.r[15], before_sp);
            if (slot_kind.touches_memory) set_sr(before_sys[0]);
            if (slot_kind.sysload) {
                emu.setReg(mir.gbr, before_sys[1]);
                emu.setReg(mir.vbr, before_sys[2]);
                emu.setReg(mir.mach, before_sys[3]);
                emu.setReg(mir.macl, before_sys[4]);
                emu.setReg(mir.pr, w.links ? pc + 4 : before_sys[5]);   // (a branch that links has set PR before its slot ran)
            }
            emu.setPc(pc);
            if (w.delayed) ++steps;   // (the branch itself ran)
            stop(*hook_fault);
            break;
        }
        steps += w.delayed ? 2 : 1;
    }
    if (out.end == sh::End::Limit && static_cast<std::uint32_t>(emu.pc()) == sh::kReturnAddress) out.end = sh::End::Return;
    for (unsigned i = 0; i < 16; ++i) out.r[i] = static_cast<std::uint32_t>(emu.reg(mir.r[i]));
    out.pr = static_cast<std::uint32_t>(emu.reg(mir.pr));
    out.gbr = static_cast<std::uint32_t>(emu.reg(mir.gbr));
    out.vbr = static_cast<std::uint32_t>(emu.reg(mir.vbr));
    out.mach = static_cast<std::uint32_t>(emu.reg(mir.mach));
    out.macl = static_cast<std::uint32_t>(emu.reg(mir.macl));
    out.sr = get_sr();
    out.fpscr = scratch.fpscr;
    out.fpul = scratch.fpul;
    for (unsigned i = 0; i < 16; ++i) out.fr[i] = scratch.fr(i);
    out.steps = steps;
    out.io.assign(bus.log().begin() + static_cast<std::ptrdiff_t>(log_start), bus.log().end());
    return out;
}

sh::RunResult call(sh::Bus& bus, std::uint32_t entry, const sh::RunOptions& options) {
    return enabled(Core::SuperH) ? pcode_cpu::run(bus, entry, options) : sh::run(bus, entry, options);
}

}  // namespace cb::engine::pcode_cpu
