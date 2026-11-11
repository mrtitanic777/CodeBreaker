// codebreaker/decompile/inline_asm.cpp -- typed C++ port of codebreaker.decompile.inline_asm.

#include "codebreaker/decompile/inline_asm.hpp"

#include <algorithm>
#include <cstdio>
#include <set>
#include <vector>

#include <capstone/capstone.h>

#include "codebreaker/platforms/package.hpp"
#include "py_os.hpp"

namespace codebreaker::decompile::inline_asm {

const py::set<py::str> _STRING = py::set<py::str>{
    py::str("movsb"), py::str("movsw"), py::str("movsd"), py::str("lodsb"), py::str("lodsw"), py::str("lodsd"),
    py::str("stosb"), py::str("stosw"), py::str("stosd"), py::str("scasb"), py::str("scasw"), py::str("scasd"),
    py::str("cmpsb"), py::str("cmpsw"), py::str("cmpsd"), py::str("insb"), py::str("insw"), py::str("outsb"),
    py::str("outsw")};
const py::set<int64_t> _REGS32 = py::set<int64_t>{X86_REG_EAX, X86_REG_EBX, X86_REG_ECX, X86_REG_EDX,
                                                  X86_REG_ESI, X86_REG_EDI, X86_REG_EBP, X86_REG_ESP};
const py::set<py::str> _JUMPS = py::set<py::str>{
    py::str("jmp"), py::str("je"), py::str("jne"), py::str("jb"), py::str("jae"), py::str("jbe"), py::str("ja"),
    py::str("jl"), py::str("jge"), py::str("jle"), py::str("jg"), py::str("js"), py::str("jns"), py::str("jo"),
    py::str("jno"), py::str("jp"), py::str("jnp"), py::str("jcxz"), py::str("loop"), py::str("loope"),
    py::str("loopne")};

namespace {

// "%04x" % v
py::str hex04(uint64_t v) {
    char buf[40];
    std::snprintf(buf, sizeof buf, "%04llx", static_cast<unsigned long long>(v));
    return py::str(buf);
}

// mnemonic.split()[-1]: the last word (IndexError if there is none)
py::str last_word(const py::str& s) {
    const std::u32string& u = s.u32();
    size_t end = u.size();
    auto space = [](char32_t c) { return c == U' ' || c == U'\t' || c == U'\n' || c == U'\r' || c == U'\f' || c == U'\v'; };
    while (end > 0 && space(u[end - 1])) end--;
    if (end == 0) {
        throw py::IndexError("list index out of range");
    }
    size_t begin = end;
    while (begin > 0 && !space(u[begin - 1])) begin--;
    return py::str(u.substr(begin, end - begin));
}

// the clamped bounds of a Python slice [start:end] of a sequence of n items
std::pair<int64_t, int64_t> slice_bounds(int64_t n, int64_t start, int64_t end) {
    auto fix = [n](int64_t v) {
        if (v < 0) {
            v += n;
            if (v < 0) v = 0;
        } else if (v > n) {
            v = n;
        }
        return v;
    };
    int64_t a = fix(start), b = fix(end);
    return {a, b < a ? a : b};
}

using Result = std::tuple<std::optional<py::str>, std::optional<py::str>>;
Result failure(const py::str& why) { return Result(std::nullopt, why); }

py::str text(const char* s) { return py::str(s); }

// sep.join(items)
py::str join(const py::str& sep, const std::vector<py::str>& items) {
    std::u32string out;
    for (size_t k = 0; k < items.size(); k++) {
        if (k) out += sep.u32();
        out += items[k].u32();
    }
    return py::str(out);
}

}  // namespace

bool _386(const Instruction& i) {
    py::list<int64_t> regs;
    for (const Operand& op : i.operands) {
        if (op.type == X86_OP_REG) regs.append(op.reg);
    }
    for (const Operand& op : i.operands) {
        if (op.type == X86_OP_MEM) {
            regs.append(op.mem.base);
            regs.append(op.mem.index);
        }
    }
    for (int64_t r : regs) {
        if (_REGS32.contains(r)) return true;
    }
    const std::string& b = i.bytes.raw();
    if (!b.empty() && b[0] == 'f') return true;   // the operand-size prefix, 0x66
    if (!b.empty() && b[0] == '\x0f') {           // 0x0f, 0x80..0x8f: a near conditional jump
        if (b.size() < 2) {                        // (i.bytes[1] fails)
            throw py::IndexError("index out of range");
        }
        unsigned char second = static_cast<unsigned char>(b[1]);
        return 0x80 <= second && second <= 0x8f;
    }
    return false;
}

std::optional<std::tuple<int64_t, int64_t, bool>> _prologue(const py::list<Instruction>& ins) {
    int64_t k = 0, frame = 0;
    bool loadds = false;
    int64_t n = py::len(ins);
    if (n > 0 && ins[0].mnemonic == py::str("enter")) {
        frame = ins[0].operands[0].imm;
        k = 1;
    } else if (n > 1 && ins[0].mnemonic == py::str("push") && ins[0].op_str == py::str("bp") &&
               ins[1].mnemonic == py::str("mov") && ins[1].op_str == py::str("bp, sp")) {
        k = 2;
        if (n > 2 && ins[2].mnemonic == py::str("sub") && ins[2].op_str.startswith(py::str("sp, "))) {
            frame = ins[2].operands[1].imm;
            k = 3;
        }
    } else {
        return std::nullopt;
    }
    // __loadds: push ds / mov ax, DGROUP / mov ds, ax (DGROUP a fix-up)
    if (n > k + 2 && ins[k].op_str == py::str("ds") && ins[k].mnemonic == py::str("push") &&
        ins[k + 1].mnemonic == py::str("mov") && ins[k + 1].op_str.startswith(py::str("ax, ")) &&
        ins[k + 2].mnemonic == py::str("mov") && ins[k + 2].op_str == py::str("ds, ax")) {
        k += 3;
        loadds = true;
    }
    return std::make_tuple(k, frame, loadds);
}

std::optional<std::tuple<int64_t, bool, int64_t>> _epilogue(const py::list<Instruction>& ins, bool loadds) {
    int64_t n = py::len(ins);
    if (n == 0 || (ins[-1].mnemonic != py::str("ret") && ins[-1].mnemonic != py::str("retf"))) {
        return std::nullopt;
    }
    bool far = ins[-1].mnemonic == py::str("retf");
    int64_t pop = ins[-1].operands.empty() ? 0 : ins[-1].operands[0].imm;
    int64_t k = 1;
    if (n > k && ins[-1 - k].mnemonic == py::str("leave")) {
        k += 1;
    } else if (n > k + 1 && ins[-1 - k].mnemonic == py::str("pop") && ins[-1 - k].op_str == py::str("bp")) {
        k += 1;
        if (ins[-1 - k].mnemonic == py::str("mov") && ins[-1 - k].op_str == py::str("sp, bp")) {
            k += 1;
        }
    } else {
        return std::nullopt;
    }
    if (loadds && ins[-1 - k].mnemonic == py::str("pop") && ins[-1 - k].op_str == py::str("ds")) {
        k += 1;
    }
    return std::make_tuple(k, far, pop);
}

py::str _operands(const Instruction& i, const Fixups&, const NameAt&, int64_t) {
    py::str s = i.op_str;
    if (_STRING.contains(last_word(i.mnemonic))) {
        return py::str("");
    }
    if ((i.mnemonic == py::str("les") || i.mnemonic == py::str("lds")) &&
        py::re::search(py::str(R"((^|[ ,])ptr )"), s)) {
        s = py::re::sub(py::str(R"((^|[ ,] ?)ptr )"), py::str(R"(\g<1>dword ptr )"), s, 1);
    }
    // a direct memory operand: the module's data (DS: the inline assembler takes [n] for an immediate)
    for (const Operand& op : i.operands) {
        if (op.type == X86_OP_MEM && op.mem.base == 0 && op.mem.index == 0 && op.mem.segment == 0) {
            int64_t disp = op.mem.disp & 0xFFFF;
            char a[32], b[40];
            std::snprintf(a, sizeof a, "[0x%llx]", static_cast<unsigned long long>(disp));
            std::snprintf(b, sizeof b, "ds:[0x%llx]", static_cast<unsigned long long>(disp));
            s = s.replace(py::str(a), py::str(b));
        }
    }
    return s;
}

namespace {

template <class Data>
Result whole_body_(const Decoder& md, const py::str& name, const Data& code_all, int64_t start, int64_t end,
                   const Fixups& fixups, const NameAt& name_at, int64_t seg, const py::str& result) {
    // bytes(code[start:end])
    const std::string& raw = code_all.raw();
    auto bounds = slice_bounds(static_cast<int64_t>(raw.size()), start, end);
    py::bytes code(raw.substr(static_cast<size_t>(bounds.first), static_cast<size_t>(bounds.second - bounds.first)));

    py::list<Instruction> ins;
    for (const Instruction& i : md(code, static_cast<uint64_t>(start))) {
        ins.append(i);
        if (i.mnemonic == py::str("ret") || i.mnemonic == py::str("retf")) {
            break;
        }
    }
    if (ins.empty() || (ins[-1].mnemonic != py::str("ret") && ins[-1].mnemonic != py::str("retf"))) {
        return failure(text("no return found"));
    }
    auto pro = _prologue(ins);
    if (!pro) {
        return failure(text("no frame of C 8's at its start"));
    }
    int64_t took = std::get<0>(*pro), frame = std::get<1>(*pro);
    bool loadds = std::get<2>(*pro);
    auto epi = _epilogue(ins, loadds);
    if (!epi) {
        return failure(text("no frame of C 8's at its end"));
    }
    int64_t left = std::get<0>(*epi);
    bool far = std::get<1>(*epi);
    int64_t pop = std::get<2>(*epi);

    // body = ins[took:len(ins) - left]
    int64_t n = py::len(ins);
    auto bb = slice_bounds(n, took, n - left);
    std::vector<Instruction> body;
    for (int64_t k = bb.first; k < bb.second; k++) {
        body.push_back(ins[k]);
    }
    uint64_t first = ins[0].address, last = ins[-1].address + static_cast<uint64_t>(ins[-1].size);
    std::set<uint64_t> targets;
    for (const Instruction& i : body) {
        if (_386(i)) {
            // (the inline assembler takes the 286's instructions: a 32-bit register, an operand-size
            // prefix, a near conditional jump are /G3's code -- C round a block, not the block)
            return failure(text("386 code, which the compiler made (") + hex04(i.address) + text(" ") + i.mnemonic +
                           text(" ") + i.op_str + text(")"));
        }
        if (_JUMPS.contains(i.mnemonic)) {
            if (i.operands.empty() || i.operands[0].type != X86_OP_IMM) {
                return failure(text("a jump through a register or memory (") + hex04(i.address) + text(")"));
            }
            uint64_t to = static_cast<uint64_t>(i.operands[0].imm & 0xFFFF);
            if (!(first <= to && to < last)) {
                return failure(text("a jump out of the function (") + hex04(i.address) + text(")"));
            }
            targets.insert(to);
        }
        if (i.mnemonic == py::str("ret") || i.mnemonic == py::str("retf") || i.mnemonic == py::str("iret")) {
            return failure(text("a return in its middle (") + hex04(i.address) + text(")"));
        }
    }
    std::vector<py::str> lines;
    for (const Instruction& i : body) {
        if (targets.count(i.address)) {
            lines.push_back(text("L_") + hex04(i.address) + text(":"));
        }
        py::str mn = last_word(i.mnemonic);   // (capstone says "rep stosw")
        std::vector<int64_t> site;
        for (uint64_t a = i.address; a < i.address + static_cast<uint64_t>(i.size); a++) {
            if (a <= static_cast<uint64_t>(INT64_MAX) && fixups.names.contains(static_cast<int64_t>(a))) {
                site.push_back(static_cast<int64_t>(a));
            }
        }
        if (_JUMPS.contains(mn)) {
            lines.push_back(text("        ") + mn + text(" L_") + hex04(i.operands[0].imm & 0xFFFF));
            continue;
        }
        if (mn == py::str("lcall") && !site.empty()) {
            std::optional<py::str> target = fixups.names.at(site[0]);
            if (fixups.segments.contains(site[0]) && py::len(i.operands) == 2) {
                // (a selector-only fix-up: the offset is in the instruction -- a far call within the
                // module, its target named by the segment the fix-up gives and that offset)
                std::optional<int64_t> sg = fixups.segments.at(site[0]);
                target = (sg && *sg != 0) ? name_at(*sg, i.operands[1].imm & 0xFFFF) : std::nullopt;
            }
            if (!target || target->empty()) {
                return failure(text("a far call it cannot name (") + hex04(i.address) + text(")"));
            }
            lines.push_back(text("        call ") + *target);
            continue;
        }
        if (mn == py::str("call") && !i.operands.empty() && i.operands[0].type == X86_OP_IMM) {
            std::optional<py::str> target = name_at(seg, i.operands[0].imm & 0xFFFF);
            if (!target || target->empty()) {
                return failure(text("a near call it cannot name (") + hex04(i.address) + text(")"));
            }
            lines.push_back(text("        call near ptr ") + *target);
            continue;
        }
        if (!site.empty()) {
            return failure(text("a fix-up in ") + mn + text(" (") + hex04(i.address) + text(")"));
        }
        if (mn == py::str("lcall") || mn == py::str("ljmp")) {
            return failure(text("a far ") + mn + text(" without a fix-up (") + hex04(i.address) + text(")"));
        }
        py::str ops = _operands(i, fixups, name_at, seg);
        py::str prefix("");
        int64_t p0 = i.prefix[0];
        if ((p0 == 0xF2 || p0 == 0xF3) && _STRING.contains(mn)) {
            if (p0 == 0xF3) {
                py::str head4(mn.u32().substr(0, 4));
                bool movs = head4 == py::str("movs") || head4 == py::str("stos") || head4 == py::str("lods") ||
                            head4 == py::str("ins") || head4 == py::str("outs");
                prefix = py::str(movs ? "rep " : "repe ");
            } else {
                prefix = py::str("repne ");
            }
        }
        lines.push_back(text("        ") + prefix + mn + (ops.empty() ? py::str("") : text(" ") + ops));
    }
    // (a jump to the frame's end: the block's)
    uint64_t body_end = body.empty() ? first : body.back().address + static_cast<uint64_t>(body.back().size);
    for (uint64_t to : targets) {
        if (to >= body_end) {
            lines.push_back(text("L_") + hex04(to) + text(":"));
        }
    }
    std::vector<py::str> params;
    py::str conv;
    if (pop != 0) {
        for (int64_t k = 0; k < (pop >= 0 ? pop / 2 : 0); k++) {   // range(pop // 2)
            params.push_back(text("a") + py::to_str(k + 1));
        }
        conv = text("__pascal");
    } else {
        conv = text("__cdecl");
    }
    py::str plist = join(text(", "), params);
    py::str head = result + text(" ") + text(far ? "__far" : "__near") + text(" ") + conv +
                   text(loadds ? " __loadds" : "") + text(" ") + name + text("(") + plist + text(")");
    std::vector<py::str> out;
    out.push_back(head);
    for (const py::str& p : params) {
        out.push_back(text("int2 ") + p + text(";"));
    }
    out.push_back(text("{"));
    if (frame != 0) {
        out.push_back(text("  char codebreaker_frame[") + py::to_str(frame) + text("];"));
    }
    out.push_back(text("  _asm {"));
    for (const py::str& l : lines) out.push_back(l);
    out.push_back(text("  }"));
    out.push_back(text("}"));
    return Result(join(text("\n"), out), std::nullopt);
}

}  // namespace

// md.disasm(code, address) of platforms.decoder(16, detail=True): capstone's records as Instructions
py::list<Instruction> decode16(const py::bytes& code, uint64_t address) {
    py::list<Instruction> out;
    platforms::decoder(16, py::str("x86"), true)->each(code.raw(), static_cast<int64_t>(address), 0, [&](const cs_insn& c) {
        Instruction i;
        i.address = c.address;
        i.size = c.size;
        i.mnemonic = py::str(std::string(c.mnemonic));
        i.op_str = py::str(std::string(c.op_str));
        i.bytes = py::bytes(std::string(reinterpret_cast<const char*>(c.bytes), c.size));
        if (c.detail) {
            const cs_x86& x = c.detail->x86;
            for (int k = 0; k < 4; ++k) i.prefix.append(x.prefix[k]);
            for (int k = 0; k < x.op_count; ++k) {
                const cs_x86_op& o = x.operands[k];
                Operand op;
                op.type = o.type;
                op.reg = static_cast<int64_t>(static_cast<uint32_t>(o.reg));
                op.imm = o.imm;
                op.mem.segment = static_cast<int64_t>(static_cast<uint32_t>(o.mem.segment));
                op.mem.base = static_cast<int64_t>(static_cast<uint32_t>(o.mem.base));
                op.mem.index = static_cast<int64_t>(static_cast<uint32_t>(o.mem.index));
                op.mem.scale = o.mem.scale;
                op.mem.disp = o.mem.disp;
                i.operands.append(op);
            }
        }
        out.append(i);
        return true;
    });
    return out;
}

std::tuple<std::optional<py::str>, std::optional<py::str>> whole_body(
    const py::str& name, const py::bytes& code, int64_t start, int64_t end, const Fixups& fixups, const NameAt& name_at,
    int64_t seg, const py::str& result) {
    return whole_body_(Decoder(decode16), name, code, start, end, fixups, name_at, seg, result);
}

std::tuple<std::optional<py::str>, std::optional<py::str>> whole_body(
    const py::str& name, const py::bytearray& code, int64_t start, int64_t end, const Fixups& fixups, const NameAt& name_at,
    int64_t seg, const py::str& result) {
    return whole_body_(Decoder(decode16), name, py::bytes(code.view()), start, end, fixups, name_at, seg, result);
}

std::tuple<std::optional<py::str>, std::optional<py::str>> whole_body(
    const Decoder& md, const py::str& name, const py::bytes& code, int64_t start, int64_t end, const Fixups& fixups,
    const NameAt& name_at, int64_t seg, const py::str& result) {
    return whole_body_(md, name, code, start, end, fixups, name_at, seg, result);
}

std::tuple<std::optional<py::str>, std::optional<py::str>> whole_body(
    const Decoder& md, const py::str& name, const py::bytearray& code, int64_t start, int64_t end, const Fixups& fixups,
    const NameAt& name_at, int64_t seg, const py::str& result) {
    return whole_body_(md, name, py::bytes(code.view()), start, end, fixups, name_at, seg, result);
}

}  // namespace codebreaker::decompile::inline_asm
