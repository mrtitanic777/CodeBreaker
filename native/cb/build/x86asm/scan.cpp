// cb/build/x86asm/scan.cpp -- the standard scanner and readnum; see scan.hpp. A port of asm/stdscan.c and nasmlib/readnum.c,
// with the --masm tokenization of 64-bit register names and dr8+/cr9+ as labels.
#include "cb/build/x86asm/scan.hpp"

#include <algorithm>
#include <string>

#include "cb/build/x86asm/assembly.hpp"
#include "cb/build/x86asm/nctype.hpp"
#include "cb/build/x86asm/quote.hpp"
#include "cb/build/x86asm/tokhash.hpp"

namespace cb::build::x86asm {

// ---- readnum ----

namespace {
// The characters a number can be made of (scan_isnumchar).
constexpr bool scan_isnumchar(char c) { return nasm_isalnum(c) || c == '$' || c == '_'; }
}  // namespace

std::int64_t readnum(Assembly& as, std::string_view str, bool& error) {
    std::size_t r = 0;
    int sign = 1;
    bool warn = false;
    error = true;

    const auto at = [&](std::size_t i) -> char { return i < str.size() ? str[i] : '\0'; };

    while (nasm_isspace(at(r))) r++;   // find the start of the number

    // If the number came from make_tok_num (as a result of an %assign), it might have a '-' built into it (rather than in
    // a preceding token).
    if (at(r) == '-') {
        r++;
        sign = -1;
    }

    std::size_t q = r;
    while (scan_isnumchar(at(q))) q++;   // find the end of the number

    const std::size_t len = q - r;
    if (!len) return 0;   // not numeric

    // Radix formats: 0<radix-letter><string>, $<string> (hexadecimal), <string><radix-letter>.
    unsigned pradix = 0, sradix = 0;
    std::size_t plen = 0, slen = 0;

    if (len > 2 && at(r) == '0' && (pradix = nasm_radix_letter(at(r + 1))) != 0) {
        plen = 2;
    } else if (len > 1 && at(r) == '$' && as.globl.dollarhex) {
        // A warning here would probably duplicate warnings.
        pradix = 16;
        plen = 1;
    }

    if (len > 1 && (sradix = nasm_radix_letter(at(q - 1))) != 0) slen = 1;

    unsigned radix;
    if (pradix > sradix) {
        radix = pradix;
        r += plen;
    } else if (sradix > pradix) {
        radix = sradix;
        q -= slen;
    } else {
        // Either decimal, or invalid -- if invalid, we trip up further down.
        radix = 10;
    }

    // `checklimit' must be 2**64 / radix. That cannot be done in 64-bit arithmetic, but all the radices are even, so divide
    // 2**63 by radix/2 instead.
    const std::uint64_t checklimit = UINT64_C(0x8000000000000000) / (radix >> 1);

    // The highest allowable value for the last digit of a 64-bit constant: in radix 10 it is 6, otherwise 0.
    const unsigned last = (radix == 10 ? 6 : 0);

    std::uint64_t result = 0;
    while (at(r) && r < q) {
        if (at(r) != '_') {
            unsigned digit = 0;
            if (at(r) < '0' || (at(r) > '9' && at(r) < 'A') || (digit = numvalue(static_cast<unsigned char>(at(r)))) >= radix) return 0;
            if (result > checklimit || (result == checklimit && digit >= last)) warn = true;
            result = radix * result + digit;
        }
        r++;
    }

    if (warn) as.warn(WARN_NUMBER_OVERFLOW, "numeric constant {} does not fit in 64 bits", str);

    error = false;
    return static_cast<std::int64_t>(result * static_cast<std::uint64_t>(static_cast<std::int64_t>(sign)));
}

// ---- the scanner ----

void Scanner::reset(std::string_view buffer) {
    buffer_ = buffer;
    state_.pushback.clear();
    state_.pos = 0;
}

std::size_t Scanner::skip_spaces(std::size_t i) const {
    while (i < buffer_.size() && at(i) && nasm_isspace(at(i))) i++;
    return i;
}

int Scanner::scan(TokenVal& tv) {
    if (!state_.pushback.empty()) {
        tv = state_.pushback.back();
        state_.pushback.pop_back();
        return token_value(tv.type);
    }

    tv = TokenVal{};

    state_.pos = skip_spaces(state_.pos);
    const std::size_t start = state_.pos;
    tv.start = buffer_.substr(std::min(start, buffer_.size()), 0);

    if (!at(state_.pos)) {
        tv.type = TokenType::EOS;
        return token_value(tv.type);
    }

    const int i = token(tv);
    tv.start = buffer_.substr(start, state_.pos - start);
    return i;
}

// Skip identifier characters and return the identifier (stdscan_symbol); only IDLEN_MAX-1 characters of it are kept.
int Scanner::symbol(TokenVal& tv) {
    std::size_t p = state_.pos;
    const std::size_t r = p;
    p++;   // the leading character is already verified

    while (nasm_isidchar(at(p))) p++;

    state_.pos = p;
    std::size_t len = p - r;
    if (len >= static_cast<std::size_t>(IDLEN_MAX)) len = IDLEN_MAX - 1;

    tv.text = std::string(buffer_.substr(r, len));
    tv.type = TokenType::ID;
    return token_value(tv.type);
}

// Parse a set of braces. A set of braces can contain either a keyword (which, unlike normal keywords, can contain -) or a
// sequence like {dfv=foo,bar,baz}, which makes a {dfv=} token whose integer is the logical OR of the integer and inttwo
// values of {dfv=foo}{dfv=bar}{dfv=baz} (all of those except {dfv=} carry TFLAG_ORBIT).
int Scanner::parse_braces(TokenVal& tv) {
    std::size_t prefix_len = 0;
    const std::size_t startp = state_.pos;   // the beginning, including {

    state_.pos = skip_spaces(startp + 1);
    const std::size_t pfx = state_.pos;
    std::size_t r = state_.pos;

    // Read the token to advance the buffer pointer: {rn-sae}, {rd-sae}, {ru-sae}, {rz-sae} contain '-'.
    while (nasm_isbrcchar(at(state_.pos))) state_.pos++;
    std::size_t e = state_.pos;

    // Followed by an equal sign?
    state_.pos = skip_spaces(state_.pos);
    if (r != e && at(state_.pos) == '=') {
        prefix_len = e - pfx;
        r = e = ++state_.pos;   // note that the first suffix is blank
    }

    // Find the terminating brace, if it exists.
    const std::size_t brace = buffer_.find('}', std::min(state_.pos, buffer_.size()));
    if (brace == std::string_view::npos) {
        as_.nonfatal("unterminated braces at end of line");
        tv.type = TokenType::INVALID;
        return token_value(tv.type);
    }
    const std::size_t endp = brace + 1;
    const std::size_t brace_len = endp - startp;

    std::string prefix;   // "dfv=" for a compound token
    if (prefix_len) {
        prefix = std::string(buffer_.substr(pfx, prefix_len)) + "=";
    }
    std::int64_t t_integer = 0, t_inttwo = 0;
    bool first = true;

    for (;;) {
        const std::string key = prefix + std::string(buffer_.substr(r, e - r));

        token_hash(key, tv);

        if (!(tv.flag & TFLAG_BRC_ANY)) {
            // An invalid token is put inside braces.
            as_.nonfatal("`{{{}}}' is not a valid brace token", key);
            tv.type = TokenType::INVALID;
            break;
        }

        if (tv.type == TokenType::REG && is_reg_class(opf::OPMASKREG, static_cast<int>(tv.integer))) {
            // Within braces, an opmask register is now used as a mask.
            tv.type = TokenType::OPMASK;
        }

        if (tv.flag & TFLAG_ORBIT) {
            t_integer |= tv.integer;
            t_inttwo |= tv.inttwo;
        } else {
            t_integer = tv.integer;
            t_inttwo = tv.inttwo;
        }

        state_.pos = skip_spaces(state_.pos);
        const char nextchar = at(state_.pos);

        if (nextchar == '}') break;

        if (!prefix_len || !(nextchar == ',' || (first && nasm_isbrcchar(nextchar)))) {
            as_.nonfatal("invalid character `{}' in brace sequence", nextchar);
            tv.type = TokenType::INVALID;
            break;
        }

        if (nextchar == ',') state_.pos = skip_spaces(state_.pos + 1);

        r = state_.pos;
        while (nasm_isbrcchar(at(state_.pos))) state_.pos++;
        e = state_.pos;

        first = false;
    }

    tv.text = std::string(buffer_.substr(startp, brace_len));
    state_.pos = endp;

    tv.integer = t_integer;
    tv.inttwo = t_inttwo;
    return token_value(tv.type);
}

int Scanner::token(TokenVal& tv) {
    std::size_t& pos = state_.pos;
    const auto at1 = [&](std::size_t k) { return at(pos + k); };

    // We have a token: an id, a number, an operator or a char.
    if (nasm_isidstart(at(pos))) {
        symbol(tv);

        if (tv.text.size() <= MAX_KEYWORD) {
            // Check to see if it is a keyword of some kind.
            const int token_type = token_hash(tv.text, tv);

            if (tv.flag & TFLAG_WARN) as_.warn(WARN_PTR, "`{}' is not a NASM keyword", tv.text);

            // MASM (--masm): a 64-bit register name (RAX..R15) cannot be a register in the 16/32-bit source this mode targets,
            // so real MASM code uses such names as ordinary labels (`rax:', `jz rax'). NASM reserves them, so return the name
            // as an identifier instead. dr8..dr31 / cr9..cr31 are NASM register names but not real 16/32-bit registers (only
            // dr0-7, cr0-4/cr8 exist), so real MASM code uses e.g. `dr20' as an ordinary label.
            if (as_.masm_mode && token_type == token_value(TokenType::REG)) {
                const int reg = static_cast<int>(tv.integer);
                if (is_reg_class(opf::REG64, reg) ||
                    (is_reg_class(opf::REG_DREG, reg) && kRegVals[static_cast<std::size_t>(reg)] >= 8) ||
                    (is_reg_class(opf::REG_CREG, reg) && kRegVals[static_cast<std::size_t>(reg)] > 8)) {
                    tv.type = TokenType::ID;
                    return token_value(TokenType::ID);
                }
            }

            if (!(tv.flag & TFLAG_BRC)) return token_type;   // most of the tokens fall into this case
        }
        tv.type = TokenType::ID;
        return token_value(TokenType::ID);
    }

    if (at(pos) == '$' && (!as_.globl.dollarhex || !nasm_isdigit(at1(1)))) {
        // A $ sign with no following hex number: a Here token ($, the current assembly location), a Base token ($$, the base
        // of the current segment), or an identifier beginning with $ (escaped by a previous $).
        pos++;
        if (at(pos) == '$' && !nasm_isidchar(at1(1))) {
            pos++;
            tv.type = TokenType::BASE;
        } else if (nasm_isidchar(at(pos))) {
            return symbol(tv);   // a $-escaped symbol that does NOT start with $
        } else {
            tv.type = TokenType::HERE;
        }
        return token_value(tv.type);
    }

    if (nasm_isnumstart(at(pos))) {   // now we have a number
        bool is_hex = false, is_float = false, has_e = false;
        const std::size_t r = pos;

        if (at(pos) == '$') {
            pos++;
            is_hex = true;
            as_.warn_dollar_hex();
        }

        for (;;) {
            const char c = at(pos);
            pos++;

            if (!is_hex && (c == 'e' || c == 'E')) {
                has_e = true;
                if (at(pos) == '+' || at(pos) == '-') {
                    // e can only be followed by +/- if it is either a prefixed hex number or a floating-point number.
                    is_float = true;
                    pos++;
                }
            } else if (c == 'H' || c == 'h' || c == 'X' || c == 'x') {
                is_hex = true;
            } else if (c == 'P' || c == 'p') {
                is_float = true;
                if (at(pos) == '+' || at(pos) == '-') pos++;
            } else if (nasm_isnumchar(c)) {
                // just advance
            } else if (c == '.') {
                is_float = true;
            } else {
                break;
            }
        }
        pos--;   // point to the first character beyond the number

        if (has_e && !is_hex) is_float = true;   // 1e13 is floating-point, but 1e13h is not

        const std::string_view text = buffer_.substr(r, pos - r);
        if (is_float) {
            tv.text = std::string(text);
            tv.type = TokenType::FLOAT;
            return token_value(tv.type);
        }
        bool rn_error = false;
        tv.integer = readnum(as_, text, rn_error);
        if (rn_error) {
            tv.type = TokenType::ERRNUM;   // some malformation occurred
            return token_value(tv.type);
        }
        tv.type = TokenType::NUM;
        return token_value(tv.type);
    }

    if (at(pos) == '\'' || at(pos) == '"' || at(pos) == '`') {
        // A quoted string.
        const char start_quote = at(pos);
        Unquoted u = nasm_unquote(as_, buffer_.substr(pos));
        tv.inttwo = static_cast<std::int64_t>(u.text.size());
        tv.text = std::move(u.text);
        pos += u.end;
        if (at(pos) != start_quote) {
            tv.type = TokenType::ERRSTR;
            return token_value(tv.type);
        }
        pos++;   // skip the final quote
        tv.type = TokenType::STR;
        return token_value(tv.type);
    }

    if (at(pos) == '{') return parse_braces(tv);   // a decorator

    if (at(pos) == ';') {
        tv.type = TokenType::EOS;   // a comment has happened - stay
        return token_value(tv.type);
    }

    // The multi-character operators.
    struct Op {
        const char* text;
        TokenType type;
    };
    static constexpr Op kOps[] = {
        {">>>", TokenType::SAR}, {">>", TokenType::SHR}, {"<<<", TokenType::SHL}, {"<<", TokenType::SHL},
        {"//", TokenType::SDIV}, {"%%", TokenType::SMOD}, {"==", TokenType::EQ},   {"<>", TokenType::NE},
        {"!=", TokenType::NE},   {"<=>", TokenType::LEG}, {"<=", TokenType::LE},   {">=", TokenType::GE},
        {"&&", TokenType::DBL_AND}, {"^^", TokenType::DBL_XOR}, {"||", TokenType::DBL_OR},
    };
    for (const Op& op : kOps) {
        const std::string_view text = op.text;
        if (buffer_.substr(std::min(pos, buffer_.size())).starts_with(text)) {
            pos += text.size();
            tv.type = op.type;
            return token_value(tv.type);
        }
    }

    // Just an ordinary character.
    tv.type = static_cast<TokenType>(static_cast<std::uint8_t>(at(pos)));
    pos++;
    return token_value(tv.type);
}

}  // namespace cb::build::x86asm
