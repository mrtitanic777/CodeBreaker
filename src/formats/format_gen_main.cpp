// format_gen -- the format descriptions (config/formats/*.fmt) as typed C++ readers, written at build time
// (docs/FORMAT_CODEGEN.md; run by src/formats/generate_formats.cmake):
//
//   format_gen OUT_DIR FMT_DIR        writes OUT_DIR/<name>.hpp and OUT_DIR/<name>.cpp for every description
//
// The descriptions are parsed by the engine's own parser (formats/format_desc.hpp). Each generated reader does what the
// engine's Reader does for that description (format_engine.cpp: read_field and the expression evaluator), with the
// names resolved at compile time. A file is only rewritten when its text changes.
#include <algorithm>
#include <cstdio>
#include <filesystem>
#include <format>
#include <fstream>
#include <functional>
#include <map>
#include <memory>
#include <optional>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include "formats/format_desc.hpp"

namespace fsys = std::filesystem;
using namespace cb::formats::engine;
using namespace cb::formats::engine::desc;

namespace {

[[noreturn]] void die(const std::string& what) {
    std::fprintf(stderr, "format_gen: %s\n", what.c_str());
    std::exit(1);
}

// A description the generator cannot write a reader for (yet): its files say why, the others are written.
struct Unsupported : std::runtime_error {
    using std::runtime_error::runtime_error;
};

// ---- types ----

struct Ty;
using TyP = std::shared_ptr<const Ty>;
struct Ty {
    enum class K { Unknown, Num, Str, Bytes, Struct, Array } k = K::Unknown;
    std::string ctype;   // Num: "std::uint32_t"; Struct: the struct's name
    TyP elem;            // Array
};

TyP make_ty(Ty::K k, std::string c = {}, TyP elem = {}) {
    auto t = std::make_shared<Ty>();
    t->k = k, t->ctype = std::move(c), t->elem = std::move(elem);
    return t;
}
const TyP kUnknown = make_ty(Ty::K::Unknown);
const TyP kInt64 = make_ty(Ty::K::Num, "std::int64_t");

bool same(const TyP& a, const TyP& b) {
    if (a->k != b->k) return false;
    if (a->k == Ty::K::Num || a->k == Ty::K::Struct) return a->ctype == b->ctype;
    if (a->k == Ty::K::Array) return same(a->elem, b->elem);
    return true;
}

std::string cpp(const TyP& t) {
    switch (t->k) {
        case Ty::K::Num: return t->ctype;
        case Ty::K::Str: return "std::string_view";
        case Ty::K::Bytes: return "cb::ByteView";
        case Ty::K::Struct: return t->ctype;
        case Ty::K::Array: return "std::vector<" + cpp(t->elem) + ">";
        default: return "std::int64_t";
    }
}

// How a value of this type is handed back by an accessor (a number as int64, the rest by reference or view).
std::string ret_type(const TyP& t) {
    switch (t->k) {
        case Ty::K::Num:
        case Ty::K::Unknown: return "std::int64_t";
        case Ty::K::Str:
        case Ty::K::Bytes: return cpp(t);
        default: return "const " + cpp(t) + "&";
    }
}

std::string suffix(const TyP& t) {
    switch (t->k) {
        case Ty::K::Num: return "num";
        case Ty::K::Str: return "str";
        case Ty::K::Bytes: return "bytes";
        case Ty::K::Struct: return t->ctype;
        case Ty::K::Array: return suffix(t->elem);
        default: return "x";
    }
}

// Two numbers' common member type: the wider of two unsigned or two signed ones, int64 when one is signed and one is not
// (a let is int64).
std::string unify_num(const std::string& a, const std::string& b) {
    if (a == b) return a;
    const bool ua = a.starts_with("std::uint"), ub = b.starts_with("std::uint");
    if (ua != ub) return "std::int64_t";
    const auto bits = [](const std::string& t) { return std::stoi(t.substr(t.find("int") + 3)); };
    return bits(a) >= bits(b) ? a : b;
}

std::string int_ctype(const TypeRef& t) {
    const unsigned w = t.width == 3 ? 4 : t.width;
    return std::format("std::{}int{}_t", t.sgn ? "" : "u", w * 8);
}

const std::set<std::string> kKeywords = {
    "alignas", "alignof", "and", "asm", "auto", "bool", "break", "case", "catch", "char", "class", "const", "constexpr", "continue",
    "decltype", "default", "delete", "do", "double", "else", "enum", "explicit", "export", "extern", "false", "float", "for", "friend",
    "goto", "if", "inline", "int", "long", "mutable", "namespace", "new", "noexcept", "not", "nullptr", "operator", "or", "private",
    "protected", "public", "register", "return", "short", "signed", "sizeof", "static", "struct", "switch", "template", "this",
    "throw", "true", "try", "typedef", "typeid", "typename", "union", "unsigned", "using", "virtual", "void", "volatile", "while",
    "xor", "self", "up", "cx", "pos", "at", "end", "index", "std", "cb", "rt", "detail", "options", "data", "read"};

std::string cname(const std::string& n) { return kKeywords.contains(n) ? n + "_" : n; }

std::string cstring(std::string_view s) {
    std::string out = "\"";
    for (unsigned char c : s) {
        if (c == '"' || c == '\\') out += '\\', out += static_cast<char>(c);
        else if (c >= 0x20 && c < 0x7F) out += static_cast<char>(c);
        else out += std::format("\\x{:02x}\"\"", c);
    }
    return out + "\"";
}

std::string sv_literal(std::string_view s) { return "std::string_view(" + cstring(s) + ", " + std::to_string(s.size()) + ")"; }

// ---- one format ----

enum class Pres { Absent, Maybe, Definite };

struct Member {
    std::string name, cname;
    TyP ty;
    bool optional = false;
    bool try_num = false;   // a number read `try` on every path: std::optional, and 0 to the description when it is not there
};

struct StructInfo {
    const StructDef* def = nullptr;
    std::vector<Member> members;                        // in the order of the description
    std::map<std::string, std::vector<std::size_t>> by_name;   // a name's members (more than one: one per type)
    std::map<std::string, Pres> end_state;
    std::set<std::string> needs;                        // names resolved by the enclosers
    std::map<std::string, TyP> up_ty;                   // ... and what they are
    bool uses_parent = false;                           // `_parent`: the struct that reads it
    TyP parent_ty = kUnknown;
};

using State = std::map<std::string, Pres>;   // by member cname

struct Site {
    std::string encloser, child;
    State state;
    std::size_t id = 0;
};

class Gen {
public:
    Gen(const Format& f, const std::map<std::string, std::shared_ptr<Format>>& all) : f_(f) {
        std::set<std::string> seen;
        std::function<void(const std::string&)> add = [&](const std::string& n) {
            if (!seen.insert(n).second) return;
            auto it = all.find(n);
            if (it == all.end()) die(f.name + " uses '" + n + "', which is not a description");
            chain_.push_back(it->second.get());
            for (const std::string& u : it->second->uses) add(u);
        };
        add(f.name);
    }

    void run(std::string& header, std::string& source) {
        // every struct of the chain (the first of a name wins, as find_struct)
        for (const Format* g : chain_)
            for (const auto& [n, d] : g->structs)
                if (!structs_.contains(n)) structs_[n].def = &d;
        for (auto& [n, s] : structs_) collect(s);
        for (auto& [n, s] : structs_) analyse(n, s);
        resolve();
        write_header(header);
        write_source(source);
    }

private:
    const Format& f_;
    std::vector<const Format*> chain_;
    std::map<std::string, StructInfo> structs_;
    std::vector<Site> sites_;
    std::set<std::string> vars_;
    std::set<std::string> parts_;
    std::string out_;   // the read functions being written

    const FnDef* find_fn(const std::string& n) const {
        for (const Format* g : chain_)
            if (auto it = g->fns.find(n); it != g->fns.end()) return &it->second;
        return nullptr;
    }
    const EnumDef* find_enum(const std::string& n) const {
        for (const Format* g : chain_)
            if (auto it = g->enums.find(n); it != g->enums.end()) return &it->second;
        return nullptr;
    }
    [[noreturn]] void fail(const std::string& where, const std::string& what) const { throw Unsupported(f_.name + ": " + where + ": " + what); }

    // ---- the members ----

    TyP field_ty(const FieldDef& fd, const std::string& where) const {
        TyP t;
        switch (fd.type.k) {
            case TypeRef::K::Int: t = make_ty(Ty::K::Num, int_ctype(fd.type)); break;
            case TypeRef::K::Str:
            case TypeRef::K::Cstr: t = make_ty(Ty::K::Str); break;
            case TypeRef::K::Bytes: t = make_ty(Ty::K::Bytes); break;
            case TypeRef::K::Struct:
                if (!structs_.contains(fd.type.name)) fail(where, "unknown struct '" + fd.type.name + "'");
                t = make_ty(Ty::K::Struct, fd.type.name);
                break;
        }
        if (fd.arr != Arr::None) t = make_ty(Ty::K::Array, {}, t);
        return t;
    }

    void collect(StructInfo& s) {
        struct Decl {
            std::string name;
            TyP ty;
        };
        std::vector<Decl> decls;
        std::function<void(const std::vector<Stmt>&)> walk = [&](const std::vector<Stmt>& body) {
            for (const Stmt& st : body) {
                if (st.k == Stmt::K::Field) {
                    decls.push_back({st.field.name, field_ty(st.field, s.def->name + "." + st.field.name)});
                    if (!st.field.as.empty() && !find_enum(st.field.as)) fail(s.def->name, "unknown enum or flags '" + st.field.as + "'");
                    if (!st.field.part.empty()) parts_.insert(st.field.part);
                } else if (st.k == Stmt::K::Let) {
                    decls.push_back({st.let_name, kInt64});
                } else if (st.k == Stmt::K::If) {
                    walk(st.body);
                    walk(st.else_body);
                }
            }
        };
        walk(s.def->body);
        // a name's types: the numbers unified, a different kind a member of its own
        std::map<std::string, std::vector<TyP>> kinds;
        std::vector<std::string> order;
        for (const Decl& d : decls) {
            auto& v = kinds[d.name];
            if (v.empty()) order.push_back(d.name);
            bool merged = false;
            for (TyP& t : v) {
                if (t->k == Ty::K::Num && d.ty->k == Ty::K::Num) {
                    t = make_ty(Ty::K::Num, unify_num(t->ctype, d.ty->ctype));
                    merged = true;
                } else if (same(t, d.ty)) {
                    merged = true;
                }
                if (merged) break;
            }
            if (!merged) v.push_back(d.ty);
        }
        for (const std::string& n : order) {
            const auto& v = kinds[n];
            for (const TyP& t : v) {
                Member m;
                m.name = n;
                m.cname = v.size() == 1 ? cname(n) : n + "_" + suffix(t);
                m.ty = t;
                s.by_name[n].push_back(s.members.size());
                s.members.push_back(std::move(m));
            }
        }
    }

    // The member a field or let of this type writes.
    const Member& member_of(const StructInfo& s, const std::string& name, const TyP& ty) const {
        for (std::size_t i : s.by_name.at(name)) {
            const Member& m = s.members[i];
            if (same(m.ty, ty) || (m.ty->k == Ty::K::Num && ty->k == Ty::K::Num)) return m;
        }
        fail(s.def->name, "no member for '" + name + "'");
    }

    // The presence of a name at a point of a struct: Absent, Maybe or Definite (a name of more than one type: only absent
    // is understood).
    Pres presence(const StructInfo& s, const State& st, const std::string& name, const Member** m = nullptr) const {
        auto it = s.by_name.find(name);
        if (it == s.by_name.end()) return Pres::Absent;
        Pres best = Pres::Absent;
        int present = 0;
        for (std::size_t i : it->second) {
            auto p = st.find(s.members[i].cname);
            if (p != st.end() && p->second != Pres::Absent) {
                ++present;
                best = p->second;
                if (m) *m = &s.members[i];
            }
        }
        if (present > 1) fail(s.def->name, "the name '" + name + "' is looked up where it may have more than one type");
        return best;
    }

    // What a field statement leaves: the member is there for certain or maybe.
    static Pres field_pres(const FieldDef& fd) {
        if (fd.when || !fd.part.empty()) return Pres::Maybe;
        if (fd.tolerant && !(fd.arr == Arr::None && fd.type.k == TypeRef::K::Int)) return Pres::Maybe;
        return Pres::Definite;
    }

    static State merge(const State& a, const State& b) {
        State out;
        for (const auto& [n, p] : a) {
            auto q = b.find(n);
            const Pres pb = q == b.end() ? Pres::Absent : q->second;
            out[n] = p == Pres::Definite && pb == Pres::Definite ? Pres::Definite : (p == Pres::Absent && pb == Pres::Absent ? Pres::Absent : Pres::Maybe);
        }
        for (const auto& [n, p] : b)
            if (!a.contains(n)) out[n] = p == Pres::Absent ? Pres::Absent : Pres::Maybe;
        return out;
    }

    void set_pres(const StructInfo& s, State& st, const std::string& cn, Pres p) const {
        auto it = st.find(cn);
        if (it != st.end() && it->second != Pres::Absent) fail(s.def->name, "the name '" + cn + "' is read twice on one path (not handled)");
        st[cn] = p;
    }

    // ---- the analysis: the names each struct needs from its enclosers, its sites ----

    // Names an expression looks up (not a function's parameters).
    void names_in(const Expr& e, const std::set<std::string>& params, std::set<std::string>& out, int depth = 0) const {
        if (depth > 32) return;
        if (e.op == Expr::Op::Name) {
            if (!params.contains(e.name) && e.name != "_" && e.name != "_index" && e.name != "_parent") out.insert(e.name);
            return;
        }
        if (e.op == Expr::Op::Call) {
            if (const FnDef* fn = find_fn(e.name)) {
                std::set<std::string> p(fn->params.begin(), fn->params.end());
                names_in(*fn->body, p, out, depth + 1);
            }
        }
        for (const ExprP& k : e.kids) names_in(*k, params, out, depth);
    }

    void field_exprs(const FieldDef& fd, std::vector<const Expr*>& out) const {
        for (const ExprP& e : {fd.at, fd.arr_expr, fd.arr_until, fd.window, fd.when, fd.type.len})
            if (e) out.push_back(e.get());
    }

    bool uses(const Expr& e, const std::string& name, int depth = 0) const {
        if (depth > 32) return false;
        if (e.op == Expr::Op::Name && e.name == name) return true;
        if (e.op == Expr::Op::Call)
            if (const FnDef* fn = find_fn(e.name); fn && uses(*fn->body, name, depth + 1)) return true;
        for (const ExprP& k : e.kids)
            if (uses(*k, name, depth)) return true;
        return false;
    }

    void vars_in(const Expr& e) {
        if (e.op == Expr::Op::Special && e.code == Expr::Var) vars_.insert(e.name);
        for (const ExprP& k : e.kids) vars_in(*k);
        if (e.op == Expr::Op::Call)
            if (const FnDef* fn = find_fn(e.name)) vars_in(*fn->body);
    }

    void analyse(const std::string& name, StructInfo& s) {
        State st;
        auto use = [&](const Expr& e, const State& at) {
            vars_in(e);
            if (uses(e, "_parent")) s.uses_parent = true;
            std::set<std::string> names;
            names_in(e, {}, names);
            for (const std::string& n : names)
                if (presence(s, at, n) != Pres::Definite) s.needs.insert(n);
        };
        std::function<void(const std::vector<Stmt>&, State&)> walk = [&](const std::vector<Stmt>& body, State& st) {
            for (const Stmt& stmt : body) {
                switch (stmt.k) {
                    case Stmt::K::Let: {
                        use(*stmt.expr, st);
                        const std::string& cn = member_of(s, stmt.let_name, kInt64).cname;
                        plain_try_[name + "." + cn] = false;
                        set_pres(s, st, cn, Pres::Definite);
                        break;
                    }
                    case Stmt::K::Assert: use(*stmt.expr, st); break;
                    case Stmt::K::If: {
                        use(*stmt.expr, st);
                        State a = st, b = st;
                        walk(stmt.body, a);
                        walk(stmt.else_body, b);
                        st = merge(a, b);
                        break;
                    }
                    case Stmt::K::Field: {
                        const FieldDef& fd = stmt.field;
                        std::vector<const Expr*> es;
                        field_exprs(fd, es);
                        for (const Expr* e : es) use(*e, st);
                        if (fd.type.k == TypeRef::K::Struct) sites_.push_back({name, fd.type.name, st, sites_.size()});
                        const std::string& cn = member_of(s, fd.name, field_ty(fd, name)).cname;
                        const bool plain_try = fd.tolerant && fd.arr == Arr::None && fd.type.k == TypeRef::K::Int && !fd.when && fd.part.empty();
                        if (!plain_try_.contains(name + "." + cn)) plain_try_[name + "." + cn] = plain_try;
                        else plain_try_[name + "." + cn] = plain_try_[name + "." + cn] && plain_try;
                        set_pres(s, st, cn, field_pres(fd));
                        break;
                    }
                }
            }
        };
        walk(s.def->body, st);
        s.end_state = st;
        for (Member& m : s.members) {
            auto it = st.find(m.cname);
            m.optional = it == st.end() || it->second != Pres::Definite;
            if (!m.optional && plain_try_[name + "." + m.cname]) m.try_num = m.optional = true;
        }
    }

    std::map<std::string, bool> plain_try_;   // struct.member: every field that writes it is a number read `try`

    // The value a member holds (a try number: 0 when it was not read; another optional: what it holds).
    static std::string mval(const std::string& prefix, const Member& m) {
        if (m.try_num) return prefix + "." + m.cname + ".value_or(0)";
        if (m.optional) return "(*" + prefix + "." + m.cname + ")";
        return prefix + "." + m.cname;
    }

    TyP merge_ty(const TyP& a, const TyP& b, const std::string& where) const {
        if (a->k == Ty::K::Unknown) return b;
        if (b->k == Ty::K::Unknown) return a;
        if (a->k == Ty::K::Num && b->k == Ty::K::Num) return kInt64;
        if (!same(a, b)) fail(where, "a name resolves to values of different types");
        return a;
    }

    TyP local_ty(const StructInfo& s, const State& st, const std::string& n, Pres& p) const {
        const Member* m = nullptr;
        p = presence(s, st, n, &m);
        return p == Pres::Absent ? kUnknown : m->ty;
    }

    // The needs of a struct climb to the struct that reads it, until a struct holds the name; then their types.
    void resolve() {
        for (bool changed = true; changed;) {
            changed = false;
            for (const Site& site : sites_) {
                StructInfo& up = structs_.at(site.encloser);
                for (const std::string& n : structs_.at(site.child).needs)
                    if (presence(up, site.state, n) != Pres::Definite && up.needs.insert(n).second) changed = true;
            }
        }
        for (auto& [n, s] : structs_)
            for (const std::string& name : s.needs) s.up_ty[name] = kUnknown;
        for (const Site& site : sites_) {   // `_parent`: the encloser, the same struct at every site
            StructInfo& child = structs_.at(site.child);
            if (child.uses_parent) child.parent_ty = merge_ty(child.parent_ty, make_ty(Ty::K::Struct, site.encloser), site.child + "._parent");
        }
        for (bool changed = true; changed;) {
            changed = false;
            for (const Site& site : sites_) {
                StructInfo& up = structs_.at(site.encloser);
                StructInfo& child = structs_.at(site.child);
                for (const std::string& n : child.needs) {
                    Pres p;
                    TyP t = local_ty(up, site.state, n, p);
                    if (p != Pres::Definite) t = merge_ty(t, up.up_ty.at(n), site.encloser + "." + n);
                    TyP& have = child.up_ty.at(n);
                    TyP now = merge_ty(have, t, site.child + "." + n);
                    if (!same(now, have)) have = now, changed = true;
                }
            }
        }
        // A name nothing above an encloser holds throws there; it is given the type the struct below wants, so that the
        // accessors on the way agree.
        for (bool changed = true; changed;) {
            changed = false;
            for (const Site& site : sites_) {
                StructInfo& up = structs_.at(site.encloser);
                const StructInfo& child = structs_.at(site.child);
                for (const std::string& n : child.needs) {
                    if (presence(up, site.state, n) == Pres::Definite) continue;
                    TyP& above = up.up_ty.at(n);
                    const TyP& want = child.up_ty.at(n);
                    if (above->k == Ty::K::Unknown && want->k != Ty::K::Unknown) above = want, changed = true;
                }
            }
        }
    }

    // ---- expressions ----

    struct Env {
        const StructInfo* s = nullptr;
        const State* st = nullptr;
        std::string pos = "pos";                  // the cursor `$pos` means
        std::string under;                         // `_` (an until's element), "" when none
        TyP under_ty;
        std::map<std::string, std::string> params; // a function's parameters, by name
    };

    struct Ex {
        std::string code;
        TyP ty;
    };

    static std::string val_of(const std::string& code, const TyP& t) {
        return t->k == Ty::K::Num ? "static_cast<std::int64_t>(" + code + ")" : code;
    }

    std::string throw_num(const Env& env, const std::string& what) const { return "cx.fail_value(" + env.pos + ", " + cstring(what) + ")"; }

    std::string num(const Expr& e, const Env& env) {
        Ex x = expr(e, env);
        switch (x.ty->k) {
            case Ty::K::Num:
            case Ty::K::Unknown: return x.code;
            default: return "(static_cast<void>(" + x.code + "), " + throw_num(env, "a value is not a number") + ")";
        }
    }

    Ex lookup(const std::string& n, const Env& env) {
        if (auto p = env.params.find(n); p != env.params.end()) return {p->second, kInt64};
        if (n == "_") {
            if (env.under.empty()) return {throw_num(env, "`_` is only meant in an `until`"), kUnknown};
            return {val_of(env.under, env.under_ty), env.under_ty->k == Ty::K::Num ? kInt64 : env.under_ty};
        }
        if (n == "_index") return {"static_cast<std::int64_t>(index)", kInt64};
        if (n == "_parent") return {"up.parent_(cx, " + env.pos + ")", env.s->parent_ty};
        if (n == "_root") fail(env.s->def->name, n + " is not handled by the generator");
        const Member* m = nullptr;
        const Pres p = presence(*env.s, *env.st, n, &m);
        auto here = [&] { return val_of(mval("self", *m), m->ty); };
        auto upcall = "up." + cname(n) + "(cx, " + env.pos + ")";
        if (p == Pres::Definite) return {here(), m->ty->k == Ty::K::Num ? kInt64 : m->ty};
        const TyP upt = env.s->up_ty.at(n);
        if (p == Pres::Maybe) {
            const TyP t = merge_ty(m->ty, upt, env.s->def->name + "." + n);
            if (upt->k == Ty::K::Unknown && t->k != Ty::K::Num)   // nothing above holds it: the throw of the encloser stands for a reference
                return {"(self." + m->cname + " ? " + "*self." + m->cname + " : (" + upcall + ", *self." + m->cname + "))", t};
            return {"(self." + m->cname + " ? " + val_of("*self." + m->cname, m->ty) + " : " + upcall + ")", t->k == Ty::K::Num ? kInt64 : t};
        }
        return {upcall, upt->k == Ty::K::Num ? kInt64 : upt};
    }

    // The member of a struct value (a field that is not there is the engine's error).
    Ex member(const Ex& base, const std::string& field, const Env& env) {
        if (base.ty->k == Ty::K::Unknown) return base;
        if (base.ty->k != Ty::K::Struct) return {throw_num(env, "'." + field + "' of something that is not a structure"), kUnknown};
        const StructInfo& t = structs_.at(base.ty->ctype);
        auto it = t.by_name.find(field);
        if (it == t.by_name.end()) return {"(static_cast<void>(" + base.code + "), " + throw_num(env, "no field '" + field + "'") + ")", kUnknown};
        if (it->second.size() != 1) fail(env.s->def->name, "'." + field + "' has more than one type in " + t.def->name);
        const Member& m = t.members[it->second[0]];
        std::string code = m.optional && !m.try_num
                               ? "rt::need(cx, " + env.pos + ", " + base.code + "." + m.cname + ", " + cstring(field) + ", " + cstring(t.def->name) + ")"
                               : mval(base.code, m);
        if (m.ty->k == Ty::K::Num) return {"static_cast<std::int64_t>(" + code + ")", kInt64};
        return {code, m.ty};
    }

    Ex expr(const Expr& e, const Env& env) {
        auto n = [&](std::size_t k) { return num(*e.kids[k], env); };
        switch (e.op) {
            case Expr::Op::Num: return {std::format("std::int64_t({})", e.num < 0 ? std::format("static_cast<std::int64_t>({}ull)", static_cast<std::uint64_t>(e.num)) : std::to_string(e.num) + "ll"), kInt64};
            case Expr::Op::Name: return lookup(e.name, env);
            case Expr::Op::Special: {
                const std::string& p = env.pos;
                switch (e.code) {
                    case Expr::Pos: return {"static_cast<std::int64_t>(" + p + ")", kInt64};
                    case Expr::Start: return {"static_cast<std::int64_t>(at)", kInt64};
                    case Expr::End: return {"static_cast<std::int64_t>(end)", kInt64};
                    case Expr::Remaining: return {"static_cast<std::int64_t>(end > " + p + " ? end - " + p + " : 0)", kInt64};
                    case Expr::Size: return {"static_cast<std::int64_t>(cx.data.size())", kInt64};
                    case Expr::Index: return {"static_cast<std::int64_t>(index)", kInt64};
                    default: return {"cx.opt.vars." + cname(e.name), kInt64};
                }
            }
            case Expr::Op::Text: return {throw_num(env, "a string is only meant as an argument of scan()"), kUnknown};
            case Expr::Op::Unary:
                if (e.code == Expr::Neg) return {"rt::neg(" + n(0) + ")", kInt64};
                if (e.code == Expr::Not) return {"static_cast<std::int64_t>(" + n(0) + " == 0)", kInt64};
                return {"(~" + n(0) + ")", kInt64};
            case Expr::Op::Cond: {
                const std::string c = n(0);
                Ex a = expr(*e.kids[1], env), b = expr(*e.kids[2], env);
                const bool an = a.ty->k == Ty::K::Num || a.ty->k == Ty::K::Unknown, bn = b.ty->k == Ty::K::Num || b.ty->k == Ty::K::Unknown;
                if (an && bn) return {"(" + c + " != 0 ? " + a.code + " : " + b.code + ")", kInt64};
                if (!same(a.ty, b.ty)) fail(env.s->def->name, "a ternary of two different kinds of value");
                return {"(" + c + " != 0 ? " + a.code + " : " + b.code + ")", a.ty};
            }
            case Expr::Op::Binary: {
                const std::string a = n(0), b = n(1);
                switch (e.code) {
                    case Expr::And: return {"static_cast<std::int64_t>(" + a + " != 0 && " + b + " != 0)", kInt64};
                    case Expr::Or: return {"static_cast<std::int64_t>(" + a + " != 0 || " + b + " != 0)", kInt64};
                    case Expr::Add: return {"rt::add(" + a + ", " + b + ")", kInt64};
                    case Expr::Sub: return {"rt::sub(" + a + ", " + b + ")", kInt64};
                    case Expr::Mul: return {"rt::mul(" + a + ", " + b + ")", kInt64};
                    case Expr::Div: return {"rt::div(cx, " + env.pos + ", " + a + ", " + b + ")", kInt64};
                    case Expr::Mod: return {"rt::mod(cx, " + env.pos + ", " + a + ", " + b + ")", kInt64};
                    case Expr::Shl: return {"rt::shl(" + a + ", " + b + ")", kInt64};
                    case Expr::Shr: return {"rt::shr(" + a + ", " + b + ")", kInt64};
                    case Expr::BitAnd: return {"(" + a + " & " + b + ")", kInt64};
                    case Expr::BitOr: return {"(" + a + " | " + b + ")", kInt64};
                    case Expr::BitXor: return {"(" + a + " ^ " + b + ")", kInt64};
                    case Expr::Eq: return {"static_cast<std::int64_t>(" + a + " == " + b + ")", kInt64};
                    case Expr::Ne: return {"static_cast<std::int64_t>(" + a + " != " + b + ")", kInt64};
                    case Expr::Lt: return {"static_cast<std::int64_t>(" + a + " < " + b + ")", kInt64};
                    case Expr::Le: return {"static_cast<std::int64_t>(" + a + " <= " + b + ")", kInt64};
                    case Expr::Gt: return {"static_cast<std::int64_t>(" + a + " > " + b + ")", kInt64};
                    default: return {"static_cast<std::int64_t>(" + a + " >= " + b + ")", kInt64};
                }
            }
            case Expr::Op::Member: return member(expr(*e.kids[0], env), e.name, env);
            case Expr::Op::Index: {
                Ex base = expr(*e.kids[0], env);
                const std::string i = n(1);
                if (base.ty->k == Ty::K::Unknown) return base;
                if (base.ty->k != Ty::K::Array) return {throw_num(env, "an index of something that is not an array"), kUnknown};
                const std::string code = "rt::element(cx, " + env.pos + ", " + base.code + ", " + i + ")";
                if (base.ty->elem->k == Ty::K::Num) return {"static_cast<std::int64_t>(" + code + ")", kInt64};
                return {code, base.ty->elem};
            }
            case Expr::Op::Call: return call(e, env);
        }
        return {"0", kInt64};
    }

    Ex call(const Expr& e, const Env& env) {
        const std::string& f = e.name;
        const std::string& p = env.pos;
        auto n = [&](std::size_t k) { return num(*e.kids[k], env); };
        auto args = [&](std::size_t want) {
            if (e.kids.size() != want) fail(env.s->def->name, f + "() takes " + std::to_string(want) + " arguments");
        };
        if (const FnDef* fn = find_fn(f)) {   // expanded here: its names are those of the caller
            args(fn->params.size());
            Env inner = env;
            std::string sig, call_args;
            for (std::size_t k = 0; k < fn->params.size(); ++k) {
                const std::string pn = "p_" + fn->params[k];
                inner.params[fn->params[k]] = pn;
                sig += (k ? ", " : "") + std::string("std::int64_t ") + pn;
                call_args += (k ? ", " : "") + n(k);
            }
            return {"[&](" + sig + ") -> std::int64_t { return " + num(*fn->body, inner) + "; }(" + call_args + ")", kInt64};
        }
        switch (e.code) {
            case Expr::Min: args(2); return {"std::min<std::int64_t>(" + n(0) + ", " + n(1) + ")", kInt64};
            case Expr::Max: args(2); return {"std::max<std::int64_t>(" + n(0) + ", " + n(1) + ")", kInt64};
            case Expr::Peek8: args(1); return {"rt::peek<1, false>(cx, " + n(0) + ")", kInt64};
            case Expr::Peek16: args(1); return {"rt::peek<2, false>(cx, " + n(0) + ")", kInt64};
            case Expr::Peek32: args(1); return {"rt::peek<4, false>(cx, " + n(0) + ")", kInt64};
            case Expr::Peek16be: args(1); return {"rt::peek<2, true>(cx, " + n(0) + ")", kInt64};
            case Expr::Peek32be: args(1); return {"rt::peek<4, true>(cx, " + n(0) + ")", kInt64};
            case Expr::Align: args(2); return {"rt::align(cx, " + p + ", " + n(0) + ", " + n(1) + ")", kInt64};
            case Expr::Sum8: args(2); return {"rt::sum_bytes(cx, " + p + ", " + n(0) + ", " + n(1) + ", false, \"sum8\")", kInt64};
            case Expr::Printable: args(2); return {"rt::sum_bytes(cx, " + p + ", " + n(0) + ", " + n(1) + ", true, \"printable\")", kInt64};
            case Expr::Scan:
                if (e.kids.size() != 2 || e.kids[1]->op != Expr::Op::Text) fail(env.s->def->name, "scan(offset, \"text\")");
                return {"rt::scan(cx, " + n(0) + ", " + sv_literal(e.kids[1]->name) + ")", kInt64};
            case Expr::At:
                if (e.kids.size() != 2 || e.kids[1]->op != Expr::Op::Text) fail(env.s->def->name, "at(offset, \"text\")");
                return {"rt::at_text(cx, " + n(0) + ", " + sv_literal(e.kids[1]->name) + ")", kInt64};
            case Expr::Ends: {   // a read has no file name: only an empty ending matches
                bool empty = false;
                for (const ExprP& k : e.kids) {
                    if (k->op != Expr::Op::Text) fail(env.s->def->name, "ends(\".ext\", ...) takes texts");
                    empty = empty || k->name.empty();
                }
                return {empty ? "std::int64_t(1)" : "std::int64_t(0)", kInt64};
            }
            case Expr::Count: {
                args(1);
                Ex v = expr(*e.kids[0], env);
                if (v.ty->k == Ty::K::Unknown) return v;
                if (v.ty->k != Ty::K::Array) fail(env.s->def->name, "count() of something that is not an array");
                return {"static_cast<std::int64_t>(" + v.code + ".size())", kInt64};
            }
            case Expr::Xlat: {   // xlat(array, v, from, len, to)
                args(5);
                Ex arr = expr(*e.kids[0], env);
                const std::string v = n(1);
                for (std::size_t k = 2; k < 5; ++k)
                    if (e.kids[k]->op != Expr::Op::Name) fail(env.s->def->name, "xlat names the fields from, len and to");
                if (arr.ty->k == Ty::K::Unknown) return arr;
                if (arr.ty->k != Ty::K::Array || arr.ty->elem->k != Ty::K::Struct) fail(env.s->def->name, "xlat of something that is not an array of structures");
                Env el = env;
                Ex elx{"el_", arr.ty->elem};
                const std::string from = member(elx, e.kids[2]->name, el).code, len = member(elx, e.kids[3]->name, el).code,
                                  to = member(elx, e.kids[4]->name, el).code;
                return {"[&]() -> std::int64_t { const auto& arr_ = " + arr.code + "; const std::int64_t v_ = " + v +
                            "; for (const auto& el_ : arr_) { const std::int64_t f0_ = " + from + "; if (v_ >= f0_ && v_ < f0_ + " + len +
                            ") return " + to + " + (v_ - f0_); } return -1; }()",
                        kInt64};
            }
            default: return {throw_num(env, "unknown function " + f), kUnknown};
        }
    }

    // ---- statements ----

    // The fewest bytes a value of this type takes in the file (0 when it may take none): what an array's reserve is cut to.
    std::uint64_t min_size(const TypeRef& t, int depth = 0) const {
        switch (t.k) {
            case TypeRef::K::Int: return t.width;
            case TypeRef::K::Str:
            case TypeRef::K::Bytes: return t.len && t.len->op == Expr::Op::Num && t.len->num > 0 ? static_cast<std::uint64_t>(t.len->num) : 0;
            case TypeRef::K::Cstr: return 0;
            case TypeRef::K::Struct: {
                if (depth > 8) return 0;
                std::uint64_t n = 0;
                for (const Stmt& s : structs_.at(t.name).def->body)
                    if (s.k == Stmt::K::Field && !s.field.at && !s.field.when && s.field.part.empty() && !s.field.tolerant && s.field.arr == Arr::None && !s.field.window)
                        n += min_size(s.field.type, depth + 1);
                return n;
            }
        }
        return 0;
    }

    // An array without a count: room for the first elements the data can hold (a few reallocations fewer).
    void open_reserve(int d, const FieldDef& fd) {
        if (const std::uint64_t each = min_size(fd.type); each != 0)
            line(d, std::format("arr.reserve(static_cast<std::size_t>(std::min<std::uint64_t>(64, c <= fend ? (fend - c) / {} : 0)));", each));
    }

    std::size_t tmp_ = 0;
    std::string fresh(const std::string& base) { return base + std::to_string(tmp_++); }

    void line(int depth, const std::string& s) {
        out_.append(static_cast<std::size_t>(depth) * 4, ' ');
        out_ += s;
        out_ += '\n';
    }

    // One value of the field's type into `dst` at cursor `c`, limit `fend`; `idx` the index a struct sees.
    void read_value(int d, const FieldDef& fd, const std::string& dst, const std::string& c, const std::string& fend, const std::string& idx,
                    const Env& env, std::size_t site, const TyP& target) {
        const TypeRef& t = fd.type;
        switch (t.k) {
            case TypeRef::K::Int: {
                const bool big = t.endian_set ? t.big : f_.big;
                line(d, std::format("rt::room(cx, {}, {}, {});", c, t.width, fend));
                line(d, std::format("{} = static_cast<{}>(rt::{}<{}, {}>(cx.data.data() + {}));", dst, cpp(target), t.sgn ? "load_signed" : "load",
                                    t.width, big ? "true" : "false", c));
                line(d, std::format("{} += {};", c, t.width));
                break;
            }
            case TypeRef::K::Str:
            case TypeRef::K::Bytes: {
                const std::string n = fresh("n");
                line(d, std::format("const std::uint64_t {} = rt::length(cx, pos, {});", n, num(*t.len, env)));
                line(d, std::format("rt::room(cx, {}, {}, {});", c, n, fend));
                line(d, std::format("{} = rt::{}(cx, {}, {});", dst, t.k == TypeRef::K::Str ? "text_at" : "bytes_at", c, n));
                line(d, std::format("{} += {};", c, n));
                break;
            }
            case TypeRef::K::Cstr: {
                const std::string lim = fresh("lim");
                line(d, std::format("if ({} > {}) cx.truncated({}, 1, 0);", c, fend, c));
                line(d, std::format("std::uint64_t {} = {} - {};", lim, fend, c));
                if (t.len) line(d, std::format("{} = std::min({}, rt::length(cx, pos, {}));", lim, lim, num(*t.len, env)));
                const std::string n = fresh("n");
                line(d, std::format("const std::uint64_t {} = rt::cstr_length(cx, {}, {});", n, c, lim));
                line(d, std::format("{} = rt::text_at(cx, {}, {});", dst, c, n));
                line(d, std::format("{} += {};", c, n));
                break;
            }
            case TypeRef::K::Struct:
                line(d, std::format("read_{}(cx, {}, {}, {}, {}, Site{}<Up>{{self, up}});", t.name, dst, c, fend, idx, site));
                line(d, std::format("{} += {}._size;", c, dst));
                break;
        }
    }

    void field(int d, const Stmt& stmt, State& st, std::size_t& site_counter, const std::string& sname) {
        const FieldDef& fd = stmt.field;
        StructInfo& s = structs_.at(sname);
        const TyP fty = field_ty(fd, sname);
        const Member& m = member_of(s, fd.name, fty);
        Env env{&s, &st};
        std::size_t site = 0;
        if (fd.type.k == TypeRef::K::Struct) site = site_ids_.at(sname)[site_counter++];
        line(d, "{   // " + fd.name);
        ++d;
        int closes = 0;
        if (!fd.part.empty()) {
            line(d, "if (cx.opt.all_parts || cx.opt.parts." + cname(fd.part) + ") {");
            ++d, ++closes;
        }
        if (fd.when) {
            line(d, "if (" + num(*fd.when, env) + " != 0) {");
            ++d, ++closes;
        }
        const bool ptr = fd.at != nullptr;
        const bool tol = fd.tolerant;
        const bool num_field = fd.arr == Arr::None && fd.type.k == TypeRef::K::Int;
        // the destination
        std::string dst;
        const TyP target = m.ty->k == Ty::K::Array ? m.ty->elem : m.ty;
        if (fd.arr == Arr::None) {
            dst = m.optional ? "(*self." + m.cname + ")" : "self." + m.cname;
            if (m.optional && !num_field) line(d, "self." + m.cname + ".emplace();");
            else if (m.optional) line(d, "self." + m.cname + ".emplace();");
        } else {
            line(d, "auto& arr = " + std::string(m.optional ? "self." + m.cname + ".emplace()" : "self." + m.cname) + ";");
            if (!m.optional) line(d, "arr.clear();");
        }
        line(d, "std::uint64_t fat = pos, c = pos, fend = end;");
        if (fd.window) line(d, "std::uint64_t wsize = 0; [[maybe_unused]] bool windowed = false;");
        if (tol) {
            line(d, "bool missing = false;");
            line(d, "try {");
            ++d;
        }
        if (ptr) {
            line(d, "fat = rt::length(cx, pos, " + num(*fd.at, env) + ");");
            line(d, "fend = cx.data.size();");
        }
        if (fd.window) {
            line(d, "wsize = rt::length(cx, pos, " + num(*fd.window, env) + ");");
            line(d, "windowed = true;");
            line(d, "fend = std::min(fend, fat + wsize);");
        }
        line(d, "c = fat;");
        int inner_close = 0;
        // a `try` value the data cannot hold even its fewest bytes of is given up without an exception (any error of its read
        // would leave it out alike)
        const std::uint64_t least = fd.arr == Arr::None ? min_size(fd.type) : 0;
        if (tol && least != 0) {
            line(d, std::format("if (fat > fend || {} > fend - fat) {{", least));
            line(d + 1, "missing = true;");
            line(d, "} else {");
            ++d, ++inner_close;
        }
        Env cond = env;
        cond.pos = "c";
        auto element = [&](int dd, const std::string& idx) {
            if (tol) {
                if (const std::uint64_t each = min_size(fd.type); each != 0) line(dd, std::format("if (c > fend || {} > fend - c) break;   // (an element the data cannot hold)", each));
                line(dd, "try {");
                line(dd + 1, "auto& el = arr.emplace_back();");
                read_value(dd + 1, fd, "el", "c", "fend", idx, env, site, target);
                line(dd, "} catch (const FormatError&) {");
                line(dd + 1, "arr.pop_back();");
                line(dd + 1, "break;");
                line(dd, "}");
            } else {
                line(dd, "auto& el = arr.emplace_back();");
                read_value(dd, fd, "el", "c", "fend", idx, env, site, target);
            }
        };
        switch (fd.arr) {
            case Arr::None: {
                read_value(d, fd, dst, "c", "fend", "0", env, site, target);
                if (!fd.magic.empty()) {
                    std::string test;
                    for (const Literal& l : fd.magic) {
                        if (!test.empty()) test += " || ";
                        if (fd.type.k == TypeRef::K::Int) {
                            if (l.is_text) test += "false";
                            else test += std::format("static_cast<std::uint64_t>({}) == {}ull", dst, l.num);
                        } else if (fd.type.k == TypeRef::K::Struct) {
                            fail(sname, "a magic on a structure");
                        } else {
                            test += l.is_text ? "rt::magic_bytes(" + dst + ", " + sv_literal(l.text) + ")" : "false";
                        }
                    }
                    line(d, "if (!(" + test + ")) cx.fail(fat, " + cstring("'" + fd.name + "' is not what the format starts with (magic mismatch)") + ");");
                }
                break;
            }
            case Arr::Count:
                line(d, "const std::uint64_t count = rt::length(cx, pos, " + num(*fd.arr_expr, env) + ");");
                line(d, "if (count > (1u << 24)) {");
                line(d + 1, "if (c > fend || count > fend - c) cx.truncated(c, count, c > fend ? 0 : fend - c);");
                line(d + 1, "cx.fail(fat, " + cstring("'" + fd.name + "' claims ") + " + std::to_string(count) + \" elements\");");
                line(d, "}");
                if (const std::uint64_t each = min_size(fd.type); each != 0)   // (as many as the data can hold: a damaged count reserves no more)
                    line(d, std::format("arr.reserve(static_cast<std::size_t>(std::min<std::uint64_t>(count, c <= fend ? (fend - c) / {} : 0)));", each));
                else
                    line(d, "arr.reserve(static_cast<std::size_t>(std::min<std::uint64_t>(count, 4096)));");
                line(d, "for (std::uint64_t i = 0; i < count; ++i) {");
                element(d + 1, "i");
                line(d, "}");
                break;
            case Arr::Until: {
                open_reserve(d, fd);
                line(d, "for (std::uint64_t i = 0; c < fend; ++i) {");
                element(d + 1, "i");
                Env u = cond;
                u.under = "arr.back()";
                u.under_ty = target;
                line(d + 1, "if (" + num(*fd.arr_expr, u) + " != 0) break;");
                line(d, "}");
                break;
            }
            case Arr::Size:
                open_reserve(d, fd);
                line(d, "const std::uint64_t limit = fat + rt::length(cx, pos, " + num(*fd.arr_expr, env) + ");");
                line(d, "for (std::uint64_t i = 0; c < limit && c < fend; ++i) {");
                element(d + 1, "i");
                line(d, "}");
                break;
            case Arr::While: {
                open_reserve(d, fd);
                line(d, "for (std::uint64_t i = 0; " + num(*fd.arr_expr, cond) + " != 0; ++i) {");
                line(d + 1, "if (i >= (1u << 24)) cx.fail(fat, " + cstring("'" + fd.name + "' has more than 16 million elements") + ");");
                element(d + 1, "i");
                if (fd.arr_until) {
                    Env u = cond;
                    u.under = "arr.back()";
                    u.under_ty = target;
                    line(d + 1, "if (" + num(*fd.arr_until, u) + " != 0) break;");
                }
                line(d, "}");
                break;
            }
        }
        while (inner_close--) {
            --d;
            line(d, "}");
        }
        if (tol) {
            --d;
            line(d, "} catch (const FormatError&) {");
            line(d + 1, "missing = true;");
            line(d, "}");
            line(d, "if (missing) {");
            if (num_field && m.try_num) line(d + 1, "self." + m.cname + ".reset();");
            else if (num_field) line(d + 1, "self." + m.cname + " = 0;");
            else line(d + 1, "self." + m.cname + ".reset();");
            if (!ptr && fd.arr == Arr::None) {
                if (fd.window) {
                    line(d + 1, "if (windowed) pos += wsize;");
                    line(d + 1, "else {");
                }
                const int dd = d + (fd.window ? 2 : 1);
                if (fd.type.k == TypeRef::K::Int) line(dd, std::format("pos += {};", fd.type.width));
                else if ((fd.type.k == TypeRef::K::Str || fd.type.k == TypeRef::K::Bytes) && fd.type.len) {
                    line(dd, "try {");
                    line(dd + 1, "pos += rt::length(cx, pos, " + num(*fd.type.len, env) + ");");
                    line(dd, "} catch (const FormatError&) {");
                    line(dd, "}");
                }
                if (fd.window) line(d + 1, "}");
            }
            line(d, "} else {");
            ++d;
        }
        if (fd.type.k == TypeRef::K::Struct && fd.arr == Arr::None) {   // a struct covers its window, if it has one
            if (fd.window) line(d, "if (windowed) " + dst + "._size = wsize;");
        }
        if (!ptr) line(d, fd.window ? "pos = windowed ? fat + wsize : c;" : "pos = c;");
        if (tol) {
            --d;
            line(d, "}");
        }
        while (closes--) {
            --d;
            line(d, "}");
        }
        --d;
        line(d, "}");
        set_pres(s, st, m.cname, field_pres(fd));
    }

    void body(int d, const std::vector<Stmt>& stmts, State& st, std::size_t& site_counter, const std::string& sname) {
        StructInfo& s = structs_.at(sname);
        for (const Stmt& stmt : stmts) {
            Env env{&s, &st};
            switch (stmt.k) {
                case Stmt::K::Let: {
                    const Member& m = member_of(s, stmt.let_name, kInt64);
                    line(d, std::format("self.{} = static_cast<{}>({});", m.cname, cpp(m.ty), num(*stmt.expr, env)));
                    set_pres(s, st, m.cname, Pres::Definite);
                    break;
                }
                case Stmt::K::Assert: {
                    const std::string msg = stmt.message.empty() ? "an assertion of the description does not hold (line " + std::to_string(stmt.line) + ")" : stmt.message;
                    line(d, "if (" + num(*stmt.expr, env) + " == 0) cx.fail(pos, " + cstring(msg) + ");");
                    break;
                }
                case Stmt::K::If: {
                    line(d, "if (" + num(*stmt.expr, env) + " != 0) {");
                    State a = st, b = st;
                    body(d + 1, stmt.body, a, site_counter, sname);
                    if (!stmt.else_body.empty()) {
                        line(d, "} else {");
                        body(d + 1, stmt.else_body, b, site_counter, sname);
                    }
                    line(d, "}");
                    st = merge(a, b);
                    break;
                }
                case Stmt::K::Field: field(d, stmt, st, site_counter, sname); break;
            }
        }
    }

    std::map<std::string, std::vector<std::size_t>> site_ids_;   // a struct's sites in the order its statements read them
    std::set<std::string> recursive_;                             // the structs read (directly or not) inside themselves

    void find_recursive() {
        std::map<std::string, std::set<std::string>> reads;
        for (const Site& site : sites_) reads[site.encloser].insert(site.child);
        for (const auto& [n, s] : structs_) {
            std::set<std::string> seen;
            std::vector<std::string> todo(reads[n].begin(), reads[n].end());
            while (!todo.empty()) {
                const std::string t = todo.back();
                todo.pop_back();
                if (t == n) {
                    recursive_.insert(n);
                    break;
                }
                if (!seen.insert(t).second) continue;
                for (const std::string& u : reads[t]) todo.push_back(u);
            }
        }
    }

    // ---- the files ----

    std::vector<std::string> struct_order() const {   // a struct after the structs it holds
        std::vector<std::string> out;
        std::set<std::string> done, busy;
        std::function<void(const std::string&)> visit = [&](const std::string& n) {
            if (done.contains(n)) return;
            if (busy.contains(n)) throw Unsupported(f_.name + ": the struct '" + n + "' holds itself (not handled)");
            busy.insert(n);
            for (const Member& m : structs_.at(n).members)   // (by value: a vector may hold a struct not yet defined)
                if (m.ty->k == Ty::K::Struct) visit(m.ty->ctype);
            busy.erase(n);
            done.insert(n);
            out.push_back(n);
        };
        for (const auto& [n, s] : structs_) visit(n);
        return out;
    }

    std::string ns() const { return "cb::formats::gen::" + cname(f_.name); }

    // values(): what read() gives as the engine tree's (path, value) pairs, for format_check --gen.
    void emit_value(std::string& c, int d, const TyP& t, const std::string& x, const std::string& path, int depth = 0) const {
        const std::string pad(static_cast<std::size_t>(d) * 4, ' ');
        switch (t->k) {
            case Ty::K::Num: c += pad + "out.emplace_back(" + path + ", std::to_string(static_cast<std::int64_t>(" + x + ")));\n"; break;
            case Ty::K::Str:
            case Ty::K::Bytes: c += pad + "out.emplace_back(" + path + ", span_at(base, " + x + ".data(), " + x + ".size()));\n"; break;
            case Ty::K::Struct: c += pad + "put_" + t->ctype + "(base, " + x + ", " + path + ", out);\n"; break;
            case Ty::K::Array: {
                const std::string i = "i" + std::to_string(depth);
                c += pad + "out.emplace_back(" + path + ", \"[\" + std::to_string(" + x + ".size()) + \"]\");\n";
                c += pad + "for (std::size_t " + i + " = 0; " + i + " < " + x + ".size(); ++" + i + ")\n";
                emit_value(c, d + 1, t->elem, x + "[" + i + "]", path + " + \"[\" + std::to_string(" + i + ") + \"]\"", depth + 1);
                break;
            }
            default: break;
        }
    }

    void write_values(std::string& c) const {
        c += "namespace values_detail {\n\nusing Values = std::vector<std::pair<std::string, std::string>>;\n\n";
        c += "[[maybe_unused]] std::string span_at(ByteView base, const void* p, std::size_t n) {\n";
        c += "    return \"@\" + std::to_string(static_cast<const std::uint8_t*>(p) - base.data()) + \"+\" + std::to_string(n);\n}\n";
        c += "[[maybe_unused]] std::string join(const std::string& path, const char* name) { return path.empty() ? std::string(name) : path + \".\" + name; }\n\n";
        for (const auto& [n, s] : structs_) c += "void put_" + n + "(ByteView base, const " + n + "& v, const std::string& path, Values& out);\n";
        c += "\n";
        for (const auto& [n, s] : structs_) {
            c += "void put_" + n + "([[maybe_unused]] ByteView base, const " + n + "& v, const std::string& path, Values& out) {\n";
            c += "    out.emplace_back(path, \"@\" + std::to_string(v._offset) + \"+\" + std::to_string(v._size));\n";
            for (const Member& m : s.members) {
                const std::string p = "join(path, " + cstring(m.name) + ")";
                if (m.try_num) {   // (the engine's tree holds a 0 for a number it could not read)
                    emit_value(c, 1, m.ty, "v." + m.cname + ".value_or(0)", p);
                } else if (m.optional) {
                    c += "    if (v." + m.cname + ") {\n";
                    emit_value(c, 2, m.ty, "(*v." + m.cname + ")", p);
                    c += "    }\n";
                } else {
                    emit_value(c, 1, m.ty, "v." + m.cname, p);
                }
            }
            c += "}\n\n";
        }
        c += "}  // namespace values_detail\n\n";
        c += "std::vector<std::pair<std::string, std::string>> values(ByteView data) {\n";
        c += "    values_detail::Values out;\n    values_detail::put_" + f_.root + "(data, read(data), \"\", out);\n    return out;\n}\n\n";
    }

public:
    // The table of every generated reader's values() (formats/format_gen_check reads it).
    static std::string registry(const std::vector<std::string>& names) {
        std::string c = "// generated by format_gen (docs/FORMAT_CODEGEN.md) -- do not edit: every generated reader's values()\n";
        c += "#include <cstddef>\n#include <string>\n#include <utility>\n#include <vector>\n\n#include \"core/bytes.hpp\"\n";
        for (const std::string& n : names) c += "#include \"formats/gen/" + n + ".hpp\"\n";
        c += "\nnamespace cb::formats::gen {\n\nstruct GeneratedReader {\n    const char* format;\n";
        c += "    std::vector<std::pair<std::string, std::string>> (*values)(ByteView data);\n};\n\n";
        c += "extern const GeneratedReader kGeneratedReaders[] = {\n";
        for (const std::string& n : names) c += "    {" + cstring(n) + ", &" + cname(n) + "::values},\n";
        c += "    {nullptr, nullptr}};\n\n}  // namespace cb::formats::gen\n";
        return c;
    }

public:
    bool has_values() const { return structs_.contains(f_.root); }

private:

    void write_header(std::string& h) {
        h += "// generated by format_gen from config/formats/" + f_.name + ".fmt (docs/FORMAT_CODEGEN.md) -- do not edit\n";
        h += "#pragma once\n\n#include <cstdint>\n#include <optional>\n#include <string_view>\n#include <vector>\n\n#include \"core/bytes.hpp\"\n\n";
        h += "namespace " + ns() + " {\n\n";
        h += "inline constexpr std::string_view kFormat = " + cstring(f_.name) + ";\n\n";
        for (const auto& [n, s] : structs_) h += "struct " + n + ";\n";
        h += "\n";
        for (const std::string& n : struct_order()) {
            const StructInfo& s = structs_.at(n);
            h += "struct " + n + " {\n";
            for (const Member& m : s.members) {
                const std::string t = cpp(m.ty);
                h += "    " + (m.optional ? "std::optional<" + t + ">" : t) + " " + m.cname;
                h += (m.ty->k == Ty::K::Num && !m.optional) ? " = 0;\n" : ";\n";
            }
            h += "    std::uint64_t _offset = 0, _size = 0;   // the bytes the struct covers\n};\n\n";
        }
        h += "struct Parts {\n";
        for (const std::string& p : parts_) h += "    bool " + cname(p) + " = false;\n";
        h += "};\n\nstruct Vars {\n";
        for (const std::string& v : vars_) h += "    std::int64_t " + cname(v) + " = 0;\n";
        h += "};\n\nstruct Options {\n    Parts parts;\n    bool all_parts = false;\n    Vars vars;\n};\n\n";
        std::set<std::string> enums;
        for (const Format* g : chain_)
            for (const auto& [n, e] : g->enums)
                if (enums.insert(n).second) h += "std::string label_" + n + "(std::uint64_t value);   // the engine's label: the name, or the flags \"a|b\"\n";
        if (!enums.empty()) h = h.replace(h.find("#include <string_view>"), 22, "#include <string>\n#include <string_view>"), h += "\n";
        const std::string root = f_.root;
        for (const std::string& n : struct_order()) h += n + " read_" + n + "(ByteView data, std::uint64_t at = 0, const Options& options = {});\n";
        if (structs_.contains(root)) {
            h += root + " read(ByteView data, const Options& options = {});   // the root struct at 0\n\n";
            h += "// What read() gives, as (path, value) pairs in the engine tree's terms (format_check --gen compares the two):\n";
            h += "// a number or let its value, a str, bytes or struct \"@offset+size\", an array \"[count]\".\n";
            h += "std::vector<std::pair<std::string, std::string>> values(ByteView data);\n";
            if (h.find("#include <string>\n") == std::string::npos) h = h.replace(h.find("#include <string_view>"), 22, "#include <string>\n#include <string_view>");
            h = h.replace(h.find("#include <vector>"), 17, "#include <utility>\n#include <vector>");
        }
        h += "\n}  // namespace " + ns() + "\n";
    }

    void write_source(std::string& c) {
        // which sites each struct has (in statement order)
        for (const Site& site : sites_) site_ids_[site.encloser].push_back(site.id);
        c += "// generated by format_gen from config/formats/" + f_.name + ".fmt (docs/FORMAT_CODEGEN.md) -- do not edit\n";
        c += "#include \"formats/gen/" + f_.name + ".hpp\"\n\n#include <format>\n\n#include \"formats/format_gen_runtime.hpp\"\n\n";
        c += "#ifdef _MSC_VER\n#pragma warning(disable : 4702)   // (code after an expression that always throws: the engine's error where it is evaluated)\n#endif\n\n";
        c += "namespace " + ns() + " {\n\nnamespace detail {\n\nnamespace rt = cb::formats::gen::rt;\n\n";
        c += "struct Cx : rt::Base {\n    const Options& opt;\n};\n\n";
        find_recursive();
        // a struct read inside itself: its enclosers behind an interface (a template on them would never end)
        for (const auto& [n, s] : structs_) {
            if (!recursive_.contains(n)) continue;
            c += "struct Up" + n + " {   // what " + n + " needs of the struct that reads it\n";
            for (const auto& [name, t] : s.up_ty) c += "    virtual " + ret_type(t) + " " + cname(name) + "(const Cx& cx, std::uint64_t pos) const = 0;\n";
            if (s.uses_parent) c += "    virtual " + ret_type(s.parent_ty) + " parent_(const Cx& cx, std::uint64_t pos) const = 0;\n";
            c += "    virtual ~Up" + n + "() = default;\n};\n\n";
        }
        // the read functions, declared
        for (const auto& [n, s] : structs_) {
            if (recursive_.contains(n)) c += "void read_" + n + "(const Cx& cx, " + n + "& self, std::uint64_t at, std::uint64_t end, std::uint64_t index, const Up" + n + "& up);\n";
            else c += "template <class Up>\nvoid read_" + n + "(const Cx& cx, " + n + "& self, std::uint64_t at, std::uint64_t end, std::uint64_t index, const Up& up);\n";
        }
        c += "\n";
        // the roots: a name no encloser holds
        for (const auto& [n, s] : structs_) {
            const bool rec = recursive_.contains(n);
            const std::string ov = rec ? " override" : "";
            c += "struct Root" + n + (rec ? " final : Up" + n : "") + " {\n";
            for (const auto& [name, t] : s.up_ty)
                c += "    [[noreturn]] " + ret_type(t) + " " + cname(name) + "(const Cx& cx, std::uint64_t pos) const" + ov + " { cx.fail(pos, " + cstring("unknown name '" + name + "'") + "); }\n";
            if (s.uses_parent)
                c += "    [[noreturn]] " + ret_type(s.parent_ty) + " parent_(const Cx& cx, std::uint64_t pos) const" + ov + " { cx.fail(pos, \"`_parent` of the root\"); }\n";
            c += "};\n";
        }
        c += "\n";
        // the sites: what a struct read from here sees of the names it needs
        for (const Site& site : sites_) {
            const StructInfo& up = structs_.at(site.encloser);
            const StructInfo& child = structs_.at(site.child);
            const bool rec = recursive_.contains(site.child);
            const std::string ov = rec ? " override" : "";
            const std::string sn = "Site" + std::to_string(site.id);
            c += "template <class Up>\nstruct " + sn + (rec ? " final : Up" + site.child : "") + " {   // " + site.child + " read by " + site.encloser + "\n";
            c += "    const " + site.encloser + "& self;\n    const Up& up;\n";
            c += "    " + sn + "(const " + site.encloser + "& s, const Up& u) : self(s), up(u) {}\n";
            if (child.uses_parent) c += "    const " + site.encloser + "& parent_(const Cx&, std::uint64_t) const" + ov + " { return self; }\n";
            for (const std::string& n : child.needs) {
                const TyP want = child.up_ty.at(n);
                const Member* m = nullptr;
                const Pres p = presence(up, site.state, n, &m);
                std::string bodytext;
                if (p == Pres::Absent) {
                    bodytext = "return up." + cname(n) + "(cx, pos);";
                } else {
                    const std::string v = val_of(mval("self", *m), m->ty);
                    if (p == Pres::Definite) bodytext = "return " + v + ";";
                    else bodytext = "if (self." + m->cname + ") return " + v + "; return up." + cname(n) + "(cx, pos);";
                }
                c += "    " + ret_type(want) + " " + cname(n) + "([[maybe_unused]] const Cx& cx, [[maybe_unused]] std::uint64_t pos) const" + ov + " { " + bodytext + " }\n";
            }
            c += "};\n\n";
        }
        // the read functions
        for (const auto& [n, s] : structs_) {
            out_.clear();
            State st;
            std::size_t site_counter = 0;
            body(1, s.def->body, st, site_counter, n);
            if (recursive_.contains(n)) {
                c += "void read_" + n + "([[maybe_unused]] const Cx& cx, " + n +
                     "& self, std::uint64_t at, [[maybe_unused]] std::uint64_t end, [[maybe_unused]] std::uint64_t index, [[maybe_unused]] const Up" + n + "& up) {\n";
                c += "    using Up = Up" + n + ";\n";
            } else {
                c += "template <class Up>\nvoid read_" + n + "([[maybe_unused]] const Cx& cx, " + n +
                     "& self, std::uint64_t at, [[maybe_unused]] std::uint64_t end, [[maybe_unused]] std::uint64_t index, [[maybe_unused]] const Up& up) {\n";
            }
            c += "    std::uint64_t pos = at;\n    self._offset = at;\n";
            c += out_;
            c += "    self._size = pos - at;\n}\n\n";
        }
        // enum labels
        std::set<std::string> enums;
        c += "}  // namespace detail\n\n";
        for (const Format* g : chain_)
            for (const auto& [n, e] : g->enums) {
                if (!enums.insert(n).second) continue;
                c += "std::string label_" + n + "(std::uint64_t value) {\n";
                if (!e.flags) {
                    for (const auto& [item, v] : e.items) c += std::format("    if (value == {}ull) return {};\n", v, cstring(item));
                    c += "    return {};\n}\n\n";
                } else {
                    c += "    std::string out;\n    std::uint64_t left = value;\n";
                    for (const auto& [item, v] : e.items)
                        if (v != 0) c += std::format("    if ((value & {0}ull) == {0}ull) out += (out.empty() ? \"\" : \"|\") + std::string({1}), left &= ~{0}ull;\n", v, cstring(item));
                    c += "    if (left) out += std::format(\"{}0x{:x}\", out.empty() ? \"\" : \"|\", left);\n    return out;\n}\n\n";
                }
            }
        for (const auto& [n, s] : structs_) {
            c += n + " read_" + n + "(ByteView data, std::uint64_t at, const Options& options) {\n";
            c += "    const detail::Cx cx{{data, kFormat}, options};\n";
            c += "    if (at > data.size()) cx.truncated(at, 1, 0);\n";
            c += "    " + n + " out;\n    detail::read_" + n + "(cx, out, at, data.size(), 0, detail::Root" + n + "{});\n    return out;\n}\n\n";
        }
        if (structs_.contains(f_.root)) {
            c += f_.root + " read(ByteView data, const Options& options) { return read_" + f_.root + "(data, 0, options); }\n\n";
            write_values(c);
        }
        c += "}  // namespace " + ns() + "\n";
    }
};

std::string read_all(const fsys::path& p) {
    std::ifstream in(p, std::ios::binary);
    std::stringstream ss;
    ss << in.rdbuf();
    return ss.str();
}

// Write the file only when its text changes (the build reruns only what an edit touches).
void put(const fsys::path& p, const std::string& text) {
    std::error_code ec;
    if (fsys::exists(p, ec) && read_all(p) == text) return;
    std::ofstream out(p, std::ios::binary);
    out << text;
    if (!out) die("cannot write " + p.string());
}

}  // namespace

int main(int argc, char** argv) {
    if (argc < 3) {
        std::fprintf(stderr, "usage: format_gen OUT_DIR FMT_DIR [STAMP]\n");
        return 2;
    }
    const fsys::path out = argv[1], dir = argv[2];
    try {
        std::map<std::string, std::shared_ptr<Format>> all;
        std::vector<fsys::path> files;
        for (const auto& e : fsys::directory_iterator(dir))
            if (e.path().extension() == ".fmt") files.push_back(e.path());
        std::sort(files.begin(), files.end());
        for (const fsys::path& p : files) {
            auto f = parse_description(read_all(p), p.filename().string());
            all[f->name] = f;
        }
        fsys::create_directories(out);
        std::vector<std::string> with_values;
        for (const auto& [name, f] : all) {
            std::string h, c;
            try {
                Gen gen(*f, all);
                gen.run(h, c);
                if (gen.has_values()) with_values.push_back(name);
            } catch (const Unsupported& e) {   // (no reader is generated: nothing may use it)
                std::fprintf(stderr, "format_gen: warning: no reader for %s: %s\n", name.c_str(), e.what());
                h = "// generated by format_gen: no reader for " + name + " (" + e.what() + ")\n#pragma once\n";
                c = "// generated by format_gen: no reader for " + name + "\n";
            }
            put(out / (name + ".hpp"), h);
            put(out / (name + ".cpp"), c);
        }
        put(out / "registry.cpp", Gen::registry(with_values));
        if (argc > 3) put(argv[3], std::to_string(all.size()) + " descriptions\n");
    } catch (const std::exception& e) {
        die(e.what());
    }
    return 0;
}
