#include "engine/pcode_cpu_sm83.hpp"

#include "engine/pcode_cpu_internal.hpp"

#include <array>
#include <cstring>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

#include "decompile/engine/pcode_emulator.hh"

namespace cb::engine::pcode_cpu {

namespace sm83cpu = cb::emulate::sm83;
namespace sl = cb::decompile::sleigh;

namespace {

// The registers, found once: the description names the halves of AF, BC, DE and HL, and SP and PC are 16 bits.
constexpr const char* kSm83Names[] = {"A", "F", "B", "C", "D", "E", "H", "L", "SP"};
namespace sm83_reg {
enum : std::size_t { A, F, B, C, D, E, H, L, SP };
}   // namespace sm83_reg

}  // namespace

sm83cpu::RunResult run(sm83cpu::Bus& bus, std::uint16_t entry, const sm83cpu::RunOptions& options) {
    sl::Emulator& emu = emulator_named("sm83.sla");
    const auto& reg = reg_refs(emu, kSm83Names);
    const std::size_t log_start = bus.log().size();
    const std::span<std::uint8_t> memory = bus.memory();
    constexpr std::uint32_t kSize = 0x10000;

    // The page map the emulator decodes from (`shadow`), and the bus's memory as it was last looked at (`plain`): they differ where an instruction was fetched from a device.
    std::vector<std::uint8_t> plain(memory.begin(), memory.begin() + kSize);
    std::vector<std::uint8_t> shadow = plain;
    emu.write(0, shadow.data(), kSize);
    emu.flushDecoded();
    emu.clearStops();

    bool wrote = false;
    emu.onRead([&](std::string_view space, std::uint64_t addr, unsigned size, std::uint64_t& value) {
        if (!is_ram(space)) return false;
        value = 0;
        for (unsigned i = 0; i < size; ++i) value |= static_cast<std::uint64_t>(bus.read(static_cast<std::uint16_t>(addr + i))) << (8 * i);
        return true;
    });
    emu.onWrite([&](std::string_view space, std::uint64_t addr, unsigned size, std::uint64_t value) {
        if (!is_ram(space)) return false;
        wrote = true;
        for (unsigned i = 0; i < size; ++i) bus.write(static_cast<std::uint16_t>(addr + i), static_cast<std::uint8_t>(value >> (8 * i)));
        return true;
    });
    // what the description leaves to the machine: HALT, STOP, the interrupt-master flag, and the opcodes the CPU does not define (the p-code has no register for them)
    bool halted = options.regs.halted.value_or(false), ime = options.regs.ime.value_or(false);
    std::optional<sm83cpu::Fault> fault;   // set by STOP or an undefined opcode: the run ends after that instruction
    emu.onUserOp([&](sl::Emulator& self, const std::string& name, const std::vector<std::uint64_t>&, std::uint64_t&) {
        const std::uint16_t at = static_cast<std::uint16_t>(self.pc());   // (still the instruction's own address)
        if (name == "halt") halted = true;
        else if (name == "enableMaskableInterrupts") ime = true;
        else if (name == "disableMaskableInterrupts") ime = false;
        else if (name == "stop") fault = sm83cpu::Fault{sm83cpu::Fault::Kind::Stop, at};
        else if (name == "undefinedOpcode")   // ($10 and a second byte that is not zero is a STOP, as the own core decodes it)
            fault = sm83cpu::Fault{shadow[at] == 0x10 ? sm83cpu::Fault::Kind::Stop : sm83cpu::Fault::Kind::Undecodable, at};
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

    const sm83cpu::RegisterInit& init = options.regs;
    const auto byte = [](const std::optional<std::uint8_t>& v) -> std::uint64_t { return v.value_or(0); };
    emu.setReg(reg[sm83_reg::A], byte(init.a));
    emu.setReg(reg[sm83_reg::F], byte(init.f) & 0xF0);   // (F keeps only its high nibble)
    emu.setReg(reg[sm83_reg::B], byte(init.b));
    emu.setReg(reg[sm83_reg::C], byte(init.c));
    emu.setReg(reg[sm83_reg::D], byte(init.d));
    emu.setReg(reg[sm83_reg::E], byte(init.e));
    emu.setReg(reg[sm83_reg::H], byte(init.h));
    emu.setReg(reg[sm83_reg::L], byte(init.l));
    // the call as CALL makes it: the return address pushed through the bus like any write
    const std::uint16_t sp = static_cast<std::uint16_t>(options.sp - 2);
    bus.write(sp, static_cast<std::uint8_t>(sm83cpu::kReturnAddress));
    bus.write(static_cast<std::uint16_t>(sp + 1), static_cast<std::uint8_t>(sm83cpu::kReturnAddress >> 8));
    emu.setReg(reg[sm83_reg::SP], sp);
    emu.setPc(entry);

    const auto sync = [&] {   // what a store or a device did to the bus's memory (a stack push, a store into RAM code runs from), to the page map
        if (std::memcmp(plain.data(), memory.data(), kSize) == 0) return;
        std::uint32_t first = 0, last = kSize - 1;
        while (plain[first] == memory[first]) ++first;
        while (plain[last] == memory[last]) --last;
        std::memcpy(plain.data() + first, memory.data() + first, last - first + 1);
        std::memcpy(shadow.data() + first, memory.data() + first, last - first + 1);
        emu.write(first, shadow.data() + first, last - first + 1);
    };
    sync();   // (the return address just pushed)

    // The own core fetches every instruction through the bus (a device in the range sees each read and answers it, and the answers are what is decoded): the first time at an
    // address three bytes, afterwards the bytes the instruction had, read again and compared, and three more if one changed. The adapter makes those reads, in that order, and
    // gives the emulator the bytes they returned.
    struct Known {
        std::uint8_t size;
        std::array<std::uint8_t, 3> raw;
    };
    std::unordered_map<std::uint16_t, Known> known;

    sm83cpu::RunResult out;
    out.end = sm83cpu::End::Limit;
    std::uint64_t steps = 0;
    std::optional<std::uint16_t> back_to;   // an undefined opcode leaves the pc at it
    while (steps < options.max_steps) {
        if (emu.pc() > 0xFFFF) emu.setPc(emu.pc() & 0xFFFF);   // (the pc is 16 bits: an instruction that ends at the top of memory goes on at 0)
        if (emu.pc() == sm83cpu::kReturnAddress) {
            out.end = sm83cpu::End::Return;
            break;
        }
        const std::uint16_t pc = static_cast<std::uint16_t>(emu.pc());
        if (halted) {   // an earlier HALT ran: nothing runs any more
            out.end = sm83cpu::End::Stopped;
            out.fault = sm83cpu::Fault{sm83cpu::Fault::Kind::Halt, pc};
            break;
        }
        const std::size_t seen = bus.log().size();
        std::array<std::uint8_t, 3> code{};
        const auto it = known.find(pc);
        bool fresh = it == known.end();
        if (!fresh)
            for (std::size_t i = 0; i < it->second.size; ++i)
                if (bus.read(static_cast<std::uint16_t>(pc + i)) != it->second.raw[i]) fresh = true;
        if (fresh)
            for (std::size_t i = 0; i < code.size(); ++i) code[i] = bus.read(static_cast<std::uint16_t>(pc + i));
        else
            code = it->second.raw;
        for (std::size_t i = 0; i < code.size(); ++i) {   // (the page map holds what was fetched)
            const std::uint16_t at = static_cast<std::uint16_t>(pc + i);
            if (shadow[at] != code[i]) {
                shadow[at] = code[i];
                emu.write(at, &code[i], 1);
            }
        }
        const std::size_t length = emu.length(pc);
        wrote = false;
        const sl::Emulator::Status status = emu.step();
        if (status == sl::Emulator::Status::BadInstruction || status == sl::Emulator::Status::Error) {
            out.end = sm83cpu::End::Stopped;
            out.fault = sm83cpu::Fault{sm83cpu::Fault::Kind::Undecodable, pc};
            break;
        }
        if (!fault || fault->kind != sm83cpu::Fault::Kind::Undecodable) known[pc] = Known{static_cast<std::uint8_t>(length), code};   // (decoded: an opcode that is none is not kept)
        if (wrote || bus.log().size() != seen) sync();
        if (fault) {   // neither a STOP nor an undefined opcode counts as a step
            out.end = sm83cpu::End::Stopped;
            out.fault = fault;
            if (fault->kind == sm83cpu::Fault::Kind::Undecodable) back_to = fault->pc;
            break;
        }
        ++steps;
    }
    sm83cpu::Registers& r = out.regs;
    r.a = static_cast<std::uint8_t>(emu.reg(reg[sm83_reg::A]));
    r.f = static_cast<std::uint8_t>(emu.reg(reg[sm83_reg::F]));
    r.b = static_cast<std::uint8_t>(emu.reg(reg[sm83_reg::B]));
    r.c = static_cast<std::uint8_t>(emu.reg(reg[sm83_reg::C]));
    r.d = static_cast<std::uint8_t>(emu.reg(reg[sm83_reg::D]));
    r.e = static_cast<std::uint8_t>(emu.reg(reg[sm83_reg::E]));
    r.h = static_cast<std::uint8_t>(emu.reg(reg[sm83_reg::H]));
    r.l = static_cast<std::uint8_t>(emu.reg(reg[sm83_reg::L]));
    r.sp = static_cast<std::uint16_t>(emu.reg(reg[sm83_reg::SP]));
    r.pc = back_to ? *back_to : static_cast<std::uint16_t>(emu.pc());
    r.ime = ime;
    r.halted = halted;
    out.steps = steps;
    out.io.assign(bus.log().begin() + static_cast<std::ptrdiff_t>(log_start), bus.log().end());
    return out;
}

sm83cpu::RunResult call(sm83cpu::Bus& bus, std::uint16_t entry, const sm83cpu::RunOptions& options) { return enabled_when_asked() ? pcode_cpu::run(bus, entry, options) : sm83cpu::run(bus, entry, options); }

}  // namespace cb::engine::pcode_cpu
