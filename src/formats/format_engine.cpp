#include "formats/format_engine.hpp"

#include <algorithm>
#include <cctype>
#include <format>
#include <set>

#include "core/error.hpp"
#include "core/fs.hpp"

namespace cb::formats::engine {

// ---- the tree ----------------------------------------------------------------------------------------------

const Node* Node::child(std::string_view child_name) const {
    for (const Node& c : children)
        if (c.name == child_name) return &c;
    return nullptr;
}

Node* Node::child(std::string_view child_name) {
    for (Node& c : children)
        if (c.name == child_name) return &c;
    return nullptr;
}

const Node* Node::find(std::string_view path) const {
    const Node* at = this;
    std::size_t i = 0;
    while (i < path.size() && at) {
        if (path[i] == '.') {
            ++i;
            continue;
        }
        if (path[i] == '[') {
            const std::size_t close = path.find(']', i);
            if (close == std::string_view::npos || at->kind != Kind::Array) return nullptr;
            std::size_t index = 0;
            for (std::size_t k = i + 1; k < close; ++k) {
                if (path[k] < '0' || path[k] > '9') return nullptr;
                index = index * 10 + static_cast<std::size_t>(path[k] - '0');
            }
            if (index >= at->children.size()) return nullptr;
            at = &at->children[index];
            i = close + 1;
            continue;
        }
        std::size_t e = i;
        while (e < path.size() && path[e] != '.' && path[e] != '[') ++e;
        at = at->child(path.substr(i, e - i));
        i = e;
    }
    return at;
}

const Node& Node::get(std::string_view path) const {
    const Node* n = find(path);
    if (!n) throw FormatError("format-engine", offset, std::format("no node '{}' under '{}'", path, name));
    return *n;
}

Node& Node::get(std::string_view path) { return const_cast<Node&>(static_cast<const Node*>(this)->get(path)); }

std::int64_t Node::as_signed() const { return static_cast<std::int64_t>(value); }
std::int64_t Node::i(std::string_view path) const { return get(path).as_signed(); }

std::string Node::str(std::string_view path) const {
    const Node& n = get(path);
    const std::size_t z = n.text.find('\0');
    return z == std::string::npos ? n.text : n.text.substr(0, z);
}

void set_text(Node& node, std::string_view text) {
    if (node.kind != Kind::Str && node.kind != Kind::Bytes) throw UsageError("set_text: '" + node.name + "' is not a string");
    if (text.size() != node.size) throw UsageError("set_text: '" + node.name + "' covers " + std::to_string(node.size) + " bytes");
    node.text.assign(text);
    node.edited = true;
}

void set_bytes(Node& node, ByteView data) { set_text(node, as_text(data)); }

void walk(const Node& node, const std::function<void(const std::string&, const Node&)>& visit, const std::string& prefix) {
    visit(prefix, node);   // `prefix` is this node's own path ("" for the root)
    for (const Node& c : node.children) {
        if (node.kind == Kind::Array) walk(c, visit, prefix + "[" + c.name + "]");
        else walk(c, visit, prefix.empty() ? c.name : prefix + "." + c.name);
    }
}

namespace {

void leaves(const Node& n, std::vector<const Node*>& out) {
    if (n.kind == Kind::Int || n.kind == Kind::Str || n.kind == Kind::Bytes) out.push_back(&n);
    for (const Node& c : n.children) leaves(c, out);
}

}  // namespace

std::uint64_t covered_bytes(const Tree& tree) {
    std::vector<const Node*> all;
    leaves(tree.root, all);
    std::vector<std::pair<std::uint64_t, std::uint64_t>> spans;
    for (const Node* n : all) spans.emplace_back(n->offset, n->offset + n->size);
    std::sort(spans.begin(), spans.end());
    std::uint64_t total = 0, reach = 0;
    for (const auto& [from, to] : spans) {
        const std::uint64_t start = std::max(from, reach);
        if (to > start) total += to - start;
        reach = std::max(reach, to);
    }
    return total;
}

Bytes write(const Tree& tree, bool covered_only) {
    Bytes out = covered_only ? Bytes(tree.source.size(), 0) : tree.source;
    std::vector<const Node*> all;
    leaves(tree.root, all);
    for (const Node* n : all) {
        if (n->offset + n->size > out.size()) out.resize(n->offset + n->size, 0);
        std::uint8_t* at = out.data() + n->offset;
        if (n->kind == Kind::Int) {
            for (unsigned k = 0; k < n->width; ++k) {
                const unsigned shift = 8 * (n->big_endian ? n->width - 1 - k : k);
                at[k] = static_cast<std::uint8_t>((n->value >> shift) & 0xFF);
            }
        } else if (n->kind == Kind::Str || n->edited) {
            if (n->text.size() != n->size) throw FormatError("format-engine", n->offset, "'" + n->name + "' changed its length");
            std::copy(n->text.begin(), n->text.end(), at);
        } else if (covered_only) {
            std::copy_n(tree.source.begin() + static_cast<std::ptrdiff_t>(n->offset), n->size, at);
        }
    }
    return out;
}

// ---- dump --------------------------------------------------------------------------------------------------

namespace {

std::string quote(std::string_view s) {
    std::string out = "\"";
    for (unsigned char c : s) {
        if (c >= 0x20 && c < 0x7F && c != '"' && c != '\\') out += static_cast<char>(c);
        else out += std::format("\\x{:02x}", c);
    }
    return out + "\"";
}

void dump_node(const Tree& tree, const Node& n, const std::string& label, int depth, std::size_t max_elements, std::string& out) {
    out.append(static_cast<std::size_t>(depth) * 2, ' ');
    out += label;
    if (n.kind == Kind::Let) {
        out += std::format(" = {} (0x{:x})\n", n.as_signed(), n.value);
        return;
    }
    out += std::format(" @0x{:x} +{}", n.offset, n.size);
    switch (n.kind) {
        case Kind::Int:
            out += std::format(" : {} = {}", n.type, n.is_signed ? std::to_string(n.as_signed()) : std::to_string(n.value));
            if (n.width > 1) out += std::format(" (0x{:x})", n.value);
            if (!n.label.empty()) out += " [" + n.label + "]";
            break;
        case Kind::Str: out += " : str " + quote(n.text.substr(0, 40)); break;
        case Kind::Bytes: {
            out += " : bytes";
            const Bytes* src = &tree.source;
            for (std::uint64_t k = 0; k < std::min<std::uint64_t>(n.size, 8) && n.offset + k < src->size(); ++k)
                out += std::format(" {:02x}", n.edited ? static_cast<std::uint8_t>(n.text[k]) : (*src)[n.offset + k]);
            if (n.size > 8) out += " ...";
            break;
        }
        case Kind::Struct: out += " : " + n.type; break;
        case Kind::Array: out += std::format(" : [{}]", n.children.size()); break;
        default: break;
    }
    out += '\n';
    std::size_t shown = 0;
    for (const Node& c : n.children) {
        if (n.kind == Kind::Array && shown++ >= max_elements) {
            out.append(static_cast<std::size_t>(depth + 1) * 2, ' ');
            out += std::format("... {} more\n", n.children.size() - max_elements);
            break;
        }
        dump_node(tree, c, n.kind == Kind::Array ? "[" + c.name + "]" : c.name, depth + 1, max_elements, out);
    }
}

}  // namespace

std::string dump(const Tree& tree, std::size_t max_elements) {
    std::string out;
    dump_node(tree, tree.root, tree.format, 0, max_elements, out);
    return out;
}

// ---- the description ---------------------------------------------------------------------------------------

namespace desc {

struct Expr;
using ExprP = std::shared_ptr<const Expr>;
struct Expr {
    enum class Op { Num, Name, Special, Unary, Binary, Cond, Call, Member, Index };
    Op op = Op::Num;
    std::int64_t num = 0;
    std::string name;          // a Name, a Special, a Member, a Call's function; a Unary/Binary's operator
    std::vector<ExprP> kids;
};

struct Literal {
    bool is_text = false;
    std::string text;
    std::uint64_t num = 0;
};

struct TypeRef {
    enum class K { Int, Str, Bytes, Struct } k = K::Int;
    unsigned width = 0;
    bool big = false, sgn = false, endian_set = false;
    ExprP len;
    std::string name;
};

enum class Arr { None, Count, Until, Size };

struct FieldDef {
    std::string name;
    ExprP at;
    TypeRef type;
    Arr arr = Arr::None;
    ExprP arr_expr;
    std::string as;
    std::vector<Literal> magic;
    ExprP window;
    ExprP when;
};

struct Stmt {
    enum class K { Field, Let, If, Assert } k = K::Field;
    FieldDef field;
    std::string let_name;
    std::string message;   // an Assert's own words for a failure (`assert expr : "text"`)
    ExprP expr;
    std::vector<Stmt> body, else_body;
    std::uint32_t line = 0;
};

struct StructDef {
    std::string name;
    std::vector<Stmt> body;
};

struct EnumDef {
    std::string name;
    bool flags = false;
    std::vector<std::pair<std::string, std::uint64_t>> items;
};

}  // namespace desc

class Format {
public:
    std::string name, origin;
    bool big = false;
    std::string root = "File";
    std::vector<std::string> uses;
    std::map<std::string, desc::StructDef, std::less<>> structs;
    std::map<std::string, desc::EnumDef, std::less<>> enums;
};

namespace {

using namespace desc;

// ---- lexer ----

enum class T { End, Ident, Num, Text, Special, Punct };

struct Token {
    T t = T::End;
    std::string s;
    std::uint64_t n = 0;
    std::uint32_t line = 0, col = 0;
};

class Parser {
public:
    Parser(std::string_view text, std::string_view origin) : origin_(origin) { lex(text); }

    std::shared_ptr<Format> parse() {
        auto f = std::make_shared<Format>();
        f->origin = origin_;
        while (peek().t != T::End) {
            const Token& t = peek();
            if (t.t != T::Ident) fail(t, "expected format, endian, root, use, struct, enum or flags");
            const std::string kw = t.s;
            next();
            if (kw == "format") {
                f->name = ident("a format name");
            } else if (kw == "endian") {
                const std::string e = ident("le or be");
                if (e == "le" || e == "little") f->big = false;
                else if (e == "be" || e == "big") f->big = true;
                else fail(prev(), "endian is le or be");
            } else if (kw == "root") {
                f->root = ident("a struct name");
            } else if (kw == "use") {
                f->uses.push_back(ident("a format name"));
            } else if (kw == "struct") {
                StructDef d;
                d.name = ident("a struct name");
                expect("{");
                block(d.body);
                f->structs[d.name] = std::move(d);
            } else if (kw == "enum" || kw == "flags") {
                EnumDef e;
                e.flags = kw == "flags";
                e.name = ident("a name");
                expect("{");
                while (!punct("}")) {
                    const std::string item = ident("an item name");
                    expect("=");
                    e.items.emplace_back(item, number());
                    if (punct(",")) next();
                }
                next();
                f->enums[e.name] = std::move(e);
            } else {
                fail(prev(), "unknown top-level word '" + kw + "'");
            }
        }
        if (f->name.empty()) throw ParseError(origin_, 1, 1, "the description has no `format name` line");
        return f;
    }

private:
    std::string origin_;
    std::vector<Token> toks_;
    std::size_t i_ = 0;

    [[noreturn]] void fail(const Token& t, const std::string& what) const { throw ParseError(origin_, t.line, t.col, what); }

    void lex(std::string_view s) {
        std::uint32_t line = 1, col = 1;
        std::size_t i = 0;
        auto adv = [&](std::size_t k = 1) {
            for (std::size_t j = 0; j < k && i < s.size(); ++j, ++i) {
                if (s[i] == '\n') ++line, col = 1;
                else ++col;
            }
        };
        while (i < s.size()) {
            const char c = s[i];
            if (c == '\n' || c == ' ' || c == '\t' || c == '\r' || c == ';') {
                adv();
                continue;
            }
            if (c == '#' || (c == '/' && i + 1 < s.size() && s[i + 1] == '/')) {
                while (i < s.size() && s[i] != '\n') adv();
                continue;
            }
            Token t;
            t.line = line, t.col = col;
            if (std::isalpha(static_cast<unsigned char>(c)) || c == '_') {
                std::size_t e = i;
                while (e < s.size() && (std::isalnum(static_cast<unsigned char>(s[e])) || s[e] == '_')) ++e;
                t.t = T::Ident, t.s = std::string(s.substr(i, e - i));
                adv(e - i);
            } else if (c == '$') {
                std::size_t e = i + 1;
                while (e < s.size() && (std::isalnum(static_cast<unsigned char>(s[e])) || s[e] == '_')) ++e;
                t.t = T::Special, t.s = std::string(s.substr(i + 1, e - i - 1));
                adv(e - i);
            } else if (std::isdigit(static_cast<unsigned char>(c))) {
                std::size_t e = i;
                std::uint64_t v = 0;
                if (c == '0' && i + 1 < s.size() && (s[i + 1] == 'x' || s[i + 1] == 'X')) {
                    e = i + 2;
                    while (e < s.size() && std::isxdigit(static_cast<unsigned char>(s[e]))) {
                        const char h = s[e++];
                        v = v * 16 + static_cast<std::uint64_t>(std::isdigit(static_cast<unsigned char>(h)) ? h - '0' : (std::tolower(h) - 'a' + 10));
                    }
                } else {
                    while (e < s.size() && std::isdigit(static_cast<unsigned char>(s[e]))) v = v * 10 + static_cast<std::uint64_t>(s[e++] - '0');
                }
                t.t = T::Num, t.n = v;
                adv(e - i);
            } else if (c == '"') {
                std::size_t e = i + 1;
                std::string text;
                while (e < s.size() && s[e] != '"') {
                    if (s[e] == '\\' && e + 1 < s.size()) {
                        const char x = s[e + 1];
                        e += 2;
                        if (x == 'n') text += '\n';
                        else if (x == 'r') text += '\r';
                        else if (x == 't') text += '\t';
                        else if (x == '0') text += '\0';
                        else if (x == 'x' && e + 1 < s.size()) {
                            text += static_cast<char>(std::stoi(std::string(s.substr(e, 2)), nullptr, 16));
                            e += 2;
                        } else text += x;
                    } else {
                        text += s[e++];
                    }
                }
                if (e >= s.size()) throw ParseError(origin_, line, col, "a string is not closed");
                t.t = T::Text, t.s = std::move(text);
                adv(e + 1 - i);
            } else {
                static const char* two[] = {"==", "!=", "<=", ">=", "<<", ">>", "&&", "||"};
                t.t = T::Punct;
                t.s = std::string(1, c);
                if (i + 1 < s.size())
                    for (const char* p : two)
                        if (s[i] == p[0] && s[i + 1] == p[1]) t.s = p;
                if (t.s.size() == 1 && std::string_view("{}()[]:@,=<>&|^~!+-*/%?.").find(c) == std::string_view::npos)
                    throw ParseError(origin_, line, col, std::string("unexpected character '") + c + "'");
                adv(t.s.size());
            }
            toks_.push_back(std::move(t));
        }
        Token end;
        end.t = T::End, end.line = line, end.col = col;
        toks_.push_back(end);
    }

    const Token& peek(std::size_t k = 0) const { return toks_[std::min(i_ + k, toks_.size() - 1)]; }
    const Token& prev() const { return toks_[i_ ? i_ - 1 : 0]; }
    void next() {
        if (i_ + 1 < toks_.size()) ++i_;
    }
    bool punct(const char* p, std::size_t k = 0) const { return peek(k).t == T::Punct && peek(k).s == p; }
    bool word(const char* w, std::size_t k = 0) const { return peek(k).t == T::Ident && peek(k).s == w; }
    void expect(const char* p) {
        if (!punct(p)) fail(peek(), std::string("expected '") + p + "'");
        next();
    }
    std::string ident(const char* what) {
        if (peek().t != T::Ident) fail(peek(), std::string("expected ") + what);
        std::string s = peek().s;
        next();
        return s;
    }
    std::uint64_t number() {
        if (peek().t != T::Num) fail(peek(), "expected a number");
        const std::uint64_t n = peek().n;
        next();
        return n;
    }

    // ---- statements ----

    void block(std::vector<Stmt>& out) {   // after '{', through '}'
        while (!punct("}")) {
            if (peek().t == T::End) fail(peek(), "a block is not closed");
            out.push_back(statement());
        }
        next();
    }

    Stmt statement() {
        Stmt s;
        s.line = peek().line;
        if (word("let") && peek(1).t == T::Ident) {
            next();
            s.k = Stmt::K::Let;
            s.let_name = ident("a name");
            expect("=");
            s.expr = expr(true);
        } else if (word("assert")) {
            next();
            s.k = Stmt::K::Assert;
            s.expr = expr(true);
            if (punct(":") && peek(1).t == T::Text) {
                next();
                s.message = peek().s;
                next();
            }
        } else if (word("if") && !punct(":", 1) && !punct("@", 1)) {
            next();
            s.k = Stmt::K::If;
            s.expr = expr(true);
            expect("{");
            block(s.body);
            if (word("else")) {
                next();
                if (word("if")) {
                    s.else_body.push_back(statement());
                } else {
                    expect("{");
                    block(s.else_body);
                }
            }
        } else {
            s.k = Stmt::K::Field;
            field(s.field);
        }
        return s;
    }

    void field(FieldDef& f) {
        f.name = ident("a field name");
        if (punct("@")) {
            next();
            f.at = expr(false);
        }
        expect(":");
        type(f.type);
        if (punct("[")) {
            next();
            if (word("until")) {
                next();
                f.arr = Arr::Until;
                f.arr_expr = expr(true);
            } else if (word("size") && !punct("]", 1)) {
                next();
                f.arr = Arr::Size;
                f.arr_expr = expr(true);
            } else {
                f.arr = Arr::Count;
                f.arr_expr = expr(true);
            }
            expect("]");
        }
        for (;;) {
            if (word("as")) {
                next();
                f.as = ident("an enum or flags name");
            } else if (punct("==")) {
                next();
                for (;;) {
                    Literal l;
                    if (peek().t == T::Text) l.is_text = true, l.text = peek().s;
                    else if (peek().t == T::Num) l.num = peek().n;
                    else fail(peek(), "a magic is a number or a string");
                    next();
                    f.magic.push_back(std::move(l));
                    if (!punct("|")) break;
                    next();
                }
            } else if (word("size") && punct("(", 1)) {
                next(), next();
                f.window = expr(true);
                expect(")");
            } else if (word("if") && peek().line == prev().line) {
                next();
                f.when = expr(true);
            } else {
                break;
            }
        }
    }

    void type(TypeRef& t) {
        const std::string n = ident("a type");
        if (n == "str" || n == "bytes") {
            t.k = n == "str" ? TypeRef::K::Str : TypeRef::K::Bytes;
            expect("(");
            t.len = expr(true);
            expect(")");
            return;
        }
        // u8 u16 u32 u64 i8 .. i64, optionally with le / be
        if ((n[0] == 'u' || n[0] == 'i') && n.size() >= 2 && std::isdigit(static_cast<unsigned char>(n[1]))) {
            std::size_t e = 1;
            unsigned bits = 0;
            while (e < n.size() && std::isdigit(static_cast<unsigned char>(n[e]))) bits = bits * 10 + static_cast<unsigned>(n[e++] - '0');
            const std::string rest = n.substr(e);
            if ((bits == 8 || bits == 16 || bits == 32 || bits == 64) && (rest.empty() || rest == "le" || rest == "be")) {
                t.k = TypeRef::K::Int;
                t.width = bits / 8;
                t.sgn = n[0] == 'i';
                t.endian_set = !rest.empty();
                t.big = rest == "be";
                t.name = n;
                return;
            }
        }
        t.k = TypeRef::K::Struct;
        t.name = n;
    }

    // ---- expressions ----

    ExprP make(Expr::Op op, std::string name = {}, std::vector<ExprP> kids = {}, std::int64_t num = 0) {
        auto e = std::make_shared<Expr>();
        e->op = op, e->name = std::move(name), e->kids = std::move(kids), e->num = num;
        return e;
    }

    ExprP expr(bool ternary) {
        ExprP c = binary(0);
        if (ternary && punct("?")) {
            next();
            ExprP a = expr(true);
            expect(":");
            ExprP b = expr(true);
            return make(Expr::Op::Cond, {}, {c, a, b});
        }
        return c;
    }

    ExprP binary(std::size_t level) {
        static const std::vector<std::vector<std::string>> levels = {
            {"||"}, {"&&"}, {"|"}, {"^"}, {"&"}, {"==", "!="}, {"<", "<=", ">", ">="}, {"<<", ">>"}, {"+", "-"}, {"*", "/", "%"}};
        if (level >= levels.size()) return unary();
        ExprP left = binary(level + 1);
        for (;;) {
            const Token& t = peek();
            if (t.t != T::Punct || std::find(levels[level].begin(), levels[level].end(), t.s) == levels[level].end()) return left;
            // a magic's `|` never reaches here (it follows `==` of an attribute and is read as literals), but an
            // attribute `== 1 | 2` starts after the field's type, not inside an expression: nothing to disambiguate.
            const std::string op = t.s;
            next();
            left = make(Expr::Op::Binary, op, {left, binary(level + 1)});
        }
    }

    ExprP unary() {
        if (punct("-") || punct("!") || punct("~")) {
            const std::string op = peek().s;
            next();
            return make(Expr::Op::Unary, op, {unary()});
        }
        return postfix();
    }

    ExprP postfix() {
        ExprP e = primary();
        for (;;) {
            if (punct(".")) {
                next();
                e = make(Expr::Op::Member, ident("a field name"), {e});
            } else if (punct("[")) {
                next();
                ExprP idx = expr(true);
                expect("]");
                e = make(Expr::Op::Index, {}, {e, idx});
            } else {
                return e;
            }
        }
    }

    ExprP primary() {
        const Token t = peek();
        if (t.t == T::Num) {
            next();
            return make(Expr::Op::Num, {}, {}, static_cast<std::int64_t>(t.n));
        }
        if (t.t == T::Special) {
            next();
            return make(Expr::Op::Special, t.s);
        }
        if (t.t == T::Punct && t.s == "(") {
            next();
            ExprP e = expr(true);
            expect(")");
            return e;
        }
        if (t.t == T::Ident) {
            next();
            if (punct("(")) {   // a call
                next();
                std::vector<ExprP> args;
                while (!punct(")")) {
                    args.push_back(expr(true));
                    if (punct(",")) next();
                    else if (!punct(")")) fail(peek(), "expected ',' or ')'");
                }
                next();
                return make(Expr::Op::Call, t.s, std::move(args));
            }
            return make(Expr::Op::Name, t.s);
        }
        fail(t, "expected an expression");
    }
};

// ---- the reader ----

struct Scope {
    Node* node = nullptr;
    const Scope* parent = nullptr;
    std::uint64_t start = 0, end = 0;
    std::uint64_t* pos = nullptr;
    std::uint64_t index = 0;
};

struct Ctx {
    const Scope* sc = nullptr;
    const Node* under = nullptr;   // `_` in an `until`
};

struct Val {
    std::int64_t n = 0;
    const Node* ref = nullptr;
};

class Reader {
public:
    Reader(std::vector<const Format*> chain, ByteView data) : chain_(std::move(chain)), data_(data) {}

    Node read_root() {
        const Format& f = *chain_.front();
        const StructDef* root = find_struct(f.root);
        if (!root) throw FormatError(f.name, 0, "the description has no struct '" + f.root + "'");
        Node n = read_struct(*root, {}, 0, data_.size(), nullptr, 0);
        return n;
    }

private:
    std::vector<const Format*> chain_;
    ByteView data_;

    const std::string& fmt() const { return chain_.front()->name; }
    [[noreturn]] void fail(std::uint64_t at, const std::string& what) const { throw FormatError(fmt(), at, what); }

    const StructDef* find_struct(std::string_view name) const {
        for (const Format* f : chain_) {
            auto it = f->structs.find(name);
            if (it != f->structs.end()) return &it->second;
        }
        return nullptr;
    }
    const EnumDef* find_enum(std::string_view name) const {
        for (const Format* f : chain_) {
            auto it = f->enums.find(name);
            if (it != f->enums.end()) return &it->second;
        }
        return nullptr;
    }

    // ---- expressions ----

    std::int64_t scalar(const Val& v, std::uint64_t at) const {
        if (!v.ref) return v.n;
        if (v.ref->kind == Kind::Int || v.ref->kind == Kind::Let) return v.ref->as_signed();
        fail(at, "'" + v.ref->name + "' is not a number");
    }

    Val lookup(const std::string& name, const Ctx& c) const {
        const Scope* s = c.sc;
        if (name == "_") {
            if (!c.under) fail(*s->pos, "`_` is only meant in an `until`");
            return {0, c.under};
        }
        if (name == "_index") return {static_cast<std::int64_t>(s->index), nullptr};
        if (name == "_root") {
            while (s->parent) s = s->parent;
            return {0, s->node};
        }
        if (name == "_parent") {
            if (!s->parent) fail(*s->pos, "`_parent` of the root");
            return {0, s->parent->node};
        }
        for (; s; s = s->parent)
            if (const Node* n = s->node->child(name)) return {0, n};
        fail(*c.sc->pos, "unknown name '" + name + "'");
    }

    Val eval(const Expr& e, const Ctx& c) const {
        const std::uint64_t here = *c.sc->pos;
        auto num = [&](const ExprP& k) { return scalar(eval(*k, c), here); };
        auto u = [](std::int64_t v) { return static_cast<std::uint64_t>(v); };
        auto s64 = [](std::uint64_t v) { return static_cast<std::int64_t>(v); };
        switch (e.op) {
            case Expr::Op::Num: return {e.num, nullptr};
            case Expr::Op::Name: return lookup(e.name, c);
            case Expr::Op::Special: {
                const Scope& s = *c.sc;
                if (e.name == "pos") return {s64(*s.pos), nullptr};
                if (e.name == "start") return {s64(s.start), nullptr};
                if (e.name == "end") return {s64(s.end), nullptr};
                if (e.name == "remaining") return {s64(s.end > *s.pos ? s.end - *s.pos : 0), nullptr};
                if (e.name == "size") return {s64(data_.size()), nullptr};
                if (e.name == "index") return {s64(s.index), nullptr};
                fail(here, "unknown $" + e.name);
            }
            case Expr::Op::Unary: {
                const std::int64_t v = num(e.kids[0]);
                if (e.name == "-") return {s64(0 - u(v)), nullptr};
                if (e.name == "!") return {v == 0 ? 1 : 0, nullptr};
                return {~v, nullptr};
            }
            case Expr::Op::Cond: return num(e.kids[0]) ? eval(*e.kids[1], c) : eval(*e.kids[2], c);
            case Expr::Op::Binary: {
                const std::string& op = e.name;
                if (op == "&&") return {num(e.kids[0]) != 0 && num(e.kids[1]) != 0 ? 1 : 0, nullptr};
                if (op == "||") return {num(e.kids[0]) != 0 || num(e.kids[1]) != 0 ? 1 : 0, nullptr};
                const std::int64_t a = num(e.kids[0]), b = num(e.kids[1]);
                if (op == "+") return {s64(u(a) + u(b)), nullptr};
                if (op == "-") return {s64(u(a) - u(b)), nullptr};
                if (op == "*") return {s64(u(a) * u(b)), nullptr};
                if (op == "/" || op == "%") {
                    if (b == 0) fail(here, "division by zero");
                    return {op == "/" ? a / b : a % b, nullptr};
                }
                if (op == "<<") return {b < 0 || b >= 64 ? 0 : s64(u(a) << b), nullptr};
                if (op == ">>") return {b < 0 || b >= 64 ? 0 : a >> b, nullptr};
                if (op == "&") return {a & b, nullptr};
                if (op == "|") return {a | b, nullptr};
                if (op == "^") return {a ^ b, nullptr};
                if (op == "==") return {a == b, nullptr};
                if (op == "!=") return {a != b, nullptr};
                if (op == "<") return {a < b, nullptr};
                if (op == "<=") return {a <= b, nullptr};
                if (op == ">") return {a > b, nullptr};
                return {a >= b, nullptr};
            }
            case Expr::Op::Member: {
                const Val base = eval(*e.kids[0], c);
                if (!base.ref || base.ref->kind != Kind::Struct) fail(here, "'." + e.name + "' of something that is not a structure");
                const Node* n = base.ref->child(e.name);
                if (!n) fail(here, "no field '" + e.name + "' in " + base.ref->name);
                return {0, n};
            }
            case Expr::Op::Index: {
                const Val base = eval(*e.kids[0], c);
                const std::int64_t i = num(e.kids[1]);
                if (!base.ref || base.ref->kind != Kind::Array) fail(here, "an index of something that is not an array");
                if (i < 0 || u(i) >= base.ref->children.size()) fail(here, "index " + std::to_string(i) + " is outside the array");
                return {0, &base.ref->children[u(i)]};
            }
            case Expr::Op::Call: {
                const std::string& f = e.name;
                if (f == "count") {
                    if (e.kids.size() != 1) fail(here, "count(array)");
                    const Val v = eval(*e.kids[0], c);
                    if (!v.ref) fail(here, "count of a number");
                    return {s64(v.ref->children.size()), nullptr};
                }
                if (f.starts_with("peek")) {   // peek8 peek16 peek32 (little-endian), peek16be peek32be: the file's raw bytes at an offset, -1 outside it
                    if (e.kids.size() != 1) fail(here, f + " takes one argument (an offset)");
                    const std::int64_t at = num(e.kids[0]);
                    const bool be = f.ends_with("be");
                    const std::string_view bits = std::string_view(f).substr(4, f.size() - 4 - (be ? 2 : 0));
                    const unsigned width = bits == "8" ? 1 : bits == "16" ? 2 : bits == "32" ? 4 : 0;
                    if (width == 0) fail(here, "unknown function " + f);
                    if (at < 0 || u(at) > data_.size() || width > data_.size() - u(at)) return {-1, nullptr};
                    std::int64_t v = 0;
                    for (unsigned k = 0; k < width; ++k) v |= static_cast<std::int64_t>(data_[u(at) + k]) << (8 * (be ? width - 1 - k : k));
                    return {v, nullptr};
                }
                if (e.kids.size() != 2) fail(here, f + " takes two arguments");
                const std::int64_t a = num(e.kids[0]), b = num(e.kids[1]);
                if (f == "sum8" || f == "printable") {   // over the n raw bytes at offset a (the part outside the file counts as zeros)
                    if (a < 0 || b < 0) fail(here, f + " of a negative range");
                    std::int64_t sum = 0;
                    for (std::int64_t k = 0; k < b; ++k) {
                        const std::uint64_t at = u(a) + u(k);
                        const unsigned byte = at < data_.size() ? data_[at] : 0;
                        sum += f == "sum8" ? static_cast<std::int64_t>(byte) : (byte >= 0x20 && byte < 0x7F ? 1 : 0);
                    }
                    return {sum, nullptr};
                }
                if (f == "min") return {std::min(a, b), nullptr};
                if (f == "max") return {std::max(a, b), nullptr};
                if (f == "align") {
                    if (b <= 0) fail(here, "align to a non-positive number");
                    return {(a + b - 1) / b * b, nullptr};
                }
                fail(here, "unknown function " + f);
            }
        }
        return {};
    }

    std::int64_t num(const ExprP& e, const Ctx& c) const { return scalar(eval(*e, c), *c.sc->pos); }
    std::uint64_t length(const ExprP& e, const Ctx& c) const {
        const std::int64_t n = num(e, c);
        if (n < 0) fail(*c.sc->pos, "a negative length " + std::to_string(n));
        return static_cast<std::uint64_t>(n);
    }

    // ---- reading ----

    Node read_struct(const StructDef& def, std::string name, std::uint64_t at, std::uint64_t end, const Scope* parent, std::uint64_t index) {
        Node n;
        n.name = std::move(name);
        n.kind = Kind::Struct;
        n.type = def.name;
        n.offset = at;
        std::uint64_t pos = at;
        Scope sc{&n, parent, at, end, &pos, index};
        run(def.body, n, sc);
        n.size = pos - at;
        return n;
    }

    void run(const std::vector<Stmt>& body, Node& self, Scope& sc) {
        for (const Stmt& s : body) {
            const Ctx c{&sc, nullptr};
            switch (s.k) {
                case Stmt::K::Field: read_field(s.field, self, sc); break;
                case Stmt::K::Let: {
                    Node n;
                    n.name = s.let_name;
                    n.kind = Kind::Let;
                    n.offset = *sc.pos;
                    n.value = static_cast<std::uint64_t>(num(s.expr, c));
                    self.children.push_back(std::move(n));
                    break;
                }
                case Stmt::K::If: run(num(s.expr, c) ? s.body : s.else_body, self, sc); break;
                case Stmt::K::Assert:
                    if (!num(s.expr, c))
                        fail(*sc.pos, s.message.empty() ? "an assertion of the description does not hold (line " + std::to_string(s.line) + ")" : s.message);
                    break;
            }
        }
    }

    std::string label_of(const std::string& as, std::uint64_t value) const {
        const EnumDef* e = find_enum(as);
        if (!e) fail(0, "unknown enum or flags '" + as + "'");
        if (!e->flags) {
            for (const auto& [n, v] : e->items)
                if (v == value) return n;
            return {};
        }
        std::string out;
        std::uint64_t left = value;
        for (const auto& [n, v] : e->items) {
            if (v != 0 && (value & v) == v) {
                out += (out.empty() ? "" : "|") + n;
                left &= ~v;
            }
        }
        if (left) out += std::format("{}0x{:x}", out.empty() ? "" : "|", left);
        return out;
    }

    Node read_value(const FieldDef& f, std::string name, std::uint64_t& c, std::uint64_t end, Scope& sc, std::uint64_t index) {
        const TypeRef& t = f.type;
        const Ctx cx{&sc, nullptr};
        Node n;
        n.name = std::move(name);
        n.offset = c;
        auto room = [&](std::uint64_t need) {
            if (c > end || need > end - c) throw TruncatedError(fmt(), c, need, end > c ? end - c : 0);
        };
        switch (t.k) {
            case TypeRef::K::Int: {
                room(t.width);
                n.kind = Kind::Int;
                n.type = t.name;
                n.width = t.width;
                n.big_endian = t.endian_set ? t.big : chain_.front()->big;
                n.is_signed = t.sgn;
                std::uint64_t v = 0;
                for (unsigned k = 0; k < t.width; ++k) {
                    const std::uint64_t b = data_[c + k];
                    v |= b << (8 * (n.big_endian ? t.width - 1 - k : k));
                }
                if (t.sgn && t.width < 8 && (v >> (8 * t.width - 1)) & 1) v |= ~std::uint64_t{0} << (8 * t.width);
                n.value = v;
                n.size = t.width;
                if (!f.as.empty()) n.label = label_of(f.as, v);
                c += t.width;
                break;
            }
            case TypeRef::K::Str:
            case TypeRef::K::Bytes: {
                const std::uint64_t len = length(t.len, cx);
                room(len);
                n.kind = t.k == TypeRef::K::Str ? Kind::Str : Kind::Bytes;
                n.size = len;
                if (n.kind == Kind::Str) n.text.assign(reinterpret_cast<const char*>(data_.data() + c), len);
                c += len;
                break;
            }
            case TypeRef::K::Struct: {
                const StructDef* def = find_struct(t.name);
                if (!def) fail(c, "unknown struct '" + t.name + "'");
                n = read_struct(*def, n.name, c, end, &sc, index);
                c += n.size;
                break;
            }
        }
        return n;
    }

    bool magic_ok(const FieldDef& f, const Node& n) const {
        if (f.magic.empty()) return true;
        for (const Literal& l : f.magic) {
            if (n.kind == Kind::Int) {
                if (!l.is_text && l.num == n.value) return true;
            } else if (l.is_text && n.size == l.text.size()) {
                if (std::equal(l.text.begin(), l.text.end(), data_.begin() + static_cast<std::ptrdiff_t>(n.offset))) return true;
            }
        }
        return false;
    }

    void read_field(const FieldDef& f, Node& self, Scope& sc) {
        const Ctx cx{&sc, nullptr};
        if (f.when && !num(f.when, cx)) return;
        const bool ptr = f.at != nullptr;
        const std::uint64_t at = ptr ? length(f.at, cx) : *sc.pos;
        std::uint64_t end = ptr ? data_.size() : sc.end;
        std::optional<std::uint64_t> window;
        if (f.window) {
            window = length(f.window, cx);
            end = std::min(end, at + *window);
        }
        std::uint64_t c = at;
        Node node;
        switch (f.arr) {
            case Arr::None:
                node = read_value(f, f.name, c, end, sc, 0);
                if (!magic_ok(f, node)) fail(at, "'" + f.name + "' is not what the format starts with (magic mismatch)");
                break;
            case Arr::Count: {
                node.kind = Kind::Array;
                const std::uint64_t count = length(f.arr_expr, cx);
                if (count > (1u << 24)) fail(at, "'" + f.name + "' claims " + std::to_string(count) + " elements");
                for (std::uint64_t i = 0; i < count; ++i) node.children.push_back(read_value(f, std::to_string(i), c, end, sc, i));
                break;
            }
            case Arr::Until:
                node.kind = Kind::Array;
                for (std::uint64_t i = 0; c < end; ++i) {
                    node.children.push_back(read_value(f, std::to_string(i), c, end, sc, i));
                    const Ctx inner{&sc, &node.children.back()};
                    if (num(f.arr_expr, inner)) break;
                }
                break;
            case Arr::Size: {
                node.kind = Kind::Array;
                const std::uint64_t limit = at + length(f.arr_expr, cx);
                for (std::uint64_t i = 0; c < limit && c < end; ++i) node.children.push_back(read_value(f, std::to_string(i), c, end, sc, i));
                break;
            }
        }
        node.name = f.name;
        node.offset = at;
        node.size = window ? *window : c - at;
        if (!ptr) *sc.pos = window ? at + *window : c;
        self.children.push_back(std::move(node));
    }
};

}  // namespace

// ---- the registry ---------------------------------------------------------------------------------------

Registry::Registry() = default;
Registry::~Registry() = default;
Registry::Registry(Registry&&) noexcept = default;
Registry& Registry::operator=(Registry&&) noexcept = default;

const std::string& Registry::add_text(std::string_view text, std::string_view origin) {
    std::shared_ptr<Format> f = Parser(text, origin).parse();
    const std::string name = f->name;
    formats_[name] = std::move(f);
    return formats_.find(name)->second->name;
}

void Registry::add_file(const std::filesystem::path& file) { add_text(fs::read_text(file), fs::to_text(file)); }

std::size_t Registry::add_directory(const std::filesystem::path& dir) {
    std::vector<std::filesystem::path> files;
    std::error_code ec;
    for (std::filesystem::directory_iterator it(dir, ec), end; !ec && it != end; it.increment(ec))
        if (it->path().extension() == ".fmt") files.push_back(it->path());
    std::sort(files.begin(), files.end());
    for (const auto& file : files) add_file(file);
    return files.size();
}

bool Registry::has(std::string_view format) const { return formats_.find(format) != formats_.end(); }

std::vector<std::string> Registry::names() const {
    std::vector<std::string> out;
    for (const auto& [name, f] : formats_) out.push_back(name);
    return out;
}

Tree Registry::read(std::string_view format, ByteView data) const {
    auto it = formats_.find(format);
    if (it == formats_.end()) throw NotFoundError(std::filesystem::path(std::string(format)), 0, "no format description named this");
    std::vector<const Format*> chain;   // the format, then what it `use`s (depth first, each once)
    std::set<std::string> seen;
    std::function<void(const std::string&)> add = [&](const std::string& name) {
        if (!seen.insert(name).second) return;
        auto f = formats_.find(name);
        if (f == formats_.end()) throw NotFoundError(std::filesystem::path(name), 0, "a description `use`s a format that is not loaded");
        chain.push_back(f->second.get());
        for (const std::string& u : f->second->uses) add(u);
    };
    add(it->first);
    Tree tree;
    tree.format = it->first;
    tree.source.assign(data.begin(), data.end());
    Reader reader(std::move(chain), tree.source);
    tree.root = reader.read_root();
    tree.root.name = it->first;
    return tree;
}

std::optional<Tree> Registry::try_read(std::string_view format, ByteView data) const {
    try {
        return read(format, data);
    } catch (const FormatError&) {   // probing: a file of another format is an ordinary answer
        return std::nullopt;
    }
}

std::vector<std::string> Registry::identify(ByteView data) const {
    std::vector<std::string> out;
    for (const auto& [name, f] : formats_)
        if (try_read(name, data)) out.push_back(name);
    return out;
}

}  // namespace cb::formats::engine
