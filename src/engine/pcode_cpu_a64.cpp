// cb-unity: alone (file-local names that other files in a batch also use)
// engine/pcode_cpu_a64.cpp -- the Switch lane's AArch64 on the one emulator (pcode_cpu_a64.hpp).
#include "engine/pcode_cpu_a64.hpp"

#include <algorithm>
#include <array>
#include "core/fs.hpp"
#include <cctype>
#include <cstdlib>
#include <format>
#include <optional>
#include <cstring>
#include <mutex>
#include <string>
#include <string_view>
#include <unordered_map>

#include "decompile/a64.hpp"
#include "decompile/engine/pcode_emulator.hh"
#include "engine/pcode_cpu_internal.hpp"
#include "engine/pcode_cpu_a64_fp.hpp"
#include "engine/pcode_cpu_a64_stops.hpp"
#include "engine/pcode_cpu_a64_vec.hpp"
#include "engine/pcode_ops_a64.hpp"

namespace cb::engine::pcode_cpu {

namespace ea = cb::emulate::a64;
namespace da = cb::decompile::a64;
namespace sl = cb::decompile::sleigh;

namespace {

using u32 = std::uint32_t;
using u64 = std::uint64_t;

// ---- what goes where -----------------------------------------------------------------------------------------------------------------------------------------------

constexpr u64 kProbeCode = 0x100000;

// Where an instruction runs: on the emulator (the description, with the lane library's operations behind its named ones), or in this file (what the lane keeps as its own semantics:
// the kernel call, the system registers, the stops, the instructions that change nothing).
enum class Kind : std::uint8_t { Emu, Nop, Clrex, DcZva, Svc, Mrs, Msr, Stop, Fp, Unpredictable, SubCarry, RoundConvert, FixedConvert, Fused, StorePair, VecNative, SignedDivide, CasPair };

// What is done around the emulator's step where the description makes the result differently from the manual and the own core: WSignExtend: a sign-extending load into a W
// register is zero extended to 32 bits (the description extends to 64); ResultInRn: SRHADD puts its result in Rn, not Rd; SourceInRd: the scalar SQSHRN and SQSHRUN read their
// source from Rd, not Rn.
enum class Fix : std::uint8_t { None, WSignExtend, ResultInRn, SourceInRd, CasOldValue };

struct Route {
    Kind kind = Kind::Emu;
    Fix fix = Fix::None;
    bool v = false;              // it reads or writes the vector registers
    bool nan_check = false;      // an arithmetic result that is a NaN is the own core's
    bool mem = false;            // it makes a data access
    bool exclusive_load = false; // a load-exclusive: the monitor is set once it has run
    bool counted = true;         // a stop that counts as an instruction (the own core's step counts a word it decoded)
    std::uint8_t vregs = 0;      // the vector registers from Rd that a data access may write
    std::uint8_t dest_bytes = 16;   // the width of the vector register(s) the instruction writes first (a scalar or 64-bit result zeroes the rest of the register: the description does not always)
    std::uint8_t dest_regs = 0;     // how many registers from Rd (0: it writes none, or all of them)
    std::int8_t nan_form = -1;      // a floating-point lane operation whose NaN lanes are the lane's, not the description's (a64fp::host_lanes' form), or -1
    bool v_rt2 = false;          // ... and Rt2
    std::uint8_t element = 0;    // the bytes of one element of a data access made of several in ascending order (a pair, a structure, a compare-and-swap pair); 0: one access
    std::uint8_t partial = 0;    // what a load leaves of the elements before a failing one: 1: the first register of a pair, 2: the first vector register of a pair, 3: the registers of a structure
    bool multi_store = false;    // it stores in more than one access: the stores are journaled, to be put back when a later one fails
    bool simd_abort = false;     // the abort of a structure load or store is reported as "simd access", read or write
    da::Op op = da::Op::None;
    std::string_view mnemonic;   // the own core's decoder's
    std::string stop;            // Kind::Stop: the reason up to " at <pc>"
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

// The width in bytes of the register(s) that the description prints first for the instruction (`saddlv h0,v1.16B`: 2; `ld1 {v0.8B,v1.8B},[x1]`: 8 for two registers; a lane
// (`v0.S[1]`) or a general register: none).
void destination_of(const std::string& text, Route& r) {
    r.dest_regs = 0;
    std::size_t at = text.find(' ');
    if (at == std::string::npos) return;
    while (at < text.size() && text[at] == ' ') ++at;
    const bool braces = at < text.size() && text[at] == '{';
    if (braces) ++at;
    unsigned regs = 0, bytes = 0;
    while (at < text.size()) {
        while (at < text.size() && text[at] == ' ') ++at;
        std::size_t end = at;
        while (end < text.size() && text[end] != ',' && text[end] != '}' && text[end] != ' ') ++end;
        const std::string item = text.substr(at, end - at);
        unsigned width = 0;
        if (item.size() >= 2 && std::string_view("bhsdqBHSDQ").find(item[0]) != std::string_view::npos && std::isdigit(static_cast<unsigned char>(item[1])) != 0) {
            width = item[0] == 'b' || item[0] == 'B' ? 1 : item[0] == 'h' || item[0] == 'H' ? 2 : item[0] == 's' || item[0] == 'S' ? 4 : item[0] == 'd' || item[0] == 'D' ? 8 : 16;
        } else if (item.size() >= 4 && (item[0] == 'v' || item[0] == 'V') && item.find('.') != std::string::npos && item.find('[') == std::string::npos) {
            const std::size_t dot = item.find('.');
            const unsigned count = static_cast<unsigned>(std::atoi(item.c_str() + dot + 1));
            const char kind = static_cast<char>(std::toupper(static_cast<unsigned char>(item.back())));
            const unsigned lane = kind == 'B' ? 1 : kind == 'H' ? 2 : kind == 'S' ? 4 : kind == 'D' ? 8 : kind == 'Q' ? 16 : 0;
            width = count * lane;
        }
        if (width == 0 || (regs != 0 && width != bytes)) break;
        ++regs;
        bytes = width;
        if (!braces || end >= text.size() || text[end] != ',') break;
        at = end + 1;
    }
    if (regs == 0) return;
    r.dest_regs = static_cast<std::uint8_t>(regs);
    r.dest_bytes = static_cast<std::uint8_t>(std::min(bytes, 16u));
}

Route make_route(u32 w, const std::string& description_text, bool everything) {
    Route r;
    const std::optional<da::Insn> insn = da::decode_word(w, kProbeCode);
    if (!insn && everything) {   // (a word only the description decodes: it runs it)
        r.mnemonic = "(description)";
        r.v = true;
        destination_of(description_text, r);
        return r;
    }
    if (!insn) {
        r.mnemonic = "(undefined)";
        r.kind = Kind::Stop;
        r.stop = std::format("undefined instruction {:08x}", w);
        r.counted = false;
        return r;
    }
    r.mnemonic = insn->mnemonic;
    r.op = insn->op;
    const auto bits = [&](unsigned pos, unsigned width) -> u32 { return (w >> pos) & ((1u << width) - 1); };
    switch (insn->op) {
        case da::Op::Svc: r.kind = Kind::Svc; return r;
        case da::Op::Brk: case da::Op::Hlt: case da::Op::Udf: case da::Op::Hvc: case da::Op::Smc: case da::Op::Eret:
            r.kind = Kind::Stop;
            r.stop = std::string(insn->mnemonic);
            return r;
        case da::Op::Hint: case da::Op::Dsb: case da::Op::Dmb: case da::Op::Isb: r.kind = Kind::Nop; return r;   // (the own core changes nothing; the pointer-authentication hints change registers on the description)
        case da::Op::Clrex: r.kind = Kind::Clrex; return r;
        case da::Op::Sys:
            r.kind = insn->mnemonic == "dc" && da::text(*insn).starts_with("dc zva") ? Kind::DcZva : Kind::Nop;
            return r;
        default: break;
    }
    if (const std::optional<std::string> why = everything ? std::nullopt : a64stops::stop_reason(*insn)) {   // (the instructions the lane stops at, "not run": its own semantics, from its table)
        r.kind = Kind::Stop;
        r.stop = *why;
        return r;
    }
    switch (insn->op) {
        case da::Op::Mrs: r.kind = Kind::Mrs; return r;
        case da::Op::Msr: r.kind = Kind::Msr; return r;
        case da::Op::LdStImm: case da::Op::LdStReg: {
            const bool simd = bits(26, 1) != 0;
            r.v = simd;
            r.mem = true;
            r.vregs = simd ? 1 : 0;
            if (!simd && bits(22, 2) == 3 && bits(30, 2) != 3) r.fix = Fix::WSignExtend;   // (a sign-extending load into a W register: the description extends to 64 bits)
            if (!simd && unpredictable_access(w, insn->op)) r.kind = Kind::Unpredictable;
            break;
        }
        case da::Op::LdLit: r.v = bits(26, 1) != 0; r.mem = true; r.vregs = r.v ? 1 : 0; break;
        case da::Op::LdStPair:
            r.v = bits(26, 1) != 0;
            r.element = static_cast<std::uint8_t>(r.v ? 4u << bits(30, 2) : bits(30, 2) == 2 ? 8 : 4);
            r.multi_store = bits(22, 1) == 0;
            r.partial = bits(22, 1) != 0 ? (r.v ? 2 : 1) : 0;
            r.mem = true;
            r.vregs = r.v ? 1 : 0;
            r.v_rt2 = r.v;
            if (!r.v && unpredictable_access(w, insn->op)) r.kind = Kind::Unpredictable;
            break;
        case da::Op::LdStExcl: {
            // (the decoder has no row for COMPARE AND SWAP, CAS and CASP: the own core stops at them as undefined instructions, so does the lane)
            const bool o2 = bits(23, 1), is_load = bits(22, 1), pair = bits(21, 1);
            r.mem = true;
            r.element = static_cast<std::uint8_t>(1u << bits(30, 2));
            r.multi_store = !is_load && pair;
            r.partial = is_load && pair ? 1 : 0;
            r.exclusive_load = is_load && !o2;
            if (!is_load && !o2 && pair) r.kind = Kind::StorePair;   // (STXP, STLXP: the description stores the second register 4 bytes on, whatever the size)
            break;
        }
        case da::Op::LdStAtomic: {   // CAS{A}{L}{B,H}: the description leaves what is above the old value (its size) in Rs; CASP is the lane's (the description's memory result is wrong)
            r.mem = true;
            r.element = static_cast<std::uint8_t>(bits(23, 1) == 0 ? 4u << bits(30, 1) : 1u << bits(30, 2));
            r.multi_store = true;
            if (bits(23, 1) == 0) {   // (an odd register of the pair is a stop of the table)
                r.kind = Kind::CasPair;
            } else {
                r.fix = Fix::CasOldValue;
            }
            break;
        }
        case da::Op::SimdLdStMulti: case da::Op::SimdLdStSingle:
            r.v = true;
            r.mem = true;
            r.vregs = 4;
            r.simd_abort = true;
            r.multi_store = bits(22, 1) == 0;
            if (insn->op == da::Op::SimdLdStMulti) {
                r.element = static_cast<std::uint8_t>(1u << bits(10, 2));
            } else {   // (the element of a single structure: the replicating forms by the size field, the lane forms by the opcode)
                const unsigned opc3 = bits(13, 3);
                r.element = static_cast<std::uint8_t>(opc3 >= 6 ? 1u << bits(10, 2) : (opc3 >> 1) == 0 ? 1 : (opc3 >> 1) == 1 ? 2 : bits(10, 2) == 0 ? 4 : 8);
                r.partial = bits(22, 1) != 0 ? 3 : 0;
            }
            if (bits(22, 1) != 0) destination_of(description_text, r);   // (a load: its registers' width)
            break;
        case da::Op::Fp1Src: {   // (the moves, abs, neg and sqrt of singles and doubles run on the description; the conversions, the rounding and the half-precision forms are the lane's)
            r.v = true;
            if (bits(22, 2) == 3 || bits(15, 6) >= 4) r.kind = Kind::Fp;
            else r.nan_check = true;
            break;
        }
        case da::Op::Fp2Src:   // (the maximum and minimum forms are the lane's: the description's NaN and zero rules differ)
            r.v = true;
            if (bits(12, 4) >= 4 && bits(12, 4) <= 7) r.kind = Kind::Fp;
            else r.nan_check = true;
            break;
        case da::Op::Fp3Src: r.v = true; r.kind = Kind::Fp; break;   // (the fused forms: the description's NaN and zero signs differ)
        case da::Op::FpInt:   // (fmov between the files is the description's; the conversions the lane's: the emulator's float-to-integer cast gives 0)
            r.v = true;
            if (bits(16, 3) < 6) r.kind = Kind::Fp;
            break;
        case da::Op::FpFixed: r.v = true; r.kind = Kind::Fp; break;
        case da::Op::FpCmp: case da::Op::FpCCmp: r.v = true; break;   // (they set the flags and write no vector register)
        case da::Op::FpCSel: case da::Op::FpImm: r.v = true; destination_of(description_text, r); break;
        case da::Op::Dp2:   // (SDIV: the emulator's signed division traps on the host for the most negative value over -1, so the lane divides itself)
            if (bits(10, 6) == 3) r.kind = Kind::SignedDivide;
            break;
        case da::Op::AddSubCarry:   // (the flag-setting subtract with carry: the description's carry and overflow differ)
            if (bits(29, 1) != 0 && bits(30, 1) != 0) r.kind = Kind::SubCarry;
            break;
        case da::Op::SimdTwoMisc: case da::Op::SimdScalarTwoMisc:   // (the rounding and the float-to-integer conversions: the emulator's cast gives 0, its rounding is not the lane's)
            r.v = true;
            if ((insn->mnemonic.starts_with("frint") && insn->mnemonic.size() == 6) ||
                (insn->mnemonic.starts_with("fcvt") && insn->mnemonic.size() == 6 && std::string_view("nmapz").find(insn->mnemonic[4]) != std::string_view::npos &&
                 (insn->mnemonic[5] == 's' || insn->mnemonic[5] == 'u')))
                r.kind = Kind::RoundConvert;
            destination_of(description_text, r);
            break;
        case da::Op::SimdShift: case da::Op::SimdScalarShift:   // (FCVTZS and FCVTZU with fractional bits: the description takes the absolute value of the product)
            r.v = true;
            if (insn->mnemonic == "fcvtzs" || insn->mnemonic == "fcvtzu") r.kind = Kind::FixedConvert;
            if (a64vec::takes(insn->mnemonic, true, insn->op == da::Op::SimdScalarShift, false)) r.kind = Kind::VecNative;   // (the shifts that accumulate, the scalar SLI)
            if (insn->op == da::Op::SimdScalarShift && (insn->mnemonic == "sqshrn" || insn->mnemonic == "sqshrun")) r.fix = Fix::SourceInRd;   // (the description reads their source from Rd)
            destination_of(description_text, r);
            break;
        case da::Op::SimdThree: case da::Op::SimdByElem: case da::Op::SimdScalarByElem:   // (FMLA and FMLS are fused on the lane: the description multiplies and adds)
            r.v = true;
            if (insn->mnemonic == "fmla" || insn->mnemonic == "fmls") r.kind = Kind::Fused;
            else if (insn->mnemonic == "fadd" || insn->mnemonic == "fsub" || insn->mnemonic == "fmul" || insn->mnemonic == "fdiv" || insn->mnemonic == "fabd")
                r.nan_form = insn->op == da::Op::SimdThree ? 0 : insn->op == da::Op::SimdByElem ? 1 : 3;
            if (a64vec::takes(insn->mnemonic, false, false, insn->op != da::Op::SimdThree)) r.kind = Kind::VecNative;   // (SMAXP, SQRDMULH by element)
            if (insn->op == da::Op::SimdThree && insn->mnemonic == "srhadd") r.fix = Fix::ResultInRn;
            destination_of(description_text, r);
            break;
        case da::Op::SimdScalarThree:
            r.v = true;
            if (insn->mnemonic == "fabd") r.nan_form = 2;
            destination_of(description_text, r);
            break;
        case da::Op::SimdThreeDiff: case da::Op::SimdModImm: case da::Op::SimdExt: case da::Op::SimdPermute:
        case da::Op::SimdTable: case da::Op::SimdAcross: case da::Op::SimdCopy:
        case da::Op::SimdScalarPair: case da::Op::SimdScalarCopy:
            r.v = true;
            destination_of(description_text, r);   // (the description does not always zero what a 64-bit or scalar result leaves of the register)
            break;
        default: break;
    }
    return r;
}

const Route& route_for(u32 word, sl::Emulator& emu, bool everything) {
    thread_local std::unordered_map<u32, Route> known[2];
    const auto [slot, fresh] = known[everything ? 1 : 0].try_emplace(word);
    if (fresh) {
        std::uint8_t code[4];
        for (unsigned i = 0; i < 4; ++i) code[i] = static_cast<std::uint8_t>(word >> (8 * i));
        thread_local u64 scratch = 0x7000000000ull;   // (a fresh address for each word: the decode of an address is remembered)
        const u64 at = scratch;
        scratch += 4;
        emu.write(at, code, 4);
        slot->second = make_route(word, emu.text(at), everything);
    }
    return slot->second;
}

// ---- the emulator's registers --------------------------------------------------------------------------------------------------------------------------------------

struct Refs {
    std::array<sl::Emulator::RegRef, 31> x;
    sl::Emulator::RegRef sp, xzr, ng, zr, cy, ov, fpcr, fpsr, tpidr, tpidrro;
    std::array<sl::Emulator::RegRef, 32> q, qlo, qhi;
};

const Refs& refs_of(sl::Emulator& emu) {
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
        for (unsigned i = 0; i < 32; ++i) {
            refs.q[i] = emu.regRef("q" + std::to_string(i));
            refs.qlo[i] = refs.q[i];   // (the low half as a register of 8 bytes)
            refs.qlo[i].size = 8;
            refs.qhi[i] = refs.q[i];   // (the high half, as a register of 8 bytes)
            refs.qhi[i].offset += 8;
            refs.qhi[i].size = 8;
        }
        pcode_ops::bind_a64(emu);   // (the named vector operations of the description, by number)
        owner = &emu;
    }
    return refs;
}

void read_v(const sl::Emulator& emu, const sl::Emulator::RegRef& ref, ea::V128& v) {
    std::uint8_t b[16];
    emu.readRegWide(ref, b);
    std::memcpy(&v.lo, b, 8);
    std::memcpy(&v.hi, b + 8, 8);
}

void write_v(sl::Emulator& emu, const sl::Emulator::RegRef& ref, const ea::V128& v) {
    std::uint8_t b[16];
    std::memcpy(b, &v.lo, 8);
    std::memcpy(b + 8, &v.hi, 8);
    emu.writeRegWide(ref, b);
}

// ---- the share -----------------------------------------------------------------------------------------------------------------------------------------------------

std::mutex g_share_lock;
A64Share g_share;

}  // namespace

bool lane_stops_at(std::uint32_t word) {
    sl::Emulator& emu = emulator_named("AARCH64.sla");
    refs_of(emu);
    return route_for(word, emu, false).kind == Kind::Stop;
}

A64Share a64_share() {
    const std::lock_guard guard(g_share_lock);
    return g_share;
}

void reset_a64_share() {
    const std::lock_guard guard(g_share_lock);
    g_share = {};
}

A64End run_a64(ea::Cpu& cpu, u64 max_steps, bool description_only) {
    sl::Emulator& emu = emulator_named("AARCH64.sla");
    const Refs& R = refs_of(emu);
    ea::Memory& memory = cpu.memory();
    ea::Registers& regs = cpu.regs;

    // The data accesses of the instruction being run: the lowest address reached, the lowest one Memory did not answer (and whether it was a store), and, for an instruction that
    // stores in more than one access, what each store replaced (so a failing instruction leaves only the stores before the failing element, as the architecture's order has it).
    struct Stored {
        u64 addr, value, before;
        unsigned size;
    };
    bool failed = false, fail_is_store = false, journal_on = false;
    u64 lowest = ~u64{0}, fail_lowest = ~u64{0};
    std::vector<Stored> journal;
    emu.onRead([&](std::string_view space, u64 addr, unsigned size, u64& value) {
        if (!is_ram(space)) return false;
        lowest = std::min(lowest, addr);
        if (!memory.read(addr, size, value)) {
            failed = true;
            value = 0;
            if (addr < fail_lowest) fail_lowest = addr, fail_is_store = false;
        }
        return true;
    });
    emu.onWrite([&](std::string_view space, u64 addr, unsigned size, u64 value) {
        if (!is_ram(space)) return false;
        lowest = std::min(lowest, addr);
        u64 before = 0;
        if (journal_on) memory.read(addr, size, before);
        if (!memory.write(addr, size, value)) {
            failed = true;
            if (addr < fail_lowest) fail_lowest = addr, fail_is_store = true;
        } else if (journal_on) {
            journal.push_back({addr, value, before, size});
        }
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

    // Which copy of the registers is current: the Cpu's, the emulator's, or both. (The general registers, SP, the flags and the pc, and the vector registers, apart. The FP
    // control and status registers and the thread pointers are the Cpu's alone: only a system register move changes them.)
    bool g_cpu = true, g_emu = false, v_cpu = true, v_emu = false;
    const auto flags_to_emu = [&] {
        emu.setReg(R.ng, (regs.nzcv >> 31) & 1);
        emu.setReg(R.zr, (regs.nzcv >> 30) & 1);
        emu.setReg(R.cy, (regs.nzcv >> 29) & 1);
        emu.setReg(R.ov, (regs.nzcv >> 28) & 1);
    };
    const auto flags_from_emu = [&] {
        return static_cast<u32>((emu.reg(R.ng) & 1) << 31 | (emu.reg(R.zr) & 1) << 30 | (emu.reg(R.cy) & 1) << 29 | (emu.reg(R.ov) & 1) << 28);
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
            for (unsigned i = 0; i < 32; ++i) write_v(emu, R.q[i], regs.v[i]);
            v_emu = true;
        }
    };
    const auto ensure_cpu = [&] {
        if (!g_cpu) {
            for (unsigned i = 0; i < 31; ++i) regs.x[i] = emu.reg(R.x[i]);
            regs.sp = emu.reg(R.sp);
            regs.nzcv = flags_from_emu();
            g_cpu = true;
        }
        if (!v_cpu) {
            for (unsigned i = 0; i < 32; ++i) read_v(emu, R.q[i], regs.v[i]);
            v_cpu = true;
        }
    };
    const auto x_of = [&](unsigned n) -> u64 { return n == 31 ? 0 : emu.reg(R.x[n]); };   // (on the emulator's copy)
    const auto set_x = [&](unsigned n, u64 value) {
        if (n != 31) emu.setReg(R.x[n], value);
    };

    A64End end;
    u64 emu_steps = 0, adapter_steps = 0;
    // The abort the lane reports for the data access at `addr` (what the first interpreter said, and so does the lane).
    const auto abort_text = [&](bool store, u64 addr) {
        if (!store) return std::format("data abort: read at {:#x}", addr);
        return std::format("data abort: {} at {:#x}", memory.mapped(addr) ? "write to read-only memory" : "write", addr);
    };
    const auto hex = [](u64 v) { return std::format("{:#x}", v); };
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
        if (!memory.read_bytes(pc, 4, code)) {
            end.end = ea::End::Stopped;
            end.reason = "instruction fetch from unmapped address " + hex(pc);
            break;
        }
        const u32 word = static_cast<u32>(code[0]) | static_cast<u32>(code[1]) << 8 | static_cast<u32>(code[2]) << 16 | static_cast<u32>(code[3]) << 24;
        const Route* route = &route_for(word, emu, description_only);
        const unsigned rd = word & 31, rn = (word >> 5) & 31, rt2 = (word >> 10) & 31, rs = (word >> 16) & 31;
        std::string fault;   // a data access nothing answers: the abort the lane reports
        switch (route->kind) {
            case Kind::Nop:
                regs.pc += 4;
                ++cpu.steps;
                ++adapter_steps;
                continue;
            case Kind::Clrex:
                cpu.set_monitor(false, 0);
                regs.pc += 4;
                ++cpu.steps;
                ++adapter_steps;
                continue;
            case Kind::Stop:
                if (route->counted) ++cpu.steps;
                ++adapter_steps;
                end.end = ea::End::Stopped;
                end.reason = route->stop + " at " + hex(pc);
                break;
            case Kind::Svc: {
                ensure_cpu();
                ++cpu.steps;
                ++adapter_steps;
                ea::SvcCall call;
                call.number = (word >> 5) & 0xFFFF;
                for (unsigned i = 0; i < 8; ++i) call.args[i] = regs.x[i];
                const ea::SvcHandler& handler = cpu.svc ? cpu.svc : ea::SvcHandler(ea::horizon_svc);
                const bool go = handler(cpu, call);
                cpu.calls.push_back(call);
                g_emu = false;   // (the handler may have changed any register of the Cpu)
                v_emu = false;
                if (!go) {
                    const std::string_view name = ea::svc_name(call.number);
                    end.end = ea::End::Stopped;
                    end.reason = "svc " + (name.empty() ? hex(call.number) : std::string(name));
                    break;
                }
                regs.pc += 4;
                continue;
            }
            case Kind::Mrs: {
                const std::string_view name = da::system_register_name((word >> 5) & 0x7FFF);
                u64 v = 0;
                ensure_emu(false);
                if (name == "nzcv") v = flags_from_emu();
                else if (name == "fpcr") v = regs.fpcr;
                else if (name == "fpsr") v = regs.fpsr;
                else if (name == "tpidr_el0") v = regs.tpidr_el0;
                else if (name == "tpidrro_el0") v = regs.tpidrro_el0;
                else if (name == "cntvct_el0" || name == "cntpct_el0") {
                    cpu.counter += 1000;
                    v = cpu.counter;
                } else if (name == "cntfrq_el0") v = 19200000;
                else if (name == "dczid_el0") v = 4;
                else if (name == "ctr_el0") v = 0x8444C004;
                set_x(rd, v);
                g_cpu = false;
                regs.pc += 4;
                ++cpu.steps;
                ++adapter_steps;
                continue;
            }
            case Kind::Msr: {
                const std::string_view name = da::system_register_name((word >> 5) & 0x7FFF);
                ensure_emu(false);
                const u64 v = x_of(rd);
                if (name == "nzcv") {
                    regs.nzcv = static_cast<u32>(v) & 0xF0000000u;
                    flags_to_emu();
                } else if (name == "fpcr") {
                    regs.fpcr = static_cast<u32>(v);
                    emu.setReg(R.fpcr, regs.fpcr);
                } else if (name == "fpsr") {
                    regs.fpsr = static_cast<u32>(v);
                    emu.setReg(R.fpsr, regs.fpsr);
                } else if (name == "tpidr_el0") {
                    regs.tpidr_el0 = v;
                    emu.setReg(R.tpidr, v);
                }
                g_cpu = false;
                regs.pc += 4;
                ++cpu.steps;
                ++adapter_steps;
                continue;
            }
            case Kind::DcZva: {
                ensure_emu(false);
                static const std::uint8_t zero[64] = {};
                if (memory.write_bytes(x_of(rd) & ~u64{63}, zero, 64)) {
                    regs.pc += 4;
                    ++cpu.steps;
                    ++adapter_steps;
                    continue;
                }
                fault = abort_text(true, x_of(rd) & ~u64{63});
                break;
            }
            case Kind::Fp: {
                ensure_emu(true);
                const unsigned ra = (word >> 10) & 31;
                const unsigned group = (word >> 24) & 0x1F;   // (0x1E: the scalar floating-point data processing, 0x1F: three sources)
                (void)group;
                switch (route->op) {
                    case da::Op::Fp1Src: case da::Op::Fp2Src: case da::Op::Fp3Src:
                        write_v(emu, R.q[rd], {a64fp::scalar_result(word, emu.reg(R.q[rn]), emu.reg(R.q[rs]), emu.reg(R.q[ra])), 0});
                        break;
                    default: {   // (the conversions between the files)
                        u64 out = 0;
                        const u64 x = rn == 31 ? 0 : emu.reg(R.x[rn]);
                        const bool to_x = route->op == da::Op::FpInt ? a64fp::int_conversion(word, x, emu.reg(R.q[rn]), out) : a64fp::fixed_conversion(word, x, emu.reg(R.q[rn]), out);
                        if (to_x) set_x(rd, out);
                        else write_v(emu, R.q[rd], {out, 0});
                        break;
                    }
                }
                g_cpu = false;
                v_cpu = false;
                regs.pc += 4;
                ++cpu.steps;
                ++adapter_steps;
                continue;
            }
            case Kind::RoundConvert: {   // FRINT*, FCVT{N,M,A,P,Z}{S,U} of a vector or a scalar
                ensure_emu(true);
                ea::V128 src;
                read_v(emu, R.q[rn], src);
                std::uint8_t in[16], out[16] = {};
                std::memcpy(in, &src.lo, 8);
                std::memcpy(in + 8, &src.hi, 8);
                std::size_t bytes = 0;
                if (!a64fp::round_convert(word, route->mnemonic, route->op == da::Op::SimdScalarTwoMisc, in, out, bytes)) {
                    end.end = ea::End::Stopped;
                    end.reason = "round_convert refused " + std::string(route->mnemonic);
                    break;
                }
                ea::V128 result;
                std::memcpy(&result.lo, out, 8);
                std::memcpy(&result.hi, out + 8, 8);
                if (bytes < 8) result.lo &= (u64{1} << (8 * bytes)) - 1;
                if (bytes <= 8) result.hi = 0;
                write_v(emu, R.q[rd], result);
                v_cpu = false;
                regs.pc += 4;
                ++cpu.steps;
                ++adapter_steps;
                continue;
            }
            case Kind::FixedConvert: case Kind::Fused: case Kind::VecNative: {
                ensure_emu(true);
                ea::V128 vn, vm, vd;
                read_v(emu, R.q[rn], vn);
                read_v(emu, R.q[route->op == da::Op::SimdByElem || route->op == da::Op::SimdScalarByElem ? a64vec::element_register(word) : rs], vm);
                read_v(emu, R.q[rd], vd);
                std::uint8_t n16[16], m16[16], d16[16], out[16] = {};
                std::memcpy(n16, &vn.lo, 8);
                std::memcpy(n16 + 8, &vn.hi, 8);
                std::memcpy(m16, &vm.lo, 8);
                std::memcpy(m16 + 8, &vm.hi, 8);
                std::memcpy(d16, &vd.lo, 8);
                std::memcpy(d16 + 8, &vd.hi, 8);
                std::size_t bytes = 0;
                const bool scalar = route->op == da::Op::SimdScalarShift || route->op == da::Op::SimdScalarByElem;
                const bool ok = route->kind == Kind::FixedConvert ? a64fp::fixed_convert(word, route->mnemonic, scalar, n16, out, bytes)
                              : route->kind == Kind::VecNative    ? a64vec::run(word, route->mnemonic, n16, m16, d16, out, bytes)
                                                                  : a64fp::fused_lanes(word, route->mnemonic, route->op == da::Op::SimdThree ? 0 : scalar ? 2 : 1, n16, m16, d16, out, bytes);
                if (!ok) {
                    end.end = ea::End::Stopped;
                    end.reason = "the lane refused " + std::string(route->mnemonic);
                    break;
                }
                ea::V128 result;
                std::memcpy(&result.lo, out, 8);
                std::memcpy(&result.hi, out + 8, 8);
                if (bytes < 8) result.lo &= (u64{1} << (8 * bytes)) - 1;
                if (bytes <= 8) result.hi = 0;
                write_v(emu, R.q[rd], result);
                v_cpu = false;
                regs.pc += 4;
                ++cpu.steps;
                ++adapter_steps;
                continue;
            }
            case Kind::StorePair: {   // STXP, STLXP: both registers if the monitor still holds the address; the status register says which
                ensure_emu(false);
                const unsigned size = word >> 30, bytes = 1u << size;
                const u64 addr = rn == 31 ? emu.reg(R.sp) : emu.reg(R.x[rn]);
                const bool held = cpu.monitor_set() && cpu.monitor_address() == addr;
                bool wrote = true;
                if (held) {
                    cpu.set_monitor(false, 0);
                    wrote = memory.write(addr, bytes, x_of(rd)) && memory.write(addr + bytes, bytes, x_of(rt2));
                    if (!wrote) cpu.set_monitor(true, addr);   // (the own core finds the monitor as it was, and makes the abort)
                } else {
                    cpu.set_monitor(false, 0);
                }
                if (wrote) {
                    set_x(rs, held ? 0 : 1);
                    emu.setReg(R.xzr, 0);
                    g_cpu = false;
                    regs.pc += 4;
                    ++cpu.steps;
                    ++adapter_steps;
                    continue;
                }
                replay = true;
                break;
            }
            case Kind::CasPair: {   // CASP{A}{L}: the pair at the address against Rs and Rs+1, then Rt and Rt+1 stored if they are equal; Rs and Rs+1 take what memory held
                ensure_emu(false);
                const unsigned bytes = 4u << ((word >> 30) & 1);
                const u64 mask = bytes == 8 ? ~0ull : 0xFFFFFFFFull;
                const u64 addr = rn == 31 ? emu.reg(R.sp) : emu.reg(R.x[rn]);
                u64 old[2] = {0, 0};
                if (memory.read(addr, bytes, old[0]) && memory.read(addr + bytes, bytes, old[1])) {
                    const bool equal = (x_of(rs) & mask) == old[0] && (x_of(rs + 1) & mask) == old[1];
                    bool wrote = true;
                    if (equal) {
                        wrote = memory.write(addr, bytes, x_of(rd) & mask);
                        if (wrote && !memory.write(addr + bytes, bytes, x_of(rd + 1) & mask)) {
                            memory.write(addr, bytes, old[0]);   // (put back what the first store changed: the own core makes the abort from the start)
                            wrote = false;
                        }
                    }
                    if (wrote) {
                        set_x(rs, old[0]);
                        set_x(rs + 1, old[1]);
                        g_cpu = false;
                        regs.pc += 4;
                        ++cpu.steps;
                        ++adapter_steps;
                        continue;
                    }
                }
                replay = true;
                break;
            }
            case Kind::SignedDivide: {
                ensure_emu(false);
                const bool sf = (word >> 31) & 1;
                const auto sext = [&](u64 v) { return sf ? static_cast<std::int64_t>(v) : static_cast<std::int64_t>(static_cast<std::int32_t>(v)); };
                const std::int64_t x = sext(x_of(rn)), y = sext(x_of(rs));
                const u64 q = y == 0 ? 0 : y == -1 ? u64{0} - static_cast<u64>(x) : static_cast<u64>(x / y);
                set_x(rd, sf ? q : q & 0xFFFFFFFFull);
                g_cpu = false;
                regs.pc += 4;
                ++cpu.steps;
                ++adapter_steps;
                continue;
            }
            case Kind::SubCarry: {   // SBCS, NGCS: Rn + ~Rm + C, with the flags of the sum
                ensure_emu(false);
                const bool sf = (word >> 31) & 1;
                const u64 mask = sf ? ~0ull : 0xFFFFFFFFull;
                const u64 x = x_of(rn) & mask, y = ~x_of(rs) & mask, carry_in = emu.reg(R.cy) & 1;
                const u64 sum = (x + y + carry_in) & mask;
                const unsigned width = sf ? 64 : 32;
                const u64 partial = (x + y) & mask;
                const bool carry = sf ? (partial < x || ((partial + carry_in) & mask) < partial) : ((x + y + carry_in) >> 32) != 0;
                const bool overflow = (((x ^ sum) & (y ^ sum)) >> (width - 1)) & 1;
                set_x(rd, sum);
                emu.setReg(R.ng, (sum >> (width - 1)) & 1);
                emu.setReg(R.zr, sum == 0);
                emu.setReg(R.cy, carry);
                emu.setReg(R.ov, overflow);
                g_cpu = false;
                regs.pc += 4;
                ++cpu.steps;
                ++adapter_steps;
                continue;
            }
            case Kind::Unpredictable: {   // (an integer load or store with write-back whose register is its base, or a pair that loads one register twice: the own core's order)
                ensure_emu(false);
                const auto sx = [](u64 v, unsigned width) { return ((v & ((1ull << width) - 1)) ^ (1ull << (width - 1))) - (1ull << (width - 1)); };
                const u64 base = emu.reg(R.x[rn]);
                bool ok = true;
                if (route->op == da::Op::LdStImm) {
                    const unsigned size = (word >> 30) & 3, opc = (word >> 22) & 3, mode = (word >> 10) & 3;
                    const u64 new_base = base + sx((word >> 12) & 0x1FF, 9);
                    const u64 addr = mode == 1 ? base : new_base;
                    if (opc >= 2 && size == 3) {   // (a prefetch: no effect, and no write-back)
                        regs.pc += 4;
                        ++cpu.steps;
                        ++adapter_steps;
                        continue;
                    }
                    const unsigned bytes = 1u << size;
                    if (opc == 0) {
                        ok = memory.write(addr, bytes, x_of(rd));
                    } else {
                        u64 v = 0;
                        ok = memory.read(addr, bytes, v);
                        if (ok) {
                            const bool signed_load = opc >= 2, to64 = opc == 2;
                            const bool is64 = signed_load ? to64 : size == 3;
                            v = signed_load ? sx(v, 8 * bytes) : v;
                            set_x(rd, is64 ? v : v & 0xFFFFFFFFull);
                        }
                    }
                    if (ok) emu.setReg(R.x[rn], new_base);
                } else {
                    const unsigned opc = (word >> 30) & 3, mode = (word >> 23) & 3;
                    const bool is_load = (word >> 22) & 1;
                    const unsigned bytes = 1u << (opc == 2 ? 3 : 2);
                    const u64 imm = sx((word >> 15) & 0x7F, 7) * bytes;
                    const u64 base_after = base + imm;
                    const u64 addr = mode != 1 ? base + imm : base;
                    u64 v1 = 0, v2 = 0;
                    if (is_load) {
                        ok = memory.read(addr, bytes, v1) && memory.read(addr + bytes, bytes, v2);
                        if (ok) {
                            const auto put = [&](unsigned reg, u64 v) { set_x(reg, opc == 1 ? sx(v, 32) : opc != 0 ? v : v & 0xFFFFFFFFull); };
                            put(rd, v1);
                            put(rt2, v2);
                        }
                    } else {
                        ok = memory.write(addr, bytes, x_of(rd)) && memory.write(addr + bytes, bytes, x_of(rt2));
                    }
                    if (ok && (mode == 1 || mode == 3)) emu.setReg(R.x[rn], mode == 1 ? base_after : addr);
                }
                if (ok) {
                    emu.setReg(R.xzr, 0);
                    regs.pc += 4;
                    ++cpu.steps;
                    ++adapter_steps;
                    g_cpu = false;
                    continue;
                }
                replay = true;   // (a data access nothing answers: the own core makes the abort)
                break;
            }
            case Kind::Emu: {
                ensure_emu(route->v);
                // what a memory instruction could have written, to be put back when an access fails
                struct Saved {
                    sl::Emulator::RegRef ref;
                    bool wide;
                    u64 lo, hi;
                };
                Saved saved[16];
                unsigned count = 0;
                const auto keep_x = [&](unsigned n) {
                    if (n < 31) saved[count++] = {R.x[n], false, emu.reg(R.x[n]), 0};
                };
                const auto keep_v = [&](unsigned n) {
                    ea::V128 v;
                    read_v(emu, R.q[n % 32], v);
                    saved[count++] = {R.q[n % 32], true, v.lo, v.hi};
                };
                if (route->mem) {
                    keep_x(rd);
                    keep_x(rt2);
                    keep_x(rs);
                    if (route->op == da::Op::LdStExcl || route->op == da::Op::LdStAtomic) {
                        keep_x(rs + 1);
                        keep_x(rd + 1);
                    }
                    if (rn < 31) keep_x(rn);
                    else saved[count++] = {R.sp, false, emu.reg(R.sp), 0};
                    for (unsigned i = 0; i < route->vregs; ++i) keep_v(rd + i);
                    if (route->v_rt2) keep_v(rt2);
                }
                ea::V128 lane_n, lane_m;
                if (route->nan_form >= 0) {
                    read_v(emu, R.q[rn], lane_n);
                    read_v(emu, R.q[route->nan_form == 1 || route->nan_form == 3 ? a64vec::element_register(word) : rs], lane_m);
                }
                u64 fn = 0, fm = 0;
                if (route->nan_check) {
                    keep_v(rd);
                    fn = emu.reg(R.q[rn]);
                    fm = emu.reg(R.q[rs]);
                }
                ea::V128 rn_before;
                if (route->fix == Fix::ResultInRn) read_v(emu, R.q[rn], rn_before);
                if (route->fix == Fix::SourceInRd) emu.setReg(R.qlo[rd], emu.reg(R.qlo[rn]));
                const u64 base = route->exclusive_load ? emu.reg(rn < 31 ? R.x[rn] : R.sp) : 0;
                emu.write(pc, code, 4);
                emu.setPc(pc);
                pcode_ops::set_a64_word(word);
                failed = false;
                const sl::Emulator::Status status = emu.step();
                if (route->nan_check && status == sl::Emulator::Status::Ok && !failed && a64fp::is_nan_result((word >> 22) & 3, emu.reg(R.q[rd])))   // (a NaN result: the lane's, not the description's)
                    write_v(emu, R.q[rd], {a64fp::scalar_result(word, fn, fm, 0), 0});
                if (route->nan_form >= 0 && status == sl::Emulator::Status::Ok && !failed) {   // a NaN lane in the result: the lane's host arithmetic gives the whole result
                    ea::V128 result;
                    read_v(emu, R.q[rd], result);
                    std::uint8_t r16[16], n16[16], m16[16], out[16] = {};
                    std::memcpy(r16, &result.lo, 8);
                    std::memcpy(r16 + 8, &result.hi, 8);
                    const unsigned ew = ((word >> 22) & 1) != 0 ? 8 : 4;
                    const std::size_t width = route->nan_form >= 2 ? ew : ((word >> 30) & 1) != 0 ? 16 : 8;
                    if (a64fp::any_nan_lane(r16, width, ew)) {
                        std::memcpy(n16, &lane_n.lo, 8);
                        std::memcpy(n16 + 8, &lane_n.hi, 8);
                        std::memcpy(m16, &lane_m.lo, 8);
                        std::memcpy(m16 + 8, &lane_m.hi, 8);
                        std::size_t bytes = 0;
                        if (a64fp::host_lanes(word, route->mnemonic, route->nan_form, n16, m16, out, bytes)) {
                            ea::V128 fixed;
                            std::memcpy(&fixed.lo, out, 8);
                            std::memcpy(&fixed.hi, out + 8, 8);
                            write_v(emu, R.q[rd], fixed);
                        }
                    }
                }
                if (status == sl::Emulator::Status::Ok && !failed) {
                    if (route->fix == Fix::CasOldValue && rs != 31 && (word >> 30) != 3) emu.setReg(R.x[rs], emu.reg(R.x[rs]) & ((u64{1} << (8u << (word >> 30))) - 1));
                    if (route->fix == Fix::WSignExtend && rd != 31) emu.setReg(R.x[rd], emu.reg(R.x[rd]) & 0xFFFFFFFFull);

                    if (route->fix == Fix::ResultInRn && rd != rn) {
                        ea::V128 result;
                        read_v(emu, R.q[rn], result);
                        write_v(emu, R.q[rd], result);
                        write_v(emu, R.q[rn], rn_before);
                    }
                    if (route->dest_bytes < 16)
                        for (unsigned i = 0; i < route->dest_regs; ++i) {
                            const unsigned q = (rd + i) % 32;
                            if (route->dest_bytes < 8) emu.setReg(R.qlo[q], emu.reg(R.qlo[q]) & ((u64{1} << (8 * route->dest_bytes)) - 1));
                            emu.setReg(R.qhi[q], 0);
                        }
                    emu.setReg(R.xzr, 0);
                    regs.pc = emu.pc();
                    ++cpu.steps;
                    ++emu_steps;
                    g_cpu = false;
                    if (route->v) v_cpu = false;
                    if (route->exclusive_load) cpu.set_monitor(true, base);
                    continue;
                }
                for (unsigned i = 0; i < count; ++i) {
                    if (saved[i].wide) write_v(emu, saved[i].ref, {saved[i].lo, saved[i].hi});
                    else emu.setReg(saved[i].ref, saved[i].lo);
                }
                emu.setReg(R.xzr, 0);
                replay = true;
                break;
            }
        }
        if (!replay) break;   // (a stop)
        if (description_only) {   // (a test of the description and the operations behind it: nothing is handed to the own core)
            end.end = ea::End::Stopped;
            end.reason = "the description does not run it";
            break;
        }
        ensure_cpu();
        ++own_steps;
        ++own_counts[std::string(route->mnemonic)];
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
        g_share.adapter += adapter_steps;
        for (const auto& [name, n] : own_counts) g_share.own_core_by_mnemonic[name] += n;
        for (const auto& [name, n] : emu.unimplementedOps()) g_share.unimplemented[name] = std::max(g_share.unimplemented[name], n);
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
