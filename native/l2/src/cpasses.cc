// cpasses.cc -- backends/c.py's passes on the tree (c_text's passes, each where L2's address spaces
// and types say so), in the order c_text ran them: the pointer passes, then those that read what the
// module says of the function. Each is the Python's, step for step.
#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstdio>
#include <mutex>

#include "cback.hpp"

namespace cb {

static const V& sym_of(const V& e) { return e.get("sym"); }
static bool field_sym(const V& e) {
    const V& s = sym_of(e);
    return py::truthy(s) && s.get("kind") == "field";
}
static bool in_whole(const V& e) { return now->whole.count(id(e)) != 0; }
static std::string mark(const V& e) {
    auto it = now->marks.find(id(e));
    return it == now->marks.end() ? std::string() : it->second;
}
static bool unary(const V& e, const char* op) {
    return e.get("node") == "Unary" && e.get("op") == op && !py::truthy(e.get("postfix"));
}
static std::string show(const V& e) { return now->whole.at(id(e))(e); }
static std::string node_kind(const V& e) {
    const V& n = e.get("node");
    return n.is_str() ? n.str() : std::string();
}
static V dict_or_empty(const V& v) { return py::truthy(v) ? v : V::dict(); }
static std::string loc_name(const V& v) { return sym_name(v); }
static V loc_name_v(const V& v) { return sym_name_v(v); }
using ParentMap = std::unordered_map<Id, V>;
static ParentMap parents(const Nodes& nodes) {
    ParentMap m;
    for (auto& np : nodes) m[id(np.e)] = np.p;
    return m;
}
static const V& parent_of(const ParentMap& m, const V& e) {
    auto it = m.find(id(e));
    return it == m.end() ? py::NONE : it->second;
}
static std::vector<V> live_locals() {
    std::vector<V> out;
    for (auto& v : now->locals)
        if (!now->drop.count(loc_name(v))) out.push_back(v);
    return out;
}
static std::vector<std::pair<std::string, V>> params_of(const V& doc) {
    std::vector<std::pair<std::string, V>> out;
    for (auto& p : stmts(py::truthy(doc.get("params")) ? doc.get("params") : V::list())) out.push_back({param_name(p), p.get("type")});
    return out;
}

const std::map<std::string, int> SIZEOF{{"int1", 1},  {"uint1", 1},  {"xunknown1", 1}, {"char", 1},     {"bool", 1},
                                        {"undefined", 1}, {"undefined1", 1}, {"int2", 2}, {"uint2", 2}, {"xunknown2", 2},
                                        {"undefined2", 2}, {"int4", 4}, {"uint4", 4},   {"xunknown4", 4}, {"undefined4", 4},
                                        {"float4", 4}, {"int8", 8},  {"uint8", 8},    {"xunknown8", 8}, {"undefined8", 8},
                                        {"float8", 8}, {"float10", 10}};

static const re::Regex& R_FRAME() {
    static const re::Regex r("\\w*Stack_[0-9a-f]+");
    return r;
}
static const re::Regex& R_IDENT() {
    static const re::Regex r("[A-Za-z_]\\w*");
    return r;
}
static const re::Regex& R_RAM() {
    static const re::Regex r("([a-z]+)Ram([0-9a-f]{8})");
    return r;
}

// ---------------------------------------------------------------------------------------------- the tree's walk
static void walk_into(const V& x, const V& parent, Nodes& out) {
    if (x.is_list()) {
        for (auto& y : x.seq()) walk_into(y, parent, out);
    } else if (x.is_dict() && x.has("node")) {
        out.push_back({x, parent});
        for (auto& kv : x.items()) {
            const V& v = kv.second;
            if ((v.is_dict() || v.is_list()) && !(kv.first == "type" || kv.first == "sym" || kv.first == "ret")) walk_into(v, x, out);
        }
    }
}
Nodes walk(const V& x) {
    Nodes out;
    walk_into(x, V(), out);
    return out;
}

// ---------------------------------------------------------------------------------------------- far from the frame
static void sets_into(const V& ss, std::vector<std::pair<std::string, V>>& found) {
    for (auto& s : stmts(ss)) {
        if (!s.is_dict()) continue;
        std::string n = node_kind(s);
        if (n == "ExprStmt") {
            const V& a = at(s, "expr");
            if (a.get("node") == "Assign" && !py::truthy(a.get("op")) && at(a, "target").get("node") == "Name")
                found.push_back({name_of(at(a, "target")), at(a, "value")});
            continue;
        }
        if (n == "If" && s.get("then").is_dict()) {
            sets_into(s.get("orelse"), found);
            continue;
        }
        for (const char* key : {"then", "orelse", "body", "stmts"})
            if (s.has(key)) sets_into(s.get(key), found);
        for (auto& c : stmts(s.get("cases"))) sets_into(c.get("body"), found);
    }
}

static std::pair<Opt, bool> origin_of(const V& e0) {
    static const re::Regex WP("[\\w()]+"), STAR("[\\w ]+\\*");
    V e = e0;
    if (e.get("node") == "Binary" && at(e, "op") == "+" && !bracket(token(e), at(e, "left"), 0) &&
        WP.fullmatch(sub(token(e), at(e, "right"), 1))) {
        V l = at(e, "left");
        e = l;
    }
    if (e.get("node") == "Cast" && STAR.fullmatch(type_text(at(e, "type"))) && !bracket(token(e), at(e, "x"), 1)) {
        V x = at(e, "x");
        e = x;
    }
    bool amp = false;
    if (e.get("node") == "Unary" && at(e, "op") == "&" && !py::truthy(e.get("postfix")) && !bracket(token(e), at(e, "x"), 1)) {
        V x = at(e, "x");
        e = x;
        amp = true;
    }
    if (e.get("node") == "Name") return {name_of(e), amp};
    return {std::nullopt, false};
}

std::set<std::string> frame_arrays(const std::vector<V>& locs) {
    std::set<std::string> out;
    for (auto& v : locs) {
        V t = dict_or_empty(v.get("type"));
        if (t.get("kind") == "array" && t.has("count") && at(t, "elem").get("kind") != "array") {
            V n = loc_name_v(v);
            std::string ns = py::truthy(n) ? py::str_of(n) : "";
            if (R_FRAME().fullmatch(ns)) out.insert(n.is_none() ? std::string("\x01None") : ns);
        }
    }
    return out;
}

void far_from_stack(const V& body, const std::vector<V>& locs) {
    std::vector<std::pair<std::string, V>> sets;
    sets_into(body, sets);
    std::set<std::string> arrays = frame_arrays(locs);
    auto& far = now->far;
    auto in_frame = [&](const std::pair<Opt, bool>& found, const std::set<std::string>& stack) {
        if (!found.first) return false;
        const std::string& name = *found.first;
        return (found.second && R_FRAME().fullmatch(name)) || arrays.count(name) || (!found.second && stack.count(name));
    };
    std::set<std::string> stack;
    for (int k = 0; k < 3; k++)
        for (auto& vr : sets)
            if (!far.count(vr.first) && in_frame(origin_of(vr.second), stack)) stack.insert(vr.first);
    bool dropped = true;
    while (dropped) {
        dropped = false;
        std::vector<std::string> order(stack.begin(), stack.end());
        for (auto& v : order) {
            bool any = false;
            for (auto& wr : sets)
                if (wr.first == v && !in_frame(origin_of(wr.second), stack)) {
                    any = true;
                    break;
                }
            if (any) {
                stack.erase(v);
                dropped = true;
            }
        }
    }
    for (auto& vr : sets)
        if (far.count(vr.first) && vr.second.get("node") == "Name" && stack.count(name_of(vr.second))) now->marks[id(vr.second)] = "_ss";
}

// ---------------------------------------------------------------------------------------------- shared helpers
static std::vector<V> terms_of(const V& e) {
    if (e.get("node") != "Binary" || e.get("op") != "+") return {e};
    Tok3 tok = token(e);
    std::vector<V> out;
    int k = 0;
    for (const char* side : {"left", "right"}) {
        const V& s = at(e, side);
        if (bracket(tok, s, k)) out.push_back(s);
        else
            for (auto& x : terms_of(s)) out.push_back(x);
        k++;
    }
    return out;
}

std::string pops(const std::string& ty) {
    static const re::Regex R("code (__far |__near )?\\*");
    if (!now->pops) return ty;
    re::Match m = R.fullmatch(ty);
    if (!m) return ty;
    return "code_callee_pops " + (m.has(1) ? m.group(1) : std::string()) + "*";
}

std::string cast_type(const V& c) {
    std::string mk = mark(c);
    if (mk == "_near")
        return flat_text([&](Em* em) {
            Stack st(em);
            push_type(st, at(c, "type"), std::nullopt, false, Opt(" __near"));
        });
    return type_text(c.get("type"), "", mk == "_far" || mk == "_ds");
}

static std::string written(const V& e, const V* under) {
    return under ? sub(token(*under), e, 1) : expr(e);
}

static V strip_paren(const V& n0) {
    V n = n0;
    while (n.get("node") == "Paren") {
        V x = at(n, "x");
        n = x;
    }
    return n;
}
static V leftmost(const V& n0) {
    V n = n0;
    while (true) {
        n = strip_paren(n);
        if (n.get("node") == "Binary" && (n.get("op") == "+" || n.get("op") == "-")) {
            V l = at(n, "left");
            n = l;
        } else
            return n;
    }
}
static Opt pointer_cast(const V& n) {
    if (n.get("node") != "Cast" || in_whole(n)) return std::nullopt;
    std::string ty = py::strip(type_text(n.get("type")));
    if (py::endswith(ty, "*") && py::count(ty, "*") == 1) return py::strip(ty.substr(0, ty.size() - 1));
    return std::nullopt;
}

static std::string shape(const V& e);

static std::pair<std::string, V> far_target(const V& off) {
    V first = leftmost(off);
    std::string target;
    V offset;
    Opt pc = pointer_cast(off), pf;
    if (pc) {
        target = *pc;
        offset = at(off, "x");
    } else if ((pf = pointer_cast(first))) {
        target = *pf;
        offset = off;
    } else {
        Opt n;
        if (first.get("node") == "Name" && mark(first) == "_ss") n = name_of(first);
        else if (shape(first) == "identifier") n = expr(first);
        if (n && now->drop.count(*n)) target = "uint1";
        else {
            auto it = n ? now->far_pointers.find(*n) : now->far_pointers.end();
            target = it != now->far_pointers.end() ? it->second : "uint1";
        }
        offset = off;
    }
    return {target == "void" || target == "code" ? "uint1" : target, offset};
}

std::string mk_fp_text(const std::string& seg, const V& off, const std::string& comma, const Opt& shown) {
    if (!now->has("far_addresses")) return "MK_FP(" + seg + comma + (shown && !shown->empty() ? *shown : expr(off)) + ")";
    auto to = far_target(off);
    std::string w = shown && !shown->empty() ? *shown : written(to.second, same(to.second, off) ? nullptr : &off);
    return "((" + pops(to.first + " __far *") + ")MK_FP(" + seg + ", " + w + "))";
}

std::string local_text(const std::string& name) {
    auto it = now->ren.find(name);
    return it != now->ren.end() ? it->second() : name;
}

std::string decl_now(const V& v) {
    std::string n = loc_name(v);
    auto it = now->decls.find(n);
    if (it != now->decls.end()) return it->second;
    return flat_text([&](Em* em) { put_type(em, v.get("type"), n, now->far.count(n) != 0, segment_register(n, v.get("type"))); });
}

static std::string token_after(const V& e0, const ParentMap& parent);

static bool unary_as_read(const V& u, const char* op, const ParentMap& parent) {
    if (u.is_none() || u.get("node") != "Unary" || u.get("op") != op || py::truthy(u.get("postfix")) || in_whole(u)) return false;
    const V& up = parent_of(parent, u);
    if (!(!up.is_none() && up.get("node") == "Cast" && same(up.get("x"), u) && type_text(up.get("type")).find('*') == std::string::npos &&
          !in_whole(up)))
        return true;
    if (std::string(op) != "&") return false;
    auto& bp = binary_prec();
    auto it = bp.find(token_after(up, parent));
    int prec = it == bp.end() ? 0 : it->second;
    return prec > bp.at("&");
}

static std::string token_after(const V& e0, const ParentMap& parent) {
    V e = e0;
    while (true) {
        const V& p = parent_of(parent, e);
        if (p.is_none()) return "";
        std::string n = at(p, "node").str();
        if (n == "Paren") return ")";
        Tok3 tok = token(p);
        if (n == "Binary") {
            if (same(at(p, "left"), e)) return bracket(tok, e, 0) ? ")" : at(p, "op").str();
            if (bracket(tok, e, 1)) return ")";
        } else if (n == "Assign") {
            if (same(at(p, "target"), e)) return "=";
            if (bracket(tok, e, 1)) return ")";
        } else if (n == "Cast" || n == "Unary") {
            if (bracket(tok, e, n == "Cast" ? 1 : 0)) return ")";
        } else if (n == "Call") {
            if (same(p.get("target"), e)) return "(";
            const V& args = p.get("args");
            if (py::truthy(args) && same(args.seq().back(), e)) return ")";
            return ",";
        } else if (n == "Index") {
            return same(at(p, "arr"), e) ? "[" : "]";
        } else if (n == "Select") {
            return ".";
        } else if (n == "Cond") {
            return same(p.get("cond"), e) ? "?" : same(p.get("then"), e) ? ":" : "";
        } else
            return "";
        V pp = p;
        e = pp;
    }
}

static bool calls_in(const V& e) {
    static const re::Regex R("\\w\\s*\\(");
    for (auto& np : walk(e)) {
        auto it = now->whole.find(id(np.e));
        if (it != now->whole.end()) {
            if (R.search(it->second(np.e))) return true;
            continue;
        }
        if (np.e.get("node") == "Call") return true;
    }
    return false;
}

static std::string shape(const V& e) {
    auto it = now->whole.find(id(e));
    if (it != now->whole.end()) {
        std::string t = py::strip(it->second(e));
        if (R_IDENT().fullmatch(t)) return "identifier";
        return t.empty() || t[0] == '*' || t[0] == '&' ? "pointer" : "other";
    }
    std::string n = node_kind(e);
    if (n == "Name") return mark(e) != "_ss" ? "identifier" : "other";
    if (n == "Unary" && !py::truthy(e.get("postfix")) && (e.get("op") == "*" || e.get("op") == "&")) return "pointer";
    if (n == "Select") return "field";
    if (n == "Lit") {
        static const re::Regex R("0x[0-9a-fA-F]+|\\d+|0[0-7]*");
        return R.fullmatch(lit(e)) ? "number" : "other";
    }
    return "other";
}

static std::optional<std::pair<V, V>> mk_fp(const V& e) {
    if (e.get("node") != "Call") return std::nullopt;
    const V& args = e.get("args");
    if ((py::truthy(args) ? args.size() : 0) != 2) return std::nullopt;
    V t = dict_or_empty(e.get("target"));
    if (t.get("node") == "Name" && name_of(t) == "MK_FP") return std::make_pair(args[0], args[1]);
    return std::nullopt;
}

static bool star_as_read(const V& u, const ParentMap& parent) {
    int chain = 0;
    V t = u;
    while (true) {
        const V& up = parent_of(parent, t);
        if (!up.is_none() && up.get("node") == "Cast" && same(up.get("x"), t) && !in_whole(up) && cast_type(up).find('*') == std::string::npos) {
            chain++;
            V x = up;
            t = x;
        } else
            break;
    }
    if (!chain) return true;
    const V& p = parent_of(parent, t);
    if (!p.is_none() && p.get("node") == "Unary" && !py::truthy(p.get("postfix")) && !in_whole(p)) return false;
    if (!p.is_none() && p.get("node") == "Binary" && (p.get("op") == "*" || p.get("op") == "/" || p.get("op") == "%") &&
        same(p.get("right"), t) && !bracket(token(p), t, 1))
        return chain == 1;
    return true;
}

static bool arrow_of(const V& u, const ParentMap& parent) {
    const V& s = parent_of(parent, u);
    return !s.is_none() && s.get("node") == "Select" && same(s.get("obj"), u) && arrow(s);
}

static bool unary_parent(const V& e, const ParentMap& parent, const char* op) {
    const V& p = parent_of(parent, e);
    return !p.is_none() && p.get("node") == "Unary" && p.get("op") == op && !py::truthy(p.get("postfix")) && !in_whole(p);
}

static bool passed_on(const std::string& arg, const std::string& reg) {
    std::string r = py::upper(reg), a = py::strip(arg);
    return a == "in_" + r || a == "in_E" + r || a == "unaff_" + r || a == "unaff_E" + r;
}

// ---------------------------------------------------------------------------------------------- _seen
// What the function's text is laid out from, as one number: a layout made in the same state is the
// same layout (the Python made it again each time; the text, and all it records, come out the same).
static uint64_t layout_key() {
    std::hash<std::string> hs;
    std::hash<const void*> hp;
    uint64_t k = 1469598103934665603ULL;
    auto mix = [&](uint64_t v) { k = (k ^ v) * 1099511628211ULL; };
    auto bag = [&](uint64_t v) { return v * 0x9E3779B97F4A7C15ULL; };
    mix(now->mut);
    uint64_t s = 0;
    for (auto& kv : now->marks) s += bag(hp(kv.first) ^ hs(kv.second));
    mix(s);
    s = 0;
    for (auto& kv : now->decls) s += bag(hs(kv.first) ^ (hs(kv.second) << 1));
    mix(s);
    for (auto& d : now->drop) mix(hs(d));
    for (auto& d : now->frame) mix(hs(d));
    for (auto& kv : now->alias) mix(hs(kv.first) ^ (hs(kv.second) << 1));
    s = 0;
    for (auto x : now->gone) s += bag(hp(x));
    mix(s);
    s = 0;
    for (auto x : now->far_read) s += bag(hp(x));
    mix(s);
    for (auto* m : {&now->before, &now->after})
        for (auto& kv : m->items) {
            mix(hp(kv.first));
            for (auto& t : kv.second) mix(hs(t));
        }
    for (auto& d : now->declare_first) mix(hs(d.first) ^ (hs(d.second) << 1));
    mix(now->old_style ? 1 + now->old_style->size() : 0);
    if (now->old_style)
        for (auto& d : *now->old_style) mix(hs(d.second));
    mix(now->locals_text ? hs(*now->locals_text) + 1 : 0);
    mix(now->pops);
    mix(now->far.size());
    return k;
}

IdSet seen_now() {
    uint64_t key = layout_key();
    if (now->seen_valid && key == now->seen_key) return now->seen;
    now->seen_key = key;
    now->seen_valid = true;
    now->seen.clear();
    now->seeing = true;
    now->at.clear();
    now->lines.clear();
    now->owners.clear();
    now->owner_of.clear();
    now->shown.clear();
    now->stmt_at.clear();
    now->top = make_flat();
    struct Done {
        ~Done() { now->seeing = false; }
    } done;
    function_text(now->doc, now->locals, now->top);
    return now->seen;
}

// ---------------------------------------------------------------------------------------------- the pointer passes
static void p_frame_through_ss(Nodes& nodes, const std::set<std::string>& arrays) {
    static const re::Regex TY("([\\w ]+?) ?\\*");
    auto in_frame = [&](const V& n) {
        if (n.get("node") != "Cast") return false;
        std::string tt = type_text(n.get("type"));
        if (tt != "int2" && tt != "uint2") return false;
        const V& v = at(n, "x");
        if (unary(v, "&")) return at(v, "x").get("node") == "Name" && (bool)R_FRAME().fullmatch(name_of(at(v, "x")));
        return v.get("node") == "Name" && arrays.count(name_of(v)) != 0;
    };
    for (auto& np : nodes) {
        const V& e = np.e;
        if (at(e, "node") != "Unary" || e.get("op") != "*" || py::truthy(e.get("postfix")) || at(e, "x").get("node") != "Cast") continue;
        V c = at(e, "x");
        re::Match ty = TY.fullmatch(type_text(c.get("type")));
        V sum_ = at(c, "x").get("node") == "Paren" ? at(at(c, "x"), "x") : at(c, "x");
        if (!ty || sum_.get("node") != "Binary" || sum_.get("op") != "+") continue;
        auto terms = terms_of(sum_);
        if (!(in_frame(terms.back()) || (terms.size() == 2 && in_frame(terms[0])))) continue;
        now->ss_casts.insert(id(c));
        std::string t = ty.group(1);
        rewrite(c, [t, sum_](const V& cc) {
            return "(" + pops(t + (now->far_read.count(id(cc)) ? " __far" : "") + " *") + ")" + mk_fp_text("codebreaker_ss()", sum_, ",");
        });
    }
}

static void p_pieces(Nodes& nodes, const std::set<std::string>&) {
    static const re::Regex PIECE("_(\\d+)_(\\d+)_");
    ParentMap parent = parents(nodes);
    for (auto& np : nodes) {
        const V& e = np.e;
        V p = np.p;
        if (!(at(e, "node") == "Select" && !arrow(e))) continue;
        re::Match m = PIECE.fullmatch(name_of(e));
        if (!m) continue;
        while (!p.is_none() && at(p, "node") == "Paren") {
            V q = parent_of(parent, p);
            p = q;
        }
        std::map<int64_t, std::string> table;
        std::string other;
        if (!now->sixteen) {
            table = {{1, "uint1"}, {2, "uint2"}, {4, "uint4"}, {8, "uint8"}};
            other = "uint4";
        } else {
            bool ord = !p.is_none() && at(p, "node") == "Binary" &&
                       (at(p, "op") == "<" || at(p, "op") == ">" || at(p, "op") == "<=" || at(p, "op") == ">=");
            if (ord) table = {{1, "int1"}, {2, "int2"}, {4, "int4"}};
            else table = {{1, "uint1"}, {2, "uint2"}, {4, "uint4"}};
            other = "uint2";
        }
        std::string off = m.group(1);
        int64_t size = py::int_parse(m.group(2), 10);
        std::string ty = table.count(size) ? table[size] : other;
        if (now->lane == "powerpc") {
            rewrite(e, [off, ty](const V& s) { return "PIECE(" + sub(token(s), at(s, "obj"), 0) + ", " + off + ", " + ty + ")"; });
            continue;
        }
        rewrite(e, [off, ty](const V& s) { return "(*(" + ty + " *)((char *)&" + sub(token(s), at(s, "obj"), 0) + " + " + off + "))"; });
    }
}

static void p_null_is_zero(Nodes& nodes, const std::set<std::string>&) {
    for (auto& np : nodes) {
        const V& e = np.e;
        if (at(e, "node") == "Binary" && (at(e, "op") == "==" || at(e, "op") == "!=")) {
            const V& r = at(e, "right");
            V rt = dict_or_empty(r.get("type"));
            if (r.get("node") == "Cast" && (rt.get("kind") == "ptr" || rt.get("kind") == "fnptr") && at(r, "x").get("node") == "Lit" &&
                lit(at(r, "x")) == "0x0")
                rewrite(r, [](const V&) { return std::string("0"); });
        }
    }
}

static std::pair<V, std::string> last_name(const V& e0) {
    std::string before = " ";
    V e = e0;
    while (true) {
        std::string n = node_kind(e);
        if (n == "Name") return {e, before};
        Tok3 tok = token(e);
        V kid;
        if (n == "Binary") {
            kid = at(e, "right");
            before = " ";
        } else if (n == "Assign") {
            kid = at(e, "value");
            before = " ";
        } else if (n == "Unary" && !py::truthy(e.get("postfix"))) {
            kid = at(e, "x");
            before = py::str_of(at(e, "op"));
        } else if (n == "Cast") {
            kid = at(e, "x");
            before = ")";
        } else
            return {V(), ""};
        if (bracket(tok, kid, n != "Unary" ? 1 : 0)) return {V(), ""};
        e = kid;
    }
}

static void p_global_pointer_steps(Nodes& nodes, const std::set<std::string>&) {
    static const re::Regex PIECE("_(\\d+)_(\\d+)_"), PG("p+[a-z]*Ram[0-9a-f]{8}");
    for (auto& np : nodes) {
        const V& e = np.e;
        std::pair<V, std::string> lb;
        if (at(e, "node") == "Select" && in_whole(e) && PIECE.fullmatch(name_of(e))) {
            const V& obj = at(e, "obj");
            if (bracket(token(e), obj, 0)) continue;
            lb = obj.get("node") == "Name" ? std::make_pair(obj, std::string("&")) : last_name(obj);
        } else if (at(e, "node") != "Binary" || at(e, "op") != "+" || bracket(token(e), at(e, "left"), 0))
            continue;
        else
            lb = last_name(at(e, "left"));
        if (!lb.first.is_none() && lb.second != "*" && lb.second != ")" && PG.fullmatch(name_of(lb.first)))
            rewrite(lb.first, [](const V& x) { return "(uint1 *)" + name_of(x); });
    }
}

static void p_one_name_for_globals(Nodes& nodes, const std::set<std::string>&) {
    static const re::Regex R("_[a-z]+Ram[0-9a-f]{8}");
    for (auto& np : nodes)
        if (at(np.e, "node") == "Name" && R.fullmatch(name_of(np.e))) rewrite(np.e, [](const V& x) { return name_of(x).substr(1); });
}

// ---------------------------------------------------------------------------------------------- what the module says
static bool is_wide(const std::string& w) {
    return w == "xunknown4" || w == "int4" || w == "uint4" || w == "undefined4" || w == "float4";
}
static std::string heading_words_text(const Heading& h, const V& doc, const Strs& more = {}) {
    Strs all{h.result};
    all.insert(all.end(), h.mods.begin(), h.mods.end());
    all.insert(all.end(), h.words.begin(), h.words.end());
    all.push_back(py::str_of(at(doc, "name")));
    all.insert(all.end(), more.begin(), more.end());
    return py::join(" ", all);
}

static void p_joined_arguments(Nodes& nodes, const std::set<std::string>&) {
    static const re::Regex J("_(a\\d+|param_\\d+)");
    const V& ctx = now->facts;
    Heading h = heading(now->doc);
    bool far_fn = heading_words_text(h, now->doc).find("far") != std::string::npos;
    V widths = dict_or_empty(dict_or_empty(ctx.get("widths")).get(V(far_fn)));
    for (auto& np : nodes) {
        const V& e = np.e;
        const V& p = np.p;
        if (at(e, "node") != "Name") continue;
        re::Match m = J.fullmatch(name_of(e));
        if (!m) continue;
        std::string a = m.group(1);
        if (!p.is_none() && at(p, "node") == "Unary" && p.get("op") == "*" && !py::truthy(p.get("postfix")) && same(at(p, "x"), e)) {
            bool one = py::eq(widths.get(V(a)), V(1));
            rewrite(p, [a, one](const V&) { return "*(*(" + std::string(one ? "char" : "int2") + " __far * *)&" + a + ")"; });
        } else
            rewrite(e, [a](const V&) { return "(*(uint4 *)&" + a + ")"; });
    }
}

static void p_far_arguments(Nodes& nodes, const std::set<std::string>&) {
    static const re::Regex TY("([A-Za-z_]\\w*) \\*");
    const V& doc = now->doc;
    const V& loads = now->facts.get("loads");
    V params = py::truthy(doc.get("params")) ? doc.get("params") : V::list();
    if (!py::truthy(loads) || !py::truthy(params)) return;
    Heading h = heading(doc);
    Strs texts;
    for (auto& p : params.seq()) {
        std::string pn = param_name(p);
        texts.push_back(type_text(p.get("type"), pn, now->far.count(pn) != 0));
    }
    bool near = heading_words_text(h, doc, texts).find("far") == std::string::npos;
    int64_t atw = near ? 4 : 6;
    struct W {
        std::string name, kind;
        bool wide;
    };
    std::map<int64_t, W> where;
    for (auto& prm : texts) {
        Strs ws = py::split_ws(prm);
        if (ws.empty()) py::raise("IndexError", "list index out of range");
        std::string name = py::lstrip_chars(ws.back(), "*");
        bool wide = (prm.find('*') != std::string::npos && prm.find("__far") != std::string::npos) || is_wide(ws[0]);
        where[atw] = {name, ws[0], wide};
        atw += wide ? 4 : 2;
    }
    auto word3 = [](const std::string& k) { return k == "xunknown2" || k == "int2" || k == "uint2"; };
    std::map<std::string, std::string> segment_of;
    V sorted_loads = py::sorted(loads);
    for (auto& off : sorted_loads.seq()) {
        int64_t o = off.as_int();
        if (!where.count(o) || !where.count(o + 2)) continue;
        W a = where[o], b = where[o + 2];
        if (a.wide || b.wide || !word3(a.kind) || !word3(b.kind)) continue;
        segment_of.emplace(a.name, b.name);
    }
    if (segment_of.empty()) return;
    for (auto& np : nodes) {
        const V& e = np.e;
        if (at(e, "node") != "Unary" || e.get("op") != "*" || py::truthy(e.get("postfix")) || at(e, "x").get("node") != "Cast") continue;
        const V& c = at(e, "x");
        re::Match ty = TY.fullmatch(type_text(c.get("type")));
        if (!ty || !mark(c).empty() || in_whole(c)) continue;
        V v = at(c, "x").get("node") == "Paren" ? at(at(c, "x"), "x") : at(c, "x");
        std::string name, rest;
        if (v.get("node") == "Name" && segment_of.count(name_of(v))) {
            name = name_of(v);
        } else if (v.get("node") == "Binary" && v.get("op") == "+" && at(v, "left").get("node") == "Name" &&
                   segment_of.count(name_of(at(v, "left"))) && !bracket(token(v), at(v, "left"), 0)) {
            name = name_of(at(v, "left"));
            std::string inner = expr(v);
            if (inner.find('(') != std::string::npos || inner.find(')') != std::string::npos || !py::startswith(inner, name + " + ")) continue;
            rest = " + " + py::strip(py::split(inner, "+", 1)[1]);
        } else
            continue;
        std::string t = ty.group(1), s = segment_of[name];
        rewrite(e, [t, s, name, rest](const V&) { return "*(" + t + " __far *)CODEBREAKER_FAR(" + s + ", " + name + rest + ")"; });
    }
}

Strs seg_layout_text(const std::vector<std::pair<int64_t, std::string>>& fields0, bool& ok) {
    auto fields = fields0;
    std::sort(fields.begin(), fields.end(), [](const std::pair<int64_t, std::string>& a, const std::pair<int64_t, std::string>& b) {
        return a.first < b.first;
    });
    Strs body;
    int64_t atb = 0;
    ok = true;
    for (auto& f : fields) {
        if (f.first < atb) {
            ok = false;
            return {};
        }
        if (f.first > atb) body.push_back("uint1 _pad_" + pyx(atb) + "[" + std::to_string(f.first - atb) + "];");
        body.push_back(f.second + " f_" + pyx(f.first) + ";");
        auto it = SIZEOF.find(f.second);
        if (it == SIZEOF.end()) py::raise("KeyError", py::str_repr(f.second));
        atb = f.first + it->second;
    }
    return body;
}

static void p_selector_structs(Nodes& nodes, const std::set<std::string>&) {
    V facts = now->facts;
    if (!py::truthy(facts.get("selector_structs"))) return;
    V decls = facts.setdefault(V("selector_decls"), V::list());
    V shared = facts.setdefault(V("selector_shared"), V::dict());
    std::string fname = py::truthy(now->doc.get("name")) ? py::str_of(now->doc.get("name")) : "";
    ParentMap parent = parents(nodes);
    struct Use {
        V e, p;
        int64_t off;
        std::string ty;
    };
    std::vector<std::pair<std::string, std::vector<Use>>> groups;
    for (auto& np : nodes) {
        const V& e = np.e;
        auto parts = mk_fp(e);
        if (!parts || in_whole(e)) continue;
        V sel_e = parts->first, off_e = parts->second;
        if (off_e.get("node") != "Cast" || in_whole(off_e) || !mark(off_e).empty()) continue;
        std::string ty = py::replace(type_text(off_e.get("type")), " ", "");
        const V& val = at(off_e, "x");
        if (!py::endswith(ty, "*") || py::count(ty, "*") != 1 || shape(val) != "number" || in_whole(val)) continue;
        int64_t off;
        try {
            off = py::int_parse(lit(val), 0);
        } catch (py::PyError& err) {
            if (err.type != "ValueError") throw;
            continue;
        }
        std::string sh = shape(sel_e);
        if ((sh != "identifier" && sh != "pointer" && sh != "field") || calls_in(sel_e)) continue;
        V up = np.p;
        bool inside = false;
        while (!up.is_none()) {
            if (mk_fp(up)) {
                inside = true;
                break;
            }
            V q = parent_of(parent, up);
            up = q;
        }
        if (inside) continue;
        std::string sel = py::join(" ", py::split_ws(expr(sel_e)));
        auto it = std::find_if(groups.begin(), groups.end(), [&](const std::pair<std::string, std::vector<Use>>& g) { return g.first == sel; });
        if (it == groups.end()) {
            groups.push_back({sel, {}});
            it = groups.end() - 1;
        }
        it->second.push_back({e, np.p, off, ty.substr(0, ty.size() - 1)});
    }
    int made = 0;
    static const re::Regex NONW("\\W");
    for (auto& g : groups) {
        const std::string& sel = g.first;
        std::vector<std::pair<int64_t, std::string>> fields;
        bool bad = false;
        for (auto& u : g.second) {
            auto f = std::find_if(fields.begin(), fields.end(), [&](const std::pair<int64_t, std::string>& x) { return x.first == u.off; });
            if (f != fields.end() && f->second != u.ty) {
                bad = true;
                break;
            }
            if (f == fields.end()) fields.push_back({u.off, u.ty});
        }
        if (bad || fields.size() < 2) continue;
        bool unknown = false;
        for (auto& f : fields) unknown = unknown || !SIZEOF.count(f.second);
        if (unknown) continue;
        bool ok;
        Strs body = seg_layout_text(fields, ok);
        if (!ok) continue;
        made++;
        std::string name;
        if (R_RAM().fullmatch(sel)) {
            name = "seg_" + sel;
            V fd = V::dict();
            for (auto& f : fields) fd.set(V(f.first), V(f.second));
            V& mine = shared.setdefault(V(name), V::dict());
            mine.set(V(fname), fd);
        } else {
            std::string tag = R_IDENT().fullmatch(sel) ? sel : "sel" + std::to_string(made);
            name = "seg_" + NONW.sub("_", fname) + "_" + tag;
            decls.append(V("typedef _Packed struct " + name + " { " + py::join(" ", body) + " } " + name + ";"));
        }
        V renames = dict_or_empty(facts.get("selector_renames"));
        if (renames.has(V(name))) name = py::str_of(renames.get(V(name)));
        for (auto& u : g.second) {
            V s = at(u.e, "args")[0];
            int64_t o = u.off;
            bool read = unary_as_read(u.p, "*", parent);
            now->seg_ptrs.push_back({read ? id(u.p) : id(u.e), name, s});
            if (read)
                rewrite(u.p, [name, s, o](const V&) { return "SEG_PTR(" + name + ", " + expr(s) + ")->f_" + pyx(o); });
            else
                rewrite(u.e, [name, s, o](const V&) { return "(&SEG_PTR(" + name + ", " + expr(s) + ")->f_" + pyx(o) + ")"; });
        }
    }
}

static std::map<std::string, std::string> pointer_distances(const V& doc) {
    static const re::Regex R("(?<![\\w*])[A-Za-z_]\\w*\\s+(__far\\s*)?\\*+\\s*(\\w+)(?: \\[\\d*\\])?");
    std::map<std::string, std::string> out;
    std::vector<std::pair<std::string, V>> decls = params_of(doc);
    for (auto& v : now->locals) decls.push_back({loc_name(v), v.get("type")});
    for (auto& nt : decls) {
        re::Match m = R.fullmatch(type_text(nt.second, nt.first, now->far.count(nt.first) != 0));
        if (m) out.emplace(m.group(2), m.has(1) ? "far" : "near");
    }
    return out;
}

static bool near_number(const std::string& inner);

static std::string gap_cast(const std::string& pt, const std::string& base) {
    std::string inner = "(" + base + ")";
    if (py::endswith(pt, "__far *") && now->has("near_to_far") && near_number(inner))
        return "(" + pt + ")(" + pt.substr(0, pt.size() - std::string(" __far *").size()) + " *)" + inner;
    return "(" + pt + ")" + inner;
}

static std::string gap_base(const V& e, const V& base, bool dot) {
    std::string arg = expr(base);
    if (bracket(token(e), base, 0)) arg = "(" + arg + ")";
    return dot ? "&(" + arg + ")" : arg;
}

static void p_struct_gaps(Nodes& nodes, const std::set<std::string>&) {
    static const re::Regex GAP("field_0x([0-9a-f]+)"), ROOT("\\(*(\\w+)"), CASTR("\\(+\\s*\\w+\\s*(__far\\s*)?\\*\\s*\\)");
    bool flat = !now->sixteen;
    if (!flat && !py::truthy(now->facts.get("selector_structs"))) return;
    std::optional<std::map<std::string, std::string>> distance;
    ParentMap parent = parents(nodes);
    for (auto& np : nodes) {
        const V& e = np.e;
        const V& p = np.p;
        if (at(e, "node") != "Select") continue;
        re::Match mm = GAP.fullmatch(name_of(e));
        if (!mm || in_whole(e)) continue;
        if (!distance) distance = !flat ? pointer_distances(now->doc) : std::map<std::string, std::string>();
        bool a = arrow(e);
        V base = a ? at(at(e, "obj"), "x") : at(e, "obj");
        std::string arg = gap_base(e, base, false);
        re::Match root = ROOT.match(arg);
        std::optional<std::string> d;
        if (root) {
            auto it = distance->find(root.group(1));
            if (it != distance->end()) d = it->second;
        }
        re::Match c = CASTR.match(arg);
        if (c) d = c.has(1) ? "far" : "near";
        if (flat) d = "near";
        if (!d) continue;
        std::string ptr = *d == "far" ? "uint1 __far *" : "uint1 *";
        bool dot = !a;
        std::string k = mm.group(1);
        V ee = e;
        if (unary_as_read(p, "&", parent)) {
            rewrite(p, [base, ptr, k, dot, ee](const V&) { return "(" + gap_cast(ptr, gap_base(ee, base, dot)) + " + 0x" + k + ")"; });
            if (!dot && in_whole(p)) now->gap_base[id(p)] = base;
        } else
            rewrite(e, [base, ptr, k, dot, ee](const V&) { return "(*(" + gap_cast(ptr, gap_base(ee, base, dot)) + " + 0x" + k + "))"; });
    }
}

static void p_own_segment_calls(Nodes& nodes, const std::set<std::string>&) {
    const V& own = now->facts.get("selector");
    if (own.is_none()) return;
    std::string spelled = "0x" + pyx(own.as_int());
    static const re::Regex NUM("0x[0-9a-fA-F]+|\\d+");
    auto ours = [&](const V& l) { return l.get("node") == "Lit" && NUM.fullmatch(lit(l)) && py::int_parse(lit(l), 0) == own.as_int(); };
    for (auto& np : nodes) {
        const V& e = np.e;
        if (at(e, "node") != "Call") continue;
        V fn = dict_or_empty(e.get("target"));
        while (fn.get("node") == "Paren") {
            V x = at(fn, "x");
            fn = x;
        }
        if (fn.get("node") != "Unary" || (fn.get("op") != "*" && fn.get("op") != "&") || py::truthy(fn.get("postfix"))) continue;
        V target = strip_paren(at(fn, "x"));
        auto parts = mk_fp(target);
        if (at(fn, "op") == "*" && parts && ours(parts->first)) {
            V off = parts->second;
            rewrite(fn, [off](const V&) { return "(*(" + pops("code __near *") + ")" + expr(off) + ")"; });
        }
    }
    for (auto& np : nodes) {
        auto parts = mk_fp(np.e);
        if (parts && parts->first.get("node") == "Lit" && lit(parts->first) == spelled)
            rewrite(parts->first, [](const V&) { return std::string("codebreaker_cs()"); });
    }
}

static const char* INTERRUPT_REGISTERS[] = {"ax", "bx", "cx", "dx", "si", "di", "ds", "es"};

static std::optional<int64_t> int_call(const V& e) {
    static const re::Regex R("int([0-9a-f]{2})");
    if (e.get("node") != "Call") return std::nullopt;
    const V& t = e.get("target");
    if (!py::truthy(t) || t.get("node") != "Name") return std::nullopt;
    re::Match m = R.fullmatch(name_of(t));
    if (!m) return std::nullopt;
    return py::int_parse(m.group(1), 16);
}

static void p_interrupts(Nodes& nodes, const std::set<std::string>&) {
    static const re::Regex RES("int_E?([A-Z][A-Z])(?:_[0-9a-f]+)?");
    V used = now->facts.setdefault(V("interrupts"), V::set());
    ParentMap parent = parents(nodes);
    for (auto& np : nodes) {
        const V& e = np.e;
        auto vector = int_call(e);
        if (!vector) continue;
        V up = np.p;
        bool inside = false;
        while (!up.is_none()) {
            if (int_call(up)) {
                inside = true;
                break;
            }
            V q = parent_of(parent, up);
            up = q;
        }
        if (inside) continue;
        std::vector<V> args;
        if (py::truthy(e.get("args")))
            for (auto& a : e.get("args").seq())
                if (args.size() < 8) args.push_back(a);
        Strs regs;
        std::vector<V> reg_v;
        for (size_t k = 0; k < args.size(); k++)
            if (!passed_on(expr(args[k]), INTERRUPT_REGISTERS[k])) {
                regs.push_back(INTERRUPT_REGISTERS[k]);
                reg_v.push_back(V(INTERRUPT_REGISTERS[k]));
            }
        py::set_add(used, V::tuple({V(*vector), V::tuple(reg_v)}));
        int64_t v = *vector;
        rewrite(e, [v, regs, args](const V&) {
            Strs vals;
            for (size_t k = 0; k < args.size(); k++)
                if (std::find(regs.begin(), regs.end(), INTERRUPT_REGISTERS[k]) != regs.end()) vals.push_back(expr(args[k]));
            return "int" + py::hexpad((uint64_t)v, 2) + "_" + (regs.empty() ? std::string("none") : py::join("_", regs)) + "(" +
                   py::join(",", vals) + ")";
        }, true);
    }
    for (auto& np : nodes) {
        const V& e = np.e;
        if (at(e, "node") == "Name" && !field_sym(e)) {
            re::Match m = RES.fullmatch(name_of(e));
            if (m) {
                std::string r = m.group(1);
                rewrite(e, [r](const V&) { return "int_" + r; });
            }
        }
    }
    for (auto& v : now->locals) {
        std::string n = loc_name(v);
        if (RES.fullmatch(n)) now->drop.insert(n);
    }
}

static void p_entry_sp(Nodes& nodes, const std::set<std::string>&) {
    static const re::Regex OWN("\\w*Stack_([0-9a-f]+)"), W("\\w+"), PLACE("stack0x([0-9a-f]{4})"), TY("(\\w+) \\*"),
        NUM("\\d+|0x[0-9a-f]+");
    std::map<int64_t, std::pair<std::string, std::string>> own;
    for (auto& v : now->locals) {
        std::string n = loc_name(v);
        re::Match m = OWN.fullmatch(n);
        std::string t = type_text(v.get("type"));
        if (m && !now->drop.count(n) && W.fullmatch(t)) own[py::int_parse(m.group(1), 16)] = {t, n};
    }
    auto place = [&](const V& u) -> std::optional<int64_t> {
        if (unary(u, "&") && at(u, "x").get("node") == "Name") {
            re::Match m = PLACE.fullmatch(name_of(at(u, "x")));
            if (m) return py::int_parse(m.group(1), 16);
        }
        return std::nullopt;
    };
    IdSet taken;
    if (!own.empty()) {
        for (auto& np : nodes) {
            const V& e = np.e;
            if (at(e, "node") != "Unary" || e.get("op") != "*" || py::truthy(e.get("postfix")) || in_whole(e)) continue;
            const V& c = at(e, "x");
            if (c.get("node") != "Cast" || in_whole(c)) continue;
            re::Match ty = TY.fullmatch(type_text(c.get("type")));
            V v = at(c, "x");
            auto base = place(v);
            int64_t plus = 0;
            if (!base && v.get("node") == "Binary" && v.get("op") == "+" && at(v, "right").get("node") == "Lit" &&
                NUM.fullmatch(lit(at(v, "right")))) {
                base = place(at(v, "left"));
                plus = py::int_parse(lit(at(v, "right")), 0);
                V l = at(v, "left");
                v = l;
            }
            if (!ty || !base || in_whole(v)) continue;
            auto hit = own.find(0x10000 - *base - plus);
            if (hit != own.end()) {
                taken.insert(id(v));
                std::string t = ty.group(1);
                auto h = hit->second;
                now->virtual_[id(e)] = {h.second, h.first == t ? "plain" : "address"};
                rewrite(e, [h, t](const V&) {
                    return h.first == t ? local_text(h.second) : "*(" + pops(t + " *") + ")&" + local_text(h.second);
                });
            }
        }
    }
    for (auto& np : nodes) {
        const V& e = np.e;
        if (at(e, "node") == "Name" && name_of(e) == ENTRY_SP) rewrite(e, [](const V&) { return std::string("codebreaker_entry_sp()"); });
        auto base = place(e);
        if (base && !taken.count(id(e))) {
            int64_t b = *base;
            rewrite(e, [b](const V&) { return "((uint1 *)codebreaker_entry_sp() + (int2)0x" + py::hexpad((uint64_t)b, 4) + ")"; });
        }
    }
}

// (a far pointer's high word as c_text's _far_addresses read it in the text)
static const char* HIGH =
    "(?:\\(\\s*\\w+\\s*\\)\\s*\\(\\s*\\(\\s*uint4\\s*\\)\\s*(\\w+)\\s*>>\\s*0x10\\s*\\)|(\\w+)\\._2_2_|"
    "\\(?\\s*\\*\\s*\\(\\s*u?int2\\s*\\*\\s*\\)\\s*\\(\\s*\\(\\s*char\\s*\\*\\s*\\)\\s*&\\s*(\\w+)\\s*\\+\\s*2\\s*\\)\\s*\\)?)";

static Opt high_of(const std::string& text) {
    static const re::Regex R(HIGH);
    re::Match m = R.fullmatch(text);
    if (!m) return std::nullopt;
    for (int g = 1; g <= 3; g++)
        if (m.has(g) && !m.group(g).empty()) return m.group(g);
    return std::nullopt;                                 // ('' or None ...: None)
}

static std::set<std::string> far_decl_names(const std::vector<std::pair<std::string, V>>& decls) {
    static const re::Regex R("__far\\s*\\*\\s*(\\w+)\\s*[;,)]");
    std::set<std::string> out;
    for (auto& nt : decls)
        for (auto& s : R.findall(type_text(nt.second, nt.first, now->far.count(nt.first) != 0) + ";")) out.insert(s);
    return out;
}

static void p_far_addresses(Nodes& nodes, const std::set<std::string>&) {
    static const re::Regex LOW("\\s*\\(\\s*u?int2\\s*\\)\\s*(?:\\(\\s*void\\s*\\*\\s*\\)\\s*)?(\\w+)\\s*"), W("\\w+");
    ParentMap parent = parents(nodes);
    auto params = params_of(now->doc);
    std::vector<std::pair<std::string, V>> locs;
    for (auto& v : now->locals)
        if (!now->drop.count(loc_name(v))) locs.push_back({loc_name(v), v.get("type")});
    auto all = params;
    all.insert(all.end(), locs.begin(), locs.end());
    std::set<std::string> far_names = far_decl_names(all);
    std::map<std::string, std::string> high_vars;
    std::vector<std::pair<std::string, V>> high_sets;
    for (auto& np : nodes) {
        const V& e = np.e;
        if (at(e, "node") == "ExprStmt" && at(e, "expr").get("node") == "Assign" && !py::truthy(at(e, "expr").get("op")) &&
            at(at(e, "expr"), "target").get("node") == "Name") {
            Opt hp = high_of(expr(at(at(e, "expr"), "value")));
            if (hp) {
                high_sets.push_back({name_of(at(at(e, "expr"), "target")), e});
                if (far_names.count(*hp)) high_vars[name_of(at(at(e, "expr"), "target"))] = *hp;
            }
        }
    }
    std::vector<V> consumed;
    auto own_halves = [&](const V& seg, const V& off) -> Opt {
        std::string s = py::strip(expr(seg));
        Opt p = high_of(s);
        if (!p) {
            auto it = high_vars.find(s);
            if (it != high_vars.end()) p = it->second;
        }
        if (!p || !far_names.count(*p)) return std::nullopt;
        V base = off;
        for (int k = 0; k < 12; k++) {
            if (in_whole(base)) {
                auto gb = now->gap_base.find(id(base));
                if (gb == now->gap_base.end()) break;
                V b = gb->second;
                base = b;
            } else if (base.get("node") == "Paren" || base.get("node") == "Cast") {
                V x = at(base, "x");
                base = x;
            } else if (base.get("node") == "Binary" && (base.get("op") == "+" || base.get("op") == "-")) {
                V l = at(base, "left");
                base = l;
            } else
                break;
        }
        if (base.get("node") == "Name" && shape(base) == "identifier" && name_of(base) == *p) {
            if (high_vars.count(s) && seg.get("node") == "Name") consumed.push_back(seg);
            return p;
        }
        return std::nullopt;
    };
    auto halves_text = [](const V& off, const std::string& p, const std::string& target) {
        std::string sum_ = pointer_cast(off) ? written(at(off, "x"), &off) : expr(off);
        std::string q = re::escape(p);
        sum_ = re::rx("\\(\\s*u?int2\\s*\\)\\s*(?:\\(\\s*void\\s*(?:__far\\s*)?\\*\\s*\\)\\s*)?" + q + "\\b").sub_fn(
            [&](const re::Match&) { return "(uint1 __far *)" + p; }, sum_);
        sum_ = re::rx("\\(\\s*((?:(?!__far)[\\w ])+?)\\s*\\*\\s*\\)\\s*" + q + "\\b").sub_fn(
            [&](const re::Match& m) { return "(" + m.group(1) + " __far *)" + p; }, sum_);
        if (!re::rx("__far \\*\\)\\s*" + q + "\\b").search(sum_))
            sum_ = re::rx("\\b" + q + "\\b").sub_fn([&](const re::Match&) { return "(uint1 __far *)" + p; }, sum_, 1);
        return "((" + (target == "void" || target == "code" ? std::string("uint1") : target) + " __far *)(" + sum_ + "))";
    };
    for (auto& np : nodes) {
        const V& e = np.e;
        auto parts = mk_fp(e);
        if (!parts || in_whole(e)) continue;
        V seg = parts->first, off = parts->second;
        Opt hp = own_halves(seg, off);
        if (hp) {
            Opt pc = pointer_cast(off);
            if (!pc) pc = pointer_cast(leftmost(off));
            std::string target = pc && !pc->empty() ? *pc : "uint1";
            std::string h = *hp;
            rewrite(e, [off, h, target, halves_text](const V&) { return halves_text(off, h, target); }, true);
            continue;
        }
        auto ft = far_target(off);
        std::string target = ft.first;
        V offset = ft.second;
        const V& up = np.p;
        if (!up.is_none() && unary(up, "*")) {
            const V& call = parent_of(parent, up);
            if (!call.is_none() && call.get("node") == "Call" && same(call.get("target"), up)) target = "code";
        }
        rewrite(e, [seg, offset, target, off](const V&) {
            std::string s = expr(seg);                     // (left to right, as the Python's % tuple)
            std::string o = written(offset, same(offset, off) ? nullptr : &off);
            return "((" + pops(target + " __far *") + ")MK_FP(" + s + ", " + o + "))";
        }, true);
    }
    IdSet merged;
    for (auto& np : nodes) {
        const V& e = np.e;
        if (at(e, "node") != "Call" || in_whole(e) || dict_or_empty(e.get("target")).get("node") != "Name") continue;
        V up = parent_of(parent, e);
        bool inside = false;
        while (!up.is_none()) {
            if (merged.count(id(up))) {
                inside = true;
                break;
            }
            V q = parent_of(parent, up);
            up = q;
        }
        std::vector<V> args = stmts(py::truthy(e.get("args")) ? e.get("args") : V::list());
        if (inside || args.size() < 2) continue;
        std::vector<std::pair<V, Opt>> nw;
        size_t k = 0;
        bool changed = false;
        while (k < args.size()) {
            re::Match lo;
            if (k + 1 < args.size()) lo = LOW.fullmatch(expr(args[k]));
            std::string hi_text = lo ? py::strip(expr(args[k + 1])) : std::string();
            Opt hp;
            if (lo) {
                hp = high_of(hi_text);
                if (!hp) {
                    auto it = high_vars.find(hi_text);
                    if (it != high_vars.end()) hp = it->second;
                }
            }
            if (lo && far_names.count(lo.group(1)) && hp && *hp == lo.group(1)) {
                nw.push_back({V(), std::string(k ? " " : "") + lo.group(1)});
                if (high_vars.count(hi_text) && args[k + 1].get("node") == "Name") consumed.push_back(args[k + 1]);
                k += 2;
                changed = true;
            } else {
                nw.push_back({args[k], std::nullopt});
                k += 1;
            }
        }
        if (changed) {
            merged.insert(id(e));
            rewrite(e, [nw](const V& c) {
                Strs parts;
                for (auto& at_ : nw) parts.push_back(at_.first.is_none() ? *at_.second : expr(at_.first));
                return name_of(at(c, "target")) + "(" + py::join(",", parts) + ")";
            });
        }
    }
    std::map<std::string, int> uses;
    for (auto& np : nodes)
        if (at(np.e, "node") == "Name") uses[name_of(np.e)] += 1;
    for (auto& x : consumed) uses[name_of(x)] -= 1;
    std::map<std::string, V> locd;
    for (auto& nt : locs) locd[nt.first] = nt.second;
    for (auto& hv : high_vars) {
        const std::string& v = hv.first;
        if ((uses.count(v) ? uses[v] : 0) == 1 && locd.count(v)) {
            for (auto& ns : high_sets)
                if (ns.first == v) now->gone.insert(id(ns.second));
            const V& t = locd[v];
            if (!t.is_none() && W.fullmatch(type_text(t))) now->drop.insert(v);
        }
    }
}

static Opt first_name(const V& e0) {
    static const re::Regex R("([A-Za-z_]\\w*)\\b");
    V e = e0;
    while (true) {
        auto it = now->whole.find(id(e));
        if (it != now->whole.end()) {
            re::Match m = R.match(it->second(e));
            return m ? Opt(m.group(1)) : std::nullopt;
        }
        std::string n = node_kind(e);
        if (n == "Name") return mark(e) != "_ss" ? Opt(name_of(e)) : std::nullopt;
        const char* first = n == "Binary" ? "left" : n == "Assign" ? "target" : n == "Call" ? "target" : n == "Index" ? "arr" : n == "Select" ? "obj" : nullptr;
        if (n == "Unary" && py::truthy(e.get("postfix"))) first = "x";
        if (!first || !py::truthy(e.get(first))) return std::nullopt;
        V child = e.at(first);
        if (n == "Select" && arrow(e)) {
            V x = at(child, "x");
            child = x;
        }
        if (bracket(token(e), child, 0)) return std::nullopt;
        e = child;
    }
}

static bool ends_with_semicolon(const V& e, const ParentMap& parent) {
    const V& p = parent_of(parent, e);
    if (!p.is_none() && p.get("node") == "ExprStmt") {
        const V& pp = parent_of(parent, p);
        if (!pp.is_none() && pp.get("node") == "For") {
            for (auto& x : stmts(pp.get("update")))
                if (same(x, p)) return false;
        }
        return true;
    }
    if (!p.is_none() && p.get("node") == "For")
        for (auto& x : stmts(p.get("init")))
            if (same(x, e)) return true;
    return false;
}

static void p_far_casts(Nodes& nodes, const std::set<std::string>&) {
    static const re::Regex NEAR("(?:(?!__far)[\\w ])+?\\s*\\*");
    auto params = params_of(now->doc);
    for (auto& v : now->locals)
        if (!now->drop.count(loc_name(v))) params.push_back({loc_name(v), v.get("type")});
    std::set<std::string> names = far_decl_names(params);
    if (names.empty()) return;
    ParentMap parent = parents(nodes);
    for (auto& np : nodes) {
        const V& e = np.e;
        if (at(e, "node") != "Cast" || in_whole(e) || !mark(e).empty()) continue;
        if (NEAR.fullmatch(type_text(e.get("type"))) && !bracket(token(e), at(e, "x"), 1)) {
            Opt fn = first_name(at(e, "x"));
            if (fn && names.count(*fn)) now->marks[id(e)] = "_far";
        }
    }
    for (auto& np : nodes) {
        const V& e = np.e;
        if (at(e, "node") == "Call" && !in_whole(e) && name_of(dict_or_empty(e.get("target"))) == "CONCAT22" &&
            (py::truthy(e.get("args")) ? e.get("args").size() : 0) == 2) {
            std::string hi = expr(e.get("args")[0]), lo = expr(e.get("args")[1]);
            for (auto& p : names) {
                std::string q = re::escape(p);
                std::string high =
                    "(?:\\(\\s*\\*\\s*\\(\\s*u?int2\\s*(?:__far\\s*)?\\*\\s*\\)\\s*\\(\\s*\\(\\s*char\\s*(?:__far\\s*)?\\*\\s*\\)\\s*&\\s*" + q +
                    "\\s*\\+\\s*2\\s*\\)\\s*\\)|\\(\\s*u?int2\\s*\\)\\s*\\(\\s*\\(\\s*uint4\\s*\\)\\s*" + q + "\\s*>>\\s*0x10\\s*\\)|" + q +
                    "\\._2_2_)";
                if (re::rx("\\s*" + high + "\\s*").fullmatch(hi) &&
                    re::rx("\\s*(?:\\(\\s*void\\s*__far\\s*\\*\\s*\\)\\s*)?" + q + "\\s*").fullmatch(lo)) {
                    std::string pp = p;
                    rewrite(e, [pp](const V&) { return "(int4)" + pp; });
                    break;
                }
            }
        }
        if (at(e, "node") == "Assign" && !py::truthy(e.get("op")) && !in_whole(e) && ends_with_semicolon(e, parent)) {
            std::string target = expr(at(e, "target")), value = expr(at(e, "value"));
            for (auto& p : names) {
                std::string q = re::escape(p);
                std::string low = "\\(\\s*\\*\\s*\\(\\s*u?int2\\s*(?:__far\\s*)?\\*\\s*\\)\\s*\\(\\s*\\(\\s*char\\s*(?:__far\\s*)?\\*\\s*\\)\\s*&\\s*" +
                                  q + "\\s*\\+\\s*0\\s*\\)\\s*\\)";
                re::Match off = re::rx("\\(\\s*void\\s*(?:__far\\s*)?\\*\\s*\\)\\s*\\(\\s*\\(\\s*u?int2\\s*\\)\\s*(?:\\(\\s*void\\s*__far\\s*\\*\\s*\\)\\s*)?" +
                                       q + "\\s*([+-])\\s*(0x[0-9a-fA-F]+|\\d+)\\s*\\)")
                                   .fullmatch(value);
                if (off && re::rx(low).fullmatch(target)) {
                    std::string pp = p, o1 = off.group(1), o2 = off.group(2);
                    rewrite(e, [pp, o1, o2](const V&) { return pp + " = (uint1 __far *)" + pp + " " + o1 + " " + o2; });
                    break;
                }
            }
        }
    }
}

static void p_stack_arguments(Nodes& nodes, const std::set<std::string>&) {
    static const re::Regex IN("in_stack_([0-9a-f]{8})"), PIECE("_?([a-z]+)Stack([0-9a-f]{4})");
    std::map<std::string, std::pair<std::string, int64_t>> places;
    for (auto& v : now->locals) {
        std::string n = loc_name(v);
        re::Match m = IN.fullmatch(n);
        if (m && !now->drop.count(n) && py::int_parse(m.group(1), 16) < 0x10000 && type_stack(v.get("type")).mods.empty() &&
            !places.count(n)) {
            places[n] = {type_text(v.get("type")), py::int_parse(m.group(1), 16)};
            now->drop.insert(n);
        }
    }
    std::set<std::string> declared;
    for (auto& v : now->locals) {
        V nv = loc_name_v(v);
        declared.insert(nv.is_none() ? std::string("\x01None") : py::str_of(nv));
    }
    for (auto& d : now->drop) declared.erase(d);
    for (auto& np : nodes) {
        const V& e = np.e;
        if (at(e, "node") != "Name" || field_sym(e)) continue;
        std::string n = name_of(e);
        auto pl = places.find(n);
        if (pl != places.end()) {
            auto p = pl->second;
            rewrite(e, [p](const V&) { return "codebreaker_stack(" + p.first + ", 0x" + pyx(p.second) + ")"; });
            continue;
        }
        re::Match m = PIECE.fullmatch(n);
        if (m && !declared.count(n)) {
            char c = m.group(1)[0];
            std::string ty = c == 'u' ? "uint2" : c == 'i' ? "int2" : c == 'c' ? "char" : c == 'b' ? "uint1" : c == 'x' ? "xunknown2" : "uint2";
            int64_t o = py::int_parse(m.group(2), 16);
            rewrite(e, [ty, o](const V&) { return "codebreaker_stack(" + ty + ", 0x" + pyx(o) + ")"; });
        }
    }
}

static V leading_cast_now(const V& e0) {
    V e = e0;
    while (true) {
        if (in_whole(e)) return V();
        std::string n = node_kind(e);
        if (n == "Cast") return e;
        const char* first = n == "Binary" ? "left" : n == "Assign" ? "target" : n == "Cond" ? "cond" : n == "Call" ? "target"
                          : n == "Index" ? "arr" : n == "Select" ? "obj" : nullptr;
        if (n == "Unary" && py::truthy(e.get("postfix"))) first = "x";
        if (!first || !py::truthy(e.get(first))) return V();
        V child = e.at(first);
        if (n == "Select" && arrow(e)) {
            V x = at(child, "x");
            child = x;
        }
        if (bracket(token(e), child, 0)) return V();
        e = child;
    }
}

static bool name_is(const V& e, const std::string& n) {
    // _name(e) == n, without making the name (the passes ask it of every node, for every local)
    const V& sym = e.get("sym");
    if (py::truthy(sym)) {
        const V& s = sym.get("name");
        if (py::truthy(s)) return s.is_str() ? s.str() == n : py::str_of(s) == n;
    }
    const V& n2 = e.get("name");
    if (py::truthy(n2)) return n2.is_str() ? n2.str() == n : py::str_of(n2) == n;
    return n == "?";
}

static bool bare_name(const V& e, const Opt& name = std::nullopt) {
    // (the name first: it rules out nearly every node, and every test here is free of side effects)
    return e.get("node") == "Name" && (!name || name_is(e, *name)) && !field_sym(e) && !in_whole(e) && mark(e) != "_ss";
}

static void p_far_locals(Nodes& nodes, const std::set<std::string>&) {
    static const re::Regex SETR("([\\w ]+?) \\*"), PTRDECL("([\\w ]+?) \\*(\\w+)"), LINE("([\\w ]+?[\\w ]*\\*+)\\s*(\\w+)");
    IdSet seen = seen_now();
    ParentMap parent = parents(nodes);
    Nodes shown;
    for (auto& np : nodes)
        if (seen.count(id(np.e))) shown.push_back(np);
    auto& decls = now->decls;
    std::vector<V> locs = live_locals();
    auto decl_text = [&](const V& v) -> Opt {
        std::string n = loc_name(v);
        auto it = decls.find(n);
        if (it != decls.end()) return it->second;
        if (segment_register(n, v.get("type"))) return std::nullopt;
        return type_text(v.get("type"), n, now->far.count(n) != 0);
    };
    auto sets_of = [&](const std::string& name, bool concat) {
        std::vector<std::pair<V, std::string>> out;
        for (auto& np : shown) {
            const V& e = np.e;
            if (at(e, "node") != "Assign" || py::truthy(e.get("op")) || in_whole(e) || !bare_name(at(e, "target"), name)) continue;
            V c = leading_cast_now(at(e, "value"));
            re::Match m;
            if (!c.is_none()) m = SETR.fullmatch(cast_type(c));
            V v = m ? at(c, "x") : V();
            if (!v.is_none() && (!concat || (v.get("node") == "Call" && !in_whole(v) && bare_name(dict_or_empty(v.get("target")), std::string("CONCAT22")))))
                out.push_back({c, m.group(1)});
        }
        return out;
    };
    auto far_casts = [&](const std::vector<std::pair<V, std::string>>& found) {
        for (auto& ct : found) {
            std::string mk = mark(ct.first);
            if (mk == "_far" || mk == "_ds") {
                std::string to = ct.second;
                rewrite(ct.first, [to](const V& c) { return "(" + pops(to + " __far *") + ")" + written(at(c, "x"), &c); });
            } else
                now->marks[id(ct.first)] = "_far";
        }
    };
    for (auto& v : locs) {
        std::string n = loc_name(v);
        Opt d = decl_text(v);
        bool wide = false;
        for (const char* w : {"xunknown4", "uint4", "int4", "undefined4"})
            if (d && *d == std::string(w) + " " + n) wide = true;
        if (!wide) continue;
        bool through = false;
        for (auto& np : shown)
            if (bare_name(np.e, n) && unary_parent(np.e, parent, "*") && !arrow_of(parent_of(parent, np.e), parent)) {
                through = true;
                break;
            }
        if (!through) continue;
        auto found = sets_of(n, false);
        decls[n] = (found.empty() ? std::string("xunknown1") : found[0].second) + " __far *" + n;
        far_casts(found);
    }
    std::set<std::string> declared;
    for (auto& v : locs) declared.insert(loc_name(v));
    for (auto& v : locs) {
        std::string n = loc_name(v);
        Opt d = decl_text(v);
        re::Match m = PTRDECL.fullmatch(d ? *d : std::string());
        std::string whole = "_" + n;
        if (!m || m.group(2) != n || declared.count(whole)) continue;
        bool any = false;
        for (auto& np : shown)
            if (bare_name(np.e, whole)) {
                any = true;
                break;
            }
        if (!any) continue;
        std::string to = m.group(1);
        bool assigned = false;
        for (auto& np : shown) {
            const V& e = np.e;
            const V& p = np.p;
            if (bare_name(e, n) && !p.is_none() &&
                ((p.get("node") == "Unary" && p.get("op") == "&" && !py::truthy(p.get("postfix")) && !in_whole(p)) ||
                 (p.get("node") == "Index" && same(at(p, "arr"), e) && !in_whole(p) && unary_parent(p, parent, "&")))) {
                assigned = true;
                break;
            }
        }
        if (!assigned)
            for (auto& np : shown) {
                const V& e = np.e;
                if (at(e, "node") == "Assign" && !py::truthy(e.get("op")) && !in_whole(e) && bare_name(at(e, "target"), n)) {
                    assigned = true;
                    break;
                }
            }
        decls[n] = (assigned ? *d + ";\n  " : std::string()) + to + " __far *" + whole;
        far_casts(sets_of(whole, true));
        if (!assigned)
            for (auto& np : shown)
                if (bare_name(np.e, n)) rewrite(np.e, [to, whole](const V&) { return "((" + pops(to + " *") + ")(uint2)(uint4)" + whole + ")"; });
    }
    std::map<std::string, std::string> types;
    for (auto& v : locs) {
        Opt d = decl_text(v);
        for (auto& line : py::split(d ? *d : std::string(), ";\n  ")) {
            re::Match m = LINE.fullmatch(line);
            if (m) types[m.group(2)] = py::strip(m.group(1));
        }
    }
    for (auto& np : shown) {
        const V& e = np.e;
        if (at(e, "node") != "ExprStmt" || !now->lines.count(id(e))) continue;
        const V& a = at(e, "expr");
        if (a.get("node") != "Assign" || py::truthy(a.get("op")) || in_whole(a)) continue;
        const V &left = at(a, "target"), &right = at(a, "value");
        if (bare_name(left) && bare_name(right) && types.count(name_of(left)) && types.count(name_of(right)) &&
            types[name_of(left)] != types[name_of(right)]) {
            std::string t = types[name_of(left)];
            rewrite(right, [t](const V& x) { return "(" + t + ")" + name_of(x); });
        }
    }
}

static void p_callee_pops(Nodes&, const std::set<std::string>&) {
    if (py::truthy(now->facts.get("callee_pops"))) now->pops = true;
}

static std::vector<std::pair<std::string, std::pair<int, int>>> named_functions(const Nodes& nodes) {
    std::vector<std::pair<std::string, std::pair<int, int>>> out;
    auto slot = [&](const std::string& n) -> std::pair<int, int>& {
        for (auto& x : out)
            if (x.first == n) return x.second;
        out.push_back({n, {0, 0}});
        return out.back().second;
    };
    for (auto& np : nodes) {
        const V& e = np.e;
        if (at(e, "node") == "Name" && !field_sym(e)) slot(name_of(e)).first += 1;
        if (at(e, "node") == "Call" && !py::truthy(e.get("args"))) {
            const V& t = e.get("target");
            V name;
            if (!t.is_none() && t.get("node") == "Name") name = V(name_of(t));
            else if (t.is_none()) name = sym_name_v(e);
            if (py::truthy(name)) {
                auto& n = slot(py::str_of(name));
                n.second += 1;
                if (t.is_none()) n.first += 1;
            }
        }
    }
    return out;
}

static void p_caller_frame_slots(Nodes& nodes, const std::set<std::string>&) {
    static const re::Regex LOCAL("(?m)^\\s+([\\w ]+?\\**) ?(\\w*Stack_([0-9a-f]+));$"),
        ARR("(?m)^\\s+[\\w ]+?\\**\\s*\\w*Stack_[0-9a-f]+ \\[\\d+\\];"), CDECL16("__cdecl16(?:near|far)"),
        BPDECL("(?m)^\\s+int2 unaff_BP;$"), SLOT("\\*\\((\\w+) \\*\\)\\(unaff_BP \\+ -(0x[0-9a-f]+|\\d+)\\)");
    V facts = now->facts;
    const V& doc = now->doc;
    std::vector<V> locs = live_locals();
    IdSet seen = seen_now();
    std::string region;
    if (now->locals_text) region = *now->locals_text;
    else {
        for (auto& v : locs) region += "\n  " + decl_now(v) + ";";
        if (!locs.empty()) region += "\n  ";
    }
    V own = V::dict();
    for (auto& m : LOCAL.finditer(region)) own.set(V(py::int_parse(m.group(3), 16)), V::tuple({V(m.group(1)), V(m.group(2))}));
    bool bp = region.find("unaff_BP") != std::string::npos;
    if (!bp)
        for (auto& np : nodes)
            if (seen.count(id(np.e)) && at(np.e, "node") == "Name" && name_of(np.e) == "unaff_BP") {
                bp = true;
                break;
            }
    Nodes seen_nodes;
    for (auto& np : nodes)
        if (seen.count(id(np.e))) seen_nodes.push_back(np);
    V named = V::dict();
    for (auto& kv : named_functions(seen_nodes)) named.set(V(kv.first), V::tuple({V(kv.second.first), V(kv.second.second)}));
    V report = V::dict();
    report.set("locals", own);
    report.set("arrays", V((bool)ARR.search(region)));
    report.set("bp", V(bp));
    report.set("named", named);
    report.set("region", V(region));
    Heading h = heading(doc);
    Strs heads = h.mods;
    heads.insert(heads.end(), h.words.begin(), h.words.end());
    V slots;
    struct Place {
        V e;
        std::string ty;
        int64_t less;
    };
    std::vector<Place> places;
    const V& sym = doc.get("sym");
    V ft = py::truthy(sym) ? dict_or_empty(sym.get("type")) : V::dict();
    if (!heads.empty() && CDECL16.fullmatch(heads.back()) && !py::truthy(doc.get("params")) && !py::truthy(ft.get("variadic")) &&
        BPDECL.search(region)) {
        int uses = 0;
        for (auto& np : nodes)
            if (seen.count(id(np.e)) && at(np.e, "node") == "Name" && name_of(np.e) == "unaff_BP") uses++;
        for (auto& np : nodes) {
            const V& e = np.e;
            if (seen.count(id(e)) && at(e, "node") == "Unary" && e.get("op") == "*" && !py::truthy(e.get("postfix"))) {
                re::Match m = SLOT.fullmatch(expr(e));
                if (m) places.push_back({e, m.group(1), py::int_parse(m.group(2), 0)});
            }
        }
        V found = V::dict();
        for (auto& pl : places) found.setdefault(V(pl.less), V(pl.ty));
        bool all_sized = true;
        for (auto& kv : found.items()) all_sized = all_sized && SIZEOF.count(kv.second.str());
        if (py::truthy(found) && uses == (int)places.size() && all_sized) slots = found;
    }
    report.set("slots", slots);
    facts.set("frame", report);
    V given = facts.get("frame_slots");
    if (py::truthy(given) && py::truthy(slots)) {
        std::vector<std::pair<std::string, std::string>> old;
        V sorted_given = py::sorted(given);
        for (auto& less : sorted_given.seq()) {
            std::string k = pyx(less.as_int());
            old.push_back({"frame_" + k, py::str_of(given.get(less)) + " __far *frame_" + k});
        }
        now->old_style = old;
        for (auto& pl : places) {
            std::string t = pl.ty;
            int64_t k = pl.less;
            V gv = given;
            rewrite(pl.e, [t, k, gv](const V&) {
                if (gv.at(V(k)) == V(t)) return "*frame_" + pyx(k);
                return "*(" + t + " __far *)frame_" + pyx(k);
            });
        }
        if (now->locals_text) {
            static const re::Regex R("(?m)^\\s+int2 unaff_BP;\\n");
            now->locals_text = R.sub("", *now->locals_text, 1);
        } else
            now->drop.insert("unaff_BP");
    }
    V calls = dict_or_empty(facts.get("frame_calls"));
    if (py::truthy(calls)) {
        for (auto& np : nodes) {
            const V& e = np.e;
            if (at(e, "node") != "Call" || py::truthy(e.get("args")) || in_whole(e)) continue;
            const V& t = e.get("target");
            V name;
            if (!t.is_none() && t.get("node") == "Name") name = V(name_of(t));
            else if (t.is_none()) name = sym_name_v(e);
            if (!name.is_none() && calls.has(name)) {
                std::string n = py::str_of(name), a = py::str_of(calls.get(name));
                rewrite(e, [n, a](const V&) { return n + "(" + a + ")"; });
            }
        }
        now->declare_first.clear();
        const V& fd = facts.get("frame_declare");
        if (py::truthy(fd))
            for (auto& d : fd.seq()) now->declare_first.push_back({py::str_of(d[0]), py::str_of(d[1])});
    }
}

static void p_selector_imports(Nodes& nodes, const std::set<std::string>&) {
    static const re::Regex SV("0x[dD]0[0-9a-fA-F]{2}");
    V values = now->facts.get("selector_values");            // (a copy: the facts grow below)
    if (!py::truthy(values)) return;
    IdSet seen = seen_now();
    V used = now->facts.setdefault(V("selector_imports"), V::dict());
    for (auto& np : nodes) {
        const V& e = np.e;
        if (at(e, "node") != "Lit" || !seen.count(id(e)) || in_whole(e)) continue;
        std::string spelled = lit(e);
        if (SV.fullmatch(spelled) && values.has(V(py::int_parse(spelled, 16)))) {
            const V& iv = values.get(V(py::int_parse(spelled, 16)));
            V imp = iv[0], how = iv[1];
            used.set(imp, how);
            std::string s = std::string(how == "segment" ? "FP_SEG" : "FP_OFF") + "(&" + py::str_of(imp) + ")";
            rewrite(e, [s](const V&) { return s; });
        }
    }
}

static void p_segment_values(Nodes& nodes, const std::set<std::string>&) {
    static const re::Regex FO("(-?)0x([0-9a-f]+)");
    const V& values = now->facts.get("segment_values");
    if (!py::truthy(values)) return;
    IdSet seen = seen_now();
    for (auto& np : nodes) {
        const V& e = np.e;
        if (at(e, "node") == "Lit" && seen.count(id(e)) && !in_whole(e) && values.has(V(lit(e)))) {
            std::string s = py::str_of(values.get(V(lit(e))));
            rewrite(e, [s](const V&) { return s; });
        }
    }
    V by_expr = dict_or_empty(now->facts.get("segment_of"));
    V places = dict_or_empty(now->facts.get("segment_places"));
    for (auto& np : nodes) {
        const V& e = np.e;
        std::vector<V> args;
        if (at(e, "node") == "Call" && py::truthy(e.get("args")))
            for (auto& x : e.get("args").seq())
                if (seen.count(id(x))) args.push_back(x);
        for (size_t k = 0; k + 1 < args.size(); k++) {
            const V &a = args[k], &b = args[k + 1];
            const V& sg = by_expr.get(V(py::strip(expr(b))));
            if (sg.is_none() || in_whole(a) || !(at(a, "node") == "Lit" || (at(a, "node") == "Unary" && a.get("op") == "-" && !py::truthy(a.get("postfix")))))
                continue;
            re::Match l = FO.fullmatch(py::replace(expr(a), " ", ""));
            if (!l) continue;
            int64_t v = py::int_parse(l.group(2), 16);
            if (!l.group(1).empty()) v = -v;
            const V& fn = places.get(V::tuple({sg, V(v & 0xFFFF)}));
            if (py::truthy(fn)) {
                std::string f = py::str_of(fn);
                rewrite(a, [f](const V&) { return "((uint2)(uint4)(void __far *)&" + f + ")"; });
            }
        }
    }
}

// (the rest of the passes: cpasses2.cc)
void p_code_segment_reads(Nodes& nodes, const std::set<std::string>& arrays);
void p_names_used(Nodes& nodes, const std::set<std::string>& arrays);
void p_heading_facts(Nodes& nodes, const std::set<std::string>& arrays);
void p_far_concat_casts(Nodes& nodes, const std::set<std::string>& arrays);
void p_buffer_fields(Nodes& nodes, const std::set<std::string>& arrays);
void p_far_reads(Nodes& nodes, const std::set<std::string>& arrays);
void p_near_to_far(Nodes& nodes, const std::set<std::string>& arrays);
void p_stack_segment_stores(Nodes& nodes, const std::set<std::string>& arrays);
void p_code_pointers(Nodes& nodes, const std::set<std::string>& arrays);
void p_compare_as_numbers(Nodes& nodes, const std::set<std::string>& arrays);
void p_keep_frame(Nodes& nodes, const std::set<std::string>& arrays);
void p_globals_through(Nodes& nodes, const std::set<std::string>& arrays);
void p_same_cast(Nodes& nodes, const std::set<std::string>& arrays);
void p_fit_calls(Nodes& nodes, const std::set<std::string>& arrays);
void p_struct_fields(Nodes& nodes, const std::set<std::string>& arrays);
void p_wide_address(Nodes& nodes, const std::set<std::string>& arrays);
void p_homed_arguments(Nodes& nodes, const std::set<std::string>& arrays);

// (exported to cpasses2.cc)
bool x_unary_as_read(const V& u, const char* op, const std::unordered_map<Id, V>& parent) { return unary_as_read(u, op, parent); }
bool x_star_as_read(const V& u, const std::unordered_map<Id, V>& parent) { return star_as_read(u, parent); }
bool x_arrow_of(const V& u, const std::unordered_map<Id, V>& parent) { return arrow_of(u, parent); }
bool x_unary_parent(const V& e, const std::unordered_map<Id, V>& parent, const char* op) { return unary_parent(e, parent, op); }
bool x_bare_name(const V& e, const Opt& name) { return bare_name(e, name); }
std::string x_written(const V& e, const V* under) { return written(e, under); }
bool x_near_number(const std::string& inner) { return near_number(inner); }
std::string x_shape(const V& e) { return shape(e); }
std::optional<std::pair<V, V>> x_mk_fp(const V& e) { return mk_fp(e); }
bool x_ends_with_semicolon(const V& e, const std::unordered_map<Id, V>& parent) { return ends_with_semicolon(e, parent); }
std::map<std::string, std::string> x_pointer_distances(const V& doc) { return pointer_distances(doc); }

#ifdef CBL2_PROFILE
static std::mutex prof_mu;
static std::map<std::string, double> prof;
void prof_add(const std::string& k, double t) {
    std::lock_guard<std::mutex> lock(prof_mu);
    prof[k] += t;
}
struct ProfDump {
    ~ProfDump() {
        FILE* f = std::fopen("cbl2_profile.txt", "w");
        if (!f) return;
        for (auto& kv : prof) std::fprintf(f, "%-24s %8.3f\n", kv.first.c_str(), kv.second);
        std::fclose(f);
    }
} prof_dump;
#endif

void run_passes(Nodes& nodes, const std::set<std::string>& arrays) {
    using P = void (*)(Nodes&, const std::set<std::string>&);
    static const std::vector<std::pair<const char*, P>> passes{
        {"wide_address", p_wide_address},
        {"frame_through_ss", p_frame_through_ss},
        {"pieces", p_pieces},
        {"joined_arguments", p_joined_arguments},
        {"null_is_zero", p_null_is_zero},
        {"global_pointer_steps", p_global_pointer_steps},
        {"far_arguments", p_far_arguments},
        {"one_name_for_globals", p_one_name_for_globals},
        {"selector_structs", p_selector_structs},
        {"struct_gaps", p_struct_gaps},
        {"own_segment_calls", p_own_segment_calls},
        {"interrupts", p_interrupts},
        {"far_addresses", p_far_addresses},
        {"far_casts", p_far_casts},
        {"entry_sp", p_entry_sp},
        {"stack_arguments", p_stack_arguments},
        {"far_locals", p_far_locals},
        {"far_concat_casts", p_far_concat_casts},
        {"buffer_fields", p_buffer_fields},
        {"far_reads", p_far_reads},
        {"near_to_far", p_near_to_far},
        {"stack_segment_stores", p_stack_segment_stores},
        {"callee_pops", p_callee_pops},
        {"caller_frame_slots", p_caller_frame_slots},
        {"selector_imports", p_selector_imports},
        {"segment_values", p_segment_values},
        {"code_segment_reads", p_code_segment_reads},
        {"names_used", p_names_used},
        {"heading_facts", p_heading_facts},
        {"code_pointers", p_code_pointers},
        {"compare_as_numbers", p_compare_as_numbers},
        {"keep_frame", p_keep_frame},
        {"globals_through", p_globals_through},
        {"same_cast", p_same_cast},
        {"fit_calls", p_fit_calls},
        {"struct_fields", p_struct_fields},
        {"homed_arguments", p_homed_arguments}};
    for (auto& np : passes)
        if (now->has(np.first)) {
#ifdef CBL2_PROFILE
            auto t0 = std::chrono::steady_clock::now();
            np.second(nodes, arrays);
            prof_add(np.first, std::chrono::duration<double>(std::chrono::steady_clock::now() - t0).count());
#else
            np.second(nodes, arrays);
#endif
        }
}

// ---------------------------------------------------------------------------------------------- near numbers (_near_to_far's test)
static std::set<std::string> wide_names() {
    static const re::Regex R("(?m)^[ \\t]*(int4|uint4|xunknown4|undefined4|float4) (\\w+)(?: \\[\\d*\\])?(?: = [^;\\n]*)?;");
    std::string text;
    if (now->locals_text) text = *now->locals_text;
    else
        for (auto& v : now->locals)
            if (!now->drop.count(loc_name(v))) text += "\n  " + decl_now(v) + ";";
    if (now->old_style)
        for (auto& nd : *now->old_style) text += "\n" + nd.second + ";";
    std::set<std::string> out;
    for (auto& m : R.finditer(text)) out.insert(m.group(2));
    return out;
}

static bool near_number(const std::string& inner) {
    static const re::Regex WIDE("\\b(?:int4|uint4|xunknown4|undefined4)\\b"), ZERO("\\(\\s*0\\s*\\)");
    if (inner.find("__far") != std::string::npos || inner.find("MK_FP") != std::string::npos || inner.find("CONCAT") != std::string::npos ||
        inner.find('&') != std::string::npos || WIDE.search(inner) || ZERO.fullmatch(inner))
        return false;
    for (auto& w : wide_names())
        if (re::rx("\\b" + re::escape(w) + "\\b").search(inner)) return false;
    return true;
}

}  // namespace cb
