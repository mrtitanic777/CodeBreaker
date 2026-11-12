// cb/invert/early_c_forward.cpp -- see early_c_forward.hpp.
#include "cb/invert/early_c_forward.hpp"

#include <format>
#include <initializer_list>

#include "cb/core/fs.hpp"
#include "cb/core/log.hpp"

namespace cb::invert::early_c_forward {
namespace {

// A quad that is malformed; a shape the model does not cover.
[[noreturn]] void bad_quad(const std::string& what, std::uint64_t at = 0) { throw FormatError("early_c_forward", at, what); }
[[noreturn]] void unmodelled(const std::string& what) { throw UnsupportedError("early_c_forward", 0, what); }

// The quad opcodes the model knows (the names of quad_dump's table).
enum Op : std::uint8_t {
    kAssign = 0x00,
    kMul = 0x10, kDiv = 0x11, kMod = 0x12, kAdd = 0x13, kSub = 0x14, kShl = 0x15, kShr = 0x16,
    kLt = 0x17, kLe = 0x18, kGt = 0x19, kGe = 0x1a, kEq = 0x1b, kNe = 0x1c,
    kAnd = 0x1d, kXor = 0x1e, kOr = 0x1f,
    kReturn = 0x24, kCondBranch = 0x2c, kLabel = 0x2e, kLoop = 0x2f, kWhileTest = 0x30, kLoopBack = 0x31,
};

// ax, [mem] and ax, imm16 forms of the ALU operations (mod=01 rm=110 [bp+d8] is the memory form + 0x46).
struct AluOps {
    std::uint8_t mem = 0, imm = 0;
};
std::optional<AluOps> alu_ops(std::uint8_t opcode) {
    switch (opcode) {
        case kAdd: return AluOps{0x03, 0x05};
        case kSub: return AluOps{0x2b, 0x2d};
        case kAnd: return AluOps{0x23, 0x25};
        case kOr: return AluOps{0x0b, 0x0d};
        case kXor: return AluOps{0x33, 0x35};
        default: return std::nullopt;
    }
}

// The short conditional jump that makes the 0/1 result of a relational, and the one that skips an `if` body when the
// condition does NOT hold.
std::optional<std::uint8_t> relational_jcc(std::uint8_t opcode) {
    switch (opcode) {
        case kLt: return std::uint8_t{0x7c};
        case kLe: return std::uint8_t{0x7e};
        case kGt: return std::uint8_t{0x7f};
        case kGe: return std::uint8_t{0x7d};
        case kEq: return std::uint8_t{0x74};
        case kNe: return std::uint8_t{0x75};
        default: return std::nullopt;
    }
}
std::uint8_t inverse_jcc(std::uint8_t opcode) {
    switch (opcode) {
        case kLt: return 0x7d;
        case kLe: return 0x7f;
        case kGt: return 0x7e;
        case kGe: return 0x7c;
        case kEq: return 0x75;
        default: return 0x74;   // kNe
    }
}

bool is_binary(std::uint8_t opcode) {
    return alu_ops(opcode) || relational_jcc(opcode) || opcode == kMul || opcode == kDiv || opcode == kMod ||
           opcode == kShl || opcode == kShr;
}

constexpr std::uint8_t kLongType = 4;   // an operand's type byte for `long`

// The frame of the function being modelled: its own id (the parameters' ids follow it), the bytes of locals the prologue
// reserves, and the stride of one parameter (2 for int, 4 for long).
struct Frame {
    int fn = 0;
    int size = 0;
    int stride = 2;
    bool is_param(int id) const { return id > fn; }
    int offset(int id) const { return size + 4 + stride * (id - fn - 1); }   // [bp+offset]
};

// A displacement as a byte of an instruction.
std::uint8_t disp(int v) {
    if (v < 0 || v > 0xff) unmodelled(std::format("frame offset {} does not fit a byte", v));
    return static_cast<std::uint8_t>(v);
}

int id_of(const Operand& op) {
    if (op.kind != Operand::Kind::Node && op.kind != Operand::Kind::Deref) bad_quad("operand names no variable");
    return op.id;
}
int param_offset(const Operand& op, const Frame& f) {
    if (op.kind != Operand::Kind::Node || !f.is_param(op.id)) unmodelled("operand is not a parameter");
    return f.offset(op.id);
}

void put(Bytes& out, std::initializer_list<int> bytes) {
    for (const int b : bytes) out.push_back(static_cast<std::uint8_t>(b));
}
void put_u16(Bytes& out, unsigned v) { put(out, {static_cast<int>(v & 0xff), static_cast<int>((v >> 8) & 0xff)}); }

// ---- the actions: load ax, store ax, ax <op>= operand ---------------------------------------------------------------
void load_ax(Bytes& out, const Operand& op, const Frame& f) {
    switch (op.kind) {
        case Operand::Kind::Const:
            if (op.value == 0) {
                put(out, {0x33, 0xc0});
            } else {
                put(out, {0xb8});
                put_u16(out, op.value);
            }
            return;
        case Operand::Kind::Node:
            if (f.is_param(op.id)) {
                put(out, {0x8b, 0x46, disp(f.offset(op.id))});
            } else {
                put(out, {0xa1, 0x00, 0x00});   // the global's address is a relocation
            }
            return;
        case Operand::Kind::Deref:   // the pointer is a parameter: into si, then through [si] or [si+field]
            put(out, {0x8b, 0x76, disp(f.offset(op.id))});
            if (op.field == 0) put(out, {0x8b, 0x04});
            else put(out, {0x8b, 0x44, op.field});
            return;
        case Operand::Kind::Temp: return;   // already in ax
    }
}

void store_ax(Bytes& out, const Operand& op, const Frame& f) {
    switch (op.kind) {
        case Operand::Kind::Node:
            if (f.is_param(op.id)) put(out, {0x89, 0x46, disp(f.offset(op.id))});
            else put(out, {0xa3, 0x00, 0x00});
            return;
        case Operand::Kind::Deref:
            put(out, {0x8b, 0x76, disp(f.offset(op.id))});
            if (op.field == 0) put(out, {0x89, 0x04});
            else put(out, {0x89, 0x44, op.field});
            return;
        default: unmodelled("store to a constant or a temporary");
    }
}

void alu_ax(Bytes& out, std::uint8_t opcode, const Operand& op, const Frame& f) {
    const AluOps alu = *alu_ops(opcode);
    if (op.kind == Operand::Kind::Const) {
        put(out, {alu.imm});
        put_u16(out, op.value);
    } else if (op.kind == Operand::Kind::Node && f.is_param(op.id)) {
        put(out, {alu.mem, 0x46, disp(f.offset(op.id))});
    } else if (op.kind == Operand::Kind::Node) {
        put(out, {alu.mem, 0x06, 0x00, 0x00});   // ax, [global]
    } else {
        unmodelled("ALU operand is neither a constant nor a variable");
    }
}

// 32-bit arithmetic: the variables are longs on the frame, lo word at the offset, hi word 2 above.
void binop_long(Bytes& out, std::uint8_t opcode, const Operand& a, const Operand& b, const Frame& f) {
    const int af = f.offset(id_of(a)), bf = f.offset(id_of(b));
    switch (opcode) {
        case kAdd:
            put(out, {0x8b, 0x46, disp(af), 0x03, 0x46, disp(bf), 0x8b, 0x5e, disp(af + 2), 0x13, 0x5e, disp(bf + 2)});
            return;
        case kSub:
            put(out, {0x8b, 0x46, disp(af), 0x2b, 0x46, disp(bf), 0x8b, 0x5e, disp(af + 2), 0x1b, 0x5e, disp(bf + 2)});
            return;
        case kMul:
        case kDiv:   // through the runtime helper (a call, its relocation left as zeros), the result moved to ax
            put(out, {0x8b, 0x46, disp(af + 2), 0x8b, 0x5e, disp(af), 0x8b, 0x4e, disp(bf + 2), 0x8b, 0x56, disp(bf), 0xe8, 0x00,
                      0x00, 0x8b, 0xc3});
            return;
        default: unmodelled(std::format("long operation {:02x}", opcode));
    }
}

void binop(Bytes& out, std::uint8_t opcode, const Operand& a, const Operand& b, const Frame& f) {
    if (alu_ops(opcode)) {
        load_ax(out, a, f);
        alu_ax(out, opcode, b, f);
        return;
    }
    // everything else works on two parameters in place
    const int af = param_offset(a, f), bf = param_offset(b, f);
    if (const auto jcc = relational_jcc(opcode)) {
        // cmp; jcc +4; xor bx,bx; jmp +3; xor bx,bx; inc bx; mov ax,bx   (the 0/1 value)
        put(out, {0x8b, 0x46, disp(af), 0x3b, 0x46, disp(bf), *jcc, 0x04, 0x33, 0xdb, 0xeb, 0x03, 0x33, 0xdb, 0x43, 0x8b, 0xc3});
        return;
    }
    switch (opcode) {
        case kMul: put(out, {0x8b, 0x46, disp(af), 0xf7, 0x6e, disp(bf)}); return;
        case kDiv: put(out, {0x8b, 0x46, disp(af), 0x99, 0xf7, 0x7e, disp(bf)}); return;
        case kMod: put(out, {0x8b, 0x46, disp(af), 0x99, 0xf7, 0x7e, disp(bf), 0x8b, 0xc2}); return;
        case kShl: put(out, {0x8b, 0x4e, disp(bf), 0x8b, 0x46, disp(af), 0xd3, 0xe0}); return;
        case kShr: put(out, {0x8b, 0x4e, disp(bf), 0x8b, 0x46, disp(af), 0xd3, 0xf8}); return;
        default: unmodelled(std::format("operation {:02x}", opcode));
    }
}

// ---- the function: items with jumps by label, resolved at the end ------------------------------------------------------
struct Item {
    enum class Kind { Code, Label, Jump } kind = Kind::Code;
    Bytes code;               // Code
    int label = 0;            // Label (defined here), Jump (the target)
    std::uint8_t opcode = 0;  // Jump: the short jump's opcode
};

// Two passes: the position of each label, then the short jumps' displacements.
Bytes resolve(const std::vector<Item>& items) {
    std::vector<std::optional<std::size_t>> position;
    auto at = [&](int label) -> std::optional<std::size_t>& {
        if (position.size() <= static_cast<std::size_t>(label)) position.resize(static_cast<std::size_t>(label) + 1);
        return position[static_cast<std::size_t>(label)];
    };
    std::size_t offset = 0;
    for (const Item& item : items) {
        switch (item.kind) {
            case Item::Kind::Code: offset += item.code.size(); break;
            case Item::Kind::Label: at(item.label) = offset; break;
            case Item::Kind::Jump: offset += 2; break;
        }
    }
    Bytes out;
    for (const Item& item : items) {
        switch (item.kind) {
            case Item::Kind::Code: append(out, item.code); break;
            case Item::Kind::Label: break;
            case Item::Kind::Jump: {
                const auto target = at(item.label);
                if (!target) bad_quad("a jump to a label that is never defined");
                const long rel = static_cast<long>(*target) - static_cast<long>(out.size() + 2);
                if (rel < -128 || rel > 127) unmodelled("jump out of the rel8 range");
                put(out, {item.opcode, static_cast<int>(static_cast<std::uint8_t>(rel))});
                break;
            }
        }
    }
    return out;
}

Item code_item(Bytes code) { return Item{Item::Kind::Code, std::move(code), 0, 0}; }
Item label_item(int label) { return Item{Item::Kind::Label, {}, label, 0}; }
Item jump_item(std::uint8_t opcode, int label) { return Item{Item::Kind::Jump, {}, label, opcode}; }

int pop_label(std::vector<int>& stack) {
    if (stack.empty()) bad_quad("a label with no open structure");
    const int label = stack.back();
    stack.pop_back();
    return label;
}

const Operand& operand_at(const std::vector<Operand>& ops, std::size_t i) {
    if (i >= ops.size()) bad_quad("a quad is missing an operand");
    return ops[i];
}

Bytes model_function(const quad_align::QuadFunction& function) {
    if (function.empty()) bad_quad("a function with no quads");
    const int fn = parse_funcbegin(function[0].payload);
    const std::span<const quad_align::Quad> body = std::span(function).subspan(1);

    // The frame: a function that does arithmetic keeps its temporaries in a 2-byte (4 with longs) frame, and the
    // parameters are 4 bytes wide throughout when any operand is a long.
    bool long_mode = false, has_binary = false;
    for (const auto& q : body) {
        if (!is_binary(q.opcode)) continue;
        has_binary = true;
        for (const Operand& o : parse_operands(q.payload)) long_mode = long_mode || (o.kind == Operand::Kind::Node && o.type == kLongType);
    }
    Frame f{fn, has_binary ? (long_mode ? 4 : 2) : 0, long_mode ? 4 : 2};

    Bytes epilogue;
    if (f.size) put(epilogue, {0x83, 0xc4, disp(f.size)});   // add sp,N
    put(epilogue, {0x5d, 0xc3});                               // pop bp; ret

    std::vector<Item> items;
    {
        Bytes prologue;
        put(prologue, {0x55});   // push bp
        // sub sp,N; the stack probe (cmp sp,[limit]; ja +3; jmp <overflow handler>)
        if (f.size) put(prologue, {0x83, 0xec, disp(f.size), 0x3b, 0x26, 0x00, 0x00, 0x77, 0x03, 0xe9, 0x00, 0x00});
        put(prologue, {0x8b, 0xec});   // mov bp,sp
        items.push_back(code_item(std::move(prologue)));
    }

    std::vector<int> open_ifs;     // labels of the `if` bodies awaiting their LABEL quad
    std::vector<int> open_loops;   // per loop: its top label, then its exit label
    int next_label = 0;

    for (std::size_t i = 0; i < body.size(); ++i) {
        const std::uint8_t opcode = body[i].opcode;
        // only the quads the model reads have their operands parsed (the others, calls and jumps among them, may carry
        // payloads this parser does not describe)
        const bool reads_operands = is_binary(opcode) || opcode == kAssign || opcode == kWhileTest || opcode == kReturn;
        const std::vector<Operand> ops = reads_operands ? parse_operands(body[i].payload) : std::vector<Operand>{};

        // x = x + 1 / x = x - 1 as the assignment back to the same parameter: inc/dec [x]
        if ((opcode == kAdd || opcode == kSub) && ops.size() >= 2 && ops[0].kind == Operand::Kind::Node && f.is_param(ops[0].id) &&
            ops[1].kind == Operand::Kind::Const && ops[1].value == 1 && i + 1 < body.size() && body[i + 1].opcode == kAssign) {
            const std::vector<Operand> assign = parse_operands(body[i + 1].payload);
            if (assign.size() >= 2 && assign[1].kind == Operand::Kind::Node && assign[1].id == ops[0].id) {
                Bytes code;
                put(code, {0xff, opcode == kAdd ? 0x46 : 0x4e, disp(f.offset(ops[0].id))});
                items.push_back(code_item(std::move(code)));
                ++i;
                continue;
            }
        }
        // a relational whose result only feeds the next CBRANCH: compare with the frame variable, skip the body if false
        if (relational_jcc(opcode) && i + 1 < body.size() && body[i + 1].opcode == kCondBranch) {
            Bytes code;
            load_ax(code, operand_at(ops, 0), f);
            put(code, {0x3b, 0x46, disp(f.offset(id_of(operand_at(ops, 1))))});
            items.push_back(code_item(std::move(code)));
            open_ifs.push_back(next_label);
            items.push_back(jump_item(inverse_jcc(opcode), next_label++));
            ++i;
            continue;
        }
        if (is_binary(opcode)) {
            Bytes code;
            if (long_mode) binop_long(code, opcode, operand_at(ops, 0), operand_at(ops, 1), f);
            else binop(code, opcode, operand_at(ops, 0), operand_at(ops, 1), f);
            items.push_back(code_item(std::move(code)));
            continue;
        }
        switch (opcode) {
            case kAssign: {
                Bytes code;
                load_ax(code, operand_at(ops, 0), f);
                store_ax(code, operand_at(ops, 1), f);
                items.push_back(code_item(std::move(code)));
                break;
            }
            case kLabel: items.push_back(label_item(pop_label(open_ifs))); break;
            case kLoop:   // the top of a loop
                open_loops.push_back(next_label);
                items.push_back(label_item(next_label++));
                break;
            case kWhileTest: {   // load the condition, test it, leave the loop if it is zero
                Bytes code;
                load_ax(code, operand_at(ops, 0), f);
                put(code, {0x85, 0xc0});
                items.push_back(code_item(std::move(code)));
                open_loops.push_back(next_label);
                items.push_back(jump_item(0x74, next_label++));
                break;
            }
            case kLoopBack: {   // jump to the top; the exit label follows
                const int exit = pop_label(open_loops);
                const int top = pop_label(open_loops);
                items.push_back(jump_item(0xeb, top));
                items.push_back(label_item(exit));
                break;
            }
            case kReturn: {
                Bytes code;
                if (!ops.empty() && ops[0].kind != Operand::Kind::Temp) load_ax(code, ops[0], f);
                append(code, epilogue);
                items.push_back(code_item(std::move(code)));
                break;
            }
            default: break;   // quads that generate no code of their own
        }
    }
    return resolve(items);
}

// Hex with a space between the bytes.
std::string spaced_hex(ByteView bytes) {
    std::string out;
    for (const std::uint8_t b : bytes) out += std::format("{}{:02x}", out.empty() ? "" : " ", b);
    return out;
}

}  // namespace

std::vector<Operand> parse_operands(ByteView p) {
    // Each operand starts with its kind byte; a 0x00 before the next operand is a one-byte separator. The list ends at the
    // first byte that starts no operand, and also where a variable, dereference or temporary would run past the end of the
    // payload (that is the quad's trailer, not an operand); only a constant cut short is an error.
    std::vector<Operand> ops;
    std::size_t i = 0;
    auto u16_at = [&](std::size_t at) { return static_cast<std::uint16_t>(p[at] | (p[at + 1] << 8)); };
    while (i < p.size()) {
        const std::uint8_t k = p[i];
        const std::size_t left = p.size() - i;
        const int next = left > 1 ? p[i + 1] : -1;
        if (k == 0x04 && next == 0x80) {   // constant: the value at +3, 7 bytes
            if (left < 5) bad_quad(std::format("constant at {} is cut short", i));
            Operand o;
            o.kind = Operand::Kind::Const;
            o.value = u16_at(i + 3);
            ops.push_back(o);
            i += 7;
        } else if (k == 0x0a && next == 0x00 && left >= 11) {   // a variable: id at +3, type at +5, 11 bytes
            Operand o;
            o.kind = Operand::Kind::Node;
            o.id = u16_at(i + 3);
            o.type = p[i + 5];
            ops.push_back(o);
            i += 11;
        } else if (k == 0x0a && next == 0x20 && left >= 11) {   // a dereference: id at +3, the field offset at +9, 11 bytes
            Operand o;
            o.kind = Operand::Kind::Deref;
            o.id = u16_at(i + 3);
            o.field = p[i + 9];
            ops.push_back(o);
            i += 11;
        } else if (k == 0x0e && left >= 15) {   // a temporary: 11 bytes and a 4-byte link back to its result
            ops.push_back(Operand{});
            i += 15;
        } else if (k == 0x00 && (next == 0x04 || next == 0x0a || next == 0x0e)) {
            i += 1;   // the separator between operands
        } else {
            break;
        }
    }
    return ops;
}

std::uint16_t parse_funcbegin(ByteView payload) {
    if (payload.size() < 5) bad_quad("FUNC-BEGIN payload too short");
    return static_cast<std::uint16_t>(payload[3] | (payload[4] << 8));
}

Result<Bytes> model(const quad_align::QuadFunction& function) {
    // the isolation boundary: one function the model cannot handle is that function's answer, not the run's
    try {
        return model_function(function);
    } catch (const Error& e) {
        return Failure(e.clone());
    }
}

std::vector<Check> check(ByteView quad_file, ByteView object) {
    const std::vector<quad_align::QuadFunction> quads = quad_align::parse_quads(quad_file);
    std::vector<Check> out;
    for (const quad_align::Aligned& f : quad_align::align(quads, quad_align::parse_obj_funcs(object))) {
        Check c{f.name, f.code, std::nullopt};
        if (!f.quads.empty()) {
            if (Result<Bytes> predicted = model(f.quads); predicted.ok()) c.predicted = std::move(predicted).value();
        }
        out.push_back(std::move(c));
    }
    return out;
}

std::string format_checks(const std::vector<Check>& checks) {
    std::string out;
    std::size_t exact = 0;
    for (const Check& c : checks) {
        const bool ok = c.exact();
        exact += ok;
        out += std::format("  {:<7} {:<5}", c.name, ok ? "OK" : "FAIL");
        if (!ok) {
            out += std::format("  pred {}\n            orig {}", spaced_hex(c.predicted.value_or(Bytes{})), spaced_hex(c.original));
        }
        out += '\n';
    }
    out += std::format("\nMC2 FORWARD MODEL: {}/{} BYTE-EXACT\n", exact, checks.size());
    return out;
}

int command(std::span<const std::string> args) {
    if (args.size() < 2) throw UsageError("usage: early_c_forward <quads> <object>");
    out_text(format_checks(check(fs::read_file(fs::from_text(args[0])), fs::read_file(fs::from_text(args[1])))));
    return 0;
}

}  // namespace cb::invert::early_c_forward
