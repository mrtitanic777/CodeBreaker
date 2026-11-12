// cb/port/asm16_emit.cpp -- the module for ml64: data, an entry of today's C for each routine it calls, and the routines,
// instruction by instruction. (See asm16.hpp.)
// cb-unity: alone

#include <algorithm>
#include <format>

#include "cb/core/intmath.hpp"
#include "cb/core/regex.hpp"
#include "cb/core/text.hpp"
#include "cb/port/asm16.hpp"

namespace cb::port::asm16 {

namespace {

using OK = Operand::Kind;

std::string signed_part(std::int64_t v) { return v ? std::format("{:+d}", v) : std::string(); }   // `%+d` when not 0

std::string size_word(int size) {
    switch (size) {
        case 1: return "byte ptr ";
        case 2: return "word ptr ";
        case 4: return "dword ptr ";
        case 8: return "qword ptr ";
        case 10: return "tbyte ptr ";
        default: throw Asm16Error("a memory operand of " + std::to_string(size) + " bytes");
    }
}

std::string r64(std::string_view r16) {
    const std::string_view r = reg64(r16);
    if (r.empty()) throw Asm16Error("`" + std::string(r16) + "` is not a 16-bit register");
    return std::string(r);
}

std::string target_of_line(const Line& l) {
    const auto words = l.args.empty() ? std::vector<std::string_view>() : text::split_ws(l.args[0]);
    if (words.empty()) throw Asm16Error("line " + std::to_string(l.no) + ": no operand");
    return std::string(words.back());
}

std::string comment_text(const Line& l) { return std::string(text::trim(strip_comment(l.text))); }

int bit_length_minus_one(std::int64_t so) {   // log2 of a power of two
    int n = 0;
    while ((std::int64_t{1} << (n + 1)) <= so) ++n;
    return n;
}

bool power_of_two(std::int64_t so) { return so > 0 && (so & (so - 1)) == 0; }

const std::array<std::string_view, 4> kArgRegs = {"rcx", "rdx", "r8", "r9"};

}  // namespace

// ---- the call of a C function from the old code ---------------------------------------------------------------------

// Its arguments, on the stack as the old caller pushed them (8 bytes each word), given today's way; what it returns where
// the old code looks (ax, dx:ax).
std::vector<std::string> Port::c_thunk(const std::string& name, const CSig& sig) {
    const auto [args, n] = slots(sig);
    const std::size_t extra = args.size() > 4 ? args.size() - 4 : 0;
    std::vector<std::string> out = {
        "", std::format("; {} from the old code: the arguments from the old stack, {}", name,
                        sig.pascal ? "taken off by it (pascal)" : "left for the caller"),
        std::format("@@c_{} PROC", name), "\tpush\tr15", "\tmov\tr15, rsp", "\tand\trsp, -16",
        std::format("\tsub\trsp, {}", 32 + 8 * (extra + extra % 2))};
    for (const Slot& s : args) {
        const std::string at = std::format("[r15+{}]", 16 + 8 * s.first);
        const std::string hi = std::format("[r15+{}]", 16 + 8 * (s.first + 1));
        const std::string dst = s.arg < 4 ? std::string(kArgRegs[s.arg]) : "rax";
        const std::string d32 = dst == "rcx" ? "ecx" : dst == "rdx" ? "edx" : dst == "r8" ? "r8d" : dst == "r9" ? "r9d" : "eax";
        switch (s.kind) {
            case CArg::Kind::Ptr: out.push_back(std::format("\tmov\t{}, qword ptr {}", dst, at)); break;
            case CArg::Kind::Long:
                out.push_back(std::format("\tmovzx\t{}, word ptr {}", d32, at));
                out.push_back(std::format("\tmovzx\tr11d, word ptr {}", hi));
                out.push_back("\tshl\tr11d, 16");
                out.push_back(std::format("\tor\t{}, r11d", d32));
                break;
            case CArg::Kind::Double:
                out.push_back(std::format("\tmov\t{}, qword ptr {}", dst, at));
                if (s.arg < 4) out.push_back(std::format("\tmovq\txmm{}, {}", s.arg, dst));
                break;
            case CArg::Kind::Val: out.push_back(std::format("\tmovsx\t{}, word ptr {}", dst, at)); break;
        }
        if (s.arg >= 4) out.push_back(std::format("\tmov\tqword ptr [rsp+{}], rax", 32 + 8 * (s.arg - 4)));
    }
    out.push_back("\tcall\t" + name);
    if (sig.ret == CSig::Ret::Long) {
        out.push_back("\tmov\tedx, eax");
        out.push_back("\tshr\tedx, 16\t\t; (a long: dx:ax)");
    }
    out.push_back("\tmov\trsp, r15");
    out.push_back("\tpop\tr15");
    out.push_back(sig.pascal && n ? std::format("\tret\t{}", 8 * n) : "\tret");
    out.push_back(std::format("@@c_{} ENDP", name));
    return out;
}

// ---- the module ------------------------------------------------------------------------------------------------------

std::string Port::emit(const std::set<std::string>& long_returns) {
    if (before_.size() != code_.size()) throw Asm16Error("the kinds of the module are not known: run() comes before emit()");
    used_int21_ = false;
    ptr_args_.clear();
    unreached_.clear();   // (lines no path reaches: said in the report)
    std::vector<std::string> out = {
        "; ported by CodeBreaker (port/asm16) for a 64-bit machine: the instructions as they were, the",
        "; registers that hold addresses used whole, members of structures at their new places, segments gone,",
        "; DOS called through the platform layer.", "OPTION CASEMAP:NONE", ""};
    for (const auto& [k, v] : equ_.values)
        if (number(v)) out.push_back(k + "\tEQU\t" + v);
    std::vector<std::string> ext;
    asm_calls_.clear();
    c_calls_.clear();
    auto add_ext = [&](const std::string& e) {
        if (std::find(ext.begin(), ext.end(), e) == ext.end()) ext.push_back(e);
    };
    for (const auto& [n, kind] : extrn_) {
        const bool proc_kind = kind == "NEAR" || kind == "FAR" || kind == "PROC";
        if (proc_kind && (settings_.asm_routines.contains(n) || callee(n))) continue;   // (called: by its own name, or its C name, below)
        if (const auto a = aliases.find(c_name(n)); a != aliases.end()) {   // (a place inside another: the other is extern)
            add_ext("EXTERN " + a->second.owner + ":BYTE");
            continue;
        }
        ext.push_back("EXTERN " + c_name(n) + ":" + (proc_kind ? "PROC" : kind));
    }
    std::vector<std::string> body;
    std::set<std::optional<std::string>> bases;
    for (std::size_t i = 0; i < code_.size(); ++i) {
        const Line& l = code_[i];
        if ((all_labels_public || (l.label && public_.contains(*l.label) && labels_.contains(*l.label))) && l.label && !l.directive)
            body.push_back("PUBLIC " + local(*l.label));   // (the other modules call it as it was)
        if (bases.insert(l.seg).second)
            body.push_back(std::format("@base_{}:\t\t\t; (the start of the segment {}: what its tables count from)", l.seg.value_or(""),
                                       l.seg.value_or("")));
        if (l.directive && (l.directive->kind == "DB" || l.directive->kind == "DW" || l.directive->kind == "DD" || l.directive->kind == "LABEL")) {
            for (std::string& x : code_data(i)) body.push_back(std::move(x));
            continue;
        }
        if (l.label) body.push_back(local(*l.label) + ":");
        if (!l.op) continue;
        const std::optional<State>& st = before_[i];
        if (!st) {
            body.push_back("\t; (never reached) " + comment_text(l));
            unreached_.push_back(l.no);
            continue;
        }
        try {
            for (const std::string& x : instr(i, *st)) body.push_back("\t" + x);
        } catch (const Asm16Error& e) {
            throw Asm16Error(std::format("line {} ({}): {}", l.no, text::trim(l.text), e.what()));
        }
    }
    if (used_int21_) ext.push_back("EXTERN dos_int21:PROC");
    for (const std::string& n : data_used_)   // (variables reached by their old addresses)
        if (!extrn_.contains(n) && !public_.contains(n)) add_ext("EXTERN " + sym_any(n) + ":BYTE");
    for (const std::string& n : asm_calls_) ext.push_back("EXTERN " + n + ":PROC");
    for (const auto& [n, sig] : c_calls_) ext.push_back("EXTERN " + n + ":PROC");
    std::vector<std::string> wrappers;
    for (const auto& [n, sig] : c_calls_)
        for (std::string& x : c_thunk(n, sig)) wrappers.push_back(std::move(x));
    std::vector<std::string> entries;
    for (const auto& [p, c] : cname) entries.push_back(p);
    std::stable_sort(entries.begin(), entries.end(), [&](const std::string& x, const std::string& y) { return labels_.at(x) < labels_.at(y); });
    for (const std::string& p : entries)
        for (std::string& x : wrapper(p, long_returns.contains(p))) wrappers.push_back(std::move(x));
    if (used_null_) ext.push_back("EXTERN dos_nullseg:BYTE");
    out.insert(out.end(), ext.begin(), ext.end());
    out.push_back("");
    out.push_back(".data");
    if (used_int21_) out.push_back("dosregs\tQWORD\t8 DUP (0)\t\t; (the registers given to DOS and back)");
    for (std::string& x : data_text()) out.push_back(std::move(x));
    out.push_back("");
    out.push_back(".code");
    out.insert(out.end(), wrappers.begin(), wrappers.end());
    out.insert(out.end(), body.begin(), body.end());
    out.push_back("");
    out.push_back("END");
    return text::join(out, "\n") + "\n";
}

// A table in the code segment: as it was -- a word that is the place of a routine of the segment, that place (counted from
// the segment's start, as the old one was).
std::vector<std::string> Port::code_data(std::size_t index) {
    const Line& l = code_[index];
    const std::string kind = l.directive->kind;
    const std::string rest(text::trim(strip_comment(l.directive->rest)));
    if (kind == "LABEL") return {*l.directive->name + "\tLABEL\t" + rest};
    std::vector<std::string> out;
    if (l.label) out.push_back(*l.label + ":");
    if (const auto at = dline_.find(index); at != dline_.end()) out.push_back(std::format("@d_{:04x}:", at->second.first));
    const std::string seg = l.seg.value_or("");
    if (kind == "DW") {
        static const Regex offset_name(R"(offset\s+(?:\w+:)?(\w+))", RegexFlags::IgnoreCase);
        std::vector<std::string> vals;
        for (const std::string& v : split_ops(rest)) {
            const auto n = number(v);
            const auto c = n ? code_label(l.seg, *n) : std::nullopt;
            const std::string trimmed(text::trim(v));
            if (const auto o = offset_name.full_match(trimmed); o && labels_.contains(std::string(o->group(1)))) {   // (a routine's place, named: from the segment's start)
                vals.push_back(local(std::string(o->group(1))) + " - @base_" + seg);
                continue;
            }
            vals.push_back(c && c->distance == 0 ? c->label + " - @base_" + seg : v);
        }
        out.push_back("\tDW\t" + text::join(vals, ", "));
        return out;
    }
    out.push_back("\t" + kind + "\t" + rest);
    return out;
}

std::vector<std::string> Port::data_text() {
    static const Regex offset_word(R"(\boffset\b)", RegexFlags::IgnoreCase);
    static const Regex offset_prefix(R"(offset\s+(?:\w+:)?)", RegexFlags::IgnoreCase);
    std::vector<std::string> out;
    for (const auto& [segment, l] : data_) {
        const auto& d = l.directive;
        if (!d) {
            if (l.label) out.push_back(*l.label + ":");
            continue;
        }
        const std::string& kind = d->kind;
        if (kind == "PUBLIC" || kind == "EXTRN" || kind == "ASSUME" || kind == "TITLE" || kind == "END") continue;
        const bool is_public = d->name && public_.contains(*d->name);
        const std::string nm = is_public ? c_name(*d->name) : d->name.value_or("");
        std::vector<std::string> pub;
        if (is_public) pub.push_back("PUBLIC " + nm);
        const std::string rest(text::trim(strip_comment(d->rest)));
        const bool is_data = kind == "DB" || kind == "DW" || kind == "DD" || kind == "DQ" || kind == "DT";
        auto emit_lines = [&](std::vector<std::string> lines) {
            out.insert(out.end(), pub.begin(), pub.end());
            out.insert(out.end(), lines.begin(), lines.end());
        };
        if (kind == "LABEL") {
            emit_lines({nm + "\tLABEL\t" + rest});
        } else if (is_data && d->name &&
                   std::any_of(own_ptr_.begin(), own_ptr_.end(), [&](const auto& p) { return p.first == *d->name; })) {   // (room for the addresses it holds, 8 bytes each)
            const std::int64_t grow = 6 * static_cast<std::int64_t>(std::count_if(own_ptr_.begin(), own_ptr_.end(), [&](const auto& p) { return p.first == *d->name; }));
            emit_lines({std::format("{}\t{}\t{}\t\t; ({} bytes more: {} words of it hold addresses)", nm, kind, rest, grow, grow / 6),
                        std::format("\tDB\t{} DUP (0)", grow)});
        } else if (is_data) {
            if (kind == "DW" && offset_word.test(rest)) {
                std::vector<std::string> vals;
                for (const std::string& v : split_ops(rest)) {
                    const std::string stripped(text::trim(offset_prefix.replace_all(v, "")));
                    vals.push_back(stripped.starts_with('_') ? c_name(stripped) : stripped);
                }
                emit_lines({nm + "\tDQ\t" + text::join(vals, ", ")});
            } else {
                emit_lines({nm + "\t" + kind + "\t" + rest});
            }
        }
    }
    return out;
}

// The entry of today's C: its arguments (rcx, rdx, r8, r9, then the stack) pushed as the old caller pushed them, the
// registers today's C keeps kept.
std::vector<std::string> Port::wrapper(const std::string& p, bool long_return) {
    const auto pit = params_.find(p);
    const std::size_t n = pit == params_.end() ? 0 : pit->second.size();
    const bool is_pascal = pascal_.contains(p);
    static const std::array<std::string, 5> kSaved = {"rbx", "rsi", "rdi", "rbp", "r15"};
    std::vector<std::string> names;
    for (std::size_t j = 0; j < n; ++j) names.push_back("a" + std::to_string(j));
    const std::string c = cname.at(p);
    std::vector<std::string> out = {"", std::format("; {}({}) - {}", c, text::join(names, ", "),
                                                    is_pascal ? "pascal: the first argument pushed first, the routine takes them off"
                                                              : "the last argument pushed first, taken off after"),
                                    "PUBLIC " + c, c + " PROC"};
    for (const std::string& r : kSaved) out.push_back("\tpush\t" + r);
    std::size_t pushed = 0;
    std::set<std::size_t> ptrs;
    {
        const auto pa = ptr_args_.find(p);
        if (pit != params_.end() && pa != ptr_args_.end())
            for (std::size_t k = 0; k < pit->second.size(); ++k)
                if (pa->second.contains(pit->second[k].old_offset)) ptrs.insert(k);
    }
    std::vector<std::pair<std::size_t, int>> words;   // (word k of the old stack: (C argument, its half))
    bool have_words = false;
    const auto types = param_types.find(p);
    if (!is_pascal && types != param_types.end() && !types->second.empty() &&
        std::any_of(types->second.begin(), types->second.end(), [](const ParamType& t) { return t.size == 4; })) {
        std::size_t total = 0;
        for (const ParamType& t : types->second) total += t.size == 4 ? 2 : 1;
        if (total == n) {
            for (std::size_t i = 0; i < types->second.size(); ++i) {   // (a long of the C: two words, the low one first)
                const ParamType& t = types->second[i];
                if (t.size == 4 && !(t.to && !t.to->empty())) {
                    words.emplace_back(i, 0);
                    words.emplace_back(i, 1);
                } else {
                    words.emplace_back(i, 0);
                }
            }
            have_words = words.size() == n;
        }
    }
    for (std::size_t k = n; k-- > 0;) {   // (slot 0 at the lowest address, as the old stack had it)
        std::size_t j;
        int half = 0;
        if (have_words) {
            j = words[k].first;
            half = words[k].second;
        } else {
            j = is_pascal ? n - 1 - k : k;
        }
        std::string reg_name;
        if (j < 4) {
            reg_name = kArgRegs[j];
        } else {
            const std::size_t off = 8 * kSaved.size() + 8 * pushed + 8 + 32 + 8 * (j - 4);
            out.push_back(std::format("\tmov\tr11, qword ptr [rsp+{}]", off));
            reg_name = "r11";
        }
        if (half) {   // (the high word of a long: its own slot)
            out.push_back("\tmov\tr11, " + reg_name);
            out.push_back("\tshr\tr11, 16");
            reg_name = "r11";
        }
        if (ptrs.contains(k)) {   // (a NULL address read what began the data segment)
            const std::string lab = std::format("@@null_{}_{}", c, k);
            out.push_back(std::format("\ttest\t{}, {}", reg_name, reg_name));
            out.push_back("\tjnz\t" + lab);
            out.push_back("\tlea\t" + reg_name + ", dos_nullseg");
            out.push_back(lab + ":");
            used_null_ = true;
        }
        out.push_back("\tpush\t" + reg_name);
        ++pushed;
    }
    out.push_back("\tcall\t" + local(p));
    if (!is_pascal && n) out.push_back(std::format("\tadd\trsp, {}", 8 * n));
    if (long_return) {
        out.push_back("\tmovzx\teax, ax");
        out.push_back("\tshl\tedx, 16");
        out.push_back("\tor\teax, edx\t\t; (a long: dx:ax)");
    }
    for (auto it = kSaved.rbegin(); it != kSaved.rend(); ++it) out.push_back("\tpop\t" + *it);
    out.push_back("\tcld");
    out.push_back("\tret");
    out.push_back(c + " ENDP");
    return out;
}

// ---- one instruction ------------------------------------------------------------------------------------------------

std::string Port::reg(std::string_view r, const State& st, std::optional<bool> wide) const {
    if (is_reg8(r)) return std::string(r);
    const K* k = st.reg(r);
    const bool w = !wide && k ? k->wide() : wide.value_or(false);
    return w ? r64(r) : std::string(r);
}

// The C name of a variable of the program, whether the module declares it or not.
std::string Port::sym_any(const std::string& s) const {
    const std::string c = c_name(s);
    const auto a = aliases.find(c);
    return a != aliases.end() ? a->second.owner : c;
}

// An argument of the routine used as an address here: noted (its NULL is the old data segment's start).
void Port::as_address(std::size_t i, const std::optional<K>& k) {
    if (!k) return;
    for (const Origin& o : k->origin.get())
        if (o.source == Origin::Source::Param) ptr_args_[routine(i)].insert(o.n);
}

// An old offset below the entry's sp: its new one (the pushes and the room made, as they are now).
std::int64_t Port::stack_place(const State& st, std::int64_t e) const {
    std::int64_t o = 0, n = 0;
    for (const StackItem& item : st.stack) {
        if (o - item.old_size <= e && e < o) return n - item.new_size + (e - (o - item.old_size));   // (the same byte of the item: a push now 8 bytes)
        o -= item.old_size;
        n -= item.new_size;
    }
    throw Asm16Error(std::format("sp{:+d} is not on the stack as it is here", e));
}

// add/sub sp, n: the bytes it is now (the pushes it takes off are 8 bytes each, room made the same).
std::int64_t Port::sp_new_bytes(const State& st, std::int64_t n, std::string_view op) const {
    if (op == "sub") return n;
    std::vector<StackItem> items = st.stack;
    std::int64_t left = n, now = 0;
    while (left > 0 && !items.empty()) {
        left -= items.back().old_size;
        now += items.back().new_size;
        items.pop_back();
    }
    return now;
}

// A memory operand at its new place: text (with its size), the instructions it needs first in `pre`.
std::string Port::mem(const Mem& m0, const State& st, int size, std::size_t i, std::vector<std::string>& pre) {
    const std::string sz = size_word(size);
    const Mem m = absolute(m0);
    std::int64_t disp = m.disp;
    std::vector<std::string> parts;
    std::vector<std::string> scratch = {"r11", "r9"};
    if (m.sym) {
        const std::string name = sym(*m.sym);
        disp = own_place(*m.sym, disp);
        if (m.regs.empty()) return sz + name + signed_part(disp);
        pre.push_back("lea\tr10, " + name);
        parts.push_back("r10");
        const auto es = extrn_.contains(*m.sym) || public_.contains(*m.sym) ? ct_.elem_sizes(c_name(*m.sym)) : std::nullopt;
        const K* k_ = m.regs.size() == 1 ? st.reg(m.regs[0]) : nullptr;
        if (es && es->old_size != es->new_size && m.regs.size() == 1 && !(k_ && k_->wide())) {   // (an index of the old elements: today's)
            const std::int64_t so = es->old_size, sn = es->new_size;
            if (!power_of_two(so)) throw Asm16Error(std::format("an index into elements of {} bytes", so));
            pre.push_back("movzx\tr11d, " + m.regs[0]);
            pre.push_back(std::format("imul\tr11, r11, {}", sn));
            pre.push_back(std::format("sar\tr11, {}\t\t; ({} bytes an element then, {} now)", bit_length_minus_one(so), so, sn));
            disp = floor_div(disp, so) * sn + floor_mod(disp, so);
            return sz + "[r10+r11" + signed_part(disp) + "]";
        }
    }
    if (m.regs.size() == 1 && !m.sym) {
        const K* k = st.reg(m.regs[0]);
        if (k && k->kind == Kind::Ptr && k->region == Region::DataSegment) return mem(ds_mem(m, *k), st, size, i, pre);
    }
    for (const std::string& r : m.regs) {
        const K* k = st.reg(r);
        as_address(i, k ? std::optional<K>(*k) : std::nullopt);
        if (k && k->kind == Kind::Frame) {
            const Rt rt = k->owner ? k->owner->routine : routine(i);
            const std::int64_t e = k->old + disp;
            std::int64_t now;
            if (e >= retsize(rt)) {
                const auto [sl, inner] = slot(rt, e);
                now = 8 + 8 * static_cast<std::int64_t>(sl) + inner;
            } else if (k->owner) {   // (the owner's stack, as it was when it called)
                std::int64_t o = 0, n = 0;
                std::optional<std::int64_t> found;
                for (const auto& [so, sn] : k->owner->stack) {
                    if (o - so <= e && e < o) {
                        found = n - sn + (e - (o - so));
                        break;
                    }
                    o -= so;
                    n -= sn;
                }
                if (!found) throw Asm16Error(std::format("sp{:+d} is not on the stack of {}", e, rt ? *rt : std::string("(no routine)")));
                now = *found;
            } else {
                now = stack_place(st, e);
            }
            disp = now - k->new_;
            parts.push_back(r64(r));
        } else if (k && k->wide()) {
            const std::string kt = k->region == Region::Any && k->to ? *k->to : std::string();
            if (k->kind == Kind::Ptr && kt.ends_with("*") && m.regs.size() == 1 && !m.sym) {
                disp = disp * 4;   // (pointers of 2 bytes, 8 now)
            } else if (k->kind == Kind::Ptr && (kt.starts_with("struct") || kt.starts_with("union")) && m.regs.size() == 1 && !m.sym) {
                if (const auto f = ct_.field(kt, disp)) disp = f->new_offset + (disp - f->old_offset);
            }
            parts.push_back(r64(r));
        } else {
            if (scratch.empty()) throw Asm16Error("a memory operand of more 16-bit registers than there are scratch registers");
            const std::string s = scratch.front();
            scratch.erase(scratch.begin());
            pre.push_back(std::format("movzx\t{}d, {}", s, r));
            parts.push_back(s);
        }
    }
    return sz + "[" + text::join(parts, "+") + signed_part(disp) + "]";
}

// The size the instruction's memory operand is used at, now.
int Port::access_size(const std::vector<Operand>& a, const State& st, std::size_t which) {
    const Mem& m = a[which].mem;
    const auto [k, f] = kind_of_mem(m, st);
    const Operand* other = a.size() > 1 ? &a[1 - which] : nullptr;
    if (other && other->kind == OK::Reg) {
        const std::string& r = other->text;
        if (is_reg8(r)) return 1;
        const K* ko = st.reg(r);
        if (ko && ko->wide() && ko->kind != Kind::Frame) {
            const K* base = !m.regs.empty() ? st.reg(m.regs[0]) : nullptr;
            if (k.wide() || (f && f->is_pointer) || (base && base->kind == Kind::Frame)) return 8;
        }
        return 2;
    }
    if (m.size) return *m.size;
    return 2;
}

std::vector<std::string> Port::instr(std::size_t i, const State& st) {
    const Line& l = code_[i];
    const std::string& op = *l.op;
    const std::vector<Operand>& a = ops(i);
    std::vector<std::string> pre;
    const std::string text_of = comment_text(l);

    auto R = [&](std::string_view r, std::optional<bool> w = std::nullopt) { return reg(r, st, w); };
    auto M = [&](std::size_t which, int size = 0) {
        if (which >= a.size() || a[which].kind != OK::Mem) throw Asm16Error("no memory operand where `" + op + "` has one");
        return mem(a[which].mem, st, size ? size : access_size(a, st, which), i, pre);
    };
    auto done = [&](std::vector<std::string> lines) {
        std::vector<std::string> out = pre;
        out.insert(out.end(), std::make_move_iterator(lines.begin()), std::make_move_iterator(lines.end()));
        return out;
    };
    auto operand_at = [&](std::size_t k) -> const Operand& {
        if (k >= a.size()) throw Asm16Error("`" + op + "` has too few operands");
        return a[k];
    };
    auto segop = [](const Operand& o) { return o.kind == OK::Reg && is_segment_register(o.text); };
    auto offset_text = [&](const Operand& o) { return sym(o.text) + signed_part(own_place(o.text, o.disp)); };   // (`offset sym+4`: the place now)
    auto segment_comment = [&] { return std::vector<std::string>{"; " + text_of + "\t\t(a segment: one flat memory)"}; };

    if (op == "mov" && a.size() >= 2 && a[0].kind == OK::Reg && is_reg16(a[0].text) && a[1].kind == OK::Reg &&
        (a[1].text == "es" || a[1].text == "ds")) {
        const std::optional<K>& pair = a[1].text == "es" ? st.es : st.ds;
        if (pair) {
            const auto partner = std::find_if(pair->origin.get().begin(), pair->origin.get().end(),
                                              [](const Origin& o) { return o.source == Origin::Source::SegOf; });
            const K* pk = partner != pair->origin.get().end() ? st.reg(partner->a) : nullptr;
            if (pk && pk->wide())
                return {std::format("mov\t{}, {}\t\t; (the segment of the address in {}: the address, 0 when it is)", r64(a[0].text),
                                    r64(partner->a), partner->a)};
        }
    }
    if (op == "mov" && a.size() >= 2 && (segop(a[0]) || segop(a[1]))) return segment_comment();
    if ((op == "push" || op == "pop") && !a.empty() && segop(a[0])) return segment_comment();
    std::optional<std::size_t> src_at;
    if (op == "push") src_at = 0;
    else if ((op == "or" || op == "and" || op == "add" || op == "sub" || op == "cmp" || op == "adc" || op == "sbb" || op == "xor" || op == "test") &&
             a.size() == 2 && a[0].kind == OK::Reg)
        src_at = 1;
    if (src_at && *src_at < a.size() && a[*src_at].kind == OK::Mem) {   // (the segment half of a far address read: none now, 0 --
        const auto [k_, f_] = kind_of_mem(a[*src_at].mem, st);          //  the address is whole in the other half's place)
        if (k_.kind == Kind::Seg) {
            if (op == "push") return {"push\t0\t\t; (" + text_of + ": a segment, none now)"};
            const K* kd = st.reg(a[0].text);
            const std::string d = is_reg16(a[0].text) && kd && kd->wide() ? r64(a[0].text) : a[0].text;
            return {op + "\t" + d + ", 0\t\t; (" + text_of + ": a segment, none now)"};
        }
    }
    if (is_jump(op)) {
        const std::string t = target_of_line(l);
        if (op == "jcxz" || op.starts_with("loop")) {   // (a short jump only, and the flags kept: through a jump beside it to the place,
            const std::string n = std::format("@@j{}", i);   //  however far)
            return {"movzx\tecx, cx", (op == "jcxz" ? std::string("jrcxz") : op) + "\t" + n + "_t", "jmp\t" + n + "_n", n + "_t:", "jmp\t" + t, n + "_n:"};
        }
        return {op + "\t" + t};
    }
    if (op == "call") {
        const std::string t = target_of_line(l);
        if (labels_.contains(t)) return {"call\t" + local(t)};
        if (!a.empty() && a[0].kind == OK::Mem && a[0].mem.seg == "cs") {   // (through a table of the code: its entries are the routines' places from the segment's start)
            std::vector<std::string> pre2;
            const std::string where = mem(a[0].mem, st, 2, i, pre2);
            pre2.push_back("movzx\tr11d, " + where);
            pre2.push_back("lea\tr10, @base_" + l.seg.value_or(""));
            pre2.push_back("add\tr11, r10");
            pre2.push_back("call\tr11");
            return pre2;
        }
        const auto ce = callee(t);
        if (ce && ce->kind == Callee::Kind::Asm) {
            asm_calls_.insert(ce->name);
            return {"call\t" + ce->name};
        }
        if (ce) {
            c_calls_[ce->name] = ce->sig;
            return {"call\t@@c_" + ce->name};
        }
        throw Asm16Error("a call of " + t + ", outside the module (not yet ported)");
    }
    if (op == "ret" || op == "retn" || op == "retf") {
        if (!l.args.empty()) {
            const Rt rt = routine(i);
            const auto nbytes = number(l.args[0]);
            if (!nbytes) throw Asm16Error("a return of `" + l.args[0] + "` bytes");
            const auto it = params_.find(rt);
            std::int64_t n = 0;
            if (it != params_.end())
                for (const Param& p : it->second)
                    if (p.old_offset < retsize(rt) + *nbytes) ++n;
            return {std::format("ret\t{}", 8 * n)};
        }
        return {"ret"};
    }
    if (op == "int") {
        if (l.args.empty() || number(l.args[0]) != 0x21) throw Asm16Error("int " + (l.args.empty() ? std::string() : l.args[0]));
        used_int21_ = true;
        return {"mov\tqword ptr dosregs, rax", "mov\tqword ptr dosregs+8, rbx", "mov\tqword ptr dosregs+16, rcx",
                "mov\tqword ptr dosregs+24, rdx", "mov\tqword ptr dosregs+32, rsi", "mov\tqword ptr dosregs+40, rdi",
                "mov\tr15, rsp\t\t; (DOS: the platform layer, called as today's C is)", "and\trsp, -16", "sub\trsp, 32",
                "lea\trcx, dosregs", "call\tdos_int21", "mov\trsp, r15", "mov\trax, qword ptr dosregs", "mov\trbx, qword ptr dosregs+8",
                "mov\trcx, qword ptr dosregs+16", "mov\trdx, qword ptr dosregs+24", "bt\tqword ptr dosregs+48, 0\t; (the carry: an error)"};
    }
    if (op == "xlat" || op == "xlatb") {   // (al from the table at bx: the table's address whole)
        const K* k = st.reg("bx");
        if (!(k && k->wide())) throw Asm16Error("xlat with bx not an address");
        return {"xlatb"};
    }
    if (op == "pushf" || op == "pushfw") return {"pushfq"};
    if (op == "popf" || op == "popfw") return {"popfq"};
    if (op == "push") {
        const Operand& a0 = operand_at(0);
        if (a0.kind == OK::Reg) return {"push\t" + r64(a0.text)};
        if (a0.kind == OK::Mem) {
            const auto [k, f] = kind_of_mem(a0.mem, st);
            if (k.wide()) {
                std::string w = M(0, 8);
                return done({"push\t" + w});
            }
            std::string w = M(0, 2);
            return done({"movzx\tr11d, " + w, "push\tr11"});
        }
        if (a0.kind == OK::Off) return {"lea\tr11, " + offset_text(a0), "push\tr11"};
        return {"push\t" + l.args[0]};
    }
    if (op == "pop") {
        const Operand& a0 = operand_at(0);
        if (a0.kind == OK::Reg) return {"pop\t" + r64(a0.text)};
        throw Asm16Error("pop to memory");
    }
    if (op == "les" || op == "lds") {
        const std::string d = r64(operand_at(0).text);
        return done({"mov\t" + d + ", " + M(1, 8)});
    }
    if (op == "lea") {
        const std::string d = r64(operand_at(0).text);
        const std::string w = M(1, 8);
        return done({"lea\t" + d + ", " + w.substr(std::string("qword ptr ").size())});
    }
    if ((op == "mov" || op == "xchg") && a.size() >= 2 && a[0].kind == OK::Reg && a[1].kind == OK::Reg) {
        const std::string& d = a[0].text;
        const std::string& s = a[1].text;
        if (s == "sp") return {op + "\t" + r64(d) + ", rsp"};
        if (is_reg8(d) || is_reg8(s)) return {op + "\t" + d + ", " + s};
        const K* ks = st.reg(s);
        const K* kd = st.reg(d);
        std::optional<bool> w;   // (a pointer on either side keeps both whole; `None` leaves each to what it holds)
        if (ks && ks->wide()) w = true;
        else if (op != "xchg") w = false;
        else if (kd) w = kd->wide();
        return {op + "\t" + R(d, w) + ", " + R(s, w)};
    }
    if ((op == "mov" || op == "xchg") && a.size() >= 2 && a[0].kind == OK::Reg && a[1].kind == OK::Mem) {
        const std::string& d = a[0].text;
        const auto [k, f] = kind_of_mem(a[1].mem, st);
        if (k.kind == Kind::Seg && op == "mov" && is_reg16(d)) {   // (the segment of a far address read: 0 when the address is 0, else not)
            const Mem& m_ = a[1].mem;
            const Mem lo{m_.size, m_.seg, m_.sym, m_.regs, m_.disp - 2, m_.text};
            const std::string where = mem(lo, st, 8, i, pre);
            return done({"mov\tr11, " + where, "test\tr11, r11", "setnz\tr11b\t\t; (" + text_of + ": the segment of an address - 0 when it is 0)",
                         "movzx\te" + d + ", r11b"});
        }
        if (k.kind == Kind::Seg) return segment_comment();
        if (is_reg8(d)) {
            const std::string w = M(1, 1);
            return done({op + "\t" + d + ", " + w});
        }
        if (k.wide() && d != "cx") {   // (cx: a count, whatever the member was declared)
            const std::string w = M(1, 8);
            return done({op + "\t" + r64(d) + ", " + w});
        }
        const std::string w = M(1, 2);
        return done({op + "\t" + d + ", " + w});
    }
    if ((op == "mov" || op == "xchg") && a.size() >= 2 && a[0].kind == OK::Mem && a[1].kind == OK::Reg) {
        const std::string& s = a[1].text;
        const auto [k, f] = kind_of_mem(a[0].mem, st);
        if (k.kind == Kind::Seg) return segment_comment();
        const int size = access_size(a, st, 0);
        if (size == 2 && f && f->is_pointer && f->old_size == 4 && f->new_size == 8 && is_reg16(s) && op == "mov") {
            const std::string w = M(0, 8);
            return done({"movzx\tr11d, " + s + "\t\t; (the offset half of a far address: the address now)", "mov\t" + w + ", r11"});
        }
        const std::string w = M(0, size);
        return done({op + "\t" + w + ", " + (size == 8 ? r64(s) : s)});
    }
    if (op == "mov" && a.size() >= 2 && a[0].kind == OK::Reg && a[1].kind == OK::Off)
        return {"lea\t" + r64(a[0].text) + ", " + offset_text(a[1])};
    const K* k_after = nullptr;
    if (op == "mov" && a.size() >= 2 && a[0].kind == OK::Reg && a[1].kind == OK::Imm && i + 1 < code_.size() && before_[i + 1])
        k_after = before_[i + 1]->reg(a[0].text);
    if (k_after && k_after->kind == Kind::Ptr && k_after->region == Region::DataSegment) {
        const auto v = data_var(k_after->ds_address);
        if (!v) throw Asm16Error("no variable at the data segment's old address");
        data_used_.insert(v->first);
        return {std::format("lea\t{}, {}{}\t\t; ({}: the old address of {})", r64(a[0].text), sym(v->first), signed_part(v->second), l.args[1],
                            c_name(v->first))};
    }
    if (k_after && k_after->kind == Kind::Ptr && k_after->region == Region::Code) {
        const std::int64_t v = number(l.args[1]).value_or(0);
        auto place = data_address(l.seg.value_or(""), v);
        if (!place) place = code_label(l.seg, v);
        if (!place) throw Asm16Error("no address in the code at " + l.args[1]);
        return {std::format("lea\t{}, {}{}\t\t; ({}: an address in the code)", r64(a[0].text), place->label, signed_part(place->distance), l.args[1])};
    }
    if (op == "mov" && a.size() >= 2 && a[0].kind == OK::Reg && a[1].kind == OK::Sym &&
        ((a[1].text.size() > 3 && text::iequals(a[1].text.substr(0, 3), "seg") && text::is_space(a[1].text[3])) ||
         text::upper(a[1].text) == "DGROUP" || code_seg_.contains(a[1].text)))
        return segment_comment();
    if (op == "mov" && a.size() >= 2 && a[0].kind == OK::Reg && (a[1].kind == OK::Imm || a[1].kind == OK::Sym))
        return {"mov\t" + a[0].text + ", " + l.args[1]};
    if (op == "mov" && a.size() >= 2 && a[0].kind == OK::Mem && a[1].kind == OK::Off) {   // (an address stored: the whole of it)
        const std::string w = M(0, 8);
        return done({"lea\tr11, " + offset_text(a[1]), "mov\t" + w + ", r11"});
    }
    if (op == "mov" && a.size() >= 2 && a[0].kind == OK::Mem) {
        const auto [k, f] = kind_of_mem(a[0].mem, st);
        int size = a[0].mem.size.value_or(2);
        if (k.kind == Kind::Ptr && f && f->is_pointer && number(l.args[1]) == 0) size = 8;
        const std::string w = M(0, size);
        return done({"mov\t" + w + ", " + l.args[1]});
    }
    static const std::set<std::string_view> kAlu = {"add", "sub", "and", "or", "xor", "cmp", "test", "adc", "sbb"};
    if (kAlu.contains(op) && a.size() >= 2 && a[0].kind == OK::Reg) {
        const std::string d = a[0].text;
        if (d == "sp") {
            const std::int64_t n = number(l.args[1]).value_or(0);
            return {std::format("{}\trsp, {}", op, sp_new_bytes(st, n, op))};
        }
        const K* kd = is_reg16(d) ? st.reg(d) : nullptr;
        if (op == "xor" && a[1].same_register(a[0]) && is_reg16(d)) return {"xor\te" + d + ", e" + d};
        if (a[1].kind == OK::Imm && op == "sub" && is_reg16(d) && !(kd && kd->wide()) && data_at(number(l.args[1]))) {
            const std::string v = *data_at(number(l.args[1]));   // (an offset less a variable's old address: today's, the low 16 bits -- the same difference)
            data_used_.insert(v);
            return {"lea\tr11, " + sym(v) + "\t\t; (" + l.args[1] + ": the old address of " + c_name(v) + ")", "sub\t" + d + ", r11w"};
        }
        if (a[1].kind == OK::Imm && (op == "sub" || op == "cmp" || op == "add") && kd && kd->wide() && data_at(number(l.args[1]))) {
            const std::string v = *data_at(number(l.args[1]));   // (the old address of a variable: its address now)
            data_used_.insert(v);
            return {"lea\tr11, " + sym(v) + "\t\t; (" + l.args[1] + ": the old address of " + c_name(v) + ")", std::string(op) + "\t" + r64(d) + ", r11"};
        }
        if (is_reg8(d)) {
            if (a[1].kind == OK::Mem) {
                const std::string w = M(1, 1);
                return done({std::string(op) + "\t" + d + ", " + w});
            }
            return {std::string(op) + "\t" + d + ", " + l.args[1]};
        }
        const Operand& src = a[1];
        const std::string ops_name(op);
        if (src.kind == OK::Reg) {
            const K* ks = is_reg16(src.text) ? st.reg(src.text) : nullptr;
            if (is_reg8(src.text)) return {ops_name + "\t" + d + ", " + src.text};
            if (kd && kd->wide() && ks && ks->wide()) return {ops_name + "\t" + r64(d) + ", " + r64(src.text)};
            if (kd && kd->wide() && kd->kind != Kind::Frame && (op == "add" || op == "sub")) {
                const ElementSizes es = element_sizes(*kd);
                if (es.old_size != es.new_size) {   // (an index into an array: the elements' size now)
                    std::vector<std::string> out = {"movsx\tr11, " + src.text, std::format("imul\tr11, r11, {}", es.new_size)};
                    if (!power_of_two(es.old_size)) throw Asm16Error(std::format("an index into elements of {} bytes", es.old_size));
                    out.push_back(std::format("sar\tr11, {}\t\t; ({} bytes an element then, {} now)", bit_length_minus_one(es.old_size), es.old_size, es.new_size));
                    out.push_back(ops_name + "\t" + r64(d) + ", r11");
                    return out;
                }
                return {"movsx\tr11, " + src.text, ops_name + "\t" + r64(d) + ", r11"};
            }
            if (op == "add" && ks && ks->wide() && ks->kind != Kind::Frame && (!kd || !kd->wide())) {
                const ElementSizes es = element_sizes(*ks);
                if (es.old_size != es.new_size && power_of_two(es.old_size))   // (an index of the old elements: today's)
                    return {"movzx\te" + d + ", " + d, std::format("imul\t{}, {}, {}", r64(d), r64(d), es.new_size),
                            std::format("sar\t{}, {}\t\t; ({} bytes an element then, {} now)", r64(d), bit_length_minus_one(es.old_size), es.old_size, es.new_size),
                            "add\t" + r64(d) + ", " + r64(src.text) + "\t\t; (a count added to an address: the address)"};
                return {"movzx\te" + d + ", " + d, "add\t" + r64(d) + ", " + r64(src.text) + "\t\t; (a count added to an address: the address)"};
            }
            return {ops_name + "\t" + d + ", " + src.text};
        }
        if (src.kind == OK::Mem) {
            const auto [k, f] = kind_of_mem(src.mem, st);
            if (kd && kd->wide() && k.wide()) {
                const std::string w = M(1, 8);
                return done({ops_name + "\t" + r64(d) + ", " + w});
            }
            if (kd && kd->wide() && kd->kind != Kind::Frame && (op == "add" || op == "sub") && is_reg16(d)) {
                const std::string where = M(1, 2);   // (a count from memory added to an address: the whole)
                std::vector<std::string> out = pre;
                out.push_back("movsx\tr8, " + where);
                out.push_back(ops_name + "\t" + r64(d) + ", r8");
                return out;
            }
            const std::string w = M(1, 2);
            return done({ops_name + "\t" + d + ", " + w});
        }
        if (src.kind == OK::Off) return {"lea\tr11, " + offset_text(src), ops_name + "\t" + r64(d) + ", r11"};
        if (op == "cmp" && kd && kd->kind == Kind::Ptr && kd->region == Region::Code && number(l.args[1])) {
            auto place = data_address(l.seg.value_or(""), *number(l.args[1]));
            if (!place) place = code_label(l.seg, *number(l.args[1]));
            if (place) return {"lea\tr11, " + place->label + signed_part(place->distance), ops_name + "\t" + r64(d) + ", r11"};
        }
        if (kd && kd->wide() && kd->kind == Kind::Ptr) {
            std::optional<std::int64_t> v = number(l.args[1]);
            if (v && *v >= 0x8000) *v -= 0x10000;
            return {ops_name + "\t" + r64(d) + ", " + (v ? std::to_string(*v) : l.args[1])};
        }
        return {ops_name + "\t" + d + ", " + l.args[1]};
    }
    if (kAlu.contains(op) && a.size() >= 2 && a[0].kind == OK::Mem) {
        int size = a[1].kind == OK::Reg && is_reg8(a[1].text) ? 1 : a[0].mem.size.value_or(2);
        const std::string ops_name(op);
        {
            const auto [k_, f_] = kind_of_mem(a[0].mem, st);
            if ((op == "add" || op == "sub") && size == 2 && k_.kind == Kind::Ptr) {   // (an address moved on by a count: the whole of it)
                if (a[1].kind == OK::Imm) {
                    const std::string w = M(0, 8);
                    return done({ops_name + "\t" + w + ", " + l.args[1]});
                }
                if (a[1].kind == OK::Reg && is_reg16(a[1].text)) {
                    const std::string where = M(0, 8);   // (r8: the operand's own place may want r11)
                    std::vector<std::string> out = pre;
                    out.push_back("movsx\tr8, " + a[1].text);
                    out.push_back(ops_name + "\t" + where + ", r8");
                    return out;
                }
            }
        }
        if ((op == "cmp" || op == "test") && size == 2 && a[1].kind == OK::Imm && number(l.args[1]) == 0) {
            const auto [k_, f_] = kind_of_mem(a[0].mem, st);
            if (k_.kind == Kind::Ptr) size = 8;   // (an address compared with 0: the whole of it)
        }
        if (size == 8) {
            const std::string w = M(0, 8);
            return done({ops_name + "\t" + w + ", 0"});
        }
        const std::string w = M(0, size);
        return done({ops_name + "\t" + w + ", " + (a[1].kind != OK::Reg ? l.args[1] : a[1].text)});
    }
    if ((op == "inc" || op == "dec" || op == "neg" || op == "not") && !a.empty() && a[0].kind == OK::Reg) {
        const std::string& d = a[0].text;
        const K* k = is_reg16(d) ? st.reg(d) : nullptr;
        return {op + "\t" + (k && k->wide() && (op == "inc" || op == "dec") ? r64(d) : d)};
    }
    if ((op == "inc" || op == "dec" || op == "neg" || op == "not") && !a.empty() && a[0].kind == OK::Mem) {
        const auto [k_, f_] = kind_of_mem(a[0].mem, st);
        const Mem& m_ = a[0].mem;
        const K* fr = m_.regs.size() == 1 && !m_.sym ? st.reg(m_.regs[0]) : nullptr;
        const bool arg_slot = fr && fr->kind == Kind::Frame && k_.wide() &&
                              fr->old + m_.disp >= retsize(fr->owner ? fr->owner->routine : routine(i));
        if ((op == "inc" || op == "dec") && (k_.kind == Kind::Ptr || arg_slot) && m_.size.value_or(2) == 2) {
            const std::string w = M(0, 8);
            return done({op + "\t" + w + "\t\t; (an address moved on: the whole of it)"});
        }
        const std::string w = M(0, m_.size.value_or(2));
        return done({op + "\t" + w});
    }
    if (op == "shl" || op == "shr" || op == "sal" || op == "sar" || op == "rol" || op == "ror" || op == "rcl" || op == "rcr") {
        const Operand& a0 = operand_at(0);
        if (l.args.size() < 2) throw Asm16Error("`" + op + "` with no count");
        if (a0.kind == OK::Mem) {
            const std::string w = M(0, a0.mem.size.value_or(2));
            return done({op + "\t" + w + ", " + l.args[1]});
        }
        return {op + "\t" + a0.text + ", " + l.args[1]};
    }
    if (op == "mul" || op == "imul" || op == "div" || op == "idiv") {
        const Operand& a0 = operand_at(0);
        if (a0.kind == OK::Mem) {
            const std::string w = M(0, a0.mem.size.value_or(2));
            return done({op + "\t" + w});
        }
        return {op + "\t" + a0.text};
    }
    if (is_string_op(op)) {
        const std::string head = op.substr(0, 4);
        for (const char* r : {"si", "di"}) {
            const std::string_view rv = r;
            if ((head == "lods" || head == "movs" || head == "cmps") && rv == "si") as_address(i, st.reg(r) ? std::optional<K>(*st.reg(r)) : std::nullopt);
            if ((head == "stos" || head == "movs" || head == "cmps" || head == "scas") && rv == "di")
                as_address(i, st.reg(r) ? std::optional<K>(*st.reg(r)) : std::nullopt);
        }
        if (l.prefix) return {"movzx\tecx, cx", *l.prefix + " " + op};
        return {op};
    }
    if (op == "cbw" || op == "cwd" || op == "cld" || op == "std" || op == "stc" || op == "clc" || op == "cmc" || op == "nop" || op == "lahf" ||
        op == "sahf" || op == "cli" || op == "sti")
        return {op};
    throw Asm16Error("no rule for " + op);
}

}  // namespace cb::port::asm16
