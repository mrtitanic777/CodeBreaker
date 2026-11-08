// codebreaker/translate/frontends/python/program.cpp -- typed C++ from
// codebreaker.translate.frontends.python.program.

#include "codebreaker/translate/frontends/python/program.hpp"

#include <algorithm>

#include "codebreaker/translate/frontends/python/infer.hpp"

namespace codebreaker::translate::frontends::python::program {

using pyengine::AnyNode;

// ------------------------------------------------------------------------------------ small helpers
bool lit_eq(const Str& s, const char* lit) {
    const std::u32string& u = s.u32();
    size_t i = 0;
    for (; lit[i]; ++i) {
        if (i >= u.size() || u[i] != static_cast<char32_t>(static_cast<unsigned char>(lit[i]))) {
            return false;
        }
    }
    return i == u.size();
}

bool k_is(const T& t, const char* lit) { return lit_eq(t.kind, lit); }

bool k_in(const T& t, std::initializer_list<const char*> kinds) {
    for (const char* k : kinds) {
        if (lit_eq(t.kind, k)) {
            return true;
        }
    }
    return false;
}

bool str_starts(const Str& s, const char* lit) {
    const std::u32string& u = s.u32();
    size_t i = 0;
    for (; lit[i]; ++i) {
        if (i >= u.size() || u[i] != static_cast<char32_t>(static_cast<unsigned char>(lit[i]))) {
            return false;
        }
    }
    return true;
}

Str last_part(const Str& s) {
    const std::u32string& u = s.u32();
    size_t p = u.rfind(U'.');
    return p == std::u32string::npos ? s : Str(u.substr(p + 1));
}

TupItem none_item() { return TupItem(std::monostate{}); }

const Str& Tup::s(size_t i) const {
    if (i < 1 || i > items.size()) {
        throw py::IndexError("tuple index out of range");
    }
    const Str* p = std::get_if<Str>(&items[i - 1]);
    if (!p) {
        throw py::TypeError("tuple item is not a str");
    }
    return *p;
}

bool Tup::in(std::initializer_list<const char*> kinds) const {
    for (const char* k : kinds) {
        if (lit_eq(kind, k)) {
            return true;
        }
    }
    return false;
}

bool Tup::operator==(const Tup& o) const {
    if (!(kind == o.kind) || items.size() != o.items.size()) {
        return false;
    }
    for (size_t i = 0; i < items.size(); ++i) {
        if (!(items[i] == o.items[i])) {
            return false;
        }
    }
    return true;
}

FactKey fk(char tag, const Str& name) { return FactKey{tag, name, 0}; }
FactKey fk_site(int64_t site) { return FactKey{'s', Str(), site}; }

const std::set<Str>& STDLIB() {
    static const std::set<Str> v = [] {
        const char* names =
            "abc argparse array ast base64 binascii bisect builtins bz2 calendar codecs collections "
            "concurrent configparser contextlib copy csv ctypes dataclasses datetime decimal difflib dis email enum errno "
            "fnmatch fractions functools gc getpass glob gzip hashlib heapq hmac html http importlib inspect io ipaddress "
            "itertools json keyword locale logging lzma math mimetypes mmap multiprocessing numbers operator os pathlib "
            "pickle platform pprint queue random re reprlib secrets select shlex shutil signal socket sqlite3 ssl stat "
            "statistics string struct subprocess sys sysconfig tarfile tempfile textwrap threading time timeit tkinter "
            "token tokenize traceback types typing unicodedata unittest urllib uuid warnings weakref winreg xml zipfile "
            "zlib zipimport __future__ msvcrt _thread";
        std::set<Str> out;
        std::string cur;
        for (const char* p = names;; ++p) {
            if (*p == ' ' || *p == 0) {
                if (!cur.empty()) {
                    out.insert(Str(cur));
                }
                cur.clear();
                if (*p == 0) {
                    break;
                }
            } else {
                cur.push_back(*p);
            }
        }
        return out;
    }();
    return v;
}

const std::set<Str>& EXCEPTIONS() {
    static const std::set<Str> v = [] {
        const char* names =
            "Exception BaseException ValueError KeyError IndexError TypeError RuntimeError "
            "OSError IOError LookupError ArithmeticError ZeroDivisionError OverflowError NotImplementedError "
            "AssertionError StopIteration EOFError FileNotFoundError AttributeError UnicodeDecodeError "
            "UnicodeEncodeError PermissionError TimeoutError";
        std::set<Str> out;
        std::string cur;
        for (const char* p = names;; ++p) {
            if (*p == ' ' || *p == 0) {
                if (!cur.empty()) {
                    out.insert(Str(cur));
                }
                cur.clear();
                if (*p == 0) {
                    break;
                }
            } else {
                cur.push_back(*p);
            }
        }
        return out;
    }();
    return v;
}

// ------------------------------------------------------------------------------------ ast helpers
Str deco_name(const A::Expr* d) {
    if (auto* n = A::as<A::Name>(d)) {
        return Str(n->id);
    }
    if (auto* a = A::as<A::Attribute>(d)) {
        return Str(a->attr);
    }
    if (auto* c = A::as<A::Call>(d)) {
        return deco_name(c->func);
    }
    return Str("?");
}

std::vector<Str> target_names(const A::Expr* t) {
    if (auto* n = A::as<A::Name>(t)) {
        return {Str(n->id)};
    }
    const std::vector<A::Expr*>* elts = nullptr;
    if (auto* tu = A::as<A::Tuple>(t)) {
        elts = &tu->elts;
    } else if (auto* li = A::as<A::List>(t)) {
        elts = &li->elts;
    }
    std::vector<Str> out;
    if (elts) {
        for (const A::Expr* e : *elts) {
            for (const Str& s : target_names(e)) {
                out.push_back(s);
            }
        }
    }
    return out;
}

std::optional<Str> str_const(const A::Expr* n) {
    auto* c = A::as<A::Constant>(n);
    if (c && c->value.kind == A::ConstValue::Kind::Str) {
        return Str(c->value.text);
    }
    return std::nullopt;
}

std::optional<Big> int_const(const A::Expr* n) {
    Big v;
    if (pyengine::int_const(n, &v)) {
        return v;
    }
    return std::nullopt;
}

bool is_none_const(const A::Expr* e) {
    auto* c = A::as<A::Constant>(e);
    return c && c->value.kind == A::ConstValue::Kind::None;
}

namespace {

// _Precedence of ast._Unparser
enum Prec {
    NAMED_EXPR = 1, TUPLE, YIELD, TEST, OR, AND, NOT, CMP, BOR, BXOR, BAND, SHIFT, ARITH, TERM, FACTOR, POWER, AWAIT, ATOM
};

int binop_prec(A::OpKind op) {
    switch (op) {
        case A::OpKind::Add: case A::OpKind::Sub: return ARITH;
        case A::OpKind::Mult: case A::OpKind::MatMult: case A::OpKind::Div: case A::OpKind::Mod:
        case A::OpKind::FloorDiv: return TERM;
        case A::OpKind::Pow: return POWER;
        case A::OpKind::LShift: case A::OpKind::RShift: return SHIFT;
        case A::OpKind::BitOr: return BOR;
        case A::OpKind::BitXor: return BXOR;
        case A::OpKind::BitAnd: return BAND;
    }
    return ARITH;
}

const char* binop_text(A::OpKind op) {
    switch (op) {
        case A::OpKind::Add: return "+"; case A::OpKind::Sub: return "-"; case A::OpKind::Mult: return "*";
        case A::OpKind::MatMult: return "@"; case A::OpKind::Div: return "/"; case A::OpKind::Mod: return "%";
        case A::OpKind::Pow: return "**"; case A::OpKind::LShift: return "<<"; case A::OpKind::RShift: return ">>";
        case A::OpKind::BitOr: return "|"; case A::OpKind::BitXor: return "^"; case A::OpKind::BitAnd: return "&";
        case A::OpKind::FloorDiv: return "//";
    }
    return "?";
}

const char* cmp_text(A::CmpOpKind op) {
    switch (op) {
        case A::CmpOpKind::Eq: return "=="; case A::CmpOpKind::NotEq: return "!="; case A::CmpOpKind::Lt: return "<";
        case A::CmpOpKind::LtE: return "<="; case A::CmpOpKind::Gt: return ">"; case A::CmpOpKind::GtE: return ">=";
        case A::CmpOpKind::Is: return "is"; case A::CmpOpKind::IsNot: return "is not"; case A::CmpOpKind::In: return "in";
        case A::CmpOpKind::NotIn: return "not in";
    }
    return "?";
}

Str unp(const A::Expr* e, int ctx);

Str unp_seq(const std::vector<A::Expr*>& xs, int ctx) {
    Str out;
    for (size_t i = 0; i < xs.size(); ++i) {
        if (i) {
            out += Str(", ");
        }
        out += unp(xs[i], ctx);
    }
    return out;
}

Str unp(const A::Expr* e, int ctx) {
    if (!e) {
        return Str();
    }
    using K = A::ExprKind;
    auto paren = [&](int own, const Str& s) { return ctx > own ? Str("(") + s + Str(")") : s; };
    switch (e->kind) {
        case K::Name: return Str(A::cast<A::Name>(e)->id);
        case K::Attribute: {
            auto* a = A::cast<A::Attribute>(e);
            Str v = unp(a->value, ATOM);
            // an int literal needs a space before the dot
            auto* c = A::as<A::Constant>(a->value);
            if (c && c->value.kind == A::ConstValue::Kind::Int) {
                v += Str(" ");
            }
            return v + Str(".") + Str(a->attr);
        }
        case K::Subscript: {
            auto* s = A::cast<A::Subscript>(e);
            Str inner;
            if (auto* tu = A::as<A::Tuple>(s->slice)) {
                if (tu->elts.empty()) {
                    inner = Str("()");
                } else {
                    inner = unp_seq(tu->elts, TEST);
                    if (tu->elts.size() == 1) {
                        inner += Str(",");
                    }
                }
            } else {
                inner = unp(s->slice, TEST);
            }
            return unp(s->value, ATOM) + Str("[") + inner + Str("]");
        }
        case K::Slice: {
            auto* s = A::cast<A::Slice>(e);
            Str out;
            if (s->lower) {
                out += unp(s->lower, TEST);
            }
            out += Str(":");
            if (s->upper) {
                out += unp(s->upper, TEST);
            }
            if (s->step) {
                out += Str(":") + unp(s->step, TEST);
            }
            return out;
        }
        case K::Call: {
            auto* c = A::cast<A::Call>(e);
            Str out = unp(c->func, ATOM) + Str("(");
            bool first = true;
            for (const A::Expr* a : c->args) {
                if (!first) {
                    out += Str(", ");
                }
                first = false;
                out += unp(a, TEST);
            }
            for (const A::keyword* k : c->keywords) {
                if (!first) {
                    out += Str(", ");
                }
                first = false;
                out += (k->has_arg ? Str(k->arg) + Str("=") : Str("**")) + unp(k->value, TEST);
            }
            return out + Str(")");
        }
        case K::Tuple: {
            auto* t = A::cast<A::Tuple>(e);
            Str s = unp_seq(t->elts, TEST);
            if (t->elts.size() == 1) {
                s += Str(",");
            }
            if (t->elts.empty() || ctx > TUPLE) {
                return Str("(") + s + Str(")");
            }
            return s;
        }
        case K::List: return Str("[") + unp_seq(A::cast<A::List>(e)->elts, TEST) + Str("]");
        case K::Set: return Str("{") + unp_seq(A::cast<A::Set>(e)->elts, TEST) + Str("}");
        case K::Starred: return Str("*") + unp(A::cast<A::Starred>(e)->value, BOR);
        case K::Constant: {
            auto& v = A::cast<A::Constant>(e)->value;
            using CK = A::ConstValue::Kind;
            switch (v.kind) {
                case CK::None: return Str("None");
                case CK::True: return Str("True");
                case CK::False: return Str("False");
                case CK::Ellipsis: return Str("...");
                case CK::Int: return Str(v.text);
                case CK::Float: return py::repr(v.f);
                case CK::Complex: return py::repr(v.f).slice(0, py::repr(v.f).size()) + Str("j");
                case CK::Str: return py::repr(Str(v.text));
                case CK::Bytes: return py::repr(py::bytes(v.text));
            }
            return Str("...");
        }
        case K::UnaryOp: {
            auto* u = A::cast<A::UnaryOp>(e);
            const char* op = u->op == A::UnaryOpKind::Not ? "not " : u->op == A::UnaryOpKind::Invert ? "~" : u->op == A::UnaryOpKind::UAdd ? "+" : "-";
            int own = u->op == A::UnaryOpKind::Not ? NOT : FACTOR;
            return paren(own, Str(op) + unp(u->operand, own));
        }
        case K::BinOp: {
            auto* b = A::cast<A::BinOp>(e);
            int own = binop_prec(b->op);
            bool rassoc = b->op == A::OpKind::Pow;
            int lp = rassoc ? own + 1 : own;
            int rp = rassoc ? own : own + 1;
            return paren(own, unp(b->left, lp) + Str(" ") + Str(binop_text(b->op)) + Str(" ") + unp(b->right, rp));
        }
        case K::BoolOp: {
            auto* b = A::cast<A::BoolOp>(e);
            int own = b->op == A::BoolOpKind::Or ? OR : AND;
            Str sep = b->op == A::BoolOpKind::Or ? Str(" or ") : Str(" and ");
            Str out;
            int p = own + 1;
            for (size_t i = 0; i < b->values.size(); ++i) {
                if (i) {
                    out += sep;
                }
                out += unp(b->values[i], p);
                p = std::min<int>(p + 1, ATOM);
            }
            return paren(own, out);
        }
        case K::Compare: {
            auto* c = A::cast<A::Compare>(e);
            Str out = unp(c->left, CMP + 1);
            for (size_t i = 0; i < c->ops.size(); ++i) {
                out += Str(" ") + Str(cmp_text(c->ops[i])) + Str(" ") + unp(c->comparators[i], CMP + 1);
            }
            return paren(CMP, out);
        }
        default: return Str("<") + Str(A::name_of(e->kind)) + Str(">");
    }
}

}  // namespace

Str unparse(const A::Expr* e) { return unp(e, TEST); }

// ------------------------------------------------------------------------------------ Fn / Class / Module
Fn::Fn(const Str& qname_, const A::FunctionDef* node_, Module* module_, Class* cls_)
    : qname(qname_), node(node_), module(module_), cls(cls_), name(node_->name), kind(PS("function")) {
    for (const A::Expr* d : node->decorator_list) {
        decorators.push_back(deco_name(d));
    }
    auto has_deco = [&](const char* n) {
        for (const Str& d : decorators) {
            if (lit_eq(d, n)) {
                return true;
            }
        }
        return false;
    };
    if (cls != nullptr) {
        kind = Str("method");
        if (has_deco("staticmethod")) {
            kind = Str("static");
        } else if (has_deco("classmethod")) {
            kind = Str("classmethod");
        } else if (has_deco("property")) {
            kind = Str("property");
        }
    }
    const A::arguments* a = node->args;
    std::vector<const A::arg*> pos;
    for (const A::arg* x : a->posonlyargs) {
        pos.push_back(x);
    }
    for (const A::arg* x : a->args) {
        pos.push_back(x);
    }
    for (const A::arg* x : pos) {
        params.push_back(Str(x->arg));
    }
    for (const A::arg* x : a->kwonlyargs) {
        kwonly.push_back(Str(x->arg));
    }
    for (const A::arg* x : pos) {
        if (x->annotation) {
            annotations.set(Str(x->arg), x->annotation);
        }
    }
    for (const A::arg* x : a->kwonlyargs) {
        if (x->annotation) {
            annotations.set(Str(x->arg), x->annotation);
        }
    }
    size_t nd = a->defaults.size();
    if (nd) {
        for (size_t i = 0; i < nd; ++i) {
            defaults.set(params[params.size() - nd + i], a->defaults[i]);
        }
    }
    for (size_t i = 0; i < a->kwonlyargs.size(); ++i) {
        if (a->kw_defaults[i] != nullptr) {
            defaults.set(Str(a->kwonlyargs[i]->arg), a->kw_defaults[i]);
        }
    }
    if (a->vararg) {
        varargs = Str(a->vararg->arg);
    }
    if (a->kwarg) {
        kwargs = Str(a->kwarg->arg);
    }
    int end = node->pos.end_lineno ? node->pos.end_lineno : node->pos.lineno;
    lines = static_cast<int64_t>(end) - node->pos.lineno + 1;
}

std::vector<Str> Fn::bound() const {
    if (lit_eq(kind, "method") || lit_eq(kind, "property") || lit_eq(kind, "classmethod")) {
        return std::vector<Str>(params.begin() + (params.empty() ? 0 : 1), params.end());
    }
    return params;
}

Class::Class(const Str& qname_, const A::ClassDef* node_, Module* module_)
    : qname(qname_), node(node_), module(module_), name(node_->name) {
    for (const A::Expr* b : node->bases) {
        base_nodes.push_back(b);
    }
    for (const A::Expr* d : node->decorator_list) {
        decorators.push_back(deco_name(d));
    }
}

Module::Module(const Str& name_, const std::optional<Str>& path_, const Str& source_)
    : name(name_), path(path_), source(source_) {
    std::string utf8 = source.utf8();
    A::ParseResult r = A::parse(utf8, "<unknown>");
    if (!r) {
        throw SyntaxError(r.error ? r.error->msg : std::string("invalid syntax"));
    }
    tree = std::move(r.module);
    is_package = py::os::path::basename(path ? *path : Str()) == Str("__init__.py");
}

bool Module::has_docstring() const {
    if (tree->body.empty()) {
        return false;
    }
    auto* e = A::as<A::ExprStmt>(tree->body[0]);
    return e && str_const(e->value).has_value();
}

// _indexing_params
namespace {

std::set<Str> indexing_params(const Fn& f) {
    std::vector<const A::Stmt*> body;
    for (const A::Stmt* s : f.node->body) {
        auto* e = A::as<A::ExprStmt>(s);
        if (!(e && A::as<A::Constant>(e->value))) {
            body.push_back(s);
        }
    }
    if (body.empty()) {
        return {};
    }
    const A::Stmt* first = body[0];
    if (!(first->kind == A::StmtKind::Return || first->kind == A::StmtKind::Assign || first->kind == A::StmtKind::Expr)) {
        return {};
    }
    std::set<Str> out;
    for (const AnyNode& an : pyengine::walk(AnyNode(first))) {
        auto* n = an.expr();
        if (!n) {
            continue;
        }
        if (auto* sub = A::as<A::Subscript>(n)) {
            if (auto* nm = A::as<A::Name>(sub->slice)) {
                out.insert(Str(nm->id));
            }
        }
        if (auto* call = A::as<A::Call>(n)) {
            auto* fa = A::as<A::Attribute>(call->func);
            if (fa && fa->attr == "unpack_from" && call->args.size() >= 3) {
                if (auto* nm = A::as<A::Name>(call->args[2])) {
                    out.insert(Str(nm->id));
                }
            }
        }
    }
    std::set<Str> res;
    for (const Str& p : f.params) {
        if (out.count(p)) {
            res.insert(p);
        }
    }
    return res;
}

// _none_aware
std::set<Str> none_aware_of(const Fn& f) {
    std::set<Str> out;
    for (const Str& n : f.defaults) {
        const A::Expr* d = f.defaults.at(n);
        if (is_none_const(d)) {
            out.insert(n);
        }
    }
    std::set<Str> params;
    for (const Str& p : f.params) {
        params.insert(p);
    }
    for (const Str& p : f.kwonly) {
        params.insert(p);
    }
    auto is_param_name = [&](const A::Expr* x) {
        auto* nm = A::as<A::Name>(x);
        return nm && params.count(Str(nm->id));
    };
    for (const AnyNode& an : pyengine::walk(AnyNode(static_cast<const A::Stmt*>(f.node)))) {
        if (const A::Expr* e = an.expr()) {
            if (auto* cmp = A::as<A::Compare>(e)) {
                std::vector<const A::Expr*> all = {cmp->left};
                for (const A::Expr* c : cmp->comparators) {
                    all.push_back(c);
                }
                bool nones = false;
                for (const A::Expr* x : all) {
                    if (is_none_const(x)) {
                        nones = true;
                    }
                }
                if (nones) {
                    for (const A::Expr* x : all) {
                        if (is_param_name(x)) {
                            out.insert(Str(A::cast<A::Name>(x)->id));
                        }
                    }
                }
            } else if (auto* ie = A::as<A::IfExp>(e)) {
                if (is_param_name(ie->test)) {
                    out.insert(Str(A::cast<A::Name>(ie->test)->id));
                }
            } else if (auto* un = A::as<A::UnaryOp>(e)) {
                if (un->op == A::UnaryOpKind::Not && is_param_name(un->operand)) {
                    out.insert(Str(A::cast<A::Name>(un->operand)->id));
                }
            } else if (auto* bo = A::as<A::BoolOp>(e)) {
                for (const A::Expr* v : bo->values) {
                    if (is_param_name(v)) {
                        out.insert(Str(A::cast<A::Name>(v)->id));
                    }
                }
            }
        } else if (const A::Stmt* s = an.stmt()) {
            const A::Expr* test = nullptr;
            if (auto* i = A::as<A::If>(s)) {
                test = i->test;
            } else if (auto* w = A::as<A::While>(s)) {
                test = w->test;
            } else if (auto* a = A::as<A::Assert>(s)) {
                test = a->test;
            }
            if (test && is_param_name(test)) {
                out.insert(Str(A::cast<A::Name>(test)->id));
            }
        }
    }
    return out;
}

}  // namespace

// ------------------------------------------------------------------------------------ Program
Program::Program() : int_policy("strict") {
    // sites start at 1: index 0 of every table is unused
    parent.push_back(0);
    ckind.push_back(Str());
    elem.push_back(P::BOTTOM());
    keys.push_back(P::BOTTOM());
    fields.push_back(py::dict<Str, T>());
    required.push_back(std::nullopt);
    site_where.push_back(std::nullopt);
    site_node.push_back(nullptr);
}

Program::~Program() = default;

Module& Program::add_module(const Str& name, const std::optional<Str>& path, const std::optional<Str>& source) {
    Str text;
    if (!source) {
        py::file f = py::open(*path, Str("r"), Str("utf-8"));
        text = f.read_text();
    } else {
        text = *source;
    }
    auto m = std::make_unique<Module>(name, path, text);
    Module* raw = m.get();
    owned_modules_.push_back(std::move(m));
    modules.set(name, raw);
    return *raw;
}

void Program::add_tree(const Str& root, const Str& package) {
    for (auto t : py::os::walk(root)) {
        const Str dp = std::get<0>(t);
        py::list<Str> dirs = std::get<1>(t);
        py::list<Str> files = std::get<2>(t);
        std::vector<Str> keep;
        for (const Str& d : dirs) {
            if (!(d == Str("__pycache__"))) {
                keep.push_back(d);
            }
        }
        std::sort(keep.begin(), keep.end());
        dirs.clear();
        dirs.extend(keep);
        std::vector<Str> fs;
        for (const Str& f : files) {
            fs.push_back(f);
        }
        std::sort(fs.begin(), fs.end());
        for (const Str& f : fs) {
            if (!f.endswith(Str(".py"))) {
                continue;
            }
            Str p = py::os::path::join(dp, f);
            Str rel = py::os::path::relpath(p, root);
            rel = rel.slice(0, rel.size() - 3);
            // .replace(os.sep, '.')
            std::u32string r = rel.u32();
            for (char32_t& c : r) {
                if (c == U'\\' || c == U'/') {
                    c = U'.';
                }
            }
            rel = Str(r);
            if (rel.endswith(Str("__init__"))) {
                rel = rel.slice(0, rel.size() - 8);
                // .rstrip('.')
                std::u32string q = rel.u32();
                while (!q.empty() && q.back() == U'.') {
                    q.pop_back();
                }
                rel = Str(q);
            }
            Str name = package + (rel.empty() ? Str() : Str(".") + rel);
            try {
                add_module(name, p);
            } catch (const SyntaxError&) {
            }
        }
    }
}

void Program::index() {
    for (Module* m : modules.values()) {
        for (const A::Stmt* st : m->tree->body) {
            index_stmt(*m, st);
        }
    }
    for (Class* c : classes.values()) {
        for (const A::Expr* b : c->base_nodes) {
            std::optional<Ref> r = resolve_expr_name(*c->module, b);
            if (r && r->is("class")) {
                c->bases.push_back(r->s(1));
                classes.at(r->s(1))->subclasses.push_back(c->qname);
            } else if (auto* nm = A::as<A::Name>(b); nm && EXCEPTIONS().count(Str(nm->id))) {
                c->exception = Str(nm->id);
            } else if (nm && nm->id == "object") {
            } else {
                c->foreign_base = unparse(b);
            }
        }
    }
    for (Class* c : classes.values()) {  // exceptions derived from exceptions
        Class* k = c;
        while (!k->exception && !k->bases.empty()) {
            k = classes.at(k->bases[0]);
        }
        c->exception = c->exception ? c->exception : k->exception;
    }
    for (Fn* f : fns.values()) {
        index_params[f->qname] = indexing_params(*f);
        f->none_aware = none_aware_of(*f);
    }
}

// ------------------------------------------------------------------------------------ type lookups
T Program::annotation_type(const Fn& f, const Str& name) {
    if (!f.annotations.contains(name)) {
        return P::BOTTOM();
    }
    const A::Expr* node = f.annotations.at(name);
    return ann_to_type(*f.module, node, node);
}

T Program::ann_to_type(Module& m, const A::Expr* a, const A::Expr* /*site_node*/) {
    auto it = ann_cache.find(a);
    if (it != ann_cache.end()) {
        return it->second;
    }
    T t = ann_(m, a);
    ann_cache.emplace(a, t);
    return t;
}

T Program::ann_(Module& m, const A::Expr* a) {
    if (auto s = str_const(a)) {
        // ast.parse(a.value, mode='eval').body: parsed as a module; a string that is one expression
        // statement is the same tree (anything else is a SyntaxError in eval mode and not an expression here)
        std::string src = s->utf8();
        A::ParseResult r = A::parse(src, "<unknown>");
        if (!r || r.module->body.size() != 1 || r.module->body[0]->kind != A::StmtKind::Expr) {
            return P::BOTTOM();
        }
        const A::Expr* inner = A::cast<A::ExprStmt>(r.module->body[0])->value;
        keep_.push_back(std::move(r.module));
        return ann_(m, inner);
    }
    if (is_none_const(a)) {
        return P::NONE();
    }
    std::optional<Str> name;
    if (auto* n = A::as<A::Name>(a)) {
        name = Str(n->id);
    } else if (auto* at = A::as<A::Attribute>(a)) {
        name = Str(at->attr);
    }
    auto where_of = [&](const A::Expr* x) { return std::optional<std::tuple<Str, int64_t>>(std::make_tuple(m.name, static_cast<int64_t>(x->pos.lineno))); };
    if (name) {
        if (lit_eq(*name, "int")) return P::INT();
        if (lit_eq(*name, "str")) return P::STR();
        if (lit_eq(*name, "bytes")) return P::BYTES();
        if (lit_eq(*name, "bytearray")) return P::BYTEARRAY();
        if (lit_eq(*name, "bool")) return P::BOOL();
        if (lit_eq(*name, "float")) return P::FLOAT();
        if (lit_eq(*name, "None")) return P::NONE();
        for (const char* nm : {"list", "List", "dict", "Dict", "set", "Set"}) {
            if (lit_eq(*name, nm)) {
                Str kind = name->lower();
                int64_t s = new_site(a, kind, where_of(a));
                if (lit_eq(kind, "dict")) {
                    make_dynamic(s, Str("annotated as a dict"));
                }
                return T(kind, {}, std::nullopt, std::nullopt, std::nullopt, s);
            }
        }
        if (A::as<A::Name>(a)) {
            std::optional<Ref> r = resolve_global(m, *name);
            if (r && r->is("class")) {
                return T(PS("inst"), {}, std::nullopt, std::nullopt, r->s(1));
            }
        }
        return P::BOTTOM();
    }
    if (auto* bo = A::as<A::BinOp>(a); bo && bo->op == A::OpKind::BitOr) {
        T l = ann_(m, bo->left);
        T r = ann_(m, bo->right);
        return join(l, r);
    }
    if (auto* sub = A::as<A::Subscript>(a)) {
        const A::Expr* v = sub->value;
        Str head;
        if (auto* n = A::as<A::Name>(v)) {
            head = Str(n->id);
        } else if (auto* at = A::as<A::Attribute>(v)) {
            head = Str(at->attr);
        }
        std::vector<const A::Expr*> args;
        if (auto* tu = A::as<A::Tuple>(sub->slice)) {
            for (const A::Expr* x : tu->elts) {
                args.push_back(x);
            }
        } else {
            args.push_back(sub->slice);
        }
        auto head_in = [&](std::initializer_list<const char*> names) {
            for (const char* n : names) {
                if (lit_eq(head, n)) {
                    return true;
                }
            }
            return false;
        };
        if (lit_eq(head, "Optional")) {
            return P::opt(ann_(m, at(args, 0)));
        }
        if (lit_eq(head, "Union")) {
            T t = P::BOTTOM();
            for (const A::Expr* x : args) {
                T xt = ann_(m, x);
                t = join(t, xt);
            }
            return t;
        }
        if (head_in({"list", "List", "set", "Set", "Sequence", "Iterable"})) {
            Str kind = head_in({"set", "Set"}) ? Str("set") : Str("list");
            int64_t s = new_site(a, kind, where_of(a));
            T et = ann_(m, at(args, 0));
            grow(elem, s, et);
            return T(kind, {}, std::nullopt, std::nullopt, std::nullopt, s);
        }
        if (head_in({"dict", "Dict", "Mapping"})) {
            int64_t s = new_site(a, Str("dict"), where_of(a));
            make_dynamic(s, Str("annotated as a dict"));
            if (args.size() == 2) {
                T kt = ann_(m, args[0]);
                grow(keys, s, kt);
                T vt = ann_(m, args[1]);
                grow(elem, s, vt);
            }
            return T(PS("dict"), {}, std::nullopt, std::nullopt, std::nullopt, s);
        }
        if (head_in({"tuple", "Tuple"})) {
            for (const A::Expr* x : args) {
                auto* c = A::as<A::Constant>(x);
                if (c && c->value.kind == A::ConstValue::Kind::Ellipsis) {
                    return P::top(Str("variable-length tuple annotation"));
                }
            }
            std::vector<T> ts;
            for (const A::Expr* x : args) {
                ts.push_back(ann_(m, x));
            }
            return P::tuple_of(ts);
        }
    }
    return P::BOTTOM();
}

T Program::param_type(const Fn& f, const Str& name) {
    read(fk('p', f.qname));
    read(fk('d'));
    T t = P::BOTTOM();
    if (params.contains(f.qname)) {
        py::dict<Str, T> ps = params.at(f.qname);
        if (ps.contains(name)) {
            t = ps.at(name);
        }
    }
    if (f.annotations.contains(name)) {
        T at_ = annotation_type(f, name);
        if (k_is(at_, "int")) {
            t = k_is(t, "int") ? t : at_;
        } else if (!k_is(at_, "bottom")) {
            t = at_;
        }
    }
    if (f.defaults.contains(name)) {
        auto key = std::make_tuple(f.qname, name);
        if (default_types.contains(key)) {
            t = join(t, default_types.at(key));
        }
    }
    return t;
}

T Program::global_type(const Str& mod, const Str& name) {
    read(fk('g', mod));
    if (globals.contains(mod)) {
        py::dict<Str, T> g = globals.at(mod);
        if (g.contains(name)) {
            return g.at(name);
        }
    }
    return P::BOTTOM();
}

Fn* Program::find_method(const Str& cq, const Str& name) {
    for (const Str& c : mro(cq)) {
        Class* k = classes.at(c);
        if (k->methods.contains(name)) {
            return k->methods.at(name);
        }
    }
    return nullptr;
}

const StrSet& Program::assigned_attrs(const Str& cq) {
    auto it = assigned_attrs_.find(cq);
    if (it == assigned_attrs_.end()) {
        StrSet got;
        for (Fn* f : classes.at(cq)->methods.values()) {
            if (f->params.empty() || lit_eq(f->kind, "static")) {
                continue;
            }
            const Str& me = f->params[0];
            for (const AnyNode& an : pyengine::walk(AnyNode(static_cast<const A::Stmt*>(f->node)))) {
                auto* n = an.expr();
                auto* at = A::as<A::Attribute>(n);
                if (at && at->ctx == A::ExprCtx::Store) {
                    if (auto* nm = A::as<A::Name>(at->value); nm && Str(nm->id) == me) {
                        got.insert(Str(at->attr));
                    }
                }
            }
        }
        it = assigned_attrs_.emplace(cq, std::move(got)).first;
    }
    return it->second;
}

StrSet Program::assigned_attrs_any(const Str& cq) {
    StrSet out = assigned_attrs(cq);
    for (const Str& d : descendants(cq)) {
        const StrSet& s = assigned_attrs(d);
        out.insert(s.begin(), s.end());
    }
    return out;
}

std::optional<Str> Program::attr_owner(const Str& cq, const Str& attr) {
    if (!classes.contains(cq)) {
        return std::nullopt;
    }
    std::vector<Str> m = mro(cq);
    for (auto it = m.rbegin(); it != m.rend(); ++it) {
        const Str& c = *it;
        if (classes.at(c)->class_attrs.contains(attr) || assigned_attrs(c).count(attr) ||
            (attrs.contains(c) && attrs.at(c).contains(attr))) {
            return c;
        }
    }
    return std::nullopt;
}

std::optional<T> Program::attr_type(const Str& cq, const Str& attr) {
    for (const Str& c : mro(cq)) {
        read(fk('a', c));
    }
    read(fk('C'));
    std::optional<Str> owner = attr_owner(cq, attr);
    if (!owner) {
        for (const Str& d : descendants(cq)) {
            if (assigned_attrs(d).count(attr)) {
                return P::top(Str("attribute ") + attr + Str(" only set by a subclass"));
            }
        }
        return std::nullopt;
    }
    auto pk = std::make_tuple(*owner, attr);
    if (poisoned.contains(pk)) {
        return P::top(Str("untyped:attribute ") + attr + Str(" is also stored a value of no single type (by ") +
                      last_part(poisoned.at(pk)) + Str(")"));
    }
    T t = P::BOTTOM();
    if (attrs.contains(*owner)) {
        py::dict<Str, T> a = attrs.at(*owner);
        if (a.contains(attr)) {
            t = a.at(attr);
        }
    }
    for (const Str& c : mro(cq)) {
        if (classes.at(c)->class_attrs.contains(attr)) {
            auto ck = std::make_tuple(c, attr);
            T ct = class_attr_types.contains(ck) ? class_attr_types.at(ck) : P::BOTTOM();
            t = join(t, ct);
        }
    }
    return t;
}

T Program::class_attr_type(const Str& cq, const Str& attr) {
    read(fk('C'));
    auto key = std::make_tuple(cq, attr);
    return class_attr_types.contains(key) ? class_attr_types.at(key) : P::BOTTOM();
}

const std::vector<A::Stmt*>* Program::block_of(const A::Stmt* st) const {
    auto it = blocks_.find(st);
    return it == blocks_.end() ? nullptr : it->second;
}

void Program::map_blocks(const A::FunctionDef* node) {
    for (const AnyNode& an : pyengine::walk(AnyNode(static_cast<const A::Stmt*>(node)))) {
        std::vector<const std::vector<A::Stmt*>*> lists;
        if (const A::Stmt* s = an.stmt()) {
            using K = A::StmtKind;
            switch (s->kind) {
                case K::FunctionDef: case K::AsyncFunctionDef: lists = {&A::cast<A::FunctionDef>(s)->body}; break;
                case K::ClassDef: lists = {&A::cast<A::ClassDef>(s)->body}; break;
                case K::For: case K::AsyncFor: lists = {&A::cast<A::For>(s)->body, &A::cast<A::For>(s)->orelse}; break;
                case K::While: lists = {&A::cast<A::While>(s)->body, &A::cast<A::While>(s)->orelse}; break;
                case K::If: lists = {&A::cast<A::If>(s)->body, &A::cast<A::If>(s)->orelse}; break;
                case K::With: case K::AsyncWith: lists = {&A::cast<A::With>(s)->body}; break;
                case K::Try: case K::TryStar:
                    lists = {&A::cast<A::Try>(s)->body, &A::cast<A::Try>(s)->orelse, &A::cast<A::Try>(s)->finalbody};
                    break;
                default: break;
            }
        } else if (an.cat == AnyNode::Handler) {
            lists = {&static_cast<const A::ExceptHandler*>(an.p)->body};
        } else if (an.cat == AnyNode::MatchCase) {
            lists = {&static_cast<const A::match_case*>(an.p)->body};
        }
        for (const auto* lst : lists) {
            for (const A::Stmt* s : *lst) {
                blocks_[s] = lst;
            }
        }
    }
}

// ------------------------------------------------------------------------------------ solving
int64_t Program::solve(int64_t max_rounds, const std::function<void(const Str&)>& log) {
    for (Fn* f : fns.values()) {
        map_blocks(f->node);
    }
    std::unordered_map<Str, FactSet, py::str_hash> mod_deps;   // ('mod', name) -> the facts its last run read
    std::unordered_map<Str, FactSet, py::str_hash> fn_deps;    // fn qname -> the facts its last run read
    auto meets = [](const FactSet& a, const FactSet& b) {
        for (const FactKey& k : a) {
            if (b.count(k)) {
                return true;
            }
        }
        return false;
    };
    bool have_prev = false;
    FactSet prev;
    for (int64_t r = 0; r < max_rounds; ++r) {
        round = r;
        changed = false;
        dirty = FactSet();
        int64_t ran = 0;
        for (Module* m : modules.values()) {
            if (have_prev) {
                auto it = mod_deps.find(m->name);
                if (!(it != mod_deps.end() && meets(it->second, prev))) {
                    continue;
                }
            }
            FactSet reads_now;
            reads = &reads_now;
            suspend = 0;
            infer::ModuleEval(*this, m).run();
            mod_deps[m->name] = reads_now;
            reads = nullptr;
        }
        for (Fn* f : fns.values()) {
            if (crashes.contains(f->qname)) {
                continue;
            }
            if (have_prev) {
                auto it = fn_deps.find(f->qname);
                if (!(it != fn_deps.end() && meets(it->second, prev))) {
                    continue;
                }
            }
            ++ran;
            FactSet reads_now;
            reads = &reads_now;
            suspend = 0;
            try {
                infer::Eval(*this, f).run();
            } catch (const py::Exception& e) {  // a front end bug: the function is refused
                crashes.set(f->qname, Str(e.type()) + Str(": ") + Str(std::string(e.what())));
            } catch (const std::exception& e) {
                crashes.set(f->qname, Str("RuntimeError: ") + Str(std::string(e.what())));
            }
            fn_deps[f->qname] = reads_now;
            reads = nullptr;
        }
        if (log) {
            log(Str("round ") + py::to_str(r) + Str(": ran ") + py::to_str(ran) + Str(", ") +
                py::to_str(static_cast<int64_t>(dirty.size())) + Str(" facts changed"));
        }
        if (dirty.empty()) {
            return r + 1;
        }
        prev = dirty;
        have_prev = true;
        // a merged container's old root is read under its new root from now on
        std::vector<FactKey> sites;
        for (const FactKey& k : prev) {
            if (k.tag == 's') {
                sites.push_back(k);
            }
        }
        for (const FactKey& k : sites) {
            prev.insert(fk_site(find(k.site)));
        }
    }
    return max_rounds;
}

std::shared_ptr<infer::Eval> Program::evaluate(Fn& f) {
    suspend = 0;
    std::shared_ptr<infer::Eval> ev;
    try {
        ev = std::make_shared<infer::Eval>(*this, &f, true);
        ev->run();
    } catch (...) {
        suspend = 0;
        throw;
    }
    suspend = 0;
    return ev;
}

void Program::index_stmt(Module& m, const A::Stmt* st) {
    using K = A::StmtKind;
    switch (st->kind) {
        case K::FunctionDef:
        case K::AsyncFunctionDef: {
            auto* fd = A::cast<A::FunctionDef>(st);
            auto f = std::make_unique<Fn>(m.name + Str(".") + Str(fd->name), fd, &m);
            if (st->kind == K::AsyncFunctionDef) {
                f->kind = Str("async");
            }
            Fn* raw = f.get();
            owned_fns_.push_back(std::move(f));
            m.functions.set(Str(fd->name), raw);
            fns.set(raw->qname, raw);
            break;
        }
        case K::ClassDef: {
            auto* cd = A::cast<A::ClassDef>(st);
            auto c = std::make_unique<Class>(m.name + Str(".") + Str(cd->name), cd, &m);
            Class* craw = c.get();
            owned_classes_.push_back(std::move(c));
            m.classes.set(Str(cd->name), craw);
            classes.set(craw->qname, craw);
            for (const A::Stmt* b : cd->body) {
                if (b->kind == K::FunctionDef || b->kind == K::AsyncFunctionDef) {
                    auto* bf = A::cast<A::FunctionDef>(b);
                    auto f = std::make_unique<Fn>(craw->qname + Str(".") + Str(bf->name), bf, &m, craw);
                    Fn* raw = f.get();
                    owned_fns_.push_back(std::move(f));
                    craw->methods.set(Str(bf->name), raw);
                    fns.set(raw->qname, raw);
                } else if (auto* as_ = A::as<A::Assign>(b); as_ && as_->targets.size() == 1 && A::as<A::Name>(as_->targets[0])) {
                    craw->class_attrs.set(Str(A::cast<A::Name>(as_->targets[0])->id), as_->value);
                } else if (auto* an = A::as<A::AnnAssign>(b); an && A::as<A::Name>(an->target) && an->value) {
                    craw->class_attrs.set(Str(A::cast<A::Name>(an->target)->id), an->value);
                }
            }
            break;
        }
        case K::Import: {
            for (const A::alias* a : A::cast<A::Import>(st)->names) {
                if (a->has_asname) {
                    m.imports.set(Str(a->asname), module_ref(Str(a->name)));
                } else {
                    size_t dot = a->name.find('.');
                    Str top = Str(dot == std::string::npos ? a->name : a->name.substr(0, dot));
                    m.imports.set(top, module_ref(top));
                }
            }
            break;
        }
        case K::ImportFrom: {
            auto* imp = A::cast<A::ImportFrom>(st);
            Str base = from_base(m, imp);
            for (const A::alias* a : imp->names) {
                if (a->name == "*") {
                    continue;
                }
                Str local = a->has_asname ? Str(a->asname) : Str(a->name);
                Str full = !base.empty() ? base + Str(".") + Str(a->name) : Str(a->name);
                if (modules.contains(full)) {
                    m.imports.set(local, Ref(PS("module"), {item(full)}));
                } else if (modules.contains(base)) {
                    m.imports.set(local, Ref(PS("obj"), {item(base), item(Str(a->name))}));
                } else {
                    m.imports.set(local, Ref(PS("extern"), {item(full)}));
                }
            }
            break;
        }
        case K::Assign: {
            auto* as_ = A::cast<A::Assign>(st);
            for (const A::Expr* t : as_->targets) {
                for (const Str& n : target_names(t)) {
                    m.assigns[n].push_back(A::as<A::Name>(t) ? as_->value : nullptr);
                }
            }
            break;
        }
        case K::AnnAssign: {
            auto* an = A::cast<A::AnnAssign>(st);
            if (auto* nm = A::as<A::Name>(an->target)) {
                m.assigns[Str(nm->id)].push_back(an->value);
            } else {
                m.statements.push_back(st);
            }
            break;
        }
        case K::If:
        case K::Try: {
            // `if TYPE_CHECKING:` / `try: import x except ImportError:` -- index what they define
            std::vector<const A::Stmt*> inner;
            if (st->kind == K::If) {
                auto* i = A::cast<A::If>(st);
                inner.assign(i->body.begin(), i->body.end());
                inner.insert(inner.end(), i->orelse.begin(), i->orelse.end());
            } else {
                auto* t = A::cast<A::Try>(st);
                inner.assign(t->body.begin(), t->body.end());
                inner.insert(inner.end(), t->orelse.begin(), t->orelse.end());
            }
            for (const A::Stmt* b : inner) {
                index_stmt(m, b);
            }
            if (st->kind == K::Try) {
                for (const A::ExceptHandler* h : A::cast<A::Try>(st)->handlers) {
                    for (const A::Stmt* b : h->body) {
                        index_stmt(m, b);
                    }
                }
            }
            m.statements.push_back(st);
            break;
        }
        case K::Expr: {
            if (A::as<A::Constant>(A::cast<A::ExprStmt>(st)->value)) {
                break;  // a docstring
            }
            m.statements.push_back(st);
            break;
        }
        default: m.statements.push_back(st); break;
    }
}

Ref Program::module_ref(const Str& name) {
    if (modules.contains(name)) {
        return Ref(PS("module"), {item(name)});
    }
    const std::u32string& u = name.u32();
    size_t dot = u.find(U'.');
    Str top = dot == std::u32string::npos ? name : Str(u.substr(0, dot));
    if (STDLIB().count(top)) {
        return Ref(PS("stdmod"), {item(name)});
    }
    return Ref(PS("extern"), {item(name)});
}

Str Program::from_base(const Module& m, const A::ImportFrom* st) {
    if (!st->level) {
        return st->has_module ? Str(st->module) : Str();
    }
    // parts = m.name.split('.')
    std::vector<Str> parts;
    {
        const std::u32string& u = m.name.u32();
        size_t b = 0;
        for (;;) {
            size_t p = u.find(U'.', b);
            if (p == std::u32string::npos) {
                parts.push_back(Str(u.substr(b)));
                break;
            }
            parts.push_back(Str(u.substr(b, p - b)));
            b = p + 1;
        }
    }
    int64_t keep = static_cast<int64_t>(parts.size()) - st->level + (m.is_package ? 1 : 0);
    // '.'.join(parts[:keep]) with Python's slice clamping
    int64_t n = static_cast<int64_t>(parts.size());
    int64_t end = keep < 0 ? std::max<int64_t>(keep + n, 0) : std::min(keep, n);
    Str base;
    for (int64_t i = 0; i < end; ++i) {
        if (i) {
            base += Str(".");
        }
        base += parts[static_cast<size_t>(i)];
    }
    return base + (st->has_module && !st->module.empty() ? Str(".") + Str(st->module) : Str());
}

std::optional<Ref> Program::resolve_global(Module& m, const Str& name) {
    if (m.functions.contains(name)) {
        return Ref(PS("fn"), {item(m.functions.at(name)->qname)});
    }
    if (m.classes.contains(name)) {
        return Ref(PS("class"), {item(m.classes.at(name)->qname)});
    }
    if (m.assigns.contains(name)) {
        return Ref(PS("global"), {item(m.name), item(name)});
    }
    if (!m.imports.contains(name)) {
        return std::nullopt;
    }
    Ref imp = m.imports.at(name);
    if (imp.is("obj")) {
        Module& other = *modules.at(imp.s(1));
        const Str& nm = imp.s(2);
        if (other.functions.contains(nm) || other.classes.contains(nm) || other.assigns.contains(nm) || other.imports.contains(nm)) {
            return resolve_global(other, nm);
        }
        return Ref(PS("missing"), {item(imp.s(1) + Str(".") + nm)});
    }
    if (imp.is("extern")) {
        const std::u32string& u = imp.s(1).u32();
        size_t dot = u.find(U'.');
        Str top = dot == std::u32string::npos ? imp.s(1) : Str(u.substr(0, dot));
        if (STDLIB().count(top)) {
            return Ref(PS("stdobj"), {item(imp.s(1))});
        }
    }
    return imp;
}

std::optional<Ref> Program::resolve_expr_name(Module& m, const A::Expr* node) {
    if (auto* n = A::as<A::Name>(node)) {
        return resolve_global(m, Str(n->id));
    }
    if (auto* at = A::as<A::Attribute>(node)) {
        std::optional<Ref> r = resolve_expr_name(m, at->value);
        if (r && r->is("module")) {
            return resolve_global(*modules.at(r->s(1)), Str(at->attr));
        }
    }
    return std::nullopt;
}

// ------------------------------------------------------------------------------------ containers
int64_t Program::find(int64_t s) {
    int64_t root = s;
    while (parent[static_cast<size_t>(root)] != root) {
        root = parent[static_cast<size_t>(root)];
    }
    while (parent[static_cast<size_t>(s)] != root) {
        int64_t nxt = parent[static_cast<size_t>(s)];
        parent[static_cast<size_t>(s)] = root;
        s = nxt;
    }
    if (reads != nullptr) {
        reads->insert(fk_site(root));
    }
    return root;
}

T Program::settle(const T& old, const T& new_, int64_t depth) {
    if (round <= 3 || new_ == old) {
        return new_;
    }
    if (k_is(new_, "int") && k_is(old, "int")) {
        return P::widen_int(old, new_);
    }
    if ((k_is(new_, "tuple") || k_is(new_, "opt")) && old.kind == new_.kind && old.args.size() == new_.args.size()) {
        if (depth > 6) {
            return P::top(Str("a type that keeps growing"));
        }
        std::vector<T> as;
        for (size_t i = 0; i < new_.args.size(); ++i) {
            as.push_back(settle(old.args[i], new_.args[i], depth + 1));
        }
        return T(new_.kind, as);
    }
    return new_;
}

void Program::touch(const FactKey& key) {
    changed = true;
    dirty.insert(key);
}

void Program::read(const FactKey& key) {
    if (reads != nullptr) {
        reads->insert(key);
    }
}

T Program::ret(const Str& fq) {
    read(fk('r', fq));
    return returns.contains(fq) ? returns.at(fq) : P::BOTTOM();
}

const std::set<Str>& Program::fn_locals(const Fn& f) {
    auto it = fn_locals_.find(f.qname);
    if (it == fn_locals_.end()) {
        std::set<Str> got = infer::assigned_names(f.node);
        for (const Str& p : f.params) {
            got.insert(p);
        }
        for (const Str& p : f.kwonly) {
            got.insert(p);
        }
        it = fn_locals_.emplace(f.qname, std::move(got)).first;
    }
    return it->second;
}

const std::vector<const A::Expr*>& Program::access_nodes(const A::Node* node) {
    auto it = access_.find(node);
    if (it != access_.end()) {
        return it->second;
    }
    std::vector<const A::Expr*> got;
    // node is an expression or a statement
    std::vector<AnyNode> todo;
    if (auto* e = dynamic_cast<const A::Expr*>(node)) {
        todo.push_back(AnyNode(e));
    } else if (auto* s = dynamic_cast<const A::Stmt*>(node)) {
        todo.push_back(AnyNode(s));
    }
    while (!todo.empty()) {
        AnyNode n = todo.back();
        todo.pop_back();
        if (const A::Expr* e = n.expr()) {
            using K = A::ExprKind;
            if (e->kind == K::Lambda || e->kind == K::ListComp || e->kind == K::GeneratorExp || e->kind == K::SetComp ||
                e->kind == K::DictComp) {
                continue;
            }
            if (e->kind == K::Subscript || e->kind == K::Call) {
                got.push_back(e);
            }
        }
        pyengine::child_nodes(n, todo);
    }
    std::reverse(got.begin(), got.end());
    return access_.emplace(node, std::move(got)).first->second;
}

int64_t Program::new_site(const A::Node* node, const Str& kind, const std::optional<std::tuple<Str, int64_t>>& where) {
    auto it = site_of_node.find(node);
    if (it != site_of_node.end()) {
        return it->second;
    }
    int64_t s = static_cast<int64_t>(parent.size());
    site_of_node.emplace(node, s);
    parent.push_back(s);
    ckind.push_back(kind);
    elem.push_back(P::BOTTOM());
    keys.push_back(P::BOTTOM());
    fields.push_back(py::dict<Str, T>());
    required.push_back(std::nullopt);
    site_where.push_back(where);
    site_node.push_back(node);
    touch(fk_site(s));
    return s;
}

int64_t Program::union_(int64_t a, int64_t b) {
    a = find(a);
    b = find(b);
    if (a == b) {
        return a;
    }
    if (a > b) {
        std::swap(a, b);
    }
    parent[static_cast<size_t>(b)] = a;
    touch(fk_site(a));
    touch(fk_site(b));
    size_t ia = static_cast<size_t>(a), ib = static_cast<size_t>(b);
    if (!(ckind[ia] == ckind[ib])) {
        dynamic.set(a, Str("a ") + ckind[ia] + Str(" and a ") + ckind[ib] + Str(" flow together"));
    }
    T je = join(elem[ia], elem[ib]);
    elem[ia] = je;
    T jk = join(keys[ia], keys[ib]);
    keys[ia] = jk;
    py::dict<Str, T> fa = fields[ia];
    for (const auto& kv : fields[ib].items()) {
        const Str& k = std::get<0>(kv);
        T jt = join(fa.contains(k) ? fa.at(k) : P::BOTTOM(), std::get<1>(kv));
        fa.set(k, jt);
    }
    std::optional<StrSet> ra = required[ia], rb = required[ib];
    if (!ra) {
        required[ia] = rb;
    } else if (!rb) {
        required[ia] = ra;
    } else {
        StrSet inter;
        for (const Str& k : *ra) {
            if (rb->count(k)) {
                inter.insert(k);
            }
        }
        required[ia] = inter;
    }
    if (dynamic.contains(b) && !dynamic.contains(a)) {
        dynamic.set(a, dynamic.at(b));
    }
    {
        // self.hints.setdefault(a, []).extend(self.hints.pop(b, []))
        std::vector<Str> moved = hints.contains(b) ? hints.pop(b) : std::vector<Str>();
        std::vector<Str>& dst = hints[a];
        dst.insert(dst.end(), moved.begin(), moved.end());
    }
    if (fixed.count(b)) {
        fixed.insert(a);
    }
    if (mutated.count(b)) {
        mutated.insert(a);
    }
    if (poisoned_sites.contains(b) && !poisoned_sites.contains(a)) {
        poisoned_sites.set(a, poisoned_sites.at(b));
    }
    return a;
}

void Program::mark_mutated(int64_t s) {
    s = find(s);
    if (!mutated.count(s)) {
        mutated.insert(s);
        touch(fk_site(s));
    }
}

int64_t Program::grow(std::vector<T>& table, int64_t s, const T& t) {
    s = find(s);
    if (k_is(t, "top") && !suspend && !poisoned_sites.contains(s)) {
        poisoned_sites.set(s, t.why && !t.why->empty() ? *t.why : Str("?"));  // its readers cannot know what its items are
        touch(fk_site(s));
    }
    if (suspend || k_is(t, "top")) {
        return s;
    }
    T old = table[static_cast<size_t>(s)];
    T new_ = settle(old, join(old, t));
    if (new_ != old) {
        table[static_cast<size_t>(s)] = new_;
        touch(fk_site(s));
    }
    return s;
}

void Program::grow_field(int64_t s, const Str& key, const T& t) {
    s = find(s);
    py::dict<Str, T> fs = fields[static_cast<size_t>(s)];
    if (k_is(t, "top")) {
        if (!fs.contains(key)) {
            fs.set(key, P::BOTTOM());
        }
        if (!suspend && !poisoned_sites.contains(s)) {
            poisoned_sites.set(s, t.why && !t.why->empty() ? *t.why : Str("?"));
            touch(fk_site(s));
        }
        return;
    }
    if (suspend) {
        if (!fs.contains(key)) {
            fs.set(key, P::BOTTOM());
        }
        return;
    }
    T old = fs.contains(key) ? fs.at(key) : P::BOTTOM();
    T new_ = settle(old, join(old, t));
    if (!fs.contains(key) || new_ != old) {
        fs.set(key, new_);
        touch(fk_site(s));
    }
    grow(elem, s, t);
    grow(keys, s, P::STR());
}

void Program::created(int64_t s, const StrSet& ks) {
    s = find(s);
    if (suspend) {
        return;
    }
    const std::optional<StrSet>& old = required[static_cast<size_t>(s)];
    std::optional<StrSet> new_;
    if (!old) {
        new_ = ks;
    } else {
        StrSet inter;
        for (const Str& k : *old) {
            if (ks.count(k)) {
                inter.insert(k);
            }
        }
        new_ = inter;
    }
    if (new_ != old) {
        required[static_cast<size_t>(s)] = new_;
        touch(fk_site(s));
    }
}

void Program::make_dynamic(int64_t s, const Str& why) {
    s = find(s);
    if (!dynamic.contains(s)) {
        dynamic.set(s, why);
        touch(fk_site(s));
    }
}

bool Program::is_record(int64_t s) {
    s = find(s);
    return lit_eq(ckind[static_cast<size_t>(s)], "dict") && !dynamic.contains(s);
}

void Program::hint(int64_t s, const Str& name) {
    s = find(s);
    std::vector<Str>& h = hints[s];
    if (std::find(h.begin(), h.end(), name) == h.end()) {
        h.push_back(name);
    }
}

// ------------------------------------------------------------------------------------ the join
namespace {

// _tname
Str tname(const T& t) {
    if (k_in(t, {"list", "dict", "set"})) {
        return t.kind;
    }
    if (k_is(t, "inst")) {
        return last_part(*t.name);
    }
    return t.kind;
}

}  // namespace

T Program::join(const T& a, const T& b) {
    if (&a == &b || a == b) {
        return a;
    }
    const Str& ka = a.kind;
    const Str& kb = b.kind;
    if (lit_eq(ka, "bottom")) {
        return b;
    }
    if (lit_eq(kb, "bottom")) {
        return a;
    }
    if (lit_eq(ka, "top")) {
        return a;
    }
    if (lit_eq(kb, "top")) {
        return b;
    }
    if (lit_eq(ka, "none") || lit_eq(kb, "none")) {
        const T& other = lit_eq(ka, "none") ? b : a;
        if (k_is(other, "opt")) {
            return other;
        }
        return P::opt(other);
    }
    if (lit_eq(ka, "opt") || lit_eq(kb, "opt")) {
        T ia = P::strip_opt(a);
        T ib = P::strip_opt(b);
        T j = join(ia, ib);
        return k_is(j, "top") ? j : P::opt(j);
    }
    if (!(ka == kb)) {
        if ((lit_eq(ka, "bytes") && lit_eq(kb, "bytearray")) || (lit_eq(ka, "bytearray") && lit_eq(kb, "bytes"))) {
            return P::top(Str("bytes and bytearray mixed"));
        }
        return P::top(tname(a) + Str(" and ") + tname(b) + Str(" mixed"));
    }
    if (lit_eq(ka, "int")) {
        return P::int_join(a, b);
    }
    if (lit_eq(ka, "bytes") || lit_eq(ka, "bytearray")) {
        Bound hi;
        if (a.hi && b.hi) {
            hi = *a.hi > *b.hi ? *a.hi : *b.hi;
        }
        return T(ka, {}, std::nullopt, hi);
    }
    if (lit_eq(ka, "tuple")) {
        if (a.args.size() != b.args.size()) {
            return P::top(Str("tuples of ") + py::to_str(static_cast<int64_t>(a.args.size())) + Str(" and ") +
                          py::to_str(static_cast<int64_t>(b.args.size())) + Str(" items mixed"));
        }
        std::vector<T> parts;
        for (size_t i = 0; i < a.args.size(); ++i) {
            parts.push_back(join(a.args[i], b.args[i]));
        }
        for (const T& p_ : parts) {
            if (k_is(p_, "top")) {
                return p_;
            }
        }
        return P::tuple_of(parts);
    }
    if (lit_eq(ka, "list") || lit_eq(ka, "dict") || lit_eq(ka, "set")) {
        return T(ka, {}, std::nullopt, std::nullopt, std::nullopt, union_(*a.site, *b.site));
    }
    if (lit_eq(ka, "inst")) {
        std::optional<Str> c = common_base(*a.name, *b.name);
        if (c) {
            return T(PS("inst"), {}, std::nullopt, std::nullopt, *c);
        }
        return P::top(Str("instances of ") + *a.name + Str(" and ") + *b.name + Str(" mixed"));
    }
    return P::top(tname(a) + Str(" and ") + tname(b) + Str(" mixed"));
}

std::optional<Str> Program::common_base(const Str& x, const Str& y) {
    std::vector<Str> anc = mro(x);
    for (const Str& c : mro(y)) {
        if (std::find(anc.begin(), anc.end(), c) != anc.end()) {
            return c;
        }
    }
    return std::nullopt;
}

std::vector<Str> Program::mro(const Str& cq) {
    std::vector<Str> out;
    std::vector<Str> todo = {cq};
    while (!todo.empty()) {
        Str c = todo.front();
        todo.erase(todo.begin());
        if (std::find(out.begin(), out.end(), c) != out.end() || !classes.contains(c)) {
            continue;
        }
        out.push_back(c);
        for (const Str& b : classes.at(c)->bases) {
            todo.push_back(b);
        }
    }
    return out;
}

std::vector<Str> Program::descendants(const Str& cq) {
    std::vector<Str> out;
    std::vector<Str> todo = {cq};
    while (!todo.empty()) {
        Str c = todo.back();
        todo.pop_back();
        for (const Str& s : classes.at(c)->subclasses) {
            if (std::find(out.begin(), out.end(), s) == out.end()) {
                out.push_back(s);
                todo.push_back(s);
            }
        }
    }
    return out;
}

T Program::canon(const T& t) {
    if (k_in(t, {"list", "dict", "set"})) {
        int64_t r = find(*t.site);
        return r == *t.site ? t : T(t.kind, {}, std::nullopt, std::nullopt, std::nullopt, r);
    }
    if (!t.args.empty()) {
        std::vector<T> as;
        for (const T& a : t.args) {
            as.push_back(canon(a));
        }
        return T(t.kind, as, t.lo, t.hi, t.name, t.site, t.why);
    }
    return t;
}

}  // namespace codebreaker::translate::frontends::python::program
