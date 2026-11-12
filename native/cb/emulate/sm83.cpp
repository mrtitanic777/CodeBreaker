// cb/emulate/sm83.cpp -- see sm83.hpp.
#include "cb/emulate/sm83.hpp"

#include <array>
#include <format>
#include <string_view>

#include "cb/core/error.hpp"
#include "cb/core/text.hpp"

namespace cb::emulate::sm83 {

namespace dec = cb::decompile::sm83;

// ---- a pattern compiled -------------------------------------------------------------------------------------------

namespace {

enum class Reg8 : std::uint8_t { A, B, C, D, E, H, L };
enum class Pair : std::uint8_t { AF, BC, DE, HL, SP };

// Where an operand's value is: a register, a pair, the instruction's own value (n, nn, s), memory through a pair or at
// an address the instruction holds, the $FF00 page through C, (HL) with HL stepped after, or a number in the pattern.
struct Operand {
    enum class Kind : std::uint8_t { None, Reg8, Pair, Value, Through, Absolute, HighC, HlPlus, HlMinus, Number };
    Kind kind = Kind::None;
    std::uint8_t index = 0;   // Reg8 or Pair
    int number = 0;           // Number
};

enum class Op : std::uint8_t {
    Nop, Stop, LdNnSp, LdHlSpS, AddSpS, Ld, Push, Pop, Alu, Add16, IncDec16, IncDec8, RotA, Rot, Bit, SetRes,
    Jump, JumpHl, Call, Ret, Reti, Rst, Halt, Interrupts, Cpl, Scf, Ccf, Daa,
};
enum class AluOp : std::uint8_t { Add, Adc, Sub, Sbc, And, Xor, Or, Cp };
enum class RotOp : std::uint8_t { Rlc, Rrc, Rl, Rr, Sla, Sra, Swap, Srl };
enum class Cond : std::uint8_t { NZ, Z, NC, C, Always };

}  // namespace

struct Program {
    Op op = Op::Nop;
    Operand first, second;        // the pattern's operands that hold a value (a condition is not one)
    AluOp alu = AluOp::Add;
    RotOp rot = RotOp::Rlc;
    Cond cc = Cond::Always;
    int bit = 0;                  // Bit, SetRes
    bool flag = false;            // IncDec16 / IncDec8: increment; SetRes: set; Interrupts: enable; Add16 unused
    Pair pair = Pair::HL;         // Push, Pop, IncDec16, Add16's source
};

namespace {

std::optional<Operand> parse_operand(std::string_view form) {
    using K = Operand::Kind;
    static const std::pair<std::string_view, Reg8> regs8[] = {{"A", Reg8::A}, {"B", Reg8::B}, {"C", Reg8::C}, {"D", Reg8::D},
                                                              {"E", Reg8::E}, {"H", Reg8::H}, {"L", Reg8::L}};
    static const std::pair<std::string_view, Pair> pairs[] = {{"AF", Pair::AF}, {"BC", Pair::BC}, {"DE", Pair::DE},
                                                              {"HL", Pair::HL}, {"SP", Pair::SP}};
    for (const auto& [name, r] : regs8) {
        if (form == name) return Operand{K::Reg8, static_cast<std::uint8_t>(r), 0};
    }
    for (const auto& [name, p] : pairs) {
        if (form == name) return Operand{K::Pair, static_cast<std::uint8_t>(p), 0};
    }
    if (form == "n" || form == "nn" || form == "s") return Operand{K::Value, 0, 0};
    if (form == "(BC)") return Operand{K::Through, static_cast<std::uint8_t>(Pair::BC), 0};
    if (form == "(DE)") return Operand{K::Through, static_cast<std::uint8_t>(Pair::DE), 0};
    if (form == "(HL)") return Operand{K::Through, static_cast<std::uint8_t>(Pair::HL), 0};
    if (form == "(nn)" || form == "(h)") return Operand{K::Absolute, 0, 0};
    if (form == "(C)") return Operand{K::HighC, 0, 0};
    if (form == "(HL+)") return Operand{K::HlPlus, 0, 0};
    if (form == "(HL-)") return Operand{K::HlMinus, 0, 0};
    if (!form.empty() && form.find_first_not_of("0123456789") == std::string_view::npos) {
        return Operand{K::Number, 0, *text::parse_int<int>(form)};
    }
    return std::nullopt;
}

std::optional<Cond> parse_condition(std::string_view form) {
    if (form == "NZ") return Cond::NZ;
    if (form == "Z") return Cond::Z;
    if (form == "NC") return Cond::NC;
    if (form == "C") return Cond::C;
    return std::nullopt;
}

Pair pair_named(std::string_view name) {
    if (name == "AF") return Pair::AF;
    if (name == "BC") return Pair::BC;
    if (name == "DE") return Pair::DE;
    if (name == "SP") return Pair::SP;
    return Pair::HL;
}

// The program of one pattern ("LD A,(HL+)", "JR NZ,e", "BIT 3,B"), or nullopt for one nothing here runs.
std::optional<Program> compile(std::string_view pattern) {
    Program p;
    const std::size_t space = pattern.find(' ');
    const std::string_view m = pattern.substr(0, space);
    std::vector<std::string_view> forms;
    if (space != std::string_view::npos) forms = text::split(pattern.substr(space + 1), ',');

    // the forms that hold a value, in order (the conditions of a jump are not operands)
    const bool jump_like = m == "JP" || m == "JR" || m == "CALL" || m == "RET";
    std::vector<Operand> operands;
    std::vector<std::string_view> operand_forms;
    for (std::string_view f : forms) {
        if (jump_like && parse_condition(f)) continue;
        operands.push_back(parse_operand(f).value_or(Operand{}));
        operand_forms.push_back(f);
    }
    auto operand = [&](std::size_t i) { return i < operands.size() ? operands[i] : Operand{}; };

    if (m == "NOP") return p;
    if (m == "STOP") { p.op = Op::Stop; return p; }
    if (pattern == "LD (nn),SP") { p.op = Op::LdNnSp; return p; }
    if (pattern == "LD HL,SP+s") { p.op = Op::LdHlSpS; return p; }
    if (pattern == "ADD SP,s") { p.op = Op::AddSpS; return p; }
    if (m == "LD" || m == "LDH") {
        p.op = Op::Ld;
        p.first = operand(0);
        p.second = operand(1);
        return p;
    }
    if (m == "PUSH" || m == "POP") {
        p.op = m == "PUSH" ? Op::Push : Op::Pop;
        p.pair = pair_named(forms.at(0));
        return p;
    }
    static const std::pair<std::string_view, AluOp> alus[] = {{"ADD", AluOp::Add}, {"ADC", AluOp::Adc}, {"SUB", AluOp::Sub},
                                                              {"SBC", AluOp::Sbc}, {"AND", AluOp::And}, {"XOR", AluOp::Xor},
                                                              {"OR", AluOp::Or},   {"CP", AluOp::Cp}};
    for (const auto& [name, op] : alus) {
        if (m != name) continue;
        if (pattern.starts_with("ADD HL,")) {
            p.op = Op::Add16;
            p.pair = pair_named(forms.at(1));
            return p;
        }
        p.op = Op::Alu;
        p.alu = op;
        p.first = operands.empty() ? Operand{} : operands.back();
        return p;
    }
    if (m == "INC" || m == "DEC") {
        p.flag = m == "INC";
        const std::string_view f = forms.at(0);
        if (f == "BC" || f == "DE" || f == "HL" || f == "SP") {
            p.op = Op::IncDec16;
            p.pair = pair_named(f);
        } else {
            p.op = Op::IncDec8;
            p.first = operand(0);
        }
        return p;
    }
    static const std::pair<std::string_view, RotOp> rotates[] = {{"RLC", RotOp::Rlc}, {"RRC", RotOp::Rrc}, {"RL", RotOp::Rl},
                                                                 {"RR", RotOp::Rr},   {"SLA", RotOp::Sla}, {"SRA", RotOp::Sra},
                                                                 {"SWAP", RotOp::Swap}, {"SRL", RotOp::Srl}};
    if (m == "RLCA" || m == "RRCA" || m == "RLA" || m == "RRA") {
        p.op = Op::RotA;
        p.rot = m == "RLCA" ? RotOp::Rlc : m == "RRCA" ? RotOp::Rrc : m == "RLA" ? RotOp::Rl : RotOp::Rr;
        return p;
    }
    for (const auto& [name, op] : rotates) {
        if (m == name) {
            p.op = Op::Rot;
            p.rot = op;
            p.first = operand(0);
            return p;
        }
    }
    if (m == "BIT" || m == "SET" || m == "RES") {
        p.op = m == "BIT" ? Op::Bit : Op::SetRes;
        p.flag = m == "SET";
        p.bit = *text::parse_int<int>(forms.at(0));
        p.first = operand(1);
        return p;
    }
    if (m == "JP" || m == "JR" || m == "CALL") {
        if (pattern == "JP HL") { p.op = Op::JumpHl; return p; }
        p.op = m == "CALL" ? Op::Call : Op::Jump;
        if (forms.size() == 2) p.cc = parse_condition(forms[0]).value_or(Cond::Always);
        return p;
    }
    if (m == "RET") {
        p.op = Op::Ret;
        if (!forms.empty()) p.cc = parse_condition(forms[0]).value_or(Cond::Always);
        return p;
    }
    if (m == "RETI") { p.op = Op::Reti; return p; }
    if (m == "RST") { p.op = Op::Rst; return p; }
    if (m == "HALT") { p.op = Op::Halt; return p; }
    if (m == "DI" || m == "EI") {
        p.op = Op::Interrupts;
        p.flag = m == "EI";
        return p;
    }
    if (m == "CPL") { p.op = Op::Cpl; return p; }
    if (m == "SCF") { p.op = Op::Scf; return p; }
    if (m == "CCF") { p.op = Op::Ccf; return p; }
    if (m == "DAA") { p.op = Op::Daa; return p; }
    return std::nullopt;
}

// Every pattern the decoder can produce, compiled once; shared by every Cpu.
const std::unordered_map<std::string, Program>& programs() {
    static const std::unordered_map<std::string, Program> table = [] {
        std::unordered_map<std::string, Program> t;
        for (dec::Page page : {dec::Page::Main, dec::Page::CB}) {
            for (unsigned opcode = 0; opcode < 256; ++opcode) {
                const std::string_view pattern = dec::pattern_of(page, static_cast<std::uint8_t>(opcode));
                if (pattern.empty() || t.contains(std::string(pattern))) continue;
                std::optional<Program> p = compile(pattern);
                CB_ASSERT(p.has_value(), "an SM83 pattern with no program");
                t.emplace(std::string(pattern), *p);
            }
        }
        return t;
    }();
    return table;
}

const Program& program_of(const std::string& pattern) {
    const auto& t = programs();
    const auto it = t.find(pattern);
    CB_ASSERT(it != t.end(), "an SM83 pattern with no program");
    return it->second;
}

// ---- the interpreter ----------------------------------------------------------------------------------------------

struct Exec {
    Registers& r;
    Bus& bus;
    const dec::Insn& insn;
    const Program& p;

    std::uint8_t rd(std::uint32_t a) { return bus.read(static_cast<std::uint16_t>(a)); }
    void wr(std::uint32_t a, std::uint32_t v) { bus.write(static_cast<std::uint16_t>(a), static_cast<std::uint8_t>(v)); }
    void push(std::uint16_t v) {
        r.sp = static_cast<std::uint16_t>(r.sp - 2);
        wr(r.sp, v);
        wr(r.sp + 1u, v >> 8);
    }
    std::uint16_t pop() {
        const std::uint16_t v = static_cast<std::uint16_t>(rd(r.sp) | rd(r.sp + 1u) << 8);
        r.sp = static_cast<std::uint16_t>(r.sp + 2);
        return v;
    }

    std::uint8_t& reg8(std::uint8_t i) {
        switch (static_cast<Reg8>(i)) {
            case Reg8::A: return r.a;
            case Reg8::B: return r.b;
            case Reg8::C: return r.c;
            case Reg8::D: return r.d;
            case Reg8::E: return r.e;
            case Reg8::H: return r.h;
            case Reg8::L: return r.l;
        }
        return r.a;
    }
    std::uint32_t get16(Pair pr) const {
        switch (pr) {
            case Pair::AF: return r.af();
            case Pair::BC: return r.bc();
            case Pair::DE: return r.de();
            case Pair::HL: return r.hl();
            case Pair::SP: return r.sp;
        }
        return 0;
    }
    void set16(Pair pr, std::uint32_t v) {
        const auto hi = static_cast<std::uint8_t>(v >> 8), lo = static_cast<std::uint8_t>(v);
        switch (pr) {
            case Pair::AF: r.a = hi; r.f = lo & 0xF0; break;   // F's low nibble is always zero
            case Pair::BC: r.b = hi; r.c = lo; break;
            case Pair::DE: r.d = hi; r.e = lo; break;
            case Pair::HL: r.h = hi; r.l = lo; break;
            case Pair::SP: r.sp = static_cast<std::uint16_t>(v); break;
        }
    }
    bool condition(Cond cc) const {
        switch (cc) {
            case Cond::NZ: return !(r.f & flag::Z);
            case Cond::Z: return (r.f & flag::Z) != 0;
            case Cond::NC: return !(r.f & flag::C);
            case Cond::C: return (r.f & flag::C) != 0;
            case Cond::Always: return true;
        }
        return true;
    }
    std::int32_t value() const { return insn.value.value_or(0); }

    // An operand's value (a pair's is 16 bits wide) and writing one.
    std::int32_t read(const Operand& o) {
        using K = Operand::Kind;
        switch (o.kind) {
            case K::Reg8: return reg8(o.index);
            case K::Pair: return static_cast<std::int32_t>(get16(static_cast<Pair>(o.index)));
            case K::Value: return value();
            case K::Through: return rd(get16(static_cast<Pair>(o.index)));
            case K::Absolute: return rd(static_cast<std::uint32_t>(value()));
            case K::HighC: return rd(0xFF00u + r.c);
            case K::HlPlus:
            case K::HlMinus: {
                const std::uint32_t hl = r.hl();
                set16(Pair::HL, o.kind == K::HlPlus ? hl + 1 : hl - 1);
                return rd(hl);
            }
            case K::Number: return o.number;
            case K::None: break;
        }
        CB_ASSERT(false, "an SM83 operand that cannot be read");
        return 0;
    }
    void write(const Operand& o, std::int32_t v) {
        using K = Operand::Kind;
        switch (o.kind) {
            case K::Reg8: reg8(o.index) = static_cast<std::uint8_t>(v); return;
            case K::Pair: set16(static_cast<Pair>(o.index), static_cast<std::uint32_t>(v)); return;
            case K::Through: wr(get16(static_cast<Pair>(o.index)), static_cast<std::uint32_t>(v)); return;
            case K::Absolute: wr(static_cast<std::uint32_t>(value()), static_cast<std::uint32_t>(v)); return;
            case K::HighC: wr(0xFF00u + r.c, static_cast<std::uint32_t>(v)); return;
            case K::HlPlus:
            case K::HlMinus: {
                const std::uint32_t hl = r.hl();
                set16(Pair::HL, o.kind == K::HlPlus ? hl + 1 : hl - 1);
                wr(hl, static_cast<std::uint32_t>(v));
                return;
            }
            default: break;
        }
        CB_ASSERT(false, "an SM83 operand that cannot be written");
    }

    void do_alu(AluOp op, std::uint32_t v) {
        const std::uint32_t a = r.a, cy = (r.f & flag::C) ? 1u : 0u;
        switch (op) {
            case AluOp::Add:
            case AluOp::Adc: {
                const std::uint32_t k = op == AluOp::Adc ? cy : 0u;
                const std::uint32_t sum = a + v + k;
                r.f = static_cast<std::uint8_t>(((sum & 0xFF) == 0 ? flag::Z : 0) | ((a & 0xF) + (v & 0xF) + k > 0xF ? flag::H : 0) |
                                                (sum > 0xFF ? flag::C : 0));
                r.a = static_cast<std::uint8_t>(sum);
                break;
            }
            case AluOp::Sub:
            case AluOp::Sbc:
            case AluOp::Cp: {
                const std::uint32_t k = op == AluOp::Sbc ? cy : 0u;
                const std::int32_t diff = static_cast<std::int32_t>(a) - static_cast<std::int32_t>(v) - static_cast<std::int32_t>(k);
                r.f = static_cast<std::uint8_t>(flag::N | ((diff & 0xFF) == 0 ? flag::Z : 0) |
                                                (static_cast<std::int32_t>(a & 0xF) - static_cast<std::int32_t>(v & 0xF) - static_cast<std::int32_t>(k) < 0 ? flag::H : 0) |
                                                (diff < 0 ? flag::C : 0));
                if (op != AluOp::Cp) r.a = static_cast<std::uint8_t>(diff);
                break;
            }
            case AluOp::And:
                r.a = static_cast<std::uint8_t>(r.a & v);
                r.f = static_cast<std::uint8_t>((r.a == 0 ? flag::Z : 0) | flag::H);
                break;
            case AluOp::Xor:
                r.a = static_cast<std::uint8_t>(r.a ^ v);
                r.f = r.a == 0 ? flag::Z : 0;
                break;
            case AluOp::Or:
                r.a = static_cast<std::uint8_t>(r.a | v);
                r.f = r.a == 0 ? flag::Z : 0;
                break;
        }
    }

    std::uint8_t do_rot(RotOp op, std::uint32_t v) {
        const std::uint32_t cy = (r.f & flag::C) ? 1u : 0u;
        std::uint32_t out, carry;
        switch (op) {
            case RotOp::Rlc: out = ((v << 1) | (v >> 7)) & 0xFF; carry = v >> 7; break;
            case RotOp::Rrc: out = ((v >> 1) | (v << 7)) & 0xFF; carry = v & 1; break;
            case RotOp::Rl: out = ((v << 1) | cy) & 0xFF; carry = v >> 7; break;
            case RotOp::Rr: out = (v >> 1) | (cy << 7); carry = v & 1; break;
            case RotOp::Sla: out = (v << 1) & 0xFF; carry = v >> 7; break;
            case RotOp::Sra: out = (v >> 1) | (v & 0x80); carry = v & 1; break;
            case RotOp::Swap: out = ((v << 4) | (v >> 4)) & 0xFF; carry = 0; break;
            default: out = v >> 1; carry = v & 1; break;   // Srl
        }
        r.f = static_cast<std::uint8_t>((out == 0 ? flag::Z : 0) | (carry ? flag::C : 0));
        return static_cast<std::uint8_t>(out);
    }

    // SP plus a signed byte: H and C from the low byte's unsigned add, Z and N clear.
    std::uint16_t sp_plus(std::int32_t e) {
        const std::uint32_t sp = r.sp, ue = static_cast<std::uint32_t>(e);
        r.f = static_cast<std::uint8_t>(((sp & 0xF) + (ue & 0xF) > 0xF ? flag::H : 0) | ((sp & 0xFF) + (ue & 0xFF) > 0xFF ? flag::C : 0));
        return static_cast<std::uint16_t>(sp + ue);
    }

    std::optional<Fault> run() {
        switch (p.op) {
            case Op::Nop: break;
            case Op::Stop: return Fault{Fault::Kind::Stop, static_cast<std::uint16_t>(insn.pc)};
            case Op::LdNnSp:
                wr(static_cast<std::uint32_t>(value()), r.sp);
                wr(static_cast<std::uint32_t>(value()) + 1u, r.sp >> 8);
                break;
            case Op::LdHlSpS: set16(Pair::HL, sp_plus(value())); break;
            case Op::AddSpS: r.sp = sp_plus(value()); break;
            case Op::Ld: write(p.first, read(p.second)); break;
            case Op::Push: push(static_cast<std::uint16_t>(get16(p.pair))); break;
            case Op::Pop: set16(p.pair, pop()); break;
            case Op::Alu: do_alu(p.alu, static_cast<std::uint32_t>(read(p.first))); break;
            case Op::Add16: {
                const std::uint32_t x = r.hl(), y = get16(p.pair), sum = x + y;
                r.f = static_cast<std::uint8_t>((r.f & flag::Z) | ((x & 0xFFF) + (y & 0xFFF) > 0xFFF ? flag::H : 0) |
                                                (sum > 0xFFFF ? flag::C : 0));
                set16(Pair::HL, sum);
                break;
            }
            case Op::IncDec16: set16(p.pair, get16(p.pair) + (p.flag ? 1u : 0xFFFFFFFFu)); break;
            case Op::IncDec8: {
                const std::uint32_t v = static_cast<std::uint32_t>(read(p.first));
                std::uint32_t out;
                if (p.flag) {
                    out = (v + 1) & 0xFF;
                    r.f = static_cast<std::uint8_t>((r.f & flag::C) | (out == 0 ? flag::Z : 0) | ((v & 0xF) == 0xF ? flag::H : 0));
                } else {
                    out = (v - 1) & 0xFF;
                    r.f = static_cast<std::uint8_t>((r.f & flag::C) | flag::N | (out == 0 ? flag::Z : 0) | ((v & 0xF) == 0 ? flag::H : 0));
                }
                write(p.first, static_cast<std::int32_t>(out));
                break;
            }
            case Op::RotA:
                r.a = do_rot(p.rot, r.a);
                r.f &= flag::C;   // Z is always cleared
                break;
            case Op::Rot: write(p.first, do_rot(p.rot, static_cast<std::uint32_t>(read(p.first)))); break;
            case Op::Bit:
                r.f = static_cast<std::uint8_t>((r.f & flag::C) | flag::H | ((read(p.first) & (1 << p.bit)) ? 0 : flag::Z));
                break;
            case Op::SetRes: {
                const std::int32_t v = read(p.first);
                write(p.first, p.flag ? (v | (1 << p.bit)) : (v & ~(1 << p.bit)));
                break;
            }
            case Op::JumpHl: r.pc = static_cast<std::uint16_t>(r.hl()); break;
            case Op::Jump:
            case Op::Call:
                if (condition(p.cc)) {
                    if (p.op == Op::Call) push(r.pc);
                    r.pc = insn.target.value_or(0);
                }
                break;
            case Op::Ret:
                if (condition(p.cc)) r.pc = pop();
                break;
            case Op::Reti:
                r.ime = true;
                r.pc = pop();
                break;
            case Op::Rst:
                push(r.pc);
                r.pc = insn.target.value_or(0);
                break;
            case Op::Halt: r.halted = true; break;
            case Op::Interrupts: r.ime = p.flag; break;
            case Op::Cpl:
                r.a ^= 0xFF;
                r.f = static_cast<std::uint8_t>((r.f & (flag::Z | flag::C)) | flag::N | flag::H);
                break;
            case Op::Scf: r.f = static_cast<std::uint8_t>((r.f & flag::Z) | flag::C); break;
            case Op::Ccf: r.f = static_cast<std::uint8_t>((r.f & flag::Z) | ((r.f & flag::C) ? 0 : flag::C)); break;
            case Op::Daa: {
                std::uint32_t a = r.a;
                const std::uint32_t f = r.f;
                std::uint32_t carry = f & flag::C;
                if (f & flag::N) {
                    if (f & flag::H) a = (a - 0x06) & 0xFF;
                    if (carry) a = (a - 0x60) & 0xFF;
                } else {
                    if ((f & flag::H) || (a & 0x0F) > 9) a += 0x06;
                    if (carry || a > 0x9F) {
                        a += 0x60;
                        carry = flag::C;
                    }
                }
                r.a = static_cast<std::uint8_t>(a);
                r.f = static_cast<std::uint8_t>((r.a == 0 ? flag::Z : 0) | (f & flag::N) | carry);   // H stays clear
                break;
            }
        }
        return std::nullopt;
    }
};

}  // namespace

std::string Fault::text() const {
    switch (kind) {
        case Kind::Halt: return "HALT";
        case Kind::Stop: return "STOP";
        case Kind::Undecodable: break;
    }
    return std::format("no instruction at {:04X}", pc);
}

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
        for (std::size_t i = 0; i < known.size; ++i) {
            if (bus_->read(static_cast<std::uint16_t>(pc + i)) != known.raw[i]) fresh = true;
        }
    }
    if (fresh) {
        std::array<std::uint8_t, 3> code{};
        for (std::size_t i = 0; i < code.size(); ++i) code[i] = bus_->read(static_cast<std::uint16_t>(pc + i));
        std::optional<dec::Insn> d = dec::decode(code, 0, pc);
        if (!d) return Fault{Fault::Kind::Undecodable, pc};
        Decoded entry;
        entry.program = &program_of(d->pattern);
        entry.insn = std::move(*d);
        it = decoded_.insert_or_assign(pc, std::move(entry)).first;
    }
    const Decoded& dd = it->second;
    regs.pc = static_cast<std::uint16_t>(pc + dd.insn.size);
    if (std::optional<Fault> fault = Exec{regs, *bus_, dd.insn, *dd.program}.run()) return fault;
    ++steps;
    return std::nullopt;
}

// ---- running a function -------------------------------------------------------------------------------------------

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
    if (init.f) r.f = *init.f & 0xF0;
    if (init.b) r.b = *init.b;
    if (init.c) r.c = *init.c;
    if (init.d) r.d = *init.d;
    if (init.e) r.e = *init.e;
    if (init.h) r.h = *init.h;
    if (init.l) r.l = *init.l;
    if (init.ime) r.ime = *init.ime;
    if (init.halted) r.halted = *init.halted;
    r.sp = options.sp;
    {   // the return address goes through the bus like any push
        r.sp = static_cast<std::uint16_t>(r.sp - 2);
        bus.write(r.sp, static_cast<std::uint8_t>(kReturnAddress));
        bus.write(static_cast<std::uint16_t>(r.sp + 1), static_cast<std::uint8_t>(kReturnAddress >> 8));
    }
    r.pc = entry;
    RunResult out;
    while (cpu.steps < options.max_steps) {
        if (r.pc == kReturnAddress) {
            out.end = End::Return;
            break;
        }
        if (std::optional<Fault> fault = cpu.step()) {
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
    run.results["SP"] = static_cast<std::uint64_t>(r.sp);
    for (const BusAccess& a : result.io) {
        cb::emulate::IoAccess io;
        io.kind = std::string(z80::to_text(a.kind));
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

cb::emulate::Run recorded(Bus& bus, std::uint16_t entry, const RunOptions& options) { return record(run(bus, entry, options)); }

}  // namespace cb::emulate::sm83
