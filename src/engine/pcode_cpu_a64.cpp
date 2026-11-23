// cb-unity: alone (file-local names that other files in a batch also use)
// engine/pcode_cpu_a64.cpp -- the Switch lane's AArch64 on the one emulator (pcode_cpu_a64.hpp).
#include "engine/pcode_cpu_a64.hpp"

#include <array>
#include <cstring>
#include <mutex>
#include <string>
#include <string_view>
#include <unordered_map>

#include "decompile/a64.hpp"
#include "decompile/engine/pcode_emulator.hh"
#include "engine/pcode_cpu_internal.hpp"

namespace cb::engine::pcode_cpu {

namespace ea = cb::emulate::a64;
namespace da = cb::decompile::a64;
namespace sl = cb::decompile::sleigh;

namespace {

using u32 = std::uint32_t;
using u64 = std::uint64_t;

// ---- what goes where -----------------------------------------------------------------------------------------------------------------------------------------------

struct Route {
    bool own = true;             // the own core runs it
    bool v = false;              // it reads or writes the vector registers
    bool nan_check = false;      // an arithmetic result that is a NaN is the own core's (the host's NaN of an invalid operation is negative, the description's positive)
    bool mem = false;            // it makes a data access
    bool exclusive_load = false; // a load-exclusive: the monitor is set once it has run
    std::string_view mnemonic;   // the own core's decoder's
};

// Whether the own core runs the word, or stops at it as "not run" (a data abort is the state's, not the word's: that run is the own core's too, but it runs the instruction).
bool own_core_runs(u32 word) {
    static constexpr u64 kCode = 0x100000;
    auto regions = std::make_shared<std::vector<ea::Region>>();
    ea::Region r;
    r.address = kCode;
    for (unsigned i = 0; i < 4; ++i) r.bytes.push_back(static_cast<std::uint8_t>(word >> (8 * i)));
    regions->push_back(std::move(r));
    ea::Memory memory(regions);
    memory.map_writable(0, 0x80000);
    ea::Cpu cpu(memory);
    cpu.regs.pc = kCode;
    const std::optional<std::string> why = cpu.step();
    return !why || why->starts_with("data abort");
}

// The mnemonics whose instructions the description gets differently from the own core (sorted).
constexpr std::string_view kOwnMnemonics[] = {
    "fcvtas", "fcvtau", "fcvtms", "fcvtmu", "fcvtns", "fcvtnu", "fcvtps", "fcvtpu", "fcvtzs", "fcvtzu",   // (the float-to-integer conversions: the emulator's float-to-integer cast gives 0)
    "scvtf", "ucvtf",                                                                                    // (integer to float: the emulator converts the unsigned ones as signed)
    "fmax", "fmin", "fmaxnm", "fminnm",                                                                  // (a NaN operand: the description's result is not the own core's)
    "fmsub", "fnmadd", "fnmsub",                                                                         // (the fused forms: the sign of a NaN and of zero)
    "frinta", "frinti", "frintm", "frintn", "frintp", "frintx", "frintz",                                 // (the rounding: the description gives 0 for some)
    "smulh", "umulh",                                                                                    // (the description multiplies 128 bits, the emulator's values are 64)
    "sbcs", "ngcs", "fmadd",
};

// A load or store whose register is also its base with write-back, or whose pair is one register twice: the architecture leaves the result unpredictable, the own core's is the
// write-back after the load, the description's the load last.
bool unpredictable_access(u32 w, da::Op op) {
    const unsigned rd = w & 31, rn = (w >> 5) & 31, rt2 = (w >> 10) & 31;
    if (op == da::Op::LdStImm) {
        const bool writeback = ((w >> 24) & 1) == 0 && ((w >> 10) & 1) == 1;
        return writeback && rn == rd && rn != 31;
    }
    if (op == da::Op::LdStPair) {
        const bool writeback = ((w >> 23) & 3) == 1 || ((w >> 23) & 3) == 3;
        const bool load = ((w >> 22) & 1) != 0;
        return (writeback && rn != 31 && (rn == rd || rn == rt2)) || (load && rd == rt2);
    }
    return false;
}

bool in_own_list(std::string_view mnemonic) {
    for (const std::string_view m : kOwnMnemonics)
        if (m == mnemonic) return true;
    return false;
}

Route make_route(u32 w) {
    Route r;
    const std::optional<da::Insn> insn = da::decode_word(w, 0x100000);
    if (!insn) {
        r.mnemonic = "(undefined)";
        return r;
    }
    r.mnemonic = insn->mnemonic;
    const auto bits = [&](unsigned pos, unsigned width) -> u32 { return (w >> pos) & ((1u << width) - 1); };
    bool ok = false;
    switch (insn->op) {
        case da::Op::Adr: case da::Op::Adrp: case da::Op::AddSubImm: case da::Op::AddSubShift: case da::Op::AddSubExt: case da::Op::AddSubCarry: case da::Op::LogicImm:
        case da::Op::LogicShift: case da::Op::MoveWide: case da::Op::Bitfield: case da::Op::Extract: case da::Op::CondCmp: case da::Op::CondSel: case da::Op::Dp3: case da::Op::Dp2:
        case da::Op::Dp1: case da::Op::B: case da::Op::Bl: case da::Op::BCond: case da::Op::Cbz: case da::Op::Tbz: case da::Op::Br: case da::Op::Blr: case da::Op::Ret:
        case da::Op::Dsb: case da::Op::Dmb: case da::Op::Isb: case da::Op::Clrex: ok = true; break;
        case da::Op::Hint: ok = bits(5, 7) <= 5; break;   // (nop, yield, wfe, wfi, sev, sevl; the pointer-authentication hints change registers the own core leaves alone)
        case da::Op::LdStImm: case da::Op::LdStReg: {
            const bool simd = bits(26, 1) != 0;
            ok = !(simd && bits(22, 2) >= 2) && !(!simd && bits(22, 2) == 3) && !unpredictable_access(w, insn->op);   // (not a 16-byte access, nor a sign-extending load into a W register: the description extends to 64 bits)
            r.v = simd;
            r.mem = true;
            break;
        }
        case da::Op::LdLit: {
            const bool simd = bits(26, 1) != 0;
            ok = !(simd && bits(30, 2) == 2);
            r.v = simd;
            r.mem = true;
            break;
        }
        case da::Op::LdStPair: {
            const bool simd = bits(26, 1) != 0;
            ok = !(simd && bits(30, 2) == 2) && !unpredictable_access(w, insn->op);
            r.v = simd;
            r.mem = true;
            break;
        }
        case da::Op::LdStExcl: {
            const bool o2 = bits(23, 1), is_load = bits(22, 1), pair = bits(21, 1);
            ok = !(o2 && pair) && !(pair && !is_load && bits(30, 2) == 3);   // (compare-and-swap pair; the description's STXP of two 64-bit registers writes the second 4 bytes on)
            r.mem = true;
            r.exclusive_load = is_load && (!o2 || pair);
            break;
        }
        case da::Op::Sys:   // dc zva (the other cache and translation operations the own core ignores)
            ok = (w & 0xFFFFFFE0u) == 0xD50B7420u;
            r.mem = true;
            break;
        case da::Op::Mrs: ok = (w & 0xFFFFFFE0u) == 0xD53BD040u || (w & 0xFFFFFFE0u) == 0xD53BD060u; break;   // (tpidr_el0 and tpidrro_el0)
        case da::Op::Fp1Src: case da::Op::Fp2Src: case da::Op::Fp3Src: case da::Op::FpCmp: case da::Op::FpCCmp: case da::Op::FpCSel: case da::Op::FpImm: case da::Op::FpInt:
        case da::Op::FpFixed:
            ok = true;
            r.v = true;
            r.nan_check = insn->op == da::Op::Fp1Src || insn->op == da::Op::Fp2Src || insn->op == da::Op::Fp3Src;
            break;
        default: break;
    }
    if (ok && (in_own_list(r.mnemonic) || !own_core_runs(w))) ok = false;
    r.own = !ok;
    return r;
}

const Route& route_for(u32 word) {
    thread_local std::unordered_map<u32, Route> known;
    const auto [slot, fresh] = known.try_emplace(word);
    if (fresh) slot->second = make_route(word);
    return slot->second;
}

// ---- the emulator's registers --------------------------------------------------------------------------------------------------------------------------------------

struct Refs {
    std::array<sl::Emulator::RegRef, 31> x;
    sl::Emulator::RegRef sp, xzr, ng, zr, cy, ov, fpcr, fpsr, tpidr, tpidrro;
    std::array<sl::Emulator::RegRef, 32> vlo, vhi;
};

const Refs& refs_of(const sl::Emulator& emu) {
    thread_local const sl::Emulator* owner = nullptr;
    thread_local Refs refs;
    if (owner != &emu) {
        for (unsigned i = 0; i < 31; ++i) refs.x[i] = emu.regRef("x" + std::to_string(i));
        refs.sp = emu.regRef("sp");
        refs.xzr = emu.regRef("xzr");
        refs.ng = emu.regRef("NG");
        refs.zr = emu.regRef("ZR");
        refs.cy = emu.regRef("CY");
        refs.ov = emu.regRef("OV");
        refs.fpcr = emu.regRef("fpcr");
        refs.fpsr = emu.regRef("fpsr");
        refs.tpidr = emu.regRef("tpidr_el0");
        refs.tpidrro = emu.regRef("tpidrro_el0");
        for (unsigned i = 0; i < 32; ++i) {   // (the low and high half of the 128-bit register: the emulator's registers are read 8 bytes at a time)
            refs.vlo[i] = emu.regRef("d" + std::to_string(i));
            refs.vhi[i] = refs.vlo[i];
            refs.vhi[i].offset += 8;
        }
        owner = &emu;
    }
    return refs;
}

// ---- the share -----------------------------------------------------------------------------------------------------------------------------------------------------

std::mutex g_share_lock;
A64Share g_share;

}  // namespace

A64Share a64_share() {
    const std::lock_guard guard(g_share_lock);
    return g_share;
}

void reset_a64_share() {
    const std::lock_guard guard(g_share_lock);
    g_share = {};
}

A64End run_a64(ea::Cpu& cpu, u64 max_steps) {
    sl::Emulator& emu = emulator_named("AARCH64.sla");
    const Refs& R = refs_of(emu);
    ea::Memory& memory = cpu.memory();
    ea::Registers& regs = cpu.regs;

    bool failed = false;   // a data access Memory did not answer
    emu.onRead([&](std::string_view space, u64 addr, unsigned size, u64& value) {
        if (!is_ram(space)) return false;
        if (!memory.read(addr, size, value)) {
            failed = true;
            value = 0;
        }
        return true;
    });
    emu.onWrite([&](std::string_view space, u64 addr, unsigned size, u64 value) {
        if (!is_ram(space)) return false;
        if (!memory.write(addr, size, value)) failed = true;
        return true;
    });
    // the exclusive monitor is the own core's (the description leaves it to the machine); the other operations (the barriers, hints) change nothing
    emu.onUserOp([&](sl::Emulator&, const std::string& name, const std::vector<u64>& args, u64& result) {
        if (name == "ExclusiveMonitorPass") {
            result = cpu.monitor_set() && !args.empty() && cpu.monitor_address() == args[0] ? 1 : 0;
            cpu.set_monitor(false, 0);
        } else if (name == "ExclusiveMonitorsStatus") {
            result = 0;
        } else if (name == "DC_ZVA") {   // the 64-byte block zeroed, as the own core does it
            static const std::uint8_t zero[64] = {};
            if (!args.empty() && !memory.write_bytes(args[0] & ~u64{63}, zero, 64)) failed = true;
        } else if (name == "ClearExclusiveLocal") {
            cpu.set_monitor(false, 0);
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

    // Which copy of the registers is current: the Cpu's, the emulator's, or both. (The general registers, SP, the flags and the pc, and the vector registers, apart.)
    bool g_cpu = true, g_emu = false, v_cpu = true, v_emu = false;
    const auto flags_to_emu = [&] {
        emu.setReg(R.ng, (regs.nzcv >> 31) & 1);
        emu.setReg(R.zr, (regs.nzcv >> 30) & 1);
        emu.setReg(R.cy, (regs.nzcv >> 29) & 1);
        emu.setReg(R.ov, (regs.nzcv >> 28) & 1);
    };
    const auto ensure_emu = [&](bool vector) {
        if (!g_emu) {
            for (unsigned i = 0; i < 31; ++i) emu.setReg(R.x[i], regs.x[i]);
            emu.setReg(R.sp, regs.sp);
            emu.setReg(R.xzr, 0);
            flags_to_emu();
            emu.setReg(R.fpcr, regs.fpcr);
            emu.setReg(R.fpsr, regs.fpsr);
            emu.setReg(R.tpidr, regs.tpidr_el0);
            emu.setReg(R.tpidrro, regs.tpidrro_el0);
            g_emu = true;
        }
        if (vector && !v_emu) {
            for (unsigned i = 0; i < 32; ++i) {
                emu.setReg(R.vlo[i], regs.v[i].lo);
                emu.setReg(R.vhi[i], regs.v[i].hi);
            }
            v_emu = true;
        }
    };
    const auto ensure_cpu = [&] {
        if (!g_cpu) {
            for (unsigned i = 0; i < 31; ++i) regs.x[i] = emu.reg(R.x[i]);
            regs.sp = emu.reg(R.sp);
            regs.nzcv = static_cast<u32>((emu.reg(R.ng) & 1) << 31 | (emu.reg(R.zr) & 1) << 30 | (emu.reg(R.cy) & 1) << 29 | (emu.reg(R.ov) & 1) << 28);
            g_cpu = true;
        }
        if (!v_cpu) {
            for (unsigned i = 0; i < 32; ++i) {
                regs.v[i].lo = emu.reg(R.vlo[i]);
                regs.v[i].hi = emu.reg(R.vhi[i]);
            }
            v_cpu = true;
        }
    };

    A64End end;
    std::map<std::string, u64> own_counts;
    u64 emu_steps = 0, own_steps = 0;
    while (true) {
        if (regs.pc == ea::kReturnAddress) {
            end.end = ea::End::Return;
            break;
        }
        if (cpu.steps >= max_steps) {
            end.end = ea::End::Limit;
            break;
        }
        const u64 pc = regs.pc;
        std::uint8_t code[4];
        const Route* route = nullptr;
        if (memory.read_bytes(pc, 4, code)) {
            const u32 word = static_cast<u32>(code[0]) | static_cast<u32>(code[1]) << 8 | static_cast<u32>(code[2]) << 16 | static_cast<u32>(code[3]) << 24;
            route = &route_for(word);
        }
        std::string_view own_mnemonic = route ? route->mnemonic : "(fetch)";
        if (route && !route->own) {
            const u32 word = static_cast<u32>(code[0]) | static_cast<u32>(code[1]) << 8 | static_cast<u32>(code[2]) << 16 | static_cast<u32>(code[3]) << 24;
            const unsigned rd = word & 31, rn = (word >> 5) & 31, rt2 = (word >> 10) & 31, rs = (word >> 16) & 31;
            ensure_emu(route->v);
            // what a memory instruction could have written, to be put back when an access fails
            struct Saved {
                sl::Emulator::RegRef ref;
                u64 value;
            };
            Saved saved[12];
            unsigned count = 0;
            if (route->mem) {
                const auto keep_x = [&](unsigned n) {
                    if (n < 31) saved[count++] = {R.x[n], emu.reg(R.x[n])};
                };
                keep_x(rd);
                keep_x(rt2);
                keep_x(rs);
                if (rn < 31) keep_x(rn);
                else saved[count++] = {R.sp, emu.reg(R.sp)};
                if (route->v) {
                    saved[count++] = {R.vlo[rd], emu.reg(R.vlo[rd])};
                    saved[count++] = {R.vhi[rd], emu.reg(R.vhi[rd])};
                    saved[count++] = {R.vlo[rt2], emu.reg(R.vlo[rt2])};
                    saved[count++] = {R.vhi[rt2], emu.reg(R.vhi[rt2])};
                }
            }
            if (route->nan_check) {
                saved[count++] = {R.vlo[rd], emu.reg(R.vlo[rd])};
                saved[count++] = {R.vhi[rd], emu.reg(R.vhi[rd])};
            }
            const u64 base = route->exclusive_load ? emu.reg(rn < 31 ? R.x[rn] : R.sp) : 0;
            emu.write(pc, code, 4);
            emu.setPc(pc);
            failed = false;
            const sl::Emulator::Status status = emu.step();
            bool nan = false;
            if (route->nan_check && status == sl::Emulator::Status::Ok) {
                unsigned type = (word >> 22) & 3;
                if (route->mnemonic == "fcvt") type = ((word >> 15) & 7) == 4 ? 0 : ((word >> 15) & 7) == 5 ? 1 : 3;
                const u64 lo = emu.reg(R.vlo[rd]);
                nan = type == 0 ? (lo & 0x7F800000u) == 0x7F800000u && (lo & 0x7FFFFFu) != 0
                    : type == 1 ? (lo & 0x7FF0000000000000ull) == 0x7FF0000000000000ull && (lo & 0xFFFFFFFFFFFFFull) != 0
                                : (lo & 0x7C00u) == 0x7C00u && (lo & 0x3FFu) != 0;
            }
            if (status == sl::Emulator::Status::Ok && !failed && !nan) {
                emu.setReg(R.xzr, 0);
                regs.pc = emu.pc();
                ++cpu.steps;
                ++emu_steps;
                g_cpu = false;
                if (route->v) v_cpu = false;
                if (route->exclusive_load) cpu.set_monitor(true, base);
                continue;
            }
            for (unsigned i = 0; i < count; ++i) emu.setReg(saved[i].ref, saved[i].value);   // (nothing ran, or an access nothing answers: the own core runs it and makes the fault)
            emu.setReg(R.xzr, 0);
        }
        ensure_cpu();
        ++own_steps;
        ++own_counts[std::string(own_mnemonic)];
        const std::optional<std::string> why = cpu.step();
        g_emu = false;
        v_emu = false;
        if (why) {
            end.end = ea::End::Stopped;
            end.reason = *why;
            break;
        }
    }
    ensure_cpu();
    {
        const std::lock_guard guard(g_share_lock);
        g_share.emulator += emu_steps;
        g_share.own_core += own_steps;
        for (const auto& [name, n] : own_counts) g_share.own_core_by_mnemonic[name] += n;
    }
    return end;
}

ea::RunResult run(ea::Memory& memory, u64 entry, const ea::RunOptions& options) {
    ea::Cpu cpu(memory);
    cpu.regs.pc = entry;
    for (unsigned i = 0; i < 8; ++i) cpu.regs.x[i] = options.x[i];
    cpu.regs.x[18] = options.x18;
    cpu.regs.x[19] = options.x19;
    cpu.regs.x[29] = options.x29;
    cpu.regs.x[30] = ea::kReturnAddress;
    cpu.regs.sp = options.sp;
    cpu.regs.tpidr_el0 = options.tpidr_el0;
    cpu.regs.tpidrro_el0 = options.tpidrro_el0;
    cpu.svc = options.svc ? options.svc : ea::SvcHandler(ea::horizon_svc);
    ea::RunResult out;
    const A64End end = run_a64(cpu, options.max_steps);
    out.end = end.end;
    out.reason = end.reason;
    out.regs = cpu.regs;
    out.steps = cpu.steps;
    out.calls = std::move(cpu.calls);
    return out;
}

ea::RunResult call(ea::Memory& memory, u64 entry, const ea::RunOptions& options) {
    return enabled() && !options.on_instruction ? pcode_cpu::run(memory, entry, options) : ea::run(memory, entry, options);   // (a trace of every instruction is the own core's)
}

}  // namespace cb::engine::pcode_cpu
