// The C# -> C++17 back end, statements.
#include <algorithm>
#include <format>
#include <map>

#include "cb/core/text.hpp"
#include "cb/translate/backends/csharp_cpp17_internal.hpp"

namespace cb::translate::backends::csharp_cpp17::detail {

namespace {

// The statements a block-like node stands for: a Block's own, else the node itself.
std::vector<Node*> cs_flat_of(const ir::Suite& suite) {
    std::vector<Node*> out;
    for (Node* n : ir::statements(suite)) {
        if (const auto* b = ir::get<ir::Block>(n); b != nullptr && ir::statements(suite).size() == 1) {
            for (Node* s : b->stmts) out.push_back(s);
        } else {
            out.push_back(n);
        }
    }
    return out;
}

struct CsCaptureWalk {
    std::set<std::string>& boxed;
    std::map<std::string, int> declared;   // the lambda depth a variable is declared at (absent: the function's own)

    void walk(Node* n, int depth) {
        if (n == nullptr) return;
        if (const auto* l = ir::get<ir::Lambda>(n)) {
            for (Node* p : l->params) {
                if (const auto* pa = ir::get<ir::Param>(p)) declared[sym_key(*pa->sym)] = depth + 1;
            }
            walk(l->body, depth + 1);
            return;
        }
        if (const auto* l = ir::get<ir::Local>(n)) declared[sym_key(*l->sym)] = depth;
        if (const auto* c = ir::get<ir::Catch>(n); c != nullptr && c->sym) declared[sym_key(*c->sym)] = depth;
        if (const auto* i = ir::get<ir::InstanceOf>(n); i != nullptr && i->binding != nullptr) {
            if (const auto* b = ir::get<ir::Local>(i->binding)) declared[sym_key(*b->sym)] = depth;
        }
        if (const auto* nm = ir::get<ir::Name>(n); nm != nullptr && nm->sym) {
            if (nm->sym->kind == ir::SymKind::Local || nm->sym->kind == ir::SymKind::Param) {
                const auto it = declared.find(sym_key(*nm->sym));
                if (depth > (it == declared.end() ? 0 : it->second)) boxed.insert(sym_key(*nm->sym));
            }
        }
        for (Node* c : ir::children(*n)) walk(c, depth);
        if (const auto* i = ir::get<ir::InstanceOf>(n); i != nullptr && i->binding != nullptr) {
            if (const auto* b = ir::get<ir::Local>(i->binding)) {
                (void)b;
            }
        }
    }
};

}  // namespace

// Locals a lambda reaches into are held through shared_ptr, so the lambda and the function share one variable as C# closures do.
void Writer::collect_captures(Node* root) {
    CsCaptureWalk w{boxed_, {}};
    w.walk(root, 0);
}

void Writer::collect_hoists(Node* root, std::vector<Node*>& out) const {
    if (root == nullptr || ir::is<ir::Lambda>(root)) return;
    if (const auto* i = ir::get<ir::InstanceOf>(root); i != nullptr && i->binding != nullptr) out.push_back(i->binding);
    for (Node* c : ir::children(*root)) collect_hoists(c, out);
}

// Pattern variables (`x is T v`) are declared at the top of their function: the test then assigns them.
void Writer::write_hoists(Node* root) {
    std::vector<Node*> found;
    collect_hoists(root, found);
    for (Node* b : found) {
        const auto* l = ir::get<ir::Local>(b);
        if (hoisted_.contains(sym_key(*l->sym))) continue;
        hoisted_.insert(sym_key(*l->sym));
        line(var_decl(l->sym, l->type, "", false));
    }
}

std::string Writer::var_decl(const ir::SymP& sym, const TypeP& t, const std::string& init, bool has_init) {
    const std::string ty = type(t);
    const std::string name = ident(sym->name);
    if (boxed_.contains(sym_key(*sym))) return "auto " + name + " = std::make_shared<" + ty + ">(" + (has_init ? init : ty + "{}") + ");";
    return ty + " " + name + (has_init ? " = " + init : "{}") + ";";
}

std::string Writer::capture(const std::function<void()>& write) {
    std::string saved = std::move(out_);
    out_.clear();
    write();
    std::string text = std::move(out_);
    out_ = std::move(saved);
    while (!text.empty() && (text.back() == '\n' || text.back() == ' ')) text.pop_back();
    return text;
}

// ------------------------------------------------------------------------------------------------------------------- statements

std::vector<Node*> Writer::flat(const ir::Suite& suite) { return cs_flat_of(suite); }

void Writer::body(const ir::Suite& suite) { stmts(flat(suite)); }

void Writer::stmts(const std::vector<Node*>& list) {
    for (Node* s : list) stmt(s);
}

void Writer::local_decl(const ir::Local& l) {
    const bool has_init = l.init != nullptr;
    line(var_decl(l.sym, l.type, has_init ? conv(l.init, l.type) : "", has_init));
}

void Writer::stmt(Node* s) {
    if (s == nullptr) return;
    ir::visit(*s, [&](const auto& k) {
        using K = std::decay_t<decltype(k)>;
        if constexpr (std::is_same_v<K, ir::Block>) {
            open("");
            stmts(k.stmts);
            close();
        } else if constexpr (std::is_same_v<K, ir::Local>) {
            local_decl(k);
        } else if constexpr (std::is_same_v<K, ir::ExprStmt>) {
            if (const auto* u = ir::get<ir::Unsupported>(k.expr)) decline(u->kind.value_or("unsupported"), u->raw.value_or("a construct the front end did not lower"));
            line(bare(expr(k.expr)) + ";");
        } else if constexpr (std::is_same_v<K, ir::If>) {
            const ir::If* cur = &k;
            std::string prefix = "if";
            for (;;) {
                line(prefix + " (" + bare(expr(cur->cond)) + ") {");
                ++indent_;
                body(cur->then);
                --indent_;
                const std::vector<Node*> other = ir::statements(cur->orelse);
                if (other.empty()) {
                    line("}");
                    break;
                }
                if (other.size() == 1 && ir::is<ir::If>(other[0])) {
                    cur = ir::get<ir::If>(other[0]);
                    prefix = "} else if";
                    continue;
                }
                line("} else {");
                ++indent_;
                body(cur->orelse);
                --indent_;
                line("}");
                break;
            }
        } else if constexpr (std::is_same_v<K, ir::While>) {
            open("while (" + bare(expr(k.cond)) + ")");
            body(k.body);
            close();
        } else if constexpr (std::is_same_v<K, ir::DoWhile>) {
            open("do");
            body(k.body);
            --indent_;
            line("} while (" + bare(expr(k.cond)) + ");");
        } else if constexpr (std::is_same_v<K, ir::For>) {
            for_loop(k);
        } else if constexpr (std::is_same_v<K, ir::ForEach>) {
            for_each(k);
        } else if constexpr (std::is_same_v<K, ir::Switch>) {
            switch_stmt(k);
        } else if constexpr (std::is_same_v<K, ir::Break>) {
            if (k.label) decline("labeled-break", "break out of a labeled statement");
            line("break;");
        } else if constexpr (std::is_same_v<K, ir::Continue>) {
            if (k.label) decline("labeled-continue", "continue to a labeled statement");
            line("continue;");
        } else if constexpr (std::is_same_v<K, ir::Return>) {
            if (k.expr == nullptr) line("return;");
            else line("return " + bare(conv(k.expr, returns_.empty() ? TypeP{} : returns_.back())) + ";");
        } else if constexpr (std::is_same_v<K, ir::Yield>) {
            line("return " + bare(conv(k.expr, returns_.empty() ? TypeP{} : returns_.back())) + ";");
        } else if constexpr (std::is_same_v<K, ir::Throw>) {
            if (k.expr == nullptr) line("throw;");
            else line("throw cs::Thrown{" + bare(expr(k.expr)) + "};");
        } else if constexpr (std::is_same_v<K, ir::Try>) {
            try_stmt(k);
        } else if constexpr (std::is_same_v<K, ir::Labeled>) {
            line(ident(k.label.value_or("label")) + ":;");
            body(k.body);
        } else if constexpr (std::is_same_v<K, ir::Goto>) {
            line("goto " + ident(k.label.value_or("label")) + ";");
        } else if constexpr (std::is_same_v<K, ir::Sync>) {
            open("");
            body(ir::Suite{k.body});
            close();
        } else if constexpr (std::is_same_v<K, ir::Empty> || std::is_same_v<K, ir::Comment>) {
            // nothing
        } else {
            decline(std::string(ir::kind_name(*s)), "a statement of this kind has no C++17 form yet");
        }
    });
}

void Writer::for_loop(const ir::For& f) {
    // (the expressions of a for's header may come as statements)
    const auto header = [&](Node* n) { return bare(expr(ir::is<ir::ExprStmt>(n) ? ir::get<ir::ExprStmt>(n)->expr : n)); };
    std::string cond = f.cond != nullptr ? bare(expr(f.cond)) : "";
    std::string update;
    for (std::size_t i = 0; i < f.update.size(); ++i) update += (i ? ", " : "") + header(f.update[i]);
    // one declaration (or several of one type) stay in the for; anything else is written before it
    std::string init;
    bool inline_init = true;
    std::string common_type;
    for (Node* n : f.init) {
        if (const auto* l = ir::get<ir::Local>(n)) {
            if (boxed_.contains(sym_key(*l->sym))) {
                inline_init = false;
                break;
            }
            const std::string t = type(l->type);
            if (common_type.empty()) common_type = t;
            else if (common_type != t) {
                inline_init = false;
                break;
            }
        } else if (!common_type.empty()) {
            inline_init = false;
            break;
        }
    }
    if (inline_init) {
        for (std::size_t i = 0; i < f.init.size(); ++i) {
            Node* n = f.init[i];
            if (const auto* l = ir::get<ir::Local>(n)) {
                init += (i ? ", " : common_type + " ") + ident(l->sym->name) + (l->init != nullptr ? " = " + conv(l->init, l->type) : "{}");
            } else {
                init += (i ? ", " : "") + header(n);
            }
        }
        open("for (" + init + "; " + cond + "; " + update + ")");
        body(f.body);
        close();
        return;
    }
    open("");
    for (Node* n : f.init) {
        if (const auto* l = ir::get<ir::Local>(n)) local_decl(*l);
        else line(header(n) + ";");
    }
    open("for (; " + cond + "; " + update + ")");
    body(f.body);
    close();
    close();
}

void Writer::for_each(const ir::ForEach& f) {
    const auto* var = ir::get<ir::Local>(f.var);
    const std::string coll = "cs_c" + std::to_string(temp_++);
    open("");
    line("auto " + coll + " = " + bare(expr(f.iterable)) + ";");
    const std::string t = type(var->type);
    if (boxed_.contains(sym_key(*var->sym))) {
        open("for (" + t + " " + ident(var->sym->name) + "__e : cs::iter(" + coll + "))");
        line("auto " + ident(var->sym->name) + " = std::make_shared<" + t + ">(" + ident(var->sym->name) + "__e);");
    } else {
        open("for (" + t + " " + ident(var->sym->name) + " : cs::iter(" + coll + "))");
    }
    body(ir::Suite{f.body});
    close();
    close();
}

// Whether a `break` in the statements leaves the section itself (not a loop or switch inside it).
bool Writer::free_break(Node* n) {
    if (n == nullptr) return false;
    if (ir::is<ir::Break>(n)) return true;
    if (ir::is<ir::While>(n) || ir::is<ir::DoWhile>(n) || ir::is<ir::For>(n) || ir::is<ir::ForEach>(n) || ir::is<ir::Switch>(n)) return false;
    for (Node* c : ir::children(*n)) {
        if (free_break(c)) return true;
    }
    return false;
}

void Writer::switch_stmt(const ir::Switch& s) { switch_cases(s.expr, s.cases); }

void Writer::switch_cases(Node* subject, const std::vector<Node*>& cases) {
    if (is_string(subject->type)) {
        // C++ cannot switch on a string: the sections are tests in order (each C# section ends in a jump, so none falls into the next)
        const std::string v = "cs_s" + std::to_string(temp_++);
        open("");
        line("const cs::String " + v + " = " + bare(expr(subject)) + ";");
        std::string prefix = "if";
        const ir::Case* default_case = nullptr;
        for (Node* c : cases) {
            const auto* cs = ir::get<ir::Case>(c);
            if (cs->is_default && cs->labels.empty()) {
                default_case = cs;
                continue;
            }
            std::string test;
            for (std::size_t i = 0; i < cs->labels.size(); ++i) test += (i ? " || " : "") + v + " == " + expr(cs->labels[i]);
            line(prefix + " (" + test + ") {");
            ++indent_;
            std::vector<Node*> list = cs->body;
            if (!list.empty() && ir::is<ir::Break>(list.back())) list.pop_back();
            for (Node* n : list) {
                if (free_break(n)) decline("switch", "a break inside a section of a switch on a string");
            }
            stmts(list);
            --indent_;
            prefix = "} else if";
        }
        if (default_case != nullptr) {
            line(prefix == "if" ? "{" : "} else {");
            ++indent_;
            std::vector<Node*> list = default_case->body;
            if (!list.empty() && ir::is<ir::Break>(list.back())) list.pop_back();
            for (Node* n : list) {
                if (free_break(n)) decline("switch", "a break inside a section of a switch on a string");
            }
            stmts(list);
            --indent_;
        }
        if (prefix != "if" || default_case != nullptr) line("}");
        close();
        return;
    }
    open("switch (" + bare(expr(subject)) + ")");
    for (Node* c : cases) {
        const auto* cs = ir::get<ir::Case>(c);
        for (Node* l : cs->labels) line("case " + expr(l) + ":");
        if (cs->is_default) line("default:");
        line("{");
        ++indent_;
        stmts(cs->body);
        --indent_;
        line("}");
    }
    close();
}

void Writer::try_stmt(const ir::Try& t) {
    open("");
    // `using`: each resource is disposed, in reverse, when the scope is left
    std::vector<std::string> disposers;
    for (Node* r : t.resources) {
        if (const auto* l = ir::get<ir::Local>(r)) {
            local_decl(*l);
            disposers.push_back(local_name(l->sym));
        } else {
            decline("using", "a resource that is not a declaration");
        }
    }
    if (!disposers.empty()) {
        std::string text = "auto cs_dispose" + std::to_string(temp_++) + " = cs::finally([&] { ";
        for (auto it = disposers.rbegin(); it != disposers.rend(); ++it) text += "if (" + *it + ") " + *it + "->Dispose(); ";
        line(text + "});");
    }
    if (t.finally_body != nullptr) {
        line("auto cs_finally" + std::to_string(temp_++) + " = cs::finally([&] {");
        ++indent_;
        body(ir::Suite{t.finally_body});
        --indent_;
        line("});");
    }
    if (t.catches.empty()) {
        body(ir::Suite{t.body});
        close();
        return;
    }
    open("try");
    body(ir::Suite{t.body});
    --indent_;
    const std::string thrown = "cs_t" + std::to_string(temp_++);
    bool typeless = false;
    for (Node* c : t.catches) typeless = typeless || ir::get<ir::Catch>(c)->types.empty();
    if (typeless) {
        // `catch { }` takes any exception, a C# one or not
        const auto* only = ir::get<ir::Catch>(t.catches.back());
        line("} catch (...) {");
        ++indent_;
        body(ir::Suite{only->body});
        --indent_;
    } else {
        line("} catch (const cs::Thrown& " + thrown + ") {");
        ++indent_;
        std::string prefix = "if";
        for (Node* c : t.catches) {
            const auto* ct = ir::get<ir::Catch>(c);
            if (ct->types.size() != 1) decline("catch", "a catch of several exception types");
            const auto* cls = class_of(ct->types[0]);
            if (cls == nullptr) decline("catch", "a catch of a type that is not a class");
            const std::string var = ct->sym ? ident(ct->sym->name) : "cs_e" + std::to_string(temp_++);
            line(prefix + " (auto " + var + " = cs::catch_as<" + class_text(*cls) + ">(" + thrown + ")) {");
            ++indent_;
            body(ir::Suite{ct->body});
            --indent_;
            prefix = "} else if";
        }
        line("} else {");
        line("    throw;");
        line("}");
    }
    line("}");
    close();
}

}  // namespace cb::translate::backends::csharp_cpp17::detail
