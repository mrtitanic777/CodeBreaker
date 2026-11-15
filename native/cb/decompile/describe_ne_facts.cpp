// cb/decompile/describe_ne_facts.cpp -- NE: how a function returns (its arguments, its result, jumps by return, forwards)
// and what the callers say (see describe.hpp).
// cb-unity: alone

#include <deque>

#include "cb/core/capstone_api.hpp"
#include "cb/decompile/describe_internal.hpp"

namespace cb::decompile::describe {

using namespace detail;

namespace {

bool hits(const std::vector<std::uint16_t>& regs, std::initializer_list<int> set) {
    for (std::uint16_t r : regs)
        for (int s : set)
            if (r == s) return true;
    return false;
}
bool hits_ax(const std::vector<std::uint16_t>& regs) { return hits(regs, {X86_REG_AX, X86_REG_AL, X86_REG_AH, X86_REG_EAX}); }
bool hits_dx(const std::vector<std::uint16_t>& regs) { return hits(regs, {X86_REG_DX, X86_REG_DL, X86_REG_DH, X86_REG_EDX}); }
bool writes_sp(const Insn& i) {
    return std::find(i.regs_write.begin(), i.regs_write.end(), X86_REG_SP) != i.regs_write.end();
}
bool ends_flow(std::string_view m) { return eq_any(m, {"ret", "retf", "iret", "jmp", "ljmp"}); }

std::set<std::int64_t> jump_targets(const std::vector<Insn>& ins) {
    std::set<std::int64_t> t;
    for (const Insn& i : ins)
        if (i.in_group(kGroupJump) && i.imm_first()) t.insert(i.first_imm() & 0xFFFF);
    return t;
}

// Bytes a push puts on the 16-bit stack (2, or 4 for a 32-bit one), 0 for anything else.
std::int64_t push_bytes(const Insn& i) {
    if (i.mn != "push") return 0;
    return (i.op_str.starts_with("e") || i.op_str.starts_with("dword")) ? 4 : 2;
}
// Bytes a plain `ret` / `retf` takes off for the address it goes to (none when it also removes arguments: `retf 6` is a
// function's own return).
std::int64_t return_pops(const Insn& i) {
    if ((i.mn != "ret" && i.mn != "retf") || !i.op_str.empty()) return 0;
    return i.mn == "ret" ? 2 : 4;
}

// "ax, 0x1f" / "ax, 31"
bool ax_const(std::string_view os) { return os.starts_with("ax, ") && is_num_lit(os.substr(4)); }

// The direct call at `i` by its target place ("call 0x.." near, "lcall 0x.., 0x.." far into the module), or none. A far
// call to an import (selector >= 0xE000) is none, as the callers write it.
std::optional<SegOff> direct_call_target(const Insn& i, std::int64_t seg) {
    if (i.mn == "call" && i.op_str.starts_with("0x") && !contains(i.op_str, ","))
        return SegOff{static_cast<std::uint32_t>(seg), static_cast<std::uint32_t>(number_of(i.op_str).value_or(0) & 0xFFFF)};
    if (i.mn == "lcall" && contains(i.op_str, ",") && i.op_str.starts_with("0x")) {
        const std::vector<std::string_view> parts = text::split(i.op_str, ", ");
        const std::int64_t s = number_of(parts[0]).value_or(0), o = number_of(parts.at(1)).value_or(0);
        if (s < kExternSelector) return where((s << 16) | o);
    }
    return std::nullopt;
}
bool is_direct_call(const Insn& i) {
    return (i.mn == "call" && i.op_str.starts_with("0x") && !contains(i.op_str, ",")) ||
           (i.mn == "lcall" && contains(i.op_str, ",") && i.op_str.starts_with("0x"));
}

}  // namespace

// Is ins[k] a return that is a jump -- the instructions straight before it push exactly what it takes off
// (`push dx / push ax / retf`, `push ax / ret`), none of them a place jumped to, and what they push is not a return
// address popped earlier? It goes to the address pushed, the stack as the function was entered: a tail call through a
// pointer. VER's sub_4_003E jumps so to the LZEXPAND function GetProcAddress gave it (`mov dx, [4] / mov ax, [2] ... push
// dx / push ax / retf`). USER's 1:81F5 is not one: `pop ax / pop dx / pop bx / pop cx ... push dx / push ax / retf` takes
// its arguments off by hand and returns to where it was called from -- what is pushed was last set by a pop, and is left
// a return (as is anything not found).
bool jump_by_return(const std::vector<Insn>& ins, std::size_t k, const std::set<std::int64_t>& targets) {
    const std::int64_t want = return_pops(ins[k]);
    if (!want || targets.count(ins[k].address)) return false;
    std::int64_t got = 0;
    std::ptrdiff_t j = static_cast<std::ptrdiff_t>(k) - 1;
    std::vector<std::string> regs;
    while (j >= 0 && got < want) {
        const Insn& pj = ins[static_cast<std::size_t>(j)];
        const std::int64_t n = push_bytes(pj);
        if (!n || pj.end() != ins[static_cast<std::size_t>(j) + 1].address) return false;
        got += n;
        if (got < want && targets.count(pj.address)) return false;
        regs.push_back(pj.op_str);
        --j;
    }
    if (got != want) return false;
    // (what set each pushed register last, straight back from the pushes: a constant pushed is an address of its own --
    // `push seg / push off / retf`)
    for (const std::string& reg : regs) {
        if (reg.starts_with("0x") || reg.starts_with("word ptr") || reg.starts_with("cs") || reg.starts_with("ss") ||
            reg.starts_with("ds") || reg.starts_with("es") || is_dec_lit(reg))
            continue;
        bool found = false;
        for (std::ptrdiff_t qi = j; qi >= 0; --qi) {
            const Insn& q = ins[static_cast<std::size_t>(qi)];
            if (ends_flow(q.mn)) break;   // (what is before is not on the way in)
            if (!q.regs_ok) return false;
            const bool wrote = std::any_of(q.regs_write.begin(), q.regs_write.end(), [&](std::uint16_t w) { return register_name16(w) == reg; });
            if (wrote) {
                if (q.mn == "pop") return false;   // (a return address given back)
                found = true;
                break;
            }
        }
        if (!found) return false;
    }
    return true;
}

// The addresses of returns that are jumps (jump_by_return), in every code segment.
std::vector<Va> return_jumps(const NeDescription& d) {
    std::vector<Va> out;
    for (const auto& [seg, image] : d.images) {   // (ascending segment order)
        if (!d.code.empty() && !d.code.contains(seg)) continue;
        const std::vector<Insn> ins = d.relocated(seg).decode(0, d.relocated(seg).size(), true);
        const std::set<std::int64_t> targets = jump_targets(ins);
        for (std::size_t k = 0; k < ins.size(); ++k) {
            const Insn& i = ins[k];
            if ((i.mn == "ret" || i.mn == "retf") && i.op_str.empty() && jump_by_return(ins, k, targets)) out.push_back(address(seg, i.address));
        }
    }
    return out;
}

// Bytes of arguments a function takes off the stack itself at its start: its return address popped (one word near, two
// far), then more words popped, then the return address pushed back. nullopt when it does not start so.
std::optional<std::int64_t> popped_by_hand(const Code16& code, std::int64_t off, Distance kind) {
    std::vector<Insn> ins = code.decode(off, off + 40, false);
    if (ins.size() > 20) ins.resize(20);
    const std::int64_t words = kind == Distance::Far ? 2 : 1;
    const auto in_regs = [](std::string_view s) { return eq_any(s, {"ax", "bx", "cx", "dx", "si", "di", "bp", "es"}); };
    // (leaves the stack and the flow alone)
    const auto quiet = [](const Insn& i) {
        return !eq_any(i.mn, {"push", "pop", "call", "lcall", "ret", "retf", "jmp", "ljmp", "int", "enter", "leave", "pushf", "popf",
                              "pusha", "popa"}) &&
               !i.mn.starts_with("j") && !contains(i.op_str, "sp");
    };
    std::size_t k = 0;
    while (k < ins.size() && k < 3 && quiet(ins[k])) ++k;   // (IsIconic: `mov cl, 20h / test ax, ...` first)
    if (k < ins.size() && ins[k].mn == "call" && ins[k].op_str.starts_with("0x") && !contains(ins[k].op_str, ",")) {
        // a helper does it for it (USER's PostQuitMessage, GetSysColor: `call 8128`, which pops its own return address, this
        // one's, and the arguments, pushes this one's back and `jmp bx` home): the helper's pops past its own return address
        // are this function's own
        const std::int64_t helper = number_of(ins[k].op_str).value_or(0) & 0xFFFF;
        if (helper != off) {
            std::vector<Insn> h = code.decode(helper, helper + 40, false);
            if (h.size() > 20) h.resize(20);
            std::vector<std::string> hp;
            std::size_t j = 0;
            while (j < h.size() && h[j].mn == "pop" && in_regs(h[j].op_str)) hp.push_back(h[j++].op_str);
            if (static_cast<std::int64_t>(hp.size()) > 1 + words) {
                while (j < h.size() && j < 14 && quiet(h[j])) ++j;
                std::vector<std::string> back;
                for (std::size_t q = j; q < std::min(h.size(), j + static_cast<std::size_t>(words)); ++q)
                    if (h[q].mn == "push") back.push_back(h[q].op_str);
                bool home = false;
                for (std::size_t q = j + static_cast<std::size_t>(words); q < std::min(h.size(), j + static_cast<std::size_t>(words) + 6); ++q)
                    if (h[q].mn == "jmp" && h[q].op_str == hp[0]) home = true;
                std::vector<std::string> want;   // the return address as it came: hp[1:1 + words], last popped first
                for (std::int64_t q = std::min<std::int64_t>(1 + words, static_cast<std::int64_t>(hp.size())) - 1; q >= 1; --q)
                    want.push_back(hp[static_cast<std::size_t>(q)]);
                if (back == want && home) return 2 * (static_cast<std::int64_t>(hp.size()) - 1 - words);
            }
        }
        return std::nullopt;
    }
    std::vector<std::string> pops;
    while (k < ins.size() && ins[k].mn == "pop" && in_regs(ins[k].op_str)) pops.push_back(ins[k++].op_str);
    if (static_cast<std::int64_t>(pops.size()) <= words) return std::nullopt;
    while (k < ins.size() && k < 12 && quiet(ins[k])) {   // (GetAsyncKeyState: `xor ah, ah` between)
        const std::string_view first = before_comma(ins[k].op_str);
        for (std::int64_t q = 0; q < std::min<std::int64_t>(words, static_cast<std::int64_t>(pops.size())); ++q)
            if (contains(first, pops[static_cast<std::size_t>(q)])) return std::nullopt;   // (its return address changed: not given back)
        ++k;
    }
    std::vector<std::string> back;
    for (std::size_t q = k; q < std::min(ins.size(), k + static_cast<std::size_t>(words)); ++q)
        if (ins[q].mn == "push") back.push_back(ins[q].op_str);
    // (the return address pushed back as it came: the last popped of it first)
    std::vector<std::string> want;
    for (std::int64_t q = std::min<std::int64_t>(words, static_cast<std::int64_t>(pops.size())) - 1; q >= 0; --q) want.push_back(pops[static_cast<std::size_t>(q)]);
    if (back != want) return std::nullopt;
    return 2 * (static_cast<std::int64_t>(pops.size()) - words);
}

// The prologue of a far function that Windows calls (exports, callbacks): `mov ax,ds` or `push ds; pop ax`, `nop`,
// `inc bp; push bp` -- the odd BP marking a far frame.
bool windows_far_prologue(ByteView code, std::int64_t off) {
    if (off < 0 || static_cast<std::size_t>(off) >= code.size()) return false;
    const ByteView head = code.subspan(static_cast<std::size_t>(off), std::min<std::size_t>(6, code.size() - static_cast<std::size_t>(off)));
    auto starts = [&](std::initializer_list<int> p) {
        return head.size() >= p.size() && std::equal(p.begin(), p.end(), head.begin());
    };
    return starts({0x8C, 0xD8, 0x90, 0x45}) || starts({0x1E, 0x58, 0x90, 0x45}) || starts({0x45, 0x55});
}

// The bytes of arguments a function reads -- through BP, after the prologue sets it (`enter`, or `push bp / mov bp, sp`)
// -- or nullopt when it keeps no such frame (its reads are not known here).
std::optional<std::int64_t> args_read(const Code16& code, std::int64_t start, std::int64_t end, Distance kind) {
    if (windows_far_prologue(code.bytes(), start)) return std::nullopt;
    const std::vector<Insn> ins = code.decode(start, std::min(end, start + 0x4000), true);
    std::size_t k;
    if (!ins.empty() && ins[0].mn == "enter") {
        k = 1;
    } else if (ins.size() > 1 && ins[0].mn == "push" && ins[0].op_str == "bp" && ins[1].mn == "mov" && ins[1].op_str == "bp, sp") {
        k = 2;
    } else {
        return std::nullopt;
    }
    const std::int64_t first = kind == Distance::Far ? 6 : 4;   // (past the saved BP and the return address)
    std::int64_t most = 0;
    for (std::size_t q = k; q < ins.size(); ++q) {
        const Insn& i = ins[q];
        if (eq_any(i.mn, {"mov", "lea", "pop", "xchg"}) && i.op_str.starts_with("bp")) break;   // (BP made something else: no longer the frame)
        for (std::size_t o = 0; o < i.operands.size(); ++o) {
            const Operand& op = i.operands[o];
            if (op.kind == OperandKind::Memory && op.base == X86_REG_BP && op.index == 0 && (op.segment == 0 || op.segment == X86_REG_SS) &&
                op.disp >= first) {
                // (a far pointer loaded or called through is four bytes, whatever Capstone says)
                const std::int64_t size = i.mn == "lea" ? 2 : eq_any(i.mn, {"les", "lds", "lss", "lfs", "lgs", "lcall", "ljmp"}) ? 4 : op.size;
                most = std::max(most, op.disp + std::max<std::int64_t>(size, 2) - first);
            }
        }
    }
    return most + (most & 1);
}

// What a function returns, by its instructions: 2 when on a way to one of its returns the last thing to set AX is its own
// (past the prologue's `mov ax, ds` / `push ds; pop ax` / `mov ax, DGROUP` ahead of `mov ds, ax`, and not a `pop ax` -- AX
// given back as it came) or a call (what the callee left there goes back as it came); 4 when DX is set there as well.
// 0 when no return is seen to carry anything. Walked back from each return through its straight run of instructions.
// 0 as well when a way to a return sets AX nowhere past that prologue: AX is then the data segment the prologue loaded -- a
// register as it came, not a result -- and a function that returns nothing on one way returns nothing at all (LZEXPAND's
// LZDONE: `dec word ptr [32h] / jne` straight to the return, the other way copying globals through AX) -- unless on
// another way AX is set and not read again before the return (LMOUSE31's CPLAPPLET: `mov ax, 0 / jmp` to the return; DS on
// its other way is a nonzero answer as well). `voided`, given a list, has what the other ways would have made it appended
// (results() keeps that where a caller reads AX after all), and `passes` is emptied.
std::int64_t result_bytes(const Code16& code, std::int64_t start, std::int64_t end, std::vector<PassOn>* passes,
                          std::vector<std::int64_t>* voided) {
    const std::vector<Insn> ins = code.decode(start, end, true);
    const std::int64_t N = static_cast<std::int64_t>(ins.size());
    auto insn = [&](std::int64_t q) -> const Insn& { return ins[static_cast<std::size_t>(q)]; };
    // (Windows' own prologue loads DS seventh or eighth: `mov ax, ds` or `push ds; pop ax`, `nop`, `inc bp; push bp; mov bp,
    // sp; push ds; mov ds, ax`)
    const bool windows = windows_far_prologue(code.bytes(), start);
    std::int64_t headk = -1;
    for (std::int64_t q = 0; q < std::min<std::int64_t>(N, windows ? 8 : 6); ++q)
        if (insn(q).mn == "mov" && insn(q).op_str == "ds, ax") {
            headk = q;
            break;
        }
    const std::int64_t prologue = headk >= 0 ? insn(headk).end() : start;
    // (AX there is the data segment -- the caller's, which Windows makes the module's own, or DGROUP's `mov ax, seg` -- not
    // an argument loaded into DS)
    const std::int64_t k = headk >= 0 ? headk : 0;
    const bool ds_in_ax = headk >= 0 && (windows || (k > 0 && insn(k - 1).mn == "mov" && ax_const(insn(k - 1).op_str)));
    const std::set<std::int64_t> targets = jump_targets(ins);
    // every way into each instruction: the one before it (unless that one jumps away or returns) and every jump to it
    std::unordered_map<std::int64_t, std::int64_t> at;
    for (std::int64_t q = 0; q < N; ++q) at[insn(q).address] = q;
    std::unordered_map<std::int64_t, std::vector<std::int64_t>> into;   // lookup only
    for (std::int64_t q = 0; q < N; ++q) {
        const Insn& i = insn(q);
        if (q + 1 < N && !ends_flow(i.mn)) into[insn(q + 1).address].push_back(i.address);
        if (i.in_group(kGroupJump) && i.imm_first()) into[i.first_imm() & 0xFFFF].push_back(i.address);
    }
    static const std::vector<std::int64_t> kNone;
    auto into_of = [&](std::int64_t a) -> const std::vector<std::int64_t>& {
        const auto it = into.find(a);
        return it == into.end() ? kNone : it->second;
    };
    // (a `pop dx` hands DX a value -- `push ax ... pop dx` -- unless the function saves DX itself, when it is DX given back)
    const bool saves_dx = std::any_of(ins.begin(), ins.end(), [](const Insn& i) { return i.mn == "push" && (i.op_str == "dx" || i.op_str == "edx"); });
    if (passes) {   // a jump on into another function's body: its result is that one's
        for (const Insn& i : ins)
            if (i.mn == "jmp" && i.imm_first() && !(start <= (i.first_imm() & 0xFFFF) && (i.first_imm() & 0xFFFF) < end))
                passes->push_back({PassOn::Kind::Into, i.first_imm() & 0xFFFF});
    }
    auto reads_dx = [](const Insn& p, const std::vector<std::uint16_t>& r) { return p.mn == "int" || hits_dx(r); };   // (an `int` reads what it is handed: DOS's DS:DX)

    auto read_after = [&](std::int64_t a) {
        // DX read from what sets AX on to the return, along the way it runs (a conditional jump's fall-through; a jump
        // followed): USER's 21:58E6 `mov ds, dx`, GDI's 4:168F `cmp dx, di` -- DX still in use there, not handed back
        for (int n = 0; n < 24; ++n) {
            const auto it = at.find(a);
            if (it == at.end()) return true;
            const Insn& p = insn(it->second);
            if (p.mn == "ret" || p.mn == "retf") return false;
            if (!p.regs_ok) return true;
            if (reads_dx(p, p.regs_read)) return true;
            if (p.in_group(kGroupCall)) return true;
            a = (p.mn == "jmp" && p.imm_first()) ? (p.first_imm() & 0xFFFF) : p.end();
        }
        return true;
    };

    auto segment_in_dx = [&](std::int64_t a) {
        // Before what sets AX, along the straight run into it: the first thing to set DX is a segment register put there
        // (`mov dx, es`) -- the selector of a far pointer that goes back in DX:AX (KRNL386's `mov es, sel / mov dx, es / mov
        // ax, es:[off]`) -- or an argument nothing reads again before the return (SUWIN's 3:319C `mov ax, [bp+6] / mov dx,
        // [bp+8] / ... sub ax, cx / add ax, 1FEh / leave / retf`: a pointer into the segment it was given; a compiler writes
        // no DX it will not use). A number or a sum there is as often scratch, and is left out.
        const auto pit = at.find(a);
        bool read;
        if (pit == at.end()) {
            read = true;
        } else {
            const Insn& p = insn(pit->second);
            read = !p.regs_ok || reads_dx(p, p.regs_read) || read_after(a);
        }
        for (int n = 0; n < 8; ++n) {
            const std::vector<std::int64_t>& prev = into_of(a);
            if (prev.size() != 1 || !at.count(prev[0]) || prev[0] < prologue) return false;
            a = prev[0];
            const Insn& p = insn(at[a]);
            if (p.in_group(kGroupCall)) return false;
            if (!p.regs_ok) return false;
            if (hits_dx(p.regs_write)) {
                std::string_view src;
                if (p.mn == "mov") {
                    const std::size_t kk = p.op_str.rfind(", ");
                    src = kk == std::string::npos ? std::string_view(p.op_str) : std::string_view(p.op_str).substr(kk + 2);
                }
                return eq_any(src, {"es", "ds", "ss", "cs"}) || (src.starts_with("word ptr [bp + ") && !read);
            }
            read = read || reads_dx(p, p.regs_read);   // (DX read on the way: scratch)
        }
        return false;
    };

    // (`bare`: a way to a return sets no AX past the prologue; `evident`: on one, what sets AX last is not read again before
    // the return -- `mov ax, 0 / jmp done` -- a value set for the caller, where one read on the way -- `mov ax, [34h] / mov
    // [44h], ax` -- was a scratch copy)
    std::int64_t best = 0;
    bool bare = false, evident = false;
    struct Work {
        std::int64_t a;
        bool wide;
        std::int64_t steps;
        bool opaque, used;
    };
    for (std::int64_t k_ret = 0; k_ret < N; ++k_ret) {
        const Insn& i = insn(k_ret);
        if (i.mn != "ret" && i.mn != "retf") continue;
        if (jump_by_return(ins, static_cast<std::size_t>(k_ret), targets)) {
            // (a jump through the address pushed: what the function jumped to returns -- a call's result, passed on; DX:AX
            // here were the address, not a result)
            if (passes) passes->push_back({PassOn::Kind::Unknown, 0});
            else best = std::max<std::int64_t>(best, 2);
            continue;
        }
        // back from the return along every way into it, to what last set AX on each (DX set too makes it four bytes only
        // right at the end -- `mov ax, lo / mov dx, hi` -- or by what sets AX itself: DX on the way is as often a scratch
        // register)
        std::vector<Work> work;
        for (std::int64_t p : into_of(i.address)) work.push_back({p, false, 0, false, false});
        std::set<std::tuple<std::int64_t, bool, bool, bool>> seen;
        while (!work.empty()) {
            const Work w0 = work.back();
            work.pop_back();
            const std::int64_t a = w0.a;
            const bool wide = w0.wide, used = w0.used;
            bool opaque = w0.opaque;
            const std::int64_t steps = w0.steps;
            if (at.count(a) && ds_in_ax && a < prologue && !opaque) bare = true;   // (into the prologue, AX still its DS)
            if (seen.count({a, wide, opaque, used}) || !at.count(a) || a < prologue) continue;
            seen.insert({a, wide, opaque, used});
            const Insn& p = insn(at[a]);
            // (an interrupt's results are not all in what Capstone says it writes)
            opaque = opaque || eq_any(p.mn, {"int", "into", "int3"});
            if (p.in_group(kGroupCall)) {
                if (passes) {   // (its size is the callee's: see results())
                    if (p.imm_first() && p.bytes[0] == 0xE8) passes->push_back({PassOn::Kind::Near, p.first_imm() & 0xFFFF});
                    else passes->push_back({PassOn::Kind::Unknown, 0});
                    best = std::max<std::int64_t>(best, wide ? 4 : 0);
                } else {
                    best = std::max<std::int64_t>(best, wide ? 4 : 2);   // the callee's result, passed on
                }
                continue;
            }
            if (!p.regs_ok) continue;
            if (hits_ax(p.regs_write)) {
                if (p.mn != "pop") {
                    best = std::max<std::int64_t>(best, (wide || hits_dx(p.regs_write) || segment_in_dx(p.address)) ? 4 : 2);
                    evident = evident || !used;
                }
                continue;
            }
            const bool wn = wide || (hits_dx(p.regs_write) && steps < 3 && (p.mn != "pop" || !saves_dx));
            const bool u = used || hits_ax(p.regs_read);
            const std::int64_t ns = steps + ((p.mn == "pop" || p.mn == "leave" || p.mn == "push") ? 0 : 1);
            for (std::int64_t q : into_of(a)) work.push_back({q, wn, ns, opaque, u});
        }
    }
    if (bare && !evident && (best || (passes && !passes->empty()))) {
        if (voided) voided->push_back(best ? best : 2);
        if (passes) passes->clear();
        return 0;
    }
    return best;
}

// `J` lines: each switch jump through a table in its own code segment, `jmp word ptr cs:[reg + table]`, with the places its
// table sends to. The analysis finds the table by the range it can prove for the index -- and the range it proves can be
// wrong: USER's 1:03A5 indexes by a byte offset known to be even (the code tested the low four bits, then shifted), the
// analysis took odd offsets too, read garbage words as places, and followed them through half the segment (sub_1_032E:
// 1.5 GB). Here the table is read as the code bounds it: the compare that guards the jump (`cmp reg, n` / `ja`), the index
// a byte offset unless doubled before (`shl bx, 1` / `add bx, bx`) -- and no further than where the code it sends to begins.
std::vector<std::string> jump_tables(const NeDescription& d) {
    std::vector<std::string> lines;
    for (const auto& [sg, codeb] : d.segment_bytes) {
        const Code16& seg = d.loaded(sg);
        const ByteView code = codeb;
        std::size_t at = 0;
        for (;;) {
            // a `jmp word ptr cs:[...]` starts 2E FF
            while (at + 1 < code.size() && !(code[at] == 0x2E && code[at + 1] == 0xFF)) ++at;
            if (at + 1 >= code.size()) break;
            const std::int64_t here = static_cast<std::int64_t>(at);
            const std::vector<Insn> got = seg.decode(here, here + 8, false, 1);
            at += 1;
            if (got.empty() || got[0].mn != "jmp") continue;
            const Insn& jmp = got[0];
            // `word ptr cs:[(bx|si|di) + 0x<hex>]` as a whole
            std::string reg;
            std::int64_t table = 0;
            {
                static constexpr std::string_view kPre = "word ptr cs:[";
                const std::string_view os = jmp.op_str;
                if (!(os.starts_with(kPre) && os.size() > kPre.size() + 2 + 5 && os.back() == ']')) continue;
                const std::string_view r = os.substr(kPre.size(), 2), rest = os.substr(kPre.size() + 2);
                if (!eq_any(r, {"bx", "si", "di"}) || !rest.starts_with(" + 0x")) continue;
                const std::string_view digits = rest.substr(5, rest.size() - 6);
                if (digits.empty() || !is_hex_lit(std::string("0x") + std::string(digits))) continue;
                reg = r;
                table = hex_of(digits).value_or(0);
            }
            // the guard: back from the jump, the last conditional branch on an unsigned compare
            std::vector<Insn> back;
            const std::int64_t iaddr = jmp.address;
            for (std::int64_t start = std::max<std::int64_t>(0, iaddr - 24); start < iaddr; ++start) {   // (a decode in step with the jump)
                std::vector<Insn> cand = seg.decode(start, iaddr, false);
                if (!cand.empty() && cand.back().end() == iaddr) {
                    std::int64_t sum = 0;
                    for (const Insn& c : cand) sum += c.size;
                    if (sum == iaddr - start) {
                        back = std::move(cand);
                        break;
                    }
                }
            }
            if (back.empty()) continue;
            std::optional<std::int64_t> bound;
            bool doubled = false;
            for (std::ptrdiff_t k = static_cast<std::ptrdiff_t>(back.size()) - 1; k >= 0; --k) {
                const Insn& i = back[static_cast<std::size_t>(k)];
                if (((i.mn == "shl" || i.mn == "sal") && i.op_str == reg + ", 1") || (i.mn == "add" && i.op_str == reg + ", " + reg)) doubled = true;
                // index <= bound (1) or < bound (0)
                const int inclusive = i.mn == "ja" || i.mn == "jbe" ? 1 : i.mn == "jae" || i.mn == "jb" ? 0 : -1;
                if (inclusive >= 0 && k > 0 && back[static_cast<std::size_t>(k) - 1].mn == "cmp") {
                    const std::string& cos = back[static_cast<std::size_t>(k) - 1].op_str;
                    // `<word>, <number>`
                    const std::size_t cm = cos.find(", ");
                    if (cm != std::string::npos && cm > 0 &&
                        std::all_of(cos.begin(), cos.begin() + static_cast<std::ptrdiff_t>(cm), [](unsigned char c) { return std::isalnum(c) || c == '_'; }) &&
                        is_num_lit(std::string_view(cos).substr(cm + 2)))
                        bound = number_of(std::string_view(cos).substr(cm + 2)).value_or(0) + inclusive - 1;   // (largest index)
                    break;
                }
            }
            if (!bound || *bound < 0 || *bound > 0x400) continue;
            const std::int64_t count = doubled ? *bound + 1 : *bound / 2 + 1;
            std::vector<std::int64_t> targets;
            for (std::int64_t k = 0; k < count; ++k) {
                const std::int64_t p = table + 2 * k;
                if (p + 2 > static_cast<std::int64_t>(code.size())) break;
                const std::int64_t t = word_at(code, p);
                if ((table <= t && t < table + 2 * count) || t >= static_cast<std::int64_t>(code.size())) break;   // (into the table itself: past its end)
                targets.push_back(t);
            }
            if (!targets.empty()) {
                std::string joined;
                for (std::size_t q = 0; q < targets.size(); ++q) {
                    if (q) joined += ",";
                    joined += hx(address(sg, targets[q]));
                }
                lines.push_back("J\t" + hx(address(sg, iaddr)) + "\t" + joined);
            }
        }
    }
    return lines;
}

// the bytes of arguments the caller takes off right after the call ins[j] -- `add sp, n`, or `pop bx` / `pop cx` a word
// each (never DX, where a result's high word comes back) -- or 0
std::int64_t caller_cleanup(const std::vector<Insn>& ins, std::size_t j) {
    if (j + 1 >= ins.size()) return 0;
    const Insn& nxt = ins[j + 1];
    if (nxt.mn == "add" && nxt.op_str.starts_with("sp, ") && is_num_lit(std::string_view(nxt.op_str).substr(4))) {
        const std::int64_t n = number_of(std::string_view(nxt.op_str).substr(4)).value_or(0);
        return (0 < n && n < 0x100 && n % 2 == 0) ? n : 0;
    }
    std::int64_t words = 0;
    for (std::size_t q = j + 1; q < std::min(ins.size(), j + 4); ++q) {
        if (ins[q].mn == "pop" && (ins[q].op_str == "bx" || ins[q].op_str == "cx")) ++words;
        else break;
    }
    return 2 * words;
}

namespace {

// nothing after the call ins[j] touches the stack before the frame's LEAVE
bool leave_follows(const std::vector<Insn>& ins, std::size_t j) {
    for (std::size_t q = j + 1; q < std::min(ins.size(), j + 12); ++q) {
        const Insn& p = ins[q];
        if (p.mn == "leave" || (p.mn == "mov" && p.op_str == "sp, bp")) return true;
        if (eq_any(p.mn, {"push", "pop", "pushf", "popf", "enter", "ret", "retf"}) || p.in_group(kGroupCall) || p.in_group(kGroupJump)) return false;
        if (!p.regs_ok || writes_sp(p)) return false;
    }
    return false;
}

// the bytes pushed just before the call ins[j] (back to a jump target, the prologue, or anything else that moves SP);
// `push cs` of a near call to a far function not counted
std::int64_t pushed_before(const std::vector<Insn>& ins, std::size_t j, const std::set<std::int64_t>& targets) {
    std::int64_t pushed = 0;
    std::ptrdiff_t n = static_cast<std::ptrdiff_t>(j);
    if (n > 0 && ins[static_cast<std::size_t>(n) - 1].mn == "push" && ins[static_cast<std::size_t>(n) - 1].op_str == "cs") --n;
    for (std::ptrdiff_t q = n - 1; q >= 0; --q) {
        const Insn& p = ins[static_cast<std::size_t>(q)];
        if (p.mn == "push") {
            pushed += p.first_size() == 4 ? 4 : 2;
            if (targets.count(p.address)) break;
            continue;
        }
        if (p.in_group(kGroupCall) || p.in_group(kGroupJump) || eq_any(p.mn, {"ret", "retf", "pop", "leave", "enter"}) || targets.count(p.address)) break;
        if (!p.regs_ok || writes_sp(p)) break;
    }
    return pushed;
}

}  // namespace

// `O` lines: for each call through a pointer (a driver's dispatch table, a callback), the words the code pushed for it just
// before -- the callee is unknown, so the analysis cannot tell what it takes, and drops what was pushed. Removed by the
// callee (Pascal: Windows' drivers and callbacks) unless the caller takes them off itself right after (`add sp, n`).
std::vector<std::string> call_overrides(const NeDescription& d) {
    std::vector<std::int64_t> segorder;
    std::map<std::int64_t, std::vector<std::pair<std::int64_t, Va>>> by_seg;
    for (const auto& [va, name] : d.names) {
        const SegOff w = where(va);
        if (!by_seg.count(w.seg)) segorder.push_back(w.seg);
        by_seg[w.seg].push_back({w.off, va});
    }
    std::vector<std::string> lines;
    for (const std::int64_t sg : segorder) {
        auto& lst = by_seg[sg];
        const Code16& code = d.loaded(sg);
        std::sort(lst.begin(), lst.end());
        for (std::size_t k = 0; k < lst.size(); ++k) {
            const std::int64_t off = lst[k].first;
            const Va va = lst[k].second;
            const std::int64_t end = k + 1 < lst.size() ? lst[k + 1].first : code.size();
            const std::vector<Insn> ins = code.decode(off, end, true);
            std::set<std::int64_t> targets = jump_targets(ins);
            // (what the prologue pushes is saved registers, not arguments: the count stops there)
            std::int64_t prologue = off;
            for (std::size_t q = 0; q < std::min<std::size_t>(ins.size(), 12); ++q) {
                const Insn& i = ins[q];
                if (i.mn == "nop" || i.mn == "enter" || i.op_str == "ax, ds" || i.op_str == "bp, sp" || i.op_str == "ds, ax" ||
                    ((i.mn == "push" || i.mn == "inc") && eq_any(i.op_str, {"bp", "ds", "si", "di"})) || (i.mn == "pop" && i.op_str == "ax") ||
                    (i.mn == "mov" && i.op_str.starts_with("ax, 0x")) || (i.mn == "sub" && i.op_str.starts_with("sp,"))) {
                    prologue = i.end();
                } else {
                    break;
                }
            }
            targets.insert(prologue);
            for (std::size_t j = 0; j < ins.size(); ++j) {
                const Insn& i = ins[j];
                if (!i.in_group(kGroupCall) || i.operands.empty()) continue;
                if (i.operands[0].kind == OperandKind::Immediate) {
                    // A direct call's callee is described -- but a C function's description says only that it removes nothing,
                    // not what it takes: its caller says that, in the cleanup right after the call (`add sp, 6`; at /Os `pop
                    // bx`/`pop cx` for each word). SUWIN's sub_2_0A92 passed sub_8_0F24 a far pointer and a character (`add sp,
                    // 6`); without this the call came out with no arguments at all.
                    // (policy.caller_args off turns it off: on SUWIN as many functions are the original's bytes with it as without
                    // -- 37 -- once each is compiled in its own source file; Setup on the 15 MINI.CAB modules rebuilt with it ran
                    // to the desktop)
                    std::int64_t took = d.policy.caller_args ? caller_cleanup(ins, j) : 0;
                    if (!took && d.policy.caller_args && i.mn == "call" && leave_follows(ins, j)) {
                        // (or none: Microsoft C 8 leaves a call's arguments to the frame's LEAVE when the function ends there --
                        // SETUPX's 4:16D8 pushes sub_4_1470 six words and never takes them off. The pushes are the arguments, when
                        // the callee is one that leaves them to its caller: its own return says so)
                        if (returns(code, i.first_imm() & 0xFFFF).pops == 0) took = pushed_before(ins, j, targets);
                    }
                    if (took) {
                        // (`push cs; call` is a far call to a function of the same file: the pushed CS is the return address's
                        // segment, not an argument)
                        const bool is_far = i.mn == "lcall" || (j > 0 && ins[j - 1].mn == "push" && ins[j - 1].op_str == "cs");
                        lines.push_back(std::format("O\t{}\t{}\t{}\t__cdecl16{}", hx(address(sg, i.address)), hx(va), took, is_far ? "far" : "near"));
                    }
                    continue;
                }
                std::int64_t pushed = 0;
                for (std::ptrdiff_t q = static_cast<std::ptrdiff_t>(j) - 1; q >= 0; --q) {
                    const Insn& p = ins[static_cast<std::size_t>(q)];
                    if (p.mn == "push") {
                        pushed += p.first_size() == 4 ? 4 : 2;
                        if (targets.count(p.address)) break;
                        continue;
                    }
                    if (p.in_group(kGroupCall) || p.in_group(kGroupJump) || eq_any(p.mn, {"ret", "retf", "pop", "leave", "enter"}) || targets.count(p.address)) break;
                    if (!p.regs_ok || writes_sp(p)) break;
                }
                if (!pushed) continue;
                const bool is_far = i.mn == "lcall" || (i.mn == "call" && contains(i.op_str, "dword"));
                const bool caller_pops = j + 1 < ins.size() && ins[j + 1].mn == "add" && ins[j + 1].op_str.starts_with("sp,");
                lines.push_back(std::format("O\t{}\t{}\t{}\t{}{}", hx(address(sg, i.address)), hx(va), pushed,
                                            caller_pops ? "__cdecl16" : "__stdcall16", is_far ? "far" : "near"));
            }
        }
    }
    return lines;
}

// {(seg, off): who removes a function's arguments}, as the original's own calls to it show: every call that pushed
// something before it and is followed by `add sp, n` says the caller does; every one that is not says the callee does.
// Only a function whose calls (two at least) all say the same is in it. The callers are the original program's own word on
// how each callee is called -- the callee's instructions can hide it (arguments taken off by hand, a shared epilogue).
std::map<SegOff, Remover> callers_say(const NeDescription& d) {
    std::map<SegOff, std::vector<Remover>> votes;
    for (const auto& [seg, image] : d.images) {
        const std::vector<Insn> ins = d.relocated(seg).decode(0, d.relocated(seg).size(), false);
        for (std::size_t k = 0; k + 1 < ins.size(); ++k) {
            const Insn& i = ins[k];
            if (!is_direct_call(i)) continue;
            const std::optional<SegOff> target = direct_call_target(i, seg);
            if (!target || k == 0 || ins[k - 1].mn != "push") continue;   // (nothing pushed just before: says nothing)
            const Insn& nxt = ins[k + 1];
            votes[*target].push_back(nxt.mn == "add" && nxt.op_str.starts_with("sp, ") ? Remover::Caller : Remover::Callee);
        }
    }
    std::map<SegOff, Remover> out;
    for (const auto& [place, v] : votes)
        if (v.size() >= 2 && std::all_of(v.begin(), v.end(), [&](Remover r) { return r == v[0]; })) out[place] = v[0];
    return out;
}

// Bytes of arguments a thunk's caller has removed, for a thunk that puts more arguments under its return address and jumps
// on: `pop bx / pop dx` (the return address), `push`es (the words it adds), `push dx / push bx`, then `jmp` / `ljmp` to the
// function that does the work (USER's 1:0110 adds a 0 and goes to BeginPaint's body at 1:75C8). What the target removes, less
// the words the thunk added. nullopt when it is not such a thunk.
std::optional<std::int64_t> forwards(const NeDescription& d, std::int64_t seg, std::int64_t off, Distance kind) {
    // (relocated: a far jump's segment as the placeholder)
    const auto image = d.images.find(seg);
    const Code16& code = (image != d.images.end() && !image->second.empty()) ? d.relocated(seg) : d.loaded(seg);
    // (read along the way it runs: a near jump inside the segment followed, not listed -- USER's 1:2440 `pop ax / pop dx / xor
    // cx, cx / jmp 244C` into 1:2446's `push cx / push dx / push ax`, then on into the body they share, which ends `retf
    // 0Eh`: its callers push six words, not seven, and the C passed DS for a seventh)
    std::vector<Insn> ins;
    std::int64_t at = off;
    while (ins.size() < 30 && 0 <= at && at < code.size()) {
        const Insn* ip = code.one(at);
        if (!ip) break;
        const Insn& i = *ip;
        if (i.mn == "jmp" && is_hex_lit(i.op_str) && !ins.empty()) {
            at = hex_of(i.op_str).value_or(0) & 0xFFFF;
            Insn into;   // (where it went: the body, if it is next)
            into.mn = "into";
            into.address = at;
            into.size = 0;
            ins.push_back(into);
            continue;
        }
        ins.push_back(i);
        at = i.end();
    }
    // (the body's address first put in a register, the rest shared: Win95 USER's entries `mov cx, 2FAh / jmp 6Bh`, and at 6B
    // `pop eax / push 0 / push eax / jmp cx`)
    std::map<std::string, std::int64_t> loaded;
    std::size_t b = 0;   // ins = ins[b:]
    const auto mov_const = [](const Insn& i) {   // mov (ax|bx|cx|dx), 0x<hex>
        return i.mn == "mov" && i.op_str.size() > 4 && eq_any(std::string_view(i.op_str).substr(0, 4), {"ax, ", "bx, ", "cx, ", "dx, "}) &&
               is_hex_lit(std::string_view(i.op_str).substr(4));
    };
    while (b < ins.size() && (ins[b].mn == "into" || mov_const(ins[b]))) {
        if (ins[b].mn == "mov") loaded[ins[b].op_str.substr(0, 2)] = hex_of(std::string_view(ins[b].op_str).substr(4)).value_or(0);
        ++b;
    }
    const std::size_t n = ins.size() - b;
    auto at_i = [&](std::size_t k) -> const Insn& { return ins[b + k]; };
    std::int64_t words = kind == Distance::Far ? 2 : 1;
    const auto in_regs = [](std::string_view s) { return eq_any(s, {"ax", "bx", "cx", "dx", "si", "di", "es"}); };
    const auto wide_reg = [&](std::string_view s) { return in_regs(s) || eq_any(s, {"eax", "ebx", "ecx", "edx", "esi", "edi"}); };
    // (a far return address taken off whole, in one 32-bit register: Win95 USER's 5:0011 (lstrcmpi) `pop eax / push word
    // es:[90h] / push eax / jmp 6E1` -- its callers push four words, not the five the body's `retf 0Ah` removes, and the C
    // passed a sixth and a seventh)
    if (kind == Distance::Far && n > 0 && at_i(0).mn == "pop" && eq_any(at_i(0).op_str, {"eax", "ebx", "ecx", "edx"})) {
        words = 1;
    } else {
        for (std::size_t q = 0; q < std::min<std::size_t>(n, static_cast<std::size_t>(words)); ++q)
            if (at_i(q).mn != "pop" || !in_regs(at_i(q).op_str)) return std::nullopt;
    }
    if (static_cast<std::int64_t>(n) < 2 * words + 2) return std::nullopt;
    std::vector<std::string> ret;
    for (std::size_t q = 0; q < std::min<std::size_t>(n, static_cast<std::size_t>(words)); ++q) ret.push_back(at_i(q).op_str);
    std::size_t k = static_cast<std::size_t>(words);
    std::int64_t added = 0, taken = 0;
    // (and the caller's arguments it takes off, before adding its own or among them -- USER's 1:014C pops one, pushes 0 and 1
    // or 5 (a `jcxz` choosing), puts the return address back, sets slots in the stack and goes to 1:3E1E: its callers' stack
    // came out four bytes short in the gate; Win95 USER.88 (EndDialog) `pop ecx / pop ax / cwde / push eax / push ecx`, its
    // int widened to a long, and USER's 8:43D6 `pop eax / pop bx / xor ecx, ecx / pop cx / push ecx`)
    const auto quiet = [&](const Insn& i) {   // (moves nothing on the stack)
        std::string_view widened;
        if (i.mn == "cbw") widened = "ax";
        else if (i.mn == "cwde") widened = "eax";
        else if (i.mn == "cwd") widened = "dx";
        else if (i.mn == "cdq") widened = "edx";
        if (!widened.empty()) {   // (widening in a register)
            for (const std::string& r : ret)
                if (contains(widened, r) || contains(r, widened)) return false;
            return true;
        }
        if (eq_any(i.mn, {"xor", "mov", "sub", "cmp", "test", "and", "or", "movzx", "movsx"}) && !contains(before_comma(i.op_str), "sp")) return true;
        if (eq_any(i.mn, {"jcxz", "je", "jne"}) && is_hex_lit(i.op_str)) {
            const std::int64_t t = hex_of(i.op_str).value_or(0);
            return i.address < t && t <= i.address + 0x20;
        }
        return false;
    };
    while (k < n && (eq_any(at_i(k).mn, {"push", "pop", "into"}) || quiet(at_i(k))) && !(at_i(k).mn == "push" && at_i(k).op_str == ret.back())) {
        const Insn& q = at_i(k);
        if (q.mn == "into") {   // (a jump followed: it goes on there)
            ++k;
            continue;
        }
        if (q.mn == "pop") {
            if (!wide_reg(q.op_str) || std::find(ret.begin(), ret.end(), q.op_str) != ret.end()) return std::nullopt;
            taken += (q.op_str.starts_with("e") && q.op_str.size() == 3) ? 2 : 1;
        } else if (q.mn == "push") {   // (a 32-bit push is two words: GDI.612's `push 0` is 66 68 00000000)
            const bool pre66 = (q.bytes.size() > 0 && q.bytes[0] == 0x66) || (q.bytes.size() > 1 && q.bytes[1] == 0x66);
            added += pre66 ? 2 : 1;
        } else {
            const std::string_view first = before_comma(q.op_str);
            for (const std::string& r : ret)
                if (contains(first, r)) return std::nullopt;   // (its return address changed)
        }
        ++k;
    }
    std::vector<std::string> back;
    for (std::size_t q = k; q < std::min(n, k + static_cast<std::size_t>(words)); ++q)
        if (at_i(q).mn == "push") back.push_back(at_i(q).op_str);
    const std::vector<std::string> rev(ret.rbegin(), ret.rend());
    if (back != rev || !added || k + static_cast<std::size_t>(words) >= n) return std::nullopt;
    k += static_cast<std::size_t>(words);
    while (k + 1 < n && quiet(at_i(k))) ++k;   // (slots set beneath, then the jump)
    if (k >= n) return std::nullopt;
    const Insn& j = at_i(k);
    std::int64_t tseg, toff;
    // a far target "0x<seg>, 0x<off>" or "0x<seg>:0x<off>"
    const auto split_far = [](std::string_view os) -> std::optional<std::pair<std::int64_t, std::int64_t>> {
        std::size_t cut = os.find(", "), sepw = 2;
        if (cut == std::string_view::npos) {
            cut = os.find(':');
            sepw = 1;
        }
        if (cut == std::string_view::npos) return std::nullopt;
        const std::string_view a = os.substr(0, cut), c = os.substr(cut + sepw);
        if (!is_hex_lit(a) || !is_hex_lit(c)) return std::nullopt;
        return std::make_pair(hex_of(a).value_or(0), hex_of(c).value_or(0));
    };
    const auto far_target = j.mn == "ljmp" ? split_far(j.op_str) : std::nullopt;
    if (far_target) {
        if (far_target->first >= kExternSelector) return std::nullopt;   // (another module's: not known here)
        const SegOff w = where((far_target->first << 16) | far_target->second);
        tseg = w.seg;
        toff = w.off;
    } else if (j.mn == "into") {   // (a near jump, followed: the body there)
        tseg = seg;
        toff = j.address;
    } else if (j.mn == "jmp" && loaded.count(j.op_str)) {   // (to the address put in the register)
        tseg = seg;
        toff = loaded[j.op_str];
    } else if (eq_any(j.mn, {"mov", "enter", "push", "inc", "nop"})) {
        tseg = seg;   // (no jump: the body goes on right here)
        toff = j.address;
    } else {
        return std::nullopt;
    }
    const std::int64_t pop = returns(d.loaded(tseg), toff, 600, kind).pops;
    if (pop < 0) return std::nullopt;   // (none met)
    return std::max<std::int64_t>(0, pop - 2 * added + 2 * taken);
}

// Does the function at `off` take arguments in AX, DX or BX (Microsoft C's _fastcall): its straight-line start (to the
// first jump, call or return) reads one of them (or AL, DL, BL) before it writes it. SUWIN's 7:0000 begins `push bp / mov
// bp, sp / push ax` -- its char argument, kept in its frame -- and its callers load AX just before the call.
bool register_arguments(const Code16& code, std::int64_t off) {
    const auto family = [](const std::string& reg) -> std::string_view {
        if (eq_any(reg, {"ax", "al", "ah", "eax"})) return "ax";
        if (eq_any(reg, {"dx", "dl", "dh", "edx"})) return "dx";
        if (eq_any(reg, {"bx", "bl", "bh", "ebx"})) return "bx";
        return {};
    };
    // (a `push ax` that the function pops back before it returns is a save, not a read of an argument: WIN87EM's WEP)
    std::set<std::string_view> popped;
    for (const Insn& i : code.decode(off, off + 160, true)) {
        if (i.in_group(kGroupRet) || eq_any(i.mn, {"iret", "retf"})) break;
        if (i.mn == "pop") {
            const std::vector<std::string> ops = split_strip_commas(i.op_str);
            if (ops.size() == 1) popped.insert(family(ops[0]));
        }
    }
    std::set<std::string_view> written;
    for (const Insn& i : code.decode(off, off + 64, true)) {
        if (i.in_group(kGroupJump) || i.in_group(kGroupCall) || i.in_group(kGroupRet) || eq_any(i.mn, {"int", "iret", "retf"})) return false;
        if (!i.regs_ok) return false;
        if (i.mn == "push") {
            const std::vector<std::string> ops = split_strip_commas(i.op_str);
            if (ops.size() == 1 && !family(ops[0]).empty() && popped.count(family(ops[0]))) continue;
        }
        // (USER's GetWindowLong family enters at `mov cl, 5 / test ax, imm16 / mov cl, 1 ...`: the `test ax` (A9) is the skip idiom, its
        // immediate the next entry's `mov cl, imm8` -- it reads nothing, and the arguments are on the stack, a pascal frame's)
        if (i.bytes.size() == 3 && i.bytes[0] == 0xA9 && (i.bytes[1] & 0xF8) == 0xB0) continue;
        const std::vector<std::string> ops = split_strip_commas(i.op_str);
        const bool same = eq_any(i.mn, {"xor", "sub", "sbb"}) && ops.size() == 2 && ops[0] == ops[1];
        if (!same) {
            for (std::uint16_t r : i.regs_read) {
                const std::string_view f = family(register_name16(r));
                if (!f.empty() && !written.count(f)) return true;
            }
        }
        for (std::uint16_t r : i.regs_write) {
            const std::string_view f = family(register_name16(r));
            if (!f.empty()) written.insert(f);
        }
    }
    return false;
}

// Is the function at `off` a helper whose effect is on its caller's registers: a short straight run to a plain `ret` -- no
// frame, no branch, no call -- that leaves SI, DI or DS changed, registers a C function keeps for its caller? KEYBOARD's
// sub_9_0A6B, `push ax / push bx / mov ax, ds / add ax, 8 / mov ds, ax / xor si, si / pop bx / pop ax / ret`, moves OEMTOANSI's
// source pointer DS:SI on to the next 64K of a huge buffer. As a function of its own its C can only `return`, and the caller's
// C goes on through the old pointer (its copy past 64K went back over the start). Such a helper is decompiled as part of each
// caller -- the description's `inline` -- so the caller's C moves its own pointer.
bool register_helper(const Code16& code, std::int64_t off, std::optional<std::int64_t> end) {
    const auto kept = [](const std::string& reg) -> std::string_view {
        if (reg == "si" || reg == "esi") return "si";
        if (reg == "di" || reg == "edi") return "di";
        if (reg == "ds") return "ds";
        return {};
    };
    const std::int64_t stop = !end ? off + 48 : std::min(*end, off + 48);
    std::vector<std::string> pushed;
    std::set<std::string_view> changed;
    int k = 0;
    for (const Insn& i : code.decode(off, stop, true)) {
        if (k >= 16) return false;
        ++k;
        if (i.mn == "ret" && i.op_str.empty()) return pushed.empty() && !changed.empty();
        if (i.in_group(kGroupJump) || i.in_group(kGroupCall) || i.in_group(kGroupRet) || i.in_group(kGroupInt) ||
            eq_any(i.mn, {"retf", "iret", "enter", "leave", "int", "into"}))
            return false;
        if (!i.regs_ok) return false;
        std::set<std::string> names;
        for (std::uint16_t r : i.regs_read) names.insert(register_name16(r));
        for (std::uint16_t r : i.regs_write) names.insert(register_name16(r));
        std::set<std::string_view> frame = {"bp", "ebp", "sp", "esp"};
        if (i.mn == "push" || i.mn == "pop") {
            frame.erase("sp");
            frame.erase("esp");
        }
        for (const std::string& nm : names)
            if (frame.count(nm)) return false;   // (a frame, or its caller's: not this)
        if (i.mn == "push") {
            pushed.push_back(i.op_str);
            continue;
        }
        if (i.mn == "pop") {
            if (pushed.empty()) return false;
            const std::string top = pushed.back();
            pushed.pop_back();
            if (top != i.op_str) return false;
            const std::string_view kp = kept(i.op_str);
            if (!kp.empty()) changed.erase(kp);
            continue;
        }
        for (std::uint16_t r : i.regs_write) {
            const std::string_view kp = kept(register_name16(r));
            if (!kp.empty()) changed.insert(kp);
        }
    }
    return false;
}

// The `places` a call to which is followed, before anything sets it, by a read of AX (`or ax, ax`, `mov [x], ax`,
// `push ax`): what it leaves there is a result its caller uses.
std::set<SegOff> callers_read_ax(const NeDescription& d, const std::set<SegOff>& places) {
    std::set<SegOff> out;
    for (const auto& [seg, image] : d.images) {
        if (!d.code.empty() && !d.code.contains(seg)) continue;
        const std::vector<Insn> ins = d.relocated(seg).decode(0, d.relocated(seg).size(), true);
        for (std::size_t k = 0; k + 1 < ins.size(); ++k) {
            const Insn& i = ins[k];
            if (!is_direct_call(i)) continue;
            const std::optional<SegOff> target = direct_call_target(i, seg);
            if (!target) continue;
            if (!places.contains(*target) || out.contains(*target)) continue;
            for (std::size_t q = k + 1; q < std::min(ins.size(), k + 8); ++q) {
                const Insn& s = ins[q];
                if (s.in_group(kGroupJump) || s.in_group(kGroupCall) || s.in_group(kGroupRet) || eq_any(s.mn, {"int", "iret", "leave"})) break;
                const std::vector<std::string> ops = split_strip_commas(s.op_str);
                if ((s.mn == "xor" || s.mn == "sub") && ops.size() == 2 && ops[0] == ops[1]) {
                    if (eq_any(ops[0], {"ax", "al", "ah", "eax"})) break;   // (cleared: written, not read)
                    continue;
                }
                if (!s.regs_ok) break;
                if (hits_ax(s.regs_read)) {
                    out.insert(*target);
                    break;
                }
                if (hits_ax(s.regs_write)) break;
            }
        }
    }
    return out;
}

// The functions a call to which is followed, before anything else could set it, by a read of DX -- the callee returns four
// bytes, DX:AX, whatever its own instructions let one tell: a compiler reads DX after a call only for a long or a far pointer
// it returned (SETUPX's 4:6C9C: `lcall SUREGCREATEKEY / or dx, ax`, the key's LONG result, read as an int and a DX nothing in
// the C had set). `edges`, given, has (caller, callee) added for each call whose AX and DX go back to the caller's own caller
// as they are (nothing writes either before the return, a jump to it followed): if the caller returns four bytes, so does
// the callee (SUREGQUERYVALUEEX hands back sub_31_002A's).
std::set<SegOff> callers_read_dx(const NeDescription& d, std::vector<std::pair<SegOff, SegOff>>* edges) {
    std::set<SegOff> out;
    std::map<std::int64_t, std::vector<std::int64_t>> starts;   // segment -> sorted function offsets
    for (const auto& [va, name] : d.names) {
        const SegOff w = where(va);
        starts[w.seg].push_back(w.off);
    }
    for (auto& [seg, offs] : starts) std::sort(offs.begin(), offs.end());
    for (const auto& [seg, image] : d.images) {
        if (!d.code.empty() && !d.code.contains(seg)) continue;   // (a data segment: no calls in it)
        const std::vector<Insn> ins = d.relocated(seg).decode(0, d.relocated(seg).size(), true);
        std::unordered_map<std::int64_t, std::size_t> at;
        for (std::size_t q = 0; q < ins.size(); ++q) at[ins[q].address] = q;
        for (std::size_t k = 0; k + 1 < ins.size(); ++k) {
            const Insn& i = ins[k];
            if (edges && (i.mn == "call" || i.mn == "lcall") && starts.count(seg)) {
                const auto& st = starts[seg];
                const auto ub = std::upper_bound(st.begin(), st.end(), i.address);
                const std::optional<std::int64_t> own = ub != st.begin() ? std::optional<std::int64_t>(*(ub - 1)) : std::nullopt;
                std::size_t n = k + 1;
                int hops = 0;
                bool through = false;
                while (own && n < ins.size() && hops < 12) {
                    const Insn& q = ins[n];
                    if (q.mn == "ret" || q.mn == "retf") {
                        through = true;
                        break;
                    }
                    if (q.mn == "jmp" && q.op_str.starts_with("0x") && at.count(number_of(q.op_str).value_or(-1))) {
                        n = at[number_of(q.op_str).value_or(-1)];
                        ++hops;
                        continue;
                    }
                    if (q.in_group(kGroupJump) || q.in_group(kGroupCall)) break;
                    if (!q.regs_ok) break;
                    if (hits_ax(q.regs_write) || hits_dx(q.regs_write)) break;
                    ++n;
                    ++hops;
                }
                if (through) {
                    std::optional<SegOff> tgt;
                    if (i.mn == "call" && i.op_str.starts_with("0x") && !contains(i.op_str, ","))
                        tgt = SegOff{static_cast<std::uint32_t>(seg), static_cast<std::uint32_t>(number_of(i.op_str).value_or(0) & 0xFFFF)};
                    else if (i.mn == "lcall" && contains(i.op_str, ",") && i.op_str.starts_with("0x")) tgt = direct_call_target(i, seg);
                    if (tgt) edges->push_back({SegOff{static_cast<std::uint32_t>(seg), static_cast<std::uint32_t>(*own)}, *tgt});
                }
            }
            if (!is_direct_call(i)) continue;
            const std::optional<SegOff> target = direct_call_target(i, seg);
            if (!target) continue;
            // (not when the caller set DX itself just before: what it reads may be its own, kept -- KEYBOARD's 9:0367 saves AX
            // and BX and leaves DX alone -- nor for a _fastcall callee, whose DX may be its argument)
            bool set_before = false;
            for (std::size_t q = k >= 6 ? k - 6 : 0; q < k; ++q)
                if (ins[q].regs_ok && hits_dx(ins[q].regs_write)) set_before = true;
            if (set_before) continue;
            if (d.segment_bytes.contains(target->seg) && register_arguments(d.loaded(target->seg), target->off)) continue;
            for (std::size_t q = k + 1; q < std::min(ins.size(), k + 8); ++q) {
                const Insn& s = ins[q];
                if (s.in_group(kGroupJump) || s.in_group(kGroupCall) || s.in_group(kGroupRet) || eq_any(s.mn, {"int", "iret", "leave"})) break;
                if (!s.regs_ok) break;
                if ((s.mn == "xor" || s.mn == "sub") && s.op_str == "dx, dx") break;   // (cleared: written, not read)
                if (hits_dx(s.regs_read)) {
                    out.insert(*target);
                    break;
                }
                if (hits_dx(s.regs_write)) break;
            }
        }
    }
    return out;
}

// ---- DxReturns ----------------------------------------------------------------------------------------------------------
struct DxReturns::Impl {
    struct Func {
        std::vector<Insn> ins;
        std::unordered_map<std::int64_t, std::size_t> at;
        std::unordered_map<std::int64_t, std::vector<std::int64_t>> into;   // lookup only
        std::int64_t end = 0;
        bool saves_dx = false, saves_ax = false;
    };
    const NeDescription* d = nullptr;
    std::map<std::int64_t, std::vector<std::int64_t>> starts;   // segment -> sorted function offsets
    std::map<SegOff, std::shared_ptr<Func>> code;                // null: no image
};

DxReturns::DxReturns(const NeDescription& d) : impl_(std::make_unique<Impl>()) {
    impl_->d = &d;
    for (const auto& [va, name] : d.names) {
        const SegOff w = where(va);
        impl_->starts[w.seg].push_back(w.off);
    }
    for (auto& [seg, offs] : impl_->starts) std::sort(offs.begin(), offs.end());
}
DxReturns::~DxReturns() = default;

namespace {

// Does it push AX as it was given -- before anything sets AX, on the way in from its start? (Not `push ebx / pop ax / pop
// dx`, KRNL386's 2:4C91 handing back a LONG made in EBX.)
bool saves_ax_of(const std::vector<Insn>& ins) {
    for (const Insn& i : ins) {
        if (i.mn == "push" && i.op_str == "ax") return true;
        if (i.in_group(kGroupJump) || i.in_group(kGroupCall) || i.in_group(kGroupRet)) return false;
        if (!i.regs_ok || hits_ax(i.regs_write)) return false;
    }
    return false;
}

// Is AX, on some way to a return, last set by a `pop ax` -- the caller's own AX given back? Then nothing comes back in DX:AX
// either, whatever a call before it did to DX: KEYBOARD's 9:0367 saves AX and BX, calls the keyboard VxD's entry through [20h]
// (a pointer, taken as handing back its DX) and returns them as they were; its callers read their own DX.
bool gives_back_ax(const DxReturns::Impl::Func& f, const std::vector<const Insn*>& ends) {
    std::vector<std::int64_t> work;
    std::set<std::int64_t> seen;
    const auto into_of = [&](std::int64_t a) -> std::vector<std::int64_t> {
        const auto it = f.into.find(a);
        return it == f.into.end() ? std::vector<std::int64_t>() : it->second;
    };
    for (const Insn* r : ends)
        for (std::int64_t x : into_of(r->address)) work.push_back(x);
    while (!work.empty()) {
        const std::int64_t a = work.back();
        work.pop_back();
        if (seen.count(a) || !f.at.count(a)) continue;
        seen.insert(a);
        const Insn& p = f.ins[f.at.at(a)];
        if (p.mn == "pop" && p.op_str == "ax") return true;
        if (p.in_group(kGroupCall) || p.mn == "int" || p.mn == "into") continue;   // (AX from a call: not the caller's)
        if (!p.regs_ok || hits_ax(p.regs_write)) continue;
        for (std::int64_t x : into_of(a)) work.push_back(x);
    }
    return false;
}

}  // namespace

bool DxReturns::every_way(const SegOff& place, const std::set<SegOff>& four) {
    const NeDescription& d = *impl_->d;
    std::shared_ptr<Impl::Func> f;
    const auto cached = impl_->code.find(place);
    if (cached != impl_->code.end()) {
        f = cached->second;
    } else {
        const std::int64_t sg = place.seg, off = place.off;
        if (d.images.contains(sg)) {
            const Code16& img = d.relocated(sg);
            std::vector<std::int64_t> later;
            if (const auto st = impl_->starts.find(sg); st != impl_->starts.end())
                for (std::int64_t o : st->second)
                    if (o > off) later.push_back(o);
            std::int64_t end = !later.empty() ? later[0] : img.size();
            std::vector<Insn> ins = img.decode(off, end, true);
            // (one that runs on into the next function's start, with no return or jump of its own at the end, is that code
            // too: SETUPX's 3:8246 is cut by a start found inside one of its instructions, 3:8259)
            for (int n = 0; n < 4; ++n) {
                if (ins.empty() || ends_flow(ins.back().mn) || end - ins.back().end() >= 16 || end >= img.size()) break;
                std::erase_if(later, [&](std::int64_t o) { return o <= end; });
                end = !later.empty() ? later[0] : img.size();
                ins = img.decode(off, end, true);
            }
            f = std::make_shared<Impl::Func>();
            for (std::size_t k = 0; k < ins.size(); ++k) f->at[ins[k].address] = k;
            for (std::size_t k = 0; k < ins.size(); ++k) {
                const Insn& i = ins[k];
                if (k + 1 < ins.size() && !ends_flow(i.mn)) f->into[ins[k + 1].address].push_back(i.address);
                if (i.in_group(kGroupJump) && i.imm_first() && i.mn != "ljmp") f->into[i.first_imm() & 0xFFFF].push_back(i.address);
            }
            f->end = end;
            f->saves_dx = std::any_of(ins.begin(), ins.end(), [](const Insn& i) { return i.mn == "push" && i.op_str == "dx"; });
            f->saves_ax = saves_ax_of(ins);
            f->ins = std::move(ins);
        }
        impl_->code[place] = f;
    }
    if (!f) return false;
    const std::vector<Insn>& ins = f->ins;
    const std::int64_t sg = place.seg, off = place.off;
    const std::int64_t end = f->end;
    const auto into_of = [&](std::int64_t a) -> std::vector<std::int64_t> {
        const auto it = f->into.find(a);
        return it == f->into.end() ? std::vector<std::int64_t>() : it->second;
    };
    // (a direct call's or jump's target in the module, Out for an import, None for one through a register or memory)
    enum class Kind { None, Out, Place };
    struct Target {
        Kind kind = Kind::None;
        SegOff place{0, 0};
    };
    const auto target_of = [&](const Insn& i) -> Target {
        Target t;
        if (!i.imm_first()) return t;
        if (i.mn == "lcall" || i.mn == "ljmp") {
            const std::vector<std::string_view> parts = text::split(i.op_str, contains(i.op_str, ":") ? std::string_view(":") : std::string_view(", "));
            const std::int64_t s = number_of(parts.at(0)).value_or(0), o = number_of(parts.at(1)).value_or(0);
            if (s >= kExternSelector) {
                t.kind = Kind::Out;
            } else {
                t.kind = Kind::Place;
                t.place = where((s << 16) | o);
            }
            return t;
        }
        t.kind = Kind::Place;
        t.place = SegOff{static_cast<std::uint32_t>(sg), static_cast<std::uint32_t>(i.first_imm() & 0xFFFF)};
        return t;
    };
    // (a call, or a jump on: its callee's DX -- an import's, or one through a pointer: SETUPX's 28:111C `call far es:[bx+4]`)
    const auto handed_on = [&](const Insn& i) {
        const Target t = target_of(i);
        return t.kind == Kind::Out || t.kind == Kind::None || four.contains(t.place);
    };
    // (the instructions reached from its start -- what lies past a jump on, a jump to an import (GETTICKCOUNT: `jmp far
    // KERNEL`), is not its; through a jump table everything may be)
    std::set<std::int64_t> reach;
    std::vector<std::int64_t> todo{off};
    while (!todo.empty()) {
        const std::int64_t a = todo.back();
        todo.pop_back();
        if (reach.count(a) || !f->at.count(a)) continue;
        reach.insert(a);
        const Insn& i = ins[f->at.at(a)];
        if (i.in_group(kGroupJump) && i.imm_first() && i.mn != "ljmp") {
            todo.push_back(i.first_imm() & 0xFFFF);
        } else if (i.in_group(kGroupJump) && i.mn == "jmp") {
            reach.clear();   // (a jump through a table or register)
            for (const auto& [a2, k] : f->at) reach.insert(a2);
            break;
        }
        if (!ends_flow(i.mn)) todo.push_back(i.end());
    }
    std::vector<const Insn*> ends;
    for (std::size_t k = 0; k < ins.size(); ++k) {
        const Insn& i = ins[k];
        if ((i.mn == "ret" || i.mn == "retf") && reach.count(i.address) && !jump_by_return(ins, k))   // (a jump through a pointer: its callee's DX)
            ends.push_back(&i);
    }
    bool jumped_on = false;
    for (const Insn& i : ins) {   // (a jump on to another function: its result)
        if (!reach.count(i.address)) continue;
        if (i.mn == "jmp" || i.mn == "ljmp") {
            const Target t = target_of(i);
            if (t.kind == Kind::Out || (t.kind == Kind::Place && t.place != place && !(t.place.seg == sg && off <= t.place.off && t.place.off < end))) {
                if (!handed_on(i)) return false;
                jumped_on = true;
            }
        }
    }
    if (ends.empty() && !jumped_on) return false;
    if (f->saves_ax && gives_back_ax(*f, ends)) return false;   // (no result at all: AX as it came)
    // work items: an address, or "not seen" (nullopt)
    std::vector<std::optional<std::int64_t>> work;
    std::set<std::int64_t> seen;
    for (const Insn* r : ends) {
        const std::vector<std::int64_t> in = into_of(r->address);
        if (in.empty()) work.push_back(std::nullopt);
        else for (std::int64_t x : in) work.push_back(x);
    }
    while (!work.empty()) {
        const std::optional<std::int64_t> w = work.back();
        work.pop_back();
        if (!w || !f->at.count(*w)) return false;   // (reached from what is not seen: DX unknown)
        const std::int64_t a = *w;
        if (seen.count(a)) continue;
        seen.insert(a);
        const Insn& p = ins[f->at.at(a)];
        if (p.in_group(kGroupCall)) {
            if (handed_on(p)) continue;
            return false;   // (a two-byte callee: DX as it was)
        }
        if (p.mn == "int" || p.mn == "into") continue;
        if (!p.regs_ok) return false;
        if (hits_dx(p.regs_write)) {
            if (p.mn == "pop" && f->saves_dx) return false;   // (DX given back as it came)
            continue;
        }
        if (a == off) return false;   // (DX as the caller had it)
        const std::vector<std::int64_t> in = into_of(a);
        if (in.empty()) work.push_back(std::nullopt);
        else for (std::int64_t x : in) work.push_back(x);
    }
    return true;
}

// {address: bytes it returns} for every function described: its own (result_bytes), or for one that passes on a callee's
// result, the callee's -- four bytes passed on are four bytes (a near callee in its segment; any other call counts as two).
std::map<Va, std::int64_t> results(const NeDescription& d) {
    std::vector<std::int64_t> segorder;
    std::map<std::int64_t, std::vector<std::int64_t>> by_seg;
    for (const auto& [va, name] : d.names) {
        const SegOff w = where(va);
        if (!by_seg.count(w.seg)) segorder.push_back(w.seg);
        by_seg[w.seg].push_back(w.off);
    }
    std::vector<SegOff> order;
    std::map<SegOff, std::int64_t> own, voided;
    std::map<SegOff, std::vector<PassOn>> passes;
    for (const std::int64_t sg : segorder) {
        const Code16& code = d.loaded(sg);
        std::vector<std::int64_t>& offs = by_seg[sg];
        std::sort(offs.begin(), offs.end());
        for (std::size_t k = 0; k < offs.size(); ++k) {
            const std::int64_t off = offs[k];
            std::vector<PassOn> p;
            std::vector<std::int64_t> v;
            const SegOff key{static_cast<std::uint32_t>(sg), static_cast<std::uint32_t>(off)};
            if (!own.count(key)) order.push_back(key);
            own[key] = result_bytes(code, off, k + 1 < offs.size() ? offs[k + 1] : code.size(), &p, &v);
            passes[key] = std::move(p);
            if (!v.empty()) voided[key] = v[0];
        }
    }
    if (!voided.empty() && !d.images.empty()) {
        // (one with a way that sets no AX, whose result a caller reads all the same: a result)
        std::set<SegOff> vs;
        for (const auto& [place, bytes] : voided) vs.insert(place);
        for (const SegOff& place : callers_read_ax(d, vs)) own[place] = voided[place];
    }
    std::map<SegOff, std::int64_t> size = own;
    for (int round = 0; round < 8; ++round) {   // (wrappers of wrappers)
        bool changed = false;
        for (const SegOff& place : order) {
            for (const PassOn& t : passes[place]) {
                std::int64_t got;
                if (t.kind == PassOn::Kind::Into) {   // the function it runs on in
                    std::optional<std::int64_t> inside;
                    for (std::int64_t o : by_seg[place.seg])
                        if (o <= t.offset && (!inside || o > *inside)) inside = o;
                    got = 0;
                    if (inside) {
                        const auto it = size.find({place.seg, static_cast<std::uint32_t>(*inside)});
                        if (it != size.end()) got = it->second;
                    }
                    if (!got) continue;
                } else {
                    if (t.kind == PassOn::Kind::Near) {
                        const auto it = size.find({place.seg, static_cast<std::uint32_t>(t.offset)});
                        got = it == size.end() ? 0 : it->second;
                    } else {
                        got = 2;
                    }
                    got = got ? got : 2;
                }
                if (got > size[place]) {
                    size[place] = got;
                    changed = true;
                }
            }
        }
        if (!changed) break;
    }
    // (off unless the policy says so until Windows 95 Setup has been booted with it. With it on, USER's C once stopped
    // Setup -- "not enough memory to run Setup". That is now thought not a wrong result but what the C it lets USER have did
    // to fixed segment 1: grown past the original's, the low memory Setup needs for its program was gone. the build now
    // keeps a fixed code segment at the original's size; the milestone boot, with the rule on and off, is to confirm it.
    // Narrowed meanwhile to a callee that hands back a DX of its own on every way -- see DxReturns)
    if (d.policy.caller_results && !d.images.empty()) {
        std::vector<std::pair<SegOff, SegOff>> edges;
        std::set<SegOff> read, handed;
        for (const SegOff& p : callers_read_dx(d, &edges)) {
            const auto it = size.find(p);
            if (it != size.end() && it->second < 4) read.insert(p);   // (what the callers read after it)
        }
        for (const auto& e : edges) {
            const auto it = size.find(e.second);
            if (it != size.end() && it->second < 4) handed.insert(e.second);
        }
        DxReturns own_dx(d);
        // (all taken as four-byte, then each that does not hand back a DX of its own under that dropped, until none is left to
        // drop: a wrapper's DX is its callee's, and the callee is four-byte only through the wrapper's callers -- USER's
        // 25:107F `call 1:1292 / leave / ret 8`, 1292 handing back GDI's GetTextExtent)
        std::set<SegOff> base;
        for (const auto& [place, bytes] : size)
            if (bytes >= 4) base.insert(place);
        std::set<SegOff> made = read;
        made.insert(handed.begin(), handed.end());
        for (std::size_t round = 0; round < made.size() + 1; ++round) {
            std::set<SegOff> four = base;
            four.insert(made.begin(), made.end());
            std::set<SegOff> keep;
            for (const SegOff& p : read)
                if (own_dx.every_way(p, four)) keep.insert(p);
            for (const auto& [caller, callee] : edges)
                if (four.count(caller) && handed.count(callee) && own_dx.every_way(callee, four)) keep.insert(callee);
            std::set<SegOff> kept;
            for (const SegOff& x : keep)
                if (made.count(x)) kept.insert(x);
            if (kept == made) break;
            made = std::move(kept);
        }
        for (const SegOff& p : made) size[p] = 4;
    }
    std::map<Va, std::int64_t> out;
    for (const auto& [va, name] : d.names) {
        const auto it = size.find(where(va));
        out[va] = it == size.end() ? 0 : it->second;
    }
    return out;
}

// The description line for a function at `va`.
std::string model_line(const NeDescription& d, Va va, const std::string& name, Distance dflt) {
    const SegOff w = where(va);
    const std::int64_t seg = w.seg, off = w.off;
    const Code16& code = d.loaded(seg);
    if (d.far_called.contains(w)) dflt = Distance::Far;   // (found later, called far: see describe16)
    // How the original calls it is the evidence (an export, a far fix-up: far); the nearest return of that kind then says what it
    // removes. With no such evidence, its own nearest return says both.
    std::optional<Distance> kind;
    std::int64_t pop;
    if (dflt == Distance::Far || windows_far_prologue(code.bytes(), off)) {
        pop = returns(code, off, 600, Distance::Far).pops;
        kind = Distance::Far;
    } else {
        const Return r = returns(code, off, 600);
        kind = r.kind;
        pop = r.pops;
    }
    // A function that removes its own arguments says so in its return (`retf 6`); one that ends in a plain return leaves them
    // to the caller -- unless it took them off by hand first (USER's IsChild: `pop ax; pop dx` its return address, `pop bx;
    // pop cx` its two arguments, `push dx; push ax` the return address back, then a plain `retf`).
    Distance kd = kind.value_or(dflt);
    if (const auto fwd = forwards(d, seg, off, kd)) {
        pop = *fwd;
        if (kd == Distance::Far) kind = Distance::Far;
    }
    // (and what it took off by hand at its start comes on top of what its return removes: USER's call-hook dispatcher 1:8005
    // pops one word by hand, then `retf 8` -- five words in all)
    kd = kind.value_or(dflt);
    const std::int64_t hand = popped_by_hand(code, off, kd).value_or(0);
    if (hand && pop >= 0) pop += hand;
    std::string conv = pop == 0 ? "__cdecl16" : "__stdcall16";
    // (the policy's fastcall off turns it off. A far one only without the Windows prolog: under /Gw, which loads DS through AX,
    // C 8 says "illegal far _fastcall function" -- without it, as in SUWIN, a far _fastcall takes AX, DX, BX as a near one
    // does: measured, `retf` / `retf n`)
    if (d.policy.fastcall && (kd == Distance::Near || !windows_far_prologue(code.bytes(), off)) && register_arguments(code, off)) conv = "__fastcall16";
    // what it returns, as its instructions show (where it runs to: the next function's start)
    std::int64_t result;
    if (const auto got = d.results.find(va); got != d.results.end()) {
        result = got->second;
    } else {   // (one found later: on its own)
        std::optional<std::int64_t> later;
        for (const auto& [v, nm] : d.names) {
            const SegOff wv = where(v);
            if (wv.seg == seg && wv.off > off && (!later || wv.off < *later)) later = wv.off;
        }
        std::vector<PassOn> ps;
        result = result_bytes(code, off, later ? *later : code.size(), &ps, nullptr);
        for (const PassOn& t : ps) {   // a callee's result passed on: its size
            if (t.kind == PassOn::Kind::Into) continue;
            std::int64_t got_size = 2;
            if (t.kind == PassOn::Kind::Near && d.segment_bytes.contains(seg)) {
                const auto r = d.results.find(address(seg, t.offset));
                got_size = r == d.results.end() ? 0 : r->second;
            }
            result = std::max(result, got_size ? got_size : 2);
        }
    }
    return std::format("F\t{}\t{}\t{}\t{}{}{}", hx(va), name, pop, conv, distance_name(kind.value_or(dflt)),
                       result ? "\t" + std::to_string(result) : std::string());
}

}  // namespace cb::decompile::describe
