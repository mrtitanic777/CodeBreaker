// translate/backends/go_java_stmt.cpp -- statements of the Go back end for Java: locals, branches, loops and the jumps out of them, switches, return and
// throw, try/catch/finally (see go_java.hpp).

#include <algorithm>
#include <format>

#include "translate/backends/go_java_internal.hpp"

namespace cb::translate::backends::go_java::detail {

namespace {

bool is_loop(const ir::Node* s) { return ir::is<ir::While>(s) || ir::is<ir::DoWhile>(s) || ir::is<ir::For>(s) || ir::is<ir::ForEach>(s); }

// A statement that always leaves the case it ends.
bool leaves(const ir::Node* s) {
    s = strip_paren(s);
    if (s == nullptr) return false;
    if (ir::is<ir::Break>(s) || ir::is<ir::Continue>(s) || ir::is<ir::Return>(s) || ir::is<ir::Throw>(s)) return true;
    if (const auto* b = ir::get<ir::Block>(s)) return !b->stmts.empty() && leaves(b->stmts.back());
    if (const auto* i = ir::get<ir::If>(s)) {
        const std::vector<ir::Node*> a = ir::statements(i->then), b = ir::statements(i->orelse);
        return !a.empty() && !b.empty() && leaves(a.back()) && leaves(b.back());
    }
    return false;
}

// Whether a statement holds a break or continue that leaves it (one whose loop, switch or label is not inside it).
bool jumps_out(const ir::Node* n, bool in_loop, bool in_switch, std::vector<std::string>& labels) {
    if (n == nullptr) return false;
    if (const auto* b = ir::get<ir::Break>(n)) {
        if (b->label) return std::find(labels.begin(), labels.end(), *b->label) == labels.end();
        return !in_loop && !in_switch;
    }
    if (const auto* c = ir::get<ir::Continue>(n)) {
        if (c->label) return std::find(labels.begin(), labels.end(), *c->label) == labels.end();
        return !in_loop;
    }
    if (ir::is<ir::Lambda>(n) || ir::is<ir::ClassDecl>(n)) return false;
    bool loop = in_loop, sw = in_switch;
    std::size_t pushed = 0;
    if (is_loop(n)) loop = true;
    if (ir::is<ir::Switch>(n)) sw = true;
    if (const auto* l = ir::get<ir::Labeled>(n); l != nullptr && l->label) {
        labels.push_back(*l->label);
        pushed = 1;
    }
    bool out = false;
    for (const ir::Node* child : ir::children(*n))
        if (!out && jumps_out(child, loop, sw, labels)) out = true;
    labels.resize(labels.size() - pushed);
    return out;
}

bool leaves_in(const ir::Node* n) {
    std::vector<std::string> labels;
    return any_node(n, [](const ir::Node* x) { return ir::is<ir::Return>(x); }) || jumps_out(n, false, false, labels);
}

std::vector<ir::Node*> stmts_of(const ir::Node* n) {
    if (const auto* b = ir::get<ir::Block>(n)) return b->stmts;
    if (n == nullptr) return {};
    return {const_cast<ir::Node*>(n)};
}

bool is_true_literal(const ir::Node* e) {
    const auto* lit = ir::get<ir::Lit>(strip_paren(e));
    return lit != nullptr && std::holds_alternative<bool>(lit->value) && std::get<bool>(lit->value);
}

}  // namespace

Writer::Writer(Program& p, const ClassInfo* cls, const ir::MethodDecl* m) : p_(p), ci_(cls), m_(m) {}

void Writer::find_reads(std::span<ir::Node* const> stmts) {
    std::set<const ir::Node*> write_only;
    for (const ir::Node* root : stmts) {
        for_each_node(root, [&](const ir::Node* n) {
            if (const auto* a = ir::get<ir::Assign>(n)) write_only.insert(strip_paren(a->target));
            else if (const auto* u = ir::get<ir::Unary>(n); u != nullptr && (u->op == "++" || u->op == "--")) write_only.insert(strip_paren(u->x));
        });
    }
    for (const ir::Node* root : stmts) {
        for_each_node(root, [&](const ir::Node* n) {
            if (const auto* name = ir::get<ir::Name>(n); name != nullptr && name->sym && !write_only.contains(n)) read_.insert(name->sym->name);
        });
    }
}

Lines Writer::body(std::span<ir::Node* const> stmts, const ir::TypeP& ret, std::size_t depth) {
    ret_ = ret;
    Lines out;
    const bool value = ret && kind_of(ret) != "void";
    for (const ir::Node* s : stmts) stmt(s, out, depth);
    if (uses_retv_) out.insert(out.begin(), indent_of(depth) + "var retv " + p_.gtype(ret));
    if (value && (stmts.empty() || !leaves(stmts.back()))) out.push_back(indent_of(depth) + "panic(\"unreachable\")");
    return out;
}

std::string Writer::value_of(const ir::Node* e, const ir::TypeP& want) {
    const Code c = conv(expr(e), want);
    return c.bare && is_prim_kind(kind_of(want)) ? c.text : typed_text(c);
}

std::vector<std::string> Writer::call_args(std::span<ir::Node* const> args, const std::vector<ir::TypeP>& params) {
    std::vector<std::string> out;
    for (std::size_t i = 0; i < args.size(); ++i) out.push_back(value_of(args[i], i < params.size() ? params[i] : params.back()));
    return out;
}

void Writer::stmts(std::span<ir::Node* const> list, Lines& out, std::size_t depth) {
    for (const ir::Node* s : list) stmt(s, out, depth);
}

void Writer::suite(const ir::Suite& s, Lines& out, std::size_t depth) {
    const std::vector<ir::Node*> list = ir::statements(s);
    if (list.size() == 1) {
        if (const auto* b = ir::get<ir::Block>(list[0])) {
            stmts(b->stmts, out, depth);
            return;
        }
    }
    stmts(list, out, depth);
}

void Writer::block_body(const ir::Node* n, Lines& out, std::size_t depth) { stmts(stmts_of(n), out, depth); }

std::string Writer::cond_text(const ir::Node* e) { cur_stmt_ = e; return conv(expr(e), ir::bool_type()).text; }

void Writer::stmt(const ir::Node* s, Lines& out, std::size_t depth) {
    if (s == nullptr) return;
    if (ir::is<ir::Local>(s) || ir::is<ir::ExprStmt>(s) || ir::is<ir::Return>(s) || ir::is<ir::Throw>(s)) cur_stmt_ = s;
    const std::string ind = indent_of(depth);
    if (const auto* b = ir::get<ir::Block>(s)) {
        out.push_back(ind + "{");
        stmts(b->stmts, out, depth + 1);
        out.push_back(ind + "}");
    } else if (const auto* l = ir::get<ir::Local>(s)) {
        emit_local(*l, out, depth);
    } else if (const auto* e = ir::get<ir::ExprStmt>(s)) {
        emit_expr_stmt(e->expr, out, depth);
    } else if (const auto* i = ir::get<ir::If>(s)) {
        emit_if(*i, out, depth);
    } else if (ir::is<ir::While>(s) || ir::is<ir::DoWhile>(s) || ir::is<ir::For>(s)) {
        emit_loop(s, std::nullopt, out, depth);
    } else if (const auto* fe = ir::get<ir::ForEach>(s)) {
        emit_foreach(*fe, std::nullopt, out, depth);
    } else if (const auto* lab = ir::get<ir::Labeled>(s)) {
        const std::vector<ir::Node*> inner = ir::statements(lab->body);
        const ir::Node* target = inner.size() == 1 ? inner[0] : nullptr;
        if (ir::is<ir::While>(target) || ir::is<ir::DoWhile>(target) || ir::is<ir::For>(target)) emit_loop(target, lab->label, out, depth);
        else if (const auto* lf = ir::get<ir::ForEach>(target)) emit_foreach(*lf, lab->label, out, depth);
        else throw Decline("a labeled block that is not a loop");
    } else if (const auto* sw = ir::get<ir::Switch>(s)) {
        emit_switch(*sw, out, depth);
    } else if (const auto* r = ir::get<ir::Return>(s)) {
        emit_return(*r, out, depth);
    } else if (const auto* br = ir::get<ir::Break>(s)) {
        emit_jump(true, br->label, out, depth);
    } else if (const auto* co = ir::get<ir::Continue>(s)) {
        emit_jump(false, co->label, out, depth);
    } else if (const auto* th = ir::get<ir::Throw>(s)) {
        out.push_back(ind + "panic(" + conv(expr(th->expr), ir::class_type("java.lang.Throwable")).text + ")");
    } else if (const auto* t = ir::get<ir::Try>(s)) {
        emit_try(*t, out, depth);
    } else if (ir::is<ir::Empty>(s) || ir::is<ir::Comment>(s) || ir::is<ir::Assert>(s)) {
        // (an assert is off unless the JVM is given -ea)
    } else if (const auto* sy = ir::get<ir::Sync>(s)) {
        block_body(sy->body, out, depth);
    } else if (ir::is<ir::CtorCall>(s)) {
        throw Decline("a constructor call that is not the constructor's first statement");
    } else if (ir::is<ir::LocalClass>(s)) {
        throw Decline("a local class");
    } else {
        throw Decline("the statement " + std::string(ir::kind_name(*s)));
    }
}

void Writer::declare(const ir::Sym& sym, const ir::TypeP& type, const std::string& init, bool has_init, Lines& out, std::size_t depth) {
    const std::string name = ident(sym.name);
    const std::string gt = p_.gtype(type);
    if (has_init) out.push_back(indent_of(depth) + init);
    else out.push_back(indent_of(depth) + "var " + name + " " + gt);
    if (!read_.contains(sym.name)) out.push_back(indent_of(depth) + "_ = " + name);
}

void Writer::emit_local(const ir::Local& k, Lines& out, std::size_t depth) {
    const ir::Sym& sym = need_sym(k.sym, "a local");
    const ir::TypeP type = k.type ? k.type : sym.type;
    const std::string name = ident(sym.name);
    const std::string gt = p_.gtype(type);
    if (k.init == nullptr) {
        declare(sym, type, "", false, out, depth);
        return;
    }
    if (const auto* na = ir::get<ir::NewArray>(strip_paren(k.init)); na != nullptr && !k.init->type) const_cast<ir::Node*>(k.init)->type = type;
    const Code c = conv(expr(k.init), type);
    const std::string_view kind = kind_of(type);
    const ir::Node* in = strip_paren(k.init);
    const bool exact_new = ir::is<ir::New>(in) && p_.program_class(type) != nullptr && class_name(type_of(in)) == class_name(type);
    std::string line;
    if (c.bare) line = "var " + name + " " + gt + " = " + c.text;
    else if (c.text != "nil" && (is_prim_kind(kind) || gt == "jrt.String" || gt == "*jrt.StringBuilder" || kind == "array" || exact_new)) line = name + " := " + c.text;
    else line = "var " + name + " " + gt + " = " + c.text;
    declare(sym, type, line, true, out, depth);
}

void Writer::emit_assign_stmt(const ir::Assign& a, Lines& out, std::size_t depth) {
    // a chain `x = y = v` is the inner assignment, then the outer one reading what it assigned
    if (!a.op) {
        if (const auto* inner = ir::get<ir::Assign>(strip_paren(a.value))) {
            if (!is_pure(inner->target)) throw Decline("a chained assignment to a place with effects");
            emit_assign_stmt(*inner, out, depth);
            out.push_back(indent_of(depth) + assign_text(a.target, inner->target, std::nullopt));
            return;
        }
    }
    out.push_back(indent_of(depth) + assign_text(a.target, a.value, a.op));
}

void Writer::emit_expr_stmt(const ir::Node* e, Lines& out, std::size_t depth) {
    const std::string ind = indent_of(depth);
    e = strip_paren(e);
    if (const auto* a = ir::get<ir::Assign>(e)) {
        emit_assign_stmt(*a, out, depth);
    } else if (const auto* u = ir::get<ir::Unary>(e); u != nullptr && (u->op == "++" || u->op == "--")) {
        out.push_back(ind + incdec_text(*u));
    } else if (ir::is<ir::Call>(e) || ir::is<ir::New>(e)) {
        out.push_back(ind + expr(e).text);
    } else {
        out.push_back(ind + "_ = " + expr(e).text);
    }
}

std::string Writer::simple_stmt(const ir::Node* e) {
    cur_stmt_ = e;
    if (const auto* es = ir::get<ir::ExprStmt>(e)) e = es->expr;
    e = strip_paren(e);
    if (const auto* a = ir::get<ir::Assign>(e)) {
        if (!a->op && ir::is<ir::Assign>(strip_paren(a->value))) throw Decline("a chained assignment in a for clause");
        return assign_text(a->target, a->value, a->op);
    }
    if (const auto* u = ir::get<ir::Unary>(e); u != nullptr && (u->op == "++" || u->op == "--")) return incdec_text(*u);
    if (ir::is<ir::Call>(e)) return expr(e).text;
    throw Decline("a for clause that is not an assignment or a call");
}

void Writer::emit_if(const ir::If& k, Lines& out, std::size_t depth) {
    const std::string ind = indent_of(depth);
    out.push_back(ind + "if " + cond_text(k.cond) + " {");
    suite(k.then, out, depth + 1);
    const ir::If* chain = &k;
    while (ir::present(chain->orelse)) {
        const std::vector<ir::Node*> rest = ir::statements(chain->orelse);
        if (rest.size() == 1 && ir::is<ir::If>(rest[0])) {
            chain = ir::get<ir::If>(rest[0]);
            out.push_back(ind + "} else if " + cond_text(chain->cond) + " {");
            suite(chain->then, out, depth + 1);
            continue;
        }
        out.push_back(ind + "} else {");
        suite(chain->orelse, out, depth + 1);
        break;
    }
    out.push_back(ind + "}");
}

// A loop's body is written inside the Target (so jumps find it); the header is the loop line without its label or brace.
void Writer::looped(Target t, const std::string& header, const std::function<void(Lines&, std::size_t)>& body, Lines& out, std::size_t depth) {
    t.loop = true;
    t.closures = tries_.size();
    targets_.push_back(t);
    Lines inner;
    body(inner, depth + 1);
    const Target done = targets_.back();
    targets_.pop_back();
    if (done.used && done.java) out.push_back(indent_of(depth - (depth > 0 ? 0 : 0)) + capital(*done.java) + ":");
    out.push_back(indent_of(depth) + header + " {");
    out.insert(out.end(), inner.begin(), inner.end());
    out.push_back(indent_of(depth) + "}");
}

void Writer::emit_loop(const ir::Node* loop, std::optional<std::string> label, Lines& out, std::size_t depth) {
    Target t;
    t.java = label;
    if (const auto* w = ir::get<ir::While>(loop)) {
        const std::string header = is_true_literal(w->cond) ? "for" : "for " + cond_text(w->cond);
        looped(t, header, [&](Lines& inner, std::size_t d) { suite(w->body, inner, d); }, out, depth);
    } else if (const auto* dw = ir::get<ir::DoWhile>(loop)) {
        // the body runs once before the condition is first tested; a continue goes on to test it
        const std::string flag = "again" + std::to_string(++temps_);
        const std::string header = "for " + flag + " := true; " + flag + "; " + flag + " = " + cond_text(dw->cond);
        looped(t, header, [&](Lines& inner, std::size_t d) { suite(dw->body, inner, d); }, out, depth);
    } else if (const auto* f = ir::get<ir::For>(loop)) {
        std::string init;
        if (f->init.size() == 1 && !ir::is<ir::Local>(f->init[0])) {
            init = simple_stmt(f->init[0]);
        } else if (!f->init.empty()) {
            std::vector<std::string> names, values;
            for (const ir::Node* n : f->init) {
                const auto* l = ir::get<ir::Local>(n);
                if (l == nullptr || l->init == nullptr || !l->sym || !is_prim_kind(kind_of(l->type ? l->type : l->sym->type))) throw Decline("a for loop initialiser that is not a number");
                if (!read_.contains(l->sym->name)) throw Decline("a for loop variable nothing reads");
                names.push_back(ident(l->sym->name));
                values.push_back(typed_text(conv(expr(l->init), l->type ? l->type : l->sym->type)));
            }
            init = join(names) + " := " + join(values);
        }
        const std::string cond = f->cond != nullptr && !is_true_literal(f->cond) ? cond_text(f->cond) : "";
        std::string update;
        if (f->update.size() == 1) {
            update = simple_stmt(f->update[0]);
        } else if (f->update.size() > 1) {   // i++, j-- as one tuple assignment
            std::vector<std::string> lhs, rhs;
            for (const ir::Node* u : f->update) {
                const ir::Node* e = strip_paren(ir::get<ir::ExprStmt>(u) != nullptr ? ir::get<ir::ExprStmt>(u)->expr : u);
                const ir::Node* target = nullptr;
                std::string step;
                if (const auto* un = ir::get<ir::Unary>(e); un != nullptr && (un->op == "++" || un->op == "--")) {
                    target = strip_paren(un->x);
                    step = (un->op == "++" ? " + 1" : " - 1");
                } else if (const auto* as = ir::get<ir::Assign>(e); as != nullptr && (as->op == "+" || as->op == "-")) {
                    target = strip_paren(as->target);
                    step = " " + *as->op + " " + wrap(conv(expr(as->value), type_of(target)), kMul + 1);
                }
                if (target == nullptr || !ir::is<ir::Name>(target)) throw Decline("a for loop with several updates that are not steps");
                const std::string n = expr(target).text;
                lhs.push_back(n);
                rhs.push_back(n + step);
            }
            update = join(lhs) + " = " + join(rhs);
        }
        std::string header = "for ";
        if (!init.empty() || !update.empty()) header += init + "; " + cond + "; " + update;
        else if (!cond.empty()) header += cond;
        else header = "for";
        if (!update.empty() && cond.empty() && init.empty()) header = "for ; ; " + update;
        looped(t, header, [&](Lines& inner, std::size_t d) { suite(f->body, inner, d); }, out, depth);
    }
}

void Writer::emit_foreach(const ir::ForEach& k, std::optional<std::string> label, Lines& out, std::size_t depth) {
    const auto* var = ir::get<ir::Local>(k.var);
    if (var == nullptr || !var->sym) throw Decline("a for-each variable that is not a local");
    const ir::TypeP type = var->type ? var->type : var->sym->type;
    const Code coll = expr(k.iterable);
    const bool array = kind_of(coll.type) == "array";
    std::string range = array ? coll.text : wrap(coll, kAtom) + ".ToSlice()";
    if (!array && class_name(coll.type).starts_with("java.util.") == false && class_name(coll.type) != "java.lang.Iterable") throw Decline("a for-each over " + std::string(class_name(coll.type)));
    const std::string name = ident(var->sym->name);
    const ir::TypeP held = array ? elem_of(coll.type) : ir::class_type("java.lang.Object");
    const bool direct = p_.gtype(held) == p_.gtype(type) && (array || p_.gtype(type) == "any");
    const bool used = read_.contains(var->sym->name);
    Target t;
    t.java = label;
    std::string header;
    if (!used) header = "for range " + range;
    else if (direct) header = "for _, " + name + " := range " + range;
    else header = "for _, item" + std::to_string(++temps_) + " := range " + range;
    const std::string item = "item" + std::to_string(temps_);
    looped(t, header,
           [&](Lines& inner, std::size_t d) {
               if (used && !direct) {
                   Code c;
                   c.text = item;
                   c.type = held;
                   if (!array) c = from_any(c, type);
                   else c = conv(c, type);
                   inner.push_back(indent_of(d) + name + " := " + c.text);
               }
               block_body(k.body, inner, d);
           },
           out, depth);
}

void Writer::emit_jump(bool is_break, const std::optional<std::string>& label, Lines& out, std::size_t depth) {
    int idx = -1;
    for (std::size_t i = targets_.size(); i-- > 0;) {
        const Target& t = targets_[i];
        if (label) {
            if (t.java == label) {
                idx = static_cast<int>(i);
                break;
            }
        } else if (is_break || t.loop) {
            idx = static_cast<int>(i);
            break;
        }
    }
    if (idx < 0) throw Decline("a break or continue with nothing to leave");
    Target& t = targets_[idx];
    if (t.closures < tries_.size()) {   // out of a try closure: the closure ends with the jump to be made after it
        TryFrame& frame = tries_.back();
        if (frame.flow.empty()) throw Decline("a jump out of a finally");
        frame.jumps.emplace_back(is_break, label);
        out.push_back(indent_of(depth) + frame.flow + " = " + std::to_string(frame.jumps.size() + 1));
        out.push_back(indent_of(depth) + (frame.handler ? "return true" : "return"));
        return;
    }
    std::string text = is_break ? "break" : "continue";
    if (label) {
        t.used = true;
        text += " " + capital(*label);
    }
    out.push_back(indent_of(depth) + text);
}

void Writer::emit_return_text(const std::string& value, Lines& out, std::size_t depth) {
    const std::string ind = indent_of(depth);
    if (tries_.empty()) {
        out.push_back(ind + (value.empty() ? "return" : "return " + value));
        return;
    }
    TryFrame& frame = tries_.back();
    if (frame.flow.empty()) throw Decline("a return inside a finally");
    frame.returns = true;
    if (!value.empty()) {
        uses_retv_ = true;
        out.push_back(ind + "retv = " + value);
    }
    out.push_back(ind + frame.flow + " = 1");
    out.push_back(ind + (frame.handler ? "return true" : "return"));
}

void Writer::emit_return(const ir::Return& k, Lines& out, std::size_t depth) {
    emit_return_text(k.expr != nullptr ? value_of(k.expr, ret_) : std::string(), out, depth);
}

// ---------------------------------------------------------------------------------------------------------------- switch

void Writer::emit_switch(const ir::Switch& k, Lines& out, std::size_t depth) {
    const std::string ind = indent_of(depth);
    const ir::TypeP stype = type_of(k.expr);
    const std::string_view sk = kind_of(stype);
    const ClassInfo* enum_class = p_.program_class(stype);
    const bool on_enum = enum_class != nullptr && enum_class->kind == "enum";
    const bool on_string = class_name(stype) == "java.lang.String";
    const std::string_view boxed = boxed_kind(stype);
    if (!on_enum && !on_string && sk != "i32" && sk != "i16" && sk != "i8" && sk != "u16" && boxed.empty()) throw Decline("a switch on " + std::string(sk));
    Code scrut = numeric(k.expr);
    std::string head = scrut.text;
    if (on_string) head = wrap(scrut, kAtom) + ".GoString()";
    struct Arm {
        std::vector<std::string> labels;
        bool is_default = false;
        std::vector<ir::Node*> body;
    };
    std::vector<Arm> arms;
    Arm pending;
    for (std::size_t i = 0; i < k.cases.size(); ++i) {
        const auto* c = ir::get<ir::Case>(k.cases[i]);
        if (c == nullptr) throw Decline("a switch member that is not a case");
        for (const ir::Node* label : c->labels) {
            const ir::Node* l = strip_paren(label);
            if (const auto* lit = ir::get<ir::Lit>(l); lit != nullptr && on_string && std::holds_alternative<std::string>(lit->value)) {
                pending.labels.push_back("\"" + go_string(std::get<std::string>(lit->value)).substr(1));
            } else if (const auto* lit2 = ir::get<ir::Lit>(l); lit2 != nullptr && kind_of(type_of(l)) == "u16" && std::holds_alternative<std::string>(lit2->value) &&
                                                                  std::get<std::string>(lit2->value).size() == 1 && std::get<std::string>(lit2->value)[0] >= 0x20 &&
                                                                  std::get<std::string>(lit2->value)[0] < 0x7f && std::get<std::string>(lit2->value)[0] != '\'' &&
                                                                  std::get<std::string>(lit2->value)[0] != '\\') {
                pending.labels.push_back(std::string("'") + std::get<std::string>(lit2->value) + "'");
            } else {
                pending.labels.push_back(conv(expr(label), on_enum || on_string ? type_of(label) : prim_by_kind(boxed.empty() ? sk : boxed)).text);
            }
        }
        if (c->is_default) pending.is_default = true;
        const bool last = i + 1 == k.cases.size();
        if (c->body.empty() && !last) continue;   // labels sharing the next body
        pending.body = c->body;
        arms.push_back(std::move(pending));
        pending = Arm();
    }
    Target t;
    t.loop = false;
    t.closures = tries_.size();
    targets_.push_back(t);
    out.push_back(ind + "switch " + head + " {");
    for (std::size_t i = 0; i < arms.size(); ++i) {
        const Arm& a = arms[i];
        out.push_back(ind + (a.is_default ? "default:" : "case " + join(a.labels) + ":"));
        std::vector<ir::Node*> body = a.body;
        if (!body.empty()) {
            if (const auto* br = ir::get<ir::Break>(body.back()); br != nullptr && !br->label) body.pop_back();
            else if (const auto* blk = ir::get<ir::Block>(body.back()); blk != nullptr && !blk->stmts.empty()) {
                if (const auto* br2 = ir::get<ir::Break>(blk->stmts.back()); br2 != nullptr && !br2->label && body.size() == 1) body = {blk->stmts.begin(), blk->stmts.end() - 1};
            }
        }
        stmts(body, out, depth + 1);
        const bool ends = !a.body.empty() && (ir::is<ir::Break>(a.body.back()) || leaves(a.body.back()));
        if (!a.body.empty() && !ends && i + 1 < arms.size()) out.push_back(indent_of(depth + 1) + "fallthrough");
    }
    out.push_back(ind + "}");
    targets_.pop_back();
}

// ---------------------------------------------------------------------------------------------------------------- try

void Writer::emit_try(const ir::Try& k, Lines& out, std::size_t depth) {
    if (!k.resources.empty()) throw Decline("try-with-resources");
    const std::string ind = indent_of(depth);
    if (k.finally_body != nullptr && leaves_in(k.finally_body)) throw Decline("a return, break or continue inside a finally");
    bool flow = leaves_in(k.body);
    for (const ir::Node* c : k.catches) flow = flow || leaves_in(c);
    const std::string fv = flow ? "flow" + std::to_string(++temps_) : "";
    if (flow) out.push_back(ind + "var " + fv + " int");
    tries_.push_back(TryFrame{fv, false, false, {}});
    out.push_back(ind + "jrt.Try(func() {");
    block_body(k.body, out, depth + 1);
    if (k.catches.empty()) {
        out.push_back(ind + "}, nil, ");
    } else {
        tries_.back().handler = true;
        out.push_back(ind + "}, func(ex jrt.Throwable) bool {");
        for (const ir::Node* cn : k.catches) {
            const auto* c = ir::get<ir::Catch>(cn);
            if (c == nullptr) throw Decline("a catch clause that is not one");
            const ir::Sym& sym = need_sym(c->sym, "a catch variable");
            const std::string var = ident(sym.name);
            const bool read = read_.contains(sym.name);
            const ClassInfo* user = c->types.size() == 1 ? p_.program_class(c->types[0]) : nullptr;
            std::vector<std::string> tests;
            for (const ir::TypeP& t : c->types) {
                if (const ClassInfo* pc = p_.program_class(t)) tests.push_back("jrt.Is[" + p_.iface(*pc) + "](ex)");
                else tests.push_back("jrt.InstanceOf(ex, " + go_string(std::string(class_name(t))) + ")");
            }
            if (user != nullptr) {
                out.push_back(indent_of(depth + 1) + "if " + var + ", ok := ex.(" + p_.iface(*user) + "); ok {");
                if (!read) out.push_back(indent_of(depth + 2) + "_ = " + var);
            } else {
                out.push_back(indent_of(depth + 1) + "if " + join(tests, " || ") + " {");
                out.push_back(indent_of(depth + 2) + var + " := ex");
                if (!read) out.push_back(indent_of(depth + 2) + "_ = " + var);
            }
            block_body(c->body, out, depth + 2);
            out.push_back(indent_of(depth + 2) + "return true");
            out.push_back(indent_of(depth + 1) + "}");
        }
        out.push_back(indent_of(depth + 1) + "return false");
        out.push_back(ind + "}, ");
    }
    if (k.finally_body != nullptr) {
        tries_.push_back(TryFrame{"", false, false, {}});
        Lines tmp;
        block_body(k.finally_body, tmp, depth + 1);
        tries_.pop_back();
        out.back() += "func() {";
        out.insert(out.end(), tmp.begin(), tmp.end());
        out.push_back(ind + "})");
    } else {
        out.back() += "nil)";
    }
    TryFrame frame = std::move(tries_.back());
    tries_.pop_back();
    if (!flow) return;
    if (frame.returns) {
        out.push_back(ind + "if " + fv + " == 1 {");
        emit_return_text(ret_ && kind_of(ret_) != "void" ? "retv" : "", out, depth + 1);
        out.push_back(ind + "}");
    }
    for (std::size_t id = 0; id < frame.jumps.size(); ++id) {
        out.push_back(ind + "if " + fv + " == " + std::to_string(id + 2) + " {");
        emit_jump(frame.jumps[id].first, frame.jumps[id].second, out, depth + 1);
        out.push_back(ind + "}");
    }
}

Lines Writer::ctor_call(const ir::CtorCall& call) {
    const ir::Sym& sym = need_sym(call.sym, "a constructor call");
    const std::string owner = sym.owner.value_or("");
    if (const ClassInfo* target = p_.model().info(owner)) {
        const auto match = p_.model().method_decl(sym);
        if (!match) throw Decline("a constructor of " + owner + " the program does not declare");
        std::vector<ir::TypeP> params;
        for (const ir::Node* pn : method_of(match->method).params) params.push_back(param_of(pn).type);
        const bool is_this = call.kind && call.kind->starts_with("this");
        return {std::string("this.") + (is_this ? "" : p_.obj(*target) + ".") + "ctor" + p_.ctor_suffix(*match->cls, match->method) + "(" + join(call_args(call.args, params)) + ")"};
    }
    if (owner == "java.lang.Object" || owner == "java.lang.Enum") return {};
    if (p_.is_lib_throwable(owner)) {
        std::string args = "this.self, " + go_string(owner);
        const std::size_t n = call.args.size();
        const std::vector<ir::TypeP> params = sym.params;
        const std::vector<std::string> a = call_args(call.args, params);
        if (n == 0) return {"this.ThrowableBase.Init(" + args + ", nil, nil)"};
        if (n == 1 && class_name(params[0]) == "java.lang.String") return {"this.ThrowableBase.Init(" + args + ", " + a[0] + ", nil)"};
        if (n == 1) return {"this.ThrowableBase.InitCause(" + args + ", " + a[0] + ")"};
        if (n == 2) return {"this.ThrowableBase.Init(" + args + ", " + a[0] + ", " + a[1] + ")"};
        throw Decline("a library exception constructor with " + std::to_string(n) + " arguments");
    }
    throw Decline("a constructor of the library class " + owner);
}

}  // namespace cb::translate::backends::go_java::detail
