// lexer.hpp -- pyfront's tokenizer (internal).
//
// Follows CPython's Parser/tokenizer: INDENT/DEDENT from a stack of indentation columns
// (tabs to multiples of 8, checked against a tab-size-1 column for consistency), implicit
// line joining inside brackets, backslash continuation, and PEP 701 f-strings tokenized as
// FSTRING_START (FSTRING_MIDDLE | '{' expression tokens ... '}')* FSTRING_END with a stack
// of f-string modes.  Tokens are produced on demand, so an error late in a file is raised
// only when the parser reaches it.
#pragma once

#include <cstdint>
#include <string>
#include <string_view>
#include <vector>

namespace pyfront {

struct SyntaxErrorInfo {
    std::string msg;
    int lineno = 0, col = 0;          // col: 0-based byte column
    int end_lineno = 0, end_col = 0;  // 0 when unknown
};

// Thrown by the lexer and the parser; caught at the parse() boundary.
struct ParseFailure {
    SyntaxErrorInfo info;
};

enum class Tok : uint8_t { EndMarker, Name, Number, String, FStringStart, FStringMiddle, FStringEnd, Newline, Indent, Dedent, Op };

enum class Op : uint8_t {
    None, LPar, RPar, LSqb, RSqb, LBrace, RBrace, Colon, Comma, Semi, Plus, Minus, Star, Slash,
    VBar, Amper, Less, Greater, Equal, Dot, Percent, EqEqual, NotEqual, LessEqual, GreaterEqual,
    Tilde, Circumflex, LeftShift, RightShift, DoubleStar, PlusEqual, MinEqual, StarEqual,
    SlashEqual, PercentEqual, AmperEqual, VBarEqual, CircumflexEqual, LeftShiftEqual,
    RightShiftEqual, DoubleStarEqual, DoubleSlash, DoubleSlashEqual, At, AtEqual, RArrow,
    Ellipsis, ColonEqual, Exclamation
};

enum class Kw : uint8_t {
    None, False_, None_, True_, And, As, Assert, Async, Await, Break, Class, Continue, Def, Del,
    Elif, Else, Except, Finally, For, From, Global, If, Import, In, Is, Lambda, Nonlocal, Not,
    Or, Pass, Raise, Return, Try, While, With, Yield
};

struct Token {
    Tok type = Tok::EndMarker;
    Op op = Op::None;
    Kw kw = Kw::None;
    uint32_t start = 0, end = 0;   // byte offsets into Lexer::buf
    int lineno = 0, col = 0, end_lineno = 0, end_col = 0;
};

class Lexer {
public:
    explicit Lexer(std::string_view source);

    // The normalized source: BOM removed, CRLF and CR turned into LF, a final LF added.
    const std::string& buffer() const { return buf; }
    std::string_view text(const Token& t) const { return std::string_view(buf).substr(t.start, t.end - t.start); }

    Token next();

    // Line and column (0-based bytes) of a buffer offset.
    void locate(uint32_t off, int& line, int& col);

    [[noreturn]] void fail(const std::string& msg, int line, int col, int end_line = 0, int end_col = 0);

private:
    struct Field { size_t depth; bool in_spec; };
    struct FMode {
        char quote; bool triple; bool raw;
        std::vector<Field> fields;
    };

    std::string buf;
    std::vector<uint32_t> line_starts;
    uint32_t pos = 0;
    int hint_line = 0;          // index into line_starts for locate()
    bool at_bol = true;
    int pending_dedents = 0;
    bool emitted_any = false;   // a token other than NEWLINE/INDENT/DEDENT since the last NEWLINE
    std::vector<int> indents{0}, alt_indents{0};
    struct Bracket { char c; uint32_t off; };
    std::vector<Bracket> brackets;
    std::vector<FMode> fmodes;
    bool eof_done = false;

    Token make(Tok t, uint32_t s, uint32_t e, Op op = Op::None);
    Token scan_fstring_literal();
    Token scan_string(uint32_t s);
    Token scan_number(uint32_t s);
    Token scan_op(uint32_t s);
    bool handle_indent(Token& out);
    void check_identifier(uint32_t s, uint32_t e);
};

}  // namespace pyfront
