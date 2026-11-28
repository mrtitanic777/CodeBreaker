// formats/format_desc.hpp -- a parsed format description (config/formats/<name>.fmt) as data: what the engine
// (format_engine.cpp) reads files through and what the reader generator (format_gen_main.cpp, docs/FORMAT_CODEGEN.md)
// turns into typed C++ readers. parse_description() is the engine's own parser.
#pragma once

#include <cstdint>
#include <map>
#include <memory>
#include <set>
#include <string>
#include <string_view>
#include <vector>

namespace cb::formats::engine {

namespace desc {

struct Expr;
using ExprP = std::shared_ptr<const Expr>;
struct Expr {
    enum class Op { Num, Text, Name, Special, Unary, Binary, Cond, Call, Member, Index };
    // What a Unary, Binary, Special or Call is, decided once when the description is parsed (`code`).
    enum Code : std::uint8_t {
        None,
        Neg, Not, BitNot,
        Add, Sub, Mul, Div, Mod, Shl, Shr, BitAnd, BitOr, BitXor, Eq, Ne, Lt, Le, Gt, Ge, And, Or,
        Pos, Start, End, Remaining, Size, Index, Var,
        Xlat, Scan, Count, At, Ends, Peek8, Peek16, Peek32, Peek16be, Peek32be, PeekBad, Sum8, Printable, Min, Max, Align, OtherCall,
    };
    Op op = Op::Num;
    Code code = None;
    std::int64_t num = 0;
    std::string name;          // a Name, a Special, a Member, a Call's function; a Unary/Binary's operator
    std::vector<ExprP> kids;
};

// The code of an operator, a special name or a built-in function.
inline Expr::Code expr_code(Expr::Op op, std::string_view n) {
    using C = Expr::Code;
    switch (op) {
        case Expr::Op::Unary: return n == "-" ? C::Neg : n == "!" ? C::Not : C::BitNot;
        case Expr::Op::Binary: {
            static constexpr std::pair<std::string_view, C> ops[] = {
                {"+", C::Add}, {"-", C::Sub}, {"*", C::Mul}, {"/", C::Div}, {"%", C::Mod}, {"<<", C::Shl}, {">>", C::Shr}, {"&", C::BitAnd},
                {"|", C::BitOr}, {"^", C::BitXor}, {"==", C::Eq}, {"!=", C::Ne}, {"<", C::Lt}, {"<=", C::Le}, {">", C::Gt}, {"&&", C::And}, {"||", C::Or}};
            for (const auto& [text, code] : ops)
                if (n == text) return code;
            return C::Ge;
        }
        case Expr::Op::Special: {
            static constexpr std::pair<std::string_view, C> names[] = {
                {"pos", C::Pos}, {"start", C::Start}, {"end", C::End}, {"remaining", C::Remaining}, {"size", C::Size}, {"index", C::Index}};
            for (const auto& [text, code] : names)
                if (n == text) return code;
            return C::Var;
        }
        case Expr::Op::Call: {
            static constexpr std::pair<std::string_view, C> fns[] = {
                {"xlat", C::Xlat}, {"scan", C::Scan}, {"count", C::Count}, {"at", C::At}, {"ends", C::Ends}, {"peek8", C::Peek8}, {"peek16", C::Peek16},
                {"peek32", C::Peek32}, {"peek16be", C::Peek16be}, {"peek32be", C::Peek32be}, {"sum8", C::Sum8}, {"printable", C::Printable},
                {"min", C::Min}, {"max", C::Max}, {"align", C::Align}};
            for (const auto& [text, code] : fns)
                if (n == text) return code;
            return n.starts_with("peek") ? C::PeekBad : C::OtherCall;
        }
        default: return C::None;
    }
}

struct Literal {
    bool is_text = false;
    std::string text;
    std::uint64_t num = 0;
};

struct TypeRef {
    enum class K { Int, Str, Bytes, Cstr, Struct } k = K::Int;
    unsigned width = 0;
    bool big = false, sgn = false, endian_set = false;
    ExprP len;
    std::string name;
};

enum class Arr { None, Count, Until, Size, While };

struct FieldDef {
    std::string name;
    ExprP at;
    TypeRef type;
    Arr arr = Arr::None;
    ExprP arr_expr;
    ExprP arr_until;   // `[while a until b]`: b
    std::string as;
    std::vector<Literal> magic;
    ExprP window;
    ExprP when;
    bool tolerant = false;   // `try`
    std::string part;        // `part NAME`
};

struct FnDef {
    std::vector<std::string> params;
    ExprP body;
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

namespace desc {

// A piece of a detect rule's detail: literal text, then (when `value` is set) a number in `spec`'s std::format form, or
// with `length` the text of `length` raw bytes at `value` (its NULs at the end left out).
struct DetailPiece {
    std::string text;
    ExprP value, length;
    std::string spec;
    bool trim = true;   // str(): the NULs at the end left out; raw(): kept
};

// `detect PRIORITY LANE "FORMAT" family F command C when EXPR detail "TEXT {expr:spec} {str(at, n)}"`
struct DetectDef {
    int priority = 0;
    std::string lane, format, family, command;
    ExprP when;
    std::vector<DetailPiece> detail;
    std::uint32_t line = 0;
};

}  // namespace desc

class Format {
public:
    std::vector<desc::DetectDef> detects;
    std::string name, origin;
    bool big = false;
    std::string root = "File";
    std::vector<std::string> uses;
    std::map<std::string, desc::StructDef, std::less<>> structs;
    std::map<std::string, desc::EnumDef, std::less<>> enums;
    std::map<std::string, desc::FnDef, std::less<>> fns;
    std::set<std::string> parts;
};

// Parse a description (ParseError on a mistake in it; `origin`, a file name, only goes into the messages).
std::shared_ptr<Format> parse_description(std::string_view text, std::string_view origin);

}  // namespace cb::formats::engine
