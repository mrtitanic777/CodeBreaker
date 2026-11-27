#include "engine/pcode_cpu.hpp"

#include "engine/pcode_cpu_internal.hpp"

#include <algorithm>
#include <atomic>
#include <cstring>
#include <filesystem>
#include <map>
#include <memory>
#include <span>
#include <string>
#include <string_view>
#include <vector>

#include "base/config.hpp"
#include "core/error.hpp"
#include "core/fs.hpp"
#include "decompile/engine/pcode_emulator.hh"
#include "decompile/m6502.hpp"

namespace cb::engine::pcode_cpu {

namespace m = cb::emulate::m6502;
namespace sl = cb::decompile::sleigh;

namespace {

std::atomic<int> g_force{-1};   // -1: the environment; 0 / 1: forced

std::filesystem::path languages() {
    if (const std::optional<std::string> env = cb::fs::get_env("CB_LANGUAGES_DIR"); env && !env->empty()) return cb::fs::from_text(*env);
    return config::current().checkout_root() / "src" / "decompile" / "engine" / "languages";
}

constexpr std::pair<const char*, std::uint8_t> kFlags[] = {{"N", m::status::N}, {"V", m::status::V}, {"B", m::status::B}, {"D", m::status::D},
                                                           {"I", m::status::I}, {"Z", m::status::Z}, {"C", m::status::C}};

}  // namespace

// One emulator per thread and variant: reading the description is the costly part of a run, the state is all set again each time.
sl::Emulator& emulator_named(const char* description) {
    thread_local std::map<std::string, std::unique_ptr<sl::Emulator>> cache;
    auto& slot = cache[description];
    if (!slot) slot = std::make_unique<sl::Emulator>(languages() / description, std::vector<std::pair<std::string, int>>{});
    return *slot;
}

bool is_ram(std::string_view space) { return space.size() == 3 && (space[0] | 32) == 'r' && (space[1] | 32) == 'a' && (space[2] | 32) == 'm'; }

namespace {
sl::Emulator& emulator_for(m::Variant variant) { return emulator_named(variant == m::Variant::Cmos ? "65sc02.sla" : "6502.sla"); }
}  // namespace

bool enabled() {
    if (const int f = g_force.load(); f >= 0) return f == 1;
    const std::optional<std::string> env = cb::fs::get_env("CB_PCODE_EMU");
    return !(env && (*env == "0" || *env == "false" || *env == "no"));
}

bool enabled_when_asked() {
    if (const int f = g_force.load(); f >= 0) return f == 1;
    const std::optional<std::string> env = cb::fs::get_env("CB_PCODE_EMU");
    return env && (*env == "1" || *env == "true" || *env == "yes");
}

void force(std::optional<bool> on) { g_force = on ? (*on ? 1 : 0) : -1; }

// ---- the 6502 -----------------------------------------------------------------------------------------------------------------------

// The 6502 family's bus on the emulator of this thread: the page map instructions are decoded from, the read and write hooks, the stack as the chip
// keeps it, code fetched from a device range. run() makes one for a call; M6502Cpu keeps one for its steps. The emulator is the thread's: a session
// that finds another one was there since it last ran puts its page map and hooks back (`claim`).
class M6502Session {
public:
    M6502Session(m::Bus& bus, m::Variant variant, bool decimal)
        : emu_(emulator_for(variant)), bus_(bus), variant_(variant), decimal_(decimal), window_(static_cast<std::uint32_t>(bus.address_mask()) + 1), memory_(bus.memory()),
          plain_(memory_.begin(), memory_.begin() + window_), shadow_(window_), logged_(bus.logged_reads()), sh_(emu_.regRef("SH")) {
        for (std::uint32_t a = 0; a < window_; ++a) shadow_[a] = bus.peek(static_cast<std::uint16_t>(a));
        // Code fetched from a device range: the own core read every byte of the instruction through the bus (the device answers, the read is logged, and what the
        // device answers is what runs). The page map shows such a range as an opcode the description has no instruction for, so a run stops before an instruction
        // there and the session makes the instruction as the chip does (device_step).
        if (!logged_.empty() && variant == m::Variant::Nmos) {   // (the CMOS description may run what its table calls undocumented)
            const auto& table = decompile::m6502::opcode_table(variant);
            for (unsigned op = 0; op < 256 && !poisoned_; ++op)
                if (!table[op]) {
                    poison_ = static_cast<std::uint8_t>(op);
                    poisoned_ = true;
                }
        }
        claim();
        seen_ = bus.log().size();
    }
    ~M6502Session() {
        if (active() == this) {
            emu_.onRead({});
            emu_.onWrite({});
            emu_.clearPin();
            active() = nullptr;
        }
    }
    M6502Session(const M6502Session&) = delete;
    M6502Session& operator=(const M6502Session&) = delete;

    sl::Emulator& emu() { return emu_; }

    // This session's page map and hooks on the thread's emulator (again, when another session ran since).
    void claim() {
        if (active() == this) return;
        active() = this;
        for (std::uint32_t base = 0; base < 0x10000; base += window_) emu_.write(base, shadow_.data(), std::min<std::uint32_t>(window_, 0x10000 - base));
        if (poisoned_)
            for (const auto& [lo, hi] : logged_)
                for (std::uint32_t a = lo; a <= hi && a < window_; ++a) put(a, shadow_[a]);
        emu_.flushDecoded();
        emu_.clearStops();
        emu_.onRead([this](std::string_view space, std::uint64_t addr, unsigned size, std::uint64_t& value) {
            if (!is_ram(space)) return false;
            value = 0;
            for (unsigned i = 0; i < size; ++i) value |= static_cast<std::uint64_t>(bus_.read(stack_address(static_cast<std::uint16_t>(addr + i), false))) << (8 * i);
            return true;
        });
        emu_.onWrite([this](std::string_view space, std::uint64_t addr, unsigned size, std::uint64_t value) {
            if (!is_ram(space)) return false;
            for (unsigned i = 0; i < size; ++i) {
                const std::uint16_t at = stack_address(static_cast<std::uint16_t>(addr + i), true);
                bus_.write(at, static_cast<std::uint8_t>(value >> (8 * i)));
                const std::uint32_t index = at & bus_.address_mask();
                if (const std::uint8_t now = bus_.peek(at); now != shadow_[index]) put(index, now);   // (a store into RAM that code runs from)
            }
            return true;
        });
        emu_.setReg("BCD", decimal_ ? 1 : 0);   // (whether ADC and SBC are decimal with D set: the description's switch)
        emu_.setPin(sh_, 1);   // (the description's SP is 16 bits wide: the chip's S wraps within page 1: kept so after every instruction)
    }

    // What a device or the caller did to the bus's memory (a bank switch, bytes put there between steps), to every mirror the emulator decodes from.
    void sync() {
        if (std::memcmp(plain_.data(), memory_.data(), window_) == 0) return;
        std::uint32_t first = 0, last = window_ - 1;
        while (plain_[first] == memory_[first]) ++first;
        while (plain_[last] == memory_[last]) --last;
        for (std::uint32_t i = first; i <= last; ++i) {
            plain_[i] = memory_[i];
            if (const std::uint8_t now = bus_.peek(static_cast<std::uint16_t>(i)); now != shadow_[i]) put(i, now);
        }
        // (a write to a page code was decoded from drops the decoded instructions by itself)
    }
    // The bytes of the instruction at `pc` as the processor finds them now (what the caller or a device put there since the last step); the rest of
    // memory is read through the bus as the instruction runs.
    void refresh(std::uint16_t pc) {
        for (unsigned i = 0; i < 3; ++i) {
            const std::uint16_t at = static_cast<std::uint16_t>(pc + i);
            const std::uint32_t index = at & bus_.address_mask();
            if (const std::uint8_t now = bus_.peek(at); now != shadow_[index]) put(index, now);
        }
    }
    // After instructions ran: when a device was touched it may have changed the memory under the code.
    void after_devices() {
        if (bus_.log().size() == seen_) return;
        sync();
        seen_ = bus_.log().size();
    }

    // The registers: the flags are one byte each in the description, P is made of them. (B is no flag of the chip: the description sets it as PHP and BRK
    // push it; P keeps the bit it was given.)
    void set_registers(const m::Registers& r) {
        emu_.setReg("A", r.a);
        emu_.setReg("X", r.x);
        emu_.setReg("Y", r.y);
        for (const auto& [name, bit] : kFlags) emu_.setReg(name, (r.p & bit) != 0 ? 1 : 0);
        emu_.setReg(sh_, 1);
        emu_.setReg("S", r.s);
        emu_.setPc(r.pc);
    }
    m::Registers registers(std::uint8_t given_p) const {
        m::Registers r;
        r.a = static_cast<std::uint8_t>(emu_.reg("A"));
        r.x = static_cast<std::uint8_t>(emu_.reg("X"));
        r.y = static_cast<std::uint8_t>(emu_.reg("Y"));
        r.s = static_cast<std::uint8_t>(emu_.reg("S"));
        r.pc = static_cast<std::uint16_t>(emu_.pc());
        std::uint8_t p = static_cast<std::uint8_t>(m::status::U | (given_p & m::status::B));
        for (const auto& [name, bit] : kFlags)
            if (bit != m::status::B && emu_.reg(name) != 0) p |= bit;
        r.p = p;
        return r;
    }

    // The instruction at the pc when the emulator stopped on it as bad: one fetched from a device range is made as the chip makes it (nullopt: it ran),
    // anything else is the fault of an undocumented opcode.
    std::optional<m::Fault> bad_instruction() {
        const std::uint16_t pc = static_cast<std::uint16_t>(emu_.pc());
        if (!(poisoned_ && is_logged(pc & bus_.address_mask()))) return m::Fault{static_cast<std::uint8_t>(emu_.readValue(pc, 1)), pc};
        const auto& table = decompile::m6502::opcode_table(variant_);
        const std::uint8_t opcode = bus_.read(pc);
        if (!table[opcode]) return m::Fault{opcode, pc};
        const unsigned size = decompile::m6502::instruction_size(table[opcode]->mode);
        std::uint8_t bytes[3] = {opcode, 0, 0};
        for (unsigned i = 1; i < size; ++i) bytes[i] = bus_.read(static_cast<std::uint16_t>(pc + i));
        emu_.write(pc, bytes, size);
        const sl::Emulator::Status status = emu_.step();
        for (unsigned i = 0; i < size; ++i) {   // (the page map as it was)
            const std::uint32_t index = static_cast<std::uint16_t>(pc + i) & bus_.address_mask();
            const std::uint8_t shown = is_logged(index) ? poison_ : shadow_[index];
            emu_.write(static_cast<std::uint16_t>(pc + i), &shown, 1);
        }
        emu_.setReg(sh_, 1);
        if (status != sl::Emulator::Status::Ok) return m::Fault{opcode, pc};
        after_devices();
        return std::nullopt;
    }

private:
    static M6502Session*& active() {
        thread_local M6502Session* current = nullptr;
        return current;
    }
    bool is_logged(std::uint32_t index) const {
        for (const auto& [lo, hi] : logged_)
            if (index >= lo && index <= hi) return true;
        return false;
    }
    void put(std::uint32_t index, std::uint8_t value) {   // one byte of the page map, to every mirror
        shadow_[index] = value;
        const std::uint8_t shown = poisoned_ && is_logged(index) ? poison_ : value;
        for (std::uint32_t base = 0; base < 0x10000; base += window_)
            if (base + index < 0x10000) emu_.write(base + index, &shown, 1);
    }
    // The stack as the chip keeps it: S wraps within page 1. The description's pointer is 16 bits, so a push that goes below page 1 (S=0, a JSR's second byte)
    // or a pull that goes above it (S=$FF) would touch page 0 or 2; the pointer's high byte is 0 or 2 then, and no other instruction runs with it so (it is set
    // back to 1 after each one).
    std::uint16_t stack_address(std::uint16_t at, bool write) const {
        const std::uint64_t high = emu_.reg(sh_);
        if (write ? (high == 0 && (at >> 8) == 0) : (high == 2 && (at >> 8) == 2)) return static_cast<std::uint16_t>(write ? at + 0x100 : at - 0x100);
        return at;
    }

    sl::Emulator& emu_;
    m::Bus& bus_;
    m::Variant variant_;
    bool decimal_;
    std::uint32_t window_;   // the bytes the chip's address lines reach
    std::span<std::uint8_t> memory_;
    std::vector<std::uint8_t> plain_;    // the bus's own memory as it was last looked at
    std::vector<std::uint8_t> shadow_;   // what the processor finds in memory (the bus's memory and the quiet RAM a machine keeps behind callbacks: Bus::peek)
    std::vector<std::pair<std::uint16_t, std::uint16_t>> logged_;
    std::uint8_t poison_ = 0;
    bool poisoned_ = false;
    sl::Emulator::RegRef sh_;
    std::size_t seen_ = 0;
};

m::RunResult run(m::Bus& bus, std::uint16_t entry, const m::RunOptions& options) {
    const std::size_t log_start = bus.log().size();
    M6502Session session(bus, options.variant, options.decimal);
    sl::Emulator& emu = session.emu();
    // the call as JSR makes it: the return address less one pushed (through the bus like any write), RTS adds one
    const std::uint16_t ret = m::kReturnAddress - 1;
    bus.write(0x1FD, static_cast<std::uint8_t>(ret >> 8));
    bus.write(0x1FC, static_cast<std::uint8_t>(ret));
    m::Registers start;
    start.a = options.a;
    start.x = options.x;
    start.y = options.y;
    start.p = options.p;
    start.s = 0xFB;
    start.pc = entry;
    session.set_registers(start);
    session.sync();   // (the return address just pushed)

    m::RunResult out;
    out.end = m::End::Limit;
    std::uint64_t steps = 0;
    while (steps < options.max_steps) {
        if (emu.pc() == m::kReturnAddress) {
            out.end = m::End::Return;
            break;
        }
        // (a run of instructions at once: it ends where a device was touched, code bytes changed or the return address was reached, which is where this looks)
        const sl::Emulator::BlockResult ran = emu.runBlocks(options.max_steps - steps, true, m::kReturnAddress);
        steps += ran.executed;
        if (ran.status == sl::Emulator::Status::BadInstruction || ran.status == sl::Emulator::Status::Error) {
            if (std::optional<m::Fault> fault = session.bad_instruction()) {
                out.end = m::End::Stopped;
                out.fault = fault;
                break;
            }
            ++steps;
            continue;
        }
        session.after_devices();
    }
    out.regs = session.registers(options.p);
    out.steps = steps;
    out.io.assign(bus.log().begin() + static_cast<std::ptrdiff_t>(log_start), bus.log().end());
    return out;
}

M6502Cpu::M6502Cpu(m::Bus& bus, bool decimal, m::Variant variant) : session_(std::make_unique<M6502Session>(bus, variant, decimal)) {}
M6502Cpu::~M6502Cpu() = default;

std::optional<m::Fault> M6502Cpu::step() {
    session_->claim();
    session_->refresh(regs.pc);   // (what was put in memory since the last step)
    session_->set_registers(regs);
    const sl::Emulator::Status status = session_->emu().step();
    if (status == sl::Emulator::Status::BadInstruction || status == sl::Emulator::Status::Error) {
        if (std::optional<m::Fault> fault = session_->bad_instruction()) {
            session_->set_registers(regs);   // (nothing changed)
            return fault;
        }
    }
    regs = session_->registers(regs.p);
    ++steps;
    return std::nullopt;
}

m::RunResult call(m::Bus& bus, std::uint16_t entry, const m::RunOptions& options) { return enabled() ? pcode_cpu::run(bus, entry, options) : m::run(bus, entry, options); }

// ---- the Z80 ----------------------------------------------------------------------------------------------------------------------

namespace z = cb::emulate::z80;

z::RunResult run(z::Bus& bus, std::uint16_t entry, const z::RunOptions& options) {
    sl::Emulator& emu = emulator_named("z80.sla");
    const std::size_t log_start = bus.log().size();
    const std::span<std::uint8_t> memory = bus.memory();
    constexpr std::uint32_t kSize = 0x10000;

    std::vector<std::uint8_t> shadow(memory.begin(), memory.begin() + kSize);
    emu.write(0, shadow.data(), kSize);
    emu.flushDecoded();
    emu.clearStops();

    bool touched = false;
    emu.onRead([&](std::string_view space, std::uint64_t addr, unsigned size, std::uint64_t& value) {
        value = 0;
        if (is_ram(space)) {
            touched = true;
            for (unsigned i = 0; i < size; ++i) value |= static_cast<std::uint64_t>(bus.read(static_cast<std::uint16_t>(addr + i))) << (8 * i);
            return true;
        }
        if (space == "io") {   // the port as the CPU puts it on the bus: all sixteen lines (the description's IN A,(n) puts A on the high ones)
            touched = true;
            value = bus.in(static_cast<std::uint16_t>(addr));
            return true;
        }
        return false;
    });
    emu.onWrite([&](std::string_view space, std::uint64_t addr, unsigned size, std::uint64_t value) {
        if (is_ram(space)) {
            touched = true;
            for (unsigned i = 0; i < size; ++i) bus.write(static_cast<std::uint16_t>(addr + i), static_cast<std::uint8_t>(value >> (8 * i)));
            return true;
        }
        if (space == "io") {
            touched = true;
            bus.out(static_cast<std::uint16_t>(addr), static_cast<std::uint8_t>(value));
            return true;
        }
        return false;
    });
    // what the description leaves to the machine: HALT, the interrupt flip-flops and the mode (the p-code has no register for them)
    bool halted = false, iff1 = false, iff2 = false;
    std::uint8_t im = 0;
    emu.onUserOp([&](sl::Emulator&, const std::string& name, const std::vector<std::uint64_t>& args, std::uint64_t& result) {
        if (name == "getIFF2") result = iff2 ? 1 : 0;
        else if (name == "halt") halted = true;
        else if (name == "disableMaskableInterrupts") iff1 = iff2 = false;
        else if (name == "enableMaskableInterrupts") iff1 = iff2 = true;
        else if (name == "setInterruptMode" && !args.empty()) im = static_cast<std::uint8_t>(args[0]);
        else return false;
        return true;
    });
    // what the description has no instruction for but the CPU executes: a DD or FD in front of an instruction that does not use IX or IY is
    // ignored (the instruction follows), and most of the ED page is two-byte NOPs
    emu.onBadInstruction([&](sl::Emulator& self, std::uint64_t at) {
        const std::uint8_t first = shadow[at & 0xFFFF], second = shadow[(at + 1) & 0xFFFF];
        if ((first == 0xDD || first == 0xFD) && second != 0xCB) {   // (DD CB d op with no description: the undocumented copies into a register)
            self.setPc((at + 1) & 0xFFFF);
            return true;
        }
        const bool block_gap = second >= 0xA0 && second < 0xC0 && (second & 4) != 0;   // A4-A7 AC-AF B4-B7 BC-BF
        if (first == 0xED && (second < 0x40 || (second >= 0x80 && second < 0xA0) || block_gap || second >= 0xC0 || second == 0x77 || second == 0x7F)) {
            self.setPc((at + 2) & 0xFFFF);
            return true;
        }
        return false;
    });
    struct Unhook {
        sl::Emulator& e;
        ~Unhook() {
            e.onRead({});
            e.onWrite({});
            e.onUserOp({});
            e.onBadInstruction({});
        }
    } unhook{emu};

    const z::RegisterInit& init = options.regs;
    const auto byte = [](const std::optional<std::uint8_t>& v) -> std::uint64_t { return v.value_or(0); };
    emu.setReg("A", byte(init.a));
    emu.setReg("F", byte(init.f));
    emu.setReg("B", byte(init.b));
    emu.setReg("C", byte(init.c));
    emu.setReg("D", byte(init.d));
    emu.setReg("E", byte(init.e));
    emu.setReg("H", byte(init.h));
    emu.setReg("L", byte(init.l));
    for (const char* name : {"A_", "F_", "B_", "C_", "D_", "E_", "H_", "L_", "I", "R"}) emu.setReg(name, 0);
    emu.setReg("IX", init.ix.value_or(0xFFFF));
    emu.setReg("IY", init.iy.value_or(0xFFFF));
    // the call as CALL makes it: the return address pushed through the bus like any write
    const std::uint16_t sp = static_cast<std::uint16_t>(options.sp - 2);
    bus.write(sp, static_cast<std::uint8_t>(z::kReturnAddress));
    bus.write(static_cast<std::uint16_t>(sp + 1), static_cast<std::uint8_t>(z::kReturnAddress >> 8));
    emu.setReg("SP", sp);
    emu.setPc(entry);

    const auto sync = [&] {
        if (std::memcmp(shadow.data(), memory.data(), kSize) == 0) return;
        std::uint32_t first = 0, last = kSize - 1;
        while (shadow[first] == memory[first]) ++first;
        while (shadow[last] == memory[last]) --last;
        std::memcpy(shadow.data() + first, memory.data() + first, last - first + 1);
        emu.write(first, shadow.data() + first, last - first + 1);
    };

    // R counts the instruction fetches: its low seven bits, two for a prefixed instruction (the description has no fetch count of its own)
    emu.setCounter(emu.regRef("R"), 0x7F, [&shadow](std::uint64_t at) -> std::uint64_t {
        const std::uint8_t first = shadow[at & 0xFFFF];
        return (first == 0xDD || first == 0xFD || first == 0xCB || first == 0xED) ? 2u : 1u;
    });
    struct Uncount {
        sl::Emulator& e;
        ~Uncount() { e.clearCounter(); }
    } uncount{emu};

    z::RunResult out;
    out.end = z::End::Limit;
    std::uint64_t steps = 0;
    while (steps < options.max_steps) {
        if (emu.pc() == z::kReturnAddress) {
            out.end = z::End::Return;
            break;
        }
        const std::uint16_t pc = static_cast<std::uint16_t>(emu.pc());
        if (options.before_step) options.before_step(pc);
        if (halted) {   // an earlier HALT ran: nothing runs any more
            out.end = z::End::Stopped;
            out.fault = z::Fault{z::Fault::Kind::Halt, pc};
            break;
        }
        touched = false;
        // (a run of instructions at once, R counted for each by the emulator; it ends where a device was touched or a HALT ran, which is where this looks; an instruction at a
        // time when the caller wants to see every pc)
        const sl::Emulator::BlockResult ran = emu.runBlocks(options.before_step ? 1 : options.max_steps - steps, true, z::kReturnAddress);
        steps += ran.executed;
        if (ran.status == sl::Emulator::Status::BadInstruction || ran.status == sl::Emulator::Status::Error) {
            out.end = z::End::Stopped;
            out.fault = z::Fault{z::Fault::Kind::Undecodable, pc};
            break;
        }
        if (touched) sync();
    }
    z::Registers& r = out.regs;
    r.a = static_cast<std::uint8_t>(emu.reg("A"));
    r.f = static_cast<std::uint8_t>(emu.reg("F"));
    r.b = static_cast<std::uint8_t>(emu.reg("B"));
    r.c = static_cast<std::uint8_t>(emu.reg("C"));
    r.d = static_cast<std::uint8_t>(emu.reg("D"));
    r.e = static_cast<std::uint8_t>(emu.reg("E"));
    r.h = static_cast<std::uint8_t>(emu.reg("H"));
    r.l = static_cast<std::uint8_t>(emu.reg("L"));
    r.a2 = static_cast<std::uint8_t>(emu.reg("A_"));
    r.f2 = static_cast<std::uint8_t>(emu.reg("F_"));
    r.b2 = static_cast<std::uint8_t>(emu.reg("B_"));
    r.c2 = static_cast<std::uint8_t>(emu.reg("C_"));
    r.d2 = static_cast<std::uint8_t>(emu.reg("D_"));
    r.e2 = static_cast<std::uint8_t>(emu.reg("E_"));
    r.h2 = static_cast<std::uint8_t>(emu.reg("H_"));
    r.l2 = static_cast<std::uint8_t>(emu.reg("L_"));
    r.ix = static_cast<std::uint16_t>(emu.reg("IX"));
    r.iy = static_cast<std::uint16_t>(emu.reg("IY"));
    r.sp = static_cast<std::uint16_t>(emu.reg("SP"));
    r.pc = static_cast<std::uint16_t>(emu.pc());
    r.i = static_cast<std::uint8_t>(emu.reg("I"));
    r.r = static_cast<std::uint8_t>(emu.reg("R"));
    r.iff1 = iff1;
    r.iff2 = iff2;
    r.im = im;
    r.halted = halted;
    out.steps = steps;
    out.io.assign(bus.log().begin() + static_cast<std::ptrdiff_t>(log_start), bus.log().end());
    return out;
}

z::RunResult call(z::Bus& bus, std::uint16_t entry, const z::RunOptions& options) { return enabled() ? pcode_cpu::run(bus, entry, options) : z::run(bus, entry, options); }

}  // namespace cb::engine::pcode_cpu
