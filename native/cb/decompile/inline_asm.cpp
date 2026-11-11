#include "cb/decompile/inline_asm.hpp"

#include <algorithm>
#include <format>

#include "cb/core/capstone_api.hpp"
#include "cb/core/text.hpp"

namespace cb::decompile::inline_asm {

namespace {

constexpr std::array<std::string_view, 19> kStringOps = {"movsb", "movsw", "movsd", "lodsb", "lodsw", "lodsd", "stosb",
                                                         "stosw", "stosd", "scasb", "scasw", "scasd", "cmpsb", "cmpsw",
                                                         "cmpsd", "insb",  "insw",  "outsb", "outsw"};
constexpr std::array<std::string_view, 21> kJumps = {"jmp", "je",  "jne", "jb",  "jae", "jbe",   "ja",
                                                     "jl",  "jge", "jle", "jg",  "js",  "jns",   "jo",
                                                     "jno", "jp",  "jnp", "jcxz", "loop", "loope", "loopne"};

bool is_reg32(std::uint16_t reg) {
    switch (reg) {
        case X86_REG_EAX:
        case X86_REG_EBX:
        case X86_REG_ECX:
        case X86_REG_EDX:
        case X86_REG_ESI:
        case X86_REG_EDI:
        case X86_REG_EBP:
        case X86_REG_ESP: return true;
        default: return false;
    }
}

bool is_ret(std::string_view m) { return m == "ret" || m == "retf"; }

// The instruction `back` places from the end (0 = the last), or null when the function is not that long.
const Instruction* from_end(std::span<const Instruction> ins, std::size_t back) {
    return back < ins.size() ? &ins[ins.size() - 1 - back] : nullptr;
}

// An instruction written `mnemonic op_str` exactly.
bool is_insn(const Instruction* i, std::string_view mnemonic, std::string_view op_str) {
    return i && i->mnemonic == mnemonic && i->op_str == op_str;
}

std::string hex4(std::uint32_t v) { return std::format("{:04x}", v); }

Result<std::string> refuse(std::string why) { return NotAsmBody(std::move(why)); }

// The `ptr ` of an operand string that names a memory operand without a size: preceded by the start, a space or a
// comma. nullopt when there is none.
std::optional<std::size_t> unsized_ptr(std::string_view s) {
    for (std::size_t at = s.find("ptr "); at != std::string_view::npos; at = s.find("ptr ", at + 1)) {
        if (at == 0 || s[at - 1] == ' ' || s[at - 1] == ',') return at;
    }
    return std::nullopt;
}

}  // namespace

std::string_view Instruction::base_mnemonic() const {
    std::string_view m = text::rtrim(mnemonic);   // the last word, as split()[-1] has it
    std::size_t at = m.size();
    while (at > 0 && !text::is_space(m[at - 1])) --at;
    return m.substr(at);
}

std::span<const std::string_view> string_op_names() { return kStringOps; }
std::span<const std::string_view> jump_names() { return kJumps; }

bool is_string_op(std::string_view m) { return std::find(kStringOps.begin(), kStringOps.end(), m) != kStringOps.end(); }
bool is_jump(std::string_view m) { return std::find(kJumps.begin(), kJumps.end(), m) != kJumps.end(); }

bool is_386(const Instruction& i) {
    for (const Operand& op : i.operands) {
        if (op.kind == OperandKind::Reg && is_reg32(op.reg)) return true;
        if (op.kind == OperandKind::Mem && (is_reg32(op.mem.base) || is_reg32(op.mem.index))) return true;
    }
    if (!i.bytes.empty() && i.bytes[0] == 0x66) return true;   // the operand-size prefix
    // 0x0f, 0x80..0x8f: a near conditional jump
    return i.bytes.size() > 1 && i.bytes[0] == 0x0f && i.bytes[1] >= 0x80 && i.bytes[1] <= 0x8f;
}

std::optional<Prologue> find_prologue(std::span<const Instruction> ins) {
    Prologue p;
    if (!ins.empty() && ins[0].mnemonic == "enter") {
        if (ins[0].operands.empty()) return std::nullopt;
        p.frame = ins[0].operands[0].imm;
        p.taken = 1;
    } else if (ins.size() > 1 && is_insn(&ins[0], "push", "bp") && is_insn(&ins[1], "mov", "bp, sp")) {
        p.taken = 2;
        if (ins.size() > 2 && ins[2].mnemonic == "sub" && ins[2].op_str.starts_with("sp, ") && ins[2].operands.size() > 1) {
            p.frame = ins[2].operands[1].imm;
            p.taken = 3;
        }
    } else {
        return std::nullopt;
    }
    // __loadds: push ds / mov ax, DGROUP / mov ds, ax (DGROUP a fix-up)
    const std::size_t k = p.taken;
    if (ins.size() > k + 2 && ins[k].op_str == "ds" && ins[k].mnemonic == "push" && ins[k + 1].mnemonic == "mov" &&
        ins[k + 1].op_str.starts_with("ax, ") && is_insn(&ins[k + 2], "mov", "ds, ax")) {
        p.taken += 3;
        p.loads_ds = true;
    }
    return p;
}

std::optional<Epilogue> find_epilogue(std::span<const Instruction> ins, bool loads_ds) {
    if (ins.empty() || !is_ret(ins.back().mnemonic)) return std::nullopt;
    Epilogue e;
    e.far = ins.back().mnemonic == "retf";
    e.pops = ins.back().operands.empty() ? 0 : ins.back().operands[0].imm;
    e.taken = 1;
    const Instruction* before = from_end(ins, e.taken);
    if (before && before->mnemonic == "leave") {
        ++e.taken;
    } else if (ins.size() > e.taken + 1 && is_insn(before, "pop", "bp")) {
        ++e.taken;
        if (is_insn(from_end(ins, e.taken), "mov", "sp, bp")) ++e.taken;
    } else {
        return std::nullopt;
    }
    if (loads_ds && is_insn(from_end(ins, e.taken), "pop", "ds")) ++e.taken;
    return e;
}

std::string operands_text(const Instruction& i) {
    if (is_string_op(i.base_mnemonic())) return "";
    std::string s = i.op_str;
    if (i.mnemonic == "les" || i.mnemonic == "lds") {
        // the assembler wants `dword ptr` where the decoder wrote a bare `ptr` (a far pointer in memory)
        if (auto at = unsized_ptr(s)) s.insert(*at, "dword ");
    }
    // a direct memory operand: the module's data (DS: the inline assembler takes [n] for an immediate)
    for (const Operand& op : i.operands) {
        if (op.kind == OperandKind::Mem && op.mem.base == 0 && op.mem.index == 0 && op.mem.segment == 0) {
            const auto disp = static_cast<std::uint64_t>(op.mem.disp) & 0xFFFF;
            s = text::replace_all(s, std::format("[0x{:x}]", disp), std::format("ds:[0x{:x}]", disp));
        }
    }
    return s;
}

std::vector<Instruction> decode16(ByteView code, std::uint32_t address) {
    namespace cs = disasm::capstone;
    const cs::Api& api = cs::api();
    csh handle = 0;
    cs::check(api.cs_open(CS_ARCH_X86, CS_MODE_16, &handle), "cs_open");
    struct Closer {
        const cs::Api& api;
        csh& handle;
        ~Closer() { api.cs_close(&handle); }
    } closer{api, handle};
    cs::check(api.cs_option(handle, CS_OPT_DETAIL, CS_OPT_ON), "cs_option");

    std::vector<Instruction> out;
    cs_insn* insn = api.cs_malloc(handle);
    const std::uint8_t* at = code.data();
    std::size_t left = code.size();
    std::uint64_t pc = address;
    while (api.cs_disasm_iter(handle, &at, &left, &pc, insn)) {
        Instruction i;
        i.address = static_cast<std::uint32_t>(insn->address);
        i.size = static_cast<std::uint8_t>(insn->size);
        i.mnemonic = insn->mnemonic;
        i.op_str = insn->op_str;
        i.bytes.assign(insn->bytes, insn->bytes + insn->size);
        if (insn->detail) {
            const cs_x86& x = insn->detail->x86;
            std::copy(std::begin(x.prefix), std::end(x.prefix), i.prefix.begin());
            for (std::uint8_t k = 0; k < x.op_count; ++k) {
                const cs_x86_op& o = x.operands[k];
                Operand op;
                op.kind = o.type == X86_OP_REG ? OperandKind::Reg
                          : o.type == X86_OP_IMM ? OperandKind::Imm
                          : o.type == X86_OP_MEM ? OperandKind::Mem
                                                 : OperandKind::Invalid;
                op.reg = static_cast<std::uint16_t>(o.reg);
                op.imm = o.imm;
                op.mem.segment = static_cast<std::uint16_t>(o.mem.segment);
                op.mem.base = static_cast<std::uint16_t>(o.mem.base);
                op.mem.index = static_cast<std::uint16_t>(o.mem.index);
                op.mem.scale = static_cast<std::uint8_t>(o.mem.scale);
                op.mem.disp = o.mem.disp;
                i.operands.push_back(op);
            }
        }
        out.push_back(std::move(i));
    }
    api.cs_free(insn, 1);
    return out;
}

Result<std::string> whole_body(std::string_view name, ByteView code, std::uint32_t start, std::uint32_t end,
                               const Fixups& fixups, const NameAt& name_at, std::uint32_t seg, std::string_view result,
                               const Decoder& decoder) {
    // code[start:end], where a range past the segment is the part that exists
    const std::size_t from = std::min<std::size_t>(start, code.size());
    const std::size_t to = std::clamp<std::size_t>(end, from, code.size());
    ByteView function = code.subspan(from, to - from);

    std::vector<Instruction> ins;
    for (Instruction& i : (decoder ? decoder : Decoder(decode16))(function, start)) {
        const bool is_return = is_ret(i.mnemonic);
        ins.push_back(std::move(i));
        if (is_return) break;
    }
    if (ins.empty() || !is_ret(ins.back().mnemonic)) return refuse("no return found");
    auto prologue = find_prologue(ins);
    if (!prologue) return refuse("no frame of C 8's at its start");
    auto epilogue = find_epilogue(ins, prologue->loads_ds);
    if (!epilogue) return refuse("no frame of C 8's at its end");

    // body = ins[taken : len - left] (empty when the two frames meet)
    const std::size_t body_begin = std::min(prologue->taken, ins.size());
    const std::size_t body_end = std::max(body_begin, ins.size() - std::min(epilogue->taken, ins.size()));
    const std::span<const Instruction> body = std::span<const Instruction>(ins).subspan(body_begin, body_end - body_begin);

    const std::uint32_t first = ins.front().address;
    const std::uint32_t last = ins.back().address + ins.back().size;
    std::vector<std::uint32_t> targets;   // sorted and unique below
    for (const Instruction& i : body) {
        if (is_386(i)) {
            // (the inline assembler takes the 286's instructions: a 32-bit register, an operand-size prefix, a near
            // conditional jump are /G3's code -- C round a block, not the block)
            return refuse(std::format("386 code, which the compiler made ({} {} {})", hex4(i.address), i.mnemonic, i.op_str));
        }
        if (is_jump(i.mnemonic)) {
            if (i.operands.empty() || i.operands[0].kind != OperandKind::Imm) {
                return refuse(std::format("a jump through a register or memory ({})", hex4(i.address)));
            }
            const auto to_address = static_cast<std::uint32_t>(i.operands[0].imm & 0xFFFF);
            if (!(first <= to_address && to_address < last)) {
                return refuse(std::format("a jump out of the function ({})", hex4(i.address)));
            }
            targets.push_back(to_address);
        }
        if (i.mnemonic == "ret" || i.mnemonic == "retf" || i.mnemonic == "iret") {
            return refuse(std::format("a return in its middle ({})", hex4(i.address)));
        }
    }
    std::sort(targets.begin(), targets.end());
    targets.erase(std::unique(targets.begin(), targets.end()), targets.end());
    auto is_target = [&](std::uint32_t a) { return std::binary_search(targets.begin(), targets.end(), a); };

    std::vector<std::string> lines;
    for (const Instruction& i : body) {
        if (is_target(i.address)) lines.push_back("L_" + hex4(i.address) + ":");
        const std::string_view mn = i.base_mnemonic();   // (capstone says "rep stosw")
        std::vector<std::uint32_t> site;                  // the fix-ups that fall inside this instruction
        for (std::uint32_t a = i.address; a < i.address + i.size; ++a) {
            if (fixups.names.contains(a)) site.push_back(a);
        }
        if (is_jump(mn)) {
            lines.push_back(std::format("        {} L_{}", mn, hex4(static_cast<std::uint32_t>(i.operands[0].imm & 0xFFFF))));
            continue;
        }
        if (mn == "lcall" && !site.empty()) {
            std::optional<std::string> target = fixups.names.at(site[0]);
            if (auto selector = fixups.segments.find(site[0]); selector != fixups.segments.end() && i.operands.size() == 2) {
                // (a selector-only fix-up: the offset is in the instruction -- a far call within the module, its target
                // named by the segment the fix-up gives and that offset)
                target = selector->second && *selector->second != 0
                             ? name_at(*selector->second, static_cast<std::uint16_t>(i.operands[1].imm & 0xFFFF))
                             : std::nullopt;
            }
            if (!target || target->empty()) return refuse(std::format("a far call it cannot name ({})", hex4(i.address)));
            lines.push_back("        call " + *target);
            continue;
        }
        if (mn == "call" && !i.operands.empty() && i.operands[0].kind == OperandKind::Imm) {
            std::optional<std::string> target = name_at(seg, static_cast<std::uint16_t>(i.operands[0].imm & 0xFFFF));
            if (!target || target->empty()) return refuse(std::format("a near call it cannot name ({})", hex4(i.address)));
            lines.push_back("        call near ptr " + *target);
            continue;
        }
        if (!site.empty()) return refuse(std::format("a fix-up in {} ({})", mn, hex4(i.address)));
        if (mn == "lcall" || mn == "ljmp") return refuse(std::format("a far {} without a fix-up ({})", mn, hex4(i.address)));
        const std::string ops = operands_text(i);
        std::string prefix;
        if ((i.prefix[0] == 0xF2 || i.prefix[0] == 0xF3) && is_string_op(mn)) {
            const bool plain_rep = mn.starts_with("movs") || mn.starts_with("stos") || mn.starts_with("lods") ||
                                   mn.starts_with("ins") || mn.starts_with("outs");
            prefix = i.prefix[0] == 0xF3 ? (plain_rep ? "rep " : "repe ") : "repne ";
        }
        lines.push_back(std::format("        {}{}{}{}", prefix, mn, ops.empty() ? "" : " ", ops));
    }
    // (a jump to the frame's end: the block's)
    const std::uint32_t body_end_address = body.empty() ? first : body.back().address + body.back().size;
    for (std::uint32_t to_address : targets) {
        if (to_address >= body_end_address) lines.push_back("L_" + hex4(to_address) + ":");
    }

    std::vector<std::string> params;
    if (epilogue->pops != 0) {
        for (std::int64_t k = 0; k < epilogue->pops / 2; ++k) params.push_back("a" + std::to_string(k + 1));
    }
    const char* convention = epilogue->pops != 0 ? "__pascal" : "__cdecl";
    std::string out = std::format("{} {} {}{} {}({})", result, epilogue->far ? "__far" : "__near", convention,
                                  (prologue->loads_ds ? " __loadds" : ""), name, text::join(params, ", "));
    for (const std::string& p : params) out += "\nint2 " + p + ";";
    out += "\n{";
    if (prologue->frame != 0) out += std::format("\n  char codebreaker_frame[{}];", prologue->frame);
    out += "\n  _asm {";
    for (const std::string& l : lines) out += "\n" + l;
    out += "\n  }\n}";
    return out;
}

}  // namespace cb::decompile::inline_asm
