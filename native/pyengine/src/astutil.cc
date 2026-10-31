// astutil.cc -- the generic walker and the `ast` module's helpers (see astutil.hpp).
#include "astutil.hpp"

#include <algorithm>
#include <cstdio>
#include <deque>

#include "pyfront/pyfront.hpp"

namespace pyengine {

using namespace pyfront;

static void push(std::vector<AnyNode>& out, const pyfront::Expr* e) {
    if (e) out.emplace_back(e);
}
static void push(std::vector<AnyNode>& out, const pyfront::Stmt* s) {
    if (s) out.emplace_back(s);
}
template <class V> static void push_all(std::vector<AnyNode>& out, const V& v) {
    for (auto* x : v) push(out, x);
}
static void push_args(std::vector<AnyNode>& out, const arguments* a) {
    if (a) out.emplace_back(a, AnyNode::Arguments);
}
static void push_comps(std::vector<AnyNode>& out, const std::vector<comprehension*>& gs) {
    for (auto* g : gs) out.emplace_back(g, AnyNode::Comprehension);
}
static void push_kws(std::vector<AnyNode>& out, const std::vector<keyword*>& ks) {
    for (auto* k : ks) out.emplace_back(k, AnyNode::Keyword);
}
static void push_tps(std::vector<AnyNode>& out, const std::vector<pyfront::TypeParam*>& ts) {
    for (auto* t : ts) out.emplace_back(t, AnyNode::TypeParam);
}
static void push_pats(std::vector<AnyNode>& out, const std::vector<pyfront::Pattern*>& ps) {
    for (auto* p : ps)
        if (p) out.emplace_back(p, AnyNode::Pattern);
}

void child_nodes(AnyNode n, std::vector<AnyNode>& out) {
    switch (n.cat) {
        case AnyNode::Expr: {
            const pyfront::Expr* e = n.expr();
            switch (e->kind) {
                case ExprKind::BoolOp: push_all(out, cast<BoolOp>(e)->values); break;
                case ExprKind::NamedExpr: push(out, cast<NamedExpr>(e)->target); push(out, cast<NamedExpr>(e)->value); break;
                case ExprKind::BinOp: push(out, cast<BinOp>(e)->left); push(out, cast<BinOp>(e)->right); break;
                case ExprKind::UnaryOp: push(out, cast<UnaryOp>(e)->operand); break;
                case ExprKind::Lambda: push_args(out, cast<pyfront::Lambda>(e)->args); push(out, cast<pyfront::Lambda>(e)->body); break;
                case ExprKind::IfExp: push(out, cast<IfExp>(e)->test); push(out, cast<IfExp>(e)->body); push(out, cast<IfExp>(e)->orelse); break;
                case ExprKind::Dict: push_all(out, cast<pyfront::Dict>(e)->keys); push_all(out, cast<pyfront::Dict>(e)->values); break;
                case ExprKind::Set: push_all(out, cast<pyfront::Set>(e)->elts); break;
                case ExprKind::ListComp: push(out, cast<ListComp>(e)->elt); push_comps(out, cast<ListComp>(e)->generators); break;
                case ExprKind::SetComp: push(out, cast<SetComp>(e)->elt); push_comps(out, cast<SetComp>(e)->generators); break;
                case ExprKind::DictComp: push(out, cast<DictComp>(e)->key); push(out, cast<DictComp>(e)->value); push_comps(out, cast<DictComp>(e)->generators); break;
                case ExprKind::GeneratorExp: push(out, cast<GeneratorExp>(e)->elt); push_comps(out, cast<GeneratorExp>(e)->generators); break;
                case ExprKind::Await: push(out, cast<Await>(e)->value); break;
                case ExprKind::Yield: push(out, cast<pyfront::Yield>(e)->value); break;
                case ExprKind::YieldFrom: push(out, cast<YieldFrom>(e)->value); break;
                case ExprKind::Compare: push(out, cast<Compare>(e)->left); push_all(out, cast<Compare>(e)->comparators); break;
                case ExprKind::Call: push(out, cast<pyfront::Call>(e)->func); push_all(out, cast<pyfront::Call>(e)->args); push_kws(out, cast<pyfront::Call>(e)->keywords); break;
                case ExprKind::FormattedValue: push(out, cast<FormattedValue>(e)->value); push(out, cast<FormattedValue>(e)->format_spec); break;
                case ExprKind::Interpolation: push(out, cast<Interpolation>(e)->value); push(out, cast<Interpolation>(e)->format_spec); break;
                case ExprKind::JoinedStr: push_all(out, cast<JoinedStr>(e)->values); break;
                case ExprKind::TemplateStr: push_all(out, cast<TemplateStr>(e)->values); break;
                case ExprKind::Attribute: push(out, cast<Attribute>(e)->value); break;
                case ExprKind::Subscript: push(out, cast<Subscript>(e)->value); push(out, cast<Subscript>(e)->slice); break;
                case ExprKind::Starred: push(out, cast<Starred>(e)->value); break;
                case ExprKind::List: push_all(out, cast<pyfront::List>(e)->elts); break;
                case ExprKind::Tuple: push_all(out, cast<pyfront::Tuple>(e)->elts); break;
                case ExprKind::Slice: push(out, cast<pyfront::Slice>(e)->lower); push(out, cast<pyfront::Slice>(e)->upper); push(out, cast<pyfront::Slice>(e)->step); break;
                default: break;
            }
            break;
        }
        case AnyNode::Stmt: {
            const pyfront::Stmt* s = n.stmt();
            switch (s->kind) {
                case StmtKind::FunctionDef:
                case StmtKind::AsyncFunctionDef: {
                    auto* f = cast<FunctionDef>(s);
                    push_args(out, f->args);
                    push_all(out, f->body);
                    push_all(out, f->decorator_list);
                    push(out, f->returns);
                    push_tps(out, f->type_params);
                    break;
                }
                case StmtKind::ClassDef: {
                    auto* c = cast<ClassDef>(s);
                    push_all(out, c->bases);
                    push_kws(out, c->keywords);
                    push_all(out, c->body);
                    push_all(out, c->decorator_list);
                    push_tps(out, c->type_params);
                    break;
                }
                case StmtKind::Return: push(out, cast<pyfront::Return>(s)->value); break;
                case StmtKind::Delete: push_all(out, cast<Delete>(s)->targets); break;
                case StmtKind::Assign: push_all(out, cast<pyfront::Assign>(s)->targets); push(out, cast<pyfront::Assign>(s)->value); break;
                case StmtKind::TypeAlias: push(out, cast<TypeAlias>(s)->name); push_tps(out, cast<TypeAlias>(s)->type_params); push(out, cast<TypeAlias>(s)->value); break;
                case StmtKind::AugAssign: push(out, cast<AugAssign>(s)->target); push(out, cast<AugAssign>(s)->value); break;
                case StmtKind::AnnAssign: push(out, cast<AnnAssign>(s)->target); push(out, cast<AnnAssign>(s)->annotation); push(out, cast<AnnAssign>(s)->value); break;
                case StmtKind::For:
                case StmtKind::AsyncFor: {
                    auto* f = cast<pyfront::For>(s);
                    push(out, f->target);
                    push(out, f->iter);
                    push_all(out, f->body);
                    push_all(out, f->orelse);
                    break;
                }
                case StmtKind::While: push(out, cast<pyfront::While>(s)->test); push_all(out, cast<pyfront::While>(s)->body); push_all(out, cast<pyfront::While>(s)->orelse); break;
                case StmtKind::If: push(out, cast<pyfront::If>(s)->test); push_all(out, cast<pyfront::If>(s)->body); push_all(out, cast<pyfront::If>(s)->orelse); break;
                case StmtKind::With:
                case StmtKind::AsyncWith: {
                    auto* w = cast<With>(s);
                    for (auto* it : w->items) out.emplace_back(it, AnyNode::WithItem);
                    push_all(out, w->body);
                    break;
                }
                case StmtKind::Match: {
                    auto* m = cast<Match>(s);
                    push(out, m->subject);
                    for (auto* c : m->cases) out.emplace_back(c, AnyNode::MatchCase);
                    break;
                }
                case StmtKind::Raise: push(out, cast<Raise>(s)->exc); push(out, cast<Raise>(s)->cause); break;
                case StmtKind::Try:
                case StmtKind::TryStar: {
                    auto* t = cast<pyfront::Try>(s);
                    push_all(out, t->body);
                    for (auto* h : t->handlers) out.emplace_back(h, AnyNode::Handler);
                    push_all(out, t->orelse);
                    push_all(out, t->finalbody);
                    break;
                }
                case StmtKind::Assert: push(out, cast<pyfront::Assert>(s)->test); push(out, cast<pyfront::Assert>(s)->msg); break;
                case StmtKind::Import:
                    for (auto* a : cast<Import>(s)->names) out.emplace_back(a, AnyNode::Alias);
                    break;
                case StmtKind::ImportFrom:
                    for (auto* a : cast<ImportFrom>(s)->names) out.emplace_back(a, AnyNode::Alias);
                    break;
                case StmtKind::Expr: push(out, cast<ExprStmt>(s)->value); break;
                default: break;
            }
            break;
        }
        case AnyNode::Arguments: {
            auto* a = static_cast<const arguments*>(n.p);
            for (auto* x : a->posonlyargs) out.emplace_back(x, AnyNode::Arg);
            for (auto* x : a->args) out.emplace_back(x, AnyNode::Arg);
            if (a->vararg) out.emplace_back(a->vararg, AnyNode::Arg);
            for (auto* x : a->kwonlyargs) out.emplace_back(x, AnyNode::Arg);
            push_all(out, a->kw_defaults);
            if (a->kwarg) out.emplace_back(a->kwarg, AnyNode::Arg);
            push_all(out, a->defaults);
            break;
        }
        case AnyNode::Arg: push(out, static_cast<const arg*>(n.p)->annotation); break;
        case AnyNode::Keyword: push(out, static_cast<const keyword*>(n.p)->value); break;
        case AnyNode::WithItem: {
            auto* w = static_cast<const withitem*>(n.p);
            push(out, w->context_expr);
            push(out, w->optional_vars);
            break;
        }
        case AnyNode::Comprehension: {
            auto* c = static_cast<const comprehension*>(n.p);
            push(out, c->target);
            push(out, c->iter);
            push_all(out, c->ifs);
            break;
        }
        case AnyNode::Handler: {
            auto* h = static_cast<const ExceptHandler*>(n.p);
            push(out, h->type);
            push_all(out, h->body);
            break;
        }
        case AnyNode::MatchCase: {
            auto* m = static_cast<const match_case*>(n.p);
            if (m->pattern) out.emplace_back(m->pattern, AnyNode::Pattern);
            push(out, m->guard);
            push_all(out, m->body);
            break;
        }
        case AnyNode::Pattern: {
            auto* p = static_cast<const pyfront::Pattern*>(n.p);
            switch (p->kind) {
                case PatternKind::MatchValue: push(out, cast<MatchValue>(p)->value); break;
                case PatternKind::MatchSequence: push_pats(out, cast<MatchSequence>(p)->patterns); break;
                case PatternKind::MatchMapping: push_all(out, cast<MatchMapping>(p)->keys); push_pats(out, cast<MatchMapping>(p)->patterns); break;
                case PatternKind::MatchClass: push(out, cast<MatchClass>(p)->cls); push_pats(out, cast<MatchClass>(p)->patterns); push_pats(out, cast<MatchClass>(p)->kwd_patterns); break;
                case PatternKind::MatchAs:
                    if (cast<MatchAs>(p)->pattern) out.emplace_back(cast<MatchAs>(p)->pattern, AnyNode::Pattern);
                    break;
                case PatternKind::MatchOr: push_pats(out, cast<MatchOr>(p)->patterns); break;
                default: break;
            }
            break;
        }
        case AnyNode::TypeParam: {
            auto* t = static_cast<const pyfront::TypeParam*>(n.p);
            push(out, t->bound);
            push(out, t->default_value);
            break;
        }
        default: break;
    }
}

std::vector<AnyNode> walk(AnyNode n) {
    std::vector<AnyNode> out;
    if (!n) return out;
    out.push_back(n);
    for (size_t i = 0; i < out.size(); i++) {
        AnyNode cur = out[i];
        child_nodes(cur, out);
    }
    return out;
}

void ParentMap::build(AnyNode root) {
    std::vector<AnyNode> kids;
    for (const AnyNode& n : walk(root)) {
        kids.clear();
        child_nodes(n, kids);
        for (auto& k : kids) up_[k.p] = n;
    }
}
AnyNode ParentMap::parent(const A::Node* n) const {
    auto it = up_.find(n);
    return it == up_.end() ? AnyNode() : it->second;
}

std::vector<const std::vector<A::Stmt*>*> stmt_lists(const A::Stmt* s, bool with_handlers) {
    std::vector<const std::vector<A::Stmt*>*> out;
    switch (s->kind) {
        case StmtKind::FunctionDef:
        case StmtKind::AsyncFunctionDef: out.push_back(&cast<FunctionDef>(s)->body); break;
        case StmtKind::ClassDef: out.push_back(&cast<ClassDef>(s)->body); break;
        case StmtKind::For:
        case StmtKind::AsyncFor: out.push_back(&cast<pyfront::For>(s)->body); out.push_back(&cast<pyfront::For>(s)->orelse); break;
        case StmtKind::While: out.push_back(&cast<pyfront::While>(s)->body); out.push_back(&cast<pyfront::While>(s)->orelse); break;
        case StmtKind::If: out.push_back(&cast<pyfront::If>(s)->body); out.push_back(&cast<pyfront::If>(s)->orelse); break;
        case StmtKind::With:
        case StmtKind::AsyncWith: out.push_back(&cast<With>(s)->body); break;
        case StmtKind::Try:
        case StmtKind::TryStar: {
            auto* t = cast<pyfront::Try>(s);
            out.push_back(&t->body);
            if (with_handlers)
                for (auto* h : t->handlers) out.push_back(&h->body);
            out.push_back(&t->orelse);
            out.push_back(&t->finalbody);
            break;
        }
        case StmtKind::Match:
            for (auto* c : cast<Match>(s)->cases) out.push_back(&c->body);
            break;
        default: break;
    }
    return out;
}

static const A::Pos kNoPos{};
const A::Pos& pos_of(AnyNode n) { return n.p ? n.p->pos : kNoPos; }
int lineno(AnyNode n) { return n.p ? n.p->pos.lineno : 0; }

std::string type_name(AnyNode n) {
    switch (n.cat) {
        case AnyNode::Expr: return name_of(n.expr()->kind);
        case AnyNode::Stmt: {
            auto k = n.stmt()->kind;
            if (k == StmtKind::Expr) return "Expr";
            return name_of(k);
        }
        case AnyNode::Arguments: return "arguments";
        case AnyNode::Arg: return "arg";
        case AnyNode::Keyword: return "keyword";
        case AnyNode::Alias: return "alias";
        case AnyNode::WithItem: return "withitem";
        case AnyNode::Comprehension: return "comprehension";
        case AnyNode::Handler: return "ExceptHandler";
        case AnyNode::MatchCase: return "match_case";
        case AnyNode::Pattern: return name_of(static_cast<const pyfront::Pattern*>(n.p)->kind);
        case AnyNode::TypeParam: return "TypeVar";
        default: return "?";
    }
}

// ------------------------------------------------------------------------------------ docstrings
static std::string expandtabs(const std::string& s) {
    std::string out;
    size_t col = 0;
    for (size_t i = 0; i < s.size(); i++) {
        unsigned char c = (unsigned char)s[i];
        if (c == '\t') {
            size_t n = 8 - col % 8;
            out.append(n, ' ');
            col += n;
        } else if (c == '\n' || c == '\r') {
            out.push_back((char)c);
            col = 0;
        } else {
            out.push_back((char)c);
            if ((c & 0xC0) != 0x80) col++;   // one column per code point
        }
    }
    return out;
}

std::string cleandoc(const std::string& doc) {
    std::string d = expandtabs(doc);
    std::vector<std::string> lines;
    size_t start = 0;
    for (;;) {
        size_t nl = d.find('\n', start);
        if (nl == std::string::npos) {
            lines.push_back(d.substr(start));
            break;
        }
        lines.push_back(d.substr(start, nl - start));
        start = nl + 1;
    }
    size_t margin = SIZE_MAX;
    for (size_t i = 1; i < lines.size(); i++) {
        const std::string& l = lines[i];
        size_t lead = l.find_first_not_of(' ');
        if (lead == std::string::npos) continue;
        margin = std::min(margin, lead);
    }
    if (!lines.empty()) {
        size_t lead = lines[0].find_first_not_of(' ');
        lines[0] = lead == std::string::npos ? "" : lines[0].substr(lead);
    }
    if (margin != SIZE_MAX)
        for (size_t i = 1; i < lines.size(); i++) lines[i] = lines[i].size() > margin ? lines[i].substr(margin) : "";
    while (!lines.empty() && lines.back().empty()) lines.pop_back();
    size_t first = 0;
    while (first < lines.size() && lines[first].empty()) first++;
    std::string out;
    for (size_t i = first; i < lines.size(); i++) {
        if (i > first) out += "\n";
        out += lines[i];
    }
    return out;
}

bool docstring(const std::vector<A::Stmt*>& body, std::string& out) {
    if (body.empty()) return false;
    auto* e = as<ExprStmt>(body[0]);
    if (!e) return false;
    std::string s;
    if (!is_str_const(e->value, &s)) return false;
    out = cleandoc(s);
    return true;
}

std::vector<size_t> line_starts(const std::string& src) {
    std::vector<size_t> out = {0};
    for (size_t i = 0; i < src.size(); i++) {
        if (src[i] == '\r') {
            if (i + 1 < src.size() && src[i + 1] == '\n') i++;
            out.push_back(i + 1);
        } else if (src[i] == '\n') {
            out.push_back(i + 1);
        }
    }
    return out;
}

bool source_segment(const std::string& src, const std::vector<size_t>& ls, const A::Pos& p, std::string& out) {
    if (p.lineno <= 0 || p.end_lineno <= 0) return false;
    size_t l0 = p.lineno - 1, l1 = p.end_lineno - 1;
    if (l1 >= ls.size()) return false;
    size_t a = ls[l0] + p.col_offset;
    size_t b = ls[l1] + p.end_col_offset;
    if (a > src.size() || b > src.size() || b < a) return false;
    out = src.substr(a, b - a);
    return true;
}

// ------------------------------------------------------------------------------------ unparse
static std::string py_repr_str(const std::string& s) {
    bool sq = s.find('\'') != std::string::npos, dq = s.find('"') != std::string::npos;
    char q = (sq && !dq) ? '"' : '\'';
    std::string out(1, q);
    for (unsigned char c : s) {
        if (c == (unsigned char)q || c == '\\') {
            out += '\\';
            out += (char)c;
        } else if (c == '\n') out += "\\n";
        else if (c == '\r') out += "\\r";
        else if (c == '\t') out += "\\t";
        else if (c < 0x20 || c == 0x7f) {
            char buf[8];
            snprintf(buf, sizeof buf, "\\x%02x", c);
            out += buf;
        } else out += (char)c;
    }
    return out + q;
}

std::string unparse(const A::Expr* e) {
    if (!e) return "";
    switch (e->kind) {
        case ExprKind::Name: return cast<pyfront::Name>(e)->id;
        case ExprKind::Attribute: return unparse(cast<Attribute>(e)->value) + "." + cast<Attribute>(e)->attr;
        case ExprKind::Call: {
            auto* c = cast<pyfront::Call>(e);
            std::string s = unparse(c->func) + "(";
            bool first = true;
            for (auto* a : c->args) {
                s += (first ? "" : ", ") + unparse(a);
                first = false;
            }
            for (auto* k : c->keywords) {
                s += (first ? "" : ", ") + (k->has_arg ? k->arg + "=" : std::string("**")) + unparse(k->value);
                first = false;
            }
            return s + ")";
        }
        case ExprKind::Subscript: return unparse(cast<Subscript>(e)->value) + "[" + unparse(cast<Subscript>(e)->slice) + "]";
        case ExprKind::Tuple: {
            auto* t = cast<pyfront::Tuple>(e);
            std::string s = "(";
            for (size_t i = 0; i < t->elts.size(); i++) s += (i ? ", " : "") + unparse(t->elts[i]);
            if (t->elts.size() == 1) s += ",";
            return s + ")";
        }
        case ExprKind::Constant: {
            auto& v = cast<Constant>(e)->value;
            switch (v.kind) {
                case ConstValue::Kind::None: return "None";
                case ConstValue::Kind::True: return "True";
                case ConstValue::Kind::False: return "False";
                case ConstValue::Kind::Int: return v.text;
                case ConstValue::Kind::Str: return py_repr_str(v.text);
                default: return "...";
            }
        }
        default: return "<" + std::string(name_of(e->kind)) + ">";
    }
}

bool same_tree(const A::Expr* a, const A::Expr* b) {
    if (!a || !b) return a == b;
    return pyfront::dump(a, false) == pyfront::dump(b, false);
}

bool is_str_const(const A::Expr* e, std::string* out) {
    auto* c = as<Constant>(e);
    if (!c || c->value.kind != ConstValue::Kind::Str) return false;
    if (out) *out = c->value.text;
    return true;
}
bool is_none_const(const A::Expr* e) {
    auto* c = as<Constant>(e);
    return c && c->value.kind == ConstValue::Kind::None;
}
bool is_const(const A::Expr* e) { return as<Constant>(e) != nullptr; }
bool int_const(const A::Expr* e, Big* out) {
    if (auto* c = as<Constant>(e)) {
        if (c->value.kind != ConstValue::Kind::Int) return false;
        if (out) *out = c->value.fits_i64 ? Big(c->value.i64) : Big::from_decimal(c->value.text);
        return true;
    }
    if (auto* u = as<UnaryOp>(e)) {
        if (u->op != UnaryOpKind::USub) return false;
        Big v;
        if (!int_const(u->operand, &v)) return false;
        if (out) *out = -v;
        return true;
    }
    return false;
}

}  // namespace pyengine
