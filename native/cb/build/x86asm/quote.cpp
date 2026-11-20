// cb/build/x86asm/quote.cpp -- NASM's string quoting rules; see quote.hpp. A port of asm/quote.c.
#include "cb/build/x86asm/quote.hpp"

#include "cb/build/x86asm/assembly.hpp"
#include "cb/build/x86asm/nctype.hpp"

namespace cb::build::x86asm {

namespace {

// Append the UTF-8 form of a value, as far as 31 bits (a value of 2^31 or more gets a sixth byte as well: NASM tries to
// do something vaguely sensible with it).
void quote_emit_utf8(std::string& q, std::uint32_t v) {
    if (v <= 0x7f) {
        q.push_back(static_cast<char>(v));
        return;
    }
    const std::uint32_t vb1 = v >> 6;
    const std::uint32_t vb2 = vb1 >> 6;
    const std::uint32_t vb3 = vb2 >> 6;
    const std::uint32_t vb4 = vb3 >> 6;
    const std::uint32_t vb5 = vb4 >> 6;
    int extension = 0;   // how many 10xxxxxx bytes follow the lead byte
    if (vb1 <= 0x1f) {
        q.push_back(static_cast<char>(0xc0 + vb1));
        extension = 1;
    } else if (vb2 <= 0x0f) {
        q.push_back(static_cast<char>(0xe0 + vb2));
        extension = 2;
    } else if (vb3 <= 0x07) {
        q.push_back(static_cast<char>(0xf0 + vb3));
        extension = 3;
    } else if (vb4 <= 0x03) {
        q.push_back(static_cast<char>(0xf8 + vb4));
        extension = 4;
    } else {
        q.push_back(static_cast<char>(0xfc + vb5));
        extension = 5;
    }
    // The extension bytes, most significant first: the low six bits of v, vb1, vb2, ... in turn.
    const std::uint32_t parts[5] = {v, vb1, vb2, vb3, vb4};
    for (int i = extension - 1; i >= 0; --i) q.push_back(static_cast<char>(0x80 + (parts[i] & 63)));
}

// The states of the unquoting machine (enum unq_state).
enum class QuoteState { Start, Backslash, Byte, Ucs, Ctrl, Done };

}  // namespace

std::string nasm_quote(std::string_view str) {
    bool sq_ok = true, dq_ok = true;
    for (const char c : str) {
        switch (c) {
        case '\'':
            sq_ok = false;
            break;
        case '"':
            dq_ok = false;
            break;
        case '`':
        case '\\':
            break;
        default:
            if (c < ' ' || c > '~') sq_ok = dq_ok = false;
            break;
        }
    }

    if (sq_ok || dq_ok) {
        // Use '...' or "...".
        const char quote = sq_ok ? '\'' : '"';
        std::string out;
        out.reserve(str.size() + 2);
        out.push_back(quote);
        out.append(str);
        out.push_back(quote);
        return out;
    }

    // Need to use the `...` syntax.
    std::string out;
    out.push_back('`');
    for (std::size_t i = 0; i < str.size(); ++i) {
        const char c = str[i];
        switch (c) {
        case '`':
        case '\\':
            out.push_back('\\');
            out.push_back(c);
            break;
        case 7: out += "\\a"; break;
        case 8: out += "\\b"; break;
        case 9: out += "\\t"; break;
        case 10: out += "\\n"; break;
        case 11: out += "\\v"; break;
        case 12: out += "\\f"; break;
        case 13: out += "\\r"; break;
        case 27: out += "\\e"; break;
        default:
            if (c < ' ' || c > '~') {
                // An octal escape; the full three digits if an octal digit follows, so it is not read as part of it.
                const char c1 = (i + 1 < str.size()) ? str[i + 1] : 0;
                const unsigned char uc = (c1 >= '0' && c1 <= '7') ? 0377 : static_cast<unsigned char>(c);
                out.push_back('\\');
                if (uc > 077) out.push_back(static_cast<char>((static_cast<unsigned char>(c) >> 6) + '0'));
                if (uc > 07) out.push_back(static_cast<char>(((static_cast<unsigned char>(c) >> 3) & 7) + '0'));
                out.push_back(static_cast<char>((static_cast<unsigned char>(c) & 7) + '0'));
            } else {
                out.push_back(c);
            }
            break;
        }
    }
    out.push_back('`');
    return out;
}

Unquoted nasm_unquote_anystr(Assembly& as, std::string_view str, std::uint32_t badctl, char qstart) {
    // The input as a C string: the byte at `i`, or NUL past the end.
    const auto at = [&](std::size_t i) -> unsigned char { return i < str.size() ? static_cast<unsigned char>(str[i]) : 0; };

    Unquoted result;
    std::string& q = result.text;
    std::uint32_t ctlmask = 0;   // the control characters seen so far

    // Append a byte unless a forbidden control character has been seen (CTL_ERR and EMIT).
    const auto ctl_err = [&](std::uint32_t c) {
        if (c < 32) ctlmask |= std::uint32_t{1} << c;
        return (badctl & ctlmask) != 0;
    };
    const auto emit = [&](std::uint64_t value) {
        const auto ec = static_cast<unsigned char>(value);
        if (!ctl_err(ec)) q.push_back(static_cast<char>(ec));
    };
    const auto emit_utf = [&](std::uint64_t value) {
        const auto ec = static_cast<std::uint32_t>(value);
        if (!ctl_err(ec)) quote_emit_utf8(q, ec);
    };

    const unsigned char bq = at(0);
    std::size_t p = 0;

    if (bq == static_cast<unsigned char>(qstart)) {
        // `...` string, or "..." with C unquoting.
        QuoteState state = QuoteState::Start;
        unsigned base = 0;            // base of the numeric escape sequence
        std::uint64_t nval = 0;       // accumulated value of the numeric sequence
        int ndig = 0;                 // max digits of the numeric sequence; < 0 means a braced sequence
        std::size_t escp = 0;         // the offset just after the backslash

        p = 1;   // skip the initial quote
        while (state != QuoteState::Done) {
            unsigned char c = at(p++);
            switch (state) {
            case QuoteState::Start:
                if (c == '\\') {
                    state = QuoteState::Backslash;
                } else if (c == 0 || c == bq) {
                    state = QuoteState::Done;
                } else {
                    emit(c);
                }
                break;

            case QuoteState::Backslash: {
                state = QuoteState::Start;
                escp = p - 1;
                nval = 0;
                bool check_brace = false;   // may this be the start of a braced sequence?
                switch (c) {
                case 'a': nval = 7; break;
                case 'b': nval = 8; break;
                case 'e': nval = 27; break;
                case 'f': nval = 12; break;
                case 'n': nval = 10; break;
                case 'r': nval = 13; break;
                case 't': nval = 9; break;
                case '^': state = QuoteState::Ctrl; break;
                case 'u':
                    state = QuoteState::Ucs;
                    base = 16;
                    ndig = 4;
                    check_brace = true;
                    break;
                case 'U':
                    state = QuoteState::Ucs;
                    base = 16;
                    ndig = 8;
                    check_brace = true;
                    break;
                case 'v': nval = 11; break;
                case 'x':
                case 'X':
                    state = QuoteState::Byte;
                    base = 16;
                    ndig = 2;
                    check_brace = true;
                    break;
                case 'd':   // NASM extension: \d = decimal
                    state = QuoteState::Byte;
                    base = 10;
                    ndig = 3;
                    check_brace = true;
                    break;
                case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7':
                    // Back up both p and escp, as if there had been an "o".
                    p = escp--;
                    state = QuoteState::Byte;
                    ndig = 3;
                    base = 8;
                    check_brace = true;
                    break;
                case 'o':
                    state = QuoteState::Byte;
                    ndig = 3;
                    base = 8;
                    check_brace = true;
                    break;
                case 0:
                    nval = '\\';
                    --p;   // reprocess; terminates the string
                    break;
                default:
                    nval = c;
                    break;
                }
                if (check_brace && at(p) == '{') {
                    ++p;
                    ndig = -1;
                }
                if (state == QuoteState::Start) emit(nval);
                break;
            }

            case QuoteState::Byte:
            case QuoteState::Ucs: {
                bool rewind = false;
                bool done = true;   // the number is complete: emit it
                const unsigned v = numvalue_chk(c);
                if (v < base) {
                    nval = (nval * base) + v;
                    if (--ndig != 0) done = false;   // continue processing the number, no output
                } else if (ndig < 0) {
                    // The end of a braced sequence.
                    if (c != '}') rewind = true;
                } else {
                    --p;   // reprocess the terminating character
                    if (p == escp + 1) rewind = true;   // no digits at all
                }
                if (rewind) {
                    p = escp;   // start over at the character following the backslash
                    state = QuoteState::Start;
                } else if (done) {
                    if (state == QuoteState::Ucs) emit_utf(nval);
                    else emit(nval);
                    state = QuoteState::Start;
                }
                break;
            }

            case QuoteState::Ctrl:
                if (c == 0) {
                    p = escp;   // rewind
                    state = QuoteState::Start;
                    break;
                }
                if (c == '?') c = 0177;   // DEL
                else c &= 0x9f;           // anything else
                emit(c);
                state = QuoteState::Start;
                break;

            case QuoteState::Done:
                break;
            }
        }
    } else if (bq == '\'' || bq == '"') {
        // '...' or "..." string, NASM legacy style: no escapes of any kind.
        p = 1;
        for (;;) {
            const unsigned char c = at(p++);
            if (c == 0 || c == bq) break;
            emit(c);
        }
    } else {
        // Not a quoted string, just return the input.
        for (;;) {
            const unsigned char c = at(p++);
            if (c == 0) break;
            emit(c);
        }
    }

    if ((ctlmask & badctl) != 0) as.nonfatal("control character in string not allowed here");

    result.end = p - 1;   // the terminating character
    return result;
}

Unquoted nasm_unquote(Assembly& as, std::string_view str) { return nasm_unquote_anystr(as, str, 0, STR_NASM); }

Unquoted nasm_unquote_cstr(Assembly& as, std::string_view str) { return nasm_unquote_anystr(as, str, BADCTL, STR_NASM); }

std::optional<std::size_t> nasm_skip_string(std::string_view str) {
    const auto at = [&](std::size_t i) -> unsigned char { return i < str.size() ? static_cast<unsigned char>(str[i]) : 0; };
    const unsigned char bq = at(0);
    std::size_t p = 1;
    switch (bq) {
    case '\'':
    case '"':
        for (;;) {
            const unsigned char c = at(p++);
            if (c == 0 || c == bq) break;
        }
        return p - 1;

    case '`': {
        // For finding the end only, every successor of a backslash except ^ is the same as the start state: a backslash or
        // a backquote returns to it, and a multi-character escape ends at any non-alphanumeric. This keeps \` and \^` from
        // ending the string, and \^\ from starting an escape (\^\` is an end of string).
        QuoteState state = QuoteState::Start;
        while (state != QuoteState::Done) {
            unsigned char c = at(p++);
            if (state == QuoteState::Start) {
                if (c == '`' || c == 0) state = QuoteState::Done;
                else if (c == '\\') state = QuoteState::Backslash;
            } else {
                if (c == '^') c = at(p++);
                state = c ? QuoteState::Start : QuoteState::Done;
            }
        }
        return p - 1;
    }

    default:
        return std::nullopt;   // not a string at all
    }
}

}  // namespace cb::build::x86asm
