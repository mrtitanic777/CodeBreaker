// cb-unity: alone (file-local names that other files in a batch also use)
// emulate/cpu/pcode_cpu_ppc.cpp -- the PowerPC lanes' (the GameCube, Wii and Wii U's Gekko family, the PlayStation 3) function runs on the one p-code emulator:
// emulate::powerpc::run's call, on the description languages/ppc_64_isa_altivec_be.sla (or ppc_64_xenon_be.sla for VMX128).
//
// The machine stays the interpreter's: a Machine holds the memory (pages made on first touch, the shared image and the fill under them, the devices and their log, the record of
// what was written) and every data access of the program goes through Machine::read and Machine::write. The registers, the program counter and the instruction fetch are the
// emulator's. Each word is run as one of the Kinds below: the description's own instruction for most (the integer, branch, condition register, load and store, floating point
// load and store and vector instructions, the vector operations it only names on the host functions of pcode_ops_ppc_vector.cpp), and the adapter's, on the emulator's registers,
// where the description has none or defines the instruction differently from the interpreter (which the lanes' results have always been made with): the floating arithmetic, the
// branches that count CTR, the doubleword rotates, the divides, the special registers, the traps and system calls, and Gekko's paired singles (pcode_ops_ppc.cpp). A word the
// interpreter has no instruction for stops the run with `unsupported`, as it does, except the vector instructions of a machine that has them. The lane judge's runs (calls out, the
// path, the branch log, absent code, a shadow) are made here too: the registers are copied to the Machine only where one of them looks at them.
#include <algorithm>
#include <array>
#include <bit>
#include <cmath>
#include <cstring>
#include <format>
#include <limits>
#include <memory>
#include <string>
#include <unordered_map>

#include "decompile/engine/pcode_emulator.hh"
#include "emulate/cpu/pcode_cpu.hpp"
#include "emulate/cpu/pcode_cpu_internal.hpp"
#include "emulate/powerpc_fpu.hpp"
#include "emulate/cpu/pcode_cpu_ppc.hpp"

namespace cb::emulate::pcode_cpu {

namespace pw = cb::emulate::powerpc;
namespace sl = cb::decompile::sleigh;
namespace po = cb::emulate::pcode_ops_ppc;
namespace fpu = cb::emulate::powerpc::fpu;

namespace {

constexpr std::uint32_t kPpcPage = 0x1000;

struct PpcCodePage {
    std::array<std::uint8_t, kPpcPage> bytes{};
    std::uint32_t run = 0;
};

struct Canonical {
    std::uint32_t clear = 0, set = 0;   // the reserved bits of a form, and the value they have
};

// One emulator and what is kept for it between runs (reading the description is the costly part of a run).
struct Core {
    sl::Emulator* emu = nullptr;
    po::Regs regs;
    po::VectorContext vector;
    pcode_ops::Binding binding;
    std::unordered_map<std::uint32_t, PpcCodePage> pages;
    std::unordered_map<std::uint32_t, Canonical> canonical;   // by canonical_key
    std::uint32_t run = 0;
};

Core& core_of(po::Flavor flavor) {
    thread_local std::array<std::unique_ptr<Core>, 2> cores;
    std::unique_ptr<Core>& slot = cores[static_cast<std::size_t>(flavor)];
    if (!slot) {
        slot = std::make_unique<Core>();
        slot->emu = &emulator_named(flavor == po::Flavor::Xenon ? "ppc_64_xenon_be.sla" : "ppc_64_isa_altivec_be.sla");
        slot->regs = po::find_regs(*slot->emu);
        slot->vector.regs = &slot->regs;
        slot->emu->keepAddressContext(false);
        slot->binding = po::bind(*slot->emu, flavor, slot->vector);
        Core* core = slot.get();
        slot->emu->onBadInstruction([core](sl::Emulator& e, std::uint64_t pc) { return po::serve_undecoded(e, core->regs, core->vector, pc); });
    }
    return *slot;
}

// A hook's refusal of an access (the Machine ran out of pages): it ends the instruction where it stands, as the interpreter's fault does.
struct HookAbort : std::runtime_error {
    HookAbort() : std::runtime_error("memory") {}
};

// How the adapter runs a word (the run loop's switch).
enum class Kind : std::uint8_t {
    Plain,       // the description's instruction, as it stands (what follows it is in the entry's `post`)
    Stop,        // an instruction the interpreter has none for (privileged, reserved, string, vector where the machine has none): the run stops with `unsupported`, as it does
    Trap,        // tw, td, twi, tdi: whether the test holds is the adapter's (a taken trap stops the run)
    Syscall,     // sc
    Paired,      // Gekko's paired singles and quantized loads and stores
    Branch,      // bc and bclr that count CTR down: the interpreter counts the 64 bits, the description the low word
    BranchCtr,   // bcctr: its BO field has no count bit (the description has no instruction when it is set; the interpreter takes it as clear)
    Rotate,      // the doubleword rotates: a mask whose start is past its end wraps, which the description does not do
    Divide,      // divw, divwu, divd, divdu: a zero divisor, the one overflowing quotient and the undefined upper word are the interpreter's (zero)
    Multiple,    // lmw, stmw
    Update,      // a load or store with update: on r0 (invalid) the interpreter writes the address to r0; lq and stq are its ld and std
    Spr,         // mfspr and mtspr: the special registers the interpreter keeps (LR, CTR, the carry of XER, Gekko's GQRs); every other reads zero and ignores the write
    Zero,        // mftb, mfmsr: they read zero
    MoveFromCr,  // mfcr (the interpreter ignores the reserved bits, the description reads one field there)
    Nop,         // the cache hints, sync, eieio, isync: nothing happens
    Dcbz,        // the 32-byte block is cleared
    StoreHalfElement,   // stvehx: the manual aligns the address to the halfword (the description stores at the address as it stands)
    Float,       // the floating arithmetic, moves and compares, as the interpreter does them (host doubles; see the case)
};

// What is done after an instruction of the description.
enum Post : std::uint8_t {
    kNoPost = 0,
    kSingle = 2,      // Gekko: a single loaded or computed fills both halves of the register
    kLinkTarget = 4,  // bclr, bcctr: the target's low two bits and upper word are dropped
    kCompareSet = 8,  // a vector compare with the record bit sets CR6
    kMemory = 32,     // a load or store: if the Machine refuses the access, its target and base register are put back
};

struct Entry {
    Kind kind = Kind::Plain;
    std::uint8_t post = 0;
};

// Whether the interpreter (emulate/powerpc) has an instruction for the word, as its step() dispatches it. The tests compare this with the interpreter itself for every opcode.
bool interpreter_handles(std::uint32_t w, bool paired) {
    const unsigned op = w >> 26;
    const unsigned xo10 = (w >> 1) & 0x3FF, xo5 = (w >> 1) & 31, xo9 = xo10 & 0x1FF;
    switch (op) {
        case 2: case 3: case 7: case 8: case 10: case 11: case 12: case 13: case 14: case 15: case 16: case 17: case 18: case 20: case 21: case 23:
        case 24: case 25: case 26: case 27: case 28: case 29: case 58: case 62:
            return true;
        case 30: return ((w >> 2) & 7) < 4 || ((w >> 1) & 0xF) == 8 || ((w >> 1) & 0xF) == 9;   // (the doubleword rotates: rldicl rldicr rldic rldimi rldcl rldcr)
        case 32: case 33: case 34: case 35: case 36: case 37: case 38: case 39: case 40: case 41: case 42: case 43: case 44: case 45: case 46: case 47:
        case 48: case 49: case 50: case 51: case 52: case 53: case 54: case 55:
            return true;
        case 56: case 57: case 60: case 61: return paired;
        case 4:
            if (!paired) return false;
            switch (xo10 & 0x3F) {   // psq_lx, psq_stx, psq_lux, psq_stux
                case 6: case 7: case 38: case 39: return true;
                default: break;
            }
            switch (xo10) {   // the compares, moves and merges, dcbz_l
                case 0: case 32: case 64: case 96: case 40: case 72: case 136: case 264: case 528: case 560: case 592: case 624: case 1014: return true;
                default: break;
            }
            switch (xo5) {
                case 18: case 20: case 21: case 23: case 24: case 25: case 26: case 28: case 29: case 30: case 31: case 10: case 11: case 12: case 13: case 14: case 15: return true;
                default: return false;
            }
        case 19:
            switch (xo10) {
                case 16: case 528: case 0: case 257: case 449: case 193: case 225: case 33: case 289: case 129: case 417: case 150: return true;
                default: return false;
            }
        case 59:
            switch (xo5) {
                case 18: case 20: case 21: case 22: case 24: case 25: case 26: case 28: case 29: case 30: case 31: return true;
                default: return false;
            }
        case 63:
            switch (xo5) {
                case 18: case 20: case 21: case 22: case 23: case 24: case 25: case 26: case 28: case 29: case 30: case 31: return true;
                default: break;
            }
            switch (xo10) {
                case 72: case 40: case 264: case 136: case 12: case 0: case 32: case 14: case 15: case 814: case 815: case 846: case 583: case 711: case 134: case 70: case 38: case 64: return true;
                default: return false;
            }
        case 31:
            switch (xo10) {
                case 23: case 55: case 87: case 119: case 279: case 311: case 343: case 375: case 341: case 373: case 21: case 53: case 534: case 790: case 20: case 84:   // loads
                case 151: case 183: case 215: case 247: case 407: case 439: case 149: case 181: case 662: case 918:                                                    // stores
                case 28: case 60: case 444: case 412: case 316: case 124: case 476: case 284:                                                                        // logical
                case 150: case 214: case 0: case 32: case 339: case 467: case 19: case 144: case 371: case 83:
                case 278: case 246: case 86: case 54: case 598: case 982: case 854: case 1014:
                case 535: case 567: case 599: case 631: case 663: case 695: case 727: case 759: case 983:
                case 4: case 68:
                case 24: case 536: case 792: case 824: case 27: case 539: case 794: case 826: case 827: case 26: case 58: case 954: case 922: case 986:
                    return true;
                default: break;
            }
            switch (xo9) {   // the arithmetic (and its OE forms)
                case 266: case 40: case 10: case 8: case 138: case 136: case 202: case 200: case 234: case 232: case 104: case 235: case 75: case 11: case 233: case 73: case 9:
                case 491: case 459: case 489: case 457:
                    return true;
                default: return false;
            }
        default: return false;
    }
}

// The vector instructions: AltiVec's and VMX128's opcodes, and the loads and stores of the vector registers in opcode 31.
bool is_vector_word(std::uint32_t w) {
    const unsigned op = w >> 26;
    if (op == 4 || op == 5 || op == 6) return true;
    if (op != 31) return false;
    switch ((w >> 1) & 0x3FF) {
        case 6: case 7: case 38: case 39: case 71: case 103: case 135: case 167: case 199: case 231: case 359: case 487:   // lvsl, lvsr, lvebx ... stvx, lvxl, stvxl
        case 342: case 374: case 406: case 438: case 822:                                                                 // dst, dstst, dstt, dststt, dss
        case 519: case 551: case 647: case 679: case 775: case 807: case 903: case 935:                                   // VMX128's lvlx, lvrx, stvlx, stvrx and their LRU forms
            return true;
        default: return false;
    }
}

// What each (opcode, extended opcode) is run as. Built once for each of the two machine kinds (paired singles on or off).
Entry entry_of(std::uint32_t w, bool paired) {
    const unsigned op = w >> 26;
    const unsigned xo10 = (w >> 1) & 0x3FF, xo9 = xo10 & 0x1FF;
    Entry e;
    if ((op >= 32 && op <= 57) || op == 58 || op == 60 || op == 61 || op == 62) e.post = kMemory;
    if (op == 31) {
        switch (xo10) {   // the indexed loads and stores (the integer ones: a vector or floating register is no register to put back)
            case 23: case 55: case 87: case 119: case 279: case 311: case 343: case 375: case 341: case 373: case 21: case 53: case 534: case 790: case 20: case 84:
            case 151: case 183: case 215: case 247: case 407: case 439: case 149: case 181: case 662: case 918: case 150: case 214: e.post = kMemory; break;
            default: break;
        }
    }
    if (!interpreter_handles(w, paired)) {
        e.kind = (!paired && is_vector_word(w)) ? Kind::Plain : Kind::Stop;
        if (e.kind == Kind::Plain && op == 31 && xo10 == 167) e.kind = Kind::StoreHalfElement;
        if (e.kind == Kind::Plain && op == 4) e.post |= kCompareSet;
        return e;
    }
    switch (op) {
        case 2: case 3: e.kind = Kind::Trap; break;
        case 17: e.kind = Kind::Syscall; break;
        case 16: e.kind = Kind::Branch; break;   // (the word decides: see the run loop)
        case 30: e.kind = Kind::Rotate; break;
        case 46: case 47: e.kind = Kind::Multiple; break;
        case 4: case 56: case 57: case 60: case 61: e.kind = Kind::Paired; break;
        case 33: case 35: case 37: case 39: case 41: case 43: case 45: case 51: case 53: case 55: e.kind = Kind::Update; break;
        case 49: e.kind = Kind::Update; e.post |= kSingle; break;
        case 58: case 62: e.kind = Kind::Update; break;   // (ldu, stdu, lq, stq: by the low bits)
        case 48:
            e.post |= kSingle;
            break;
        case 59: e.kind = Kind::Float; break;
        case 63: e.kind = Kind::Float; break;
        case 19:
            if (xo10 == 16) e.kind = Kind::Branch, e.post |= kLinkTarget;
            if (xo10 == 528) e.kind = Kind::BranchCtr, e.post |= kLinkTarget;
            if (xo10 == 150) e.kind = Kind::Nop;
            break;
        case 31:
            switch (xo10) {
                case 4: case 68: e.kind = Kind::Trap; return e;
                case 339: case 467: e.kind = Kind::Spr; return e;
                case 371: case 83: e.kind = Kind::Zero; return e;
                case 19: e.kind = Kind::MoveFromCr; return e;
                case 278: case 246: case 86: case 54: case 598: case 982: case 854: e.kind = Kind::Nop; return e;
                case 1014: e.kind = Kind::Dcbz; return e;
                case 55: case 119: case 311: case 375: case 373: case 53: case 183: case 247: case 439: case 181: case 631: case 695: case 759: e.kind = Kind::Update; break;
                case 567: e.kind = Kind::Update; e.post |= kSingle; break;
                case 535: e.post |= kSingle; break;
                default: break;
            }
            if (xo9 == 459 || xo9 == 491 || xo9 == 457 || xo9 == 489) e.kind = Kind::Divide;
            break;
        default: break;
    }
    return e;
}

// The table: opcode, extended opcode (the 10 bits) -> what to do.
struct Table {
    std::array<Entry, 64 * 1024> entries;
    Table(bool paired) {
        for (unsigned op = 0; op < 64; ++op)
            for (unsigned xo = 0; xo < 1024; ++xo) entries[(op << 10) | xo] = entry_of((op << 26) | (xo << 1), paired);
    }
};

const Entry& lookup(std::uint32_t w, bool paired) {
    static const Table tables[2] = {Table(false), Table(true)};
    return tables[paired ? 1 : 0].entries[((w >> 26) << 10) | ((w >> 1) & 0x3FF)];
}

constexpr std::int64_t s64(std::uint64_t v) { return static_cast<std::int64_t>(v); }
constexpr std::int64_t s32(std::uint64_t v) { return static_cast<std::int32_t>(static_cast<std::uint32_t>(v)); }

bool trap_taken(unsigned to, std::int64_t a, std::int64_t b, std::uint64_t ua, std::uint64_t ub) {
    return ((to & 16) && a < b) || ((to & 8) && a > b) || ((to & 4) && a == b) || ((to & 2) && ua < ub) || ((to & 1) && ua > ub);
}

// ---- the fields a form leaves reserved ----------------------------------------------------------------------------------------------------------------------------------

// A word the description has no instruction for may still be the interpreter's: it ignores the fields a form leaves reserved (a compare with its reserved bit set, an `isync` with
// operands). Those bits are learned from the description when first wanted: of the valid words of the word's (opcode, extended opcode), the bits whose turning over makes every
// one invalid, and the value they must have. The word with those bits as the form wants them is what the interpreter ran.
bool word_decodes(sl::Emulator& emu, std::uint32_t word) {
    constexpr std::uint64_t kScratch = 0xFFFFFF00ull;   // (an address no program runs at)
    const std::uint8_t bytes[4] = {static_cast<std::uint8_t>(word >> 24), static_cast<std::uint8_t>(word >> 16), static_cast<std::uint8_t>(word >> 8), static_cast<std::uint8_t>(word)};
    emu.write(kScratch, bytes, 4);
    return emu.length(kScratch) == 4;
}

bool has_extended_opcode(unsigned op) { return op == 4 || op == 19 || op == 31 || op == 59 || op == 63 || op == 30 || op == 58 || op == 62; }

std::uint32_t canonical_key(std::uint32_t w) { return ((w >> 26) << 10) | (has_extended_opcode(w >> 26) ? (w >> 1) & 0x3FF : 0); }

Canonical learn_canonical(sl::Emulator& emu, std::uint32_t key) {
    const unsigned op = key >> 10, xo = key & 0x3FF;
    std::uint32_t state = key * 2654435761u + 12345u;
    const auto next = [&] {
        state ^= state << 13;
        state ^= state >> 17;
        state ^= state << 5;
        return state;
    };
    std::vector<unsigned> bits = {0};
    for (unsigned b = 11; b <= 25; ++b) bits.push_back(b);
    std::vector<std::uint32_t> valid;
    for (unsigned attempt = 0; attempt < 80 && valid.size() < 8; ++attempt) {
        std::uint32_t w = (op << 26) | (xo << 1) | (next() & 0x03FFF800u) | (next() & 1);
        if (!has_extended_opcode(op)) w |= next() & 0x7FE;
        if (!word_decodes(emu, w)) {   // the operand bits cleared one at a time until it is an instruction
            for (std::size_t i = bits.size() - 1; i > 0; --i) std::swap(bits[i], bits[next() % (i + 1)]);
            for (unsigned b : bits) {
                if (!((w >> b) & 1)) continue;
                w &= ~(1u << b);
                if (word_decodes(emu, w)) break;
            }
            if (!word_decodes(emu, w)) w |= 1;   // (some forms want the low bit)
        }
        if (word_decodes(emu, w)) valid.push_back(w);
    }
    Canonical c;
    if (valid.size() < 3) return c;
    for (unsigned b : bits) {
        bool reserved = true;
        const unsigned first = (valid[0] >> b) & 1;
        for (std::uint32_t v : valid) {
            if (((v >> b) & 1) != first || word_decodes(emu, v ^ (1u << b))) {
                reserved = false;
                break;
            }
        }
        if (reserved) {
            c.clear |= 1u << b;
            c.set |= first << b;
        }
    }
    return c;
}

}  // namespace

pw::RunResult run(po::Flavor flavor, std::span<const pw::Region> regions, std::uint32_t entry, std::span<const std::uint64_t> args, const pw::RunOptions& options) {
    Core& core = core_of(flavor);
    sl::Emulator& emu = *core.emu;
    const po::Regs& regs = core.regs;
    ++core.run;

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

    // ---- the registers: in the emulator while the run goes, copied to the Machine where something looks at them
    const auto to_emulator = [&]() {
        for (unsigned i = 0; i < 32; ++i) {
            emu.setReg(regs.gpr[i], m.r[i]);
            emu.setReg(regs.fpr[i], m.f[i]);
        }
        for (unsigned i = 0; i < 8; ++i) emu.setReg(regs.cr[i], (m.cr >> (28 - 4 * i)) & 0xF);
        emu.setReg(regs.lr, m.lr);
        emu.setReg(regs.ctr, m.ctr);
        emu.setReg(regs.ca, m.ca ? 1 : 0);
        emu.setReg(regs.so, m.so ? 1 : 0);
        emu.setReg(regs.ov, m.ov ? 1 : 0);
        emu.setPc(m.pc);
    };
    const auto to_machine = [&]() {
        for (unsigned i = 0; i < 32; ++i) {
            m.r[i] = emu.reg(regs.gpr[i]);
            m.f[i] = emu.reg(regs.fpr[i]);
        }
        std::uint32_t cr = 0;
        for (unsigned i = 0; i < 8; ++i) cr |= (static_cast<std::uint32_t>(emu.reg(regs.cr[i])) & 0xF) << (28 - 4 * i);
        m.cr = cr;
        m.lr = static_cast<std::uint32_t>(emu.reg(regs.lr));
        m.ctr = emu.reg(regs.ctr);
        m.ca = (emu.reg(regs.ca) & 1) != 0;
        m.so = (emu.reg(regs.so) & 1) != 0;
        m.ov = (emu.reg(regs.ov) & 1) != 0;
        m.pc = static_cast<std::uint32_t>(emu.pc());
    };
    {   // everything a run does not set starts from zero (the emulator is the thread's and was used before)
        std::array<std::uint8_t, 16> zero{};
        for (unsigned i = 0; i < 32; ++i) emu.writeRegWide(regs.vr[i], zero.data());
        for (const sl::Emulator::RegRef& r : regs.status) emu.setReg(r, 0);
        for (const sl::Emulator::RegRef& r : {regs.so, regs.ov, regs.ov32, regs.ca32, regs.count, regs.xer, regs.reserve, regs.reserve_address, regs.reserve_length, regs.vrsave, regs.msr})
            emu.setReg(r, 0);
        core.vector.word = 0;
        core.vector.vscr = 0;
        for (const sl::Emulator::RegRef& r : regs.vr_wide) emu.writeRegWide(r, zero.data());
    }
    to_emulator();

    // A page of code is read from the Machine the first time an instruction is fetched from it in a run (and handed to the emulator when it differs from what the
    // emulator holds); what the program writes to a page already read is written to the emulator too.
    std::uint32_t last_base = 1;   // (the page last made current: not an address of a page)
    const auto load_page = [&](std::uint32_t address) {
        const std::uint32_t base = address & ~(kPpcPage - 1);
        if (base == last_base) return;
        last_base = base;
        PpcCodePage& page = core.pages[base];
        if (page.run == core.run) return;
        page.run = core.run;
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
            throw HookAbort{};
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
            throw HookAbort{};
        }
        for (std::uint64_t at64 = a; at64 < std::uint64_t{a} + size; at64 += kPpcPage - (at64 & (kPpcPage - 1))) {   // (the program may run what it wrote; addresses wrap at 4 GB)
            const std::uint32_t at = static_cast<std::uint32_t>(at64);
            const auto page = core.pages.find(at & ~(kPpcPage - 1));
            if (page == core.pages.end() || page->second.run != core.run) continue;
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
    const pw::AddressRange span = options.span.value_or(pw::AddressRange{0, 0});
    std::optional<pw::CompareTracker> tracker;
    if (options.branches) tracker.emplace();
    std::vector<bool> seen;
    if (span.hi > span.lo) seen.assign((span.hi - span.lo) / 4 + 1, false);
    std::optional<std::uint32_t> last, linked;   // the last instruction; where a call from it went

    const auto call_out = [&](std::string name) {   // a function outside what was given: record the call, return through lr
        pw::Call call;
        call.name = std::move(name);
        for (unsigned i = 0; i < 8; ++i) call.args[i] = emu.reg(regs.gpr[3 + i]);
        out.calls.push_back(std::move(call));
        emu.setReg(regs.gpr[3], options.stub_ret);
        emu.setPc(static_cast<std::uint32_t>(emu.reg(regs.lr)));
        if (options.shadow != nullptr) {
            to_machine();
            options.shadow->after_call(m, true, options.stub_ret);
        }
    };

    // One instruction of the description at `pc`, undone where the Machine refuses an access. False: the description has no instruction for the word (nothing was done).
    const auto step_description = [&](std::uint32_t pc, std::uint32_t w, bool memory) {
        load_page(pc);
        hook_fault.reset();
        // a load or store that fails changes neither its target nor its base, and the pc stays on it: the description may have written them by then
        const unsigned target = (w >> 21) & 31, base = (w >> 16) & 31;
        const std::uint64_t before_target = memory ? emu.reg(regs.gpr[target]) : 0, before_base = memory ? emu.reg(regs.gpr[base]) : 0;
        core.vector.word = w;
        const sl::Emulator::Status status = emu.step();
        if (hook_fault) {
            if (memory) {
                emu.setReg(regs.gpr[target], before_target);
                emu.setReg(regs.gpr[base], before_base);
            }
            emu.setPc(pc);
            throw pw::Fault(*hook_fault);
        }
        if (status == sl::Emulator::Status::BadInstruction) return false;
        if (status != sl::Emulator::Status::Ok) throw pw::Fault(pw::StopReason::Unsupported);
        return true;
    };

    // The word as another is run: the description knows no instruction for it as it stands (an update form on r0, lq and stq, a branch to CTR that counts, a word with its reserved
    // fields set), and the interpreter runs it as the other. The other word stands in the emulator's memory for the one instruction. False: the description has none for it either.
    const auto step_as = [&](std::uint32_t pc, std::uint32_t original, std::uint32_t rewritten, bool memory) {
        load_page(pc);
        if (rewritten == original) return step_description(pc, original, memory);
        const auto put = [&](std::uint32_t word) {
            const std::uint8_t bytes[4] = {static_cast<std::uint8_t>(word >> 24), static_cast<std::uint8_t>(word >> 16), static_cast<std::uint8_t>(word >> 8), static_cast<std::uint8_t>(word)};
            emu.write(pc, bytes, 4);
        };
        put(rewritten);
        bool ran = false;
        try {
            ran = step_description(pc, rewritten, memory);
        } catch (...) {
            put(original);
            throw;
        }
        put(original);
        return ran;
    };

    // `word` (the original, or a rewriting of it) as an instruction of the description, or with its reserved fields as the form wants them; or the run stops: the interpreter has the
    // instruction, the description does not.
    const auto run_word = [&](std::uint32_t pc, std::uint32_t original, std::uint32_t word, bool memory) {
        if (step_as(pc, original, word, memory)) return;
        if ((word >> 26) == 31 && (word & 0x400)) {   // (mulhdu, mulhwu, mulhd, mulhw have no OE form: the interpreter ignores the bit)
            const unsigned x = (word >> 1) & 0x1FF;
            if (x == 9 || x == 11 || x == 73 || x == 75) {
                if (step_as(pc, original, word & ~0x400u, memory)) return;
            }
        }
        const std::uint32_t key = canonical_key(word);
        auto found = core.canonical.find(key);
        if (found == core.canonical.end()) found = core.canonical.emplace(key, learn_canonical(emu, key)).first;
        const std::uint32_t canon = (word & ~found->second.clear) | found->second.set;
        if (canon == word || !step_as(pc, original, canon, memory)) throw pw::Fault(pw::StopReason::Unsupported);
    };

    try {
        for (;;) {
            const std::uint32_t pc = static_cast<std::uint32_t>(emu.pc());
            if (pc == pw::kReturnAddress) {
                out.exit = pw::Exit::Ret;
                break;
            }
            if (options.externs != nullptr) {
                if (const auto it = options.externs->find(pc); it != options.externs->end()) {
                    if (out.calls.size() >= pw::kMaxCalls) {
                        out.truncated = true;
                        break;
                    }
                    call_out(it->second);
                    continue;
                }
            }
            if (count >= options.max_steps) {
                out.truncated = true;
                break;
            }
            ++count;
            if (span.contains(pc)) {
                const std::size_t slot = (pc - span.lo) / 4;
                if ((pc & 3) == 0 && !seen[slot]) {
                    seen[slot] = true;
                    out.path.push_back(pc);
                } else if ((pc & 3) != 0 && std::find(out.path.begin(), out.path.end(), pc) == out.path.end()) {
                    out.path.push_back(pc);   // (an unaligned address: it fails the next check, but it ran)
                }
            }
            if (pc & 3) throw pw::Fault(pw::StopReason::Alignment);
            m.pc = pc;
            const std::uint32_t w = static_cast<std::uint32_t>(m.read(pc, 4));
            if (w == 0) {
                if (options.absent && options.absent->contains(pc) && linked == pc && last && static_cast<std::uint32_t>(emu.reg(regs.lr)) == *last + 4) {
                    // Called, and nothing is there: code outside what was given (a partial image calls into the rest of its title). A call out, like one to `externs`.
                    if (out.calls.size() >= pw::kMaxCalls) {
                        out.truncated = true;
                        break;
                    }
                    call_out(std::format("@{:08x}", pc));
                    continue;
                }
                throw pw::Fault(pw::StopReason::Invalid);
            }
            last = pc;
            linked.reset();
            if (options.shadow != nullptr) {
                to_machine();
                options.shadow->step(m, pc);
            }
            std::optional<pw::CompareTracker::Pending> pending;
            if (tracker && span.contains(pc)) {
                to_machine();
                pending = tracker->before(m, w);
            }
            const Entry& how = lookup(w, options.paired_singles);
            Kind kind = how.kind;
            const unsigned op = w >> 26;
            const unsigned xo10 = (w >> 1) & 0x3FF;
            const unsigned rt = (w >> 21) & 31, ra = (w >> 16) & 31, rb = (w >> 11) & 31;
            if (kind == Kind::Branch && (rt & 4) != 0) kind = Kind::Plain;   // (no count: the description's)
            const auto set_cr0 = [&](std::uint64_t v) { emu.setReg(regs.cr[0], (s64(v) < 0 ? 8 : s64(v) > 0 ? 4 : 2) | (emu.reg(regs.so) & 1)); };
            switch (kind) {
                case Kind::Plain: run_word(pc, w, w, (how.post & kMemory) != 0); break;
                case Kind::Stop: throw pw::Fault(pw::StopReason::Unsupported);
                case Kind::Syscall: throw pw::Fault(pw::StopReason::Syscall);
                case Kind::Trap: {
                    const bool wide = op == 2 || (op == 31 && xo10 == 68);
                    const std::uint64_t a = emu.reg(regs.gpr[ra]);
                    const std::uint64_t b = (op == 2 || op == 3) ? pw::sign_extend(w & 0xFFFF, 16) : emu.reg(regs.gpr[rb]);
                    const bool taken = wide ? trap_taken(rt, s64(a), s64(b), a, b) : trap_taken(rt, s32(a), s32(b), a & 0xFFFFFFFFull, b & 0xFFFFFFFFull);
                    if (taken) throw pw::Fault(pw::StopReason::Trap);
                    emu.setPc(pc + 4);
                    break;
                }
                case Kind::Paired: {
                    po::PairedState state{emu, regs, m};
                    hook_fault.reset();
                    try {
                        po::paired_single(state, w);
                    } catch (const HookAbort&) {
                        throw pw::Fault(*hook_fault);
                    }
                    if (op == 4 && (w & 1)) {   // a record form of the arithmetic, merges and moves: CR1 (not the compares, the indexed psq loads and stores, dcbz_l)
                        const unsigned pxo = (w >> 1) & 0x3FF, pxo6 = (w >> 1) & 63;
                        if (pxo != 0 && pxo != 32 && pxo != 64 && pxo != 96 && pxo6 != 6 && pxo6 != 7 && pxo6 != 38 && pxo6 != 39 && pxo != 1014) emu.setReg(regs.cr[1], fpu::cr1_of(m.fpscr));
                    }
                    emu.setPc(pc + 4);
                    break;
                }
                case Kind::Branch: {   // bc, bclr that count CTR down (all 64 bits of it)
                    const std::uint64_t ctr = emu.reg(regs.ctr) - 1;
                    emu.setReg(regs.ctr, ctr);
                    bool taken = (ctr != 0) != ((rt & 2) != 0);
                    if (taken && !(rt & 16)) {
                        const unsigned field = static_cast<unsigned>(emu.reg(regs.cr[ra >> 2])) & 0xF;
                        taken = (((field >> (3 - (ra & 3))) & 1) != 0) == (((rt >> 3) & 1) != 0);
                    }
                    const std::uint32_t next = pc + 4;
                    const std::uint32_t lr = static_cast<std::uint32_t>(emu.reg(regs.lr));   // (bclr goes where LR pointed before it links)
                    std::uint32_t to = next;
                    if (taken) to = op == 16 ? static_cast<std::uint32_t>((w & 2) ? pw::sign_extend(w & 0xFFFC, 16) : pc + pw::sign_extend(w & 0xFFFC, 16)) : lr & 0xFFFFFFFCu;
                    if (w & 1) emu.setReg(regs.lr, next);
                    emu.setPc(to);
                    break;
                }
                case Kind::BranchCtr: run_word(pc, w, (rt & 4) ? w : w | (4u << 21), false); break;
                case Kind::Rotate: {
                    const std::uint64_t rs = emu.reg(regs.gpr[rt]);
                    const unsigned sh = rb | (((w >> 1) & 1) << 5);
                    const unsigned mbe = ((w >> 6) & 31) | (((w >> 5) & 1) << 5);
                    const unsigned form = (w >> 2) & 7;
                    std::uint64_t v = 0;
                    if (form == 0) {   // rldicl
                        v = pw::rotl64(rs, sh) & pw::mask(mbe, 63);
                    } else if (form == 1) {   // rldicr
                        v = pw::rotl64(rs, sh) & pw::mask(0, mbe);
                    } else if (form == 2) {   // rldic
                        v = pw::rotl64(rs, sh) & pw::mask(mbe, 63 - sh);
                    } else if (form == 3) {   // rldimi
                        const std::uint64_t k = pw::mask(mbe, 63 - sh);
                        v = (pw::rotl64(rs, sh) & k) | (emu.reg(regs.gpr[ra]) & ~k);
                    } else {   // rldcl, rldcr: the count is in rB
                        const unsigned n = static_cast<unsigned>(emu.reg(regs.gpr[rb]) & 63);
                        v = pw::rotl64(rs, n) & (((w >> 1) & 0xF) == 8 ? pw::mask(mbe, 63) : pw::mask(0, mbe));
                    }
                    emu.setReg(regs.gpr[ra], v);
                    if (w & 1) set_cr0(v);
                    emu.setPc(pc + 4);
                    break;
                }
                case Kind::Divide: {
                    const std::uint64_t a = emu.reg(regs.gpr[ra]), b = emu.reg(regs.gpr[rb]);
                    std::uint64_t v = 0;
                    bool overflow = false;   // (a zero divisor, or the one quotient that does not fit: the result is undefined, and OV says so)
                    switch (xo10 & 0x1FF) {
                        case 491: {   // divw
                            const std::int64_t x = s32(a), y = s32(b);
                            overflow = y == 0 || (x == -0x80000000ll && y == -1);
                            v = overflow ? 0 : static_cast<std::uint64_t>(x / y) & 0xFFFFFFFFull;
                            break;
                        }
                        case 459: overflow = (b & 0xFFFFFFFFull) == 0; v = overflow ? 0 : (a & 0xFFFFFFFFull) / (b & 0xFFFFFFFFull); break;   // divwu
                        case 489: {   // divd
                            const std::int64_t x = s64(a), y = s64(b);
                            overflow = y == 0 || (x == std::numeric_limits<std::int64_t>::min() && y == -1);
                            v = overflow ? 0 : static_cast<std::uint64_t>(x / y);
                            break;
                        }
                        default: overflow = b == 0; v = overflow ? 0 : a / b; break;   // divdu
                    }
                    emu.setReg(regs.gpr[rt], v);
                    if (w & 0x400) {   // OE
                        emu.setReg(regs.ov, overflow ? 1 : 0);
                        if (overflow) emu.setReg(regs.so, 1);
                    }
                    if (w & 1) set_cr0(v);
                    emu.setPc(pc + 4);
                    break;
                }
                case Kind::Update: {
                    // An update form is the description's as it stands. Where it has none (the base is r0, or is the target) and in lq and stq (the interpreter's ld and std), the plain form
                    // is run and the address, formed before it, goes to the base register (r0 when that is the base).
                    std::uint32_t plain = w;
                    bool update = false;
                    std::uint64_t address = 0;
                    const std::uint64_t base = ra ? emu.reg(regs.gpr[ra]) : 0;
                    if (op == 58 || op == 62) {
                        const unsigned low = w & 3;
                        if ((op == 58 && low == 3) || (op == 62 && low >= 2)) plain = w & ~3u;
                        else if (low == 1) plain = w & ~3u, update = true, address = base + pw::sign_extend(w & 0xFFFC, 16);
                    } else if (op == 31) {
                        plain = w - (32u << 1), update = true, address = base + emu.reg(regs.gpr[rb]);
                    } else {
                        plain = w - (1u << 26), update = true, address = base + pw::sign_extend(w & 0xFFFF, 16);
                    }
                    if (!update) {   // (lq, stq)
                        run_word(pc, w, plain, true);
                        break;
                    }
                    if (ra != 0 && step_as(pc, w, w, true)) break;
                    run_word(pc, w, plain, true);
                    emu.setReg(regs.gpr[ra], address);
                    break;
                }
                case Kind::Multiple: {   // lmw, stmw: the registers from rT on, a word each, as the interpreter does (a refused access leaves what was done)
                    const std::uint32_t ea = static_cast<std::uint32_t>((ra ? emu.reg(regs.gpr[ra]) : 0) + pw::sign_extend(w & 0xFFFF, 16));
                    hook_fault.reset();
                    try {
                        for (unsigned i = rt; i < 32; ++i) {
                            const std::uint32_t at = static_cast<std::uint32_t>(ea + 4ull * (i - rt));
                            if (op == 46) emu.setReg(regs.gpr[i], emu.busRead(at, 4));
                            else emu.busWrite(at, 4, emu.reg(regs.gpr[i]) & 0xFFFFFFFFull);
                        }
                    } catch (const HookAbort&) {
                        throw pw::Fault(*hook_fault);
                    }
                    emu.setPc(pc + 4);
                    break;
                }
                case Kind::Spr: {
                    const unsigned spr = ra | (rb << 5);
                    const bool gqr = options.paired_singles && spr >= 912 && spr <= 919;
                    if (xo10 == 339) {   // mfspr
                        std::uint64_t v = 0;
                        if (gqr) v = m.gqr[spr - 912];
                        else if (spr == 8) v = static_cast<std::uint32_t>(emu.reg(regs.lr));
                        else if (spr == 9) v = emu.reg(regs.ctr);
                        else if (spr == 1) v = ((emu.reg(regs.ca) & 1) << 29) | ((emu.reg(regs.ov) & 1) << 30) | ((emu.reg(regs.so) & 1) << 31);
                        emu.setReg(regs.gpr[rt], v);
                    } else {   // mtspr
                        const std::uint64_t v = emu.reg(regs.gpr[rt]);
                        if (gqr) m.gqr[spr - 912] = static_cast<std::uint32_t>(v);
                        else if (spr == 8) emu.setReg(regs.lr, static_cast<std::uint32_t>(v));
                        else if (spr == 9) emu.setReg(regs.ctr, v);
                        else if (spr == 1) emu.setReg(regs.ca, (v >> 29) & 1), emu.setReg(regs.ov, (v >> 30) & 1), emu.setReg(regs.so, (v >> 31) & 1);
                    }
                    emu.setPc(pc + 4);
                    break;
                }
                case Kind::Float: {
                    // The floating point is the manual's, the one unit the interpreter uses too (emulate/powerpc_fpu.hpp): exact arithmetic rounded under FPSCR[RN], the status register
                    // (kept in the Machine), and CR1 for a record form.
                    const unsigned frc = (w >> 6) & 31;
                    const std::uint64_t araw = emu.reg(regs.fpr[ra]), braw = emu.reg(regs.fpr[rb]), craw = emu.reg(regs.fpr[frc]);
                    const bool single = op == 59;
                    const auto put = [&](const fpu::Result& r) {
                        if (!r.written) return;
                        emu.setReg(regs.fpr[rt], r.bits);
                        if (single && options.paired_singles) m.ps1[rt] = r.bits;   // (Gekko: the single-precision operations fill both halves)
                    };
                    const auto put_plain = [&](const fpu::Result& r) {
                        if (r.written) emu.setReg(regs.fpr[rt], r.bits);
                    };
                    bool compare_form = false;
                    switch (xo10 & 31) {
                        case 21: put(fpu::execute(m.fpscr, fpu::Op::Add, single, araw, braw, craw)); break;       // fadd
                        case 20: put(fpu::execute(m.fpscr, fpu::Op::Sub, single, araw, braw, craw)); break;       // fsub
                        case 25: put(fpu::execute(m.fpscr, fpu::Op::Mul, single, araw, braw, craw)); break;       // fmul
                        case 18: put(fpu::execute(m.fpscr, fpu::Op::Div, single, araw, braw, craw)); break;       // fdiv
                        case 29: put(fpu::execute(m.fpscr, fpu::Op::Madd, single, araw, braw, craw)); break;      // fmadd
                        case 28: put(fpu::execute(m.fpscr, fpu::Op::Msub, single, araw, braw, craw)); break;      // fmsub
                        case 31: put(fpu::execute(m.fpscr, fpu::Op::Nmadd, single, araw, braw, craw)); break;     // fnmadd
                        case 30: put(fpu::execute(m.fpscr, fpu::Op::Nmsub, single, araw, braw, craw)); break;     // fnmsub
                        case 22: put(fpu::execute(m.fpscr, fpu::Op::Sqrt, single, araw, braw, craw)); break;      // fsqrt
                        case 24: put(fpu::execute(m.fpscr, fpu::Op::Res, single, araw, braw, craw)); break;       // fres
                        case 26: put(fpu::execute(m.fpscr, fpu::Op::Rsqrte, single, araw, braw, craw)); break;    // frsqrte
                        case 23: emu.setReg(regs.fpr[rt], pw::to_double(araw) >= 0.0 ? craw : braw); break;          // fsel
                        default:
                            switch (xo10) {
                                case 72: emu.setReg(regs.fpr[rt], braw); break;                                                 // fmr
                                case 40: emu.setReg(regs.fpr[rt], braw ^ (std::uint64_t{1} << 63)); break;                    // fneg
                                case 264: emu.setReg(regs.fpr[rt], braw & ~(std::uint64_t{1} << 63)); break;                  // fabs
                                case 136: emu.setReg(regs.fpr[rt], braw | (std::uint64_t{1} << 63)); break;                   // fnabs
                                case 12: put_plain(fpu::execute(m.fpscr, fpu::Op::Frsp, true, 0, braw, 0)); break;             // frsp
                                case 0: case 32:                                                                               // fcmpu, fcmpo
                                    emu.setReg(regs.cr[rt >> 2], fpu::compare(m.fpscr, araw, braw, xo10 == 32));
                                    compare_form = true;
                                    break;
                                case 14: put_plain(fpu::execute(m.fpscr, fpu::Op::Fctiw, false, 0, braw, 0)); break;            // fctiw
                                case 15: put_plain(fpu::execute(m.fpscr, fpu::Op::Fctiwz, false, 0, braw, 0)); break;           // fctiwz
                                case 814: put_plain(fpu::execute(m.fpscr, fpu::Op::Fctid, false, 0, braw, 0)); break;           // fctid
                                case 815: put_plain(fpu::execute(m.fpscr, fpu::Op::Fctidz, false, 0, braw, 0)); break;          // fctidz
                                case 846: put_plain(fpu::execute(m.fpscr, fpu::Op::Fcfid, false, 0, braw, 0)); break;           // fcfid
                                case 583: emu.setReg(regs.fpr[rt], fpu::move_from(m.fpscr)); break;                             // mffs
                                case 711: fpu::move_to_fields(m.fpscr, (w >> 17) & 0xFF, braw); break;                          // mtfsf
                                case 134: fpu::move_to_field_immediate(m.fpscr, rt >> 2, (w >> 12) & 0xF); break;               // mtfsfi
                                case 70: fpu::set_bit(m.fpscr, rt, false); break;                                               // mtfsb0
                                case 38: fpu::set_bit(m.fpscr, rt, true); break;                                                // mtfsb1
                                case 64:                                                                                       // mcrfs
                                    emu.setReg(regs.cr[rt >> 2], fpu::move_to_cr_from_field(m.fpscr, ra >> 2));
                                    compare_form = true;
                                    break;
                                default: break;
                            }
                    }
                    if ((w & 1) && !compare_form) emu.setReg(regs.cr[1], fpu::cr1_of(m.fpscr));   // a record form: CR1 from FX, FEX, VX, OX
                    emu.setPc(pc + 4);
                    break;
                }
                case Kind::Zero: emu.setReg(regs.gpr[rt], 0); emu.setPc(pc + 4); break;
                case Kind::MoveFromCr: {
                    std::uint32_t cr = 0;
                    for (unsigned i = 0; i < 8; ++i) cr |= (static_cast<std::uint32_t>(emu.reg(regs.cr[i])) & 0xF) << (28 - 4 * i);
                    emu.setReg(regs.gpr[rt], cr);
                    emu.setPc(pc + 4);
                    break;
                }
                case Kind::Nop: emu.setPc(pc + 4); break;
                case Kind::StoreHalfElement: {
                    const std::uint32_t ea = static_cast<std::uint32_t>((ra ? emu.reg(regs.gpr[ra]) : 0) + emu.reg(regs.gpr[rb])) & ~1u;
                    std::array<std::uint8_t, 16> v;
                    emu.readRegWide(regs.vr[rt], v.data());
                    const unsigned lane = 7 - (ea & 15) / 2;   // (element 0 is the most significant)
                    hook_fault.reset();
                    try {
                        emu.busWrite(ea, 2, static_cast<std::uint64_t>(v[2 * lane]) | (static_cast<std::uint64_t>(v[2 * lane + 1]) << 8));
                    } catch (const HookAbort&) {
                        throw pw::Fault(*hook_fault);
                    }
                    emu.setPc(pc + 4);
                    break;
                }
                case Kind::Dcbz: {
                    const std::uint32_t ea = static_cast<std::uint32_t>((ra ? emu.reg(regs.gpr[ra]) : 0) + emu.reg(regs.gpr[rb])) & ~31u;
                    hook_fault.reset();
                    try {
                        for (unsigned i = 0; i < 32; i += 8) emu.busWrite(ea + i, 8, 0);
                    } catch (const HookAbort&) {
                        throw pw::Fault(*hook_fault);
                    }
                    emu.setPc(pc + 4);
                    break;
                }
            }
            if (how.post != 0) {
                if ((how.post & kSingle) && options.paired_singles) m.ps1[rt] = emu.reg(regs.fpr[rt]);
                if ((how.post & kCompareSet) && (w & 0x400)) {   // a vector compare with the record bit: CR6 says whether every element (or none) compared true (vcmpbfp: all within bounds)
                    switch (w & 0x3FF) {
                        case 6: case 70: case 134: case 198: case 454: case 518: case 582: case 646: case 710: case 774: case 838: case 902: case 966: {
                            std::array<std::uint8_t, 16> v;
                            emu.readRegWide(regs.vr[rt], v.data());
                            const bool ones = std::all_of(v.begin(), v.end(), [](std::uint8_t b) { return b == 0xFF; });
                            const bool zeros = std::all_of(v.begin(), v.end(), [](std::uint8_t b) { return b == 0; });
                            emu.setReg(regs.cr[6], (w & 0x3FF) == 966 ? (zeros ? 2 : 0) : ones ? 8 : zeros ? 2 : 0);
                            break;
                        }
                        default: break;
                    }
                }
                if (how.post & kLinkTarget) {   // a branch to LR or CTR drops the low two bits and the upper word, as the interpreter does
                    const std::uint64_t at = emu.pc();
                    if ((at & 3) != 0 || at > 0xFFFFFFFFull) emu.setPc(static_cast<std::uint32_t>(at) & 0xFFFFFFFCu);
                }
            }
            if (pending) {
                to_machine();
                tracker->after(m, pc, w, *pending);
            }
            if ((w & 1) && (op == 16 || op == 18 || op == 19)) linked = static_cast<std::uint32_t>(emu.pc());   // a branch that links: where it went
        }
    } catch (const pw::Fault& fault) {
        out.exit = pw::Exit::Exception;
        out.stopped = fault.reason();
    }
    to_machine();
    out.ret = m.r[3];
    out.fret = m.f[1];
    out.steps = count;
    out.pc = m.pc;
    for (const auto& [address, bytes] : m.writes()) {
        if (bytes.first != bytes.second) out.writes.push_back({address, bytes.first, bytes.second});
    }
    out.saved = saved_at_entry() == entry_saved;
    out.machine = std::move(machine);
    if (tracker) out.branches = std::move(tracker->events);
    return out;
}

bool ppc_interpreter_handles(std::uint32_t word, bool paired) { return interpreter_handles(word, paired); }

PpcProbe ppc_probe(po::Flavor flavor) {
    Core& core = core_of(flavor);
    return {*core.emu, core.regs, core.vector, core.binding};
}

pw::RunResult call(po::Flavor flavor, std::span<const pw::Region> regions, std::uint32_t entry, std::span<const std::uint64_t> args, const pw::RunOptions& options) {
    return enabled() ? pcode_cpu::run(flavor, regions, entry, args, options) : pw::run(regions, entry, args, options);
}

pw::RunResult run(std::span<const pw::Region> regions, std::uint32_t entry, std::span<const std::uint64_t> args, const pw::RunOptions& options) {
    return run(po::Flavor::AltiVec, regions, entry, args, options);
}

pw::RunResult call(std::span<const pw::Region> regions, std::uint32_t entry, std::span<const std::uint64_t> args, const pw::RunOptions& options) {
    return enabled() ? pcode_cpu::run(regions, entry, args, options) : pw::run(regions, entry, args, options);
}

}  // namespace cb::emulate::pcode_cpu
