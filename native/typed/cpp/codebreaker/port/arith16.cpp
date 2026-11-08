// codebreaker/port/arith16.cpp -- typed C++ of codebreaker.port.arith16.

#include "codebreaker/port/arith16.hpp"

#include <algorithm>
#include <functional>

#include "codebreaker/base/cache.hpp"
#include "json.hpp"
#include "py_subprocess.hpp"

namespace codebreaker::port::arith16 {

py::dict<py::str, py::str> W16 = {{"int16_t", "i16"},  {"short", "i16"},         {"uint16_t", "u16"},       {"unsigned short", "u16"},
                                  {"char", "i16"},     {"signed char", "i16"},   {"unsigned char", "i16"},  {"uint8_t", "i16"},
                                  {"int8_t", "i16"},   {"int32_t", "i32"},       {"long", "i32"},           {"uint32_t", "u32"},
                                  {"unsigned long", "u32"}};
py::set<py::str> ARITH = {"+", "-", "*", "<<", "&", "|", "^"};
py::set<py::str> TESTS = {"<", ">", "<=", ">=", "==", "!=", "/", "%", ">>"};
py::dict<py::str, py::str> CAST = {{"u16", "uint16_t"}, {"i16", "int16_t"}};

std::optional<py::str> MAIN;
std::optional<py::str> TEXT;

namespace {

// (an optional str is the given one)
bool is_(const std::optional<py::str>& a, const char* s) {
    return a.has_value() && *a == py::str(s);
}

// W16.get(x)
std::optional<py::str> w16(const std::optional<py::str>& k) {
    if (!k.has_value() || !W16.contains(*k)) {
        return std::nullopt;
    }
    return W16.at(*k);
}

// n["type"].get("qualType")  (KeyError when the node has no "type")
std::optional<py::str> qual(const Node& n) {
    if (!n.type.has_value()) {
        throw py::KeyError("'type'");
    }
    return n.type->qualType;
}

// int(n.get("value", 0))
int64_t value_int(const std::optional<Number>& v) {
    if (!v.has_value()) {
        return 0;
    }
    if (const int64_t* i = std::get_if<int64_t>(&*v)) {
        return *i;
    }
    return py::int_parse(std::get<py::str>(*v));
}

// {} -- what `(n.get("inner") or [{}])[0]` is when there is no inner
const Node& empty_node() {
    static const Node e;
    return e;
}

bool in_(const std::optional<py::str>& a, std::initializer_list<const char*> xs) {
    for (const char* x : xs) {
        if (is_(a, x)) {
            return true;
        }
    }
    return false;
}

// ---- the JSON of clang's tree as records
Loc loc_of(const py::V& v) {
    Loc l;
    if (!v.is_dict()) {
        return l;
    }
    const py::V& off = v.get("offset");
    if (off.is_int()) {
        l.offset = off.as_int();
    }
    const py::V& tok = v.get("tokLen");
    if (tok.is_int()) {
        l.tokLen = tok.as_int();
    }
    const py::V& file = v.get("file");
    if (file.is_str()) {
        l.file = py::str(file.str());
    }
    if (v.get("spellingLoc").is_dict()) {
        l.spellingLoc = std::make_shared<Loc>(loc_of(v.get("spellingLoc")));
    }
    if (v.get("expansionLoc").is_dict()) {
        l.expansionLoc = std::make_shared<Loc>(loc_of(v.get("expansionLoc")));
    }
    if (v.get("includedFrom").is_dict()) {
        l.includedFrom = std::make_shared<Loc>(loc_of(v.get("includedFrom")));
    }
    return l;
}

std::optional<py::str> json_str(const py::V& v) {
    if (v.is_str()) {
        return py::str(v.str());
    }
    return std::nullopt;
}

Node node_of(const py::V& v) {
    Node n;
    n.kind = json_str(v.get("kind"));
    n.opcode = json_str(v.get("opcode"));
    n.castKind = json_str(v.get("castKind"));
    if (v.get("type").is_dict()) {
        TypeRef t;
        t.qualType = json_str(v.get("type").get("qualType"));
        n.type = t;
    }
    const py::V& val = v.get("value");
    if (val.is_intlike()) {
        n.value = Number(val.as_int());
    } else if (val.is_str()) {
        n.value = Number(py::str(val.str()));
    }
    n.loc = loc_of(v.get("loc"));
    if (v.get("range").is_dict()) {
        n.range.begin = loc_of(v.get("range").get("begin"));
        n.range.end = loc_of(v.get("range").get("end"));
    }
    if (v.get("inner").is_list()) {
        for (const py::V& x : v.get("inner").seq()) {
            if (x.is_dict()) {
                n.inner.append(node_of(x));
            }
        }
    }
    return n;
}

// mark_files on a location: it is the value of a key of its parent
void mark_loc(Loc& v, std::optional<py::str>& cur) {
    if (v.offset.has_value() || v.file.has_value() || v.spellingLoc) {
        if (v.file.has_value()) {
            cur = v.file;
        }
        v._file = cur;
    }
    if (v.spellingLoc) {
        mark_loc(*v.spellingLoc, cur);
    }
    if (v.expansionLoc) {
        mark_loc(*v.expansionLoc, cur);
    }
    if (v.includedFrom) {
        mark_loc(*v.includedFrom, cur);
    }
}

void files_loc(const Loc& l, py::list<py::str>& seen) {
    if (l.file.has_value() && !seen.contains(*l.file)) {
        seen.append(*l.file);
    }
    if (l.spellingLoc) {
        files_loc(*l.spellingLoc, seen);
    }
    if (l.expansionLoc) {
        files_loc(*l.expansionLoc, seen);
    }
    if (l.includedFrom) {
        files_loc(*l.includedFrom, seen);
    }
}

}  // namespace

std::optional<Node> dump(const py::str& cc, const py::str& src, const py::str& folder, const py::list<py::str>& extra) {
    py::list<py::str> args{cc,
                           py::str("-fsyntax-only"),
                           py::str("-Xclang"),
                           py::str("-ast-dump=json"),
                           py::str("-fsigned-char"),
                           py::str("-std=c11"),
                           py::str("-D_CRT_SECURE_NO_WARNINGS"),
                           py::str("-D_CRT_NONSTDC_NO_WARNINGS"),
                           py::str("-I.")};
    args = args + extra + py::list<py::str>{src};
    py::subprocess::RunOptions opts;
    opts.cwd = folder;
    opts.text = false;
    py::subprocess::CompletedProcess p = py::subprocess::run(args, opts);
    try {
        py::V doc = py::json_parse(p.stdout_bytes->decode("utf-8", "replace").utf8());
        if (!doc.is_dict()) {
            return std::nullopt;
        }
        return node_of(doc);
    } catch (const py::PyError& e) {
        if (e.type == "ValueError") {
            return std::nullopt;
        }
        throw;
    } catch (const py::ValueError&) {
        return std::nullopt;
    }
}

std::optional<py::str> old_type(const Node& n, const py::str& text) {
    const std::optional<py::str>& k = n.kind;
    const py::list<Node>& inner = n.inner;
    if (is_(k, "ParenExpr") || (is_(k, "ImplicitCastExpr") && (is_(n.castKind, "LValueToRValue") || is_(n.castKind, "NoOp")))) {
        return py::truthy(inner) ? old_type(inner[0], text) : std::nullopt;
    }
    if (is_(k, "ImplicitCastExpr") && is_(n.castKind, "IntegralCast")) {
        std::optional<py::str> t = qual(n);
        if (is_(t, "int")) {   // (a promotion: what it was, made an int of 16 bits)
            return py::truthy(inner) ? old_type(inner[0], text) : std::nullopt;
        }
        return w16(t);
    }
    if (is_(k, "IntegerLiteral")) {
        std::optional<py::str> sp = spelled(n, text);
        py::str s = (sp.has_value() && py::truthy(*sp)) ? *sp : py::str("");
        if (!py::re::fullmatch(py::str("(?:0[xX][0-9a-fA-F]+|\\d+)[uUlL]*"), s)) {
            s = py::str("");                          // (a macro's name, not the number's own spelling)
        }
        int64_t v = value_int(n.value);
        if (py::re::search(py::str("[lL]$"), s) || v > 0xFFFF) {
            return py::str("i32");
        }
        if (v > 0x7FFF) {
            return py::re::match(py::str("0"), s) ? py::str("u16") : py::str("i32");   // (0177777, 0xFFFF unsigned; 65535 a long)
        }
        return py::str("i16");
    }
    if (is_(k, "CharacterLiteral")) {
        return py::str("i16");
    }
    if (is_(k, "CStyleCastExpr")) {
        return w16(qual(n));
    }
    if (is_(k, "BinaryOperator")) {
        const std::optional<py::str>& op = n.opcode;
        if (in_(op, {"<", ">", "<=", ">=", "==", "!=", "&&", "||"})) {
            return py::str("i16");
        }
        if (is_(op, ",")) {
            return py::len(inner) > 1 ? old_type(inner[1], text) : std::nullopt;
        }
        if (is_(op, "=")) {
            return py::truthy(inner) ? old_type(inner[0], text) : std::nullopt;
        }
        std::optional<py::str> a, b;
        if (py::len(inner) >= 2) {
            a = old_type(inner[0], text);
            b = old_type(inner[1], text);
        }
        if (in_(op, {"<<", ">>"})) {
            return a;
        }
        return common(a, b);
    }
    if (is_(k, "CompoundAssignOperator")) {
        return py::truthy(inner) ? old_type(inner[0], text) : std::nullopt;
    }
    if (is_(k, "UnaryOperator")) {
        const std::optional<py::str>& op = n.opcode;
        if (is_(op, "!")) {
            return py::str("i16");
        }
        if (in_(op, {"-", "~", "+", "++", "--"})) {
            return py::truthy(inner) ? old_type(inner[0], text) : std::nullopt;
        }
        if (is_(op, "*")) {
            return w16(qual(n));
        }
        return std::nullopt;
    }
    if (is_(k, "ConditionalOperator")) {
        return py::len(inner) >= 3 ? common(old_type(inner[1], text), old_type(inner[2], text)) : std::nullopt;
    }
    if (in_(k, {"DeclRefExpr", "MemberExpr", "ArraySubscriptExpr", "CallExpr"})) {
        return w16(qual(n));
    }
    return std::nullopt;
}

std::optional<py::str> common(const std::optional<py::str>& a, const std::optional<py::str>& b) {
    if (!a.has_value() || !b.has_value()) {
        return std::nullopt;
    }
    if (*a == py::str("u32") || *b == py::str("u32")) {
        return py::str("u32");
    }
    if (*a == py::str("i32") || *b == py::str("i32")) {
        return py::str("i32");
    }
    if (*a == py::str("u16") || *b == py::str("u16")) {
        return py::str("u16");
    }
    return py::str("i16");
}

std::optional<std::tuple<int64_t, int64_t, std::optional<py::str>>> place(const Node& n) {
    const Loc& b = n.range.begin;
    const Loc& e = n.range.end;
    if (b.expansionLoc && e.expansionLoc) {   // (all of it from one use of a macro: that use)
        const Loc& x = *b.expansionLoc;
        const Loc& y = *e.expansionLoc;
        if (x.offset.has_value() && x.offset == y.offset) {
            std::optional<py::str> f = (x._file.has_value() && py::truthy(*x._file)) ? x._file : b._file;
            int64_t end = *x.offset + x.tokLen.value_or(0);
            if (TEXT.has_value() && py::re::match(py::str("\\s*\\("), TEXT->slice(end, end + 40))) {
                return std::nullopt;                // (a macro with arguments - assert(...) - not a constant)
            }
            return std::make_tuple(*x.offset, end, f);
        }
        return std::nullopt;
    }
    if (!b.offset.has_value() || !e.offset.has_value() || b.spellingLoc || e.spellingLoc) {
        return std::nullopt;
    }
    if (b._file != e._file || !b._file.has_value()) {
        return std::nullopt;
    }
    return std::make_tuple(*b.offset, *e.offset + e.tokLen.value_or(0), b._file);
}

std::optional<py::str> spelled(const Node& n, const py::str& text) {
    auto p = place(n);
    if (p.has_value() && std::get<2>(*p) == MAIN) {
        return text.slice(std::get<0>(*p), std::get<1>(*p));
    }
    return std::nullopt;
}

void mark_files(Node& node, std::optional<py::str>& cur) {
    mark_loc(node.loc, cur);
    mark_loc(node.range.begin, cur);
    mark_loc(node.range.end, cur);
    for (Node& x : node.inner.vec()) {
        mark_files(x, cur);
    }
}

const Node& bare(const Node& n0) {
    const Node* n = &n0;
    while (is_(n->kind, "ParenExpr") ||
           (is_(n->kind, "ImplicitCastExpr") && (is_(n->castKind, "LValueToRValue") || is_(n->castKind, "NoOp") || is_(n->castKind, "IntegralCast")))) {
        n = py::truthy(n->inner) ? &n->inner[0] : &empty_node();
    }
    return *n;
}

bool nonneg(const Node& n0) {
    const Node& n = bare(n0);
    return in_(n.kind, {"IntegerLiteral", "CharacterLiteral"}) && value_int(n.value) >= 0;
}

const Node* compound(const Node& n0, const std::optional<py::str>& text) {
    const Node& n = bare(n0);
    if (is_(n.kind, "BinaryOperator") && n.opcode.has_value() && ARITH.contains(*n.opcode)) {
        if (in_(n.opcode, {"&", "|", "^"})) {   // (bits of two 16-bit values stay 16 bits - unless a
            py::str t = text.has_value() && py::truthy(*text) ? *text : py::str("");   // signed one meets an unsigned)
            std::vector<std::optional<py::str>> ts;
            for (int64_t i = 0; i < py::len(n.inner) && i < 2; ++i) {
                ts.push_back(old_type(n.inner[i], t));
            }
            bool has_u16 = false, mixed = false;
            for (const auto& x : ts) {
                if (is_(x, "u16")) {
                    has_u16 = true;
                }
            }
            for (size_t i = 0; i < ts.size(); ++i) {
                if (is_(ts[i], "i16") && !nonneg(n.inner[static_cast<int64_t>(i)])) {
                    mixed = true;
                }
            }
            if (!(has_u16 && mixed)) {
                return nullptr;
            }
        }
        return &n;
    }
    if (is_(n.kind, "UnaryOperator") && in_(n.opcode, {"-", "~"}) &&
        !in_(bare(py::truthy(n.inner) ? n.inner[0] : empty_node()).kind, {"IntegerLiteral", "CharacterLiteral"})) {
        return &n;
    }
    return nullptr;
}

py::list<std::tuple<int64_t, py::str>> edits_of(const Node& tree, const py::str& text, const py::str& path) {
    MAIN = path;
    TEXT = text;
    py::list<std::tuple<int64_t, py::str>> out;

    auto cast = [&](const Node& n, const py::str& t, bool wrap) {
        auto p = place(n);
        if (!p.has_value() || std::get<2>(*p) != path) {
            return;
        }
        py::str s = text.slice(std::get<0>(*p), std::get<1>(*p));
        if (s.startswith(py::percent("(%s)", CAST.at(t)))) {
            return;
        }
        if (wrap) {
            out.append(std::make_tuple(std::get<0>(*p), py::percent("(%s)(", CAST.at(t))));
            out.append(std::make_tuple(std::get<1>(*p), py::str(")")));
        } else {
            out.append(std::make_tuple(std::get<0>(*p), py::percent("(%s)", CAST.at(t))));
        }
    };

    std::function<void(const Node&)> walk = [&](const Node& n) {
        const py::list<Node>& inner = n.inner;
        const std::optional<py::str>& k = n.kind;
        if (is_(k, "BinaryOperator") && n.opcode.has_value() && TESTS.contains(*n.opcode) && py::len(inner) >= 2) {
            std::optional<py::str> a = old_type(inner[0], text);
            std::optional<py::str> b = old_type(inner[1], text);
            std::optional<py::str> c = is_(n.opcode, ">>") ? a : common(a, b);
            if (is_(c, "u16") || is_(c, "i16")) {
                for (int64_t i = 0; i < 2; ++i) {
                    const Node& x = inner[i];
                    const std::optional<py::str>& t = i == 0 ? a : b;
                    if (is_(n.opcode, ">>") && i == 1) {
                        continue;
                    }
                    const Node* y = compound(x, text);
                    if (is_(c, "u16") && is_(t, "i16") && !nonneg(x)) {   // (the 8086 made it unsigned; here it
                        cast(y != nullptr ? *y : x, py::str("u16"), y != nullptr || !simple(x, text));   //  stays signed)
                    } else if (y != nullptr) {                           // (16 bits of arithmetic, cut as the 8086's were)
                        cast(*y, *c, true);
                    }
                }
            }
        }
        if ((is_(k, "ImplicitCastExpr") || is_(k, "CStyleCastExpr")) && is_(n.castKind, "IntegralCast") && py::truthy(inner)) {
            std::optional<py::str> to = w16(qual(n));
            if (is_(to, "i32") || is_(to, "u32")) {
                const Node* y = compound(inner[0], text);
                std::optional<py::str> t = old_type(inner[0], text);
                if (y != nullptr && (is_(t, "u16") || is_(t, "i16"))) {   // (16-bit arithmetic made a long: cut first)
                    cast(*y, *t, true);
                }
            }
        }
        for (const Node& x : inner.vec()) {
            walk(x);
        }
    };

    for (const Node& top : tree.inner.vec()) {
        walk(top);
    }
    return out;
}

bool simple(const Node& n, const py::str& text) {
    std::optional<py::str> sp = spelled(n, text);
    py::str s = (sp.has_value() && py::truthy(*sp)) ? *sp : py::str("");
    return py::re::fullmatch(py::str("[\\w.\\->\\[\\]]+|-?\\d+|0[xX][0-9a-fA-F]+"), s.strip()).has_value();
}

py::str apply(const py::str& text0, const py::list<std::tuple<int64_t, py::str>>& edits) {
    // set(edits): each edit once. (The Python's order of equal keys is that of a set of strings -- hash
    // dependent; here the first one met comes first.)
    std::vector<std::tuple<int64_t, py::str>> uniq;
    for (const auto& e : edits) {
        bool seen = false;
        for (const auto& u : uniq) {
            if (u == e) {
                seen = true;
                break;
            }
        }
        if (!seen) {
            uniq.push_back(e);
        }
    }
    // sorted(..., key=lambda e: (e[0], e[1] != ")"), reverse=True)
    std::stable_sort(uniq.begin(), uniq.end(), [](const std::tuple<int64_t, py::str>& a, const std::tuple<int64_t, py::str>& b) {
        int64_t ka = std::get<0>(a), kb = std::get<0>(b);
        if (ka != kb) {
            return ka > kb;
        }
        bool ba = std::get<1>(a) != py::str(")"), bb = std::get<1>(b) != py::str(")");
        return ba && !bb;
    });
    py::str text = text0;
    for (const auto& [at, s] : uniq) {
        text = text.slice(0, at) + s + text.slice(at, py::len(text));
    }
    return text;
}

py::str _headers_key(const py::str& folder) {
    py::hashlib::Hash h = py::hashlib::sha256();
    for (const py::str& n : py::sorted(py::os::listdir(folder))) {
        if (n.endswith(py::str(".h"))) {
            h.update(n.encode("utf-8", "strict") + py::bytes("\000", 1) + py::open_rb(py::os::path::join(folder, n)).read());
        }
    }
    return h.hexdigest();
}

py::str reach_key(const py::str& folder, const py::str& f) {
    py::hashlib::Hash h = py::hashlib::sha256();
    py::list<py::str> todo{f};
    py::set<py::str> seen;
    while (py::truthy(todo)) {
        py::str x = todo.pop();
        if (seen.contains(x)) {
            continue;
        }
        seen.add(x);
        py::bytes t;
        try {
            t = py::open_rb(py::os::path::join(folder, x)).read();
        } catch (const py::OSError&) {
            continue;
        }
        h.update(x.encode("utf-8", "strict") + py::bytes("\000", 1) + t + py::bytes("\000", 1));
        // (the pattern is a bytes pattern in the Python: ASCII whitespace, latin-1 text)
        for (const py::str& m : py::re::findall(py::str("#\\s*include\\s*[<\"]([^>\"]+)[>\"]"), t.decode("latin1", "strict"), py::re::ASCII)) {
            todo.append(m);
        }
    }
    return h.hexdigest();
}

py::list<py::str> _files(const Node& node, const std::optional<py::list<py::str>>& seen0) {
    py::list<py::str> seen = seen0.has_value() ? *seen0 : py::list<py::str>();
    files_loc(node.loc, seen);
    files_loc(node.range.begin, seen);
    files_loc(node.range.end, seen);
    for (const Node& x : node.inner.vec()) {
        _files(x, seen);
    }
    return seen;
}

const py::str CODE_VERSION("arith16-cpp-1");

py::str code_key(const py::str& kind) {
    py::hashlib::Hash h = py::hashlib::sha256(kind.encode("utf-8", "strict"));
    h.update(py::bytes("\000", 1) + CODE_VERSION.encode("utf-8", "strict"));
    return h.hexdigest();
}

py::str pass_key(const py::str& cc, const py::str& folder, const py::str& f, const py::str& kind, const py::list<py::str>& extra,
                 const std::optional<py::str>& /*headers*/) {
    py::hashlib::Hash h = py::hashlib::sha256();
    h.update(code_key(kind).encode("utf-8", "strict"));
    // repr((os.path.abspath(cc), kind, extra)) -- extra a tuple of strs
    py::list<py::str> parts;
    for (const py::str& x : extra) {
        parts.append(py::repr(x));
    }
    py::str extra_repr = py::len(parts) == 0 ? py::str("()")
                         : py::len(parts) == 1 ? py::str("(") + parts[0] + py::str(",)")
                                               : py::str("(") + py::join(py::str(", "), parts) + py::str(")");
    h.update(py::percent("(%s, %s, %s)", py::repr(py::os::path::abspath(cc)), py::repr(kind), extra_repr).encode("utf-8", "strict"));
    h.update(reach_key(folder, f).encode("utf-8", "strict"));
    return h.hexdigest();
}

py::dict<py::str, py::str> run(const py::str& cc, const py::str& folder, const py::list<py::str>& files) {
    py::dict<py::str, py::str> out;
    py::str hk = _headers_key(folder);
    for (const py::str& f : files) {
        py::str key = pass_key(cc, folder, f, py::str("arith16"), py::list<py::str>(), hk);
        auto got = base::cache::value<py::dict<py::str, std::optional<py::str>>>(py::str("port-passes"), key);
        if (got.has_value()) {
            auto nw = got->get(py::str("new"));
            if (nw.has_value() && nw->has_value()) {
                out.set(f, **nw);
            }
            continue;
        }
        std::optional<Node> tree = dump(cc, f, folder);
        if (!tree.has_value()) {
            continue;
        }
        std::optional<py::str> cur;
        mark_files(*tree, cur);
        py::str path = f;
        for (const py::str& v : _files(*tree)) {
            if (v.replace(py::str("\\"), py::str("/")).endswith(f)) {
                path = v;
                break;
            }
        }
        py::file fh = py::open(folder + py::str("/") + f, py::str("r"), py::str("latin1"));
        py::str text = fh.read_text();
        fh.close();
        auto e = edits_of(*tree, text, path);
        std::optional<py::str> nw = py::truthy(e) ? std::optional<py::str>(arith16::apply(text, e)) : std::nullopt;
        if (nw.has_value()) {
            out.set(f, *nw);
        }
        py::dict<py::str, std::optional<py::str>> kept;
        kept.set(py::str("new"), nw);
        base::cache::keep(py::str("port-passes"), key, kept);
    }
    return out;
}
}  // namespace codebreaker::port::arith16
