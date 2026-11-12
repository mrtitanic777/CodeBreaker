// cb/analyze/origin.cpp -- see origin.hpp.
//
// One piece of assembly evidence makes a function assembly; the evidence is kept with the verdict. The tests are the instruction
// repertoire of the period's C compilers: what they never emit, a person writing assembly does.
// cb-unity: alone
#include "cb/analyze/origin.hpp"

#include <algorithm>
#include <array>
#include <format>
#include <optional>
#include <unordered_map>
#include <unordered_set>

#include "cb/core/capstone_api.hpp"
#include "cb/core/text.hpp"
#include "cb/formats/ne.hpp"

namespace cb::analyze::origin {

namespace dsc = decompile::describe;
using dsc::Code16;
using dsc::Insn;
using dsc::OperandKind;

namespace {

std::string hex4(std::uint64_t v) { return std::format("{:04x}", v); }

const std::unordered_set<std::string_view> kStringOps = {"lodsb", "lodsw", "lodsd", "stosb", "stosw", "stosd", "scasb", "scasw", "scasd", "cmpsb", "cmpsw", "cmpsd", "movsb", "movsw", "movsd"};
// instructions of the machine itself, by the name the evidence gives them
const std::unordered_map<std::string_view, std::string_view> kMachineOps = {{"pushf", "PUSHF"}, {"popf", "POPF"}, {"cli", "CLI"},   {"sti", "STI"},   {"in", "IN"},     {"out", "OUT"},
                                                                            {"lgdt", "LGDT"},   {"lidt", "LIDT"}, {"lmsw", "LMSW"}, {"smsw", "SMSW"}, {"arpl", "ARPL"}, {"lsl", "LSL"},
                                                                            {"lar", "LAR"},     {"verr", "VERR"}, {"verw", "VERW"}, {"hlt", "HLT"}};
const std::unordered_set<std::uint32_t> kRegs32 = {X86_REG_EAX, X86_REG_EBX, X86_REG_ECX, X86_REG_EDX, X86_REG_ESI, X86_REG_EDI, X86_REG_EBP, X86_REG_ESP};
const std::set<std::string, std::less<>> kFastcallRegs = {"AX", "AL", "DX", "DL", "BX", "BL"};   // what _fastcall passes in
// evidence that an _asm block, not a person, left (C 7/8's inline assembler)
constexpr std::string_view kInlineEvidence[] = {"INT 0x", "PUSHF", "POPF", "CLI", "STI", "IN (", "OUT (", "STC:", "CLC:", "CMC:", "argument in the flags"};
constexpr std::string_view kArgumentIn = "argument in ";

bool origin_inline_evidence(std::string_view e) {
    return std::any_of(std::begin(kInlineEvidence), std::end(kInlineEvidence), [&](std::string_view p) { return e.starts_with(p); });
}

// The kind of a piece of evidence: the text before its " (" detail.
std::string_view origin_kind_of(std::string_view e) {
    const std::size_t at = e.find(" (");
    return at == std::string_view::npos ? e : e.substr(0, at);
}

bool origin_one_of(std::string_view s, std::initializer_list<std::string_view> list) { return std::find(list.begin(), list.end(), s) != list.end(); }

std::string origin_upper(std::string_view s) { return text::upper(s); }

struct Regs {
    std::span<const std::uint16_t> read, written;
    bool has_read(std::uint32_t r) const { return std::find(read.begin(), read.end(), r) != read.end(); }
    bool has_written(std::uint32_t r) const { return std::find(written.begin(), written.end(), r) != written.end(); }
};
// What an instruction reads and writes, or nothing where Capstone cannot say.
Regs origin_regs_of(const Insn& i) { return i.regs_ok ? Regs{i.regs_read, i.regs_write} : Regs{}; }

bool origin_has_rep(const Insn& ins) {
    // a REP / REPNE prefix among the instruction's leading prefix bytes
    for (const std::uint8_t b : ins.bytes) {
        if (b == 0xF2 || b == 0xF3) return true;
        const bool prefix = b == 0x66 || b == 0x67 || b == 0xF0 || b == 0x2E || b == 0x36 || b == 0x3E || b == 0x26 || b == 0x64 || b == 0x65;
        if (!prefix) return false;
    }
    return false;
}

// The names of the C compiler's own helpers: a call to one is noted (o.compiler)
bool origin_is_compiler_helper(std::string_view target) {
    static const char* const kHelpers[] = {"chkstk",   "__aFlmul",  "__aFldiv",  "__aFuldiv", "__aFlrem",   "__aFulrem",   "__aFlshl",   "__aFlshr",    "__aFulshr", "__aNlmul",
                                           "__aNldiv", "__aNuldiv", "__aNlrem",  "__aNulrem", "__aFNalmul", "__aFNauldiv", "__aFahdiff", "__aFahincr", "__fac"};
    const std::string low = text::lower(target);
    return std::any_of(std::begin(kHelpers), std::end(kHelpers), [&](const char* h) { return low.find(text::lower(h)) != std::string::npos; });
}

// A function judged assembly whose only evidence is what C 7/8 make for _fastcall or an _asm block: C, with what is said of it beside it
// (o.compiler).
void origin_c8_forms(Origin& o, const Code16& code, std::int64_t start, std::int64_t end) {
    std::set<std::string> regs;   // the registers it takes arguments in (sorted: they are named in that order)
    for (const std::string& e : o.evidence) {
        const std::string_view k = origin_kind_of(e);
        if (k.starts_with(kArgumentIn) && k != "argument in the flags") regs.insert(std::string(k.substr(kArgumentIn.size())));
    }
    std::vector<std::string> inl, rest;
    for (const std::string& e : o.evidence) {
        if (origin_inline_evidence(e)) inl.push_back(e);
        else if (!(e.starts_with(kArgumentIn) && kFastcallRegs.contains(std::string(origin_kind_of(e).substr(kArgumentIn.size()))))) rest.push_back(e);
    }
    if (!rest.empty()) return;
    const std::vector<Insn> ins = code.decode(start, std::min(end, start + 8), true);
    const bool framed = !ins.empty() && (ins[0].mn == "enter" || (ins.size() > 1 && ins[0].mn == "push" && ins[0].op_str == "bp" && ins[1].mn == "mov" && ins[1].op_str == "bp, sp"));
    if (!inl.empty() && !framed) return;
    if (!regs.empty() && !regs.contains("AX") && !regs.contains("AL")) return;   // (the first argument is AX's, a long's DX:AX)
    if (!regs.empty()) o.compiler.push_back("_fastcall (arguments in " + text::join(std::vector<std::string>(regs.begin(), regs.end()), ", ") + ")");
    if (!inl.empty()) {
        inl.resize(std::min<std::size_t>(inl.size(), 3));
        o.compiler.push_back("an _asm block (" + text::join(inl, "; ") + ")");
    }
    o.evidence.clear();
    o.language = Language::C;
}

// The offset one past a function: the next function's start in its segment, else the segment's end.
std::int64_t origin_end_of(const std::vector<std::int64_t>& starts, std::int64_t off, std::int64_t segment_size) {
    const auto next = std::upper_bound(starts.begin(), starts.end(), off);
    return next != starts.end() ? *next : segment_size;
}

}  // namespace

std::string_view to_text(Language language) { return language == Language::Assembly ? "assembly" : "c"; }

std::string Origin::describe() const {
    const std::vector<std::string>& src = !evidence.empty() ? evidence : compiler;
    const std::string ev = src.empty() ? "nothing either way" : text::join(src, "; ");
    return std::format("<{} {}: {}>", name, to_text(language), ev);
}

Summary Result::summary() const {
    Summary s;
    std::vector<std::pair<std::string, std::size_t>> reasons;   // in the order first met
    for (const auto& [va, o] : functions) {
        (o.language == Language::Assembly ? s.assembly : s.c) += 1;
        if (o.evidence.empty()) continue;
        const std::string kind(origin_kind_of(o.evidence[0]));
        const auto found = std::find_if(reasons.begin(), reasons.end(), [&](const auto& r) { return r.first == kind; });
        if (found != reasons.end()) ++found->second;
        else reasons.emplace_back(kind, 1);
    }
    std::stable_sort(reasons.begin(), reasons.end(), [](const auto& a, const auto& b) { return a.second > b.second; });
    s.reasons = std::move(reasons);
    return s;
}

void judge(Origin& o, const Code16& code, std::int64_t start, std::int64_t end, const std::set<std::int64_t>& starts, bool g3,
           const std::map<std::int64_t, std::string>& names, std::int64_t base) {
    std::set<std::string> written;   // the registers written so far, by name (a byte register also names its word)
    bool prologue_ax = false, frame = false, last_push_seg = false, flags_set = false, ax_from_ss = false, prev_call = false;
    struct Pending {
        std::string reg;
        std::int64_t at;
        int left;
    };
    std::optional<Pending> pending;
    std::map<std::string, std::int64_t> code_in;   // register -> the code address put in it
    std::vector<std::string> evidence = o.evidence;
    auto add = [&](std::string e) { evidence.push_back(std::move(e)); };
    auto assembly_now = [&] {
        o.evidence = evidence;
        o.language = Language::Assembly;
    };

    const std::vector<Insn> body0 = code.decode(start, end, true);   // (the first instruction is the head of the body)
    const Insn* first_ins = body0.empty() ? nullptr : &body0[0];
    if (first_ins != nullptr && first_ins->mn == "pop") {
        // it takes its own return address off the stack (to take its arguments off by hand, or to return elsewhere) -- C never does
        add("pops its own return address (" + hex4(start) + " pop " + first_ins->op_str + ")");
    }
    if (first_ins != nullptr && (first_ins->mn == "ret" || first_ins->mn == "retf")) {
        // nothing but its return: every register as it came -- compiled C would load its own in a prologue
        add("does nothing but return (" + hex4(start) + " " + first_ins->mn + ")");
        assembly_now();
        return;
    }
    for (const dsc::Distance kind : {dsc::Distance::Far, dsc::Distance::Near}) {
        const auto taken = dsc::popped_by_hand(code, start, kind);
        if (taken && *taken != 0) {
            // it takes its own return address and its arguments off the stack by hand -- a place in the first few instructions, not only the first; C cannot
            add("takes its return address and " + std::to_string(*taken) + " bytes of arguments off by hand");
            assembly_now();
            return;
        }
    }
    {
        const std::vector<Insn> all2 = code.decode(start, std::min(end, start + 12), true);
        if (all2.size() >= 2 && (all2[0].mn == "call" || all2[0].mn == "lcall") && (all2[1].mn == "ret" || all2[1].mn == "retf") && all2[1].op_str.empty()) {
            // a call and a return, nothing else: whatever the callee leaves goes back to the caller as it came. C hands back AX and DX only.
            add("a call passed on (" + hex4(start) + " " + all2[0].mn + " " + all2[0].op_str + "; " + all2[1].mn + ")");
            assembly_now();
            return;
        }
    }
    if (first_ins != nullptr && (first_ins->mn == "jmp" || first_ins->mn == "ljmp") && end - start <= first_ins->size + 2) {
        add("a jump stub (" + hex4(start) + " " + first_ins->mn + " " + first_ins->op_str + ")");
        assembly_now();
        return;
    }
    bool first = true;
    // the function's own bytes -- and, when they end without a return or a jump (the next entry lies inside its code: overlapping entry points),
    // on until the code does leave
    std::vector<Insn> body = body0;
    const std::int64_t at = !body.empty() ? body.back().address + body.back().size : start;
    const std::vector<Insn> straddle = code.decode(at, at + 16, true);
    const Insn* straddles = straddle.empty() ? nullptr : &straddle[0];
    std::vector<const Insn*> real;   // the instructions that are not padding
    for (const Insn& i : body) {
        if (!(i.mn == "nop" || i.mn == "int3")) real.push_back(&i);
    }
    if (real.empty()) {
        for (const Insn& i : body) real.push_back(&i);
    }
    auto leaves = [](std::string_view m) { return origin_one_of(m, {"ret", "retf", "iret", "jmp", "ljmp"}); };
    const bool runs_on = !body.empty() && at == end && starts.contains(end) && !origin_one_of(real.back()->mn, {"ret", "retf", "iret", "jmp", "ljmp", "int", "hlt"});
    if (runs_on) {
        // it ends where the next function starts, without returning: it runs on into it -- C functions do not
        add("runs on into the next function (" + hex4(end) + ")");
    }
    if (((at < end && straddles != nullptr && at + straddles->size > end) || runs_on) && (body.empty() || !leaves(body.back().mn))) {
        for (const Insn& ins : code.decode(at, at + 0x400, true)) {
            body.push_back(ins);
            if (leaves(ins.mn) || body.size() > 400) break;
        }
        end = std::max<std::int64_t>(end, body.back().address + body.back().size);
    }
    // It keeps DS for its caller round a call -- `push ds` in its prologue, `pop ds` before its return -- and never loads DS itself: C makes
    // no such promise.
    {
        std::vector<const Insn*> pushes;
        for (std::size_t k = 0; k < body.size() && k < 6; ++k) {
            if (body[k].mn == "push" && body[k].op_str == "ds") pushes.push_back(&body[k]);
        }
        const bool any_call = std::any_of(body.begin(), body.end(), [](const Insn& i) { return i.mn == "call" || i.mn == "lcall"; });
        bool loads_ds = false;
        for (std::size_t k = 0; k < body.size(); ++k) {
            const Insn& i = body[k];
            if (!(i.mn == "mov" || i.mn == "lds" || i.mn == "pop") || !i.op_str.starts_with("ds")) continue;
            bool ret_follows = false;
            if (i.mn == "pop") {
                for (std::size_t q = k + 1; q < body.size() && q < k + 4; ++q) {
                    if (body[q].address > i.address && (body[q].mn == "ret" || body[q].mn == "retf")) ret_follows = true;
                }
            }
            if (!(i.mn == "pop" && ret_follows)) loads_ds = true;
        }
        if (!pushes.empty() && any_call && !loads_ds) {
            const Insn* last_pop = nullptr;
            for (const Insn& i : body) {
                if (i.mn == "pop" && i.op_str == "ds") last_pop = &i;
            }
            if (last_pop != nullptr) {
                add("keeps DS for its caller round a call (" + hex4(pushes[0]->address) + " push ds .. " + hex4(last_pop->address) + " pop ds)");
                assembly_now();
                return;
            }
        }
    }
    for (const Insn& ins : body) {
        const std::string& mn = ins.mn;
        const std::vector<dsc::Operand>& ops = ins.operands;
        const Regs rg = origin_regs_of(ins);
        auto names_of = [](std::span<const std::uint16_t> regs) {
            std::set<std::string> s;
            for (const std::uint16_t r : regs) s.insert(dsc::register_name16(r));
            return s;
        };
        const std::set<std::string> read_names = names_of(rg.read), written_names = names_of(rg.written);
        if (first && ins.bytes.size() >= 2 && ins.bytes[0] == 0x8C && ins.bytes[1] == 0xD8) prologue_ax = true;   // mov ax, ds (Windows far prologue)
        first = false;
        const std::string at4 = hex4(static_cast<std::uint64_t>(ins.address));
        if (origin_one_of(mn, {"loop", "loope", "loopne", "jcxz", "jecxz"})) {
            add(origin_upper(mn) + " (" + at4 + ")");
        } else if (kStringOps.contains(mn) && !origin_has_rep(ins) && !origin_one_of(mn, {"movsb", "movsw", "movsd"})) {
            add(origin_upper(mn) + " without REP (" + at4 + ")");
        } else if (mn == "int") {
            add("INT " + ins.op_str + " (" + at4 + ")");
        } else if (mn == "mov" && ops.size() == 2 && ops[1].kind == OperandKind::Register && ops[1].reg == X86_REG_SP &&
                   !(ops[0].kind == OperandKind::Register && ops[0].reg == X86_REG_BP)) {
            // SP copied to reach the stack through another register (no frame): hand-written
            add("reaches the stack through SP (" + at4 + " " + mn + " " + ins.op_str + ")");
        } else if (mn == "mov" && ins.op_str == "ds, ax" && ax_from_ss) {
            // DS made the stack segment (`mov ax, ss / mov ds, ax`): switched by hand, around a call
            add("loads DS with SS (" + at4 + ")");
        } else if (mn == "mov" && ins.op_str.starts_with("ds, word ptr [")) {
            // DS loaded from a variable (a segment it keeps, switched to): compiled code loads DS from a register
            add("loads DS from " + ins.op_str.substr(4) + " (" + at4 + ")");
        } else if (mn == "ljmp") {
            add("a far jump (" + at4 + " " + ins.op_str + ")");
        } else if (mn == "pop" && prev_call && origin_one_of(ins.op_str, {"ax", "bx", "cx", "dx"})) {
            // a word pushed before a call taken back after it as a value (pop, then used)
            pending = Pending{ins.op_str, ins.address, 3};
        } else if (mn == "call" && !ops.empty() && ops[0].kind == OperandKind::Immediate && start < (ops[0].imm & 0xFFFF) && (ops[0].imm & 0xFFFF) < end) {
            // a call into its own body (a far entry that calls the near routine after it)
            add("calls into its own body (" + at4 + " -> " + hex4(static_cast<std::uint64_t>(ops[0].imm & 0xFFFF)) + ")");
        } else if (origin_one_of(mn, {"stc", "clc", "cmc"})) {
            // the carry flag set as a result (callers test it): C has no word for it
            add(origin_upper(mn) + ": a result in the carry flag (" + at4 + ")");
        } else if (const auto machine = kMachineOps.find(mn); machine != kMachineOps.end()) {
            add(std::string(machine->second) + " (" + at4 + ")");
        }
        // Microsoft C 7 and 8 under /G3 move and compute longs in 32-bit registers, but address memory with 16-bit registers only: in a module
        // their linker made, a 32-bit register as an operand says nothing, one that addresses memory still says assembly.
        {
            bool any32 = false;
            for (const dsc::Operand& op : ops) {
                if (op.kind == OperandKind::Register && !g3 && kRegs32.contains(op.reg)) any32 = true;
                if (op.kind == OperandKind::Memory && (kRegs32.contains(op.base) || kRegs32.contains(op.index))) any32 = true;
            }
            if (any32) add("32-bit register (" + at4 + " " + mn + " " + ins.op_str + ")");   // (not the stack pointer push and call imply)
        }
        for (const dsc::Operand& op : ops) {
            // (a JMP through a table in the code segment is how the compiler makes a switch)
            if (op.kind == OperandKind::Memory && op.segment == X86_REG_CS && mn != "jmp") add("reads its code segment (" + at4 + ")");
        }
        if ((rg.has_written(X86_REG_SS) && mn != "pop") || (mn == "pop" && ins.op_str == "ss")) add("loads SS (" + at4 + ")");
        if (rg.has_written(X86_REG_SP) && mn == "mov" && ins.op_str != "sp, bp") add("moves SP (" + at4 + ")");
        if (mn == "enter") frame = true;
        // a place through BP before the function makes its own frame: its caller's frame
        if (!frame && !origin_one_of(mn, {"push", "pop", "inc", "dec", "leave"})) {
            const bool through_bp = std::any_of(ops.begin(), ops.end(), [](const dsc::Operand& op) { return op.kind == OperandKind::Memory && op.base == X86_REG_BP; });
            if (through_bp) add("works in its caller's frame (" + at4 + " " + mn + " " + ins.op_str + ")");
        }
        if (mn == "mov" && ins.op_str == "bp, sp") {
            frame = true;
        } else if (rg.has_written(X86_REG_BP) && !origin_one_of(mn, {"push", "pop", "inc", "dec", "leave", "enter"}) && !frame) {
            add("BP as a plain register (" + at4 + ")");
        }
        // a flag read before anything sets one (a jump on the carry its caller left): an argument in the flags
        if (rg.has_read(X86_REG_EFLAGS) && !flags_set && !origin_one_of(mn, {"pushf", "pushfd"})) add("argument in the flags (" + at4 + " " + mn + ")");
        if (rg.has_written(X86_REG_EFLAGS)) flags_set = true;
        // AX and DX swapped: a 32-bit result's halves put the other way round
        if (mn == "xchg") {
            std::string squeezed;
            for (const char c : ins.op_str) {
                if (c != ' ') squeezed += c;
            }
            const std::size_t comma = squeezed.find(',');
            if (comma != std::string::npos && squeezed.find(',', comma + 1) == std::string::npos) {
                std::string a = squeezed.substr(0, comma), b = squeezed.substr(comma + 1);
                if (a > b) std::swap(a, b);
                if (a == "ax" && b == "dx") add("swaps AX and DX (" + at4 + ")");
            }
        }
        // registers read before written (arguments in registers)
        for (const std::uint16_t r : rg.read) {
            const std::string nm = dsc::register_name16(r);
            if (origin_one_of(nm, {"ax", "bx", "cx", "dx", "si", "di", "al", "ah", "bl", "bh", "cl", "ch", "dl", "dh", "es"}) && !written.contains(nm.substr(0, 1) + "x") &&
                !written.contains(nm) && !origin_one_of(mn, {"push", "xor", "sub"}) && !(prologue_ax && nm == "ax") && nm != "es") {
                add("argument in " + origin_upper(nm) + " (" + at4 + ")");
            }
        }
        // BX, CX or DX pushed before the function set it: what its caller put there, kept (a C prologue saves SI, DI, DS)
        if (mn == "push" && origin_one_of(ins.op_str, {"bx", "cx", "dx"})) {
            const std::string& r = ins.op_str;
            if (!written.contains(r) && !written.contains("e" + r) && !written.contains(r.substr(0, 1) + "l") && !written.contains(r.substr(0, 1) + "h")) {
                add("argument in " + origin_upper(r) + " (" + at4 + " push)");
            }
        }
        // SI or DI pushed, as it came, right after a segment register: a far pointer made of it to pass on
        if (mn == "push" && origin_one_of(ins.op_str, {"si", "di"}) && !written.contains(ins.op_str) && last_push_seg) {
            add("argument in " + origin_upper(ins.op_str) + " (" + at4 + " push after a segment)");
        }
        last_push_seg = mn == "push" && origin_one_of(ins.op_str, {"ss", "ds", "es", "cs"});
        // a code address put in a register and called through it (with arguments in others)
        if (mn == "mov" && ops.size() == 2 && ops[0].kind == OperandKind::Register && ops[1].kind == OperandKind::Immediate && starts.contains(ops[1].imm)) {
            code_in[dsc::register_name16(ops[0].reg)] = ops[1].imm;
        } else if (mn == "call" && !ops.empty() && ops[0].kind == OperandKind::Register && code_in.contains(dsc::register_name16(ops[0].reg))) {
            add("calls a code address it put in " + ins.op_str + " (" + at4 + " call " + ins.op_str + ")");
        }
        if (pending && ins.address != pending->at) {
            const std::string& reg = pending->reg;
            const std::set<std::string> alias = {reg, reg.substr(0, 1) + "l", reg.substr(0, 1) + "h"};
            const bool hit_read = std::any_of(read_names.begin(), read_names.end(), [&](const std::string& n) { return alias.contains(n); });
            const bool hit_write = std::any_of(written_names.begin(), written_names.end(), [&](const std::string& n) { return alias.contains(n); });
            if (hit_read) {
                add("takes back a pushed word as a value (" + hex4(static_cast<std::uint64_t>(pending->at)) + " pop " + reg + ")");
                pending.reset();
            } else if (hit_write || pending->left <= 1) {
                pending.reset();
            } else {
                pending->left -= 1;
            }
        }
        prev_call = mn == "call" || mn == "lcall" || (prev_call && mn == "pop");
        if (written_names.contains("ax") || written_names.contains("eax")) ax_from_ss = mn == "mov" && ins.op_str == "ax, ss";
        for (const std::uint16_t r : rg.written) {
            const std::string nm = dsc::register_name16(r);
            written.insert(nm);
            if (nm.size() == 2 && (nm[1] == 'l' || nm[1] == 'h')) written.insert(nm.substr(0, 1) + "x");
            else if (nm.size() == 3 && nm[0] == 'e') written.insert(nm.substr(1));   // (cdq, movsx eax ...: its 16-bit half too)
        }
        if (mn == "call" || mn == "lcall") {   // what a call leaves: its result in DX:AX, and the registers it may use
            for (const char* r : {"ax", "bx", "cx", "dx", "es"}) written.insert(r);
        } else if (mn == "int") {
            for (const char* r : {"ax", "bx", "cx", "dx", "si", "di", "es"}) written.insert(r);
        }
        if ((mn == "call" || mn == "lcall") && !ops.empty() && ops[0].kind == OperandKind::Immediate) {   // (a call to one of the compiler's helpers, linked in and named)
            std::optional<std::int64_t> va;
            if (mn == "call") va = base + (ops[0].imm & 0xFFFF);
            else if (ops.size() == 2) va = (ops[0].imm << 16) + ops[1].imm;
            if (va) {
                const auto named = names.find(*va);
                if (named != names.end() && origin_is_compiler_helper(named->second)) o.compiler.push_back("calls " + named->second);
            }
        }
        // a jump out of the function: into the middle of other code (entry points sharing a body) or to another function (a tail jump): the C
        // compilers of the time do neither
        if (!mn.empty() && mn[0] == 'j' && !ops.empty() && ops[0].kind == OperandKind::Immediate && !(start <= (ops[0].imm & 0xFFFF) && (ops[0].imm & 0xFFFF) < end) && !starts.empty()) {
            add(std::string("jumps ") + (starts.contains(ops[0].imm & 0xFFFF) ? "to another function" : "into the middle of other code") + " (" + at4 + " -> " +
                hex4(static_cast<std::uint64_t>(ops[0].imm & 0xFFFF)) + ")");
        }
        // a return address of its own pushed (push the place to come back to, then jump): hand-made calling
        if (mn == "push" && !ops.empty() && ops[0].kind == OperandKind::Immediate && ins.address < ops[0].imm && ops[0].imm < end) {
            add("pushes a return address of its own (" + at4 + " push " + hex4(static_cast<std::uint64_t>(ops[0].imm)) + ")");
        }
        if (mn == "iret") add("IRET (" + at4 + ")");
    }
    // SI or DI changed and never saved: given back changed -- a result, or a pointer moved on for the caller. The C compilers keep both (a
    // register variable is pushed first).
    std::set<std::string> changed, saved;
    for (const Insn& ins : body) {
        if (origin_one_of(ins.mn, {"int", "call", "lcall", "pop"}) || !ins.regs_ok) continue;
        for (const std::uint16_t r : ins.regs_write) {
            const std::string nm = dsc::register_name16(r);
            if (origin_one_of(nm, {"si", "di", "esi", "edi"})) changed.insert(nm);
        }
    }
    for (const Insn& ins : body) {
        if (ins.mn == "push") saved.insert(ins.op_str);
    }
    for (const std::string& reg : changed) {
        const std::string tail = reg.substr(reg.size() - 2);
        if (!saved.contains(tail) && !saved.contains("e" + tail)) add("gives " + origin_upper(tail) + " back changed (never saved)");
    }
    // de-duplicate, keep the first of each kind
    std::set<std::string_view> seen;
    o.evidence.clear();
    for (const std::string& e : evidence) {
        if (seen.insert(origin_kind_of(e)).second) o.evidence.push_back(e);
    }
    o.language = o.evidence.empty() ? Language::C : Language::Assembly;
}

Result module16(const formats::ne::Module& module, const std::filesystem::path& workdir, dsc::Knowledge& knowledge, const std::map<std::int64_t, std::string>* more,
                const dsc::Policy& policy) {
    dsc::NeDescription d = dsc::describe16(module, workdir, knowledge, policy);
    std::map<std::int64_t, std::string> names;   // the module's functions, by address
    for (const auto& [va, name] : d.names) {
        if (d.where_map.contains(va)) names.emplace(va, name);
    }
    if (more != nullptr) {
        for (const auto& [va, name] : *more) {
            if (names.emplace(va, name).second) d.where_map[va] = dsc::where(va);
        }
    }
    std::map<std::int64_t, std::vector<std::int64_t>> starts;   // segment -> its functions' offsets, ascending
    for (const auto& [va, name] : names) starts[d.where_map.at(va).seg].push_back(d.where_map.at(va).off);
    for (auto& [seg, offsets] : starts) std::sort(offsets.begin(), offsets.end());
    std::map<std::int64_t, Code16> codes;   // each segment's bytes, decoded once
    for (const auto& [seg, image] : d.images) codes.emplace(seg, Code16(image));
    static const Code16 kNoSegment{Bytes{}};
    auto code_of = [&](std::int64_t seg) -> const Code16& {
        const auto it = codes.find(seg);
        return it != codes.end() ? it->second : kNoSegment;
    };
    const bool g3 = std::make_pair(module.header().linker_version, module.header().linker_revision) >= std::make_pair<std::uint8_t, std::uint8_t>(5, 30);   // (C 7 and 8: /G3 makes 32-bit registers)

    Result res;
    for (const auto& [va, name] : names) {
        const dsc::SegOff place = d.where_map.at(va);
        const Code16& code = code_of(place.seg);
        const std::vector<std::int64_t>& offsets = starts.at(place.seg);
        const std::int64_t end = origin_end_of(offsets, place.off, code.size());
        Origin o;
        o.va = va;
        o.name = name;
        judge(o, code, place.off, end, std::set<std::int64_t>(offsets.begin(), offsets.end()), g3, names, dsc::address(place.seg, 0));
        if (g3 && o.language == Language::Assembly) origin_c8_forms(o, code, place.off, end);
        res.functions.emplace(va, std::move(o));
    }
    // A function whose inner addresses a table names (KRNL386's fault chain: a start and an end of the instructions that may fault, with where
    // to go on if they do) was written in assembly: its instructions have to be where the table says, which C's cannot be. Three words side by
    // side -- a start, an end and where to go on -- all instructions of the same function and none its start.
    for (const auto& [seg, offsets] : starts) {
        const Code16& code = code_of(seg);
        const ByteView image = code.bytes();
        const std::int64_t size = code.size();
        std::map<std::int64_t, std::int64_t> inner;   // an instruction that is not a function's first -> that function
        std::vector<char> covered(static_cast<std::size_t>(size), 0);   // the bytes of instructions
        for (std::size_t k = 0; k < offsets.size(); ++k) {
            const std::int64_t off = offsets[k], end = k + 1 < offsets.size() ? offsets[k + 1] : size;
            for (const Insn& ins : code.decode(off, end, true)) {
                if (ins.address != off) inner[ins.address] = off;
                for (std::int64_t q = ins.address; q < ins.address + ins.size && q < size; ++q) covered[static_cast<std::size_t>(q)] = 1;
            }
        }
        std::map<std::int64_t, std::set<std::array<std::int64_t, 4>>> named;   // function -> (segment, table, start, end)
        auto word = [&](std::int64_t o) { return static_cast<std::int64_t>(image[static_cast<std::size_t>(o)]) | (static_cast<std::int64_t>(image[static_cast<std::size_t>(o) + 1]) << 8); };
        for (std::int64_t at = 0; at < size - 5; at += 2) {   // (a table of its own segment, not code)
            bool any_covered = false;
            for (std::int64_t q = at; q < at + 6 && q < size; ++q) any_covered = any_covered || covered[static_cast<std::size_t>(q)] != 0;
            if (any_covered) continue;
            const std::int64_t a = word(at), b = word(at + 2), c = word(at + 4);
            const auto ia = inner.find(a);
            if (a < b && ia != inner.end()) {
                const auto ib = inner.find(b), ic = inner.find(c);
                if (ib != inner.end() && ib->second == ia->second && ic != inner.end() && ic->second == ia->second) named[ia->second].insert({seg, at, a, b});
            }
        }
        for (auto& [va, o] : res.functions) {
            const dsc::SegOff place = d.where_map.at(va);
            const auto hit = named.find(place.off);
            if (place.seg == seg && hit != named.end() && o.language != Language::Assembly) {
                const auto& first = *hit->second.begin();
                o.evidence.push_back(std::format("its instructions {:04x}-{:04x} are named by a table ({}:{:04x})", first[2], first[3], first[0], first[1]));
                o.language = Language::Assembly;
            }
        }
    }
    // A function that calls one taking its arguments in registers, having put them there itself, was written in assembly as well (C passes
    // arguments on the stack)
    std::map<std::pair<std::int64_t, std::int64_t>, const Origin*> by_place;   // (segment, offset) -> its verdict
    for (const auto& [va, o] : res.functions) {
        const dsc::SegOff place = d.where_map.at(va);
        by_place[{place.seg, place.off}] = &o;
    }
    std::set<std::pair<std::int64_t, std::int64_t>> takes_registers;
    for (const auto& [place, o] : by_place) {
        const bool arg = std::any_of(o->evidence.begin(), o->evidence.end(), [](const std::string& e) { return e.starts_with(kArgumentIn); });
        if (arg && o->language == Language::Assembly) takes_registers.insert(place);
    }
    std::vector<std::pair<std::int64_t, std::string>> found;   // the verdicts to add (made after the walk: the walk reads the table above)
    for (auto& [va, o] : res.functions) {
        if (o.language == Language::Assembly) continue;
        const dsc::SegOff place = d.where_map.at(va);
        const Code16& code = code_of(place.seg);
        const std::int64_t end = origin_end_of(starts.at(place.seg), place.off, code.size());
        for (const Insn& ins : code.decode(place.off, end, true)) {
            if (ins.mn == "call" && !ins.operands.empty() && ins.operands[0].kind == OperandKind::Immediate) {
                const std::int64_t to = ins.operands[0].imm & 0xFFFF;   // (a call back down wraps round)
                const auto callee = by_place.find({place.seg, to});
                if (takes_registers.contains({place.seg, to}) && callee != by_place.end()) {
                    // (whether it put them there or passes its own caller's on: C can do neither)
                    found.emplace_back(va, "calls " + callee->second->name + " with arguments in registers (" + hex4(static_cast<std::uint64_t>(ins.address)) + ")");
                    break;
                }
            }
        }
    }
    for (const auto& [va, evidence] : found) {
        Origin& o = res.functions.at(va);
        o.evidence.push_back(evidence);
        o.language = Language::Assembly;
    }
    return res;
}

std::string report(std::string_view module_name, const Result& result, bool list) {
    const Summary s = result.summary();
    std::string out = std::format("{}: {} functions -- {} C, {} assembly\n", module_name, result.functions.size(), s.c, s.assembly);
    for (std::size_t i = 0; i < s.reasons.size() && i < 12; ++i) out += std::format("  {:4}  {}\n", s.reasons[i].second, s.reasons[i].first);
    if (list) {
        for (const auto& [va, o] : result.functions) {
            const std::vector<std::string>& src = !o.evidence.empty() ? o.evidence : o.compiler;
            std::string ev;
            const std::size_t shown = std::min<std::size_t>(src.size(), o.evidence.empty() ? 2 : 3);
            for (std::size_t i = 0; i < shown; ++i) ev += (i != 0 ? "; " : "") + src[i];
            out += std::format("  {:<28} {:<9} {}\n", o.name, to_text(o.language), ev);
        }
    }
    return out;
}

}  // namespace cb::analyze::origin
