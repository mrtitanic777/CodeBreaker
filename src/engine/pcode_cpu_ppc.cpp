// engine/pcode_cpu_ppc.cpp -- the PowerPC lanes' (the GameCube, Wii and Wii U's Gekko family, the PlayStation 3) function runs on the one p-code emulator:
// emulate::powerpc::run's call, on the description languages/ppc_64_isa_altivec_be.sla.
//
// The machine stays the interpreter's: a Machine holds the memory (pages made on first touch, the shared image and the fill under them, the devices and their
// log, the record of what was written) and every data access of the program goes through Machine::read and Machine::write. What the emulator carries is the
// instruction set: the integer, branch and condition-register instructions run on the description, the registers kept in it while they do. What the description
// does not carry for these targets runs as the interpreter runs it, on the registers as they stand (copied to the Machine and back): the floating point and
// paired singles (the floating registers live in the Machine only), the vector instructions, the system calls and traps, the reservations, the cache and
// special-purpose-register instructions, and the arithmetic with overflow recording. A run that wants calls to outside functions, a path, a branch log or a
// shadow is the lane judge's, and runs the interpreter.
#include <algorithm>
#include <array>
#include <cstring>
#include <string>
#include <unordered_map>

#include "decompile/engine/pcode_emulator.hh"
#include "engine/pcode_cpu.hpp"
#include "engine/pcode_cpu_internal.hpp"

namespace cb::engine::pcode_cpu {

namespace pw = cb::emulate::powerpc;
namespace sl = cb::decompile::sleigh;

namespace {

constexpr std::uint32_t kPpcPage = 0x1000;

struct PpcCodePage {
    std::array<std::uint8_t, kPpcPage> bytes{};
    std::uint32_t run = 0;
};

struct PpcRegs {
    std::array<sl::Emulator::RegRef, 32> gpr;
    std::array<sl::Emulator::RegRef, 8> cr;
    sl::Emulator::RegRef lr, ctr, ca;
};

struct PpcMirror {
    std::unordered_map<std::uint32_t, PpcCodePage> pages;
    std::uint32_t run = 0;
    bool configured = false;
    PpcRegs regs;
};

PpcMirror& ppc_mirror(const void* emulator) {
    thread_local std::unordered_map<const void*, PpcMirror> mirrors;
    return mirrors[emulator];
}

const char* const kGpr[32] = {"r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7", "r8", "r9", "r10", "r11", "r12", "r13", "r14", "r15",
                              "r16", "r17", "r18", "r19", "r20", "r21", "r22", "r23", "r24", "r25", "r26", "r27", "r28", "r29", "r30", "r31"};
const char* const kCrField[8] = {"cr0", "cr1", "cr2", "cr3", "cr4", "cr5", "cr6", "cr7"};

// What the interpreter has no instruction for (it stops with `unsupported`): found by trying a word of each major opcode and each extended opcode of the two
// groups on an empty Machine. The description may know more (a reserved or vector instruction); those words are the interpreter's too, so both stop alike.
struct Unsupported {
    std::array<bool, 64> major{};
    std::array<bool, 1024> group19{}, group31{};
    Unsupported() {
        const auto probe = [](std::uint32_t word) {
            pw::Machine m;
            m.pc = 0x10000000;
            for (unsigned i = 0; i < 32; ++i) m.r[i] = 0x60000000 + i * 16;
            try {
                pw::step(m, word);
            } catch (const pw::Fault& f) {
                return f.reason() == pw::StopReason::Unsupported;
            }
            return false;
        };
        constexpr std::uint32_t kFields = (1u << 21) | (2u << 16) | (3u << 11);
        for (unsigned op = 0; op < 64; ++op) major[op] = probe((op << 26) | kFields | 4);
        for (unsigned xo = 0; xo < 1024; ++xo) {
            group19[xo] = probe((19u << 26) | kFields | (xo << 1));
            group31[xo] = probe((31u << 26) | kFields | (xo << 1));
        }
    }
};

// Whether the interpreter runs the word (see the file's head).
bool interpreter_runs(std::uint32_t w) {
    static const Unsupported unsupported;
    const unsigned op = w >> 26;
    if (op == 30) return true;   // the doubleword rotates: the interpreter's
    if (op == 46 || op == 47) return true;   // lmw, stmw: a fault part way through leaves the registers and memory the interpreter left
    if (op == 16 || (op == 19 && ((w >> 1) & 0x3FF) == 16)) {
        if (((w >> 21) & 4) == 0) return true;   // a branch that counts CTR down: the interpreter counts the 64 bits, the description the low word
    }
    if (op == 31) {
        const unsigned x = (w >> 1) & 0x3FF;
        if ((x >> 1) == 413 || (x & 0x1FF) == 457 || (x & 0x1FF) == 489 || (x & 0x1FF) == 9 || (x & 0x1FF) == 73 || (x & 0x1FF) == 233 || (x & 0x1FF) == 491 || (x & 0x1FF) == 459) return true;   // sradi, the doubleword divides and multiplies, divw and divwu (the upper word is undefined: the interpreter clears it)
    }
    if (((w >> 16) & 31) == 0 && (op == 33 || op == 35 || op == 37 || op == 39 || op == 41 || op == 43 || op == 45 || ((op == 58 || op == 62) && (w & 3) == 1)))
        return true;   // an update form on r0 (invalid): the interpreter writes r0 with the address, the description does not
    if ((op == 62 && (w & 3) == 2) || (op == 58 && (w & 3) == 3)) return true;   // stq, lq: the description's quadword instructions, the interpreter's std and ld
    if (op != 19 && op != 31 && unsupported.major[op]) return true;
    if (op == 19 && unsupported.group19[(w >> 1) & 0x3FF]) return true;
    if (op == 31 && unsupported.group31[(w >> 1) & 0x3FF]) return true;
    if (op == 4 || op == 2 || op == 3 || op == 17 || (op >= 48 && op <= 57) || op == 59 || op == 60 || op == 61 || op == 63) return true;   // vector and paired, td/tw immediates, sc, floating point
    if (op == 19) {
        const unsigned xo = (w >> 1) & 0x3FF;
        return xo == 150;   // isync
    }
    if (op != 31) return false;
    const unsigned xo = (w >> 1) & 0x3FF;
    switch (xo) {
        case 4: case 68:                                              // tw, td
        case 20: case 84: case 150: case 214:                         // the reservations
        case 54: case 86: case 246: case 278: case 470: case 982: case 1014: case 598: case 854: case 83: case 146: case 371: case 310: case 438:   // cache, sync, msr, time base
        case 533: case 597: case 661: case 725:                       // the string instructions
        case 535: case 567: case 599: case 631: case 663: case 695: case 727: case 759: case 983:   // floating loads and stores
        case 6: case 7: case 38: case 39: case 71: case 103: case 135: case 167: case 199: case 231: case 359: case 487:   // vector loads and stores
            return true;
        case 339: case 467: {   // mfspr, mtspr: LR, CTR and XER the description keeps
            const unsigned spr = ((w >> 16) & 31) | (((w >> 11) & 31) << 5);
            return spr != 8 && spr != 9;
        }
        default: break;
    }
    // the arithmetic with overflow recording (OE): the interpreter never sets XER[OV]
    if ((w & 0x400) != 0) {
        switch (xo & 0x1FF) {   // the OE forms' extended opcodes (the 9-bit field)
            case 8: case 10: case 40: case 104: case 136: case 138: case 200: case 202: case 232: case 234: case 235: case 266: case 459: case 491: case 457: case 489: return true;
            default: break;
        }
    }
    return false;
}

}  // namespace

pw::RunResult run(std::span<const pw::Region> regions, std::uint32_t entry, std::span<const std::uint64_t> args, const pw::RunOptions& options) {
    sl::Emulator& emu = emulator_named("ppc_64_isa_altivec_be.sla");
    PpcMirror& mir = ppc_mirror(&emu);
    if (!mir.configured) {
        for (unsigned i = 0; i < 32; ++i) mir.regs.gpr[i] = emu.regRef(kGpr[i]);
        for (unsigned i = 0; i < 8; ++i) mir.regs.cr[i] = emu.regRef(kCrField[i]);
        mir.regs.lr = emu.regRef("LR");
        mir.regs.ctr = emu.regRef("CTR");
        mir.regs.ca = emu.regRef("xer_ca");
        emu.keepAddressContext(false);
        mir.configured = true;
    }
    ++mir.run;

    auto machine = std::make_shared<pw::Machine>(regions, options.shared);
    pw::Machine& m = *machine;
    if (options.fill != pw::Fill::None) m.set_under(pw::fill_image(options.fill));
    m.paired_singles = options.paired_singles;
    m.gqr = options.gqr;
    m.set_devices(options.devices);
    for (std::size_t i = 0; i < args.size() && i < 8; ++i) m.r[3 + i] = args[i];
    for (std::size_t i = 0; i < options.fargs.size() && i < 13; ++i) m.f[1 + i] = pw::to_bits(options.fargs[i]);
    for (unsigned i = 14; i < 32; ++i) m.r[i] = pw::kSavedRegisterBase + i;
    m.r[1] = pw::kEntryStackPointer;
    m.r[13] = options.r13.value_or(0x7E000000);
    m.r[2] = options.r2.value_or(0);
    m.lr = pw::kReturnAddress;
    m.pc = entry;
    const auto saved_at_entry = [&] {
        std::array<std::uint64_t, 19> s;
        for (unsigned i = 14; i < 32; ++i) s[i - 14] = m.r[i];
        s[18] = m.r[1];
        return s;
    };
    const auto entry_saved = saved_at_entry();

    // ---- the registers: in the emulator while the description runs, in the Machine while the interpreter does
    const PpcRegs& regs = mir.regs;   // (found by name once: a hand-over to the interpreter happens for every floating point instruction)
    const auto to_emulator = [&]() {
        for (unsigned i = 0; i < 32; ++i) emu.setReg(regs.gpr[i], m.r[i]);
        for (unsigned i = 0; i < 8; ++i) emu.setReg(regs.cr[i], (m.cr >> (28 - 4 * i)) & 0xF);
        emu.setReg(regs.lr, m.lr);
        emu.setReg(regs.ctr, m.ctr);
        emu.setReg(regs.ca, m.ca ? 1 : 0);
        emu.setPc(m.pc);
    };
    const auto to_machine = [&]() {
        for (unsigned i = 0; i < 32; ++i) m.r[i] = emu.reg(regs.gpr[i]);
        std::uint32_t cr = 0;
        for (unsigned i = 0; i < 8; ++i) cr |= (static_cast<std::uint32_t>(emu.reg(regs.cr[i])) & 0xF) << (28 - 4 * i);
        m.cr = cr;
        m.lr = static_cast<std::uint32_t>(emu.reg(regs.lr));
        m.ctr = emu.reg(regs.ctr);
        m.ca = (emu.reg(regs.ca) & 1) != 0;
        m.pc = static_cast<std::uint32_t>(emu.pc());
    };
    for (const char* name : {"xer_so", "xer_ov", "xer_ov32", "xer_ca32", "XER"}) emu.setReg(name, 0);
    to_emulator();

    // A page of code is read from the Machine the first time an instruction is fetched from it in a run (and handed to the emulator when it differs from what the
    // emulator holds); what the program writes to a page already read is written to the emulator too.
    const auto load_page = [&](std::uint32_t address) {
        const std::uint32_t base = address & ~(kPpcPage - 1);
        PpcCodePage& page = mir.pages[base];
        if (page.run == mir.run) return;
        page.run = mir.run;
        std::array<std::uint8_t, kPpcPage> now{};
        if (const pw::Page* p = m.peek(base)) std::memcpy(now.data(), p->data(), kPpcPage);
        if (page.bytes != now) {
            page.bytes = now;
            emu.write(base, now.data(), kPpcPage);
        }
    };
    std::optional<pw::StopReason> hook_fault;
    emu.onRead([&](std::string_view space, std::uint64_t addr, unsigned size, std::uint64_t& value) {
        if (!is_ram(space)) return false;
        try {
            value = m.read(static_cast<std::uint32_t>(addr), size);
        } catch (const pw::Fault& f) {
            if (!hook_fault) hook_fault = f.reason();
            value = 0;
        }
        return true;
    });
    emu.onWrite([&](std::string_view space, std::uint64_t addr, unsigned size, std::uint64_t value) {
        if (!is_ram(space)) return false;
        const std::uint32_t a = static_cast<std::uint32_t>(addr);
        try {
            m.write(a, size, value);
        } catch (const pw::Fault& f) {
            if (!hook_fault) hook_fault = f.reason();
            return true;
        }
        for (std::uint64_t at64 = a; at64 < std::uint64_t{a} + size; at64 += kPpcPage - (at64 & (kPpcPage - 1))) {   // (the program may run what it wrote; addresses wrap at 4 GB)
            const std::uint32_t at = static_cast<std::uint32_t>(at64);
            const auto page = mir.pages.find(at & ~(kPpcPage - 1));
            if (page == mir.pages.end() || page->second.run != mir.run) continue;
            if (const pw::Page* p = m.peek(at)) {
                std::memcpy(page->second.bytes.data(), p->data(), kPpcPage);
                emu.write(at & ~(kPpcPage - 1), p->data(), kPpcPage);
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

    pw::RunResult out;
    std::uint64_t count = 0;
    bool registers_in_machine = false;   // the interpreter faulted: what it left in the Machine is the state, the emulator's copy is from before the instruction
    const auto interpret = [&](std::uint32_t word) {
        to_machine();
        registers_in_machine = true;
        pw::step(m, word);
        registers_in_machine = false;
        to_emulator();
    };
    try {
        for (;;) {
            const std::uint32_t pc = static_cast<std::uint32_t>(emu.pc());
            if (pc == pw::kReturnAddress) {
                out.exit = pw::Exit::Ret;
                break;
            }
            if (count >= options.max_steps) {
                out.truncated = true;
                break;
            }
            ++count;
            if (pc & 3) throw pw::Fault(pw::StopReason::Alignment);
            m.pc = pc;
            const std::uint32_t w = static_cast<std::uint32_t>(m.read(pc, 4));
            if (w == 0) throw pw::Fault(pw::StopReason::Invalid);
            if (interpreter_runs(w)) {
                interpret(w);
                continue;
            }
            load_page(pc);
            hook_fault.reset();
            // a load or store that fails changes neither its target nor its base, and the pc stays on it: the description has written them by then
            const unsigned target = (w >> 21) & 31, base = (w >> 16) & 31;
            const std::uint64_t before_target = emu.reg(regs.gpr[target]), before_base = emu.reg(regs.gpr[base]);
            const sl::Emulator::Status status = emu.step();
            if (hook_fault) {
                emu.setReg(regs.gpr[target], before_target);
                emu.setReg(regs.gpr[base], before_base);
                emu.setPc(pc);
                throw pw::Fault(*hook_fault);
            }
            if (status == sl::Emulator::Status::BadInstruction) {   // (the description is stricter about reserved bits than the interpreter: nothing ran, it does)
                interpret(w);
                continue;
            }
            if (status != sl::Emulator::Status::Ok) throw pw::Fault(pw::StopReason::Unsupported);
            if (w >> 26 == 19 && (((w >> 1) & 0x3FF) == 16 || ((w >> 1) & 0x3FF) == 528)) {   // a branch to LR or CTR drops the low two bits and the upper word, as the interpreter does
                const std::uint64_t at = emu.pc();
                if ((at & 3) != 0 || at > 0xFFFFFFFFull) emu.setPc(static_cast<std::uint32_t>(at) & 0xFFFFFFFCu);
            }
        }
    } catch (const pw::Fault& fault) {
        out.exit = pw::Exit::Exception;
        out.stopped = fault.reason();
    }
    if (!registers_in_machine) to_machine();
    out.ret = m.r[3];
    out.fret = m.f[1];
    out.steps = count;
    out.pc = m.pc;
    for (const auto& [address, bytes] : m.writes()) {
        if (bytes.first != bytes.second) out.writes.push_back({address, bytes.first, bytes.second});
    }
    out.saved = saved_at_entry() == entry_saved;
    out.machine = std::move(machine);
    return out;
}

pw::RunResult call(std::span<const pw::Region> regions, std::uint32_t entry, std::span<const std::uint64_t> args, const pw::RunOptions& options) {
    const bool lane_run = options.externs == nullptr && !options.span && !options.branches && !options.absent && options.shadow == nullptr;   // (the judge's runs want more)
    return enabled() && lane_run ? pcode_cpu::run(regions, entry, args, options) : pw::run(regions, entry, args, options);
}

}  // namespace cb::engine::pcode_cpu
