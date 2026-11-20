// build/x86asm/preproc_directive.cpp -- the preprocessor directives: %if and its family (if_condition), %define and
// friends, %macro / %rep, %include, %push / %pop, %line, %arg / %local ... (preproc.c: do_directive and the helpers it uses).
#include <climits>

#include "build/x86asm/preproc.hpp"

namespace cb::build::x86asm {

using namespace ppt;
using namespace pp;

// ---- directive names ----

int Preproc::pp_get_nasm_directive(std::string_view dname) const { return pp_token_hash(dname); }

int Preproc::pp_get_tasm_directive(std::string_view dname) const {
    if (!(ppopt_ & PP_TASM)) return PP_invalid;
    return pp_tasm_token_hash(dname);
}

int Preproc::pp_get_directive(std::string_view dname) const {
    if (!dname.empty() && dname[0] == '%') return pp_get_nasm_directive(dname);
    return pp_get_tasm_directive(dname);
}

// Is the text a preprocessor or assembler directive? (for %ifdirective)
bool Preproc::is_directive(std::string_view dname_in) {
    const std::string copy(dname_in);
    const char* dname = skip_blanks(copy.c_str());
    if (*dname == '[') {
        dname = skip_blanks(dname + 1);
    } else {
        if (*dname == '%') return pp_get_nasm_directive(dname) != PP_invalid;
        if (pp_get_tasm_directive(dname) != PP_invalid) return true;
    }
    const char* q = skip_word(dname);
    const std::string p(dname, static_cast<std::size_t>(q - dname));
    return host_.directive_valid ? host_.directive_valid(p) : false;
}

// ---- small parsing helpers ----

// A boolean option value: yes/no, true/false, on/off, or a constant expression.
bool Preproc::pp_get_boolean_option(Tok* tline, bool defval) {
    static const char* const noyes[] = {"no", "yes", "false", "true", "off", "on"};
    tline = skip_white(tline);
    if (!tline) return true;
    if (tline->type == ID) {
        for (std::size_t i = 0; i < 6; i++)
            if (ieq(tline->text, noyes[i])) return i & 1;
    }
    PpScan pps;
    pps.tptr = tline;
    pps.ntokens = -1;
    TokenVal tokval;
    std::optional<Expr> evalresult = evaluate_tokens(pps, tokval);
    if (!evalresult) return true;
    if (tokval.type != TokenType::EOS) as_.warn(WARN_PP_TRAILING, "trailing garbage after expression ignored");
    if (!pp_is_really_simple(*evalresult)) {
        as_.nonfatal("boolean flag expression must be a constant");
        return defval;
    }
    return pp_reloc_value(*evalresult) != 0;
}

int Preproc::parse_size(std::string_view str) {
    TokenVal tv;
    if (!host_.token_hash) return 0;
    if (host_.token_hash(str, tv) != TokenType::SIZE) return 0;
    return static_cast<int>(tv.inttwo);
}

void Preproc::do_pragma_preproc(Tok* tline) {
    tline = tline->next;
    tline = skip_white(tline);
    if (!tok_is(tline, ID)) return;
    if (ieq(tline->text, "sane_empty_expansion")) {
        tline = skip_white(tline->next);
        ppconf_.sane_empty_expansion = pp_get_boolean_option(tline, ppconf_.sane_empty_expansion);
    }
    // Unknown preprocessor pragmas are silently ignored
}

// The macro identifier after a directive (expanded if it was made by macros); `ok` says whether there was one.
std::string_view Preproc::get_id(Tok** tp, std::string_view dname, bool& ok) {
    *tp = (*tp)->next;
    return get_id_noskip(tp, dname, ok);
}

std::string_view Preproc::get_id_noskip(Tok** tp, std::string_view dname, bool& ok) {
    Tok* t = *tp;
    t = skip_white(t);
    t = expand_id(t);
    if (!tok_macro_id(t)) {
        as_.nonfatal("`{}' expects a macro identifier", dname);
        ok = false;
        return {};
    }
    *tp = t;
    ok = true;
    return t->text;
}

const UsePackage* Preproc::get_use_pkg(Tok* t, std::string_view dname, std::optional<std::string>& name) {
    t = skip_white(t);
    t = expand_smacro(t);
    name.reset();
    if (!t) {
        as_.nonfatal("`{}' expects a package name, got end of line", dname);
        return nullptr;
    } else if (t->type != ID && t->type != STR) {
        as_.nonfatal("`{}' expects a package name, got `{}'", dname, t->text);
        return nullptr;
    }
    name = std::string(unquote_token(t));
    t = t->next;
    t = skip_white(t);
    if (t) as_.warn(WARN_PP_TRAILING, "trailing garbage after `{}' ignored", dname);
    for (const UsePackage& p : use_packages())
        if (ieq(p.name, *name)) return &p;
    return nullptr;
}

// ---- file names in directives ----

// The file name of %include and friends: the expanded string, unquoted, as a new token (the original string token in *otp).
Tok* Preproc::tlist_filename(Tok** tp, Tok** otp, std::string_view dname) {
    Tok* t;
    *tp = t = expand_smacro_noreset(*tp);
    t = skip_white(t);
    if (!tok_string(t)) {
        if (otp) *otp = nullptr;
        as_.nonfatal("`{}' expects a file name", dname);
        return nullptr;
    }
    if (skip_white(t->next)) as_.warn(WARN_PP_TRAILING, "trailing garbage after `{}' ignored", dname);
    if (otp) *otp = t;
    t = dup_token(nullptr, t);
    unquote_token_cstr(t);
    return t;
}

Tok* Preproc::pp_do_pathsearch(Tok** tp, std::string_view dname) {
    Tok *t, *ot;
    t = tlist_filename(tp, &ot, dname);
    if (!t) return nullptr;
    FileHashEntry* fhe = nullptr;
    inc_open(t->text, nullptr, &fhe, INC_PROBE, nullptr);
    if (fhe) {
        do_delete_token(t);
        return make_tok_qstr(nullptr, fhe->path);
    }
    return steal_token(t, ot);
}

Tok* Preproc::pp_do_depend(Tok** tp, std::string_view dname) {
    Tok *t, *ot;
    t = tlist_filename(tp, &ot, dname);
    if (!t) return nullptr;
    if (deplist_) deplist_->push_back(t->text);
    return steal_token(t, ot);
}

// ---- %if ----

CondState Preproc::if_condition(Tok* tline, int ct, std::string_view dname) {
    bool j;
    Tok *t, *tt, *origline;
    int needtype;
    bool casesense = true;
    const int cond = pp_cond(ct);

    origline = tline;

    switch (cond) {
        case PP_IFCTX:
            j = false;
            while (true) {
                tline = skip_white(tline);
                if (!tline) break;
                if (tline->type != ID) {
                    as_.nonfatal("`{}' expects context identifiers", dname);
                    goto fail;
                }
                if (cstk_ && cstk_->name && ieq(tline->text, *cstk_->name)) j = true;
                tline = tline->next;
            }
            break;

        case PP_IFDEF:
        case PP_IFDEFALIAS: {
            const bool alias = cond == PP_IFDEFALIAS;
            SMacro* smac = nullptr;
            j = false;
            while (tline) {
                tline = skip_white(tline);
                if (!tok_macro_or_func_id(tline)) {
                    as_.nonfatal("`{}' expects macro identifiers", dname);
                    goto fail;
                }
                std::string_view mname = tline->text;
                Context* ctx = get_ctx(mname, &mname);
                if (smacro_defined(ctx, mname, -1, &smac, true, alias) && smac && smac->alias == alias) {
                    j = true;
                    break;
                }
                tline = tline->next;
            }
            break;
        }

        case PP_IFDIFI:
            // NASM has no %ifdifi (it is accepted as a name, but not implemented)
            j = false;
            goto fail;

        case PP_IFDIRECTIVE:
            tline = skip_white(expand_smacro(tline));
            j = false;
            if (tline) j = is_directive(unquote_token(tline));
            break;

        case PP_IFENV:
            tline = expand_smacro(tline);
            j = false;
            while (tline) {
                tline = skip_white(tline);
                if (!tline || (tline->type != ID && tline->type != STR && tline->type != INTERNAL_STR && tline->type != ENVIRON)) {
                    as_.nonfatal("`{}' expects environment variable names", dname);
                    goto fail;
                }
                j |= pp_getenv(tline, false).has_value();
                tline = tline->next;
            }
            break;

        case PP_IFFILE: {
            FileHashEntry* fhe = nullptr;
            t = tlist_filename(&origline, nullptr, dname);
            if (!t) goto fail;
            inc_open(t->text, nullptr, &fhe, INC_PROBE | INC_EXACT, nullptr);
            j = fhe && (fhe->omode & INC_EXACT);
            do_delete_token(t);
            break;
        }

        case PP_IFIDNI:
            casesense = false;
            [[fallthrough]];
        case PP_IFIDN:
            tline = expand_smacro(tline);
            t = tt = tline;
            while (tok_isnt(tt, ',')) tt = tt->next;
            if (!tt) {
                as_.nonfatal("`{}' expects two comma-separated arguments", dname);
                goto fail;
            }
            tt = tt->next;
            j = true;   // assume equality unless proved otherwise
            while (tok_isnt(t, ',') && tt) {
                if (tok_is(tt, ',')) {
                    as_.nonfatal("`{}': more than one comma on line", dname);
                    goto fail;
                }
                if (t->type == WHITESPACE) {
                    t = t->next;
                    continue;
                }
                if (tt->type == WHITESPACE) {
                    tt = tt->next;
                    continue;
                }
                if (tt->type != t->type) {
                    j = false;   // token types differ
                    break;
                }
                unquote_token(t);
                unquote_token(tt);
                if (t->text.size() != tt->text.size() || !(casesense ? t->text == tt->text : ieq(t->text, tt->text))) {
                    j = false;
                    break;
                }
                t = t->next;
                tt = tt->next;
            }
            if (!tok_is(t, ',') || tt) j = false;   // trailing white space is not allowed; unequal lengths differ
            break;

        case PP_IFMACRO: {
            bool found = false;
            MMacro searching;
            tline = skip_white(tline);
            tline = expand_id(tline);
            if (!tok_is(tline, ID)) {
                as_.nonfatal("`{}' expects a macro name", dname);
                goto fail;
            }
            searching.name = tline->text;
            searching.casesense = true;
            searching.nparam_min = 0;
            searching.nparam_max = INT_MAX;
            tline = expand_smacro(tline->next);
            tline = skip_white(tline);
            if (!tline) {
                // no parameter count: any
            } else if (!tok_is(tline, NUM)) {
                as_.nonfatal("`{}' expects a parameter count or nothing", dname);
            } else {
                searching.nparam_min = searching.nparam_max = read_param_count(tline->text);
            }
            if (tline && tok_is(tline->next, '-')) {
                tline = tline->next->next;
                if (tok_is(tline, '*')) {
                    searching.nparam_max = INT_MAX;
                } else if (!tok_is(tline, NUM)) {
                    as_.nonfatal("`{}' expects a parameter count after `-'", dname);
                } else {
                    searching.nparam_max = read_param_count(tline->text);
                    if (searching.nparam_min > searching.nparam_max) {
                        as_.nonfatal("minimum parameter count exceeds maximum");
                        searching.nparam_max = searching.nparam_min;
                    }
                }
            }
            if (tline && tok_is(tline->next, '+')) {
                tline = tline->next;
                searching.plus = true;
            }
            {
                auto it = mmacros_.find(lcase(searching.name));
                MMacro* mmac = it == mmacros_.end() ? nullptr : it->second;
                while (mmac) {
                    if (mmac->name == searching.name && (mmac->nparam_min <= searching.nparam_max || searching.plus) &&
                        (searching.nparam_min <= mmac->nparam_max || mmac->plus)) {
                        found = true;
                        break;
                    }
                    mmac = mmac->next;
                }
            }
            if (tline && tline->next) as_.warn(WARN_PP_TRAILING, "trailing garbage after `{}' ignored", dname);
            j = found;
            break;
        }

        case PP_IFID:
            needtype = ID;
            goto iftype;
        case PP_IFNUM:
            needtype = NUM;
            goto iftype;
        case PP_IFSTR:
            needtype = STR;
        iftype:
            t = tline = expand_smacro(tline);
            while (tok_white(t) || (needtype == NUM && (tok_is(t, '-') || tok_is(t, '+')))) t = t->next;
            j = tok_is(t, needtype);
            break;

        case PP_IFTOKEN:
            tline = expand_smacro(tline);
            t = skip_white(tline);
            j = false;
            if (t) {
                t = skip_white(t->next);   // Skip the token itself
                j = !t;                    // Should be nothing left
            }
            break;

        case PP_IFEMPTY:
            tline = expand_smacro(tline);
            t = skip_white(tline);
            j = !t;   // Should be empty
            break;

        case PP_IF: {
            PpScan pps;
            TokenVal tokval;
            pps.tptr = tline = expand_smacro(tline);
            pps.ntokens = -1;
            as_.masm_if_undef_zero = as_.masm_mode;
            std::optional<Expr> evalresult = evaluate_tokens(pps, tokval);
            as_.masm_if_undef_zero = false;
            if (!evalresult) return CondState::EVAL_ERROR;
            if (tokval.type != TokenType::EOS) as_.warn(WARN_PP_TRAILING, "trailing garbage after expression ignored");
            if (!pp_is_simple(*evalresult)) {
                as_.nonfatal("non-constant value given to `{}'", dname);
                goto fail;
            }
            j = pp_reloc_value(*evalresult) != 0;
            break;
        }

        case PP_IFUSING:
        case PP_IFUSABLE: {
            std::optional<std::string> name;
            const UsePackage* pkg = get_use_pkg(tline, dname, name);
            if (!name) goto fail;
            j = pkg && ((cond == PP_IFUSABLE) | use_loaded_[static_cast<std::size_t>(pkg->index)]);
            break;
        }

        default:
            as_.nonfatal("unknown preprocessor directive `{}'", dname);
            goto fail;
    }

    delete_tlist(origline);
    return (j ^ pp_cond_negative(ct)) ? CondState::IF_TRUE : CondState::IF_FALSE;

fail:
    delete_tlist(origline);
    return CondState::NEVER;
}

// ---- %line and # line ----

int Preproc::line_directive(Tok* origline, Tok* tline) {
    int k, m;
    bool err = false;
    std::string dname;

    if ((ppopt_ & PP_NOLINE) || istk_->mstk.mstk) goto done;   // Ignore, or inside a macro: nothing to do

    dname = tline->text;
    tline = tline->next;
    tline = skip_white(tline);
    if (!tok_is(tline, NUM)) {
        as_.nonfatal("`{}' expects a line number", dname);
        goto done;
    }
    k = static_cast<int>(as_.hooks.readnum(tline->text, err));
    m = 1;
    tline = tline->next;
    if (tok_is(tline, '+') || tok_is(tline, '-')) {
        const bool minus = tok_is(tline, '-');
        tline = tline->next;
        if (!tok_is(tline, NUM)) {
            as_.nonfatal("`{}' expects a line increment", dname);
            goto done;
        }
        m = static_cast<int>(as_.hooks.readnum(tline->text, err));
        if (minus) m = -m;
        tline = tline->next;
    }
    tline = skip_white(tline);
    if (tline) {
        if (tline->type == STR) {
            const std::string fname = unquote_token_anystr(tline, BADCTL, dname[0] == '#' ? STR_C : STR_NASM);
            as_.src.set_fname(fname);
        } else {
            const std::string fname = detoken(tline, false);
            as_.src.set_fname(fname);
        }
    }
    as_.src.set_linnum(k);
    istk_->where = as_.src.where();
    istk_->lineinc = m;

done:
    delete_tlist(origline);
    return DIRECTIVE_FOUND;
}

// ---- string directives ----

Tok* Preproc::pp_strcat(Tok* tline, std::string_view dname) {
    Tok* res = nullptr;
    for (Tok* t = tline; t; t = t->next) {
        switch (t->type) {
            case WHITESPACE:
            case ',': break;
            case STR: unquote_token(t); [[fallthrough]];
            case INTERNAL_STR: break;
            default:
                as_.nonfatal("non-string passed to `{}': {}", dname, t->text);
                goto err;
        }
    }
    {
        std::string buf;
        for (Tok* t = tline; t; t = t->next)
            if (t->type == INTERNAL_STR) buf += t->text;
        res = make_tok_qstr(nullptr, buf);
    }
err:
    delete_tlist(tline);
    return res;
}

Tok* Preproc::pp_substr(Tok* tline, std::string_view dname) {
    std::int64_t start, count;
    PpScan pps;
    Tok* t;
    Tok* res = nullptr;
    std::optional<Expr> evalresult;
    TokenVal tokval;

    t = skip_white(tline);
    if (!tok_is(t, STR)) {
        as_.nonfatal("`{}' requires a string as parameter", dname);
        goto err;
    }

    pps.tptr = skip_white(t->next);
    if (tok_is(pps.tptr, ',')) pps.tptr = skip_white(pps.tptr->next);
    if (!pps.tptr) {
        as_.nonfatal("`{}' requires a starting index", dname);
        goto err;
    }

    pps.ntokens = -1;
    evalresult = evaluate_tokens(pps, tokval);
    if (!evalresult) {
        goto err;
    } else if (!pp_is_simple(*evalresult)) {
        as_.nonfatal("non-constant value given to `{}'", dname);
        goto err;
    }
    start = pp_first_value(*evalresult);

    pps.tptr = skip_white(pps.tptr);
    if (!pps.tptr) {
        count = 1;   // Backwards compatibility: one character
    } else {
        evalresult = evaluate_tokens(pps, tokval);
        if (!evalresult) {
            goto err;
        } else if (!pp_is_simple(*evalresult)) {
            as_.nonfatal("non-constant value given to `{}'", dname);
            goto err;
        }
        count = pp_first_value(*evalresult);
    }
    res = pp_substr_common(t, start, count);

err:
    delete_tlist(tline);
    return res;
}

// The part of string token `t` a (1-based) start and count select, with *countp set to its length. A negative count
// means "that many from the end"; out of range gives the empty string.
std::string_view Preproc::pp_get_substr(Tok* t, std::int64_t start, std::int64_t* countp) {
    std::int64_t count = *countp;
    unquote_token(t);
    const std::int64_t len = static_cast<std::int64_t>(t->text.size());
    start -= 1;   // 1-based
    if (start < 0) start = 0;
    if (count < 0) count = len + count + 1 - start;
    if (start + count > len) count = len - start;
    if (!len || count < 0 || start >= len) start = -1;
    if (start < 0) {
        *countp = 0;
        return std::string_view();
    }
    *countp = count;
    return std::string_view(t->text).substr(static_cast<std::size_t>(start), static_cast<std::size_t>(count));
}

Tok* Preproc::pp_substr_common(Tok* t, std::int64_t start, std::int64_t count) {
    const std::string_view txt = pp_get_substr(t, start, &count);
    return make_tok_qstr(nullptr, txt);
}

// %exitmacro and %exitrep: suppress the remaining lines up to the end of the nearest block; the block is returned.
MMacro* Preproc::do_exit_macro(std::string_view dname, bool named) {
    Line* l;
    for (l = istk_->expansion; l; l = l->next)
        if (l->finishes && (l->finishes->has_name == named)) break;
    if (l) {
        for (Line* ll = istk_->expansion; ll != l; ll = ll->next) ll->suppressed = true;
        return l->finishes;
    }
    as_.nonfatal("`{}' not within `%{}' block", dname, dname.substr(5));
    return nullptr;
}

// %error %warning %fatal %note
void Preproc::user_error(int op, Tok** tlinep) {
    Tok *tline, *t;
    std::string text;
    ErrFlags severity;

    switch (op) {
        case PP_NOTE: severity = ERR_NOTE; break;
        case PP_WARNING: severity = ERR_WARNING | WARN_USER | ERR_PASS2; break;
        case PP_ERROR: severity = ERR_NONFATAL | ERR_PASS2; break;
        case PP_FATAL: severity = ERR_FATAL; break;
        default: as_.panic_here();
    }

    *tlinep = tline = expand_smacro(skip_white(*tlinep));
    t = skip_tok_white(tline);
    if (tok_is(tline, STR) && !t) {
        text = unquote_token(tline);   // a single string: its contents
    } else {
        text = detoken(tline, false);
    }
    const char* q = skip_blanks(text.c_str());
    if (!*q) q = pp_directive_name(op);
    as_.report(severity, q);
}

// ---- the directive dispatcher ----

// If the line (token list) is a preprocessor directive, run it and return DIRECTIVE_FOUND; else NO_DIRECTIVE_FOUND and the
// line is left alone. *output is a line to hand on to the assembler (a %pragma), or null.
int Preproc::do_directive(Tok* tline, Tok** output, bool suppressed) {
    int op;
    int j;
    int nolist;
    bool casesense;
    int offset;
    std::string_view mname;
    Context* ctx;
    MMacro *mmac, **mmhead;
    Tok *t = nullptr, *tt, *macro_start, *origline;
    PpScan pps;
    TokenVal tokval;
    std::optional<Expr> evalresult;
    std::int64_t count;
    std::string dname;
    bool idok;

    *output = nullptr;
    origline = tline;

    if (tok_is(tline, '#')) return line_directive(origline, tline);

    tline = skip_white(tline);
    if (!tline) return NO_DIRECTIVE_FOUND;

    dname = tline->text;
    switch (tline->type) {
        case PREPROC_ID:
            op = pp_get_nasm_directive(dname);
            if (op == PP_invalid) {
                // Not a directive, but a bogus %if... or %elif... must still be counted so %endif can match
                if (!strnicmp_(dname, "%if", 3)) op = PP_IF_BOGUS;
                else if (!strnicmp_(dname, "%elif", 5)) op = PP_ELIF_BOGUS;
            }
            break;
        case ID:
            op = pp_get_tasm_directive(tline->text);
            break;
        default:
            op = PP_invalid;
            break;
    }

    switch (op) {
        case PP_invalid: return NO_DIRECTIVE_FOUND;
        case PP_LINE:
            // %line directives are always processed immediately and never passed on
            return line_directive(origline, tline);
        default: break;
    }

    if (ppopt_ & PP_TRIVIAL) goto done;   // Only %line and # directives are processed

    casesense = true;
    if (pp_has_case(op) & pp_insensitive(op)) {
        casesense = false;
        op--;
    }

    // If we're in a non-emitting branch of a condition, only conditionals are processed
    if (suppressed && !is_condition(op)) return NO_DIRECTIVE_FOUND;

    // If we're defining a macro or reading a %rep block, only the directives that nest or end it are processed
    if (defining_ && op != PP_MACRO && op != PP_RMACRO && op != PP_ENDMACRO && op != PP_ENDM &&
        (defining_->has_name || (op != PP_ENDREP && op != PP_REP)))
        return NO_DIRECTIVE_FOUND;

    if (defining_) {
        if (op == PP_MACRO || op == PP_RMACRO) {
            nested_mac_count_++;
            return NO_DIRECTIVE_FOUND;
        } else if (nested_mac_count_ > 0) {
            if (op == PP_ENDMACRO) {
                nested_mac_count_--;
                return NO_DIRECTIVE_FOUND;
            }
        }
        if (!defining_->has_name) {
            if (op == PP_REP) {
                nested_rep_count_++;
                return NO_DIRECTIVE_FOUND;
            } else if (nested_rep_count_ > 0) {
                if (op == PP_ENDREP) {
                    nested_rep_count_--;
                    return NO_DIRECTIVE_FOUND;
                }
            }
        }
    }

    // A directive name that is also a function ("%strlen(...)") is not a directive here
    if (op >= 0 && op < PP_count && pp_op_may_be_function_[static_cast<std::size_t>(op)]) {
        if (tok_is(skip_white(tline->next), '(')) return NO_DIRECTIVE_FOUND;
    }

    switch (op) {
        default:
            as_.nonfatal("unknown preprocessor directive `{}'", dname);
            return NO_DIRECTIVE_FOUND;   // didn't get it

        case PP_PRAGMA:
            // %pragma namespace options...: the preproc namespace is ours; the rest becomes a [pragma ...] line
            t = tline;
            tline = tline->next;
            t->next = nullptr;
            tline = zap_white(expand_smacro(tline));
            if (tok_is(tline, ID)) {
                if (ieq(tline->text, "preproc")) {
                    do_pragma_preproc(tline);
                    delete_tlist(tline);
                } else {
                    for (t = tline; t->next; t = t->next) {}
                    t->next = make_tok_char(nullptr, ']');
                    t = new_white(tline);
                    t = new_token(t, ID, "pragma");
                    t = make_tok_char(t, '[');
                    tline = t;
                    *output = tline;
                }
            }
            break;

        case PP_STACKSIZE: {
            tline = skip_white(tline->next);
            if (!tline || tline->type != ID) {
                as_.nonfatal("`{}' missing size parameter", dname);
                break;
            }
            const std::string& arg = tline->text;
            if (ieq(arg, "flat")) {
                stack_size_ = 4;
                stack_pointer_ = "ebp";
                arg_offset_ = 8;
                local_offset_ = 0;
            } else if (ieq(arg, "flat64")) {
                stack_size_ = 8;
                stack_pointer_ = "rbp";
                arg_offset_ = 16;
                local_offset_ = 0;
            } else if (ieq(arg, "large")) {
                stack_size_ = 2;
                stack_pointer_ = "bp";
                arg_offset_ = 4;
                local_offset_ = 0;
            } else if (ieq(arg, "small")) {
                stack_size_ = 2;
                stack_pointer_ = "bp";
                arg_offset_ = 6;
                local_offset_ = 0;
            } else {
                as_.nonfatal("`{}' invalid size type", dname);
            }
            break;
        }

        case PP_ARG:
            // TASM-style %arg name:size, ...
            offset = arg_offset_;
            do {
                int size = stack_size_;
                tline = skip_white(tline->next);
                if (!tline || tline->type != ID) {
                    as_.nonfatal("`{}' missing argument parameter", dname);
                    goto done;
                }
                const std::string arg = tline->text;
                tline = tline->next;
                if (!tok_is(tline, ':')) {
                    as_.nonfatal("syntax error processing `{}' directive", dname);
                    goto done;
                }
                tline = tline->next;
                if (!tok_is(tline, ID)) {
                    as_.nonfatal("`{}' missing size type parameter", dname);
                    goto done;
                }
                tt = tokenize(tline->text);
                tt = expand_smacro(tt);
                size = parse_size(tt->text);
                delete_tlist(tt);
                if (!size) {
                    as_.nonfatal("invalid size type for `{}' missing directive", dname);
                    goto done;
                }
                size = (size + stack_size_ - 1) & ~(stack_size_ - 1);   // ALIGN(size, StackSize)
                define_stack_smacro(arg, offset);
                offset += size;
                tline = skip_white(tline->next);
            } while (tok_is(tline, ','));
            arg_offset_ = offset;
            break;

        case PP_LOCAL: {
            int total_size = 0;
            offset = local_offset_;
            do {
                int size = stack_size_;
                tline = skip_white(tline->next);
                if (!tline || tline->type != ID) {
                    as_.nonfatal("`{}' missing argument parameter", dname);
                    goto done;
                }
                const std::string local = tline->text;
                tline = tline->next;
                if (!tok_is(tline, ':')) {
                    as_.nonfatal("syntax error processing `{}' directive", dname);
                    goto done;
                }
                tline = tline->next;
                if (!tok_is(tline, ID)) {
                    as_.nonfatal("`{}' missing size type parameter", dname);
                    goto done;
                }
                tt = tokenize(tline->text);
                tt = expand_smacro(tt);
                size = parse_size(tt->text);
                delete_tlist(tt);
                if (!size) {
                    as_.nonfatal("invalid size type for `{}' missing directive", dname);
                    goto done;
                }
                size = (size + stack_size_ - 1) & ~(stack_size_ - 1);
                offset += size;
                define_stack_smacro(local, -offset);
                total_size += size;
                tline = skip_white(tline->next);
            } while (tok_is(tline, ','));
            tt = make_tok_num(nullptr, total_size);
            tt = make_tok_char(tt, '+');
            tt = new_token(tt, LOCAL_MACRO, "%$localsize");
            assign_smacro("%$localsize", true, tt, dname);
            local_offset_ = offset;
            break;
        }

        case PP_CLEAR: {
            bool context = false;
            t = tline->next = expand_smacro(tline->next);
            t = skip_white(t);
            if (!t) {
                // Absolutely nothing to do: clear all
                do_clear(static_cast<ClearWhat>(CLEAR_DEFINE | CLEAR_MMACRO), false);
            } else {
                while (tok_is(t, ID)) {
                    const std::string txt = t->text;
                    while ((t = skip_white(t->next)) && tok_is(t, ',')) {}
                    if (ieq(txt, "all")) {
                        do_clear(CLEAR_ALL, context);
                    } else if (ieq(txt, "define") || ieq(txt, "def") || ieq(txt, "smacro")) {
                        do_clear(CLEAR_DEFINE, context);
                    } else if (ieq(txt, "defalias") || ieq(txt, "alias") || ieq(txt, "salias")) {
                        do_clear(CLEAR_DEFALIAS, context);
                    } else if (ieq(txt, "alldef") || ieq(txt, "alldefine")) {
                        do_clear(CLEAR_ALLDEFINE, context);
                    } else if (ieq(txt, "macro") || ieq(txt, "mmacro")) {
                        do_clear(CLEAR_MMACRO, context);
                    } else if (ieq(txt, "context") || ieq(txt, "ctx")) {
                        context = true;
                    } else if (ieq(txt, "global")) {
                        context = false;
                    } else if (ieq(txt, "nothing") || ieq(txt, "none") || ieq(txt, "ignore") || ieq(txt, "-") || ieq(txt, "--")) {
                        // do nothing
                    } else {
                        as_.nonfatal("invalid option to {}: {}", dname, txt);
                        t = nullptr;
                        break;
                    }
                }
            }
            t = skip_white(t);
            if (t) as_.warn(WARN_PP_TRAILING, "trailing garbage after `{}' ignored", dname);
            break;
        }

        case PP_DEPEND:
            t = pp_do_depend(&tline->next, dname);
            do_delete_token(t);
            goto done;

        case PP_INCLUDE:
        case PP_REQUIRE: {
            t = tlist_filename(&tline->next, nullptr, dname);
            if (!t) goto done;

            const std::string p = t->text;
            FileHashEntry* fhe = nullptr;
            std::string text;
            const bool opened = inc_open(p, deplist_, &fhe,
                                         (pp_mode_ == PP_DEPS) ? INC_OPTIONAL : (op == PP_REQUIRE) ? INC_REQUIRED : INC_NEEDED, &text);
            if (opened) {
                Include* inc = new Include();
                inc->next = istk_;
                inc->is_file = true;
                inc->data = std::move(text);
                inc->nolist = istk_->nolist;
                inc->noline = istk_->noline;
                inc->where = istk_->where;
                inc->lineinc = 0;
                istk_ = inc;
                if (as_.masm_mode && masm_) masm_prescan(istk_->data);
                if (!istk_->noline) {
                    as_.src.set(0, fhe ? fhe->path : p);
                    istk_->where = as_.src.where();
                    istk_->lineinc = 1;
                }
                if (!istk_->nolist && host_.list_uplevel) host_.list_uplevel(LIST_INCLUDE);
            }
            do_delete_token(t);
            break;
        }

        case PP_USE: {
            std::optional<std::string> name;
            const UsePackage* pkg = get_use_pkg(tline->next, dname, name);
            if (!name) goto done;
            if (!pkg) {
                as_.nonfatal("unknown `{}' package: `{}'", dname, *name);
            } else if (!use_loaded_[static_cast<std::size_t>(pkg->index)]) {
                use_loaded_[static_cast<std::size_t>(pkg->index)] = true;
                pp_start_stdmac();
                pp_add_stdmac(pkg->macros);
            }
            break;
        }

        case PP_PUSH:
        case PP_REPL:
        case PP_POP: {
            std::optional<std::string> p;
            tline = tline->next;
            tline = skip_white(tline);
            tline = expand_id(tline);
            if (tline) {
                if (!tok_is(tline, ID)) {
                    as_.nonfatal("`{}' expects a context identifier", dname);
                    goto done;
                }
                if (skip_white(tline->next)) as_.warn(WARN_PP_TRAILING, "trailing garbage after `{}' ignored", dname);
                p = tline->text;
            }
            if (op == PP_PUSH) {
                ctx = new Context();
                ctx->depth = cstk_ ? cstk_->depth + 1 : 1;
                ctx->next = cstk_;
                ctx->name = p;
                ctx->number = unique_++;
                cstk_ = ctx;
            } else {
                // %pop or %repl
                if (!cstk_) {
                    as_.nonfatal("`{}': context stack is empty", dname);
                } else if (op == PP_POP) {
                    if (p && (!cstk_->name || !ieq(*p, *cstk_->name)))
                        as_.nonfatal("`{}' in wrong context: {}, expected {}", dname, cstk_->name ? *cstk_->name : std::string("anonymous"), *p);
                    else
                        ctx_pop();
                } else {
                    cstk_->name = p;
                }
            }
            break;
        }

        case PP_FATAL:
        case PP_ERROR:
        case PP_WARNING:
        case PP_NOTE: user_error(op, &tline->next); break;

        case PP_IF: case PP_IF_BOGUS: case PP_IFCTX: case PP_IFDEF: case PP_IFDEFALIAS: case PP_IFDIFI: case PP_IFDIRECTIVE:
        case PP_IFEMPTY: case PP_IFENV: case PP_IFFILE: case PP_IFID: case PP_IFIDN: case PP_IFIDNI: case PP_IFMACRO:
        case PP_IFNUM: case PP_IFSTR: case PP_IFTOKEN: case PP_IFUSABLE: case PP_IFUSING:
        case PP_IFN: case PP_IFN_BOGUS: case PP_IFNCTX: case PP_IFNDEF: case PP_IFNDEFALIAS: case PP_IFNDIFI:
        case PP_IFNDIRECTIVE: case PP_IFNEMPTY: case PP_IFNENV: case PP_IFNFILE: case PP_IFNID: case PP_IFNIDN:
        case PP_IFNIDNI: case PP_IFNMACRO: case PP_IFNNUM: case PP_IFNSTR: case PP_IFNTOKEN: case PP_IFNUSABLE:
        case PP_IFNUSING: {
            CondState state;
            if (suppressed || (!istk_->conds.empty() && !emitting(istk_->conds.back()))) {
                // We're inside a skipped part: the %if's own condition is not evaluated
                state = CondState::NEVER;
            } else {
                state = if_condition(tline->next, op, dname);
                tline->next = nullptr;
            }
            istk_->conds.push_back(state);
            if (istk_->mstk.mstk) istk_->mstk.mstk->condcnt++;
            break;
        }

        case PP_ELIF: case PP_ELIF_BOGUS: case PP_ELIFCTX: case PP_ELIFDEF: case PP_ELIFDEFALIAS: case PP_ELIFDIFI:
        case PP_ELIFDIRECTIVE: case PP_ELIFEMPTY: case PP_ELIFENV: case PP_ELIFFILE: case PP_ELIFID: case PP_ELIFIDN:
        case PP_ELIFIDNI: case PP_ELIFMACRO: case PP_ELIFNUM: case PP_ELIFSTR: case PP_ELIFTOKEN: case PP_ELIFUSABLE:
        case PP_ELIFUSING:
        case PP_ELIFN: case PP_ELIFN_BOGUS: case PP_ELIFNCTX: case PP_ELIFNDEF: case PP_ELIFNDEFALIAS: case PP_ELIFNDIFI:
        case PP_ELIFNDIRECTIVE: case PP_ELIFNEMPTY: case PP_ELIFNENV: case PP_ELIFNFILE: case PP_ELIFNID: case PP_ELIFNIDN:
        case PP_ELIFNIDNI: case PP_ELIFNMACRO: case PP_ELIFNNUM: case PP_ELIFNSTR: case PP_ELIFNTOKEN: case PP_ELIFNUSABLE:
        case PP_ELIFNUSING:
            if (istk_->conds.empty()) {
                as_.nonfatal("`{}': no matching `%if'", dname);
                break;
            }
            switch (istk_->conds.back()) {
                case CondState::IF_TRUE: istk_->conds.back() = CondState::DONE; break;
                case CondState::DONE:
                case CondState::NEVER: break;
                case CondState::ELSE_TRUE:
                case CondState::ELSE_FALSE:
                    as_.warn(WARN_PP_ELSE_ELIF | ERR_PP_PRECOND, "`{}' after `%else', ignoring content", dname);
                    istk_->conds.back() = CondState::NEVER;
                    break;
                case CondState::IF_FALSE: {
                    const CondState s = if_condition(expand_mmac_params(tline->next), op, dname);
                    tline->next = nullptr;
                    istk_->conds.back() = s;
                    break;
                }
                default: break;
            }
            break;

        case PP_ELSE:
            if (tline->next) as_.warn(WARN_PP_TRAILING | ERR_PP_PRECOND, "trailing garbage after `{}' ignored", dname);
            if (istk_->conds.empty()) {
                as_.nonfatal("`{}': no matching `%if'", dname);
                break;
            }
            switch (istk_->conds.back()) {
                case CondState::IF_TRUE:
                case CondState::DONE: istk_->conds.back() = CondState::ELSE_FALSE; break;
                case CondState::NEVER: break;
                case CondState::IF_FALSE: istk_->conds.back() = CondState::ELSE_TRUE; break;
                case CondState::ELSE_TRUE:
                case CondState::ELSE_FALSE:
                    as_.warn(WARN_PP_ELSE_ELSE | ERR_PP_PRECOND, "`{}' after `%else', ignoring content", dname);
                    istk_->conds.back() = CondState::NEVER;
                    break;
                default: break;
            }
            break;

        case PP_ENDIF:
            if (tline->next) as_.warn(WARN_PP_TRAILING | ERR_PP_PRECOND, "trailing garbage after `{}' ignored", dname);
            if (istk_->conds.empty()) {
                as_.nonfatal("`{}': no matching `%if'", dname);
                break;
            }
            istk_->conds.pop_back();
            if (istk_->mstk.mstk) istk_->mstk.mstk->condcnt--;
            break;

        case PP_RMACRO:
            // %rmacro is reserved; it is treated as %macro
            op = PP_MACRO;
            as_.warn(WARN_PP_RESERVED, "reserved directive `{}', treating as '{}'", dname, pp_directive_name(op + !casesense));
            [[fallthrough]];
        case PP_MACRO: {
            MMacro* def;
            if (defining_) as_.assert_failed("!defining");
            def = new_mmacro();
            def->casesense = casesense;
            if (!parse_mmacro_spec(tline, def, dname)) {
                free_mmacro(def);
                goto done;
            }
            def->dstk.mmac = get_mmacro(def);
            defining_ = get_mmacro(def);
            defining_->where = istk_->where;

            // Warn if the macro is already defined with an overlapping parameter range
            {
                auto it = mmacros_.find(lcase(defining_->name));
                mmac = it == mmacros_.end() ? nullptr : it->second;
            }
            while (mmac) {
                if (mmac->name == defining_->name && (mmac->nparam_min <= defining_->nparam_max || defining_->plus) &&
                    (defining_->nparam_min <= mmac->nparam_max || mmac->plus)) {
                    as_.warn(WARN_PP_MACRO_REDEF_MULTI, "redefining multi-line macro `{}'", defining_->name);
                    break;
                }
                mmac = mmac->next;
            }
            break;
        }

        case PP_ENDM:
        case PP_ENDMACRO:
            if (!defining_) {
                as_.nonfatal("`{}': not defining a macro", dname);
                goto done;
            } else if (!defining_->has_name) {
                as_.nonfatal("expected `%endrep' before `{}'", dname);
                goto done;
            }
            pop_mstk(&defining_->dstk, defining_->dstk.mstk);
            if (defining_->refcnt != 1) as_.panic("defining->refcnt == {}", defining_->refcnt);
            mmhead = &mmacros_[lcase(defining_->name)];
            defining_->next = *mmhead;
            *mmhead = defining_;
            defining_ = nullptr;
            break;

        case PP_EXITMACRO:
            // Exit a macro: the remaining lines of its expansion are suppressed
            do_exit_macro(dname, true);
            break;

        case PP_UNIMACRO:
            casesense = false;
            [[fallthrough]];
        case PP_UNMACRO: {
            MMacro** mmac_p;
            MMacro spec;
            spec.casesense = casesense;
            if (!parse_mmacro_spec(tline, &spec, dname)) goto done;
            {
                auto it = mmacros_.find(lcase(spec.name));
                mmac_p = it == mmacros_.end() ? nullptr : &it->second;
            }
            if (!mmac_p) {
                // Nothing to do
                delete_tlist(spec.dlist);
                break;
            }
            while (mmac_p && *mmac_p) {
                mmac = *mmac_p;
                if (mmac->casesense == spec.casesense && streq(mmac->name, spec.name, spec.casesense) &&
                    mmac->nparam_min == spec.nparam_min && mmac->nparam_max == spec.nparam_max && mmac->plus == spec.plus) {
                    *mmac_p = mmac->next;   // unlink it
                } else {
                    mmac_p = &mmac->next;
                }
            }
            delete_tlist(spec.dlist);
            break;
        }

        case PP_ROTATE:
            while (tok_white(tline->next)) tline = tline->next;
            if (!tline->next) {
                delete_tlist(origline);
                as_.nonfatal("`{}' missing rotate count", dname);
                return DIRECTIVE_FOUND;
            }
            t = expand_smacro(tline->next);
            tline->next = nullptr;
            pps.tptr = tline = t;
            pps.ntokens = -1;
            evalresult = evaluate_tokens(pps, tokval);
            delete_tlist(tline);
            if (!evalresult) goto done;
            if (tokval.type != TokenType::EOS) as_.warn(WARN_PP_TRAILING, "trailing garbage after expression ignored");
            if (!pp_is_simple(*evalresult)) {
                as_.nonfatal("non-constant value given to `{}'", dname);
                goto done;
            }
            mmac = istk_->mstk.mmac;
            if (!mmac) {
                as_.nonfatal("`{}' invoked outside a macro call", dname);
            } else if (mmac->nparam == 0) {
                as_.nonfatal("`{}' invoked within macro without parameters", dname);
            } else {
                int rotate = static_cast<int>(mmac->rotate) + static_cast<int>(pp_reloc_value(*evalresult));
                rotate %= static_cast<int>(mmac->nparam);
                if (rotate < 0) rotate += static_cast<int>(mmac->nparam);
                mmac->rotate = static_cast<unsigned>(rotate);
            }
            break;

        case PP_REP: {
            MMacro* def;
            nolist = 0;
            tline = skip_white(tline->next);
            if (tok_is(tline, ID) && tline->text.size() == 7 && ieq(tline->text, ".nolist")) {
                if (!(host_.list_option && host_.list_option('f'))) nolist |= NL_LIST;
                tline = skip_white(tline->next);
            }
            if (tline) {
                pps.tptr = expand_smacro(tline);
                pps.ntokens = -1;
                evalresult = evaluate_tokens(pps, tokval);
                // tline is freed by the delete_tlist(origline) at done
                if (!evalresult) goto done;
                if (tokval.type != TokenType::EOS) as_.warn(WARN_PP_TRAILING, "trailing garbage after expression ignored");
                if (!pp_is_simple(*evalresult)) {
                    as_.nonfatal("non-constant value given to `{}'", dname);
                    goto done;
                }
                count = pp_reloc_value(*evalresult);
                if (count > as_.nasm_limit[LIMIT_REP]) {
                    as_.nonfatal("`{}' count {} exceeds limit (currently {})", dname, count, as_.nasm_limit[LIMIT_REP]);
                    count = 0;
                } else if (count < 0) {
                    as_.warn(ERR_PASS2 | WARN_PP_REP_NEGATIVE, "negative `{}' count: {}", dname, count);
                    count = 0;
                } else {
                    count++;
                }
            } else {
                as_.nonfatal("`{}' expects a repeat count", dname);
                count = 0;
            }
            def = new_mmacro();
            def->nolist = nolist;
            def->in_progress = static_cast<std::int32_t>(count);
            def->mstk.mstk = get_mmacro(istk_->mstk.mstk);
            def->mstk.mmac = get_mmacro(istk_->mstk.mmac);
            def->dstk.mstk = defining_;
            def->dstk.mmac = defining_ ? get_mmacro(defining_->dstk.mmac) : nullptr;
            def->where = istk_->where;
            defining_ = get_mmacro(def);
            break;
        }

        case PP_ENDREP: {
            Line* l;
            if (!defining_ || defining_->has_name) {
                as_.nonfatal("`%endrep': no matching `%rep'");
                goto done;
            }
            // The line that marks the end of the block
            l = new_line();
            l->next = istk_->expansion;
            l->finishes = get_mmacro(defining_);
            l->first = nullptr;
            l->where = as_.src.where();
            istk_->expansion = l;

            // We can't free the macro definition yet: the lines of the block run first
            istk_->nolist += !!(defining_->nolist & NL_LIST);
            if (!istk_->nolist && host_.list_uplevel) host_.list_uplevel(LIST_MACRO);

            put_mmacro(&defining_->dstk.mmac);
            pop_mmacro(&istk_->mstk.mstk, defining_);
            defining_ = defining_->dstk.mstk;
            break;
        }

        case PP_EXITREP: {
            // Exit a %rep: set the remaining count to 1 so it finishes after this pass
            MMacro* m = do_exit_macro(dname, false);
            if (m) m->in_progress = 1;
            break;
        }

        case PP_DEFINE:
        case PP_XDEFINE:
        case PP_DEFALIAS: {
            SMacro tmpl;
            Tok** lastp;
            int nparam;

            mname = get_id(&tline, dname, idok);
            if (!idok) goto done;

            lastp = &tline->next;
            nparam = parse_smacro_template(&lastp, &tmpl);
            tline = *lastp;
            *lastp = nullptr;

            if (op == PP_DEFALIAS) {
                // The expansion is the identifier of the macro this one is an alias for
                macro_start = tline;
                if (!tok_macro_id(macro_start)) {
                    as_.nonfatal("`{}' expects a macro identifier to alias", dname);
                    goto done;
                }
                tt = macro_start->next;
                macro_start->next = nullptr;
                tline = tline->next;
                tline = skip_white(tline);
                if (tline && tline->type) as_.warn(WARN_PP_TRAILING, "trailing garbage after aliasing identifier ignored");
                delete_tlist(tt);
                tmpl.alias = true;
            } else {
                if (op == PP_XDEFINE) {
                    // Expand the expansion now; the parameters stay as they are
                    if (nparam) mark_smac_params(tline, &tmpl, XDEF_PARAM);
                    tline = expand_smacro(tline);
                }
                macro_start = tline;
            }
            define_smacro(mname, casesense, macro_start, &tmpl);
            break;
        }

        case PP_UNDEF:
        case PP_UNDEFALIAS:
            mname = get_id(&tline, dname, idok);
            if (!idok) goto done;
            if (tline->next) as_.warn(WARN_PP_TRAILING, "trailing garbage after macro name ignored");
            undef_smacro(mname, op == PP_UNDEFALIAS);
            break;

        case PP_DEFSTR:
            mname = get_id(&tline, dname, idok);
            if (!idok) goto done;
            tline = expand_smacro(cut_tlist(tline));
            tline = zap_white(tline);
            {
                const std::string q = detoken(tline, false);
                macro_start = make_tok_qstr(nullptr, q);
            }
            delete_tlist(tline);
            define_smacro(mname, casesense, macro_start, nullptr);
            break;

        case PP_DEFTOK:
            mname = get_id(&tline, dname, idok);
            if (!idok) goto done;
            tline = expand_smacro(cut_tlist(tline));
            t = skip_white(tline);
            if (!tok_is(t, STR)) {
                as_.nonfatal("`{}' requires string as second parameter", dname);
                delete_tlist(tline);
                goto done;
            }
            macro_start = tokenize(unquote_token_cstr(t));
            define_smacro(mname, casesense, macro_start, nullptr);
            delete_tlist(tline);
            break;

        case PP_PATHSEARCH:
            mname = get_id(&tline, dname, idok);
            if (!idok) goto done;
            macro_start = pp_do_pathsearch(&tline->next, dname);
            if (macro_start) define_smacro(mname, casesense, macro_start, nullptr);
            break;

        case PP_STRLEN:
            mname = get_id(&tline, dname, idok);
            if (!idok) goto done;
            tline = expand_smacro(cut_tlist(tline));
            t = skip_white(tline);
            if (!tok_is(t, STR)) {
                as_.nonfatal("`{}' requires string as second parameter", dname);
                delete_tlist(tline);
                delete_tlist(origline);
                return DIRECTIVE_FOUND;
            }
            unquote_token(t);
            macro_start = make_tok_num(nullptr, static_cast<std::int64_t>(t->text.size()));
            define_smacro(mname, casesense, macro_start, nullptr);
            delete_tlist(tline);
            delete_tlist(origline);
            return DIRECTIVE_FOUND;

        case PP_STRCAT:
            mname = get_id(&tline, dname, idok);
            if (!idok) goto done;
            tline = expand_smacro(cut_tlist(tline));
            macro_start = pp_strcat(tline, dname);
            if (macro_start) define_smacro(mname, casesense, macro_start, nullptr);
            break;

        case PP_SUBSTR:
            mname = get_id(&tline, dname, idok);
            if (!idok) goto done;
            tline = expand_smacro(cut_tlist(tline));
            macro_start = pp_substr(tline, dname);
            if (macro_start) define_smacro(mname, casesense, macro_start, nullptr);
            break;

        case PP_ASSIGN:
            mname = get_id(&tline, dname, idok);
            if (!idok) goto done;
            tline = cut_tlist(tline);
            assign_smacro(mname, casesense, tline, dname);
            goto done;

        case PP_ALIASES:
            tline = tline->next;
            tline = expand_smacro(tline);
            ppconf_.noaliases = !pp_get_boolean_option(tline, !ppconf_.noaliases);
            break;

        case PP_LINE:
            as_.panic("`{}' directive not preprocessed early", dname);

        case PP_NULL:
            // Nothing: the line is gone
            break;
    }
    (void)j;

done:
    delete_tlist(origline);
    return DIRECTIVE_FOUND;
}

}  // namespace cb::build::x86asm
