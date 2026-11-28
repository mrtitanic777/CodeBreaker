#include "emulate/cpu/pcode_cpu_sm83.hpp"

#include "emulate/cpu/pcode_cpu_internal.hpp"

#include <array>
#include <cstring>
#include <optional>
#include <string>
#include <vector>

#include "decompile/engine/pcode_emulator.hh"

namespace cb::emulate::pcode_cpu {

namespace sm83cpu = cb::emulate::sm83;
namespace sl = cb::decompile::sleigh;

namespace {

// The registers, found once: the description names the halves of AF, BC, DE and HL, and SP and PC are 16 bits.
constexpr const char* kSm83Names[] = {"A", "F", "B", "C", "D", "E", "H", "L", "SP"};
namespace sm83_reg {
enum : std::size_t { A, F, B, C, D, E, H, L, SP };
}   // namespace sm83_reg

constexpr std::uint32_t kSize = 0x10000;

}  // namespace

// The SM83's bus on the emulator of this thread: the page map instructions are decoded from (only the bytes of the instruction being run are read from it, put there
// from the bus's own fetch before each step, so a store or a device's doing to memory needs no copying), the read and write hooks, and what the description leaves to
// the machine (HALT, STOP, the interrupt-master flag, the opcodes the CPU does not define). run() makes one for a call; Sm83Cpu keeps one for its steps. A session that
// finds another one ran on the thread's emulator since puts its hooks back (`claim`).
class Sm83Session {
public:
    explicit Sm83Session(sm83cpu::Bus& bus) : emu_(emulator_named("sm83.sla")), reg_(reg_refs(emu_, kSm83Names)), bus_(bus), run_(++run_number()) {
        std::vector<std::uint8_t>& image = shadow();
        if (image.size() != kSize) {
            image.assign(kSize, 0);
            emu_.flushDecoded();
        }
        claim();
    }
    ~Sm83Session() {
        if (active() == this) {
            emu_.onRead({});
            emu_.onWrite({});
            emu_.onUserOp({});
            active() = nullptr;
        }
    }
    Sm83Session(const Sm83Session&) = delete;
    Sm83Session& operator=(const Sm83Session&) = delete;

    sl::Emulator& emu() { return emu_; }
    bool halted = false, ime = false;

    void claim() {
        if (active() == this) return;
        active() = this;
        emu_.clearStops();
        emu_.onRead([this](std::string_view space, std::uint64_t addr, unsigned size, std::uint64_t& value) {
            if (!is_ram(space)) return false;
            value = 0;
            for (unsigned i = 0; i < size; ++i) value |= static_cast<std::uint64_t>(bus_.read(static_cast<std::uint16_t>(addr + i))) << (8 * i);
            return true;
        });
        emu_.onWrite([this](std::string_view space, std::uint64_t addr, unsigned size, std::uint64_t value) {
            if (!is_ram(space)) return false;
            for (unsigned i = 0; i < size; ++i) bus_.write(static_cast<std::uint16_t>(addr + i), static_cast<std::uint8_t>(value >> (8 * i)));
            return true;
        });
        emu_.onUserOp([this](sl::Emulator& self, const std::string& name, const std::vector<std::uint64_t>&, std::uint64_t&) {
            const std::uint16_t at = static_cast<std::uint16_t>(self.pc());   // (still the instruction's own address)
            if (name == "halt") halted = true;
            else if (name == "enableMaskableInterrupts") ime = true;
            else if (name == "disableMaskableInterrupts") ime = false;
            else if (name == "stop") fault_ = sm83cpu::Fault{sm83cpu::Fault::Kind::Stop, at};
            else if (name == "undefinedOpcode")   // ($10 and a second byte that is not zero is a STOP, as the own core decodes it)
                fault_ = sm83cpu::Fault{shadow()[at] == 0x10 ? sm83cpu::Fault::Kind::Stop : sm83cpu::Fault::Kind::Undecodable, at};
            else return false;
            return true;
        });
    }

    void set_registers(const sm83cpu::Registers& r) {
        emu_.setReg(reg_[sm83_reg::A], r.a);
        emu_.setReg(reg_[sm83_reg::F], r.f);
        emu_.setReg(reg_[sm83_reg::B], r.b);
        emu_.setReg(reg_[sm83_reg::C], r.c);
        emu_.setReg(reg_[sm83_reg::D], r.d);
        emu_.setReg(reg_[sm83_reg::E], r.e);
        emu_.setReg(reg_[sm83_reg::H], r.h);
        emu_.setReg(reg_[sm83_reg::L], r.l);
        emu_.setReg(reg_[sm83_reg::SP], r.sp);
        emu_.setPc(r.pc);
        halted = r.halted;
        ime = r.ime;
    }
    sm83cpu::Registers registers() const {
        sm83cpu::Registers r;
        r.a = static_cast<std::uint8_t>(emu_.reg(reg_[sm83_reg::A]));
        r.f = static_cast<std::uint8_t>(emu_.reg(reg_[sm83_reg::F]));
        r.b = static_cast<std::uint8_t>(emu_.reg(reg_[sm83_reg::B]));
        r.c = static_cast<std::uint8_t>(emu_.reg(reg_[sm83_reg::C]));
        r.d = static_cast<std::uint8_t>(emu_.reg(reg_[sm83_reg::D]));
        r.e = static_cast<std::uint8_t>(emu_.reg(reg_[sm83_reg::E]));
        r.h = static_cast<std::uint8_t>(emu_.reg(reg_[sm83_reg::H]));
        r.l = static_cast<std::uint8_t>(emu_.reg(reg_[sm83_reg::L]));
        r.sp = static_cast<std::uint16_t>(emu_.reg(reg_[sm83_reg::SP]));
        r.pc = static_cast<std::uint16_t>(emu_.pc());
        r.ime = ime;
        r.halted = halted;
        return r;
    }

    // The instruction at the pc. nullopt: it ran (a step). A Fault: it did not (an earlier HALT; no instruction there, the pc left at it) or it was a STOP (the pc
    // past it); neither is a step.
    std::optional<sm83cpu::Fault> execute() {
        if (emu_.pc() > 0xFFFF) emu_.setPc(emu_.pc() & 0xFFFF);   // (the pc is 16 bits: an instruction that ends at the top of memory goes on at 0)
        const std::uint16_t pc = static_cast<std::uint16_t>(emu_.pc());
        if (halted) return sm83cpu::Fault{sm83cpu::Fault::Kind::Halt, pc};   // an earlier HALT ran: nothing runs any more
        // The own core fetched every instruction through the bus (a device in the range sees each read and answers it, and the answers are what is decoded): the first
        // time at an address three bytes, afterwards the bytes the instruction had, read again and compared, and three more if one changed. These are those reads, in
        // that order, and the emulator is given the bytes they returned.
        std::vector<std::uint8_t>& image = shadow();
        std::array<std::uint8_t, 3> code{};
        Known& at_pc = known()[pc];
        bool fresh = at_pc.run != run_;
        if (!fresh)
            for (std::size_t i = 0; i < at_pc.size; ++i)
                if (bus_.read(static_cast<std::uint16_t>(pc + i)) != at_pc.raw[i]) fresh = true;
        if (fresh)
            for (std::size_t i = 0; i < code.size(); ++i) code[i] = bus_.read(static_cast<std::uint16_t>(pc + i));
        else
            code = at_pc.raw;
        for (std::size_t i = 0; i < code.size(); ++i) {   // (the page map holds what was fetched)
            const std::uint16_t at = static_cast<std::uint16_t>(pc + i);
            if (image[at] != code[i]) {
                image[at] = code[i];
                emu_.write(at, &code[i], 1);
            }
        }
        const std::size_t length = fresh ? emu_.length(pc) : at_pc.size;
        fault_.reset();
        const sl::Emulator::Status status = emu_.step();
        if (status == sl::Emulator::Status::BadInstruction || status == sl::Emulator::Status::Error) return sm83cpu::Fault{sm83cpu::Fault::Kind::Undecodable, pc};
        if (!fault_ || fault_->kind != sm83cpu::Fault::Kind::Undecodable) at_pc = Known{run_, static_cast<std::uint8_t>(length), code};   // (an opcode that is none is not kept)
        if (fault_ && fault_->kind == sm83cpu::Fault::Kind::Undecodable) emu_.setPc(pc);   // (an undefined opcode leaves the pc at it)
        return fault_;
    }

private:
    struct Known {
        std::uint32_t run = 0;   // the session that fetched it (a table kept between sessions is not cleared: an entry of an earlier one is none)
        std::uint8_t size = 0;
        std::array<std::uint8_t, 3> raw{};
    };
    static Sm83Session*& active() {
        thread_local Sm83Session* current = nullptr;
        return current;
    }
    static std::vector<std::uint8_t>& shadow() {   // what the emulator holds: kept between sessions, with what it decoded from it
        thread_local std::vector<std::uint8_t> image;
        return image;
    }
    static std::vector<Known>& known() {
        thread_local std::vector<Known> table(kSize);
        return table;
    }
    static std::uint32_t& run_number() {
        thread_local std::uint32_t n = 0;
        return n;
    }
    sl::Emulator& emu_;
    const std::array<sl::Emulator::RegRef, std::size(kSm83Names)>& reg_;
    sm83cpu::Bus& bus_;
    std::uint32_t run_;
    std::optional<sm83cpu::Fault> fault_;   // set by STOP or an undefined opcode during the step
};

sm83cpu::RunResult run(sm83cpu::Bus& bus, std::uint16_t entry, const sm83cpu::RunOptions& options) {
    const std::size_t log_start = bus.log().size();
    Sm83Session session(bus);
    sl::Emulator& emu = session.emu();
    const sm83cpu::RegisterInit& init = options.regs;
    sm83cpu::Registers start;
    start.a = init.a.value_or(0);
    start.f = static_cast<std::uint8_t>(init.f.value_or(0) & 0xF0);   // (F keeps only its high nibble)
    start.b = init.b.value_or(0);
    start.c = init.c.value_or(0);
    start.d = init.d.value_or(0);
    start.e = init.e.value_or(0);
    start.h = init.h.value_or(0);
    start.l = init.l.value_or(0);
    start.halted = init.halted.value_or(false);
    start.ime = init.ime.value_or(false);
    // the call as CALL makes it: the return address pushed through the bus like any write
    start.sp = static_cast<std::uint16_t>(options.sp - 2);
    bus.write(start.sp, static_cast<std::uint8_t>(sm83cpu::kReturnAddress));
    bus.write(static_cast<std::uint16_t>(start.sp + 1), static_cast<std::uint8_t>(sm83cpu::kReturnAddress >> 8));
    start.pc = entry;
    session.set_registers(start);

    sm83cpu::RunResult out;
    out.end = sm83cpu::End::Limit;
    std::uint64_t steps = 0;
    while (steps < options.max_steps) {
        if ((emu.pc() & 0xFFFF) == sm83cpu::kReturnAddress) {
            out.end = sm83cpu::End::Return;
            break;
        }
        if (std::optional<sm83cpu::Fault> fault = session.execute()) {
            out.end = sm83cpu::End::Stopped;
            out.fault = fault;
            break;
        }
        ++steps;
    }
    out.regs = session.registers();
    out.steps = steps;
    out.io.assign(bus.log().begin() + static_cast<std::ptrdiff_t>(log_start), bus.log().end());
    return out;
}

Sm83Cpu::Sm83Cpu(sm83cpu::Bus& bus) : session_(std::make_unique<Sm83Session>(bus)) {}
Sm83Cpu::~Sm83Cpu() = default;

std::optional<sm83cpu::Fault> Sm83Cpu::step() {
    session_->claim();
    session_->set_registers(regs);
    std::optional<sm83cpu::Fault> fault = session_->execute();
    if (fault && fault->kind != sm83cpu::Fault::Kind::Stop) return fault;   // (nothing changed)
    regs = session_->registers();
    if (!fault) ++steps;
    return fault;
}

}  // namespace cb::emulate::pcode_cpu
