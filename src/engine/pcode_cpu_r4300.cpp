// engine/pcode_cpu_r4300.cpp -- the Nintendo 64 lane's R4300i on the one p-code emulator: emulate::r4300::run's call, on languages/mips64be.sla.
//
// The integer machine, the loads and stores (every access goes through the Bus at the physical address KSEG0/KSEG1 give), the branches (branch likely
// included: the description skips the slot), and the faults (alignment, an unmapped or TLB-mapped address, overflow traps, trap instructions, break,
// syscall, a word the R4300 has no instruction for) are carried. Coprocessor 0 and the FPU's control and compare are NOT yet: those instructions end the
// run as "not run", which the own core does not.
#include <algorithm>
#include <array>
#include <bit>
#include <cmath>
#include <cstring>
#include <string>
#include <vector>

#include "decompile/engine/pcode_emulator.hh"
#include "decompile/mips.hpp"
#include "engine/pcode_cpu.hpp"
#include "engine/pcode_cpu_internal.hpp"

namespace cb::engine::pcode_cpu {

namespace rc = cb::emulate::r4300;
namespace sl = cb::decompile::sleigh;

namespace {

constexpr std::uint32_t kBlock = 0x1000;
constexpr std::uint64_t kViews[2] = {0xFFFFFFFF80000000ull, 0xFFFFFFFFA0000000ull};
constexpr const char* kNames[32] = {"zero", "at", "v0", "v1", "a0", "a1", "a2", "a3", "t0", "t1", "t2", "t3", "t4", "t5", "t6", "t7",
                                    "s0",   "s1", "s2", "s3", "s4", "s5", "s6", "s7", "t8", "t9", "k0", "k1", "gp", "sp", "s8", "ra"};

struct Mirror {
    std::vector<std::uint8_t> seen;
};

Mirror& mirror() {
    thread_local Mirror m;
    return m;
}

std::int32_t sext16(std::uint32_t w) { return static_cast<std::int16_t>(w & 0xFFFF); }
std::uint64_t sx32(std::uint32_t v) { return static_cast<std::uint64_t>(static_cast<std::int64_t>(static_cast<std::int32_t>(v))); }

bool is_branch(std::uint32_t w) {
    const unsigned op = w >> 26;
    if (op == 0) return (w & 0x3F) == 0x08 || (w & 0x3F) == 0x09;
    return op == 0x01 || (op >= 0x02 && op <= 0x07) || (op >= 0x14 && op <= 0x17) || (op == 0x11 && ((w >> 21) & 31) == 8);
}

// A branch read on its own, for a branch that sits in another's delay slot: the description takes a branch and its slot as one step, and cannot say what such a pair does.
struct Branch {
    bool taken = false;
    std::uint32_t target = 0;
    bool likely = false;
    int link = -1;   // the register given the return address
};

std::optional<Branch> read_branch(std::uint32_t w, std::uint32_t at, std::uint64_t rs_value, std::uint64_t rt_value, std::uint32_t fcsr) {
    const unsigned op = w >> 26, rt = (w >> 16) & 31;
    const std::int64_t a = static_cast<std::int64_t>(rs_value);
    const std::uint32_t relative = at + 4 + (static_cast<std::uint32_t>(static_cast<std::int32_t>(static_cast<std::int16_t>(w & 0xFFFF))) << 2);
    const std::uint32_t absolute = ((at + 4) & 0xF0000000u) | ((w & 0x03FFFFFFu) << 2);
    switch (op) {
        case 0x00:
            if ((w & 0x3F) == 0x08) return Branch{true, static_cast<std::uint32_t>(rs_value), false, -1};
            if ((w & 0x3F) == 0x09) return Branch{true, static_cast<std::uint32_t>(rs_value), false, static_cast<int>((w >> 11) & 31)};
            return std::nullopt;
        case 0x01:
            switch (rt) {
                case 0: case 2: return Branch{a < 0, relative, rt == 2, -1};
                case 1: case 3: return Branch{a >= 0, relative, rt == 3, -1};
                case 16: case 18: return Branch{a < 0, relative, rt == 18, 31};
                case 17: case 19: return Branch{a >= 0, relative, rt == 19, 31};
                default: return std::nullopt;   // (the trap instructions of this opcode)
            }
        case 0x02: return Branch{true, absolute, false, -1};
        case 0x03: return Branch{true, absolute, false, 31};
        case 0x04: case 0x14: return Branch{rs_value == rt_value, relative, op == 0x14, -1};
        case 0x05: case 0x15: return Branch{rs_value != rt_value, relative, op == 0x15, -1};
        case 0x06: case 0x16: return Branch{a <= 0, relative, op == 0x16, -1};
        case 0x07: case 0x17: return Branch{a > 0, relative, op == 0x17, -1};
        case 0x11: {
            if (((w >> 21) & 31) != 8) return std::nullopt;
            const bool condition = (fcsr & (1u << 23)) != 0;
            return Branch{(rt & 1) != 0 ? condition : !condition, relative, (rt & 2) != 0, -1};
        }
        default: return std::nullopt;
    }
}

// The size of the memory access an opcode makes (0: none) and whether it must be aligned to it.
unsigned access_size(unsigned op) {
    switch (op) {
        case 0x20: case 0x24: case 0x28: return 1;
        case 0x21: case 0x25: case 0x29: return 2;
        case 0x23: case 0x27: case 0x2B: case 0x30: case 0x31: case 0x38: case 0x39: return 4;
        case 0x37: case 0x3F: case 0x34: case 0x35: case 0x3C: case 0x3D: return 8;
        case 0x22: case 0x26: case 0x2A: case 0x2E: case 0x1A: case 0x1B: case 0x2C: case 0x2D: return 1;   // the unaligned ones: any address
        default: return 0;
    }
}

}  // namespace

rc::RunResult run(rc::Bus& bus, std::uint32_t entry, const rc::RunOptions& options) {
    sl::Emulator& emu = emulator_named("mips64be.sla");
    const std::size_t log_start = bus.log().size();
    const std::span<std::uint8_t> memory = bus.rdram();
    Mirror& mir = mirror();
    if (mir.seen.size() != memory.size()) {
        emu.clearMemory();
        mir.seen.assign(memory.size(), 0);
    }
    for (std::size_t at = 0; at < memory.size(); at += kBlock) {
        const std::size_t n = std::min<std::size_t>(kBlock, memory.size() - at);
        if (std::memcmp(mir.seen.data() + at, memory.data() + at, n) == 0) continue;
        std::memcpy(mir.seen.data() + at, memory.data() + at, n);
        for (const std::uint64_t base : kViews) emu.write(base + at, memory.data() + at, n);
    }
    emu.clearStops();

    std::optional<rc::Fault::Kind> raised;
    std::string raised_text;
    const auto physical = [](std::uint64_t address) { return static_cast<std::uint32_t>(address) & 0x1FFFFFFFu; };
    emu.onRead([&](std::string_view space, std::uint64_t addr, unsigned size, std::uint64_t& value) {
        if (!is_ram(space)) return false;
        value = bus.read(physical(addr), size);
        return true;
    });
    emu.onWrite([&](std::string_view space, std::uint64_t addr, unsigned size, std::uint64_t value) {
        if (!is_ram(space)) return false;
        const std::uint32_t phys = physical(addr);
        bus.write(phys, size, value);
        if (phys + size <= memory.size()) {   // (the program may run what it wrote)
            std::memcpy(mir.seen.data() + phys, memory.data() + phys, size);
            for (const std::uint64_t base : kViews) emu.write(base + phys, memory.data() + phys, size);
        }
        return true;
    });
    std::uint32_t current_word = 0;
    emu.onUserOp([&](sl::Emulator&, const std::string& name, const std::vector<std::uint64_t>&, std::uint64_t&) {
        std::string lower = name;
        std::transform(lower.begin(), lower.end(), lower.begin(), [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
        if (lower == "setisamode") return true;
        if (lower.find("syscall") != std::string::npos) raised = rc::Fault::Kind::Syscall;
        else if (lower.find("break") != std::string::npos) raised = rc::Fault::Kind::Break;
        else if (lower == "trap") raised = (current_word >> 26) == 0 && (current_word & 0x3F) == 0x0D ? rc::Fault::Kind::Break : rc::Fault::Kind::Trap;
        else {
            raised = rc::Fault::Kind::NotRun;
            raised_text = name;
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

    const auto& refs = reg_refs(emu, kNames);
    const auto reg = [&](unsigned i) { return emu.reg(refs[i]); };
    const auto set_reg = [&](unsigned i, std::uint64_t v) {
        if (i != 0) emu.setReg(refs[i], v);
    };
    for (unsigned i = 1; i < 32; ++i) emu.setReg(refs[i], 0);
    for (unsigned i = 0; i < 4; ++i) set_reg(4 + i, sx32(options.a[i]));
    for (unsigned i = 0; i < 8; ++i) set_reg(16 + i, sx32(options.s[i]));
    for (unsigned i = 0; i < 8; ++i) set_reg(8 + i, sx32(options.t[i]));
    set_reg(28, sx32(options.gp));
    set_reg(29, sx32(options.sp));
    set_reg(31, sx32(rc::kReturnAddress));
    emu.setReg("hi", 0);
    emu.setReg("lo", 0);
    for (unsigned i = 0; i < 32; ++i) emu.setReg("f" + std::to_string(i), 0);
    emu.setReg("fcsr", 0);
    std::array<std::uint64_t, 32> cop0{};   // as the boot code leaves it: COP0, COP1 and FR on
    cop0[12] = 0x34000000;
    cop0[15] = 0x0B22;
    cop0[16] = 0x7006E463;
    cop0[1] = 31;
    emu.setPc(sx32(entry));

    rc::RunResult out;
    out.end = rc::End::Limit;
    std::uint64_t steps = 0;
    const auto stop = [&](rc::Fault::Kind kind, std::uint32_t pc, std::uint32_t address) {
        out.end = rc::End::Stopped;
        out.fault = rc::Fault{kind, pc, address, {}, 0};
    };
    const auto read_word = [&](std::uint32_t phys) {
        return (static_cast<std::uint32_t>(memory[phys]) << 24) | (static_cast<std::uint32_t>(memory[phys + 1]) << 16) | (static_cast<std::uint32_t>(memory[phys + 2]) << 8) |
               static_cast<std::uint32_t>(memory[phys + 3]);
    };
    // A branch in a delay slot, one instruction at a time as the own core runs it: `follow` is where the instruction now running hands over to, `then` the one after it.
    std::optional<std::uint32_t> follow, then;
    while (steps < options.max_steps) {
        if (follow) {
            emu.setPc(sx32(*follow));
            follow.reset();
        }
        std::swap(follow, then);
        const std::uint32_t pc = static_cast<std::uint32_t>(emu.pc());
        if (pc == rc::kReturnAddress) {
            out.end = rc::End::Return;
            break;
        }
        if ((pc & 3) != 0) {
            stop(rc::Fault::Kind::AddressError, pc, pc);
            break;
        }
        const std::uint32_t phys = pc & 0x1FFFFFFFu;
        if ((pc >> 29) != 4 && (pc >> 29) != 5) {   // (the TLB: not carried)
            stop(rc::Fault::Kind::TlbMiss, pc, pc);
            break;
        }
        if (phys + 8 > memory.size()) {
            stop(rc::Fault::Kind::BusError, pc, pc);
            break;
        }
        const std::uint32_t word = read_word(phys);
        const std::uint32_t slot_word = is_branch(word) && phys + 8 <= memory.size() ? read_word(phys + 4) : 0;
        const auto check = [&](std::uint32_t w, std::uint32_t at, bool in_slot) -> std::optional<rc::Fault> {
            const std::optional<decompile::mips::Insn> insn = decompile::mips::decode_word(w, at, decompile::mips::Variant::R4300);
            if (!insn) return rc::Fault{rc::Fault::Kind::NoInstruction, at, 0, {}, 0};
            const unsigned op = w >> 26, rs = (w >> 21) & 31, rt = (w >> 16) & 31;
            const std::uint32_t simm = static_cast<std::uint32_t>(sext16(w));
            if (const unsigned size = access_size(op); size != 0) {
                const std::uint32_t address = static_cast<std::uint32_t>(reg(rs)) + simm;
                const bool unaligned_op = op == 0x22 || op == 0x26 || op == 0x2A || op == 0x2E || op == 0x1A || op == 0x1B || op == 0x2C || op == 0x2D;
                if (!unaligned_op && (address & (size - 1)) != 0) return rc::Fault{rc::Fault::Kind::AddressError, at, address, {}, 0};
                if ((address >> 29) != 4 && (address >> 29) != 5) return rc::Fault{rc::Fault::Kind::TlbMiss, at, address, {}, 0};
                if (!bus.mapped(address & 0x1FFFFFFFu)) return rc::Fault{rc::Fault::Kind::BusError, at, address, {}, 0};
            }
            if ((op == 0x10 && (in_slot || (rs != 0 && rs != 1 && rs != 4 && rs != 5))) || op == 0x2F)
                return rc::Fault{rc::Fault::Kind::NotRun, at, 0, insn->mnemonic, 0};   // coprocessor 0 and CACHE: not carried yet (the FPU is the description's)
            const bool add32 = op == 0x08 || (op == 0 && ((w & 0x3F) == 0x20 || (w & 0x3F) == 0x22));
            const bool add64 = op == 0x18 || (op == 0 && ((w & 0x3F) == 0x2C || (w & 0x3F) == 0x2E));
            if (add32 || add64) {
                const bool is_sub = op == 0 && ((w & 0x3F) == 0x22 || (w & 0x3F) == 0x2E);
                const std::uint64_t a = reg(rs);
                const std::uint64_t b = (op == 0x08 || op == 0x18) ? static_cast<std::uint64_t>(static_cast<std::int64_t>(static_cast<std::int32_t>(simm))) : reg(rt);
                if (add32) {
                    const std::int64_t s = is_sub ? static_cast<std::int64_t>(static_cast<std::int32_t>(a)) - static_cast<std::int32_t>(b)
                                                  : static_cast<std::int64_t>(static_cast<std::int32_t>(a)) + static_cast<std::int32_t>(b);
                    if (s != static_cast<std::int32_t>(s)) return rc::Fault{rc::Fault::Kind::Overflow, at, 0, {}, 0};
                } else {
                    const std::int64_t x = static_cast<std::int64_t>(a), y = static_cast<std::int64_t>(b);
                    const std::uint64_t r = is_sub ? a - b : a + b;   // (signed overflow: the operands' signs say it, the result's sign disagrees)
                    const bool overflow = is_sub ? ((x < 0) != (y < 0) && (static_cast<std::int64_t>(r) < 0) != (x < 0)) : ((x < 0) == (y < 0) && (static_cast<std::int64_t>(r) < 0) != (x < 0));
                    if (overflow) return rc::Fault{rc::Fault::Kind::Overflow, at, 0, {}, 0};
                }
            }
            return std::nullopt;
        };
        if (const std::optional<rc::Fault> fault = check(word, pc, false)) {
            out.end = rc::End::Stopped;
            out.fault = *fault;
            break;
        }
        if ((word >> 26) == 0x10 && !is_branch(word)) {   // coprocessor 0 (the registers a program moves to and from), made here as the own core makes them
            const unsigned rs = (word >> 21) & 31, rt = (word >> 16) & 31, rd = (word >> 11) & 31;
            const auto read = [&](unsigned n) -> std::uint64_t {
                if (n == 1) {   // Random counts down through the TLB's unwired entries
                    const unsigned wired = static_cast<unsigned>(cop0[6] & 0x3F);
                    return wired >= 32 ? 31 : 31 - steps % (32 - wired);
                }
                if (n == 9) return (cop0[9] + steps + 1) & 0xFFFFFFFFu;   // Count: one per instruction
                return cop0[n];
            };
            if (rs == 0) set_reg(rt, sx32(static_cast<std::uint32_t>(read(rd))));
            else if (rs == 1) set_reg(rt, read(rd));
            else {
                const bool wide = rs == 5;
                const std::uint64_t v = wide ? reg(rt) : sx32(static_cast<std::uint32_t>(reg(rt)));
                switch (rd) {
                    case 0: cop0[0] = v & 0x8000003F; break;
                    case 1: case 8: case 15: break;
                    case 2: case 3: cop0[rd] = v & 0x3FFFFFFF; break;
                    case 5: cop0[5] = v & 0x01FFE000; break;
                    case 6: cop0[6] = v & 0x3F; break;
                    case 9: cop0[9] = (v - steps - 1) & 0xFFFFFFFFu; break;
                    case 10: cop0[10] = v & ~std::uint64_t{0x1F00}; break;
                    case 11: cop0[11] = v & 0xFFFFFFFFu; cop0[13] &= ~std::uint64_t{0x8000}; break;
                    case 13: cop0[13] = (cop0[13] & ~std::uint64_t{0x300}) | (v & 0x300); break;
                    case 16: cop0[16] = (cop0[16] & ~std::uint64_t{0x0F00800F}) | (v & 0x0F00800F); break;
                    default: cop0[rd] = v; break;
                }
            }
            emu.setPc(sx32(pc + 4));
            ++steps;
            continue;
        }
        // The FPU's control register moves and the conversions that follow its rounding mode are made here: the description keeps the whole control word and
        // rounds to nearest, the R4300 keeps 0x0183FFFF of it and converts as the word's rounding mode says.
        if ((word >> 26) == 0x11 && !is_branch(word)) {
            const unsigned rs = (word >> 21) & 31, rt = (word >> 16) & 31, fs = (word >> 11) & 31, fd = (word >> 6) & 31, function = word & 63;
            if (rs == 2 || rs == 6) {
                if (rs == 2) set_reg(rt, sx32(fs == 0 ? 0x0A00u : fs == 31 ? static_cast<std::uint32_t>(emu.reg("fcsr")) : 0u));
                else if (fs == 31) emu.setReg("fcsr", static_cast<std::uint32_t>(reg(rt)) & 0x0183FFFFu);
                emu.setPc(sx32(pc + 4));
                ++steps;
                continue;
            }
            if ((rs == 16 || rs == 17) && ((function >= 8 && function <= 15) || function == 36 || function == 37)) {
                const std::uint64_t bits = emu.reg("f" + std::to_string(fs));
                const double v = rs == 16 ? static_cast<double>(std::bit_cast<float>(static_cast<std::uint32_t>(bits))) : std::bit_cast<double>(bits);
                // (ROUND TRUNC CEIL FLOOR name their mode, to a long for 8-11 and to a word for 12-15 -- the description rounds ties away from zero, the R4300 to even)
                const unsigned mode = function >= 8 && function <= 15 ? ((function & 3) == 0 ? 0u : (function & 3) == 1 ? 1u : (function & 3) == 2 ? 2u : 3u)
                                                                       : static_cast<unsigned>(emu.reg("fcsr")) & 3;
                const bool word_result = function >= 12 && function <= 15;
                const unsigned convert = function >= 8 && function <= 15 ? (word_result ? 36u : 37u) : function;
                const double whole = std::isnan(v) ? v : mode == 0 ? std::nearbyint(v) : mode == 1 ? std::trunc(v) : mode == 2 ? std::ceil(v) : std::floor(v);
                const bool out_of_range = std::isnan(v) || (convert == 36 ? (whole < -2147483648.0 || whole > 2147483647.0)
                                                                           : (whole < -9223372036854775808.0 || whole >= 9223372036854775808.0));
                if (out_of_range) {
                    out.end = rc::End::Stopped;
                    out.fault = rc::Fault{rc::Fault::Kind::NotRun, pc, 0, "cvt", 0};
                    break;
                }
                const std::string name = "f" + std::to_string(fd);
                if (convert == 36) emu.setReg(name, (emu.reg(name) & ~std::uint64_t{0xFFFFFFFFu}) | static_cast<std::uint32_t>(static_cast<std::int32_t>(whole)));
                else emu.setReg(name, static_cast<std::uint64_t>(static_cast<std::int64_t>(whole)));
                emu.setPc(sx32(pc + 4));
                ++steps;
                continue;
            }
        }
        if (is_branch(word) && (follow || is_branch(slot_word))) {
            if (const std::optional<Branch> branch = read_branch(word, pc, reg((word >> 21) & 31), reg((word >> 16) & 31), static_cast<std::uint32_t>(emu.reg("fcsr")))) {
                const std::uint32_t sequential = follow ? *follow : pc + 4;   // what runs next if this branch changes nothing
                follow.reset();
                if (branch->link > 0) set_reg(static_cast<unsigned>(branch->link), sx32(pc + 8));
                ++steps;
                if (branch->taken) {
                    emu.setPc(sx32(sequential));
                    then = branch->target;
                } else if (branch->likely) {
                    emu.setPc(sx32(sequential + 4));   // (the slot is skipped)
                } else {
                    emu.setPc(sx32(sequential));
                }
                continue;
            }
        }
        const bool half = is_branch(word) && steps + 1 >= options.max_steps;
        int link_reg = -1;
        if (is_branch(word)) {
            const unsigned bop = word >> 26;
            if (bop == 0x03) link_reg = 31;
            else if (bop == 0 && (word & 0x3F) == 0x09) link_reg = static_cast<int>((word >> 11) & 31);
            else if (bop == 0x01 && ((word >> 16) & 31) >= 16) link_reg = 31;
            if (link_reg == 0) link_reg = -1;
        }
        if (is_branch(word) && !half) {
            const std::uint64_t before = link_reg > 0 ? reg(static_cast<unsigned>(link_reg)) : 0;
            if (link_reg > 0) set_reg(static_cast<unsigned>(link_reg), sx32(pc + 8));
            const std::optional<rc::Fault> fault = check(slot_word, pc + 4, true);
            if (link_reg > 0) set_reg(static_cast<unsigned>(link_reg), before);
            if (fault) {
                if (link_reg > 0) set_reg(static_cast<unsigned>(link_reg), sx32(pc + 8));
                emu.setPc(sx32(pc + 4));
                out.end = rc::End::Stopped;
                out.fault = *fault;
                ++steps;
                break;
            }
        }
        std::uint8_t saved[4] = {};
        if (half) {   // the step limit falls between the branch and its slot: the branch alone runs
            const std::uint8_t zero4[4] = {};
            for (unsigned i = 0; i < 4; ++i) saved[i] = memory[phys + 4 + i];
            for (const std::uint64_t base : kViews) emu.write(base + phys + 4, zero4, 4);
        }
        raised.reset();
        current_word = word;
        const sl::Emulator::Status status = emu.step();
        if (half)
            for (const std::uint64_t base : kViews) emu.write(base + phys + 4, saved, 4);
        if (status != sl::Emulator::Status::Ok) {
            stop(rc::Fault::Kind::NoInstruction, pc, 0);
            break;
        }
        if (raised) {
            out.end = rc::End::Stopped;
            const bool pair = is_branch(word) && !half;
            out.fault = rc::Fault{*raised, pair ? pc + 4 : pc, 0, *raised == rc::Fault::Kind::NotRun ? raised_text : std::string{}, ((pair ? slot_word : word) >> 6) & 0xFFFFFu};
            if (pair) ++steps;
            if (*raised == rc::Fault::Kind::Syscall || *raised == rc::Fault::Kind::Break) ++steps;
            break;
        }
        emu.setReg(refs[0], 0);
        if (is_branch(word) && !half) {
            const std::optional<decompile::mips::Insn> insn = decompile::mips::decode_word(word, pc, decompile::mips::Variant::R4300);
            const bool skipped = insn && insn->likely && static_cast<std::uint32_t>(emu.pc()) == pc + 8;   // (a branch likely not taken: its slot did not run)
            steps += skipped ? 1 : 2;
        } else {
            ++steps;
        }
    }
    for (unsigned i = 0; i < 32; ++i) out.r[i] = reg(i);
    for (unsigned i = 0; i < 32; ++i) out.fpr[i] = emu.reg("f" + std::to_string(i));
    out.hi = emu.reg("hi");
    out.lo = emu.reg("lo");
    out.fcr31 = static_cast<std::uint32_t>(emu.reg("fcsr"));
    out.steps = steps;
    out.io.assign(bus.log().begin() + static_cast<std::ptrdiff_t>(log_start), bus.log().end());
    return out;
}

rc::RunResult call(rc::Bus& bus, std::uint32_t entry, const rc::RunOptions& options) { return enabled() ? pcode_cpu::run(bus, entry, options) : rc::run(bus, entry, options); }

}  // namespace cb::engine::pcode_cpu
