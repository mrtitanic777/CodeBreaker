// parser.cpp -- pyfront's recursive-descent parser for Python 3.
//
// One method per rule of CPython's Grammar/python.gram (same names where it helps), over a
// token buffer filled on demand.  Every node's position is CPython's EXTRA: from the first
// token its rule consumed to the last one that is not NEWLINE/INDENT/DEDENT.  Assignment,
// `for` and `with` targets are parsed as expressions and then given Store context (or
// refused), which accepts exactly the target grammar.  Two spots backtrack, as the PEG
// grammar does: the parenthesized `with (a as b, ...)` form and the soft keyword `match`.
#include "lexer.hpp"
#include "literals.hpp"
#include "unicode.hpp"
#include "pyfront/pyfront.hpp"

#include <cctype>
#include <cstring>
#include <deque>
#include <optional>

namespace pyfront {

namespace {

class Parser {
public:
    Parser(std::string_view src, Module* mod) : lx(src), m(mod) {}

    void file() {
        while (tok().type != Tok::EndMarker) {
            if (tok().type == Tok::Newline) { ++p; continue; }
            statement(m->body);
        }
    }

    Lexer lx;

private:
    Module* m;
    std::deque<Token> toks;
    size_t p = 0;
    bool lex_failed = false;

    // ------------------------------------------------------------ tokens

    const Token& tok(size_t k = 0) {
        while (toks.size() <= p + k) {
            try {
                toks.push_back(lx.next());
            } catch (ParseFailure&) {
                lex_failed = true;
                throw;
            }
        }
        return toks[p + k];
    }
    bool at_op(Op o, size_t k = 0) { const Token& t = tok(k); return t.type == Tok::Op && t.op == o; }
    bool at_kw(Kw w, size_t k = 0) { const Token& t = tok(k); return t.type == Tok::Name && t.kw == w; }
    bool at_name(size_t k = 0) { const Token& t = tok(k); return t.type == Tok::Name && t.kw == Kw::None; }
    bool at_soft(std::string_view w, size_t k = 0) { return at_name(k) && lx.text(tok(k)) == w; }
    std::string_view text(const Token& t) { return lx.text(t); }

    [[noreturn]] void error_at(const Token& t, const std::string& msg) {
        lx.fail(msg, t.lineno, t.col, t.end_lineno, t.end_col);
    }
    [[noreturn]] void error_node(const Node* n, const std::string& msg) {
        lx.fail(msg, n->pos.lineno, n->pos.col_offset, n->pos.end_lineno, n->pos.end_col_offset);
    }
    [[noreturn]] void invalid() {
        const Token& t = tok();
        if (t.type == Tok::Indent) unexpected_indent(t);
        error_at(t, "invalid syntax");
    }
    // CPython puts the caret of "unexpected indent" on the last character of the indentation.
    [[noreturn]] void unexpected_indent(const Token& t) {
        lx.fail("unexpected indent", t.end_lineno, t.end_col > 0 ? t.end_col - 1 : 0);
    }
    void expect_op(Op o, const char* what) {
        if (!at_op(o)) {
            const Token& t = tok();
            if (o == Op::Colon || o == Op::Equal || o == Op::RPar || o == Op::RSqb || o == Op::RBrace) {
                if (t.type == Tok::Newline || t.type == Tok::Op || t.type == Tok::Name)
                    error_at(t, std::string("expected '") + what + "'");
            }
            invalid();
        }
        ++p;
    }
    void expect_kw(Kw w) {
        if (!at_kw(w)) invalid();
        ++p;
    }
    Token take() { tok(); return toks[p++]; }
    std::string name() {
        if (!at_name()) invalid();
        return ident(take());
    }
    // An identifier: NFKC-normalized when it is not ASCII, as CPython stores it.
    std::string ident(const Token& t) {
        std::string_view s = text(t);
        for (char c : s) {
            if ((unsigned char)c >= 0x80) {
                std::string out;
                uni::nfkc(s, out);
                return out;
            }
        }
        return std::string(s);
    }

    // ------------------------------------------------------------ positions

    static Pos tpos(const Token& t) { return Pos{t.lineno, t.col, t.end_lineno, t.end_col}; }

    template <class N> N* fin(N* n, size_t s) {
        size_t e = p - 1;
        while (e > s && (toks[e].type == Tok::Newline || toks[e].type == Tok::Indent || toks[e].type == Tok::Dedent || toks[e].type == Tok::EndMarker)) --e;
        const Token& a = toks[s];
        const Token& b = toks[e];
        n->pos = Pos{a.lineno, a.col, b.end_lineno, b.end_col};
        return n;
    }
    template <class T> T* mk() { return m->make<T>(); }

    // ------------------------------------------------------------ helpers

    bool starts_expr(size_t k = 0) {
        const Token& t = tok(k);
        switch (t.type) {
        case Tok::Name:
            switch (t.kw) {
            case Kw::None: case Kw::False_: case Kw::None_: case Kw::True_: case Kw::Not: case Kw::Lambda: case Kw::Await:
                return true;
            default:
                return false;
            }
        case Tok::Number: case Tok::String: case Tok::FStringStart:
            return true;
        case Tok::Op:
            switch (t.op) {
            case Op::LPar: case Op::LSqb: case Op::LBrace: case Op::Minus: case Op::Plus: case Op::Tilde: case Op::Ellipsis: case Op::Star:
                return true;
            default:
                return false;
            }
        default:
            return false;
        }
    }

    static const char* expr_desc(const Expr* e) {
        switch (e->kind) {
        case ExprKind::Call: return "function call";
        case ExprKind::Constant: {
            auto* c = cast<Constant>(e);
            switch (c->value.kind) {
            case ConstValue::Kind::None: return "None";
            case ConstValue::Kind::True: return "True";
            case ConstValue::Kind::False: return "False";
            case ConstValue::Kind::Ellipsis: return "ellipsis";
            default: return "literal";
            }
        }
        case ExprKind::BoolOp: case ExprKind::BinOp: case ExprKind::UnaryOp: return "expression";
        case ExprKind::Compare: return "comparison";
        case ExprKind::Lambda: return "lambda";
        case ExprKind::IfExp: return "conditional expression";
        case ExprKind::NamedExpr: return "named expression";
        case ExprKind::Dict: return "dict literal";
        case ExprKind::Set: return "set display";
        case ExprKind::ListComp: return "list comprehension";
        case ExprKind::SetComp: return "set comprehension";
        case ExprKind::DictComp: return "dict comprehension";
        case ExprKind::GeneratorExp: return "generator expression";
        case ExprKind::Await: return "await expression";
        case ExprKind::Yield: case ExprKind::YieldFrom: return "yield expression";
        case ExprKind::JoinedStr: case ExprKind::FormattedValue: return "f-string expression";
        default: return "expression";
        }
    }

    // Give a parsed expression target context; refuse what is not a target.
    void set_ctx(Expr* e, ExprCtx c, bool allow_seq = true) {
        switch (e->kind) {
        case ExprKind::Name: cast<Name>(e)->ctx = c; return;
        case ExprKind::Attribute: cast<Attribute>(e)->ctx = c; return;
        case ExprKind::Subscript: cast<Subscript>(e)->ctx = c; return;
        case ExprKind::Starred:
            if (allow_seq) { cast<Starred>(e)->ctx = c; set_ctx(cast<Starred>(e)->value, c, true); return; }
            break;
        case ExprKind::Tuple:
            if (allow_seq) { cast<Tuple>(e)->ctx = c; for (Expr* x : cast<Tuple>(e)->elts) set_ctx(x, c, true); return; }
            break;
        case ExprKind::List:
            if (allow_seq) { cast<List>(e)->ctx = c; for (Expr* x : cast<List>(e)->elts) set_ctx(x, c, true); return; }
            break;
        default:
            break;
        }
        std::string verb = c == ExprCtx::Del ? "cannot delete " : "cannot assign to ";
        if (!allow_seq && (e->kind == ExprKind::Tuple || e->kind == ExprKind::List || e->kind == ExprKind::Starred))
            error_node(e, std::string("only single target (not ") + (e->kind == ExprKind::List ? "list" : "tuple") + ") can be annotated");
        error_node(e, verb + expr_desc(e));
    }

    // ------------------------------------------------------------ statements

    void statement(std::vector<Stmt*>& out) {
        const Token& t = tok();
        if (t.type == Tok::Name) {
            switch (t.kw) {
            case Kw::Def: out.push_back(funcdef({}, p)); return;
            case Kw::If: out.push_back(if_stmt()); return;
            case Kw::Class: out.push_back(classdef({}, p)); return;
            case Kw::With: out.push_back(with_stmt()); return;
            case Kw::For: out.push_back(for_stmt()); return;
            case Kw::Try: out.push_back(try_stmt()); return;
            case Kw::While: out.push_back(while_stmt()); return;
            case Kw::Async:
                if (at_kw(Kw::Def, 1)) { out.push_back(funcdef({}, p)); return; }
                if (at_kw(Kw::With, 1)) { out.push_back(with_stmt()); return; }
                if (at_kw(Kw::For, 1)) { out.push_back(for_stmt()); return; }
                invalid();
            case Kw::None:
                if (text(t) == "match") {
                    if (Stmt* s = match_stmt()) { out.push_back(s); return; }
                }
                break;
            default:
                break;
            }
        } else if (t.type == Tok::Op && t.op == Op::At) {
            out.push_back(decorated());
            return;
        } else if (t.type == Tok::Indent) {
            unexpected_indent(t);
        }
        simple_stmts(out);
    }

    void simple_stmts(std::vector<Stmt*>& out) {
        out.push_back(simple_stmt());
        while (at_op(Op::Semi)) {
            ++p;
            if (tok().type == Tok::Newline) break;
            out.push_back(simple_stmt());
        }
        if (tok().type != Tok::Newline) invalid();
        ++p;
    }

    std::vector<Stmt*> block(const char* what, int line) {
        std::vector<Stmt*> body;
        if (tok().type == Tok::Newline) {
            ++p;
            if (tok().type != Tok::Indent)
                error_at(tok(), std::string("expected an indented block after ") + what + " on line " + std::to_string(line));
            ++p;
            while (tok().type != Tok::Dedent && tok().type != Tok::EndMarker) {
                if (tok().type == Tok::Newline) { ++p; continue; }
                statement(body);
            }
            if (tok().type == Tok::Dedent) ++p;
        } else {
            simple_stmts(body);
        }
        return body;
    }

    Stmt* simple_stmt() {
        size_t s = p;
        const Token& t = tok();
        if (t.type == Tok::Name) {
            switch (t.kw) {
            case Kw::Return: {
                ++p;
                auto* r = mk<Return>();
                if (starts_expr()) r->value = star_expressions();
                return fin(r, s);
            }
            case Kw::Import: return import_name();
            case Kw::From: return import_from();
            case Kw::Raise: {
                ++p;
                auto* r = mk<Raise>();
                if (starts_expr()) {
                    r->exc = expression();
                    if (at_kw(Kw::From)) { ++p; r->cause = expression(); }
                }
                return fin(r, s);
            }
            case Kw::Pass: ++p; return fin(mk<Pass>(), s);
            case Kw::Break: ++p; return fin(mk<Break>(), s);
            case Kw::Continue: ++p; return fin(mk<Continue>(), s);
            case Kw::Del: {
                ++p;
                auto* d = mk<Delete>();
                for (;;) {
                    Expr* e = primary_target();
                    set_ctx(e, ExprCtx::Del);
                    d->targets.push_back(e);
                    if (!at_op(Op::Comma)) break;
                    ++p;
                    if (tok().type == Tok::Newline || at_op(Op::Semi)) break;
                }
                return fin(d, s);
            }
            case Kw::Global: case Kw::Nonlocal: {
                bool g = t.kw == Kw::Global;
                ++p;
                std::vector<std::string> names{name()};
                while (at_op(Op::Comma)) { ++p; names.push_back(name()); }
                if (g) { auto* n = mk<Global>(); n->names = std::move(names); return fin(n, s); }
                auto* n = mk<Nonlocal>();
                n->names = std::move(names);
                return fin(n, s);
            }
            case Kw::Assert: {
                ++p;
                auto* a = mk<Assert>();
                a->test = expression();
                if (at_op(Op::Comma)) { ++p; a->msg = expression(); }
                return fin(a, s);
            }
            case Kw::None:
                if (text(t) == "type" && at_name(1) && (at_op(Op::Equal, 2) || at_op(Op::LSqb, 2))) return type_alias();
                break;
            default:
                break;
            }
        }
        Expr* first = at_kw(Kw::Yield) ? yield_expr() : star_expressions();
        if (at_op(Op::Colon)) {
            set_ctx(first, ExprCtx::Store, false);
            ++p;
            auto* a = mk<AnnAssign>();
            a->target = first;
            a->simple = (first->kind == ExprKind::Name && !(toks[s].type == Tok::Op && toks[s].op == Op::LPar)) ? 1 : 0;
            a->annotation = expression();
            if (at_op(Op::Equal)) {
                ++p;
                a->value = at_kw(Kw::Yield) ? yield_expr() : star_expressions();
            }
            return fin(a, s);
        }
        if (at_op(Op::Equal)) {
            auto* a = mk<Assign>();
            a->targets.push_back(first);
            for (;;) {
                ++p;
                Expr* v = at_kw(Kw::Yield) ? yield_expr() : star_expressions();
                if (at_op(Op::Equal)) { a->targets.push_back(v); continue; }
                a->value = v;
                break;
            }
            for (Expr* tg : a->targets) set_ctx(tg, ExprCtx::Store);
            return fin(a, s);
        }
        if (tok().type == Tok::Op) {
            OpKind k;
            bool aug = true;
            switch (tok().op) {
            case Op::PlusEqual: k = OpKind::Add; break;
            case Op::MinEqual: k = OpKind::Sub; break;
            case Op::StarEqual: k = OpKind::Mult; break;
            case Op::AtEqual: k = OpKind::MatMult; break;
            case Op::SlashEqual: k = OpKind::Div; break;
            case Op::PercentEqual: k = OpKind::Mod; break;
            case Op::AmperEqual: k = OpKind::BitAnd; break;
            case Op::VBarEqual: k = OpKind::BitOr; break;
            case Op::CircumflexEqual: k = OpKind::BitXor; break;
            case Op::LeftShiftEqual: k = OpKind::LShift; break;
            case Op::RightShiftEqual: k = OpKind::RShift; break;
            case Op::DoubleStarEqual: k = OpKind::Pow; break;
            case Op::DoubleSlashEqual: k = OpKind::FloorDiv; break;
            default: aug = false; k = OpKind::Add; break;
            }
            if (aug) {
                if (first->kind != ExprKind::Name && first->kind != ExprKind::Attribute && first->kind != ExprKind::Subscript)
                    error_node(first, std::string("'") + expr_desc(first) + "' is an illegal expression for augmented assignment");
                set_ctx(first, ExprCtx::Store, false);
                ++p;
                auto* a = mk<AugAssign>();
                a->target = first;
                a->op = k;
                a->value = at_kw(Kw::Yield) ? yield_expr() : star_expressions();
                return fin(a, s);
            }
        }
        auto* e = mk<ExprStmt>();
        e->value = first;
        return fin(e, s);
    }

    Stmt* type_alias() {
        size_t s = p;
        ++p;  // 'type'
        auto* a = mk<TypeAlias>();
        auto* n = mk<Name>();
        n->pos = tpos(tok());
        n->id = name();
        n->ctx = ExprCtx::Store;
        a->name = n;
        if (at_op(Op::LSqb)) a->type_params = type_params();
        expect_op(Op::Equal, "=");
        a->value = expression();
        return fin(a, s);
    }

    std::string dotted_name() {
        std::string n = name();
        while (at_op(Op::Dot)) {
            ++p;
            n += '.';
            n += name();
        }
        return n;
    }

    Stmt* import_name() {
        size_t s = p;
        ++p;
        auto* im = mk<Import>();
        for (;;) {
            size_t as = p;
            auto* a = mk<alias>();
            a->name = dotted_name();
            if (at_kw(Kw::As)) { ++p; a->asname = name(); a->has_asname = true; }
            im->names.push_back(fin(a, as));
            if (!at_op(Op::Comma)) break;
            ++p;
        }
        return fin(im, s);
    }

    Stmt* import_from() {
        size_t s = p;
        ++p;
        auto* im = mk<ImportFrom>();
        for (;;) {
            if (at_op(Op::Dot)) { ++p; im->level += 1; }
            else if (at_op(Op::Ellipsis)) { ++p; im->level += 3; }
            else break;
        }
        if (!at_kw(Kw::Import)) {
            im->module = dotted_name();
            im->has_module = true;
        }
        expect_kw(Kw::Import);
        if (at_op(Op::Star)) {
            size_t as = p;
            ++p;
            auto* a = mk<alias>();
            a->name = "*";
            im->names.push_back(fin(a, as));
            return fin(im, s);
        }
        bool paren = at_op(Op::LPar);
        if (paren) ++p;
        for (;;) {
            size_t as = p;
            auto* a = mk<alias>();
            a->name = name();
            if (at_kw(Kw::As)) { ++p; a->asname = name(); a->has_asname = true; }
            im->names.push_back(fin(a, as));
            if (!at_op(Op::Comma)) break;
            ++p;
            if (paren && at_op(Op::RPar)) break;
            if (!paren && !at_name()) {
                error_at(tok(), "trailing comma not allowed without surrounding parentheses");
            }
        }
        if (paren) expect_op(Op::RPar, ")");
        return fin(im, s);
    }

    Stmt* if_stmt() {
        size_t s = p;
        int line = tok().lineno;
        bool is_elif = at_kw(Kw::Elif);
        ++p;
        auto* n = mk<If>();
        n->test = named_expression();
        expect_op(Op::Colon, ":");
        n->body = block(is_elif ? "'elif' statement" : "'if' statement", line);
        if (at_kw(Kw::Elif)) {
            n->orelse.push_back(if_stmt());
        } else if (at_kw(Kw::Else)) {
            int el = tok().lineno;
            ++p;
            expect_op(Op::Colon, ":");
            n->orelse = block("'else' statement", el);
        }
        return fin(n, s);
    }

    Stmt* while_stmt() {
        size_t s = p;
        int line = tok().lineno;
        ++p;
        auto* n = mk<While>();
        n->test = named_expression();
        expect_op(Op::Colon, ":");
        n->body = block("'while' statement", line);
        if (at_kw(Kw::Else)) {
            int el = tok().lineno;
            ++p;
            expect_op(Op::Colon, ":");
            n->orelse = block("'else' statement", el);
        }
        return fin(n, s);
    }

    Stmt* for_stmt() {
        size_t s = p;
        int line = tok().lineno;
        bool is_async = at_kw(Kw::Async);
        if (is_async) ++p;
        ++p;  // for
        auto* n = m->make<For>(is_async);
        n->target = star_targets();
        expect_kw(Kw::In);
        n->iter = star_expressions();
        expect_op(Op::Colon, ":");
        n->body = block("'for' statement", line);
        if (at_kw(Kw::Else)) {
            int el = tok().lineno;
            ++p;
            expect_op(Op::Colon, ":");
            n->orelse = block("'else' statement", el);
        }
        return fin(n, s);
    }

    withitem* with_item() {
        auto* w = mk<withitem>();
        w->context_expr = expression();
        if (at_kw(Kw::As)) {
            ++p;
            w->optional_vars = star_target();
            set_ctx(w->optional_vars, ExprCtx::Store);
        }
        return w;
    }

    Stmt* with_stmt() {
        size_t s = p;
        int line = tok().lineno;
        bool is_async = at_kw(Kw::Async);
        if (is_async) ++p;
        ++p;  // with
        auto* n = m->make<With>(is_async);
        bool done = false;
        if (at_op(Op::LPar)) {
            size_t save = p;
            try {
                ++p;
                std::vector<withitem*> items;
                for (;;) {
                    items.push_back(with_item());
                    if (!at_op(Op::Comma)) break;
                    ++p;
                    if (at_op(Op::RPar)) break;
                }
                if (at_op(Op::RPar) && at_op(Op::Colon, 1)) {
                    ++p;
                    n->items = std::move(items);
                    done = true;
                } else {
                    p = save;
                }
            } catch (ParseFailure&) {
                if (lex_failed) throw;
                p = save;
            }
        }
        if (!done) {
            for (;;) {
                n->items.push_back(with_item());
                if (!at_op(Op::Comma)) break;
                ++p;
            }
        }
        expect_op(Op::Colon, ":");
        n->body = block("'with' statement", line);
        return fin(n, s);
    }

    Stmt* try_stmt() {
        size_t s = p;
        int line = tok().lineno;
        ++p;
        expect_op(Op::Colon, ":");
        auto body = block("'try' statement", line);
        bool star = false;
        std::vector<ExceptHandler*> handlers;
        while (at_kw(Kw::Except)) {
            size_t hs = p;
            int hl = tok().lineno;
            ++p;
            if (at_op(Op::Star)) { ++p; star = true; }
            auto* h = mk<ExceptHandler>();
            if (!at_op(Op::Colon)) {
                size_t ts = p;
                h->type = expression();
                if (at_op(Op::Comma)) {
                    auto* t = mk<Tuple>();
                    t->elts.push_back(h->type);
                    while (at_op(Op::Comma)) {
                        ++p;
                        if (at_op(Op::Colon)) break;
                        t->elts.push_back(expression());
                    }
                    h->type = fin(t, ts);
                }
                if (at_kw(Kw::As)) {
                    ++p;
                    h->name = name();
                    h->has_name = true;
                }
            }
            expect_op(Op::Colon, ":");
            h->body = block(star ? "'except*' statement" : "'except' statement", hl);
            handlers.push_back(fin(h, hs));
        }
        auto* n = m->make<Try>(star);
        n->body = std::move(body);
        n->handlers = std::move(handlers);
        if (at_kw(Kw::Else)) {
            int el = tok().lineno;
            ++p;
            expect_op(Op::Colon, ":");
            n->orelse = block("'else' statement", el);
        }
        if (at_kw(Kw::Finally)) {
            int fl = tok().lineno;
            ++p;
            expect_op(Op::Colon, ":");
            n->finalbody = block("'finally' statement", fl);
        }
        if (n->handlers.empty() && n->finalbody.empty()) error_at(tok(), "expected 'except' or 'finally' block");
        return fin(n, s);
    }

    Stmt* decorated() {
        std::vector<Expr*> decos;
        while (at_op(Op::At)) {
            ++p;
            decos.push_back(named_expression());
            if (tok().type != Tok::Newline) invalid();
            ++p;
        }
        size_t s = p;
        if (at_kw(Kw::Def) || (at_kw(Kw::Async) && at_kw(Kw::Def, 1))) return funcdef(std::move(decos), s);
        if (at_kw(Kw::Class)) return classdef(std::move(decos), s);
        invalid();
    }

    Stmt* funcdef(std::vector<Expr*> decos, size_t s) {
        int line = tok().lineno;
        bool is_async = at_kw(Kw::Async);
        if (is_async) ++p;
        ++p;  // def
        auto* f = m->make<FunctionDef>(is_async);
        f->decorator_list = std::move(decos);
        f->name = name();
        if (at_op(Op::LSqb)) f->type_params = type_params();
        expect_op(Op::LPar, "(");
        f->args = parameters(false, Op::RPar);
        expect_op(Op::RPar, ")");
        if (at_op(Op::RArrow)) {
            ++p;
            f->returns = expression();
        }
        expect_op(Op::Colon, ":");
        f->body = block("function definition", line);
        return fin(f, s);
    }

    Stmt* classdef(std::vector<Expr*> decos, size_t s) {
        int line = tok().lineno;
        ++p;  // class
        auto* c = mk<ClassDef>();
        c->decorator_list = std::move(decos);
        c->name = name();
        if (at_op(Op::LSqb)) c->type_params = type_params();
        if (at_op(Op::LPar)) {
            ++p;
            call_args(c->bases, c->keywords, nullptr);
        }
        expect_op(Op::Colon, ":");
        c->body = block("class definition", line);
        return fin(c, s);
    }

    std::vector<TypeParam*> type_params() {
        ++p;  // [
        std::vector<TypeParam*> out;
        while (!at_op(Op::RSqb)) {
            size_t s = p;
            TypeParam* t;
            if (at_op(Op::Star)) {
                ++p;
                t = m->make<TypeParam>(TypeParamKind::TypeVarTuple);
                t->name = name();
                if (at_op(Op::Equal)) { ++p; t->default_value = star_expression(); }
            } else if (at_op(Op::DoubleStar)) {
                ++p;
                t = m->make<TypeParam>(TypeParamKind::ParamSpec);
                t->name = name();
                if (at_op(Op::Equal)) { ++p; t->default_value = expression(); }
            } else {
                t = m->make<TypeParam>(TypeParamKind::TypeVar);
                t->name = name();
                if (at_op(Op::Colon)) { ++p; t->bound = expression(); }
                if (at_op(Op::Equal)) { ++p; t->default_value = expression(); }
            }
            out.push_back(fin(t, s));
            if (!at_op(Op::Comma)) break;
            ++p;
        }
        expect_op(Op::RSqb, "]");
        return out;
    }

    // def/lambda parameters up to (not including) `close`.
    arguments* parameters(bool is_lambda, Op close) {
        auto* a = mk<arguments>();
        bool star = false;
        while (!at_op(close)) {
            if (at_op(Op::Slash)) {
                ++p;
                a->posonlyargs = std::move(a->args);
                a->args.clear();
            } else if (at_op(Op::Star)) {
                ++p;
                star = true;
                if (at_name()) a->vararg = param(is_lambda, true);
            } else if (at_op(Op::DoubleStar)) {
                ++p;
                a->kwarg = param(is_lambda, false);
            } else {
                arg* ar = param(is_lambda, false);
                Expr* d = nullptr;
                if (at_op(Op::Equal)) { ++p; d = expression(); }
                if (star) {
                    a->kwonlyargs.push_back(ar);
                    a->kw_defaults.push_back(d);
                } else {
                    a->args.push_back(ar);
                    if (d) a->defaults.push_back(d);
                    else if (!a->defaults.empty()) error_node(ar, "parameter without a default follows parameter with a default");
                }
            }
            if (!at_op(Op::Comma)) break;
            ++p;
        }
        return a;
    }

    arg* param(bool is_lambda, bool star_annotation) {
        size_t s = p;
        auto* a = mk<arg>();
        a->arg = name();
        if (!is_lambda && at_op(Op::Colon)) {
            ++p;
            a->annotation = (star_annotation && at_op(Op::Star)) ? star_expression() : expression();
        }
        return fin(a, s);
    }

    // ------------------------------------------------------------ match

    Stmt* match_stmt() {
        size_t s = p;
        Expr* subject = nullptr;
        try {
            ++p;  // 'match'
            size_t ss = p;
            subject = star_named_expression();
            if (at_op(Op::Comma)) {
                auto* t = mk<Tuple>();
                t->elts.push_back(subject);
                while (at_op(Op::Comma)) {
                    ++p;
                    if (at_op(Op::Colon)) break;
                    t->elts.push_back(star_named_expression());
                }
                subject = fin(t, ss);
            }
            if (!(at_op(Op::Colon) && tok(1).type == Tok::Newline && tok(2).type == Tok::Indent && at_soft("case", 3))) {
                p = s;
                return nullptr;
            }
        } catch (ParseFailure&) {
            if (lex_failed) throw;
            p = s;
            return nullptr;
        }
        p += 3;
        auto* mt = mk<Match>();
        mt->subject = subject;
        while (at_soft("case")) {
            int line = tok().lineno;
            ++p;
            auto* c = mk<match_case>();
            c->pattern = patterns();
            if (at_kw(Kw::If)) { ++p; c->guard = named_expression(); }
            expect_op(Op::Colon, ":");
            c->body = block("'case' statement", line);
            mt->cases.push_back(c);
        }
        if (tok().type != Tok::Dedent) invalid();
        ++p;
        return fin(mt, s);
    }

    Pattern* patterns() {
        size_t s = p;
        Pattern* first = maybe_star_pattern();
        if (!at_op(Op::Comma)) {
            if (first->kind == PatternKind::MatchStar) {
                auto* q = mk<MatchSequence>();
                q->patterns.push_back(first);
                return fin(q, s);
            }
            return first;
        }
        auto* q = mk<MatchSequence>();
        q->patterns.push_back(first);
        while (at_op(Op::Comma)) {
            ++p;
            if (at_op(Op::Colon) || at_kw(Kw::If)) break;
            q->patterns.push_back(maybe_star_pattern());
        }
        return fin(q, s);
    }

    Pattern* maybe_star_pattern() {
        if (at_op(Op::Star)) {
            size_t s = p;
            ++p;
            auto* st = mk<MatchStar>();
            std::string n = name();
            if (n != "_") { st->name = n; st->has_name = true; }
            return fin(st, s);
        }
        return pattern();
    }

    Pattern* pattern() {
        size_t s = p;
        Pattern* o = or_pattern();
        if (at_kw(Kw::As)) {
            ++p;
            auto* a = mk<MatchAs>();
            a->pattern = o;
            const Token& t = tok();
            std::string n = name();
            if (n == "_") error_at(t, "cannot use '_' as a target");
            a->name = n;
            a->has_name = true;
            return fin(a, s);
        }
        return o;
    }

    Pattern* or_pattern() {
        size_t s = p;
        Pattern* first = closed_pattern();
        if (!at_op(Op::VBar)) return first;
        auto* o = mk<MatchOr>();
        o->patterns.push_back(first);
        while (at_op(Op::VBar)) {
            ++p;
            o->patterns.push_back(closed_pattern());
        }
        return fin(o, s);
    }

    // signed_number, complex_number, strings, None/True/False as expressions (for literal
    // patterns and mapping keys).
    Expr* literal_expr() {
        size_t s = p;
        if (tok().type == Tok::String || tok().type == Tok::FStringStart) return strings();
        Expr* e;
        if (at_op(Op::Minus)) {
            ++p;
            if (tok().type != Tok::Number) invalid();
            auto* u = mk<UnaryOp>();
            u->op = UnaryOpKind::USub;
            u->operand = atom();
            e = fin(u, s);
        } else if (tok().type == Tok::Number) {
            e = atom();
        } else {
            invalid();
        }
        if (at_op(Op::Plus) || at_op(Op::Minus)) {
            OpKind k = at_op(Op::Plus) ? OpKind::Add : OpKind::Sub;
            ++p;
            if (tok().type != Tok::Number) invalid();
            auto* b = mk<BinOp>();
            b->left = e;
            b->op = k;
            b->right = atom();
            if (auto* c = as<Constant>(b->right); !c || c->value.kind != ConstValue::Kind::Complex)
                error_node(b->right, "imaginary number required in complex literal");
            e = fin(b, s);
        }
        return e;
    }

    Expr* name_or_attr() {
        size_t s = p;
        auto* n = mk<Name>();
        n->pos = tpos(tok());
        n->id = name();
        Expr* e = n;
        while (at_op(Op::Dot)) {
            ++p;
            auto* a = mk<Attribute>();
            a->value = e;
            a->attr = name();
            e = fin(a, s);
        }
        return e;
    }

    Pattern* closed_pattern() {
        size_t s = p;
        const Token& t = tok();
        if (t.type == Tok::Number || at_op(Op::Minus) || t.type == Tok::String || t.type == Tok::FStringStart) {
            auto* v = mk<MatchValue>();
            v->value = literal_expr();
            return fin(v, s);
        }
        if (t.type == Tok::Name && (t.kw == Kw::None_ || t.kw == Kw::True_ || t.kw == Kw::False_)) {
            auto* v = mk<MatchSingleton>();
            v->value.kind = t.kw == Kw::None_ ? ConstValue::Kind::None : t.kw == Kw::True_ ? ConstValue::Kind::True : ConstValue::Kind::False;
            ++p;
            return fin(v, s);
        }
        if (at_name()) {
            Expr* e = name_or_attr();
            if (at_op(Op::LPar)) return class_pattern(e, s);
            if (e->kind == ExprKind::Attribute) {
                auto* v = mk<MatchValue>();
                v->value = e;
                return fin(v, s);
            }
            auto* a = mk<MatchAs>();
            const std::string& id = cast<Name>(e)->id;
            if (id != "_") { a->name = id; a->has_name = true; }
            return fin(a, s);
        }
        if (at_op(Op::LPar)) {
            ++p;
            if (at_op(Op::RPar)) { ++p; return fin(mk<MatchSequence>(), s); }
            Pattern* first = maybe_star_pattern();
            if (at_op(Op::RPar) && first->kind != PatternKind::MatchStar) { ++p; return first; }
            auto* q = mk<MatchSequence>();
            q->patterns.push_back(first);
            while (at_op(Op::Comma)) {
                ++p;
                if (at_op(Op::RPar)) break;
                q->patterns.push_back(maybe_star_pattern());
            }
            expect_op(Op::RPar, ")");
            return fin(q, s);
        }
        if (at_op(Op::LSqb)) {
            ++p;
            auto* q = mk<MatchSequence>();
            while (!at_op(Op::RSqb)) {
                q->patterns.push_back(maybe_star_pattern());
                if (!at_op(Op::Comma)) break;
                ++p;
            }
            expect_op(Op::RSqb, "]");
            return fin(q, s);
        }
        if (at_op(Op::LBrace)) {
            ++p;
            auto* mm = mk<MatchMapping>();
            while (!at_op(Op::RBrace)) {
                if (at_op(Op::DoubleStar)) {
                    ++p;
                    mm->rest = name();
                    mm->has_rest = true;
                } else {
                    Expr* key;
                    const Token& k = tok();
                    if (k.type == Tok::Name && (k.kw == Kw::None_ || k.kw == Kw::True_ || k.kw == Kw::False_)) key = atom();
                    else if (at_name()) key = name_or_attr();
                    else key = literal_expr();
                    mm->keys.push_back(key);
                    expect_op(Op::Colon, ":");
                    mm->patterns.push_back(pattern());
                }
                if (!at_op(Op::Comma)) break;
                ++p;
            }
            expect_op(Op::RBrace, "}");
            return fin(mm, s);
        }
        invalid();
    }

    Pattern* class_pattern(Expr* cls, size_t s) {
        ++p;  // (
        auto* c = mk<MatchClass>();
        c->cls = cls;
        while (!at_op(Op::RPar)) {
            if (at_name() && at_op(Op::Equal, 1)) {
                c->kwd_attrs.push_back(name());
                ++p;
                c->kwd_patterns.push_back(pattern());
            } else {
                if (!c->kwd_attrs.empty()) error_at(tok(), "positional patterns follow keyword patterns");
                c->patterns.push_back(pattern());
            }
            if (!at_op(Op::Comma)) break;
            ++p;
        }
        expect_op(Op::RPar, ")");
        return fin(c, s);
    }

    // ------------------------------------------------------------ targets

    // A target at the level of a primary: NAME, attribute, subscript, or a parenthesized /
    // bracketed target list (parsed as an atom).
    Expr* primary_target() {
        if (!(at_name() || at_op(Op::LPar) || at_op(Op::LSqb))) invalid();
        return primary();
    }

    Expr* star_target() {
        if (at_op(Op::Star)) {
            size_t s = p;
            ++p;
            auto* st = mk<Starred>();
            st->value = star_target();
            return fin(st, s);
        }
        return primary_target();
    }

    Expr* star_targets() {
        size_t s = p;
        Expr* first = star_target();
        Expr* result = first;
        if (at_op(Op::Comma)) {
            auto* t = mk<Tuple>();
            t->elts.push_back(first);
            while (at_op(Op::Comma)) {
                ++p;
                if (!(at_name() || at_op(Op::LPar) || at_op(Op::LSqb) || at_op(Op::Star))) break;
                t->elts.push_back(star_target());
            }
            result = fin(t, s);
        }
        set_ctx(result, ExprCtx::Store);
        return result;
    }

    // ------------------------------------------------------------ expressions

    Expr* star_expressions() {
        size_t s = p;
        Expr* first = star_expression();
        if (!at_op(Op::Comma)) return first;
        auto* t = mk<Tuple>();
        t->elts.push_back(first);
        while (at_op(Op::Comma)) {
            ++p;
            if (!starts_expr()) break;
            t->elts.push_back(star_expression());
        }
        return fin(t, s);
    }

    Expr* star_expression() {
        if (at_op(Op::Star)) {
            size_t s = p;
            ++p;
            auto* st = mk<Starred>();
            st->value = bitwise_or();
            return fin(st, s);
        }
        return expression();
    }

    Expr* star_named_expression() {
        if (at_op(Op::Star)) {
            size_t s = p;
            ++p;
            auto* st = mk<Starred>();
            st->value = bitwise_or();
            return fin(st, s);
        }
        return named_expression();
    }

    Expr* named_expression() {
        if (at_name() && at_op(Op::ColonEqual, 1)) {
            size_t s = p;
            auto* n = mk<Name>();
            n->pos = tpos(tok());
            n->id = name();
            n->ctx = ExprCtx::Store;
            ++p;  // :=
            auto* ne = mk<NamedExpr>();
            ne->target = n;
            ne->value = expression();
            return fin(ne, s);
        }
        return expression();
    }

    Expr* expression() {
        if (at_kw(Kw::Lambda)) return lambdef();
        size_t s = p;
        Expr* d = disjunction();
        if (!at_kw(Kw::If)) return d;
        ++p;
        auto* e = mk<IfExp>();
        e->body = d;
        e->test = disjunction();
        expect_kw(Kw::Else);
        e->orelse = expression();
        return fin(e, s);
    }

    Expr* lambdef() {
        size_t s = p;
        ++p;
        auto* l = mk<Lambda>();
        l->args = parameters(true, Op::Colon);
        expect_op(Op::Colon, ":");
        l->body = expression();
        return fin(l, s);
    }

    Expr* yield_expr() {
        size_t s = p;
        ++p;  // yield
        if (at_kw(Kw::From)) {
            ++p;
            auto* y = mk<YieldFrom>();
            y->value = expression();
            return fin(y, s);
        }
        auto* y = mk<Yield>();
        if (starts_expr()) y->value = star_expressions();
        return fin(y, s);
    }

    Expr* disjunction() {
        size_t s = p;
        Expr* a = conjunction();
        if (!at_kw(Kw::Or)) return a;
        auto* b = mk<BoolOp>();
        b->op = BoolOpKind::Or;
        b->values.push_back(a);
        while (at_kw(Kw::Or)) { ++p; b->values.push_back(conjunction()); }
        return fin(b, s);
    }

    Expr* conjunction() {
        size_t s = p;
        Expr* a = inversion();
        if (!at_kw(Kw::And)) return a;
        auto* b = mk<BoolOp>();
        b->op = BoolOpKind::And;
        b->values.push_back(a);
        while (at_kw(Kw::And)) { ++p; b->values.push_back(inversion()); }
        return fin(b, s);
    }

    Expr* inversion() {
        if (at_kw(Kw::Not)) {
            size_t s = p;
            ++p;
            auto* u = mk<UnaryOp>();
            u->op = UnaryOpKind::Not;
            u->operand = inversion();
            return fin(u, s);
        }
        return comparison();
    }

    bool cmp_op(CmpOpKind& k, int& n) {
        const Token& t = tok();
        n = 1;
        if (t.type == Tok::Op) {
            switch (t.op) {
            case Op::EqEqual: k = CmpOpKind::Eq; return true;
            case Op::NotEqual: k = CmpOpKind::NotEq; return true;
            case Op::Less: k = CmpOpKind::Lt; return true;
            case Op::LessEqual: k = CmpOpKind::LtE; return true;
            case Op::Greater: k = CmpOpKind::Gt; return true;
            case Op::GreaterEqual: k = CmpOpKind::GtE; return true;
            default: return false;
            }
        }
        if (t.type == Tok::Name) {
            if (t.kw == Kw::In) { k = CmpOpKind::In; return true; }
            if (t.kw == Kw::Not && at_kw(Kw::In, 1)) { k = CmpOpKind::NotIn; n = 2; return true; }
            if (t.kw == Kw::Is) {
                if (at_kw(Kw::Not, 1)) { k = CmpOpKind::IsNot; n = 2; }
                else k = CmpOpKind::Is;
                return true;
            }
        }
        return false;
    }

    Expr* comparison() {
        size_t s = p;
        Expr* a = bitwise_or();
        CmpOpKind k;
        int n;
        if (!cmp_op(k, n)) return a;
        auto* c = mk<Compare>();
        c->left = a;
        while (cmp_op(k, n)) {
            p += (size_t)n;
            c->ops.push_back(k);
            c->comparators.push_back(bitwise_or());
        }
        return fin(c, s);
    }

    template <class Next, class Match> Expr* binary_left(Next next, Match match) {
        size_t s = p;
        Expr* a = (this->*next)();
        OpKind k;
        while (match(k)) {
            ++p;
            auto* b = mk<BinOp>();
            b->left = a;
            b->op = k;
            b->right = (this->*next)();
            a = fin(b, s);
        }
        return a;
    }

    Expr* bitwise_or() {
        return binary_left(&Parser::bitwise_xor, [&](OpKind& k) { k = OpKind::BitOr; return at_op(Op::VBar); });
    }
    Expr* bitwise_xor() {
        return binary_left(&Parser::bitwise_and, [&](OpKind& k) { k = OpKind::BitXor; return at_op(Op::Circumflex); });
    }
    Expr* bitwise_and() {
        return binary_left(&Parser::shift_expr, [&](OpKind& k) { k = OpKind::BitAnd; return at_op(Op::Amper); });
    }
    Expr* shift_expr() {
        return binary_left(&Parser::sum, [&](OpKind& k) {
            if (at_op(Op::LeftShift)) { k = OpKind::LShift; return true; }
            if (at_op(Op::RightShift)) { k = OpKind::RShift; return true; }
            return false;
        });
    }
    Expr* sum() {
        return binary_left(&Parser::term, [&](OpKind& k) {
            if (at_op(Op::Plus)) { k = OpKind::Add; return true; }
            if (at_op(Op::Minus)) { k = OpKind::Sub; return true; }
            return false;
        });
    }
    Expr* term() {
        return binary_left(&Parser::factor, [&](OpKind& k) {
            const Token& t = tok();
            if (t.type != Tok::Op) return false;
            switch (t.op) {
            case Op::Star: k = OpKind::Mult; return true;
            case Op::Slash: k = OpKind::Div; return true;
            case Op::DoubleSlash: k = OpKind::FloorDiv; return true;
            case Op::Percent: k = OpKind::Mod; return true;
            case Op::At: k = OpKind::MatMult; return true;
            default: return false;
            }
        });
    }

    Expr* factor() {
        const Token& t = tok();
        if (t.type == Tok::Op && (t.op == Op::Plus || t.op == Op::Minus || t.op == Op::Tilde)) {
            size_t s = p;
            UnaryOpKind k = t.op == Op::Plus ? UnaryOpKind::UAdd : t.op == Op::Minus ? UnaryOpKind::USub : UnaryOpKind::Invert;
            ++p;
            auto* u = mk<UnaryOp>();
            u->op = k;
            u->operand = factor();
            return fin(u, s);
        }
        return power();
    }

    Expr* power() {
        size_t s = p;
        Expr* a = await_primary();
        if (!at_op(Op::DoubleStar)) return a;
        ++p;
        auto* b = mk<BinOp>();
        b->left = a;
        b->op = OpKind::Pow;
        b->right = factor();
        return fin(b, s);
    }

    Expr* await_primary() {
        if (at_kw(Kw::Await)) {
            size_t s = p;
            ++p;
            auto* a = mk<Await>();
            a->value = primary();
            return fin(a, s);
        }
        return primary();
    }

    Expr* primary() {
        size_t s = p;
        Expr* a = atom();
        for (;;) {
            if (at_op(Op::Dot)) {
                ++p;
                auto* at = mk<Attribute>();
                at->value = a;
                at->attr = name();
                a = fin(at, s);
            } else if (at_op(Op::LPar)) {
                ++p;
                auto* c = mk<Call>();
                c->func = a;
                call_args(c->args, c->keywords, c);
                a = fin(c, s);
            } else if (at_op(Op::LSqb)) {
                ++p;
                auto* sub = mk<Subscript>();
                sub->value = a;
                sub->slice = slices();
                expect_op(Op::RSqb, "]");
                a = fin(sub, s);
            } else {
                return a;
            }
        }
    }

    // After '(' up to and including ')'.  `call` is non-null for a call (a lone generator
    // argument without its own parentheses is allowed there).
    void call_args(std::vector<Expr*>& args, std::vector<keyword*>& kws, Call* call) {
        size_t open = p - 1;
        bool first = true;
        while (!at_op(Op::RPar)) {
            size_t s = p;
            if (at_op(Op::Star)) {
                ++p;
                auto* st = mk<Starred>();
                st->value = expression();
                args.push_back(fin(st, s));
            } else if (at_op(Op::DoubleStar)) {
                ++p;
                auto* k = mk<keyword>();
                k->value = expression();
                kws.push_back(fin(k, s));
            } else if (at_name() && at_op(Op::Equal, 1)) {
                auto* k = mk<keyword>();
                k->arg = name();
                k->has_arg = true;
                ++p;
                k->value = expression();
                kws.push_back(fin(k, s));
            } else {
                Expr* e = named_expression();
                if (first && call && (at_kw(Kw::For) || (at_kw(Kw::Async) && at_kw(Kw::For, 1)))) {
                    auto* g = mk<GeneratorExp>();
                    g->elt = e;
                    g->generators = for_if_clauses();
                    expect_op(Op::RPar, ")");
                    fin(g, open);
                    args.push_back(g);
                    return;
                }
                if (at_op(Op::Equal)) error_node(e, "expression cannot contain assignment, perhaps you meant \"==\"?");
                if (!kws.empty()) {
                    bool has_kw = false;
                    for (keyword* k : kws) if (k->has_arg) has_kw = true;
                    // CPython reports these at the last token it read.
                    if (has_kw) error_at(tok(), "positional argument follows keyword argument");
                    error_at(tok(), "positional argument follows keyword argument unpacking");
                }
                args.push_back(e);
            }
            first = false;
            if (!at_op(Op::Comma)) break;
            ++p;
        }
        expect_op(Op::RPar, ")");
    }

    Expr* slices() {
        size_t s = p;
        Expr* first = slice_item();
        if (!at_op(Op::Comma) && first->kind != ExprKind::Starred) return first;
        auto* t = mk<Tuple>();
        t->elts.push_back(first);
        while (at_op(Op::Comma)) {
            ++p;
            if (at_op(Op::RSqb)) break;
            t->elts.push_back(slice_item());
        }
        return fin(t, s);
    }

    Expr* slice_item() {
        size_t s = p;
        if (at_op(Op::Star)) {
            ++p;
            auto* st = mk<Starred>();
            st->value = expression();
            return fin(st, s);
        }
        Expr* lower = nullptr;
        if (!at_op(Op::Colon)) {
            lower = named_expression();
            if (!at_op(Op::Colon)) return lower;
        }
        ++p;  // :
        auto* sl = mk<Slice>();
        sl->lower = lower;
        if (starts_expr() && !at_op(Op::Star)) sl->upper = expression();
        if (at_op(Op::Colon)) {
            ++p;
            if (starts_expr() && !at_op(Op::Star)) sl->step = expression();
        }
        return fin(sl, s);
    }

    std::vector<comprehension*> for_if_clauses() {
        std::vector<comprehension*> gens;
        while (at_kw(Kw::For) || (at_kw(Kw::Async) && at_kw(Kw::For, 1))) {
            auto* c = mk<comprehension>();
            if (at_kw(Kw::Async)) { ++p; c->is_async = 1; }
            ++p;  // for
            c->target = star_targets();
            expect_kw(Kw::In);
            c->iter = disjunction();
            while (at_kw(Kw::If)) {
                ++p;
                c->ifs.push_back(disjunction());
            }
            gens.push_back(c);
        }
        return gens;
    }

    bool at_for() { return at_kw(Kw::For) || (at_kw(Kw::Async) && at_kw(Kw::For, 1)); }

    Expr* atom() {
        size_t s = p;
        const Token& t = tok();
        switch (t.type) {
        case Tok::Name:
            switch (t.kw) {
            case Kw::None: {
                auto* n = mk<Name>();
                n->id = ident(t);
                n->pos = tpos(t);
                ++p;
                return n;
            }
            case Kw::True_: case Kw::False_: case Kw::None_: {
                auto* c = mk<Constant>();
                c->value.kind = t.kw == Kw::True_ ? ConstValue::Kind::True : t.kw == Kw::False_ ? ConstValue::Kind::False : ConstValue::Kind::None;
                c->pos = tpos(t);
                ++p;
                return c;
            }
            default:
                invalid();
            }
        case Tok::Number: {
            auto* c = mk<Constant>();
            c->value = parse_number(text(t));
            c->pos = tpos(t);
            ++p;
            return c;
        }
        case Tok::String: case Tok::FStringStart:
            return strings();
        case Tok::Op:
            switch (t.op) {
            case Op::Ellipsis: {
                auto* c = mk<Constant>();
                c->value.kind = ConstValue::Kind::Ellipsis;
                c->pos = tpos(t);
                ++p;
                return c;
            }
            case Op::LPar: return paren_atom();
            case Op::LSqb: {
                ++p;
                if (at_op(Op::RSqb)) { ++p; return fin(mk<List>(), s); }
                Expr* first = star_named_expression();
                if (at_for()) {
                    auto* lc = mk<ListComp>();
                    lc->elt = first;
                    lc->generators = for_if_clauses();
                    expect_op(Op::RSqb, "]");
                    return fin(lc, s);
                }
                auto* l = mk<List>();
                l->elts.push_back(first);
                while (at_op(Op::Comma)) {
                    ++p;
                    if (at_op(Op::RSqb)) break;
                    l->elts.push_back(star_named_expression());
                }
                expect_op(Op::RSqb, "]");
                return fin(l, s);
            }
            case Op::LBrace: return brace_atom();
            default:
                invalid();
            }
        default:
            invalid();
        }
    }

    Expr* paren_atom() {
        size_t s = p;
        ++p;
        if (at_op(Op::RPar)) { ++p; return fin(mk<Tuple>(), s); }
        if (at_kw(Kw::Yield)) {
            Expr* y = yield_expr();
            expect_op(Op::RPar, ")");
            return y;
        }
        Expr* first = star_named_expression();
        if (at_for()) {
            auto* g = mk<GeneratorExp>();
            g->elt = first;
            g->generators = for_if_clauses();
            expect_op(Op::RPar, ")");
            return fin(g, s);
        }
        if (at_op(Op::RPar)) {
            if (first->kind == ExprKind::Starred) error_node(first, "cannot use starred expression here");
            ++p;
            return first;
        }
        if (!at_op(Op::Comma)) {
            if (tok().type == Tok::Name || tok().type == Tok::Number || tok().type == Tok::String)
                error_at(toks[s], "'(' was never closed");
            invalid();
        }
        auto* t = mk<Tuple>();
        t->elts.push_back(first);
        while (at_op(Op::Comma)) {
            ++p;
            if (at_op(Op::RPar)) break;
            t->elts.push_back(star_named_expression());
        }
        expect_op(Op::RPar, ")");
        return fin(t, s);
    }

    Expr* brace_atom() {
        size_t s = p;
        ++p;
        if (at_op(Op::RBrace)) { ++p; return fin(mk<Dict>(), s); }
        if (at_op(Op::DoubleStar)) {
            auto* d = mk<Dict>();
            dict_items(d);
            return fin(d, s);
        }
        Expr* first = star_named_expression();
        if (at_op(Op::Colon) && first->kind != ExprKind::Starred) {
            ++p;
            Expr* v = expression();
            if (at_for()) {
                auto* dc = mk<DictComp>();
                dc->key = first;
                dc->value = v;
                dc->generators = for_if_clauses();
                expect_op(Op::RBrace, "}");
                return fin(dc, s);
            }
            auto* d = mk<Dict>();
            d->keys.push_back(first);
            d->values.push_back(v);
            if (at_op(Op::Comma)) {
                ++p;
                dict_items(d);
            } else {
                expect_op(Op::RBrace, "}");
            }
            return fin(d, s);
        }
        if (at_for()) {
            auto* sc = mk<SetComp>();
            sc->elt = first;
            sc->generators = for_if_clauses();
            expect_op(Op::RBrace, "}");
            return fin(sc, s);
        }
        auto* st = mk<Set>();
        st->elts.push_back(first);
        while (at_op(Op::Comma)) {
            ++p;
            if (at_op(Op::RBrace)) break;
            st->elts.push_back(star_named_expression());
        }
        expect_op(Op::RBrace, "}");
        return fin(st, s);
    }

    // Remaining `key: value` / `**mapping` items, through the closing '}'.
    void dict_items(Dict* d) {
        while (!at_op(Op::RBrace)) {
            if (at_op(Op::DoubleStar)) {
                ++p;
                d->keys.push_back(nullptr);
                d->values.push_back(bitwise_or());
            } else {
                d->keys.push_back(expression());
                expect_op(Op::Colon, ":");
                d->values.push_back(expression());
            }
            if (!at_op(Op::Comma)) break;
            ++p;
        }
        expect_op(Op::RBrace, "}");
    }

    // ------------------------------------------------------------ strings

    struct Piece {
        Expr* e;          // a Constant (str) or a FormattedValue
        bool is_const;
    };

    // A bad escape is reported, as CPython reports it, at the start of its token.
    void decode_or_fail(const Token& t, std::string_view body, bool raw, bool bytes, bool fstring, std::string& out) {
        std::string err;
        size_t eoff = 0;
        if (!decode_string(body, raw, bytes, fstring, out, err, eoff)) error_at(t, err);
    }

    Expr* strings() {
        size_t s = p;
        std::vector<Piece> pieces;
        bool any_f = false, any_t = false, any_bytes = false, any_str = false;
        std::string first_kind;
        bool first = true;
        while (tok().type == Tok::String || tok().type == Tok::FStringStart) {
            if (tok().type == Tok::String) {
                Token t = take();
                std::string_view tx = text(t);
                size_t q = tx.find_first_of("'\"");
                std::string_view prefix = tx.substr(0, q);
                bool raw = false, bytes = false, u = false;
                for (char c : prefix) {
                    char l = (char)(c | 0x20);
                    if (l == 'r') raw = true;
                    else if (l == 'b') bytes = true;
                    else if (l == 'u') u = true;
                }
                size_t ql = (tx.size() - q >= 6 && tx[q + 1] == tx[q] && tx[q + 2] == tx[q]) ? 3 : 1;
                std::string_view body = tx.substr(q + ql, tx.size() - q - 2 * ql);
                auto* c = mk<Constant>();
                std::string val;
                decode_or_fail(t, body, raw, bytes, false, val);
                c->value.kind = bytes ? ConstValue::Kind::Bytes : ConstValue::Kind::Str;
                c->value.text = std::move(val);
                if (u) c->kind = "u";
                c->pos = tpos(t);
                if (bytes) any_bytes = true; else any_str = true;
                if (first) first_kind = c->kind;
                pieces.push_back({c, true});
            } else {
                bool is_t = false;
                for (char c : text(tok())) if ((c | 0x20) == 't') is_t = true;
                if (is_t) any_t = true; else { any_f = true; any_str = true; }
                fstring(pieces, is_t);
            }
            first = false;
        }
        if (any_t && (any_str || any_bytes)) {
            const Token& a = toks[s];
            const Token& b = toks[p - 1];
            lx.fail("cannot mix t-string literals with string or bytes literals", a.lineno, a.col, b.end_lineno, b.end_col);
        }
        if (any_bytes && any_str) error_at(tok(), "cannot mix bytes and nonbytes literals");
        if (any_t) {
            auto* ts = mk<TemplateStr>();
            join_pieces(pieces, ts->values);
            return fin(ts, s);
        }
        if (!any_f) {
            if (pieces.size() == 1) return pieces[0].e;
            auto* c = mk<Constant>();
            c->value.kind = any_bytes ? ConstValue::Kind::Bytes : ConstValue::Kind::Str;
            for (Piece& pc : pieces) c->value.text += cast<Constant>(pc.e)->value.text;
            c->kind = first_kind;
            return fin(c, s);
        }
        auto* js = mk<JoinedStr>();
        join_pieces(pieces, js->values);
        return fin(js, s);
    }

    // CPython's concatenation: fold runs of constants (positions from the first to the
    // last of the run, kind from the first), then drop empty constants.
    void join_pieces(std::vector<Piece>& pieces, std::vector<Expr*>& out) {
        for (size_t i = 0; i < pieces.size(); ++i) {
            Expr* e = pieces[i].e;
            if (pieces[i].is_const) {
                if (i + 1 < pieces.size() && pieces[i + 1].is_const) {
                    auto* f = cast<Constant>(e);
                    auto* c = mk<Constant>();
                    c->value.kind = ConstValue::Kind::Str;
                    c->kind = f->kind;
                    Constant* last = f;
                    size_t j = i;
                    for (; j < pieces.size() && pieces[j].is_const; ++j) {
                        last = cast<Constant>(pieces[j].e);
                        c->value.text += last->value.text;
                    }
                    i = j - 1;
                    c->pos = Pos{f->pos.lineno, f->pos.col_offset, last->pos.end_lineno, last->pos.end_col_offset};
                    e = c;
                }
                if (cast<Constant>(e)->value.text.empty()) continue;
            }
            out.push_back(e);
        }
    }

    // The text of a `{expr=}` field as CPython keeps it: comments (outside string literals)
    // removed up to their newline.
    static std::string strip_comments(std::string_view s) {
        std::string out;
        char quote = 0;
        bool triple = false;
        for (size_t i = 0; i < s.size(); ++i) {
            char c = s[i];
            if (quote) {
                out.push_back(c);
                if (c == '\\' && i + 1 < s.size()) { out.push_back(s[++i]); continue; }
                if (c == quote && (!triple || (i + 2 < s.size() && s[i + 1] == quote && s[i + 2] == quote))) {
                    if (triple) { out.push_back(s[++i]); out.push_back(s[++i]); }
                    quote = 0;
                }
                continue;
            }
            if (c == '\'' || c == '"') {
                quote = c;
                triple = i + 2 < s.size() && s[i + 1] == c && s[i + 2] == c;
                out.push_back(c);
                if (triple) { out.push_back(s[++i]); out.push_back(s[++i]); }
                continue;
            }
            if (c == '#') {
                while (i < s.size() && s[i] != '\n') ++i;
                if (i < s.size()) out.push_back('\n');
                continue;
            }
            out.push_back(c);
        }
        return out;
    }

    // One f-string (or t-string, whose fields become Interpolation nodes).
    void fstring(std::vector<Piece>& out, bool is_t) {
        Token st = take();
        std::string_view tx = text(st);
        bool raw = false;
        for (char c : tx) if ((c | 0x20) == 'r') raw = true;
        fstring_body(out, raw, is_t);
        if (tok().type != Tok::FStringEnd) invalid();
        ++p;
    }

    // FSTRING_MIDDLE pieces and replacement fields, up to FSTRING_END (or the '}' that
    // closes a format spec).
    void fstring_body(std::vector<Piece>& parts, bool raw, bool interpolation) {
        for (;;) {
            const Token& t = tok();
            if (t.type == Tok::FStringMiddle) {
                Token mt = take();
                auto* c = mk<Constant>();
                c->value.kind = ConstValue::Kind::Str;
                decode_or_fail(mt, text(mt), raw, false, true, c->value.text);
                c->pos = tpos(mt);
                parts.push_back({c, true});
            } else if (t.type == Tok::Op && t.op == Op::LBrace) {
                replacement_field(parts, raw, interpolation);
            } else {
                return;
            }
        }
    }

    void replacement_field(std::vector<Piece>& parts, bool raw, bool interpolation) {
        size_t s = p;
        uint32_t expr_off = tok().end;
        ++p;  // {
        if (at_op(Op::RBrace)) error_at(tok(), std::string(interpolation ? "t" : "f") + "-string: valid expression required before '}'");
        if (at_kw(Kw::Lambda)) error_at(tok(), std::string(interpolation ? "t" : "f") + "-string: lambda expressions are not allowed without parentheses");
        Expr* e = at_kw(Kw::Yield) ? yield_expr() : star_expressions();
        uint32_t expr_end = toks[p - 1].end;
        Constant* debug = nullptr;
        if (at_op(Op::Equal)) {
            ++p;
            uint32_t end_off = tok().start;
            debug = mk<Constant>();
            debug->value.kind = ConstValue::Kind::Str;
            debug->value.text = strip_comments(std::string_view(lx.buffer()).substr(expr_off, end_off - expr_off));
            int l1, c1, l2, c2;
            lx.locate(expr_off, l1, c1);
            lx.locate(end_off, l2, c2);
            debug->pos = Pos{l1, c1, l2, c2};
        }
        int conversion = -1;
        if (at_op(Op::Exclamation)) {
            ++p;
            const Token& ct = tok();
            std::string_view cv = ct.type == Tok::Name ? text(ct) : std::string_view();
            if (cv != "s" && cv != "r" && cv != "a") error_at(ct, "f-string: invalid conversion character: expected 's', 'r', or 'a'");
            conversion = cv[0];
            ++p;
        }
        Expr* spec = nullptr;
        if (at_op(Op::Colon)) {
            size_t ss = p;
            ++p;
            std::vector<Piece> sp;
            fstring_body(sp, raw, false);
            auto* js = mk<JoinedStr>();
            join_pieces(sp, js->values);
            spec = fin(js, ss);
        }
        if (!at_op(Op::RBrace)) {
            if (tok().type == Tok::FStringEnd || tok().type == Tok::Newline) error_at(tok(), "f-string: expecting '}'");
            invalid();
        }
        ++p;
        if (debug && conversion == -1 && !spec) conversion = 'r';
        if (debug) parts.push_back({debug, true});
        if (interpolation) {
            auto* in = mk<Interpolation>();
            in->value = e;
            in->str = lx.buffer().substr(expr_off, expr_end - expr_off);
            in->conversion = conversion;
            in->format_spec = spec;
            parts.push_back({fin(in, s), false});
        } else {
            auto* fv = mk<FormattedValue>();
            fv->value = e;
            fv->conversion = conversion;
            fv->format_spec = spec;
            parts.push_back({fin(fv, s), false});
        }
    }
};

// Offset in characters (CPython's SyntaxError.offset counts characters, 1-based).
int char_offset(std::string_view line, int byte_col) {
    int n = 0;
    for (int i = 0; i < byte_col && i < (int)line.size(); ++i)
        if (((unsigned char)line[(size_t)i] & 0xC0) != 0x80) ++n;
    if (byte_col > (int)line.size()) n += byte_col - (int)line.size();
    return n + 1;
}

std::string_view line_text(const std::string& buf, int lineno) {
    size_t pos = 0;
    for (int l = 1; l < lineno; ++l) {
        size_t nl = buf.find('\n', pos);
        if (nl == std::string::npos) return {};
        pos = nl + 1;
    }
    size_t e = buf.find('\n', pos);
    if (e == std::string::npos) e = buf.size();
    return std::string_view(buf).substr(pos, e - pos);
}

// PEP 263: the coding cookie of the first or second line (the second only when the first
// is blank or a comment), as `coding[:=][ \t]*([-\w.]+)` after a '#'.  Normalized as
// CPython's get_normal_name does.
std::string coding_cookie(std::string_view src) {
    size_t pos = 0;
    if (src.size() >= 3 && (unsigned char)src[0] == 0xEF && (unsigned char)src[1] == 0xBB && (unsigned char)src[2] == 0xBF) pos = 3;
    for (int line = 0; line < 2 && pos < src.size(); ++line) {
        size_t e = src.find_first_of("\r\n", pos);
        if (e == std::string_view::npos) e = src.size();
        std::string_view l = src.substr(pos, e - pos);
        size_t i = 0;
        while (i < l.size() && (l[i] == ' ' || l[i] == '\t' || l[i] == '\f')) ++i;
        if (i < l.size() && l[i] == '#') {
            for (size_t k = l.find("coding", i); k != std::string_view::npos; k = l.find("coding", k + 1)) {
                size_t j = k + 6;
                if (j >= l.size() || (l[j] != ':' && l[j] != '=')) continue;
                ++j;
                while (j < l.size() && (l[j] == ' ' || l[j] == '\t')) ++j;
                size_t st = j;
                while (j < l.size() && (std::isalnum((unsigned char)l[j]) || l[j] == '-' || l[j] == '_' || l[j] == '.')) ++j;
                if (j == st) continue;
                std::string name(l.substr(st, j - st));
                std::string norm;
                for (char c : name.substr(0, 12)) norm.push_back(c == '_' ? '-' : (char)std::tolower((unsigned char)c));
                if (norm == "utf-8" || norm.rfind("utf-8-", 0) == 0) return "utf-8";
                for (const char* lat : {"latin-1", "iso-8859-1", "iso-latin-1"}) {
                    size_t n = strlen(lat);
                    if (norm == lat || (norm.size() > n && norm.compare(0, n, lat) == 0 && norm[n] == '-')) return "iso-8859-1";
                }
                return name;
            }
        } else if (i < l.size()) {
            break;  // a code line: no cookie on the line after it
        }
        pos = e;
        if (pos < src.size() && src[pos] == '\r') ++pos;
        if (pos < src.size() && src[pos] == '\n') ++pos;
    }
    return "";
}

}  // namespace

ParseResult parse(std::string_view source, std::string_view filename) {
    ParseResult r;
    std::string cookie = coding_cookie(source);
    std::string converted;
    if (!cookie.empty() && cookie != "utf-8") {
        bool bom = source.size() >= 3 && (unsigned char)source[0] == 0xEF && (unsigned char)source[1] == 0xBB && (unsigned char)source[2] == 0xBF;
        std::string why;
        if (bom) why = "encoding problem: " + cookie + " with BOM";
        else if (!uni::to_utf8(cookie, source, converted)) why = "unknown encoding: " + cookie;
        if (!why.empty()) {
            SyntaxError e;
            e.msg = why;
            e.filename = std::string(filename);
            e.lineno = 0;
            e.offset = -1;
            r.error = std::move(e);
            return r;
        }
        source = converted;
    }
    auto mod = std::make_unique<Module>();
    Parser ps(source, mod.get());
    try {
        ps.file();
        r.module = std::move(mod);
    } catch (ParseFailure& f) {
        SyntaxError e;
        e.msg = f.info.msg;
        e.filename = std::string(filename);
        e.lineno = f.info.lineno;
        std::string_view lt = line_text(ps.lx.buffer(), f.info.lineno);
        e.text = std::string(lt);
        e.offset = char_offset(lt, f.info.col);
        if (f.info.end_lineno) {
            e.end_lineno = f.info.end_lineno;
            e.end_offset = char_offset(line_text(ps.lx.buffer(), f.info.end_lineno), f.info.end_col);
        }
        r.error = std::move(e);
    }
    return r;
}

}  // namespace pyfront
