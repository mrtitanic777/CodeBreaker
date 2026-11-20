// pyfront/ast.hpp -- the Python 3 abstract syntax tree as plain C++ types.
//
// The shape is CPython's `ast` module (Python 3.14): one struct per node class, the same
// fields in the same order with the same names, and the same positions (lineno and
// end_lineno 1-based, col_offset and end_col_offset in UTF-8 bytes from the start of the
// line).  A pyfront tree dumped with pyfront::dump() is byte for byte what
// `ast.dump(ast.parse(src), include_attributes=True)` prints.
//
// Ownership: every node lives in its Module's arena and is freed with the Module.  Child
// links are plain pointers (nullptr where CPython has None); lists are std::vector.
// Dispatch on the `kind` field of Expr, Stmt, Pattern and TypeParam, then downcast with
// pyfront::as<T>(node) (nullptr when the kind does not match) or pyfront::cast<T>(node).
#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

namespace pyfront {

struct Pos {
    int lineno = 0;
    int col_offset = 0;      // UTF-8 bytes
    int end_lineno = 0;
    int end_col_offset = 0;  // UTF-8 bytes
};

// Every node derives from Node so the arena can own it.  Nodes CPython gives no positions
// (arguments, comprehension, withitem, match_case) carry a Pos that is left zero.
struct Node {
    Pos pos;
    virtual ~Node() = default;
};

enum class ExprCtx : uint8_t { Load, Store, Del };
enum class BoolOpKind : uint8_t { And, Or };
enum class OpKind : uint8_t { Add, Sub, Mult, MatMult, Div, Mod, Pow, LShift, RShift, BitOr, BitXor, BitAnd, FloorDiv };
enum class UnaryOpKind : uint8_t { Invert, Not, UAdd, USub };
enum class CmpOpKind : uint8_t { Eq, NotEq, Lt, LtE, Gt, GtE, Is, IsNot, In, NotIn };

const char* name_of(ExprCtx);
const char* name_of(BoolOpKind);
const char* name_of(OpKind);
const char* name_of(UnaryOpKind);
const char* name_of(CmpOpKind);

// ---------------------------------------------------------------- constants

// The value of a Constant (or MatchSingleton).  Python's int is unbounded, so an Int keeps
// its decimal digits; `i64` holds the value when `fits_i64`.  Str is UTF-8 (a lone
// surrogate from a \uD800 escape is kept in its 3-byte generalized UTF-8 form); Bytes is
// the raw bytes.  Float and Complex use `f` (Complex is always 0 + f*j, as in CPython's
// literal `2j`).
struct ConstValue {
    enum class Kind : uint8_t { None, True, False, Ellipsis, Int, Float, Complex, Str, Bytes };
    Kind kind = Kind::None;
    std::string text;      // Int: decimal digits; Str: UTF-8; Bytes: raw bytes
    double f = 0.0;        // Float, Complex (imaginary part)
    int64_t i64 = 0;       // Int, when fits_i64
    bool fits_i64 = false;
};

// ---------------------------------------------------------------- expressions

enum class ExprKind : uint8_t {
    BoolOp, NamedExpr, BinOp, UnaryOp, Lambda, IfExp, Dict, Set, ListComp, SetComp, DictComp,
    GeneratorExp, Await, Yield, YieldFrom, Compare, Call, FormattedValue, Interpolation,
    JoinedStr, TemplateStr, Constant, Attribute, Subscript, Starred, Name, List, Tuple, Slice
};
const char* name_of(ExprKind);

struct Expr : Node {
    const ExprKind kind;
    explicit Expr(ExprKind k) : kind(k) {}
};
template <ExprKind K> struct ExprOf : Expr {
    static constexpr ExprKind Kind = K;
    ExprOf() : Expr(K) {}
};

struct arguments;
struct comprehension;
struct keyword;

struct BoolOp : ExprOf<ExprKind::BoolOp> { BoolOpKind op{}; std::vector<Expr*> values; };
struct NamedExpr : ExprOf<ExprKind::NamedExpr> { Expr* target = nullptr; Expr* value = nullptr; };
struct BinOp : ExprOf<ExprKind::BinOp> { Expr* left = nullptr; OpKind op{}; Expr* right = nullptr; };
struct UnaryOp : ExprOf<ExprKind::UnaryOp> { UnaryOpKind op{}; Expr* operand = nullptr; };
struct Lambda : ExprOf<ExprKind::Lambda> { arguments* args = nullptr; Expr* body = nullptr; };
struct IfExp : ExprOf<ExprKind::IfExp> { Expr* test = nullptr; Expr* body = nullptr; Expr* orelse = nullptr; };
// keys[i] is nullptr for a `**mapping` entry, as in CPython.
struct Dict : ExprOf<ExprKind::Dict> { std::vector<Expr*> keys; std::vector<Expr*> values; };
struct Set : ExprOf<ExprKind::Set> { std::vector<Expr*> elts; };
struct ListComp : ExprOf<ExprKind::ListComp> { Expr* elt = nullptr; std::vector<comprehension*> generators; };
struct SetComp : ExprOf<ExprKind::SetComp> { Expr* elt = nullptr; std::vector<comprehension*> generators; };
struct DictComp : ExprOf<ExprKind::DictComp> { Expr* key = nullptr; Expr* value = nullptr; std::vector<comprehension*> generators; };
struct GeneratorExp : ExprOf<ExprKind::GeneratorExp> { Expr* elt = nullptr; std::vector<comprehension*> generators; };
struct Await : ExprOf<ExprKind::Await> { Expr* value = nullptr; };
struct Yield : ExprOf<ExprKind::Yield> { Expr* value = nullptr; };
struct YieldFrom : ExprOf<ExprKind::YieldFrom> { Expr* value = nullptr; };
struct Compare : ExprOf<ExprKind::Compare> { Expr* left = nullptr; std::vector<CmpOpKind> ops; std::vector<Expr*> comparators; };
struct Call : ExprOf<ExprKind::Call> { Expr* func = nullptr; std::vector<Expr*> args; std::vector<keyword*> keywords; };
// conversion: -1 none, or the character code ('s' 115, 'r' 114, 'a' 97).
struct FormattedValue : ExprOf<ExprKind::FormattedValue> { Expr* value = nullptr; int conversion = -1; Expr* format_spec = nullptr; };
struct Interpolation : ExprOf<ExprKind::Interpolation> { Expr* value = nullptr; std::string str; int conversion = -1; Expr* format_spec = nullptr; };
struct JoinedStr : ExprOf<ExprKind::JoinedStr> { std::vector<Expr*> values; };
struct TemplateStr : ExprOf<ExprKind::TemplateStr> { std::vector<Expr*> values; };
// kind is "u" for a string whose first piece had a u prefix, else empty (CPython's None).
struct Constant : ExprOf<ExprKind::Constant> { ConstValue value; std::string kind; };
struct Attribute : ExprOf<ExprKind::Attribute> { Expr* value = nullptr; std::string attr; ExprCtx ctx = ExprCtx::Load; };
struct Subscript : ExprOf<ExprKind::Subscript> { Expr* value = nullptr; Expr* slice = nullptr; ExprCtx ctx = ExprCtx::Load; };
struct Starred : ExprOf<ExprKind::Starred> { Expr* value = nullptr; ExprCtx ctx = ExprCtx::Load; };
struct Name : ExprOf<ExprKind::Name> { std::string id; ExprCtx ctx = ExprCtx::Load; };
struct List : ExprOf<ExprKind::List> { std::vector<Expr*> elts; ExprCtx ctx = ExprCtx::Load; };
struct Tuple : ExprOf<ExprKind::Tuple> { std::vector<Expr*> elts; ExprCtx ctx = ExprCtx::Load; };
struct Slice : ExprOf<ExprKind::Slice> { Expr* lower = nullptr; Expr* upper = nullptr; Expr* step = nullptr; };

// ---------------------------------------------------------------- helper nodes

struct comprehension : Node { Expr* target = nullptr; Expr* iter = nullptr; std::vector<Expr*> ifs; int is_async = 0; };
struct arg : Node { std::string arg; Expr* annotation = nullptr; std::string type_comment; };
// kw_defaults[i] is nullptr where keyword-only argument i has no default.
struct arguments : Node {
    std::vector<arg*> posonlyargs, args;
    arg* vararg = nullptr;
    std::vector<arg*> kwonlyargs;
    std::vector<Expr*> kw_defaults;
    arg* kwarg = nullptr;
    std::vector<Expr*> defaults;
};
// arg is empty for `**mapping` (CPython's None).
struct keyword : Node { std::string arg; bool has_arg = false; Expr* value = nullptr; };
struct alias : Node { std::string name; std::string asname; bool has_asname = false; };
struct withitem : Node { Expr* context_expr = nullptr; Expr* optional_vars = nullptr; };

// ---------------------------------------------------------------- patterns (match)

enum class PatternKind : uint8_t { MatchValue, MatchSingleton, MatchSequence, MatchMapping, MatchClass, MatchStar, MatchAs, MatchOr };
const char* name_of(PatternKind);

struct Pattern : Node {
    const PatternKind kind;
    explicit Pattern(PatternKind k) : kind(k) {}
};
template <PatternKind K> struct PatternOf : Pattern {
    static constexpr PatternKind Kind = K;
    PatternOf() : Pattern(K) {}
};
struct MatchValue : PatternOf<PatternKind::MatchValue> { Expr* value = nullptr; };
struct MatchSingleton : PatternOf<PatternKind::MatchSingleton> { ConstValue value; };
struct MatchSequence : PatternOf<PatternKind::MatchSequence> { std::vector<Pattern*> patterns; };
struct MatchMapping : PatternOf<PatternKind::MatchMapping> { std::vector<Expr*> keys; std::vector<Pattern*> patterns; std::string rest; bool has_rest = false; };
struct MatchClass : PatternOf<PatternKind::MatchClass> { Expr* cls = nullptr; std::vector<Pattern*> patterns; std::vector<std::string> kwd_attrs; std::vector<Pattern*> kwd_patterns; };
struct MatchStar : PatternOf<PatternKind::MatchStar> { std::string name; bool has_name = false; };
struct MatchAs : PatternOf<PatternKind::MatchAs> { Pattern* pattern = nullptr; std::string name; bool has_name = false; };
struct MatchOr : PatternOf<PatternKind::MatchOr> { std::vector<Pattern*> patterns; };

struct Stmt;
struct match_case : Node { Pattern* pattern = nullptr; Expr* guard = nullptr; std::vector<Stmt*> body; };

// ---------------------------------------------------------------- type parameters (PEP 695)

enum class TypeParamKind : uint8_t { TypeVar, ParamSpec, TypeVarTuple };
struct TypeParam : Node {
    const TypeParamKind kind;
    std::string name;
    Expr* bound = nullptr;          // TypeVar only
    Expr* default_value = nullptr;
    explicit TypeParam(TypeParamKind k) : kind(k) {}
};

// ---------------------------------------------------------------- statements

enum class StmtKind : uint8_t {
    FunctionDef, AsyncFunctionDef, ClassDef, Return, Delete, Assign, TypeAlias, AugAssign,
    AnnAssign, For, AsyncFor, While, If, With, AsyncWith, Match, Raise, Try, TryStar, Assert,
    Import, ImportFrom, Global, Nonlocal, Expr, Pass, Break, Continue
};
const char* name_of(StmtKind);

struct Stmt : Node {
    const StmtKind kind;
    explicit Stmt(StmtKind k) : kind(k) {}
};
template <StmtKind K> struct StmtOf : Stmt {
    static constexpr StmtKind Kind = K;
    StmtOf() : Stmt(K) {}
};

struct ExceptHandler : Node { pyfront::Expr* type = nullptr; std::string name; bool has_name = false; std::vector<Stmt*> body; };

// FunctionDef and AsyncFunctionDef share one layout; `kind` tells them apart.
struct FunctionDef : Stmt {
    std::string name;
    arguments* args = nullptr;
    std::vector<Stmt*> body;
    std::vector<pyfront::Expr*> decorator_list;
    pyfront::Expr* returns = nullptr;
    std::string type_comment;
    std::vector<TypeParam*> type_params;
    explicit FunctionDef(bool is_async = false) : Stmt(is_async ? StmtKind::AsyncFunctionDef : StmtKind::FunctionDef) {}
};
struct ClassDef : StmtOf<StmtKind::ClassDef> {
    std::string name;
    std::vector<pyfront::Expr*> bases;
    std::vector<keyword*> keywords;
    std::vector<Stmt*> body;
    std::vector<pyfront::Expr*> decorator_list;
    std::vector<TypeParam*> type_params;
};
struct Return : StmtOf<StmtKind::Return> { pyfront::Expr* value = nullptr; };
struct Delete : StmtOf<StmtKind::Delete> { std::vector<pyfront::Expr*> targets; };
struct Assign : StmtOf<StmtKind::Assign> { std::vector<pyfront::Expr*> targets; pyfront::Expr* value = nullptr; std::string type_comment; };
struct TypeAlias : StmtOf<StmtKind::TypeAlias> { pyfront::Expr* name = nullptr; std::vector<TypeParam*> type_params; pyfront::Expr* value = nullptr; };
struct AugAssign : StmtOf<StmtKind::AugAssign> { pyfront::Expr* target = nullptr; OpKind op{}; pyfront::Expr* value = nullptr; };
// simple is 1 for a bare name target (not parenthesized), else 0.
struct AnnAssign : StmtOf<StmtKind::AnnAssign> { pyfront::Expr* target = nullptr; pyfront::Expr* annotation = nullptr; pyfront::Expr* value = nullptr; int simple = 0; };
// For and AsyncFor share one layout.
struct For : Stmt {
    pyfront::Expr* target = nullptr;
    pyfront::Expr* iter = nullptr;
    std::vector<Stmt*> body, orelse;
    std::string type_comment;
    explicit For(bool is_async = false) : Stmt(is_async ? StmtKind::AsyncFor : StmtKind::For) {}
};
struct While : StmtOf<StmtKind::While> { pyfront::Expr* test = nullptr; std::vector<Stmt*> body, orelse; };
struct If : StmtOf<StmtKind::If> { pyfront::Expr* test = nullptr; std::vector<Stmt*> body, orelse; };
// With and AsyncWith share one layout.
struct With : Stmt {
    std::vector<withitem*> items;
    std::vector<Stmt*> body;
    std::string type_comment;
    explicit With(bool is_async = false) : Stmt(is_async ? StmtKind::AsyncWith : StmtKind::With) {}
};
struct Match : StmtOf<StmtKind::Match> { pyfront::Expr* subject = nullptr; std::vector<match_case*> cases; };
struct Raise : StmtOf<StmtKind::Raise> { pyfront::Expr* exc = nullptr; pyfront::Expr* cause = nullptr; };
// Try and TryStar (except*) share one layout.
struct Try : Stmt {
    std::vector<Stmt*> body;
    std::vector<ExceptHandler*> handlers;
    std::vector<Stmt*> orelse, finalbody;
    explicit Try(bool star = false) : Stmt(star ? StmtKind::TryStar : StmtKind::Try) {}
};
struct Assert : StmtOf<StmtKind::Assert> { pyfront::Expr* test = nullptr; pyfront::Expr* msg = nullptr; };
struct Import : StmtOf<StmtKind::Import> { std::vector<alias*> names; };
// module is empty for `from . import x` (CPython's None); has_module tells.
struct ImportFrom : StmtOf<StmtKind::ImportFrom> { std::string module; bool has_module = false; std::vector<alias*> names; int level = 0; };
struct Global : StmtOf<StmtKind::Global> { std::vector<std::string> names; };
struct Nonlocal : StmtOf<StmtKind::Nonlocal> { std::vector<std::string> names; };
struct ExprStmt : StmtOf<StmtKind::Expr> { pyfront::Expr* value = nullptr; };   // CPython's ast.Expr
struct Pass : StmtOf<StmtKind::Pass> {};
struct Break : StmtOf<StmtKind::Break> {};
struct Continue : StmtOf<StmtKind::Continue> {};

// ---------------------------------------------------------------- module

struct Module {
    std::vector<Stmt*> body;
    std::vector<std::unique_ptr<Node>> arena;   // owns every node of this tree

    template <class T, class... A> T* make(A&&... a) {
        auto p = std::make_unique<T>(std::forward<A>(a)...);
        T* raw = p.get();
        arena.push_back(std::move(p));
        return raw;
    }
};

// ---------------------------------------------------------------- downcasts

template <class T> T* as(Expr* e) { return e && e->kind == T::Kind ? static_cast<T*>(e) : nullptr; }
template <class T> const T* as(const Expr* e) { return e && e->kind == T::Kind ? static_cast<const T*>(e) : nullptr; }
template <class T> T* as(Stmt* s) { return s && s->kind == T::Kind ? static_cast<T*>(s) : nullptr; }
template <class T> const T* as(const Stmt* s) { return s && s->kind == T::Kind ? static_cast<const T*>(s) : nullptr; }
template <class T> T* as(Pattern* p) { return p && p->kind == T::Kind ? static_cast<T*>(p) : nullptr; }
template <class T> const T* as(const Pattern* p) { return p && p->kind == T::Kind ? static_cast<const T*>(p) : nullptr; }
// Unchecked downcast, for code that has already switched on `kind`.
template <class T, class B> T* cast(B* b) { return static_cast<T*>(b); }
template <class T, class B> const T* cast(const B* b) { return static_cast<const T*>(b); }

}  // namespace pyfront
