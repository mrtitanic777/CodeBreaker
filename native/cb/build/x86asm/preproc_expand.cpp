// cb/build/x86asm/preproc_expand.cpp -- macro expansion: token pasting, the %1 / %% / %? parameter substitution of a
// multi-line macro body, single-line macro calls (argument parsing, the expansion with parameters), multi-line macro
// invocation. (preproc.c: paste_tokens, expand_mmac_params, expand_smacro, expand_id, is_mmacro, expand_mmacro ...)
#include <climits>
#include <cstdlib>

#include "cb/build/x86asm/preproc.hpp"

namespace cb::build::x86asm {

using namespace ppt;
using namespace pp;

namespace {

// The condition codes a %+1 or %-1 parameter can name, in the order NASM keeps them, and the opposite of each (-1: none).
const char* const kPpConditions[] = {"a", "ae", "b", "be", "c", "cxz", "e", "ecxz", "g", "ge", "l", "le",
                                   "na", "nae", "nb", "nbe", "nc", "ne", "ng", "nge", "nl", "nle", "no",
                                   "np", "ns", "nz", "o", "p", "pe", "po", "rcxz", "s", "z"};
constexpr int kPpNumConditions = 33;
const int kPpInverseCcs[] = {
    // c_NA, c_NAE, c_NB, c_NBE, c_NC, -1, c_NE, -1, c_NG, c_NGE, c_NL, c_NLE
    12, 13, 14, 15, 16, -1, 17, -1, 18, 19, 20, 21,
    // c_A, c_AE, c_B, c_BE, c_C, c_E, c_G, c_GE, c_L, c_LE, c_O, c_P, c_S
    0, 1, 2, 3, 4, 6, 8, 9, 10, 11, 26, 27, 31,
    // c_Z, c_NO, c_NP, c_PO, c_PE, -1, c_NS, c_NZ
    32, 22, 23, 29, 28, -1, 24, 25};

// An unsigned decimal number filling the whole text (strtoul with the check that nothing follows); false otherwise.
bool pp_parse_unsigned(const char* s, unsigned long& out) {
    if (!*s) {
        out = 0;
        return true;   // strtoul of "" is 0 with the end at the start, which is the end of the string
    }
    unsigned long v = 0;
    const char* p = s;
    while (*p == ' ' || (*p >= 9 && *p <= 13)) ++p;
    if (*p == '+') ++p;
    if (!(*p >= '0' && *p <= '9')) return false;
    while (*p >= '0' && *p <= '9') v = v * 10 + static_cast<unsigned long>(*p++ - '0');
    out = v;
    return !*p;
}

}  // namespace

// The index of the condition code the first token names, or -1.
int Preproc::find_cc(Tok* t) {
    if (!t) return -1;
    t = skip_white(t);
    if (!tok_is(t, ID)) return -1;
    Tok* tt = skip_white(t->next);
    if (tok_isnt(tt, ',')) return -1;
    for (int i = 0; i < kPpNumConditions; i++)
        if (ieq(t->text, kPpConditions[i])) return i;
    return -1;
}

// ---- token pasting ----

bool Preproc::pp_concat_match(const Tok* t, int mask) {
    int ctype = 0;
    if (!t) return false;
    switch (t->type) {
        case ID:
        case QMARK: ctype = CONCAT_ID; break;
        case LOCAL_MACRO: ctype = CONCAT_LOCAL_MACRO; break;
        case ENVIRON: ctype = CONCAT_ENVIRON; break;
        case PREPROC_ID: ctype = CONCAT_PREPROC_ID; break;
        case NUM:
        case FLOAT: ctype = CONCAT_NUM; break;
        case HERE:
        case BASE: ctype = CONCAT_OP; break;
        case OTHER: ctype = CONCAT_OP; break;
        default:
            if (t->type > WHITESPACE && t->type < MAX_OPERATOR) ctype = CONCAT_OP;
            else ctype = 0;
            break;
    }
    return !!(ctype & mask);
}

// Paste tokens together: runs of tokens whose types match the masks (and, with `handle_explicit`, the %+ operator).
// Returns true if anything was pasted.
bool Preproc::paste_tokens(Tok** head, const ConcatMask* m, std::size_t mnum, bool handle_explicit) {
    Tok *tok, *t, *next, **prev_next, **prev_nonspace, **nextp;
    bool pasted = false;
    std::size_t len, i;

    tok = *head;
    prev_next = prev_nonspace = head;
    if (tok_white(tok) || tok_is(tok, PASTE)) prev_nonspace = nullptr;

    while (tok && (next = tok->next)) {
        bool did_paste = false;
        switch (tok->type) {
            case WHITESPACE:
                // Zap redundant whitespace tokens
                tok->next = next = zap_white(next);
                break;
            case PASTE:
                // Explicit paste, as in CAT %+ CAT
                if (!handle_explicit) break;
                did_paste = true;
                if (!prev_nonspace) {
                    // Leading paste, ignore
                    prev_next = nextp = head;
                    t = nullptr;
                } else {
                    prev_next = prev_nonspace;
                    t = *prev_next;
                    nextp = &t->next;
                }
                next = *nextp;
                while (next) {
                    if (next->type == PASTE || next->type == WHITESPACE) next = do_delete_token(next);
                    else break;
                }
                *nextp = next;
                if (!next) break;   // Trailing paste, ignore
                if (!t) {
                    // Leading paste: a nop
                    *prev_next = tok = next;
                    break;
                }
                {
                    std::string buf = t->text + next->text;
                    do_delete_token(t);
                    t = tokenize(buf);
                    if (!t) {
                        tok = t = new_white(nullptr);
                    } else {
                        *prev_nonspace = tok = t;
                    }
                }
                while (t->next) t = t->next;
                t->next = do_delete_token(next);
                *prev_next = next = tok;
                break;
            default:
                // implicit paste
                for (i = 0; i < mnum; i++)
                    if (pp_concat_match(tok, m[i].mask_head)) break;
                if (i >= mnum) break;
                len = tok->text.size();
                while (pp_concat_match(next, m[i].mask_tail)) {
                    len += next->text.size();
                    next = next->next;
                }
                if (len == tok->text.size()) break;   // nothing to paste
                {
                    std::string buf;
                    while (tok != next) {
                        buf += tok->text;
                        tok = do_delete_token(tok);
                    }
                    *prev_next = tok = t = tokenize(buf);
                    if (!t) *prev_next = tok = t = new_white(nullptr);
                }
                while ((tok = t->next)) {
                    if (tok->type != WHITESPACE && tok->type != PASTE) prev_nonspace = &t->next;
                    t = tok;
                }
                t->next = next;
                prev_next = &t->next;
                did_paste = true;
                break;
        }
        if (did_paste) {
            pasted = true;
        } else {
            prev_next = &tok->next;
            if (next && next->type != WHITESPACE && next->type != PASTE) prev_nonspace = prev_next;
        }
        tok = next;
    }
    return pasted;
}

// ---- %1 %2 ... %0 %+1 %-1 %{1:3} %? %?? %%x %[...] inside a multi-line macro body ----

int Preproc::mmac_rotate(const MMacro* mac, unsigned n) {
    if (--n < mac->nparam) n = (n + mac->rotate) % mac->nparam;
    return static_cast<int>(n + 1);
}

// "%{first:last}": a range of the parameters, comma separated (negative numbers count from the end).
void Preproc::expand_mmac_params_range(MMacro* mac, Tok* tline, Tok*** tail) {
    const std::string arg = tline->text.substr(1);
    int fst = 0, lst = 0, incr, n;
    {
        char* e = nullptr;
        fst = static_cast<int>(std::strtol(arg.c_str(), &e, 10));
        if (*e != ':') as_.assert_failed("parsed == 2");
        lst = static_cast<int>(std::strtol(e + 1, &e, 10));
    }
    const int np = static_cast<int>(mac->nparam);
    if (lst == 0 || fst == 0) goto err;
    if ((fst > np || fst < -np) || (lst > np || lst < -np)) goto err;
    fst = fst < 0 ? fst + np + 1 : fst;
    lst = lst < 0 ? lst + np + 1 : lst;
    incr = (fst < lst) ? 1 : -1;
    while (true) {
        n = mmac_rotate(mac, static_cast<unsigned>(fst));
        dup_tlistn(mac->params[static_cast<std::size_t>(n)], static_cast<std::size_t>(mac->paramlen[static_cast<std::size_t>(n)]), tail);
        if (fst == lst) break;
        Tok* t = make_tok_char(nullptr, ',');
        **tail = t;
        *tail = &t->next;
        fst += incr;
    }
    return;
err:
    as_.nonfatal("`%{{{}}}': macro parameters out of range", arg);
}

// Expand the macro-parameter tokens of a line of a multi-line macro's body (those that need the current invocation).
Tok* Preproc::expand_mmac_params(Tok* tline) {
    Tok **tail, *thead;
    bool changed = false;
    MMacro* const mac = istk_->mstk.mmac;

    tail = &thead;
    thead = nullptr;

    while (tline) {
        bool change;
        bool err_not_mac = false;
        Tok* t = tline;
        std::optional<std::string> text = t->text;
        int type = t->type;

        tline = tline->next;
        t->next = nullptr;

        switch (type) {
            case LOCAL_SYMBOL:
                change = true;
                if (!mac) {
                    err_not_mac = true;
                    break;
                }
                type = ID;
                text = "..@" + std::to_string(mac->unique) + "." + t->text.substr(2);
                break;

            case MMACRO_PARAM: {
                Tok* tt = nullptr;
                change = true;
                if (!mac) {
                    err_not_mac = true;
                    break;
                }
                const std::string& tx = t->text;
                if (tx.find(':') != std::string::npos) {
                    expand_mmac_params_range(mac, t, &tail);
                    text.reset();
                    break;
                }
                const char c1 = tx.size() > 1 ? tx[1] : '\0';
                bool invalid = false;
                switch (c1) {
                    case '-':
                    case '+': {
                        unsigned long n;
                        if (!pp_parse_unsigned(tx.c_str() + 2, n)) {
                            invalid = true;
                            break;
                        }
                        if (n && n <= mac->nparam) {
                            n = static_cast<unsigned long>(mmac_rotate(mac, static_cast<unsigned>(n)));
                            tt = mac->params[n];
                        }
                        int cc = find_cc(tt);
                        if (cc == -1) {
                            as_.nonfatal("macro parameter `{}' is not a condition code", tx);
                            text.reset();
                            break;
                        }
                        type = ID;
                        if (c1 == '-') {
                            const int ncc = kPpInverseCcs[cc];
                            if (ncc == -1) {
                                as_.nonfatal("condition code `{}' is not invertible", kPpConditions[cc]);
                                break;   // the text is left as it was
                            }
                            cc = ncc;
                        }
                        text = kPpConditions[cc];
                        break;
                    }
                    case '0':
                        if (tx.size() == 2) {
                            type = NUM;
                            text = std::to_string(mac->nparam);
                            break;
                        }
                        if (tx[2] != '0' || tx.size() > 3) {
                            invalid = true;
                            break;
                        }
                        [[fallthrough]];   // %00 is the label
                    default: {
                        unsigned long n;
                        if (!pp_parse_unsigned(tx.c_str() + 1, n)) {
                            invalid = true;
                            break;
                        }
                        if (n <= mac->nparam) {
                            n = static_cast<unsigned long>(mmac_rotate(mac, static_cast<unsigned>(n)));
                            dup_tlistn(mac->params[n], static_cast<std::size_t>(mac->paramlen[n]), &tail);
                        }
                        text.reset();
                        break;
                    }
                }
                if (invalid) {
                    as_.nonfatal("invalid macro parameter: `{}'", tx);
                    text.reset();
                }
                break;
            }

            case PREPROC_Q:
                // %?: the name of the macro as it was invoked
                if (mac) {
                    type = ID;
                    text = mac->iname;
                    change = true;
                } else {
                    change = false;
                }
                break;

            case PREPROC_QQ:
                // %??: the canonical name of the macro
                if (mac) {
                    type = ID;
                    text = mac->name;
                    change = true;
                } else {
                    change = false;
                }
                break;

            case INDIRECT: {
                // %[...]: expand the contents, then paste the result in
                Tok* tt = tokenize(t->text);
                tt = expand_mmac_params(tt);
                tt = expand_smacro(tt);
                tail = steal_tlist(tt, tail);
                text.reset();
                change = true;
                break;
            }

            default:
                change = false;
                break;
        }

        if (err_not_mac) {
            as_.nonfatal("`{}': not in a macro call", *text);
            text.reset();
            change = true;
        }

        if (change) {
            if (!text) {
                do_delete_token(t);
            } else {
                *tail = t;
                tail = &t->next;
                t->type = type;
                t->text = *text;
            }
            changed = true;
        } else {
            *tail = t;
            tail = &t->next;
        }
    }
    *tail = nullptr;

    if (changed) {
        static const ConcatMask kPaste[] = {
            {CONCAT_ID | CONCAT_FLOAT, CONCAT_ID | CONCAT_NUM | CONCAT_FLOAT | CONCAT_OP},
            {CONCAT_NUM, CONCAT_NUM}};
        paste_tokens(&thead, kPaste, 2, false);
    }
    return thead;
}

// ---- single-line macros ----

// A parameter of a & macro as a quoted string: the expanded tokens, written out, in quotes (unless it already is one string
// and the parameter is && conditional).
Tok* Preproc::expand_sparm_str(Tok* param, int flags) {
    Tok* qs = expand_smacro_noreset(param);
    if ((flags & SPARM_CONDQUOTE) && tok_is(qs, STR) && !qs->next) {
        // Already a single string: use it as it is
    } else {
        const std::string arg = detoken(qs, false);
        delete_tlist(qs);
        qs = make_tok_qstr(nullptr, arg);
    }
    return qs;
}

// Expand macro `m`, called at `mstart`, with its arguments `params`. The expansion is returned in reverse order; if `epp`
// is given it gets the pointer to the link after the last token.
Tok* Preproc::expand_smacro_with_params(SMacro* m, Tok* mstart, Tok** params, int nparam, Tok*** epp) {
    const char* const mtype = (!m->name.empty() && m->name[0] == '%') ? "function" : "macro";
    Tok *t, *tline, *tup;
    bool cond_comma;
    const SmacParam* mparm;
    int i;

    m->in_progress++;

    if (nparam) {
        for (i = 0, mparm = m->params.data(); i < nparam; i++, mparm += !(mparm->flags & SPARM_VARADIC)) {
            const int flags = mparm->flags;
            if (flags & SPARM_EVAL) {
                // Evaluate the parameter as a number
                Tok* eval_param = zap_white(expand_smacro_noreset(params[i]));
                params[i] = nullptr;

                if (!eval_param) {
                    if (mparm->def) {
                        params[i] = dup_tlist(mparm->def, nullptr);
                        continue;
                    } else if (flags & SPARM_OPTIONAL) {
                        continue;
                    }
                }

                PpScan pps;
                pps.tptr = eval_param;
                pps.ntokens = -1;
                TokenVal tokval;
                std::optional<Expr> evalresult = evaluate_tokens(pps, tokval);
                delete_tlist(eval_param);
                if (!evalresult) {
                    // Error already reported
                } else if (tokval.type != TokenType::EOS) {
                    as_.nonfatal("invalid expression in parameter {} of {} `{}'", i + 1, mtype, m->name);
                } else if (!pp_is_simple(*evalresult)) {
                    as_.nonfatal("non-constant expression in parameter {} of {} `{}'", i + 1, mtype, m->name);
                } else {
                    const std::int64_t v = pp_reloc_value(*evalresult);
                    params[i] = make_tok_num_radix(nullptr, v, mparm->radix, !!(flags & SPARM_UNSIGNED));
                }
            }
            if (flags & SPARM_STR) params[i] = expand_sparm_str(params[i], flags);
        }
    }

    t = m->expand ? (this->*m->expand)(m, params, nparam) : smacro_expand_default(m, params, nparam);
    tup = tline = nullptr;
    cond_comma = false;

    while (t) {
        const int type = t->type;
        Tok* tnext = t->next;

        switch (type) {
            case PREPROC_Q:
            case PREPROC_SQ:
                do_delete_token(t);
                t = dup_token(tline, mstart);
                break;

            case PREPROC_QQ:
            case PREPROC_SQQ: {
                t->type = mstart->type;
                std::string from;
                if (t->type == LOCAL_MACRO) {
                    std::string_view pep;
                    get_ctx(mstart->text, &pep);
                    const std::size_t plen = mstart->text.size() - pep.size();
                    from = mstart->text.substr(0, plen);
                }
                from += m->name;
                t->text = from;
                t->next = tline;
                break;
            }

            case COND_COMMA:
                do_delete_token(t);
                t = cond_comma ? make_tok_char(tline, ',') : nullptr;
                break;

            case ID:
            case PREPROC_ID:
            case LOCAL_MACRO: {
                // Expand macros in the expansion as soon as possible
                Tok** tp = &t;
                t->next = tline;
                expand_one_smacro(&tp);
                tline = *tp;   // the rest of the line, from here on
                break;
            }

            default:
                if (is_smac_param(t->type)) {
                    const int param = smac_nparam(t->type);
                    if (tup || param >= nparam) as_.assert_failed("!tup && param < nparam");
                    do_delete_token(t);
                    t = nullptr;
                    tup = tnext;
                    tnext = dup_tlist_reverse(params[param], nullptr);
                    cond_comma = false;
                } else {
                    t->next = tline;
                }
                break;
        }

        if (t) {
            Tok* endt = tline;
            tline = t;
            while (!cond_comma && t && t != endt) {
                cond_comma = t->type != WHITESPACE;
                t = t->next;
            }
        }

        if (tnext) {
            t = tnext;
        } else {
            t = tup;
            tup = nullptr;
        }
    }

    if (epp) {
        Tok** ep = *epp;
        for (t = tline; t; t = t->next) ep = &t->next;
        *epp = ep;
    }

    m->in_progress--;
    return tline;
}

// The number of arguments in the call that starts at `t` (a "(": 0 if there is none), and the closing ")" in *tp.
int Preproc::count_smacro_args(Tok* t, Tok** tp) {
    int nparam, paren, brackets;

    t = skip_white(t);
    if (!tok_is(t, '(')) return 0;

    paren = 1;
    nparam = 1;
    brackets = 0;
    while (paren) {
        t = t->next;
        if (!t) {
            as_.nonfatal("macro call expects terminating `)'");
            return 0;
        }
        switch (t->type) {
            case ',':
                if (!brackets && paren == 1) nparam++;
                break;
            case '{': brackets++; break;
            case '}':
                if (brackets > 0) brackets--;
                break;
            case '(':
                if (!brackets) paren++;
                break;
            case ')':
                if (!brackets) paren--;
                break;
            default: break;
        }
    }
    if (tp) *tp = t;
    return nparam;
}

// Split the arguments of a call into nparam token lists (the result has *nparamp entries).
std::vector<Tok*> Preproc::parse_smacro_args(Tok** tp, int* nparamp, const SMacro* m) {
    Tok **phead, **pep;
    int white = 0;
    int brackets = 0;
    int paren;
    bool bracketed = false;
    int i;
    int flags;
    const SmacParam* mparm;
    Tok* t = *tp;
    int nparam = *nparamp;
    const char* const mtype = (!m->name.empty() && m->name[0] == '%') ? "function" : "macro";

    t = skip_white(t);
    if (!tok_is(t, '(')) as_.assert_failed("tok_is(t, '(')");

    if (nparam > m->nparam) {
        // Too many parameters: only legal if the last is greedy
        if (m->params[static_cast<std::size_t>(m->nparam - 1)].flags & SPARM_GREEDY) *nparamp = nparam = m->nparam;
    } else if (nparam < m->nparam) {
        // Too few: pad with empty parameters
        *nparamp = nparam = m->nparam;
    }

    paren = 1;
    std::vector<Tok*> params(static_cast<std::size_t>(nparam > 0 ? nparam : 1), nullptr);
    i = 0;
    mparm = m->params.data();
    flags = mparm->flags;
    phead = pep = &params[0];
    *pep = nullptr;

    while (paren) {
        bool skip;
        t = t->next;
        if (!t) {
            as_.nonfatal("{} `{}' call expects terminating `)'", mtype, m->name);
            break;
        }
        skip = false;
        switch (t->type) {
            case WHITESPACE:
                if (!(flags & SPARM_NOSTRIP)) {
                    if (brackets || *phead) white++;
                    skip = true;
                }
                break;
            case ',':
                if (!brackets && paren == 1 && !(flags & SPARM_GREEDY)) {
                    i++;
                    if (i >= nparam) as_.assert_failed("i < nparam");
                    phead = pep = &params[static_cast<std::size_t>(i)];
                    *pep = nullptr;
                    bracketed = false;
                    skip = true;
                    if (!(flags & SPARM_VARADIC)) {
                        mparm++;
                        flags = mparm->flags;
                    }
                }
                break;
            case '{':
                if (!bracketed) {
                    bracketed = !*phead && !(flags & SPARM_NOSTRIP);
                    skip = bracketed;
                }
                brackets++;
                break;
            case '}':
                if (brackets > 0) {
                    if (!--brackets) skip = bracketed;
                }
                break;
            case '(':
                if (!brackets) paren++;
                break;
            case ')':
                if (!brackets) {
                    paren--;
                    if (!paren) {
                        skip = true;
                        i++;   // Found the last argument
                    }
                }
                break;
            default: break;
        }
        if (!skip) {
            Tok* tt;
            if (white) {
                *pep = tt = new_white(nullptr);
                pep = &tt->next;
                white = 0;
            }
            *pep = tt = dup_token(nullptr, t);
            pep = &tt->next;
        }
    }
    *tp = t;
    return params;
}

// Expand the single-line macro that starts at **tpp, if it is one. On return *tpp is the link after what was done.
// Returns the macro that was expanded, or null.
SMacro* Preproc::expand_one_smacro(Tok*** tpp) {
    std::vector<Tok*> params;
    std::string_view mname;
    Tok* mstart = **tpp;
    Tok* tline = mstart;
    SMacro *head, *m;
    Tok *tafter, **tep;
    int nparam = 0;

    if (!tline) return nullptr;

    mname = mstart->text;

    smacro_deadman_.total--;
    smacro_deadman_.levels--;

    if (smacro_deadman_.total < 0 || smacro_deadman_.levels < 0) {
        if (!smacro_deadman_.triggered) {
            as_.nonfatal("interminable macro recursion");
            smacro_deadman_.triggered = true;
        }
        goto not_a_macro;
    } else if (tline->type == ID || tline->type == PREPROC_ID) {
        head = find_smacro_head(smacros_, mname);
    } else if (tline->type == LOCAL_MACRO) {
        Context* ctx = get_ctx(mname, &mname);
        head = ctx ? find_smacro_head(ctx->localmac, mname) : nullptr;
    } else {
        goto not_a_macro;
    }

    for (m = head; m; m = m->next) {
        if (m->alias && ppconf_.noaliases) continue;
        if (streq(m->name, mname, m->casesense)) break;
    }
    if (!m) goto not_a_macro;

    // if this is a macro with parameters, grab them
    nparam = 0;
    if (m->nparam == 0) {
        // No parameters: just use the macro definition
    } else {
        tline = skip_white(tline->next);
        nparam = count_smacro_args(tline, nullptr);
        if (!nparam) goto not_a_macro;   // No arguments: not a macro call

        // Find a version of the macro that takes this number of arguments
        while (1) {
            if (!m) {
                as_.warn(WARN_PP_MACRO_PARAMS_SINGLE, "single-line macro `{}' exists, but not taking {} parameter{}", mname,
                         nparam, (nparam == 1) ? "" : "s");
                goto not_a_macro;
            }
            if (streq(m->name, mname, m->casesense)) {
                if (nparam >= m->nparam_min && (m->varadic || nparam <= m->nparam)) break;
            }
            m = m->next;
        }
    }

    // Expand it, unless it is already being expanded (and is not marked recursive)
    if (m->in_progress && !m->recursive) goto not_a_macro;

    if (nparam) params = parse_smacro_args(&tline, &nparam, m);

    tafter = tline->next;
    tline->next = nullptr;
    tline = expand_smacro_with_params(m, mstart, params.data(), nparam, &tep);
    if (tline) {
        **tpp = tline;
        *tep = tafter;
        *tpp = tep;
    } else {
        **tpp = tafter;
    }
    delete_tlist(mstart);
    goto done;

not_a_macro:
    *tpp = &mstart->next;
    m = nullptr;

done:
    for (Tok* pt : params) free_tlist(pt);
    smacro_deadman_.levels++;
    return m;
}

Tok* Preproc::expand_smacro(Tok* tline) {
    smacro_deadman_.total = as_.nasm_limit[LIMIT_MACRO_TOKENS];
    smacro_deadman_.levels = as_.nasm_limit[LIMIT_MACRO_LEVELS];
    smacro_deadman_.triggered = false;
    return expand_smacro_noreset(tline);
}

Tok* Preproc::expand_smacro_noreset(Tok* org_tline) {
    Tok* tline;
    bool expanded;
    ErrHold errhold = 0;

    if (!org_tline) return nullptr;   // Empty input

    // Make a duplicate of the first token, so the caller's pointer stays valid (its contents are replaced below)
    tline = dup_token(org_tline->next, org_tline);
    expanded = true;
    while (true) {
        static const ConcatMask tmatch[] = {
            {CONCAT_ID | CONCAT_LOCAL_MACRO | CONCAT_ENVIRON | CONCAT_PREPROC_ID,
             CONCAT_ID | CONCAT_LOCAL_MACRO | CONCAT_ENVIRON | CONCAT_PREPROC_ID | CONCAT_NUM}};
        Tok** tail = &tline;

        // Hold the errors of this round: if another round follows they are dropped (they will be seen again)
        errhold = as_.error_hold_push();
        while (*tail) expanded |= !!expand_one_smacro(&tail);

        if (!expanded) break;
        if (!paste_tokens(&tline, tmatch, 1, true)) break;

        as_.error_hold_pop(errhold, false);
        expanded = false;
    }
    as_.error_hold_pop(errhold, true);

    if (!tline) tline = new_white(nullptr);   // Don't lose the first token

    steal_token(org_tline, tline);
    org_tline->next = tline->next;
    do_delete_token(tline);
    return org_tline;
}

// Expand the identifier tokens at the start of a line (a macro name or a label that is itself made by a macro).
Tok* Preproc::expand_id(Tok* tline) {
    Tok *cur, *oldnext = nullptr;

    if (!tline || !tline->next) return tline;

    cur = tline;
    while (cur->next && (cur->next->type == ID || cur->next->type == PREPROC_ID || cur->next->type == LOCAL_MACRO ||
                         cur->next->type == NUM))
        cur = cur->next;

    // If identifier tokens follow, make the list end there, expand, and reattach the rest
    if (cur == tline) return tline;

    if (cur) {
        oldnext = cur->next;
        cur->next = nullptr;
    }

    tline = expand_smacro(tline);

    if (cur) {
        // Find the end of the expansion and hook the rest of the line back on
        cur = tline;
        while (cur && cur->next) cur = cur->next;
        if (cur) cur->next = oldnext;
    }
    return tline;
}

// ---- multi-line macros ----

// Prepare macro `m` for a call with *nparamp arguments (add the defaults, trim the "+" tail); on a recursion limit or an
// impossible count, returns null with the parameters dropped.
MMacro* Preproc::use_mmacro(MMacro* m, int* nparamp, std::vector<Tok*>* paramsp) {
    int nparam = *nparamp;
    std::vector<Tok*>& params = *paramsp;

    // Is this macro already in progress at its maximum depth?
    if (m->in_progress > m->max_depth) {
        if (m->max_depth > 0) as_.warn(WARN_OTHER, "reached maximum recursion depth of {}", m->max_depth);
        params.clear();
        *nparamp = 0;
        return nullptr;
    }

    // Add default parameters
    if (!m->defaults.empty() && nparam < m->nparam_min + m->ndefs) {
        const int newnparam = m->nparam_min + m->ndefs;
        params.resize(static_cast<std::size_t>(newnparam + 2), nullptr);
        for (int k = 0; k < newnparam - nparam; k++)
            params[static_cast<std::size_t>(nparam + 1 + k)] = m->defaults[static_cast<std::size_t>(nparam + 1 - m->nparam_min + k)];
        nparam = newnparam;
    }

    // A "+" macro: the last parameter takes the rest
    if (m->plus && nparam > m->nparam_max) nparam = m->nparam_max;

    if (params.size() < static_cast<std::size_t>(nparam) + 2) params.resize(static_cast<std::size_t>(nparam) + 2, nullptr);
    params[static_cast<std::size_t>(nparam) + 1] = nullptr;
    *nparamp = nparam;
    return m;
}

MMacro* Preproc::find_mmacro_in_list(MMacro* m, std::string_view finding, int* nparamp, std::vector<Tok*>* paramsp) {
    const int nparam = *nparamp;
    while (m) {
        if (m->nparam_min <= nparam && (m->plus || nparam <= m->nparam_max)) return use_mmacro(m, nparamp, paramsp);
        for (m = m->next; m; m = m->next)
            if (streq(m->name, finding, m->casesense)) break;
    }
    return nullptr;
}

// If the line starts with a multi-line macro name, find the macro that takes this many arguments (and split the arguments).
MMacro* Preproc::is_mmacro(Tok* tline, int* nparamp, std::vector<Tok*>* paramsp) {
    MMacro *head, *m, *found;
    Tok** comma;
    int raw_nparam, nparam;
    std::string finding;
    bool empty_args;

    *nparamp = 0;
    paramsp->clear();

    if (!tok_macro_id(tline)) return nullptr;

    finding = tline->text;
    empty_args = !tline->next;

    auto it = mmacros_.find(lcase(finding));
    head = it == mmacros_.end() ? nullptr : it->second;
    for (m = head; m; m = m->next) {
        if (streq(m->name, finding, m->casesense) && (m->in_progress != 1 || m->max_depth > 0)) break;
    }
    if (!m) return nullptr;   // No such macro (or one that cannot be recursed into)

    // Count the arguments and find the macro that takes this number
    comma = count_mmac_params(tline->next, nparamp, paramsp);
    raw_nparam = *nparamp;
    found = find_mmacro_in_list(m, finding, nparamp, paramsp);
    if (paramsp->empty()) return nullptr;
    nparam = *nparamp;
    std::vector<Tok*>& params = *paramsp;

    if (!ppconf_.sane_empty_expansion) {
        // Backwards-compatibility: handling of empty arguments as NASM 2.x did
        if (!found) {
            if (raw_nparam == 0 && !empty_args) {
                // Plain "macro" with no arguments is "macro" with one empty argument
                int bogus_nparam = 1;
                params[2] = nullptr;
                found = find_mmacro_in_list(m, finding, &bogus_nparam, paramsp);
            } else if (raw_nparam > 1 && comma) {
                // Drop the trailing empty argument and try again
                Tok* comma_tail = *comma;
                *comma = nullptr;
                *nparamp = raw_nparam - 1;
                found = find_mmacro_in_list(m, finding, nparamp, paramsp);
                if (found) delete_tlist(comma_tail);
                else *comma = comma_tail;
            }
            if (paramsp->empty()) return nullptr;
        } else if (comma) {
            delete_tlist(*comma);
            *comma = nullptr;
            if (raw_nparam > found->nparam_min && raw_nparam <= found->nparam_min + found->ndefs) {
                // The empty argument is replaced by its default
                (*paramsp)[static_cast<std::size_t>(raw_nparam)] =
                    found->defaults[static_cast<std::size_t>(raw_nparam - found->nparam_min)];
            } else if (raw_nparam > found->nparam_max && found->plus) {
                // The + parameter takes it
            } else {
                // Drop the empty argument
                (*paramsp)[static_cast<std::size_t>(raw_nparam)] = nullptr;
                *nparamp = nparam = raw_nparam - 1;
            }
        }
        if (found) {
            if (raw_nparam < found->nparam_min || (raw_nparam > found->nparam_max && !found->plus)) {
                as_.warn(WARN_PP_MACRO_PARAMS_LEGACY, "improperly calling multi-line macro `{}' with {} parameters", found->name,
                         raw_nparam);
            } else if (comma) {
                as_.warn(WARN_PP_MACRO_PARAMS_LEGACY, "dropping trailing empty parameter in call to multi-line macro `{}'",
                         found->name);
            }
        }
    }

    if (found) return found;

    as_.warn(WARN_PP_MACRO_PARAMS_MULTI, "multi-line macro `{}' exists, but not taking {} parameter{}", finding, nparam,
             (nparam == 1) ? "" : "s");
    paramsp->clear();
    return nullptr;
}

// The "[macro]" comment line of the listing for a macro call.
void Preproc::list_mmacro_call(const MMacro* m) {
    std::string buf = " ;;; [macro] " + m->iname + " ";
    for (unsigned i = 1; i <= m->nparam; i++) {
        int j = 0;
        buf += '{';
        for (const Tok* t = m->params[i]; t; t = t->next) {
            if (j++ >= m->paramlen[i]) break;
            buf += t->text;
        }
        buf += "},";
    }
    buf.pop_back();   // the last comma (or the blank after a name with no arguments)
    if (host_.list_line) host_.list_line(LIST_MACRO, -1, buf);
}

// If the line is a multi-line macro call (maybe after a label), set up its expansion: the macro's lines are pushed onto
// the input. Returns 1 if it was a call.
int Preproc::expand_mmacro(Tok* tline) {
    Tok* startline = tline;
    Tok* label = nullptr;
    bool dont_prepend = false;
    std::vector<Tok*> params;
    Tok *t, *tt;
    MMacro* m;
    Line *l, *ll;
    int i;
    std::vector<int> paramlen;
    std::string mname;
    int nparam = 0;

    t = tline;
    t = skip_white(t);
    if (!tok_macro_id(t)) return 0;   // Probably a label

    m = is_mmacro(t, &nparam, &params);
    if (m) {
        mname = t->text;
    } else {
        Tok* last;
        // Try a label first: "label: macro" or "label macro"
        label = last = t;
        t = t->next;
        if (tok_white(t)) last = t, t = t->next;
        if (tok_is(t, ':')) {
            dont_prepend = true;
            last = t, t = t->next;
            if (tok_white(t)) last = t, t = t->next;
        }
        m = is_mmacro(t, &nparam, &params);
        if (!m) return 0;
        last->next = nullptr;
        mname = t->text;
        tline = t;
    }

    // Check the limits
    if (mmacro_deadman_.total >= as_.nasm_limit[LIMIT_MMACROS] || mmacro_deadman_.levels >= as_.nasm_limit[LIMIT_MACRO_LEVELS]) {
        if (!mmacro_deadman_.triggered) {
            as_.nonfatal("interminable multiline macro recursion");
            mmacro_deadman_.triggered = true;
        }
        return 0;
    }
    mmacro_deadman_.total++;
    mmacro_deadman_.levels++;

    // The lengths of the parameters (they point into the call line, so their ends are marked by a length)
    paramlen.assign(static_cast<std::size_t>(nparam) + 1, 0);
    for (i = 1; i < nparam + 1 && (t = params[static_cast<std::size_t>(i)]); i++) {
        bool braced = false;
        int brace = 0;
        int white = 0;
        bool comma = !m->plus || i < nparam;
        t = skip_white(t);
        if (tok_is(t, '{')) {
            t = t->next;
            brace = 1;
            braced = true;
            comma = false;
        }
        params[static_cast<std::size_t>(i)] = t;
        for (; t; t = t->next) {
            if (tok_white(t)) {
                white++;
                continue;
            }
            switch (t->type) {
                case ',':
                    if (comma && !brace) goto endparam;
                    break;
                case '{': brace++; break;
                case '}':
                    brace--;
                    if (braced && !brace) {
                        paramlen[static_cast<std::size_t>(i)] += white;
                        goto endparam;
                    }
                    break;
                default: break;
            }
            paramlen[static_cast<std::size_t>(i)] += white + 1;
            white = 0;
        }
    endparam:;
    }

    // The line that marks the end of the expansion (it pops the macro)
    ll = new_line();
    ll->next = istk_->expansion;
    ll->finishes = get_mmacro(m);
    ll->where = istk_->where;
    istk_->expansion = ll;

    // Fill in the state of this invocation
    m->in_progress++;
    m->params = std::move(params);
    m->iline = tline;
    m->iname = mname;
    m->nparam = static_cast<unsigned>(nparam);
    m->rotate = 0;
    m->paramlen = std::move(paramlen);
    m->unique = unique_++;
    m->condcnt = 0;
    m->mstk = istk_->mstk;   // the references move to the macro, and the stack takes new ones
    istk_->mstk.mstk = get_mmacro(m);
    istk_->mstk.mmac = get_mmacro(m);

    // Push the macro's lines (in reverse, so the first is read first)
    for (l = m->expansion; l; l = l->next) {
        ll = new_line();
        ll->next = istk_->expansion;
        istk_->expansion = ll;
        ll->first = dup_tlist(l->first, nullptr);
        ll->where = l->where;
    }

    // The label before the call: kept as %00 if the macro wants it, else emitted as a line of its own
    if (label) {
        if (m->capture_label) {
            m->params[0] = dup_token(nullptr, label);
            m->paramlen[0] = 1;
            delete_tlist(startline);
        } else {
            ll = new_line();
            ll->finishes = nullptr;
            ll->next = istk_->expansion;
            istk_->expansion = ll;
            ll->first = startline;
            ll->where = istk_->where;
            if (!dont_prepend) {
                while (label->next) label = label->next;
                label->next = tt = make_tok_char(nullptr, ':');
            }
        }
    }

    istk_->nolist += !!(m->nolist & NL_LIST);
    istk_->noline += !!(m->nolist & NL_LINE);

    if (!istk_->nolist) {
        if (host_.list_option && host_.list_option('m')) list_mmacro_call(m);
        if (host_.list_uplevel) host_.list_uplevel(LIST_MACRO);
    }
    if (!istk_->noline) as_.src.macro_push(get_mmacro(m), istk_->where);

    return 1;
}

}  // namespace cb::build::x86asm
