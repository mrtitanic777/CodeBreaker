// cb/build/x86asm/preproc_tok.cpp -- the preprocessor's tokens: the pool they live in, the tokenizer (a line of text into a
// token list), detoken (back to text), the list helpers and the scan callback the evaluator pulls tokens through.
// (preproc.c: Token, tokenize, new_Token, detoken, ppscan ...)
#include "cb/build/x86asm/preproc.hpp"

#include <cstdio>

namespace cb::build::x86asm {

using namespace ppt;
using namespace pp;

// ---- the pool ----

Tok* Preproc::alloc_token() {
    Tok* t = free_tokens_;
    if (!t) {
        token_pool_.emplace_back();
        return &token_pool_.back();
    }
    free_tokens_ = t->next;
    t->next = nullptr;
    return t;
}

Tok* Preproc::free_token(Tok* t) {
    Tok* next = t->next;
    t->text.clear();
    t->type = 0;
    t->next = free_tokens_;
    free_tokens_ = t;
    return next;
}

Tok* Preproc::do_delete_token(Tok*& t) {
    if (t) return t = free_token(t);
    return nullptr;
}

void Preproc::free_tlist(Tok* list) {
    while (list) list = free_token(list);
}

void Preproc::delete_tlist(Tok*& list) {
    free_tlist(list);
    list = nullptr;
}

Tok* Preproc::new_token(Tok* next, int type, std::string_view text, std::size_t) {
    Tok* t = alloc_token();
    t->next = next;
    t->type = type;
    if (type == WHITESPACE) t->text = " ";
    else t->text.assign(text);
    return t;
}

Tok* Preproc::new_token_str(Tok* next, int type, std::string text) {
    Tok* t = alloc_token();
    t->next = next;
    t->type = type;
    t->text = std::move(text);
    return t;
}

Tok* Preproc::dup_token(Tok* next, const Tok* src) {
    if (!src) return nullptr;
    Tok* t = alloc_token();
    t->type = src->type;
    t->text = src->text;
    t->next = next;
    return t;
}

Tok* Preproc::new_white(Tok* next) {
    Tok* t = alloc_token();
    t->next = next;
    t->type = WHITESPACE;
    t->text = " ";
    return t;
}

// Move everything but the link from `src` into `dst` (the old text of `dst` is dropped, `src` is left empty).
Tok* Preproc::steal_token(Tok* dst, Tok* src) {
    dst->type = src->type;
    dst->text = std::move(src->text);
    src->text.clear();
    src->type = 0;
    return dst;
}

Tok* Preproc::dup_tlist(const Tok* list, Tok*** tailp) {
    Tok* newlist = nullptr;
    Tok** tailpp = &newlist;
    for (const Tok* t = list; t; t = t->next) {
        Tok* nt = dup_token(nullptr, t);
        *tailpp = nt;
        tailpp = &nt->next;
    }
    if (tailp) {
        **tailp = newlist;
        *tailp = tailpp;
    }
    return newlist;
}

Tok* Preproc::dup_tlistn(const Tok* list, std::size_t cnt, Tok*** tailp) {
    Tok* newlist = nullptr;
    Tok** tailpp = &newlist;
    for (const Tok* t = list; t; t = t->next) {
        if (!cnt--) break;
        Tok* nt = dup_token(nullptr, t);
        *tailpp = nt;
        tailpp = &nt->next;
    }
    if (tailp) {
        **tailp = newlist;
        if (newlist) *tailp = tailpp;
    }
    return newlist;
}

// Copy a list in reverse order onto the front of `tail`.
Tok* Preproc::dup_tlist_reverse(const Tok* list, Tok* tail) {
    for (const Tok* t = list; t; t = t->next) tail = dup_token(tail, t);
    return tail;
}

// Hang `tlist` at *tailp, returning the new tail pointer (the `next` of the last token).
Tok** Preproc::steal_tlist(Tok* tlist, Tok** tailp) {
    *tailp = tlist;
    if (!tlist) return tailp;
    while (tlist->next) tlist = tlist->next;
    return &tlist->next;
}

Tok* Preproc::cut_tlist(Tok* t) {
    Tok* nt = t->next;
    t->next = nullptr;
    return nt;
}

Tok** Preproc::tlist_endptr(Tok** tptr) {
    while (*tptr) tptr = &(*tptr)->next;
    return tptr;
}

Tok* Preproc::reverse_tokens(Tok* t) {
    Tok* prev = nullptr;
    while (t) {
        Tok* next = t->next;
        t->next = prev;
        prev = t;
        t = next;
    }
    return prev;
}

// ---- making tokens ----

Tok* Preproc::make_tok_char(Tok* next, char op) {
    Tok* t = new_token(next, static_cast<unsigned char>(op), std::string_view());
    t->text.assign(1, op);
    return t;
}

Tok* Preproc::make_tok_num(Tok* next, std::int64_t val) {
    const bool minus = val < 0;
    const std::uint64_t uval = minus ? 0 - static_cast<std::uint64_t>(val) : static_cast<std::uint64_t>(val);
    next = new_token(next, NUM, std::to_string(uval));
    if (minus) next = make_tok_char(next, '-');
    return next;
}

Tok* Preproc::make_tok_num_radix(Tok* next, std::int64_t val, char radix, bool uns) {
    const bool minus = val < 0 && !uns;
    const std::uint64_t uval = minus ? 0 - static_cast<std::uint64_t>(val) : static_cast<std::uint64_t>(val);
    std::string s;
    unsigned base = 10;
    bool upper = false;
    if (radix) {
        s += '0';
        s += radix;
        base = radix_letter(radix);
        upper = !(radix & 0x20);
    }
    s += numstr(uval, -1, base, upper);
    next = new_token(next, NUM, s);
    if (minus) next = make_tok_char(next, '-');
    return next;
}

std::int64_t Preproc::get_tok_num(const Tok* t, bool* err) {
    bool minus = false;
    if (tok_is(t, '-')) {
        minus = true;
        t = t->next;
    }
    if (!tok_is(t, NUM)) {
        if (err) *err = true;
        return 0;
    }
    bool e = false;
    const std::int64_t v = as_.hooks.readnum ? as_.hooks.readnum(t->text, e) : 0;
    if (err) *err = e;
    return minus ? -v : v;
}

Tok* Preproc::make_tok_qstr(Tok* next, std::string_view str) {
    return new_token_str(next, STR, quote(str));
}

// ---- strings in tokens ----

// Unquote a string token in place (it becomes an INTERNAL_STR); returns its text. Other tokens are returned as they are.
const char* Preproc::unquote_token_anystr(Tok* t, std::uint32_t badctl, char qstart) {
    if (t->type != STR) return t->text.c_str();
    bool ctl_error = false;
    t->text = unquote(t->text, badctl, qstart, ctl_error);
    if (ctl_error) as_.nonfatal("control character in string not allowed here");
    t->type = INTERNAL_STR;
    return t->text.c_str();
}

Tok* Preproc::quote_any_token(Tok* t) {
    t->text = quote(t->text);
    t->type = STR;
    return t;
}

std::optional<std::string> Preproc::pp_getenv(const Tok* t, bool warn) {
    if (!t) return std::nullopt;
    std::string txt = t->text;
    bool is_string = false;
    switch (t->type) {
        case ENVIRON:
            txt = txt.substr(2);
            is_string = !txt.empty() && isquote(txt[0]);
            break;
        case STR: is_string = true; break;
        case INTERNAL_STR:
        case NAKED_STR:
        case ID: is_string = false; break;
        default: return std::nullopt;
    }
    if (is_string) {
        bool ctl = false;
        txt = unquote(txt, BADCTL, STR_NASM, ctl);
        if (ctl) as_.nonfatal("control character in string not allowed here");
    }
    std::optional<std::string> v = host_.getenv ? host_.getenv(txt) : std::nullopt;
    if (warn && !v) {
        as_.warn(WARN_PP_ENVIRONMENT, "nonexistent environment variable `{}'", txt);
        v = std::string();
    }
    return v;
}

// ---- the tokenizer ----

Tok* Preproc::tokenize(std::string_view input) {
    const std::string buf(input);
    const char* line = buf.c_str();
    Tok* list = nullptr;
    Tok** tail = &list;

    while (*line) {
        const char* p = line;
        const char* ep = nullptr;
        int type;
        char firstchar = *p;

        if (*p == '%') {
            p++;
            if (*p == '+' && !isdigit_(p[1])) {
                p++;
            } else if (isdigit_(*p) || ((*p == '-' || *p == '+') && isdigit_(p[1]))) {
                do { p++; } while (isdigit_(*p));
            } else if (*p == '{' || *p == '[') {
                const char first = *p;
                const char endchar = static_cast<char>(*p + 2);
                int lvl = 1;
                line += (*p++ == '{');
                while (lvl) {
                    if (*p == first) {
                        lvl++;
                    } else if (*p == endchar) {
                        lvl--;
                    } else if (isquote(*p)) {
                        p += skip_string(p);
                    }
                    if (!*p) {
                        // NASM compares against '}' here, which is never true: the class is always the bracket one.
                        as_.warn(first == '}' ? WARN_PP_OPEN_BRACES : WARN_PP_OPEN_BRACKETS,
                                 "unterminated %{}...{} construct (missing `{}')", first, endchar, endchar);
                        break;
                    }
                    p++;
                }
                ep = lvl ? p : p - 1;
            } else if (*p == '?') {
                p++;
                if (*p == '?') p++;
            } else if (*p == '*' && p[1] == '?') {
                p += 2;
                if (*p == '?') p++;
            } else if (*p == '!') {
                p++;
                if (isidchar(*p)) {
                    do { p++; } while (isidchar(*p));
                } else if (isquote(*p)) {
                    p += skip_string(p);
                    if (*p) p++;
                    else as_.nonfatal("unterminated %! string");
                }
            } else if (*p == ',') {
                p++;
            } else if (isidchar(*p) || (*p == '%' && isidchar(p[1]))) {
                do { p++; } while (isidchar(*p));
            } else if (*p == '%') {
                p++;
            }
            if (!ep) ep = p;
            std::size_t toklen = static_cast<std::size_t>(ep - line);
            if (toklen < 2) {
                type = '%';
                if (*line == '{') as_.warn(WARN_PP_EMPTY_BRACES, "empty %{{}} construct expands to the % operator");
            } else {
                const char c0 = line[1];
                switch (c0) {
                    case '+': type = (toklen == 2) ? PASTE : MMACRO_PARAM; break;
                    case '-': type = MMACRO_PARAM; break;
                    case '?':
                        if (toklen == 2) type = PREPROC_Q;
                        else if (toklen == 3 && line[2] == '?') type = PREPROC_QQ;
                        else type = PREPROC_ID;
                        break;
                    case '*':
                        type = OTHER;
                        if (line[2] == '?') {
                            if (toklen == 3) type = PREPROC_SQ;
                            else if (toklen == 4 && line[3] == '?') type = PREPROC_SQQ;
                        }
                        break;
                    case '!': type = (toklen == 2) ? OTHER : ENVIRON; break;
                    case '%': type = (toklen == 2) ? SMOD : LOCAL_SYMBOL; break;
                    case '$': type = (toklen == 2) ? OTHER : LOCAL_MACRO; break;
                    case '[':
                        line += 2;
                        firstchar = *line;
                        toklen -= 2;
                        type = INDIRECT;
                        break;
                    case ',': type = (toklen == 2) ? COND_COMMA : PREPROC_ID; break;
                    case '\'':
                    case '"':
                    case '`': type = PREPROC_ID; break;
                    case ':': type = MMACRO_PARAM; break;
                    default:
                        if (isdigit_(c0)) type = MMACRO_PARAM;
                        else if (isidchar(c0) || toklen > 2) type = PREPROC_ID;
                        else type = OTHER;
                        break;
                }
            }
        } else if (*p == '?' && !isidchar(p[1])) {
            type = QMARK;
            p++;
        } else if (isidstart(*p) ||
                   (*p == '$' && isidchar(p[1]) && (p[1] != '$' || isidchar(p[2])) &&
                    (!as_.globl.dollarhex || !isdigit_(p[1])))) {
            type = ID;
            while (isidchar(*++p)) {}
        } else if (isquote(*p)) {
            const char quote_ch = *p;
            type = STR;
            p += skip_string(p);
            if (*p) {
                p++;
            } else {
                as_.warn(WARN_PP_OPEN_STRING, "unterminated string (missing `{}')", quote_ch);
                type = ERRSTR;
            }
        } else if (p[0] == '$' && p[1] == '$') {
            type = BASE;
            p += 2;
        } else if (isnumstart(*p)) {
            bool is_hex = false, is_float = false, has_e = false;
            char c;
            if (*p == '$') {
                p++;
                is_hex = true;
            }
            for (;;) {
                c = *p++;
                if (!is_hex && (c == 'e' || c == 'E')) {
                    has_e = true;
                    if (*p == '+' || *p == '-') {
                        p++;   // 1e+10 1e-10: a floating-point number
                        is_float = true;
                    }
                } else if (c == 'H' || c == 'h' || c == 'X' || c == 'x') {
                    is_hex = true;
                } else if (c == 'P' || c == 'p') {
                    is_float = true;
                    if (*p == '+' || *p == '-') p++;
                } else if (isnumchar(c)) {
                    // part of the number
                } else if (c == '.') {
                    // A dot is part of a number only when a floating-point mantissa follows
                    const char* r = p;
                    while (*r == '_') r++;
                    if (isdigit_(*r) || (is_hex && isxdigit_(*r)) || (!is_hex && (*r == 'e' || *r == 'E')) ||
                        (*r == 'p' || *r == 'P')) {
                        p = r;
                        is_float = true;
                    } else {
                        break;
                    }
                } else {
                    break;
                }
            }
            p--;   // point to the first character beyond the number
            if (p == line + 1 && *line == '$') {
                type = HERE;
            } else {
                if (has_e && !is_hex) is_float = true;
                type = is_float ? FLOAT : NUM;
            }
        } else if (isspace_(*p)) {
            firstchar = ' ';
            type = WHITESPACE;
            p = skip_blanks(p);
            if (!*p || *p == ';') type = EOS;   // a comment, or trailing white space, ends the line
        } else if (*p == ';') {
            type = EOS;
        } else {
            type = static_cast<unsigned char>(*p);
            switch (*p++) {
                case '>':
                    if (*p == '>') {
                        p++;
                        type = SHR;
                        if (*p == '>') { type = SAR; p++; }
                    } else if (*p == '=') {
                        type = GE;
                        p++;
                    }
                    break;
                case '<':
                    if (*p == '<') {
                        p++;
                        type = SHL;
                        if (*p == '<') p++;
                    } else if (*p == '=') {
                        p++;
                        type = LE;
                        if (*p == '>') { p++; type = LEG; }
                    } else if (*p == '>') {
                        p++;
                        type = NE;
                    }
                    break;
                case '!':
                    if (*p == '=') { p++; type = NE; }
                    break;
                case '/':
                    if (*p == '/') { p++; type = SDIV; }
                    break;
                case '=':
                    if (*p == '=') p++;
                    break;
                case '&':
                    if (*p == '&') { p++; type = DBL_AND; }
                    break;
                case '|':
                    if (*p == '|') { p++; type = DBL_OR; }
                    break;
                case '^':
                    if (*p == '^') { p++; type = DBL_XOR; }
                    break;
                default: break;
            }
        }

        if (type == EOS) break;
        if (!ep) ep = p;
        const std::size_t toklen = static_cast<std::size_t>(ep - line);
        if (toklen) {
            Tok* t = new_token(nullptr, type, std::string_view(line, toklen));
            if (!t->text.empty()) t->text[0] = firstchar;   // a whitespace run is one blank, %{ becomes %
            *tail = t;
            tail = &t->next;
        }
        line = p;
    }
    return list;
}

// ---- back to text ----

std::string Preproc::detoken(Tok* tlist, bool expand_locals) {
    for (Tok* t = tlist; t; t = t->next) {
        switch (t->type) {
            case ENVIRON: {
                const std::optional<std::string> v = pp_getenv(t, true);
                t->text = v ? *v : std::string();
                t->type = NAKED_STR;
                break;
            }
            case LOCAL_MACRO:
            case LOCAL_SYMBOL:
                if (expand_locals) {
                    std::string_view q;
                    Context* ctx = get_ctx(t->text, &q);
                    if (ctx) {
                        t->text = "..@" + std::to_string(ctx->number) + "." + std::string(q);
                        t->type = ID;
                    }
                }
                break;
            default: break;
        }
    }
    std::string line;
    for (const Tok* t = tlist; t; t = t->next) {
        if (t->type == INDIRECT) {
            line += "%[";
            line += t->text;
            line += ']';
        } else {
            line += t->text;
        }
    }
    return line;
}

// ---- the scan callback of the evaluator ----

TokenType Preproc::ppscan(PpScan& pps, TokenVal& tokval) {
    Tok* tline;
    do {
        if (pps.ntokens && (tline = pps.tptr)) {
            pps.ntokens--;
            pps.tptr = tline->next;
        } else {
            pps.tptr = nullptr;
            pps.ntokens = 0;
            return tokval.type = TokenType::EOS;
        }
    } while (tline->type == WHITESPACE);

    const std::string& txt = tline->text;
    tokval.start = txt;
    tokval.text = txt;
    switch (tline->type) {
        default: break;
        case ID:
            if (txt[0] == '$') {
                tokval.text = txt.substr(1);   // a $-prefixed identifier is never a keyword
            } else {
                if (host_.token_hash) return host_.token_hash(txt, tokval);
                return tokval.type = TokenType::ID;
            }
            break;
        case NUM: {
            bool rn_error = false;
            if (txt[0] == '$') as_.warn_dollar_hex();
            tokval.integer = as_.hooks.readnum ? as_.hooks.readnum(txt, rn_error) : 0;
            return tokval.type = rn_error ? TokenType::ERRNUM : TokenType::NUM;
        }
        case STR:
            unquote_token(tline);
            tokval.text = tline->text;
            [[fallthrough]];
        case INTERNAL_STR:
        case NAKED_STR:
            tokval.text = tline->text;
            tokval.start = tline->text;
            tokval.inttwo = static_cast<std::int64_t>(tline->text.size());
            return tokval.type = TokenType::STR;
    }
    return tokval.type = static_cast<TokenType>(tline->type);
}

std::optional<Expr> Preproc::evaluate_tokens(PpScan& pps, TokenVal& tokval) {
    if (!host_.evaluate) return std::nullopt;
    tokval.type = TokenType::INVALID;
    PpScanFn scan = [this, &pps](TokenVal& tv) { return ppscan(pps, tv); };
    return host_.evaluate(scan, tokval, 1);
}

}  // namespace cb::build::x86asm
