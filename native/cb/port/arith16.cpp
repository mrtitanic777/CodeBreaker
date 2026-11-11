// cb/port/arith16.cpp -- see arith16.hpp.

#include "cb/port/arith16.hpp"

#include <algorithm>
#include <charconv>
#include <format>
#include <set>
#include <tuple>
#include <unordered_map>

#include <nlohmann/json.hpp>

#include "cb/core/json_fwd.hpp"

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/process.hpp"
#include "cb/core/text.hpp"

namespace cb::port::arith16 {

namespace {

// ---- names the tree uses ---------------------------------------------------------------------------------------

Kind kind_of(std::string_view s) {
    static const std::unordered_map<std::string_view, Kind> kinds = {
        {"ParenExpr", Kind::ParenExpr},
        {"ImplicitCastExpr", Kind::ImplicitCastExpr},
        {"CStyleCastExpr", Kind::CStyleCastExpr},
        {"IntegerLiteral", Kind::IntegerLiteral},
        {"CharacterLiteral", Kind::CharacterLiteral},
        {"BinaryOperator", Kind::BinaryOperator},
        {"CompoundAssignOperator", Kind::CompoundAssignOperator},
        {"UnaryOperator", Kind::UnaryOperator},
        {"ConditionalOperator", Kind::ConditionalOperator},
        {"DeclRefExpr", Kind::DeclRefExpr},
        {"MemberExpr", Kind::MemberExpr},
        {"ArraySubscriptExpr", Kind::ArraySubscriptExpr},
        {"CallExpr", Kind::CallExpr},
    };
    const auto it = kinds.find(s);
    return it == kinds.end() ? Kind::Other : it->second;
}

CastKind cast_kind_of(std::string_view s) {
    if (s == "LValueToRValue") return CastKind::LValueToRValue;
    if (s == "NoOp") return CastKind::NoOp;
    if (s == "IntegralCast") return CastKind::IntegralCast;
    return CastKind::Other;
}

// Unary and binary operators share spellings ("-", "*", "+", "&"); the node's kind (always named before its opcode)
// says which one it is.
Op op_of(std::string_view s, Kind kind) {
    const bool unary = kind == Kind::UnaryOperator;
    static const std::unordered_map<std::string_view, Op> both = {
        {"-", Op::Sub}, {"<<", Op::Shl}, {">>", Op::Shr}, {"^", Op::Xor}, {"|", Op::Or},   {"/", Op::Div},
        {"%", Op::Rem}, {"<", Op::Lt},   {">", Op::Gt},   {"<=", Op::Le}, {">=", Op::Ge},  {"==", Op::Eq},
        {"!=", Op::Ne}, {"&&", Op::LogAnd}, {"||", Op::LogOr}, {",", Op::Comma}, {"=", Op::Assign},
        {"!", Op::Not}, {"~", Op::BitNot}, {"++", Op::Inc}, {"--", Op::Dec},
    };
    if (const auto it = both.find(s); it != both.end()) return it->second;
    if (s == "+") return unary ? Op::Plus : Op::Add;
    if (s == "*") return unary ? Op::Deref : Op::Mul;
    if (s == "&") return unary ? Op::AddrOf : Op::And;
    return Op::Other;
}

// The 8086's type of a type as the source names it (the Python's W16): chars and bytes were ints of 16 bits.
std::optional<Prim> prim_of(std::string_view t) {
    static const std::unordered_map<std::string_view, Prim> prims = {
        {"int16_t", Prim::I16}, {"short", Prim::I16},         {"uint16_t", Prim::U16},      {"unsigned short", Prim::U16},
        {"char", Prim::I16},    {"signed char", Prim::I16},   {"unsigned char", Prim::I16}, {"uint8_t", Prim::I16},
        {"int8_t", Prim::I16},  {"int32_t", Prim::I32},       {"long", Prim::I32},          {"uint32_t", Prim::U32},
        {"unsigned long", Prim::U32},
    };
    const auto it = prims.find(t);
    return it == prims.end() ? std::nullopt : std::optional<Prim>(it->second);
}

constexpr bool is_arith(Op op) {
    return op == Op::Add || op == Op::Sub || op == Op::Mul || op == Op::Shl || op == Op::And || op == Op::Or ||
           op == Op::Xor;
}

// The operators whose result or whose test the 8086 did on unsigned operands when either was.
constexpr bool is_test(Op op) {
    return op == Op::Lt || op == Op::Gt || op == Op::Le || op == Op::Ge || op == Op::Eq || op == Op::Ne ||
           op == Op::Div || op == Op::Rem || op == Op::Shr;
}

constexpr bool is_comparison(Op op) {
    return op == Op::Lt || op == Op::Gt || op == Op::Le || op == Op::Ge || op == Op::Eq || op == Op::Ne ||
           op == Op::LogAnd || op == Op::LogOr;
}

constexpr std::string_view cast_name(Prim p) { return p == Prim::U16 ? "uint16_t" : "int16_t"; }

// A node with nothing in it: what an expression with no operand is taken for.
const Node& empty_node() {
    static const Node empty;
    return empty;
}

// ---- reading the tree --------------------------------------------------------------------------------------------

// Builds a Tree while the JSON is read, without making a document of it. The parts of a node it keeps are the ones
// named in Node; everything else in the dump is skipped as it goes by.
class Builder final : public nlohmann::json_sax<Json> {
public:
    using string_t = Json::string_t;

    Builder(Node& root, std::vector<std::string>& files) : root_(root), files_(files) {}

    // Whether the document was one tree, whole.
    bool complete() const { return started_ && stack_.empty() && !failed_; }

    bool null() override { return true; }
    bool boolean(bool) override { return true; }
    bool number_float(number_float_t, const string_t&) override { return true; }
    bool binary(binary_t&) override { return true; }
    bool parse_error(std::size_t, const std::string&, const nlohmann::detail::exception&) override { return false; }

    bool number_integer(number_integer_t v) override { return number(v); }
    bool number_unsigned(number_unsigned_t v) override {
        return number(v > static_cast<number_unsigned_t>(INT64_MAX) ? INT64_MAX : static_cast<std::int64_t>(v));
    }

    bool start_object(std::size_t) override {
        if (!started_) {
            started_ = true;
            stack_.push_back({.ctx = Ctx::NodeObj, .node = &root_});
            return true;
        }
        if (stack_.empty()) return fail();
        Frame& top = stack_.back();
        switch (top.ctx) {
            case Ctx::Skip:
                ++top.depth;
                return true;
            case Ctx::InnerArr: {
                top.node->inner.emplace_back();
                Node* child = &top.node->inner.back();
                stack_.push_back({.ctx = Ctx::NodeObj, .node = child});
                return true;
            }
            case Ctx::NodeObj:
                if (top.key == "loc") return push_loc(&top.node->loc);
                if (top.key == "range") {
                    stack_.push_back({.ctx = Ctx::RangeObj, .range = &top.node->range});
                    return true;
                }
                if (top.key == "type") {
                    stack_.push_back({.ctx = Ctx::TypeObj, .node = top.node});
                    return true;
                }
                return skip();
            case Ctx::RangeObj:
                if (top.key == "begin") return push_loc(&top.range->begin);
                if (top.key == "end") return push_loc(&top.range->end);
                return skip();
            case Ctx::LocObj: {
                Loc* here = top.loc;
                if (top.key == "spellingLoc") {
                    here->has_spelling = true;
                    auto scratch = std::make_unique<Loc>();   // (only its file matters: it moves the cursor)
                    Loc* l = scratch.get();
                    push_loc(l);
                    stack_.back().scratch = std::move(scratch);
                    return true;
                }
                if (top.key == "expansionLoc") {
                    here->expansion = std::make_unique<Loc>();
                    return push_loc(here->expansion.get());
                }
                if (top.key == "includedFrom") {
                    stack_.push_back({.ctx = Ctx::IncludedObj});
                    return true;
                }
                return skip();
            }
            default:
                return skip();
        }
    }

    bool end_object() override { return end(); }

    bool start_array(std::size_t) override {
        if (stack_.empty()) return fail();   // (the document is an object)
        Frame& top = stack_.back();
        if (top.ctx == Ctx::Skip) {
            ++top.depth;
            return true;
        }
        if (top.ctx == Ctx::NodeObj && top.key == "inner") {
            stack_.push_back({.ctx = Ctx::InnerArr, .node = top.node});
            return true;
        }
        return skip();
    }

    bool end_array() override { return end(); }

    bool key(string_t& k) override {
        if (stack_.empty()) return fail();
        Frame& top = stack_.back();
        if (top.ctx != Ctx::Skip) top.key = k;
        return true;
    }

    bool string(string_t& s) override {
        if (stack_.empty()) return fail();
        Frame& top = stack_.back();
        switch (top.ctx) {
            case Ctx::NodeObj: {
                Node& n = *top.node;
                if (top.key == "kind") {
                    n.kind = kind_of(s);
                } else if (top.key == "opcode") {
                    n.op = op_of(s, n.kind);
                } else if (top.key == "castKind") {
                    n.cast_kind = cast_kind_of(s);
                } else if (top.key == "value" && n.kind == Kind::IntegerLiteral) {
                    // a number as text; one too big for 64 bits is only ever asked whether it is over 16 bits
                    std::int64_t v = 0;
                    const auto r = std::from_chars(s.data(), s.data() + s.size(), v);
                    n.value = r.ec == std::errc::result_out_of_range ? INT64_MAX : v;
                }
                return true;
            }
            case Ctx::TypeObj:
                if (top.key == "qualType") {
                    top.node->qual_type = s;
                    top.node->prim = prim_of(s);
                }
                return true;
            case Ctx::LocObj:
                if (top.key == "file") {
                    top.own = intern(s);
                    cursor_ = *top.own;
                }
                return true;
            case Ctx::IncludedObj:
                if (top.key == "file") intern(s);   // (the includer's name: it does not say where the location is)
                return true;
            default:
                return true;
        }
    }

private:
    enum class Ctx : std::uint8_t { NodeObj, InnerArr, RangeObj, LocObj, IncludedObj, TypeObj, Skip };

    struct Frame {
        Ctx ctx = Ctx::Skip;
        Node* node = nullptr;
        Range* range = nullptr;
        Loc* loc = nullptr;
        std::string key;
        int depth = 1;                       // Skip: how many containers deep
        FileId before = kNoFile;             // LocObj: the file the cursor named when it began
        std::optional<FileId> own;           // LocObj: the file it names itself
        std::unique_ptr<Loc> scratch;        // LocObj of a spelling: nothing keeps it
    };

    bool fail() {
        failed_ = true;
        return false;
    }

    bool skip() {
        stack_.push_back({.ctx = Ctx::Skip});
        return true;
    }

    bool push_loc(Loc* l) {
        stack_.push_back({.ctx = Ctx::LocObj, .loc = l, .before = cursor_});
        return true;
    }

    bool end() {
        if (stack_.empty()) return fail();
        Frame& top = stack_.back();
        if (top.ctx == Ctx::Skip && --top.depth > 0) return true;
        if (top.ctx == Ctx::LocObj) top.loc->file = top.own ? *top.own : top.before;
        stack_.pop_back();
        return true;
    }

    bool number(std::int64_t v) {
        if (stack_.empty()) return fail();
        Frame& top = stack_.back();
        if (top.ctx == Ctx::NodeObj && top.key == "value") {
            top.node->value = v;   // a character's code
        } else if (top.ctx == Ctx::LocObj) {
            if (top.key == "offset") top.loc->offset = static_cast<std::uint64_t>(std::max<std::int64_t>(v, 0));
            else if (top.key == "tokLen") top.loc->tok_len = static_cast<std::uint64_t>(std::max<std::int64_t>(v, 0));
        }
        return true;
    }

    FileId intern(const std::string& name) {
        const auto [it, added] = ids_.try_emplace(name, static_cast<FileId>(files_.size()));
        if (added) files_.push_back(name);
        return it->second;
    }

    Node& root_;
    std::vector<std::string>& files_;
    std::unordered_map<std::string, FileId> ids_;
    std::vector<Frame> stack_;
    FileId cursor_ = kNoFile;   // clang names a location's file only where it changes: the last it named
    bool started_ = false;
    bool failed_ = false;
};

// A file's name as clang may spell it, for comparing: separators '/', no leading "./".
std::string normal_name(std::string_view name) {
    std::string s(name);
    std::ranges::replace(s, '\\', '/');
    while (s.starts_with("./")) s.erase(0, 2);
    return s;
}

// ---- the expressions -----------------------------------------------------------------------------------------------

// A '(' within the next 40 bytes of the text (blanks between): the name before it is a macro that takes arguments.
bool followed_by_paren(std::string_view text, std::uint64_t end) {
    if (end > text.size()) return false;
    const std::string_view after = text.substr(static_cast<std::size_t>(end), 40);
    for (const char c : after) {
        if (c == '(') return true;
        if (!text::is_space(c)) return false;
    }
    return false;
}

// An integer as C spells it, with its suffixes: 0x8000, 0177777, 70000L, 5u.
bool is_c_integer(std::string_view s) {
    std::size_t i = 0;
    const auto hex = [](char c) { return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'); };
    if (s.size() > 2 && s[0] == '0' && (s[1] == 'x' || s[1] == 'X') && hex(s[2])) {
        i = 2;
        while (i < s.size() && hex(s[i])) ++i;
    } else {
        while (i < s.size() && s[i] >= '0' && s[i] <= '9') ++i;
        if (i == 0) return false;
    }
    for (; i < s.size(); ++i)
        if (std::string_view("uUlL").find(s[i]) == std::string_view::npos) return false;
    return true;
}

}  // namespace

FileId Tree::find_file(std::string_view name) const {
    const std::string want = normal_name(name);
    for (std::size_t i = 0; i < files_.size(); ++i)
        if (normal_name(files_[i]) == want) return static_cast<FileId>(i);
    return kNoFile;
}

std::optional<Tree> parse_tree(std::string_view json) {
    Tree tree;
    Builder builder(tree.root_, tree.files_);
    const bool ok = Json::sax_parse(json, &builder);
    if (!ok || !builder.complete()) return std::nullopt;
    return tree;
}

std::optional<Tree> dump(const std::filesystem::path& clang, std::string_view file, const std::filesystem::path& folder,
                         std::span<const std::string> extra) {
    process::ProcessSpec spec;
    spec.argv = {fs::to_text(clang), "-fsyntax-only", "-Xclang", "-ast-dump=json", "-fsigned-char", "-std=c11",
                 "-D_CRT_SECURE_NO_WARNINGS", "-D_CRT_NONSTDC_NO_WARNINGS", "-I."};
    spec.argv.insert(spec.argv.end(), extra.begin(), extra.end());
    spec.argv.emplace_back(file);
    spec.cwd = folder;
    spec.err = process::Capture::Discard;
    const process::ProcessResult r = process::run(spec);
    if (r.out.empty()) return std::nullopt;   // (a file clang can read nothing of says nothing)
    return parse_tree(as_text(r.out));
}

// ---- types ---------------------------------------------------------------------------------------------------------

std::optional<Span> place(const Node& n, std::string_view text) {
    const Loc& b = n.range.begin;
    const Loc& e = n.range.end;
    if (b.expansion && e.expansion) {   // (all of it from one use of a macro: that use)
        const Loc& x = *b.expansion;
        const Loc& y = *e.expansion;
        if (!x.offset || x.offset != y.offset) return std::nullopt;
        const std::uint64_t end = *x.offset + x.tok_len;
        if (followed_by_paren(text, end)) return std::nullopt;   // (a macro with arguments - assert(...) - not a constant)
        return Span{*x.offset, end, x.file != kNoFile ? x.file : b.file};
    }
    if (!b.offset || !e.offset || b.has_spelling || e.has_spelling) return std::nullopt;
    if (b.file != e.file || b.file == kNoFile) return std::nullopt;
    return Span{*b.offset, *e.offset + e.tok_len, b.file};
}

std::optional<std::string_view> spelled(const Node& n, const Source& src) {
    const std::optional<Span> p = place(n, src.text);
    if (!p || p->file != src.main || src.main == kNoFile || p->start > src.text.size() || p->end < p->start)
        return std::nullopt;
    const std::size_t end = static_cast<std::size_t>(std::min<std::uint64_t>(p->end, src.text.size()));
    return src.text.substr(static_cast<std::size_t>(p->start), end - static_cast<std::size_t>(p->start));
}

std::optional<Prim> common(std::optional<Prim> a, std::optional<Prim> b) {
    if (!a || !b) return std::nullopt;
    if (*a == Prim::U32 || *b == Prim::U32) return Prim::U32;
    if (*a == Prim::I32 || *b == Prim::I32) return Prim::I32;
    if (*a == Prim::U16 || *b == Prim::U16) return Prim::U16;
    return Prim::I16;
}

std::optional<Prim> old_type(const Node& n, const Source& src) {
    const auto first = [&]() -> std::optional<Prim> {
        return n.inner.empty() ? std::nullopt : old_type(n.inner[0], src);
    };
    switch (n.kind) {
        case Kind::ParenExpr:
            return first();
        case Kind::ImplicitCastExpr:
            if (n.cast_kind == CastKind::LValueToRValue || n.cast_kind == CastKind::NoOp) return first();
            if (n.cast_kind == CastKind::IntegralCast) {
                if (n.qual_type == "int") return first();   // (a promotion: what it was, made an int of 16 bits)
                return n.prim;
            }
            return std::nullopt;
        case Kind::IntegerLiteral: {
            std::string_view s = spelled(n, src).value_or("");
            if (!is_c_integer(s)) s = {};   // (a macro's name, not the number's own spelling)
            const std::int64_t v = n.value.value_or(0);
            if ((!s.empty() && (s.back() == 'l' || s.back() == 'L')) || v > 0xFFFF) return Prim::I32;
            if (v > 0x7FFF) return s.starts_with('0') ? Prim::U16 : Prim::I32;   // (0177777, 0xFFFF unsigned; 65535 a long)
            return Prim::I16;
        }
        case Kind::CharacterLiteral:
            return Prim::I16;
        case Kind::CStyleCastExpr:
            return n.prim;
        case Kind::BinaryOperator: {
            if (is_comparison(n.op)) return Prim::I16;
            if (n.op == Op::Comma) return n.inner.size() > 1 ? old_type(n.inner[1], src) : std::nullopt;
            if (n.op == Op::Assign) return first();
            std::optional<Prim> a, b;
            if (n.inner.size() >= 2) {
                a = old_type(n.inner[0], src);
                b = old_type(n.inner[1], src);
            }
            if (n.op == Op::Shl || n.op == Op::Shr) return a;
            return common(a, b);
        }
        case Kind::CompoundAssignOperator:
            return first();
        case Kind::UnaryOperator:
            if (n.op == Op::Not) return Prim::I16;
            if (n.op == Op::Sub || n.op == Op::BitNot || n.op == Op::Plus || n.op == Op::Inc || n.op == Op::Dec)
                return first();
            if (n.op == Op::Deref) return n.prim;
            return std::nullopt;
        case Kind::ConditionalOperator:
            return n.inner.size() >= 3 ? common(old_type(n.inner[1], src), old_type(n.inner[2], src)) : std::nullopt;
        case Kind::DeclRefExpr:
        case Kind::MemberExpr:
        case Kind::ArraySubscriptExpr:
        case Kind::CallExpr:
            return n.prim;
        case Kind::Other:
            return std::nullopt;
    }
    return std::nullopt;
}

const Node& bare(const Node& n) {
    const Node* p = &n;
    while (p->kind == Kind::ParenExpr ||
           (p->kind == Kind::ImplicitCastExpr && (p->cast_kind == CastKind::LValueToRValue ||
                                                  p->cast_kind == CastKind::NoOp || p->cast_kind == CastKind::IntegralCast)))
        p = p->inner.empty() ? &empty_node() : &p->inner[0];
    return *p;
}

bool nonneg(const Node& n) {
    const Node& b = bare(n);
    return (b.kind == Kind::IntegerLiteral || b.kind == Kind::CharacterLiteral) && b.value.value_or(0) >= 0;
}

const Node* compound(const Node& n, const Source& src) {
    const Node& b = bare(n);
    if (b.kind == Kind::BinaryOperator && is_arith(b.op)) {
        if (b.op == Op::And || b.op == Op::Or || b.op == Op::Xor) {
            // bits of two 16-bit values stay 16 bits - unless a signed one meets an unsigned
            bool any_unsigned = false, any_signed = false;
            for (std::size_t i = 0; i < b.inner.size() && i < 2; ++i) {
                const std::optional<Prim> t = old_type(b.inner[i], src);
                if (t == Prim::U16) any_unsigned = true;
                if (t == Prim::I16 && !nonneg(b.inner[i])) any_signed = true;
            }
            if (!(any_unsigned && any_signed)) return nullptr;
        }
        return &b;
    }
    if (b.kind == Kind::UnaryOperator && (b.op == Op::Sub || b.op == Op::BitNot)) {
        const Node& operand = bare(b.inner.empty() ? empty_node() : b.inner[0]);
        if (operand.kind != Kind::IntegerLiteral && operand.kind != Kind::CharacterLiteral) return &b;
    }
    return nullptr;
}

bool simple(const Node& n, const Source& src) {
    const std::string_view s = text::trim(spelled(n, src).value_or(""));
    if (s.empty()) return false;
    return std::ranges::all_of(s, [](char c) {
        return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_' || c == '.' ||
               c == '-' || c == '>' || c == '[' || c == ']';
    });
}

// ---- the casts -----------------------------------------------------------------------------------------------------

namespace {

class Caster {
public:
    Caster(const Source& src) : src_(src) {}

    std::vector<Edit> take() { return std::move(edits_); }

    void walk(const Node& n) {
        if (n.kind == Kind::BinaryOperator && is_test(n.op) && n.inner.size() >= 2) compare(n);
        if ((n.kind == Kind::ImplicitCastExpr || n.kind == Kind::CStyleCastExpr) &&
            n.cast_kind == CastKind::IntegralCast && !n.inner.empty() && (n.prim == Prim::I32 || n.prim == Prim::U32)) {
            // 16-bit arithmetic made a long: cut first
            const Node* y = compound(n.inner[0], src_);
            const std::optional<Prim> t = old_type(n.inner[0], src_);
            if (y && (t == Prim::U16 || t == Prim::I16)) cast(*y, *t, true);
        }
        for (const Node& child : n.inner) walk(child);
    }

private:
    void compare(const Node& n) {
        const std::optional<Prim> a = old_type(n.inner[0], src_);
        const std::optional<Prim> b = old_type(n.inner[1], src_);
        const std::optional<Prim> c = n.op == Op::Shr ? a : common(a, b);
        if (c != Prim::U16 && c != Prim::I16) return;
        for (std::size_t i = 0; i < 2; ++i) {
            if (n.op == Op::Shr && i == 1) continue;   // (the count of a shift is its own)
            const Node& x = n.inner[i];
            const std::optional<Prim> t = i == 0 ? a : b;
            const Node* y = compound(x, src_);
            if (*c == Prim::U16 && t == Prim::I16 && !nonneg(x)) {
                // the 8086 made it unsigned; here it stays signed
                cast(y ? *y : x, Prim::U16, y != nullptr || !simple(x, src_));
            } else if (y) {
                cast(*y, *c, true);   // 16 bits of arithmetic, cut as the 8086's were
            }
        }
    }

    // A cast of `type` on the expression: before it, and (`wrap`) around it in parentheses.
    void cast(const Node& n, Prim type, bool wrap) {
        const std::optional<Span> p = place(n, src_.text);
        if (!p || p->file != src_.main || p->end < p->start || p->end > src_.text.size()) return;
        const std::string head = std::format("({})", cast_name(type));
        if (src_.text.substr(static_cast<std::size_t>(p->start), static_cast<std::size_t>(p->end - p->start)).starts_with(head)) return;   // (it is cast already)
        if (!done_.insert({p->start, p->end, type, wrap}).second) return;                      // (this very cast, again)
        edits_.push_back({p->start, wrap ? head + "(" : head});
        if (wrap) edits_.push_back({p->end, ")"});
    }

    const Source& src_;
    std::vector<Edit> edits_;
    std::set<std::tuple<std::uint64_t, std::uint64_t, Prim, bool>> done_;
};

}  // namespace

std::vector<Edit> edits_of(const Tree& tree, std::string_view text, FileId main) {
    if (main == kNoFile) return {};
    const Source src{text, main};
    Caster caster(src);
    for (const Node& top : tree.top()) caster.walk(top);
    return caster.take();
}

std::string apply_edits(std::string_view text, std::span<const Edit> edits) {
    std::vector<const Edit*> order;
    order.reserve(edits.size());
    for (const Edit& e : edits) {
        if (e.at > text.size()) throw UsageError(std::format("an edit at {} is past the end of a text of {}", e.at, text.size()));
        order.push_back(&e);
    }
    // a ")" ends an expression where the next one may begin: it goes first
    std::ranges::stable_sort(order, [](const Edit* a, const Edit* b) {
        if (a->at != b->at) return a->at < b->at;
        return (a->text == ")") > (b->text == ")");
    });
    std::string out;
    std::size_t done = 0;
    for (const Edit* e : order) {
        const std::size_t at = static_cast<std::size_t>(e->at);
        out.append(text.substr(done, at - done));
        out.append(e->text);
        done = at;
    }
    out.append(text.substr(done));
    return out;
}

// ---- keeping the results -------------------------------------------------------------------------------------------

void declare_cache(base::cache::Cache& cache) {
    cache.declare_kind(kCacheKind, {.scope = base::cache::Scope::State, .generation = std::string(kCodeVersion)});
}

std::vector<std::string> included_names(std::string_view text) {
    std::vector<std::string> names;
    const auto blank = [](char c) { return c == ' ' || c == '\t' || c == '\f' || c == '\v'; };
    for (std::size_t at = text.find('#'); at != std::string_view::npos; at = text.find('#', at + 1)) {
        std::size_t i = at + 1;
        while (i < text.size() && blank(text[i])) ++i;
        if (!text.substr(i).starts_with("include")) continue;
        i += 7;
        while (i < text.size() && blank(text[i])) ++i;
        if (i >= text.size() || (text[i] != '<' && text[i] != '"')) continue;
        ++i;
        std::size_t end = i;
        while (end < text.size() && text[end] != '>' && text[end] != '"' && text[end] != '\n' && text[end] != '\r') ++end;
        if (end == i || end >= text.size() || (text[end] != '>' && text[end] != '"')) continue;
        names.emplace_back(text.substr(i, end - i));
    }
    return names;
}

std::string reach_key(const std::filesystem::path& folder, std::string_view file) {
    std::vector<std::string> parts;   // (name, contents) of each file reached, in the order they were reached
    std::vector<std::string> todo{std::string(file)};
    std::set<std::string> seen;
    while (!todo.empty()) {
        const std::string name = std::move(todo.back());
        todo.pop_back();
        if (!seen.insert(name).second) continue;
        if (!text::is_valid_utf8(name)) continue;   // (a name no file of the port can have)
        const std::filesystem::path path = folder / fs::from_text(name);
        std::error_code ec;
        if (!std::filesystem::is_regular_file(path, ec)) continue;   // (not part of the port: a system header)
        std::string text = fs::read_text(path);
        for (std::string& included : included_names(text)) todo.push_back(std::move(included));
        parts.push_back(name);
        parts.push_back(std::move(text));
    }
    std::vector<base::cache::KeyPart> keys(parts.begin(), parts.end());
    return base::cache::key(keys);
}

std::string pass_key(const std::filesystem::path& clang, const std::filesystem::path& folder, std::string_view file,
                     std::string_view kind, std::string_view code_version, std::span<const std::string> extra) {
    const std::vector<std::string> extras(extra.begin(), extra.end());
    return base::cache::key({base::cache::tool_id(clang), kind, code_version, extras, reach_key(folder, file)});
}

std::map<std::string, std::string> run(base::cache::Cache& cache, const std::filesystem::path& clang,
                                       const std::filesystem::path& folder, std::span<const std::string> files) {
    using Result = std::optional<std::string>;   // the file's text with the casts; nothing when it needs none
    std::map<std::string, std::string> out;
    for (const std::string& f : files) {
        const std::string key = pass_key(clang, folder, f, "arith16", kCodeVersion);
        if (const std::optional<Result> kept = cache.value<Result>(kCacheKind, key)) {
            if (*kept) out[f] = **kept;
            continue;
        }
        const std::optional<Tree> tree = dump(clang, f, folder);
        if (!tree) continue;
        const std::string text = fs::read_text(folder / fs::from_text(f));
        const std::vector<Edit> edits = edits_of(*tree, text, tree->find_file(f));
        Result made;
        if (!edits.empty()) made = apply_edits(text, edits);
        if (made) out[f] = *made;
        cache.keep(kCacheKind, key, made);
    }
    return out;
}

}  // namespace cb::port::arith16
