// cback.cc -- backends/c.py's writer: types, expressions (each through the reverse-Polish stack),
// conditions, statements, the heading and the function (PrintC's docFunction).
#include "cback.hpp"

#include <algorithm>

namespace cb {

const char* ENTRY_SP = "register0x00000010";

// ---------------------------------------------------------------------------------------------- helpers
V sym_name_v(const V& e) {
    const V& sym = e.get("sym");
    if (!py::truthy(sym)) return V();
    return sym.get("name");
}
std::string sym_name(const V& e) {
    V n = sym_name_v(e);
    return n.is_str() ? n.str() : std::string();
}
std::string name_of(const V& e) {
    const V& sym = e.get("sym");
    if (py::truthy(sym)) {
        const V& n = sym.get("name");
        if (py::truthy(n)) return py::str_of(n);
    }
    const V& n2 = e.get("name");
    if (py::truthy(n2)) return py::str_of(n2);
    return "?";
}
std::string param_name(const V& p) {
    V n = sym_name_v(p);
    if (py::truthy(n)) return py::str_of(n);
    if (p.has("name")) {
        const V& x = p.get("name");
        return x.is_none() ? std::string() : py::str_of(x);
    }
    return "";
}
std::vector<V> stmts(const V& s) {
    if (s.is_none()) return {};
    if (s.is_list()) return s.seq();
    return {s};
}
bool same(const V& a, const V& b) {
    if (a.h || b.h) return a.h == b.h;
    return a.k == b.k && a.k == py::K::None;
}
bool in(const std::set<std::string>& s, const std::string& x) { return s.count(x) != 0; }
bool in(const IdSet& s, Id x) { return s.count(x) != 0; }

std::string fmt(const char* f, std::initializer_list<std::string> args) {
    std::string out;
    auto it = args.begin();
    for (const char* p = f; *p; p++) {
        if (p[0] == '%' && p[1] == 's') {
            out += *it++;
            p++;
        } else if (p[0] == '%' && p[1] == '%') {
            out += '%';
            p++;
        } else
            out += *p;
    }
    return out;
}

static std::string mark_of(const V& e) {
    auto it = now->marks.find(id(e));
    return it == now->marks.end() ? std::string() : it->second;
}

Flat* make_flat() {
    now->keep.push_back(std::make_shared<Flat>());
    return now->keep.back().get();
}

std::string flat_text(const std::function<void(Em*)>& put_) {
    if (!now->seeing) {                                  // (kept only while a layout records where things are)
        Flat local;
        put_(&local);
        return local.text();
    }
    Flat* em = make_flat();
    if (now->seeing && !now->owners.empty()) now->owner_of[em] = now->owners.back();
    put_(em);
    return em->text();
}

// ---------------------------------------------------------------------------------------------- types
static const std::map<std::string, std::string>& prim_names() {
    static const std::map<std::string, std::string> m{{"i8", "int1"},   {"u8", "uint1"},   {"i16", "int2"}, {"u16", "uint2"},
                                                      {"i32", "int4"},  {"u32", "uint4"},  {"i64", "int8"}, {"u64", "uint8"},
                                                      {"f32", "float4"}, {"f64", "float8"}, {"f80", "float10"},
                                                      {"bool", "bool"}, {"char", "char"},  {"void", "void"}};
    return m;
}

static std::string record_text(const V& t) {
    Strs fields;
    const V& fs = t.get("fields");
    if (!fs.is_none())
        for (auto& f : fs.seq()) {
            std::string name;
            if (f.has("name")) name = f.get("name").is_none() ? std::string() : py::str_of(f.get("name"));
            else name = "_f" + pyx(f.has("offset") ? f.get("offset").as_int() : 0);
            fields.push_back(type_text(f.at("type"), name) + ";");
        }
    return std::string(py::truthy(t.get("packed")) ? "_Packed " : "") + "struct { " + py::join(" ", fields) + " }";
}

TypeStack type_stack(const V& t0) {
    TypeStack r;
    V t = t0;
    while (true) {
        if (t.is_none()) {
            r.base = "void";
            return r;
        }
        const V& kv = t.at("kind");
        std::string k = kv.is_str() ? kv.str() : py::repr(kv);
        auto it = prim_names().find(k);
        if (it != prim_names().end()) {
            r.base = it->second;
            return r;
        }
        if (k == "class") {
            const V& n = t.get("name");
            r.base = py::truthy(n) ? py::str_of(n) : "undefined";
            return r;
        }
        if (k == "record") {
            const V& n = t.get("name");
            r.base = py::truthy(n) ? py::str_of(n) : record_text(t);
            return r;
        }
        if (k == "ptr") {
            r.mods.push_back("ptr");
            r.is_ptr.push_back(true);
            V nt = t.at("to");
            t = nt;
        } else if (k == "fnptr") {
            r.mods.push_back("ptr");
            r.is_ptr.push_back(true);
            r.base = "code";
            return r;
        } else if (k == "fn") {
            r.base = "code";
            return r;
        } else if (k == "array") {
            r.mods.push_back(t.has("count") ? py::str_of(t.get("count")) : std::string());
            r.is_ptr.push_back(false);
            V nt = t.at("elem");
            t = nt;
        } else if (k == "selector") {
            r.base = "uint2";
            return r;
        } else
            throw Decline("type kind " + py::repr(kv));
    }
}

bool is_far(const V& t) {
    if (!py::truthy(t)) return false;
    const V& k = t.get("kind");
    return (k == "ptr" || k == "fnptr") && t.get("space") == "far";
}

void push_type(Stack& st, const V& t, const Opt& name, bool far, const Opt& word0, const Opt& shown_name,
               const Opt& shown_base, bool) {
    TypeStack ts = type_stack(t);
    st.op(!name && ts.mods.empty() ? &TYPE_NOSPACE : &TYPE_SPACE);
    Opt word = far ? Opt(" __far") : word0;
    bool all_ptr = true;
    for (bool p : ts.is_ptr) all_ptr = all_ptr && p;
    Opt shown;
    if (word && !word->empty() && !ts.mods.empty() && all_ptr)
        shown = (shown_base && !shown_base->empty() ? *shown_base : ts.base) + *word;
    else
        shown = shown_base;
    st.atom(ts.base, shown);
    for (size_t k = ts.mods.size(); k-- > 0;) st.op(ts.is_ptr[k] ? &PTR : &ARRAY);
    if (!name) st.atom("", std::nullopt, true);
    else st.atom(*name, shown_name);
    for (size_t k = 0; k < ts.mods.size(); k++)
        if (!ts.is_ptr[k]) st.atom(ts.mods[k]);
}

void put_type(Em* em, const V& t, const Opt& name, bool far, const Opt& shown_name, const Opt& shown_base) {
    Stack st(em);
    push_type(st, t, name, far, std::nullopt, shown_name, shown_base);
}

std::string type_text(const V& t, const std::string& name, bool far) {
    if (t.is_none()) return py::strip("void " + name);
    // (the same type, name and distance: the same text -- the passes ask it over and over. The
    // emitter it is laid out on holds no node, so no layout's record of places needs it)
    // (keyed by the type's object, held in the table so its address is not another's later)
    Opt nm = name.empty() ? Opt() : Opt(name);
    if (!t.is_dict()) return flat_text([&](Em* em) { put_type(em, t, nm, far); });
    const void* p = t.id();
    std::string key((const char*)&p, sizeof p);
    key += far ? '1' : '0';
    key += name;
    auto it = now->type_texts.find(key);
    if (it != now->type_texts.end()) return it->second.second;
    std::string s = flat_text([&](Em* em) { put_type(em, t, nm, far); });
    now->type_texts.emplace(std::move(key), std::make_pair(t, s));
    return s;
}

// ---------------------------------------------------------------------------------------------- expressions
static int natural_base(uint64_t v) {
    int dec = 0;
    uint64_t tmp = v;
    uint64_t setdig = tmp % 10;
    if (setdig == 0 || setdig == 9) {
        dec = 1;
        tmp /= 10;
        while (tmp && tmp % 10 == setdig) {
            dec++;
            tmp /= 10;
        }
    }
    if (dec == 0) return 16;
    if ((dec == 1 && (tmp > 1 || setdig == 9)) || (dec == 2 && tmp > 10) || ((dec == 3 || dec == 4) && tmp > 100) ||
        (dec > 4 && tmp > 1000))
        return 16;
    int hexes = 0;
    tmp = v;
    setdig = tmp & 0xF;
    if (setdig == 0 || setdig == 0xF) {
        hexes = 1;
        tmp >>= 4;
        while (tmp && (tmp & 0xF) == setdig) {
            hexes++;
            tmp >>= 4;
        }
    }
    return dec > hexes ? 10 : 16;
}

std::string int_text(const V& v) {
    if (v.is_bool()) return v.b ? "true" : "false";
    if (!v.is_int()) {
        if (v.is_float()) py::raise("TypeError", "an int was expected: " + py::repr(v));
        py::raise("TypeError", "'<' not supported between instances");
    }
    bool neg = !v.u64 && v.i < 0;
    uint64_t m = v.u64 ? (uint64_t)v.i : neg ? (uint64_t)(-(v.i + 1)) + 1 : (uint64_t)v.i;
    std::string sign = neg ? "-" : "";
    if (m <= 10 || natural_base(m) == 10) return sign + std::to_string(m);
    return sign + "0x" + py::hex(m);
}

static std::string char_text(int64_t v) {
    if (v == 0) return "'\\0'";
    switch (v) {
        case 7: return "'\\a'";
        case 8: return "'\\b'";
        case 9: return "'\\t'";
        case 10: return "'\\n'";
        case 11: return "'\\v'";
        case 12: return "'\\f'";
        case 13: return "'\\r'";
        case 39: return "'\\''";
        case 92: return "'\\\\'";
    }
    if (v >= 32 && v < 127) return std::string("'") + (char)v + "'";
    return "'\\x" + py::hexpad((uint64_t)(v & 0xFF), 2) + "'";
}

std::string lit(const V& e) {
    const V& text = e.get("text");
    if (py::truthy(text)) return py::str_of(text);
    const V& v = e.get("value");
    const V& t0 = e.get("type");
    V t = py::truthy(t0) ? t0 : V::dict();
    if (v.is_str()) return "\"" + py::replace(py::replace(v.str(), "\\", "\\\\"), "\"", "\\\"") + "\"";
    if (v.is_float()) return py::float_repr(v.f);
    const V& kind = t.get("kind");
    if (kind == "char" && v.is_intlike()) {
        int64_t x = v.as_int();
        if (!v.u64 && x >= 0 && x < 0x80) return char_text(x);
        if (v.u64) return int_text(V((unsigned long long)((uint64_t)x - 0x100)));
        return int_text(V(x >= 0x80 ? x - 0x100 : x));
    }
    if ((kind == "ptr" || kind == "fnptr") && v.is_intlike() && (v.u64 || v.as_int() >= 0))
        return "0x" + py::hex((uint64_t)v.as_int());
    if (v.is_none()) throw Decline("literal without a value");
    return int_text(v);
}

Tok3 token(const V& e) {
    const std::string n = at(e, "node").str();
    if (n == "Binary") {
        const V& op = at(e, "op");
        auto& bp = binary_prec();
        auto it = op.is_str() ? bp.find(op.str()) : bp.end();
        if (it == bp.end()) py::raise("KeyError", py::repr(op));
        return {it->second, op.str(), "binary", true};
    }
    if (n == "Assign") {
        const V& op = e.get("op");
        return {14, (py::truthy(op) ? op.str() : std::string()) + "=", "binary", true};
    }
    if (n == "Unary") {
        std::string op = py::str_of(at(e, "op"));
        if (py::truthy(e.get("postfix"))) return {66, op, "postsurround", true};
        return {62, op, "unary_prefix", true};
    }
    if (n == "Cast") return {62, "cast", "presurround", true};
    if (n == "Call" || n == "Index") return {66, n, "postsurround", true};
    if (n == "Select") return {66, arrow(e) ? "->" : ".", "binary", true};
    if (n == "Cond") return {16, "?", "binary", true};
    return {100, "", "atom", false};
}

bool bracket(const Tok3& parent, const V& child, int stage) {
    Tok3 c = token(child);
    if (c.kind == "atom") return false;
    const std::string& pk = parent.kind;
    if (pk == "binary") {
        if (parent.prec != c.prec) return parent.prec > c.prec;
        if (parent.has_op && associative(parent.op) && c.has_op && parent.op == c.op) return false;
        return !(c.kind == "postsurround" && stage == 0);
    }
    if (pk == "unary_prefix" || pk == "presurround") {
        if (parent.prec != c.prec) return parent.prec > c.prec;
        return c.kind != "unary_prefix" && c.kind != "presurround";
    }
    if (pk == "postsurround") {
        if (stage == 1) return false;
        if (parent.prec != c.prec) return parent.prec > c.prec;
        return c.kind != "postsurround" && c.kind != "binary";
    }
    return false;
}

bool arrow(const V& e) {
    const V& obj = at(e, "obj");
    return obj.get("node") == "Unary" && obj.get("op") == "*" && !py::truthy(obj.get("postfix"));
}

std::string expr(const V& e) {
    return flat_text([&](Em* em) { put(em, e); });
}

std::string sub(const Tok3& parent, const V& child, int stage) {
    return bracket(parent, child, stage) ? "(" + expr(child) + ")" : expr(child);
}

static bool is_word_type(const std::string& s) {
    return s == "xunknown2 *" || s == "int2 *" || s == "uint2 *" || s == "undefined2 *";
}

void near_call(const V& e) {
    if (!now->has("near_calls") || e.get("node") != "Unary" || e.get("op") != "*" || py::truthy(e.get("postfix"))) return;
    const V& c = at(e, "x");
    const V& ct = c.get("type");
    if (c.get("node") != "Cast" || (py::truthy(ct) ? ct.get("kind") : py::NONE) != "fnptr") return;
    const V& word = at(c, "x");
    if (word.get("node") == "Unary" && word.get("op") == "*" && !py::truthy(word.get("postfix")) &&
        at(word, "x").get("node") == "Cast" && is_word_type(type_text(at(at(word, "x"), "type"))))
        now->marks[id(c)] = "_near";
}

static const char* first_slot(const std::string& n, bool with_cond) {
    if (n == "Binary") return "left";
    if (n == "Assign") return "target";
    if (n == "Cond" && with_cond) return "cond";
    if (n == "Call") return "target";
    if (n == "Index") return "arr";
    if (n == "Select") return "obj";
    return nullptr;
}

V leading_cast(const V& e0) {
    V e = e0;
    while (true) {
        const V& nv = e.get("node");
        std::string n = nv.is_str() ? nv.str() : "";
        if (n == "Cast") return e;
        Tok3 tok = token(e);
        const char* first = first_slot(n, true);
        if (n == "Unary" && py::truthy(e.get("postfix"))) first = "x";
        if (!first || !py::truthy(e.get(first))) return V();
        V child = e.at(first);
        if (n == "Select" && child.get("node") == "Unary" && child.get("op") == "*" && !py::truthy(child.get("postfix"))) {
            V x = child.at("x");
            child = x;
        }
        if (bracket(tok, child, 0)) return V();
        e = child;
    }
}

bool data_address(const V& e) {
    if (e.get("node") != "Lit") return false;
    static const re::Regex R("(0x[0-9a-fA-F]+|\\d+)[uUlL]*");
    std::string t = lit(e);
    re::Match m = R.fullmatch(t);
    if (!m) return false;
    int64_t v = py::int_parse(m.group(1), 0);
    return 0 < v && v <= 0xFFFF;
}

Opt assigned_name(const V& target0) {
    V target = target0;
    while (target.get("node") == "Unary" && target.get("op") == "*" && !py::truthy(target.get("postfix"))) {
        V x = target.at("x");
        target = x;
    }
    if (target.get("node") != "Name") return std::nullopt;
    V n = sym_name_v(target);
    if (n.is_none()) return std::nullopt;
    return py::str_of(n);
}

void put(Em* em, const V& e) {
    Stack st(em);
    push(st, e);
    if (!st.ops.empty()) throw Decline("expression left unfinished");
}

static void operands(const Tok3& tok, std::initializer_list<const V*> kids) {
    int stage = 0;
    for (const V* child : kids) {
        if (child && !child->is_none() && bracket(tok, *child, stage)) near_call(*child);
        stage++;
    }
}

const Op* op_of(const V& e) {
    std::string n = at(e, "node").str();
    if (n == "Binary") return binop(py::str_of(at(e, "op")));
    if (n == "Assign") return assign_op(e.get("op"));
    if (n == "Unary") {
        std::string op = py::str_of(at(e, "op"));
        return py::truthy(e.get("postfix")) ? postfix_op(op) : unary_op(op);
    }
    if (n == "Select") return member_op(arrow(e));
    if (n == "Cast") return &CAST;
    if (n == "Call") return &CALL;
    if (n == "Index") return &SUBSCRIPT;
    if (n == "Paren") return &HIDDEN;
    return nullptr;
}

std::string raw_text(const V& e) {
    now->bare.insert(id(e));
    now->raw += 1;
    struct Done {
        Id i;
        ~Done() {
            now->bare.erase(i);
            now->raw -= 1;
        }
    } done{id(e)};
    Flat em;
    put(&em, e);
    std::string s;
    for (auto& x : em.raw) s += x;
    return s;
}

void rewrite(const V& e, Rewrite shown, bool room) {
    if (!now->whole.count(id(e))) {
        now->whole[id(e)] = std::move(shown);
        if (room) now->room.insert(id(e));
        now->mut++;
    }
}

void push(Stack& st, const V& e, std::optional<bool> paren) {
    std::string n = at(e, "node").str();
    if (now->seeing && !now->raw) now->seen.insert(id(e));
    auto rw = now->whole.find(id(e));
    if (rw != now->whole.end() && !now->bare.count(id(e))) {
        bool seeing = now->seeing && !now->raw;
        if (seeing) now->owners.push_back(id(e));
        std::string shown;
        try {
            Rewrite f = rw->second;
            shown = f(e);
        } catch (...) {
            if (seeing) now->owners.pop_back();
            throw;
        }
        if (seeing) now->owners.pop_back();
        if (seeing) now->shown[id(e)] = shown;
        std::string raw = now->room.count(id(e)) ? std::string() : raw_text(e);
        st.whole(op_of(e), raw, shown, paren, &e);
        return;
    }
    if (n == "Lit") {
        st.atom(lit(e));
        return;
    }
    if (n == "Name") {
        std::string name = name_of(e);
        const V& sym = e.get("sym");
        bool field = (py::truthy(sym) ? sym.get("kind") : py::NONE) == "field";
        if (now->frame.count(name) && !field) {
            st.atom(name, name == ENTRY_SP ? std::string("((int4)&_f + (int4)sizeof(_f))") : "_f." + name, false, &e);
            return;
        }
        auto al = now->alias.find(name);
        if (al != now->alias.end()) {
            st.atom(name, al->second, false, &e);
            return;
        }
        Opt shown;
        if (mark_of(e) == "_ss") shown = mk_fp_text("codebreaker_ss()", e, ", ", name);
        st.atom(name, shown, false, &e);
        return;
    }
    if (n == "Paren") {
        near_call(at(e, "x"));
        st.op(&HIDDEN, paren);
        push(st, at(e, "x"), true);
        return;
    }
    Tok3 tok = token(e);
    if (n == "Cast") {
        const V& x = at(e, "x");
        operands(tok, {nullptr, &x});
        std::string mark = mark_of(e);
        st.op(&CAST, paren);
        std::string ct = cast_type(e);
        Opt pops_ = pops(ct) != ct ? Opt("code_callee_pops") : Opt();
        if (mark == "_near")
            push_type(st, at(e, "type"), std::nullopt, false, Opt(" __near"), std::nullopt, pops_);
        else
            push_type(st, at(e, "type"), std::nullopt, mark == "_far" || mark == "_ds", std::nullopt, std::nullopt, pops_);
        if (mark == "_ds") {
            std::string num = lit(x);
            st.atom(num, mk_fp_text("codebreaker_ds()", x, ", ", num));
            return;
        }
        push(st, x);
        return;
    }
    if (n == "Unary" && py::truthy(e.get("postfix")) && postfix_op(py::str_of(at(e, "op")))) {
        const V& x = at(e, "x");
        operands(tok, {&x});
        st.op(postfix_op(at(e, "op").str()), paren);
        push(st, x);
        return;
    }
    if (n == "Unary") {
        std::string op = py::str_of(at(e, "op"));
        if (py::truthy(e.get("postfix")) || !unary_op(op)) throw Decline("unary " + op);
        const V& x = at(e, "x");
        operands(tok, {&x});
        st.op(unary_op(op), paren);
        push(st, x);
        return;
    }
    if (n == "Binary") {
        const V &l = at(e, "left"), &r = at(e, "right");
        operands(tok, {&l, &r});
        st.op(binop(at(e, "op").str()), paren);
        push(st, l);
        push(st, r);
        return;
    }
    if (n == "Assign") {
        const V &target = at(e, "target"), &value = at(e, "value");
        if (now->has("far") && !py::truthy(e.get("op"))) {
            Opt an = assigned_name(target);
            if (an && now->far.count(*an)) {
                V c = leading_cast(value);
                if (!c.is_none()) {
                    const V& ct = c.get("type");
                    const V& kind = py::truthy(ct) ? ct.get("kind") : py::NONE;
                    if (kind == "ptr" || kind == "fnptr") now->marks[id(c)] = data_address(at(c, "x")) ? "_ds" : "_far";
                }
            }
        }
        operands(tok, {&target, &value});
        st.op(assign_op(e.get("op")), paren);
        push(st, target);
        push(st, value);
        return;
    }
    if (n == "Call") {
        const V& target = e.get("target");
        V args = py::truthy(e.get("args")) ? e.get("args") : V::list();
        operands(tok, {&target});
        st.op(&CALL, paren);
        if (py::truthy(target))
            push(st, target);
        else {
            const V& sym = e.get("sym");
            V nm = py::truthy(sym) ? (sym.has("name") ? sym.get("name") : V("?")) : V("?");
            st.atom(py::str_of(nm));
        }
        auto& a = args.seq();
        if (a.empty()) st.atom("", std::nullopt, true);
        for (size_t k = 1; k < a.size(); k++) st.op(binop(","));
        for (auto& x : a) push(st, x);
        return;
    }
    if (n == "Index") {
        const V &arr = at(e, "arr"), &index = at(e, "index");
        operands(tok, {&arr, &index});
        st.op(&SUBSCRIPT, paren);
        push(st, arr);
        push(st, index);
        return;
    }
    if (n == "Select") {
        bool a = arrow(e);
        const V& obj = a ? at(at(e, "obj"), "x") : at(e, "obj");
        operands(tok, {&obj});
        st.op(member_op(a), paren);
        push(st, obj);
        st.atom(name_of(e));
        return;
    }
    throw Decline("expression " + n);
}

// ---------------------------------------------------------------------------------------------- conditions
static bool joined(const V& e) {
    return e.get("node") == "Binary" && (e.get("op") == "&&" || e.get("op") == "||") && at(e, "left").get("node") == "Paren" &&
           at(e, "right").get("node") == "Paren";
}

static void listed(const V& e, std::vector<V>& out) {
    if (e.get("node") == "Binary" && e.get("op") == ",") {
        listed(at(e, "left"), out);
        out.push_back(at(e, "right"));
        return;
    }
    out.push_back(e);
}

void cond(Em* em, const V& e, bool comma) {
    if (joined(e)) {
        em->open_paren("(");
        cond(em, at(at(e, "left"), "x"), comma);
        em->spaces(1);
        em->print(at(e, "op").str());
        em->spaces(1);
        em->open_paren("(");
        cond(em, at(at(e, "right"), "x"), true);
        em->close_paren(")");
        em->close_paren(")");
        return;
    }
    if (!comma) {
        em->open_paren("(");
        put(em, e);
        em->close_paren(")");
        return;
    }
    std::vector<V> items;
    listed(e, items);
    for (size_t i = 0; i < items.size(); i++) {
        if (i) {
            em->print(",");
            em->spaces(1);
        }
        em->open_group();
        if (i == items.size() - 1) {
            em->open_group();
            put(em, items[i]);
            em->close_group();
        } else
            put(em, items[i]);
        em->close_group();
    }
}

std::string condition(const V& e, bool inner) {
    return flat_text([&](Em* em) { cond(em, e, inner); });
}

// ---------------------------------------------------------------------------------------------- statements
static bool true_lit(const V& e) { return e.get("node") == "Lit" && e.get("value").is_bool() && e.get("value").b; }

static V case_pointer(const V& lab) {
    const V& t0 = lab.get("type");
    V t = py::truthy(t0) ? t0 : V::dict();
    if ((lab.get("node") == "Cast" || lab.get("node") == "Lit") && (t.get("kind") == "ptr" || t.get("kind") == "fnptr")) return t;
    return V();
}

static V integer_switch(const V& s) {
    std::set<int> widths;
    for (auto& c : stmts(s.get("cases")))
        for (auto& lab : stmts(c.get("labels"))) {
            V p = case_pointer(lab);
            if (!p.is_none()) widths.insert(p.get("space") == "near" ? 2 : 4);
        }
    if (widths.size() != 1) return s;
    V u = V::dict();
    u.set("kind", V(*widths.begin() == 2 ? "u16" : "u32"));
    static const re::Regex NUM("0x[0-9a-fA-F]+|\\d+");
    auto number = [&](const V& lab) -> V {
        if (case_pointer(lab).is_none()) return lab;
        const V& l = lab.get("node") == "Cast" ? at(lab, "x") : lab;
        if (l.get("node") != "Lit" || !l.get("value").is_intlike()) throw Decline("a case label of a pointer type that is no number");
        V out = py::copy_dict(l);
        out.set("type", u);
        std::string text = out.has("text") ? py::str_of(out.get("text")) : "0";
        if (out.has("text") && out.get("text").is_none()) text = "None";
        if (!NUM.fullmatch(text)) out.erase(V("text"));
        return out;
    };
    V cases = V::list();
    for (auto& c : stmts(s.get("cases"))) {
        V labels = V::list();
        for (auto& lab : stmts(c.get("labels"))) labels.append(number(lab));
        V nc = py::copy_dict(c);
        nc.set("labels", labels);
        cases.append(nc);
    }
    V out = py::copy_dict(s);
    V cast = V::dict();
    cast.set("node", V("Cast"));
    cast.set("type", u);
    cast.set("x", at(s, "expr"));
    out.set("expr", cast);
    out.set("cases", cases);
    return out;
}

static void inserted(Em* em, const V& s, bool before) {
    if (before && now->seeing && em->flat()) now->stmt_at[id(s)] = static_cast<Flat*>(em)->out.size();
    Strs* texts = before ? now->before.get(id(s)) : now->after.get(id(s));
    if (!texts) return;
    Strs copy = *texts;
    for (auto& t : copy) {
        em->tag_line();
        em->print("", t);
    }
}

void block(Em* em, const V& ss) {
    std::vector<V> list = stmts(ss);
    for (auto& s : list) stmt(em, s);
    if (!list.empty()) {
        const V& last = list.back();
        if (last.is_dict() && last.get("node") == "Labeled" && stmts(last.get("body")).empty()) em->print(";");
    }
}

static void braced(Em* em, const V& ss) {
    em->spaces(1);
    em->start_indent();
    em->print("{");
    block(em, ss);
    em->stop_indent();
    em->tag_line();
    em->print("}");
}

void comment(Em* em, const std::string& text, std::optional<int> indent) {
    em->tag_line(indent);
    em->start_comment();
    em->print("/* ");
    static const re::Regex W("[ \\t]+|\\n|\\r|\\{@[^}]*\\}?|[^\\s]+");
    for (auto& m : W.finditer(text)) {
        std::string w = m.group(0);
        if (w[0] == ' ' || w[0] == '\t') em->spaces((int)cp_len(w));
        else if (w == "\n") em->tag_line();
        else if (w != "\r") em->print(w);
    }
    em->print(" */");
    em->stop_comment();
}

static void jump(Em* em, const V& s) {
    em->open_group();
    std::string n = at(s, "node").str();
    if (n == "Goto") {
        em->print("goto");
        em->spaces(1);
        em->print(py::str_of(at(s, "label")));
    } else if (n == "Break" || n == "Continue") {
        em->print(py::lower(n));
    } else
        throw Decline("jump " + n);
    em->print(";");
    em->close_group();
}

static void while_true(Em* em) {
    em->open_paren("(");
    em->spaces(1);
    em->print("true");
    em->spaces(1);
    em->close_paren(")");
}

static void if_stmt(Em* em, const V& s, bool merged = false) {
    if (merged) em->spaces(1);
    else em->tag_line();
    em->print("if");
    em->spaces(1);
    cond(em, at(s, "cond"), false);
    const V& then = s.get("then");
    if (then.is_dict()) {
        em->spaces(1);
        jump(em, then);
        return;
    }
    braced(em, then);
    const V& orelse = s.get("orelse");
    if (orelse.is_none()) return;
    em->tag_line();
    em->print("else");
    if (orelse.is_list() && orelse.size() == 1 && at(orelse[0], "node") == "If") {
        if_stmt(em, orelse[0], true);
        return;
    }
    braced(em, orelse);
}

static void statement(Em* em, const V& e, const char* word = nullptr) {
    em->tag_line();
    em->open_group();
    if (word) {
        em->print(word);
        if (!e.is_none()) em->spaces(1);
    }
    if (!e.is_none()) put(em, e);
    em->close_group();
    em->print(";");
}

void stmt(Em* em, const V& s0) {
    V s = s0;
    std::string n = at(s, "node").str();
    if (n == "Local") return;
    if (now->gone.count(id(s))) return;
    inserted(em, s, true);
    if (n == "ExprStmt") {
        if (now->seeing) {
            now->seen.insert(id(s));
            now->lines.insert(id(s));
        }
        statement(em, at(s, "expr"));
        inserted(em, s, false);
    } else if (n == "Return") {
        statement(em, s.get("expr"), "return");
    } else if (n == "If") {
        if_stmt(em, s);
    } else if (n == "While") {
        em->tag_line();
        em->print("while");
        if (true_lit(at(s, "cond")))
            while_true(em);
        else {
            em->spaces(1);
            em->open_paren("(");
            cond(em, at(s, "cond"), true);
            em->close_paren(")");
        }
        braced(em, s.get("body"));
    } else if (n == "DoWhile") {
        em->tag_line();
        em->print("do");
        braced(em, s.get("body"));
        em->spaces(1);
        em->print("while");
        if (true_lit(at(s, "cond")))
            while_true(em);
        else {
            em->spaces(1);
            cond(em, at(s, "cond"), false);
        }
        em->print(";");
    } else if (n == "For") {
        em->tag_line();
        em->print("for");
        em->spaces(1);
        em->open_paren("(");
        auto init = stmts(s.get("init"));
        for (size_t i = 0; i < init.size(); i++) {
            if (i) {
                em->print(";");
                em->spaces(1);
            }
            em->open_group();
            put(em, init[i].get("node") == "ExprStmt" ? at(init[i], "expr") : init[i]);
            em->close_group();
        }
        em->print(";");
        em->spaces(1);
        if (py::truthy(s.get("cond"))) cond(em, at(s, "cond"), true);
        em->print(";");
        em->spaces(1);
        auto upd = stmts(s.get("update"));
        for (size_t i = 0; i < upd.size(); i++) {
            if (i) em->print(",");
            em->open_group();
            put(em, upd[i].get("node") == "ExprStmt" ? at(upd[i], "expr") : upd[i]);
            em->close_group();
        }
        em->close_paren(")");
        braced(em, s.get("body"));
    } else if (n == "Switch") {
        s = integer_switch(s);
        em->tag_line();
        em->print("switch");
        em->open_paren("(");
        put(em, at(s, "expr"));
        em->close_paren(")");
        em->spaces(1);
        em->print("{");
        for (auto& c : stmts(s.get("cases"))) {
            if (py::truthy(c.get("default"))) {
                inserted(em, c, true);
                em->tag_line();
                em->print("default");
                em->print(":");
            } else {
                auto labels = stmts(c.get("labels"));
                for (size_t k = 0; k < labels.size(); k++) {
                    if (k == labels.size() - 1) inserted(em, c, true);
                    em->tag_line();
                    em->print("case");
                    em->spaces(1);
                    put(em, labels[k]);
                    em->print(":");
                }
            }
            em->start_indent();
            block(em, c.get("body"));
            em->stop_indent();
        }
        em->tag_line();
        em->print("}");
    } else if (n == "Goto" || n == "Break" || n == "Continue") {
        em->tag_line();
        jump(em, s);
    } else if (n == "Labeled") {
        em->tag_line(0);
        em->print(py::str_of(at(s, "label")));
        em->print(":");
        block(em, s.get("body"));
    } else if (n == "Block") {
        block(em, s.get("stmts"));
    } else if (n == "Comment") {
        comment(em, py::str_of(at(s, "text")), 20);
    } else if (n == "Empty") {
    } else if (n == "MachineFragment" && s.get("reason") == "inline") {
        em->tag_line();
        em->print("_asm {");
        em->start_indent();
        for (auto& ln : stmts(s.get("asm"))) {
            const std::string& l = ln.str();
            if (py::endswith(l, ":")) em->tag_line(0);
            else em->tag_line();
            em->print(l);
        }
        em->stop_indent();
        em->tag_line();
        em->print("}");
    } else
        throw Decline("statement " + n);
}

static void locals_into(const V& ss, std::vector<V>& found) {
    for (auto& s : stmts(ss)) {
        if (!s.is_dict()) continue;
        if (s.get("node") == "Local") {
            found.push_back(s);
            continue;
        }
        for (const char* key : {"then", "orelse", "body", "stmts", "cases"})
            if (s.has(key)) locals_into(s.get(key), found);
    }
}

std::vector<V> locals_of(const V& body) {
    std::vector<V> found;
    locals_into(body, found);
    return found;
}

// ---------------------------------------------------------------------------------------------- the heading
V ret_of(const V& doc) {
    const V& r = doc.get("ret");
    if (py::truthy(r)) return r;
    const V& sym = doc.get("sym");
    const V& ft = py::truthy(sym) ? sym.get("type") : py::NONE;
    const V& res = py::truthy(ft) ? ft.get("result") : py::NONE;
    const V& t = py::truthy(res) ? res.get("type") : py::NONE;
    if (py::truthy(t)) return t;
    V v = V::dict();
    v.set("kind", V("void"));
    return v;
}

static Strs mods_of(const V& doc) {
    Strs out;
    const V& m = doc.get("mods");
    if (py::truthy(m))
        for (auto& x : m.seq()) out.push_back(py::str_of(x));
    return out;
}

bool lost(const V& doc) {
    if (py::truthy(doc.get("params"))) return false;
    static const re::Regex F("__fastcall\\w+");
    for (auto& m : mods_of(doc))
        if (F.fullmatch(m)) return true;
    return false;
}

Heading heading(const V& doc) {
    const V& ctx = now->facts;
    V ret = ret_of(doc);
    Heading h;
    h.result = type_text(ret);
    h.mods = mods_of(doc);
    Strs head0v{h.result};
    head0v.insert(head0v.end(), h.mods.begin(), h.mods.end());
    std::string head0 = py::join(" ", head0v);
    if (now->has("model_word") && py::truthy(ctx.get("distance")) && head0.find("16near") == std::string::npos &&
        head0.find("16far") == std::string::npos) {
        std::string t = py::replace(py::replace(py::replace(head0, "(", " "), ")", " "), "*", " ");
        bool said = false;
        for (auto& w : py::split_ws(t))
            if (w == "__stdcall" || w == "__cdecl" || w == "__pascal") said = true;
        if (py::truthy(ctx.get("model")) && !said) h.words.push_back(py::str_of(ctx.get("model")));
        else h.words.push_back("__" + py::str_of(ctx.at("distance")));
    }
    Strs mw = h.mods;
    mw.insert(mw.end(), h.words.begin(), h.words.end());
    bool fm = false;
    for (auto& w : mw)
        if (w.find("__stdcall16far") != std::string::npos || w.find("__cdecl16far") != std::string::npos) fm = true;
    if (!mw.empty() && mw.back() == "__far") fm = true;
    h.far_model = fm;
    if (fm && now->has("export") && py::truthy(ctx.get("export")) && !lost(doc)) h.words.push_back("__export");
    if (now->has("loadds") && py::truthy(ctx.get("loadds")) && !lost(doc)) h.words.push_back("__loadds");
    return h;
}

static bool is_byte(const std::string& b) {
    return b == "bool" || b == "char" || b == "int1" || b == "uint1" || b == "xunknown1" || b == "undefined1" || b == "undefined";
}

std::string result_shown_base(const V& ret, bool& has) {
    TypeStack ts = type_stack(ret);
    has = true;
    if (ts.base == "void" && now->has("int_result")) return "int";
    if (ts.base == "void" && now->has("uint8_result")) {
        now->facts.set("novalue", V(true));
        return "uint8";
    }
    if (ts.base == "void" && now->has("void_result")) return "xunknown2";
    if (ts.mods.empty() && is_byte(ts.base) && now->has("byte_result") && !lost(now->doc.is_none() ? V::dict() : now->doc))
        return ts.base == "char" || ts.base == "int1" ? "int2" : "uint2";
    has = false;
    return "";
}

static std::string register_arguments(const V& doc) {
    Strs mods = mods_of(doc);
    for (auto wr : {std::make_pair("__thiscall", "ecx"), std::make_pair("__fastcall", "edx")}) {
        for (auto& m : mods)
            if (m == wr.first) {
                now->facts.set("registers", V(wr.second));
                return wr.first;
            }
    }
    return std::string("\x01");      // (None: matches no word)
}

void signature_em(Em* em, const V& doc) {
    const V& sym = doc.get("sym");
    V ft = py::truthy(sym) && py::truthy(sym.get("type")) ? sym.get("type") : V::dict();
    em->open_group();
    em->open_group();
    V ret = ret_of(doc);
    bool far = now->has("far_result") && is_far(ret);
    if (far) {
        TypeStack ts = type_stack(ret);
        if (!(ts.mods.size() == 1 && ts.is_ptr[0])) throw Decline("a far result of a pointer to a pointer");
    }
    bool has;
    std::string sb = result_shown_base(ret, has);
    put_type(em, ret, std::nullopt, far, std::nullopt, has ? Opt(sb) : Opt());
    em->close_group();
    em->spaces(1);
    for (auto& m : mods_of(doc)) {
        if (now->has("register_arguments") && m == register_arguments(doc)) {
            em->print(m + " ", std::string());
            continue;
        }
        em->print(m);
        em->spaces(1);
    }
    em->open_group();
    Strs words = py::truthy(now->facts) ? heading(doc).words : Strs();
    std::string name = py::str_of(at(doc, "name"));
    if (!words.empty()) {
        Strs w = words;
        w.push_back(name);
        em->print(name, py::join(" ", w));
    } else
        em->print(name);
    em->spaces(0, 10);
    em->open_paren("(");
    em->spaces(0, 10);
    std::vector<V> params = stmts(py::truthy(doc.get("params")) ? doc.get("params") : V::list());
    bool old_here = now->old_style && same(now->doc, doc);
    if (old_here) {
        Strs names;
        for (auto& nd : *now->old_style) names.push_back(nd.first);
        if (!names.empty()) em->print(py::join(",", names));
        params.clear();
        ft = V::dict();
    } else if (params.empty())
        em->print("void");
    for (size_t i = 0; i < params.size(); i++) {
        if (i) em->print(",");
        em->open_group();
        std::string pn = param_name(params[i]);
        put_type(em, params[i].get("type"), pn, now->far.count(pn) != 0);
        em->close_group();
    }
    if (py::truthy(ft.get("variadic"))) {
        if (!params.empty()) em->print(",");
        em->print("...");
    }
    em->close_paren(")");
    em->close_group();
    em->close_group();
}

std::string signature(const V& doc) {
    return flat_text([&](Em* em) { signature_em(em, doc); });
}

Opt segment_register(const std::string& name, const V& t) {
    static const re::Regex R("unaff_(SS|DS|CS)"), W("\\w+");
    if (!now->has("segment_registers")) return std::nullopt;
    re::Match m = R.fullmatch(name);
    if (!m || !W.fullmatch(type_text(t))) return std::nullopt;
    return name + " = codebreaker_" + py::lower(m.group(1)) + "()";
}

std::string function_text(const V& doc, const std::vector<V>& locs0, Em* em0) {
    std::vector<V> notes;
    if (em0 || !py::truthy(now->facts.get("without_notes"))) {
        const V& c = doc.get("comments");
        if (py::truthy(c)) notes = c.seq();
    }
    std::unique_ptr<Pretty> own;
    Em* em = em0;
    if (!em) {
        own.reset(new Pretty());
        em = own.get();
    }
    em->open_group();
    for (auto& t : notes) comment(em, py::str_of(t), 0);
    if (!notes.empty()) em->tag_line();
    em->tag_line();
    signature_em(em, doc);
    if (now->old_style)
        for (auto& nd : *now->old_style) {
            em->tag_line(0);
            em->print(nd.second + ";");
        }
    if (!now->old_style) em->tag_line();
    em->tag_line();
    em->start_indent();
    em->print("{");
    for (size_t k = now->declare_first.size(); k-- > 0;) {
        em->tag_line();
        em->print("", now->declare_first[k].first + " " + now->declare_first[k].second + ";");
    }
    const std::vector<V>* locs = &locs0;
    std::vector<V> none;
    if (now->locals_text) {
        em->print("", *now->locals_text);
        locs = &none;
    }
    for (auto& loc : *locs) {
        const V& sym = loc.get("sym");
        std::string name;
        if (py::truthy(sym) && sym.has("name")) name = sym.get("name").is_none() ? std::string() : py::str_of(sym.get("name"));
        if (now->drop.count(name)) continue;
        em->tag_line();
        em->open_group();
        bool far = now->far.count(name) != 0;
        auto d = now->decls.find(name);
        if (d != now->decls.end())
            em->print(type_text(loc.get("type"), name, far), d->second);
        else
            put_type(em, loc.get("type"), name, far, segment_register(name, loc.get("type")));
        em->close_group();
        em->print(";");
    }
    if (!locs->empty()) em->tag_line();
    block(em, doc.get("body"));
    em->stop_indent();
    em->tag_line();
    em->print("}");
    em->tag_line();
    em->close_group();
    std::string t = em->text();
    return t.empty() ? t : t.substr(1);
}

}  // namespace cb
