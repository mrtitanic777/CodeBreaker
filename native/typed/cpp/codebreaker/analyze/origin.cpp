// codebreaker/analyze/origin.cpp -- typed C++ from codebreaker.analyze.origin (see the header).
//
// Was a 16-bit function written in assembly, or made by a C compiler? One piece of assembly evidence makes a function
// assembly; the evidence is kept with the verdict.
//
// What differs from the Python, and why:
//   * module16() does not keep its answer in base.cache (the Python pickles the Result under the module's bytes, `more`
//     and the source text of origin.py and describe.py): there is no C++ form of that pickle, so it is worked out every
//     time (module16 == _module16).
//   * `imported.get(ops[0].imm)` of _judge looks an integer up in describe's import_names, whose keys are the symbols'
//     names: it never finds one, so the "calls <compiler helper>" note of that branch is never made -- the C++ has no
//     such branch.
//   * In _module16 the `covered` bytearray of the Python is trimmed by `covered[a:b] = b"" * n` (an empty replacement: it
//     deletes bytes) and holds only zeros, so `any(covered[at:at + 6])` is never true: the loop does not test it.
#include "codebreaker/analyze/origin.hpp"

#include <algorithm>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <set>

#include <capstone/capstone.h>

#include "codebreaker/base/corpus.hpp"
#include "codebreaker/build/watcom.hpp"
#include "codebreaker/decompile/describe.hpp"
#include "py_os.hpp"

namespace codebreaker::analyze::origin {

namespace describe = decompile::describe;
using describe::Insn;
using Decoder = describe::Decoder;

namespace {

std::string hex4(uint64_t v) {
    char b[32];
    std::snprintf(b, sizeof b, "%04llx", static_cast<unsigned long long>(v));
    return b;
}

const std::set<std::string> kString = {"lodsb", "lodsw", "lodsd", "stosb", "stosw", "stosd", "scasb", "scasw", "scasd", "cmpsb", "cmpsw", "cmpsd", "movsb", "movsw", "movsd"};
const std::map<std::string, std::string> kMachine = {{"pushf", "PUSHF"}, {"popf", "POPF"}, {"cli", "CLI"},   {"sti", "STI"},   {"in", "IN"},     {"out", "OUT"},
                                                     {"lgdt", "LGDT"},   {"lidt", "LIDT"}, {"lmsw", "LMSW"}, {"smsw", "SMSW"}, {"arpl", "ARPL"}, {"lsl", "LSL"},
                                                     {"lar", "LAR"},     {"verr", "VERR"}, {"verw", "VERW"}, {"hlt", "HLT"}};
const std::set<int64_t> kRegs32 = {X86_REG_EAX, X86_REG_EBX, X86_REG_ECX, X86_REG_EDX, X86_REG_ESI, X86_REG_EDI, X86_REG_EBP, X86_REG_ESP};
const std::set<std::string> kFastcall = {"AX", "AL", "DX", "DL", "BX", "BL"};                               // _FASTCALL
const char* const kInline[] = {"INT 0x", "PUSHF", "POPF", "CLI", "STI", "IN (", "OUT (", "STC:", "CLC:", "CMC:", "argument in the flags"};   // _INLINE

bool starts_with(const std::string& s, const std::string& p) { return p.size() <= s.size() && s.compare(0, p.size(), p) == 0; }
bool inline_ev(const std::string& e) {
    for (const char* p : kInline) if (starts_with(e, p)) return true;
    return false;
}
// e.split(" (")[0]
std::string kind_of(const std::string& e) {
    size_t p = e.find(" (");
    return p == std::string::npos ? e : e.substr(0, p);
}
bool one_of(const std::string& s, std::initializer_list<const char*> l) {
    for (const char* x : l) if (s == x) return true;
    return false;
}
std::string upper(std::string s) {
    for (char& c : s) c = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
    return s;
}

struct Regs {
    std::vector<int64_t> read, written;
};
// ins.regs_access(), or nothing read or written where Capstone cannot say (the Python catches CsError)
Regs regs_of(const Insn& i) {
    Regs r;
    try {
        auto ra = i.regs_access();
        for (int64_t x : std::get<0>(ra).vec()) r.read.push_back(x);
        for (int64_t x : std::get<1>(ra).vec()) r.written.push_back(x);
    } catch (const describe::CsError&) {
    }
    return r;
}

py::list<Insn> disasm(const Decoder& md, const py::bytes& image, int64_t from, int64_t to, int64_t addr) { return md.disasm(image.slice(from, to), addr); }

// A function judged assembly whose only evidence is what C 7/8 make for _fastcall or an _asm block: C, with what it is
// said beside it (o.compiler).
void c8_forms(Origin& o, const Decoder& md, const py::bytes& image, int64_t start, int64_t end) {
    std::vector<std::string> ev;
    for (const py::str& e : o.evidence.vec()) ev.push_back(e.utf8());
    const std::string arg = "argument in ";
    std::set<std::string> regs;
    for (const std::string& e : ev) {
        std::string k = kind_of(e);
        if (starts_with(k, arg) && k != "argument in the flags") regs.insert(k.substr(arg.size()));
    }
    std::vector<std::string> inl, rest;
    for (const std::string& e : ev) {
        if (inline_ev(e)) inl.push_back(e);
        else if (!(starts_with(e, arg) && kFastcall.count(kind_of(e).substr(arg.size())))) rest.push_back(e);
    }
    if (!rest.empty()) return;
    py::list<Insn> ins = disasm(md, image, start, std::min(end, start + 8), start);
    bool framed = !ins.empty() && (ins[0].mn == "enter" ||
                                   (ins.size() > 1 && ins[0].mn == "push" && ins[0].os == "bp" && ins[1].mn == "mov" && ins[1].os == "bp, sp"));
    if (!inl.empty() && !framed) return;
    if (!regs.empty() && !regs.count("AX") && !regs.count("AL")) return;   // (the first argument is AX's, a long's DX:AX)
    std::vector<std::string> said;
    if (!regs.empty()) {
        std::string j;
        for (const std::string& r : regs) j += (j.empty() ? "" : ", ") + r;
        said.push_back("_fastcall (arguments in " + j + ")");
    }
    if (!inl.empty()) {
        std::string j;
        for (size_t i = 0; i < inl.size() && i < 3; i++) j += (i ? "; " : "") + inl[i];
        said.push_back("an _asm block (" + j + ")");
    }
    for (const std::string& s : said) o.compiler.append(py::str(s));
    o.evidence = py::list<py::str>();
    o.language = py::str("c");
}

void judge(Origin& o, const Decoder& md, const py::bytes& image, int64_t start, int64_t end, const std::set<int64_t>& starts, bool g3) {
    std::set<std::string> written;
    bool prologue_ax = false, frame = false, last_push_seg = false, flags_set = false, ax_from_ss = false, prev_call = false;
    struct Pending { std::string reg; uint64_t at; int64_t left; };
    std::optional<Pending> pending;
    std::map<std::string, int64_t> code_in;
    std::vector<std::string> evidence;
    auto add = [&](const std::string& e) { evidence.push_back(e); };
    auto finish_evidence = [&]() {
        o.evidence = py::list<py::str>();
        for (const std::string& e : evidence) o.evidence.append(py::str(e));
    };
    for (const py::str& e : o.evidence.vec()) evidence.push_back(e.utf8());

    py::list<Insn> body0 = md.disasm(image.slice(start, end), start);   // (the first instruction is the head of the body)
    const Insn* first_ins = body0.empty() ? nullptr : &body0[0];
    if (first_ins && first_ins->mn == "pop") {
        // it takes its own return address off the stack (to take its arguments off by hand, or to return elsewhere) -- C never does
        add("pops its own return address (" + hex4(start) + " pop " + first_ins->os + ")");
    }
    if (first_ins && (first_ins->mn == "ret" || first_ins->mn == "retf")) {
        // nothing but its return: every register as it came -- compiled C would load its own in a prologue
        add("does nothing but return (" + hex4(start) + " " + first_ins->mn + ")");
        o.language = py::str("assembly");
        finish_evidence();
        return;
    }
    for (const char* kind : {"far", "near"}) {
        auto taken = describe::popped_by_hand(image, start, py::str(kind));
        if (taken && *taken) {
            // it takes its own return address and its arguments off the stack by hand -- a place in the first few
            // instructions, not only the first; C cannot
            add("takes its return address and " + std::to_string(*taken) + " bytes of arguments off by hand");
            o.language = py::str("assembly");
            finish_evidence();
            return;
        }
    }
    {
        py::list<Insn> all2 = disasm(md, image, start, std::min(end, start + 12), start);
        if (all2.size() >= 2 && (all2[0].mn == "call" || all2[0].mn == "lcall") && (all2[1].mn == "ret" || all2[1].mn == "retf") && all2[1].os.empty()) {
            // a call and a return, nothing else: whatever the callee leaves goes back to the caller as it came. C hands back AX and DX only.
            add("a call passed on (" + hex4(start) + " " + all2[0].mn + " " + all2[0].os + "; " + all2[1].mn + ")");
            o.language = py::str("assembly");
            finish_evidence();
            return;
        }
    }
    if (first_ins && (first_ins->mn == "jmp" || first_ins->mn == "ljmp") && end - start <= first_ins->size + 2) {
        add("a jump stub (" + hex4(start) + " " + first_ins->mn + " " + first_ins->os + ")");
        o.language = py::str("assembly");
        finish_evidence();
        return;
    }
    bool first = true;
    // the function's own bytes -- and, when they end without a return or a jump (the next entry lies inside its code:
    // overlapping entry points), on until the code does leave
    std::vector<Insn> body(body0.vec());
    int64_t at = !body.empty() ? static_cast<int64_t>(body.back().address) + body.back().size : start;
    py::list<Insn> straddle = disasm(md, image, at, at + 16, at);
    const Insn* straddles = straddle.empty() ? nullptr : &straddle[0];
    std::vector<const Insn*> code;
    for (const Insn& i : body) if (!(i.mn == "nop" || i.mn == "int3")) code.push_back(&i);
    if (code.empty()) for (const Insn& i : body) code.push_back(&i);
    auto leaves = [](const std::string& m) { return one_of(m, {"ret", "retf", "iret", "jmp", "ljmp"}); };
    bool runs_on = !body.empty() && at == end && starts.count(end) && !one_of(code.back()->mn, {"ret", "retf", "iret", "jmp", "ljmp", "int", "hlt"});
    if (runs_on) {
        // it ends where the next function starts, without returning: it runs on into it -- C functions do not
        add("runs on into the next function (" + hex4(end) + ")");
    }
    if (((at < end && straddles && at + straddles->size > end) || runs_on) && (body.empty() || !leaves(body.back().mn))) {
        for (const Insn& ins : md.disasm(image.slice(at, at + 0x400), at).vec()) {
            body.push_back(ins);
            if (leaves(ins.mn) || body.size() > 400) break;
        }
        end = std::max<int64_t>(end, static_cast<int64_t>(body.back().address) + body.back().size);
    }
    // It keeps DS for its caller round a call -- `push ds` in its prologue, `pop ds` before its return -- and never loads
    // DS itself: C makes no such promise.
    {
        std::vector<const Insn*> pushes;
        for (size_t k = 0; k < body.size() && k < 6; k++) if (body[k].mn == "push" && body[k].os == "ds") pushes.push_back(&body[k]);
        bool any_call = false;
        for (const Insn& i : body) if (i.mn == "call" || i.mn == "lcall") any_call = true;
        bool loads_ds = false;
        for (size_t k = 0; k < body.size(); k++) {
            const Insn& i = body[k];
            if (!(i.mn == "mov" || i.mn == "lds" || i.mn == "pop") || !starts_with(i.os, "ds")) continue;
            bool ret_follows = false;
            if (i.mn == "pop") {
                for (size_t q = k + 1; q < body.size() && q < k + 4; q++) {
                    if (body[q].address > i.address && (body[q].mn == "ret" || body[q].mn == "retf")) ret_follows = true;
                }
            }
            if (!(i.mn == "pop" && ret_follows)) loads_ds = true;
        }
        if (!pushes.empty() && any_call && !loads_ds) {
            const Insn* last_pop = nullptr;
            for (const Insn& i : body) if (i.mn == "pop" && i.os == "ds") last_pop = &i;
            if (last_pop) {
                add("keeps DS for its caller round a call (" + hex4(pushes[0]->address) + " push ds .. " + hex4(last_pop->address) + " pop ds)");
                o.language = py::str("assembly");
                finish_evidence();
                return;
            }
        }
    }
    for (const Insn& ins : body) {
        const std::string& mn = ins.mn;
        const py::list<decompile::inline_asm::Operand>& ops = ins.operands;
        Regs rg = regs_of(ins);
        auto names = [&](const std::vector<int64_t>& v) {
            std::set<std::string> s;
            for (int64_t r : v) s.insert(md.reg_name(r).utf8());
            return s;
        };
        std::set<std::string> read_names = names(rg.read), written_names = names(rg.written);
        auto has_reg = [&](const std::vector<int64_t>& v, int64_t r) { return std::find(v.begin(), v.end(), r) != v.end(); };
        if (first && ins.bytes.size() >= 2 && ins.bytes.raw().compare(0, 2, std::string("\x8c\xd8", 2)) == 0) prologue_ax = true;   // mov ax, ds (Windows far prologue)
        first = false;
        if (one_of(mn, {"loop", "loope", "loopne", "jcxz", "jecxz"})) {
            add(upper(mn) + " (" + hex4(ins.address) + ")");
        } else if (kString.count(mn) && !(ins.prefix.size() > 0 && (ins.prefix[0] == 0xF2 || ins.prefix[0] == 0xF3)) && !one_of(mn, {"movsb", "movsw", "movsd"})) {
            add(upper(mn) + " without REP (" + hex4(ins.address) + ")");
        } else if (mn == "int") {
            add("INT " + ins.os + " (" + hex4(ins.address) + ")");
        } else if (mn == "mov" && ops.size() == 2 && ops[1].type == X86_OP_REG && ops[1].reg == X86_REG_SP && !(ops[0].type == X86_OP_REG && ops[0].reg == X86_REG_BP)) {
            // SP copied to reach the stack through another register (no frame): hand-written
            add("reaches the stack through SP (" + hex4(ins.address) + " " + mn + " " + ins.os + ")");
        } else if (mn == "mov" && ins.os == "ds, ax" && ax_from_ss) {
            // DS made the stack segment (`mov ax, ss / mov ds, ax`): switched by hand, around a call
            add("loads DS with SS (" + hex4(ins.address) + ")");
        } else if (mn == "mov" && starts_with(ins.os, "ds, word ptr [")) {
            // DS loaded from a variable (a segment it keeps, switched to): compiled code loads DS from a register
            add("loads DS from " + ins.os.substr(4) + " (" + hex4(ins.address) + ")");
        } else if (mn == "ljmp") {
            add("a far jump (" + hex4(ins.address) + " " + ins.os + ")");
        } else if (mn == "pop" && prev_call && one_of(ins.os, {"ax", "bx", "cx", "dx"})) {
            // a word pushed before a call taken back after it as a value (pop, then used)
            pending = Pending{ins.os, ins.address, 3};
        } else if (mn == "call" && !ops.empty() && ops[0].type == X86_OP_IMM && start < (ops[0].imm & 0xFFFF) && (ops[0].imm & 0xFFFF) < end) {
            // a call into its own body (a far entry that calls the near routine after it)
            add("calls into its own body (" + hex4(ins.address) + " -> " + hex4(ops[0].imm & 0xFFFF) + ")");
        } else if (one_of(mn, {"stc", "clc", "cmc"})) {
            // the carry flag set as a result (callers test it): C has no word for it
            add(upper(mn) + ": a result in the carry flag (" + hex4(ins.address) + ")");
        } else if (kMachine.count(mn)) {
            add(kMachine.at(mn) + " (" + hex4(ins.address) + ")");
        }
        // Microsoft C 7 and 8 under /G3 move and compute longs in 32-bit registers, but address memory with 16-bit
        // registers only: in a module their linker made, a 32-bit register as an operand says nothing, one that
        // addresses memory still says assembly.
        {
            std::vector<int64_t> explicit_;
            if (!g3) for (const auto& op : ops.vec()) if (op.type == X86_OP_REG) explicit_.push_back(op.reg);
            for (const auto& op : ops.vec()) if (op.type == X86_OP_MEM) { explicit_.push_back(op.mem.base); explicit_.push_back(op.mem.index); }
            bool any32 = false;
            for (int64_t r : explicit_) if (kRegs32.count(r)) any32 = true;
            if (any32) add("32-bit register (" + hex4(ins.address) + " " + mn + " " + ins.os + ")");   // (not the stack pointer push and call imply)
        }
        for (const auto& op : ops.vec()) {
            // (a JMP through a table in the code segment is how the compiler makes a switch)
            if (op.type == X86_OP_MEM && op.mem.segment == X86_REG_CS && mn != "jmp") add("reads its code segment (" + hex4(ins.address) + ")");
        }
        if ((has_reg(rg.written, X86_REG_SS) && mn != "pop") || (mn == "pop" && ins.os == "ss")) add("loads SS (" + hex4(ins.address) + ")");
        if (has_reg(rg.written, X86_REG_SP) && mn == "mov" && ins.os != "sp, bp") add("moves SP (" + hex4(ins.address) + ")");
        if (mn == "enter") frame = true;
        // a place through BP before the function makes its own frame: its caller's frame
        if (!frame && !one_of(mn, {"push", "pop", "inc", "dec", "leave"})) {
            bool bp = false;
            for (const auto& op : ops.vec()) if (op.type == X86_OP_MEM && op.mem.base == X86_REG_BP) bp = true;
            if (bp) add("works in its caller's frame (" + hex4(ins.address) + " " + mn + " " + ins.os + ")");
        }
        if (mn == "mov" && ins.os == "bp, sp") {
            frame = true;
        } else if (has_reg(rg.written, X86_REG_BP) && !one_of(mn, {"push", "pop", "inc", "dec", "leave", "enter"}) && !frame) {
            add("BP as a plain register (" + hex4(ins.address) + ")");
        }
        // a flag read before anything sets one (a jump on the carry its caller left): an argument in the flags
        if (has_reg(rg.read, X86_REG_EFLAGS) && !flags_set && !one_of(mn, {"pushf", "pushfd"})) add("argument in the flags (" + hex4(ins.address) + " " + mn + ")");
        if (has_reg(rg.written, X86_REG_EFLAGS)) flags_set = true;
        // AX and DX swapped: a 32-bit result's halves put the other way round
        if (mn == "xchg") {
            std::string squeezed;
            for (char c : ins.os) if (c != ' ') squeezed += c;
            size_t comma = squeezed.find(',');
            if (comma != std::string::npos && squeezed.find(',', comma + 1) == std::string::npos) {
                std::string a = squeezed.substr(0, comma), b = squeezed.substr(comma + 1);
                if (a > b) std::swap(a, b);
                if (a == "ax" && b == "dx") add("swaps AX and DX (" + hex4(ins.address) + ")");
            }
        }
        // registers read before written (arguments in registers)
        for (int64_t r : rg.read) {
            std::string nm = md.reg_name(r).utf8();
            if (one_of(nm, {"ax", "bx", "cx", "dx", "si", "di", "al", "ah", "bl", "bh", "cl", "ch", "dl", "dh", "es"}) && !written.count(nm.substr(0, 1) + "x") &&
                !written.count(nm) && !one_of(mn, {"push", "xor", "sub"}) && !(prologue_ax && nm == "ax") && nm != "es") {
                add("argument in " + upper(nm) + " (" + hex4(ins.address) + ")");
            }
        }
        // BX, CX or DX pushed before the function set it: what its caller put there, kept (a C prologue saves SI, DI, DS)
        if (mn == "push" && one_of(ins.os, {"bx", "cx", "dx"})) {
            const std::string& r = ins.os;
            if (!written.count(r) && !written.count("e" + r) && !written.count(r.substr(0, 1) + "l") && !written.count(r.substr(0, 1) + "h")) {
                add("argument in " + upper(r) + " (" + hex4(ins.address) + " push)");
            }
        }
        // SI or DI pushed, as it came, right after a segment register: a far pointer made of it to pass on
        if (mn == "push" && one_of(ins.os, {"si", "di"}) && !written.count(ins.os) && last_push_seg) {
            add("argument in " + upper(ins.os) + " (" + hex4(ins.address) + " push after a segment)");
        }
        last_push_seg = mn == "push" && one_of(ins.os, {"ss", "ds", "es", "cs"});
        // a code address put in a register and called through it (with arguments in others)
        if (mn == "mov" && ops.size() == 2 && ops[0].type == X86_OP_REG && ops[1].type == X86_OP_IMM && starts.count(ops[1].imm)) {
            code_in[md.reg_name(ops[0].reg).utf8()] = ops[1].imm;
        } else if (mn == "call" && !ops.empty() && ops[0].type == X86_OP_REG && code_in.count(md.reg_name(ops[0].reg).utf8())) {
            add("calls a code address it put in " + ins.os + " (" + hex4(ins.address) + " call " + ins.os + ")");
        }
        if (pending && ins.address != pending->at) {
            const std::string& reg = pending->reg;
            std::set<std::string> alias = {reg, reg.substr(0, 1) + "l", reg.substr(0, 1) + "h"};
            bool hit_read = false, hit_write = false;
            for (const std::string& n : read_names) if (alias.count(n)) hit_read = true;
            for (const std::string& n : written_names) if (alias.count(n)) hit_write = true;
            if (hit_read) {
                add("takes back a pushed word as a value (" + hex4(pending->at) + " pop " + reg + ")");
                pending.reset();
            } else if (hit_write || pending->left <= 1) {
                pending.reset();
            } else {
                pending->left -= 1;
            }
        }
        prev_call = mn == "call" || mn == "lcall" || (prev_call && mn == "pop");
        if (written_names.count("ax") || written_names.count("eax")) ax_from_ss = mn == "mov" && ins.os == "ax, ss";
        for (int64_t r : rg.written) {
            std::string nm = md.reg_name(r).utf8();
            written.insert(nm);
            if (nm.size() == 2 && (nm[1] == 'l' || nm[1] == 'h')) written.insert(nm.substr(0, 1) + "x");
            else if (nm.size() == 3 && nm[0] == 'e') written.insert(nm.substr(1));   // (cdq, movsx eax ...: its 16-bit half too)
        }
        if (mn == "call" || mn == "lcall") {   // what a call leaves: its result in DX:AX, and the registers it may use
            for (const char* r : {"ax", "bx", "cx", "dx", "es"}) written.insert(r);
        } else if (mn == "int") {
            for (const char* r : {"ax", "bx", "cx", "dx", "si", "di", "es"}) written.insert(r);
        }
        // a jump out of the function: into the middle of other code (entry points sharing a body) or to another function
        // (a tail jump): the C compilers of the time do neither
        if ((mn == "jmp" || (!mn.empty() && mn[0] == 'j')) && !ops.empty() && ops[0].type == X86_OP_IMM && !(start <= (ops[0].imm & 0xFFFF) && (ops[0].imm & 0xFFFF) < end) && !starts.empty()) {
            add(std::string("jumps ") + (starts.count(ops[0].imm & 0xFFFF) ? "to another function" : "into the middle of other code") + " (" + hex4(ins.address) + " -> " + hex4(ops[0].imm & 0xFFFF) + ")");
        }
        // a return address of its own pushed (push the place to come back to, then jump): hand-made calling
        if (mn == "push" && !ops.empty() && ops[0].type == X86_OP_IMM && static_cast<int64_t>(ins.address) < ops[0].imm && ops[0].imm < end) {
            add("pushes a return address of its own (" + hex4(ins.address) + " push " + hex4(ops[0].imm) + ")");
        }
        if (mn == "iret") add("IRET (" + hex4(ins.address) + ")");
    }
    // SI or DI changed and never saved: given back changed -- a result, or a pointer moved on for the caller. The C
    // compilers keep both (a register variable is pushed first).
    std::set<std::string> changed, saved;
    for (const Insn& ins : body) {
        if (one_of(ins.mn, {"int", "call", "lcall", "pop"})) continue;
        if (!ins.regs_ok) continue;
        for (int64_t r : std::get<1>(ins.regs_access()).vec()) {
            std::string nm = md.reg_name(r).utf8();
            if (one_of(nm, {"si", "di", "esi", "edi"})) changed.insert(nm);
        }
    }
    for (const Insn& ins : body) if (ins.mn == "push") saved.insert(ins.os);
    for (const std::string& reg : changed) {
        std::string tail = reg.substr(reg.size() - 2);
        if (!saved.count(tail) && !saved.count("e" + tail)) add("gives " + upper(tail) + " back changed (never saved)");
    }
    // de-duplicate, keep the first few kinds
    std::set<std::string> seen;
    std::vector<std::string> ev;
    for (const std::string& e : evidence) {
        std::string k = kind_of(e);
        if (seen.insert(k).second) ev.push_back(e);
    }
    o.evidence = py::list<py::str>();
    for (const std::string& e : ev) o.evidence.append(py::str(e));
    o.language = py::str(ev.empty() ? "c" : "assembly");
}

}  // namespace

py::list<std::tuple<py::str, int64_t>> most_common(const Counter& c, const std::optional<int64_t>& n) {
    std::vector<std::tuple<py::str, int64_t>> items = c.items();
    std::stable_sort(items.begin(), items.end(), [](const auto& a, const auto& b) { return std::get<1>(a) > std::get<1>(b); });
    py::list<std::tuple<py::str, int64_t>> out;
    for (size_t i = 0; i < items.size() && (!n || static_cast<int64_t>(i) < *n); i++) out.append(items[i]);
    return out;
}

py::str Origin::repr() const {
    std::string ev;
    const py::list<py::str>& src = !evidence.empty() ? evidence : compiler;
    for (int64_t i = 0; i < src.size(); i++) ev += (i ? "; " : "") + src[i].utf8();
    if (ev.empty()) ev = "nothing either way";
    return py::str("<" + name.utf8() + " " + language.utf8() + ": " + ev + ">");
}

std::tuple<Counter, Counter> Result::summary() const {
    Counter n, why;
    for (const Origin& o : functions.values()) {
        n.set(o.language, n.get(o.language, int64_t(0)) + 1);
        if (!o.evidence.empty()) {
            py::str k = py::str(kind_of(o.evidence[0].utf8()));
            why.set(k, why.get(k, int64_t(0)) + 1);
        }
    }
    return {n, why};
}

std::tuple<int64_t, int64_t> _linker16(const py::str& path) {
    std::ifstream f(path.utf8(), std::ios::binary);
    if (!f) throw py::OSError("No such file or directory: " + path.utf8());
    std::string d((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
    int64_t o = 0;
    if (d.size() >= 0x40) o = static_cast<unsigned char>(d[0x3C]) | (static_cast<unsigned char>(d[0x3D]) << 8) | (static_cast<unsigned char>(d[0x3E]) << 16) |
                              (static_cast<int64_t>(static_cast<unsigned char>(d[0x3F])) << 24);
    if (static_cast<size_t>(o) + 2 <= d.size() && d[static_cast<size_t>(o)] == 'N' && d[static_cast<size_t>(o) + 1] == 'E') {
        if (static_cast<size_t>(o) + 3 >= d.size()) throw py::IndexError("index out of range");
        return {static_cast<unsigned char>(d[static_cast<size_t>(o) + 2]), static_cast<unsigned char>(d[static_cast<size_t>(o) + 3])};
    }
    return {0, 0};
}

Result _module16(const py::str& path, const std::optional<py::dict<int64_t, py::str>>& more) {
    py::str work = build::watcom::_scratch(py::list<py::str>{py::str("origin"), py::os::path::basename(path).lower()});
    auto d = describe::describe16(path, work);
    py::dict<int64_t, py::str> names;
    for (const auto& kv : d->names.items()) if (d->where.contains(std::get<0>(kv))) names.set(std::get<0>(kv), std::get<1>(kv));
    if (more) {
        for (const auto& kv : more->items()) {
            if (!names.contains(std::get<0>(kv))) {
                names.set(std::get<0>(kv), std::get<1>(kv));
                d->where.set(std::get<0>(kv), describe::where(std::get<0>(kv)));
            }
        }
    }
    std::vector<int64_t> seg_order;
    std::map<int64_t, std::vector<int64_t>> starts;
    for (int64_t va : names.keys()) {
        auto so = d->where.at(va);
        if (!starts.count(std::get<0>(so))) seg_order.push_back(std::get<0>(so));
        starts[std::get<0>(so)].push_back(std::get<1>(so));
    }
    for (auto& kv : starts) std::sort(kv.second.begin(), kv.second.end());
    auto md = describe::decoder16(true);
    Result res;
    auto link = _linker16(path);
    bool g3 = link >= std::make_tuple<int64_t, int64_t>(5, 30);   // (C 7 and 8: /G3 makes 32-bit registers)
    auto image_of = [&](int64_t seg) { auto it = d->images.get(seg); return it ? *it : py::bytes(); };
    std::vector<std::pair<int64_t, py::str>> sorted_names;
    for (const auto& kv : names.items()) sorted_names.emplace_back(std::get<0>(kv), std::get<1>(kv));
    std::sort(sorted_names.begin(), sorted_names.end(), [](const auto& a, const auto& b) { return a.first < b.first; });
    for (const auto& vn : sorted_names) {
        int64_t va = vn.first;
        auto so = d->where.at(va);
        int64_t seg = std::get<0>(so), off = std::get<1>(so);
        py::bytes image = image_of(seg);
        int64_t end = image.size();
        for (int64_t o : starts[seg]) if (o > off) { end = o; break; }
        Origin o(va, vn.second);
        std::set<int64_t> st(starts[seg].begin(), starts[seg].end());
        judge(o, *md, image, off, end, st, g3);
        if (g3 && o.language == "assembly") c8_forms(o, *md, image, off, end);
        res.functions.set(va, o);
    }
    // A function whose inner addresses a table names (KRNL386's fault chain: a start and an end of the instructions that
    // may fault, with where to go on if they do) was written in assembly: its instructions have to be where the table
    // says, which C's cannot be. Three words side by side -- a start, an end and where to go on -- all instructions of
    // the same function and none its start.
    for (int64_t seg : seg_order) {
        const std::vector<int64_t>& offs = starts[seg];
        py::bytes image = image_of(seg);
        std::map<int64_t, int64_t> inner;
        for (size_t k = 0; k < offs.size(); k++) {
            int64_t off = offs[k], end = k + 1 < offs.size() ? offs[k + 1] : image.size();
            for (const Insn& ins : disasm(*md, image, off, end, off).vec()) {
                if (static_cast<int64_t>(ins.address) != off) inner[static_cast<int64_t>(ins.address)] = off;
            }
        }
        std::map<int64_t, std::set<std::tuple<int64_t, int64_t, int64_t, int64_t>>> named;
        const std::string& raw = image.raw();
        for (int64_t at = 0; at < image.size() - 5; at += 2) {   // (a table of its own segment, not code)
            auto w = [&](int64_t o) { return static_cast<int64_t>(static_cast<unsigned char>(raw[static_cast<size_t>(o)])) | (static_cast<int64_t>(static_cast<unsigned char>(raw[static_cast<size_t>(o) + 1])) << 8); };
            int64_t a = w(at), b = w(at + 2), c = w(at + 4);
            auto ia = inner.find(a);
            if (a < b && ia != inner.end()) {
                auto ib = inner.find(b), ic = inner.find(c);
                if (ib != inner.end() && ib->second == ia->second && ic != inner.end() && ic->second == ia->second) named[ia->second].insert({seg, at, a, b});
            }
        }
        for (const auto& kv : res.functions.items()) {
            Origin o = std::get<1>(kv);
            auto so = d->where.at(std::get<0>(kv));
            if (std::get<0>(so) == seg && named.count(std::get<1>(so)) && o.language != "assembly") {
                auto first = *named[std::get<1>(so)].begin();
                char buf[160];
                std::snprintf(buf, sizeof buf, "its instructions %04llx-%04llx are named by a table (%d:%04llx)", static_cast<unsigned long long>(std::get<2>(first)),
                              static_cast<unsigned long long>(std::get<3>(first)), static_cast<int>(std::get<0>(first)), static_cast<unsigned long long>(std::get<1>(first)));
                o.evidence.append(py::str(buf));
                o.language = py::str("assembly");
                res.functions.set(std::get<0>(kv), o);
            }
        }
    }
    // A function that calls one taking its arguments in registers, having put them there itself, was written in assembly
    // as well (C passes arguments on the stack)
    std::map<std::pair<int64_t, int64_t>, Origin> by_place;
    for (const auto& kv : res.functions.items()) {
        auto so = d->where.at(std::get<0>(kv));
        by_place[{std::get<0>(so), std::get<1>(so)}] = std::get<1>(kv);
    }
    std::set<std::pair<int64_t, int64_t>> takes_registers;
    for (auto& kv : by_place) {
        bool arg = false;
        for (const py::str& e : kv.second.evidence.vec()) if (starts_with(e.utf8(), "argument in ")) arg = true;
        if (arg && kv.second.language == "assembly") takes_registers.insert(kv.first);
    }
    for (const auto& kv : res.functions.items()) {
        Origin o = std::get<1>(kv);
        if (o.language == "assembly") continue;
        auto so = d->where.at(std::get<0>(kv));
        int64_t seg = std::get<0>(so), off = std::get<1>(so);
        py::bytes image = image_of(seg);
        int64_t end = image.size();
        for (int64_t x : starts[seg]) if (x > off) { end = x; break; }
        for (const Insn& ins : disasm(*md, image, off, end, off).vec()) {
            if (ins.mn == "call" && !ins.operands.empty() && ins.operands[0].type == X86_OP_IMM) {
                int64_t to = ins.operands[0].imm & 0xFFFF;   // (a call back down wraps round)
                auto callee = by_place.find({seg, to});
                if (takes_registers.count({seg, to}) && callee != by_place.end()) {
                    // (whether it put them there or passes its own caller's on: C can do neither)
                    o.evidence.append(py::str("calls " + callee->second.name.utf8() + " with arguments in registers (" + hex4(ins.address) + ")"));
                    o.language = py::str("assembly");
                    res.functions.set(std::get<0>(kv), o);
                    break;
                }
            }
        }
    }
    return res;
}

Result module16(const py::str& path, const std::optional<py::dict<int64_t, py::str>>& more) { return _module16(path, more); }

int64_t main(const py::list<py::str>& argv) {
    if (argv.empty()) {
        std::cout << "python -m codebreaker origin <module>   which functions were written in assembly\n";
        return 2;
    }
    py::str path = py::os::path::isfile(argv[0]) ? argv[0] : *base::corpus::retail(argv[0]);
    Result r = module16(path);
    auto [n, why] = r.summary();
    char buf[512];
    std::snprintf(buf, sizeof buf, "%s: %d functions -- %d C, %d assembly\n", py::os::path::basename(path).utf8().c_str(), static_cast<int>(r.functions.size()),
                  static_cast<int>(n.get(py::str("c"), int64_t(0))), static_cast<int>(n.get(py::str("assembly"), int64_t(0))));
    std::cout << buf;
    for (const auto& kv : most_common(why, 12).vec()) {
        std::snprintf(buf, sizeof buf, "  %4d  %s\n", static_cast<int>(std::get<1>(kv)), std::get<0>(kv).utf8().c_str());
        std::cout << buf;
    }
    bool list = false;
    for (const py::str& a : argv.vec()) if (a == "--list") list = true;
    if (list) {
        for (const Origin& o : r.functions.values()) {
            std::string ev;
            const py::list<py::str>& src = !o.evidence.empty() ? o.evidence : o.compiler;
            for (int64_t i = 0; i < std::min<int64_t>(src.size(), o.evidence.empty() ? 2 : 3); i++) ev += (i ? "; " : "") + src[i].utf8();
            std::snprintf(buf, sizeof buf, "  %-28s %-9s %s\n", o.name.utf8().c_str(), o.language.utf8().c_str(), ev.c_str());
            std::cout << buf;
        }
    }
    return 0;
}

}  // namespace codebreaker::analyze::origin
