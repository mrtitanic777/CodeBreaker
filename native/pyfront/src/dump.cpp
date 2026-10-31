// dump.cpp -- the canonical text form of a tree: CPython 3.14's
// ast.dump(tree, include_attributes=...) with its defaults (annotate_fields=True,
// show_empty=False: a field that is None or an empty list is left out, except the value of
// Constant and MatchSingleton).
#include "literals.hpp"
#include "pyfront/pyfront.hpp"

namespace pyfront {

const char* name_of(ExprCtx c) {
    switch (c) {
    case ExprCtx::Load: return "Load";
    case ExprCtx::Store: return "Store";
    case ExprCtx::Del: return "Del";
    }
    return "?";
}
const char* name_of(BoolOpKind k) { return k == BoolOpKind::And ? "And" : "Or"; }
const char* name_of(OpKind k) {
    static const char* n[] = {"Add", "Sub", "Mult", "MatMult", "Div", "Mod", "Pow", "LShift", "RShift", "BitOr", "BitXor", "BitAnd", "FloorDiv"};
    return n[(int)k];
}
const char* name_of(UnaryOpKind k) {
    static const char* n[] = {"Invert", "Not", "UAdd", "USub"};
    return n[(int)k];
}
const char* name_of(CmpOpKind k) {
    static const char* n[] = {"Eq", "NotEq", "Lt", "LtE", "Gt", "GtE", "Is", "IsNot", "In", "NotIn"};
    return n[(int)k];
}
const char* name_of(ExprKind k) {
    static const char* n[] = {"BoolOp", "NamedExpr", "BinOp", "UnaryOp", "Lambda", "IfExp", "Dict", "Set", "ListComp", "SetComp", "DictComp",
                              "GeneratorExp", "Await", "Yield", "YieldFrom", "Compare", "Call", "FormattedValue", "Interpolation",
                              "JoinedStr", "TemplateStr", "Constant", "Attribute", "Subscript", "Starred", "Name", "List", "Tuple", "Slice"};
    return n[(int)k];
}
const char* name_of(StmtKind k) {
    static const char* n[] = {"FunctionDef", "AsyncFunctionDef", "ClassDef", "Return", "Delete", "Assign", "TypeAlias", "AugAssign",
                              "AnnAssign", "For", "AsyncFor", "While", "If", "With", "AsyncWith", "Match", "Raise", "Try", "TryStar", "Assert",
                              "Import", "ImportFrom", "Global", "Nonlocal", "Expr", "Pass", "Break", "Continue"};
    return n[(int)k];
}
const char* name_of(PatternKind k) {
    static const char* n[] = {"MatchValue", "MatchSingleton", "MatchSequence", "MatchMapping", "MatchClass", "MatchStar", "MatchAs", "MatchOr"};
    return n[(int)k];
}

namespace {

class Dumper {
public:
    std::string o;
    bool attrs;
    explicit Dumper(bool a) : attrs(a) {}

    // One node's field list: `Name(f1=..., f2=..., lineno=...)`.
    class F {
    public:
        F(Dumper& d, const char* name) : d(d) { d.o += name; d.o += '('; }
        void sep() { if (!first) d.o += ", "; first = false; d.o += cur; d.o += '='; }
        F& expr(const char* n, const Expr* e) { if (e) { cur = n; sep(); d.expr(e); } return *this; }
        F& exprs(const char* n, const std::vector<Expr*>& v) {
            if (v.empty()) return *this;
            cur = n; sep();
            d.o += '[';
            for (size_t i = 0; i < v.size(); ++i) { if (i) d.o += ", "; if (v[i]) d.expr(v[i]); else d.o += "None"; }
            d.o += ']';
            return *this;
        }
        F& stmts(const char* n, const std::vector<Stmt*>& v) {
            if (v.empty()) return *this;
            cur = n; sep();
            d.o += '[';
            for (size_t i = 0; i < v.size(); ++i) { if (i) d.o += ", "; d.stmt(v[i]); }
            d.o += ']';
            return *this;
        }
        template <class T, class Fn> F& list(const char* n, const std::vector<T>& v, Fn fn) {
            if (v.empty()) return *this;
            cur = n; sep();
            d.o += '[';
            for (size_t i = 0; i < v.size(); ++i) { if (i) d.o += ", "; fn(v[i]); }
            d.o += ']';
            return *this;
        }
        F& str(const char* n, const std::string& s) { cur = n; sep(); d.o += repr_str(s); return *this; }
        F& ostr(const char* n, const std::string& s, bool has) { if (has) str(n, s); return *this; }
        F& raw(const char* n, const std::string& s) { cur = n; sep(); d.o += s; return *this; }
        F& num(const char* n, long long v) { cur = n; sep(); d.o += std::to_string(v); return *this; }
        F& word(const char* n, const char* w) { cur = n; sep(); d.o += w; d.o += "()"; return *this; }
        template <class N> F& node(const char* n, const N* x) { if (x) { cur = n; sep(); d.node(x); } return *this; }
        void end(const Pos* pos) {
            if (pos && d.attrs) {
                num("lineno", pos->lineno);
                num("col_offset", pos->col_offset);
                num("end_lineno", pos->end_lineno);
                num("end_col_offset", pos->end_col_offset);
            }
            d.o += ')';
        }
    private:
        Dumper& d;
        bool first = true;
        const char* cur = "";
    };

    void node(const arguments* a) {
        F f(*this, "arguments");
        auto args = [&](const char* n, const std::vector<arg*>& v) { f.list(n, v, [&](const arg* x) { node(x); }); };
        args("posonlyargs", a->posonlyargs);
        args("args", a->args);
        f.node("vararg", a->vararg);
        args("kwonlyargs", a->kwonlyargs);
        f.exprs("kw_defaults", a->kw_defaults);
        f.node("kwarg", a->kwarg);
        f.exprs("defaults", a->defaults);
        f.end(nullptr);
    }
    void node(const arg* a) {
        F f(*this, "arg");
        f.str("arg", a->arg).expr("annotation", a->annotation).ostr("type_comment", a->type_comment, !a->type_comment.empty());
        f.end(&a->pos);
    }
    void node(const keyword* k) {
        F f(*this, "keyword");
        f.ostr("arg", k->arg, k->has_arg).expr("value", k->value);
        f.end(&k->pos);
    }
    void node(const alias* a) {
        F f(*this, "alias");
        f.str("name", a->name).ostr("asname", a->asname, a->has_asname);
        f.end(&a->pos);
    }
    void node(const withitem* w) {
        F f(*this, "withitem");
        f.expr("context_expr", w->context_expr).expr("optional_vars", w->optional_vars);
        f.end(nullptr);
    }
    void node(const comprehension* c) {
        F f(*this, "comprehension");
        f.expr("target", c->target).expr("iter", c->iter).exprs("ifs", c->ifs).num("is_async", c->is_async);
        f.end(nullptr);
    }
    void node(const ExceptHandler* h) {
        F f(*this, "ExceptHandler");
        f.expr("type", h->type).ostr("name", h->name, h->has_name).stmts("body", h->body);
        f.end(&h->pos);
    }
    void node(const match_case* c) {
        F f(*this, "match_case");
        f.node("pattern", c->pattern).expr("guard", c->guard).stmts("body", c->body);
        f.end(nullptr);
    }
    void node(const TypeParam* t) {
        const char* n = t->kind == TypeParamKind::TypeVar ? "TypeVar" : t->kind == TypeParamKind::ParamSpec ? "ParamSpec" : "TypeVarTuple";
        F f(*this, n);
        f.str("name", t->name);
        if (t->kind == TypeParamKind::TypeVar) f.expr("bound", t->bound);
        f.expr("default_value", t->default_value);
        f.end(&t->pos);
    }
    void node(const Pattern* pt) {
        F f(*this, name_of(pt->kind));
        switch (pt->kind) {
        case PatternKind::MatchValue: f.expr("value", cast<MatchValue>(pt)->value); break;
        case PatternKind::MatchSingleton: f.raw("value", repr_const(cast<MatchSingleton>(pt)->value)); break;
        case PatternKind::MatchSequence: patterns(f, "patterns", cast<MatchSequence>(pt)->patterns); break;
        case PatternKind::MatchMapping: {
            auto* x = cast<MatchMapping>(pt);
            f.exprs("keys", x->keys);
            patterns(f, "patterns", x->patterns);
            f.ostr("rest", x->rest, x->has_rest);
            break;
        }
        case PatternKind::MatchClass: {
            auto* x = cast<MatchClass>(pt);
            f.expr("cls", x->cls);
            patterns(f, "patterns", x->patterns);
            f.list("kwd_attrs", x->kwd_attrs, [&](const std::string& s) { o += repr_str(s); });
            patterns(f, "kwd_patterns", x->kwd_patterns);
            break;
        }
        case PatternKind::MatchStar: f.ostr("name", cast<MatchStar>(pt)->name, cast<MatchStar>(pt)->has_name); break;
        case PatternKind::MatchAs: {
            auto* x = cast<MatchAs>(pt);
            f.node("pattern", x->pattern);
            f.ostr("name", x->name, x->has_name);
            break;
        }
        case PatternKind::MatchOr: patterns(f, "patterns", cast<MatchOr>(pt)->patterns); break;
        }
        f.end(&pt->pos);
    }
    void patterns(F& f, const char* n, const std::vector<Pattern*>& v) {
        f.list(n, v, [&](const Pattern* x) { node(x); });
    }
    void comps(F& f, const std::vector<comprehension*>& v) {
        f.list("generators", v, [&](const comprehension* x) { node(x); });
    }

    void expr(const Expr* e) {
        F f(*this, name_of(e->kind));
        switch (e->kind) {
        case ExprKind::BoolOp: { auto* x = cast<BoolOp>(e); f.word("op", name_of(x->op)).exprs("values", x->values); break; }
        case ExprKind::NamedExpr: { auto* x = cast<NamedExpr>(e); f.expr("target", x->target).expr("value", x->value); break; }
        case ExprKind::BinOp: { auto* x = cast<BinOp>(e); f.expr("left", x->left).word("op", name_of(x->op)).expr("right", x->right); break; }
        case ExprKind::UnaryOp: { auto* x = cast<UnaryOp>(e); f.word("op", name_of(x->op)).expr("operand", x->operand); break; }
        case ExprKind::Lambda: { auto* x = cast<Lambda>(e); f.node("args", x->args).expr("body", x->body); break; }
        case ExprKind::IfExp: { auto* x = cast<IfExp>(e); f.expr("test", x->test).expr("body", x->body).expr("orelse", x->orelse); break; }
        case ExprKind::Dict: { auto* x = cast<Dict>(e); f.exprs("keys", x->keys).exprs("values", x->values); break; }
        case ExprKind::Set: f.exprs("elts", cast<Set>(e)->elts); break;
        case ExprKind::ListComp: { auto* x = cast<ListComp>(e); f.expr("elt", x->elt); comps(f, x->generators); break; }
        case ExprKind::SetComp: { auto* x = cast<SetComp>(e); f.expr("elt", x->elt); comps(f, x->generators); break; }
        case ExprKind::DictComp: { auto* x = cast<DictComp>(e); f.expr("key", x->key).expr("value", x->value); comps(f, x->generators); break; }
        case ExprKind::GeneratorExp: { auto* x = cast<GeneratorExp>(e); f.expr("elt", x->elt); comps(f, x->generators); break; }
        case ExprKind::Await: f.expr("value", cast<Await>(e)->value); break;
        case ExprKind::Yield: f.expr("value", cast<Yield>(e)->value); break;
        case ExprKind::YieldFrom: f.expr("value", cast<YieldFrom>(e)->value); break;
        case ExprKind::Compare: {
            auto* x = cast<Compare>(e);
            f.expr("left", x->left);
            f.list("ops", x->ops, [&](CmpOpKind k) { o += name_of(k); o += "()"; });
            f.exprs("comparators", x->comparators);
            break;
        }
        case ExprKind::Call: {
            auto* x = cast<Call>(e);
            f.expr("func", x->func).exprs("args", x->args);
            f.list("keywords", x->keywords, [&](const keyword* k) { node(k); });
            break;
        }
        case ExprKind::FormattedValue: {
            auto* x = cast<FormattedValue>(e);
            f.expr("value", x->value).num("conversion", x->conversion).expr("format_spec", x->format_spec);
            break;
        }
        case ExprKind::Interpolation: {
            auto* x = cast<Interpolation>(e);
            f.expr("value", x->value).str("str", x->str).num("conversion", x->conversion).expr("format_spec", x->format_spec);
            break;
        }
        case ExprKind::JoinedStr: f.exprs("values", cast<JoinedStr>(e)->values); break;
        case ExprKind::TemplateStr: f.exprs("values", cast<TemplateStr>(e)->values); break;
        case ExprKind::Constant: {
            auto* x = cast<Constant>(e);
            f.raw("value", repr_const(x->value)).ostr("kind", x->kind, !x->kind.empty());
            break;
        }
        case ExprKind::Attribute: { auto* x = cast<Attribute>(e); f.expr("value", x->value).str("attr", x->attr).word("ctx", name_of(x->ctx)); break; }
        case ExprKind::Subscript: { auto* x = cast<Subscript>(e); f.expr("value", x->value).expr("slice", x->slice).word("ctx", name_of(x->ctx)); break; }
        case ExprKind::Starred: { auto* x = cast<Starred>(e); f.expr("value", x->value).word("ctx", name_of(x->ctx)); break; }
        case ExprKind::Name: { auto* x = cast<Name>(e); f.str("id", x->id).word("ctx", name_of(x->ctx)); break; }
        case ExprKind::List: { auto* x = cast<List>(e); f.exprs("elts", x->elts).word("ctx", name_of(x->ctx)); break; }
        case ExprKind::Tuple: { auto* x = cast<Tuple>(e); f.exprs("elts", x->elts).word("ctx", name_of(x->ctx)); break; }
        case ExprKind::Slice: { auto* x = cast<Slice>(e); f.expr("lower", x->lower).expr("upper", x->upper).expr("step", x->step); break; }
        }
        f.end(&e->pos);
    }

    void stmt(const Stmt* s) {
        F f(*this, name_of(s->kind));
        auto tparams = [&](const std::vector<TypeParam*>& v) { f.list("type_params", v, [&](const TypeParam* t) { node(t); }); };
        switch (s->kind) {
        case StmtKind::FunctionDef: case StmtKind::AsyncFunctionDef: {
            auto* x = cast<FunctionDef>(s);
            f.str("name", x->name).node("args", x->args).stmts("body", x->body).exprs("decorator_list", x->decorator_list)
                .expr("returns", x->returns).ostr("type_comment", x->type_comment, !x->type_comment.empty());
            tparams(x->type_params);
            break;
        }
        case StmtKind::ClassDef: {
            auto* x = cast<ClassDef>(s);
            f.str("name", x->name).exprs("bases", x->bases);
            f.list("keywords", x->keywords, [&](const keyword* k) { node(k); });
            f.stmts("body", x->body).exprs("decorator_list", x->decorator_list);
            tparams(x->type_params);
            break;
        }
        case StmtKind::Return: f.expr("value", cast<Return>(s)->value); break;
        case StmtKind::Delete: f.exprs("targets", cast<Delete>(s)->targets); break;
        case StmtKind::Assign: {
            auto* x = cast<Assign>(s);
            f.exprs("targets", x->targets).expr("value", x->value).ostr("type_comment", x->type_comment, !x->type_comment.empty());
            break;
        }
        case StmtKind::TypeAlias: {
            auto* x = cast<TypeAlias>(s);
            f.expr("name", x->name);
            tparams(x->type_params);
            f.expr("value", x->value);
            break;
        }
        case StmtKind::AugAssign: { auto* x = cast<AugAssign>(s); f.expr("target", x->target).word("op", name_of(x->op)).expr("value", x->value); break; }
        case StmtKind::AnnAssign: {
            auto* x = cast<AnnAssign>(s);
            f.expr("target", x->target).expr("annotation", x->annotation).expr("value", x->value).num("simple", x->simple);
            break;
        }
        case StmtKind::For: case StmtKind::AsyncFor: {
            auto* x = cast<For>(s);
            f.expr("target", x->target).expr("iter", x->iter).stmts("body", x->body).stmts("orelse", x->orelse)
                .ostr("type_comment", x->type_comment, !x->type_comment.empty());
            break;
        }
        case StmtKind::While: { auto* x = cast<While>(s); f.expr("test", x->test).stmts("body", x->body).stmts("orelse", x->orelse); break; }
        case StmtKind::If: { auto* x = cast<If>(s); f.expr("test", x->test).stmts("body", x->body).stmts("orelse", x->orelse); break; }
        case StmtKind::With: case StmtKind::AsyncWith: {
            auto* x = cast<With>(s);
            f.list("items", x->items, [&](const withitem* w) { node(w); });
            f.stmts("body", x->body).ostr("type_comment", x->type_comment, !x->type_comment.empty());
            break;
        }
        case StmtKind::Match: {
            auto* x = cast<Match>(s);
            f.expr("subject", x->subject);
            f.list("cases", x->cases, [&](const match_case* c) { node(c); });
            break;
        }
        case StmtKind::Raise: { auto* x = cast<Raise>(s); f.expr("exc", x->exc).expr("cause", x->cause); break; }
        case StmtKind::Try: case StmtKind::TryStar: {
            auto* x = cast<Try>(s);
            f.stmts("body", x->body);
            f.list("handlers", x->handlers, [&](const ExceptHandler* h) { node(h); });
            f.stmts("orelse", x->orelse).stmts("finalbody", x->finalbody);
            break;
        }
        case StmtKind::Assert: { auto* x = cast<Assert>(s); f.expr("test", x->test).expr("msg", x->msg); break; }
        case StmtKind::Import: f.list("names", cast<Import>(s)->names, [&](const alias* a) { node(a); }); break;
        case StmtKind::ImportFrom: {
            auto* x = cast<ImportFrom>(s);
            f.ostr("module", x->module, x->has_module);
            f.list("names", x->names, [&](const alias* a) { node(a); });
            f.num("level", x->level);
            break;
        }
        case StmtKind::Global: f.list("names", cast<Global>(s)->names, [&](const std::string& n) { o += repr_str(n); }); break;
        case StmtKind::Nonlocal: f.list("names", cast<Nonlocal>(s)->names, [&](const std::string& n) { o += repr_str(n); }); break;
        case StmtKind::Expr: f.expr("value", cast<ExprStmt>(s)->value); break;
        case StmtKind::Pass: case StmtKind::Break: case StmtKind::Continue: break;
        }
        f.end(&s->pos);
    }
};

}  // namespace

std::string dump(const Module& m, bool include_attributes) {
    Dumper d(include_attributes);
    d.o.reserve(m.arena.size() * 64);
    Dumper::F f(d, "Module");
    f.stmts("body", m.body);
    f.end(nullptr);
    return std::move(d.o);
}

std::string dump(const Expr* e, bool include_attributes) {
    Dumper d(include_attributes);
    d.expr(e);
    return std::move(d.o);
}

std::string dump(const Stmt* s, bool include_attributes) {
    Dumper d(include_attributes);
    d.stmt(s);
    return std::move(d.o);
}

}  // namespace pyfront
