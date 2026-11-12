#include "cb/translate/frontends/python/pyast.hpp"

#include "cb/core/text.hpp"

namespace cb::translate::frontends::python::pyast {

namespace {

void pyast_add(std::vector<AnyNode>& out, const syntax::Expr* e) {
    if (e) out.emplace_back(e);
}
void pyast_add(std::vector<AnyNode>& out, const syntax::Stmt* s) {
    if (s) out.emplace_back(s);
}
template <class V>
void pyast_add_all(std::vector<AnyNode>& out, const V& items) {
    for (auto* item : items) pyast_add(out, item);
}
void pyast_add_patterns(std::vector<AnyNode>& out, const std::vector<syntax::Pattern*>& patterns) {
    for (auto* p : patterns) {
        if (p) out.emplace_back(p, Category::Pattern);
    }
}
template <class V>
void pyast_add_helpers(std::vector<AnyNode>& out, const V& items, Category category) {
    for (auto* item : items) out.emplace_back(item, category);
}

void pyast_expr_children(const syntax::Expr* e, std::vector<AnyNode>& out) {
    using K = syntax::ExprKind;
    switch (e->kind) {
        case K::BoolOp: pyast_add_all(out, syntax::cast<syntax::BoolOp>(e)->values); break;
        case K::NamedExpr:
            pyast_add(out, syntax::cast<syntax::NamedExpr>(e)->target);
            pyast_add(out, syntax::cast<syntax::NamedExpr>(e)->value);
            break;
        case K::BinOp:
            pyast_add(out, syntax::cast<syntax::BinOp>(e)->left);
            pyast_add(out, syntax::cast<syntax::BinOp>(e)->right);
            break;
        case K::UnaryOp: pyast_add(out, syntax::cast<syntax::UnaryOp>(e)->operand); break;
        case K::Lambda:
            if (auto* args = syntax::cast<syntax::Lambda>(e)->args) out.emplace_back(args, Category::Arguments);
            pyast_add(out, syntax::cast<syntax::Lambda>(e)->body);
            break;
        case K::IfExp:
            pyast_add(out, syntax::cast<syntax::IfExp>(e)->test);
            pyast_add(out, syntax::cast<syntax::IfExp>(e)->body);
            pyast_add(out, syntax::cast<syntax::IfExp>(e)->orelse);
            break;
        case K::Dict:
            pyast_add_all(out, syntax::cast<syntax::Dict>(e)->keys);
            pyast_add_all(out, syntax::cast<syntax::Dict>(e)->values);
            break;
        case K::Set: pyast_add_all(out, syntax::cast<syntax::Set>(e)->elts); break;
        case K::ListComp:
            pyast_add(out, syntax::cast<syntax::ListComp>(e)->elt);
            pyast_add_helpers(out, syntax::cast<syntax::ListComp>(e)->generators, Category::Comprehension);
            break;
        case K::SetComp:
            pyast_add(out, syntax::cast<syntax::SetComp>(e)->elt);
            pyast_add_helpers(out, syntax::cast<syntax::SetComp>(e)->generators, Category::Comprehension);
            break;
        case K::DictComp:
            pyast_add(out, syntax::cast<syntax::DictComp>(e)->key);
            pyast_add(out, syntax::cast<syntax::DictComp>(e)->value);
            pyast_add_helpers(out, syntax::cast<syntax::DictComp>(e)->generators, Category::Comprehension);
            break;
        case K::GeneratorExp:
            pyast_add(out, syntax::cast<syntax::GeneratorExp>(e)->elt);
            pyast_add_helpers(out, syntax::cast<syntax::GeneratorExp>(e)->generators, Category::Comprehension);
            break;
        case K::Await: pyast_add(out, syntax::cast<syntax::Await>(e)->value); break;
        case K::Yield: pyast_add(out, syntax::cast<syntax::Yield>(e)->value); break;
        case K::YieldFrom: pyast_add(out, syntax::cast<syntax::YieldFrom>(e)->value); break;
        case K::Compare:
            pyast_add(out, syntax::cast<syntax::Compare>(e)->left);
            pyast_add_all(out, syntax::cast<syntax::Compare>(e)->comparators);
            break;
        case K::Call:
            pyast_add(out, syntax::cast<syntax::Call>(e)->func);
            pyast_add_all(out, syntax::cast<syntax::Call>(e)->args);
            pyast_add_helpers(out, syntax::cast<syntax::Call>(e)->keywords, Category::Keyword);
            break;
        case K::FormattedValue:
            pyast_add(out, syntax::cast<syntax::FormattedValue>(e)->value);
            pyast_add(out, syntax::cast<syntax::FormattedValue>(e)->format_spec);
            break;
        case K::Interpolation:
            pyast_add(out, syntax::cast<syntax::Interpolation>(e)->value);
            pyast_add(out, syntax::cast<syntax::Interpolation>(e)->format_spec);
            break;
        case K::JoinedStr: pyast_add_all(out, syntax::cast<syntax::JoinedStr>(e)->values); break;
        case K::TemplateStr: pyast_add_all(out, syntax::cast<syntax::TemplateStr>(e)->values); break;
        case K::Attribute: pyast_add(out, syntax::cast<syntax::Attribute>(e)->value); break;
        case K::Subscript:
            pyast_add(out, syntax::cast<syntax::Subscript>(e)->value);
            pyast_add(out, syntax::cast<syntax::Subscript>(e)->slice);
            break;
        case K::Starred: pyast_add(out, syntax::cast<syntax::Starred>(e)->value); break;
        case K::List: pyast_add_all(out, syntax::cast<syntax::List>(e)->elts); break;
        case K::Tuple: pyast_add_all(out, syntax::cast<syntax::Tuple>(e)->elts); break;
        case K::Slice:
            pyast_add(out, syntax::cast<syntax::Slice>(e)->lower);
            pyast_add(out, syntax::cast<syntax::Slice>(e)->upper);
            pyast_add(out, syntax::cast<syntax::Slice>(e)->step);
            break;
        case K::Constant:
        case K::Name: break;
    }
}

void pyast_stmt_children(const syntax::Stmt* s, std::vector<AnyNode>& out) {
    using K = syntax::StmtKind;
    switch (s->kind) {
        case K::FunctionDef:
        case K::AsyncFunctionDef: {
            auto* f = syntax::cast<syntax::FunctionDef>(s);
            if (f->args) out.emplace_back(f->args, Category::Arguments);
            pyast_add_all(out, f->body);
            pyast_add_all(out, f->decorator_list);
            pyast_add(out, f->returns);
            pyast_add_helpers(out, f->type_params, Category::TypeParam);
            break;
        }
        case K::ClassDef: {
            auto* c = syntax::cast<syntax::ClassDef>(s);
            pyast_add_all(out, c->bases);
            pyast_add_helpers(out, c->keywords, Category::Keyword);
            pyast_add_all(out, c->body);
            pyast_add_all(out, c->decorator_list);
            pyast_add_helpers(out, c->type_params, Category::TypeParam);
            break;
        }
        case K::Return: pyast_add(out, syntax::cast<syntax::Return>(s)->value); break;
        case K::Delete: pyast_add_all(out, syntax::cast<syntax::Delete>(s)->targets); break;
        case K::Assign:
            pyast_add_all(out, syntax::cast<syntax::Assign>(s)->targets);
            pyast_add(out, syntax::cast<syntax::Assign>(s)->value);
            break;
        case K::TypeAlias:
            pyast_add(out, syntax::cast<syntax::TypeAlias>(s)->name);
            pyast_add_helpers(out, syntax::cast<syntax::TypeAlias>(s)->type_params, Category::TypeParam);
            pyast_add(out, syntax::cast<syntax::TypeAlias>(s)->value);
            break;
        case K::AugAssign:
            pyast_add(out, syntax::cast<syntax::AugAssign>(s)->target);
            pyast_add(out, syntax::cast<syntax::AugAssign>(s)->value);
            break;
        case K::AnnAssign:
            pyast_add(out, syntax::cast<syntax::AnnAssign>(s)->target);
            pyast_add(out, syntax::cast<syntax::AnnAssign>(s)->annotation);
            pyast_add(out, syntax::cast<syntax::AnnAssign>(s)->value);
            break;
        case K::For:
        case K::AsyncFor: {
            auto* f = syntax::cast<syntax::For>(s);
            pyast_add(out, f->target);
            pyast_add(out, f->iter);
            pyast_add_all(out, f->body);
            pyast_add_all(out, f->orelse);
            break;
        }
        case K::While:
            pyast_add(out, syntax::cast<syntax::While>(s)->test);
            pyast_add_all(out, syntax::cast<syntax::While>(s)->body);
            pyast_add_all(out, syntax::cast<syntax::While>(s)->orelse);
            break;
        case K::If:
            pyast_add(out, syntax::cast<syntax::If>(s)->test);
            pyast_add_all(out, syntax::cast<syntax::If>(s)->body);
            pyast_add_all(out, syntax::cast<syntax::If>(s)->orelse);
            break;
        case K::With:
        case K::AsyncWith: {
            auto* w = syntax::cast<syntax::With>(s);
            pyast_add_helpers(out, w->items, Category::WithItem);
            pyast_add_all(out, w->body);
            break;
        }
        case K::Match: {
            auto* m = syntax::cast<syntax::Match>(s);
            pyast_add(out, m->subject);
            pyast_add_helpers(out, m->cases, Category::MatchCase);
            break;
        }
        case K::Raise:
            pyast_add(out, syntax::cast<syntax::Raise>(s)->exc);
            pyast_add(out, syntax::cast<syntax::Raise>(s)->cause);
            break;
        case K::Try:
        case K::TryStar: {
            auto* t = syntax::cast<syntax::Try>(s);
            pyast_add_all(out, t->body);
            pyast_add_helpers(out, t->handlers, Category::Handler);
            pyast_add_all(out, t->orelse);
            pyast_add_all(out, t->finalbody);
            break;
        }
        case K::Assert:
            pyast_add(out, syntax::cast<syntax::Assert>(s)->test);
            pyast_add(out, syntax::cast<syntax::Assert>(s)->msg);
            break;
        case K::Import: pyast_add_helpers(out, syntax::cast<syntax::Import>(s)->names, Category::Alias); break;
        case K::ImportFrom: pyast_add_helpers(out, syntax::cast<syntax::ImportFrom>(s)->names, Category::Alias); break;
        case K::Expr: pyast_add(out, syntax::cast<syntax::ExprStmt>(s)->value); break;
        case K::Global:
        case K::Nonlocal:
        case K::Pass:
        case K::Break:
        case K::Continue: break;
    }
}

void pyast_pattern_children(const syntax::Pattern* p, std::vector<AnyNode>& out) {
    using K = syntax::PatternKind;
    switch (p->kind) {
        case K::MatchValue: pyast_add(out, syntax::cast<syntax::MatchValue>(p)->value); break;
        case K::MatchSequence: pyast_add_patterns(out, syntax::cast<syntax::MatchSequence>(p)->patterns); break;
        case K::MatchMapping:
            pyast_add_all(out, syntax::cast<syntax::MatchMapping>(p)->keys);
            pyast_add_patterns(out, syntax::cast<syntax::MatchMapping>(p)->patterns);
            break;
        case K::MatchClass:
            pyast_add(out, syntax::cast<syntax::MatchClass>(p)->cls);
            pyast_add_patterns(out, syntax::cast<syntax::MatchClass>(p)->patterns);
            pyast_add_patterns(out, syntax::cast<syntax::MatchClass>(p)->kwd_patterns);
            break;
        case K::MatchAs:
            if (auto* inner = syntax::cast<syntax::MatchAs>(p)->pattern) out.emplace_back(inner, Category::Pattern);
            break;
        case K::MatchOr: pyast_add_patterns(out, syntax::cast<syntax::MatchOr>(p)->patterns); break;
        case K::MatchSingleton:
        case K::MatchStar: break;
    }
}

// A string constant's text as Python's repr shows it: single quotes unless the text holds one and no double quote.
std::string pyast_quote(const std::string& text) {
    const bool single = text.find('\'') == std::string::npos || text.find('"') != std::string::npos;
    const char quote = single ? '\'' : '"';
    std::string out(1, quote);
    for (const char c : text) {
        if (c == quote || c == '\\') {
            out += '\\';
            out += c;
        } else if (c == '\n') {
            out += "\\n";
        } else if (c == '\r') {
            out += "\\r";
        } else if (c == '\t') {
            out += "\\t";
        } else {
            out += c;
        }
    }
    out += quote;
    return out;
}

std::string pyast_constant_text(const syntax::ConstValue& v) {
    using K = syntax::ConstValue::Kind;
    switch (v.kind) {
        case K::None: return "None";
        case K::True: return "True";
        case K::False: return "False";
        case K::Ellipsis: return "...";
        case K::Int: return v.text;
        case K::Float: return text::format_double(v.f);
        case K::Complex: return text::format_double(v.f) + "j";
        case K::Str: return pyast_quote(v.text);
        case K::Bytes: return "b" + pyast_quote(v.text);
    }
    return "<Constant>";
}

}  // namespace

void children(AnyNode parent, std::vector<AnyNode>& out) {
    switch (parent.category) {
        case Category::Expr: pyast_expr_children(parent.expr(), out); break;
        case Category::Stmt: pyast_stmt_children(parent.stmt(), out); break;
        case Category::Arguments: {
            auto* a = static_cast<const syntax::arguments*>(parent.node);
            pyast_add_helpers(out, a->posonlyargs, Category::Arg);
            pyast_add_helpers(out, a->args, Category::Arg);
            if (a->vararg) out.emplace_back(a->vararg, Category::Arg);
            pyast_add_helpers(out, a->kwonlyargs, Category::Arg);
            pyast_add_all(out, a->kw_defaults);
            if (a->kwarg) out.emplace_back(a->kwarg, Category::Arg);
            pyast_add_all(out, a->defaults);
            break;
        }
        case Category::Arg: pyast_add(out, static_cast<const syntax::arg*>(parent.node)->annotation); break;
        case Category::Keyword: pyast_add(out, static_cast<const syntax::keyword*>(parent.node)->value); break;
        case Category::WithItem: {
            auto* w = static_cast<const syntax::withitem*>(parent.node);
            pyast_add(out, w->context_expr);
            pyast_add(out, w->optional_vars);
            break;
        }
        case Category::Comprehension: {
            auto* c = static_cast<const syntax::comprehension*>(parent.node);
            pyast_add(out, c->target);
            pyast_add(out, c->iter);
            pyast_add_all(out, c->ifs);
            break;
        }
        case Category::Handler: {
            auto* h = static_cast<const syntax::ExceptHandler*>(parent.node);
            pyast_add(out, h->type);
            pyast_add_all(out, h->body);
            break;
        }
        case Category::MatchCase: {
            auto* m = static_cast<const syntax::match_case*>(parent.node);
            if (m->pattern) out.emplace_back(m->pattern, Category::Pattern);
            pyast_add(out, m->guard);
            pyast_add_all(out, m->body);
            break;
        }
        case Category::Pattern: pyast_pattern_children(static_cast<const syntax::Pattern*>(parent.node), out); break;
        case Category::TypeParam: {
            auto* t = static_cast<const syntax::TypeParam*>(parent.node);
            pyast_add(out, t->bound);
            pyast_add(out, t->default_value);
            break;
        }
        case Category::Alias:
        case Category::None: break;
    }
}

std::vector<AnyNode> walk(AnyNode root) {
    std::vector<AnyNode> out;
    if (!root) return out;
    out.push_back(root);
    for (std::size_t i = 0; i < out.size(); ++i) {
        const AnyNode current = out[i];   // a copy: `children` appends to `out`
        children(current, out);
    }
    return out;
}

std::vector<const std::vector<syntax::Stmt*>*> statement_lists(AnyNode node) {
    std::vector<const std::vector<syntax::Stmt*>*> lists;
    switch (node.category) {
        case Category::Stmt: {
            const syntax::Stmt* s = node.stmt();
            switch (s->kind) {
                case syntax::StmtKind::FunctionDef:
                case syntax::StmtKind::AsyncFunctionDef: lists.push_back(&syntax::cast<syntax::FunctionDef>(s)->body); break;
                case syntax::StmtKind::ClassDef: lists.push_back(&syntax::cast<syntax::ClassDef>(s)->body); break;
                case syntax::StmtKind::For:
                case syntax::StmtKind::AsyncFor:
                    lists.push_back(&syntax::cast<syntax::For>(s)->body);
                    lists.push_back(&syntax::cast<syntax::For>(s)->orelse);
                    break;
                case syntax::StmtKind::While:
                    lists.push_back(&syntax::cast<syntax::While>(s)->body);
                    lists.push_back(&syntax::cast<syntax::While>(s)->orelse);
                    break;
                case syntax::StmtKind::If:
                    lists.push_back(&syntax::cast<syntax::If>(s)->body);
                    lists.push_back(&syntax::cast<syntax::If>(s)->orelse);
                    break;
                case syntax::StmtKind::With:
                case syntax::StmtKind::AsyncWith: lists.push_back(&syntax::cast<syntax::With>(s)->body); break;
                case syntax::StmtKind::Try:
                case syntax::StmtKind::TryStar:
                    lists.push_back(&syntax::cast<syntax::Try>(s)->body);
                    lists.push_back(&syntax::cast<syntax::Try>(s)->orelse);
                    lists.push_back(&syntax::cast<syntax::Try>(s)->finalbody);
                    break;
                default: break;
            }
            break;
        }
        case Category::Handler: lists.push_back(&static_cast<const syntax::ExceptHandler*>(node.node)->body); break;
        case Category::MatchCase: lists.push_back(&static_cast<const syntax::match_case*>(node.node)->body); break;
        default: break;
    }
    return lists;
}

std::set<std::string> bound_names(const syntax::FunctionDef& fn) {
    std::set<std::string> names;
    std::vector<AnyNode> todo;
    for (const syntax::Stmt* s : fn.body) todo.emplace_back(s);
    std::vector<AnyNode> kids;
    while (!todo.empty()) {
        const AnyNode current = todo.back();
        todo.pop_back();
        if (const syntax::Stmt* s = current.stmt()) {
            if (s->kind == syntax::StmtKind::FunctionDef || s->kind == syntax::StmtKind::AsyncFunctionDef) {
                names.insert(syntax::cast<syntax::FunctionDef>(s)->name);
                continue;
            }
            if (s->kind == syntax::StmtKind::ClassDef) {
                names.insert(syntax::cast<syntax::ClassDef>(s)->name);
                continue;
            }
            if (s->kind == syntax::StmtKind::Import || s->kind == syntax::StmtKind::ImportFrom) {
                const auto& aliases = s->kind == syntax::StmtKind::Import ? syntax::cast<syntax::Import>(s)->names
                                                                          : syntax::cast<syntax::ImportFrom>(s)->names;
                for (const syntax::alias* a : aliases) {
                    const std::string& local = a->has_asname && !a->asname.empty() ? a->asname : a->name;
                    names.insert(local.substr(0, local.find('.')));
                }
            }
        } else if (const syntax::Expr* e = current.expr()) {
            if (e->kind == syntax::ExprKind::Lambda) continue;
            if (const auto* name = syntax::as<syntax::Name>(e)) {
                if (name->ctx != syntax::ExprCtx::Load) names.insert(name->id);
            }
            if (e->kind == syntax::ExprKind::ListComp || e->kind == syntax::ExprKind::SetComp ||
                e->kind == syntax::ExprKind::DictComp || e->kind == syntax::ExprKind::GeneratorExp) {
                continue;   // their targets are their own scope
            }
        } else if (current.category == Category::Handler) {
            const auto* h = static_cast<const syntax::ExceptHandler*>(current.node);
            if (h->has_name && !h->name.empty()) names.insert(h->name);
        }
        kids.clear();
        children(current, kids);
        todo.insert(todo.end(), kids.begin(), kids.end());
    }
    return names;
}

std::optional<std::string> string_constant(const syntax::Expr* e) {
    const auto* c = syntax::as<syntax::Constant>(e);
    if (c == nullptr || c->value.kind != syntax::ConstValue::Kind::Str) return std::nullopt;
    return c->value.text;
}

bool is_none_constant(const syntax::Expr* e) {
    const auto* c = syntax::as<syntax::Constant>(e);
    return c != nullptr && c->value.kind == syntax::ConstValue::Kind::None;
}

bool is_ellipsis(const syntax::Expr* e) {
    const auto* c = syntax::as<syntax::Constant>(e);
    return c != nullptr && c->value.kind == syntax::ConstValue::Kind::Ellipsis;
}

std::string describe_expr(const syntax::Expr* e) {
    if (e == nullptr) return "None";
    switch (e->kind) {
        case syntax::ExprKind::Name: return syntax::cast<syntax::Name>(e)->id;
        case syntax::ExprKind::Attribute: {
            const auto* a = syntax::cast<syntax::Attribute>(e);
            return describe_expr(a->value) + "." + a->attr;
        }
        case syntax::ExprKind::Constant: return pyast_constant_text(syntax::cast<syntax::Constant>(e)->value);
        case syntax::ExprKind::Subscript: {
            const auto* s = syntax::cast<syntax::Subscript>(e);
            std::string index = describe_expr(s->slice);
            if (const auto* tuple = syntax::as<syntax::Tuple>(s->slice); tuple != nullptr && !tuple->elts.empty()) {
                std::vector<std::string> parts;
                for (const syntax::Expr* x : tuple->elts) parts.push_back(describe_expr(x));
                index = text::join(parts, ", ");   // a subscript's tuple has no parentheses
            }
            return describe_expr(s->value) + "[" + index + "]";
        }
        case syntax::ExprKind::Tuple: {
            const auto* t = syntax::cast<syntax::Tuple>(e);
            std::vector<std::string> parts;
            for (const syntax::Expr* x : t->elts) parts.push_back(describe_expr(x));
            return "(" + text::join(parts, ", ") + (parts.size() == 1 ? ",)" : ")");
        }
        case syntax::ExprKind::Slice: {
            const auto* s = syntax::cast<syntax::Slice>(e);
            std::string out = (s->lower != nullptr ? describe_expr(s->lower) : "") + ":" +
                              (s->upper != nullptr ? describe_expr(s->upper) : "");
            if (s->step != nullptr) out += ":" + describe_expr(s->step);
            return out;
        }
        case syntax::ExprKind::Call: {
            const auto* c = syntax::cast<syntax::Call>(e);
            std::vector<std::string> parts;
            for (const syntax::Expr* x : c->args) parts.push_back(describe_expr(x));
            for (const syntax::keyword* k : c->keywords) {
                parts.push_back((k->has_arg ? k->arg + "=" : std::string("**")) + describe_expr(k->value));
            }
            return describe_expr(c->func) + "(" + text::join(parts, ", ") + ")";
        }
        default: return std::string("<") + syntax::name_of(e->kind) + ">";
    }
}

}  // namespace cb::translate::frontends::python::pyast
