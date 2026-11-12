// cb/translate/backends/java_fn.cpp -- the Java back end's function writer: one C function's body in the faithful tier
// (statements, places, values, conversions). The tier's file and the program are java.cpp.
#include <algorithm>
#include <cmath>
#include <format>

#include "cb/core/text.hpp"
#include "cb/translate/backends/java.hpp"

namespace cb::translate::backends::java {

namespace {

using JLines = std::vector<std::string>;

void jf_append(JLines& out, const JLines& more) { out.insert(out.end(), more.begin(), more.end()); }

JLines jf_indented(const JLines& lines, std::string_view prefix = "    ") {
    JLines out;
    out.reserve(lines.size());
    for (const std::string& l : lines) out.push_back(std::string(prefix) + l);
    return out;
}

// Indented by `prefix`, an empty line staying empty.
JLines jf_indented_text(const JLines& lines, std::string_view prefix) {
    JLines out;
    out.reserve(lines.size());
    for (const std::string& l : lines) out.push_back(l.empty() ? l : std::string(prefix) + l);
    return out;
}

// One statement of a body slot: the node, or a Block holding a list (made in the arena).
ir::Node* jf_one(Arena& arena, const ir::Suite& suite) {
    if (const auto* n = std::get_if<ir::Node*>(&suite)) return *n;
    return ir::make(arena, ir::Block{.stmts = std::get<std::vector<ir::Node*>>(suite)});
}

// The statements of a function or block body: a Block's own, else the statement (or list) as it is.
std::vector<ir::Node*> jf_body_statements(const ir::Suite& suite) {
    if (const auto* n = std::get_if<ir::Node*>(&suite)) {
        if (const ir::Block* b = ir::get<ir::Block>(*n)) return b->stmts;
    }
    return ir::statements(suite);
}

bool jf_is_loop_or_switch(const ir::Node* n) {
    return ir::is<ir::While>(n) || ir::is<ir::For>(n) || ir::is<ir::DoWhile>(n) || ir::is<ir::Switch>(n);
}

bool jf_ends_with(const std::string& s, std::string_view tail) { return s.ends_with(tail); }

// `<head> + <digits>` at the end of an address text, when the head holds no call or condition: its parts.
std::optional<std::pair<std::string, std::int64_t>> jf_split_plus_number(const std::string& addr) {
    const std::size_t plus = addr.rfind(" + ");
    if (plus == std::string::npos) return std::nullopt;
    const std::string_view digits = std::string_view(addr).substr(plus + 3);
    if (digits.empty() || !std::all_of(digits.begin(), digits.end(), [](char c) { return c >= '0' && c <= '9'; })) return std::nullopt;
    const std::string head = addr.substr(0, plus);
    if (head.find('(') != std::string::npos || head.find('?') != std::string::npos) return std::nullopt;
    const auto n = text::parse_int<std::int64_t>(digits);
    if (!n) return std::nullopt;
    return std::pair<std::string, std::int64_t>{head, *n};
}

// The value of Java integer text (`5`, `(-3)`).
std::int64_t jf_int_text_value(std::string_view code) {
    const std::string_view bare = text::trim(code, "()");
    return text::parse_int<std::int64_t>(bare).value_or(0);
}

bool jf_word_char(char c) { return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_'; }

// Is this Java text a statement expression (an assignment, ++/--, a call, new)?
bool jf_java_statement(const std::string& text) {
    // a call: a dotted name, then a parenthesised argument list that is closed only by the last character
    std::size_t i = 0;
    while (i < text.size() && (jf_word_char(text[i]) || text[i] == '.')) ++i;
    if (i > 0 && i < text.size() && text[i] == '(' && text.back() == ')' && text.size() > i + 1) {
        int depth = 0;
        bool balanced = true;
        for (std::size_t k = 0; k < text.size(); ++k) {
            if (text[k] == '(') ++depth;
            if (text[k] == ')') --depth;
            if (depth == 0 && text[k] == ')' && k != text.size() - 1) balanced = false;
        }
        if (balanced) return true;
    }
    auto lvalue_char = [](char c) { return jf_word_char(c) || c == '.' || c == '[' || c == ']'; };
    auto all_lvalue = [&](std::string_view s) { return !s.empty() && std::all_of(s.begin(), s.end(), lvalue_char); };
    if (text.starts_with("++") || text.starts_with("--")) {
        if (all_lvalue(std::string_view(text).substr(2))) return true;
    }
    if (text.ends_with("++") || text.ends_with("--")) {
        if (all_lvalue(std::string_view(text).substr(0, text.size() - 2))) return true;
    }
    // `target op= value`
    const std::size_t space = text.find(' ');
    if (space != std::string::npos && all_lvalue(std::string_view(text).substr(0, space))) {
        const std::size_t op_end = text.find(' ', space + 1);
        if (op_end != std::string::npos) {
            static const std::unordered_set<std::string_view> ops = {"=", "+=", "-=", "*=", "/=", "%=", "&=", "|=", "^=", "<<=", ">>=", ">>>="};
            if (ops.contains(std::string_view(text).substr(space + 1, op_end - space - 1))) return true;
        }
    }
    return false;
}

std::optional<std::int64_t> jf_literal_int(const ir::Lit& lit) {
    if (const auto* i = std::get_if<std::int64_t>(&lit.value)) return *i;
    if (const auto* u = std::get_if<std::uint64_t>(&lit.value)) return static_cast<std::int64_t>(*u);
    return std::nullopt;
}

std::string jf_zero_of(std::string_view jt) {
    if (jt == "boolean") return "false";
    if (jt == "float") return "0.0f";
    if (jt == "double") return "0.0";
    if (jt == "long") return "0L";
    return "0";
}

// A C float literal's text where the front end left it as text (L2 keeps infinity and NaN so).
std::optional<double> jf_float_from_text(std::string_view text) {
    const std::string t = text::lower(text);
    if (t == "nan" || t == "+nan" || t == "-nan") return std::nan("");
    if (t == "inf" || t == "+inf" || t == "infinity" || t == "+infinity") return HUGE_VAL;
    if (t == "-inf" || t == "-infinity") return -HUGE_VAL;
    try {
        std::size_t used = 0;
        const double v = std::stod(std::string(text), &used);
        if (used == text.size()) return v;
    } catch (const std::exception&) {
    }
    return std::nullopt;
}

}  // namespace

// ---- the frame ------------------------------------------------------------------------------------------------------------

FnWriter::FnWriter(Faithful& program, const ir::MethodDecl* f) : p(program), fn(f) {
    ret_struct = f != nullptr && f->ret && is_record(f->ret);
}

FnWriter::~FnWriter() = default;

std::string FnWriter::alloc(const ir::TypeP& type, std::optional<std::int64_t> size, std::optional<std::int64_t> align) {
    const std::int64_t sz = size ? *size : p.size(type);
    const std::int64_t al = std::max<std::int64_t>(align ? *align : p.align(type), 1);
    frame = (frame + al - 1) / al * al;
    const std::int64_t off = frame;
    frame += sz;
    has_frame = true;
    return off == 0 ? "frame" : "frame + " + std::to_string(off);
}

std::string FnWriter::temp(const ir::TypeP& type) { return alloc(type); }

std::string FnWriter::fresh(const std::string& base) {
    ++tmp;
    std::string n = base + std::to_string(tmp);
    while (declared.contains(n) || p.used_names.contains(n)) {
        ++tmp;
        n = base + std::to_string(tmp);
    }
    declared.insert(n);
    return n;
}

// ---- the function -----------------------------------------------------------------------------------------------------------

std::vector<std::string> FnWriter::write() {
    const ir::MethodDecl& f = *fn;
    const ir::Sym& s = *f.sym;
    const Sig sig = p.fn_sig(s);
    const std::string& ret_type = sig.ret;
    JLines params;
    std::vector<std::pair<std::string, std::string>> pre;
    if (ret_struct) {
        params.push_back("int result");
        declared.insert("result");
    }
    for (const ir::Node* pn : f.params) {
        const ir::Param* prm = ir::get<ir::Param>(pn);
        const std::string pname = jname(prm->sym->name);
        declared.insert(pname);
        const ir::TypeP t = prm->type ? prm->type : prm->sym->type;
        if (is_record(t)) {
            // a struct passed by value arrives as the address of the caller's copy
            vars[prm->sym->name] = Var{pname, true, t, VarKind::Param};
            params.push_back("int " + pname);
        } else if (p.addr_taken.contains(key_of(*prm->sym))) {
            const std::string arg = pname + "Arg";
            declared.insert(arg);
            params.push_back(p.jt(t) + " " + arg);
            vars[prm->sym->name] = Var{pname, true, t, VarKind::Param};
            pre.emplace_back("int " + pname + " = " + alloc(t) + ";", "Mem.set" + p.access(t) + "(" + pname + ", " + arg + ");");
        } else {
            vars[prm->sym->name] = Var{pname, false, t, VarKind::Param};
            params.push_back(p.jt(t) + " " + pname);
        }
    }
    JLines body;
    if (s.varargs) {
        // the arguments past the last named one: 8-byte slots in memory, as MSVC's va_list reads them
        params.push_back("Object... va");
        body.push_back("int vaList = Mem.varargs(va);");
        declared.insert("va");
        declared.insert("vaList");
        has_frame = true;
    }
    for (const auto& [decl, store_text] : pre) {
        body.push_back(decl);
        body.push_back(store_text);
    }
    block_stmts(jf_body_statements(f.body), body);
    if (ret_type != "void" && can_complete(f.body)) {
        const std::string fname = f.name.value_or("");
        body.push_back(fname == "main" ? "return 0;" : "return " + std::string(ret_type == "boolean" ? "false" : "0") + ";  // (C: the function ends without a value)");
    }
    const std::string head = "    static " + ret_type + " " + jname(f.name.value_or("")) + "(" + text::join(params, ", ") + ") {";
    JLines out;
    if (has_frame) {
        const std::int64_t size = (frame + 15) / 16 * 16;
        out = {head, "        int frame = Mem.enter(" + std::to_string(size) + ");", "        try {"};
        jf_append(out, jf_indented_text(body, "            "));
        jf_append(out, {"        } finally {", "            Mem.leave(frame);", "        }", "    }"});
    } else {
        out = {head};
        jf_append(out, jf_indented_text(body, "        "));
        out.push_back("    }");
    }
    return out;
}

// ---- statements -------------------------------------------------------------------------------------------------------------

namespace {

std::string jf_fn_name(const ir::MethodDecl* fn) { return fn != nullptr ? fn->name.value_or("") : "an initialiser"; }

}  // namespace

std::vector<ir::Node*> FnWriter::structure(std::vector<ir::Node*> stmts) {
    const auto has_label = [](const std::vector<ir::Node*>& list) {
        return std::any_of(list.begin(), list.end(), [](ir::Node* s) { return ir::is<ir::Labeled>(s); });
    };
    if (!has_label(stmts)) return stmts;
    Arena& arena = p.arena();
    while (true) {
        std::vector<std::size_t> labs;
        for (std::size_t i = 0; i < stmts.size(); ++i) {
            if (ir::is<ir::Labeled>(stmts[i]) && !labels_done.contains(stmts[i])) labs.push_back(i);
        }
        if (labs.empty()) return stmts;
        const std::size_t j = labs.back();
        ir::Node* lab_node = stmts[j];
        const ir::Labeled& lab = *ir::get<ir::Labeled>(lab_node);
        labels_done.insert(lab_node);
        const std::string label = lab.label.value_or("");
        // the statements that hold a goto to this label not yet placed
        std::vector<std::size_t> where;
        for (std::size_t i = 0; i < stmts.size(); ++i) {
            for (ir::Node* n : ir::walk(stmts[i])) {
                const ir::Goto* g = ir::get<ir::Goto>(n);
                if (g != nullptr && g->label.value_or("") == label && !goto_map.contains(n)) {
                    where.push_back(i);
                    break;
                }
            }
        }
        std::vector<std::size_t> fwd;
        std::vector<std::size_t> back;
        for (const std::size_t i : where) (i < j ? fwd : back).push_back(i);
        const std::string jl = jname(label);
        ir::Node* labelled = jf_one(arena, lab.body);
        const auto map_gotos = [&](std::size_t i, const std::string& how) {
            for (ir::Node* n : ir::walk(stmts[i])) {
                const ir::Goto* g = ir::get<ir::Goto>(n);
                if (g != nullptr && g->label.value_or("") == label) goto_map[n] = how + jl;
            }
        };
        if (!fwd.empty() && !back.empty()) throw Refused("goto " + label + " both forward and backward in " + jf_fn_name(fn));
        if (!fwd.empty()) {
            const std::size_t a = *std::min_element(fwd.begin(), fwd.end());
            for (const std::size_t i : fwd) map_gotos(i, "break ");
            ir::Node* inside = ir::make(arena, ir::Block{.stmts = std::vector<ir::Node*>(stmts.begin() + static_cast<std::ptrdiff_t>(a), stmts.begin() + static_cast<std::ptrdiff_t>(j))});
            ir::Node* block = ir::make(arena, ir::Labeled{.label = label, .body = inside});
            labels_done.insert(block);
            std::vector<ir::Node*> next(stmts.begin(), stmts.begin() + static_cast<std::ptrdiff_t>(a));
            next.push_back(block);
            next.push_back(labelled);
            next.insert(next.end(), stmts.begin() + static_cast<std::ptrdiff_t>(j) + 1, stmts.end());
            stmts = std::move(next);
        } else if (!back.empty()) {
            const std::size_t b = *std::max_element(back.begin(), back.end());
            std::vector<ir::Node*> region = {labelled};
            region.insert(region.end(), stmts.begin() + static_cast<std::ptrdiff_t>(j) + 1, stmts.begin() + static_cast<std::ptrdiff_t>(b) + 1);
            if (std::any_of(region.begin(), region.end(), [&](ir::Node* s) { return loose_jump(s); })) {
                throw Refused("a break or continue inside the loop goto " + label + " makes in " + jf_fn_name(fn));
            }
            for (const std::size_t i : back) map_gotos(i, "continue ");
            region.push_back(ir::make(arena, ir::Break{}));
            ir::Node* one = ir::make(arena, ir::Lit{.value = std::int64_t{1}}, ir::i32_type());
            ir::Node* loop_body = ir::make(arena, ir::Block{.stmts = std::move(region)});
            ir::Node* forever = ir::make(arena, ir::While{.cond = one, .body = loop_body});
            ir::Node* loop = ir::make(arena, ir::Labeled{.label = label, .body = forever});
            labels_done.insert(loop);
            std::vector<ir::Node*> next(stmts.begin(), stmts.begin() + static_cast<std::ptrdiff_t>(j));
            next.push_back(loop);
            next.insert(next.end(), stmts.begin() + static_cast<std::ptrdiff_t>(b) + 1, stmts.end());
            stmts = std::move(next);
        } else {
            std::vector<ir::Node*> next(stmts.begin(), stmts.begin() + static_cast<std::ptrdiff_t>(j));
            next.push_back(labelled);
            next.insert(next.end(), stmts.begin() + static_cast<std::ptrdiff_t>(j) + 1, stmts.end());
            stmts = std::move(next);
        }
    }
}

bool FnWriter::loose_jump(ir::Node* s) {
    if (const ir::Break* b = ir::get<ir::Break>(s); b != nullptr && b->label.value_or("").empty()) return true;
    if (const ir::Continue* c = ir::get<ir::Continue>(s); c != nullptr && c->label.value_or("").empty()) return true;
    if (jf_is_loop_or_switch(s)) return false;
    for (ir::Node* c : ir::children(*s)) {
        if (ir::category(*c) == ir::Category::Statement && loose_jump(c)) return true;
    }
    return false;
}

void FnWriter::block_stmts(std::span<ir::Node* const> stmts, std::vector<std::string>& out) {
    const std::vector<ir::Node*> arranged = structure(std::vector<ir::Node*>(stmts.begin(), stmts.end()));
    for (ir::Node* s : arranged) {
        stmt(s, out);
        if (abrupt(s)) break;   // (Java rejects what follows a jump in a block)
    }
}

bool FnWriter::abrupt(ir::Node* s) {
    if (ir::is<ir::Return>(s) || ir::is<ir::Break>(s) || ir::is<ir::Continue>(s) || ir::is<ir::Throw>(s) || ir::is<ir::Goto>(s)) return true;
    if (const ir::Block* b = ir::get<ir::Block>(s)) {
        return std::any_of(b->stmts.begin(), b->stmts.end(), [&](ir::Node* x) { return abrupt(x); });
    }
    if (ir::is<ir::While>(s) || ir::is<ir::For>(s) || ir::is<ir::DoWhile>(s)) return !can_complete(s);
    return false;
}

bool FnWriter::can_complete(ir::Node* s) {
    if (s == nullptr) return true;
    if (ir::is<ir::Return>(s) || ir::is<ir::Break>(s) || ir::is<ir::Continue>(s) || ir::is<ir::Throw>(s) || ir::is<ir::Goto>(s)) return false;
    if (const ir::Block* b = ir::get<ir::Block>(s)) {
        return std::all_of(b->stmts.begin(), b->stmts.end(), [&](ir::Node* x) { return can_complete(x); });
    }
    if (const ir::If* i = ir::get<ir::If>(s)) {
        if (!ir::present(i->orelse)) return true;
        return can_complete(i->then) || can_complete(i->orelse);
    }
    if (const ir::While* w = ir::get<ir::While>(s)) {
        if (w->cond == nullptr || const_true(w->cond)) return has_break(w->body);
        return true;
    }
    if (const ir::For* f = ir::get<ir::For>(s)) {
        if (f->cond == nullptr || const_true(f->cond)) return has_break(f->body);
        return true;
    }
    if (const ir::DoWhile* d = ir::get<ir::DoWhile>(s)) {
        if (const_true(d->cond)) return has_break(d->body);
        return true;
    }
    if (const ir::Switch* sw = ir::get<ir::Switch>(s)) {
        // Java: a switch completes unless it has a default, no break leaves it, and its last group cannot
        bool has_default = false;
        for (const ir::Node* c : sw->cases) {
            const ir::Case* cc = ir::get<ir::Case>(c);
            has_default = has_default || (cc != nullptr && cc->is_default);
        }
        if (!has_default || sw->cases.empty()) return true;
        for (ir::Node* c : sw->cases) {
            const ir::Case* cc = ir::get<ir::Case>(c);
            if (cc != nullptr && std::any_of(cc->body.begin(), cc->body.end(), [&](ir::Node* x) { return has_break(x); })) return true;
        }
        const ir::Case* last = ir::get<ir::Case>(sw->cases.back());
        return last == nullptr || std::all_of(last->body.begin(), last->body.end(), [&](ir::Node* x) { return can_complete(x); });
    }
    if (const ir::Labeled* l = ir::get<ir::Labeled>(s)) {
        const std::string brk = "break " + jname(l->label.value_or(""));
        for (ir::Node* x : ir::statements(l->body)) {
            for (ir::Node* n : ir::walk(x)) {
                if (!ir::is<ir::Goto>(n)) continue;
                const auto it = goto_map.find(n);
                if (it != goto_map.end() && it->second == brk) return true;
            }
        }
        return can_complete(l->body);
    }
    return true;
}

bool FnWriter::can_complete(const ir::Suite& s) {
    if (const auto* one = std::get_if<ir::Node*>(&s)) return can_complete(*one);
    const auto& list = std::get<std::vector<ir::Node*>>(s);
    return std::all_of(list.begin(), list.end(), [&](ir::Node* x) { return can_complete(x); });
}

bool FnWriter::const_true(ir::Node* c) {
    c = strip(c);
    if (const ir::Cast* cast = ir::get<ir::Cast>(c)) c = strip(cast->x);
    const ir::Lit* lit = ir::get<ir::Lit>(c);
    if (lit == nullptr) return false;
    if (const bool* b = std::get_if<bool>(&lit->value)) return *b;
    if (const auto v = jf_literal_int(*lit)) return *v != 0;
    return false;
}

bool FnWriter::has_break(ir::Node* n) {
    if (ir::is<ir::Break>(n)) return true;
    if (jf_is_loop_or_switch(n)) return false;
    for (ir::Node* c : ir::children(*n)) {
        if (ir::category(*c) == ir::Category::Statement && has_break(c)) return true;
    }
    return false;
}

bool FnWriter::has_break(const ir::Suite& body) {
    for (ir::Node* x : ir::statements(body)) {
        if (has_break(x)) return true;
    }
    return false;
}

std::vector<std::string> FnWriter::sub(const ir::Suite& s) {
    JLines out;
    if (const auto* one = std::get_if<ir::Node*>(&s); one != nullptr && !ir::is<ir::Block>(*one)) {
        stmt(*one, out);
    } else {
        block_stmts(jf_body_statements(s), out);
    }
    return jf_indented(out);
}

void FnWriter::stmt(ir::Node* s, std::vector<std::string>& out) {
    Arena& arena = p.arena();
    if (const ir::Block* b = ir::get<ir::Block>(s)) {
        JLines inner;
        block_stmts(b->stmts, inner);
        out.push_back("{");
        jf_append(out, jf_indented(inner));
        out.push_back("}");
    } else if (const ir::Local* l = ir::get<ir::Local>(s)) {
        local(*l, out);
    } else if (const ir::ExprStmt* es = ir::get<ir::ExprStmt>(s)) {
        expr_stmt(es->expr, out);
    } else if (const ir::Return* r = ir::get<ir::Return>(s)) {
        ret(*r, out);
    } else if (const ir::If* i = ir::get<ir::If>(s)) {
        out.push_back("if (" + cond(i->cond).code + ") {");
        jf_append(out, sub(i->then));
        ir::Suite e = i->orelse;
        while (true) {
            const auto* one = std::get_if<ir::Node*>(&e);
            const ir::If* chained = one != nullptr ? ir::get<ir::If>(*one) : nullptr;
            if (chained == nullptr) break;
            out.push_back("} else if (" + cond(chained->cond).code + ") {");
            jf_append(out, sub(chained->then));
            e = chained->orelse;
        }
        if (ir::present(e)) {
            out.push_back("} else {");
            jf_append(out, sub(e));
        }
        out.push_back("}");
    } else if (const ir::While* w = ir::get<ir::While>(s)) {
        out.push_back("while (" + loop_cond(w->cond) + ") {");
        jf_append(out, sub(w->body));
        out.push_back("}");
    } else if (const ir::DoWhile* d = ir::get<ir::DoWhile>(s)) {
        out.push_back("do {");
        jf_append(out, sub(d->body));
        out.push_back("} while (" + loop_cond(d->cond) + ");");
    } else if (const ir::For* f = ir::get<ir::For>(s)) {
        for_(*f, out);
    } else if (const ir::Switch* sw = ir::get<ir::Switch>(s)) {
        switch_(*sw, out);
    } else if (const ir::Break* brk = ir::get<ir::Break>(s)) {
        out.push_back(!brk->label.value_or("").empty() ? "break " + jname(*brk->label) + ";" : "break;");
    } else if (const ir::Continue* c = ir::get<ir::Continue>(s)) {
        out.push_back(!c->label.value_or("").empty() ? "continue " + jname(*c->label) + ";" : "continue;");
    } else if (ir::is<ir::Empty>(s)) {
        // nothing
    } else if (const ir::Goto* g = ir::get<ir::Goto>(s)) {
        const auto it = goto_map.find(s);
        if (it == goto_map.end()) throw Refused("goto " + g->label.value_or("") + " in " + jf_fn_name(fn) + ": not a jump that nests");
        out.push_back(it->second + ";");
    } else if (const ir::Labeled* lab = ir::get<ir::Labeled>(s)) {
        if (!labels_done.contains(s)) throw Refused("a label (" + lab->label.value_or("") + ") for goto in " + jf_fn_name(fn) + ": not one that nests");
        JLines inner;
        stmt(jf_one(arena, lab->body), inner);
        if (!inner.empty()) inner[0] = jname(lab->label.value_or("")) + ": " + inner[0];
        jf_append(out, inner);
    } else {
        throw Refused("statement " + std::string(ir::kind_name(*s)));
    }
}

std::string FnWriter::loop_cond(ir::Node* c) {
    if (c == nullptr || const_true(c)) return "true";
    return cond(c).code;
}

void FnWriter::for_(const ir::For& s, std::vector<std::string>& out) {
    std::vector<JLines> inits;
    bool simple = true;
    for (ir::Node* i : s.init) {
        if (const ir::Local* l = ir::get<ir::Local>(i)) {
            JLines lines;
            local(*l, lines);
            if (lines.size() != 1 || !jf_ends_with(lines[0], ";")) simple = false;
            inits.push_back(std::move(lines));
        } else if (const ir::ExprStmt* es = ir::get<ir::ExprStmt>(i)) {
            inits.push_back({expr_text(es->expr) + ";"});
        } else {
            simple = false;
        }
    }
    static const std::unordered_set<std::string_view> decl_words = {"int", "long", "byte", "short", "char", "float", "double", "boolean"};
    std::size_t decl_count = 0;
    for (const JLines& l : inits) {
        if (!l.empty() && decl_words.contains(std::string_view(l[0]).substr(0, l[0].find(' ')))) ++decl_count;
    }
    JLines updates;
    for (ir::Node* u : s.update) updates.push_back(expr_text(u));
    const bool all_single = std::all_of(inits.begin(), inits.end(), [](const JLines& l) { return l.size() == 1; });
    const auto cond_text = [&]() -> std::string {
        if (s.cond == nullptr || const_true(s.cond)) return "";
        return loop_cond(s.cond);
    };
    const auto without_semicolon = [](const std::string& line) { return line.substr(0, line.size() - 1); };
    if (simple && all_single && ((decl_count == inits.size() && inits.size() <= 1) || decl_count == 0)) {
        std::string init_text;
        if (decl_count != 0) {
            init_text = without_semicolon(inits[0][0]);
        } else {
            for (std::size_t k = 0; k < inits.size(); ++k) init_text += (k != 0 ? ", " : "") + without_semicolon(inits[k][0]);
        }
        out.push_back("for (" + init_text + "; " + cond_text() + "; " + text::join(updates, ", ") + ") {");
        jf_append(out, sub(s.body));
        out.push_back("}");
        return;
    }
    // an initialiser Java's for cannot hold: before the loop, in a block of its own
    JLines block;
    for (const JLines& l : inits) jf_append(block, l);
    block.push_back("for (; " + cond_text() + "; " + text::join(updates, ", ") + ") {");
    jf_append(block, sub(s.body));
    block.push_back("}");
    out.push_back("{");
    jf_append(out, jf_indented(block));
    out.push_back("}");
}

void FnWriter::switch_(const ir::Switch& s, std::vector<std::string>& out) {
    const Ex subject = value(s.expr);
    const CKind sk = kind_of(s.expr->type);
    if (sk == CKind::I64 || sk == CKind::U64) throw Refused("a switch on a 64-bit value");
    out.push_back("switch (" + subject.code + ") {");
    const std::string jt = (subject.jt == "int" || subject.jt == "char" || subject.jt == "byte" || subject.jt == "short") ? subject.jt : "int";
    for (ir::Node* cn : s.cases) {
        const ir::Case& c = *ir::get<ir::Case>(cn);
        JLines heads;
        for (ir::Node* label : c.labels) {
            const ir::Lit* lit = ir::get<ir::Lit>(strip(label));
            if (lit == nullptr) throw Refused("a case label that is not a constant");
            std::int64_t v = 0;
            if (const auto i = jf_literal_int(*lit)) v = *i;
            else if (const bool* b = std::get_if<bool>(&lit->value)) v = *b ? 1 : 0;
            else if (const double* d = std::get_if<double>(&lit->value)) v = static_cast<std::int64_t>(*d);
            else throw Refused("a case label that is not a constant");
            heads.push_back("case " + int_lit(v, jt).code + ":");
        }
        if (c.is_default) heads.push_back("default:");
        JLines body;
        block_stmts(c.body, body);
        out.push_back("    " + text::join(heads, " "));
        jf_append(out, jf_indented(body, "        "));
    }
    out.push_back("}");
}

void FnWriter::local(const ir::Local& s, std::vector<std::string>& out) {
    std::string lname = jname(s.sym->name);
    if (declared.contains(lname)) lname = fresh(lname + "_");
    declared.insert(lname);
    const ir::TypeP& t = s.type;
    if (std::find(s.mods.begin(), s.mods.end(), "extern") != s.mods.end()) return;
    if (std::find(s.mods.begin(), s.mods.end(), "static") != s.mods.end()) {
        static_local(s, lname);
        return;
    }
    const CKind k = kind_of(t);
    const bool mem = k == CKind::Array || k == CKind::Class || k == CKind::Record || p.addr_taken.contains(key_of(*s.sym));
    const Var v{lname, mem, t, VarKind::Local};
    if (mem) {
        const std::string addr = alloc(t);
        out.push_back("int " + lname + " = " + addr + ";  // " + ctype_text(t, s.sym->name));
        vars[s.sym->name] = v;
        if (s.init != nullptr) jf_append(out, init_place(lname, t, s.init, true));
        return;
    }
    vars[s.sym->name] = v;
    const std::string jt = p.jt(t);
    if (s.init == nullptr) {
        out.push_back(jt + " " + lname + " = " + zero(jt) + ";");
    } else {
        const Ex val = conv(value(s.init), s.init->type, t);
        out.push_back(jt + " " + lname + " = " + val.code + ";");
    }
}

void FnWriter::static_local(const ir::Local& s, const std::string&) {
    ir::FieldDecl g;
    g.sym = ir::make_sym(ir::SymKind::Global, jf_fn_name(fn) + "_" + s.sym->name, std::nullopt, true, s.type);
    g.type = s.type;
    g.init = s.init;
    g.mods = {"static"};
    if (p.addr_taken.contains(key_of(*s.sym))) p.addr_taken.insert(VarKey{true, "", g.sym->name});
    p.declare_global(g);
    p.init_global(g);
    vars[s.sym->name] = p.globals.at(g.sym->name);
}

std::string FnWriter::zero(std::string_view jt) const { return jf_zero_of(jt); }

std::vector<std::string> FnWriter::init_place(const std::string& addr, const ir::TypeP& t, ir::Node* init_in, bool) {
    ir::Node* init = strip(init_in);
    JLines out;
    const ir::Lit* lit = ir::get<ir::Lit>(init);
    const std::string* text = lit != nullptr ? std::get_if<std::string>(&lit->value) : nullptr;
    if (text != nullptr && kind_of(t) == CKind::Array) {
        const auto& arr = std::get<ir::ArrayType>(t->body);
        const std::uint64_t n = arr.count ? *arr.count : char_count(*text) + 1;
        out.push_back("Mem.setString(" + addr + ", " + java_string(*text) + ", " + std::to_string(n) + ");");
        return out;
    }
    if (const ir::NewArray* na = ir::get<ir::NewArray>(init); na != nullptr && kind_of(t) == CKind::Array) {
        const auto& arr = std::get<ir::ArrayType>(t->body);
        const std::vector<ir::Node*>& items = na->init;
        const std::uint64_t n = arr.count ? *arr.count : items.size();
        if (const auto leaves = flat_leaves(items, arr.elem)) {
            const ir::TypeP& et = leaves->second;
            const std::int64_t total = p.size(t) / std::max<std::int64_t>(p.size(et), 1);
            if (static_cast<std::int64_t>(leaves->first.size()) < total) out.push_back("Mem.fill(" + addr + ", 0, " + std::to_string(p.size(t)) + ");");
            if (!leaves->first.empty()) {
                std::vector<std::string> vals;
                for (ir::Node* x : leaves->first) {
                    if (const auto small = small_lit(x, et)) vals.push_back(*small);
                    else vals.push_back(conv(value(x), x->type, et).code);
                }
                jf_append(out, wrap_call("Mem.set" + p.access(et) + "s(" + addr + ", ", vals, ");"));
            }
            return out;
        }
        const std::int64_t esz = p.size(arr.elem);
        if (items.size() < n) out.push_back("Mem.fill(" + addr + ", 0, " + std::to_string(p.size(t)) + ");");
        for (std::size_t i = 0; i < items.size(); ++i) {
            jf_append(out, init_place(offset(addr, static_cast<std::int64_t>(i) * esz), arr.elem, items[i]));
        }
        return out;
    }
    if (const ir::New* nw = ir::get<ir::New>(init); nw != nullptr && is_record(t) && nw->sym != nullptr && nw->sym->name.starts_with("<aggregate")) {
        const ir::RecordType& lay = p.layout(t);
        std::vector<const ir::RecordField*> fields;
        for (const ir::RecordField& f : lay.fields) fields.push_back(&f);
        const std::string ln = p.layout_names.at(p.rec_name(t));
        if (nw->sym->name.starts_with("<aggregate:")) {
            const std::string fname = nw->sym->name.substr(std::string_view("<aggregate:").size(), nw->sym->name.size() - std::string_view("<aggregate:").size() - 1);
            std::erase_if(fields, [&](const ir::RecordField* f) { return f->name != fname; });
        }
        if (nw->args.size() < fields.size() || lay.role == ir::RecordRole::Union) out.push_back("Mem.fill(" + addr + ", 0, " + ln + ".SIZE);");
        for (std::size_t i = 0; i < std::min(fields.size(), nw->args.size()); ++i) {
            jf_append(out, init_place(field_addr(addr, ln, *fields[i]), fields[i]->type, nw->args[i]));
        }
        return out;
    }
    if (is_record(t)) {
        out.push_back("Mem.copy(" + addr + ", " + value(init).code + ", " + p.size_code(t) + ");");
        return out;
    }
    const Ex val = conv(value(init), init->type, t);
    out.push_back("Mem.set" + p.access(t) + "(" + addr + ", " + val.code + ");");
    return out;
}

std::optional<std::string> FnWriter::small_lit(ir::Node* x, const ir::TypeP& et) {
    x = strip(x);
    if (const ir::Cast* c = ir::get<ir::Cast>(x); c != nullptr && c->implicit) x = strip(c->x);
    const ir::Lit* lit = ir::get<ir::Lit>(x);
    if (lit == nullptr || !is_int(et)) return std::nullopt;
    const auto value_opt = jf_literal_int(*lit);
    if (!value_opt) return std::nullopt;
    const std::int64_t v = *value_opt;
    const std::string jt = p.jt(et);
    if (jt == "long") return int_lit(v, "long").code;
    if (lit->text && lit->text->starts_with("'") && v >= 0 && v < 128 && java_char_text(*lit->text)) {
        return *lit->text != "'\\0'" ? *lit->text : std::string("0");
    }
    if (is_unsigned(et) || ((jt == "byte" || jt == "short") && v >= 0)) {
        const int bits = jt == "byte" ? 8 : jt == "short" || jt == "char" ? 16 : 32;
        const std::uint64_t masked = static_cast<std::uint64_t>(v) & ((std::uint64_t{1} << bits) - 1);
        if (jt == "byte" && masked > 9) return std::format("0x{:02x}", masked);
        return std::to_string(masked);
    }
    return int_lit(v, "int").code;
}

std::optional<std::pair<std::vector<ir::Node*>, ir::TypeP>> FnWriter::flat_leaves(std::span<ir::Node* const> items, const ir::TypeP& et) {
    if (kind_of(et) == CKind::Array) {
        const auto& arr = std::get<ir::ArrayType>(et->body);
        std::vector<ir::Node*> rows;
        for (ir::Node* original : items) {
            ir::Node* x = strip(original);
            const ir::NewArray* na = ir::get<ir::NewArray>(x);
            if (na == nullptr) return std::nullopt;
            const auto got = flat_leaves(na->init, arr.elem);
            if (!got || !arr.count) return std::nullopt;
            const std::int64_t per = p.size(et) / std::max<std::int64_t>(p.size(got->second), 1);
            if (static_cast<std::int64_t>(got->first.size()) != per && x != items.back()) return std::nullopt;
            rows.insert(rows.end(), got->first.begin(), got->first.end());
        }
        ir::TypeP scalar = et;
        while (kind_of(scalar) == CKind::Array) scalar = std::get<ir::ArrayType>(scalar->body).elem;
        return std::pair<std::vector<ir::Node*>, ir::TypeP>{std::move(rows), std::move(scalar)};
    }
    if (is_record(et)) return std::nullopt;
    return std::pair<std::vector<ir::Node*>, ir::TypeP>{std::vector<ir::Node*>(items.begin(), items.end()), et};
}

std::vector<std::string> FnWriter::wrap_call(const std::string& head, const std::vector<std::string>& vals, const std::string& tail, std::size_t width) {
    const std::string line = head + text::join(vals, ", ") + tail;
    if (line.size() <= width) return {line};
    JLines out = {std::string(text::rtrim(head))};
    std::string cur = "        ";
    for (std::size_t i = 0; i < vals.size(); ++i) {
        const std::string piece = vals[i] + (i + 1 < vals.size() ? ", " : "");
        if (cur.size() + piece.size() > width) {
            out.push_back(std::string(text::rtrim(cur)));
            cur = "        ";
        }
        cur += piece;
    }
    out.push_back(std::string(text::rtrim(cur)) + tail);
    return out;
}

std::string FnWriter::offset(const std::string& addr, std::int64_t off) const {
    if (off == 0) return addr;
    if (const auto split = jf_split_plus_number(addr)) return split->first + " + " + std::to_string(split->second + off);
    return addr + " + " + std::to_string(off);
}

std::string FnWriter::field_addr(const std::string& addr, const std::string& layout_name, const ir::RecordField& field) const {
    return addr + " + " + layout_name + "." + (field.name != "SIZE" ? jname(field.name) : std::string("SIZE_"));
}

void FnWriter::ret(const ir::Return& s, std::vector<std::string>& out) {
    const ir::TypeP result = fn != nullptr ? fn->ret : ir::TypeP{};
    if (s.expr == nullptr) {
        if (result && kind_of(result) != CKind::Void) out.push_back("return " + zero(p.jt(result)) + ";  // (C: a return without a value)");
        else out.push_back("return;");
        return;
    }
    if (ret_struct) {
        out.push_back("return Mem.copy(result, " + value(s.expr).code + ", " + p.size_code(result) + ");");
        return;
    }
    if (result && kind_of(result) == CKind::Void) {
        expr_stmt(s.expr, out);
        out.push_back("return;");
        return;
    }
    out.push_back("return " + conv(value(s.expr), s.expr->type, result).code + ";");
}

void FnWriter::expr_stmt(ir::Node* e_in, std::vector<std::string>& out) {
    ir::Node* e = strip(e_in);
    if (const ir::Cast* c = ir::get<ir::Cast>(e); c != nullptr && e->type && kind_of(e->type) == CKind::Void) {
        e = strip(c->x);
        if (ir::is<ir::Name>(e) || ir::is<ir::Lit>(e)) return;
    }
    if (const ir::Binary* b = ir::get<ir::Binary>(e); b != nullptr && b->op == ",") {
        expr_stmt(b->left, out);
        expr_stmt(b->right, out);
        return;
    }
    if (ir::is<ir::Lit>(e) || ir::is<ir::Name>(e)) return;
    if (const ir::Binary* b = ir::get<ir::Binary>(e); b != nullptr && (b->op == "&&" || b->op == "||") && !pure(b->right)) {
        // `a || f();` as a statement (assert's expansion): if (!a) f();
        Ex c = cond(b->left);
        if (b->op == "||") {
            if (jf_ends_with(c.code, " != 0")) c = {c.code.substr(0, c.code.size() - 5) + " == 0", kEq, "boolean"};
            else c = {"!" + c.at(kUnary), kUnary, "boolean"};
        }
        JLines inner;
        expr_stmt(b->right, inner);
        out.push_back("if (" + c.code + ") {");
        jf_append(out, jf_indented(inner));
        out.push_back("}");
        return;
    }
    const auto is_statement_expr = [](ir::Node* x) {
        x = strip(x);
        const ir::Unary* u = ir::get<ir::Unary>(x);
        return ir::is<ir::Assign>(x) || ir::is<ir::Call>(x) || (u != nullptr && (u->op == "++" || u->op == "--"));
    };
    if (const ir::Cond* c = ir::get<ir::Cond>(e); c != nullptr && is_statement_expr(c->then) && is_statement_expr(c->orelse)) {
        out.push_back("if (" + cond(c->cond).code + ") {");
        JLines a;
        JLines b;
        expr_stmt(c->then, a);
        expr_stmt(c->orelse, b);
        jf_append(out, jf_indented(a));
        out.push_back("} else {");
        jf_append(out, jf_indented(b));
        out.push_back("}");
        return;
    }
    if (const ir::Assign* a = ir::get<ir::Assign>(e); a != nullptr && a->op && !pure(a->target)) {
        const Place pl = place(a->target);
        if (pl.mem) {
            const std::string t = fresh("at");
            out.push_back("{");
            out.push_back("    int " + t + " = " + pl.java + ";");
            out.push_back("    " + compound(Place{t, true, pl.type, kAtom}, *e).code + ";");
            out.push_back("}");
            return;
        }
    }
    const std::string text = expr_text(e);
    if (!is_statement_expr(e) && !text.starts_with("Mem.") && !text.starts_with("printf")) {
        out.push_back(jf_java_statement(text) ? text + ";" : "discard(" + text + ");");
        return;
    }
    out.push_back(text + ";");
}

std::string FnWriter::expr_text(ir::Node* e_in) {
    ir::Node* e = strip(e_in);
    if (const ir::Unary* u = ir::get<ir::Unary>(e); u != nullptr && (u->op == "++" || u->op == "--")) return incdec(*e, true).code;
    return value(e).code;
}

// ---- places -------------------------------------------------------------------------------------------------------------------

Var FnWriter::var(const ir::Sym& s) {
    if (s.kind == ir::SymKind::Local || s.kind == ir::SymKind::Param) {
        const auto it = vars.find(s.name);
        if (it != vars.end()) return it->second;
    }
    const auto g = p.globals.find(s.name);
    if (g != p.globals.end()) return g->second;
    throw Refused("unknown variable " + s.name + " (" + std::string(ir::to_text(s.kind)) + ", from " + s.owner.value_or("None") + ")");
}

Place FnWriter::place(ir::Node* e_in) {
    ir::Node* e = strip(e_in);
    if (const ir::Name* n = ir::get<ir::Name>(e)) {
        const Var v = var(*n->sym);
        return {v.java, v.mem, v.type, kAtom};
    }
    if (const ir::Unary* u = ir::get<ir::Unary>(e); u != nullptr && u->op == "*") {
        const Ex x = value(u->x);
        return {x.code, true, e->type, x.prec};
    }
    if (const ir::Index* ix = ir::get<ir::Index>(e)) {
        const Ex base = value(ix->arr);   // (an array decays to its address)
        const ir::TypeP& et = e->type;
        const auto* index_node = std::get_if<ir::Node*>(&ix->index);
        if (index_node == nullptr || *index_node == nullptr) throw Refused("an index with several arguments");
        const Ex idx = conv(value(*index_node), (*index_node)->type, ir::i32_type());
        const std::string scale = p.size_code(et);
        if (is_int_text(idx.code) && !is_record(et)) {
            const std::int64_t n = jf_int_text_value(idx.code);
            return {offset(base.at(kAdd), n * p.size(et)), true, et, kAdd};
        }
        const std::string term = scale == "1" ? idx.at(kMul) : idx.at(kMul) + " * " + scale;
        return {base.at(kAdd) + " + " + term, true, et, kAdd};
    }
    if (const ir::Select* sel = ir::get<ir::Select>(e)) {
        ir::Node* obj = sel->obj;
        const ir::TypeP& ot = obj->type;
        if (!is_record(ot)) throw Refused("a field of a non-struct");
        const Ex base = addr(obj);
        const ir::RecordField& f = p.field_off(ot, sel->sym->name);
        const std::string ln = p.layout_names.at(p.rec_name(ot));
        return {field_addr(base.at(kAdd), ln, f), true, e->type ? e->type : f.type, kAdd};
    }
    if (const ir::Cast* c = ir::get<ir::Cast>(e)) return place(c->x);
    throw Refused("an lvalue " + std::string(ir::kind_name(*e)));
}

bool FnWriter::pure(ir::Node* e) {
    for (ir::Node* n : ir::walk(e)) {
        if (ir::is<ir::Assign>(n) || ir::is<ir::Call>(n)) return false;
        if (const ir::Unary* u = ir::get<ir::Unary>(n); u != nullptr && (u->op == "++" || u->op == "--")) return false;
    }
    return true;
}

Ex FnWriter::addr(ir::Node* e_in) {
    ir::Node* e = strip(e_in);
    if (const ir::Unary* u = ir::get<ir::Unary>(e); u != nullptr && u->op == "*") return value(u->x);
    if ((ir::is<ir::Call>(e) || ir::is<ir::New>(e) || ir::is<ir::Assign>(e) || ir::is<ir::Cond>(e)) && is_record(e->type)) return value(e);
    const Place pl = place(e);
    if (!pl.mem) throw Refused("the address of " + pl.java + ", which is not in memory");
    return {pl.java, pl.prec, "int", false};
}

Ex FnWriter::load(const Place& pl) {
    if (!pl.mem) return {pl.java, kAtom, p.jt(pl.type), false};
    const CKind k = kind_of(pl.type);
    if (k == CKind::Array || k == CKind::Class || k == CKind::Record) return {pl.java, pl.prec, "int", false};
    return {"Mem.get" + p.access(pl.type) + "(" + pl.java + ")", kPost, p.jt(pl.type), false};
}

Ex FnWriter::store(const Place& pl, const Ex& val) {
    if (!pl.mem) return {pl.java + " = " + val.at(kAssign), kAssign, p.jt(pl.type), false};
    if (is_record(pl.type)) return {"Mem.copy(" + pl.java + ", " + val.code + ", " + p.size_code(pl.type) + ")", kPost, "int", false};
    return {"Mem.set" + p.access(pl.type) + "(" + pl.java + ", " + val.code + ")", kPost, p.jt(pl.type), false};
}

// ---- values -------------------------------------------------------------------------------------------------------------------

Ex FnWriter::cond(ir::Node* e_in) {
    ir::Node* e = strip(e_in);
    const Ex j = value(e);
    return truth(j, e->type);
}

Ex FnWriter::truth(const Ex& j, const ir::TypeP&) {
    if (j.jt == "boolean") return j;
    return {j.at(kEq + 1) + " != 0", kEq, "boolean", false};
}

Ex FnWriter::as_value(const Ex& j, const ir::TypeP& type) {
    if (j.jt == "boolean" && kind_of(type) != CKind::Bool) {
        const std::string jt = p.jt(type);
        const bool wide = jt == "long";
        return {j.at(kCond + 1) + " ? " + (wide ? "1L" : "1") + " : " + (wide ? "0L" : "0"), kCond, jt, false};
    }
    return j;
}

Ex FnWriter::value(ir::Node* e) {
    if (e == nullptr) throw Refused("a missing expression");
    return ir::visit(*e, [&](auto& k) -> Ex {
        using K = std::decay_t<decltype(k)>;
        if constexpr (std::is_same_v<K, ir::Paren>) return x_paren(*e);
        else if constexpr (std::is_same_v<K, ir::Unsupported>) return x_unsupported(*e);
        else if constexpr (std::is_same_v<K, ir::Lit>) return x_lit(*e);
        else if constexpr (std::is_same_v<K, ir::Name>) return x_name(*e);
        else if constexpr (std::is_same_v<K, ir::Select>) return x_select(*e);
        else if constexpr (std::is_same_v<K, ir::Index>) return x_index(*e);
        else if constexpr (std::is_same_v<K, ir::TypeName>) return x_type_name(*e);
        else if constexpr (std::is_same_v<K, ir::AddressOf>) return x_address_of(*e);
        else if constexpr (std::is_same_v<K, ir::Unary>) return x_unary(*e);
        else if constexpr (std::is_same_v<K, ir::Binary>) return x_binary(*e);
        else if constexpr (std::is_same_v<K, ir::Assign>) return x_assign(*e);
        else if constexpr (std::is_same_v<K, ir::Cond>) return x_cond(*e);
        else if constexpr (std::is_same_v<K, ir::Cast>) return x_cast(*e);
        else if constexpr (std::is_same_v<K, ir::Call>) return x_call(*e);
        else if constexpr (std::is_same_v<K, ir::New>) return x_new(*e);
        else if constexpr (std::is_same_v<K, ir::NewArray>) return x_new_array(*e);
        else if constexpr (std::is_same_v<K, ir::Concat>) return x_concat(*e);
        else if constexpr (std::is_same_v<K, ir::Length>) return x_length(*e);
        else if constexpr (std::is_same_v<K, ir::This>) return x_this(*e);
        else throw Refused("expression " + std::string(ir::kind_name(*e)));
    });
}

Ex FnWriter::x_paren(ir::Node& e) {
    // the grouping the source chose is kept where it groups an operator (readability, not precedence)
    const Ex j = value(ir::get<ir::Paren>(&e)->x);
    if (j.prec < kUnary && j.jt != "void") return {"(" + j.code + ")", kAtom, j.jt, j.fresh};
    return j;
}

Ex FnWriter::x_unsupported(ir::Node& e) {
    throw Refused(ir::get<ir::Unsupported>(&e)->kind.value_or("?") + " (line " + std::to_string(e.line) + "): the front end did not lower it");
}

Ex FnWriter::x_lit(ir::Node& e) {
    const ir::Lit& lit = *ir::get<ir::Lit>(&e);
    const ir::TypeP& t = e.type;
    if (const std::string* text = std::get_if<std::string>(&lit.value); text != nullptr && is_float(t)) {
        // (L2 keeps infinity and NaN as text)
        const auto v = jf_float_from_text(*text);
        if (!v) throw Refused("a floating-point literal that is not a number: " + *text);
        const std::string jt = p.jt(t);
        return float_lit(*v, jt == "float" || jt == "double" ? jt : "double");
    }
    if (const bool* b = std::get_if<bool>(&lit.value)) {
        if (kind_of(t) == CKind::Bool) return {*b ? "true" : "false", kAtom, "boolean", false};
        return {*b ? "1" : "0", kAtom, "int", false};
    }
    if (std::holds_alternative<std::monostate>(lit.value)) return {"0", kAtom, "int", false};
    if (std::holds_alternative<std::string>(lit.value)) return {std::string("Mem.") + (is_wide(e) ? "wstr" : "str") + "(" + string_lit(e) + ")", kPost, "int", false};
    const std::string jt = t && kind_of(t) != CKind::Other ? p.jt(t) : "int";
    if (const double* d = std::get_if<double>(&lit.value)) return float_lit(*d, jt == "float" || jt == "double" ? jt : "double");
    const std::int64_t iv = *jf_literal_int(lit);
    const std::uint64_t uv = static_cast<std::uint64_t>(iv);
    if (is_float(t)) {
        const double d = std::holds_alternative<std::uint64_t>(lit.value) ? static_cast<double>(uv) : static_cast<double>(iv);
        return float_lit(d, jt == "float" || jt == "double" ? jt : "double");
    }
    if (lit.text && lit.text->starts_with("'") && jt == "int" && java_char_text(*lit.text)) return {*lit.text, kAtom, "char", false};
    return int_lit(iv, jt);
}

Ex FnWriter::x_name(ir::Node& e) {
    const Var v = var(*ir::get<ir::Name>(&e)->sym);
    const CKind k = kind_of(v.type);
    if (v.mem && (k == CKind::Array || k == CKind::Class || k == CKind::Record)) return {v.java, kAtom, "int", false};
    return load(Place{v.java, v.mem, v.type, kAtom});
}

Ex FnWriter::x_select(ir::Node& e) { return load(place(&e)); }
Ex FnWriter::x_index(ir::Node& e) { return load(place(&e)); }
Ex FnWriter::x_type_name(ir::Node&) { throw Refused("a type used as a value"); }

Ex FnWriter::x_address_of(ir::Node& e) { return {p.fn_const(ir::get<ir::AddressOf>(&e)->sym->name), kAtom, "int", false}; }

Ex FnWriter::x_unary(ir::Node& e) {
    const ir::Unary& u = *ir::get<ir::Unary>(&e);
    const std::string& op = u.op;
    if (op == "*") {
        if (kind_of(e.type) == CKind::Fn) return value(u.x);
        return load(place(&e));
    }
    if (op == "&") {
        ir::Node* x = strip(u.x);
        if (ir::is<ir::AddressOf>(x)) return value(x);
        return addr(x);
    }
    if (op == "++" || op == "--") return incdec(e);
    if (op == "!") {
        const Ex c = cond(u.x);
        if (jf_ends_with(c.code, " != 0") && c.prec == kEq) return {c.code.substr(0, c.code.size() - 5) + " == 0", kEq, "boolean", false};
        return {"!" + c.at(kUnary), kUnary, "boolean", false};
    }
    const Ex x = as_value(value(u.x), u.x->type);
    const std::string jt = p.jt(e.type);
    if (op == "-") {
        std::string code = "-" + x.at(kUnary);
        if (code.starts_with("--")) code = "-(" + x.code + ")";
        return narrow({code, kUnary, promoted(x.jt), false}, jt);
    }
    if (op == "+") return x;
    if (op == "~") return narrow({"~" + x.at(kUnary), kUnary, promoted(x.jt), false}, jt);
    throw Refused("unary " + op);
}

Ex FnWriter::narrow(const Ex& j, std::string_view jt) {
    if (j.jt == jt) return j;
    if ((jt == "byte" || jt == "short" || jt == "char") && (j.jt == "int" || j.jt == "long")) return {"(" + std::string(jt) + ") " + j.at(kUnary), kUnary, std::string(jt), false};
    if (jt == "int" && j.jt == "long") return {"(int) " + j.at(kUnary), kUnary, "int", false};
    return j;
}

Ex FnWriter::incdec(ir::Node& e, bool as_statement) {
    const ir::Unary& u = *ir::get<ir::Unary>(&e);
    const ir::TypeP& t = e.type;
    const Place pl = place(u.x);
    std::int64_t delta = 1;
    if (is_ptr(t)) delta = kind_of(t) == CKind::Ptr ? p.size(pointee(t)) : 1;
    const char sign = u.op == "++" ? '+' : '-';
    if (!pl.mem) {
        if (delta == 1 || !is_ptr(t)) {
            if (kind_of(t) == CKind::Bool) throw Refused("++ on a bool");
            return {u.postfix ? pl.java + u.op : u.op + pl.java, u.postfix ? kPost : kUnary, p.jt(t), false};
        }
        const std::string step = pl.java + " " + sign + "= " + std::to_string(delta);
        if (as_statement || !u.postfix) return {step, kAssign, "int", false};
        return {"(" + step + ") " + (sign == '+' ? "-" : "+") + " " + std::to_string(delta), kAdd, "int", false};
    }
    const std::string acc = p.access(t);
    const std::int64_t d = sign == '+' ? delta : -delta;
    const std::string jt = p.jt(t);
    const std::string dl = jt == "long" ? int_lit(d, "long").code : (jt == "float" || jt == "double") ? float_lit(static_cast<double>(d), jt).code : std::to_string(d);
    const std::string fn_name = u.postfix && !as_statement ? "Mem.postAdd" + acc : "Mem.add" + acc;
    return {fn_name + "(" + pl.java + ", " + dl + ")", kPost, jt, false};
}

Ex FnWriter::x_binary(ir::Node& e) {
    const ir::Binary& b = *ir::get<ir::Binary>(&e);
    const std::string& op = b.op;
    if (op == ",") {
        const Ex l = value(b.left);
        const Ex r = value(b.right);
        return {"seq(" + l.code + ", " + r.code + ")", kPost, r.jt, false};
    }
    if (op == "&&" || op == "||") {
        const Ex l = cond(b.left);
        const Ex r = cond(b.right);
        const int prec = *binary_precedence(op);
        return {l.at(prec) + " " + op + " " + r.at(prec + 1), prec, "boolean", false};
    }
    const ir::TypeP& lt = b.left->type;
    const ir::TypeP& rt = b.right->type;
    if (op == "<" || op == "<=" || op == ">" || op == ">=" || op == "==" || op == "!=") return compare(e, op);
    // pointer arithmetic
    if ((op == "+" || op == "-") && is_ptr(lt) && is_int(rt)) return ptr_add(value(b.left), value(b.right), rt, lt, op);
    if (op == "+" && is_int(lt) && is_ptr(rt)) return ptr_add(value(b.right), value(b.left), lt, rt, "+");
    if (op == "-" && is_ptr(lt) && is_ptr(rt)) {
        const std::int64_t size = kind_of(lt) == CKind::Ptr ? p.size(pointee(lt)) : 1;
        const std::string diff = value(b.left).at(kAdd) + " - " + value(b.right).at(kAdd + 1);
        const Ex j{size == 1 ? diff : "(" + diff + ") / " + std::to_string(size), size == 1 ? kAdd : kMul, "int", false};
        return conv(j, ir::i32_type(), e.type);
    }
    const Ex l = as_value(value(b.left), lt);
    const Ex r = as_value(value(b.right), rt);
    return arithmetic(op, l, r, e.type);
}

Ex FnWriter::arithmetic(std::string_view op, const Ex& l, const Ex& r, const ir::TypeP& t) {
    const std::string jt = p.jt(t);
    if (op == "<<" || op == ">>" || op == ">>>") {
        const std::string jop = op == ">>" && is_unsigned(t) ? ">>>" : std::string(op);
        const int prec = kShift;
        Ex rr = r;
        if (r.jt == "long") rr = {"(int) " + r.at(kUnary), kUnary, "int", false};
        return narrow({l.at(prec) + " " + jop + " " + rr.at(prec + 1), prec, promoted(l.jt), false}, jt);
    }
    const CKind tk = kind_of(t);
    if ((op == "/" || op == "%") && (tk == CKind::U32 || tk == CKind::U64)) {
        const std::string cls = tk == CKind::U32 ? "Integer" : "Long";
        return {cls + "." + (op == "/" ? "divideUnsigned" : "remainderUnsigned") + "(" + l.code + ", " + r.code + ")", kPost, jt, false};
    }
    const auto prec = binary_precedence(op);
    if (!prec) throw Refused("binary operator " + std::string(op));
    const Ex res{l.at(*prec) + " " + std::string(op) + " " + r.at(*prec + 1), *prec, promoted2(l.jt, r.jt), false};
    return jt == "byte" || jt == "short" || jt == "char" ? narrow(res, jt) : res;
}

Ex FnWriter::ptr_add(const Ex& base, const Ex& index, const ir::TypeP& index_type, const ir::TypeP& ptr_type, std::string_view op) {
    const std::int64_t size = kind_of(ptr_type) == CKind::Ptr ? p.size(pointee(ptr_type)) : 1;
    const Ex i = conv(index, index_type, ir::i32_type());
    if (is_int_text(i.code)) {
        std::int64_t n = jf_int_text_value(i.code) * size;
        if (op == "-") n = -n;
        if (n == 0) return base;
        return {base.at(kAdd) + (n > 0 ? " + " : " - ") + std::to_string(n > 0 ? n : -n), kAdd, "int", false};
    }
    std::string term = size == 1 ? i.at(op == "-" ? kMul + 1 : kMul) : i.at(kMul) + " * " + std::to_string(size);
    if (size != 1 && op == "-") term = "(" + term + ")";
    return {base.at(kAdd) + " " + std::string(op) + " " + term, kAdd, "int", false};
}

Ex FnWriter::compare(ir::Node& e, std::string_view op) {
    const ir::Binary& b = *ir::get<ir::Binary>(&e);
    const ir::TypeP& lt = b.left->type;
    const ir::TypeP& rt = b.right->type;
    Ex l = as_value(value(b.left), lt);
    Ex r = as_value(value(b.right), rt);
    const int prec = *binary_precedence(op);
    const CKind lk = kind_of(lt);
    if ((op == "<" || op == "<=" || op == ">" || op == ">=") && (lk == CKind::U32 || lk == CKind::U64)) {
        return {std::string(lk == CKind::U32 ? "Integer" : "Long") + ".compareUnsigned(" + l.code + ", " + r.code + ") " + std::string(op) + " 0", kRel, "boolean", false};
    }
    if (l.jt == "boolean" && r.jt == "boolean") return {l.at(prec) + " " + std::string(op) + " " + r.at(prec + 1), prec, "boolean", false};
    if (l.jt == "boolean") l = as_value(l, ir::i32_type());
    if (r.jt == "boolean") r = as_value(r, ir::i32_type());
    return {l.at(prec) + " " + std::string(op) + " " + r.at(prec + 1), prec, "boolean", false};
}

Ex FnWriter::x_assign(ir::Node& e) {
    const ir::Assign& a = *ir::get<ir::Assign>(&e);
    const Place pl = place(a.target);
    if (!a.op) {
        const ir::TypeP& t = a.target->type;
        if (is_record(t)) return {"Mem.copy(" + addr(a.target).code + ", " + value(a.value).code + ", " + p.size_code(t) + ")", kPost, "int", false};
        const Ex val = conv(value(a.value), a.value->type, t);
        return store(pl, val);
    }
    if (pl.mem && !pure(a.target)) throw Refused("a compound assignment through an address with side effects, inside an expression");
    return compound(pl, e);
}

Ex FnWriter::compound(const Place& pl, ir::Node& e) {
    const ir::Assign& a = *ir::get<ir::Assign>(&e);
    const ir::TypeP& t = a.target->type;
    const std::string& op = *a.op;
    const ir::TypeP& vt = a.value->type;
    if (is_ptr(t)) {
        const std::int64_t size = kind_of(t) == CKind::Ptr ? p.size(pointee(t)) : 1;
        const Ex i = conv(value(a.value), vt, ir::i32_type());
        const std::string term = size == 1 ? i.code : i.at(kMul) + " * " + std::to_string(size);
        if (!pl.mem) return {pl.java + " " + op + "= " + term, kAssign, "int", false};
        const std::string operand = size == 1 ? i.at(kMul) : op == "-" ? "(" + term + ")" : term;
        return store(pl, {load(pl).at(kAdd) + " " + op + " " + operand, kAdd, "int", false});
    }
    const std::string jt = p.jt(t);
    const Ex val = as_value(value(a.value), vt);
    const CKind tk = kind_of(t);
    const CKind vk = kind_of(vt);
    const bool simple_op = op == "+" || op == "-" || op == "*" || op == "&" || op == "|" || op == "^" || op == "<<";
    if (!pl.mem && is_int(t) && simple_op && is_int(vt) && (bits_of(vk) <= 32 || jt == "long")) {
        Ex v = val;
        if (op == "<<" && v.jt == "long") v = {"(int) " + v.at(kUnary), kUnary, "int", false};
        return {pl.java + " " + op + "= " + v.at(kAssign), kAssign, jt, false};
    }
    if (!pl.mem && (tk == CKind::I32 || tk == CKind::I64 || tk == CKind::F64 || tk == CKind::F32) && vk == tk && (op == "/" || op == "%" || op == ">>")) {
        return {pl.java + " " + op + "= " + val.at(kAssign), kAssign, jt, false};
    }
    if (!pl.mem && (tk == CKind::U32 || tk == CKind::U64) && vk == tk && op == ">>") return {pl.java + " >>>= " + val.at(kAssign), kAssign, jt, false};
    if (!pl.mem && is_float(t) && is_float(vt) && tk == vk) return {pl.java + " " + op + "= " + val.at(kAssign), kAssign, jt, false};
    // the general form: target = (T) (target op value), in the computation type
    ir::TypeP ct = (op == "<<" || op == ">>") ? promote_type(t) : vt;
    if (is_float(t) && !is_float(vt)) ct = t;
    const Ex cur = conv(load(pl), t, ct);
    const Ex res = arithmetic(op, as_value(cur, ct), as_value(val, vt), ct);
    return store(pl, conv(res, ct, t));
}

Ex FnWriter::x_cond(ir::Node& e) {
    const ir::Cond& c = *ir::get<ir::Cond>(&e);
    const Ex test = cond(c.cond);
    const ir::TypeP& t = e.type;
    if (is_record(t)) {
        const Ex a = value(c.then);
        const Ex b = value(c.orelse);
        return {test.at(kCond + 1) + " ? " + a.at(kCond + 1) + " : " + b.at(kCond), kCond, "int", false};
    }
    const Ex a = conv(value(c.then), c.then->type, t);
    const Ex b = conv(value(c.orelse), c.orelse->type, t);
    if (kind_of(t) == CKind::Void) throw Refused("a conditional expression of type void used as a value");
    const std::string jt = p.jt(t);
    const std::string both = test.at(kCond + 1) + " ? " + a.at(kCond + 1) + " : " + b.at(kCond);
    if (jt == "byte" || jt == "short" || jt == "char") return {"(" + jt + ") (" + both + ")", kUnary, jt, false};
    return {both, kCond, jt, false};
}

Ex FnWriter::x_cast(ir::Node& e) {
    const ir::Cast& c = *ir::get<ir::Cast>(&e);
    ir::Node* x = strip(c.x);
    const ir::TypeP& t = e.type;
    const ir::TypeP& st = x->type;
    if (kind_of(t) == CKind::Void) return value(x);
    if (kind_of(st) == CKind::Array && is_ptr(t)) {
        if (const ir::Lit* lit = ir::get<ir::Lit>(x); lit != nullptr && std::holds_alternative<std::string>(lit->value)) {
            return {std::string("Mem.") + (is_wide(*x) ? "wstr" : "str") + "(" + string_lit(*x) + ")", kPost, "int", false};
        }
        return addr(x);
    }
    if (const ir::Lit* lit = ir::get<ir::Lit>(x); lit != nullptr && std::holds_alternative<std::monostate>(lit->value)) return {"0", kAtom, "int", false};
    if (ir::is<ir::AddressOf>(x)) return value(x);
    const Ex j = value(x);
    return conv(j, st, t);
}

Ex FnWriter::conv(const Ex& j_in, const ir::TypeP& s, const ir::TypeP& t) {
    Ex j = j_in;
    if (!t || !s) return j;
    CKind sk = kind_of(s);
    CKind tk = kind_of(t);
    if (tk == CKind::Void) return j;
    if (tk == CKind::Bool) {
        if (j.jt == "boolean") return j;
        return {j.at(kEq + 1) + " != 0", kEq, "boolean", false};
    }
    if (j.jt == "boolean") {
        j = {j.at(kCond + 1) + " ? 1 : 0", kCond, "int", false};
        sk = CKind::I32;
    }
    const std::string tj = p.jt(t);
    if (sk == CKind::Ptr || sk == CKind::FnPtr || sk == CKind::Array || sk == CKind::Null || sk == CKind::Class || sk == CKind::Record) sk = CKind::I32;   // an address is an int
    if (tk == CKind::Ptr || tk == CKind::FnPtr || tk == CKind::Array || tk == CKind::Class || tk == CKind::Record) tk = CKind::I32;
    if (sk == tk) return j.jt == tj ? j : retype(j, tj);
    const auto is_float_kind = [](CKind k) { return k == CKind::F32 || k == CKind::F64 || k == CKind::F80; };
    if (is_float_kind(tk)) {
        const std::string fj = tk == CKind::F32 ? "float" : "double";
        if (is_float_kind(sk)) {
            if ((kind_of(s) == CKind::F64 || kind_of(s) == CKind::F80) && fj == "double") return j;
            return {"(" + fj + ") " + j.at(kUnary), kUnary, fj, false};
        }
        if (sk == CKind::U32) return {"(" + fj + ") Integer.toUnsignedLong(" + j.code + ")", kUnary, fj, false};
        if (sk == CKind::U64) return {std::string(fj == "float" ? "(float) " : "") + "unsignedToDouble(" + j.code + ")", fj == "float" ? kUnary : kPost, fj, false};
        if (sk == CKind::U8) return {"(" + fj + ") (" + j.code + " & 0xFF)", kUnary, fj, false};
        return {"(" + fj + ") " + j.at(kUnary), kUnary, fj, false};
    }
    // to an integer
    if (is_float_kind(sk)) {
        if (tk == CKind::U32) return {"(int) (long) " + j.at(kUnary), kUnary, "int", false};
        if (tk == CKind::U64) return {"doubleToUnsigned(" + j.code + ")", kPost, "long", false};
        if (tk == CKind::I64) return {"(long) " + j.at(kUnary), kUnary, "long", false};
        if (tk == CKind::I32) return {"(int) " + j.at(kUnary), kUnary, "int", false};
        return {"(" + tj + ") (int) " + j.at(kUnary), kUnary, tj, false};
    }
    const int sb = bits_of(sk);
    const int tb = bits_of(tk);
    if (sb == 0 || tb == 0) throw Refused("a conversion from " + spell(s) + " to " + spell(t));
    if (tb <= sb) {   // narrowing or the same width: the low bits
        if (tj == j.jt) return {j.code, j.prec, tj, false};
        if ((j.jt == "byte" || j.jt == "short" || j.jt == "char") && tj == "int" && tb == sb) return widen(j, sk, tj);
        return {"(" + tj + ") " + j.at(kUnary), kUnary, tj, false};
    }
    // widening: zero-extended when the source is unsigned
    const Ex w = widen(j, sk, tb == 64 ? "long" : "int");
    if ((tj == "byte" || tj == "short" || tj == "char") && w.jt != tj) return {"(" + tj + ") " + w.at(kUnary), kUnary, tj, false};
    return w;
}

Ex FnWriter::widen(const Ex& j, CKind sk, std::string_view to) {
    if (sk == CKind::U8) {
        if (is_int_text(j.code)) return {std::to_string(jf_int_text_value(j.code) & 0xFF), kAtom, std::string(to), false};
        const Ex masked{j.at(kBitAnd) + " & 0xFF", kBitAnd, "int", false};
        return to == "int" ? masked : Ex{"(long) (" + masked.code + ")", kUnary, "long", false};
    }
    if (sk == CKind::U32 && to == "long") return {"Integer.toUnsignedLong(" + j.code + ")", kPost, "long", false};
    if (sk == CKind::U16) {
        return to == "int" ? Ex{j.code, j.prec, std::string(to), false} : Ex{"(long) " + j.at(kUnary), kUnary, "long", false};
    }
    if (to == "long" && j.jt != "long") {
        if (is_int_text(j.code)) return int_lit(jf_int_text_value(j.code), "long");
        return {"(long) " + j.at(kUnary), kUnary, "long", false};
    }
    const bool small = j.jt == "byte" || j.jt == "short" || j.jt == "int" || j.jt == "char";
    return {j.code, j.prec, small ? std::string(to) : j.jt, false};
}

Ex FnWriter::retype(const Ex& j, std::string_view to) {
    if (j.jt == to) return j;
    if ((j.jt == "int" || j.jt == "long") && (to == "byte" || to == "short" || to == "char")) return {"(" + std::string(to) + ") " + j.at(kUnary), kUnary, std::string(to), false};
    return {j.code, j.prec, std::string(to), false};
}

// ---- calls ------------------------------------------------------------------------------------------------------------------------

Ex FnWriter::x_call(ir::Node& e) {
    const ir::Call& c = *ir::get<ir::Call>(&e);
    const std::span<ir::Node* const> call_args = c.args;
    if (c.sym != nullptr && c.target == nullptr) {
        const std::string& name = c.sym->name;
        if (p.functions.contains(name)) return own_call(*p.function_for(*c.sym), jname(name), call_args);
        return lib_call(e, name, call_args);
    }
    ir::Node* target = c.target != nullptr ? strip(c.target) : nullptr;
    // (*fp)(...) calls fp
    while (const ir::Unary* u = ir::get<ir::Unary>(target)) {
        if (u->op != "*" || kind_of(u->x->type) != CKind::FnPtr) break;
        target = strip(u->x);
    }
    if (target == nullptr) throw Refused("a call this back end cannot place");
    ir::TypeP ft;
    std::optional<Ex> fn_text;
    if (kind_of(target->type) == CKind::Fn) {
        fn_text = value(target);
        ft = ir::make_type(ir::FnPtrType{ir::AddressSpace::Flat, target->type});
    } else if (kind_of(target->type) == CKind::FnPtr) {
        ft = target->type;
    } else {
        throw Refused("a call this back end cannot place");
    }
    const ir::FnType* fnt = std::get_if<ir::FnType>(&std::get<ir::FnPtrType>(ft->body).fn->body);
    if (fnt == nullptr) throw Refused("a function pointer without a function type");
    std::vector<ir::TypeP> params;
    for (const ir::FnParam& prm : fnt->params) params.push_back(prm.type);
    if (params.empty() && !call_args.empty()) {   // a K&R pointer `int (*f)()`: typed by the call
        ir::FnType typed{.conv = ir::CallingConvention::Cdecl, .result = fnt->result};
        for (ir::Node* a : call_args) {
            params.push_back(a->type);
            typed.params.push_back(ir::FnParam{.type = a->type});
        }
        ft = ir::make_type(ir::FnPtrType{ir::AddressSpace::Flat, ir::make_type(std::move(typed))});
        fnt = std::get_if<ir::FnType>(&std::get<ir::FnPtrType>(ft->body).fn->body);
    }
    const Sig sig = p.fn_sig_of_type(ft);
    const std::string iface_name = p.iface(sig);
    const Ex fn_value = fn_text ? *fn_text : value(target);
    const std::vector<std::string> ac = args(call_args, params, fnt->result.type);
    return {"((" + iface_name + ") Fn.at(" + fn_value.code + ")).call(" + text::join(ac, ", ") + ")", kPost, sig.ret, false};
}

std::vector<std::string> FnWriter::args(std::span<ir::Node* const> call_args, const std::vector<ir::TypeP>& params, const ir::TypeP& result) {
    JLines out;
    if (result && is_record(result)) out.push_back(temp(result));
    for (std::size_t i = 0; i < call_args.size(); ++i) {
        ir::Node* a = call_args[i];
        const ir::TypeP pt = i < params.size() ? params[i] : a->type;
        if (is_record(pt)) {
            const Ex v = value(a);
            if (v.fresh) out.push_back(v.code);
            else out.push_back("Mem.copy(" + temp(pt) + ", " + v.code + ", " + p.size_code(pt) + ")");
        } else {
            out.push_back(conv(value(a), a->type, pt).code);
        }
    }
    return out;
}

Ex FnWriter::own_call(const ir::MethodDecl& f, const std::string& java_name, std::span<ir::Node* const> call_args) {
    const ir::Sym& s = *f.sym;
    // the definition's own parameter types (a K&R definition's symbol has no prototype)
    std::vector<ir::TypeP> params;
    for (const ir::Node* prm : f.params) {
        const ir::Param* pp = ir::get<ir::Param>(prm);
        params.push_back(pp->type ? pp->type : pp->sym->type);
    }
    const std::vector<std::string> ac = args(call_args, params, s.ret);
    const std::string jt = s.ret ? p.jt(s.ret) : "void";
    return {java_name + "(" + text::join(ac, ", ") + ")", kPost, jt, s.ret && is_record(s.ret)};
}

Ex FnWriter::lib_call(ir::Node& e, std::string_view name, std::span<ir::Node* const> call_args) {
    std::vector<ir::Node*> own_args(call_args.begin(), call_args.end());
    if (name.starts_with("__builtin_va_") && !own_args.empty()) {
        ir::Node* a0 = strip(own_args[0]);   // (the builtins take their va_list by reference)
        const ir::Unary* u = ir::get<ir::Unary>(a0);
        const auto* pt = a0->type ? std::get_if<ir::PtrType>(&a0->type->body) : nullptr;
        if (u != nullptr && u->op == "&" && pt != nullptr && pt->ref) own_args[0] = u->x;
    }
    if (name == "__builtin_va_start") return store(place(own_args.at(0)), {"vaList", kAtom, "int", false});
    if (name == "__builtin_va_end") return {"discard(0)", kPost, "void", false};
    if (name == "__builtin_va_copy") return store(place(own_args.at(0)), value(own_args.at(1)));
    if (name == "__builtin_va_arg") {
        const ir::TypeP& t = e.type;
        if (is_record(t) || kind_of(t) == CKind::F32) throw Refused("va_arg of " + spell(t));
        const Place pl = place(own_args.at(0));
        const std::string slot = !pl.mem ? "(" + pl.java + " += 8) - 8" : "Mem.postAddPtr(" + pl.java + ", 8)";
        return {"Mem.get" + p.access(t) + "(" + slot + ")", kPost, p.jt(t), false};
    }
    if (p.library == nullptr) throw Refused("library function " + std::string(name) + ": the back end was given no library mapping");
    return p.library->call(*this, e, name, own_args);
}

Ex FnWriter::x_new(ir::Node& e) {
    const ir::New& n = *ir::get<ir::New>(&e);
    if (n.memory == ir::MemoryModel::Stack && is_record(e.type)) {
        const std::string tmp_addr = temp(e.type);
        const JLines lines = init_place(tmp_addr, e.type, &e);
        // an aggregate value in an expression: written into a temporary in the frame first
        return {seq_lines(lines, tmp_addr), kPost, "int", true};
    }
    throw Refused("new (" + (n.memory ? std::string(ir::to_text(*n.memory)) : std::string("None")) + ")");
}

std::string FnWriter::seq_lines(const std::vector<std::string>& lines, const std::string& result) const {
    if (lines.empty()) return result;
    JLines calls;
    for (const std::string& l : lines) calls.push_back(l.substr(0, l.size() - 1));
    calls.push_back(result);
    return "seq(" + text::join(calls, ", ") + ")";
}

Ex FnWriter::x_new_array(ir::Node&) { throw Refused("an array value outside an initialiser"); }
Ex FnWriter::x_concat(ir::Node& e) { throw Refused("expression " + std::string(ir::kind_name(e))); }
Ex FnWriter::x_length(ir::Node& e) { throw Refused("expression " + std::string(ir::kind_name(e))); }
Ex FnWriter::x_this(ir::Node& e) { throw Refused("expression " + std::string(ir::kind_name(e))); }

}  // namespace cb::translate::backends::java
