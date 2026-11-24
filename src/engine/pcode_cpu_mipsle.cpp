// cb-unity: alone (file-local names that other files in a batch also use)
// engine/pcode_cpu_mipsle.cpp -- the run the Allegrex and the Emotion Engine share (pcode_cpu_mipsle.hpp).
//
// The registers live in one place at a time: the emulator while the description runs instructions, the own core's Cpu while it does. A switch copies the 32 registers and
// HI/LO across (the rest of the own core's state, the 128-bit halves, the second multiplier, the FPU and coprocessors 0 and 2, only ever lives in the Cpu). Before an
// instruction goes to the description the adapter looks at it and at the registers it reads: the instruction must be one the description carries the same way, and must
// not fault; otherwise it is the own core's, which also gives the fault. An access the machine does not answer is found out by the hook, after the fact: the registers
// the instruction could have written are put back and the own core runs it again.
#include <array>
#include <cstring>
#include <string>

#include "decompile/engine/pcode_emulator.hh"
#include "engine/pcode_cpu_internal.hpp"
#include "engine/pcode_cpu_mipsle.hpp"

namespace cb::engine::pcode_cpu {

namespace ml = cb::emulate::mipsle;
namespace sl = cb::decompile::sleigh;

namespace {

using u32 = std::uint32_t;
using u64 = std::uint64_t;
using i64 = std::int64_t;

constexpr const char* kNames[32] = {"zero", "at", "v0", "v1", "a0", "a1", "a2", "a3", "t0", "t1", "t2", "t3", "t4", "t5", "t6", "t7",
                                    "s0",   "s1", "s2", "s3", "s4", "s5", "s6", "s7", "t8", "t9", "k0", "k1", "gp", "sp", "s8", "ra"};

u64 sx32(u32 v) { return static_cast<u64>(static_cast<i64>(static_cast<std::int32_t>(v))); }
i64 simm16(u32 w) { return static_cast<std::int16_t>(w & 0xFFFF); }

// What the adapter does with an instruction.
enum class Kind : std::uint8_t {
    Own,       // the own core
    Plain,     // the description: nothing can go wrong
    Add32,     // ADD, SUB, ADDI: the description, unless it overflows
    Add64,     // DADD, DSUB, DADDI
    Divide,    // DIV, DIVU: the description, unless the divisor is zero (or the quotient overflows)
    VarShift64,   // DSLLV, DSRLV, DSRAV: the description shifts by all of rs, the processor by its low six bits: the description, when rs is below 64
    Load,
    Store,
    Branch,    // a branch or jump with a delay slot, to a place the instruction gives
    JumpReg,   // JR, JALR: to a register's value
};

struct Info {
    Kind kind = Kind::Own;
    unsigned size = 0;      // a load or store: its bytes
    bool likely = false;    // a branch likely
    unsigned link = 0;      // the register a branch gives the return address (0: none)
};

Info classify(u32 w, bool ee) {
    const unsigned op = w >> 26, rs = (w >> 21) & 31, rt = (w >> 16) & 31, rd = (w >> 11) & 31, sh = (w >> 6) & 31, fn = w & 63;
    const auto kind = [](Kind k) { return Info{k, 0, false, 0}; };
    const auto when = [&](bool ok, Kind k) { return ok ? kind(k) : kind(Kind::Own); };
    switch (op) {
        case 0x00:
            switch (fn) {
                case 0x00: case 0x02: case 0x03: return when(rs == 0, Kind::Plain);   // (SRL with rs of 1 is the Allegrex's ROTR)
                case 0x04: case 0x06: case 0x07: return when(sh == 0, Kind::Plain);
                case 0x08: return when(rt == 0 && rd == 0 && sh == 0, Kind::JumpReg);
                case 0x09: {
                    Info i = when(rt == 0 && sh == 0, Kind::JumpReg);
                    i.link = rd;
                    return i;
                }
                case 0x0A: case 0x0B: return when(sh == 0, Kind::Plain);
                case 0x10: case 0x12: return when(rs == 0 && rt == 0 && sh == 0, Kind::Plain);
                case 0x11: case 0x13: return when(rt == 0 && rd == 0 && sh == 0, Kind::Plain);
                case 0x18: case 0x19: return when(rd == 0 && sh == 0, Kind::Plain);   // (the R5900's MULT with a destination is the own core's)
                case 0x1A: case 0x1B: return when(rd == 0 && sh == 0, Kind::Divide);
                case 0x20: case 0x22: return when(sh == 0, Kind::Add32);
                case 0x21: case 0x23: case 0x24: case 0x25: case 0x26: case 0x27: case 0x2A: case 0x2B: return when(sh == 0, Kind::Plain);
                case 0x14: case 0x16: case 0x17: return when(ee && sh == 0, Kind::VarShift64);
                case 0x2C: case 0x2E: return when(ee && sh == 0, Kind::Add64);
                case 0x2D: case 0x2F: return when(ee && sh == 0, Kind::Plain);
                case 0x38: case 0x3A: case 0x3B: case 0x3C: case 0x3E: case 0x3F: return when(ee && rs == 0, Kind::Plain);
                default: return kind(Kind::Own);
            }
        case 0x01: {
            Info i = kind(Kind::Branch);
            switch (rt) {
                case 0x00: case 0x01: return i;
                case 0x02: case 0x03: i.likely = true; return i;
                case 0x10: case 0x11: i.link = 31; return i;
                case 0x12: case 0x13: i.likely = true; i.link = 31; return i;
                default: return kind(Kind::Own);
            }
        }
        case 0x02: return kind(Kind::Branch);
        case 0x03: {
            Info i = kind(Kind::Branch);
            i.link = 31;
            return i;
        }
        case 0x04: case 0x05: return kind(Kind::Branch);
        case 0x06: case 0x07: return when(rt == 0, Kind::Branch);
        case 0x14: case 0x15: case 0x16: case 0x17: {
            Info i = (op >= 0x16 && rt != 0) ? kind(Kind::Own) : kind(Kind::Branch);
            i.likely = true;
            return i;
        }
        case 0x08: return kind(Kind::Add32);
        case 0x09: case 0x0A: case 0x0B: case 0x0C: case 0x0D: case 0x0E: case 0x0F: return kind(Kind::Plain);
        case 0x18: return when(ee, Kind::Add64);
        case 0x19: return when(ee, Kind::Plain);
        case 0x20: case 0x24: return Info{Kind::Load, 1, false, 0};
        case 0x21: case 0x25: return Info{Kind::Load, 2, false, 0};
        case 0x23: return Info{Kind::Load, 4, false, 0};
        case 0x27: return ee ? Info{Kind::Load, 4, false, 0} : kind(Kind::Own);
        case 0x37: return ee ? Info{Kind::Load, 8, false, 0} : kind(Kind::Own);
        case 0x28: return Info{Kind::Store, 1, false, 0};
        case 0x29: return Info{Kind::Store, 2, false, 0};
        case 0x2B: return Info{Kind::Store, 4, false, 0};
        case 0x3F: return ee ? Info{Kind::Store, 8, false, 0} : kind(Kind::Own);
        default: return kind(Kind::Own);
    }
}

// The register an instruction writes (-1: none; a branch's link is not counted).
int destination(u32 w, const Info& info) {
    const unsigned op = w >> 26, rt = (w >> 16) & 31, rd = (w >> 11) & 31, fn = w & 63;
    if (info.kind == Kind::Branch || info.kind == Kind::Store) return -1;
    if (op == 0) {
        if (info.kind == Kind::JumpReg || fn == 0x11 || fn == 0x13 || (fn >= 0x18 && fn <= 0x1B)) return -1;
        return static_cast<int>(rd);
    }
    return static_cast<int>(rt);
}

}  // namespace

MipsLeEnd run_mipsle(ml::Cpu& cpu, const MipsLeMachine& machine, u32 return_address, u64 max_steps) {
    const bool ee = cpu.model() == ml::Model::R5900;
    sl::Emulator& emu = emulator_named(ee ? "mips64le.sla" : "mips32le.sla");
    ml::Port& port = cpu.port();
    bool failed = false;   // a data access the machine did not answer
    emu.onRead([&](std::string_view space, u64 addr, unsigned size, u64& value) {
        if (!is_ram(space)) return false;
        if (!port.read(static_cast<u32>(addr), size, value)) {
            failed = true;
            value = 0;
        }
        return true;
    });
    emu.onWrite([&](std::string_view space, u64 addr, unsigned size, u64 value) {
        if (!is_ram(space)) return false;
        if (!port.write(static_cast<u32>(addr), size, value)) failed = true;
        return true;
    });
    emu.onUserOp([](sl::Emulator&, const std::string&, const std::vector<u64>&, u64&) { return true; });   // (the ISA mode: there is one)
    struct Unhook {
        sl::Emulator& e;
        ~Unhook() {
            e.onRead({});
            e.onWrite({});
            e.onUserOp({});
        }
    } unhook{emu};

    const auto& refs = reg_refs(emu, kNames);
    // HI and LO: the R5900's are two registers, the 32-bit description keeps them as one 64-bit accumulator
    sl::Emulator::RegRef acc{}, hi_ref{}, lo_ref{};
    if (ee) {
        hi_ref = emu.regRef("hi");
        lo_ref = emu.regRef("lo");
    } else {
        acc = emu.regRef("ac0");
    }
    const auto gr = [&](unsigned i) { return ee ? emu.reg(refs[i]) : sx32(static_cast<u32>(emu.reg(refs[i]))); };
    const auto to_emu = [&] {
        for (unsigned i = 1; i < 32; ++i) emu.setReg(refs[i], cpu.r[i]);
        emu.setReg(refs[0], 0);
        if (ee) {
            emu.setReg(hi_ref, cpu.hi[0]);
            emu.setReg(lo_ref, cpu.lo[0]);
        } else {
            emu.setReg(acc, static_cast<u64>(static_cast<u32>(cpu.hi[0])) << 32 | static_cast<u32>(cpu.lo[0]));
        }
    };
    const auto from_emu = [&] {
        for (unsigned i = 1; i < 32; ++i) cpu.r[i] = gr(i);
        cpu.r[0] = 0;
        if (ee) {
            cpu.hi[0] = emu.reg(hi_ref);
            cpu.lo[0] = emu.reg(lo_ref);
        } else {
            const u64 a = emu.reg(acc);
            cpu.hi[0] = sx32(static_cast<u32>(a >> 32));
            cpu.lo[0] = sx32(static_cast<u32>(a));
        }
    };
    bool in_emu = false;
    const auto use_emu = [&] {
        if (!in_emu) to_emu();
        in_emu = true;
    };
    const auto use_cpu = [&] {
        if (in_emu) from_emu();
        in_emu = false;
    };

    // Whether the description may run the instruction on the registers as they stand (they are the emulator's).
    const auto fine = [&](u32 w, const Info& info) {
        const unsigned op = w >> 26, rs = (w >> 21) & 31, rt = (w >> 16) & 31;
        switch (info.kind) {
            case Kind::Add32: {
                const std::int32_t a = static_cast<std::int32_t>(gr(rs)), b = op == 0 ? static_cast<std::int32_t>(gr(rt)) : static_cast<std::int32_t>(simm16(w));
                const std::int64_t s = (op == 0 && (w & 0x3F) == 0x22) ? static_cast<std::int64_t>(a) - b : static_cast<std::int64_t>(a) + b;
                return s == static_cast<std::int32_t>(s);
            }
            case Kind::Add64: {
                const u64 a = gr(rs), b = op == 0 ? gr(rt) : static_cast<u64>(simm16(w));
                const bool sub = op == 0 && (w & 0x3F) == 0x2E;
                const u64 r = sub ? a - b : a + b;
                const bool overflow = sub ? ((static_cast<i64>(a) < 0) != (static_cast<i64>(b) < 0) && (static_cast<i64>(r) < 0) != (static_cast<i64>(a) < 0))
                                          : ((static_cast<i64>(a) < 0) == (static_cast<i64>(b) < 0) && (static_cast<i64>(r) < 0) != (static_cast<i64>(a) < 0));
                return !overflow;
            }
            case Kind::Divide: {
                const u32 x = static_cast<u32>(gr(rs)), y = static_cast<u32>(gr(rt));
                return y != 0 && !((w & 0x3F) == 0x1A && x == 0x80000000u && y == 0xFFFFFFFFu);
            }
            case Kind::Load: case Kind::Store: {
                const u32 address = static_cast<u32>(gr(rs)) + static_cast<u32>(simm16(w));
                return address % info.size == 0;
            }
            case Kind::VarShift64: return gr(rs) < 64;
            case Kind::JumpReg: return (static_cast<u32>(gr(rs)) & 3) == 0;
            default: return true;
        }
    };
    // Whether a branch is taken, from the registers as they stand.
    const auto taken = [&](u32 w, const Info& info) {
        const unsigned op = w >> 26, rs = (w >> 21) & 31, rt = (w >> 16) & 31;
        if (info.kind == Kind::JumpReg || op == 2 || op == 3) return true;
        const i64 a = static_cast<i64>(gr(rs));
        switch (op) {
            case 0x01: return (rt & 1) ? a >= 0 : a < 0;
            case 0x04: case 0x14: return gr(rs) == gr(rt);
            case 0x05: case 0x15: return gr(rs) != gr(rt);
            case 0x06: case 0x16: return a <= 0;
            default: return a > 0;
        }
    };

    MipsLeEnd end;
    while (true) {
        if (cpu.pc == return_address) {
            end.kind = MipsLeEnd::Kind::Return;
            break;
        }
        if (cpu.steps >= max_steps) {
            end.kind = MipsLeEnd::Kind::Limit;
            break;
        }
        const u32 pc = cpu.pc;
        bool own = true;
        u32 word = 0, slot_word = 0;
        Info info, slot;
        if (cpu.next_pc == pc + 4 && (pc & 3) == 0 && !machine.is_stub(pc)) {
            u64 v = 0;
            if (machine.peek(pc, 4, v)) {
                word = static_cast<u32>(v);
                info = classify(word, ee);
                if (info.kind == Kind::Branch || info.kind == Kind::JumpReg) {
                    u64 s = 0;   // a branch and its delay slot go together; the slot must be an instruction of its own and clear of the branch's registers
                    if (cpu.steps + 2 <= max_steps && !machine.is_stub(pc + 4) && machine.peek(pc + 4, 4, s)) {
                        slot_word = static_cast<u32>(s);
                        slot = classify(slot_word, ee);
                        const int slot_destination = destination(slot_word, slot);
                        const unsigned brs = (word >> 21) & 31, brt = (word >> 16) & 31, srs = (slot_word >> 21) & 31, srt = (slot_word >> 16) & 31;
                        // (a branch that links through the register it reads, BLTZAL $ra or JALR $ra, $ra: the processor reads it before the link is written, the description after)
                        own = slot.kind == Kind::Own || slot.kind == Kind::Branch || slot.kind == Kind::JumpReg || (info.link != 0 && brs == info.link) ||
                              (slot_destination > 0 && (static_cast<unsigned>(slot_destination) == brs || static_cast<unsigned>(slot_destination) == brt)) ||
                              (info.link != 0 && (srs == info.link || srt == info.link));
                    }
                } else {
                    own = info.kind == Kind::Own;
                }
            }
        }
        if (!own) {
            use_emu();
            own = !fine(word, info) || ((info.kind == Kind::Branch || info.kind == Kind::JumpReg) && !fine(slot_word, slot));
        }
        if (!own) {
            const bool pair = info.kind == Kind::Branch || info.kind == Kind::JumpReg;
            const bool touches = info.kind == Kind::Load || info.kind == Kind::Store || slot.kind == Kind::Load || slot.kind == Kind::Store;
            const unsigned saved_at[4] = {31, (word >> 16) & 31, (word >> 11) & 31, (slot_word >> 16) & 31};
            u64 saved[4] = {};
            if (touches)
                for (unsigned i = 0; i < 4; ++i) saved[i] = emu.reg(refs[saved_at[i]]);
            const bool goes = pair ? taken(word, info) : false;
            std::uint8_t code[8] = {static_cast<std::uint8_t>(word), static_cast<std::uint8_t>(word >> 8), static_cast<std::uint8_t>(word >> 16), static_cast<std::uint8_t>(word >> 24),
                                    static_cast<std::uint8_t>(slot_word), static_cast<std::uint8_t>(slot_word >> 8), static_cast<std::uint8_t>(slot_word >> 16),
                                    static_cast<std::uint8_t>(slot_word >> 24)};
            const u64 at = ee ? sx32(pc) : pc;
            emu.write(at, code, pair ? 8 : 4);   // (the slot only with its branch: the bytes after a plain instruction are the next one's)
            emu.setPc(at);
            failed = false;
            const sl::Emulator::Status status = emu.step();
            if (status == sl::Emulator::Status::Error) {   // (the description failed half way: the state is not the instruction's)
                use_cpu();
                end.kind = MipsLeEnd::Kind::Stopped;
                end.fault = ml::Fault{ml::Fault::Kind::NoInstruction, pc, 0, {}, 0};
                break;
            }
            if (status == sl::Emulator::Status::Ok && !failed) {
                emu.setReg(refs[0], 0);
                const u32 next = static_cast<u32>(emu.pc());
                cpu.pc = next;
                cpu.next_pc = next + 4;
                cpu.steps += pair ? (info.likely && !goes ? 1 : 2) : 1;
                continue;
            }
            if (touches)   // (an access nothing answers: what the instruction wrote is put back, and the own core makes the fault)
                for (unsigned i = 0; i < 4; ++i) emu.setReg(refs[saved_at[i]], saved[i]);
            // (a word the description does not decode: nothing ran)
        }
        use_cpu();
        if (std::optional<ml::Fault> fault = cpu.step()) {
            end.kind = MipsLeEnd::Kind::Stopped;
            end.fault = std::move(fault);
            break;
        }
    }
    use_cpu();
    return end;
}

}  // namespace cb::engine::pcode_cpu
