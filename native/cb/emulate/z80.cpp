// cb/emulate/z80.cpp -- see z80.hpp.
#include "cb/emulate/z80.hpp"

#include <algorithm>
#include <array>
#include <format>
#include <mutex>
#include <string_view>

#include "cb/core/error.hpp"

namespace cb::emulate::z80 {

namespace dec = cb::decompile::z80;

namespace {

// ---- the flag tables ---------------------------------------------------------------------------------------------

constexpr std::uint8_t parity_flag(unsigned v) {
    v ^= v >> 4;
    v ^= v >> 2;
    v ^= v >> 1;
    return (v & 1) ? 0 : flag::P;
}

// S, bits 5 and 3 of the value, and Z; and the same with the parity.
constexpr std::array<std::uint8_t, 256> make_sz() {
    std::array<std::uint8_t, 256> t{};
    for (unsigned v = 0; v < 256; ++v) t[v] = static_cast<std::uint8_t>((v & (flag::S | flag::Y | flag::X)) | (v == 0 ? flag::Z : 0));
    return t;
}
constexpr std::array<std::uint8_t, 256> make_szp() {
    std::array<std::uint8_t, 256> t = make_sz();
    for (unsigned v = 0; v < 256; ++v) t[v] = static_cast<std::uint8_t>(t[v] | parity_flag(v));
    return t;
}
constexpr std::array<std::uint8_t, 256> kSz = make_sz();
constexpr std::array<std::uint8_t, 256> kSzp = make_szp();

}  // namespace

// ---- the compiled instruction -----------------------------------------------------------------------------------------

enum class Reg8 : std::uint8_t { A, F, B, C, D, E, H, L, IXH, IXL, IYH, IYL, I, R };
enum class Reg16 : std::uint8_t { AF, BC, DE, HL, SP, IX, IY };
enum class Loc : std::uint8_t { None, R8, R16, Imm, Pair, Nn, Indexed, Number };
enum class Cond : std::uint8_t { NZ, Z, NC, C, PO, PE, P, M, Always };
enum class Alu : std::uint8_t { Add, Adc, Sub, Sbc, And, Xor, Or, Cp };
enum class Rot : std::uint8_t { Rlc, Rrc, Rl, Rr, Sla, Sra, Sll, Srl };

// Where an operand is: a register, an immediate (value k of the instruction), memory through a pair (BC DE HL), at an
// absolute address (value k), or at IX/IY plus a displacement (value k).
struct Operand {
    Loc loc = Loc::None;
    std::uint8_t reg = 0;      // Reg8, Reg16 (a pair, or IX/IY for Indexed)
    std::uint8_t k = 0;        // which of the instruction's values
    std::uint8_t number = 0;   // Loc::Number: a bit number
};

enum class Mn : std::uint8_t {
    Nop, LdAIR, LdNnReg, LdRegNn, LdIdxN, Ld, Push, Pop, ExAf, ExDeHl, ExSp, Exx, Arith8, Arith16, Inc16, Dec16, Inc8, Dec8,
    RotA, Rotate, Bit, SetRes, JpReg, Jump, Djnz, Ret, Reti, Rst, Halt, DiEi, Im, Cpl, Neg, Scf, Ccf, Daa, Rld, InA, InC, OutA,
    OutC, Ldx, Cpx, IoBlock,
};

struct Program {
    Mn mn = Mn::Nop;
    Operand op[3];
    std::uint8_t count = 0;        // operands written in the pattern
    std::uint8_t sub = 0;          // Alu / Rot / Cond / bit number / RotA kind, by mnemonic
    std::uint8_t reg = 0;          // a register the mnemonic names (Reg8 / Reg16)
    std::uint8_t copy = 0;         // 1 + the Reg8 a DDCB form copies its result to (0: none)
    bool flag_a = false;           // Rotate/SetRes copy; Ldx/Cpx/IoBlock: decrement; Jump: call; Bit: indexed; Cpl...
    bool flag_b = false;           // Ldx/Cpx/IoBlock: repeat; Jump: relative-or-absolute not needed; Bit: (IX+d)
    bool flag_c = false;           // IoBlock: input
    bool flag_d = false;           // OutC: writes 0
    bool conditional = false;      // Jump / Ret: a condition is given
};

namespace {

std::optional<Reg8> reg8_of(std::string_view s) {
    static constexpr std::pair<std::string_view, Reg8> kNames[] = {
        {"A", Reg8::A}, {"B", Reg8::B}, {"C", Reg8::C}, {"D", Reg8::D}, {"E", Reg8::E}, {"H", Reg8::H}, {"L", Reg8::L},
        {"IXH", Reg8::IXH}, {"IXL", Reg8::IXL}, {"IYH", Reg8::IYH}, {"IYL", Reg8::IYL}, {"I", Reg8::I}, {"R", Reg8::R}};
    for (const auto& [name, r] : kNames) {
        if (name == s) return r;
    }
    return std::nullopt;
}

std::optional<Reg16> reg16_of(std::string_view s) {
    static constexpr std::pair<std::string_view, Reg16> kNames[] = {
        {"AF", Reg16::AF}, {"BC", Reg16::BC}, {"DE", Reg16::DE}, {"HL", Reg16::HL}, {"SP", Reg16::SP}, {"IX", Reg16::IX}, {"IY", Reg16::IY}};
    for (const auto& [name, r] : kNames) {
        if (name == s) return r;
    }
    return std::nullopt;
}

std::optional<Cond> cond_of(std::string_view s) {
    for (std::size_t i = 0; i < dec::kConditions.size(); ++i) {
        if (dec::kConditions[i] == s) return static_cast<Cond>(i);
    }
    return std::nullopt;
}

std::optional<Rot> rot_of(std::string_view s) {
    for (std::size_t i = 0; i < dec::kRotates.size(); ++i) {
        if (dec::kRotates[i] == s) return static_cast<Rot>(i);
    }
    return std::nullopt;
}

bool all_digits(std::string_view s) {
    return !s.empty() && std::all_of(s.begin(), s.end(), [](char c) { return c >= '0' && c <= '9'; });
}

// One written operand form; `k` is the index of its value among the instruction's. Whether it takes a value goes
// back in `takes`.
Operand operand_of(std::string_view form, std::uint8_t k, bool& takes) {
    takes = false;
    Operand o;
    if (const auto r = reg8_of(form)) {
        o.loc = Loc::R8;
        o.reg = static_cast<std::uint8_t>(*r);
    } else if (const auto r16 = reg16_of(form)) {
        o.loc = Loc::R16;
        o.reg = static_cast<std::uint8_t>(*r16);
    } else if (form == "n" || form == "nn") {
        o.loc = Loc::Imm;
        o.k = k;
        takes = true;
    } else if (form == "(BC)" || form == "(DE)" || form == "(HL)") {
        o.loc = Loc::Pair;
        o.reg = static_cast<std::uint8_t>(*reg16_of(form.substr(1, 2)));
    } else if (form == "(nn)") {
        o.loc = Loc::Nn;
        o.k = k;
        takes = true;
    } else if (form == "(IX+d)" || form == "(IY+d)") {
        o.loc = Loc::Indexed;
        o.reg = static_cast<std::uint8_t>(*reg16_of(form.substr(1, 2)));
        o.k = k;
        takes = true;
    } else if (all_digits(form)) {
        o.loc = Loc::Number;
        o.number = static_cast<std::uint8_t>(std::stoi(std::string(form)));
    } else {
        takes = form == "(n)";   // (C) (n) (SP) (IX) AF' and the like: read by their instruction's own code
    }
    return o;
}

// The instruction pattern compiled: its mnemonic's variant, its operands resolved to places.
Program compile(std::string_view pattern) {
    Program p;
    std::string_view m = pattern, rest;
    if (const auto sp = pattern.find(' '); sp != std::string_view::npos) {
        m = pattern.substr(0, sp);
        rest = pattern.substr(sp + 1);
    }
    std::vector<std::string_view> forms;
    while (!rest.empty()) {
        const auto comma = rest.find(',');
        if (comma == std::string_view::npos) {
            forms.push_back(rest);
            break;
        }
        forms.push_back(rest.substr(0, comma));
        rest = rest.substr(comma + 1);
    }
    const bool jumps = m == "JP" || m == "JR" || m == "CALL" || m == "RET";
    std::uint8_t k = 0;
    for (std::size_t i = 0; i < forms.size() && i < 3; ++i) {
        if (jumps && cond_of(forms[i])) {   // a condition, not a register
            p.op[i] = {};
            ++p.count;
            continue;
        }
        bool takes = false;
        p.op[i] = operand_of(forms[i], k, takes);
        k = static_cast<std::uint8_t>(k + (takes ? 1 : 0));
        ++p.count;
    }
    auto form = [&](std::size_t i) { return i < forms.size() ? forms[i] : std::string_view(); };
    auto is_wide = [](std::string_view f) { return reg16_of(f).has_value(); };

    if (m == "NOP" || m == ".db") {
        p.mn = Mn::Nop;
    } else if (m == "LD") {
        if (form(0) == "A" && (form(1) == "I" || form(1) == "R")) {
            p.mn = Mn::LdAIR;
            p.reg = static_cast<std::uint8_t>(*reg8_of(form(1)));
        } else if (form(0) == "(nn)" && is_wide(form(1))) {
            p.mn = Mn::LdNnReg;
            p.reg = static_cast<std::uint8_t>(*reg16_of(form(1)));
        } else if (form(1) == "(nn)" && is_wide(form(0))) {
            p.mn = Mn::LdRegNn;
            p.reg = static_cast<std::uint8_t>(*reg16_of(form(0)));
        } else if (form(0).ends_with("+d)") && form(1) == "n") {   // LD (IX+d),n: the displacement is value 0, the byte value 1
            p.mn = Mn::LdIdxN;
            p.reg = static_cast<std::uint8_t>(*reg16_of(form(0).substr(1, 2)));
        } else {
            p.mn = Mn::Ld;
        }
    } else if (m == "PUSH" || m == "POP") {
        p.mn = m == "PUSH" ? Mn::Push : Mn::Pop;
        p.reg = static_cast<std::uint8_t>(*reg16_of(form(0)));
    } else if (m == "EX") {
        if (form(0) == "AF") {
            p.mn = Mn::ExAf;
        } else if (form(0) == "DE") {
            p.mn = Mn::ExDeHl;
        } else {
            p.mn = Mn::ExSp;   // EX (SP),HL/IX/IY
            p.reg = static_cast<std::uint8_t>(*reg16_of(form(1)));
        }
    } else if (m == "EXX") {
        p.mn = Mn::Exx;
    } else if (m == "ADD" || m == "ADC" || m == "SUB" || m == "SBC" || m == "AND" || m == "XOR" || m == "OR" || m == "CP") {
        static constexpr std::pair<std::string_view, Alu> kAlu[] = {{"ADD", Alu::Add}, {"ADC", Alu::Adc}, {"SUB", Alu::Sub}, {"SBC", Alu::Sbc},
                                                                    {"AND", Alu::And}, {"XOR", Alu::Xor}, {"OR", Alu::Or}, {"CP", Alu::Cp}};
        for (const auto& [name, a] : kAlu) {
            if (name == m) p.sub = static_cast<std::uint8_t>(a);
        }
        const std::string_view first = form(0);
        if ((first == "HL" || first == "IX" || first == "IY") && forms.size() == 2) {
            p.mn = Mn::Arith16;
            p.reg = static_cast<std::uint8_t>(*reg16_of(first));
        } else {
            p.mn = Mn::Arith8;
        }
    } else if (m == "INC" || m == "DEC") {
        const bool inc = m == "INC";
        if (is_wide(form(0))) {
            p.mn = inc ? Mn::Inc16 : Mn::Dec16;
            p.reg = static_cast<std::uint8_t>(*reg16_of(form(0)));
        } else {
            p.mn = inc ? Mn::Inc8 : Mn::Dec8;
        }
    } else if (m == "RLCA" || m == "RRCA" || m == "RLA" || m == "RRA") {
        p.mn = Mn::RotA;
        p.sub = static_cast<std::uint8_t>(*rot_of(m.substr(0, m.size() - 1)));
    } else if (const auto rot = rot_of(m)) {
        p.mn = Mn::Rotate;
        p.sub = static_cast<std::uint8_t>(*rot);
        if (forms.size() > 1) p.copy = static_cast<std::uint8_t>(1 + static_cast<std::uint8_t>(*reg8_of(form(1))));   // DDCB undocumented: also into a register
    } else if (m == "BIT") {
        p.mn = Mn::Bit;
        p.sub = static_cast<std::uint8_t>(std::stoi(std::string(form(0))));
        p.flag_a = form(1).ends_with("+d)") || form(1) == "(HL)";   // the CPU takes bits 5 and 3 from elsewhere than the operand
        p.flag_b = form(1).ends_with("+d)");
        if (p.flag_b) p.reg = static_cast<std::uint8_t>(*reg16_of(form(1).substr(1, 2)));
    } else if (m == "SET" || m == "RES") {
        p.mn = Mn::SetRes;
        p.flag_a = m == "SET";
        p.sub = static_cast<std::uint8_t>(std::stoi(std::string(form(0))));
        if (forms.size() > 2) p.copy = static_cast<std::uint8_t>(1 + static_cast<std::uint8_t>(*reg8_of(form(2))));
    } else if (m == "JP" || m == "JR" || m == "CALL") {
        if (!forms.empty() && forms[0].starts_with("(")) {   // JP (HL) / (IX) / (IY)
            p.mn = Mn::JpReg;
            p.reg = static_cast<std::uint8_t>(*reg16_of(forms[0].substr(1, forms[0].size() - 2)));
        } else {
            p.mn = Mn::Jump;
            p.flag_a = m == "CALL";
            p.conditional = forms.size() == 2;
            if (p.conditional) p.sub = static_cast<std::uint8_t>(*cond_of(forms[0]));
        }
    } else if (m == "DJNZ") {
        p.mn = Mn::Djnz;
    } else if (m == "RET") {
        p.mn = Mn::Ret;
        p.conditional = !forms.empty();
        if (p.conditional) p.sub = static_cast<std::uint8_t>(*cond_of(forms[0]));
    } else if (m == "RETI" || m == "RETN") {
        p.mn = Mn::Reti;
    } else if (m == "RST") {
        p.mn = Mn::Rst;
    } else if (m == "HALT") {
        p.mn = Mn::Halt;
    } else if (m == "DI" || m == "EI") {
        p.mn = Mn::DiEi;
        p.flag_a = m == "EI";
    } else if (m == "IM") {
        p.mn = Mn::Im;
        p.sub = static_cast<std::uint8_t>(std::stoi(std::string(form(0))));
    } else if (m == "CPL") {
        p.mn = Mn::Cpl;
    } else if (m == "NEG") {
        p.mn = Mn::Neg;
    } else if (m == "SCF") {
        p.mn = Mn::Scf;
    } else if (m == "CCF") {
        p.mn = Mn::Ccf;
    } else if (m == "DAA") {
        p.mn = Mn::Daa;
    } else if (m == "RLD" || m == "RRD") {
        p.mn = Mn::Rld;
        p.flag_a = m == "RLD";
    } else if (m == "IN") {
        if (pattern == "IN A,(n)") {
            p.mn = Mn::InA;
        } else {
            p.mn = Mn::InC;
            p.flag_a = form(0) != "(C)";   // IN (C): only the flags
            if (p.flag_a) p.reg = static_cast<std::uint8_t>(*reg8_of(form(0)));
        }
    } else if (m == "OUT") {
        if (pattern == "OUT (n),A") {
            p.mn = Mn::OutA;
        } else {
            p.mn = Mn::OutC;
            p.flag_d = form(1) == "0";
            if (!p.flag_d) p.reg = static_cast<std::uint8_t>(*reg8_of(form(1)));
        }
    } else if (m == "LDI" || m == "LDD" || m == "LDIR" || m == "LDDR" || m == "CPI" || m == "CPD" || m == "CPIR" || m == "CPDR") {
        p.mn = m.starts_with("LD") ? Mn::Ldx : Mn::Cpx;
        p.flag_a = m[2] == 'D';           // goes down
        p.flag_b = m.ends_with("R");       // repeats
    } else if (m == "INI" || m == "IND" || m == "INIR" || m == "INDR" || m == "OUTI" || m == "OUTD" || m == "OTIR" || m == "OTDR") {
        p.mn = Mn::IoBlock;
        p.flag_c = m.starts_with("IN");
        p.flag_a = !(m == "INI" || m == "INIR" || m == "OUTI" || m == "OTIR");   // goes down
        p.flag_b = m.ends_with("R");       // repeats
    } else {
        throw InternalError(std::format("z80: no handler for {}", pattern));
    }
    return p;
}

// Every pattern compiled once, for every Cpu on every thread. (A node of the map never moves.)
const Program& program_of(const std::string& pattern) {
    static std::mutex mutex;
    static std::unordered_map<std::string, Program> programs;
    const std::lock_guard<std::mutex> lock(mutex);
    auto it = programs.find(pattern);
    if (it == programs.end()) it = programs.emplace(pattern, compile(pattern)).first;
    return it->second;
}

}  // namespace

// ---- Bus ---------------------------------------------------------------------------------------------------------

Bus::Bus(PortIn in, PortOut out) : owned_(kMemorySize, 0), ram_(owned_), port_in_(std::move(in)), port_out_(std::move(out)) {}

Bus::Bus(ByteView image, PortIn in, PortOut out) : Bus(std::move(in), std::move(out)) {
    if (image.size() > kMemorySize) throw UsageError(std::format("a Z80 bus holds 64 KB, not {} bytes", image.size()));
    std::copy(image.begin(), image.end(), owned_.begin());
}

Bus Bus::over(ByteSpan ram, PortIn in, PortOut out) {
    if (ram.size() != kMemorySize) throw UsageError(std::format("a Z80 bus is 64 KB, not {} bytes", ram.size()));
    Bus bus(std::move(in), std::move(out));
    bus.owned_.clear();
    bus.owned_.shrink_to_fit();
    bus.ram_ = ram;
    return bus;
}

Bus Bus::bare(ByteSpan ram) {
    Bus bus = over(ram);
    bus.ports_ = false;
    return bus;
}

void Bus::map(std::uint16_t lo, std::uint16_t hi, ReadFn read, WriteFn write) {
    ranges_.push_back({lo, hi, std::move(read), std::move(write)});
}

std::uint8_t Bus::read(std::uint16_t address) {
    for (const Range& r : ranges_) {
        if (r.lo <= address && address <= r.hi) {
            const std::uint8_t v = r.read ? r.read(address) : 0;
            log_.push_back({BusAccess::Kind::Read, address, v});
            return v;
        }
    }
    return ram_[address];
}

void Bus::write(std::uint16_t address, std::uint8_t value) {
    for (const Range& r : ranges_) {
        if (r.lo <= address && address <= r.hi) {
            log_.push_back({BusAccess::Kind::Write, address, value});
            if (r.write) r.write(address, value);
            return;
        }
    }
    ram_[address] = value;
}

std::uint8_t Bus::in(std::uint16_t port) {
    if (!ports_) return 0xFF;
    const std::uint8_t v = port_in_ ? port_in_(port) : 0xFF;
    log_.push_back({BusAccess::Kind::In, static_cast<std::uint8_t>(port), v});
    return v;
}

void Bus::out(std::uint16_t port, std::uint8_t value) {
    if (!ports_) return;
    log_.push_back({BusAccess::Kind::Out, static_cast<std::uint8_t>(port), value});
    if (port_out_) port_out_(port, value);
}

std::string_view to_text(BusAccess::Kind kind) {
    switch (kind) {
        case BusAccess::Kind::Read: return "r";
        case BusAccess::Kind::Write: return "w";
        case BusAccess::Kind::In: return "in";
        case BusAccess::Kind::Out: return "out";
    }
    return "?";
}

std::string Fault::text() const {
    return kind == Kind::Halt ? std::string("HALT") : std::format("undecodable at {:04X}", pc);
}

// ---- the interpreter ----------------------------------------------------------------------------------------------

struct Exec {
    Registers& r;
    Bus& bus;
    const dec::Insn& insn;
    const Program& p;

    // memory
    std::uint8_t rd(std::uint32_t a) { return bus.read(static_cast<std::uint16_t>(a)); }
    void wr(std::uint32_t a, std::uint32_t v) { bus.write(static_cast<std::uint16_t>(a), static_cast<std::uint8_t>(v)); }
    std::uint16_t rd16(std::uint32_t a) {
        const unsigned lo = rd(a);   // (low byte first: a device sees the accesses in this order)
        const unsigned hi = rd(a + 1);
        return static_cast<std::uint16_t>(lo | hi << 8);
    }
    void wr16(std::uint32_t a, std::uint32_t v) {
        wr(a, v);
        wr(a + 1, v >> 8);
    }
    void push(std::uint16_t v) {
        r.sp = static_cast<std::uint16_t>(r.sp - 2);
        wr16(r.sp, v);
    }
    std::uint16_t pop() {
        const std::uint16_t v = rd16(r.sp);
        r.sp = static_cast<std::uint16_t>(r.sp + 2);
        return v;
    }

    // registers
    std::uint8_t get8(Reg8 which) const {
        switch (which) {
            case Reg8::A: return r.a;
            case Reg8::F: return r.f;
            case Reg8::B: return r.b;
            case Reg8::C: return r.c;
            case Reg8::D: return r.d;
            case Reg8::E: return r.e;
            case Reg8::H: return r.h;
            case Reg8::L: return r.l;
            case Reg8::IXH: return static_cast<std::uint8_t>(r.ix >> 8);
            case Reg8::IXL: return static_cast<std::uint8_t>(r.ix);
            case Reg8::IYH: return static_cast<std::uint8_t>(r.iy >> 8);
            case Reg8::IYL: return static_cast<std::uint8_t>(r.iy);
            case Reg8::I: return r.i;
            case Reg8::R: return r.r;
        }
        return 0;
    }
    void set8(Reg8 which, std::uint32_t v) {
        const std::uint8_t b = static_cast<std::uint8_t>(v);
        switch (which) {
            case Reg8::A: r.a = b; break;
            case Reg8::F: r.f = b; break;
            case Reg8::B: r.b = b; break;
            case Reg8::C: r.c = b; break;
            case Reg8::D: r.d = b; break;
            case Reg8::E: r.e = b; break;
            case Reg8::H: r.h = b; break;
            case Reg8::L: r.l = b; break;
            case Reg8::IXH: r.ix = static_cast<std::uint16_t>(b << 8 | (r.ix & 0xFF)); break;
            case Reg8::IXL: r.ix = static_cast<std::uint16_t>((r.ix & 0xFF00) | b); break;
            case Reg8::IYH: r.iy = static_cast<std::uint16_t>(b << 8 | (r.iy & 0xFF)); break;
            case Reg8::IYL: r.iy = static_cast<std::uint16_t>((r.iy & 0xFF00) | b); break;
            case Reg8::I: r.i = b; break;
            case Reg8::R: r.r = b; break;
        }
    }
    std::uint16_t get16(Reg16 which) const {
        switch (which) {
            case Reg16::AF: return r.af();
            case Reg16::BC: return r.bc();
            case Reg16::DE: return r.de();
            case Reg16::HL: return r.hl();
            case Reg16::SP: return r.sp;
            case Reg16::IX: return r.ix;
            case Reg16::IY: return r.iy;
        }
        return 0;
    }
    void set16(Reg16 which, std::uint32_t v) {
        const std::uint8_t hi = static_cast<std::uint8_t>(v >> 8), lo = static_cast<std::uint8_t>(v);
        switch (which) {
            case Reg16::AF: r.a = hi; r.f = lo; break;
            case Reg16::BC: r.b = hi; r.c = lo; break;
            case Reg16::DE: r.d = hi; r.e = lo; break;
            case Reg16::HL: r.h = hi; r.l = lo; break;
            case Reg16::SP: r.sp = static_cast<std::uint16_t>(v); break;
            case Reg16::IX: r.ix = static_cast<std::uint16_t>(v); break;
            case Reg16::IY: r.iy = static_cast<std::uint16_t>(v); break;
        }
    }

    bool cond(Cond cc) const {
        const unsigned f = r.f;
        switch (cc) {
            case Cond::NZ: return !(f & flag::Z);
            case Cond::Z: return f & flag::Z;
            case Cond::NC: return !(f & flag::C);
            case Cond::C: return f & flag::C;
            case Cond::PO: return !(f & flag::P);
            case Cond::PE: return f & flag::P;
            case Cond::P: return !(f & flag::S);
            case Cond::M: return f & flag::S;
            case Cond::Always: return true;
        }
        return false;
    }

    // operands
    std::int32_t value(const Operand& o) const { return insn.values[o.k]; }
    std::uint16_t indexed_address(const Operand& o) const {
        return static_cast<std::uint16_t>(get16(static_cast<Reg16>(o.reg)) + value(o));
    }
    std::uint32_t load(const Operand& o) {
        switch (o.loc) {
            case Loc::R8: return get8(static_cast<Reg8>(o.reg));
            case Loc::R16: return get16(static_cast<Reg16>(o.reg));
            case Loc::Imm: return static_cast<std::uint32_t>(value(o));
            case Loc::Pair: return rd(get16(static_cast<Reg16>(o.reg)));
            case Loc::Nn: return rd(static_cast<std::uint32_t>(value(o)));
            case Loc::Indexed: return rd(indexed_address(o));
            case Loc::Number: return o.number;
            case Loc::None: break;
        }
        throw InternalError("z80: an operand that cannot be read");
    }
    void store(const Operand& o, std::uint32_t v) {
        switch (o.loc) {
            case Loc::R8: set8(static_cast<Reg8>(o.reg), v); return;
            case Loc::R16: set16(static_cast<Reg16>(o.reg), v); return;
            case Loc::Pair: wr(get16(static_cast<Reg16>(o.reg)), v); return;
            case Loc::Nn: wr(static_cast<std::uint32_t>(value(o)), v); return;
            case Loc::Indexed: wr(indexed_address(o), v); return;
            default: break;
        }
        throw InternalError("z80: an operand that cannot be written");
    }

    // arithmetic with flags
    std::uint8_t add8(unsigned a, unsigned b, unsigned carry = 0) {
        const unsigned res = a + b + carry;
        unsigned f = kSz[res & 0xFF] | ((a ^ b ^ res) & flag::H) | (res > 0xFF ? flag::C : 0);
        f |= (~(a ^ b) & (a ^ res) & 0x80) ? flag::P : 0;
        r.f = static_cast<std::uint8_t>(f);
        return static_cast<std::uint8_t>(res);
    }
    std::uint8_t sub8(unsigned a, unsigned b, unsigned carry = 0) {
        const int res = static_cast<int>(a) - static_cast<int>(b) - static_cast<int>(carry);
        const unsigned u = static_cast<unsigned>(res);
        unsigned f = kSz[u & 0xFF] | flag::N | ((a ^ b ^ u) & flag::H) | (res < 0 ? flag::C : 0);
        f |= ((a ^ b) & (a ^ u) & 0x80) ? flag::P : 0;
        r.f = static_cast<std::uint8_t>(f);
        return static_cast<std::uint8_t>(res);
    }
    void alu(Alu op, unsigned v) {
        const unsigned a = r.a;
        switch (op) {
            case Alu::Add: r.a = add8(a, v); break;
            case Alu::Adc: r.a = add8(a, v, r.f & flag::C); break;
            case Alu::Sub: r.a = sub8(a, v); break;
            case Alu::Sbc: r.a = sub8(a, v, r.f & flag::C); break;
            case Alu::And: r.a = static_cast<std::uint8_t>(a & v); r.f = kSzp[r.a] | flag::H; break;
            case Alu::Xor: r.a = static_cast<std::uint8_t>(a ^ v); r.f = kSzp[r.a]; break;
            case Alu::Or: r.a = static_cast<std::uint8_t>(a | v); r.f = kSzp[r.a]; break;
            case Alu::Cp:   // the flags of A - v, bits 5 and 3 from v
                sub8(a, v);
                r.f = static_cast<std::uint8_t>((r.f & ~(flag::Y | flag::X)) | (v & (flag::Y | flag::X)));
                break;
        }
    }
    std::uint8_t rotate(Rot op, unsigned v) {
        const unsigned cy = r.f & flag::C;
        unsigned res = 0, out = 0;
        switch (op) {
            case Rot::Rlc: res = ((v << 1) | (v >> 7)) & 0xFF; out = v >> 7; break;
            case Rot::Rrc: res = ((v >> 1) | (v << 7)) & 0xFF; out = v & 1; break;
            case Rot::Rl: res = ((v << 1) | cy) & 0xFF; out = v >> 7; break;
            case Rot::Rr: res = (v >> 1) | (cy << 7); out = v & 1; break;
            case Rot::Sla: res = (v << 1) & 0xFF; out = v >> 7; break;
            case Rot::Sra: res = (v >> 1) | (v & 0x80); out = v & 1; break;
            case Rot::Sll: res = ((v << 1) | 1) & 0xFF; out = v >> 7; break;
            case Rot::Srl: res = v >> 1; out = v & 1; break;
        }
        r.f = static_cast<std::uint8_t>(kSzp[res] | (out ? flag::C : 0));
        return static_cast<std::uint8_t>(res);
    }
    std::uint8_t inc8(unsigned v) {
        const unsigned res = (v + 1) & 0xFF;
        r.f = static_cast<std::uint8_t>((r.f & flag::C) | kSz[res] | ((v & 0x0F) == 0x0F ? flag::H : 0) | (v == 0x7F ? flag::P : 0));
        return static_cast<std::uint8_t>(res);
    }
    std::uint8_t dec8(unsigned v) {
        const unsigned res = (v - 1) & 0xFF;
        r.f = static_cast<std::uint8_t>((r.f & flag::C) | kSz[res] | flag::N | ((v & 0x0F) == 0 ? flag::H : 0) | (v == 0x80 ? flag::P : 0));
        return static_cast<std::uint8_t>(res);
    }
    std::uint16_t add16(unsigned a, unsigned b) {
        const unsigned res = a + b;
        r.f = static_cast<std::uint8_t>((r.f & (flag::S | flag::Z | flag::P)) | (((a ^ b ^ res) >> 8) & flag::H) | ((res >> 8) & (flag::Y | flag::X)) |
                                        (res > 0xFFFF ? flag::C : 0));
        return static_cast<std::uint16_t>(res);
    }
    std::uint16_t adc16(unsigned a, unsigned b) {
        const unsigned res = a + b + (r.f & flag::C);
        unsigned f = (res & 0x8000 ? flag::S : 0) | ((res & 0xFFFF) == 0 ? flag::Z : 0) | (((a ^ b ^ res) >> 8) & flag::H);
        f |= ((res >> 8) & (flag::Y | flag::X)) | (res > 0xFFFF ? flag::C : 0);
        f |= (~(a ^ b) & (a ^ res) & 0x8000) ? flag::P : 0;
        r.f = static_cast<std::uint8_t>(f);
        return static_cast<std::uint16_t>(res);
    }
    std::uint16_t sbc16(unsigned a, unsigned b) {
        const int res = static_cast<int>(a) - static_cast<int>(b) - static_cast<int>(r.f & flag::C);
        const unsigned u = static_cast<unsigned>(res);
        unsigned f = flag::N | (u & 0x8000 ? flag::S : 0) | ((u & 0xFFFF) == 0 ? flag::Z : 0) | (((a ^ b ^ u) >> 8) & flag::H);
        f |= ((u >> 8) & (flag::Y | flag::X)) | (res < 0 ? flag::C : 0);
        f |= ((a ^ b) & (a ^ u) & 0x8000) ? flag::P : 0;
        r.f = static_cast<std::uint8_t>(f);
        return static_cast<std::uint16_t>(res);
    }

    void run(std::uint16_t& pc);   // the instruction (pc is already past it)
};

void Exec::run(std::uint16_t& pc) {
    const std::optional<std::uint16_t>& target = insn.target;
    const Operand& o0 = p.op[0];
    const Operand& o1 = p.op[1];
    switch (p.mn) {
        case Mn::Nop: break;
        case Mn::LdAIR: {
            r.a = get8(static_cast<Reg8>(p.reg));
            r.f = static_cast<std::uint8_t>((r.f & flag::C) | kSz[r.a] | (r.iff2 ? flag::P : 0));
            break;
        }
        case Mn::LdNnReg: wr16(static_cast<std::uint32_t>(insn.values[0]), get16(static_cast<Reg16>(p.reg))); break;
        case Mn::LdRegNn: set16(static_cast<Reg16>(p.reg), rd16(static_cast<std::uint32_t>(insn.values[0]))); break;
        case Mn::LdIdxN:
            wr(static_cast<std::uint16_t>(get16(static_cast<Reg16>(p.reg)) + insn.values[0]), static_cast<std::uint32_t>(insn.values[1]));
            break;
        case Mn::Ld: {
            const std::uint32_t v = load(o1);
            store(o0, v);
            break;
        }
        case Mn::Push: push(get16(static_cast<Reg16>(p.reg))); break;
        case Mn::Pop: set16(static_cast<Reg16>(p.reg), pop()); break;
        case Mn::ExAf:
            std::swap(r.a, r.a2);
            std::swap(r.f, r.f2);
            break;
        case Mn::ExDeHl: {
            const std::uint16_t de = r.de(), hl = r.hl();
            set16(Reg16::DE, hl);
            set16(Reg16::HL, de);
            break;
        }
        case Mn::ExSp: {
            const std::uint16_t v = rd16(r.sp);
            wr16(r.sp, get16(static_cast<Reg16>(p.reg)));
            set16(static_cast<Reg16>(p.reg), v);
            break;
        }
        case Mn::Exx:
            std::swap(r.b, r.b2);
            std::swap(r.c, r.c2);
            std::swap(r.d, r.d2);
            std::swap(r.e, r.e2);
            std::swap(r.h, r.h2);
            std::swap(r.l, r.l2);
            break;
        case Mn::Arith8: alu(static_cast<Alu>(p.sub), load(p.op[p.count - 1])); break;
        case Mn::Arith16: {
            const Reg16 dst = static_cast<Reg16>(p.reg);
            const unsigned a = get16(dst), b = get16(static_cast<Reg16>(o1.reg));
            const Alu op = static_cast<Alu>(p.sub);
            set16(dst, op == Alu::Add ? add16(a, b) : op == Alu::Adc ? adc16(a, b) : sbc16(a, b));
            break;
        }
        case Mn::Inc16: set16(static_cast<Reg16>(p.reg), get16(static_cast<Reg16>(p.reg)) + 1u); break;
        case Mn::Dec16: set16(static_cast<Reg16>(p.reg), get16(static_cast<Reg16>(p.reg)) - 1u); break;
        case Mn::Inc8: {
            const std::uint8_t v = inc8(load(o0));
            store(o0, v);
            break;
        }
        case Mn::Dec8: {
            const std::uint8_t v = dec8(load(o0));
            store(o0, v);
            break;
        }
        case Mn::RotA: {
            const unsigned keep = r.f & (flag::S | flag::Z | flag::P);
            r.a = rotate(static_cast<Rot>(p.sub), r.a);
            r.f = static_cast<std::uint8_t>(keep | (r.f & flag::C) | (r.a & (flag::Y | flag::X)));
            break;
        }
        case Mn::Rotate: {
            const std::uint8_t v = rotate(static_cast<Rot>(p.sub), load(o0));
            store(o0, v);
            if (p.copy) set8(static_cast<Reg8>(p.copy - 1), v);
            break;
        }
        case Mn::Bit: {
            const unsigned v = load(o1);
            const unsigned t = v & (1u << p.sub);
            unsigned f = (r.f & flag::C) | flag::H | (!t ? (flag::Z | flag::P) : 0) | (p.sub == 7 && t ? flag::S : 0);
            if (!p.flag_a) {
                f |= v & (flag::Y | flag::X);
            } else if (p.flag_b) {   // (IX+d): bits 5 and 3 of the address's high byte
                f |= (static_cast<std::uint16_t>(get16(static_cast<Reg16>(p.reg)) + insn.values[0]) >> 8) & (flag::Y | flag::X);
            }
            r.f = static_cast<std::uint8_t>(f);
            break;
        }
        case Mn::SetRes: {
            unsigned v = load(o1);
            v = p.flag_a ? (v | (1u << p.sub)) : (v & ~(1u << p.sub));
            store(o1, v);
            if (p.copy) set8(static_cast<Reg8>(p.copy - 1), v);
            break;
        }
        case Mn::JpReg: pc = get16(static_cast<Reg16>(p.reg)); break;
        case Mn::Jump:
            if (!p.conditional || cond(static_cast<Cond>(p.sub))) {
                if (p.flag_a) push(pc);
                CB_ASSERT(target.has_value(), "a jump with no target");
                pc = *target;
            }
            break;
        case Mn::Djnz:
            r.b = static_cast<std::uint8_t>(r.b - 1);
            if (r.b) {
                CB_ASSERT(target.has_value(), "DJNZ with no target");
                pc = *target;
            }
            break;
        case Mn::Ret:
            if (!p.conditional || cond(static_cast<Cond>(p.sub))) pc = pop();
            break;
        case Mn::Reti:
            r.iff1 = r.iff2;
            pc = pop();
            break;
        case Mn::Rst:
            push(pc);
            CB_ASSERT(target.has_value(), "RST with no target");
            pc = *target;
            break;
        case Mn::Halt: r.halted = true; break;
        case Mn::DiEi: r.iff1 = r.iff2 = p.flag_a; break;
        case Mn::Im: r.im = p.sub; break;
        case Mn::Cpl:
            r.a ^= 0xFF;
            r.f = static_cast<std::uint8_t>((r.f & (flag::S | flag::Z | flag::P | flag::C)) | flag::H | flag::N | (r.a & (flag::Y | flag::X)));
            break;
        case Mn::Neg: r.a = sub8(0, r.a); break;
        case Mn::Scf: r.f = static_cast<std::uint8_t>((r.f & (flag::S | flag::Z | flag::P)) | flag::C | (r.a & (flag::Y | flag::X))); break;
        case Mn::Ccf:
            r.f = static_cast<std::uint8_t>(((r.f & (flag::S | flag::Z | flag::P | flag::C)) | ((r.f & flag::C) ? flag::H : 0) | (r.a & (flag::Y | flag::X))) ^ flag::C);
            break;
        case Mn::Daa: {
            const unsigned a = r.a, f = r.f;
            unsigned adjust = 0, carry = f & flag::C;
            if ((f & flag::H) || (a & 0x0F) > 9) adjust |= 0x06;
            if (carry || a > 0x99) {
                adjust |= 0x60;
                carry = flag::C;
            }
            const unsigned res = ((f & flag::N) ? a - adjust : a + adjust) & 0xFF;
            const unsigned h = (a ^ res) & flag::H;
            r.a = static_cast<std::uint8_t>(res);
            r.f = static_cast<std::uint8_t>(kSzp[res] | (f & flag::N) | h | carry);
            break;
        }
        case Mn::Rld: {
            const std::uint16_t hl = r.hl();
            const unsigned v = rd(hl);
            if (p.flag_a) {
                wr(hl, (v << 4) | (r.a & 0x0F));
                r.a = static_cast<std::uint8_t>((r.a & 0xF0) | (v >> 4));
            } else {
                wr(hl, (v >> 4) | ((r.a & 0x0F) << 4));
                r.a = static_cast<std::uint8_t>((r.a & 0xF0) | (v & 0x0F));
            }
            r.f = static_cast<std::uint8_t>((r.f & flag::C) | kSzp[r.a]);
            break;
        }
        case Mn::InA: r.a = bus.in(static_cast<std::uint16_t>(insn.values[0] | r.a << 8)); break;
        case Mn::InC: {
            const std::uint8_t v = bus.in(r.bc());
            if (p.flag_a) set8(static_cast<Reg8>(p.reg), v);
            r.f = static_cast<std::uint8_t>((r.f & flag::C) | kSzp[v]);
            break;
        }
        case Mn::OutA: bus.out(static_cast<std::uint16_t>(insn.values[0] | r.a << 8), r.a); break;
        case Mn::OutC: bus.out(r.bc(), p.flag_d ? 0 : get8(static_cast<Reg8>(p.reg))); break;
        case Mn::Ldx: {
            const int step = p.flag_a ? -1 : 1;
            const unsigned v = rd(r.hl());
            wr(r.de(), v);
            set16(Reg16::HL, r.hl() + step);
            set16(Reg16::DE, r.de() + step);
            const std::uint16_t bc = static_cast<std::uint16_t>(r.bc() - 1);
            set16(Reg16::BC, bc);
            const unsigned n = (v + r.a) & 0xFF;
            r.f = static_cast<std::uint8_t>((r.f & (flag::S | flag::Z | flag::C)) | (bc ? flag::P : 0) | (n & flag::X) | ((n << 4) & flag::Y));
            if (p.flag_b && bc) pc = static_cast<std::uint16_t>(pc - 2);
            break;
        }
        case Mn::Cpx: {
            const int step = p.flag_a ? -1 : 1;
            const unsigned v = rd(r.hl());
            const unsigned res = (r.a - v) & 0xFF;
            const unsigned h = (r.a ^ v ^ res) & flag::H;
            set16(Reg16::HL, r.hl() + step);
            const std::uint16_t bc = static_cast<std::uint16_t>(r.bc() - 1);
            set16(Reg16::BC, bc);
            const unsigned n = (res - (h ? 1 : 0)) & 0xFF;
            r.f = static_cast<std::uint8_t>((r.f & flag::C) | flag::N | (kSz[res] & (flag::S | flag::Z)) | h | (bc ? flag::P : 0) | (n & flag::X) | ((n << 4) & flag::Y));
            if (p.flag_b && bc && res != 0) pc = static_cast<std::uint16_t>(pc - 2);
            break;
        }
        case Mn::IoBlock: {
            const int step = p.flag_a ? -1 : 1;
            const std::uint16_t hl = r.hl();
            unsigned v = 0;
            if (p.flag_c) {   // INI / IND ...
                v = bus.in(r.bc());
                wr(hl, v);
                r.b = static_cast<std::uint8_t>(r.b - 1);
            } else {          // OUTI / OUTD ...
                v = rd(hl);
                r.b = static_cast<std::uint8_t>(r.b - 1);
                bus.out(r.bc(), static_cast<std::uint8_t>(v));
            }
            set16(Reg16::HL, hl + step);
            r.f = static_cast<std::uint8_t>(kSz[r.b] | ((v & 0x80) ? flag::N : 0) | (r.f & flag::C));
            if (p.flag_b && r.b) pc = static_cast<std::uint16_t>(pc - 2);
            break;
        }
    }
}

// ---- Cpu ---------------------------------------------------------------------------------------------------------

Cpu::Cpu(Bus& bus) : bus_(&bus) {}
Cpu::~Cpu() = default;

std::optional<Fault> Cpu::step() {
    if (regs.halted) return Fault{Fault::Kind::Halt, regs.pc};
    const std::uint16_t pc = regs.pc;
    auto it = decoded_.find(pc);
    bool fresh = it == decoded_.end();
    if (!fresh) {
        // the bytes under it, read as the CPU fetches them (every one: a device sees each access); a change decodes it again
        const dec::Insn& known = it->second.insn;
        bool same = true;
        for (std::size_t i = 0; i < known.size; ++i) {
            if (bus_->read(static_cast<std::uint16_t>(pc + i)) != known.raw[i]) same = false;
        }
        fresh = !same;
    }
    if (fresh) {
        std::array<std::uint8_t, 4> code{};
        for (std::size_t i = 0; i < code.size(); ++i) code[i] = bus_->read(static_cast<std::uint16_t>(pc + i));
        std::optional<dec::Insn> d = dec::decode(code, 0, pc);
        if (!d) return Fault{Fault::Kind::Undecodable, pc};
        Decoded entry;
        entry.program = &program_of(d->pattern);
        entry.insn = std::move(*d);
        it = decoded_.insert_or_assign(pc, std::move(entry)).first;
    }
    const Decoded& dd = it->second;
    const dec::Insn& insn = dd.insn;
    const std::uint8_t first = insn.raw[0];
    const unsigned bump = (first == 0xDD || first == 0xFD || first == 0xCB || first == 0xED) ? 2u : 1u;   // R counts fetches
    regs.r = static_cast<std::uint8_t>((regs.r & 0x80) | ((regs.r + bump) & 0x7F));
    std::uint16_t next = static_cast<std::uint16_t>(pc + insn.size);
    regs.pc = next;
    Exec{regs, *bus_, insn, *dd.program}.run(next);
    regs.pc = next;
    ++steps;
    return std::nullopt;
}

// ---- running a function --------------------------------------------------------------------------------------------

std::string RunResult::exit() const {
    switch (end) {
        case End::Return: return "return";
        case End::Stopped: return "stopped: " + (fault ? fault->text() : std::string());
        case End::Limit: break;
    }
    return "limit";
}

RunResult run(Bus& bus, std::uint16_t entry, const RunOptions& options) {
    const std::size_t log_start = bus.log().size();
    Cpu cpu(bus);
    Registers& r = cpu.regs;
    const RegisterInit& init = options.regs;
    if (init.a) r.a = *init.a;
    if (init.f) r.f = *init.f;
    if (init.b) r.b = *init.b;
    if (init.c) r.c = *init.c;
    if (init.d) r.d = *init.d;
    if (init.e) r.e = *init.e;
    if (init.h) r.h = *init.h;
    if (init.l) r.l = *init.l;
    if (init.ix) r.ix = *init.ix;
    if (init.iy) r.iy = *init.iy;
    r.sp = options.sp;
    {   // the return address goes through the bus like any push
        r.sp = static_cast<std::uint16_t>(r.sp - 2);
        bus.write(r.sp, static_cast<std::uint8_t>(kReturnAddress));
        bus.write(static_cast<std::uint16_t>(r.sp + 1), static_cast<std::uint8_t>(kReturnAddress >> 8));
    }
    r.pc = entry;
    RunResult out;
    out.end = End::Limit;
    while (cpu.steps < options.max_steps) {
        if (r.pc == kReturnAddress) {
            out.end = End::Return;
            break;
        }
        if (auto fault = cpu.step()) {
            out.end = End::Stopped;
            out.fault = fault;
            break;
        }
    }
    out.regs = r;
    out.steps = cpu.steps;
    out.io.assign(bus.log().begin() + static_cast<std::ptrdiff_t>(log_start), bus.log().end());
    return out;
}

cb::emulate::Run record(const RunResult& result) {
    cb::emulate::Run run;
    run.exit = result.exit();
    const Registers& r = result.regs;
    run.results["A"] = static_cast<std::uint64_t>(r.a);
    run.results["F"] = static_cast<std::uint64_t>(r.f);
    run.results["BC"] = static_cast<std::uint64_t>(r.bc());
    run.results["DE"] = static_cast<std::uint64_t>(r.de());
    run.results["HL"] = static_cast<std::uint64_t>(r.hl());
    run.results["IX"] = static_cast<std::uint64_t>(r.ix);
    run.results["IY"] = static_cast<std::uint64_t>(r.iy);
    run.results["SP"] = static_cast<std::uint64_t>(r.sp);
    for (const BusAccess& a : result.io) {
        cb::emulate::IoAccess io;
        io.kind = std::string(to_text(a.kind));
        io.port = a.where;
        io.value = a.value;
        run.io.push_back(std::move(io));
    }
    run.steps = result.steps;
    run.truncated = result.end == End::Limit;
    run.endian = cb::emulate::Endian::Little;
    run.raw = result;
    return run;
}

cb::emulate::Run recorded(Bus& bus, std::uint16_t entry, const RunOptions& options) {
    return record(run(bus, entry, options));
}

}  // namespace cb::emulate::z80
