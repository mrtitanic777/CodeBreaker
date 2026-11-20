// cb/build/x86asm/preproc_util.cpp -- the preprocessor's text helpers (see preproc_util.hpp): quote.c and numstr.c.
#include "cb/build/x86asm/preproc_util.hpp"

namespace cb::build::x86asm::pp {

std::string quote(std::string_view str) {
    bool sq_ok = true, dq_ok = true;
    const std::size_t len = str.size();
    for (std::size_t i = 0; i < len; ++i) {
        const char c = str[i];
        switch (c) {
            case '\'': sq_ok = false; break;
            case '"': dq_ok = false; break;
            case '`':
            case '\\': break;
            default:
                if (c < ' ' || c > '~') sq_ok = dq_ok = false;
                break;
        }
    }
    std::string out;
    if (sq_ok || dq_ok) {
        const char q = sq_ok ? '\'' : '"';
        out.reserve(len + 2);
        out.push_back(q);
        out.append(str);
        out.push_back(q);
        return out;
    }
    out.push_back('`');
    for (std::size_t i = 0; i < len; ++i) {
        const char c = str[i];
        switch (c) {
            case '`':
            case '\\': out.push_back('\\'); out.push_back(c); break;
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
                    const char c1 = (i + 1 < len) ? str[i + 1] : 0;
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

namespace {

void emit_utf8(std::string& q, std::uint32_t v) {
    if (v <= 0x7f) { q.push_back(static_cast<char>(v)); return; }
    const std::uint32_t vb1 = v >> 6;
    if (vb1 <= 0x1f) {
        q.push_back(static_cast<char>(0xc0 + vb1));
        q.push_back(static_cast<char>(0x80 + (v & 63)));
        return;
    }
    const std::uint32_t vb2 = vb1 >> 6;
    if (vb2 <= 0x0f) {
        q.push_back(static_cast<char>(0xe0 + vb2));
        q.push_back(static_cast<char>(0x80 + (vb1 & 63)));
        q.push_back(static_cast<char>(0x80 + (v & 63)));
        return;
    }
    const std::uint32_t vb3 = vb2 >> 6;
    if (vb3 <= 0x07) {
        q.push_back(static_cast<char>(0xf0 + vb3));
        q.push_back(static_cast<char>(0x80 + (vb2 & 63)));
        q.push_back(static_cast<char>(0x80 + (vb1 & 63)));
        q.push_back(static_cast<char>(0x80 + (v & 63)));
        return;
    }
    const std::uint32_t vb4 = vb3 >> 6;
    if (vb4 <= 0x03) {
        q.push_back(static_cast<char>(0xf8 + vb4));
        q.push_back(static_cast<char>(0x80 + (vb3 & 63)));
        q.push_back(static_cast<char>(0x80 + (vb2 & 63)));
        q.push_back(static_cast<char>(0x80 + (vb1 & 63)));
        q.push_back(static_cast<char>(0x80 + (v & 63)));
        return;
    }
    const std::uint32_t vb5 = vb4 >> 6;
    q.push_back(static_cast<char>(0xfc + vb5));
    q.push_back(static_cast<char>(0x80 + (vb4 & 63)));
    q.push_back(static_cast<char>(0x80 + (vb3 & 63)));
    q.push_back(static_cast<char>(0x80 + (vb2 & 63)));
    q.push_back(static_cast<char>(0x80 + (vb1 & 63)));
    q.push_back(static_cast<char>(0x80 + (v & 63)));
}

constexpr std::uint32_t ctlbit(std::uint32_t v) { return v < 32 ? 1u << v : 0u; }

// The value of a digit character in any base up to 64, or 0xffffffff for a non-digit (numvalue_chk).
unsigned numvalue_chk(unsigned char c) {
    unsigned v = c - '0';
    if (v < 10) return v;
    v = (c | 0x20) - 'a';
    if (v < 26) return v + 10;
    return 0xffffffffu;
}

enum class UnqState { start, backslash, byte, ucs, ctrl, done };

}  // namespace

std::string unquote(std::string_view str, std::uint32_t badctl, char qstart, bool& ctl_error, std::size_t* end) {
    std::string q;
    std::uint32_t ctlmask = 0;
    auto at = [&](std::size_t i) -> unsigned char { return i < str.size() ? static_cast<unsigned char>(str[i]) : 0; };
    auto emit = [&](unsigned char c) {
        ctlmask |= ctlbit(c);
        if (!(badctl & ctlmask)) q.push_back(static_cast<char>(c));
    };
    auto emit_u = [&](std::uint32_t c) {
        ctlmask |= ctlbit(c);
        if (!(badctl & ctlmask)) emit_utf8(q, c);
    };
    const unsigned char bq = at(0);
    std::size_t p = 0;
    if (bq == static_cast<unsigned char>(qstart)) {
        UnqState state = UnqState::start;
        unsigned base = 0;
        std::uint64_t nval = 0;
        int ndig = 0;
        std::size_t escp = 0;
        ++p;
        while (state != UnqState::done) {
            unsigned char c = at(p++);
            switch (state) {
                case UnqState::start:
                    if (c == '\\') state = UnqState::backslash;
                    else if (c == '\0' || c == bq) state = UnqState::done;
                    else emit(c);
                    break;
                case UnqState::backslash: {
                    state = UnqState::start;
                    escp = p - 1;
                    nval = 0;
                    bool check_brace = false;
                    switch (c) {
                        case 'a': nval = 7; break;
                        case 'b': nval = 8; break;
                        case 'e': nval = 27; break;
                        case 'f': nval = 12; break;
                        case 'n': nval = 10; break;
                        case 'r': nval = 13; break;
                        case 't': nval = 9; break;
                        case '^': state = UnqState::ctrl; break;
                        case 'u': state = UnqState::ucs; base = 16; ndig = 4; check_brace = true; break;
                        case 'U': state = UnqState::ucs; base = 16; ndig = 8; check_brace = true; break;
                        case 'v': nval = 11; break;
                        case 'x':
                        case 'X': state = UnqState::byte; base = 16; ndig = 2; check_brace = true; break;
                        case 'd': state = UnqState::byte; base = 10; ndig = 3; check_brace = true; break;
                        case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7':
                            p = escp--;   // an octal escape: the digit is part of the number
                            [[fallthrough]];
                        case 'o': state = UnqState::byte; ndig = 3; base = 8; check_brace = true; break;
                        case '\0': nval = '\\'; --p; break;
                        default: nval = c; break;
                    }
                    if (check_brace && at(p) == '{') {
                        ++p;
                        ndig = -1;
                    }
                    if (state == UnqState::start) emit(static_cast<unsigned char>(nval));
                    break;
                }
                case UnqState::byte:
                case UnqState::ucs: {
                    const unsigned v = numvalue_chk(c);
                    bool rewind = false;
                    if (v < base) {
                        nval = nval * base + v;
                        if (--ndig) break;   // more digits
                    } else if (ndig < 0) {
                        if (c != '}') rewind = true;
                    } else {
                        --p;
                        if (p == escp + 1) rewind = true;
                    }
                    if (rewind) {
                        p = escp;
                        state = UnqState::start;
                        break;
                    }
                    if (state == UnqState::ucs) emit_u(static_cast<std::uint32_t>(nval));
                    else emit(static_cast<unsigned char>(nval));
                    state = UnqState::start;
                    break;
                }
                case UnqState::ctrl:
                    if (!c) {
                        p = escp;
                        state = UnqState::start;
                    } else {
                        if (c == '?') c = 0177;
                        else c &= 0x9f;
                        emit(c);
                        state = UnqState::start;
                    }
                    break;
                default: break;
            }
        }
    } else if (bq == '\'' || bq == '"') {
        ++p;
        unsigned char c;
        while ((c = at(p++)) && c != bq) emit(c);
    } else {
        unsigned char c;
        while ((c = at(p++))) emit(c);
    }
    ctl_error = (ctlmask & badctl) != 0;
    if (end) *end = p ? p - 1 : 0;
    return q;
}

std::size_t skip_string(const char* str) {
    const char bq = str[0];
    const char* p = str + 1;
    char c;
    switch (bq) {
        case '\'':
        case '"':
            while ((c = *p++) && c != bq) {}
            break;
        case '`': {
            UnqState state = UnqState::start;
            while (state != UnqState::done) {
                c = *p++;
                if (state == UnqState::start) {
                    if (c == '\\') state = UnqState::backslash;
                    else if (c == '`' || c == '\0') state = UnqState::done;
                } else {
                    if (c == '^') c = *p++;
                    state = c ? UnqState::start : UnqState::done;
                }
            }
            break;
        }
        default: return std::string_view::npos;
    }
    return static_cast<std::size_t>(p - str) - 1;
}

const char* digit_chars(bool ucase) {
    return ucase ? "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz@_"
                 : "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ@_";
}

std::string numstr(std::uint64_t n, int digits, unsigned base, bool ucase) {
    if (base < 2 || base > NUMSTR_MAXBASE) return {};
    const char* dchars = digit_chars(ucase);
    const bool moredigits = digits <= 0;
    if (moredigits) digits = -digits;
    std::string r;
    while (digits-- > 0 || (moredigits && n)) {
        r.insert(r.begin(), dchars[n % base]);
        n /= base;
    }
    return r;
}

unsigned radix_letter(char c) {
    switch (c) {
        case 'b': case 'B': case 'y': case 'Y': return 2;
        case 'o': case 'O': case 'q': case 'Q': return 8;
        case 'h': case 'H': case 'x': case 'X': return 16;
        case 'd': case 'D': case 't': case 'T': return 10;
        default: return 0;
    }
}

}  // namespace cb::build::x86asm::pp
