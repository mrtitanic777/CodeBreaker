// translate/backends/c/writer.cpp -- backends/c.py's writer (the L2 core library's cback.cc) on the typed tree: types (read from
// ir's structs), expressions (each through the reverse-Polish stack), conditions, statements, the heading and the function
// (PrintC's docFunction).
#include <algorithm>
#include <variant>

#include "translate/backends/c/ctree.hpp"

namespace cb::translate::ctree {

const char* ENTRY_SP = "register0x00000010";

namespace {

std::string mark_of(const X& e) {
    auto it = now->marks.find(id(e));
    return it == now->marks.end() ? std::string() : it->second;
}

}  // namespace

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
namespace {

const char* prim_name(ir::Prim p) {
    switch (p) {
        case ir::Prim::I8: return "int1";
        case ir::Prim::U8: return "uint1";
        case ir::Prim::I16: return "int2";
        case ir::Prim::U16: return "uint2";
        case ir::Prim::I32: return "int4";
        case ir::Prim::U32: return "uint4";
        case ir::Prim::I64: return "int8";
        case ir::Prim::U64: return "uint8";
        case ir::Prim::F32: return "float4";
        case ir::Prim::F64: return "float8";
        case ir::Prim::F80: return "float10";
        case ir::Prim::Bool: return "bool";
        case ir::Prim::Char: return "char";
    }
    return "?";
}

std::string record_text(const ir::RecordType& t) {
    Strs fields;
    for (const ir::RecordField& f : t.fields) fields.push_back(type_text(f.type.get(), f.name) + ";");   // (a field always has its name)
    return std::string(t.packed && *t.packed ? "_Packed " : "") + "struct { " + py::join(" ", fields) + " }";
}

}  // namespace

TypeStack type_stack(const ir::Type* t) {
    TypeStack r;
    while (true) {
        if (t == nullptr) {
            r.base = "void";
            return r;
        }
        if (const auto* p = std::get_if<ir::PrimType>(&t->body)) {
            r.base = prim_name(p->prim);
            return r;
        }
        if (std::holds_alternative<ir::VoidType>(t->body)) {
            r.base = "void";
            return r;
        }
        if (const auto* c = std::get_if<ir::ClassType>(&t->body)) {
            r.base = !c->name.empty() ? c->name : "undefined";
            return r;
        }
        if (const auto* rec = std::get_if<ir::RecordType>(&t->body)) {
            r.base = rec->name && !rec->name->empty() ? *rec->name : record_text(*rec);
            return r;
        }
        if (const auto* p = std::get_if<ir::PtrType>(&t->body)) {
            r.mods.push_back("ptr");
            r.is_ptr.push_back(true);
            t = p->to.get();
        } else if (std::holds_alternative<ir::FnPtrType>(t->body)) {
            r.mods.push_back("ptr");
            r.is_ptr.push_back(true);
            r.base = "code";
            return r;
        } else if (std::holds_alternative<ir::FnType>(t->body)) {
            r.base = "code";
            return r;
        } else if (const auto* a = std::get_if<ir::ArrayType>(&t->body)) {
            r.mods.push_back(a->count ? std::to_string(*a->count) : std::string());
            r.is_ptr.push_back(false);
            t = a->elem.get();
        } else if (std::holds_alternative<ir::SelectorType>(t->body)) {
            r.base = "uint2";
            return r;
        } else
            throw Decline("type kind " + py::str_repr(std::string(t->kind())));
    }
}

bool is_far(const ir::Type* t) {
    if (t == nullptr) return false;
    if (const auto* p = std::get_if<ir::PtrType>(&t->body)) return p->space == ir::AddressSpace::Far;
    if (const auto* f = std::get_if<ir::FnPtrType>(&t->body)) return f->space == ir::AddressSpace::Far;
    return false;
}

void push_type(Stack& st, const ir::Type* t, const Opt& name, bool far, const Opt& word0, const Opt& shown_name,
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

void put_type(Em* em, const ir::Type* t, const Opt& name, bool far, const Opt& shown_name, const Opt& shown_base) {
    Stack st(em);
    push_type(st, t, name, far, std::nullopt, shown_name, shown_base);
}

std::string type_text(const ir::Type* t, const std::string& name, bool far) {
    if (t == nullptr) return py::strip("void " + name);
    // (the same type, name and distance: the same text -- the passes ask it over and over. The emitter it is laid out on holds
    // no node, so no layout's record of places needs it. Keyed by the type's object: the tree holds it while the function is written)
    Opt nm = name.empty() ? Opt() : Opt(name);
    std::string key(reinterpret_cast<const char*>(&t), sizeof t);
    key += far ? '1' : '0';
    key += name;
    auto it = now->type_texts.find(key);
    if (it != now->type_texts.end()) return it->second;
    std::string s = flat_text([&](Em* em) { put_type(em, t, nm, far); });
    now->type_texts.emplace(std::move(key), s);
    return s;
}

// ---------------------------------------------------------------------------------------------- expressions
namespace {

int natural_base(uint64_t v) {
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

std::string char_text(int64_t v) {
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
    if (v >= 32 && v < 127) return std::string("'") + static_cast<char>(v) + "'";
    return "'\\x" + py::hexpad(static_cast<uint64_t>(v & 0xFF), 2) + "'";
}

}  // namespace

std::string int_text(const V& v) {
    if (v.is_bool()) return v.b ? "true" : "false";
    if (!v.is_int()) {
        if (v.is_float()) py::raise("TypeError", "an int was expected: " + py::repr(v));
        py::raise("TypeError", "'<' not supported between instances");
    }
    bool neg = !v.u64 && v.i < 0;
    uint64_t m = v.u64 ? static_cast<uint64_t>(v.i) : neg ? static_cast<uint64_t>(-(v.i + 1)) + 1 : static_cast<uint64_t>(v.i);
    std::string sign = neg ? "-" : "";
    if (m <= 10 || natural_base(m) == 10) return sign + std::to_string(m);
    return sign + "0x" + py::hex(m);
}

std::string lit(const X& e) {
    const X text = e.get("text");
    if (py::truthy(text)) return py::str_of(text);
    const X v = e.get("value");
    const X t = e.get("type");
    if (v.is_str()) return "\"" + py::replace(py::replace(v.str(), "\\", "\\\\"), "\"", "\\\"") + "\"";
    if (v.is_float()) return py::float_repr(v.as_float());
    const X kind = t.get("kind");
    if (kind == "char" && v.is_intlike()) {
        int64_t x = v.as_int();
        if (!v.is_u64() && x >= 0 && x < 0x80) return char_text(x);
        if (v.is_u64()) return int_text(V(static_cast<unsigned long long>(static_cast<uint64_t>(x) - 0x100)));
        return int_text(V(static_cast<long long>(x >= 0x80 ? x - 0x100 : x)));
    }
    if ((kind == "ptr" || kind == "fnptr") && v.is_intlike() && (v.is_u64() || v.as_int() >= 0))
        return "0x" + py::hex(static_cast<uint64_t>(v.as_int()));
    if (v.is_none()) throw Decline("literal without a value");
    return int_text(to_v(v));
}

Tok3 token(const X& e) {
    const std::string n = at(e, "node").str();
    if (n == "Binary") {
        const X op = at(e, "op");
        auto& bp = binary_prec();
        auto it = op.is_str() ? bp.find(op.str()) : bp.end();
        if (it == bp.end()) py::raise("KeyError", py::repr(op));
        return {it->second, op.str(), "binary", true};
    }
    if (n == "Assign") {
        const X op = e.get("op");
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

bool bracket(const Tok3& parent, const X& child, int stage) {
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

bool arrow(const X& e) {
    const X obj = at(e, "obj");
    return obj.get("node") == "Unary" && obj.get("op") == "*" && !py::truthy(obj.get("postfix"));
}

std::string expr(const X& e) {
    return flat_text([&](Em* em) { put(em, e); });
}

std::string sub(const Tok3& parent, const X& child, int stage) {
    return bracket(parent, child, stage) ? "(" + expr(child) + ")" : expr(child);
}

namespace {

bool is_word_type(const std::string& s) {
    return s == "xunknown2 *" || s == "int2 *" || s == "uint2 *" || s == "undefined2 *";
}

}  // namespace

void near_call(const X& e) {
    if (!now->has("near_calls") || e.get("node") != "Unary" || e.get("op") != "*" || py::truthy(e.get("postfix"))) return;
    const X c = at(e, "x");
    const X ct = c.get("type");
    if (c.get("node") != "Cast" || (py::truthy(ct) ? ct.get("kind") : X()) != "fnptr") return;
    const X word = at(c, "x");
    if (word.get("node") == "Unary" && word.get("op") == "*" && !py::truthy(word.get("postfix")) &&
        at(word, "x").get("node") == "Cast" && is_word_type(type_text(at(at(word, "x"), "type"))))
        now->marks[id(c)] = "_near";
}

namespace {

const char* first_slot(const std::string& n, bool with_cond) {
    if (n == "Binary") return "left";
    if (n == "Assign") return "target";
    if (n == "Cond" && with_cond) return "cond";
    if (n == "Call") return "target";
    if (n == "Index") return "arr";
    if (n == "Select") return "obj";
    return nullptr;
}

}  // namespace

X leading_cast(const X& e0) {
    X e = e0;
    while (true) {
        const X nv = e.get("node");
        std::string n = nv.is_str() ? nv.str() : "";
        if (n == "Cast") return e;
        Tok3 tok = token(e);
        const char* first = first_slot(n, true);
        if (n == "Unary" && py::truthy(e.get("postfix"))) first = "x";
        if (!first || !py::truthy(e.get(first))) return X();
        X child = e.at(first);
        if (n == "Select" && child.get("node") == "Unary" && child.get("op") == "*" && !py::truthy(child.get("postfix"))) {
            X x = child.at("x");
            child = x;
        }
        if (bracket(tok, child, 0)) return X();
        e = child;
    }
}

bool data_address(const X& e) {
    if (e.get("node") != "Lit") return false;
    static const re::Regex R("(0x[0-9a-fA-F]+|\\d+)[uUlL]*");
    std::string t = lit(e);
    re::Match m = R.fullmatch(t);
    if (!m) return false;
    int64_t v = py::int_parse(m.group(1), 0);
    return 0 < v && v <= 0xFFFF;
}

Opt assigned_name(const X& target0) {
    X target = target0;
    while (target.get("node") == "Unary" && target.get("op") == "*" && !py::truthy(target.get("postfix"))) {
        X x = target.at("x");
        target = x;
    }
    if (target.get("node") != "Name") return std::nullopt;
    V n = sym_name_v(target);
    if (n.is_none()) return std::nullopt;
    return py::str_of(n);
}

void put(Em* em, const X& e) {
    Stack st(em);
    push(st, e);
    if (!st.ops.empty()) throw Decline("expression left unfinished");
}

namespace {

void operands(const Tok3& tok, std::initializer_list<const X*> kids) {
    int stage = 0;
    for (const X* child : kids) {
        if (child && !child->is_none() && bracket(tok, *child, stage)) near_call(*child);
        stage++;
    }
}

}  // namespace

const Op* op_of(const X& e) {
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

std::string raw_text(const X& e) {
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

void rewrite(const X& e, Rewrite shown, bool room) {
    if (!now->whole.count(id(e))) {
        now->whole[id(e)] = std::move(shown);
        if (room) now->room.insert(id(e));
        now->mut++;
    }
}

void push(Stack& st, const X& e, std::optional<bool> paren) {
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
        const X sym = e.get("sym");
        bool field = (py::truthy(sym) ? sym.get("kind") : X()) == "field";
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
        const X x = at(e, "x");
        operands(tok, {nullptr, &x});
        std::string mk = mark_of(e);
        st.op(&CAST, paren);
        std::string ct = cast_type(e);
        Opt pops_ = pops(ct) != ct ? Opt("code_callee_pops") : Opt();
        if (mk == "_near")
            push_type(st, at(e, "type").as_type(), std::nullopt, false, Opt(" __near"), std::nullopt, pops_);
        else
            push_type(st, at(e, "type").as_type(), std::nullopt, mk == "_far" || mk == "_ds", std::nullopt, std::nullopt, pops_);
        if (mk == "_ds") {
            std::string num = lit(x);
            st.atom(num, mk_fp_text("codebreaker_ds()", x, ", ", num));
            return;
        }
        push(st, x);
        return;
    }
    if (n == "Unary" && py::truthy(e.get("postfix")) && postfix_op(py::str_of(at(e, "op")))) {
        const X x = at(e, "x");
        operands(tok, {&x});
        st.op(postfix_op(at(e, "op").str()), paren);
        push(st, x);
        return;
    }
    if (n == "Unary") {
        std::string op = py::str_of(at(e, "op"));
        if (py::truthy(e.get("postfix")) || !unary_op(op)) throw Decline("unary " + op);
        const X x = at(e, "x");
        operands(tok, {&x});
        st.op(unary_op(op), paren);
        push(st, x);
        return;
    }
    if (n == "Binary") {
        const X l = at(e, "left"), r = at(e, "right");
        operands(tok, {&l, &r});
        st.op(binop(at(e, "op").str()), paren);
        push(st, l);
        push(st, r);
        return;
    }
    if (n == "Assign") {
        const X target = at(e, "target"), value = at(e, "value");
        if (now->has("far") && !py::truthy(e.get("op"))) {
            Opt an = assigned_name(target);
            if (an && now->far.count(*an)) {
                X c = leading_cast(value);
                if (!c.is_none()) {
                    const X ct = c.get("type");
                    const X kind = py::truthy(ct) ? ct.get("kind") : X();
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
        const X target = e.get("target");
        const X args = e.get("args");
        operands(tok, {&target});
        st.op(&CALL, paren);
        if (py::truthy(target))
            push(st, target);
        else {
            const X sym = e.get("sym");
            std::string nm = py::truthy(sym) ? (sym.has("name") ? py::str_of(sym.get("name")) : "?") : "?";
            st.atom(nm);
        }
        std::vector<X> a = py::truthy(args) ? args.seq() : std::vector<X>();
        if (a.empty()) st.atom("", std::nullopt, true);
        for (size_t k = 1; k < a.size(); k++) st.op(binop(","));
        for (auto& x : a) push(st, x);
        return;
    }
    if (n == "Index") {
        const X arr = at(e, "arr"), index = at(e, "index");
        operands(tok, {&arr, &index});
        st.op(&SUBSCRIPT, paren);
        push(st, arr);
        push(st, index);
        return;
    }
    if (n == "Select") {
        bool a = arrow(e);
        const X obj = a ? at(at(e, "obj"), "x") : at(e, "obj");
        operands(tok, {&obj});
        st.op(member_op(a), paren);
        push(st, obj);
        st.atom(name_of(e));
        return;
    }
    throw Decline("expression " + n);
}

// ---------------------------------------------------------------------------------------------- conditions
namespace {

bool joined(const X& e) {
    return e.get("node") == "Binary" && (e.get("op") == "&&" || e.get("op") == "||") && at(e, "left").get("node") == "Paren" &&
           at(e, "right").get("node") == "Paren";
}

void listed(const X& e, std::vector<X>& out) {
    if (e.get("node") == "Binary" && e.get("op") == ",") {
        listed(at(e, "left"), out);
        out.push_back(at(e, "right"));
        return;
    }
    out.push_back(e);
}

}  // namespace

void cond(Em* em, const X& e, bool comma) {
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
    std::vector<X> items;
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

std::string condition(const X& e, bool inner) {
    return flat_text([&](Em* em) { cond(em, e, inner); });
}

// ---------------------------------------------------------------------------------------------- statements
namespace {

bool true_lit(const X& e) { return e.get("node") == "Lit" && e.get("value").is_bool() && e.get("value").b(); }

const ir::Type* case_pointer(const X& lab) {
    const X t = lab.get("type");
    if ((lab.get("node") == "Cast" || lab.get("node") == "Lit") && (t.get("kind") == "ptr" || t.get("kind") == "fnptr")) return t.as_type();
    return nullptr;
}

ir::Node* copy_node(const ir::Node* n) {
    ir::Node* out = now->arena.make<ir::Node>();
    *out = *n;
    return out;
}

// A switch on a pointer whose labels are numbers: the same switch on the pointer as a number (new nodes, as the Python's new dicts).
X integer_switch(const X& s) {
    std::set<int> widths;
    for (auto& c : stmts(s.get("cases")))
        for (auto& lab : stmts(c.get("labels"))) {
            const ir::Type* p = case_pointer(lab);
            if (p != nullptr) widths.insert(X::type(p).get("space") == "near" ? 2 : 4);
        }
    if (widths.size() != 1) return s;
    now->types.push_back(ir::prim_type(*widths.begin() == 2 ? ir::Prim::U16 : ir::Prim::U32));
    const ir::TypeP u = now->types.back();
    static const re::Regex NUM("0x[0-9a-fA-F]+|\\d+");
    auto number = [&](const X& lab) -> X {
        if (case_pointer(lab) == nullptr) return lab;
        const X l = lab.get("node") == "Cast" ? at(lab, "x") : lab;
        if (l.get("node") != "Lit" || !l.get("value").is_intlike()) throw Decline("a case label of a pointer type that is no number");
        ir::Node* out = copy_node(l.as_node());
        out->type = u;
        auto* k = ir::get<ir::Lit>(out);
        std::string text = k->text ? *k->text : "0";
        if (!NUM.fullmatch(text)) k->text.reset();
        return X::node(out);
    };
    ir::Node* out = copy_node(s.as_node());
    auto* sw = ir::get<ir::Switch>(out);
    sw->cases.clear();
    for (auto& c : stmts(s.get("cases"))) {
        ir::Node* nc = copy_node(c.as_node());
        auto* cs = ir::get<ir::Case>(nc);
        cs->labels.clear();
        for (auto& lab : stmts(c.get("labels"))) cs->labels.push_back(number(lab).as_node());
        sw->cases.push_back(nc);
    }
    ir::Cast cast;
    cast.x = at(s, "expr").as_node();
    sw->expr = ir::make(now->arena, std::move(cast), u);
    return X::node(out);
}

void inserted(Em* em, const X& s, bool before) {
    if (before && now->seeing && em->flat()) now->stmt_at[id(s)] = static_cast<Flat*>(em)->out.size();
    Strs* texts = before ? now->before.get(id(s)) : now->after.get(id(s));
    if (!texts) return;
    Strs copy = *texts;
    for (auto& t : copy) {
        em->tag_line();
        em->print("", t);
    }
}

}  // namespace

void block(Em* em, const X& ss) {
    std::vector<X> list = stmts(ss);
    for (auto& s : list) stmt(em, s);
    if (!list.empty()) {
        const X& last = list.back();
        if (last.is_dict() && last.get("node") == "Labeled" && stmts(last.get("body")).empty()) em->print(";");
    }
}

namespace {

void braced(Em* em, const X& ss) {
    em->spaces(1);
    em->start_indent();
    em->print("{");
    block(em, ss);
    em->stop_indent();
    em->tag_line();
    em->print("}");
}

}  // namespace

void comment(Em* em, const std::string& text, std::optional<int> indent) {
    em->tag_line(indent);
    em->start_comment();
    em->print("/* ");
    static const re::Regex W("[ \\t]+|\\n|\\r|\\{@[^}]*\\}?|[^\\s]+");
    for (auto& m : W.finditer(text)) {
        std::string w = m.group(0);
        if (w[0] == ' ' || w[0] == '\t') em->spaces(static_cast<int>(cp_len(w)));
        else if (w == "\n") em->tag_line();
        else if (w != "\r") em->print(w);
    }
    em->print(" */");
    em->stop_comment();
}

namespace {

void jump(Em* em, const X& s) {
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

void while_true(Em* em) {
    em->open_paren("(");
    em->spaces(1);
    em->print("true");
    em->spaces(1);
    em->close_paren(")");
}

void if_stmt(Em* em, const X& s, bool merged = false) {
    if (merged) em->spaces(1);
    else em->tag_line();
    em->print("if");
    em->spaces(1);
    cond(em, at(s, "cond"), false);
    const X then = s.get("then");
    if (then.is_dict()) {
        em->spaces(1);
        jump(em, then);
        return;
    }
    braced(em, then);
    const X orelse = s.get("orelse");
    if (orelse.is_none()) return;
    em->tag_line();
    em->print("else");
    if (orelse.is_list() && orelse.size() == 1 && at(orelse[0], "node") == "If") {
        if_stmt(em, orelse[0], true);
        return;
    }
    braced(em, orelse);
}

void statement(Em* em, const X& e, const char* word = nullptr) {
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

}  // namespace

void stmt(Em* em, const X& s0) {
    X s = s0;
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
        const X text = s.get("asm");
        for (const std::string& l : text.is_str() ? py::split(text.str(), "\n") : Strs()) {   // (the tree holds the lines as one text)
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

namespace {

void locals_into(const X& ss, std::vector<X>& found) {
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

}  // namespace

std::vector<X> locals_of(const X& body) {
    std::vector<X> found;
    locals_into(body, found);
    return found;
}

// ---------------------------------------------------------------------------------------------- the heading
const ir::FnType* fn_type_of(const X& doc) {
    const ir::Node* n = doc.as_node();
    const auto* m = n != nullptr ? ir::get<ir::MethodDecl>(n) : nullptr;
    if (m == nullptr || !m->sym || !m->sym->type) return nullptr;
    return std::get_if<ir::FnType>(&m->sym->type->body);
}

const ir::Type* ret_of(const X& doc) {
    const ir::Node* n = doc.as_node();
    const auto* m = n != nullptr ? ir::get<ir::MethodDecl>(n) : nullptr;
    if (m != nullptr && m->ret) return m->ret.get();
    // (the symbol's type's result: a function type's; another kind of type has none)
    if (m != nullptr && m->sym && m->sym->type) {
        if (const auto* ft = std::get_if<ir::FnType>(&m->sym->type->body); ft != nullptr && ft->result.type) return ft->result.type.get();
    }
    static const ir::TypeP void_t = ir::make_type(ir::VoidType{});
    return void_t.get();
}

Strs mods_of(const X& doc) {
    Strs out;
    const X m = doc.get("mods");
    if (py::truthy(m))
        for (auto& x : m.seq()) out.push_back(py::str_of(x));
    return out;
}

bool lost(const X& doc) {
    if (py::truthy(doc.get("params"))) return false;
    static const re::Regex F("__fastcall\\w+");
    for (auto& m : mods_of(doc))
        if (F.fullmatch(m)) return true;
    return false;
}

Heading heading(const X& doc) {
    const V& ctx = now->facts;
    const ir::Type* ret = ret_of(doc);
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

namespace {

bool is_byte(const std::string& b) {
    return b == "bool" || b == "char" || b == "int1" || b == "uint1" || b == "xunknown1" || b == "undefined1" || b == "undefined";
}

}  // namespace

std::string result_shown_base(const ir::Type* ret, bool& has) {
    TypeStack ts = type_stack(ret);
    has = true;
    if (ts.base == "void" && now->has("int_result")) return "int";
    if (ts.base == "void" && now->has("uint8_result")) {
        now->facts.set("novalue", V(true));
        return "uint8";
    }
    if (ts.base == "void" && now->has("void_result")) return "xunknown2";
    if (ts.mods.empty() && is_byte(ts.base) && now->has("byte_result") && !lost(now->doc))
        return ts.base == "char" || ts.base == "int1" ? "int2" : "uint2";
    has = false;
    return "";
}

namespace {

std::string register_arguments(const X& doc) {
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

}  // namespace

void signature_em(Em* em, const X& doc) {
    const ir::FnType* ft = fn_type_of(doc);
    em->open_group();
    em->open_group();
    const ir::Type* ret = ret_of(doc);
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
    std::vector<X> params = stmts(doc.get("params"));
    bool old_here = now->old_style && same(now->doc, doc);
    if (old_here) {
        Strs names;
        for (auto& nd : *now->old_style) names.push_back(nd.first);
        if (!names.empty()) em->print(py::join(",", names));
        params.clear();
        ft = nullptr;
    } else if (params.empty())
        em->print("void");
    for (size_t i = 0; i < params.size(); i++) {
        if (i) em->print(",");
        em->open_group();
        std::string pn = param_name(params[i]);
        put_type(em, params[i].get("type"), pn, now->far.count(pn) != 0);
        em->close_group();
    }
    if (ft != nullptr && ft->variadic) {
        if (!params.empty()) em->print(",");
        em->print("...");
    }
    em->close_paren(")");
    em->close_group();
    em->close_group();
}

std::string signature(const X& doc) {
    return flat_text([&](Em* em) { signature_em(em, doc); });
}

Opt segment_register(const std::string& name, const X& t) {
    static const re::Regex R("unaff_(SS|DS|CS)"), W("\\w+");
    if (!now->has("segment_registers")) return std::nullopt;
    re::Match m = R.fullmatch(name);
    if (!m || !W.fullmatch(type_text(t))) return std::nullopt;
    return name + " = codebreaker_" + py::lower(m.group(1)) + "()";
}

std::string function_text(const X& doc, const std::vector<X>& locs0, Em* em0) {
    std::vector<X> notes;
    if (em0 || !py::truthy(now->facts.get("without_notes"))) {
        const X c = doc.get("comments");
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
    const std::vector<X>* locs = &locs0;
    std::vector<X> none;
    if (now->locals_text) {
        em->print("", *now->locals_text);
        locs = &none;
    }
    for (auto& loc : *locs) {
        const X sym = loc.get("sym");
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

}  // namespace cb::translate::ctree
