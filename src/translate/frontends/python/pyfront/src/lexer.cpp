// lexer.cpp -- pyfront's tokenizer.  See lexer.hpp.
#include "lexer.hpp"
#include "unicode.hpp"

#include <algorithm>
#include <cstdio>
#include <cstring>

namespace pyfront {

namespace {

bool is_id_start(unsigned char c) { return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_' || c >= 0x80; }
bool is_id_char(unsigned char c) { return is_id_start(c) || (c >= '0' && c <= '9'); }
bool is_digit(unsigned char c) { return c >= '0' && c <= '9'; }
bool is_hex(unsigned char c) { return is_digit(c) || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'); }

Kw keyword_of(std::string_view s) {
    switch (s.size()) {
    case 2:
        if (s == "as") return Kw::As;
        if (s == "if") return Kw::If;
        if (s == "in") return Kw::In;
        if (s == "is") return Kw::Is;
        if (s == "or") return Kw::Or;
        break;
    case 3:
        if (s == "and") return Kw::And;
        if (s == "def") return Kw::Def;
        if (s == "del") return Kw::Del;
        if (s == "for") return Kw::For;
        if (s == "not") return Kw::Not;
        if (s == "try") return Kw::Try;
        break;
    case 4:
        if (s == "None") return Kw::None_;
        if (s == "True") return Kw::True_;
        if (s == "elif") return Kw::Elif;
        if (s == "else") return Kw::Else;
        if (s == "from") return Kw::From;
        if (s == "pass") return Kw::Pass;
        if (s == "with") return Kw::With;
        break;
    case 5:
        if (s == "False") return Kw::False_;
        if (s == "async") return Kw::Async;
        if (s == "await") return Kw::Await;
        if (s == "break") return Kw::Break;
        if (s == "class") return Kw::Class;
        if (s == "raise") return Kw::Raise;
        if (s == "while") return Kw::While;
        if (s == "yield") return Kw::Yield;
        break;
    case 6:
        if (s == "assert") return Kw::Assert;
        if (s == "except") return Kw::Except;
        if (s == "global") return Kw::Global;
        if (s == "import") return Kw::Import;
        if (s == "lambda") return Kw::Lambda;
        if (s == "return") return Kw::Return;
        break;
    case 7:
        if (s == "finally") return Kw::Finally;
        break;
    case 8:
        if (s == "continue") return Kw::Continue;
        if (s == "nonlocal") return Kw::Nonlocal;
        break;
    }
    return Kw::None;
}

// Is `s` a string prefix: any of r u b f t with the combinations Python allows.
bool is_string_prefix(std::string_view s, bool& f, bool& t, bool& raw, bool& bytes) {
    f = t = raw = bytes = false;
    bool u = false;
    if (s.size() > 2) return false;
    for (char c : s) {
        switch (c | 0x20) {
        case 'r': if (raw) return false; raw = true; break;
        case 'b': if (bytes) return false; bytes = true; break;
        case 'u': if (u) return false; u = true; break;
        case 'f': if (f) return false; f = true; break;
        case 't': if (t) return false; t = true; break;
        default: return false;
        }
    }
    if (u && s.size() > 1) return false;
    int kinds = int(bytes) + int(f) + int(t);
    return kinds <= 1;
}

}  // namespace

Lexer::Lexer(std::string_view source) {
    if (source.size() >= 3 && (unsigned char)source[0] == 0xEF && (unsigned char)source[1] == 0xBB && (unsigned char)source[2] == 0xBF)
        source.remove_prefix(3);
    buf.reserve(source.size() + 2);
    for (size_t i = 0; i < source.size(); ++i) {
        char c = source[i];
        if (c == '\r') {
            buf.push_back('\n');
            if (i + 1 < source.size() && source[i + 1] == '\n') ++i;
        } else {
            buf.push_back(c);
        }
    }
    if (buf.empty() || buf.back() != '\n') buf.push_back('\n');
    line_starts.push_back(0);
    for (uint32_t i = 0; i + 1 < buf.size(); ++i)
        if (buf[i] == '\n') line_starts.push_back(i + 1);
}

void Lexer::locate(uint32_t off, int& line, int& col) {
    size_t h = (size_t)hint_line;
    if (h >= line_starts.size() || line_starts[h] > off) {
        h = size_t(std::upper_bound(line_starts.begin(), line_starts.end(), off) - line_starts.begin()) - 1;
    } else {
        while (h + 1 < line_starts.size() && line_starts[h + 1] <= off) ++h;
    }
    hint_line = (int)h;
    line = (int)h + 1;
    col = (int)(off - line_starts[h]);
}

void Lexer::fail(const std::string& msg, int line, int col, int end_line, int end_col) {
    throw ParseFailure{SyntaxErrorInfo{msg, line, col, end_line, end_col}};
}

Token Lexer::make(Tok t, uint32_t s, uint32_t e, Op op) {
    Token k;
    k.type = t;
    k.op = op;
    k.start = s;
    k.end = e;
    locate(s, k.lineno, k.col);
    // The end of a token is the position just past its last byte (the start of the next
    // line when the token ends with a newline, as an f-string piece can).
    if (e > s && e < buf.size()) {
        locate(e, k.end_lineno, k.end_col);
    } else if (e > s) {
        int l, c;
        locate(e - 1, l, c);
        k.end_lineno = l;
        k.end_col = c + 1;
    } else {
        k.end_lineno = k.lineno;
        k.end_col = k.col;
    }
    return k;
}

bool Lexer::handle_indent(Token& out) {
    for (;;) {
        int col = 0, alt = 0;
        uint32_t p = pos;
        while (p < buf.size()) {
            char c = buf[p];
            if (c == ' ') { ++col; ++alt; }
            else if (c == '\t') { col = (col / 8 + 1) * 8; ++alt; }
            else if (c == '\f') { col = alt = 0; }
            else break;
            ++p;
        }
        if (p >= buf.size()) { pos = p; at_bol = false; return false; }
        char c = buf[p];
        if (c == '#' || c == '\n') {
            while (p < buf.size() && buf[p] != '\n') ++p;
            pos = p + 1;
            if (pos >= buf.size()) { pos = (uint32_t)buf.size(); at_bol = false; return false; }
            continue;
        }
        if (c == '\\' && p + 1 < buf.size() && buf[p + 1] == '\n') {
            // A continuation line starting the logical line: its indentation counts as
            // the line's, as in CPython; the tokens begin after the join.
        }
        pos = p;
        at_bol = false;
        if (col == indents.back()) {
            if (alt != alt_indents.back()) {
                int l, cc; locate(p, l, cc);
                fail("inconsistent use of tabs and spaces in indentation", l, cc);
            }
            return false;
        }
        if (col > indents.back()) {
            if (alt <= alt_indents.back()) {
                int l, cc; locate(p, l, cc);
                fail("inconsistent use of tabs and spaces in indentation", l, cc);
            }
            indents.push_back(col);
            alt_indents.push_back(alt);
            int l, cc;
            locate(p, l, cc);
            out = make(Tok::Indent, p - (uint32_t)cc, p);
            return true;
        }
        while (col < indents.back()) {
            indents.pop_back();
            alt_indents.pop_back();
            ++pending_dedents;
        }
        if (col != indents.back()) {
            // CPython points past the end of the offending line.
            uint32_t eol = p;
            while (eol < buf.size() && buf[eol] != '\n') ++eol;
            int l, cc; locate(p, l, cc);
            fail("unindent does not match any outer indentation level", l, cc + (int)(eol - p));
        }
        if (alt != alt_indents.back()) {
            int l, cc; locate(p, l, cc);
            fail("inconsistent use of tabs and spaces in indentation", l, cc);
        }
        --pending_dedents;
        out = make(Tok::Dedent, p, p);
        return true;
    }
}

Token Lexer::next() {
    if (pending_dedents > 0) {
        --pending_dedents;
        return make(Tok::Dedent, pos, pos);
    }
    if (!fmodes.empty() && (fmodes.back().fields.empty() || fmodes.back().fields.back().in_spec))
        return scan_fstring_literal();
    if (at_bol && brackets.empty()) {
        Token t;
        if (handle_indent(t)) return t;
    }
    for (;;) {
        if (pos >= buf.size()) break;
        char c = buf[pos];
        if (c == ' ' || c == '\t' || c == '\f') { ++pos; continue; }
        if (c == '#') {
            while (pos < buf.size() && buf[pos] != '\n') ++pos;
            continue;
        }
        if (c == '\\') {
            if (pos + 1 < buf.size() && buf[pos + 1] == '\n') {
                pos += 2;
                if (pos >= buf.size()) {
                    int l, cc; locate(pos - 1, l, cc);
                    fail("unexpected EOF while parsing", l, cc);
                }
                continue;
            }
            int l, cc; locate(pos, l, cc);
            fail("unexpected character after line continuation character", l, cc + 1);
        }
        if (c == '\n') {
            if (!brackets.empty()) { ++pos; continue; }
            if (!emitted_any) { ++pos; at_bol = true; return next(); }
            Token t = make(Tok::Newline, pos, pos + 1);
            ++pos;
            at_bol = true;
            emitted_any = false;
            return t;
        }
        break;
    }
    if (pos >= buf.size()) {
        if (!fmodes.empty()) {
            fail("unterminated f-string literal", 0, 0);
        }
        if (!brackets.empty()) {
            int l, cc; locate(brackets.back().off, l, cc);
            fail(std::string("'") + brackets.back().c + "' was never closed", l, cc);
        }
        if (indents.size() > 1) {
            indents.pop_back();
            alt_indents.pop_back();
            return make(Tok::Dedent, pos, pos);
        }
        return make(Tok::EndMarker, pos, pos);
    }
    emitted_any = true;
    uint32_t s = pos;
    unsigned char c = (unsigned char)buf[pos];
    if (is_id_start(c)) {
        uint32_t e = pos + 1;
        while (e < buf.size() && is_id_char((unsigned char)buf[e])) ++e;
        std::string_view word(buf.data() + s, e - s);
        if (e < buf.size() && (buf[e] == '\'' || buf[e] == '"')) {
            bool f, t, raw, bytes;
            if (is_string_prefix(word, f, t, raw, bytes)) return scan_string(s);
        }
        for (uint32_t i = s; i < e; ++i) {
            if ((unsigned char)buf[i] < 0x80) continue;
            check_identifier(s, e);
            break;
        }
        pos = e;
        Token k = make(Tok::Name, s, e);
        k.kw = keyword_of(word);
        return k;
    }
    if (is_digit(c) || (c == '.' && pos + 1 < buf.size() && is_digit((unsigned char)buf[pos + 1])))
        return scan_number(s);
    if (c == '\'' || c == '"') return scan_string(s);
    return scan_op(s);
}

// A non-ASCII identifier: every character must be XID_Start (the first, or '_') or
// XID_Continue, as CPython's verify_identifier requires.
void Lexer::check_identifier(uint32_t s, uint32_t e) {
    for (uint32_t i = s; i < e;) {
        unsigned char c = (unsigned char)buf[i];
        uint32_t n = c < 0x80 ? 1 : c >= 0xF0 ? 4 : c >= 0xE0 ? 3 : c >= 0xC0 ? 2 : 1;
        if (i + n > e) n = 1;
        uint32_t cp = n == 1 ? c : (uint32_t)(c & (0x7F >> n));
        for (uint32_t k = 1; k < n; ++k) cp = (cp << 6) | ((unsigned char)buf[i + k] & 0x3F);
        if (cp >= 0x80) {
            int ok = i == s ? uni::xid_start(cp) : uni::xid_continue(cp);
            if (ok == 0) {
                int l, cc;
                locate(i, l, cc);
                char hex[16];
                snprintf(hex, sizeof hex, "%04X", cp);
                fail("invalid character '" + buf.substr(i, n) + "' (U+" + hex + ")", l, cc);
            }
        }
        i += n;
    }
}

Token Lexer::scan_string(uint32_t s) {
    uint32_t q = s;
    while (buf[q] != '\'' && buf[q] != '"') ++q;
    bool f, t, raw, bytes;
    is_string_prefix(std::string_view(buf.data() + s, q - s), f, t, raw, bytes);
    char quote = buf[q];
    bool triple = q + 2 < buf.size() && buf[q + 1] == quote && buf[q + 2] == quote;
    uint32_t body = q + (triple ? 3 : 1);
    if (f || t) {
        pos = body;
        fmodes.push_back(FMode{quote, triple, raw, {}});
        return make(Tok::FStringStart, s, body);
    }
    uint32_t p = body;
    for (;;) {
        if (p >= buf.size()) {
            int l, cc, el, ec;
            locate(s, l, cc);
            locate((uint32_t)buf.size() - 1, el, ec);
            if (triple)
                fail("unterminated triple-quoted string literal (detected at line " + std::to_string(el) + ")", l, cc);
            fail("unterminated string literal (detected at line " + std::to_string(el) + ")", l, cc);
        }
        char c = buf[p];
        if (c == '\\') { p += 2; continue; }
        if (c == '\n' && !triple) {
            int l, cc;
            locate(s, l, cc);
            fail("unterminated string literal (detected at line " + std::to_string(l) + ")", l, cc);
        }
        if (c == quote) {
            if (!triple) { ++p; break; }
            if (p + 2 < buf.size() && buf[p + 1] == quote && buf[p + 2] == quote) { p += 3; break; }
        }
        ++p;
    }
    pos = p;
    return make(Tok::String, s, p);
}

Token Lexer::scan_fstring_literal() {
    FMode& m = fmodes.back();
    bool in_spec = !m.fields.empty();
    uint32_t s = pos;
    uint32_t p = pos;
    for (;;) {
        if (p >= buf.size()) {
            int l, cc; locate(s, l, cc);
            fail(m.triple ? "unterminated triple-quoted f-string literal" : "unterminated f-string literal", l, cc);
        }
        char c = buf[p];
        if (c == m.quote && (!m.triple || (p + 2 < buf.size() && buf[p + 1] == m.quote && buf[p + 2] == m.quote))) {
            if (p > s) { pos = p; return make(Tok::FStringMiddle, s, p); }
            if (in_spec) {
                int l, cc; locate(p, l, cc);
                fail("f-string: expecting '}'", l, cc);
            }
            uint32_t e = p + (m.triple ? 3 : 1);
            pos = e;
            fmodes.pop_back();
            return make(Tok::FStringEnd, p, e);
        }
        if (c == '\n' && !m.triple) {
            int l, cc; locate(s, l, cc);
            if (in_spec) {
                if (p > s) { pos = p; return make(Tok::FStringMiddle, s, p); }
                locate(p, l, cc);
                fail("f-string: newlines are not allowed in format specifiers for single quoted f-strings", l, cc);
            }
            fail("unterminated f-string literal (detected at line " + std::to_string(l) + ")", l, cc);
        }
        if (c == '\\') {
            if (m.raw) {
                char n = p + 1 < buf.size() ? buf[p + 1] : 0;
                p += (n == m.quote || n == '\\') ? 2 : 1;
                continue;
            }
            char n = p + 1 < buf.size() ? buf[p + 1] : 0;
            if (n == 'N' && p + 2 < buf.size() && buf[p + 2] == '{') {
                p += 3;
                while (p < buf.size() && buf[p] != '}' && buf[p] != m.quote && buf[p] != '\n') ++p;
                if (p < buf.size() && buf[p] == '}') ++p;
                continue;
            }
            if (n == '{' || n == '}') { ++p; continue; }
            p += 2;
            continue;
        }
        if (c == '{') {
            if (!in_spec && p + 1 < buf.size() && buf[p + 1] == '{') { p += 2; continue; }
            if (p > s) { pos = p; return make(Tok::FStringMiddle, s, p); }
            brackets.push_back({'{', p});
            m.fields.push_back(Field{brackets.size(), false});
            pos = p + 1;
            return make(Tok::Op, p, p + 1, Op::LBrace);
        }
        if (c == '}') {
            if (!in_spec) {
                if (p + 1 < buf.size() && buf[p + 1] == '}') { p += 2; continue; }
                int l, cc; locate(p, l, cc);
                fail("f-string: single '}' is not allowed", l, cc);
            }
            if (p > s) { pos = p; return make(Tok::FStringMiddle, s, p); }
            brackets.pop_back();
            m.fields.pop_back();
            pos = p + 1;
            return make(Tok::Op, p, p + 1, Op::RBrace);
        }
        ++p;
    }
}

Token Lexer::scan_number(uint32_t s) {
    uint32_t p = s;
    auto digits = [&](auto pred) {
        // digit (['_'] digit)*
        bool any = false;
        while (p < buf.size()) {
            if (pred((unsigned char)buf[p])) { ++p; any = true; continue; }
            if (buf[p] == '_' && any && p + 1 < buf.size() && pred((unsigned char)buf[p + 1])) { ++p; continue; }
            break;
        }
        return any;
    };
    auto bad = [&](const char* what) {
        int l, cc; locate(p, l, cc);
        fail(what, l, cc);
    };
    if (buf[p] == '0' && p + 1 < buf.size() && ((buf[p + 1] | 0x20) == 'x' || (buf[p + 1] | 0x20) == 'o' || (buf[p + 1] | 0x20) == 'b')) {
        char k = (char)(buf[p + 1] | 0x20);
        p += 2;
        if (p < buf.size() && buf[p] == '_') ++p;
        bool ok = k == 'x' ? digits(is_hex) : k == 'o' ? digits([](unsigned char c) { return c >= '0' && c <= '7'; })
                                                     : digits([](unsigned char c) { return c == '0' || c == '1'; });
        if (!ok) bad(k == 'x' ? "invalid hexadecimal literal" : k == 'o' ? "invalid octal literal" : "invalid binary literal");
    } else {
        bool is_float = false;
        if (buf[p] != '.') digits(is_digit);
        if (p < buf.size() && buf[p] == '.') {
            ++p;
            is_float = true;
            digits(is_digit);
        }
        if (p < buf.size() && (buf[p] | 0x20) == 'e') {
            uint32_t q = p + 1;
            if (q < buf.size() && (buf[q] == '+' || buf[q] == '-')) ++q;
            if (q < buf.size() && is_digit((unsigned char)buf[q])) {
                p = q;
                digits(is_digit);
                is_float = true;
            }
        }
        if (p < buf.size() && (buf[p] | 0x20) == 'j') { ++p; is_float = true; }
        if (!is_float) {
            // Leading zeros in a nonzero decimal integer are not allowed.
            if (buf[s] == '0') {
                for (uint32_t i = s; i < p; ++i)
                    if (buf[i] != '0' && buf[i] != '_') {
                        int l, cc; locate(s, l, cc);
                        fail("leading zeros in decimal integer literals are not permitted; use an 0o prefix for octal integers", l, cc);
                    }
            }
        }
    }
    if (p < buf.size() && is_id_char((unsigned char)buf[p])) {
        // `1if x else y` style keywords right after a number are accepted by CPython (with a
        // warning); anything else is an invalid literal.
        static const char* kws[] = {"and", "else", "for", "if", "in", "is", "not", "or"};
        bool kw = false;
        for (const char* w : kws) {
            size_t n = strlen(w);
            if (buf.compare(p, n, w) == 0 && (p + n >= buf.size() || !is_id_char((unsigned char)buf[p + n]))) kw = true;
        }
        if (!kw) bad("invalid decimal literal");
    }
    pos = p;
    return make(Tok::Number, s, p);
}

Token Lexer::scan_op(uint32_t s) {
    char c = buf[s];
    char c1 = s + 1 < buf.size() ? buf[s + 1] : 0;
    char c2 = s + 2 < buf.size() ? buf[s + 2] : 0;
    Op op = Op::None;
    int n = 1;
    FMode* fm = fmodes.empty() ? nullptr : &fmodes.back();
    bool at_field = fm && !fm->fields.empty() && !fm->fields.back().in_spec && brackets.size() == fm->fields.back().depth;
    switch (c) {
    case '(': op = Op::LPar; break;
    case ')': op = Op::RPar; break;
    case '[': op = Op::LSqb; break;
    case ']': op = Op::RSqb; break;
    case '{': op = Op::LBrace; break;
    case '}': op = Op::RBrace; break;
    case ',': op = Op::Comma; break;
    case ';': op = Op::Semi; break;
    case '~': op = Op::Tilde; break;
    case ':':
        if (at_field) { op = Op::Colon; fm->fields.back().in_spec = true; break; }
        if (c1 == '=') { op = Op::ColonEqual; n = 2; } else op = Op::Colon;
        break;
    case '.':
        if (c1 == '.' && c2 == '.') { op = Op::Ellipsis; n = 3; } else op = Op::Dot;
        break;
    case '+': if (c1 == '=') { op = Op::PlusEqual; n = 2; } else op = Op::Plus; break;
    case '-':
        if (c1 == '=') { op = Op::MinEqual; n = 2; }
        else if (c1 == '>') { op = Op::RArrow; n = 2; }
        else op = Op::Minus;
        break;
    case '*':
        if (c1 == '*') { if (c2 == '=') { op = Op::DoubleStarEqual; n = 3; } else { op = Op::DoubleStar; n = 2; } }
        else if (c1 == '=') { op = Op::StarEqual; n = 2; }
        else op = Op::Star;
        break;
    case '/':
        if (c1 == '/') { if (c2 == '=') { op = Op::DoubleSlashEqual; n = 3; } else { op = Op::DoubleSlash; n = 2; } }
        else if (c1 == '=') { op = Op::SlashEqual; n = 2; }
        else op = Op::Slash;
        break;
    case '%': if (c1 == '=') { op = Op::PercentEqual; n = 2; } else op = Op::Percent; break;
    case '&': if (c1 == '=') { op = Op::AmperEqual; n = 2; } else op = Op::Amper; break;
    case '|': if (c1 == '=') { op = Op::VBarEqual; n = 2; } else op = Op::VBar; break;
    case '^': if (c1 == '=') { op = Op::CircumflexEqual; n = 2; } else op = Op::Circumflex; break;
    case '@': if (c1 == '=') { op = Op::AtEqual; n = 2; } else op = Op::At; break;
    case '=': if (c1 == '=') { op = Op::EqEqual; n = 2; } else op = Op::Equal; break;
    case '!': if (c1 == '=') { op = Op::NotEqual; n = 2; } else op = Op::Exclamation; break;
    case '<':
        if (c1 == '<') { if (c2 == '=') { op = Op::LeftShiftEqual; n = 3; } else { op = Op::LeftShift; n = 2; } }
        else if (c1 == '=') { op = Op::LessEqual; n = 2; }
        else op = Op::Less;
        break;
    case '>':
        if (c1 == '>') { if (c2 == '=') { op = Op::RightShiftEqual; n = 3; } else { op = Op::RightShift; n = 2; } }
        else if (c1 == '=') { op = Op::GreaterEqual; n = 2; }
        else op = Op::Greater;
        break;
    default: {
        int l, cc; locate(s, l, cc);
        unsigned char u = (unsigned char)c;
        char hex[16];
        snprintf(hex, sizeof hex, "%04X", u);
        fail(std::string("invalid character '") + c + "' (U+" + hex + ")", l, cc);
    }
    }
    if (op == Op::LPar || op == Op::LSqb || op == Op::LBrace) {
        brackets.push_back({c, s});
    } else if (op == Op::RPar || op == Op::RSqb || op == Op::RBrace) {
        if (brackets.empty()) {
            int l, cc; locate(s, l, cc);
            fail(std::string("unmatched '") + c + "'", l, cc);
        }
        char open = brackets.back().c;
        char want = open == '(' ? ')' : open == '[' ? ']' : '}';
        if (want != c) {
            int l, cc; locate(s, l, cc);
            int ol, oc; locate(brackets.back().off, ol, oc);
            if (ol != l)
                fail(std::string("closing parenthesis '") + c + "' does not match opening parenthesis '" + open + "' on line " + std::to_string(ol), l, cc);
            fail(std::string("closing parenthesis '") + c + "' does not match opening parenthesis '" + open + "'", l, cc);
        }
        brackets.pop_back();
        if (op == Op::RBrace && fm && !fm->fields.empty() && brackets.size() + 1 == fm->fields.back().depth) {
            fm->fields.pop_back();
        }
    }
    pos = s + n;
    return make(Tok::Op, s, s + n, op);
}

}  // namespace pyfront
